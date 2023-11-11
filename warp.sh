#!/bin/bash

# 提示用户输入数字
read -p "请输入一个数字: " number

# 创建 lab<数字> 文件夹并复制文件
folder_name="lab$number"
mkdir -p "export/$folder_name/src"

# 拷贝 常见源代码文件到目标目录
find "$folder_name/src/"* -type f -name f \( -name "*.c" -o -name "*.cpp" -o -name "*.py" -o -name "*.sh" -o -name "Makefile" \) -exec cp {} "export/$folder_name/src/" \;

cp "$folder_name/lab_report.md" "export/$folder_name"

# 切换到 export/lab<数字> 目录
cd "export/$folder_name" || exit

# 运行 pandoc 命令将 lab_report.md 转换为 lab_report.docx
pandoc "lab_report.md" -o "lab_report.docx"

# 压缩 lab<数字> 文件夹并重命名为指定字符串
zip -r "$folder_name.zip" .
# 将eencoded_string中的内容更改为你的名字+学号
encoded_string="1145141919810 李田所 实验$number"
mv "$folder_name.zip" "$encoded_string.zip"

echo "操作完成"