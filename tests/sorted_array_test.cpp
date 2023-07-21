#include <gtest/gtest.h>
#include "src/sorted_array.hpp"

TEST(SortedArrayTest, TestInsert) {
    SortedArray<int> arr(5);
    arr.insert(5);
    arr.insert(2);
    arr.insert(1);
    arr.insert(3);
    arr.insert(4);
    for(int i=0; i<5; i++) {
        EXPECT_EQ(arr.at(i), i+1);
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  return 0;
}