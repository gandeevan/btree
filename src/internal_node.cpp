#include "internal_node.hpp"

unsigned InternalNode::size() {
    return data.size();
}



bool InternalNode::isFull(){
    throw std::runtime_error("Function not implemented");
}



void InternalNode::insert(int key, Node* node) {
    throw std::runtime_error("Function not implemented");
}