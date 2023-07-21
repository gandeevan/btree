#pragma once

#define MAX_NODE_SIZE 4

#define UNDEFINED 0
#define INTERNAL_NODE_TYPE 1
#define LEAF_NODE_TYPE 2

class Node {
protected:
   int nodeType = UNDEFINED;  
   int maxSize = MAX_NODE_SIZE;
public:
    friend class BTree;

    Node(int _nodeType) : nodeType(_nodeType) {

    };

    virtual unsigned size() = 0;

    int getNodeType() {
        return nodeType;
    }
};