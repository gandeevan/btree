
#include "leaf_node.hpp"
#include <sstream>
#include <stdexcept>
#include <cassert>
#include "constants.hpp"
#include "internal_node.hpp"

// TODO: 
// 1. Use a sorted array instead of a map to store the key value pairs
// 2. Implement the accessor operator ([]) for both internal node and leaf node



/// @brief Returns the number of key value pairs in the node
size_t LeafNode::size() const {
    return data_.size();
}

bool LeafNode::empty() {
    return data_.size() == 0;
}

bool LeafNode::remove(int key) {
    if(data_.find(key) != data_.end()) {
        data_.erase(key);
        return true;
    }
    return false;
}

bool LeafNode::isHalfFull() const {
    return data_.size() >= _order;
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
        PANIC("index out of bounds");
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
        PANIC("node is full");
    }

    data_.insert({key, value});
}

std::pair<int, LeafNode*> LeafNode::split(int newKey, int newValue) {
    assert(isFull());

    LeafNode* nodeToInsert = this;
    auto splitNode = new LeafNode(_order);
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

bool LeafNode::canLendKeys() const {
    return size() > _order;
}

void LeafNode::redistributeToRight(Node* other) {
    auto rightNode = LeafNode::fromNode(other);
    assert(rightNode->size() < size());
    while(rightNode->size() < size()) {
        assert(canLendKeys());
        auto it = data_.rbegin();
        rightNode->insert(it->first, it->second);
        data_.erase(it->first);
    }
}

void LeafNode::redistributeToLeft(Node* other) {
    auto leftNode = LeafNode::fromNode(other);
    assert(leftNode->size() < size());
    while(leftNode->size() < size()) {
        assert(canLendKeys());
        auto it = data_.begin();
        leftNode->insert(it->first, it->second);
        data_.erase(it->first);
    }
}

void LeafNode::merge(Node* other) {
    auto otherLeaf = LeafNode::fromNode(other);
    for(auto it = otherLeaf->data_.begin(); it != otherLeaf->data_.end(); it++) {
        data_.insert({it->first, it->second});
    }
    otherLeaf->data_.clear();
}

bool LeafNode::canMerge(Node* other) {
    return (size() + other->size()) <=  2*_order;
}

int LeafNode::getLargestKey() const {
    if (data_.size() == 0) {
        PANIC("node is empty");
    }
    return data_.rbegin()->first;
}

// TODO: maybe this does not belong to this class
// TODO: update type(index) to size_t
BorrowResult LeafNode::tryBorrowFromSiblings(int index, Node* parent) {
    InternalNode *parentNode = InternalNode::fromNode(parent);
    LeafNode *leftSibling = LeafNode::fromNode((size_t)index == 0 ? nullptr :  parentNode->at(index-1).second());
    LeafNode *rightSibling = LeafNode::fromNode((size_t)index == (parentNode->size()-1) ? nullptr : parentNode->at(index+1).second());
        
    if(leftSibling != nullptr) {
        if(leftSibling->canLendKeys()) {
            leftSibling->redistributeToRight(this);
            parentNode->updateKey(index, data_.begin()->first);
            return BorrowResult::LEFT;
        }
    }

    if(rightSibling != nullptr) {
        if(rightSibling->canLendKeys()) {
            rightSibling->redistributeToLeft(this);
            parentNode->updateKey(index+1, rightSibling->data_.begin()->first);
            return BorrowResult::RIGHT;
        }
    }

    return BorrowResult::NONE;
}

LeafNode* LeafNode::mergeWithSiblings(int index, Node* parent) {
    InternalNode *parentNode = InternalNode::fromNode(parent);
    LeafNode *leftSibling = LeafNode::fromNode((size_t)index == 0 ? nullptr :  parentNode->at(index-1).second());
    LeafNode *rightSibling = LeafNode::fromNode((size_t)index == (parentNode->size()-1) ? nullptr : parentNode->at(index+1).second());

    int idxToErase = -1;
    LeafNode *nodeToReceive = nullptr;
    LeafNode *nodeToMerge = nullptr;
    if(leftSibling != nullptr) {
        assert(leftSibling->canMerge(this));
        nodeToReceive = leftSibling;
        nodeToMerge = this;
        idxToErase = index;
    } else if(rightSibling != nullptr) {
        assert(rightSibling->canMerge(this));
        nodeToReceive = this;
        nodeToMerge = rightSibling;
        idxToErase = index+1;
    } else {
        assert(false);
    }

    nodeToReceive->merge(nodeToMerge);
    parentNode->eraseIndex(idxToErase);
    return nodeToMerge;
}
