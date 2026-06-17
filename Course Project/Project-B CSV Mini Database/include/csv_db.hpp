#ifndef CSV_DB_HPP
#define CSV_DB_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <variant>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// ==========================================
// 1. 基礎型態與列舉定義
// ==========================================
enum class DataType { INT, FLOAT, STRING };
enum class Op { EQ, NE, GT, LT, CONTAINS };
enum class IndexType { HASH, TREE };

using ValueType = std::variant<int64_t, double, std::string>;

struct VariantHash {
    std::size_t operator()(const ValueType& v) const {
        return std::visit([](const auto& arg) -> std::size_t {
            return std::hash<std::decay_t<decltype(arg)>>{}(arg);
        }, v);
    }
};

struct Schema {
    std::vector<std::string> column_names;
    std::unordered_map<std::string, size_t> name_to_index;
    std::unordered_map<std::string, DataType> column_types;
};

class Row {
public:
    std::vector<ValueType> values;
};

// ==========================================
// 2. 索引機制實作 (M5 新增)
// ==========================================
class BaseIndex {
public:
    virtual ~BaseIndex() = default;
    virtual void insert(const ValueType& value, size_t row_id) = 0;
    virtual std::vector<size_t> lookup_equal(const ValueType& value) = 0;
};

class HashIndex : public BaseIndex {
private:
    std::unordered_map<ValueType, std::vector<size_t>, VariantHash> index_map;
public:
    void insert(const ValueType& value, size_t row_id) override {
        index_map[value].push_back(row_id);
    }
    std::vector<size_t> lookup_equal(const ValueType& value) override {
        auto it = index_map.find(value);
        if (it != index_map.end()) {
            return it->second;
        }
        return {}; // 沒找到，回傳空陣列
    }
};

// ==========================================
// 3. Table 資料表類別 (M5 整合索引)
// ==========================================
class Table {
public:
    std::string table_name;
    Schema schema;
    std::vector<Row> rows;
    std::unordered_map<std::string, std::shared_ptr<BaseIndex>> indexes;

    void append_row(const std::vector<std::string>& raw_fields) {
        Row row;
        for (size_t i = 0; i < schema.column_names.size(); ++i) {
            if (i >= raw_fields.size()) {
                row.values.push_back("");
                continue;
            }
            std::string col_name = schema.column_names[i];
            DataType type = schema.column_types[col_name];
            std::string raw_val = raw_fields[i];
            try {
                if (type == DataType::INT) row.values.push_back(std::stoll(raw_val));
                else if (type == DataType::FLOAT) row.values.push_back(std::stod(raw_val));
                else row.values.push_back(raw_val);
            } catch (...) {
                if (type == DataType::INT) row.values.push_back(int64_t(0));
                else if (type == DataType::FLOAT) row.values.push_back(0.0);
                else row.values.push_back("");
            }
        }
        
        // 先把資料塞進 rows
        rows.push_back(row);
        size_t new_row_id = rows.size() - 1;

        for (auto& [col_name, index_ptr] : indexes) {
            size_t col_idx = schema.name_to_index[col_name];
            index_ptr->insert(row.values[col_idx], new_row_id);
        }
    }

    // 建立索引的工具函式
    void create_index(const std::string& column_name, IndexType type) {
        if (schema.name_to_index.find(column_name) == schema.name_to_index.end()) return;
        
        std::shared_ptr<BaseIndex> index_ptr;
        if (type == IndexType::HASH) {
            index_ptr = std::make_shared<HashIndex>();
        }
        
        size_t col_idx = schema.name_to_index[column_name];
        for (size_t i = 0; i < rows.size(); ++i) {
            index_ptr->insert(rows[i].values[col_idx], i);
        }
        
        indexes[column_name] = index_ptr;
    }
};

struct Condition {
    std::string column;
    Op op;
    ValueType value;
    bool is_or = false;
};

class ResultSet {
private:
    std::vector<std::string> columns;
    std::vector<Row> result_rows;
public:
    ResultSet(std::vector<std::string> cols, std::vector<Row> r) 
        : columns(cols), result_rows(r) {}

    auto begin() const { return result_rows.begin(); }
    auto end() const { return result_rows.end(); }
    size_t size() const { return result_rows.size(); }

    void print() const {
        for (const auto& col : columns) std::cout << col << "\t";
        std::cout << "\n-----------------------------------\n";
        for (const auto& row : result_rows) {
            for (const auto& val : row.values) {
                std::visit([](const auto& arg) { std::cout << arg << "\t"; }, val);
            }
            std::cout << "\n";
        }
    }
};

inline bool evaluate_condition(const ValueType& cell, Op op, const ValueType& target) {
    return std::visit([op](const auto& v1, const auto& v2) -> bool {
        using T1 = std::decay_t<decltype(v1)>;
        using T2 = std::decay_t<decltype(v2)>;
        if constexpr (std::is_same_v<T1, T2>) {
            switch (op) {
                case Op::EQ: return v1 == v2;
                case Op::NE: return v1 != v2;
                case Op::GT: return v1 > v2;
                case Op::LT: return v1 < v2;
                case Op::CONTAINS: {
                    if constexpr (std::is_same_v<T1, std::string>) return v1.find(v2) != std::string::npos;
                    return false;
                }
            }
        } else if constexpr ((std::is_same_v<T1, int64_t> && std::is_same_v<T2, double>) ||
                           (std::is_same_v<T1, double> && std::is_same_v<T2, int64_t>)) {
            double d1 = static_cast<double>(v1);
            double d2 = static_cast<double>(v2);
            switch (op) {
                case Op::EQ: return d1 == d2;
                case Op::NE: return d1 != d2;
                case Op::GT: return d1 > d2;
                case Op::LT: return d1 < d2;
                default: return false;
            }
        }
        return false;
    }, cell, target);
}

// ==========================================
// 4. QueryBuilder 最佳化優化引擎 (M5 升級)
// ==========================================
class QueryBuilder {
private:
    Table& source_table;
    std::vector<std::string> selected_columns;
    std::vector<Condition> conditions;
    bool used_index_lookup = false; // 用於內部效能追蹤

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

    bool was_index_used() const { return used_index_lookup; }

    ResultSet execute() {
        std::vector<Row> matched_rows;
        if (selected_columns.empty()) {
            selected_columns = source_table.schema.column_names;
        }

        std::vector<size_t> row_ids_to_check;
        bool index_applied = false;

        // 最佳化策略：檢查第一個條件是否可以走 HashIndex 快速通道
        if (!conditions.empty() && conditions[0].op == Op::EQ && !conditions[0].is_or) {
            std::string target_col = conditions[0].column;
            if (source_table.indexes.find(target_col) != source_table.indexes.end()) {
                // 發現索引！調用 Index Lookup，只抓出符合條件的特定的 Row IDs
                row_ids_to_check = source_table.indexes[target_col]->lookup_equal(conditions[0].value);
                index_applied = true;
                used_index_lookup = true;
            }
        }

        if (!index_applied) {
            row_ids_to_check.resize(source_table.rows.size());
            for (size_t i = 0; i < source_table.rows.size(); ++i) row_ids_to_check[i] = i;
        }

        // 開始過濾篩選
        for (size_t row_id : row_ids_to_check) {
            const auto& row = source_table.rows[row_id];
            bool row_match = true;

            if (!conditions.empty()) {
                size_t start_cond_idx = index_applied ? 1 : 0;
                
                if (!index_applied) {
                    size_t col_idx = source_table.schema.name_to_index[conditions[0].column];
                    row_match = evaluate_condition(row.values[col_idx], conditions[0].op, conditions[0].value);
                }

                // 處理殘餘條件過濾
                for (size_t i = start_cond_idx; i < conditions.size(); ++i) {
                    const auto& cond = conditions[i];
                    size_t c_idx = source_table.schema.name_to_index[cond.column];
                    bool current_cond_res = evaluate_condition(row.values[c_idx], cond.op, cond.value);

                    if (cond.is_or) {
                        row_match = row_match || current_cond_res;
                    } else {
                        row_match = row_match && current_cond_res;
                    }
                }
            }

            if (row_match) {
                Row projected_row;
                for (const auto& col_name : selected_columns) {
                    size_t idx = source_table.schema.name_to_index[col_name];
                    projected_row.values.push_back(row.values[idx]);
                }
                matched_rows.push_back(projected_row);
            }
        }

        return ResultSet(selected_columns, matched_rows);
    }
};

// ==========================================
// 5. CSV Parser
// ==========================================
class CSVParser {
public:
    static std::vector<std::vector<std::string>> parse(const std::string& filename) {
        std::vector<std::vector<std::string>> records;
        std::ifstream file(filename);
        if (!file.is_open()) return records;
        std::vector<std::string> current_row;
        std::string current_field;
        enum class State { Unquoted, Quoted, Escape };
        State state = State::Unquoted;
        char ch;
        while (file.get(ch)) {
            switch (state) {
                case State::Unquoted:
                    if (ch == ',') { current_row.push_back(current_field); current_field.clear(); }
                    else if (ch == '\n') { current_row.push_back(current_field); records.push_back(current_row); current_field.clear(); current_row.clear(); }
                    else if (ch == '\r') {}
                    else if (ch == '"') state = State::Quoted;
                    else current_field += ch;
                    break;
                case State::Quoted:
                    if (ch == '"') state = State::Escape;
                    else current_field += ch;
                    break;
                case State::Escape:
                    if (ch == '"') { current_field += '"'; state = State::Quoted; }
                    else if (ch == ',') { current_row.push_back(current_field); current_field.clear(); state = State::Unquoted; }
                    else if (ch == '\n') { current_row.push_back(current_field); records.push_back(current_row); current_field.clear(); current_row.clear(); state = State::Unquoted; }
                    else if (ch == '\r') {}
                    else { current_field += ch; state = State::Unquoted; }
                    break;
            }
        }
        if (!current_field.empty() || !current_row.empty()) { current_row.push_back(current_field); records.push_back(current_row); }
        return records;
    }

    static Table load_to_table(const std::string& filename, const Schema& schema) {
        Table table;
        table.schema = schema;
        auto raw_data = parse(filename);
        if (raw_data.empty()) return table;
        for (size_t i = 1; i < raw_data.size(); ++i) {
            table.append_row(raw_data[i]);
        }
        return table;
    }
};

#endif // CSV_DB_HPP