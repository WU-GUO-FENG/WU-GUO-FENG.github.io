# 03_SDS.md - 軟體設計規格書 (Software Design Specification)

## 1. 系統架構總覽 (System Architecture)
本系統採用分層且解耦的物件導向設計，核心模組主要分為四個部分：
1. **儲存與表示層 (Storage & Representation)**：定義記憶體中的資料表（Table）、資料列（Row）與動態型態（Value）。
2. **解析層 (Parsing Layer)**：負責將 CSV 檔案解析並載入至儲存層。
3. **查詢建構層 (Query Builder / Fluent API)**：提供 Method Chaining 介面，蒐集使用者的查詢條件。
4. **執行與優化層 (Execution & Indexing Engine)**：負責執行查詢，包含全表掃描（Table Scan）與索引尋找（Index Lookup），並回傳結果。

---

## 2. 核心資料結構設計 (Core Data Structures)

為了在 C++ 中動態處理 `INT`, `FLOAT`, `STRING` 三種型態，系統引入 `std::variant`。

### 2.1 動態數值與型態定義
```cpp
#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>

// 定義系統支援的資料型態列舉
enum class DataType {
    INT,
    FLOAT,
    STRING
};

// 使用 std::variant 實作安全的多型態數值儲存
using ValueType = std::variant<int64_t, double, std::string>;

// 定義支援的比較運算子
enum class Op {
    EQ,       // ==
    NE,       // !=
    GT,       // >
    LT,       // <
    CONTAINS  // 字串包含
};

// 定義索引型態
enum class IndexType {
    HASH,
    TREE
};

### 2.2 記憶體資料表設計 (In-Memory Table Design)
Row：代表資料庫中的一列，內部使用 `std::vector<ValueType>` 儲存實際數據。
Table：代表整張資料表，包含 Schema（欄位名稱與型態的映射）以及整體的 Row 陣列。

```cpp
class BaseIndex;

struct Schema {
    std::vector<std::string> column_names;
    std::unordered_map<std::string, size_t> name_to_index; // 欄位名對應到 vector 的索引
    std::unordered_map<std::string, DataType> column_types;
};

class Row {
public:
    std::vector<ValueType> values;
};

class Table {
public:
    std::string table_name;
    Schema schema;
    std::vector<Row> rows;

    // 索引儲存結構：欄位名稱 -> 索引物件指標
    std::unordered_map<std::string, std::shared_ptr<BaseIndex>> indexes;

    void create_index(const std::string& column_name, IndexType type);
};
```

## 3. 核心類別與 API 介面設計 (Class Interfaces)

### 3.1 CSV Parser 模組
負責檔案 I/O 與邊界條件處理。

```cpp
class CSVParser {
public:
    // 讀取 CSV 檔案並轉換為二維字串陣列
    static std::vector<std::vector<std::string>> parse(const std::string& filename);

    // 根據 Schema 將二維字串陣列載入至 Table 物件中
    static Table load_to_table(const std::string& filename, const Schema& schema);
};
```

### 3.2 Fluent API 查詢建構器 (Query & QueryBuilder)
透過 Method Chaining 蒐集條件。每次呼叫 `where` 均回傳 `QueryBuilder&` 本身。

```cpp
struct Condition {
    std::string column;
    Op op;
    ValueType value;
    bool is_or = false; // 預設為 AND，若為 true 則與前一個條件為 OR 關係
};

class QueryBuilder {
private:
    Table& source_table;
    std::vector<std::string> selected_columns;
    std::vector<Condition> conditions;

public:
    QueryBuilder(Table& table) : source_table(table) {}

    QueryBuilder& select(const std::vector<std::string>& columns) {
        selected_columns = columns;
        return *this;
    }

    QueryBuilder& where(const std::string& column, Op op, ValueType val) {
        conditions.push_back({column, op, val, false});
        return *this;
    }

    QueryBuilder& or_where(const std::string& column, Op op, ValueType val) {
        conditions.push_back({column, op, val, true});
        return *this;
    }

    // 觸發查詢引擎執行
    ResultSet execute();
};
```

### 3.3 查詢結果集 (ResultSet)
儲存篩選後的結果，並提供迭代器支援與格式化輸出。

```cpp
class ResultSet {
private:
    std::vector<std::string> columns;
    std::vector<Row> result_rows;

public:
    ResultSet(std::vector<std::string> cols, std::vector<Row> r)
        : columns(cols), result_rows(r) {}

    // 迭代器支援，供 for (const auto& row : result) 使用
    auto begin() const { return result_rows.begin(); }
    auto end() const { return result_rows.end(); }

    // 以 ASCII 表格形式列印結果至控制台
    void print() const;
};
```

## 4. 索引機制設計 (Indexing Mechanism)
系統設計一個基底類別 `BaseIndex`，並衍生出 `HashIndex` 與 `TreeIndex`。索引儲存的是「數值」到「該資料列在 `Table.rows` 中的智慧指標或整數索引（Row ID）」的映射。

```cpp
class BaseIndex {
public:
    virtual ~BaseIndex() = default;
    virtual void insert(const ValueType& value, size_t row_id) = 0;
    virtual std::vector<size_t> lookup_equal(const ValueType& value) = 0;
    virtual std::vector<size_t> lookup_range(const ValueType& low, const ValueType& high) = 0;
};

// 雜湊索引：用於 O(1) 等值查詢
class HashIndex : public BaseIndex {
private:
    std::unordered_map<ValueType, std::vector<size_t>, VariantHash> index_map;
public:
    void insert(const ValueType& value, size_t row_id) override;
    std::vector<size_t> lookup_equal(const ValueType& value) override;
    std::vector<size_t> lookup_range(const ValueType& low, const ValueType& high) override {
        // HASH 不支援範圍查詢，拋出異常或回傳空
        return {};
    }
};

// 樹狀索引：用於 O(log N) 範圍查詢
class TreeIndex : public BaseIndex {
private:
    std::map<ValueType, std::vector<size_t>> index_map;
public:
    void insert(const ValueType& value, size_t row_id) override;
    std::vector<size_t> lookup_equal(const ValueType& value) override;
    std::vector<size_t> lookup_range(const ValueType& low, const ValueType& high) override;
};
```

註：由於 `ValueType` 是 `std::variant`，作為 `std::unordered_map` 的 Key 時需要自訂 `VariantHash` 雜湊函式。

## 5. 查詢執行流程與優化策略
當 `QueryBuilder::execute()` 被呼叫時，核心查詢引擎（Query Engine）將採取以下步驟優化與執行：

```text
[呼叫 execute()]
       │
       ▼
[檢查第一個關鍵條件的欄位是否擁有索引？]
       ├──> 有索引 ──> [調用 Index Lookup] ──> 僅取出特定 Row IDs ──> [執行剩餘條件過濾]
       └──> 無索引 ──> [執行 Table Scan]   ──> 遍歷所有 Table Rows   ──> [執行完整條件過濾]
                                                                            │
                                                                            ▼
                                                                     [建構 ResultSet 並回傳]
```

- 條件檢查（Index Optimization Check）：引擎優先檢查 `conditions` 陣列中的欄位。如果某個 `Op::EQ` 條件的欄位上建有 `HashIndex`，或者 `Op::GT`/`Op::LT` 條件的欄位上建有 `TreeIndex`，則優先啟用該索引。
- 索引檢索（Index Lookup）：透過索引直接取得符合該條件的 `row_id` 集合，將搜尋範圍從 `N` 縮小至 `M`。
- 殘餘條件過濾（Residual Filtering）：對於索引篩選出來的少數資料列，再進行其他未被索引覆蓋的條件（如 `AND` 的其餘條件）進行線性檢查。
- 型態安全比較機制：在比對 `ValueType` 時，系統會透過 `std::holds_alternative` 或 `std::visit` 確保比較雙方的底層型態一致（例如 `int64_t` 與 `int64_t` 相比）。若型態不符，則進行安全的轉型或視為不匹配，避免程式崩潰。
