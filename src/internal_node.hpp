#pragma once

#include <map>
#include "node.hpp"
#include "sorted_array.hpp"
using namespace std;





class InternalNode : public Node {
public:
    friend class BTree;

    class Value {
    public:
        std::pair<int, Node*> data;

        Value() = default;

        Value(int value, Node* ptr) : data({value, ptr}) {

        }

        int first() const {
            return data.first;
        }

        Node* second() const {
            return data.second;
        }

        bool operator==(const Value& other) const {
            return this->data.first == other.data.first;
        }

        bool operator>(const Value& other) const {
            return !(this->operator<(other));
        }

        bool operator<(const Value& other) const {
            return this->data.first < other.data.first;   
        }
    };

    InternalNode(int key, Node* left, Node* right) : InternalNode() {
        data.insert({INT_MIN, left});
        data.insert({key, right});
    }

    unsigned size();
    void insert(int key, Node* node);
    std::pair<int, InternalNode*> split(int key, Node* node);

private:
    InternalNode() : Node(INTERNAL_NODE_TYPE), data(capacity()+1) {

    }

    SortedArray<Value> data;
};