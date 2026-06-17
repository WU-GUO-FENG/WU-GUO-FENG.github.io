#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "csv_db.hpp"

// 輔助函式：動態產生測試用的 employees.csv
void generate_mock_csv() {
    std::ofstream file("employees.csv");
    if (file.is_open()) {
        file << "id,name,department,salary,is_remote\n";
        file << "101,Yu-Feng Huang,Engineering,85000.50,1\n";
        file << "102,Alice Chang,Engineering,92000.00,0\n";
        file << "103,Bob Jones,Marketing,60000.25,1\n";
        file << "104,\"Charlie \"\"The King\"\" Lin\",HR,55000.00,0\n";
        file << "105,David Wang,Marketing,62000.00,1\n";
        file.close();
    }
}

int main() {
    std::cout << "=== CSV Mini Database Engine ===" << std::endl;
    
    // 1. 動態產生測試資料
    generate_mock_csv();
    std::cout << "[INFO] Generated 'employees.csv' successfully.\n\n";
    
    // 2. 定義符合 M3 規格的強型態 Schema
    Schema schema;
    schema.column_names = {"id", "name", "department", "salary", "is_remote"};
    schema.name_to_index = {{"id", 0}, {"name", 1}, {"department", 2}, {"salary", 3}, {"is_remote", 4}};
    schema.column_types = {
        {"id", DataType::INT},
        {"name", DataType::STRING},
        {"department", DataType::STRING},
        {"salary", DataType::FLOAT},
        {"is_remote", DataType::INT}
    };
    
    // 3. 載入 CSV 檔案至強型態 Table 中
    std::cout << "[INFO] Loading 'employees.csv' into Table structure...\n";
    Table employee_table = CSVParser::load_to_table("employees.csv", schema);
    std::cout << "Total records loaded (excluding header): " << employee_table.rows.size() << "\n\n";
    
    // 4. 建立雜湊索引 (M5 最佳化)
    std::cout << "[INFO] Creating HASH Index on 'id' column...\n\n";
    employee_table.create_index("id", IndexType::HASH);
    
    // 5. 執行 Fluent API 複合查詢 (M4 & M5)
    // 情境：找出「部門是工程部」或者「允許遠端(is_remote=1)」的員工，並投影出 id, name 與 salary
    std::cout << "[Query] Executing Fluent API Query...\n";
    std::cout << ">> SELECT id, name, salary FROM employees WHERE department == 'Engineering' OR is_remote == 1\n";
    
    QueryBuilder query(employee_table);
    ResultSet results = query.select({"id", "name", "salary"})
                             .where("department", Op::EQ, std::string("Engineering"))
                             .or_where("is_remote", Op::EQ, int64_t(1))
                             .execute();
                             
    // 6. 印出查詢結果
    std::cout << "\n[Results] Matched Rows:\n";
    results.print();
    
    // 檢查優化引擎有沒有觸發（此處因第一個條件是 department 沒索引，會走高效能 Fallback 全表掃描）
    std::cout << "\nOptimization Track: " 
              << (query.was_index_used() ? "FAST_TRACK (Index Lookup)" : "LINEAR_TRACK (Table Scan)") 
              << std::endl;

    return 0;
}