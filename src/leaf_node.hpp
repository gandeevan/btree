#pragma once
#include "node.hpp"
#include <map>
#include "sorted_array.hpp"

using namespace std;

class LeafNode : public Node {
public:
    friend class BTree;

    LeafNode() : Node(LEAF_NODE_TYPE) {

    }

    std::pair<int, int> at(int idx);
    unsigned size();
    bool update(int key, int value);
    void insert(int key, int value);
    void print();
private:
    std::pair<int, LeafNode*> split(int key, int value);
    map<int, int> data_;
};
