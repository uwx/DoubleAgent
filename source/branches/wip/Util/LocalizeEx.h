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
#if !defined(AFX_LOCALIZEEX_H__C67392BF_D926_4DB8_BA5C_A24CAB2CA1DA__INCLUDED_)
#define AFX_LOCALIZEEX_H__10AD0B9D_CA8C_4FFB_9471_F05F73598B6F__INCLUDED_
#pragma once

#include <atlcomtime.h>
#include "Localize.h"

//////////////////////////////////////////////////////////////////////

#define	DATE_LONGDATE_NOWEEKDAY		(DATE_LONGDATE | 0x80000000)
#define	DATE_LONGDATE_FORCEWEEKDAY	(DATE_LONGDATE | 0x40000000)
#define	TIME_FORCESECONDS			(0x10000000)
#define	TIME_FORCETIMEMARKER		(0x20000000)
#define	DURATION_NOHOURS			(TIME_FORCE24HOURFORMAT)
#define	DURATION_NOHOURSORMINUTES	(TIME_FORCE24HOURFORMAT | 0x80000000)
#define	DURATION_SHORTNAMES			(TIME_NOTIMEMARKER)
#define	DURATION_LONGNAMES			(TIME_NOTIMEMARKER | 0x40000000)

#ifndef	LOCALE_CURRENT
#ifdef	_UNICODE
#define	LOCALE_CURRENT	(GetThreadLocale ())
#else
#define	LOCALE_CURRENT	LOCALE_USER_DEFAULT
#endif
#endif

//////////////////////////////////////////////////////////////////////

enum LclSizeFormat
{
	LclSizeFormatDefault = 0,
	LclSizeFormatBytes = 0x1100,
	LclSizeFormatKBytes = 0x1200,
	LclSizeFormatMBytes = 0x1402,
	LclSizeFormatGBytes = 0x1802,
	LclSizeFormatFlexible = 0x1FFD,
	LclSizeFormatSeparators = 0x1000,
	LclSizeFormatTypeMask = 0x0F00,
	LclSizeFormatRoundedMask = 0x0E00,
	LclSizeFormatDecimalsMask = 0x00FF
};

//////////////////////////////////////////////////////////////////////

class CLocalizeEx
{
public:
	friend CString LclFormatDate (const SYSTEMTIME & pDate, DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatDate (const FILETIME & pDate, DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, bool pDateUTC = true, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatDate (const VARIANT & pDate, DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatDate (const COleDateTime & pDate, DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatDate (const CTime & pDate, DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclDateFormatString (DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, WORD pLangId = LANG_USER_DEFAULT);

	friend CString LclFormatTime (const SYSTEMTIME & pTime, DWORD pFlags = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatTime (const FILETIME & pTime, DWORD pFlags = 0, bool pTimeUTC = true, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatTime (const VARIANT & pTime, DWORD pFlags = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatTime (const COleDateTime & pTime, DWORD pFlags = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatTime (const CTime & pTime, DWORD pFlags = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclTimeFormatString (DWORD pFlags = 0, WORD pLangId = 0);

	friend CString LclFormatDuration (long pDuration, DWORD pFlags = DURATION_LONGNAMES, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = 1, int pMinDecimals = 0);
	friend CString LclFormatDuration (LONGLONG pDuration, DWORD pFlags = DURATION_LONGNAMES, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = 1, int pMinDecimals = 0);
	friend CString LclFormatDuration (double pDuration, DWORD pFlags = DURATION_LONGNAMES, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = 1, int pMinDecimals = 0);
	friend CString LclFormatDuration (const FILETIME & pDuration, DWORD pFlags = DURATION_LONGNAMES, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = 1, int pMinDecimals = 0);

#ifdef	_FILETIME_H
	friend CString LclFormatDate (const CFiletime & pDate, DWORD pFlags = DATE_LONGDATE_NOWEEKDAY, bool pDateUTC = false, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatTime (const CFiletime & pTime, DWORD pFlags = 0, bool pTimeUTC = false, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatDuration (const CFiletimeSpan & pDuration, DWORD pFlags = DURATION_LONGNAMES, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = 1, int pMinDecimals = 0);
#endif

	friend CString LclFormatCurrency (VARIANT & pCurrency, bool pZeroNull = false, bool pNoDecimals = false, WORD pLangId = LANG_USER_DEFAULT);
#ifdef	__AFX_H__
	friend CString LclFormatCurrency (COleCurrency & pCurrency, bool pZeroNull = false, bool pNoDecimals = false, WORD pLangId = LANG_USER_DEFAULT);
#endif

	friend CString LclFormatNumber (long pNumber, bool pZeroNull = false, bool pNoThousands = false, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatNumber (unsigned long pNumber, bool pZeroNull = false, bool pNoThousands = false, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatNumber (double pNumber, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = -1, int pMinDecimals = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatNumber (LONGLONG pNumber, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = -1, int pMinDecimals = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatNumber (ULONGLONG pNumber, bool pZeroNull = false, bool pNoThousands = false, int pMaxDecimals = -1, int pMinDecimals = 0, WORD pLangId = LANG_USER_DEFAULT);

	friend double LclParseNumber (LPCTSTR pNumber, CString * pRemainder = NULL);
	friend bool LclParseNumber (LPCTSTR pNumber, double & pValue);
	friend bool LclParseNumber (LPCTSTR pNumber, long & pValue, int pBase=10);
	friend bool LclParseNumber (LPCTSTR pNumber, ULONG & pValue, int pBase=10);

	friend CString LclFormatPercent (double pNumber, int pRoundDigits = 0, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatPercent (const CString & pNumber, WORD pLangId = LANG_USER_DEFAULT);

	friend CString LclFormatSize (double pSize, UINT pFormat = LclSizeFormatDefault, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatSize (long pSize, UINT pFormat = LclSizeFormatDefault, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatSize (unsigned long pSize, UINT pFormat = LclSizeFormatDefault, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatSize (LONGLONG pSize, UINT pFormat = LclSizeFormatDefault, WORD pLangId = LANG_USER_DEFAULT);
	friend CString LclFormatSize (ULONGLONG pSize, UINT pFormat = LclSizeFormatDefault, WORD pLangId = LANG_USER_DEFAULT);

	friend bool LclParseSize (LPCTSTR pSizeStr, double & pSize, UINT * pFormat = NULL);
	friend double LclParseSize (LPCTSTR pSizeStr);

public:
	static CString	mSizeSuffixBytes;
	static CString	mSizeSuffixKBytes;
	static CString	mSizeSuffixMBytes;
	static CString	mSizeSuffixGBytes;
};

//////////////////////////////////////////////////////////////////////

#ifdef	_FILETIME_H
inline CString LclFormatDate (const CFiletime & pDate, DWORD pFlags, bool pDateUTC, WORD pLangId) {return LclFormatDate ((const FILETIME &) pDate, pFlags, pDateUTC, pLangId);}
inline CString LclFormatTime (const CFiletime & pTime, DWORD pFlags, bool pTimeUTC, WORD pLangId) {return LclFormatTime ((const FILETIME &) pTime, pFlags, pTimeUTC, pLangId);}
inline CString LclFormatDuration (const CFiletimeSpan & pDuration, DWORD pFlags, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals) {return LclFormatDuration ((LONGLONG) pDuration, pFlags, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);}
#endif

inline CString LclFormatNumber (LONGLONG pNumber, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals, WORD pLangId) {return LclFormatNumber ((double) pNumber, pZeroNull, pNoThousands, 0, 0, pLangId);}
inline CString LclFormatNumber (ULONGLONG pNumber, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals, WORD pLangId) {return LclFormatNumber ((double) (LONGLONG) pNumber, pZeroNull, pNoThousands, 0, 0, pLangId);}

inline CString LclFormatSize (long pSize, UINT pFormat, WORD pLangId) {return LclFormatSize ((double) pSize, pFormat, pLangId);}
inline CString LclFormatSize (unsigned long pSize, UINT pFormat, WORD pLangId) {return LclFormatSize ((double)(long) pSize, pFormat, pLangId);}
inline CString LclFormatSize (LONGLONG pSize, UINT pFormat, WORD pLangId) {return LclFormatSize ((double) pSize, pFormat, pLangId);}
inline CString LclFormatSize (ULONGLONG pSize, UINT pFormat, WORD pLangId) {return LclFormatSize ((double)(LONGLONG) pSize, pFormat, pLangId);}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#define	DATETIME_DEFAULT	MAKELONG(DATE_SHORTDATE,TIME_FORCE24HOURFORMAT|TIME_NOTIMEMARKER)
#define	DATETIME_NOSECONDS	MAKELONG(DATE_SHORTDATE,TIME_FORCE24HOURFORMAT|TIME_NOTIMEMARKER|TIME_NOSECONDS)

static inline CString DebugDateTime (const SYSTEMTIME & pDateTime, DWORD pFlags = DATETIME_DEFAULT) {return LclFormatDate(pDateTime,LOWORD(pFlags)) + _T(" ") + LclFormatTime(pDateTime,HIWORD(pFlags));}
static inline CString DebugDateTime (const FILETIME & pDateTime, DWORD pFlags = DATETIME_DEFAULT, bool pUTC = true) {return LclFormatDate(pDateTime,LOWORD(pFlags),pUTC) + _T(" ") + LclFormatTime(pDateTime,HIWORD(pFlags),pUTC);}
static inline CString DebugDateTime (const VARIANT & pDateTime, DWORD pFlags = DATETIME_DEFAULT) {return LclFormatDate(pDateTime,LOWORD(pFlags)) + _T(" ") + LclFormatTime(pDateTime,HIWORD(pFlags));}
static inline CString DebugDateTime (const COleDateTime & pDateTime, DWORD pFlags = DATETIME_DEFAULT) {return LclFormatDate(pDateTime,LOWORD(pFlags)) + _T(" ") + LclFormatTime(pDateTime,HIWORD(pFlags));}
static inline CString DebugDateTime (const CTime & pDateTime, DWORD pFlags = DATETIME_DEFAULT) {return LclFormatDate(pDateTime,LOWORD(pFlags)) + _T(" ") + LclFormatTime(pDateTime,HIWORD(pFlags));}
#ifdef	_FILETIME_H
static inline CString DebugDateTime (const CFiletime & pDateTime, DWORD pFlags = DATETIME_DEFAULT, bool pUTC = false) {return LclFormatDate(pDateTime,LOWORD(pFlags),pUTC) + _T(" ") + LclFormatTime(pDateTime,HIWORD(pFlags),pUTC);}
#endif

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_LOCALIZEEX_H__10AD0B9D_CA8C_4FFB_9471_F05F73598B6F__INCLUDED_)
