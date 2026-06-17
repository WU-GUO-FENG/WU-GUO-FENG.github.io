#include <gtest/gtest.h>
#include <fstream>
#include "csv_db.hpp"

void create_test_csv(const std::string& filename) {
    std::ofstream file(filename);
    file << "id,name,gpa\n";
    file << "1,Yu-Feng Huang,3.9\n";
    file << "2,Bob Jones,3.2\n";
    file << "3,Alice Chang,3.7\n";
    file.close();
}

TEST(CSVParserTest, RobustParsingCheck) {
    std::string test_file = "test_data.csv";
    create_test_csv(test_file);
    auto result = CSVParser::parse(test_file);
    ASSERT_EQ(result.size(), 4);
    std::remove(test_file.c_str());
}

// M5 核心測試：驗證資料庫索引優化機制
TEST(QueryIndexTest, HashIndexOptimizationCheck) {
    std::string test_file = "test_index_data.csv";
    create_test_csv(test_file);

    Schema schema;
    schema.column_names = {"id", "name", "gpa"};
    schema.name_to_index = {{"id", 0}, {"name", 1}, {"gpa", 2}};
    schema.column_types = {{"id", DataType::INT}, {"name", DataType::STRING}, {"gpa", DataType::FLOAT}};

    Table table = CSVParser::load_to_table(test_file, schema);

    // 1. 未建立索引前進行查詢
    QueryBuilder q_before(table);
    ResultSet res_before = q_before.select({"name"}).where("id", Op::EQ, int64_t(3)).execute();
    EXPECT_EQ(res_before.size(), 1);
    EXPECT_FALSE(q_before.was_index_used()); // 預期：此時不應該用到索引

    // 2. 為 "id" 欄位建立雜湊索引
    table.create_index("id", IndexType::HASH);

    // 3. 建立索引後再度查詢同樣條件
    QueryBuilder q_after(table);
    ResultSet res_after = q_after.select({"name"}).where("id", Op::EQ, int64_t(3)).execute();
    
    // 驗證結果正確性（依然能查到 Alice）
    EXPECT_EQ(res_after.size(), 1);
    
    // 關鍵驗證：查詢引擎必須聰明地自動啟用了索引查找！
    EXPECT_TRUE(q_after.was_index_used()); 

    std::remove(test_file.c_str());
}