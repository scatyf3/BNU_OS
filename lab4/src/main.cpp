#include <iostream>
#include <string>
#include <fstream>

#include "banker.h"

Banker::Banker(const std::vector<int>& available,
               const std::vector<std::vector<int>>& max_claim,
               const std::vector<std::vector<int>>& allocation)
    : available_(available),
      max_claim_(max_claim),
      allocation_(allocation),
      num_processes_(max_claim.size()),
      num_resources_(available.size()) {}

Banker::Banker(const std::string& filename) {
    // 通过文件中的数据建立banker
    std::ifstream inputFile(filename);
    // 读取available_
    int num_resources_;
    inputFile >> num_resources_;
    int buffer;
    char name;
    for (int i = 0; i < num_resources_; i++) {
        inputFile >> name;  // 事实上好像没用
        inputFile >> buffer;
        available_.push_back(buffer);
    }
    // 读取max_claim_
    inputFile >> num_processes_;
    max_claim_[num_processes_][num_resources_];
    for (int i = 0; i < num_processes_; i++) {
        for (int j = 0; j < num_resources_; j++) {
            inputFile >> max_claim_[i][j];
        }
    }
    // 读取allocation_
    allocation_[num_processes_][num_resources_];
    for (int i = 0; i < num_processes_; i++) {
        for (int j = 0; j < num_resources_; j++) {
            inputFile >> allocation_[i][j];
        }
    }
}

void Banker::displayAllocation() {
    // 展示当前全部资源信息
    std::cout << "Banker Information:\n";
    std::cout << "-------------------\n";

    std::cout << "Available Resources: ";
    for (const auto& resource : available_) {
        std::cout << resource << " ";
    }
    std::cout << "\n\n";

    std::cout << "Maximum Claim Matrix:\n";
    for (const auto& process : max_claim_) {
        for (const auto& resource : process) {
            std::cout << resource << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "Allocation Matrix:\n";
    for (const auto& process : allocation_) {
        for (const auto& resource : process) {
            std::cout << resource << " ";
        }
        std::cout << "\n";
    }

    std::cout << "-------------------\n";
}

bool Banker::checkSafety() {
    // 实现检查当前系统是否安全的逻辑

    vector work = available_;
    Matrix need_ = subtractMatrix(max_claim_, allocation_);
    int counter = num_processes_;
    int success = 0;
    while (counter!= 0) {
        for (int i = 0; i < num_processes_; i++) {
            bool safe = true;
            vector row = need_[i];
            for (int j = 0; j < num_resources_; j++) {
                if (row[j] > work[j]) {
                    safe = false;
                }
            }
            if (safe) {
                success++;
                work = vectorAddition(work, row);
            }
        }
        counter--;
    }
    if(success==num_processes_){return true;}
    return false;
}

bool Banker::requestResources(int processId, const std::vector<int>& request) {

    allocation_[processId]=vectorAddition(allocation_[processId],request);
    if(!checkSafety()){
        allocation_[processId]=vectorSubtraction(allocation_[processId],request);
        return false;
    }
    
    return true;
}

void Banker::createProcess() {
    // 实现新建一个进程的逻辑（选做）

}

void Banker::terminateProcess(int processId) {
    // 实现撤销一个进程的逻辑（选做）
}

Matrix subtractMatrix(const Matrix& matrix1, const Matrix& matrix2) {
    // 检查矩阵的尺寸是否相同
    if (matrix1.size() != matrix2.size() ||
        matrix1[0].size() != matrix2[0].size()) {
        std::cerr << "Error: Matrix dimensions do not match." << std::endl;
        return Matrix();
    }

    int rows = matrix1.size();
    int cols = matrix1[0].size();

    Matrix result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }

    return result;
}

Matrix addMatrix(const Matrix& matrix1, const Matrix& matrix2) {
    // 检查矩阵的尺寸是否相同
    if (matrix1.size() != matrix2.size() ||
        matrix1[0].size() != matrix2[0].size()) {
        std::cerr << "Error: Matrix dimensions do not match." << std::endl;
        return Matrix();
    }

    int rows = matrix1.size();
    int cols = matrix1[0].size();

    Matrix result(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}

std::vector<int> vectorAddition(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> result;

    // 检查两个向量的大小是否相等
    if (vec1.size() != vec2.size()) {
        std::cout << "Error: Vector sizes do not match." << std::endl;
        return result; // 返回空向量表示操作失败
    }

    // 逐元素相加
    for (size_t i = 0; i < vec1.size(); ++i) {
        result.push_back(vec1[i] + vec2[i]);
    }

    return result;
}

std::vector<int> vectorSubtraction(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> result;

    // 检查两个向量的大小是否相等
    if (vec1.size() != vec2.size()) {
        std::cout << "Error: Vector sizes do not match." << std::endl;
        return result; // 返回空向量表示操作失败
    }

    // 逐元素相减
    for (size_t i = 0; i < vec1.size(); ++i) {
        result.push_back(vec1[i] - vec2[i]);
    }

    return result;
}

int main() {
    Banker banker = Banker(
        "/Users/juanitahowe/Documents/GitHub/BNU_OS/lab4/src/"
        "银行家测试数据例子.txt");
    int choice;

    do {
        std::cout << "Please select an option:" << std::endl;
        std::cout << "1. Allocate resources" << std::endl;
        std::cout << "2. Display resource information" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "请输入你想分配的资源量" << std::endl;
                vector allocateResourse_(banker.num_resources_, 0);
                int processId;
                std::cout << "请输入你想给第几个进程分配资源" << std::endl;
                std::cin >> processId;
                std::cout << "请输入你想分配的资源数" << std::endl;
                for (int i = 0; i < banker.num_resources_; i++) {
                    std::cin >> allocateResourse_[i];
                }
                banker.requestResources(processId, allocateResourse_);
                break;
            }

            case 2: {
                banker.displayAllocation();
                break;
            }

            case 3: {
                std::cout << "Exiting..." << std::endl;
                break;
            }

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 3);

    return 0;
}

