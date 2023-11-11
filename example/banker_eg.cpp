/*
src:https://blog.csdn.net/zheliku/article/details/121209243
它的代码是分段的，我可能拷贝的时候有误，根本跑不起来，但是它的抽象层次感觉对我来说有一定帮助
*/

#include <iostream>
#include <vector>

using namespace std;

void init();  // 初始化数据，即获得用户输入

void showResourse();  // 显示系统资源

void showInfoStart();  // 显示程序开始的信息

void showInfoOp();  // 显示用户操作信息

void showInfoEnd();  // 显示程序结束的信息

bool safe();  // 检查系统是否安全

vector<vector<int>> operator-(const vector<vector<int>> &v1,
                              const vector<vector<int>> &v2);

vector<vector<int>> operator+(const vector<vector<int>> &v1,
                              const vector<vector<int>> &v2);

vector<int> operator-(const vector<int> &v1, const vector<int> &v2);

vector<int> operator+(const vector<int> &v1, const vector<int> &v2);

bool operator<=(const vector<int> &v1, const vector<int> &v2);

bool operator>(const vector<int> &v1, const vector<int> &v2);

vector<int> Available;           // 可利用资源向量
vector<string> resourseName;     // 资源 Available[i] 的名称
vector<vector<int>> Max;         // 最大需求矩阵
vector<vector<int>> Allocation;  // 分配矩阵
vector<vector<int>> Need;        // 需求矩阵
vector<int> Request_i;           // 进程 Pi 的请求向量
vector<int> P;                   // 记录安全序列
vector<int> Allocated;           // 记录进程是否已被分配资源

int m = 0;  // m 种资源
int n = 0;  // n 个进程

int main() {
    int choice = 0;  // 记录用户的选择
    int Pi;          // 记录用户选择的进程

    // 显示程序开始信息
    showInfoStart();

    // 初始化操作，获得用户输入数据
    init();

    // 显示系统资源
    showResourse();

    // 首先检查一遍，输出系统是否安全
    if (safe()) {
        cout << "\n系统安全！\n";
        cout << "安全序列为：";
        for (int i = 0; i < P.size() - 1; ++i) {
            cout << P[i] << " -> ";
        }
        cout << P[n - 1] << "\n";
    } else {
        cout << "系统不安全！！！\n";
    }
    // 进入循环，开始与用户交互
    do {
        // 显示用户的操作信息
        showInfoOp();
        cout << "请选择：";

        // 获取用户操作
        cin >> choice;
        switch (choice) { 
			case 1:
            // 与用户交互，获得需要的信息
            cout << "请输入要求分配的资源进程号(0 - " << (n - 1) << ")：";
            cin >> Pi;
            cout << "请输入进程 " << Pi << "申请的资源\n( ";
            for (const auto &elem : resourseName) cout << elem << " ";
            cout << "):\n";
            for (auto &elem : Request_i) cin >> elem;

            /********************************************************************
             *                           进入银行家算法 *
             *******************************************************************/

            // 步骤 1：检查请求是否超过其需要
            if (Request_i > Need[Pi]) {
                cout << "\n进程 " << Pi
                     << " 申请的资源大于它需要的资源，不给予分配！\n";
                break;
            }

            // 步骤 2：检查系统剩余资源是否能够满足请求
            if (Request_i > Available) {
                cout << "\n进程 " << Pi
                     << " 申请的资源大于系统已有的资源，请下次再申请！\n";
                break;
            }

            // 步骤 3：系统尝试分配资源
            Available = Available - Request_i;
            Allocation[Pi] = Allocation[Pi] + Request_i;
            Need[Pi] = Need[Pi] - Request_i;

            // 步骤 4：执行安全性算法
            // 如果安全，则分配并打印信息显示
            if (safe()) {
                // Available = Available + Max[Pi];    //
                // 系统安全，令进程运行结束，回收资源
                showResourse();
                cout << "\n系统安全！\n";
                cout << "安全序列为：";
                for (int i = 0; i < P.size() - 1; ++i) {
                    cout << P[i] << " -> ";
                }
                cout << P[n - 1] << "\n";
            }
            // 如果不安全，则恢复分配之前的状态，打印信息
            else {
                cout << "\n系统不安全，撤销资源分配！！！\n";
                Available = Available + Request_i;
                Allocation[Pi] = Allocation[Pi] - Request_i;
                Need[Pi] = Need[Pi] + Request_i;
            }
		
		 }
        while (choice != 0)
            ;  // 用户输入 0，则退出循环
        // 操作 1：尝试分配资源
        
            // 打印程序结束信息
            showInfoEnd();

            return 0;
    }
    /**
     * 检查系统是否安全
     * @return 若安全，返回 true <br>
     *           若不安全，返回 false
     */
    bool safe() {
        // 初始化向量 Work 和 Finish
        vector<int> Work = Available;
        vector<bool> Finish(n, false);

        // 清除上次的进程队列
        P.clear();

        // 遍历 n 次
        for (int j = 0; j < n; ++j)
            // 遍历 1 次，寻找 n 个进程中满足条件的一个
            for (int i = 0; i < n; ++i)
                if (!Finish[i] && Need[i] <= Work) {
                    // 选中该进程 i，进行相应操作后将其加入队列 P 中
                    Finish[i] = true;
                    Work = Work + Allocation[i];
                    P.push_back(i);
                }

        // 检查每个进程是否都被选中
        for (int i = 0; i < n; ++i)
            // 如果有一个进程未被选中，说明系统不安全，返回 false
            if (!Finish[i]) return false;

        // 所有进程都被选中，说明系统状态安全，返回 true
        return true;
    }
    /**
     * 初始化数据，即获得用户输入
     */
    void init() {
        cout << "请输入系统可供资源种类的数量 m：";
        cin >> m;
        cout << "\n";

        // 设置资源向量的大小
        resourseName.resize(m);
        Available.resize(m);
        Request_i.resize(m);

        // 获取用户输入
        for (int i = 0; i < m; ++i) {
            cout << "资源 Available[" << (i + 1) << "] 的名称：";
            cin >> resourseName[i];
            cout << "资源的数量：";
            cin >> Available[i];
            cout << "\n";
        }

        cout << "\n请输入作业的数量 n：";
        cin >> n;

        // 设置相应数据结构的大小
        Max.resize(n);
        Allocation.resize(n);
        for (int i = 0; i < n; ++i) {
            Max[i].resize(m);
            Allocation[i].resize(m);
        }
        Allocated.resize(n, 0);

        // 输出信息
        cout << "\n请输入各进程的最大需求量 Max[" << n << "][" << m << "]：\n";
        for (auto &line : Max)
            for (auto &elem : line) cin >> elem;
        cout << "\n请输入各进程已经申请的资源量 Allocation[" << n << "][" << m
             << "]：\n";
        for (auto &line : Allocation)
            for (auto &elem : line) cin >> elem;

        // 初始化 Need 矩阵
        Need = Max - Allocation;
    }
    /**
     * 显示系统资源
     */
    void showResourse() {
        // 记录原先的 cout 输出格式，并设置其为左对齐输出
        auto oldStyle = cout.setf(ios_base::left, ios_base::adjustfield);

        // 设置默认数据打印宽度，用于格式化输出
        int DEFAULTNUMWIDTH = DEFAULTNAMEWIDTH / m;
        int DEFAULTLEFT = DEFAULTNAMEWIDTH - DEFAULTNUMWIDTH * m;

        // 打印 Available
        cout << "\n";
        cout << "系统目前可用的资源 Available[m]：";
        cout << "\n";
        for (const auto &elem : resourseName)
            cout << setw(DEFAULTNUMWIDTH) << elem;
        cout << "\n";
        for (const auto &elem : Available)
            cout << setw(DEFAULTNUMWIDTH) << elem;

        cout << "\n\n";

        // 打印信息表
        cout << setw(DEFAULTNAMEWIDTH) << "-----------";
        cout << setw(DEFAULTNAMEWIDTH) << "Max";
        cout << setw(DEFAULTNAMEWIDTH) << "Allocation";
        cout << setw(DEFAULTNAMEWIDTH) << "Need";

        cout << "\n";

        // 打印资源名称
        cout << setw(DEFAULTNAMEWIDTH) << "resourseName";
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << setw(DEFAULTNUMWIDTH) << resourseName[j];
            }
            cout << setw(DEFAULTLEFT) << "";
        }

        cout << "\n\n";

        // 格式化打印资源数据
        for (int i = 0; i < n; ++i) {
            cout << setw(DEFAULTNAMEWIDTH) << i;
            for (int j = 0; j < m; ++j) {
                cout << setw(DEFAULTNUMWIDTH) << Max[i][j];
            }
            cout << setw(DEFAULTLEFT) << "";

            for (int j = 0; j < m; ++j) {
                cout << setw(DEFAULTNUMWIDTH) << Allocation[i][j];
            }
            cout << setw(DEFAULTLEFT) << "";

            for (int j = 0; j < m; ++j) {
                cout << setw(DEFAULTNUMWIDTH) << Need[i][j];
            }
            cout << setw(DEFAULTLEFT) << "";
            cout << "\n";
        }

        // 将 cout 的输出格式恢复
        cout.setf(oldStyle, ios_base::adjustfield);
    }
    /**
     * 显示程序开始的信息
     */
    void showInfoStart() {
        cout << "*******************************************\n";
        cout << "*               银行家算法演示              *\n";
        cout << "*******************************************\n\n";
    }
    /**
     * 显示用户操作信息
     */
    void showInfoOp() {
        cout << "***************银行家算法演示***************\n";
        cout << "             1 —— 分配资源\n";
        cout << "             2 —— 显示分配情况\n";
        cout << "             0 —— 离开\n";
        cout << "******************************************\n";
    }
    /**
     * 显示程序结束的信息
     */
    void showInfoEnd() {
        cout << "*******************************************\n";
        cout << "*             银行家算法演示结束             *\n";
        cout << "*******************************************\n";
    }
    /**
     * 检查系统是否安全
     * @return 若安全，返回 true <br>
     * 		   若不安全，返回 false
     */
    bool safe() {
        // 初始化向量 Work 和 Finish
        vector<int> Work = Available;
        vector<bool> Finish(n, false);

        // 清除上次的进程队列
        P.clear();

        // 遍历 n 次
        for (int j = 0; j < n; ++j)
            // 遍历 1 次，寻找 n 个进程中满足条件的一个
            for (int i = 0; i < n; ++i)
                if (!Finish[i] && Need[i] <= Work) {
                    // 选中该进程 i，进行相应操作后将其加入队列 P 中
                    Finish[i] = true;
                    Work = Work + Allocation[i];
                    P.push_back(i);
                }

        // 检查每个进程是否都被选中
        for (int i = 0; i < n; ++i)
            // 如果有一个进程未被选中，说明系统不安全，返回 false
            if (!Finish[i]) return false;

        // 所有进程都被选中，说明系统状态安全，返回 true
        return true;
    }
    /**
     * 执行矩阵（二维数组）的减法
     * @param v1 矩阵 1
     * @param v2 矩阵 2
     * @return 相减后的矩阵
     */
    vector<vector<int>> operator-(const vector<vector<int>> &v1,
                                  const vector<vector<int>> &v2) {
        vector<vector<int>> ans(n, vector<int>(m, 0));
        for (int i = 0; i < n; ++i) {
            ans[i] = v1[i] - v2[i];
        }
        return ans;
    }

    /**
     * 执行矩阵（二维数组）的加法
     * @param v1 矩阵 1
     * @param v2 矩阵 2
     * @return 相加后的矩阵
     */
    vector<vector<int>> operator+(const vector<vector<int>> &v1,
                                  const vector<vector<int>> &v2) {
        vector<vector<int>> ans(n, vector<int>(m, 0));
        for (int i = 0; i < n; ++i) {
            ans[i] = v1[i] + v2[i];
        }
        return ans;
    }

    /**
     * 执行向量（数组）的减法
     * @param v1 向量 1
     * @param v2 向量 2
     * @return 相减后的向量
     */
    vector<int> operator-(const vector<int> &v1, const vector<int> &v2) {
        vector<int> ans(m, 0);
        for (int i = 0; i < m; ++i) ans[i] = v1[i] - v2[i];
        return ans;
    }

    /**
     * 执行向量（数组）的加法
     * @param v1 向量 1
     * @param v2 向量 2
     * @return 相加后的向量
     */
    vector<int> operator+(const vector<int> &v1, const vector<int> &v2) {
        vector<int> ans(m, 0);
        for (int i = 0; i < m; ++i) ans[i] = v1[i] + v2[i];
        return ans;
    }

    /**
     * 重载 vector <= 符号，使得满足银行家算法的条件
     * @param v1 向量 1
     * @param v2 向量 2
     * @return 若 v1 <= v2，返回 true <br>
     * 		   若 v1 > v2，返回 false
     */
    bool operator<=(const vector<int> &v1, const vector<int> &v2) {
        for (int i = 0; i < v1.size(); ++i)
            if (v1[i] > v2[i]) return false;
        return true;
    }

    /**
     * 重载 vector > 符号，使得满足银行家算法的条件
     * @param v1 向量 1
     * @param v2 向量 2
     * @return 若 v1 > v2，返回 true <br>
     * 		   若 v1 <= v2，返回 false
     */
    bool operator>(const vector<int> &v1, const vector<int> &v2) {
        return !(v1 <= v2);
    }
