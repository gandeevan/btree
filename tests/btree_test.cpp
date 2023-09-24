#include <climits>
#include <gtest/gtest.h>
#include <unordered_map>

#include "logger/logger.hpp"
#include "utilities/utils.hpp"
#include "src/btree.hpp"

unordered_map<int, int> generateTestDataSet(int count, int maxKey, int minKey) {
  unordered_map<int, int> testData;
  for(int i=0; i<count; i++) {
    int key = getRandomNumber(minKey, maxKey);
    int value = getRandomNumber(SHRT_MIN, SHRT_MAX);
    testData.insert_or_assign(key, value);
    LOG_DEBUG("Inserting Key {} Value {}", key, value);
  }
  return testData;
}

void checkIfKeysExist(BTree& tree, unordered_map<int, int>& data, set<int> deletedKeys) {
  for(auto& data : data) {
    if(deletedKeys.find(data.first) == deletedKeys.end()) {
      auto result = tree.get(data.first);
      ASSERT_TRUE(result.has_value());
      ASSERT_EQ(result.value(), data.second);
    }
  }  
}

void checkIfKeysDontExist(BTree& tree, set<int> keys) {
  for(auto& key : keys) {
    auto result = tree.get(key);
    ASSERT_FALSE(result.has_value());
  }
}

TEST(BTreeTest, TestInsert) {
    BTree tree;
    int count = 10000;
    int minKey = -count;
    int maxKey = count;

    unordered_map<int, int> testData;
    for(int i=0; i<count; i++) {
      int key = getRandomNumber(minKey, maxKey);
      int value = getRandomNumber(SHRT_MIN, SHRT_MAX);
      testData.insert_or_assign(key, value);
      LOG_DEBUG("Inserting Key {} Value {}", key, value);
    }

    for(auto& data : testData) {
      tree.insertOrUpdate(data.first, data.second);
    }
    checkIfKeysExist(tree, testData, set<int>());
}


TEST(BTreeTest, TestDelete) {
  BTree tree;

  int numKeysToInsert = 10000;
  int numKeysToDelete = numKeysToInsert/2;
  int minKey = -numKeysToInsert;
  int maxKey = numKeysToInsert;

  set<int> deletedKeys;  
  unordered_map<int, int> testData = generateTestDataSet(numKeysToInsert, maxKey, minKey);
  
  for(auto& data : testData) {
    tree.insertOrUpdate(data.first, data.second);
  }
  checkIfKeysExist(tree, testData, deletedKeys);

  int count = 0;
  for(auto& data : testData) {
    if(count++ == numKeysToDelete) {
      break;
    }
    ASSERT_TRUE(tree.remove(data.first));
    deletedKeys.insert(data.first);
  }
  checkIfKeysExist(tree, testData, deletedKeys);
  checkIfKeysDontExist(tree, deletedKeys);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}