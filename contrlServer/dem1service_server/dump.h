#pragma once
#pragma execution_character_set("utf-8")

// pro_dump.h
// 包含本文件是用来处理程序崩溃时自动生成dump文件的
// 生成的dump文件在程序的工作目录下
// 文件名格式是:年月日_时分秒.dmp
// 使用dmp示例：可直接把该dmp文件、程序和程序所对应的PDB文件
// 放在同一个目录下，然后双击打开dmp文件即可查看崩溃的现场
#pragma once
#include <Windows.h>
#include <tchar.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

#ifdef UNICODE
#define TSprintf	wsprintf
#else
#define TSprintf	sprintf
#endif

// 启动自动生成dump文件的话，只需要在main函数开始处
// 调用该函数（EnableAutoDump）即可
void EnableAutoDump();

// 其它函数
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    TCHAR szDumpDir[MAX_PATH] = { 0 };
    TCHAR szDumpFile[MAX_PATH] = { 0 };
    TCHAR szMsg[MAX_PATH] = { 0 };
    SYSTEMTIME	stTime = { 0 };

    // 构建dump文件路径
    GetLocalTime(&stTime);
    ::GetCurrentDirectory(MAX_PATH, szDumpDir);
    TSprintf(szDumpFile, _T("%s\\%04d%02d%02d_%02d%02d%02d.dmp"), szDumpDir,
        stTime.wYear, stTime.wMonth, stTime.wDay,
        stTime.wHour, stTime.wMinute, stTime.wSecond);
    // 创建dump文件
    CreateDumpFile(LPCSTR(szDumpFile), pException);

    // 这里弹出一个错误对话框并退出程序
    TSprintf(szMsg, _T("Sorry, contrlServer has crashed, please contact to us for help! \r\ndump file : %s"), szDumpFile);
    FatalAppExit(-1, szMsg);

    return EXCEPTION_EXECUTE_HANDLER;
}

void EnableAutoDump()
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
}

// 创建Dump文件
void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
    // 创建Dump文件
	HANDLE hDumpFile = CreateFile(LPCWSTR(lpstrDumpFilePathName), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Dump信息
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    // 写入Dump文件内容
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    CloseHandle(hDumpFile);
}