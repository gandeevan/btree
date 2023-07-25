#include "internal_node.hpp"
#include "node.hpp"
#include <iostream>
#include <stdexcept>


unsigned InternalNode::size() {
    return data.size()-1;
}

std::pair<int, InternalNode*> InternalNode::split(int newKey, Node* newNode) {
    data.insert({newKey, newNode});
    
    // Create a new splt node and copy half of the elements from the current node
    // to the split node. The middle key is promoted to the next level to point to 
    // the split node. 
    InternalNode* splitNode = new InternalNode();
    int start = data.size()/2, end = data.size();
    for(int i=start; i<end; i++) {
        int val = i == (start) ? INT_MIN : this->data.at(i).first();
        splitNode->data.insert({val, this->data.at(i).second()});
    }
    int keyToPromote = data.at(data.size()/2).first();
    data.remove(data.size()/2, data.size());
    return {keyToPromote, splitNode};
}   


void InternalNode::insert(int key, Node* node) {
    if(isFull()) {
        throw runtime_error("node is full");
    }
    if(data.get({key, nullptr}) != -1) {
        throw runtime_error("key " + std::to_string(key) +  " already exists in the node");
    }
    data.insert({key, node});
    return;
}