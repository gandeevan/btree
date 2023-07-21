#pragma once

#include <map>
#include "node.hpp"
#include "sorted_array.hpp"
using namespace std;

class InternalNode : public Node {
public:
    friend class BTree;

    InternalNode() : Node(INTERNAL_NODE_TYPE), data(MAX_NODE_SIZE) {
    }

    unsigned size();
    bool isFull();
    void insert(int key, Node* node);

private:
    InternalNode* split();

    SortedArray<std::pair<int, Node*>> data;
};