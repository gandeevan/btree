/*
    TODO(Bug Fixes):
    -

    TODO(Improvements):
    - use smart pointers
    - Use a sorted array instead of a map to store the data in the internal/leaf node. 
    - On inserts, try distributing the data with the siblings before attempting to split the node.
    - Merge nodes on deletes when they are less than half full.
    - Support range queries
    - Support duplicate keys.
*/


#include "btree.hpp"
#include "internal_node.hpp"
#include "node.hpp"
#include <iterator>
#include <stdexcept>
#include <queue>
#include <cassert>

#define TO_INTERNAL_NODE(x) reinterpret_cast<InternalNode *>(x)
#define TO_LEAF_NODE(x) reinterpret_cast<LeafNode *>(x)

BTree::BTree() {
    root_ = new LeafNode();
}

BTree::~BTree() {
    // TODO: delete the tree
}

LeafNode* BTree::traverseToLeafNode(Node* node, int key) const {
    if(node->type() == LEAF_NODE_TYPE) {
        return TO_LEAF_NODE(node);
    } 


    auto nextNode = TO_INTERNAL_NODE(node)->traverseToNextLevel(key).second();
    return traverseToLeafNode(nextNode, key);;
}

std::pair<int, Node*> BTree::insertOrUpdateImpl(Node* node, int key, int value) {
    if(node->type() == LEAF_NODE_TYPE) {
        auto leaf = TO_LEAF_NODE(node);
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
    auto nextNode = TO_INTERNAL_NODE(node)->traverseToNextLevel(key).second();
    auto [newKey, newNode] = insertOrUpdateImpl(nextNode, key, value); 

    // Was able to insert the node at the next level without any splits
    if(newNode == nullptr)
        return {-1, nullptr};

    // The next level was split because of the insert, we now need to add the pointer to the new node
    // in the current level. 
    auto internalNode = TO_INTERNAL_NODE(node);
    if(!internalNode->isFull()) {
        internalNode->data_.insert({newKey, newNode});
        return {-1, nullptr};
    } else {
        return internalNode->split(newKey, newNode);
    }
}

void BTree::insertOrUpdate(int key, int value) {
   auto [newKey, newNode] = insertOrUpdateImpl(root_, key, value);
   
   // the root has been split, we have a new root
   if(newNode != nullptr) {
        auto newRoot = new InternalNode(newKey, root_, newNode);
        root_ = newRoot;
   }

   return;
}

std::pair<bool, bool> BTree::removeImpl(Node* node, int key) {
    if(node->type() == LEAF_NODE_TYPE) {
        auto leafNode = TO_LEAF_NODE(node);
        bool ok = leafNode->remove(key);
        if(!ok || !leafNode->empty()) {
            return {ok, false};
        }

        // Delete the leaf node and inform the parent to remove the pointer to the leaf node. 
        delete leafNode;
        return {ok, true};
    }

    assert(node->type() == INTERNAL_NODE_TYPE);
    const InternalNode::Value& elem = TO_INTERNAL_NODE(node)->traverseToNextLevel(key);
    auto nextNode = elem.second();
    auto [found, removePtr] = removeImpl(nextNode, key);
    if(!found || !removePtr) {
        assert(removePtr == false);
        return {found, removePtr};
    }

    // remove the pointer to the node that was deleted
    found = TO_INTERNAL_NODE(node)->eraseElement(elem);
    assert(found);

    throw runtime_error("Not implemented");
}

bool BTree::remove(int key) {
    auto leafNode = traverseToLeafNode(root_, key);
    bool ok = leafNode->remove(key);
    if(!ok) {
        return false;
    }

    // The leaf node is not empty, we are done
    if(!leafNode->empty()) {
        return true;
    }

    return false;
}


void BTree::print() {
    int maxLevel = 0;
    std::map<int, vector<Node*>> levelMap; 

    if(root_ == nullptr) {
        std::cout << "Root equals null!";
        return;
    }

    std::queue<std::pair<int, Node *>> q;
    q.push({0, root_});
    while(!q.empty()) {
        auto [level, node] = q.front();
        q.pop();

        if(level > maxLevel) {
            maxLevel = level;
        }

        if(node->type() == INTERNAL_NODE_TYPE) {
            auto internalNode = TO_INTERNAL_NODE(node);
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

    // print the nodes at each level
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
    auto leafNode = traverseToLeafNode(root_, key);
    auto it = leafNode->data_.find(key);
    if(it != leafNode->data_.end()) {
        return it->second;
    } 
    return optional<int>();
}