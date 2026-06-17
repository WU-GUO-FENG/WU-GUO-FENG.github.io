#include "twosum.hpp"
#include <unordered_map>

/**
 * TwoSumArray: 暴力法 (O(n^2))
 */
std::vector<int> TwoSumArray(const std::vector<int>& nums, int target) {
    if (nums.size() < 2) return {};

    for (std::size_t i = 0; i < nums.size(); ++i) {
        for (std::size_t j = i + 1; j < nums.size(); ++j) {
            if (nums[i] + nums[j] == target) {
                return {static_cast<int>(i), static_cast<int>(j)};
            }
        }
    }
    return {};
}

/**
 * TwoSumHashTable: 哈希表法 (O(n))
 */
std::vector<int> TwoSumHashTable(const std::vector<int>& nums, int target) {
    if (nums.size() < 2) return {};

    std::unordered_map<int, int> num_map;
    num_map.reserve(nums.size()); 

    for (std::size_t i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];

        // C++20 特性：使用 .contains() 檢查 Key
        if (num_map.contains(complement)) {
            return {num_map[complement], static_cast<int>(i)};
        }

        num_map[nums[i]] = static_cast<int>(i);
    }

    return {};
}