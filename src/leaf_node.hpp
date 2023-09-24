#pragma once
#include "node.hpp"
#include "internal_node.hpp"
#include "sorted_array.hpp"
#include <cassert>
#include <map>
#include "logger/logger.hpp"


using namespace std;


class LeafNode : public Node {
public:
    friend class BTree;
    LeafNode(size_t order) : Node(LEAF_NODE_TYPE, LEAF_NODE_CAPACITY(order)), _order(order) {

    }

    static LeafNode* fromNode(Node *node) {
        if(node != nullptr) {
            assert(node->type() == LEAF_NODE_TYPE);
        }
        return reinterpret_cast<LeafNode *>(node);
    }

    std::pair<int, int> at(int idx);
    size_t size() const;
    void insert(int key, int value);
    void print();
    bool empty();
    bool canLendKeys() const;
    bool remove(int key);
    bool update(int key, int value);
private:
    void redistributeToRight(Node* other);
    void redistributeToLeft(Node *other);
    void merge(Node* other);
    bool canMerge(Node* other);
    bool isHalfFull() const;
    BorrowResult tryBorrowFromSiblings(int index, Node* parentNode);
    LeafNode* mergeWithSiblings(int index, Node* parentNode);
    int getLargestKey() const;
    std::pair<int, LeafNode*> split(int key, int value);

    map<int, int> data_; // TODO: rename this to _data
    size_t _order;
};
