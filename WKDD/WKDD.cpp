// WKDD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "threadpool.h"
#include "WKDD.h"

bool g_IsThreadPoolRunning = false;
MY_THREAD_POOL tp = { 0 };
MY_CONTEXT ctx = { 0 };
NTSTATUS status = STATUS_UNSUCCESSFUL;


NTSTATUS RunWorkItems(_Inout_ MY_THREAD_POOL* tp, _Out_ MY_CONTEXT* ctx, _In_ UINT32 numItems)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;

    InitializeSRWLock(&ctx->ContextLock);
    ctx->Number = 0;

    for (UINT32 i = 0; i < numItems; ++i)
    {
        status = TpEnqueueWorkItem(tp, TestThreadPoolRoutine, ctx);
        if (!NT_SUCCESS(status))
        {
            printf("Failed to enqueue work item. Status: 0x%08X\n", status);
            return status;
        }
    }

    return STATUS_SUCCESS;
}

void PrintHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  help   - Show this message" << std::endl;
    std::cout << "  start  - Start the thread pool" << std::endl;
    std::cout << "  stop   - Stop the thread pool" << std::endl;
    std::cout << "  exit   - Exit the application" << std::endl;
}

int main()
{
    std::string command;

    std::cout << "Welcome to the User Mode Console Application!" << std::endl;
    std::cout << "Type 'help' for available commands, or 'exit' to quit." << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "help") {
            PrintHelp();
        }
        else if (command == "start") {
            if (g_IsThreadPoolRunning) {
                std::cout << "Thread pool is already running." << std::endl;
            }
            else {
                status = TpInit(&tp, 5);
                if (!NT_SUCCESS(status))
                {
                    std::cout << "Failed to start thread pool. Status: " << status << std::endl;
                    return status;
                }

                std::cout << "Started thread pool..." << std::endl;
                g_IsThreadPoolRunning = true;
            }
        }
        else if (command == "stop") {
            if (g_IsThreadPoolRunning) {
                TpUninit(&tp);
                g_IsThreadPoolRunning = false;
                std::cout << "Thread pool stopped. Current ctx number is " << ctx.Number << std::endl;
            }
            else {
                std::cout << "Thread pool is not running." << std::endl;
            }
        }
        else if (command == "work") {
            std::cout << "Sending work to thread pool..." << std::endl;
            RunWorkItems(&tp, &ctx, 1000);
        }
        else if (command == "exit") {
            std::cout << "Exiting application..." << std::endl;
            break;
        }
        else {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
