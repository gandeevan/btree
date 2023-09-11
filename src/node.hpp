#pragma once

#include <stddef.h>


#define MAX_NODE_SIZE 4

// TODO: define a enum instead
#define UNDEFINED 0
#define INTERNAL_NODE_TYPE 1
#define LEAF_NODE_TYPE 2

class Node {
protected:
   int _nodeType;  
   size_t _capacity = MAX_NODE_SIZE;
public:
    friend class BTree;

    Node(int nodeType) : _nodeType(nodeType) {

    };

    virtual ~Node() {}

    virtual bool isFull() {
        return size() == capacity();
    }

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