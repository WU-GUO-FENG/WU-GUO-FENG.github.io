# 02_SRS.md - 軟體需求規格書 (Software Requirements Specification)

## 1. 引言 (Introduction)
本文件詳細說明「CSV 微型資料庫與查詢引擎」的功能與非功能需求。本系統採用 C++ 撰寫，透過 Fluent API 提供使用者直覺、型態安全的記憶體內（In-Memory）資料查詢功能，並選用適當的資料結構實作索引以優化查詢效能。

---

## 2. 資料型態規格 (Data Type Specifications)
系統必須支援將 CSV 的純文字欄位解析並轉換為以下三種核心基礎資料型態：

| 型態名稱 | 對應 C++ 型態 | 說明 |
| :--- | :--- | :--- |
| **INT** | `int64_t` | 支援 64 位元有號整數，用於 ID、年齡、數量等計數欄位。 |
| **FLOAT** | `double` | 支援雙精度浮點數，用於價格、比率、座標等精確數值。 |
| **STRING** | `std::string` | 支援文字字串，用於名稱、文字描述等。 |

> **備註**：在載入 CSV 時，系統應支援由使用者提供 Schema（定義各欄位名稱與型態）；若未提供，則預設全數視為 `STRING`。

---

## 3. 功能需求 (Functional Requirements)

### 3.1 CSV 解析需求 (CSV Parsing)
* **REQ-F-01**：系統必須能正確解析標準的逗號分隔值（CSV）檔案。
* **REQ-F-02**：系統必須處理**包含逗號的欄位**。若欄位被雙引號包覆（例如 `"Taipei, Taiwan"`），內部的逗號不可被視為分欄符號。
* **REQ-F-03**：系統必須處理**包含雙引號的欄位**。若文字內部含有雙引號，標準 CSV 會以連續兩個雙引號表示（例如 `"He said ""Hello"""`），解析後應還原為 `He said "Hello"`。

### 3.2 Fluent API 查詢介面需求 (Query Interface)
系統必須提供方法鏈結（Method Chaining）的 C++ 呼叫介面，其核心類別與函式簽章必須滿足以下行為：

* `from(std::string filename)`：指定要查詢的 CSV 檔案來源。
* `select(std::vector<std::string> columns)`：指定欲投影（Project）的核心欄位。若傳入空陣列或不呼叫，預設為選擇所有欄位（相當於 `SELECT *`）。
* `where(std::string column, Op op, ValueType val)`：設定過濾條件。
* `execute()`：觸發查詢引擎執行，並回傳結果資料集（如 `ResultSet` 物件）。

### 3.3 支援的運算子規格 (Operators)
`where` 子句中支援的比較運算子（`Op` 列舉）如下：

| 運算子 (Op) | 適用型態 | 語義说明 |
| :--- | :--- | :--- |
| `Op::EQ` (`==`) | INT, FLOAT, STRING | 精確等於 |
| `Op::NE` (`!=`) | INT, FLOAT, STRING | 不等於 |
| `Op::GT` (`>`) | INT, FLOAT | 大於（數值比較） |
| `Op::LT` (`<`) | INT, FLOAT | 小於（數值比較） |
| `Op::CONTAINS`  | STRING | 字串模糊匹配（檢查是否包含子字串） |

### 3.4 複合條件需求 (Logical Logical)
* **REQ-F-04**：系統必須支援多個 `where` 條件的組合。
* **REQ-F-05**：系統預設連續呼叫的 `.where().where()` 之間為 **AND** 關係。
* **REQ-F-06**：系統應提供 `.or_where()` 介面以支援 **OR** 邏輯的條件組合。

### 3.5 索引管理需求 (Index Management)
* **REQ-F-07**：系統必須提供獨立 API 供使用者為特定欄位建立索引（例如 `table.create_index("age", IndexType::HASH)`）。
* **REQ-F-08**：系統應支援兩種索引類型：
  * **HASH**：用於優化 `Op::EQ` 的精準查詢。
  * **TREE**：用於優化 `Op::GT` 與 `Op::LT` 的範圍查詢。

---

## 4. 非功能需求 (Non-Functional Requirements)

### 4.1 效能需求 (Performance)
* **REQ-NF-01 (全表掃描)**：在無索引情況下，線性掃描的時間複雜度應為 $O(N)$（$N$ 為總資料列數）。
* **REQ-NF-02 (雜湊索引)**：針對已建立 `HASH` 索引的欄位進行 `Op::EQ` 查詢，其平均時間複雜度必須達到 $O(1)$。
* **REQ-NF-03 (樹狀索引)**：針對已建立 `TREE` 索引的欄位進行範圍查詢，其時間複雜度必須達到 $O(\log N + M)$（$M$ 為符合條件的資料筆數）。

### 4.2 記憶體與環境約束 (Memory & Environment)
* **REQ-NF-04**：本系統為全記憶體（In-Memory）資料庫，不實作磁碟暫存（Paging）。系統能夠處理的資料量上限取決於主記憶體（RAM）大小（預期穩定支援 100 萬筆內、約 100MB 級別的 CSV 檔案）。
* **REQ-NF-05**：必須在現代 C++ 編譯器（支援 C++17 或以上）下順利編譯，無任何警告（Warning）。
* **REQ-NF-06**：程式碼不得有記憶體洩漏（Memory Leaks），必須通過 `Valgrind` 或 AddressSanitizer (ASan) 的檢測。

---

## 5. 介面與輸出需求 (Interface & Output)
* 查詢執行後回傳的 `ResultSet` 必須提供迭代器（Iterator），讓使用者能透過標準的 `for (const auto& row : result)` 語法遍歷結果。
* 必須實作一個 `print()` 函式，能將查詢結果以格式化的表格（ASCII Table）漂亮地輸出至終端機（如 `std::cout`）。