#include "internal_node.hpp"
#include "node.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>



std::ostream& operator<<(std::ostream& os, const InternalNode::Value& other) {
    os << "(" << other.data.first << "," << other.data.second << ")";
    return os;
}

void InternalNode::print() {
    std::stringstream ss;
    ss << "Node(" << this << "): [ ";
    for(size_t i=0; i<data_.size(); i++) {
        auto [key, node] = data_.at(i).data;
        ss << " (k="<<key<<",v="<<node<<") ";
    }
    ss << " ] ";
    std::cout << ss.str() << endl;
}

const InternalNode::Value& InternalNode::at(size_t idx) const {
    if(idx >= this->size()) {
        throw runtime_error("index out of bounds");
    }
    return data_.at(idx);
}
 
size_t InternalNode::size() const {
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
    data_.eraseIndexRange(start, end);
    return {keyToPromote, splitNode};
}   


void InternalNode::insert(int key, Node* node) {
    if(isFull()) {
        throw runtime_error("node is full");
    }
    if(!(data_.find({key, nullptr}).has_value())) {
        throw runtime_error("key " + std::to_string(key) +  " already exists in the node");
    }
    data_.insert({key, node});
    return;
}

bool InternalNode::empty() {
    return data_.size() == 0;
}

InternalNode::Value InternalNode::traverseToNextLevel(int key) const {
    // TODO: binary search
    for(size_t i=1; i<data_.size(); i++) {
        auto kv = data_.at(i);
        if(key < kv.first()) {
            return data_.at(i-1);
        }
    }
    return data_.at(data_.size()-1);
}

bool InternalNode::eraseElement(const Value& elem) {
    return data_.eraseElement(elem);
} 