# Project Plan (計畫)

## 1. 目標與範圍

- 開發可在 C++23 環境下運行的 Two Sum 演算法套件，包含兩種解法：
  - `TwoSumArray`（暴力法，O(n^2)）
  - `TwoSumHashTable`（雜湊表優化法，O(n)）
- 提供 CLI/程式庫入口，回傳符合目標值的索引對。
- 確保單一解存在假設下正確，並對無解情境具基礎錯誤處理。

## 2. 需求與交付

### 功能需求
- 函式接口：`std::vector<int> TwoSumArray(const std::vector<int>& nums, int target)`
- 函式接口：`std::vector<int> TwoSumHashTable(std::vector<int> nums, int target)`
- 內部必須處理負數、重複值、目標為 0 或極大值情形。
- 如未找到，採拋例外或返回空向量（需在 SRS 中明確說明）。

### 非功能需求
- 使用 Google Test 完成單元測試覆蓋常見案例與邊界案例。
- 使用 GitHub Actions 自動化 CI，PR 時跑測試。
- 建構 Dockerfile，包含編譯與測試。

## 3. 成本與里程碑

1. 需求分析、設計與文件確認（1-2 天）
2. 演算法實作與本地測試（2-3 天）
3. 單元測試用例實作（2 天）
4. CI/CD 流程（GitHub Actions）與 Docker 支援（1-2 天）
5. 整合測試、驗收測試與問題修正（1-2 天）
6. 最終程式碼審查與提交（1 天）

## 4. 任務細分

- `src/two_sum.cpp` + `include/two_sum.hpp`:
  - `TwoSumArray`
  - `TwoSumHashTable`

- `test/test_two_sum.cpp`:
  - 正常案例：`[2,7,11,15], target=9` -> `[0,1]`
  - 負數/重複：`[-1,-2,-3,-4,-5], target=-8` -> `[2,4]`
  - 邊界：空陣列、無解、單一元素
  - 大小與性能驗證

- `docs/00_intended_use.md`、`docs/02_SRS.md`、`docs/03_SDS.md`、`docs/04_test_plan.md` 等補齊

## 5. 質量與驗收標準

- 所有單元測試通過且 CI 綠燈。
- 代碼符合 C++23，編譯通過無警告。
- Docker 映像建構與測試執行成功。

## 6. 風險與因應

- 風險：暴力法時間複雜度高。因應：僅作教學對比，主要推薦雜湊法；大數據測試時限制樣本。
- 風險：目標輸入無解。因應：明確回傳空向量或拋例外，並於文件中說明。
- 風險：CI 失敗。因應：本地多平台測試、單測覆蓋進行補強。

## 7. 排程/里程碑時間示範（2 週 Sprint）

- Week 1: 設計 + 實作 + 單測初版
- Week 2: CI/Docker + 進階測試 + 文件完工

## 8. 通信與交付

- 每日簡短更新：完成事項、待辦事項、障礙。
- 版本控制分支策略：`main`、`feature/two-sum`、`ci/docker`。
