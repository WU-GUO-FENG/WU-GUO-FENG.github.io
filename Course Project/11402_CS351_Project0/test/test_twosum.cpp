#include <gtest/gtest.h>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "twosum.hpp"

// --- 功能性測試 (Functional Tests) ---

// TC-01: 標準案例
TEST(TwoSumTest, BasicScenarios) {
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> expected = {0, 1};
    
    EXPECT_EQ(TwoSumArray(nums, target), expected);
    EXPECT_EQ(TwoSumHashTable(nums, target), expected);
}

// TC-02: 負數案例
TEST(TwoSumTest, NegativeNumbers) {
    std::vector<int> nums = {-1, -3, 2, 4};
    int target = -4;
    std::vector<int> expected = {0, 1};
    
    EXPECT_EQ(TwoSumHashTable(nums, target), expected);
}

// TC-07 & TC-08: 無解與空陣列
TEST(TwoSumTest, EdgeCases) {
    std::vector<int> empty_nums = {};
    EXPECT_TRUE(TwoSumHashTable(empty_nums, 0).empty());

    std::vector<int> no_solution = {1, 2, 3};
    EXPECT_TRUE(TwoSumHashTable(no_solution, 7).empty());
}

// --- 性能測試 (Performance Tests) ---

// TC-09: 大數據檢驗 (10,000 筆資料)
TEST(TwoSumPerformance, HashTableEfficiency) {
    const int N = 10000;
    std::vector<int> nums;
    nums.reserve(N);
    for (int i = 0; i < N; ++i) {
        nums.push_back(i);
    }
    
    int target = (N - 2) + (N - 1); 
    
    auto start = std::chrono::high_resolution_clock::now();
    auto result = TwoSumHashTable(nums, target);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::vector<int> expected = {N - 2, N - 1};
    EXPECT_EQ(result, expected);
    EXPECT_LT(duration.count(), 10); // 確保在 10ms 內完成
}