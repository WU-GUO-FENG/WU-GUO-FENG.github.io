cmake_minimum_required(VERSION 3.20)
project(TwoSumProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 讓 CMake 去系統中尋找由 vcpkg 安裝的 GTest
find_package(GTest REQUIRED)

include_directories(include)

# 建立測試執行檔
add_executable(twosum_test test/test_twosum.cpp src/twosum.cpp)

# 連結 GTest 庫
target_link_libraries(twosum_test GTest::gtest_main)