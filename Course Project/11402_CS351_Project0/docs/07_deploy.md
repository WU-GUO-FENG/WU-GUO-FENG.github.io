# Project 0 部署與環境配置報告 (Deployment Guide)

本報告記錄了 `TwoSum` 專案的容器化（Containerization）封裝與部署流程。透過 Docker 技術，將 C++20 編譯環境、vcpkg 套件管理器以及 Google Test 框架完整打包，實現跨平台「一鍵建置與測試」的自動化部署。

---

## 1. 部署策略與架構 (Deployment Strategy)

本專案採用 **多階段建置 (Multi-stage Build)** 的 Docker 策略，以優化最後產出的容器映像檔（Image）大小，並確保開發環境與執行環境的分離：
1. **Build 階段**：基於 Ubuntu 基礎映像檔，安裝 GCC-13 編譯器、CMake、Git，並透過 vcpkg 下載與編譯 Google Test。
2. **Run/Test 階段**：僅複製編譯完成的執行檔（`twosum_app` 與 `twosum_test`）至乾淨的執行環境中運行，確保安全與輕量化。

---

## 2. Dockerfile 環境配置明細

專案根目錄下的 `Dockerfile` 關鍵配置如下（完整代碼參見原始碼倉庫）：
* **基礎作業系統**: `ubuntu:24.04` (支援原生 GCC-13，完美符合 C++20 標準)
* **依賴工具**: `cmake`, `make`, `g++`, `git`, `curl`, `zip`, `unzip`, `tar`
* **套件管理**: `vcpkg` (全域整合，自動導引工具鏈)

---

## 3. 本地部署與執行步驟 (Deployment Steps)

請確保部署目標機器已安裝 **Docker Desktop** 或 Docker 引擎，並於專案根目錄執行以下指令：

### 步驟 1：建置 Docker 映像檔
執行以下命令來下載依賴並編譯專案，這會自動跑完 CMake 與 GTest 的安裝：
```bash
docker build -t twosum-project:v1.0 .