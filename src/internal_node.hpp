#pragma once

#include <map>
#include "node.hpp"
#include "sorted_vector.hpp"

using namespace std;

class InternalNode : public Node {
public:
    friend class BTree;

    InternalNode() : Node(INTERNAL_NODE_TYPE) {

    }

    unsigned size();
    bool isFull();
    void insert(int key, Node* node);

private:
    InternalNode* split();

    SortedVector<std::pair<int, Node*>> data;
};