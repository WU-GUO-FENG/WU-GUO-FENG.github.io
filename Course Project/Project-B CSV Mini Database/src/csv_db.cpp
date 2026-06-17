#include "csv_db.hpp"

// 核心邏輯（Table, QueryBuilder, CSVParser 等）都採用 Header-only 的形式實作在 include/csv_db.hpp 中。
//給 CMake 一個實體 .cpp 檔案，讓編譯器知道這是一個標準的 C++ 專案，從而順利編譯。 