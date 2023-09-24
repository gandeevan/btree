#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <map>

#include "node.hpp"
#include "internal_node.hpp"
#include "leaf_node.hpp"
#include "sorted_array.hpp"

#define DEFAULT_ORDER 2

struct NodeContext {
    Node* node;
    InternalNode* parent;
    size_t idx;     // The index of the node in the parent
};



class BTree {
private:
    Node* _root;
    size_t _order;
    
    std::map<int, vector<Node*>> levelOrderTraversal() const; 
    LeafNode* traverseToLeafNode(Node* node, int key) const;
    std::pair<int, Node*> insertOrUpdateImpl(Node* node, int key, int value);
    std::pair<bool, bool> removeImpl(NodeContext nodeCtx, int key); 
    bool checkFillFactorInvariant() const;
    
public:
    BTree(size_t order = DEFAULT_ORDER);
    ~BTree();

    void print();
    void insertOrUpdate(int key, int value);
    std::optional<int> get(int key) const;
    bool remove(int key);
};

