#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <io.h> 
#include <iostream>
#include <QProcess>
#include <QTimer>
#include <QThread>
#include "KillExe.h"

using namespace std;

KillExe kill_exe_;
//隐藏DOS黑窗口
//#pragma comment(linker,"/subsystem:\"windows\"  /entry:\"mainCRTStartup\"" ) 

//定义路径最大程度
#define MAX_PATH 4096
//定义守护进程名称
#define PROCCESS_NAME "FaceTection.exe"
//定义写入的注册表路径
#define SELFSTART_REGEDIT_PATH "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\"

//设置本身开机自启动
BOOL SetSelfStart()
{
	//获取程序完整名称
	char pName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, pName, MAX_PATH);

	//在注册表中写入启动信息
	HKEY hKey = NULL;
	LONG lRet = NULL;
	lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, SELFSTART_REGEDIT_PATH, 0, KEY_ALL_ACCESS, &hKey);

	//判断是否成功
	if (lRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	lRet = RegSetValueExA(hKey, "DaemonTool", 0, REG_SZ, (const unsigned char*)pName, strlen(pName) + sizeof(char));
    
	//判断是否成功
	if (lRet != ERROR_SUCCESS)
	{
		return FALSE;
	}

	//关闭注册表
	RegCloseKey(hKey);
	return TRUE;
}

int main()
{
	kill_exe_.KillProcess(_T("Daemon.exe"));
	Sleep(500);

	HWND hwnd = GetForegroundWindow();
	ShowWindow(hwnd, true);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	//获取程序完整名称
	char pName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, pName, MAX_PATH);
	std::string workingFolder(pName);
	size_t pos = workingFolder.find_last_of("\\");
	workingFolder = workingFolder.substr(0, pos + 1);

	//设置程序开机自启动
	if (!SetSelfStart())
	{
		cout << "守护进程开机自启动失败" << endl;
		return -1;
	}

	STARTUPINFOA si;
	//进程对象
	PROCESS_INFORMATION pi;
	//初始化
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//获取当前程序的路径
	char pPath[MAX_PATH] = { 0 };
	memcpy(pPath, workingFolder.c_str(), workingFolder.length());

	//拼接需要守护的程序
//	strcat_s(pPath, "");
	strcat_s(pPath, PROCCESS_NAME);

	//构造cmd执行守护进程的字符串
	char pCmd[MAX_PATH] = { 0 };
	strcat_s(pCmd, "cmd /c ");
	strcat_s(pCmd, pPath);
	cout << pCmd << endl;

	//无限循环，监视守护进程
	do{
		//检查守护程序是否存在
		if (_access(pPath, 0) != -1)
		{
			//创建子进程，判断是否执行成功
			if (!CreateProcessA(NULL, pCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				cout << "守护进程启动失败，程序即将退出" << endl;
				return -1;
			}
			//启动成功，获取进程的ID
			cout << "守护进程成功，ID:" << pi.dwProcessId << endl;
			cout << "人脸检测程序已启动，5秒后自动关闭窗口，请耐心等待！！！"<<endl;
			Sleep(500);
			for (int i = 0; i < 5; ++i)
			{
				cout << "********"<< 5 - i <<"秒后退出********"<< endl;
				Sleep(1000);
			}
			ShowWindow(hwnd, false);
			//无限等待子进程退出
			WaitForSingleObject(pi.hProcess, INFINITE);
			//如果退出了
			cout << "守护进程退出了。。。" << endl;
			//关闭进程和句柄
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else
		{
			cout << "守护程序不存在" << endl;
		}
		//睡一下，重启
		Sleep(5000);
	} while (true);

	return 0;
}