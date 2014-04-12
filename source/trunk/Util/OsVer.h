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
#ifndef	_OSVER_H
#define	_OSVER_H
////////////////////////////////////////////////////////////////////////

#define	IsWindows2000()				(LOWORD(::GetVersion ())==0x0005)
#define	IsWindows2000_AtMost()		((LOBYTE(LOWORD(::GetVersion ()))<5)||IsWindows2000())
#define	IsWindows2000_AtLeast()		(LOBYTE(LOWORD(::GetVersion ()))>=5)

#define	IsWindowsXp()				((LOBYTE(LOWORD(::GetVersion ()))==5)&&(HIBYTE(LOWORD(::GetVersion ()))>0))
#define	IsWindowsXp_AtMost()		(LOBYTE(LOWORD(::GetVersion ()))<=5)
#define	IsWindowsXp_AtLeast()		((LOBYTE(LOWORD(::GetVersion ()))>5)||IsWindowsXp())

#define	IsWindowsVista()			(LOWORD(::GetVersion ())==0x0006)
#define	IsWindowsVista_AtMost()		((LOBYTE(LOWORD(::GetVersion ()))<6)||IsWindowsVista())
#define	IsWindowsVista_AtLeast()	(LOBYTE(LOWORD(::GetVersion ()))>=6)

#define	IsWindows7()				(LOWORD(::GetVersion ())==0x0106)
#define	IsWindows7_AtMost()			(LOBYTE(LOWORD(::GetVersion ()))=<6)
#define	IsWindows7_AtLeast()		((LOBYTE(LOWORD(::GetVersion ()))>6)||IsWindows7())

////////////////////////////////////////////////////////////////////////

static inline bool IsWindows9X()
{
	OSVERSIONINFO lVersionInfo;
	memset (&lVersionInfo, 0, sizeof(lVersionInfo));
	lVersionInfo.dwOSVersionInfoSize =	sizeof(lVersionInfo);
    GetVersionEx (&lVersionInfo);
	return (VER_PLATFORM_WIN32_NT != lVersionInfo.dwPlatformId);
}

static inline bool IsWindowsNT()
{
	OSVERSIONINFO lVersionInfo;
	memset (&lVersionInfo, 0, sizeof(lVersionInfo));
	lVersionInfo.dwOSVersionInfoSize =	sizeof(lVersionInfo);
    GetVersionEx (&lVersionInfo);
	return (VER_PLATFORM_WIN32_NT == lVersionInfo.dwPlatformId);
}

static inline bool IsWindows64()
{
	SYSTEM_INFO lSystemInfo;
	memset (&lSystemInfo, 0, sizeof(lSystemInfo));
#if	(_WIN32_WINNT >= 0x0501)
	GetNativeSystemInfo (&lSystemInfo);
#else
	void (WINAPI * lGetNativeSystemInfo) (LPSYSTEM_INFO) = NULL;
	if	(lGetNativeSystemInfo = (void (WINAPI *) (LPSYSTEM_INFO)) ::GetProcAddress (GetModuleHandle(_T("Kernel32")), "GetNativeSystemInfo"))
	{
		try
		{
			(*lGetNativeSystemInfo) (&lSystemInfo);
		}
		catch (...)
		{}
	}
#endif
	return (lSystemInfo.wProcessorArchitecture == 6) || (lSystemInfo.wProcessorArchitecture == 9);
}

static inline bool IsWow64()
{
	BOOL lIsWow64 = FALSE;
#if	(_WIN32_WINNT >= 0x0501)
	IsWow64Process (GetCurrentProcess(), &lIsWow64);
#else
	BOOL (WINAPI * lIsWow64Process) (HANDLE, PBOOL) = NULL;
	if	(lIsWow64Process = (BOOL (WINAPI *) (HANDLE, PBOOL)) ::GetProcAddress (GetModuleHandle(_T("Kernel32")), "IsWow64Process"))
	{
		try
		{
			(*lIsWow64Process) (GetCurrentProcess(), &lIsWow64);
		}
		catch (...)
		{}
	}
#endif
	return (lIsWow64 == TRUE);
}

////////////////////////////////////////////////////////////////////////
#ifdef	_MFC_VER

#if	(_MFC_VER >= 0x0800)
#define	_MFC_NCHITTEST_RESULT	LRESULT
#define	_MFC_ACTIVATEAPP_PARAM2	DWORD
#else
#define	_MFC_NCHITTEST_RESULT	UINT
#define	_MFC_ACTIVATEAPP_PARAM2	HTASK
#endif

#if	(_MFC_VER >= 0x0800)
#if (_WIN32_WINNT >= 0x0600)
#define	PROPSHEETPAGE_MFC_VER	PROPSHEETPAGE_V4
#else
#define	PROPSHEETPAGE_MFC_VER	PROPSHEETPAGE_V3
#endif
#else
#define	PROPSHEETPAGE_MFC_VER	PROPSHEETPAGE_V2
#endif

#if	(_MFC_VER >= 0x0800)
#define	PROPSHEETHEADER_MFC_VER	PROPSHEETHEADER
#else
#ifdef	_UNICODE
#define	PROPSHEETHEADER_MFC_VER	PROPSHEETHEADERW_V1
#else
#define	PROPSHEETHEADER_MFC_VER	PROPSHEETHEADERA_V1
#endif
#endif

#endif	// _MFC_VER
////////////////////////////////////////////////////////////////////////
#endif	// _OSVER_H
