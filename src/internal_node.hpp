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

        friend std::ostream& operator<<(std::ostream& os, const Value& other);
    };

    InternalNode(size_t capacity, int key, Node* left, Node* right) : InternalNode(capacity) {
        data_.insert({INT_MIN, left});
        data_.insert({key, right});
    }

    const Value& at(size_t idx) const;
    const SortedArray<Value>& data();
    size_t size() const;
    bool eraseElement(const Value& elem);
    void insert(int key, Node* node);
    std::pair<int, InternalNode*> split(int key, Node* node);
    void print();
    bool empty();
private:
    // The capacity of the sorted array is 1 + the capacity of the node so that
    // the data item that leads to a split can be inserted before the split. 
    InternalNode(size_t capacity) : Node(INTERNAL_NODE_TYPE, capacity), data_(capacity+1) {

    }

    Value traverseToNextLevel(int key) const;

    SortedArray<Value> data_;
};