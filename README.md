# CG 114 Assignment 1

> 本次作業目的為讓同學熟悉 OpenGL 與 GLUT 中基本函式與視角操作，詳細說明如下：
> 1. 請同學下載 Moodle 提供的.stl 檔案(也可自行使用 Solidworks 等軟體繪圖)，格式可為 ASCII 或 Binary
> 2. 讀取上述之.stl 檔案並顯示於視窗 (60%)
> 3. 使用上下左右鍵操作視角進行 x-y 方向旋轉 (30%)
> 4. 程式架構是否清楚、簡潔 (10%)

## Compile Instructions
### Linux
```bash
make displaystl
```
### Windows Cross Compile (MinGW)
```bash
make displaystl.exe
```

## Run Instructions
### Linux
```bash
./displaystl [ascii|binary] [filename.stl]
```
### Windows
```bash
displaystl.exe [ascii|binary] [filename.stl]
```

If no arguments are provided, the program defaults to loading the binary STL file `Bunny_Binary.stl`.

Use arrow keys to rotate the model.