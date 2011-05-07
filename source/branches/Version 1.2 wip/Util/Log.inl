/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
/////////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_DISABLED
#ifdef __cplusplus
#ifndef	_LOG_NO_EXTERN_C
EXTERN_C {
#endif
#endif
/////////////////////////////////////////////////////////////////////////////

TCHAR			gLogFileName [MAX_PATH*2] = _T("");
unsigned long	gLogLevel = 0;
bool			gLogAllComErr = false;
bool			gLogAllWinErr = false;

/////////////////////////////////////////////////////////////////////////////
#ifndef _LOG_ROOT_KEY
#define	_LOG_ROOT_KEY HKEY_CURRENT_USER
#endif
#ifndef _LOG_ROOT_PATH
#define	_LOG_ROOT_PATH _T("Software\\Cinnamon\\")
#endif
#ifndef	_LOG_FILE_FLAGS
#define	_LOG_FILE_FLAGS FILE_FLAG_WRITE_THROUGH
#endif
#ifndef	_LOG_FILE_SECURITY
#define	_LOG_FILE_SECURITY NULL
#endif
#ifndef	_LOG_FILE_SHARED
#define _LOG_FILE_SHARED FALSE
#endif
/////////////////////////////////////////////////////////////////////////////
#if (_MSC_VER >= 1400)
#ifndef	_CPPUNWIND
#define	_CPPUNWIND
#endif
#endif

#ifdef	_CPPUNWIND
#define	_LOG_TRY	try
#define _LOG_CATCH	catch (...) {}
#else
#define	_LOG_TRY
#define	_LOG_CATCH
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	_LOG_CRITICAL_SECTION
#ifdef	_DEBUG_LOG_CRITICAL_SECTION
#define	_LOG_ENTER_CRITICAL_SECTION \
		_LOG_TRY \
		{ \
			TCHAR lDebugStr [200]; \
			_stprintf (lDebugStr, _T("EnterCritical [%6.6lX %6.6lX] (LogWrite)\n"), GetCurrentProcessId () & 0x00FFFFFF, GetCurrentThreadId () & 0x00FFFFFF); \
			OutputDebugString (lDebugStr); \
			if	(_LOG_CRITICAL_SECTION != NULL) \
			{ \
				EnterCriticalSection (_LOG_CRITICAL_SECTION); \
			} \
			_stprintf (lDebugStr, _T("  Entered     [%6.6lX %6.6lX] (LogWrite)\n"), GetCurrentProcessId () & 0x00FFFFFF, GetCurrentThreadId () & 0x00FFFFFF); \
			OutputDebugString (lDebugStr); \
		} \
		_LOG_CATCH
#else
#define	_LOG_ENTER_CRITICAL_SECTION \
		_LOG_TRY \
		{ \
			if	(_LOG_CRITICAL_SECTION != NULL) \
			{ \
				EnterCriticalSection (_LOG_CRITICAL_SECTION); \
			} \
		} \
		_LOG_CATCH
#endif
#ifdef	_DEBUG_LOG_CRITICAL_SECTION
#define	_LOG_EXIT_CRITICAL_SECTION \
		_LOG_TRY \
		{ \
			TCHAR lDebugStr [200]; \
			_stprintf (lDebugStr, _T("LeaveCritical [%6.6lX %6.6lX] (LogWrite)\n"), GetCurrentProcessId () & 0x00FFFFFF, GetCurrentThreadId () & 0x00FFFFFF); \
			OutputDebugString (lDebugStr); \
			if	(_LOG_CRITICAL_SECTION != NULL) \
			{ \
				LeaveCriticalSection (_LOG_CRITICAL_SECTION); \
			} \
			_stprintf (lDebugStr, _T("  Left        [%6.6lX %6.6lX] (LogWrite)\n"), GetCurrentProcessId () & 0x00FFFFFF, GetCurrentThreadId () & 0x00FFFFFF); \
			OutputDebugString (lDebugStr); \
		} \
		_LOG_CATCH
#else
#define	_LOG_EXIT_CRITICAL_SECTION \
		_LOG_TRY \
		{ \
			if	(_LOG_CRITICAL_SECTION != NULL) \
			{ \
				LeaveCriticalSection (_LOG_CRITICAL_SECTION); \
			} \
		} \
		_LOG_CATCH
#endif
#else
#define	_LOG_ENTER_CRITICAL_SECTION
#define	_LOG_EXIT_CRITICAL_SECTION
#endif
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
class __OsHandle
{
public:
	__OsHandle ()
	:	mHandle (INVALID_HANDLE_VALUE)
	{
		mConstructed = &mHandle;
	}
	~__OsHandle ()
	{
		Close ();
#ifdef	_UNICODE
		InterlockedExchangePointer ((LPVOID*)&mConstructed, NULL);
#else
		mConstructed = NULL;
#endif
	}
	bool IsValid () const
	{
		return (mConstructed == &mHandle);
	}
	bool IsOpen () const
	{
		return ((mConstructed == &mHandle) && (mHandle != INVALID_HANDLE_VALUE));
	}
	void Close ()
	{
#ifdef	_UNICODE
		PHANDLE lConstructed = (PHANDLE) InterlockedExchangePointer ((LPVOID*)&mConstructed, NULL);
		if	(lConstructed == &mHandle)
#else
		if	(mConstructed == &mHandle)
#endif
		{
			_LOG_TRY
			{
#ifdef	_UNICODE
				HANDLE lHandle = (HANDLE) InterlockedExchangePointer (&mHandle, INVALID_HANDLE_VALUE);
				if	(lHandle != INVALID_HANDLE_VALUE)
				{
					CloseHandle (lHandle);
				}
#else
				if	(mHandle != INVALID_HANDLE_VALUE)
				{
					HANDLE lHandle = mHandle;
					mHandle = INVALID_HANDLE_VALUE;
					CloseHandle (lHandle);
				}
#endif
			}
			_LOG_CATCH
		}
#ifdef	_UNICODE
		InterlockedExchangePointer ((LPVOID*)&mConstructed, lConstructed);
#endif
	}

	HANDLE Attach (HANDLE pHandle, bool pFailIfOpen = true, bool pCloseOnFail = true)
	{
		HANDLE	lRet = INVALID_HANDLE_VALUE;

		if	(pHandle != INVALID_HANDLE_VALUE)
		{
#ifdef	_UNICODE
			PHANDLE lConstructed = (PHANDLE) InterlockedExchangePointer ((LPVOID*)&mConstructed, NULL);
			if	(lConstructed == &mHandle)
#else
			if	(mConstructed == &mHandle)
#endif
			{
				_LOG_TRY
				{
#ifdef	_UNICODE
					HANDLE lHandle = (HANDLE) InterlockedExchangePointer (&mHandle, INVALID_HANDLE_VALUE);
#else
					HANDLE lHandle = mHandle;
#endif
					if	(
							(pFailIfOpen)
						&&	(lHandle != INVALID_HANDLE_VALUE)
						)
					{
						lRet = INVALID_HANDLE_VALUE;
					}
					else
					{
						if	(lHandle != INVALID_HANDLE_VALUE)
						{
							CloseHandle (lHandle);
						}
#ifdef	_UNICODE
						lRet = lHandle = pHandle;
#else
						lRet = mHandle = pHandle;
#endif
					}
#ifdef	_UNICODE
					InterlockedExchangePointer (&mHandle, lHandle);
#endif
				}
				_LOG_CATCH
			}
#ifdef	_UNICODE
			InterlockedExchangePointer ((LPVOID*)&mConstructed, lConstructed);
#endif
			if	(
					(pCloseOnFail)
				&&	(lRet == INVALID_HANDLE_VALUE)
				)
			{
				_LOG_TRY
				{
					CloseHandle (pHandle);
				}
				_LOG_CATCH
			}
		}
		return lRet;
	}

public:
	HANDLE		mHandle;
private:
	HANDLE *	mConstructed;
};
static __OsHandle sLogFileHandle;
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool LogIsActive (UINT pLogLevel)
{
	if	(
			(pLogLevel & LogToCache)
		||	(
				((pLogLevel & LogLevelMask) <= (gLogLevel & LogLevelMask))
			&&	(gLogFileName [0])
			)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	_WIN32
/////////////////////////////////////////////////////////////////////////////

static void _LogWriteStr (HANDLE pFile, LPCTSTR pStr)
{
	DWORD	lWritten = 0;
#ifdef	_UNICODE
	int		lStrSize;
	char	lFixedStr [2048];
	char *	lDynStr = NULL;
	char *	lAnsiStr = NULL;

	if	(
			(lStrSize = WideCharToMultiByte (CP_ACP, 0, pStr, -1, NULL, 0, NULL, NULL))
		&&	(
				(lStrSize <= sizeof (lFixedStr))
			?	(lAnsiStr = lFixedStr)
			:	(lAnsiStr = lDynStr = new char [lStrSize])
			)
		&&	(WideCharToMultiByte (CP_ACP, 0, pStr, -1, lAnsiStr, lStrSize, NULL, NULL))
		)
	{
		WriteFile (pFile, lAnsiStr, (DWORD) strlen (lAnsiStr), &lWritten, NULL);
	}

	if	(lDynStr)
	{
#if (_MSC_VER >= 1400)
		delete lDynStr;
#else
		delete [] lDynStr;
#endif
	}
#else
	WriteFile (pFile, pStr, _tcslen (pStr) * sizeof (TCHAR), &lWritten, NULL);
#endif
}

/////////////////////////////////////////////////////////////////////////////

static VOID CALLBACK _LogCloseFileAsync (HWND pWinHandle, UINT pMsg, UINT_PTR pEventId, DWORD pTime)
{
	_LOG_TRY
	{
		::KillTimer (pWinHandle, pEventId);
	}
	_LOG_CATCH

	_LOG_TRY
	{
		_LOG_ENTER_CRITICAL_SECTION

		_LOG_TRY
		{
			sLogFileHandle.Close ();
		}
		_LOG_CATCH

		_LOG_EXIT_CRITICAL_SECTION
	}
	_LOG_CATCH
}

/////////////////////////////////////////////////////////////////////////////

#if	(_WIN32_WINNT >= 0x0500)
class __TimerHandle
{
public:
	__TimerHandle ()
	:	mHandle (0)
	{
		mConstructed = &mHandle;
	}
	~__TimerHandle ()
	{
		Close ();
		mConstructed = NULL;
	}
	bool IsValid () const
	{
		return (mConstructed == &mHandle);
	}
	void Close ()
	{
		if	(mConstructed == &mHandle)
		{
			_LOG_TRY
			{
				if	(mHandle != 0)
				{
					_LOG_TRY
					{
						UnregisterWait (mHandle);
					}
					_LOG_CATCH
				}
				mHandle = 0;
			}
			_LOG_CATCH
		}
	}

public:
	HANDLE		mHandle;
private:
	HANDLE *	mConstructed;
};

static __TimerHandle	sLogCloseTimer;
static __OsHandle		sLogCloseSemaphore;

static VOID CALLBACK _LogCloseFileTimeout (LPVOID pArgument, BOOL pTimerOrWaitFired)
{
	_LOG_TRY
	{
		_LOG_ENTER_CRITICAL_SECTION
		_LOG_TRY
		{
			sLogFileHandle.Close ();
		}
		_LOG_CATCH
		_LOG_EXIT_CRITICAL_SECTION
	}
	_LOG_CATCH
}
#endif	// _WIN32_WINNT

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
class __LogCache
{
public:
	__LogCache ()
	:	mCacheSize(500),
		mCacheCount(0),
		mCache (NULL)
	{
		mConstructed = this;
	}
	~__LogCache ()
	{
		Empty ();
		mConstructed = NULL;
	}
	bool IsValid () const
	{
		return (mConstructed == this);
	}

	void Empty ()
	{
		_LOG_TRY
		{
			if	(
					(IsValid ())
				&&	(mCache != NULL)
				)
			{
				LPCTSTR *	lCache = mCache;

				mCache = NULL;

				_LOG_TRY
				{
					for	(int lNdx = 0; lNdx < mCacheSize; lNdx++)
					{
						if	(lCache[lNdx] != NULL)
						{
							::free ((void*) (lCache[lNdx]));
						}
					}
				}
				_LOG_CATCH
				_LOG_TRY
				{
					::free (lCache);
				}
				_LOG_CATCH
			}
		}
		_LOG_CATCH
	}

	LPCTSTR * NextFree ()
	{
		LPCTSTR * lRet = NULL;
		_LOG_TRY
		{
			if	(IsValid ())
			{
				_LOG_ENTER_CRITICAL_SECTION
				_LOG_TRY
				{
					if	(mCache == NULL)
					{
						mCache = (LPCTSTR*) ::calloc (mCacheSize, sizeof(LPCTSTR*));
						mCacheCount = 0;
					}
					if	(
							(mCache != NULL)
						&&	(mCacheCount > 0)
						)
					{
						memmove (mCache+1, mCache, min(mCacheCount,mCacheSize-1)*sizeof(LPCTSTR*));
						*mCache = NULL;
					}
					if	(lRet = mCache)
					{
						mCacheCount = min (mCacheCount+1,mCacheSize);
					}
				}
				_LOG_CATCH
				_LOG_EXIT_CRITICAL_SECTION
			}
		}
		_LOG_CATCH
		return lRet;
	}

public:
	const int		mCacheSize;
	int				mCacheCount;
	LPCTSTR *		mCache;
private:
	__LogCache *	mConstructed;
};
static __LogCache sLogCache;
#endif

/////////////////////////////////////////////////////////////////////////////

static void _LogPutCache (LPCTSTR pMessage)
{
	_LOG_TRY
	{
		if	(sLogCache.IsValid())
		{
			_LOG_ENTER_CRITICAL_SECTION
			_LOG_TRY
			{
				size_t		lStrSize = (_tcslen(pMessage)+1)*sizeof(TCHAR);
				LPCTSTR *	lCache = sLogCache.NextFree();
				if	(
						(lCache != NULL)
					&&	(*lCache = (LPCTSTR) ::malloc (lStrSize))
					)
				{
					memcpy ((void*)*lCache, pMessage, lStrSize);
				}
			}
			_LOG_CATCH
			_LOG_EXIT_CRITICAL_SECTION
		}
	}
	_LOG_CATCH
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _WIN32
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogWrite (LPCTSTR pStr, LPCTSTR pLogFileName)
{
	bool	lHighVolume = false;

	if	(pLogFileName == (LPCTSTR)-1)
	{
		lHighVolume = true;
		pLogFileName = gLogFileName;
	}
	else
	if	(!pLogFileName)
	{
		pLogFileName = gLogFileName;
	}
	if	(pLogFileName [0])
	{
#ifdef	_WIN32

		_LOG_TRY
		{
			_LOG_ENTER_CRITICAL_SECTION
			_LOG_TRY
			{
				DWORD	lSaveError = GetLastError ();
				_LOG_TRY
				{
					HANDLE	lLogFile = INVALID_HANDLE_VALUE;
					DWORD	lLogFileFlags = FILE_ATTRIBUTE_NORMAL|_LOG_FILE_FLAGS;
#ifdef __cplusplus
					if	(
							(lHighVolume)
						&&	(sLogFileHandle.IsValid())
						)
					{
						lLogFile = sLogFileHandle.mHandle;
						lLogFileFlags &= ~FILE_FLAG_WRITE_THROUGH;
					}
					else
					{
						sLogFileHandle.Close ();
					}
#endif
					if	(lLogFile == INVALID_HANDLE_VALUE)
					{
						int	lRetryCount = 50;

						SetLastError (0);
						lLogFile = CreateFile (pLogFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, _LOG_FILE_SECURITY, OPEN_ALWAYS, lLogFileFlags, NULL);

						while	(
									(lLogFile == INVALID_HANDLE_VALUE)
								&&	(GetLastError () == ERROR_SHARING_VIOLATION)
								&&	(lRetryCount-- > 0)
								)
						{
							Sleep (20);
							SetLastError (0);
							lLogFile = CreateFile (pLogFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, _LOG_FILE_SECURITY, OPEN_ALWAYS, lLogFileFlags, NULL);
						}

						lRetryCount = 20;

						while	(
									(lLogFile == INVALID_HANDLE_VALUE)
								&&	(GetLastError () == ERROR_SHARING_VIOLATION)
								&&	(lRetryCount-- > 0)
								)
						{
							Sleep (20);
							SetLastError (0);
							lLogFile = CreateFile (pLogFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, _LOG_FILE_SECURITY, OPEN_ALWAYS, lLogFileFlags, NULL);
						}

//						if	(
//								(lLogFile == INVALID_HANDLE_VALUE)
//							&&	(GetLastError () == ERROR_SHARING_VIOLATION)
//							)
//						{
//							MessageBeep(0);
//						}
					}

					if	(
							(pStr)
						&&	(lLogFile != INVALID_HANDLE_VALUE)
						)
					{
						_LOG_TRY
						{
							SetFilePointer (lLogFile, 0, 0, FILE_END);
							_LogWriteStr (lLogFile, pStr);
						}
						_LOG_CATCH
					}

#ifdef __cplusplus
					if	(
							(lHighVolume)
						&&	(lLogFile != INVALID_HANDLE_VALUE)
						&&	(sLogFileHandle.IsValid())
						&&	(
								(sLogFileHandle.mHandle == lLogFile)
							||	(sLogFileHandle.mHandle == INVALID_HANDLE_VALUE)
							)
						)
					{
#if	(_WIN32_WINNT >= 0x0500)
						sLogFileHandle.mHandle = INVALID_HANDLE_VALUE;
						sLogCloseTimer.Close ();
						if	(
								(sLogCloseSemaphore.IsOpen())
							&&	(sLogCloseTimer.IsValid())
							&&	(RegisterWaitForSingleObject (&sLogCloseTimer.mHandle, sLogCloseSemaphore.mHandle, (WAITORTIMERCALLBACK) &_LogCloseFileTimeout, (LPVOID)1, 100, WT_EXECUTELONGFUNCTION|WT_EXECUTEONLYONCE))
							)
						{
							sLogFileHandle.mHandle = lLogFile;
							lLogFile = INVALID_HANDLE_VALUE;
						}
						else
#endif
						if	(
//#if	(_WIN32_WINNT >= 0x0501)
//								(IsGUIThread (FALSE))
//#else
								(true)
//#endif
							&&	(::SetTimer (NULL, 0, 100, _LogCloseFileAsync))
							)
						{
							sLogFileHandle.mHandle = lLogFile;
							lLogFile = INVALID_HANDLE_VALUE;
						}
						else
						if	(sLogFileHandle.mHandle == lLogFile)
						{
							sLogFileHandle.Close ();
							lLogFile = INVALID_HANDLE_VALUE;
						}
					}
#endif
					if	(lLogFile != INVALID_HANDLE_VALUE)
					{
#if	(_WIN32_WINNT >= 0x0500)
						sLogCloseTimer.Close ();
#endif
						if	(sLogFileHandle.mHandle == lLogFile)
						{
							sLogFileHandle.Close ();
						}
						else
						{
							CloseHandle (lLogFile);
						}
					}
				}
				_LOG_CATCH
				SetLastError (lSaveError);
			}
			_LOG_CATCH
			_LOG_EXIT_CRITICAL_SECTION
		}
		_LOG_CATCH

#else	// _WIN32
		DWORD		lSaveError = GetLastError ();
		HFILE		lFile;
		OFSTRUCT	lOfStruct;

		if	(OpenFile (pLogFileName, &lOfStruct, OF_EXIST) == HFILE_ERROR)
		{
			lFile = OpenFile (pLogFileName, &lOfStruct, OF_CREATE|OF_READWRITE);
			if	(lFile != HFILE_ERROR)
			{
				_lclose (lFile);
			}
		}

		lFile = OpenFile (pLogFileName, &lOfStruct, OF_READWRITE|OF_SHARE_DENY_WRITE);

		if	(lFile != HFILE_ERROR)
		{
			_lseek (lFile, 0, SEEK_END);
			_lwrite (lFile, pStr, lstrlen (pStr));
			_lclose (lFile);
		}

		SetLastError (lSaveError);
#endif
    }
}

/////////////////////////////////////////////////////////////////////////////

int LogWriteCache (LPCTSTR pLogFileName)
{
	int	lRet = 0;

	_LOG_TRY
	{
		if	(sLogCache.IsValid())
		{
			UINT	lLogLevel;
			TCHAR	lLogFileName [MAX_PATH*2] = _T("");
			TCHAR	lOldLogFileName [MAX_PATH*2] = _T("");

			if	(pLogFileName)
			{
				_tcscpy (lLogFileName, pLogFileName);
			}
			if	(!lLogFileName[0])
			{
				LogControl (lLogFileName, lLogLevel);
			}

			_LOG_ENTER_CRITICAL_SECTION
			_LOG_TRY
			{
				lLogLevel = gLogLevel;
				_tcscpy (lOldLogFileName, gLogFileName);

				_LOG_TRY
				{
					if	(
							(sLogCache.IsValid())
						&&	(sLogCache.mCache != NULL)
						&&	(sLogCache.mCacheCount > 0)
						)
					{
						LogStart (false, lLogFileName, MaxLogLevel(gLogLevel,LogIfActive)&~LogToCache);
						LogMessage (LogIfActive|LogDateTime, _T("=== Start Cache ==="));

						while	(--sLogCache.mCacheCount >= 0)
						{
							LogWrite (sLogCache.mCache [sLogCache.mCacheCount], (LPCTSTR)-1);
							lRet++;
						}
						sLogCache.Empty();

						LogMessage (LogIfActive|LogDateTime, _T("==== End Cache ===="));
					}
				}
				_LOG_CATCH

				gLogLevel = lLogLevel;
				_tcscpy (gLogFileName, lOldLogFileName);
			}
			_LOG_CATCH
			_LOG_EXIT_CRITICAL_SECTION
		}
	}
	_LOG_CATCH

	return lRet;
}

bool LogEmptyCache ()
{
	bool lRet = false;

	_LOG_TRY
	{
		if	(sLogCache.IsValid())
		{
			lRet = true;
		}
		sLogCache.Empty();
	}
	_LOG_CATCH

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//	Bool return is so the function can be within an 'if'
//	Always returns true
/////////////////////////////////////////////////////////////////////////////

bool LogMessage (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(LogIsActive (pLogLevel))
	{
		pLogLevel |= gLogLevel & (LogOptionsMask|LogToCache);
#ifdef	_WIN32

		_LOG_TRY
		{
			_LOG_ENTER_CRITICAL_SECTION
			_LOG_TRY
			{
				DWORD	lSaveError = GetLastError ();

				_LOG_TRY
				{
					va_list 	lArgPtr;
					TCHAR		lMessage [8192];

					memset (lMessage, 0, sizeof (lMessage));
					if	(pFormat)
					{
						va_start (lArgPtr, pFormat);
						_LOG_TRY
						{
							_vsntprintf (lMessage, sizeof (lMessage) / sizeof (TCHAR), pFormat, lArgPtr);
						}
						_LOG_CATCH
						lMessage [(sizeof (lMessage)/sizeof (TCHAR))-3] = 0;
						_tcscat (lMessage, _T("\r\n"));
					}

#ifdef	_LOG_PREFIX
					if	(!(pLogLevel & (LogHighVolume|LogToCache)))
					{
						if	(
								(pFormat)
							&&  (!(pLogLevel & LogNoPrefix))
							)
						{
							OutputDebugString (_LOG_PREFIX);
						}
					}
#endif

					if	(pLogLevel & LogTicks)
					{
						static bool				lCountStarted = false;
						static DWORD			lTickStart;
						static DWORD			lTickPrev;
						static BOOL				lLargeAvailable = FALSE;
						static LARGE_INTEGER	lLargeStart;
						static LARGE_INTEGER	lLargePrev;
						static LARGE_INTEGER	lLargeFrequency;
						TCHAR					lTicksStr [200];
						DWORD					lTickCount;
						LARGE_INTEGER			lLargeCount;
						BOOL					lLargeValid;

						static const UINT		lTickStackSize = 100;
						static UINT				lTickStackCount = 0;
						static DWORD			lTickStartStack [lTickStackSize];
						static DWORD			lTickPrevStack [lTickStackSize];
						static LARGE_INTEGER	lLargePrevStack [lTickStackSize];

						if	(lCountStarted)
						{
							lTickCount = GetTickCount ();
							if	(lLargeAvailable)
							{
								lLargeValid = QueryPerformanceCounter (&lLargeCount);
							}

							if	((pLogLevel & LogTicksDeltaRestart) == LogTicksDeltaRestart)
							{
								lTickStart = lTickCount;
								lTickPrev = lTickCount;
								lLargeStart.QuadPart = lLargeCount.QuadPart;
								lLargePrev.QuadPart = lLargeCount.QuadPart;
								lTickStackCount = 0;
							}

							if	(
									((pLogLevel & LogTicksDeltaPop) == LogTicksDeltaPop)
								&&	(lTickStackCount > 0)
								)
							{
								lTickStackCount--;
								lTickStart = lTickStartStack [lTickStackCount];
								lTickPrev = lTickPrevStack [lTickStackCount];
								lLargePrev.QuadPart = lLargePrevStack [lTickStackCount].QuadPart;
							}
						}
						else
						{
							lTickCount = GetTickCount ();
							lTickStart = lTickCount;
							lTickPrev = lTickCount;

							lLargeAvailable = QueryPerformanceFrequency (&lLargeFrequency);
							lLargeValid = lLargeAvailable;
							if	(lLargeAvailable)
							{
								QueryPerformanceCounter (&lLargeCount);
								lLargeStart.QuadPart = lLargeCount.QuadPart;
								lLargePrev.QuadPart = lLargeCount.QuadPart;
							}
						}

						if	((pLogLevel & LogTicksDelta) == LogTicksDelta)
						{
							if	(lLargeValid)
							{
//								_stprintf (lTicksStr, _T("[%12.12I64d] "), lLargeCount.QuadPart - lLargePrev.QuadPart);
								_stprintf (lTicksStr, _T("[%4.10f] "), (double) (lLargeCount.QuadPart - lLargePrev.QuadPart) / (double) lLargeFrequency.QuadPart);
							}
							else
							{
								_stprintf (lTicksStr, _T("[%6.6u (%6.6u)] "), lTickCount - lTickPrev, lTickCount - lTickStart);
							}
							lTickPrev = lTickCount;
							lLargePrev.QuadPart = lLargeCount.QuadPart;

							if	(lTickStackCount > 0)
							{
								TCHAR	lTempStr [200];
								_stprintf (lTempStr, _T("%*s%s"), lTickStackCount*2, _T(""), lTicksStr);
								_tcscpy (lTicksStr, lTempStr);
							}
						}
						else
						{
							if	(lLargeValid)
							{
								_stprintf (lTicksStr, _T("[%12.12I64d] "), lLargeCount.QuadPart);
							}
							else
							{
								_stprintf (lTicksStr, _T("[%u] "), lTickCount);
							}
						}

						if	(!lCountStarted)
						{
/*
							if	(lLargeAvailable)
							{
								TCHAR	lTempStr [200];
								_stprintf (lTempStr, _T("%s at [%I64d]"), lTicksStr, lLargeFrequency.QuadPart);
								_tcscpy (lTicksStr, lTempStr);
							}
*/
							lCountStarted = true;
						}

						if	(
								((pLogLevel & LogTicksDeltaPush) == LogTicksDeltaPush)
							&&	(lTickStackCount < lTickStackSize-1)
							)
						{
							lTickStartStack [lTickStackCount] = lTickStart;
							lTickPrevStack [lTickStackCount] = lTickPrev;
							lLargePrevStack [lTickStackCount].QuadPart = lLargePrev.QuadPart;
							lTickStackCount++;
						}

						_LOG_TRY
						{
							size_t	lStrSize = sizeof (lMessage) / sizeof (TCHAR);
							size_t	lStrLen;

							if	(lStrLen = _tcslen (lTicksStr))
							{
								memmove (lMessage + lStrLen, lMessage, (lStrSize - lStrLen - 1) * sizeof (TCHAR));
								memcpy (lMessage, lTicksStr, lStrLen * sizeof (TCHAR));
							}
						}
						_LOG_CATCH

						if	(!(pLogLevel & (LogHighVolume|LogToCache)))
						{
							OutputDebugString (lTicksStr);
						}
					}

					if	(!(pLogLevel & (LogHighVolume|LogToCache)))
					{
						if	(pFormat)
						{
							OutputDebugString (lMessage);
						}
						else
						{
							OutputDebugString (_T("\n"));
						}
					}

					if  (!(pLogLevel & LogNoPrefix))
					{
						TCHAR	lThreadStr [50];
						DWORD	lSessionId = 0;

#ifdef	_UNICODE
						ProcessIdToSessionId (GetCurrentProcessId (), &lSessionId);
						_stprintf (lThreadStr, _T("[%2.2u %4.4u %4.4u] "), lSessionId, GetCurrentProcessId (), GetCurrentThreadId ());
#else
						_stprintf (lThreadStr, _T("[%6.6lX %6.6lX] "), GetCurrentProcessId () & 0x00FFFFFF, GetCurrentThreadId () & 0x00FFFFFF);
#endif
						_LOG_TRY
						{
							size_t	lStrSize = sizeof (lMessage) / sizeof (TCHAR);
							size_t	lStrLen;

							if  (lStrLen = _tcslen (lThreadStr))
							{
								memmove (lMessage + lStrLen, lMessage, (lStrSize - lStrLen - 1) * sizeof (TCHAR));
								memcpy (lMessage, lThreadStr, lStrLen * sizeof (TCHAR));
							}
						}
						_LOG_CATCH
					}

					if  (
							(
								(!(pLogLevel & LogNoPrefix))
							||  (pLogLevel & LogDateTime)
							)
#ifdef  _DEBUG
						&&  (pLogLevel & LogDateTime)
#endif
						)
					{
						SYSTEMTIME	lDateTime;
						TCHAR		lDateStr [100] = _T("");
						TCHAR		lTimeStr [100] = _T("");

						memset (&lDateTime, 0, sizeof(lDateTime));
						GetLocalTime (&lDateTime);
#ifdef  _DEBUG
						if  (pLogLevel & LogDate)
#endif
						{
							GetDateFormat (LOCALE_USER_DEFAULT, 0, &lDateTime, _T("yyyy'/'MM'/'dd"), lDateStr, sizeof (lDateStr) / sizeof (TCHAR));
							_tcscat (lDateStr, _T(" "));
						}
#ifdef  _DEBUG
						if  (pLogLevel & LogTime)
#endif
						{
							GetTimeFormat (LOCALE_USER_DEFAULT, 0, &lDateTime, _T("HH':'mm':'ss"), lTimeStr, sizeof (lTimeStr) / sizeof (TCHAR));
#ifdef	_UNICODE
							if  ((pLogLevel & LogTimeMs) == LogTimeMs)
#else
							if  (
									((pLogLevel & LogTimeMs) == LogTimeMs)
								||  ((pLogLevel & LogTimeNs) == LogTimeNs)
								)
#endif
							{
								TCHAR	lTickStr [50];
								_stprintf (lTickStr, _T(".%3.3u"), lDateTime.wMilliseconds);
								_tcscat (lTimeStr, lTickStr);
							}
#ifdef	_UNICODE
							if  ((pLogLevel & LogTimeNs) == LogTimeNs)
							{
								static LARGE_INTEGER	lPerfFreq = {0,0};
								LARGE_INTEGER			lPerfTime;
								TCHAR					lTickStr [50];
								_LOG_TRY
								{
									if	(lPerfFreq.QuadPart == 0)
									{
										QueryPerformanceFrequency (&lPerfFreq);
									}
									if	(QueryPerformanceCounter (&lPerfTime))
									{
										_stprintf (lTickStr, _T(" (%6.6u."), lPerfTime.QuadPart/lPerfFreq.QuadPart);
										_tcscat (lTimeStr, lTickStr);
										_stprintf (lTickStr, _T("%6.6u)"), ((lPerfTime.QuadPart*(__int64)1000000)/lPerfFreq.QuadPart)%(__int64)1000000);
										_tcscat (lTimeStr, lTickStr);
									}
								}
								_LOG_CATCH
							}
#endif
							_tcscat (lTimeStr, _T(" "));
						}

						_LOG_TRY
						{
							size_t	lStrSize = sizeof (lMessage) / sizeof (TCHAR);
							size_t	lStrLen;

							if	(lStrLen = _tcslen (lTimeStr))
							{
								memmove (lMessage + lStrLen, lMessage, (lStrSize - lStrLen - 1) * sizeof (TCHAR));
								memcpy (lMessage, lTimeStr, lStrLen * sizeof (TCHAR));
							}

							if  (lStrLen = _tcslen (lDateStr))
							{
								memmove (lMessage + lStrLen, lMessage, (lStrSize - lStrLen - 1) * sizeof (TCHAR));
								memcpy (lMessage, lDateStr, lStrLen * sizeof (TCHAR));
							}
						}
						_LOG_CATCH
					}

#ifdef	_LOG_PREFIX
					if  (!(pLogLevel & LogNoPrefix))
					{
						_LOG_TRY
						{
							int	lStrSize = sizeof (lMessage) / sizeof (TCHAR);
							int	lStrLen;

							if  (lStrLen = (int)_tcslen (_LOG_PREFIX))
							{
								memmove (lMessage + lStrLen, lMessage, (lStrSize - lStrLen - 1) * sizeof (TCHAR));
								memcpy (lMessage, _LOG_PREFIX, lStrLen * sizeof (TCHAR));
							}
						}
						_LOG_CATCH
					}
#endif

					if  (pLogLevel & LogToCache)
					{
						_LogPutCache (lMessage);
					}
					else
					{
						LogWrite (lMessage, (LPCTSTR)((pLogLevel & LogHighVolume)?-1:0));
					}
				}
#ifdef	_CPPUNWIND
				catch (...)
				{
					try
					{
						OutputDebugString (_T("*** LogMessage Exception ***\n"));
					}
					catch (...)
					{}
				}
#endif
				SetLastError (lSaveError);
			}
			_LOG_CATCH
			_LOG_EXIT_CRITICAL_SECTION
		}
		_LOG_CATCH

#else	// _WIN32
		DWORD			lSaveError = GetLastError ();
        char            lMessage [1024];
		va_list			lArgPtr;

		if	(pFormat)
		{
			va_start (lArgPtr, pFormat);
			wvsprintf (lMessage, pFormat, lArgPtr);
			lstrcat (lMessage, "\r\n");
			OutputDebugString (lMessage);
		}

		if	(
				(pFormat)
			&&	(gLogFileName [0])
			)
		{
			HFILE		lFile;
			OFSTRUCT	lOfStruct;
			TCHAR		lTaskStr [50];

			wsprintf (lTaskStr, "[%4.4X] [%4.4X] ", gInstanceHandle, GetCurrentTask ());

			if	(OpenFile (gLogFileName, &lOfStruct, OF_EXIST) == HFILE_ERROR)
			{
				lFile = OpenFile (gLogFileName, &lOfStruct, OF_CREATE|OF_READWRITE);
				if	(lFile != HFILE_ERROR)
				{
					_lclose (lFile);
				}
			}

			lFile = OpenFile (gLogFileName, &lOfStruct, OF_READWRITE|OF_SHARE_DENY_WRITE);

			if	(lFile != HFILE_ERROR)
			{
				_lseek (lFile, 0, SEEK_END);
				_lwrite (lFile, lTaskStr, lstrlen (lTaskStr));
				_lwrite (lFile, lMessage, lstrlen (lMessage));
				_lclose (lFile);
			}
		}

		SetLastError (lSaveError);
#endif	// _WIN32
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
//	Bool return is so the function can be within an 'if'
//	Always returns true
/////////////////////////////////////////////////////////////////////////////

bool LogDump (UINT pLogLevel, LPCVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix, bool pDumpOffsets)
{
	if	(LogIsActive (pLogLevel))
	{
		LPCTSTR	lPrefix = pPrefix ? pPrefix : _T("");
#if	_WIN32_WINNT >= 0x0500
		if	(IsBadReadPtr (pBuffer, pBufferSize))
		{
			LogMessage (pLogLevel, _T("%s[%p] (%u) IsBadReadPtr"), lPrefix, pBuffer, pBufferSize);
			return false;
		}
#endif
		_LOG_TRY
		{
			const int   lStrSize = 120;
			LPTSTR		lCodes;
			char *      lChars;
			UINT        lNdx;
			UINT        lExtra;
			UINT        lLimit = (((UINT)pBufferSize + 15) / 16) * 16;

			lCodes = new TCHAR [lStrSize];
			lChars = new char [lStrSize];
			lCodes [0] = 0;

			for (lNdx = 0; lNdx <= lLimit; lNdx++)
			{
				if  (!(lNdx % 16))
				{
					if  (lCodes [0])
					{
#ifdef	_WIN32
						LogMessage (pLogLevel, _T("%s%s %hs"), lPrefix, lCodes, lChars);
#else
						LogMessage (pLogLevel, _T("%s%s %s"), (LPSTR) lPrefix, (LPSTR) lCodes, (LPSTR) lChars);
#endif
					}
					lCodes [0] = 0;

					if  (pDumpOffsets)
					{
#ifdef	_WIN32
						_stprintf (lCodes, _T("[%4.4lu %4.4lX] %p: "), lNdx, lNdx, ((LPBYTE) pBuffer)+lNdx);
#else
						wsprintf (lCodes, "[%4.4u %4.4X] %4.4hX:%4.4hX: ", lNdx, lNdx, SELECTOROF (pBuffer), OFFSETOF (pBuffer)+lNdx);
#endif
					}
					else
					{
#ifdef	_WIN32
						_stprintf (lCodes, _T("%p: "), ((LPBYTE) pBuffer)+lNdx);
#else
						wsprintf (lCodes, "%4.4hX:%4.4hX: ", SELECTOROF (pBuffer), OFFSETOF (pBuffer)+lNdx);
#endif
					}
					memset (lChars, 0, lStrSize);
					lExtra = 0;
				}
				else
				if  (!(lNdx % 8))
				{
#ifdef	_WIN32
					_tcscat (lCodes, _T("  "));
#else
	    			lstrcat (lCodes, "  ");
#endif
					lChars [8] = ' ';
					lExtra++;
				}

				if  (lNdx < pBufferSize)
				{
#ifdef	_WIN32
					_stprintf (lCodes+_tcslen (lCodes), _T("%2.2hX "), ((LPBYTE) pBuffer) [lNdx]);
#else
					wsprintf (lCodes+lstrlen (lCodes), "%2.2hX ", ((LPBYTE) pBuffer) [lNdx]);
#endif
					if  (isprint (((LPBYTE) pBuffer) [lNdx]))
					{
						lChars [(lNdx % 16) + lExtra] = ((LPBYTE) pBuffer) [lNdx];
					}
					else
					{
						lChars [(lNdx % 16) + lExtra] = '.';
					}
				}
				else
				{
#ifdef	_WIN32
					_tcscat (lCodes, _T("   "));
#else
		    	    lstrcat (lCodes, "   ");
#endif
				}
			}

#if (_MSC_VER >= 1400)
			delete lCodes;
			delete lChars;
#else
			delete [] lCodes;
			delete [] lChars;
#endif
		}
		_LOG_CATCH

		return true;
    }
    return false;
}

bool LogDumpWords (UINT pLogLevel, LPCVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix, bool pDumpOffsets)
{
	if	(LogIsActive (pLogLevel))
	{
		LPCTSTR	lPrefix = pPrefix ? pPrefix : _T("");
#if	_WIN32_WINNT >= 0x0500
		if	(IsBadReadPtr (pBuffer, ((pBufferSize + 15) / 16) * 16))
		{
			LogMessage (pLogLevel, _T("%s[%p] (%u) IsBadReadPtr"), lPrefix, pBuffer, pBufferSize);
			return false;
		}
#endif
		_LOG_TRY
		{
			const int   lStrSize = 120;
			LPTSTR		lCodes8;
			LPTSTR		lCodes16;
			LPTSTR		lCodes32;
			char *      lChars;
			UINT        lNdx;
			UINT        lExtra;
			UINT        lLimit = (((UINT)pBufferSize + 15) / 16) * 16;

			lCodes8 = new TCHAR [lStrSize];
			lCodes16 = new TCHAR [lStrSize];
			lCodes32 = new TCHAR [lStrSize];
			lChars = new char [lStrSize];
			lCodes8 [0] = 0;
			lCodes16 [0] = 0;
			lCodes32 [0] = 0;

			for (lNdx = 0; lNdx <= lLimit; lNdx++)
			{
				if  (!(lNdx % 16))
				{
					if  (lCodes32 [0])
					{
#ifdef	_WIN32
						LogMessage (pLogLevel, _T("%s%s %s %s %hs"), lPrefix, lCodes32, lCodes16, lCodes8, lChars);
#else
						LogMessage (pLogLevel, _T("%s%s %s %s %s"), (LPSTR) lPrefix, (LPSTR) lCodes32, (LPSTR) lCodes16, (LPSTR) lCodes8, (LPSTR) lChars);
#endif
					}
					lCodes8 [0] = 0;
					lCodes16 [0] = 0;
					lCodes32 [0] = 0;

					if  (pDumpOffsets)
					{
#ifdef	_WIN32
						_stprintf (lCodes32, _T("[%4.4lu %4.4lX] %p: "), lNdx, lNdx, ((LPBYTE) pBuffer)+lNdx);
#else
						wsprintf (lCodes32, "[%4.4u %4.4X] %4.4hX:%4.4hX: ", lNdx, lNdx, SELECTOROF (pBuffer), OFFSETOF (pBuffer)+lNdx);
#endif
					}
					else
					{
#ifdef	_WIN32
						_stprintf (lCodes32, _T("%p: "), ((LPBYTE) pBuffer)+lNdx);
#else
						wsprintf (lCodes32, "%4.4hX:%4.4hX: ", SELECTOROF (pBuffer), OFFSETOF (pBuffer)+lNdx);
#endif
					}
					memset (lChars, 0, lStrSize);
					lExtra = 0;
				}
				else
				if  (!(lNdx % 8))
				{
#ifdef	_WIN32
					_tcscat (lCodes8, _T(" "));
					_tcscat (lCodes16, _T(" "));
#else
	    			lstrcat (lCodes8, " ");
	    			lstrcat (lCodes16, " ");
#endif
					lChars [8] = ' ';
					lExtra++;
				}

				if	(!(lNdx % 2))
				{
					if  (lNdx < pBufferSize)
					{
#ifdef	_WIN32
						_stprintf (lCodes16+_tcslen (lCodes16), _T("%4.4X "), ((LPWORD) pBuffer) [lNdx/2]);
#else
						wsprintf (lCodes16+lstrlen (lCodes16), "%4.4X ", ((LPWORD) pBuffer) [lNdx/2]);
#endif
					}
					else
					{
#ifdef	_WIN32
						_tcscat (lCodes16, _T("     "));
#else
		    			lstrcat (lCodes16, "     ");
#endif
					}
				}

				if	(!(lNdx % 4))
				{
					if  (lNdx < pBufferSize)
					{
#ifdef	_WIN32
						_stprintf (lCodes32+_tcslen (lCodes32), _T("%8.8X "), ((LPDWORD) pBuffer) [lNdx/4]);
#else
						wsprintf (lCodes32+lstrlen (lCodes32), "%8.8X ", ((LPDWORD) pBuffer) [lNdx/4]);
#endif
					}
					else
					{
#ifdef	_WIN32
						_tcscat (lCodes32, _T("         "));
#else
		    			lstrcat (lCodes32, "         ");
#endif
					}
				}

				if  (lNdx < pBufferSize)
				{
#ifdef	_WIN32
					_stprintf (lCodes8+_tcslen (lCodes8), _T("%2.2hX "), ((LPBYTE) pBuffer) [lNdx]);
#else
					wsprintf (lCodes8+lstrlen (lCodes8), "%2.2hX ", ((LPBYTE) pBuffer) [lNdx]);
#endif
					if  (isprint (((LPBYTE) pBuffer) [lNdx]))
					{
						lChars [(lNdx % 16) + lExtra] = ((LPBYTE) pBuffer) [lNdx];
					}
					else
					{
						lChars [(lNdx % 16) + lExtra] = '.';
					}
				}
				else
				{
#ifdef	_WIN32
					_tcscat (lCodes8, _T("   "));
#else
		    	    lstrcat (lCodes8, "   ");
#endif
				}
			}

#if (_MSC_VER >= 1400)
			delete lCodes8;
			delete lCodes16;
			delete lCodes32;
			delete lChars;
#else
			delete [] lCodes8;
			delete [] lCodes16;
			delete [] lCodes32;
			delete [] lChars;
#endif
		}
		_LOG_CATCH

		return true;
    }
    return false;
}

bool LogDumpBits (UINT pLogLevel, LPCVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix, UINT pBytesPerLine)
{
	if	(LogIsActive (pLogLevel))
	{
		LPCTSTR	lPrefix = pPrefix ? pPrefix : _T("");
#if	_WIN32_WINNT >= 0x0500
		if	(IsBadReadPtr (pBuffer, pBufferSize))
		{
			LogMessage (pLogLevel, _T("%s[%p] (%u) IsBadReadPtr"), lPrefix, pBuffer, pBufferSize);
			return false;
		}
#endif
		_LOG_TRY
		{
			const int   lStrSize = 120;
			LPTSTR		lCodes;
			UINT        lNdx;
			int			lBit;

			if	(!pPrefix)
			{
				lPrefix = _T("");
			}
			if	(pBytesPerLine == 0)
			{
				pBytesPerLine = 8;
			}

			lCodes = new TCHAR [lStrSize];
			lCodes [0] = 0;

			for (lNdx = 0; lNdx < pBufferSize; lNdx++)
			{
				if  (lCodes [0])
				{
#ifdef	_WIN32
					_tcscat (lCodes, _T("  "));
#else
	    			lstrcat (lCodes, "  ");
#endif
				}

				for	(lBit = 7; lBit >= 0; lBit--)
				{
					if	(((LPBYTE) pBuffer) [lNdx] & (1 << lBit))
					{
#ifdef	_WIN32
						_tcscat (lCodes, _T("1"));
#else
	    				lstrcat (lCodes, "1");
#endif
					}
					else
					{
#ifdef	_WIN32
						_tcscat (lCodes, _T("0"));
#else
	    				lstrcat (lCodes, "0");
#endif
					}
				}

				if	(
						(
							(pBytesPerLine == 1)
						||	((lNdx % pBytesPerLine) == pBytesPerLine-1)
						||	(lNdx == pBufferSize-1)
						)
					&&  (lCodes [0])
					)
				{
#ifdef	_WIN32
					LogMessage (pLogLevel, _T("%s%s"), lPrefix, lCodes);
#else
					LogMessage (pLogLevel, _T("%s%s"), (LPSTR) lPrefix, (LPSTR) lCodes);
#endif
					lCodes [0] = 0;
				}
			}

#if (_MSC_VER >= 1400)
			delete lCodes;
#else
			delete [] lCodes;
#endif
		}
		_LOG_CATCH

		return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
#ifndef	_LOG_SKIPFILELINE
#define	_LOG_SKIPFILELINE
#endif
#endif

#ifdef	_LOG_SKIPFILELINE
#define	_LogPrepFileLine(s,f,l)
#else
void _LogPrepFileLine (LPTSTR pString, LPCSTR pFile, UINT pLine)
{
	if	(pFile)
	{
		TCHAR	lFileNameStr [MAX_PATH];
		TCHAR	lFileLineStr [20];

		if	(strrchr (pFile, '\\'))
		{
			pFile = strrchr (pFile, '\\') + 1;
		}
#ifdef	_WIN32
		_stprintf (lFileNameStr, _T(" in %hs"), pFile);
		_stprintf (lFileLineStr, _T("%u"), pLine);
		_tcscat (pString, _T(" {"));
		_tcscat (pString, lFileLineStr);
		_tcscat (pString, lFileNameStr);
		_tcscat (pString, _T("}"));
#else
		wsprintf (lFileLineStr, _T("%u "), pLine);
		lstrcat (pString, " {");
		lstrcat (pString, lFileLineStr);
		lstrcat (pString, " in ");
		lstrcat (pString, pFile);
		lstrcat (pString, "}");
#endif
	}
}
#endif	// _LOG_SKIPFILELINE

/////////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
DWORD LogWinErr (UINT pLogLevel, DWORD pError, LPCTSTR pFormat, ...)
#else
DWORD _LogWinErrFL::LogErr (UINT pLogLevel, DWORD pError, LPCTSTR pFormat, ...)
#endif
{
	if	(
			(
				(pError)
			||	(gLogAllWinErr)
			||	((pLogLevel & LogLevelMask) == LogAlways)
			)
		&&	(LogIsActive (pLogLevel))
		)
	{
		TCHAR			lFormat [400];
		const size_t	lExtraSize = 4096;
		LPTSTR			lExtra = NULL;
		va_list 		lArgPtr;
#ifdef	_WIN32
		LPTSTR 			lMessage = NULL;
#endif

		_LOG_TRY
		{
			if	(pError)
			{
#ifdef	_WIN32
				_tcscpy (lFormat, _T("Error [%lu (%8.8lX)]"));
#else
				lstrcpy (lFormat, "Error [%lu (%8.8lX)]");
#endif
			}
			else
			{
#ifdef	_WIN32
				_tcscpy (lFormat, _T("Success"));
#else
				lstrcpy (lFormat, "Success");
#endif
			}
            if  (pFormat)
            {
			    va_start (lArgPtr, pFormat);
				lExtra = new TCHAR [lExtraSize];
				lExtra[0] = 0;
#ifdef	_WIN32
				while (pFormat[0] == _T(' '))
				{
					_tcscat (lExtra, _T(" "));
					pFormat++;
				}
				if	(lExtra[0])
				{
					_tcscat (lExtra, lFormat);
					_tcscpy (lFormat, lExtra);
				}
				_tcscpy (lExtra, _T("=> "));
			    _vsntprintf (lExtra+_tcslen(lExtra), lExtraSize-_tcslen(lExtra), pFormat, lArgPtr);
			    _tcscat (lFormat, _T(" %s"));
#else
				while (pFormat[0] == ' ')
				{
					lstrcat (lExtra, " ");
					pFormat++;
				}
				if	(lExtra[0])
				{
					lstrcat (lExtra, lFormat);
					lstrcpy (lFormat, lExtra);
				}
				lstrcpy (lExtra, "=> ");
			    wvsprintf (lExtra+lstrlen(lExtra), pFormat, lArgPtr);
			    lstrcat (lFormat, " %s");
#endif
            }

			if	(pError)
			{
#ifdef	_WIN32
				FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, pError, 0, (LPTSTR) &lMessage, 0, NULL);
				if	(lMessage)
				{
					while ((lMessage [_tcslen (lMessage)-1] == _T('\n')) || (lMessage [_tcslen (lMessage)-1] == _T('\r'))) lMessage [_tcslen (lMessage)-1] = 0;
					_tcscat (lFormat, _T(" %s"));
#ifndef	_LOG_SKIPFILELINE
					_LogPrepFileLine (lFormat, mFile, mLine);
#endif
					LogMessage (pLogLevel, lFormat, pError, pError, lMessage, lExtra);
				}
				else
				{
#ifndef	_LOG_SKIPFILELINE
					_LogPrepFileLine (lFormat, mFile, mLine);
#endif
					LogMessage (pLogLevel, lFormat, pError, pError, lExtra);
				}
#else
#ifndef	_LOG_SKIPFILELINE
				_LogPrepFileLine (lFormat, mFile, mLine);
#endif
				LogMessage (pLogLevel, (LPSTR) lFormat, pError, pError, lExtra);
#endif
			}
			else
			{
#ifndef	_LOG_SKIPFILELINE
				_LogPrepFileLine (lFormat, mFile, mLine);
#endif
#ifdef	_WIN32
				LogMessage (pLogLevel, lFormat, lExtra);
#else
				LogMessage (pLogLevel, (LPSTR) lFormat, lExtra);
#endif
			}
		}
#ifdef	_CPPUNWIND
		catch (...)
		{
			try
			{
				LogMessage (pLogLevel, _T("*** LogWinErr Exception ***"));
			}
			catch (...)
			{}
		}
#endif

#ifdef	_WIN32
		_LOG_TRY
		{
	        if	(lMessage)
    		{
				LocalFree (lMessage);
			}
		}
		_LOG_CATCH
#endif

		_LOG_TRY
		{
	        if	(lExtra)
    		{
#if (_MSC_VER >= 1400)
    			delete lExtra;
#else
    			delete [] lExtra;
#endif
			}
		}
		_LOG_CATCH
	}

    return pError;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_LOG_NOFILELINE
DWORD LogComErr (UINT pLogLevel, DWORD pError, LPCTSTR pFormat, ...)
#else
HRESULT _LogComErrFL::LogErr (UINT pLogLevel, HRESULT pError, LPCTSTR pFormat, ...)
#endif
{
	if	(
			(
				(FAILED (pError))
			||	(gLogAllComErr)
			||	((pLogLevel & LogLevelMask) == LogAlways)
			)
		&&	(LogIsActive (pLogLevel))
		)
	{
		_LOG_TRY
		{
			bool            lLogged = false;
			TCHAR			lFormat [400];
			const size_t    lExtraSize = 4096;
			LPTSTR			lExtra = NULL;
			va_list 		lArgPtr;
			LPTSTR  		lMessage = NULL;
			IErrorInfo *    lErrorInfo = NULL;
			BSTR			lErrorSource = NULL;
			BSTR			lErrorStr = NULL;
#ifdef	OLEDBVER
			IErrorRecords * lErrorRecords;
			ULONG			lRecCount = 0;
#endif

			_LOG_TRY
			{
				if	(pError == S_OK)
				{
					_tcscpy (lFormat, _T("S_OK"));
				}
				else
				if	(pError == S_FALSE)
				{
					_tcscpy (lFormat, _T("S_FALSE"));
				}
				else
				if	(FAILED (pError))
				{
					_tcscpy (lFormat, _T("Error [%8.8X (%hu)]"));
				}
				else
				{
					_tcscpy (lFormat, _T("Warning [%8.8X (%hu)]"));
				}

				if  (pFormat)
				{
					va_start (lArgPtr, pFormat);
					lExtra = new TCHAR [lExtraSize];
					lExtra[0] = 0;
					while (pFormat[0] == _T(' '))
					{
						_tcscat (lExtra, _T(" "));
						pFormat++;
					}
					if	(lExtra[0])
					{
						_tcscat (lExtra, lFormat);
						_tcscpy (lFormat, lExtra);
					}
					_tcscpy (lExtra, _T("=> "));
					_vsntprintf (lExtra+_tcslen(lExtra), lExtraSize-_tcslen(lExtra), pFormat, lArgPtr);
					_tcscat (lFormat, _T(" %s"));
				}

				if	(
						(pError != S_OK)
					&&	(pError != S_FALSE)
					&&	(SUCCEEDED (GetErrorInfo (0, &lErrorInfo)))
					&&	(lErrorInfo)
					)
				{
					SetErrorInfo (0, lErrorInfo);
#ifdef	OLEDBVER
					if	(
							(SUCCEEDED (lErrorInfo->QueryInterface (&lErrorRecords)))
						&&	(lErrorRecords)
						&&	(SUCCEEDED (lErrorRecords->GetRecordCount (&lRecCount)))
						)
					{
						tS <ERRORINFO>	lError;
						ULONG			lRecNum;

						for (lRecNum = 0; lRecNum < lRecCount; lRecNum++)
						{
    						IErrorInfo *    lErrorInfo = NULL;
                			BSTR			lErrorStr = NULL;
                			BSTR			lErrorSource = NULL;

							_LOG_TRY
							{
								if	(
										(SUCCEEDED (lErrorRecords->GetBasicErrorInfo (lRecNum, &lError)))
									&&	(SUCCEEDED (lErrorRecords->GetErrorInfo (lRecNum, LOCALE_USER_DEFAULT, &lErrorInfo)))
									&&	(SUCCEEDED (lErrorInfo->GetDescription (&lErrorStr)))
									)
								{
									lErrorInfo->GetSource (&lErrorSource);
									if  (!lLogged)
									{
#ifndef	_LOG_SKIPFILELINE
										_LogPrepFileLine (lFormat, mFile, mLine);
#endif
    									LogMessage (pLogLevel, lFormat, pError, HRESULT_CODE (pError), lExtra);
									}
    								LogMessage (pLogLevel, _T("  Error: %ls from: %ls"), lErrorStr, lErrorSource);
									lLogged = true;
								}
							}
							_LOG_CATCH

							_LOG_TRY
							{
								if  (lErrorInfo)
								{
									lErrorInfo->Release ();
								}
							}
							_LOG_CATCH

							_LOG_TRY
							{
								if  (lErrorStr)
								{
									SysFreeString (lErrorStr);
								}
								lErrorStr = NULL;
							}
							_LOG_CATCH

							_LOG_TRY
							{
								if  (lErrorSource)
								{
									SysFreeString (lErrorSource);
								}
								lErrorSource = NULL;
							}
							_LOG_CATCH
						}
					}
#endif
				}

				if  (
						(!lLogged)
					&&	(lErrorInfo)
					)
				{
					_LOG_TRY
					{
						if  (
								(SUCCEEDED (lErrorInfo->GetDescription (&lErrorStr)))
							&&	(lErrorStr)
							&&	(*lErrorStr)
							)
						{
							while (wcschr (lErrorStr, L'\n') != NULL) {*wcschr (lErrorStr, L'\n') = L' ';}
							while (wcschr (lErrorStr, L'\r') != NULL) {*wcschr (lErrorStr, L'\r') = L' ';}
							lErrorInfo->GetSource (&lErrorSource);
							_tcscat (lFormat, _T(" Info: %ls from: %ls"));
#ifndef	_LOG_SKIPFILELINE
							_LogPrepFileLine (lFormat, mFile, mLine);
#endif
							if	(lExtra)
							{
								LogMessage (pLogLevel, lFormat, pError, HRESULT_CODE (pError), lExtra, lErrorStr, lErrorSource);
							}
							else
							{
								LogMessage (pLogLevel, lFormat, pError, HRESULT_CODE (pError), lErrorStr, lErrorSource);
							}
							lLogged = true;
						}
#ifdef	_CPPUNWIND
					}
					catch (...)
					{
						try
						{
							LogMessage (pLogLevel, _T("*** LogComErr ErrorInfo Exception ***"));
						}
						catch (...)
						{}
					}
#endif
				}

				if  (!lLogged)
				{
					if	(
							(pError == S_OK)
						||	(pError == S_FALSE)
						)
					{
#ifndef	_LOG_SKIPFILELINE
						_LogPrepFileLine (lFormat, mFile, mLine);
#endif
						LogMessage (pLogLevel, lFormat, lExtra);
					}
					else
					{
						FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, pError, 0, (LPTSTR) &lMessage, 0, NULL);
						if	(lMessage)
						{
							while ((lMessage [_tcslen (lMessage)-1] == _T('\n')) || (lMessage [_tcslen (lMessage)-1] == _T('\r'))) lMessage [_tcslen (lMessage)-1] = 0;
							_tcscat (lFormat, _T(" %s"));
#ifndef	_LOG_SKIPFILELINE
							_LogPrepFileLine (lFormat, mFile, mLine);
#endif
							LogMessage (pLogLevel, lFormat, pError, HRESULT_CODE (pError), lMessage, lExtra);
						}
						else
						{
#ifndef	_LOG_SKIPFILELINE
							_LogPrepFileLine (lFormat, mFile, mLine);
#endif
							LogMessage (pLogLevel, lFormat, pError, HRESULT_CODE (pError), lExtra);
						}
					}
					lLogged = true;
				}
			}
#ifdef	_CPPUNWIND
			catch (...)
			{
				try
				{
					LogMessage (pLogLevel, _T("*** LogComErr Exception ***"));
				}
				catch (...)
				{}
			}
#endif

			_LOG_TRY
			{
				if	(lErrorSource)
				{
					SysFreeString (lErrorSource);
				}
			}
			_LOG_CATCH

			_LOG_TRY
			{
				if	(lErrorStr)
				{
					SysFreeString (lErrorStr);
				}
			}
			_LOG_CATCH

			_LOG_TRY
			{
				if	(lErrorInfo)
				{
					lErrorInfo->Release ();
				}
			}
			_LOG_CATCH

			_LOG_TRY
			{
				if	(lMessage)
				{
					LocalFree (lMessage);
				}
			}
			_LOG_CATCH

			_LOG_TRY
			{
				if  (lExtra)
				{
#if (_MSC_VER >= 1400)
					delete lExtra;
#else
					delete [] lExtra;
#endif
				}
			}
			_LOG_CATCH
		}
		_LOG_CATCH
	}

    return pError;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DWORD LogStart (bool pNewLogFile, LPCTSTR pLogFileName, UINT pLogLevel)
{
	DWORD	lRet = 0;

	_LOG_ENTER_CRITICAL_SECTION

#ifdef __cplusplus
	_LOG_TRY
	{
		_LOG_ENTER_CRITICAL_SECTION

		_LOG_TRY
		{
			sLogFileHandle.Close ();
		}
		_LOG_CATCH

		_LOG_EXIT_CRITICAL_SECTION
	}
	_LOG_CATCH
#endif

	_LOG_TRY
	{
		bool	lFirstTime = (gLogFileName [0] == 0);
		UINT	lPrevLogLevel = gLogLevel;
		HKEY	lRegKey = 0;

#if	(_WIN32_WINNT >= 0x0500)
		if	(
				(sLogCloseSemaphore.IsValid())
			&&	(!sLogCloseSemaphore.IsOpen())
			)
		{
			sLogCloseSemaphore.Attach (CreateSemaphore (NULL, 0, 1, NULL), true, true);
		}
#endif
		if	(
				(lFirstTime)
			||	(pLogFileName)
			)
		{
			memset (gLogFileName, 0, sizeof (gLogFileName));
			if	(
					(pLogFileName)
				&&	(*pLogFileName)
				)
			{
				_tcscpy (gLogFileName, pLogFileName);
			}
		}
		LogControl (gLogFileName, pLogLevel);
		gLogLevel = pLogLevel;

		if	(
				((gLogLevel & LogLevelMask) != 0)
			&&	(gLogFileName [0])
			)
		{
			if	((gLogLevel & LogLevelMask) != (lPrevLogLevel & LogLevelMask))
			{
				lFirstTime = true;
			}

			if	(pNewLogFile)
			{
				_LOG_TRY
				{
					DWORD	lSaveError = GetLastError ();
					HANDLE	lLogFile;

					lLogFile = CreateFile (gLogFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, _LOG_FILE_SECURITY, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|_LOG_FILE_FLAGS, NULL);
					if	(lLogFile != INVALID_HANDLE_VALUE)
					{
						CloseHandle (lLogFile);
					}
					SetLastError (lSaveError);
				}
				_LOG_CATCH
			}

			if  (
					(
						(pNewLogFile)
					||	(lFirstTime)
					)
#ifdef	_LOG_LEVEL_START
				&&	(LogIsActive (_LOG_LEVEL_START))
#endif
				)
			{
				{
					HMODULE		lModule = 0;
					TCHAR		lProgramName [MAX_PATH];
#if	(WINVER >= 0x0600)
#ifdef	_WIN64
					LPCTSTR		lVersionSuffix = _T(" Vx64");
#else
					LPCTSTR		lVersionSuffix = _T(" Vx86");
#endif
#else
#ifdef	_UNICODE
					LPCTSTR		lVersionSuffix = _T(" W");
#else
					LPCTSTR		lVersionSuffix = _T("");
#endif
#endif
#ifdef	__AFXWIN_H__
					TCHAR		lModuleName [MAX_PATH];
					_LOG_TRY
					{
						lModule = afxCurrentInstanceHandle;
					}
					_LOG_CATCH
#endif
					if	(GetModuleFileName (0, lProgramName, MAX_PATH))
					{
#ifdef	__AFXWIN_H__
						if	(
								(lModule != 0)
							&&	(GetModuleFileName (lModule, lModuleName, MAX_PATH))
							&&	(_tcsicmp (lModuleName, lProgramName) != 0)
							)
						{
#ifdef	_LOG_VERSION_NAME
							LogMessage (LogDateTime|LogIfActive, _T("START Logging [%lX] From [%s] Version [%s%s] in [%s] [%s] to [%s]"), gLogLevel, lModuleName, (LPCTSTR) (CFileVersion (lModule).FileVersionString()), lVersionSuffix, lProgramName, (LPCTSTR) (CFileVersion ((HMODULE) 0).FileVersionString()), (LPCTSTR) gLogFileName);
#else
							LogMessage (LogDateTime|LogIfActive, _T("START Logging [%lX] From [%s] in [%s] to [%s]"), gLogLevel, lModuleName, lProgramName, (LPCTSTR) gLogFileName);
#endif
						}
						else
#endif
						{
#ifdef	_LOG_VERSION_NAME
							LogMessage (LogDateTime|LogIfActive, _T("START Logging [%lX] From [%s] Version [%s%s] to [%s]"), gLogLevel, lProgramName, (LPCTSTR) _LOG_VERSION_NAME, lVersionSuffix, (LPCTSTR) gLogFileName);
#else
							LogMessage (LogDateTime|LogIfActive, _T("START Logging [%lX] From [%s] to [%s]"), gLogLevel, lProgramName, (LPCTSTR) gLogFileName);
#endif
						}
					}
					else
					{
#ifdef	_LOG_VERSION_NAME
						LogMessage (LogDateTime|LogIfActive, _T("START Logging [%lX] From <unknown> Version [%s%s] to [%s]"), gLogLevel, (LPCTSTR) _LOG_VERSION_NAME, lVersionSuffix, (LPCTSTR) gLogFileName);
#else
						LogMessage (LogDateTime|LogIfActive, _T("START Logging [%lX] to [%s]"), gLogLevel, (LPCTSTR) gLogFileName);
#endif
					}
				}
				{
					TCHAR			lWindowsName [50];
					OSVERSIONINFO	lVersionInfo;
					TCHAR			lVersion [250];
					LPCTSTR			lProcessor = NULL;

					memset (lWindowsName, 0, sizeof (lWindowsName));
					if	(RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
					{
						DWORD	lValueSize;
						DWORD	lValueType;

						if	(
								(RegQueryValueEx (lRegKey, _T("ProductName"), NULL, &(lValueType = 0), NULL, &(lValueSize = sizeof (lWindowsName))) == ERROR_SUCCESS)
							&&	(lValueType == REG_SZ)
							&&	(lValueSize <= sizeof (lWindowsName))
							&&	(RegQueryValueEx (lRegKey, _T("ProductName"), NULL, &lValueType, (LPBYTE) lWindowsName, &lValueSize)) == ERROR_SUCCESS)
						{}
						RegCloseKey (lRegKey);
					}
					if	(!lWindowsName [0])
					{
						_tcscpy (lWindowsName, _T("Windows"));
					}

					lVersionInfo.dwOSVersionInfoSize = sizeof (lVersionInfo);
					if	(GetVersionEx (&lVersionInfo))
					{
						_stprintf (lVersion, _T("%u.%u.%u (%s)"), lVersionInfo.dwMajorVersion, lVersionInfo.dwMinorVersion, lVersionInfo.dwBuildNumber, lVersionInfo.szCSDVersion);
					}
					else
					{
						_stprintf (lVersion, _T("%d.%d"), LOBYTE (LOWORD (GetVersion ())), HIBYTE (HIWORD (GetVersion ())));
					}
#ifdef	_WIN64
					_tcscat (lVersion, _T(" (WIN64)"));
#elif	(_WIN32_WINNT >= 0x0501)
					{
						BOOL lWow64 = FALSE;
						if	(
								(IsWow64Process (GetCurrentProcess(), &lWow64))
							&&	(lWow64)
							)
						{
							_tcscat (lVersion, _T(" (WOW64)"));
						}
					}
#endif
#ifdef	_UNICODE
					TCHAR	lProcessorStr [MAX_PATH];

					if	(RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("Hardware\\Description\\System\\CentralProcessor\\0"), 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
					{
						DWORD	lValueSize;
						DWORD	lValueType;
						BYTE	lValue [MAX_PATH*2];

						if	(
								(RegQueryValueEx (lRegKey, _T("Identifier"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
							&&	(lValueType == REG_SZ)
							)
						{
							_tcscpy (lProcessorStr, (LPCTSTR) lValue);
							lProcessor = lProcessorStr;

							if	(
									(RegQueryValueEx (lRegKey, _T("VendorIdentifier"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
								&&	(lValueType == REG_SZ)
								)
							{
								_tcscat (lProcessorStr, _T(" "));
								_tcscat (lProcessorStr, (LPCTSTR) lValue);
							}

							if	(
									(RegQueryValueEx (lRegKey, _T("~MHz"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
								&&	(lValueType == REG_DWORD)
								)
							{
								TCHAR	Speed [20];
								_stprintf (Speed, _T("%uMHz"), *(DWORD*) lValue);
								_tcscat (lProcessorStr, _T(" "));
								_tcscat (lProcessorStr, Speed);
							}
						}

						RegCloseKey (lRegKey);
					}
					if	(!lProcessor)
#endif
					{
						SYSTEM_INFO lSysInfo;
						memset (&lSysInfo, 0, sizeof (lSysInfo));
						GetSystemInfo (&lSysInfo);
						switch (lSysInfo.dwProcessorType)
						{
							case PROCESSOR_INTEL_386: lProcessor = _T("I386"); break;
							case PROCESSOR_INTEL_486: lProcessor = _T("I486"); break;
							case PROCESSOR_INTEL_PENTIUM: lProcessor = _T("Pentium"); break;
						}
					}
					if	(!lProcessor)
					{
						lProcessor = _T("Unknown");
					}
					LogMessage (LogDateTime|LogIfActive, _T("  %s %s (Processor %s) (Locale System %8.8X User %8.8X Thread %8.8X)"), lWindowsName, lVersion, lProcessor, GetSystemDefaultLCID (), GetUserDefaultLCID (), GetThreadLocale ());
				}
			}

			_LOG_TRY
			{
				HANDLE	lLogFile;

				lLogFile = CreateFile (gLogFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, _LOG_FILE_SECURITY, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|_LOG_FILE_FLAGS, NULL);
				if	(lLogFile != INVALID_HANDLE_VALUE)
				{
					lRet = GetFileSize (lLogFile, NULL);
					CloseHandle (lLogFile);
				}
			}
			_LOG_CATCH
		}
	}
	_LOG_CATCH

	_LOG_EXIT_CRITICAL_SECTION

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void LogStop (unsigned int pPutLogEnd)
{
#ifdef	_CRTDBG_ALLOC_MEM_DF
	LogDebugRuntime (false);
#endif
#ifdef __cplusplus
	if	(gLogLevel & LogToCache)
	{
		LogWriteCache ();
	}
	gLogLevel &= ~LogToCache;

	_LOG_TRY
	{
		sLogFileHandle.Close ();
	}
	_LOG_CATCH
	_LOG_TRY
	{
		sLogCache.Empty ();
	}
	_LOG_CATCH
#if	(_WIN32_WINNT >= 0x0500)
	_LOG_TRY
	{
		sLogCloseTimer.Close ();
	}
	_LOG_CATCH
	_LOG_TRY
	{
		sLogCloseSemaphore.Close ();
	}
	_LOG_CATCH
#endif
#endif

	_LOG_TRY
	{
#ifdef	_LOG_LEVEL_STOP
		if	(LogIsActive(MaxLogLevel(pPutLogEnd,_LOG_LEVEL_STOP)))
#else
		if	(LogIsActive(pPutLogEnd))
#endif
		{
			HMODULE	lModule = 0;
			BOOL	lModuleNameValid = FALSE;
			TCHAR	lModuleName [MAX_PATH];

#ifdef	__AFXWIN_H__
			_LOG_TRY
			{
				lModule = afxCurrentInstanceHandle;
			}
			_LOG_CATCH
#endif
			_LOG_TRY
			{
				lModuleNameValid = GetModuleFileName (lModule, lModuleName, MAX_PATH);
			}
			_LOG_CATCH

			if	(lModuleNameValid)
			{
				LogMessage (LogDateTime|LogIfActive, _T("STOP Logging [%lX] From [%s]"), gLogLevel, lModuleName);
			}
			else
			{
				LogMessage (LogDateTime|LogIfActive, _T("STOP Logging [%lX]"), gLogLevel);
			}
		}
	}
	_LOG_CATCH

	_LOG_TRY
	{
		memset (gLogFileName, 0, sizeof(gLogFileName));
		gLogLevel = 0;
	}
	_LOG_CATCH
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void LogControl (LPTSTR pLogFileName, UINT& pLogLevel)
{
	UINT	lLogLevelSave = pLogLevel;
	HKEY	lRegKey = 0;

	pLogLevel &= LogSettingsMask;

#ifdef	_LOG_SECTION_NAME
	if	(
			(!pLogFileName [0])
		||	(!pLogLevel)
		)
	{
		_LOG_TRY
		{
#ifdef	_WIN32
			if	(RegOpenKeyEx (_LOG_ROOT_KEY, _LOG_ROOT_PATH _LOG_SECTION_NAME, 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
#else
			if	(RegOpenKey (_LOG_ROOT_KEY, _LOG_ROOT_PATH _LOG_SECTION_NAME, &lRegKey) == ERROR_SUCCESS)
#endif
			{
#ifdef	_WIN32
				DWORD	lValueSize;
				DWORD	lValueType;
#else
				long	lValueSize;
				long	lValueType;
#endif
				BYTE	lValue [MAX_PATH*2];

				if	(
						(!pLogFileName [0])
					&&	(RegQueryValueEx (lRegKey, _T("LogFile"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
					&&	(
							(lValueType == REG_SZ)
#ifdef	_WIN32
						||	(lValueType == REG_EXPAND_SZ)
#endif
						)
					)
				{
					_tcscpy (pLogFileName, (LPCTSTR) lValue);

#ifdef	_WIN32
					if	(lValueType == REG_EXPAND_SZ)
					{
						TCHAR	lExpand [MAX_PATH*2];
						if	(ExpandEnvironmentStrings (pLogFileName, lExpand, MAX_PATH*2))
						{
							_tcscpy (pLogFileName, lExpand);
						}
					}
#endif
				}

				if	(
						(!pLogLevel)
					&&	(RegQueryValueEx (lRegKey, _T("LogLevel"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
					&&	(lValueType == REG_DWORD)
					)
				{
					pLogLevel = (*(DWORD*) lValue) & LogSettingsMask;
				}
				RegCloseKey (lRegKey);
			}

#ifdef	_LOG_SECTION_NAME_ALT
#ifdef	_WIN32
			if	(RegOpenKeyEx (_LOG_ROOT_KEY, _LOG_ROOT_PATH _LOG_SECTION_NAME_ALT, 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
#else
			if	(RegOpenKey (_LOG_ROOT_KEY, _LOG_ROOT_PATH _LOG_SECTION_NAME_ALT, &lRegKey) == ERROR_SUCCESS)
#endif
			{
#ifdef	_WIN32
				DWORD	lValueSize;
				DWORD	lValueType;
#else
				long	lValueSize;
				long	lValueType;
#endif
				BYTE	lValue [MAX_PATH*2];

				if	(
						(RegQueryValueEx (lRegKey, _T("LogFile"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
					&&	(
							(lValueType == REG_SZ)
#ifdef	_WIN32
						||	(lValueType == REG_EXPAND_SZ)
#endif
						)
					)
				{
					_tcscpy (pLogFileName, (LPCTSTR) lValue);

#ifdef	_WIN32
					if	(lValueType == REG_EXPAND_SZ)
					{
						TCHAR	lExpand [MAX_PATH*2];
						if	(ExpandEnvironmentStrings (pLogFileName, lExpand, MAX_PATH*2))
						{
							_tcscpy (pLogFileName, lExpand);
						}
					}
#endif
				}

				if	(
						(RegQueryValueEx (lRegKey, _T("LogLevel"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
					&&	(lValueType == REG_DWORD)
					&&	(((*(DWORD*) lValue) & LogLevelMask) > (pLogLevel & LogLevelMask))
					)
				{
					pLogLevel = (*(DWORD*) lValue) & LogSettingsMask;
				}
				RegCloseKey (lRegKey);
			}
#endif	// _LOG_SECTION_NAME_ALT
		}
		_LOG_CATCH
	}
#endif	// _LOG_SECTION_NAME

#ifndef	_LOG_LEVEL_DEBUG
#ifdef	_DEBUG_LOGLEVEL
#define	_LOG_LEVEL_DEBUG _DEBUG_LOGLEVEL
#endif
#endif

#ifdef	_LOG_LEVEL_DEBUG
	if	(
			(!pLogLevel)
		&&	(!lLogLevelSave)
		)
	{
		pLogLevel = _LOG_LEVEL_DEBUG;
	}
#endif

	if	((pLogLevel & LogLevelMask) != 0)
	{
		_LOG_TRY
		{
#ifdef	_LOG_NAME_PARSE
			_LOG_TRY
			{
				_LOG_NAME_PARSE (pLogFileName);
			}
			_LOG_CATCH
#endif

#ifdef	_LOG_DEF_LOGNAME
			if	(
					(!pLogFileName [0])
				&&	(_tcspbrk (_LOG_DEF_LOGNAME, _T(":")))
				)
			{
				_tcscpy (pLogFileName, _LOG_DEF_LOGNAME);
			}
			else
			if	(
					(!pLogFileName [0])
				||	(
						(pLogFileName [0])
					&&	(!_tcspbrk (pLogFileName, _T(":\\/")))
					)
				)
#else
			if	(
					(pLogFileName [0])
				&&	(!_tcspbrk (pLogFileName, _T(":\\/")))
				)
#endif	// _LOG_DEF_LOGNAME
			{
#ifdef	_WIN32
				DWORD	lValueSize;
				DWORD	lValueType;
#else
				long	lValueSize;
				long	lValueType;
#endif
				BYTE	lValue [MAX_PATH*2];
				TCHAR	lLogPathShared [MAX_PATH*2];
				TCHAR	lLogPathUser [MAX_PATH*2];
				TCHAR	lLogFileName [MAX_PATH*2];

				memset (lLogPathShared, 0, sizeof (lLogPathShared));
				memset (lLogPathUser, 0, sizeof (lLogPathUser));
				memset (lLogFileName, 0, sizeof (lLogFileName));

				lRegKey = 0;
#ifdef	_LOG_FILE_SHARED
				if	(
						(_LOG_FILE_SHARED)
#ifdef	_WIN32
					&&	(RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
#else
					&&	(RegOpenKey (HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", &lRegKey) == ERROR_SUCCESS)
#endif
					&&	(RegQueryValueEx (lRegKey, _T("Common Desktop"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
					&&	(
							(lValueType == REG_SZ)
						||	(lValueType == REG_EXPAND_SZ)
						)
					)
				{
					if	(lValueType == REG_EXPAND_SZ)
					{
						ExpandEnvironmentStrings ((LPCTSTR) lValue, lLogPathShared, MAX_PATH);
					}
					else
					{
						_tcscpy (lLogPathShared, (LPCTSTR) lValue);
					}
					_tcscat (lLogPathShared, _T("\\"));
				}
				if	(lRegKey)
				{
					RegCloseKey (lRegKey);
					lRegKey = 0;
				}
#endif
				if	(
#ifdef	_WIN32
						(RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), 0, KEY_READ, &lRegKey) == ERROR_SUCCESS)
#else
						(RegOpenKey (HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", &lRegKey) == ERROR_SUCCESS)
#endif
					&&	(RegQueryValueEx (lRegKey, _T("Desktop"), NULL, &(lValueType = 0), lValue, &(lValueSize = sizeof (lValue))) == ERROR_SUCCESS)
					&&	(
							(lValueType == REG_SZ)
						||	(lValueType == REG_EXPAND_SZ)
						)
					)
				{
					if	(lValueType == REG_EXPAND_SZ)
					{
						ExpandEnvironmentStrings ((LPCTSTR) lValue, lLogPathUser, MAX_PATH);
					}
					else
					{
						_tcscpy (lLogPathUser, (LPCTSTR) lValue);
					}
					_tcscat (lLogPathUser, _T("\\"));
				}
				if	(lRegKey)
				{
					RegCloseKey (lRegKey);
				}

				if	(lLogPathShared [0])
				{
#ifdef	_LOG_DEF_LOGNAME
					if	(!pLogFileName [0])
					{
						_tcscpy (lLogFileName, lLogPathShared);
						_tcscat (lLogFileName, _LOG_DEF_LOGNAME);
					}
					else
#endif
					{
						_tcscpy (lLogFileName, lLogPathShared);
						_tcscat (lLogFileName, pLogFileName);
					}

					_LOG_TRY
					{
						DWORD	lSaveError = GetLastError ();
						HANDLE	lLogFile;

						lLogFile = CreateFile (lLogFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, _LOG_FILE_SECURITY, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|_LOG_FILE_FLAGS, NULL);

						if	(lLogFile == INVALID_HANDLE_VALUE)
						{
							if	(GetLastError () == ERROR_ACCESS_DENIED)
							{
								memset (lLogFileName, 0, sizeof (lLogFileName));
							}
						}
						else
						{
							CloseHandle (lLogFile);
						}
						SetLastError (lSaveError);
					}
					_LOG_CATCH
				}

				if	(
						(!lLogFileName [0])
					&&	(lLogPathUser [0])
					)
				{
#ifdef	_LOG_DEF_LOGNAME
					if	(!pLogFileName [0])
					{
						_tcscpy (lLogFileName, lLogPathUser);
						_tcscat (lLogFileName, _LOG_DEF_LOGNAME);
					}
					else
#endif
					{
						_tcscpy (lLogFileName, lLogPathUser);
						_tcscat (lLogFileName, pLogFileName);
					}
				}

#ifdef	_LOG_DEF_LOGNAME
				if	(!lLogFileName [0])
				{
					_tcscpy (lLogFileName, _LOG_DEF_LOGNAME);
				}
#endif
				if	(lLogFileName [0])
				{
					_tcscpy (pLogFileName, lLogFileName);
				}
			}
		}
		_LOG_CATCH
	}
}

/////////////////////////////////////////////////////////////////////////////

bool LogLock ()
{
	bool	lRet = false;
#ifdef	_LOG_CRITICAL_SECTION
	_LOG_TRY
	{
		if	(_LOG_CRITICAL_SECTION != NULL)
		{
			EnterCriticalSection (_LOG_CRITICAL_SECTION);
			lRet = true;
		}
	}
	_LOG_CATCH
#endif
	return lRet;
}

bool LogUnlock ()
{
	bool	lRet = false;
#ifdef	_LOG_CRITICAL_SECTION
	_LOG_TRY
	{
		if	(_LOG_CRITICAL_SECTION != NULL)
		{
			LeaveCriticalSection (_LOG_CRITICAL_SECTION);
			lRet = true;
		}
	}
	_LOG_CATCH
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
#ifndef	_LOG_NO_EXTERN_C
}
#endif
#endif
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
static bool	gLogCrtAssert = true;
static bool	gLogCrtError = true;
static bool	gLogCrtWarning = false;

#ifndef	_CRT_WARN
#define	_CRT_WARN	0
#endif
#ifndef	_CRT_ERROR
#define	_CRT_ERROR	1
#endif
#ifndef	_CRT_ASSERT
#define _CRT_ASSERT	2
#endif

int __cdecl LogCrtDbgReport (int pRptType, char * pMsg, int * pRetVal)
{
	_LOG_TRY
	{
		static	bool	lRecursionBreak = false;
		static	char	lMsg [2048] = "";

		if	(
				(!lRecursionBreak)
			&&	(
					(pRptType != _CRT_ASSERT)
				||	(gLogCrtAssert)
				)
			&&	(
					(pRptType != _CRT_ERROR)
				||	(gLogCrtError)
				)
			&&	(
					(pRptType != _CRT_WARN)
				||	(gLogCrtWarning)
				)
			)
		{
			lRecursionBreak = true;
			_LOG_TRY
			{
				char *	lEnd = strpbrk (pMsg, "\r\n");

				if	(lEnd)
				{
					(*lEnd) = '\0';

					strcat (lMsg, pMsg);
					LogMessage (LogNormal|LogHighVolume, _T("<CRT> %hs"), lMsg);
					memset (lMsg, 0, sizeof (lMsg));

					if	(
							(pRptType == _CRT_ASSERT)
						&&	(IsDebuggerPresent())
						)
					{
						DebugBreak ();
					}
				}
				else
				{
					strcat (lMsg, pMsg);
				}
			}
			_LOG_CATCH
			lRecursionBreak = false;
		}
	}
#ifdef	_CPPUNWIND
	catch (...)
	{
		return TRUE;
	}
#endif
	return FALSE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_CRTDBG_ALLOC_MEM_DF
/////////////////////////////////////////////////////////////////////////////
void LogDebugRuntime (bool pDebugRuntime, bool pAsserts, bool pErrors, bool pWarnings)
{
#ifndef	__cplusplus_cli
#ifdef	_DEBUG
	_LOG_TRY
	{
		if	(pDebugRuntime)
		{
			gLogCrtAssert = pAsserts;
			gLogCrtError = pErrors;
			gLogCrtWarning = pWarnings;

			_CrtSetReportMode (_CRT_WARN, pWarnings?_CRTDBG_MODE_DEBUG:0);
			_CrtSetReportMode (_CRT_ERROR, pErrors?_CRTDBG_MODE_DEBUG:0);
			_CrtSetReportMode (_CRT_ASSERT, pAsserts?_CRTDBG_MODE_DEBUG:0);
			_CrtSetReportHook (LogCrtDbgReport);
		}
		else
		{
			_CrtSetReportHook (NULL);
		}
	}
	_LOG_CATCH
#endif
#endif
}

int LogDebugMemory (int pDbgFlag)
{
	int	lDbgFlag = 0;
#ifndef	__cplusplus_cli
#ifdef	_DEBUG
	_LOG_TRY
	{
		lDbgFlag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
		_CrtSetDbgFlag (lDbgFlag & ~(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF|_CRTDBG_DELAY_FREE_MEM_DF) | pDbgFlag);
		LogMessage (LogIfActive, _T("_CrgDbgFlag from [%4.4X] to [%4.4X]"), lDbgFlag, _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG));
	}
	_LOG_CATCH
#endif
#endif
	return lDbgFlag;
}
/////////////////////////////////////////////////////////////////////////////
#endif	// _CRTDBG_ALLOC_MEM_DF
/////////////////////////////////////////////////////////////////////////////
#endif	// _LOG_DISABLED
/////////////////////////////////////////////////////////////////////////////
#undef	_LOG_TRY
#undef	_LOG_CATCH
#undef	_LOG_ENTER_CRITICAL_SECTION
#undef	_LOG_EXIT_CRITICAL_SECTION
/////////////////////////////////////////////////////////////////////////////
