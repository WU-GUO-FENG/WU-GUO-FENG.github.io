# Project 0 - Two Sum開發專案


本專案為 **元智大學 CS351 人工智慧輔助軟體開發** 開放之 Bootcamp 專案。透過經典的 LeetCode #1 Two Sum 問題，完整導入現代化 C++20 開發流、Google Test 自動化測試、vcpkg 依賴管理，以及 Docker 容器化部署與 GitHub Actions CI/CD 流程。

---

## 🚀 專案核心特點 (Key Features)

* **雙軌演算法實作**：包含 $O(n^2)$ 暴力法 (`TwoSumArray`) 與 $O(n)$ 雜湊表法 (`TwoSumHashTable`)。
* **現代 C++20 特性**：雜湊表法利用 C++20 標準之 `.contains()` 進行優雅且高效的鍵值查找。
* **自動化單元測試**：整合 **Google Test (GTest)** 框架，全面覆蓋基礎案例、負數、重複值、空值等邊界條件。
* **資料驅動測試 (Data-Driven)**：支援自外部 `test/data.txt` 讀取萬筆大數據，並設有 **10ms 硬性效能限制** 驗證。
* **DevOps 容器化架構**：撰寫高階 Docker 多階段建置 (`Multi-stage Build`)，大幅縮減生產環境映像檔體積。
* **持續整合 (CI/CD)**：配置 GitHub Actions，在每次 Push 與 Pull Request 時自動於雲端 Linux 環境執行建置與測試。

---

## 📁 專案目錄結構 (Directory Structure)

```text
11402_CS351_Project0/
├── .github/workflows/
│   └── ci.yml                     # GitHub Actions 自動化測試腳本
├── include/
│   └── twosum.hpp                 # 核心演算法介面宣告 (Interface)
├── src/
│   ├── main.cpp                   # 應用程式入口主程式
│   └── twosum.cpp                 # 雙軌演算法具體實作 (Implementation)
├── test/
│   ├── data.txt                   # 萬筆效能基準測試數據 (10,000 items)
│   └── test_twosum.cpp            # Google Test 測試案例集 (Verification)
├── docs/                          # 完整軟體工程開發文件 (00_~08_)
├── Dockerfile                     # 跨平台多階段建置封裝腳本
├── .dockerignore                  # 容器打包過濾快取配置
└── CMakeLists.txt                 # CMake 專案建置腳本