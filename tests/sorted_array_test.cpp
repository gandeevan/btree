#include <gtest/gtest.h>
#include "src/sorted_array.hpp"


TEST(SortedArrayTest, TestInsert) {
    SortedArray<int> arr(5);
    arr.insert(3);
    arr.insert(1);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    ASSERT_EQ(arr.at(0), 1);
    ASSERT_EQ(arr.at(1), 2);
    ASSERT_EQ(arr.at(2), 3);
    ASSERT_EQ(arr.at(3), 4);
    ASSERT_EQ(arr.at(4), 5);
}


TEST(SortedArrayTest, TestInsertBeyondCapacity) {
    SortedArray<int> arr(3);
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    ASSERT_THROW(arr.insert(4), std::runtime_error);
}

TEST(SortedArrayTest, TestRemoveSingle) {
    SortedArray<int> arr(5);
    arr.insert(3);
    arr.insert(1);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);
    
    arr.remove(2);
    
    ASSERT_EQ(arr.size(), 4);
    ASSERT_EQ(arr.at(0), 1);
    ASSERT_EQ(arr.at(1), 2);
    ASSERT_EQ(arr.at(2), 4);
    ASSERT_EQ(arr.at(3), 5);
}

TEST(SortedArrayTest, TestRemoveRange) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    arr.remove(1, 4);

    ASSERT_EQ(arr.size(), 2);
    ASSERT_EQ(arr.at(0), 1);
    ASSERT_EQ(arr.at(1), 5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}