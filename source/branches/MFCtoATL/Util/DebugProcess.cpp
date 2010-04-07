/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#include "StdAfx.h"
#define PSAPI_VERSION 1
#include <psapi.h>
#include <tlhelp32.h>
#include "DebugProcess.h"
#include "Localize.h"
#include "LocalizeEx.h"
#include "Log.h"

#pragma comment(lib, "psapi.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////

CDebugProcess::CDebugProcess ()
{
	mProcessHandle = GetCurrentProcess ();
	mProcessId = GetCurrentProcessId ();
}

CDebugProcess::CDebugProcess (HANDLE mProcessHandle)
:	mProcessHandle (mProcessHandle),
	mProcessId (0)
{
}

CDebugProcess::CDebugProcess (DWORD mProcessId)
:	mProcessHandle (NULL),
	mProcessId (mProcessId)
{
	mProcessHandle = OpenProcess (PROCESS_QUERY_INFORMATION, FALSE, mProcessId);
}

CDebugProcess::~CDebugProcess ()
{
	if	(mProcessHandle)
	{
		CloseHandle (mProcessHandle);
	}
}

/////////////////////////////////////////////////////////////////////////////

#define InitLogTitle(pFormat, pTitle, pIndent, pDefaultTitle) \
if	(pFormat) \
{ \
	va_list	lArgPtr; \
	va_start (lArgPtr, pFormat); \
	_vsntprintf (pTitle.GetBuffer(2048), 2048, pFormat, lArgPtr); \
	pTitle.ReleaseBuffer (); \
	pIndent = pTitle; \
	pTitle.TrimLeft (); \
	pIndent = CString (_T(' '), pIndent.GetLength()-pTitle.GetLength()); \
	if (!lTitle.IsEmpty()) lTitle += _T(' '); \
} \
else \
{ \
	pTitle = pDefaultTitle; \
}

//////////////////////////////////////////////////////////////////////

void CDebugProcess::LogWorkingSet (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessHandle != NULL)
		)
	{
		try
		{
			CString									lTitle;
			CString									lIndent;
			tSS <PROCESS_MEMORY_COUNTERS, DWORD>	lCounters;

			InitLogTitle (pFormat, lTitle, lIndent, _T("WorkingSet"));
			lTitle.TrimRight ();
			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);

			if	(GetProcessMemoryInfo (mProcessHandle, &lCounters, sizeof (lCounters)))
			{
				LogMessage (pLogLevel, _T("%s  WorkingSet    [%8u] Peak [%8u] (%s %s)"), lIndent, lCounters.WorkingSetSize, lCounters.PeakWorkingSetSize, LclFormatSize(lCounters.WorkingSetSize), LclFormatSize(lCounters.PeakWorkingSetSize));
				LogMessage (pLogLevel, _T("%s  PagedPool     [%8u] Peak [%8u] (%s %s)"), lIndent, lCounters.QuotaPagedPoolUsage, lCounters.QuotaPeakPagedPoolUsage, LclFormatSize(lCounters.QuotaPagedPoolUsage), LclFormatSize(lCounters.QuotaPeakPagedPoolUsage));
				LogMessage (pLogLevel, _T("%s  NonPagedPool  [%8u] Peak [%8u] (%s %s)"), lIndent, lCounters.QuotaNonPagedPoolUsage, lCounters.QuotaPeakNonPagedPoolUsage, LclFormatSize(lCounters.QuotaNonPagedPoolUsage), LclFormatSize(lCounters.QuotaPeakNonPagedPoolUsage));
				LogMessage (pLogLevel, _T("%s  PageFile      [%8u] Peak [%8u] (%s %s)"), lIndent, lCounters.PagefileUsage, lCounters.PeakPagefileUsage, LclFormatSize(lCounters.PagefileUsage), LclFormatSize(lCounters.PeakPagefileUsage));
				LogMessage (pLogLevel, _T("%s    Faults      [%8u]"), lIndent, lCounters.PageFaultCount);
			}
			else
			{
				LogWinErr (pLogLevel, GetLastError (), _T("GetProcessMemoryInfo"));
			}
		}
		catch AnyExceptionDebug
	}
}

void CDebugProcess::LogWorkingSetInline (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessHandle != NULL)
		)
	{
		try
		{
			CString									lTitle;
			CString									lIndent;
			tSS <PROCESS_MEMORY_COUNTERS, DWORD>	lCounters;

			InitLogTitle (pFormat, lTitle, lIndent, _T(""));
			GetProcessMemoryInfo (mProcessHandle, &lCounters, sizeof (lCounters));
			LogMessage (pLogLevel, _T("%s%s%-12s [%12s] %-12s [%12s] %-12s [%12s] %-12s [%12s]"), lIndent, lTitle, _T("WorkingSet"), LclFormatNumber((double)(LONGLONG)lCounters.WorkingSetSize), _T("PagedPool"), LclFormatNumber((double)(LONGLONG)lCounters.QuotaPagedPoolUsage), _T("NonPagedPool"), LclFormatNumber((double)(LONGLONG)lCounters.QuotaNonPagedPoolUsage), _T("PageFile"), LclFormatNumber((double)(LONGLONG)lCounters.PagefileUsage));
			lTitle = CString (_T(' '), lTitle.GetLength());
			LogMessage (pLogLevel, _T("%s%s%-12s [%12s] %-12s [%12s] %-12s [%12s] %-12s [%12s]"), lIndent, lTitle, _T(""), LclFormatSize((double)(LONGLONG)lCounters.WorkingSetSize), _T(""), LclFormatSize((double)(LONGLONG)lCounters.QuotaPagedPoolUsage), _T(""), LclFormatSize((double)(LONGLONG)lCounters.QuotaNonPagedPoolUsage), _T(""), LclFormatSize((double)(LONGLONG)lCounters.PagefileUsage));
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CDebugProcess::LogGuiResources (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessHandle != NULL)
		)
	{
		try
		{
			CString	lTitle;
			CString	lIndent;
			DWORD	lHandleCount = 0;

			InitLogTitle (pFormat, lTitle, lIndent, _T("GuiResources"));
			lTitle.TrimRight ();
			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
			if	(GetProcessHandleCount (mProcessHandle, &lHandleCount))
			{
				LogMessage (pLogLevel, _T("%s  Handles     [%u]"), lIndent, lHandleCount);
			}
			LogMessage (pLogLevel, _T("%s  GdiObjects  [%u]"), lIndent, GetGuiResources (GetCurrentProcess (), GR_GDIOBJECTS));
			LogMessage (pLogLevel, _T("%s  UserObjects [%u]"), lIndent, GetGuiResources (GetCurrentProcess (), GR_USEROBJECTS));
		}
		catch AnyExceptionDebug
	}
}

void CDebugProcess::LogGuiResourcesInline (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessHandle != NULL)
		)
	{
		try
		{
			CString	lTitle;
			CString	lIndent;
			DWORD	lHandleCount = 0;

			InitLogTitle (pFormat, lTitle, lIndent, _T(""));
			GetProcessHandleCount (mProcessHandle, &lHandleCount);
			LogMessage (pLogLevel, _T("%s%s%-12s [%12s] %-12s [%12s] %-12s [%12s] %s"), lIndent, lTitle, _T("Handles"), LclFormatNumber ((long)lHandleCount), _T("GdiObjects"), LclFormatNumber ((long)GetGuiResources (GetCurrentProcess (), GR_GDIOBJECTS)), _T("UserObjects"), LclFormatNumber ((long)GetGuiResources (GetCurrentProcess (), GR_USEROBJECTS)), lTitle);
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CDebugProcess::LogAddressSpace (UINT pLogLevel, bool pLogSections, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessHandle != NULL)
		)
	{
		try
		{
			CString	lTitle;
			CString	lIndent;

			InitLogTitle (pFormat, lTitle, lIndent, _T("AddressSpace"));
			lTitle.TrimRight ();
			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);

			try
			{
				LPBYTE							lBase;
				tS <MEMORY_BASIC_INFORMATION>	lInfo;
				ULONGLONG						lImageCommit = 0;
				ULONGLONG						lMappedCommit = 0;
				ULONGLONG						lPrivateCommit = 0;
				ULONGLONG						lImageReserve = 0;
				ULONGLONG						lMappedReserve = 0;
				ULONGLONG						lPrivateReserve = 0;
				ULONGLONG						lTotalCommit = 0;
				ULONGLONG						lTotalReserve = 0;
				ULONGLONG						lTotalImage = 0;
				ULONGLONG						lTotalMapped = 0;
				ULONGLONG						lTotalPrivate = 0;
				ULONGLONG						lTotalGuard = 0;
				ULONGLONG						lNoAccessCommit = 0;
				ULONGLONG						lReadOnlyCommit = 0;
				ULONGLONG						lReadWriteCommit = 0;
				ULONGLONG						lWriteCopyCommit = 0;
				ULONGLONG						lExecCommit = 0;
				ULONGLONG						lExecReadCommit = 0;
				ULONGLONG						lExecReadWriteCommit = 0;
				ULONGLONG						lExecWriteCopyCommit = 0;
				ULONGLONG						lTotalUsed = 0;
				ULONGLONG						lTotalUnused = 0;

				for	(lBase = 0; lBase <= (LPBYTE) 0xFFFFFF00; lBase += lInfo.RegionSize)
				{
					if	(
							(!VirtualQueryEx (mProcessHandle, lBase, &lInfo, sizeof(lInfo)))
						||	(lInfo.RegionSize == 0)
						)
					{
						break;
					}

					if	(lInfo.Type)
					{
						if	(pLogSections)
						{
							CString	lProtect;
							CString	lAllocProtect;
							CString	lState;
							CString	lType;

							switch (lInfo.State)
							{
								case MEM_COMMIT:				lState = _T("Commit "); break;
								case MEM_RESERVE:				lState = _T("Reserve"); break;
								case MEM_FREE:					lState = _T("Free   "); break;
							}

							switch (lInfo.Type)
							{
								case MEM_IMAGE:					lType = _T("Image  "); break;
								case MEM_MAPPED:				lType = _T("Mapped "); break;
								case MEM_PRIVATE:				lType = _T("Private"); break;
							}

							switch (lInfo.Protect & 0xFF)
							{
								case PAGE_NOACCESS:				lProtect = _T("NoAccess"); break;
								case PAGE_READONLY:				lProtect = _T("ReadOnly"); break;
								case PAGE_READWRITE:			lProtect = _T("ReadWrite"); break;
								case PAGE_WRITECOPY:			lProtect = _T("WriteCopy"); break;
								case PAGE_EXECUTE:				lProtect = _T("Execute"); break;
								case PAGE_EXECUTE_READ:			lProtect = _T("ExecuteRead"); break;
								case PAGE_EXECUTE_READWRITE:	lProtect = _T("ExecuteReadWrite"); break;
								case PAGE_EXECUTE_WRITECOPY:	lProtect = _T("ExecuteWriteCopy"); break;
							}

							switch (lInfo.Protect & 0xFF00)
							{
								case PAGE_GUARD:				lProtect += _T(" Guard"); break;
								case PAGE_NOCACHE:				lProtect += _T(" NoCache"); break;
								case PAGE_WRITECOMBINE:			lProtect += _T(" WriteCombine"); break;
							}

							switch (lInfo.AllocationProtect & 0xFF)
							{
								case PAGE_NOACCESS:				lAllocProtect = _T("NoAccess"); break;
								case PAGE_READONLY:				lAllocProtect = _T("ReadOnly"); break;
								case PAGE_READWRITE:			lAllocProtect = _T("ReadWrite"); break;
								case PAGE_WRITECOPY:			lAllocProtect = _T("WriteCopy"); break;
								case PAGE_EXECUTE:				lAllocProtect = _T("Execute"); break;
								case PAGE_EXECUTE_READ:			lAllocProtect = _T("ExecuteRead"); break;
								case PAGE_EXECUTE_READWRITE:	lAllocProtect = _T("ExecuteReadWrite"); break;
								case PAGE_EXECUTE_WRITECOPY:	lAllocProtect = _T("ExecuteWriteCopy"); break;
							}

							switch (lInfo.AllocationProtect & 0xFF00)
							{
								case PAGE_GUARD:				lAllocProtect += _T(" Guard"); break;
								case PAGE_NOCACHE:				lAllocProtect += _T(" NoCache"); break;
								case PAGE_WRITECOMBINE:			lAllocProtect += _T(" WriteCombine"); break;
							}

							if	(
									(lInfo.State == MEM_RESERVE)
								||	(lProtect.IsEmpty ())
								)
							{
								lProtect = lAllocProtect;
							}
							else
							if	(lAllocProtect != lProtect)
							{
								lProtect += _T(" (") + lAllocProtect + _T(")");
							}

							LogMessage (pLogLevel|LogHighVolume, _T("  Base [%p] in [%p] [%s %s] Size [%I64u] %-10.10s [%s]"), lInfo.BaseAddress, lInfo.AllocationBase, lType, lState, (ULONGLONG)lInfo.RegionSize, LclFormatSize (lInfo.RegionSize), lProtect);
						}

						switch (lInfo.Type)
						{
							case MEM_IMAGE:
							{
								lTotalImage += lInfo.RegionSize;

								switch (lInfo.State)
								{
									case MEM_COMMIT:
									{
										lImageCommit += lInfo.RegionSize;
										lTotalCommit += lInfo.RegionSize;
									}	break;
									case MEM_RESERVE:
									{
										lImageReserve += lInfo.RegionSize;
										lTotalReserve += lInfo.RegionSize;
									}	break;
								}
							}	break;
							case MEM_MAPPED:
							{
								lTotalMapped += lInfo.RegionSize;

								switch (lInfo.State)
								{
									case MEM_COMMIT:
									{
										lMappedCommit += lInfo.RegionSize;
										lTotalCommit += lInfo.RegionSize;
									}	break;
									case MEM_RESERVE:
									{
										lMappedReserve += lInfo.RegionSize;
										lTotalReserve += lInfo.RegionSize;
									}	break;
								}
							}	break;
							case MEM_PRIVATE:
							{
								lTotalPrivate += lInfo.RegionSize;

								switch (lInfo.State)
								{
									case MEM_COMMIT:
									{
										lPrivateCommit += lInfo.RegionSize;
										lTotalCommit += lInfo.RegionSize;
									}	break;
									case MEM_RESERVE:
									{
										lPrivateReserve += lInfo.RegionSize;
										lTotalReserve += lInfo.RegionSize;
									}	break;
								}
							}	break;
						}

						if	((lInfo.Protect & 0xFF00) == PAGE_GUARD)
						{
							lTotalGuard += lInfo.RegionSize;
						}

						if	(lInfo.State == MEM_COMMIT)
						{
							switch (lInfo.Protect & 0xFF)
							{
								case PAGE_NOACCESS:
								{
									lNoAccessCommit += lInfo.RegionSize;
								}	break;
								case PAGE_READONLY:
								{
									lReadOnlyCommit += lInfo.RegionSize;
								}	break;
								case PAGE_READWRITE:
								{
									lReadWriteCommit += lInfo.RegionSize;
								}	break;
								case PAGE_WRITECOPY:
								{
									lWriteCopyCommit += lInfo.RegionSize;
								}	break;
								case PAGE_EXECUTE:
								{
									lExecCommit += lInfo.RegionSize;
								}	break;
								case PAGE_EXECUTE_READ:
								{
									lExecReadCommit += lInfo.RegionSize;
								}	break;
								case PAGE_EXECUTE_READWRITE:
								{
									lExecReadWriteCommit += lInfo.RegionSize;
								}	break;
								case PAGE_EXECUTE_WRITECOPY:
								{
									lExecWriteCopyCommit += lInfo.RegionSize;
								}	break;
							}
						}

						lTotalUsed += lInfo.RegionSize;
					}
					else
					{
						lTotalUnused += lInfo.RegionSize;
					}
				}

				if	(pLogSections)
				{
					LogMessage (pLogLevel, _T(""));
				}
				LogMessage (pLogLevel|LogHighVolume, _T("  Image Commit    [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lImageCommit), LclFormatNumber ((double)(LONGLONG)lImageCommit));
				LogMessage (pLogLevel|LogHighVolume, _T("  Mapped Commit   [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lMappedCommit), LclFormatNumber ((double)(LONGLONG)lMappedCommit));
				LogMessage (pLogLevel|LogHighVolume, _T("  Private Commit  [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lPrivateCommit), LclFormatNumber ((double)(LONGLONG)lPrivateCommit));
				LogMessage (pLogLevel|LogHighVolume, _T("  Image Reserve   [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lImageReserve), LclFormatNumber ((double)(LONGLONG)lImageReserve));
				LogMessage (pLogLevel|LogHighVolume, _T("  Mapped Reserve  [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lMappedReserve), LclFormatNumber ((double)(LONGLONG)lMappedReserve));
				LogMessage (pLogLevel|LogHighVolume, _T("  Private Reserve [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lPrivateReserve), LclFormatNumber ((double)(LONGLONG)lPrivateReserve));
				if	(pLogSections)
				{
					LogMessage (pLogLevel, _T(""));
					LogMessage (pLogLevel|LogHighVolume, _T("  Total Image     [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalImage), LclFormatNumber ((double)(LONGLONG)lTotalImage));
					LogMessage (pLogLevel|LogHighVolume, _T("  Total Mapped    [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalMapped), LclFormatNumber ((double)(LONGLONG)lTotalMapped));
					LogMessage (pLogLevel|LogHighVolume, _T("  Total Private   [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalPrivate), LclFormatNumber ((double)(LONGLONG)lTotalPrivate));
					LogMessage (pLogLevel|LogHighVolume, _T("  Total Commit    [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalCommit), LclFormatNumber ((double)(LONGLONG)lTotalCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Total Reserve   [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalReserve), LclFormatNumber ((double)(LONGLONG)lTotalReserve));
					LogMessage (pLogLevel|LogHighVolume, _T("  Total Guard     [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalGuard), LclFormatNumber ((double)(LONGLONG)lTotalGuard));
					LogMessage (pLogLevel, _T(""));
					LogMessage (pLogLevel|LogHighVolume, _T("  No Access       [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lNoAccessCommit), LclFormatNumber ((double)(LONGLONG)lNoAccessCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Read Only       [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lReadOnlyCommit), LclFormatNumber ((double)(LONGLONG)lReadOnlyCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Read/Write      [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lReadWriteCommit), LclFormatNumber ((double)(LONGLONG)lReadWriteCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Write Copy      [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lWriteCopyCommit), LclFormatNumber ((double)(LONGLONG)lWriteCopyCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Exec            [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lExecCommit), LclFormatNumber ((double)(LONGLONG)lExecCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Exec Read       [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lExecReadCommit), LclFormatNumber ((double)(LONGLONG)lExecReadCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Exec Read/Write [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lExecReadWriteCommit), LclFormatNumber ((double)(LONGLONG)lExecReadWriteCommit));
					LogMessage (pLogLevel|LogHighVolume, _T("  Exec Write Copy [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lExecWriteCopyCommit), LclFormatNumber ((double)(LONGLONG)lExecWriteCopyCommit));
					LogMessage (pLogLevel, _T(""));
				}
				LogMessage (pLogLevel|LogHighVolume, _T("  Used            [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalUsed), LclFormatNumber ((double)(LONGLONG)lTotalUsed));
				LogMessage (pLogLevel|LogHighVolume, _T("  Unused          [%s (%s)]"), LclFormatSize ((double)(LONGLONG)lTotalUnused), LclFormatNumber ((double)(LONGLONG)lTotalUnused));
			}
			catch AnyExceptionDebug
		}
		catch AnyExceptionDebug
	}
}

void CDebugProcess::LogAddressSpaceInline (UINT pLogLevel, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessHandle != NULL)
		)
	{
		try
		{
			CString							lTitle;
			CString							lIndent;
			LPBYTE							lBase;
			tS <MEMORY_BASIC_INFORMATION>	lInfo;
			ULONGLONG						lImageCommit = 0;
			ULONGLONG						lMappedCommit = 0;
			ULONGLONG						lPrivateCommit = 0;
			ULONGLONG						lTotalCommit = 0;

			InitLogTitle (pFormat, lTitle, lIndent, _T(""));

			for	(lBase = 0; lBase <= (LPBYTE) 0xFFFFFF00; lBase += lInfo.RegionSize)
			{
				if	(
						(!VirtualQueryEx (mProcessHandle, lBase, &lInfo, sizeof(lInfo)))
					||	(lInfo.RegionSize == 0)
					)
				{
					break;
				}

				switch (lInfo.Type)
				{
					case MEM_IMAGE:
					{
						if	(lInfo.State == MEM_COMMIT)
						{
							lImageCommit += lInfo.RegionSize;
							lTotalCommit += lInfo.RegionSize;
						}
					}	break;
					case MEM_MAPPED:
					{
						if	(lInfo.State == MEM_COMMIT)
						{
							lMappedCommit += lInfo.RegionSize;
							lTotalCommit += lInfo.RegionSize;
						}
					}	break;
					case MEM_PRIVATE:
					{
						if	(lInfo.State == MEM_COMMIT)
						{
							lPrivateCommit += lInfo.RegionSize;
							lTotalCommit += lInfo.RegionSize;
						}
					}
				}
			}

			LogMessage (pLogLevel, _T("%s%s%-12s [%12s] %-12s [%12s] %-12s [%12s] %-12s [%12s]"), lIndent, lTitle, _T("Committed"), LclFormatNumber ((double)(LONGLONG)lTotalCommit), _T("Image"), LclFormatNumber ((double)(LONGLONG)lImageCommit), _T("Mapped"), LclFormatNumber ((double)(LONGLONG)lMappedCommit), _T("Private"), LclFormatNumber ((double)(LONGLONG)lPrivateCommit));
			lTitle = CString (_T(' '), lTitle.GetLength());
			LogMessage (pLogLevel, _T("%s%s%-12s [%12s] %-12s [%12s] %-12s [%12s] %-12s [%12s]"), lIndent, lTitle, _T(""), LclFormatSize ((double)(LONGLONG)lTotalCommit), _T(""), LclFormatSize ((double)(LONGLONG)lImageCommit), _T(""), LclFormatSize ((double)(LONGLONG)lMappedCommit), _T(""), LclFormatSize ((double)(LONGLONG)lPrivateCommit));
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CDebugProcess::LogThreads (UINT pLogLevel, bool pLogDetails, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessId != NULL)
		)
	{
		try
		{
			CString									lTitle;
			CString									lIndent;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Threads"));
			if	(pLogDetails)
			{
				lTitle.TrimRight ();
				LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
			}

			try
			{
				HANDLE						lSnapshot;
				BOOL						lThreadFound;
				tSS <THREADENTRY32, SIZE_T>	lThread;
				long						lThreadCount = 0;

				lSnapshot = CreateToolhelp32Snapshot (TH32CS_SNAPTHREAD, mProcessId);

				if	(lSnapshot != INVALID_HANDLE_VALUE)
				{
					try
					{
						for	(lThreadFound = Thread32First (lSnapshot, &lThread); lThreadFound; lThreadFound = Thread32Next (lSnapshot, &lThread))
						{
							if	(lThread.th32OwnerProcessID == mProcessId)
							{
								lThreadCount++;
								if	(pLogDetails)
								{
									LogMessage (pLogLevel|LogHighVolume, _T("%s  [%4.4u (%8.8X)] Priority [%d]"), lIndent, lThread.th32ThreadID, lThread.th32ThreadID, lThread.tpBasePri);
								}
							}
							lThread.Clear ();
						}
					}
					catch AnyExceptionDebug

					CloseHandle (lSnapshot);
				}

				if	(pLogDetails)
				{
					LogMessage (pLogLevel, _T("%s  [%d] threads"), lIndent, lThreadCount);
				}
				else
				{
					LogMessage (pLogLevel, _T("%s%s[%d] threads"), lIndent, lTitle, lThreadCount);
				}
			}
			catch AnyExceptionDebug
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CDebugProcess::LogHeaps (UINT pLogLevel, bool pLogDetails, UINT pLogEntries, LPCTSTR pFormat, ...)
{
	if	(
			(LogIsActive (pLogLevel))
		&&	(mProcessId != 0)
		)
	{
		try
		{
			CString	lTitle;
			CString	lIndent;

			InitLogTitle (pFormat, lTitle, lIndent, _T("Heaps"));
			lTitle.TrimRight ();
			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);

			try
			{
 				HANDLE						lSnapshot;
				BOOL						lHeapFound;
				BOOL						lEntryFound;
				tSS <HEAPLIST32, SIZE_T>	lHeap;
				long						lHeapCount = 0;
				LONGLONG					lTotalUsedCount = 0;
				LONGLONG					lTotalFreeCount = 0;
				LONGLONG					lTotalUsedSize = 0;
				LONGLONG					lTotalFreeSize = 0;

				lSnapshot = CreateToolhelp32Snapshot (TH32CS_SNAPHEAPLIST, mProcessId);
				if	(lSnapshot != INVALID_HANDLE_VALUE)
				{
					try
					{
						for	(lHeapFound = Heap32ListFirst (lSnapshot, &lHeap); lHeapFound; lHeapFound = Heap32ListNext (lSnapshot, &lHeap))
						{
							UINT						lUsedCount = 0;
							long						lFixedCount = 0;
							long						lMoveableCount = 0;
							long						lFreeCount = 0;
							LONGLONG					lFixedSize = 0;
							LONGLONG					lMoveableSize = 0;
							LONGLONG					lFreeSize = 0;
							tSS <HEAPENTRY32, SIZE_T>	lHeapEntry;

							if	(pLogEntries)
							{
								for	(lEntryFound = Heap32First (&lHeapEntry, lHeap.th32ProcessID, lHeap.th32HeapID); lEntryFound; lEntryFound = Heap32Next (&lHeapEntry))
								{
									if	(lHeapEntry.dwFlags != LF32_FREE)
									{
										lUsedCount++;
									}
								}
								if	(lUsedCount <= pLogEntries)
								{
									LogMessage (pLogLevel|LogHighVolume, _T("  Heap [%p]"), lHeap.th32HeapID);
								}
							}
							else
							{
								lUsedCount = 1;
							}

							for	(lEntryFound = Heap32First (&lHeapEntry, lHeap.th32ProcessID, lHeap.th32HeapID); lEntryFound; lEntryFound = Heap32Next (&lHeapEntry))
							{
								if	(lHeapEntry.dwFlags == LF32_FIXED)
								{
									lFixedCount++;
									lFixedSize += lHeapEntry.dwBlockSize;
									if	(lUsedCount <= pLogEntries)
									{
										LogMessage (pLogLevel|LogHighVolume, _T("    Fixed [%p] [%s]"), lHeapEntry.dwAddress, LclFormatSize(lHeapEntry.dwBlockSize));
									}
								}
								else
								if	(lHeapEntry.dwFlags == LF32_MOVEABLE)
								{
									lMoveableCount++;
									lMoveableSize += lHeapEntry.dwBlockSize;
									if	(lUsedCount <= pLogEntries)
									{
										LogMessage (pLogLevel|LogHighVolume, _T("    Entry [%p] [%s] lock [%u]"), lHeapEntry.dwAddress, LclFormatSize(lHeapEntry.dwBlockSize), lHeapEntry.dwLockCount);
									}
								}
								else
								if	(lHeapEntry.dwFlags == LF32_FREE)
								{
									lFreeCount++;
									lFreeSize += lHeapEntry.dwBlockSize;
//									if	(lUsedCount <= pLogEntries)
//									{
//										LogMessage (pLogLevel|LogHighVolume, _T("    Free  [%p] [%s]"), lHeapEntry.dwAddress, LclFormatSize(lHeapEntry.dwBlockSize));
//									}
								}
								else
								{
									if	(lUsedCount <= pLogEntries)
									{
										LogMessage (pLogLevel|LogHighVolume, _T("    [%8.8x] [%p] [%s]"), lHeapEntry.dwFlags, lHeapEntry.dwAddress, LclFormatSize(lHeapEntry.dwBlockSize));
									}
								}
							}

							if	(pLogDetails)
							{
								LogMessage (pLogLevel|LogHighVolume, _T("  Heap [%p] Used [%4u] Free [%4u] Total [%4u] (Fixed [%s] [%s] Moveable [%s] [%s] Free [%s] [%s]) Total [%s] [%s]"), lHeap.th32HeapID, lFixedCount+lMoveableCount, lFreeCount, lFixedCount+lMoveableCount+lFreeCount, LclFormatNumber(lFixedCount), LclFormatSize(lFixedSize), LclFormatNumber(lMoveableCount), LclFormatSize(lMoveableSize), LclFormatNumber(lFreeCount), LclFormatSize(lFixedSize), LclFormatNumber(lFixedCount+lMoveableCount+lFixedCount), LclFormatSize(lFixedSize+lMoveableSize+lFreeSize));
							}
							lHeapCount++;
							lTotalUsedCount += lFixedCount + lMoveableCount;
							lTotalFreeCount += lFreeCount;
							lTotalUsedSize += lFixedSize + lMoveableSize;
							lTotalFreeSize += lFreeSize;

							lHeap.Clear ();
						}
					}
					catch AnyExceptionDebug

					CloseHandle (lSnapshot);

					if	(pLogDetails)
					{
						LogMessage (pLogLevel, _T("  Heaps [%d] Used [%6I64u] Free [%6I64u] Total [%6I64u] (Used [%s] [%s] Free [%s] [%s] Total [%s])"), lHeapCount, lTotalUsedCount, lTotalFreeCount, lTotalUsedCount+lTotalFreeCount, LclFormatNumber(lTotalUsedCount), LclFormatSize(lTotalUsedSize), LclFormatNumber(lTotalFreeCount), LclFormatSize(lTotalFreeSize), LclFormatSize(lTotalUsedSize+lTotalFreeSize));
					}
					else
					{
						LogMessage (pLogLevel, _T("[%4.4u] %s Heaps [%d] Used [%6I64u] Free [%6I64u] Total [%6I64u] (Used [%s] [%s] Free [%s] [%s] Total [%s])"), mProcessId, lTitle, lHeapCount, lTotalUsedCount, lTotalFreeCount, lTotalUsedCount+lTotalFreeCount, LclFormatNumber(lTotalUsedCount), LclFormatSize(lTotalUsedSize), LclFormatNumber(lTotalFreeCount), LclFormatSize(lTotalFreeSize), LclFormatSize(lTotalUsedSize+lTotalFreeSize));
					}
				}
			}
			catch AnyExceptionDebug
		}
		catch AnyExceptionDebug
	}
}
