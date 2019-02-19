#pragma once

#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>

class KillExe
{
public:
	KillExe();
	~KillExe();
	void KillProcess(LPCWSTR processName);
};

