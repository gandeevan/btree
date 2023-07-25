
#include "leaf_node.hpp"

unsigned LeafNode::size() {
    return data.size();
}

bool LeafNode::update(int key, int value) {
    if(data.find(key) != data.end()) {
        data[key] = value;   
        return true;     
    }
    return false;
}

void LeafNode::insert(int key, int value) {
    if(isFull()) {
        throw runtime_error("node is full");
    }

    data.insert({key, value});
}

LeafNode* LeafNode::split(int newKey, int newValue) {
    LeafNode* nodeToInsert = this;
    auto splitNode = new LeafNode();
    auto it = data.begin();
    std::advance(it, data.size()/2);

    // makes sure the two nodes are equal in size after the split
    if(data.size()%2 != 0) {
        if(newKey > it->first) {
            nodeToInsert = splitNode;
            it++;    
        }
    }

    while(it != data.end()) {
        splitNode->insert(it->first, it->second);
        it = data.erase(it);
    }

    nodeToInsert->insert(newKey, newValue);
    return splitNode;
}
