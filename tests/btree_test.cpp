#include <gtest/gtest.h>
#include "src/btree.hpp"

TEST(BTreeTest, TestInsert) {
    BTree tree;
    tree.insertOrUpdate(1, 1);
    // EXPECT_EQ(tree.get(1).value(), 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  return 0;
}