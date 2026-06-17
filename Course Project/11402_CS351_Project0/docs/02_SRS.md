# Software Requirements Specification (SRS)

## 1. 引言

### 1.1 目的
本文件描述一個 Two Sum 演算法專案的軟體需求，用以支持學生/開發者練習演算法實作、單元測試與 CI/CD。目標是設計兩種解法：暴力法 `TwoSumArray` 和優化法 `TwoSumHashTable`，提供可靠的輸入檢查、錯誤處理與輸出格式。

### 1.2 範圍
系統接受整數向量 `nums` 及整數目標 `target`，輸出一個含兩個索引的向量。假設輸入中恆有且只有一組解；若無解則返回空向量或拋出例外（由請求方決定）。系統不處理超大資料到無法載入記憶體的情況。

### 1.3 定義、縮寫與術語
- Two Sum：在整數陣列中尋找兩個元素，使其和等於目標值。
- `nums`：輸入整數陣列。
- `target`：整數目標值。
- `index`：答案中返回的元素位置（0-based）。
- API：應用程式介面。

## 2. 整體描述

### 2.1 產品透視
兩個功能函式可被 CLI 或其他 C++ 應用呼叫；本專案亦含單元測試與 CI/CD 流程。設計前端由文件構成，後端由 C++ 函式實作。

### 2.2 產品功能
1. TwoSumArray:
   - 從 `nums` 兩層迴圈遍歷。
   - 若 `nums[i] + nums[j] == target`，回傳 `{i, j}`。
   - 如果找不到，回傳空向量或拋出 `std::runtime_error("No two sum solution")`。

2. TwoSumHashTable:
   - 使用 `std::unordered_map<int,int>` 儲存已遍歷元素及其索引。
   - 對每個 `nums[i]` 計算 `complement = target - nums[i]`，檢查 `map` 是否含 `complement`。
   - 找到則回傳 `{map[complement], i}`。
   - 不 found 則回傳空向量或拋例。

3. 輸入驗證:
   - `nums` 長度 < 2 時，立即返回空向量或拋例。
   - 支援負數、零、正數混合。

### 2.3 使用者特性
- 學生/開發者：需要正確結果與易讀實作，同時具測試驗證。
- CI 工程師：需確保自動測試綠燈。

### 2.4 限制
- C++23（標準庫、GCC 13+/Clang 16+）
- 假設存在唯一解（單一解）。
- 在「資料超出記憶體」情況下不保證可用。

### 2.5 一般假設與依賴
- 輸入資料合法（整數、vector 結構）。
- 單元測試使用 Google Test。
- CI 使用 GitHub Actions、Docker 環境執行。

## 3. 特定需求

### 3.1 功能需求
- FR1：`TwoSumArray(nums, target)` 必須在存在解時返回兩個正確索引。
- FR2：`TwoSumHashTable(nums, target)` 回傳一致正確索引且時間複雜度 O(n)。
- FR3：若無解或輸入不合法，應回傳空向量 std::vector<int>{}
- FR4：輸入為空或一個元素時，返空或例外。
- FR5：函數應保證不修改輸入的 nums 陣列（即使用 const &）

### 3.2 性能需求
- PR1：`TwoSumArray` 執行時間 O(n^2)，面向教學及驗證。
- PR2：`TwoSumHashTable` 執行時間 O(n)，空間 O(n) 。

### 3.3 可用性需求
- 所有公開 API 文件需清楚說明輸入/輸出與例外情境。

### 3.4 可測試性需求
- 單元測試包含至少以下案例：
  - 正常必達（2、7、11、15, target=9）
  - 負數、重複、目標 0
  - 空陣列、1 元素、無解
  - 大 10000 元素執行時間測量（Hash 版）
  - 確保在  n=10,000 時，TwoSumArray 不會導致系統當機，且 TwoSumHashTable 能在極短時間內完成。

### 3.5 可靠性需求
- 在資料長度 1000 內范圍，100% 通過測試。
- 報錯時回傳清晰訊息。

## 4. 外部界面需求

### 4.1 CLI 需求 (選用)
- 可建簡單 main 顯示執行結果或讀取參數。

### 4.2 軟體介面
- C++ 函式呼叫：`std::vector<int> TwoSumArray(const std::vector<int>& nums, int target);`、`std::vector<int> TwoSumHashTable(const std::vector<int>& nums, int target);`。

## 5. 其他需求

### 5.1 文檔與維護
- 提供 `README.md`, `docs/01_plan.md`, `docs/02_SRS.md`, `docs/03_SDS.md`, `docs/04_test_plan.md`, `docs/05_acceptance_tests.md`。

### 5.2 安全與法規
- 無敏感資料處理。保證不使用有安全漏洞的外部套件。

## 6. 附件

- 相關需求來源：`docs/00_intended_use.md`、`docs/08_known_issues.md`。
