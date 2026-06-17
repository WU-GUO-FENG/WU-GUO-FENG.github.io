cmake_minimum_required(VERSION 3.15)
project(CSVMiniDB VERSION 1.0.0 LANGUAGES CXX)

# 強制要求 C++17 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === 加入這段：解決 Windows MSVC 繁體中文編碼警告 ===
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 編譯輸出路徑設定
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 包含標頭檔路徑
include_directories(include)

# 自動下載並設定 Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
# 排除安裝規則以加速建置
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# -----------------------------------------------------------------------------
# 核心程式碼目標 (Library / Executable)
# -----------------------------------------------------------------------------
# 這裡將實作程式碼包成一個名為 csv_db_lib 的靜態庫，方便主程式與測試端同時連結
add_library(csv_db_lib
    include/csv_db.hpp
    src/csv_db.cpp
    # 未來實作的 .cpp 檔案請陸續加在下方
)

# 終端機主程式執行檔
add_executable(csv_mini_db src/main.cpp)
target_link_libraries(csv_mini_db PRIVATE csv_db_lib)

# -----------------------------------------------------------------------------
# 單元測試目標 (Unit Tests)
# -----------------------------------------------------------------------------
enable_testing()
add_executable(unit_tests tests/test_main.cpp)
target_link_libraries(unit_tests PRIVATE csv_db_lib gtest_main)

# 讓 CMake 能夠識別 Google Test
include(GoogleTest)
gtest_discover_tests(unit_tests)