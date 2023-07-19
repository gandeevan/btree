/*
    TODO(Optimizations):
    - Use an array instead of a map to store the data in the internal/leaf node. 

    TODO(Bug Fixes):
    -

    TODO(Improvements):
    - Support duplicate keys
*/


#include "btree.hpp"
#include <iterator>


BTree::BTree() {
    root = new LeafNode();
}

BTree::~BTree() {
    // TODO: delete the tree
}

Node* BTree::traverseToNextLevel(InternalNode* node, int key) {
    auto sv = node->data;
    for(int i=1; i<sv.size(); i++) {
        auto kv = sv.at(i);
        if(key < kv.first) {
            auto prev_kv = sv.at(i-1);
            return prev_kv.second;
        }
    }
    return sv.at(sv.size()-1).second;
}

LeafNode* BTree::traverseToLeafNode(Node* node, int key) {
    if(node->getNodeType() == LEAF_NODE_TYPE) {
        return reinterpret_cast<LeafNode *>(node);
    } 

    auto nextNode = traverseToNextLevel(reinterpret_cast<InternalNode *>(node), key);
    return traverseToLeafNode(nextNode, key);;
}

std::pair<int, Node*> BTree::insertOrUpdateImpl(Node* node, int key, int value) {
    if(node->getNodeType() == LEAF_NODE_TYPE) {
        auto leaf = reinterpret_cast<LeafNode *>(node);
        if(leaf->update(key, value)) {
            return {-1, nullptr};
        }
        if(leaf->size() < leaf->maxSize) {
            leaf->insert(key, value);
            return {-1, nullptr};
        }

        // leaf node needs to be split
        LeafNode *splitNode = leaf->splitNode(key, value);
        return {splitNode->data.begin()->first, splitNode};
    }

    auto nextNode = traverseToNextLevel(reinterpret_cast<InternalNode *>(node), key);
    auto [newKey, newNode] = insertOrUpdateImpl(nextNode, key, value); 

    if(newNode == nullptr)
        return {-1, nullptr};

    auto internalNode = reinterpret_cast<InternalNode *>(node);
    if(!internalNode->isFull()) {
        auto newEntry = std::pair<int, Node*>{newKey, newNode};
        internalNode->data.insert(newEntry);
        return {-1, nullptr};
    } 
    
    // TODO: split the internal node and propogate a key to the upper level

    throw std::runtime_error("Function not implemented");
}

void BTree::insertOrUpdate(int key, int value) {
   auto [newKey, newNode] = insertOrUpdateImpl(root, key, value);
   
   // the root has been split, we have a new root
   if(newNode != nullptr) {
        // auto newRoot = new InternalNode();
   }

   throw std::runtime_error("Function not implemented");
}

bool BTree::remove(int key) {
    return false;
}


std::optional<int> BTree::get(int key) {
    auto leafNode = traverseToLeafNode(root, key);
    auto it = leafNode->data.find(key);
    if(it != leafNode->data.end()) {
        return it->second;
    } 
    return optional<int>();
}

