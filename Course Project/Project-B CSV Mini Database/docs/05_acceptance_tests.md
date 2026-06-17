# 05_acceptance_tests.md - 驗收測試說明書 (Acceptance Tests)

## 1. 引言 (Introduction)
本文件定義了「CSV 微型資料庫與查詢引擎」的驗收測試案例（Acceptance Test Cases）。驗收測試旨在由使用者或系統整合視角，驗證系統功能是否完全符合軟體需求規格書（SRS）中所定義之各項行為。所有測試均以系統最終交付的 API 行為與核心輸出為準。

---

## 2. 測試環境與前置資料 (Test Setup)
所有驗收測試案例均基於以下預設的測試資料集 `employees.csv` 進行驗證：

```text
id,name,department,salary,is_remote
101,Yu-Feng Huang,Engineering,85000.50,1
102,Alice Chang,Engineering,92000.00,0
103,Bob Jones,Marketing,60000.25,1
104,"Charlie ""The King"" Lin",HR,55000.00,0
105,David Wang,Marketing,62000.00,1
```

預期 Schema 配置：

- `id`: `DataType::INT`
- `name`: `DataType::STRING`
- `department`: `DataType::STRING`
- `salary`: `DataType::FLOAT`
- `is_remote`: `DataType::INT`

## 3. 驗收測試案例列表 (Acceptance Test Cases)

### ATC-01: 複雜 CSV 格式正確解析與強型態載入驗收

測試目的：驗證系統能正確讀取內含特殊轉義字元（如雙引號）的 CSV 檔案，並正確轉換為指定之強型態。

測試步驟：

1. 調用 `CSVParser::load_to_table("employees.csv", schema)`。
2. 檢查記憶體 `Table` 中 `rows` 的數量。
3. 讀取 `id = 104` 的資料列。

預期結果：

- `Table` 內成功載入 5 筆資料。
- `id = 104` 的 `name` 欄位解開引號後應精確等於 `Charlie "The King" Lin`。
- `salary` 欄位應可成功透過 `std::get<double>(row.values[idx])` 取出而非字串。

### ATC-02: Fluent API 與單條件全表掃描驗收

測試目的：驗證使用者能透過流式 C++ 語法，進行基礎的數值範圍篩選（全表掃描路徑）。

測試步驟：

```cpp
QueryBuilder query(table);
auto result = query.select({"name", "salary"})
                   .where("salary", Op::GT, 61000.0)
                   .execute();
```

預期結果：

- 查詢回傳的 `ResultSet` 筆數必須精確為 3 筆（Yu-Feng、Alice、David）。
- 每筆結果的 `Row` 物件中僅包含 2 個投影欄位（`name` 與 `salary`），其餘欄位不應出現。

### ATC-03: 複合 AND/OR 邏輯查詢驗收

測試目的：驗證多條件組合在包含 `or_where` 時，查詢引擎能正確計算邏輯優先級。

測試步驟：

```cpp
QueryBuilder query(table);
auto result = query.select({"id"})
                   .where("department", Op::EQ, std::string("Engineering"))
                   .or_where("is_remote", Op::EQ, int64_t(1))
                   .execute();
```

預期結果：

- 符合「部門是工程部」或者「允許遠端 (`is_remote=1`)」的員工。
- 預期回傳 4 筆資料（101、102、103、105），僅有 104 (HR) 被過濾掉。

### ATC-04: 字串模糊搜尋 (CONTAINS) 驗收

測試目的：驗證字串類型的欄位能正確執行子字串模糊匹配。

測試步驟：

```cpp
QueryBuilder query(table);
auto result = query.where("name", Op::CONTAINS, std::string("Huang")).execute();
```

預期結果：

- 成功回傳 1 筆資料（Yu-Feng Huang）。

### ATC-05: 雜湊索引（Hash Index）自動最佳化與正確性驗收

測試目的：驗證建立 HASH 索引後，精準查詢（EQ）的結果不變，且查詢快車道確實生效。

測試步驟：

1. 呼叫 `table.create_index("id", IndexType::HASH)`。
2. 執行查詢：

```cpp
QueryBuilder query(table);
auto result = query.where("id", Op::EQ, int64_t(102)).execute();
```

3. 檢查 `query.was_index_used()` 的狀態。

預期結果：

- 成功回傳 1 筆資料（Alice Chang）。
- `query.was_index_used()` 必須回傳 `true`，確認引擎未進行全表掃描。

## 4. 驗收通過標準 (Acceptance Criteria)

本專案若要通過最終驗收，必須滿足以下定量指標：

- 測試通過率：上述所有 ATC-01 至 ATC-05 驗收案例必須 100% 執行通過。
- 記憶體洩漏零容忍：執行完所有驗收案例後，透過 AddressSanitizer (ASan) 檢測報告中必須顯示 `0 bytes leaked`。
- 編譯警告零容忍：在 MSVC 開啟 `/W4` 或 GCC 開啟 `-Wall -Wextra` 情況下，編譯不產生任何 Warning。
