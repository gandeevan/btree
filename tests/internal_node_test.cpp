#include <cstdlib>
#include <gtest/gtest.h>
#include <stdexcept>
#include "src/internal_node.hpp"
#include "src/leaf_node.hpp"


// TODO:
// 1. Make the node size configurable and test split with both even and odd number of elements

TEST(InternalNodeTest, Insert) {
    InternalNode *node;
    SortedArray<InternalNode::Value> expectedNodeData(MAX_NODE_SIZE+1);

    int insertCount = MAX_NODE_SIZE-1;
    for(int i=1; i<=insertCount; i++) {
        if(i==1) {
            auto leftPtr = new LeafNode(); 
            auto rightPtr = new LeafNode();
            expectedNodeData.insert({INT_MIN, leftPtr});
            expectedNodeData.insert({i, rightPtr});
            node = new InternalNode(i, leftPtr, rightPtr);
        } else {
            auto ptr = new LeafNode();
            expectedNodeData.insert({i, ptr});
            node->insert(i, ptr); 
        }
        ASSERT_EQ(node->size(), i+1);
    }
    ASSERT_EQ(node->data(), expectedNodeData);
}

TEST(InternalNodeTest, SplitNode) {
    int insertCount = MAX_NODE_SIZE-1;
    InternalNode* node;

    std::vector<std::pair<int, Node*>> data;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(int i=1; i<=insertCount; i++) {
        if(i==1) {
            auto leftPtr = new LeafNode(); //ignore
            auto rightPtr = new LeafNode();
            node = new InternalNode(i, leftPtr, rightPtr);
            data.push_back({INT_MIN, leftPtr});
            data.push_back({i, rightPtr});
        } else {
            auto ptr = new LeafNode();
            node->insert(i, ptr);
            data.push_back({i, ptr});
        }
        ASSERT_EQ(node->size(), i+1);
    }
    int sizeBeforeSplit = node->size();

    ASSERT_TRUE(node->isFull());
    ASSERT_THROW(node->insert(data.size(), new LeafNode()), std::runtime_error);

    data.push_back({data.size(), new LeafNode()});
    auto [promotedKey, splitNode] = node->split(data.back().first, data.back().second);


    int expectedPromotedKey;
    SortedArray<InternalNode::Value> expectedNodeData(MAX_NODE_SIZE+1);
    SortedArray<InternalNode::Value> expectedSplitNodeData(MAX_NODE_SIZE+1);
    sort(data.begin(), data.end());
    for(int i=0; i<data.size()/2; i++) {
        if(i==0) {
            expectedNodeData.insert({INT_MIN, data[i].second});
        } else {
            expectedNodeData.insert({data[i].first, data[i].second});
        }
    }

    for(int i=data.size()/2; i<data.size(); i++) {
        if(i==data.size()/2) {
            expectedPromotedKey = data[i].first;
            expectedSplitNodeData.insert({INT_MIN, data[i].second});
        } else {
            expectedSplitNodeData.insert({data[i].first, data[i].second});
        }
    }
    
    // Validate the promoted key
    ASSERT_EQ(promotedKey, expectedPromotedKey);

    // InternalNode.size() returns the number of pointers in the 
    // node. After the split, the splitnode should have either the same 
    // or one extra pointer than the right node. And the sum of pointers 
    // in the left node and the right node should be 1+sizeBeforeSplit.
    int offset = 1 ? ((sizeBeforeSplit%2) == 0) : 0;
    ASSERT_EQ(node->size(), (sizeBeforeSplit+1)/2);
    ASSERT_EQ(splitNode->size(), ((sizeBeforeSplit+1)/2)+offset);
    ASSERT_EQ(node->size()+splitNode->size(), 1+sizeBeforeSplit);

    ASSERT_EQ(node->data(), expectedNodeData);
    ASSERT_EQ(splitNode->data(), expectedSplitNodeData);
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}