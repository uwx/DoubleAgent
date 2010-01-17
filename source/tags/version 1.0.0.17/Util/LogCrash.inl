/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_DISABLED
#ifndef	_VISTA
#ifndef	_LOG_CRASH_NOSTACK
#define	_LOG_CRASH_NOSTACK			
#endif
#endif
#if	(defined _LOG_CRASH_NOSTACK) && (defined _LOG_CRASH_NOMODULES) && (defined _LOG_CRASH_NOMINIDUMP) 
#ifndef	_LOG_CRASH_NODBGHELP
#define	_LOG_CRASH_NODBGHELP			
#endif
#endif
#ifdef	_LOG_CRASH_NODBGHELP
#ifndef	_LOG_CRASH_NOMODULES
#define	_LOG_CRASH_NOMODULES			
#endif
#ifndef	_LOG_CRASH_NOMINIDUMP
#define	_LOG_CRASH_NOMINIDUMP			
#endif
#endif
#define	_LOG_CRASH_DUMPNONE		0
#define	_LOG_CRASH_DUMPMICRO	1
#define	_LOG_CRASH_DUMPMINI		2
#define	_LOG_CRASH_DUMPFULL		3
#ifndef	_LOG_CRASH_DUMPLEVEL
#define	_LOG_CRASH_DUMPLEVEL	_LOG_CRASH_DUMPMINI
#endif
#ifdef	_WIN64
#ifndef	_IMAGEHLP64
#define	_IMAGEHLP64
#endif
#endif
////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_CRASH_NODESCRIPTION
////////////////////////////////////////////////////////////////////////
static void _LogCrash_LogDescription (unsigned int pCode)
{
	LPCTSTR	lCodeName = NULL;
	
	switch (pCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:			lCodeName = _T("ACCESS_VIOLATION"); break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:		lCodeName = _T("DATATYPE_MISALIGNMENT"); break;
		case EXCEPTION_BREAKPOINT:					lCodeName = _T("BREAKPOINT"); break;
		case EXCEPTION_SINGLE_STEP:					lCodeName = _T("SINGLE_STEP"); break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		lCodeName = _T("ARRAY_BOUNDS_EXCEEDED"); break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:		lCodeName = _T("FLT_DENORMAL_OPERAND"); break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:			lCodeName = _T("FLT_DIVIDE_BY_ZERO"); break;
		case EXCEPTION_FLT_INEXACT_RESULT:			lCodeName = _T("FLT_INEXACT_RESULT"); break;
		case EXCEPTION_FLT_INVALID_OPERATION:		lCodeName = _T("FLT_INVALID_OPERATION"); break;
		case EXCEPTION_FLT_OVERFLOW:				lCodeName = _T("FLT_OVERFLOW"); break;
		case EXCEPTION_FLT_STACK_CHECK:				lCodeName = _T("FLT_STACK_CHECK"); break;
		case EXCEPTION_FLT_UNDERFLOW:				lCodeName = _T("FLT_UNDERFLOW"); break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:			lCodeName = _T("INT_DIVIDE_BY_ZERO"); break;
		case EXCEPTION_INT_OVERFLOW:				lCodeName = _T("INT_OVERFLOW"); break;
		case EXCEPTION_PRIV_INSTRUCTION:			lCodeName = _T("PRIV_INSTRUCTION"); break;
		case EXCEPTION_IN_PAGE_ERROR:				lCodeName = _T("IN_PAGE_ERROR"); break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:			lCodeName = _T("ILLEGAL_INSTRUCTION"); break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:	lCodeName = _T("NONCONTINUABLE_EXCEPTION"); break;
		case EXCEPTION_STACK_OVERFLOW:				lCodeName = _T("STACK_OVERFLOW"); break;
		case EXCEPTION_INVALID_DISPOSITION:			lCodeName = _T("INVALID_DISPOSITION"); break;
		case EXCEPTION_GUARD_PAGE:					lCodeName = _T("GUARD_PAGE"); break;
		case EXCEPTION_INVALID_HANDLE:				lCodeName = _T("INVALID_HANDLE"); break;
	}
	LogMessage (LogAlways, _T("*** Exception [%8.8X] [%s] ***"), pCode, lCodeName);
}
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_CRASH_NODESCRIPTION 
#ifndef	_LOG_CRASH_NOEXCEPTION
////////////////////////////////////////////////////////////////////////
static void _LogCrash_LogException (struct _EXCEPTION_POINTERS * pException)
{
	if	(
			(pException)
		&&	(pException->ExceptionRecord)
		)
	{
		PEXCEPTION_RECORD	lExceptionRecord;
		int					lIndent = 6;
		
		for	(lExceptionRecord = pException->ExceptionRecord; lExceptionRecord; lExceptionRecord = (PEXCEPTION_RECORD) lExceptionRecord->ExceptionRecord)
		{		
			LogMessage (LogAlways, _T("%*cFlags   [%8.8X]"), lIndent, _T(' '), lExceptionRecord->ExceptionFlags);
			LogMessage (LogAlways, _T("%*cAddress [%p]"), lIndent, _T(' '), lExceptionRecord->ExceptionAddress);

			if	(
					(lExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
				&&	(lExceptionRecord->NumberParameters > 1)
				)
			{
				LogMessage (LogAlways, _T("%*cAddress [%p] access [%I64u]"), lIndent, _T(' '), lExceptionRecord->ExceptionInformation [1], lExceptionRecord->ExceptionInformation [0]);
			}
			lIndent += 2;
		}
	}
}
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_CRASH_NOEXCEPTION 
#ifndef	_LOG_CRASH_NOSTACK			
////////////////////////////////////////////////////////////////////////
typedef struct 
{
	PVOID	mStackFrame [64];
	PVOID	mStackFrameBase [64];
	char	mFrameModule [64] [MAX_PATH];
	UINT	mFrameCount;
}	_LogCrash_Stack;
#ifdef	_WIN64
#define	_LOG_CRASH_STACK_SKIP_FRAMES	7
#endif
////////////////////////////////////////////////////////////////////////
static void _LogCrash_DumpStack (_LogCrash_Stack & pStack)
{
	UINT	lStackFrameNdx;

#ifdef	_WIN64
	LogMessage (LogAlways, _T("    %u of %u Stack frames"), pStack.mFrameCount, pStack.mFrameCount+_LOG_CRASH_STACK_SKIP_FRAMES);
#else
	LogMessage (LogAlways, _T("    %u Stack frames"), pStack.mFrameCount);
#endif	
	for	(lStackFrameNdx = 0; lStackFrameNdx < pStack.mFrameCount; lStackFrameNdx++)
	{
		PVOID	lStackFrameBase = pStack.mStackFrameBase [lStackFrameNdx];
#ifdef	_WIN64
		PVOID	lFileHeaderBase = NULL;

		if	(
				(lStackFrameBase == NULL)
			&&	(RtlPcToFileHeader (pStack.mStackFrame [lStackFrameNdx], &lFileHeaderBase))
			)
		{
			lStackFrameBase = pStack.mStackFrameBase [lStackFrameNdx] = lFileHeaderBase;
		}
#endif			
		if	(lStackFrameBase)
		{
			LogMessage (LogAlways, _T("      [%p] (base [%p] offset [%p] module [%hs])"), pStack.mStackFrame [lStackFrameNdx], lStackFrameBase, (ULONG_PTR)pStack.mStackFrame [lStackFrameNdx]-(ULONG_PTR)lStackFrameBase, pStack.mFrameModule [lStackFrameNdx]);	
		}
		else
		{
			LogMessage (LogAlways, _T("      [%p]"), pStack.mStackFrame [lStackFrameNdx]);	
		}
	}

}
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_CRASH_NOSTACK 
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_CRASH_NODBGHELP
////////////////////////////////////////////////////////////////////////
#include <dbghelp.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
typedef BOOL (__stdcall * tEnumerateLoadedModules) (HANDLE, PENUMLOADED_MODULES_CALLBACK, PVOID);
typedef BOOL (WINAPI * tMiniDumpWriteDump) (HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION);

static HMODULE _LogCrash_LoadDbgHelp ()
{
	HINSTANCE	lModule = NULL;
	UINT		lErrMode = SetErrorMode (SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX);
	__try
	{
		lModule = ::LoadLibrary (_T("DBGHELP.DLL"));
	}
	__finally
	{
		SetErrorMode (lErrMode);
	}
	return lModule;
}
////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_CRASH_NOMODULES
static BOOL CALLBACK _LogCrash_LogLoadedModules (PCSTR pModuleName, ULONG_PTR pModuleBase, ULONG pModuleSize, PVOID pUserContext)
{
	LogMessage (LogAlways, _T("      Module [%p] [%hs] (%u)"), pModuleBase, pModuleName, pModuleSize);
	return TRUE;
}
#endif	// _LOG_CRASH_NOMODULES	
////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_CRASH_NOSTACK
static BOOL CALLBACK _LogCrash_GetStackModules (PCSTR pModuleName, ULONG_PTR pModuleBase, ULONG pModuleSize, PVOID pUserContext)
{
	_LogCrash_Stack *	lStack = (_LogCrash_Stack *) pUserContext;
	UINT				lStackFrameNdx;

	for	(lStackFrameNdx = 0; lStackFrameNdx < lStack->mFrameCount; lStackFrameNdx++)
	{
		if	(
				(pModuleBase <= (ULONG_PTR) lStack->mStackFrame [lStackFrameNdx])
			&&	(
					(lStack->mStackFrameBase [lStackFrameNdx] == 0)
				||	(pModuleBase > (ULONG_PTR) lStack->mStackFrameBase [lStackFrameNdx])
				)
			)
		{
			lStack->mStackFrameBase [lStackFrameNdx] = (PVOID) pModuleBase;
			strncpy (lStack->mFrameModule [lStackFrameNdx], pModuleName, MAX_PATH);
		}
	}
	return TRUE;
}
#endif	// _LOG_CRASH_NOSTACK
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_CRASH_NOMINIDUMP
////////////////////////////////////////////////////////////////////////
static int _LogCrash_MiniDumpLevel ()
{
	int		lDumpLevel = _LOG_CRASH_DUMPLEVEL;
	HKEY	lRegKey = 0;
	DWORD	lValueSize;
	DWORD	lValueType;
	DWORD	lValue;

	if	(RegOpenKeyEx (_LOG_ROOT_KEY, _LOG_ROOT_PATH _LOG_SECTION_NAME, 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
	{
		if	(
				(RegQueryValueEx (lRegKey, _T("CrashDump"), NULL, &(lValueType = 0), (LPBYTE)&lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
			&&	(lValueType == REG_DWORD)
			)
		{
			lDumpLevel = lValue;
		}
		RegCloseKey (lRegKey);
	}
	return lDumpLevel;
}
////////////////////////////////////////////////////////////////////////
static void _LogCrash_MiniDump (HMODULE pDbgHelp, struct _EXCEPTION_POINTERS * pException) 
{
	tMiniDumpWriteDump	lMiniDumpWriteDump;
	TCHAR				lDumpFilePath [MAX_PATH];
	TCHAR				lDumpFileName [MAX_PATH];
	HANDLE				lDumpFile = INVALID_HANDLE_VALUE;
	int					lDumpLevel;

	lDumpLevel = _LogCrash_MiniDumpLevel ();

	if	(
			(lDumpLevel != _LOG_CRASH_DUMPNONE)
		&&	(pDbgHelp)
		&&	(lMiniDumpWriteDump = (tMiniDumpWriteDump) ::GetProcAddress (pDbgHelp, "MiniDumpWriteDump"))
		)
	{
		_tcsncpy (lDumpFilePath, gLogFileName, MAX_PATH);
		PathRemoveFileSpec (lDumpFilePath);
		GetModuleFileName (NULL, lDumpFileName, MAX_PATH);
		PathStripPath (lDumpFileName);
		PathRemoveExtension (lDumpFileName);
		PathAddExtension (lDumpFileName, _T(".mdmp")); 
		PathAppend (lDumpFilePath, lDumpFileName);

		lDumpFile = CreateFile (lDumpFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, _LOG_FILE_SECURITY, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|_LOG_FILE_FLAGS, NULL);
		if	(lDumpFile != INVALID_HANDLE_VALUE)
		{
			__try
			{
				MINIDUMP_TYPE					lDumpType = MiniDumpNormal;
				MINIDUMP_EXCEPTION_INFORMATION	lExceptionInfo;
			
				memset (&lExceptionInfo, 0, sizeof(lExceptionInfo)); 
				lExceptionInfo.ThreadId = GetCurrentThreadId ();
				lExceptionInfo.ExceptionPointers = pException;
				lExceptionInfo.ClientPointers = FALSE;
				
				if	(lDumpLevel == _LOG_CRASH_DUMPMICRO)
				{
					lDumpType = (MINIDUMP_TYPE)(MiniDumpWithDataSegs|MiniDumpWithUnloadedModules|MiniDumpWithProcessThreadData|MiniDumpWithoutOptionalData);
				}
				else
				if	(lDumpLevel == _LOG_CRASH_DUMPMINI)
				{
					lDumpType = (MINIDUMP_TYPE)(MiniDumpFilterMemory|MiniDumpFilterModulePaths);
				}
				else
				{
					lDumpType = (MINIDUMP_TYPE)(MiniDumpWithDataSegs|MiniDumpWithUnloadedModules|MiniDumpWithProcessThreadData|MiniDumpWithHandleData|MiniDumpWithFullMemoryInfo);
				}
				
				if	((*lMiniDumpWriteDump) (GetCurrentProcess(), GetCurrentProcessId(), lDumpFile, (MINIDUMP_TYPE)(MiniDumpNormal|MiniDumpWithDataSegs|MiniDumpWithUnloadedModules), &lExceptionInfo, NULL, NULL))
				{
					LogMessage (LogAlways, _T("    MiniDump [%s]"), lDumpFilePath);
				}
				else
				{
					LogMessage (LogAlways, _T("    MiniDump FAILED [%s]"), lDumpFilePath);
				}
			}
			__finally
			{		
				CloseHandle (lDumpFile);
			}
		} 
	}
}
#endif	// _LOG_CRASH_NOMINIDUMP
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_CRASH_NODBGHELP
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
static int LogCrash (unsigned int pCode, struct _EXCEPTION_POINTERS * pException, int pAction = EXCEPTION_CONTINUE_SEARCH)
{
	__try
	{
		if	(LogIsActive())
		{
#ifndef	_LOG_CRASH_NOSTACK			
			_LogCrash_Stack	lStack;

			if	(pCode != EXCEPTION_STACK_OVERFLOW)
			{			
				memset (&lStack, 0, sizeof(_LogCrash_Stack));
#ifdef	_WIN64
				lStack.mFrameCount = RtlCaptureStackBackTrace (_LOG_CRASH_STACK_SKIP_FRAMES, 64, lStack.mStackFrame, NULL);
#else
				lStack.mFrameCount = RtlCaptureStackBackTrace (0, 64, lStack.mStackFrame, NULL);
#endif				
			}
#endif
			if	(gLogLevel & LogToCache)
			{
				LogWriteCache ();
			}
#ifdef	_LOG_CRASH_NODESCRIPTION		
			LogMessage (LogAlways, _T("*** Exception [%8.8X] ***"), pCode);
#else
			_LogCrash_LogDescription (pCode);
#endif		
#ifndef	_LOG_CRASH_NOEXCEPTION
			_LogCrash_LogException (pException);
#endif		
			if	(pCode != EXCEPTION_STACK_OVERFLOW)
			{			
#ifndef	_LOG_CRASH_NODBGHELP
				HMODULE					lDbgHelp = _LogCrash_LoadDbgHelp ();
				tEnumerateLoadedModules	lDbgEnumModules = NULL;
				if	(lDbgHelp)
				{
					lDbgEnumModules = (tEnumerateLoadedModules) ::GetProcAddress (lDbgHelp, "EnumerateLoadedModules");
				}
#endif

#ifndef	_LOG_CRASH_NOSTACK			
#ifndef	_LOG_CRASH_NODBGHELP
				if	(lDbgEnumModules)
				{
					(*lDbgEnumModules) (GetCurrentProcess(), _LogCrash_GetStackModules, &lStack);
				}
#endif
				_LogCrash_DumpStack (lStack);
#endif
#ifndef	_LOG_CRASH_NOMODULES
				if	(lDbgEnumModules)
				{
					LogMessage (LogAlways, _T("    Modules"));
					(*lDbgEnumModules) (GetCurrentProcess(), _LogCrash_LogLoadedModules, NULL);
				}
#endif		
#ifndef	_LOG_CRASH_NOMINIDUMP
				_LogCrash_MiniDump (lDbgHelp, pException);
#endif
#ifndef	_LOG_CRASH_NODBGHELP
				if	(lDbgHelp)
				{
					::FreeLibrary (lDbgHelp);
				}
#endif				
			}
			
			LogMessage (LogAlways, _T("*** Exception End ***"));
			if	(gLogLevel & LogToCache)
			{
				LogWriteCache ();
			}
		}		
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{;}
	
	return pAction;
}
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_DISABLED
////////////////////////////////////////////////////////////////////////
