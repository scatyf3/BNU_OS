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
