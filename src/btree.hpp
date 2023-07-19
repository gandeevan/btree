#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <map>

#include "node.hpp"
#include "internal_node.hpp"
#include "leaf_node.hpp"
#include "sorted_vector.hpp"

using namespace std;

class BTree {
private:
    Node* root;

    Node* traverseToNextLevel(InternalNode* node, int key);
    std::pair<int, Node*> insertOrUpdateImpl(Node* node, int key, int value);
    LeafNode* traverseToLeafNode(Node* node, int key);
public:
    BTree();
    ~BTree();

    void insertOrUpdate(int key, int value);
    optional<int> get(int key);
    bool remove(int key);
};

