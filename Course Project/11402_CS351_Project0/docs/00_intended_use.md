00_intended_use.md# Intended Use (預期用途)

## 1. Problem Statement (問題陳述)

### What problem does this tool solve?
本專案解決經典的「兩數之和 (Two Sum)」演算法問題：在一個整數陣列中尋找兩個數字，使其總和等於特定目標值 (Target)，並回傳其索引值。本工具旨在展示同一問題的兩種不同解法（暴力法與哈希表法）在時間與空間複雜度上的差異。

### Who is the user?
- **學生/開發者**： 用於練習 AI 輔助開發流程、單元測試撰寫及 CI/CD 流程設定。
- **面試準備者**： 理解基礎演算法優化邏輯。

## 2. Intended Use (預期用途)

### The tool is intended to:
- 提供兩個功能函數：`TwoSumArray` (Brute Force) 與 `TwoSumHashTable` (Optimized)。
- 透過單元測試驗證演算法在各種邊界條件（如負數、重複數字）下的正確性。
- 作為 DevOps 練習，透過 GitHub Actions 與 Docker 實現自動化測試與環境隔離。

### The tool is NOT intended to:
- 處理極端大數據（超出內存限制的陣列）。
- 提供圖形化使用者介面 (GUI)，本專案僅限於 CLI 與程式庫形式。
- 處理多種解答的情況（本專案假設輸入僅存在唯一解）。

## 3. Inputs / Outputs (輸入與輸出)

### Input(s):
- `std::vector<int> nums`: 整數陣列 (長度範圍 2 至 10,000)。
- `int target`: 目標總和 (支援正負整數)。

### Output(s):
- `std::vector<int>`: 包含兩個索引值的陣列，例如 `[0, 1]`。

### Execution environment:
- **編譯環境**：支持 C++23 的編譯器（如 GCC 13+ 或 Clang 16+）。
- **運行環境**：Linux/macOS/Windows (via Docker) 命令行介面 (CLI)。

## 4. Success Criteria (成功標準)

### Functional success:
- 兩個演算法均能精確回傳正確的索引值。
- **異常處理**：若輸入不符合「唯一解」假設（如無解），函數應回傳空向量 `std::vector<int>{}` 並記錄日誌。

### Quality success:
- GitHub Actions CI 狀態為 Green（所有測試通過）。
- Docker Image 能成功構建並執行內部測試。
- 程式碼符合 C++23 標準規範，無記憶體洩漏。

## 5. Constraints (限制條件)

### Language:
- C++23 (使用標準模板庫 STL)。

### Time/Scope constraints:
- 需在 Bootcamp 規定期限內完成開發、測試與容器化部署。

### External dependency policy:
- 僅限使用 C++ 標準庫與 Google Test (或類似單元測試框架)，不使用第三方演算法庫。

## 6. Risks / Assumptions (風險與假設)

### Key assumptions:
- 輸入陣列中絕對存在唯一解。
- 同一個元素不能重複使用兩次。

### Main risks:
- **效能風險**： `TwoSumArray` 在陣列極大時會導致執行時間過長 ($O(n^2)$)。
- **記憶體風險**： `TwoSumHashTable` 在空間受限環境下可能消耗過多記憶體 ($O(n)$)。

### Mitigations:
- 透過時間複雜度分析與單元測試監控不同規模資料的執行表現。