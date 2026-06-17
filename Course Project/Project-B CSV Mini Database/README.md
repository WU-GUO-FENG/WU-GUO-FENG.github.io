# Project B - CSV Mini Database

## 專案簡介

本專案旨在開發一個基於 Python 的小型 CSV 資料庫管理系統。使用者可以透過此系統對 CSV 檔案進行基本的資料操作（CRUD），包括讀取、寫入、查詢與過濾，模擬資料庫的核心邏輯。

系統適合用於管理結構化資料，例如學生名單、商品資訊或簡易記錄檔，並透過介面提供直覺的資料處理流程。

## 功能說明

- 讀取 CSV 檔案，將資料載入記憶體
- 寫入或更新 CSV 檔案內容
- 查詢指定欄位或關鍵字
- 過濾資料、顯示符合條件的紀錄
- 支援 CRUD 操作：新增、讀取、更新、刪除資料
- 保持 CSV 檔案格式一致，避免資料遺失

## 技術要點

- 使用 Python 標準函式庫 `csv` 進行檔案讀寫
- 透過 `list` 與 `dict` 建立資料結構與索引
- 強化資料處理能力與錯誤處理邏輯
- 支援可擴充的查詢與過濾條件

## 環境需求

- Python 3.8 以上版本
- 作業系統：Windows / macOS / Linux

## 安裝與執行

1. 下載或 clone 專案內容
2. 建議建立虛擬環境：
   ```bash
   python -m venv venv
   .\venv\Scripts\activate   # Windows
   source venv/bin/activate    # macOS / Linux
   ```
3. 安裝專案相依套件（若有 requirements.txt）：
   ```bash
   pip install -r requirements.txt
   ```
4. 執行專案主程式：
   ```bash
   python main.py
   ```

> 提示：若專案主程式名稱不同，請改為對應的檔案名稱。

## 專案結構（範例）

- `README.md`：專案說明文件
- `main.py`：程式進入點
- `csv_db.py`：CSV 資料庫處理邏輯
- `data/`：存放範例 CSV 檔案
- `tests/`：測試程式碼

## 使用範例

1. 載入 CSV 檔案
2. 查詢某欄位資料
3. 新增一筆紀錄
4. 更新或刪除指定資料
5. 將修改結果寫回 CSV

## 學習目標

1. 實作 CSV 檔案的解析與資料處理邏輯。
2. 練習標準的軟體開發工作流（Jira + GitHub + VS Code）。
3. 強化 Python 中的資料結構運用（如 List、Dictionary）。
4. 掌握資料過濾與查詢的基本設計。

## 未來擴充方向

- 加入圖形化使用者介面（GUI）
- 支援多重 CSV 檔案管理
- 實作進階排序與統計分析功能
- 支援更複雜的查詢語法與條件組合


