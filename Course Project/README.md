# 📚 課程專案集合 (Course Projects Collection)

這個資料夾存放了我在元智大學資訊工程系各項課程中的專案作品、核心作業與實踐成果。透過版本控制與工業級規範，落實軟體工程生命週期（SDLC）。

## 🔗 專案清單 (Project List)

| 專案名稱 | 技術棧 | 說明 |
| :--- | :--- | :--- |
| **個人作品集網站** | `HTML5`, `CSS3`, `JavaScript` | 響應式前端個人網站，採用極簡卡片式佈局，完整呈現個人特質、技術棧與經歷。 |
| **Project B-CSV 微型資料庫與查詢引擎** | `C++17`, `CMake`, `Google Test` | 純手寫 FSM Parser、強型態記憶體 Table 與 Fluent API 引擎，支援 HASH 索引自動最佳化。|
| **Project 0 - Two Sum 自動化建置與測試** | `C++20`, `CMake`, `Google Test`, `Docker`, `GitHub Actions` | 實作雙軌演算法（暴力法與雜湊表法），利用 C++20 `.contains()` 優化。整合 GTest 與大數據壓測（通過 <10ms 限制），並配置 Docker 多階段建置與 CI 工作流。 |
---

## 🛠️ 技術棧總覽 (Technology Stack)

本儲存庫核心涉及的技術比例與應用場景：

- **C++ ** - 核心資料結構、有限狀態機（FSM）解析器與資料庫查詢引擎實現。
- **HTML / CSS / JS ** - 個人品牌作品集網站、動態標籤頁交互介面與響應式排版。
- **CMake ** - 跨平台專案構建管理、第三方框架（Google Test）自動化整合。
- **DevOps & CI/CD ** - GitHub Actions 雲端自動化建置與測試流水線。

---

## 💻 快速環境配置與建置 (Setup & Build)

### 前置需求 (Prerequisites)
- **C++ 編譯器**：支援 C++17 或以上標準（GCC 9+、Clang 10+ 或 MSVC 2019+）
- **建置工具**：CMake 3.15+

### 構建與執行步驟 (C++ 專案專用)
打開終端機，切換至本儲存庫根目錄，依序執行以下指令：

```bash
# 1. 克隆儲存庫
git clone [https://github.com/WU-GUO-FENG/WU-GUO-FENG.github.io.git](https://github.com/WU-GUO-FENG/WU-GUO-FENG.github.io.git)
cd WU-GUO-FENG.github.io

# 2. 配置 CMake 構建目錄 (在根目錄下執行，避免子目錄路徑錯亂)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 3. 執行編譯 (將自動建置主程式與測試集)
cmake --build build
