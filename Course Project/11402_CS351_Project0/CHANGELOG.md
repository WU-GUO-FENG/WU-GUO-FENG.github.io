# 變更日誌 (Changelog)

本專案的所有重大變更皆會記錄於此。本專案遵循 [Semantic Versioning](https://semver.org/lang/zh-TW/) 規範。

## [1.0.0] - 2026-06-16
### Added (新增)
* 新增 `Dockerfile` 支援多階段建置 (Multi-stage Build)，完美隔離編譯與執行環境。
* 新增 `.dockerignore` 檔案，自動過濾 Windows 本地端快取，優化容器打包速度。
* 新增 `docs/07_deploy.md` 容器化部署指南與 `docs/08_known_issues.md` 系統限制報告。

### Changed (修改)
* 修正標頭檔引用路徑，將 `#include "include/twosum.hpp"` 統一優化為 `#include "twosum.hpp"`，解決 CMake 尋找路徑重疊衝突。

## [0.2.0] - 2026-05-28
### Added (新增)
* 整合 **Google Test (GTest)** 測試框架，實作功能性測試與萬筆大數據性能測試。
* 核心雜湊表算法實作成功，萬筆資料耗時 6ms，通過 10ms 效能限制。
* 撰寫並發布 `docs/05_acceptance_tests.md` 與 `docs/06_traceability.md`。

## [0.1.0] - 2026-03-15
### Added (新增)
* 專案初始化，建立基本目錄結構 (`src/`, `include/`, `test/`, `docs/`)。
* 設定 `CMakeLists.txt`，導入 C++20 標準與 Windows 本地端 vcpkg 套件管理機制。
* 實作 `TwoSumArray` 暴力法（$O(n^2)$）基本邏輯。