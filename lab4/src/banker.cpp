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
