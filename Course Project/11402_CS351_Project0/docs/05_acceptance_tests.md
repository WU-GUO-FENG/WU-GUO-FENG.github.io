# Project 0 驗收測試報告 (Acceptance Test Report)

本報告記錄了 `TwoSum` 專案的功能性測試與性能測試結果，驗證演算法實作（暴力法與雜湊表法）是否符合預期要求。

## 1. 測試環境 (Test Environment)

* **作業系統**: Windows 11
* **編譯器**: Visual Studio Community 2022 Release (MSVC v143, amd64)
* **C++ 標準**: C++20
* **建置系統**: CMake 3.20+ & vcpkg
* **測試框架**: Google Test (GTest)

---

## 2. 測試案例執行結果 (Test Case Execution Summary)

本專案實作了四大核心測試區塊（包含標準案例、負數邊界、極端空值以及大數據性能驗證），共計 4 個測試群組、10 個以上測試測資，皆已全數通過。

| 測試編號 | 測試類別 | 測試名稱 (Test Name) | 涵蓋測資與條件 | 預期結果 | 狀態 (Pass/Fail) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC-01** | 功能測試 | `TwoSumTest.BasicScenarios` | `[2, 7, 11, 15]`, target = 9 | 回傳 `[0, 1]` | **PASS** (0 ms) |
| **TC-02** | 邊界案例 | `TwoSumTest.NegativeNumbers` | `[-1, -3, 2, 4]`, target = -4 | 支援負數並回傳 `[0, 1]` | **PASS** (0 ms) |
| **TC-03** | 錯誤處理 | `TwoSumTest.EdgeCases` | 空陣列 `[]` 與 無解陣列 `[1,2,3]` | 安全回傳空向量 `[]` | **PASS** (0 ms) |
| **TC-04** | 性能測試 | `TwoSumPerformance.HashTableEfficiency` | 10,000 筆資料大數據檢驗 | 耗時 < 10ms 限額 | **PASS** (4 ms) |

---

## 3. 性能測試與複雜度分析 (Performance Analysis)

根據 **TC-04 (HashTableEfficiency)** 的實際執行數據，針對 10,000 筆線性遞增數據進行測試：
* **TwoSumHashTable (雜湊表法)** 實際執行耗時僅需 **4 ms**。
* 成功通過專案規劃書中所設定的 **10 ms 嚴格效能限制**。

### 複雜度分析對比
* **暴力法 (TwoSumArray)**：時間複雜度為 $O(n^2)$。當數據量達到 10,000 筆時，雙重迴圈的執行次數會逼近 $10^8$ 次，耗時會顯著飆升。
* **雜湊表法 (TwoSumHashTable)**：時間複雜度為 $O(n)$。利用 `std::unordered_map` 搭配 C++20 的 `.contains()` 特性，達到空間換時間的效果，即使在萬筆數據下依然能在 4ms 內瞬間完成。

---

## 4. 測試執行紀錄明細 (Test Execution Logs)

以下為 Windows PowerShell 實際執行 `twosum_test.exe` 的真實輸出紀錄：

```text
Running main() from C:\vcpkg\vcpkg\buildtrees\gtest\src\v1.17.0-0c449efaff.clean\googletest\src\gtest_main.cc
[==========] Running 4 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from TwoSumTest
[ RUN      ] TwoSumTest.BasicScenarios
[        OK ] TwoSumTest.BasicScenarios (0 ms)
[ RUN      ] TwoSumTest.NegativeNumbers
[        OK ] TwoSumTest.NegativeNumbers (0 ms)
[ RUN      ] TwoSumTest.EdgeCases
[        OK ] TwoSumTest.EdgeCases (0 ms)
[----------] 3 tests from TwoSumTest (1 ms total)

[----------] 1 test from TwoSumPerformance
[ RUN      ] TwoSumPerformance.HashTableEfficiency
[        OK ] TwoSumPerformance.HashTableEfficiency (4 ms)
[----------] 1 test from TwoSumPerformance (4 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 2 test suites ran. (7 ms total)
[  PASSED  ] 4 tests.