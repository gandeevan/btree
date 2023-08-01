#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <map>

#include "node.hpp"
#include "internal_node.hpp"
#include "leaf_node.hpp"
#include "sorted_array.hpp"

using namespace std;

class BTree {
private:
    Node* root_;

    Node* traverseToNextLevel(InternalNode* node, int key) const;
    LeafNode* traverseToLeafNode(Node* node, int key) const;
    std::pair<int, Node*> insertOrUpdateImpl(Node* node, int key, int value);
public:
    BTree();
    ~BTree();

    void insertOrUpdate(int key, int value);
    optional<int> get(int key) const;
    bool remove(int key);
};

