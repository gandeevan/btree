#pragma once
#include "node.hpp"
#include <map>
#include "sorted_array.hpp"

using namespace std;

class LeafNode : public Node {
public:
    friend class BTree;

    LeafNode(size_t capacity) : Node(LEAF_NODE_TYPE, capacity) {

    }

    std::pair<int, int> at(int idx);
    size_t size() const;
    bool remove(int key);
    bool update(int key, int value);
    void insert(int key, int value);
    void print();
    bool empty();
private:
    std::pair<int, LeafNode*> split(int key, int value);
    map<int, int> data_;
};
