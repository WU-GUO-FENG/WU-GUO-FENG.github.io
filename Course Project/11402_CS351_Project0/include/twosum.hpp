#ifndef TWOSUM_HPP
#define TWOSUM_HPP

#include <vector>

/**
 * @brief 使用暴力法尋找兩數之和 (O(n^2))
 */
std::vector<int> TwoSumArray(const std::vector<int>& nums, int target);

/**
 * @brief 使用哈希表優化法尋找兩數之和 (O(n))
 */
std::vector<int> TwoSumHashTable(const std::vector<int>& nums, int target);

#endif // TWOSUM_HPP