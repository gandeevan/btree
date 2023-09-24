#pragma once

#include <map>
#include <cassert>
#include <limits.h>
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

        std::pair<int, Node*> get() const {
            return data;
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

        bool operator!=(const Value& other) const {
            return !(this->operator==(other));
        }

        bool operator>(const Value& other) const {
            return !(this->operator<(other));
        }

        bool operator<(const Value& other) const {
            return this->data.first < other.data.first;   
        }

        friend std::ostream& operator<<(std::ostream& os, const Value& other);
    };

    InternalNode(size_t order, int key, Node* left, Node* right) : InternalNode(order) {
        data_.insert({INT_MIN, left});
        data_.insert({key, right});
    }

    static InternalNode* fromNode(Node *node) {
        if(node != nullptr) {
            assert(node->type() == INTERNAL_NODE_TYPE);
        }
        return reinterpret_cast<InternalNode *>(node);
    }

    const Value& at(size_t idx) const;
    const SortedArray<Value>& data();
    size_t size() const;
    void updateKey(int index, int newKey);
    void eraseIndex(size_t idx);
    void insert(int key, Node* node);
    void print();
    bool empty();
    bool canLendKeys() const;
    bool eraseElement(const Value& elem);
    std::pair<int, InternalNode*> split(int key, Node* node);
private:
    // The capacity of the sorted array is 1 + the capacity of the node so that
    // the data item that leads to a split can be inserted before the split. This simplifies the split logic.
    InternalNode(size_t order) : Node(INTERNAL_NODE_TYPE, INTERNAL_NODE_CAPACITY(order)), _order(order), data_(capacity()+1) {

    }


    void merge(InternalNode* other, int anchorKey);
    bool canMerge(Node* other);
    bool isHalfFull() const;
    int getLargestKey() const;
    BorrowResult tryBorrowFromSiblings(int index, Node* parent);
    Node* mergeWithSiblings(int index, Node* parent);

    size_t traverseToNextLevel(int key) const;
    size_t _order;
    SortedArray<Value> data_; // TODO: rename to _data
};