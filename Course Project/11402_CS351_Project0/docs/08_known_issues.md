# Project 0 已知問題與系統限制報告 (Known Issues & Limitations)

本報告記錄了 `TwoSum` 專案在目前架構設計下的已知限制、潛在風險（如極端測資下的表現）以及未來的優化與改進方向（Roadmap）。

---

## 1. 已知限制與技術債 (Known Limitations & Technical Debt)

### 1.1 記憶體與整數溢位風險 (Integer Overflow Risk)
* **問題描述**：目前演算法的核心介面使用標準 `int` 作為元素與 `target` 的型別。若輸入陣列中包含接近 `std::numeric_limits<int>::max()`（約 $2 \times 10^9$）的極端大數，在執行 `nums[i] + nums[j]` 或計算 `complement = target - nums[i]` 时，會發生**整數溢位（Integer Overflow）**，導致非預期的計算錯誤。
* **影響範圍**：`TwoSumArray` 與 `TwoSumHashTable` 皆受影響。
* **緩解方案**：未來重構時，應將內部計算與介面型別提升至 `long long` 或使用 `int64_t`。

### 1.2 雜湊碰撞與最壞時間複雜度 (Hash Collision & Worst-case Performance)
* **問題描述**：`TwoSumHashTable` 依賴 C++ 標準庫的 `std::unordered_map`。雖然平均時間複雜度為 $O(1)$，但在極端精心設計的惡意測資下（所有元素的 Hash 值皆完全相同），會發生嚴重的**雜湊碰撞（Hash Collision）**。
* **影響範圍**：此時雜湊表底層的鏈結串列會退化，導致最壞時間複雜度由 $O(n)$ 退化至 $O(n^2)$。
* **緩解方案**：對於高安全性要求的場景，可考慮導入自訂的防碰撞雜湊函數（如 `splitmix64`）或改用平衡樹架構的 `std::map`（維持穩定的 $O(n \log n)$）。

---

## 2. 環境與部署限制 (Environment & Deployment Issues)

### 2.1 Windows 本地端 MSVC 編碼警告 (Warning C4819)
* **問題描述**：在 Windows 環境下使用 MSVC 編譯器建置時，會因為標頭檔與原始碼包含 UTF-8 中文註解，而噴出 `warning C4819: 檔案含有無法在目前字碼頁 (950) 中表示的字元`。
* **影響範圍**：僅影響 Windows 編譯時的 Log 視覺整潔度，**不影響程式運作與測試結果**。在 Linux (Docker) 環境下無此問題。
* **緩解方案**：已於部署報告中註記，未來可在 `CMakeLists.txt` 中加入 MSVC 專用參數 `add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")` 強制指定編碼。

### 2.2 Docker 映像檔建置時間較長 (Docker Build Latency)
* **問題描述**：由於 `Dockerfile` 在第一階段需要從頭下載 `vcpkg` 原始碼並在容器內現場編譯 Google Test 框架，導致首次執行 `docker build` 時需耗時數分鐘（取決於網路頻寬）。
* **緩解方案**：已在 `Dockerfile` 導入 Docker Layer Cache 機制，且未來可考慮直接將 vcpkg 編譯好的二進位依賴打包成預建置基礎映像檔（Pre-built Base Image），以將建置時間縮短至 5 秒內。

---

## 3. 未來擴充計畫 (Future Roadmap)

為使本專案達到工業級生產標準，預計於後續版本進行以下優化：
1. **泛型化設計 (Generics)**：利用 C++20 的 **Concepts** 特性，將 `TwoSum` 改寫為模板函式（Template），使其不僅支援 `int`，亦能泛型支援 `float`、`double` 或自訂數值型別。
2. **多執行緒加速 (Parallelism)**：針對百萬級以上的超大型數據集，在 `TwoSumArray` 中導入 C++17 Execution Policy (`std::execution::par`)，利用多核心 CPU 進行並行化暴力搜尋加速。

---
報告發布日期: 2026-06-16