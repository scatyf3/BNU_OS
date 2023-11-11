#ifndef BANKER_H
#define BANKER_H

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