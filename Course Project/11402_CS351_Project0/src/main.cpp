#include <iostream>
#include <vector>
#include <format>
#include "twosum.hpp"

void printResult(const std::string& label, const std::vector<int>& res) {
    std::cout << label << ": ";
    if (res.size() == 2) {
        std::cout << "[" << res[0] << ", " << res[1] << "]\n";
    } else {
        std::cout << "No solution found.\n";
    }
}

int main() {
    std::vector<int> nums = {3, 2, 4};
    int target = 6;

    std::cout << "--- Two Sum Project (C++20) ---\n";
    std::cout << "Input: nums = [3, 2, 4], target = 6\n\n";

    // 測試暴力法
    auto res1 = TwoSumArray(nums, target);
    printResult("Brute Force ", res1);

    // 測試雜湊法
    auto res2 = TwoSumHashTable(nums, target);
    printResult("Hash Table  ", res2);

    return 0;
}