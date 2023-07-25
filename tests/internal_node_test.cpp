#include <cstdlib>
#include <gtest/gtest.h>
#include <stdexcept>
#include "src/internal_node.hpp"


TEST(InternalNodeTest, Insert) {
    InternalNode node(5, nullptr, nullptr);
    ASSERT_EQ(node.size(), 1);

    Node* new_node = new InternalNode(7, nullptr, nullptr);
    node.insert(7, new_node);    
    ASSERT_EQ(node.size(), 2); 
}

TEST(InternalNodeTest, SplitNode) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::pair<int, Node*>> data;
    for(int i=0; i<=MAX_NODE_SIZE; i++) {
        data.push_back({i, (Node*)std::rand()});
    }

    InternalNode node(data[1].first, data[0].second, data[1].second);
    EXPECT_EQ(node.size(), 1);        
    for(int i=2; i<data.size(); i++) {
        node.insert(data[i].first, (Node *)data[i].second);
        EXPECT_EQ(node.size(), i);
    }

    EXPECT_TRUE(node.isFull());
    EXPECT_THROW(node.insert(data.size(), (Node*) rand()), std::runtime_error);
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}