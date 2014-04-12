/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#ifndef _LOG_H
#define _LOG_H
////////////////////////////////////////////////////////////////////////
typedef	enum
{
	LogAlways = 0,
	LogIfActive = 1,
	LogNormal = 2,
	LogDetails = 3,
	LogDetail = LogDetails,
	LogVerbose = 4,
	LogTime = 0x1000,
	LogDate = 0x2000,
	LogDateTime = LogDate|LogTime,
	LogTimeMs = LogTime|0x0100,
	LogDateTimeMs = LogDateTime|0x0100,
	LogTimeNs = LogTime|0x0200,
	LogDateTimeNs = LogDateTime|0x0200,
	LogNoPrefix = 0x4000,
	LogHighVolume = 0x8000,
	LogToCache = 0x01000000,
	LogTicks = 0x00010000,
	LogTicksDelta = 0x00030000,
	LogTicksDeltaRestart = 0x00070000,
	LogTicksDeltaPush = 0x00130000,
	LogTicksDeltaPop = 0x00230000,
	LogLevelMask = 0x000000FF,
	LogOptionsMask = 0x0000FF00,
	LogTicksMask = 0x00FF0000,
	LogSettingsMask = LogLevelMask|LogOptionsMask|LogToCache,
	LogDebug = LogNormal|LogNoPrefix,
	LogDebugFast = LogNormal|LogNoPrefix|LogHighVolume
}	LogLevel;

#if	defined (_M_CEE_PURE) || defined (_M_CEE_SAFE)
#ifndef	_LOG_DISABLED
#define	_LOG_DISABLED
#endif
#else	// _M_CEE_XXXX
#ifdef	__cplusplus
#ifndef	_LOG_NO_EXTERN_C
EXTERN_C {
#endif
#else
#include <tchar.h>
#endif

extern TCHAR FAR		gLogFileName [];
extern unsigned long	gLogLevel;
extern bool				gLogAllComErr;
extern bool				gLogAllWinErr;

#ifndef	_LOG_NOFILELINE
#define	BEGIN_LogErrFL(n)		struct __declspec(novtable) _Log##n##ErrFL {LPCSTR mFile; UINT mLine; __forceinline _Log##n##ErrFL (LPCSTR pFile, UINT pLine) : mFile (pFile), mLine (pLine) {}
#define FUNC_LogErrFL(t)		t LogErr (UINT pLogLevel, t pError, LPCTSTR pFormat = NULL, ...);
#define	END_LogErrFL()			};
#define DEFINE_LogErrFL(n,t)	BEGIN_LogErrFL(n) FUNC_LogErrFL(t) END_LogErrFL()
#endif

#ifdef	__cplusplus
#ifndef	_LOG_NO_EXTERN_C
}
#endif
#endif
#endif	// _M_CEE_XXXX

#ifdef	_LOG_DISABLED

static inline unsigned long LogStart (bool pNewLogFile = false, const void* pLogFileName = 0, unsigned int pLogLevel = 0) {return 0;}
static inline void LogStop (unsigned int pPutLogEnd = 15) {}
static inline void LogControl (void* pLogFileName, unsigned int& pLogLevel) {}
static inline bool LogLock () {return false;}
static inline bool LogUnlock () {return false;}
static inline bool LogIsActive (unsigned int pLogLevel = LogIfActive) {return false;}
static inline bool LogMessage (unsigned int pLogLevel, const void* pFormat, ...) {return true;}
static inline void LogWrite (const void* pStr, const void* pLogFileName = 0) {}
static inline int LogWriteCache (const void* pLogFileName = 0) {}
static inline bool LogEmptyCache () {return false;}
static inline bool LogDump (unsigned int pLogLevel, void* pBuffer, unsigned int pBufferSize, const void* pPrefix = 0, bool pDumpOffsets = false) {return true;}
static inline bool LogDumpBits (unsigned int pLogLevel, void* pBuffer, unsigned int pBufferSize, const void* pPrefix = 0, unsigned int pBytesPerLine = 8) {return true;}
static inline unsigned long LogWinErr (unsigned int pLogLevel, unsigned long pError, const void* pFormat = 0, ...) {return pError;}
static inline long LogComErr (unsigned int pLogLevel, long pError, const void* pFormat = 0, ...) {return pError;}
static inline void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false) {}
static inline int LogDebugMemory (int pDbgFlag = 0) {return 0;}
#define	LogWinErrAnon LogWinErr
#define	LogComErrAnon LogComErr

#else	// _LOG_DISABLED

#ifdef	__cplusplus
#ifndef	_LOG_NO_EXTERN_C
EXTERN_C {
#endif

DWORD LogStart (bool pNewLogFile = false, LPCTSTR pLogFileName = NULL, UINT pLogLevel = 0);
void LogStop (unsigned int pPutLogEnd = 15);
void LogControl (LPTSTR pLogFileName, UINT& pLogLevel);
bool LogLock ();
bool LogUnlock ();
bool LogIsActive (UINT pLogLevel = LogIfActive);
bool LogMessage (UINT pLogLevel, LPCTSTR pFormat, ...);
void LogWrite (LPCTSTR pStr, LPCTSTR pLogFileName = NULL);
int LogWriteCache (LPCTSTR pLogFileName = NULL);
bool LogEmptyCache ();
bool LogDump (UINT pLogLevel, LPCVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix = NULL, bool pDumpOffsets = false);
bool LogDumpWords (UINT pLogLevel, LPCVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix = NULL, bool pDumpOffsets = false);
bool LogDumpBits (UINT pLogLevel, LPCVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix = NULL, UINT pBytesPerLine = 8);
#ifdef	_CRTDBG_ALLOC_MEM_DF
void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false);
int LogDebugMemory (int pDbgFlag = _CRTDBG_ALLOC_MEM_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#else
static inline void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false) {}
static inline int LogDebugMemory (int pDbgFlag = 0) {return 0;}
#endif

#ifdef	_LOG_NOFILELINE
extern DWORD LogWinErr (UINT pLogLevel, DWORD pError, LPCTSTR pFormat = NULL, ...);
extern HRESULT LogComErr (UINT pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...);
#define	LogWinErrAnon LogWinErr
#define	LogComErrAnon LogComErr
#else
DEFINE_LogErrFL(Win,DWORD)
DEFINE_LogErrFL(Com,HRESULT)
#define	LogWinErr _LogWinErrFL(__FILE__,__LINE__).LogErr
#define	LogComErr _LogComErrFL(__FILE__,__LINE__).LogErr
#define	LogWinErrAnon _LogWinErrFL(NULL,0).LogErr
#define	LogComErrAnon _LogComErrFL(NULL,0).LogErr
#endif

#ifndef	_LOG_NO_EXTERN_C
}
#endif
#else	// __cplusplus

extern DWORD LogStart (int pNewLogFile = false, LPCTSTR pLogFileName = NULL, UINT pLogLevel = 0);
extern void LogStop (unsigned int pPutLogEnd = 15);
extern void LogControl (LPTSTR pLogFileName, UINT& pLogLevel);
extern bool LogLock ();
extern bool LogUnlock ();
extern int LogIsActive (unsigned int pLogLevel = LogIfActive);
extern bool LogMessage (unsigned int pLogLevel, LPCTSTR pFormat, ...);
extern void LogWrite (LPCTSTR pStr, LPCTSTR pLogFileName = NULL);
extern int LogWriteCache (LPCTSTR pLogFileName = NULL);
extern bool LogEmptyCache ();
extern bool LogDump (unsigned int pLogLevel, LPVOID pBuffer, unsigned int pBufferSize, int pDumpOffsets);
extern unsigned long LogWinErr (unsigned int pLogLevel, unsigned long pError, LPCTSTR pFormat = NULL, ...);
extern HRESULT LogComErr (unsigned int pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...);
#ifdef	_CRTDBG_ALLOC_MEM_DF
extern void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false);
extern int LogDebugMemory (int pDbgFlag = _CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#else
static inline void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false) {}
static inline int LogDebugMemory (int pDbgFlag = 0) {return 0;}
#endif

#ifndef	_LOG_NOFILELINE
#define	_LOG_NOFILELINE
#endif

#endif	// __cplusplus

#ifdef	__AFX_H__
static inline void LogMfcException (UINT pLogLevel, CException * pException, LPCSTR pFile, UINT pLine)
{
	try
	{
		if	(LogIsActive (pLogLevel))
		{
			TCHAR	lMsg [1024];
			memset (lMsg, 0, sizeof (lMsg));
			pException->GetErrorMessage (lMsg, (sizeof (lMsg)/sizeof (TCHAR))-1);
		    while ((lMsg [_tcslen (lMsg)-1] == _T('\n')) || (lMsg [_tcslen (lMsg)-1] == _T('\r'))) lMsg [_tcslen (lMsg)-1] = 0;
			LogMessage (pLogLevel, _T("Exception [%hs] [%s] at %hs %d"), pException->GetRuntimeClass ()->m_lpszClassName, lMsg, pFile, pLine);
		}
	}
	catch (...)
	{}
}
#endif

#ifdef	__ATLEXCEPT_H__
static inline void LogAtlException (UINT pLogLevel, ATL::CAtlException & pException, LPCSTR pFile, UINT pLine)
{
	try
	{
		if	(LogIsActive (pLogLevel))
		{
			LPTSTR lMessage = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, (HRESULT)pException, 0, (LPTSTR) &lMessage, 0, NULL);
			if	(lMessage)
			{
				while ((lMessage [_tcslen (lMessage)-1] == _T('\n')) || (lMessage [_tcslen (lMessage)-1] == _T('\r'))) lMessage [_tcslen (lMessage)-1] = 0;
			}
			LogMessage (pLogLevel, _T("Exception [%8.8X] [%s] at %hs %d"), (HRESULT)pException, lMessage, pFile, pLine);
			if	(lMessage)
			{
				LocalFree (lMessage);
			}
		}
	}
	catch (...)
	{}
}
#endif

#ifdef	__cplusplus_cli
#pragma managed(push,on)
static inline void LogCliException (UINT pLogLevel, System::Exception^ pException, LPCSTR pFile, UINT pLine)
{
	try
	{
		if	(LogIsActive (pLogLevel))
		{
			System::Reflection::ReflectionTypeLoadException^	lTypeLoadException;
			cli::pin_ptr<const System::Char>					lMsg;

			lMsg = PtrToStringChars (pException->ToString());
			LogMessage (pLogLevel, _T("Exception [%s] at %hs %d"), (const System::Char*)lMsg, pFile, pLine);

			try
			{
				lTypeLoadException = safe_cast <System::Reflection::ReflectionTypeLoadException^> (pException);
			}
			catch (...)
			{}
			if	(
					(lTypeLoadException != nullptr)
				&&	(lTypeLoadException->LoaderExceptions != nullptr)
				)
			{
				System::Exception^	lException;

				for each (lException in lTypeLoadException->LoaderExceptions)
				{
					lMsg = PtrToStringChars (lException->ToString());
					LogMessage (pLogLevel, _T("Exception [%s] at %hs %d"), (const System::Char*)lMsg, pFile, pLine);
				}
			}
		}
	}
	catch (...)
	{}
}
#pragma managed(pop)
#endif
#endif	// _LOG_DISABLED

////////////////////////////////////////////////////////////////////////

inline unsigned int MinLogLevel (unsigned int a, unsigned int b)
{
	return (((a & 0x00FF) <= (b & 0x00FF)) ? a : b) | (a & 0xFFFFFF00) | (b & 0xFFFFFF00);
}

inline unsigned int MaxLogLevel (unsigned int a, unsigned int b)
{
	return (((a & 0x00FF) >= (b & 0x00FF)) ? a : b) | (a & 0xFFFFFF00) | (b & 0xFFFFFF00);
}

////////////////////////////////////////////////////////////////////////
#ifdef	__AFXWIN_H__

static void SetRegistryKeyEx (LPCTSTR pRegistryKey, LPCTSTR pProfileName)
{
	CWinApp *	lApp = AfxGetApp ();
	BOOL		lMemoryTracking;

	lMemoryTracking = AfxEnableMemoryTracking (FALSE);
	::free ((void*) lApp->m_pszRegistryKey);
	lApp->m_pszRegistryKey = _tcsdup (pRegistryKey);
	::free ((void*) lApp->m_pszProfileName);
	lApp->m_pszProfileName = _tcsdup (pProfileName);
	AfxEnableMemoryTracking (lMemoryTracking);
}

#endif	//	__AFXWIN_H__
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_H
