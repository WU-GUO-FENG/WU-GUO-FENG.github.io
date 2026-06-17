# 04_test_plan.md - 測試計畫書 (Test Plan)

## 1. 測試策略與目標
本專案採用自動化單元測試（Unit Testing）與整合測試（Integration Testing）來確保「CSV 微型資料庫」的正確性與穩定性。所有測試均整合於 Google Test 框架中，並透過 GitHub Actions CI 流程於每次程式碼推送時自動執行。

## 2. 測試範疇與測資設計

### 2.1 CSV 解析器測試 (M2)
* **測試目標**：驗證有限狀態機（FSM）能否應對各種現實世界中的極端 CSV 格式。
* **關鍵測資**：
  * 普通逗號分隔資料。
  * 欄位內包含被引號包覆的逗號（例如 `"Taipei, Taiwan"`）。
  * 欄位內包含轉義的連續雙引號（例如 `"He said ""Hello"""` -> `He said "Hello"`）。

### 2.2 強型態轉換與 Table 結構測試 (M3)
* **測試目標**：驗證字串資料能否正確依據 Schema 轉換為 C++ 的 `int64_t`、`double` 與 `std::string`。
* **驗證方式**：利用 `std::get` 提取 `std::variant` 中的數值，確保型態與數值完全精確。

### 2.3 查詢引擎與全表掃描測試 (M4)
* **測試目標**：驗證過濾邏輯（EQ, NE, GT, LT, CONTAINS）以及多條件（AND / OR）組合的正確性。
* **驗證方式**：對比過濾後的 `ResultSet` 筆數與內容是否與預期資料集完全吻合。

### 2.4 雜湊索引優化測試 (M5)
* **測試目標**：驗證索引建立後，引擎能自動辨識並切換至 $O(1)$ 的 Index Lookup 快車道。
* **驗證方式**：檢查 `was_index_used()` 是否在有索引時觸發為 `true`，且過濾結果保持完全正確。

## 3. 測試執行環境
* **本地端**：Windows 11 / Visual Studio 2022 MSVC (x64) / CMake & CTest
* **雲端 CI**：Ubuntu Latest / GCC / GitHub Actions