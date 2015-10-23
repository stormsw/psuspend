// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <Windows.h>
#include <TlHelp32.h>

typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);
enum prModes
{
	ModesUnknown,
	ModesThreadsSuspend,
	ModesThreadsResume,
	ModesNtSuspend,
	ModesNtResume,
	ModesDebugStart,
	ModesDebugResume
};

void thsuspend(DWORD processId);
void thresume(DWORD processId);
void debug(DWORD processId);
void resume(DWORD processId);
void ntresume(DWORD processId);
void ntsuspend(DWORD processId);
void printUsage(_TCHAR* argv[]);
bool validateProcessId(DWORD processId);
prModes getMode(_TCHAR* argv[]);
