// WKDD.h
#ifndef WKDD_H
#define WKDD_H

#include <Windows.h>
#include "threadpool.h"

extern bool g_IsThreadPoolRunning;
extern MY_THREAD_POOL tp;
extern MY_CONTEXT ctx;
extern NTSTATUS status;

NTSTATUS RunWorkItems(_Inout_ MY_THREAD_POOL* tp, _Out_ MY_CONTEXT* ctx, _In_ UINT32 numItems);
void PrintHelp();

#endif // WKDD_H
