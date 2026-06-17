# 08_known_issues.md - 已知問題與未來展望 (Known Issues & Future Roadmap)

## 1. 已知限制與技術取捨 (Known Issues / Limitations)

### 1.1 記憶體容量約束 (In-Memory Boundary)
* **現狀**：本資料庫屬於全記憶體（In-Memory）運作，處理的 CSV 檔案大小受限於系統實體 RAM 的大小。
* **影響**：若導入數 GB 以上的超巨型 CSV 檔案，可能會引發 OOM (Out of Memory) 崩潰。目前最佳適用場景為 100 萬筆以內、100MB 級別的中輕量級資料。

### 1.2 僅支援 HASH 等值索引
* **現狀**：目前僅實作了基於 `std::unordered_map` 的 `HashIndex`，僅能優化 `Op::EQ`（等於）查詢。
* **影響**：當執行 `Op::GT`（大於）或 `Op::LT`（小於）等範圍查詢時，引擎依然會自動降級（Fallback）使用 $O(N)$ 的全表掃描。

### 1.3 唯讀架構
* **現狀**：本系統目前聚焦於「載入與高效率查詢」，未實作執行期的 `INSERT`、`UPDATE`、`DELETE` API。

---

## 2. 未來優化展望 (Future Roadmap)

* **引入 TreeIndex (M5 擴充)**：未來計畫使用 `std::map`（紅黑樹）實作 `TreeIndex`，以將大於、小於等範圍查詢的時間複雜度優化至 $O(\log N)$。
* **持久化與磁碟暫存 (Disk Paging)**：實作頁面管理（Page Manager），允許資料儲存在磁碟中並按需（On-Demand）載入，解除實體記憶體大小的限制。
* **多執行緒平行掃描 (Parallel Scan)**：在無法套用索引、必須走全表掃描時，利用 `std::execution::par`（C++17 平行演算法）將資料分塊（Chunks），進行多核心平行過濾以壓榨極致效能。