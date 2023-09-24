#include "internal_node.hpp"
#include "node.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "constants.hpp"
#include "sorted_array.hpp"
#include "logger/logger.hpp"

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
        THROW_EXCEPTION("index out of bounds");
    }
    return data_.at(idx);
}

/// @brief Returns the number of pointers in the internal node
size_t InternalNode::size() const {
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
    InternalNode* splitNode = new InternalNode(_order);
    int start = data_.size()/2, end = data_.size();
    for(int i=start; i<end; i++) {
        int key = i == (start) ? INT_MIN : this->data_.at(i).first();
        splitNode->data_.insert({key, this->data_.at(i).second()});
    }
    int keyToPromote = data_.at(start).first();
    data_.eraseIndexRange(start, end);
    return {keyToPromote, splitNode};
}   


void InternalNode::insert(int key, Node* node) {
    if(isFull()) {
        THROW_EXCEPTION("node is full");
    }
    if(data_.find({key, nullptr}).has_value()) {
        THROW_EXCEPTION("key " + std::to_string(key) +  " already exists in the node");
    }
    data_.insert({key, node});
    return;
}

bool InternalNode::empty() {
    return data_.size() == 0;
}

size_t InternalNode::traverseToNextLevel(int key) const {
    // TODO: binary search
    for(size_t i=1; i<data_.size(); i++) {
        auto kv = data_.at(i);
        if(key < kv.first()) {
            return i-1;
        }
    }
    return data_.size()-1;
}

bool InternalNode::eraseElement(const Value& elem) {
    return data_.eraseElement(elem);
} 

bool InternalNode::isHalfFull() const {
    return data_.size() >= (_order+1);
}

bool InternalNode::canLendKeys() const {
    return data_.size() > (_order+1);
}

void InternalNode::updateKey(int index, int newKey) {
    Value newVal = Value(newKey, data_.at(index).second());
    data_.replace(index, newVal);
}

void InternalNode::eraseIndex(size_t idx) {
    data_.eraseIndex(idx);
}

void InternalNode::merge(InternalNode* other, int anchorKey) {
    assert(canMerge(other));
    data_.insert({anchorKey, other->data_.at(0).second()});
    for(size_t i=1; i<other->size(); i++) {
        data_.insert(other->data_.at(i));
    }
}

bool InternalNode::canMerge(Node* other) {
    return (size() + other->size()) <= ((2*_order)+1); 
}

int InternalNode::getLargestKey() const {
    if(data_.size() == 0) {
        THROW_EXCEPTION("node is empty");
    }   
    return (*(data_.rbegin())).first();
}

BorrowResult InternalNode::tryBorrowFromSiblings(int index, Node* parent) {
    InternalNode *parentNode = InternalNode::fromNode(parent);
    InternalNode *leftSibling = InternalNode::fromNode(index == 0 ? nullptr :  parentNode->at(index-1).second());
    InternalNode *rightSibling = InternalNode::fromNode(index == (parentNode->size()-1) ? nullptr : parentNode->at(index+1).second());

    if(leftSibling && leftSibling->canLendKeys()) {
        
        int anchorKey = parentNode->at(index).first();
        // the anchor key of the current node is moved down
        data_.replace(0, {anchorKey, data_.at(0).second()});
        
        // redistribute keys from the left sibling
        while(leftSibling->size() > size()) {
            assert(leftSibling->canLendKeys());
            auto it = leftSibling->data_.rbegin();
            data_.insert(*it);
            leftSibling->data_.eraseIndex(leftSibling->size()-1);
        }

        // update the anchor key in the parent
        parentNode->updateKey(index, data_[0].first());
        return BorrowResult::LEFT;

    } else if(rightSibling && rightSibling->canLendKeys()) {

        int anchorKey = parentNode->at(index+1).first();
        data_.insert({anchorKey, rightSibling->data_.at(0).second()});
        rightSibling->data_.eraseIndex(0);

        while(rightSibling->size() > size()) {
            assert(rightSibling->canLendKeys());
            data_.insert(rightSibling->data_.at(0));
            rightSibling->data_.eraseIndex(0);
        }

        // update the anchor key in the parent
        parentNode->updateKey(index+1, rightSibling->data_.at(0).first());
        return BorrowResult::RIGHT;

    } else {
        return BorrowResult::NONE;
    }
     return BorrowResult::NONE;
}

Node* InternalNode::mergeWithSiblings(int index, Node* parent) {
    InternalNode *parentNode = InternalNode::fromNode(parent);
    InternalNode *leftSibling = InternalNode::fromNode(index == 0 ? nullptr :  parentNode->at(index-1).second());
    InternalNode *rightSibling = InternalNode::fromNode(index == (parentNode->size()-1) ? nullptr : parentNode->at(index+1).second());

    InternalNode *nodeToReceive = nullptr;
    InternalNode *nodeToMerge = nullptr;
    int idxToErase = -1;
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

    // the anchor key of the current node is moved down
    int anchorKey = parentNode->at(idxToErase).first();
    nodeToReceive->merge(nodeToMerge, anchorKey);
    parentNode->eraseIndex(idxToErase);
    return nodeToMerge;
}