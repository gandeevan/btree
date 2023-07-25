#pragma once

#define MAX_NODE_SIZE 4

#define UNDEFINED 0
#define INTERNAL_NODE_TYPE 1
#define LEAF_NODE_TYPE 2

class Node {
protected:
   int _nodeType;  
   int _capacity = MAX_NODE_SIZE;
public:
    friend class BTree;

    Node(int nodeType) : _nodeType(nodeType) {

    };

    virtual ~Node() {}

    virtual bool isFull() {
        return size() == capacity();
    }

    virtual unsigned size() = 0;

    virtual int capacity() {
        return _capacity;
    }

    int nodeType() {
        return _nodeType;
    }
};