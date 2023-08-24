
#include "leaf_node.hpp"
#include <sstream>
#include <stdexcept>

// TODO: 
// 1. Use a sorted array instead of a map to store the key value pairs
// 2. Implement the accessor operator ([]) for both internal node and leaf node

unsigned LeafNode::size() {
    return data_.size();
}

void LeafNode::print() {
    std::stringstream ss;
    ss << "Node(" << this << "): [ ";
    for(auto it = data_.begin(); it != data_.end(); it++) {
        ss << " (k="<<it->first<<",v="<<it->second<<") ";
    }
    ss << " ] ";
    std::cout << ss.str() << endl;
}

std::pair<int, int> LeafNode::at(int idx) {
    // TODO: this is inefficient since std::advance is O(N)
    // Accessing the item at idx i would be O(1) with sorted arrays 
    auto it = data_.begin();
    std::advance(it, idx);
    if(it == data_.end()) {
        throw runtime_error("index out of bounds");
    } else {
        return *it;
    }
}

bool LeafNode::update(int key, int value) {
    if(data_.find(key) != data_.end()) {
        data_[key] = value;   
        return true;     
    }
    return false;
}

void LeafNode::insert(int key, int value) {
    if(isFull()) {
        throw runtime_error("node is full");
    }

    data_.insert({key, value});
}

std::pair<int, LeafNode*> LeafNode::split(int newKey, int newValue) {
    LeafNode* nodeToInsert = this;
    auto splitNode = new LeafNode();
    auto it = data_.begin();
    std::advance(it, data_.size()/2);

    // makes sure the two nodes are equal in size after the split
    if(newKey > it->first) {
        nodeToInsert = splitNode;
        if(data_.size()%2 != 0) {
            it++;
        }  
    } 
    
    while(it != data_.end()) {
        splitNode->insert(it->first, it->second);
        it = data_.erase(it);
    }

    nodeToInsert->insert(newKey, newValue);
    return {splitNode->data_.begin()->first, splitNode};
}
