# 06_traceability.md - 需求追溯規格書 (Requirements Traceability Matrix)

## 1. 引言 (Introduction)
本文件為「CSV 微型資料庫與查詢引擎」的需求追溯表（RTM）。本表建立了雙向追溯機制，將軟體需求規格書（SRS）中的各項功能與非功能需求，映射至軟體設計規格書（SDS）的架構模組，以及驗收測試說明書（Acceptance Tests）的具體測試案例中，以確保所有規格均有對應的設計落實與測試驗證。

---

## 2. 需求追溯矩陣 (Traceability Matrix)

| 需求編號 (SRS) | 需求名稱與描述 | 設計模組 / 核心類別 (SDS) | 驗收測試案例 (Acceptance Test) |
| :--- | :--- | :--- | :--- |
| **REQ-F-01** | 標準 CSV 格式解析 | `CSVParser::parse` | **ATC-01** (載入 5 筆資料驗證) |
| **REQ-F-02** | 處理欄位內包含引號包覆的逗號 | `CSVParser::parse` (狀態機 `State::Quoted`) | **ATC-01** (驗證包含逗號欄位) |
| **REQ-F-03** | 處理欄位內連續雙引號轉義 (`""` -> `"`) | `CSVParser::parse` (狀態機 `State::Escape`) | **ATC-01** (驗證 `Charlie "The King" Lin`) |
| **REQ-F-04** | 支援 Fluent API 方法鏈結呼叫介面 | `QueryBuilder::select`, `QueryBuilder::where` | **ATC-02**, **ATC-03** (流式語法串接驗證) |
| **REQ-F-05** | 預設連續 `where` 條件之間為 `AND` 關係 | `QueryBuilder::execute` (條件過濾迴圈) | **ATC-02** (多條件全表掃描過濾) |
| **REQ-F-06** | 支援 `or_where` 提供 `OR` 邏輯組合 | `QueryBuilder::or_where`, `QueryBuilder::execute` | **ATC-03** (驗證工程部 OR 遠端員工) |
| **REQ-F-07** | 支援使用者為特定欄位建立索引 API | `Table::create_index` | **ATC-05** (呼叫 `create_index` 步驟) |
| **REQ-F-08** | 實作 `HASH` 類型索引優化等值查詢 | `HashIndex`, `BaseIndex` | **ATC-05** (雜湊索引功能驗證) |
| **REQ-NF-01**| 無索引時執行全表掃描 $O(N)$ | `QueryBuilder::execute` (線性掃描 fallback) | **ATC-02**, **ATC-04** (無索引過濾驗證) |
| **REQ-NF-02**| 雜湊索引精準查詢平均時間複雜度 $O(1)$ | `QueryBuilder::execute` (Index Lookup 通道) | **ATC-05** (檢查 `was_index_used() == true`) |
| **REQ-NF-03**| 資料庫型態安全比較機制與寬容轉型 | `evaluate_condition` (利用 `std::visit`) | **ATC-01** (浮點數轉型), **ATC-02** (大小比較) |

---

## 3. 追溯性評估與結論 (Traceability Assessment)

### 3.1 覆蓋率分析 (Coverage Analysis)
* **需求覆蓋率**：100%。軟體需求規格書（`02_SRS.md`）中定義的所有核心功能需求（REQ-F）與關鍵非功能需求（REQ-NF）皆已在設計（`03_SDS.md`）中落實，並全數被驗收測試案例（`05_acceptance_tests.md`）所覆蓋。
* **設計孤兒檢查**：經審查，`03_SDS.md` 中設計的 `Table`、`QueryBuilder`、`CSVParser`、`HashIndex` 類別均有明確對應的需求來源，無任何憑空捏造的冗餘設計。
* **測試孤兒檢查**：經審查，`05_acceptance_tests.md` 中的所有驗收測試案例（ATC-01 至 ATC-05）皆有明確對應的 SRS 需求編號，無無效測試。

### 3.2 結論
本專案之需求、設計與測試架構具備完全的**雙向可追溯性（Bidirectional Traceability）**。未來若規格發生變更（例如：欲擴充 `TREE` 索引或引入 `DELETE` 語法），開發人員可透過本表快速定位受影響的軟體類別與測試代碼，確保系統的可維護性。