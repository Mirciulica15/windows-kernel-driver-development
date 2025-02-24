#ifndef THREADPOOL_H
#define THREADPOOL_H

#define WIN32_NO_STATUS
#include <Windows.h>
#include <winternl.h>
#include <intsafe.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

#include <stdio.h>
#include <assert.h>
#include <crtdbg.h>

// **********************************************************
// *                        LIST API                        *
// **********************************************************
void ListInitializeHead(_Inout_ PLIST_ENTRY ListHead);
bool ListIsEmpty(_In_ _Const_ const PLIST_ENTRY ListHead);
void ListInsertHead(_Inout_ PLIST_ENTRY ListHead, _Inout_ PLIST_ENTRY Element);
PLIST_ENTRY ListRemoveTail(_Inout_ PLIST_ENTRY ListHead);

// **********************************************************
// *                        TP API                          *
// **********************************************************

// MY_THREAD_POOL - simple thread pool implementation
typedef struct _MY_THREAD_POOL {
    HANDLE StopThreadPoolEvent;
    HANDLE WorkScheduledEvent;
    UINT32 NumberOfThreads;
    HANDLE* ThreadHandles;
    SRWLOCK QueueLock;
    LIST_ENTRY Queue;
} MY_THREAD_POOL;

// MY_WORK_ITEM - A very basic work item
typedef struct _MY_WORK_ITEM {
    LIST_ENTRY ListEntry;
    LPTHREAD_START_ROUTINE WorkRoutine;
    PVOID Context;
} MY_WORK_ITEM;

DWORD WINAPI TpRoutine(_In_opt_ PVOID Context);
void TpUninit(_Inout_ MY_THREAD_POOL* ThreadPool);
NTSTATUS TpInit(_Inout_ MY_THREAD_POOL* ThreadPool, _In_ UINT32 NumberOfThreads);
NTSTATUS TpEnqueueWorkItem(_Inout_ MY_THREAD_POOL* ThreadPool, _In_ LPTHREAD_START_ROUTINE WorkRoutine, _In_opt_ PVOID Context);

// **********************************************************
// *                        Testing API                     *
// **********************************************************
typedef struct _MY_CONTEXT {
    SRWLOCK ContextLock;
    UINT32 Number;
} MY_CONTEXT;

DWORD WINAPI TestThreadPoolRoutine(_In_opt_ PVOID Context);

#endif // THREADPOOL_H
