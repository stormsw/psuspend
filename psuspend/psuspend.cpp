// psuspend.cpp : Defines the entry point for the console application.

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 2)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nArguments count: %d\n"), argc);
#endif
		prModes mode = getMode(argv);
		
		if (mode == ModesUnknown)
		{
			printUsage(argv);
			return 1;
		}

		for (int i = 2; i < argc; i++)
		{
			DWORD pid = wcstol(argv[i], _T('\0'), 10);
			_tprintf(TEXT("\nPassed process id is %d\n"), pid);
			if (validateProcessId(pid)){
				switch (mode)
				{
				case ModesUnknown:
					break;
				case ModesThreadsSuspend:
					thsuspend(pid);
					break;
				case ModesThreadsResume:
					thresume(pid);
					break;
				case ModesNtSuspend:
					ntsuspend(pid);
					break;
				case ModesNtResume:
					ntresume(pid);
					break;
				case ModesDebugStart:
					debug(pid);
					break;
				case ModesDebugResume:
					resume(pid);
					break;
				default:
					break;
				}
			}
			else {
				//wrong pid
			}
		}

	}
	else
	{
		printUsage(argv);
	}

	return 0;
}

void thsuspend(DWORD processId)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);

			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}

void thresume(DWORD processId)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	Thread32First(hThreadSnapshot, &threadEntry);

	do
	{
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
				threadEntry.th32ThreadID);
			ResumeThread(hThread);
			//SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} while (Thread32Next(hThreadSnapshot, &threadEntry));

	CloseHandle(hThreadSnapshot);
}

// starting from XP
void ntsuspend(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle(TEXT("ntdll")), "NtSuspendProcess");

	pfnNtSuspendProcess(processHandle);
	CloseHandle(processHandle);
}

void ntresume(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(
		GetModuleHandle(TEXT("ntdll")), "NtResumeProcess");

	pfnNtResumeProcess(processHandle);
	CloseHandle(processHandle);
}


void debug(DWORD processId)
{
	DebugActiveProcess(processId);
}

void resume(DWORD processId)
{
	DebugActiveProcessStop(processId);
}

prModes getMode(_TCHAR* argv[])
{
	prModes mode = ModesUnknown;

#ifdef _DEBUG
	_tprintf(TEXT("\nProcessing params: %s\n"), __FUNCTIONW__);
#endif

	if (_tcsicmp(argv[1], TEXT("-tr")) == 0)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nMode selected: threads resume\n"));
#endif
		mode = ModesThreadsResume;
	}
	else 	if (_tcsicmp(argv[1], TEXT("-ts")) == 0)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nMode selected: threads suspend\n"));
#endif
		mode = ModesThreadsSuspend;
	}
	else 	if (_tcsicmp(argv[1], TEXT("-ns")) == 0)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nMode selected: NT suspend\n"));
#endif
		mode = ModesNtSuspend;
	}
	else 	if (_tcsicmp(argv[1], TEXT("-nr")) == 0)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nMode selected: NT resume\n"));
#endif
		mode = ModesNtResume;
	}
	else 	if (_tcsicmp(argv[1], TEXT("-dr")) == 0)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nMode selected: Debug resume\n"));
#endif
		mode = ModesDebugResume;
	}
	else 	if (_tcsicmp(argv[1], TEXT("-ds")) == 0)
	{
#ifdef _DEBUG
		_tprintf(TEXT("\nMode selected: Debug start\n"));
#endif
		mode = ModesDebugStart;
	}

	return mode;
}

bool validateProcessId(DWORD processId)
{
	// TODO: get proc lis and chek pid, status, security info
	return processId > 0;
}

void printUsage(_TCHAR* argv[])
{
	_tprintf(TEXT("\n\n====================================================="));
	_tprintf(TEXT("\nProcess suspend/resume tool"));
	_tprintf(TEXT("\nUsage:  %s <s|r> <pid1> <pid2> ... "), argv[0]);
	_tprintf(TEXT("\n\t -ts:  Threads suspend"));
	_tprintf(TEXT("\n\t -tr:  Threads resume"));
	_tprintf(TEXT("\n\t -ns:  NT suspend"));
	_tprintf(TEXT("\n\t -nr:  NT resume"));
	_tprintf(TEXT("\n\t -ds:  Debug start"));
	_tprintf(TEXT("\n\t -dr:  Debug resume"));
	_tprintf(TEXT("\n-------------------------------------------------------"));

}
