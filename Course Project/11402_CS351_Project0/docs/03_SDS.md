# Software Design Specification (SDS) - Two Sum Project

## 1. 引言

### 1.1 目的

本文件詳細說明「Project 0 - Two Sum」的技術設計方案。內容涵蓋演算法邏輯、資料結構選用、模組劃分以及複雜度分析，作為開發實作的直接依據。

### 1.2 範疇

本設計僅針對 TwoSumArray 與 TwoSumHashTable 兩個核心函式及其配套的測試環境進行說明。

## 2. 系統架構

### 2.1 模組劃分

專案採用標頭檔與實作檔分離的結構，確保程式碼的可重用性與清晰度：

- 標頭檔 (include/two_sum.hpp)：定義公開 API 介面。
- 實作檔 (src/two_sum.cpp)：實作演算法邏輯。
- 主程式 (src/main.cpp)：提供範例展示與基本 CLI 交互。

### 2.2 技術棧

- 語言標準：C++23
- 容器：std::vector (輸出/輸入), std::unordered_map (哈希表)
- 錯誤處理：回傳空向量 std::vector<int>{}。

## 3. 演算法設計

### 3.1 TwoSumArray (暴力法)

使用雙層嵌套迴圈，遍歷所有可能的數值組合。

**流程說明：**

- 外層迴圈 $i$ 從 $0$ 遍歷到 $n-1$。
- 內層迴圈 $j$ 從 $i+1$ 遍歷到 $n-1$。
- 檢查 nums[i] + nums[j] == target。若成立，回傳 {i, j}。

**優點：** 不需額外空間。

**缺點：** 執行時間隨資料量呈平方級增長。

### 3.2 TwoSumHashTable (哈希表法)

利用空間換取時間，透過一次遍歷完成查找。

**流程說明：**

- 建立一個 std::unordered_map<int, int>，Key 為數值，Value 為索引。
- 遍歷陣列，對於當前數值 nums[i]：
  - 計算補數 complement = target - nums[i]。
  - 在 Map 中尋找 complement。若找到，回傳 {map[complement], i}。
  - 若未找到，將 {nums[i], i} 存入 Map。

**優點：** 查找速度極快。

**缺點：** 需要額外的記憶體來儲存雜湊表。

## 4. 資料結構設計

| 函式 | 關鍵資料結構 | 說明 |
|------|--------------|------|
| TwoSumArray | 無 | 僅使用原始陣列進行索引存取。 |
| TwoSumHashTable | std::unordered_map<int, int> | 基於 Hash Table 實作，提供平均 $O(1)$ 的查找效能。 |

## 5. 複雜度分析

| 演算法 | 時間複雜度 | 空間複雜度 | 備註 |
|--------|------------|------------|------|
| TwoSumArray | $O(n^2)$ | $O(1)$ | 雙層迴圈。 |
| TwoSumHashTable | $O(n)$ | $O(n)$ | 一次遍歷，最壞情況需儲存 $n$ 個元素。 |

## 6. 詳細介面定義 (API)

```cpp
/**
 * @brief 使用暴力法尋找兩數之和
 * @param nums 輸入整數向量 (const reference 以優化效能)
 * @param target 目標值
 * @return std::vector<int> 包含兩個索引的向量；若無解則回傳空向量
 */
std::vector<int> TwoSumArray(const std::vector<int>& nums, int target);

/**
 * @brief 使用哈希表優化法尋找兩數之和
 * @param nums 輸入整數向量
 * @param target 目標值
 * @return std::vector<int> 包含兩個索引的向量；若無解則回傳空向量
 */
std::vector<int> TwoSumHashTable(const std::vector<int>& nums, int target);
```

## 7. 錯誤處理與邊界設計

- **輸入驗證：** 若 nums.size() < 2，直接返回 {}。
- **無解處理：** 若遍歷結束仍未找到符合條件的組合，返回 {}。
- **重複元素：** 哈希表法應先檢查「補數」是否存在，再存入當前元素，以避免「同一個元素被使用兩次」的問題。

## 8. 追蹤矩陣

**簡述**

- REQ-FR1 $\rightarrow$ 由 TwoSumArray 實作。
- REQ-FR2 $\rightarrow$ 由 TwoSumHashTable 實作。
- REQ-PR2 $\rightarrow$ 哈希表選用 std::unordered_map 確保 $O(n)$ 時間複雜度。