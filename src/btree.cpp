/*
    TODO(Optimizations):
    - Use an array instead of a map to store the data in the internal/leaf node. 

    TODO(Bug Fixes):
    -

    TODO(Improvements):
    - Support duplicate keys
*/


#include "btree.hpp"
#include "internal_node.hpp"
#include <iterator>
#include <stdexcept>


BTree::BTree() {
    root_ = new LeafNode();
}

BTree::~BTree() {
    // TODO: delete the tree
}

Node* BTree::traverseToNextLevel(InternalNode* node, int key) const {
    auto& sv = node->data();
    for(int i=1; i<sv.size(); i++) {
        auto kv = sv.at(i);
        if(key < kv.first()) {
            auto prev_kv = sv.at(i-1);
            return prev_kv.second();
        }
    }
    return sv.at(sv.size()-1).second();
}

LeafNode* BTree::traverseToLeafNode(Node* node, int key) const {
    if(node->nodeType() == LEAF_NODE_TYPE) {
        return reinterpret_cast<LeafNode *>(node);
    } 

    auto nextNode = traverseToNextLevel(reinterpret_cast<InternalNode *>(node), key);
    return traverseToLeafNode(nextNode, key);;
}

std::pair<int, Node*> BTree::insertOrUpdateImpl(Node* node, int key, int value) {
    if(node->nodeType() == LEAF_NODE_TYPE) {
        auto leaf = reinterpret_cast<LeafNode *>(node);
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
    auto nextNode = traverseToNextLevel(reinterpret_cast<InternalNode *>(node), key);
    auto [newKey, newNode] = insertOrUpdateImpl(nextNode, key, value); 

    // Was able to insert the node at the next level without any splits
    if(newNode == nullptr)
        return {-1, nullptr};

    // The next level was split because of the insert, we now need to add the pointer to the new node
    // in the current level. 
    auto internalNode = reinterpret_cast<InternalNode *>(node);
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

bool BTree::remove(int key) {
    throw std::runtime_error("Not implemented");
}


std::optional<int> BTree::get(int key) const {
    auto leafNode = traverseToLeafNode(root_, key);
    auto it = leafNode->data.find(key);
    if(it != leafNode->data.end()) {
        return it->second;
    } 
    return optional<int>();
}

