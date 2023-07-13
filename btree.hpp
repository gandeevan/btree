#include <vector>
#include <memory>
#include <iostream>
#include <optional>

using namespace std;

#define NODE_SIZE 4

#define INTERNAL_NODE_TYPE 0
#define LEAF_NODE_TYPE 1

class Node {
   int nodeType;  
public:
    Node(int _nodeType) : nodeType(_nodeType) {

    };

    int getNodeType() {
        return nodeType;
    }

    vector<int> keys;
};

class InternalNode : public Node {
public:
    InternalNode() : Node(INTERNAL_NODE_TYPE) {

    }
    vector<Node*> data;
};

class LeafNode : public Node {
public:
    LeafNode() : Node(LEAF_NODE_TYPE) {

    }
    vector<int> data;
};

class BTree {
private:
    Node* root;
public:
    BTree();
    ~BTree();

    void insertOrUpdate(int key, int value);
    optional<int> get(int key);
    bool remove(int key);
};

