@echo off
setlocal enabledelayedexpansion

REM 设置源目录和输出文件
set "source_dir=E:\gameDevelop\TutorialZero\SuperSmashPlants\SuperSmashPlants\resources"
set "output_file=E:\gameDevelop\TutorialZero\SuperSmashPlants\SuperSmashPlants\resources\file_list.txt"

REM 如果输出文件已存在，则删除
if exist "%output_file%" del "%output_file%"

REM 遍历源目录中的所有 .png 文件
for /r "%source_dir%" %%f in (*.png) do (
    REM 获取文件名
    set "filename=%%~nxf"
    REM 格式化文件名
    echo resource/!filename!>>"%output_file%"
)

echo Done! List of .png files saved to %output_file%
