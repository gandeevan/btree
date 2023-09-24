/*
    TODO:
    - use smart pointers
    - Use a sorted array instead of a map to store the data in the internal/leaf node. 
    - On inserts, try distributing the data with the siblings before attempting to split the node.
    - Merge nodes on deletes when they are less than half full.
    - Support range queries
    - Support duplicate keys.
*/


#include "btree.hpp"
#include "internal_node.hpp"
#include "constants.hpp"
#include "leaf_node.hpp"
#include "node.hpp"
#include <iterator>
#include <stdexcept>
#include <queue>
#include <cassert>


BTree::BTree(size_t order) : _order(order) {
    _root = new LeafNode(_order);
}

BTree::~BTree() {
    map<int, vector<Node*>> levelMap = levelOrderTraversal();
    for(auto it = levelMap.begin(); it != levelMap.end(); it++) {
        auto& nodes = it->second;
        for(auto &node : nodes) {
            delete node;
        }
    }
}

LeafNode* BTree::traverseToLeafNode(Node* node, int key) const {
    if(node->type() == LEAF_NODE_TYPE) {
        return LeafNode::fromNode(node);
    } 

    auto internalNode = InternalNode::fromNode(node);
    auto nextNode = internalNode->at(internalNode->traverseToNextLevel(key)).second();
    return traverseToLeafNode(nextNode, key);;
}

std::pair<int, Node*> BTree::insertOrUpdateImpl(Node* node, int key, int value) {
    if(node->type() == LEAF_NODE_TYPE) {
        auto leaf = LeafNode::fromNode(node);
        if(leaf->update(key, value)) {
            return {-1, nullptr};
        }
        if(!leaf->isFull()) {
            leaf->insert(key, value);
            return {-1, nullptr};
        } else {
            return leaf->split(key, value);
        }
    }

    // The current node is an internal node
    auto internalNode = InternalNode::fromNode(node);
    auto nextNode = internalNode->at(internalNode->traverseToNextLevel(key)).second();
    auto [newKey, newNode] = insertOrUpdateImpl(nextNode, key, value); 

    // Was able to insert the node at the next level without any splits
    if(newNode == nullptr)
        return {-1, nullptr};

    // The next level was split because of the insert, we now need to add the pointer to the new node
    // in the current level. 
    if(!internalNode->isFull()) {
        internalNode->data_.insert({newKey, newNode});
        return {-1, nullptr};
    } else {
        return internalNode->split(newKey, newNode);
    }
}

void BTree::insertOrUpdate(int key, int value) {
    DEBUG_EXEC(defer {
        checkFillFactorInvariant();
    });

    auto [newKey, newNode] = insertOrUpdateImpl(_root, key, value);

    // the root has been split, we have a new root
    if(newNode != nullptr) {
        auto newRoot = new InternalNode(_order, newKey, _root, newNode);
        _root = newRoot;
    }
    return;
}

std::pair<bool, bool> BTree::removeImpl(NodeContext nodeCtx, int key) {
    if(nodeCtx.node->type() == LEAF_NODE_TYPE) {
        auto leafNode = LeafNode::fromNode(nodeCtx.node);
        bool found = leafNode->remove(key);

        // If the key was not found in the leaf node, then we are done
        if(!found) {
            return {found, false};
        }

        // If the leaf node is at least half full, then we are done
        // No changes to the tree are required
        if(leafNode->isHalfFull()) {
            return {found, false};
        }

        // If the leaf node is less than half full, but the root node is a leaf node, 
        // then we are done.
        if(nodeCtx.parent == nullptr) {
            return {found, false};
        }

        // Try to borrow from the siblings and propagate the changes to the parent
        BorrowResult result = leafNode->tryBorrowFromSiblings(nodeCtx.idx, nodeCtx.parent);
        if(result != BorrowResult::NONE) {
            return {found, false};
        }

        // Couldn't borrow from the siblings, so the leaf node now needs to be merged with either of the two siblings 
        // to ensure the occupancy invariant is maintained.
        LeafNode* nodeToDelete = leafNode->mergeWithSiblings(nodeCtx.idx, nodeCtx.parent);
        assert(nodeToDelete != nullptr);
        delete nodeToDelete;

        return {found, true};
    }

    assert(nodeCtx.node->type() == INTERNAL_NODE_TYPE);
    auto internalNode = InternalNode::fromNode(nodeCtx.node);
    auto idx = internalNode->traverseToNextLevel(key);
    auto [found, didMerge] = removeImpl(NodeContext{
        .node = internalNode->at(idx).second(),
        .parent = internalNode,
        .
        idx = idx,
    }, key);

    if(!found) {
        return {found, didMerge};
    }

    if(!didMerge) {
        return {found, didMerge};
    } 
    assert(found == true);
    assert(didMerge == true);

    // If the node is the root node, then we are done, root can't borrow or merge with siblings
    if(nodeCtx.parent == nullptr) {
        return {found, didMerge};
    }
    
    // If the node is at least half full after the merge, then we are done
    if(didMerge && internalNode->isHalfFull()) {
        return {found, false};
    }

    if(internalNode->tryBorrowFromSiblings(nodeCtx.idx, nodeCtx.parent) != BorrowResult::NONE) {
        return {found, false};
    }

    Node* nodeToDelete = internalNode->mergeWithSiblings(nodeCtx.idx, nodeCtx.parent);
    assert(nodeToDelete != nullptr);
    delete nodeToDelete;

    return {found, true};
}

bool BTree::remove(int key) {
    DEBUG_EXEC(defer {
        checkFillFactorInvariant();
    });

    auto [found, didMerge] = removeImpl(
        NodeContext{
            .node = _root,
            .parent =  nullptr, 
            .idx = 0,
        }, 
        key
    );

    if(!found) {
        return false;
    }

    // If the root node is an internal node and 
    // it has only one child, then we need to replace the root node.
    if(didMerge) {
        auto internalNode = InternalNode::fromNode(_root);
        if(internalNode->size() == 1) {
            _root = internalNode->at(0).second();
        }
    } 

    return true;
}


std::map<int, vector<Node*>> BTree::levelOrderTraversal() const {
    std::map<int, vector<Node*>> levelMap; 

    if(_root == nullptr) {
        return levelMap;
    }

    std::queue<std::pair<int, Node *>> q;
    q.push({0, _root});
    while(!q.empty()) {
        auto [level, node] = q.front();
        q.pop();

        if(node->type() == INTERNAL_NODE_TYPE) {
            auto internalNode = InternalNode::fromNode(node);
            for(size_t i=0; i<internalNode->size(); i++) {
                const InternalNode::Value& elem = internalNode->at(i);
                q.push({level+1, elem.second()});
            }
        }

        if(levelMap.find(level) == levelMap.end()) {
            levelMap[level] = {node};
        } else {
            levelMap[level].push_back(node);
        }
    }
    return levelMap;
}

void BTree::print() {
    map<int, vector<Node*>> levelMap = levelOrderTraversal();
    for(auto it = levelMap.begin(); it != levelMap.end(); it++) {
        auto level = it->first;
        auto& nodes = it->second;
        std::cout<<"Nodes at level " << level << ":" << endl;
        for(auto &node : nodes) {
            node->print();
        }
    }
}

std::optional<int> BTree::get(int key) const {
    auto leafNode = traverseToLeafNode(_root, key);
    auto it = leafNode->data_.find(key);
    if(it != leafNode->data_.end()) {
        return it->second;
    } 
    return optional<int>();
}

bool BTree::checkFillFactorInvariant() const {
    if(_root == nullptr) {
        return true;
    }

    map<int, vector<Node*>> levelMap = levelOrderTraversal();
    for(auto it = levelMap.begin(); it != levelMap.end(); it++) {
        auto level = it->first;
        auto& nodes = it->second;
        if(level == 0) {
            ASSERT(nodes.size() == 1, "Root node is not the only node at level 0");
            if(nodes[0]->type() == INTERNAL_NODE_TYPE) {
                if(nodes[0]->size() < 2) {
                    LOG_ERROR("Root node is an internal node, but has less than 2 children");
                    return false;
                }
            }
        } else {
            for(auto &node : nodes) {
                if(!node->isHalfFull()) {
                    LOG_ERROR(fmt::format("Node {} at level {} is not at least half full", (void *)node, level));
                    return false;
                }
            }
        }
        
    }
    return true;
}