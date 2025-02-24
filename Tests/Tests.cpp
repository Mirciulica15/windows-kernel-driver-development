#include "CppUnitTest.h"
#include "threadpool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
    TEST_CLASS(ListTests)
    {
    public:

        TEST_METHOD(TestListInitialize)
        {
            LIST_ENTRY list;
            ListInitializeHead(&list);
            Assert::IsTrue(ListIsEmpty(&list), L"List should be initialized as empty");
        }

        TEST_METHOD(TestListInsertAndRemove)
        {
            LIST_ENTRY list;
            ListInitializeHead(&list);

            LIST_ENTRY item1, item2;
            ListInsertHead(&list, &item1);
            ListInsertHead(&list, &item2);

            Assert::IsFalse(ListIsEmpty(&list), L"List should not be empty after insertions");

            PLIST_ENTRY removed = ListRemoveTail(&list);
            Assert::IsTrue(removed == &item1, L"Removed item should be item1");
        }
    };

    TEST_CLASS(ThreadPoolTests)
    {
    public:

        TEST_METHOD(TestThreadPoolInitialization)
        {
            MY_THREAD_POOL threadPool;
            NTSTATUS status = TpInit(&threadPool, 2);
            Assert::IsTrue(NT_SUCCESS(status), L"Thread pool should initialize successfully");
            TpUninit(&threadPool);
        }

        TEST_METHOD(TestThreadPoolWorkExecution)
        {
            MY_THREAD_POOL threadPool;
            MY_CONTEXT ctx;
            RtlZeroMemory(&ctx, sizeof(ctx));
            InitializeSRWLock(&ctx.ContextLock);

            NTSTATUS status = TpInit(&threadPool, 2);
            Assert::IsTrue(NT_SUCCESS(status), L"Thread pool should initialize successfully");

            for (int i = 0; i < 5; ++i)
            {
                status = TpEnqueueWorkItem(&threadPool, TestThreadPoolRoutine, &ctx);
                Assert::IsTrue(NT_SUCCESS(status), L"Work item should be enqueued successfully");
            }

            Sleep(1000); // Allow some time for threads to process
            TpUninit(&threadPool);

            Assert::IsTrue(ctx.Number > 0, L"Work items should be processed");
        }
    };
}
