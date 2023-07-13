#include "btree.hpp"

BTree::BTree() {
    root = new LeafNode();
}

void BTree::insertOrUpdate(int key, int value) {

}

bool BTree::remove(int key) {
    return false;
}


std::optional<int> BTree::get(int key) {
    return optional<int>();
}