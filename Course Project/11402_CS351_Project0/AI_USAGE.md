# AI 輔助開發具體紀錄 (AI Usage Log)

本專案在開發過程中，部分環節採用了 AI 協作工具進行效率提升，以下為具體使用紀錄明細：

## 1. 工具與模型 (Tools & Models)
* **使用工具**：Google Gemini / ChatGPT
* **使用時間**：2026年5月 ~ 2026年6月

## 2. 具體協作項目與成效 (Specific Task & Outcomes)

### 項目 A：C++20 雜湊表語法諮詢與優化
* **AI 協助內容**：詢問如何在 C++20 中以最優雅且具備 $O(1)$ 平均複雜度的方式檢查 `std::unordered_map` 是否包含特定 Key。
* **最終採納**：AI 建議使用 C++20 新增的 `.contains()` 特性，取代舊版的 `.find() != .end()`，提升了代碼可讀性。

### 項目 B：Docker 多階段建置 (Multi-stage Build) 除錯
* **AI 協助內容**：本地端 Windows Build 正常，但在 Docker 容器化過程中出現 `Could NOT find GTest` 錯誤。
* **最終採納**：AI 準確識別出 Linux 容器內缺乏 `gtest` 依賴套件，引導在 `Dockerfile` 中加入 `RUN vcpkg install gtest` 指令，並引入 `.dockerignore` 排除 Windows 殘留的 `build/` 快取，成功打通環境。

### 項目 C：文件結構草擬
* **AI 協助內容**：輔助建立 `05_acceptance_tests.md` 到 `08_known_issues.md` 的標準軟體工程文件框架。