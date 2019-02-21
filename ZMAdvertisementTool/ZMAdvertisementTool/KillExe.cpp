#include "KillExe.h"


KillExe::KillExe()
{
	
}


KillExe::~KillExe()
{
}

void KillExe::KillProcess(LPCWSTR processName)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD current_id = GetCurrentProcessId();


	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
	{
		return;
	}
	int process_num = 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
		{
			break;
		}
		//find processName
		if (_wcsicmp(pe.szExeFile, processName) == 0)
		{
			process_num++;
			id = pe.th32ProcessID;
			if (process_num >1)
			{
				//Kill The Process
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, current_id);
				if (hProcess != NULL)
				{
					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
				}
			}

		}
	}

	CloseHandle(hSnapshot);
}