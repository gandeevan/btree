#include "internal_node.hpp"
#include "node.hpp"
#include <iostream>
#include <stdexcept>



std::ostream& operator<<(std::ostream& os, const InternalNode::Value& other) {
    os << "(" << other.data.first << "," << other.data.second << ")";
    return os;
}


unsigned InternalNode::size() {
    // Returns the number of pointers in the internal node
    return data_.size();
}

const SortedArray<InternalNode::Value>& InternalNode::data() {
    return data_;
}

std::pair<int, InternalNode*> InternalNode::split(int newKey, Node* newNode) {
    data_.insert({newKey, newNode});
    
    // Create a new split node and copy half of the elements from the current node
    // to the split node. The middle key is promoted to the next level to point to 
    // the split node. 
    InternalNode* splitNode = new InternalNode();
    int start = data_.size()/2, end = data_.size();
    for(int i=start; i<end; i++) {
        std::cout << "copying element at index " << i << endl; 
        int key = i == (start) ? INT_MIN : this->data_.at(i).first();
        splitNode->data_.insert({key, this->data_.at(i).second()});
    }
    std::cout << "done copying!" << endl;
    int keyToPromote = data_.at(start).first();
    data_.remove(start, end);
    return {keyToPromote, splitNode};
}   


void InternalNode::insert(int key, Node* node) {
    if(isFull()) {
        throw runtime_error("node is full");
    }
    if(data_.find({key, nullptr}) != -1) {
        throw runtime_error("key " + std::to_string(key) +  " already exists in the node");
    }
    data_.insert({key, node});
    return;
}