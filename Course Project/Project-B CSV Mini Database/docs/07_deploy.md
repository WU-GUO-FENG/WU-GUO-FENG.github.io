# 07_deploy.md - 建置與部署說明書 (Build & Deployment Guide)

## 1. 引言 (Introduction)
本文件詳細說明「CSV 微型資料庫與查詢引擎」的建置、編譯、測試執行與部署流程。本專案採用跨平台的 CMake 作為建置系統，並內建自動化下載 Google Test 框架之機制，開發者僅需安裝核心基礎工具即可快速上手。

---

## 2. 環境需求 (Prerequisites)
在開始建置本專案前，請確保目標系統已安裝以下環境：

- **作業系統**：Windows 10/11、Linux (Ubuntu 20.04+) 或 macOS。
- **C++ 編譯器**：必須支援 **C++17** 或以上標準。
  - Windows: Visual Studio 2019 / 2022 (MSVC)
  - Linux: GCC 9+ 或 Clang 10+
- **建置工具**：CMake (最低版本需求 `>= 3.15`)。

---

## 3. 本地建置與編譯步驟 (Build Instructions)

本專案提供兩種建置方式：**VS Code 整合介面建置**（推薦 Windows 使用者）與 **終端機命令列建置**。

### 3.1 方式 A：使用 VS Code 整合介面建置 (推薦)

1. 使用 VS Code 開啟專案根目錄 `CSV Mini Database`。
2. 確保已安裝延伸套件：`CMake Tools`。
3. 點擊 VS Code 底部狀態列的 **套件 (Kit)** 選擇對應的編譯器（如 `Visual Studio Community 2022 - amd64`）。
4. 點擊底部狀態列的 **`⚙️ 建置 (Build)`**（或按下快速鍵 `F7`），CMake 將會自動下載 Google Test 並完成編譯。

### 3.2 方式 B：使用終端機命令列建置 (跨平台)

打開終端機或 PowerShell，切換至專案根目錄，依序執行以下指令：

```bash
# 1. 建立並配置 CMake 建置輸出目錄
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 2. 執行編譯 (將自動下載 GTest 並編譯主程式與測試檔)
cmake --build build
```

編譯完成後，產生的執行檔與測試檔將存放於 `build/bin/`（Linux/macOS）或 `build/bin/Debug/`（Windows）目錄下。

## 4. 測試執行指南 (Testing Guide)

編譯成功後，強烈建議執行全面測試以驗證資料庫核心（Parser、Engine、Index）的正確性。

### 4.1 執行自動化單元測試

在 `build` 目錄下執行以下命令：

#### Windows (PowerShell)

```powershell
.\build\bin\Debug\unit_tests.exe
```

#### Linux / macOS / 跨平台 CTest 工具

```bash
cd build && ctest --output-on-failure
```

### 4.2 執行主程式範例

在終端機執行編譯出來的主執行檔：

#### Windows 系統 (PowerShell)

```powershell
.\build\bin\Debug\csv_mini_db.exe
```

#### Linux / macOS 系統

```bash
./build/bin/csv_mini_db
```

## 5. 開發者快速上手：寫你的第一支查詢程式 (Developer Quickstart)

若要在你自己的 C++ 專案中引入並使用這個微型資料庫，只需引入 `csv_db.hpp` 標頭檔即可。以下為核心使用範例：

```cpp
#include <iostream>
#include "csv_db.hpp"

int main() {
    // 1. 定義資料表的 Schema
    Schema schema;
    schema.column_names = {"id", "name", "gpa"};
    schema.name_to_index = {{"id", 0}, {"name", 1}, {"gpa", 2}};
    schema.column_types = {{"id", DataType::INT}, {"name", DataType::STRING}, {"gpa", DataType::FLOAT}};

    // 2. 透過 Parser 載入 CSV 檔案
    Table student_table = CSVParser::load_to_table("students.csv", schema);

    // 3. 效能優化：為 id 欄位建立雜湊索引 (選配)
    student_table.create_index("id", IndexType::HASH);

    // 4. 使用優雅的 Fluent API 流式呼叫建構查詢條件並執行
    QueryBuilder query(student_table);
    ResultSet results = query.select({"name", "gpa"})
                             .where("id", Op::EQ, int64_t(1))       // 走索引快車道
                             .or_where("gpa", Op::GT, 3.8)         // 殘餘線性過濾
                             .execute();

    // 5. 列印漂亮的 ASCII 結果
    results.print();

    return 0;
}
```

## 6. 連續整合與雲端部署 (CI/CD Deployment)

本專案已完整配置 GitHub Actions。當程式碼被 `git push` 至 `main` 或 `master` 分支時，雲端工作流（`.github/workflows/ci.yml`）會自動在乾淨的 Ubuntu 環境下執行以下動作：

- 簽出程式碼（Checkout Repository）。
- 安裝環境所需的 CMake 與編譯工具鏈。
- 完整執行 CMake 配置與編譯。
- 跑通所有單元測試，確保專案具備生產環境等級的穩定度。
