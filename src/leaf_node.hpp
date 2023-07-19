#pragma once
#include "node.hpp"
#include <map>

using namespace std;

class LeafNode : public Node {
public:
    friend class BTree;

    LeafNode() : Node(LEAF_NODE_TYPE) {

    }

    unsigned size();
    bool update(int key, int value);
    void insert(int key, int value);
private:
    LeafNode* splitNode(int key, int value);
    map<int, int> data;
};
