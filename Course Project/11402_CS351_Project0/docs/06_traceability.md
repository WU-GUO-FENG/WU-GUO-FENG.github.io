# Project 0 需求追蹤報告 (Traceability Report)

本報告建立需求（Requirements）、設計實作（Implementation）與測試驗證（Verification）之間的雙向追蹤關係，以確保所有系統功能皆已完整實作並通過驗證，無任何遺漏。

---

## 1. 追蹤標籤定義 (Traceability IDs)

為方便進行交叉引用，本專案定義以下三大類別之標籤：
* **REQ-XX**: 系統與功能需求 (From `01_plan.md` / `intended_use.md`)
* **IMPL-XX**: 核心代碼實作模組 (From `src/` & `include/`)
* **TC-XX**: 驗收測試案例 (From `04.md` / `test_twosum.cpp`)

---

## 2. 需求追蹤矩陣 (Requirements Traceability Matrix)

以下矩陣呈現了從「需求定義」到「代碼實作」再到「最終測試」的完整映射關係。

| 需求編號 (REQ) | 需求描述 (Description) | 實作模組 (IMPL) | 驗證測試案例 (TC) | 狀態 (Status) |
| :--- | :--- | :--- | :--- | :--- |
| **REQ-01** | 實作暴力法演算法，時間複雜度為 $O(n^2)$ | `twosum.cpp` -> `TwoSumArray()` | `TwoSumTest.BasicScenarios` | **已完成 (Passed)** |
| **REQ-02** | 實作雜湊表演算法，時間複雜度為 $O(n)$ | `twosum.cpp` -> `TwoSumHashTable()` | `TwoSumTest.BasicScenarios` | **已完成 (Passed)** |
| **REQ-03** | 演算法必須正確處理負整數輸入 | `twosum.cpp` | `TwoSumTest.NegativeNumbers` | **已完成 (Passed)** |
| **REQ-04** | 演算法必須正確處理重複元素與 0 | `twosum.cpp` | `TwoSumTest.BasicScenarios` | **已完成 (Passed)** |
| **REQ-05** | 針對無解或長度小於 2 的極端狀況進行錯誤處理 | `twosum.cpp` | `TwoSumTest.EdgeCases` | **已完成 (Passed)** |
| **REQ-06** | 雜湊表法在 10,000 筆大數據下需符合效能限制 (< 10ms) | `twosum.cpp` (使用 reserve 優化) | `TwoSumPerformance.HashTableEfficiency` | **已完成 (Passed)** |
| **REQ-07** | 使用 CMake 作為專案建置系統，支援自動化設定 | `CMakeLists.txt` | 系統成功編譯產出執行檔 | **已完成 (Passed)** |
| **REQ-08** | 整合 Google Test (GTest) 進行自動化單元測試 | `vcpkg`, `test_twosum.cpp` | 執行 `twosum_test.exe` 全綠燈 | **已完成 (Passed)** |

---

## 3. 覆蓋率與完整性評估 (Coverage Analysis)

### 3.1 需求覆蓋率 (Requirements Coverage)
$$R_{coverage} = \frac{\text{已實作且通過測試的需求數}}{\text{總需求數}} = \frac{8}{8} = 100\%$$
* 評估：所有在專案初期定義的功能與非功能性需求，皆已 100% 映射至對應的原始碼與 GTest 測試案例中，無任何孤兒需求（Orphan Requirements）。

### 3.2 測試有效性 (Test Effectiveness)
* 所有的單元測試皆成功調用 `include/twosum.hpp` 所宣告的介面。
* 性能測試（TC-04）成功驗證了 `REQ-06` 的時間限制（實際耗時 4ms < 10ms），證明優化有效。

---
報告發布日期: 2026-05-28