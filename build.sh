#!/usr/bin/env zsh

# 检查并创建 example 文件夹
if [[ ! -d "example" ]]; then
    mkdir "example"
    echo "已创建 example 文件夹"
fi

# 检查并创建 export 文件夹
if [[ ! -d "export" ]]; then
    mkdir "export"
    echo "已创建 export 文件夹"
fi


# 提示用户输入数字
read -p "请输入一个数字: " number

# 创建 lab<数字> 文件夹，并切换到该目录
folder_name="lab$number"
mkdir "$folder_name"
cd "$folder_name" || exit

# 创建 src, docs, asserts 文件夹
mkdir src
mkdir docs
mkdir asserts

# 复制 lab_report.md 文件到当前文件夹
cp ../lab_report.md .
cp /path

echo "已经在 $folder_name 文件夹中执行其他操作"