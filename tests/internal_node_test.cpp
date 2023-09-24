#include <cstdlib>
#include <stdexcept>

#include <gtest/gtest.h>

#include "src/internal_node.hpp"
#include "src/leaf_node.hpp"
#include "src/constants.hpp"

#include "logger/logger.hpp"


/**
  TODO:
    1. Test splits with both even and odd number of elements
**/


TEST(InternalNodeTest, TestIsFull) {
    size_t internalNodeCapacity = INTERNAL_NODE_CAPACITY(DEFAULT_ORDER);
    InternalNode *node = nullptr;
    for(size_t i=1; i<internalNodeCapacity; i++) {
        if(node)
            ASSERT_FALSE(node->isFull());
        if(i==1) {
            auto leftPtr = new LeafNode(DEFAULT_ORDER); 
            auto rightPtr = new LeafNode(DEFAULT_ORDER);
            node = new InternalNode(DEFAULT_ORDER, i, leftPtr, rightPtr);
        } else {
            auto ptr = new LeafNode(DEFAULT_ORDER);
            node->insert(i, ptr); 
        }
        ASSERT_EQ(node->size(), i+1);
    }
    ASSERT_TRUE(node->isFull());
}

TEST(InternalNodeTest, Insert) {
    size_t internalNodeCapacity = INTERNAL_NODE_CAPACITY(DEFAULT_ORDER);
    SortedArray<InternalNode::Value> expectedNodeData(internalNodeCapacity+1);

    InternalNode *node;
    for(size_t i=1; i<internalNodeCapacity; i++) {
        if(i==1) {
            auto leftPtr = new LeafNode(DEFAULT_ORDER); 
            auto rightPtr = new LeafNode(DEFAULT_ORDER);
            expectedNodeData.insert({INT_MIN, leftPtr});
            expectedNodeData.insert({(int)i, rightPtr});
            node = new InternalNode(DEFAULT_ORDER, i, leftPtr, rightPtr);
        } else {
            auto ptr = new LeafNode(DEFAULT_ORDER);
            expectedNodeData.insert({i, ptr});
            LOG_INFO("inserting element {}", i);
            node->insert(i, ptr); 
        }
        ASSERT_EQ(node->size(), i+1);
    }
    ASSERT_EQ(node->data(), expectedNodeData);
}

TEST(InternalNodeTest, SplitNode) {
    size_t internalNodeCapacity = INTERNAL_NODE_CAPACITY(DEFAULT_ORDER);
    size_t insertCount = internalNodeCapacity-1;

    InternalNode* node;
    std::vector<std::pair<int, Node*>> data;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(size_t i=1; i<=insertCount; i++) {
        if(i==1) {
            auto leftPtr = new LeafNode(DEFAULT_ORDER); //ignore
            auto rightPtr = new LeafNode(DEFAULT_ORDER);
            node = new InternalNode(DEFAULT_ORDER, i, leftPtr, rightPtr);
            data.push_back({INT_MIN, leftPtr});
            data.push_back({(int)i, rightPtr});
        } else {
            auto ptr = new LeafNode(DEFAULT_ORDER);
            node->insert(i, ptr);
            data.push_back({i, ptr});
        }
        ASSERT_EQ(node->size(), i+1);
    }
    int sizeBeforeSplit = node->size();

    ASSERT_TRUE(node->isFull());
    ASSERT_THROW(node->insert(data.size(), new LeafNode(DEFAULT_ORDER)), std::runtime_error);

    data.push_back({data.size(), new LeafNode(DEFAULT_ORDER)});
    auto [promotedKey, splitNode] = node->split(data.back().first, data.back().second);


    int expectedPromotedKey;
    SortedArray<InternalNode::Value> expectedNodeData(internalNodeCapacity+1);
    SortedArray<InternalNode::Value> expectedSplitNodeData(internalNodeCapacity+1);
    sort(data.begin(), data.end());
    for(size_t i=0; i<data.size()/2; i++) {
        if(i==0) {
            expectedNodeData.insert({INT_MIN, data[i].second});
        } else {
            expectedNodeData.insert({data[i].first, data[i].second});
        }
    }

    for(size_t i=data.size()/2; i<data.size(); i++) {
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