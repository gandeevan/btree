#pragma once

#include <map>
#include "node.hpp"
#include "sorted_array.hpp"
using namespace std;

class InternalNode : public Node {
public:
    friend class BTree;

    InternalNode() : Node(INTERNAL_NODE_TYPE), data(MAX_NODE_SIZE+1) {

    }

    InternalNode(int key, Node* left, Node* right) : InternalNode() {
        data.insert({INT_MIN, left});
        data.insert({key, right});
    }

    unsigned size();
    bool isFull();
    void insert(int key, Node* node);

private:

    std::pair<int, InternalNode*> split(int key, InternalNode* node);
    SortedArray<std::pair<int, Node*>> data;
};