#pragma once

#include "constants.hpp"
#include <stddef.h>


// TODO: define a enum instead
#define UNDEFINED 0
#define INTERNAL_NODE_TYPE 1
#define LEAF_NODE_TYPE 2

class Node {
protected:
    int _nodeType;  
    size_t _capacity;

    virtual bool isHalfFull() const = 0;
    virtual int getLargestKey() const = 0;
    virtual bool canMerge(Node* other) = 0;
    virtual BorrowResult tryBorrowFromSiblings(int index, Node* parentNode) = 0;
    virtual Node* mergeWithSiblings(int index, Node* parentNode) = 0;

public:
    friend class BTree;

    Node(int nodeType, size_t capacity) : _nodeType(nodeType), _capacity(capacity) {

    };

    virtual ~Node() {}

    virtual bool isFull() {
        return size() == capacity();
    }

    virtual bool canLendKeys() const = 0;

    virtual void print() = 0;

    virtual bool empty() = 0;

    virtual size_t size() const = 0;

    virtual size_t capacity() const {
        return _capacity;
    }

    int type() {
        return _nodeType;
    }
};