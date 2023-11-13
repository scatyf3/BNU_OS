# 实验内容

用C语言或C++编写一个简单的银行家算法模拟程序，实现多个进程争用系统临界资源时的分配过程。要求实现如下功能：

1.  初始化当前系统状态。

2.  显示当前资源分配情况。

3.  检查当前系统是否安全。

4.  分配资源。某进程请求分配一组资源时，先进行安全性检查，分配后系统是否处于安全状态？如果安全，显示安全序列，将资源分配给该进程，否则该进程等待。

5.  新建一个进程（选做）。

6.  撤销一个进程（选做）。

# 实验过程
## 1
建立代码框架。我分析了代码的核心需求，编写了头文件banker.h，在主函数中编写了处理输入输出和不同情况的主框架。
## 2
设计并实现了banker类的方法，尤其是实现了复杂的`bool checkSafety()`和`bool requestResources(int processId, const std::vector<int>& request)`函数
## 3
完成程序，测试bug

# 实验代码
我的主程序是`main.cpp`，我的banker类的生命和一些宏，include在`banker.h`这个头文件里，然后banker类的具体实现在`banker.cpp`中。构建这个程序的构建逻辑在`Makefile`文件中。其中我展示我的三个源代码

## banker.h
```cpp

#ifndef BANKER_H
#define BANKER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

using vector = std::vector<int>;
using Matrix = std::vector<std::vector<int> >;

class Banker {
public:
    Banker(const std::vector<int>& available, const std::vector<std::vector<int> >& max_claim, const std::vector<std::vector<int> >& allocation);
    Banker(const std::string& filename);
    void displayAllocation();
    bool checkSafety();
    bool requestResources(int processId, const std::vector<int>& request);
    void createProcess();
    void terminateProcess(int processId);
    int num_processes_;
    int num_resources_;

private:
    vector available_;
    Matrix max_claim_;
    Matrix allocation_;
};

Matrix subtractMatrix(const Matrix& matrix1, const Matrix& matrix2);
Matrix addMatrix(const Matrix& matrix1, const Matrix& matrix2);
std::vector<int> vectorSubtraction(const std::vector<int>& vec1, const std::vector<int>& vec2);
std::vector<int> vectorAddition(const std::vector<int>& vec1, const std::vector<int>& vec2);

#endif
```

## banker.cpp

```cpp
#include "banker.h"



Banker::Banker(const std::vector<int> &available,
               const std::vector<std::vector<int>> &max_claim,
               const std::vector<std::vector<int>> &allocation)
    : available_(available),
      max_claim_(max_claim),
      allocation_(allocation),
      num_processes_(max_claim.size()),
      num_resources_(available.size()) {}

Banker::Banker(const std::string &filename)
{
    // 通过文件中的数据建立banker
    std::ifstream inputFile(filename);
    // 读取available_
    if (inputFile.is_open())
    {
        std::cout << "文件成功打开！" << std::endl;
        // 在这里可以继续处理文件内容
    }
    else
    {
        std::cout << "文件打开失败！" << std::endl;
    }
    inputFile >> num_resources_;
    int buffer;
    char name;
    for (int i = 0; i < num_resources_; i++)
    {
        inputFile >> name; // 事实上好像没用
        inputFile >> buffer;
        available_.push_back(buffer);
    }
    // 读取max_claim_
    inputFile >> num_processes_;

    // 实例化并初始化二维向量，感谢gpt3.5教会我这个良好的写法
    max_claim_.resize(num_processes_, std::vector<int>(num_resources_, 0));
    for (int i = 0; i < num_processes_; i++)
    {
        for (int j = 0; j < num_resources_; j++)
        {
            inputFile >> max_claim_[i][j];
        }
    }
    // 读取allocation_
    allocation_.resize(num_processes_, std::vector<int>(num_resources_, 0));
    for (int i = 0; i < num_processes_; i++)
    {
        for (int j = 0; j < num_resources_; j++)
        {
            inputFile >> allocation_[i][j];
        }
    }
}

void Banker::displayAllocation()
{
    // 展示当前全部资源信息
    std::cout << "Banker Information:\n";
    std::cout << "-------------------\n";

    std::cout << "Available Resources: ";
    for (const auto &resource : available_)
    {
        std::cout << resource << " ";
    }
    std::cout << "\n\n";

    std::cout << "Maximum Claim Matrix:\n";
    for (const auto &process : max_claim_)
    {
        for (const auto &resource : process)
        {
            std::cout << resource << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "Allocation Matrix:\n";
    for (const auto &process : allocation_)
    {
        for (const auto &resource : process)
        {
            std::cout << resource << " ";
        }
        std::cout << "\n";
    }

    std::cout << "-------------------\n";
}

bool Banker::checkSafety()
{
    // 实现检查当前系统是否安全的逻辑

    vector work = available_;
    Matrix need_ = subtractMatrix(max_claim_, allocation_);
    // std::cout<<num_processes_<<std::endl;
    bool allocationFound;
    int success = 0;
    std::map<int, bool> isMarked;
    for (int i = 0; i < num_processes_; i++)
    {
        isMarked[i] = false;
    }
    while (true)
    {
        allocationFound = false;
        // 添加一个标志以确保至少有一个可分配的进程
        for (int i = 0; i < num_processes_; i++)
        // 重复遍历
        {
            if (!isMarked[i])
            {
                // 遍历全部的进程
                bool safe = true;
                vector row = need_[i];
                // 遍历进程中的每种资源量，和work做比较
                for (int j = 0; j < num_resources_; j++)
                {
                    if (row[j] > work[j])
                    {
                        safe = false;
                    }
                }
                if (safe)
                {
                    isMarked[i] = true;
                    work = vectorAddition(work, allocation_[i]); // 这里应该使用 allocation_ 而不是 need_[i]
                    allocationFound = true;
                }
            }
        }
        if (!allocationFound)
        {
            break;
        }
    }
    bool result = true;
    for (int i = 0; i < num_processes_; i++)
    {
        result = result && isMarked[i];
    }
    if (result == true)
    {
        std::cout << "分配成功🎉" << std::endl;
    }
    else
    {
        std::cout << "分配失败，若是真的分配会导致死锁:(😠" << std::endl;
    }
    return result;
}

bool Banker::requestResources(int processId, const std::vector<int> &request)
{
    for(int i = 0;i<num_processes_;i++){
        if(available_[i]<request[i]){
            std::cout << "不合法请求：您请求的资源比有的资源还多👻" << std::endl;
            return false;
        }
    }
    allocation_[processId] = vectorAddition(allocation_[processId], request);
    if (!this->checkSafety())
    {
        allocation_[processId] = vectorSubtraction(allocation_[processId], request);
        return false;
    }

    return true;
}


void Banker::createProcess() {
    // 添加新进程的最大需求矩阵
    std::vector<int> max_claim_row(num_resources_, 0);  // 初始化为所有资源数为0
    max_claim_.push_back(max_claim_row);

    // 添加新进程的分配矩阵
    std::vector<int> allocation_row(num_resources_, 0);  // 初始化为所有资源数为0
    allocation_.push_back(allocation_row);

    // 更新进程数量
    num_processes_++;
}

void Banker::terminateProcess(int processId)
{
    // 检查进程ID是否有效
    if (processId < 0 || processId >= num_processes_) {
        std::cout << "无效的进程ID" << std::endl;
        return;
    }

    // 释放进程占用的资源
    for (int i = 0; i < num_resources_; i++) {
        available_[i] += allocation_[processId][i];
        allocation_[processId][i] = 0;
        max_claim_[processId][i] = 0;
    }

    // 删除进程的最大需求矩阵和分配矩阵行
    max_claim_.erase(max_claim_.begin() + processId);
    allocation_.erase(allocation_.begin() + processId);

    // 更新进程数量
    num_processes_--;

    std::cout << "进程 " << processId << " 已撤销🫥" << std::endl;
}

Matrix subtractMatrix(const Matrix &matrix1, const Matrix &matrix2)
{
    // 检查矩阵的尺寸是否相同
    if (matrix1.size() != matrix2.size() ||
        matrix1[0].size() != matrix2[0].size())
    {
        std::cerr << "Error: Matrix dimensions do not match." << std::endl;
        return Matrix();
    }

    int rows = matrix1.size();
    int cols = matrix1[0].size();

    Matrix result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }

    return result;
}

Matrix addMatrix(const Matrix &matrix1, const Matrix &matrix2)
{
    // 检查矩阵的尺寸是否相同
    if (matrix1.size() != matrix2.size() ||
        matrix1[0].size() != matrix2[0].size())
    {
        std::cerr << "Error: Matrix dimensions do not match." << std::endl;
        return Matrix();
    }

    int rows = matrix1.size();
    int cols = matrix1[0].size();

    Matrix result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}

std::vector<int> vectorAddition(const std::vector<int> &vec1, const std::vector<int> &vec2)
{
    std::vector<int> result;

    // 检查两个向量的大小是否相等
    if (vec1.size() != vec2.size())
    {
        std::cout << "Error: Vector sizes do not match." << std::endl;
        return result; // 返回空向量表示操作失败
    }

    // 逐元素相加
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        result.push_back(vec1[i] + vec2[i]);
    }

    return result;
}

std::vector<int> vectorSubtraction(const std::vector<int> &vec1, const std::vector<int> &vec2)
{
    std::vector<int> result;

    // 检查两个向量的大小是否相等
    if (vec1.size() != vec2.size())
    {
        std::cout << "Error: Vector sizes do not match." << std::endl;
        return result; // 返回空向量表示操作失败
    }

    // 逐元素相减
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        result.push_back(vec1[i] - vec2[i]);
    }

    return result;
}

```

## main.cpp


```cpp
#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "banker.h"


int main()
{
    Banker banker = Banker("input.txt");
    int choice;

    do
    {
        std::cout << "Please select an option:" << std::endl;
        std::cout << "1. Allocate resources" << std::endl;
        std::cout << "2. Display resource information" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "4. Create process" << std::endl;
        std::cout << "5. Terminate process" << std::endl;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            vector allocateResourse_(banker.num_resources_, 0);
            int processId;
            std::cout << "请输入你想给第几个进程分配资源" << std::endl;
            std::cin >> processId;
            std::cout << "请输入你想分配的资源数" << std::endl;
            for (int i = 0; i < banker.num_resources_; i++)
            {
                std::cin >> allocateResourse_[i];
            }
            banker.requestResources(processId, allocateResourse_);
            break;
        }

        case 2:
        {
            banker.displayAllocation();
            break;
        }

        case 3:
        {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        case 4:
        {
            banker.createProcess();
            break;
        }

        case 5:
        {
            int processId;
            std::cout << "请输入你想撤销的进程ID" << std::endl;
            std::cin >> processId;
            banker.terminateProcess(processId);
            break;
        }

        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 3);

    return 0;
}


```

# 实验结果

## 分配资源

![Alt text](image.png)

## 展示当前资源占用情况

![Alt text](image-1.png)

## 创建进程

![Alt text](image-2.png)

## 删除进程

![Alt text](image-3.png)

# 实验反思

在本次实验中，我们实现了银行家算法，并对其进行了测试和验证。通过这次实验，我对银行家算法的原理和实现细节有了更深入的理解，并学到了如何使用C++编程语言来实现该算法。

在实验过程中，我首先根据实验要求编写了银行家类的头文件，并在另一个文件中实现了该类的成员函数和辅助函数。通过使用向量和矩阵数据结构，我能够有效地表示资源的可用性、进程的最大需求和已分配资源的情况。

在完成实现后，我对银行家类的各个成员函数进行了测试。我编写了一个主函数，在其中创建了一个银行家对象，并调用了其成员函数进行资源请求、分配和终止进程等操作。

通过这次实验，我深入了解了银行家算法的核心思想和实现步骤。我学会了如何设计类和成员函数，以及如何使用向量和矩阵来处理资源分配问题。我还了解了如何进行错误处理和异常处理，以确保程序的稳定性和健壮性。