#include <gtest/gtest.h>
#include "src/sorted_array.hpp"

// TODO:
// 1. Add a unit test for the find() method.

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

TEST(SortedArrayTest, TestEraseElement) {
    SortedArray<int> arr(5);
    arr.insert(3);
    arr.insert(1);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);
    
    arr.eraseElement(2);
    
    ASSERT_EQ(arr.size(), 4);
    ASSERT_EQ(arr.at(0), 1);
    ASSERT_EQ(arr.at(1), 3);
    ASSERT_EQ(arr.at(2), 4);
    ASSERT_EQ(arr.at(3), 5);
}

TEST(SortedArrayTest, TestEraseIndex) {
    SortedArray<int> arr(5);
    arr.insert(3);
    arr.insert(1);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);
    
    arr.eraseIndex(2);
    
    ASSERT_EQ(arr.size(), 4);
    ASSERT_EQ(arr.at(0), 1);
    ASSERT_EQ(arr.at(1), 2);
    ASSERT_EQ(arr.at(2), 4);
    ASSERT_EQ(arr.at(3), 5);
}

TEST(SortedArrayTest, TestEraseIndexRange) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    arr.eraseIndexRange(1, 5);
    ASSERT_EQ(arr.size(), 1);
    ASSERT_EQ(arr.at(0), 1);
}

TEST(SortedArrayTest, EraseIndexRangeOutOfBoundsDeathTest) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    ASSERT_DEATH(arr.eraseIndexRange(1, 6), "");
}

TEST(SortedArrayTest, EraseIndexRangeInvalidRangeDeathTest) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    ASSERT_DEATH(arr.eraseIndexRange(3, 2),  "");
    ASSERT_DEATH(arr.eraseIndexRange(3, 3),  "");
}

TEST(SortedArrayTest, TestForwardIterator) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    auto it = arr.begin();
    ASSERT_EQ(*it, 1);
    it++;
    ASSERT_EQ(*it, 2);
    it++;
    ASSERT_EQ(*it, 3);
    it++;
    ASSERT_EQ(*it, 4);
    it++;
    ASSERT_EQ(*it, 5);
    it++;
    ASSERT_EQ(it, arr.end());
    
    ASSERT_THROW(*it, std::runtime_error);
    ASSERT_THROW(it++, std::runtime_error);
}

TEST(SortedArrayTest, TestReverseIterator) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);

    auto it = arr.rbegin();
    ASSERT_EQ(*it, 5);
    it++;
    ASSERT_EQ(*it, 4);
    it++;
    ASSERT_EQ(*it, 3);
    it++;
    ASSERT_EQ(*it, 2);
    it++;
    ASSERT_EQ(*it, 1);
    it++;
    ASSERT_EQ(it, arr.rend());
    ASSERT_THROW(*it, std::runtime_error);
    ASSERT_THROW(it++, std::runtime_error);
}

TEST(SortedArrayTest, TestIteratorArrowOperator) {
    SortedArray<std::pair<int, int>> arr(5);
    arr.insert(std::make_pair(1, 1));
    arr.insert(std::make_pair(3, 3));
    arr.insert(std::make_pair(5, 5));
    arr.insert(std::make_pair(2, 2));
    arr.insert(std::make_pair(4, 4));

    auto it = arr.begin();
    ASSERT_EQ(it->first, 1);
    it++;
    ASSERT_EQ(it->first, 2);
    it++;
    ASSERT_EQ(it->first, 3);
    it++;
    ASSERT_EQ(it->first, 4);
    it++;
    ASSERT_EQ(it->first, 5);
    it++;
    ASSERT_EQ(it, arr.end());
    ASSERT_THROW(it->first, std::runtime_error);
}



TEST(SortedArrayTest, TestReplace) {
    SortedArray<int> arr(5);
    arr.insert(1);
    arr.insert(3);
    arr.insert(5);
    arr.insert(2);
    arr.insert(4);
    ASSERT_EQ(arr.at(0), 1);
    ASSERT_EQ(arr.at(1), 2);
    ASSERT_EQ(arr.at(2), 3);
    ASSERT_EQ(arr.at(3), 4);
    ASSERT_EQ(arr.at(4), 5);

    arr.replace(0, 0);
    ASSERT_EQ(arr.at(0), 0);
    ASSERT_EQ(arr.at(1), 2);
    ASSERT_EQ(arr.at(2), 3);
    ASSERT_EQ(arr.at(3), 4);
    ASSERT_EQ(arr.at(4), 5);

    ASSERT_THROW(arr.replace(0, 4), std::runtime_error);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}