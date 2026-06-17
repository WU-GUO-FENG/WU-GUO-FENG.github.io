# 01_plan.md - 開發計畫書 (Project Plan)

本計畫書採用疊代式（Iterative）開發策略，將「CSV 微型資料庫與查詢引擎」專案拆解為五個核心里程碑。每個里程碑皆包含開發目標、具體任務以及相應的測試驗證，確保專案穩健推進。

## 1. 里程碑總覽 (Milestones Overview)

| 里程碑 | 核心目標 | 預估權重 | 依賴關係 |
| :--- | :--- | :--- | :--- |
| **M1: 基礎設施與環境架設** | 建立目錄、CMake 建置系統、CI 整合與測試框架 | 10% | 無 |
| **M2: 健壯的 CSV 解析器** | 實作能夠處理複雜引號、逗號的 CSV Parser | 25% | M1 |
| **M3: 記憶體資料結構與 Fluent API**| 設計 Table 結構與實作流式查詢（Fluent API）介面 | 25% | M2 |
| **M4: 核心查詢引擎與全表掃描**| 實作查詢執行邏輯，透過線性掃描（Table Scan）回傳結果 | 20% | M3 |
| **M5: 索引優化與效能對比** | 實作 Hash/Tree 索引，進行效能取捨分析與文件收尾 | 20% | M4 |

---

## 2. 里程碑詳細規劃 (Detailed Milestones)

### 2.1 M1: 基礎設施與環境架設 (Infrastructure & Environment)
* **目標**：搭建完備的 C++ 開發與自動化測試環境。
* **開發任務**：
  * 初始化 Git 儲存庫，配置標準的 `.gitignore`。
  * 撰寫主 `CMakeLists.txt`，配置 `src/`、`include/` 與 `tests/` 的編譯目標。
  * 整合單元測試框架（如 Google Test 或 Catch2，可透過 `vcpkg` 引入）。
  * 撰寫 `.github/workflows/ci.yml`，確保每次 Push 都能在雲端自動編譯並執行測試。
* **驗證點**：CI 流程綠燈，且一個簡單的 `Hello Test` 能夠成功執行。

### 2.2 M2: 健壯的 CSV 解析器 (Robust CSV Parser)
* **目標**：寫出一個能 100% 正確解析各種邊界條件的 CSV Parser。
* **開發任務**：
  * **技術決策**：決定自手寫有限狀態機（FSM）或引入輕量級 Library。
  * 實作逐行/逐字元解析邏輯。
  * **邊界條件處理**：確保欄位內包含逗號（如 `"Taipei, Taiwan"`）、雙引號（如 `"He said ""Hello"""`）或換行符號時，皆能正確切分。
* **驗證點**：編寫 `tests/test_parser.cpp`，導入包含上述特殊字元的測試用 CSV，預期解析出的二維陣列（`std::vector<std::vector<std::string>>`）維度與內容完全正確。

### 2.3 M3: 記憶體資料結構與 Fluent API 設計 (Data Structure & Fluent API)
* **目標**：在記憶體中建立資料表結構，並提供優雅的 C++ 呼叫介面。
* **開發任務**：
  * 設計 `DataType`（如 `INT`, `FLOAT`, `STRING`）與 `Row`、`Table` 類別。
  * 實作欄位型態自動推導或手動指定機制。
  * 設計 `Query` 類別，利用方法鏈結（Method Chaining）實作 Fluent API：
    ```cpp
    db.from("data.csv").select({"name"}).where("age", Op::GT, 20);
    ```
  * 此階段的 `where()` 僅負責**記錄查詢條件（Metadata）**，不執行實際過濾。
* **驗證點**：編寫測試，確保 Fluent API 的語法能編譯通過，且 `Query` 物件能正確儲存使用者傳入的欄位與條件。

### 2.4 M4: 核心查詢引擎與全表掃描 (Query Engine & Table Scan)
* **目標**：讓查詢引擎真正運作起來，並能透過最基礎的線性掃描找到資料。
* **開發任務**：
  * 實作 `QueryEngine::execute(Query q)` 核心邏輯。
  * 實作**全表掃描（Table Scan）**：遍歷整個 `Table` 的每一列（Row），逐一比對是否符合 `Query` 中的條件（支援 `==`, `!=`, `>`, `<` 以及 `AND`/`OR` 邏輯）。
  * 處理型態轉換（例如將 CSV 的字串 `"25"` 轉為數字後與條件中的 `20` 進行大小比較）。
* **驗證點**：撰寫整合測試，驗證多條件組合查詢下的回傳結果，必須與預期的資料集完全吻合。

### 2.5 M5: 索引優化與效能對比 (Indexing & Performance Analysis)
* **目標**：引入資料庫核心的「索引」技術，大幅提升查詢速度，並完成專案。
* **開發任務**：
  * 設計 `Index` 基底類別。
  * 實作 **Hash Index**（使用 `std::unordered_map`），優化等值查詢（`==`）。
  * 實作 **Range Index**（使用 `std::map` 或平衡樹），優化範圍查詢（`>`、`<`）。
  * 修改查詢引擎：當偵測到查詢欄位有建立索引時，自動繞過全表掃描，改用索引查詢（Index Lookup）。
  * 撰寫效能測試（Benchmark），對比百萬級資料量下，有低索引與無索引的耗時差異。
  * 補齊 `docs/` 剩餘的技術文件（SDS, Test Plan, Known Issues 等）。
* **驗證點**：效能數據顯示索引查詢達到 $O(1)$ 或 $O(\log N)$ 的時間複雜度，明顯快於全表掃描的 $O(N)$。

---

## 3. 風險管理與應對策略 (Risk Management)

* **風險 1：CSV 欄位型態過於複雜，導致解析或比較失敗。**
  * *應對策略*：初期一律將所有資料視為 `std::string`。M4 階段再引入輕量型的型態轉換邏輯，或在載入 CSV 時強制要求使用者定義 Schema。
* **風險 2：記憶體佔用過大（Memory Consumption）。**
  * *應對策略*：本專案聚焦於 In-Memory 實作，若測試巨型 CSV 導致記憶體不足，將在 `08_known_issues.md` 中明確定義資料量邊界（例如限制在 100 萬筆以內）。