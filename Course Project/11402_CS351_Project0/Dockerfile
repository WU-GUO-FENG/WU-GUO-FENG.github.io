# ==========================================
# 階段一：建置與編譯環境 (Build Stage)
# ==========================================
FROM ubuntu:24.04 AS builder

# 避免安裝過程跳出互動式選單
ENV DEBIAN_FRONTEND=noninteractive

# 安裝 C++20 編譯器、CMake 與 vcpkg 所需的工具
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# 在容器內安裝並設定 vcpkg
WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git \
    && ./vcpkg/bootstrap-vcpkg.sh

# 將 vcpkg 執行檔加入環境變數
ENV PATH="/opt/vcpkg:${PATH}"

# 設定專案工作目錄
WORKDIR /app

# 在複製原始碼之前，先利用 vcpkg 安裝 Linux 版的 Google Test
RUN vcpkg install gtest

# 複製本地所有專案原始碼進容器 (會自動被 .dockerignore 過濾掉 build 快取)
COPY . .

# 使用 CMake 搭配 vcpkg 工具鏈進行配置與編譯
RUN mkdir build && cd build \
    && cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
    && cmake --build .

# ==========================================
# 階段二：輕量化執行環境 (Run Stage)
# ==========================================
FROM ubuntu:24.04

WORKDIR /root/

# 1. 誠實拷貝：直接把 builder 階段編譯出來的整個 build 目錄倒進來
COPY --from=builder /app/build ./build
# 2. 拷貝測試資料
COPY --from=builder /app/test ./test

# 預設改為直接執行單元測試（因為剛剛看到 build 完有產生它）
CMD ["./build/twosum_test"]