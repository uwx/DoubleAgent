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
#include <math.h>
#include "LocalizeEx.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	_UNICODE
static inline LCID __Locale (WORD pLangId)
{
	return (pLangId ? (pLangId == LANG_USER_DEFAULT) ? LOCALE_CURRENT : MAKELCID (pLangId, SORT_DEFAULT) : LOCALE_CURRENT);
}
#else
#define __Locale(l) LOCALE_CURRENT
#endif

//////////////////////////////////////////////////////////////////////

CString LclDateFormatString (DWORD pFlags, WORD pLangId)
{
	CString	lRet;

	if	(pFlags == DATE_SHORTDATE)
	{
		GetLocaleInfo (__Locale(pLangId), LOCALE_SSHORTDATE, lRet.GetBuffer (MAX_PATH), MAX_PATH);
		lRet.ReleaseBuffer ();
	}
	else
	if	(pFlags == DATE_LONGDATE)
	{
		GetLocaleInfo (__Locale(pLangId), LOCALE_SLONGDATE, lRet.GetBuffer (MAX_PATH), MAX_PATH);
		lRet.ReleaseBuffer ();
	}
	else
	if	(
			(pFlags == DATE_LONGDATE_NOWEEKDAY)
		||	(pFlags == DATE_LONGDATE_FORCEWEEKDAY)
		)
	{
		GetLocaleInfo (__Locale(pLangId), LOCALE_SLONGDATE, lRet.GetBuffer (MAX_PATH), MAX_PATH);
		lRet.ReleaseBuffer ();

		if	(pFlags == DATE_LONGDATE_NOWEEKDAY)
		{
			CString	lWeekdayFormat;
			int		lWeekdayPos;

			do
			{
				lWeekdayFormat = _T("dddd");
				lWeekdayPos = lRet.Find (lWeekdayFormat);
				if	(lWeekdayPos < 0)
				{
					lWeekdayFormat = _T("ddd");
					lWeekdayPos = lRet.Find (lWeekdayFormat);
				}
				if	(lWeekdayPos >= 0)
				{
					lRet.Delete (lWeekdayPos, lWeekdayFormat.GetLength ());

					while	(
								(lWeekdayPos < lRet.GetLength ())
							&&	(
									(_istspace (lRet [lWeekdayPos]))
								||	(_istpunct (lRet [lWeekdayPos]))
								)
							)
					{
						lRet.Delete (lWeekdayPos);
					}
					while	(
								(lWeekdayPos > 0)
							&&	(
									(_istspace (lRet [lWeekdayPos-1]))
								||	(_istpunct (lRet [lWeekdayPos-1]))
								)
							)
					{
						lRet.Delete (--lWeekdayPos);
					}
				}
			} while (lWeekdayPos >= 0);
		}
		else
		{
			if	(lRet.Find (_T("ddd")) < 0)
			{
				lRet.Insert (0, _T("dddd, "));
			}
		}
		lRet.TrimLeft ();
		lRet.TrimRight ();
	}

	return CString (lRet);
}

//////////////////////////////////////////////////////////////////////

CString LclFormatDate (const SYSTEMTIME & pDate, DWORD pFlags, WORD pLangId)
{
	CString	lRet;

	if	(
			(pFlags == DATE_LONGDATE_NOWEEKDAY)
		||	(pFlags == DATE_LONGDATE_FORCEWEEKDAY)
		)
	{
		CString	lDateFormat = LclDateFormatString (pFlags);

		if	(lDateFormat.IsEmpty ())
		{
			GetDateFormat (__Locale(pLangId), DATE_LONGDATE, &pDate, NULL, lRet.GetBuffer (MAX_PATH), MAX_PATH);
		}
		else
		{
			GetDateFormat (__Locale(pLangId), 0, &pDate, lDateFormat, lRet.GetBuffer (MAX_PATH), MAX_PATH);
		}
	}
	else
	{
		GetDateFormat (__Locale(pLangId), pFlags, &pDate, NULL, lRet.GetBuffer (MAX_PATH), MAX_PATH);
	}

	lRet.ReleaseBuffer ();
	lRet.TrimLeft ();
	lRet.TrimRight ();
	lRet.FreeExtra ();
	return CString (lRet);
}

CString LclFormatDate (const FILETIME & pDate, DWORD pFlags, bool pDateUTC, WORD pLangId)
{
	tS <SYSTEMTIME>	lSysTime;

	if	(pDateUTC)
	{
		tS <SYSTEMTIME>	lFileSysTime;

		FileTimeToSystemTime (&pDate, &lFileSysTime);
		SystemTimeToTzSpecificLocalTime (NULL, &lFileSysTime, &lSysTime);
	}
	else
	{
		FileTimeToSystemTime (&pDate, &lSysTime);
	}
	return LclFormatDate (lSysTime, pFlags);
}

CString LclFormatDate (const VARIANT & pDate, DWORD pFlags, WORD pLangId)
{
	SYSTEMTIME	lDate;

	if	(
			(V_VT (&pDate) == VT_DATE)
		&&	(VariantTimeToSystemTime (V_DATE (&pDate), &lDate))
		)
	{
		return LclFormatDate (lDate, pFlags);
	}
	else
	{
		COleVariant lDateVal (pDate);
		VariantChangeType (&lDateVal, &lDateVal, 0, VT_BSTR);
		return CString (V_BSTR (&lDateVal));
	}
}

CString LclFormatDate (const COleDateTime & pDate, DWORD pFlags, WORD pLangId)
{
	SYSTEMTIME lDate;
	pDate.GetAsSystemTime (lDate);
	return LclFormatDate (lDate, pFlags);
}

CString LclFormatDate (const CTime & pDate, DWORD pFlags, WORD pLangId)
{
	SYSTEMTIME lDate;
	pDate.GetAsSystemTime (lDate);
	return LclFormatDate (lDate, pFlags);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString LclTimeFormatString (DWORD pFlags, WORD pLangId)
{
	CString	lRet;
	CString	lTimeSep;

	GetLocaleInfo (__Locale(pLangId), LOCALE_STIMEFORMAT, lRet.GetBuffer (MAX_PATH), MAX_PATH);
	lRet.ReleaseBuffer ();
	GetLocaleInfo (__Locale(pLangId), LOCALE_STIME, lTimeSep.GetBuffer (10), 10);
	lTimeSep.ReleaseBuffer ();

	if	(pFlags & TIME_NOMINUTESORSECONDS)
	{
		lRet.Replace (_T("mm"), _T("m"));
		lRet.Replace (_T(" m "), _T("m"));
		lRet.Replace (lTimeSep + _T("m"), _T(""));
		lRet.Replace (_T("m") + lTimeSep, _T(""));
		lRet.Replace (_T("m"), _T(""));
	}

	if	(pFlags & (TIME_NOMINUTESORSECONDS | TIME_NOSECONDS))
	{
		lRet.Replace (_T("ss"), _T("s"));
		lRet.Replace (_T(" s "), _T("s"));
		lRet.Replace (lTimeSep + _T("s"), _T(""));
		lRet.Replace (_T("s") + lTimeSep, _T(""));
		lRet.Replace (_T("s"), _T(""));
	}
	else
	if	(
			(pFlags & TIME_FORCESECONDS)
		&&	(lRet.Find (_T('s')) < 0)
		)
	{
		int	lCharPos;

		if	((lCharPos = lRet.Find (_T(" ") + lTimeSep+_T(" mm"))) >= 0)
		{
			lRet.Insert (lCharPos+lTimeSep.GetLength()+4, _T(" ") + lTimeSep + _T(" ss"));
		}
		else
		if	((lCharPos = lRet.Find (_T(" ") + lTimeSep+_T("mm"))) >= 0)
		{
			lRet.Insert (lCharPos+lTimeSep.GetLength()+3, _T(" ") + lTimeSep + _T("ss"));
		}
		else
		if	((lCharPos = lRet.Find (lTimeSep+_T(" mm"))) >= 0)
		{
			lRet.Insert (lCharPos+lTimeSep.GetLength()+3, lTimeSep + _T(" ss"));
		}
		else
		if	((lCharPos = lRet.Find (lTimeSep+_T("mm"))) >= 0)
		{
			lRet.Insert (lCharPos+lTimeSep.GetLength()+2, lTimeSep + _T("ss"));
		}
		else
		if	((lCharPos = lRet.ReverseFind (_T('m'))) >= 0)
		{
			lRet.Insert (lCharPos+1, _T("ss"));
		}
		else
		{
			lRet += _T("ss");
		}
	}

	if	(pFlags & (TIME_NOTIMEMARKER | TIME_FORCE24HOURFORMAT))
	{
		lRet.Replace (_T("tt"), _T("t"));
		lRet.Replace (_T(" t "), _T("t"));
		lRet.Replace (lTimeSep + _T("t"), _T(""));
		lRet.Replace (_T("t") + lTimeSep, _T(""));
		lRet.Replace (_T("t"), _T(""));
	}

	if	(pFlags & TIME_FORCE24HOURFORMAT)
	{
		lRet.Replace (_T("h"), _T("hh"));
		lRet.Replace (_T("hhhh"), _T("hh"));
		lRet.Replace (_T('h'), _T('H'));
	}
	else
	if	(
			(pFlags & TIME_FORCETIMEMARKER)
		&&	(lRet.Find (_T('t')) < 0)
		)
	{
		lRet += _T(" tt");
	}

	lRet.TrimLeft ();
	lRet.TrimRight ();

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString LclFormatTime (const SYSTEMTIME & pTime, DWORD pFlags, WORD pLangId)
{
	CString	lRet;

	if	(pFlags & (TIME_FORCESECONDS | TIME_FORCETIMEMARKER))
	{
		GetTimeFormat (__Locale(pLangId), 0, &pTime, LclTimeFormatString (pFlags, pLangId), lRet.GetBuffer (100), 100);
	}
	else
	{
		GetTimeFormat (__Locale(pLangId), pFlags, &pTime, NULL, lRet.GetBuffer (100), 100);
	}

	lRet.ReleaseBuffer ();
	lRet.FreeExtra ();
	return CString (lRet);
}

CString LclFormatTime (const FILETIME & pTime, DWORD pFlags, bool pTimeUTC, WORD pLangId)
{
	tS <SYSTEMTIME>	lSysTime;

	if	(pTimeUTC)
	{
		tS <SYSTEMTIME>	lFileSysTime;

		FileTimeToSystemTime (&pTime, &lFileSysTime);
		SystemTimeToTzSpecificLocalTime (NULL, &lFileSysTime, &lSysTime);
	}
	else
	{
		FileTimeToSystemTime (&pTime, &lSysTime);
	}
	return LclFormatTime (lSysTime, pFlags);
}

CString LclFormatTime (const VARIANT & pTime, DWORD pFlags, WORD pLangId)
{
	SYSTEMTIME	lTime;

	if	(
			(V_VT (&pTime) == VT_DATE)
		&&	(VariantTimeToSystemTime (V_DATE (&pTime), &lTime))
		)
	{
		return LclFormatTime (lTime, pFlags);
	}
	else
	{
		COleVariant lTimeVal (pTime);
		VariantChangeType (&lTimeVal, &lTimeVal, 0, VT_BSTR);
		return CString (V_BSTR (&lTimeVal));
	}
}

CString LclFormatTime (const COleDateTime & pTime, DWORD pFlags, WORD pLangId)
{
	SYSTEMTIME lTime;
	pTime.GetAsSystemTime (lTime);
	return LclFormatTime (lTime, pFlags);
}

CString LclFormatTime (const CTime & pTime, DWORD pFlags, WORD pLangId)
{
	SYSTEMTIME lTime;
	pTime.GetAsSystemTime (lTime);
	return LclFormatTime (lTime, pFlags);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString LclFormatDuration (long pDuration, DWORD pFlags, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals)
{
	return LclFormatDuration ((double) pDuration, pFlags, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);
}

CString LclFormatDuration (LONGLONG pDuration, DWORD pFlags, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals)
{
	return LclFormatDuration ((double) pDuration / 10000.0, pFlags, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);
}

CString LclFormatDuration (const FILETIME & pDuration, DWORD pFlags, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals)
{
	LARGE_INTEGER	lDuration = {pDuration.dwLowDateTime, pDuration.dwHighDateTime};

	return LclFormatDuration ((double) lDuration.QuadPart / 10000.0, pFlags, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);
}

CString LclFormatDuration (double pDuration, DWORD pFlags, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals)
{
	CString			lRet;
	double			lDuration = fabs (pDuration);
	CString			lTimeSep;
	CString			lTimeLeadZero;

	if	(pFlags & TIME_NOTIMEMARKER)
	{
		lTimeSep = _T(" ");
	}
	else
	{
		int	lDataSize;

		if	(lDataSize = GetLocaleInfo (LOCALE_CURRENT, LOCALE_STIME, NULL, 0))
		{
			GetLocaleInfo (LOCALE_CURRENT, LOCALE_STIME, lTimeSep.GetBuffer (lDataSize), lDataSize);
			lTimeSep.ReleaseBuffer ();
		}
		if	(lDataSize = GetLocaleInfo (LOCALE_CURRENT, LOCALE_ITLZERO, NULL, 0))
		{
			GetLocaleInfo (LOCALE_CURRENT, LOCALE_ITLZERO, lTimeLeadZero.GetBuffer (lDataSize), lDataSize);
			lTimeLeadZero.ReleaseBuffer ();
		}
	}

	if	(
			((pFlags & DURATION_NOHOURS) != DURATION_NOHOURS)
		&&	((pFlags & DURATION_NOHOURSORMINUTES) != DURATION_NOHOURSORMINUTES)
		)
	{
		if	(
				((pFlags & DURATION_NOHOURSORMINUTES) != DURATION_NOHOURSORMINUTES)
			&&	((pFlags & TIME_NOMINUTESORSECONDS) != TIME_NOMINUTESORSECONDS)
			&&	((pFlags & TIME_NOSECONDS) != TIME_NOSECONDS)
			)
		{
			long lHours = (long) (lDuration / (60.0 * 60.0 * 1000.0));
			lDuration = fmod (lDuration, (60.0 * 60.0 * 1000.0));

			if	(lHours > 24)
			{
				lRet += LclFormatNumber (lHours/24L, pZeroNull, pNoThousands);
				lRet += (lHours >= 48) ? _T(" days") : _T(" day");
				lHours %= 24;
			}

			if	(
					(lHours != 0)
				||	(
						(!(pFlags & TIME_NOTIMEMARKER))
					&&	(!pZeroNull)
					)
				)
			{
				if	(!lRet.IsEmpty ())
				{
					lRet += lTimeSep;
				}

				if	(pFlags & TIME_NOTIMEMARKER)
				{
					lRet += LclFormatNumber (lHours, pZeroNull, pNoThousands);

					if	((pFlags & DURATION_LONGNAMES) == DURATION_LONGNAMES)
					{
						lRet += (lHours == 1) ? _T(" hour") : _T(" hours");
					}
					else
					{
						lRet += _T(" hr.");
					}
				}
				else
				if	(lHours <= 10)
				{
					if	(_ttoi (lTimeLeadZero))
					{
						if	(lRet.IsEmpty ())
						{
							lRet.Format (_T("%2.2d"), lHours);
						}
						else
						{
							lRet.Format (_T("%s %2.2d"), CString (lRet), lHours);
						}
					}
					else
					{
						if	(lRet.IsEmpty ())
						{
							lRet.Format (_T("%d"), lHours);
						}
						else
						{
							lRet.Format (_T("%s %d"), CString (lRet), lHours);
						}
					}
				}
				else
				{
					lRet += LclFormatNumber (lHours, pZeroNull, pNoThousands);
				}

				pMaxDecimals = min (pMaxDecimals, pMinDecimals);
				pMinDecimals = max (pMinDecimals, pMaxDecimals);
			}
		}
		else
		{
			lDuration /= (60.0 * 60.0 * 1000.0);
			lRet = LclFormatNumber (lDuration, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);
		}
	}

	if	(
			((pFlags & DURATION_NOHOURSORMINUTES) != DURATION_NOHOURSORMINUTES)
		&&	((pFlags & TIME_NOMINUTESORSECONDS) != TIME_NOMINUTESORSECONDS)
		)
	{
		if	((pFlags & TIME_NOSECONDS) != TIME_NOSECONDS)
		{
			long lMinutes = (long) (lDuration / (60.0 * 1000.0));
			lDuration = fmod (lDuration, (60.0 * 1000.0));

			if	(
					(lMinutes != 0)
				||	(
						(!(pFlags & TIME_NOTIMEMARKER))
					&&	(
							(!pZeroNull)
						||	(!lRet.IsEmpty ())
						)
					)
				)
			{
				if	(!lRet.IsEmpty ())
				{
					lRet += lTimeSep;
				}

				if	(pFlags & TIME_NOTIMEMARKER)
				{
					lRet += LclFormatNumber (lMinutes, pZeroNull, pNoThousands);

					if	((pFlags & DURATION_LONGNAMES) == DURATION_LONGNAMES)
					{
						lRet += (lMinutes == 1) ? _T(" minute") : _T(" minutes");
					}
					else
					{
						lRet += _T(" min.");
					}
				}
				else
				if	(lMinutes <= 10)
				{
					if	(
							(!lRet.IsEmpty ())
						&&	(_ttoi (lTimeLeadZero))
						)
					{
						lRet.Format (_T("%s%-2.2d"), CString (lRet), lMinutes);
					}
					else
					{
						lRet.Format (_T("%s%-d"), CString (lRet), lMinutes);
					}
				}
				else
				{
					lRet += LclFormatNumber (lMinutes, pZeroNull, pNoThousands);
				}

				pMaxDecimals = min (pMaxDecimals, pMinDecimals);
				pMinDecimals = max (pMinDecimals, pMaxDecimals);
			}
		}
		else
		{
			lDuration /= (60.0 * 1000.0);
			lRet = LclFormatNumber (lDuration, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);
		}
	}

	if	(
			((pFlags & TIME_NOMINUTESORSECONDS) != TIME_NOMINUTESORSECONDS)
		&&	((pFlags & TIME_NOSECONDS) != TIME_NOSECONDS)
		)
	{
		if	(
				(lDuration != 0.0)
			||	(pMaxDecimals == 0)
			||	(
					(pFlags & TIME_NOTIMEMARKER)
				?	(
						(!pZeroNull)
					&&	(!lRet.IsEmpty ())
					)
				:	(
						(!pZeroNull)
					||	(!lRet.IsEmpty ())
					)
				)
			)
		{
			if	(lRet.IsEmpty ())
			{
				if	(
						((pFlags & DURATION_NOHOURS) != DURATION_NOHOURS)
					||	((pFlags & DURATION_NOHOURSORMINUTES) != DURATION_NOHOURSORMINUTES)
					||	((pFlags & TIME_NOMINUTESORSECONDS) != TIME_NOMINUTESORSECONDS)
					)
				{
					pMinDecimals = max (pMinDecimals, pMaxDecimals);
				}
			}
			else
			{
				lRet += lTimeSep;
			}

			lDuration /= 1000.0;

			if	(pFlags & TIME_NOTIMEMARKER)
			{
				CString	lSeconds = LclFormatNumber (lDuration, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);

				lRet += lSeconds;

				if	((pFlags & DURATION_LONGNAMES) == DURATION_LONGNAMES)
				{
					lRet += (lSeconds == _T("1")) ? _T(" second") : _T(" seconds");
				}
				else
				{
					lRet += _T(" sec.");
				}
			}
			else
			{
				int	lCharPos = lRet.GetLength ();

				lRet += LclFormatNumber (lDuration, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);

				if	(
						(lCharPos > 0)
					&&	(_ttoi (lTimeLeadZero))
					&&	(
							(lDuration != 0)
						||	(!pZeroNull)
						)
					)
				{
					while	(
								(lCharPos < lRet.GetLength ())
							&&	(!_istdigit (lRet [lCharPos]))
							)
					{
						lCharPos++;
					}
					if	(
							(lCharPos < lRet.GetLength ()-1)
						&&	(_istdigit (lRet [lCharPos]))
						&&	(!_istdigit (lRet [lCharPos+1]))
						)
					{
						lRet.Insert (lCharPos, _T('0'));
					}
				}
			}
		}
	}

	if	(lRet.IsEmpty ())
	{
		CString	lMs = LclFormatNumber (lDuration, pZeroNull, pNoThousands, pMaxDecimals, pMinDecimals);

		if	(!lMs.IsEmpty ())
		{
			lRet = lMs;

			if	((pFlags & DURATION_LONGNAMES) == DURATION_LONGNAMES)
			{
				lRet += (lMs == _T("1")) ? _T(" millisecond") : _T(" milliseconds");
			}
			else
			if	((pFlags & DURATION_SHORTNAMES) == DURATION_SHORTNAMES)
			{
				lRet += _T(" ms.");
			}
		}
	}

	if	(
			(!lRet.IsEmpty ())
		&&	(pDuration < 0)
		)
	{
		CString	lNegMode;
		CString	lNegSign;
		int		lDataSize;

		if	(lDataSize = GetLocaleInfo (LOCALE_CURRENT, LOCALE_INEGNUMBER, NULL, 0))
		{
			GetLocaleInfo (LOCALE_CURRENT, LOCALE_INEGNUMBER, lNegMode.GetBuffer (lDataSize), lDataSize);
			lNegMode.ReleaseBuffer ();
		}
		if	(lDataSize = GetLocaleInfo (LOCALE_CURRENT, LOCALE_SNEGATIVESIGN, NULL, 0))
		{
			GetLocaleInfo (LOCALE_CURRENT, LOCALE_SNEGATIVESIGN, lNegSign.GetBuffer (lDataSize), lDataSize);
			lNegSign.ReleaseBuffer ();
		}

		switch (_ttoi (lNegMode))
		{
			case 0:
			{
				lRet.Format (_T("(%s)"), CString (lRet));
			}	break;
			case 2:
			{
				lRet.Format (_T("%s %s"), lNegSign, CString (lRet));
			}	break;
			case 3:
			{
				lRet.Format (_T("%s%s"), CString (lRet), lNegSign);
			}	break;
			case 4:
			{
				lRet.Format (_T("%s %s"), CString (lRet), lNegSign);
			}	break;
			default:
			{
				lRet.Format (_T("%s%s"), lNegSign, CString (lRet));
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString LclFormatCurrency (VARIANT & pCurrency, bool pZeroNull, bool pNoDecimals, WORD pLangId)
{
	CString	lRet;

	if	(
			(V_VT (&pCurrency) == VT_CY)
		&&	(
				(!pZeroNull)
			||	(V_CY (&pCurrency).int64 != 0)
			)
		)
	{
		COleVariant	lCurrencyVal (pCurrency);
		CString		lFormatted;
		int			lFormattedSize;

		VariantChangeType (&lCurrencyVal, &lCurrencyVal, 0, VT_BSTR);
		lRet = V_BSTR (&lCurrencyVal);

		if	(
				(lFormattedSize = GetCurrencyFormat (__Locale(pLangId), 0, lRet, NULL, NULL, 0))
			&&	(GetCurrencyFormat (__Locale(pLangId), 0, lRet, NULL, lFormatted.GetBufferSetLength (lFormattedSize), lFormattedSize))
			)
		{
			lFormatted.ReleaseBuffer ();
			lFormatted.FreeExtra ();
			lRet = lFormatted;
		}

		if	(pNoDecimals)
		{
			TCHAR lDecimal [4];
			memset (lDecimal, 0, sizeof (lDecimal));
			if	(GetLocaleInfo (__Locale(pLangId), LOCALE_SDECIMAL, lDecimal, sizeof (lDecimal)))
			{
				lRet = lRet.SpanExcluding (lDecimal);
			}
		}
	}

	return CString (lRet);
}

CString LclFormatCurrency (COleCurrency & pCurrency, bool pZeroNull, bool pNoDecimals, WORD pLangId)
{
	CString	lRet;

	if	(
			(pCurrency.GetStatus () != COleCurrency::null)
		&&	(
				(!pZeroNull)
			||	(pCurrency.m_cur.int64 != 0)
			)
		)
	{
		CString	lFormatted;
		int		lFormattedSize;

		lRet = pCurrency.Format ();

		if	(
				(lFormattedSize = GetCurrencyFormat (__Locale(pLangId), 0, lRet, NULL, NULL, 0))
			&&	(GetCurrencyFormat (__Locale(pLangId), 0, lRet, NULL, lFormatted.GetBufferSetLength (lFormattedSize), lFormattedSize))
			)
		{
			lFormatted.ReleaseBuffer ();
			lFormatted.FreeExtra ();
			lRet = lFormatted;
		}

		if	(pNoDecimals)
		{
			TCHAR lDecimal [4];
			memset (lDecimal, 0, sizeof (lDecimal));
			if	(GetLocaleInfo (__Locale(pLangId), LOCALE_SDECIMAL, lDecimal, sizeof (lDecimal)))
			{
				lRet = lRet.SpanExcluding (lDecimal);
			}
		}
	}

	return CString (lRet);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString LclFormatNumber (long pNumber, bool pZeroNull, bool pNoThousands, WORD pLangId)
{
	CString	lRet;

	if	(
			(!pZeroNull)
		||	(pNumber)
		)
	{
		lRet.Format (_T("%ld"), pNumber);

		if	(!pNoThousands)
		{
			TCHAR			lNumber [50];
			tS <NUMBERFMT>	lFormat;
			TCHAR			lDecSep [3];
			TCHAR			lThouSep [3];
			TCHAR			lNegMode [3];

			GetLocaleInfo (__Locale(pLangId), LOCALE_SDECIMAL, lDecSep, sizeof (lDecSep) / sizeof (TCHAR));
			GetLocaleInfo (__Locale(pLangId), LOCALE_STHOUSAND, lThouSep, sizeof (lThouSep) / sizeof (TCHAR));
			GetLocaleInfo (__Locale(pLangId), LOCALE_INEGNUMBER, lNegMode, sizeof (lNegMode) / sizeof (TCHAR));

			lFormat.Grouping = 3;
			lFormat.lpDecimalSep = lDecSep;
			lFormat.lpThousandSep = lThouSep;
			lFormat.NegativeOrder = _ttoi (lNegMode);

			if	(GetNumberFormat (__Locale(pLangId), 0, lRet, &lFormat, lNumber, sizeof (lNumber) / sizeof (TCHAR)))
			{
				lRet = lNumber;
			}
		}
	}

	return CString (lRet);
}

CString LclFormatNumber (unsigned long pNumber, bool pZeroNull, bool pNoThousands, WORD pLangId)
{
	CString	lRet;

	if	(
			(!pZeroNull)
		||	(pNumber)
		)
	{
		lRet.Format (_T("%lu"), pNumber);

		if	(!pNoThousands)
		{
			TCHAR			lNumber [50];
			tS <NUMBERFMT>	lFormat;
			TCHAR			lDecSep [3];
			TCHAR			lThouSep [3];
			TCHAR			lNegMode [3];

			GetLocaleInfo (__Locale(pLangId), LOCALE_SDECIMAL, lDecSep, sizeof (lDecSep) / sizeof (TCHAR));
			GetLocaleInfo (__Locale(pLangId), LOCALE_STHOUSAND, lThouSep, sizeof (lThouSep) / sizeof (TCHAR));
			GetLocaleInfo (__Locale(pLangId), LOCALE_INEGNUMBER, lNegMode, sizeof (lNegMode) / sizeof (TCHAR));

			lFormat.Grouping = 3;
			lFormat.lpDecimalSep = lDecSep;
			lFormat.lpThousandSep = lThouSep;
			lFormat.NegativeOrder = _ttoi (lNegMode);

			if	(GetNumberFormat (__Locale(pLangId), 0, lRet, &lFormat, lNumber, sizeof (lNumber) / sizeof (TCHAR)))
			{
				lRet = lNumber;
			}
		}
	}

	return CString (lRet);
}

CString LclFormatNumber (double pNumber, bool pZeroNull, bool pNoThousands, int pMaxDecimals, int pMinDecimals, WORD pLangId)
{
	CString	lRet;

	if	(
			(!pZeroNull)
		||	(pNumber != 0.0)
		)
	{
		TCHAR			lNumber [100];
		tS <NUMBERFMT>	lFormat;
		TCHAR			lDigits [4];
		TCHAR			lDecSep [3];
		TCHAR			lThouSep [3];
		TCHAR			lNegMode [3];
		TCHAR			lLeadZero [3];

		GetLocaleInfo (__Locale(pLangId), LOCALE_SDECIMAL, lDecSep, sizeof (lDecSep) / sizeof (TCHAR));
		GetLocaleInfo (__Locale(pLangId), LOCALE_STHOUSAND, lThouSep, sizeof (lThouSep) / sizeof (TCHAR));
		GetLocaleInfo (__Locale(pLangId), LOCALE_INEGNUMBER, lNegMode, sizeof (lNegMode) / sizeof (TCHAR));
		GetLocaleInfo (__Locale(pLangId), LOCALE_ILZERO, lLeadZero, sizeof (lLeadZero) / sizeof (TCHAR));

		if	(
				(
					(pMinDecimals < -1)
				||	(pMaxDecimals < -1)
				)
			&&	(GetLocaleInfo (__Locale(pLangId), LOCALE_IDIGITS, lDigits, sizeof (lDigits) / sizeof (TCHAR)))
			)
		{
			if	(pMinDecimals < -1)
			{
				pMinDecimals = _ttoi (lDigits);
			}
			if	(pMaxDecimals < -1)
			{
				pMaxDecimals = _ttoi (lDigits);
			}
		}

		if	(pMaxDecimals < 0)
		{
			lFormat.NumDigits = 6;
		}
		else
		{
			lFormat.NumDigits = pMaxDecimals;
		}

		if	(!pNoThousands)
		{
			lFormat.Grouping = 3;
		}
		lFormat.lpThousandSep = lThouSep;
		lFormat.lpDecimalSep = lDecSep;
		lFormat.LeadingZero = _ttoi (lLeadZero);
		lFormat.NegativeOrder = _ttoi (lNegMode);

		_stprintf (lRet.GetBuffer (200), _T("%f"), pNumber);
		lRet.ReleaseBuffer ();

		if	(GetNumberFormat (__Locale(pLangId), 0, lRet, &lFormat, lNumber, sizeof (lNumber) / sizeof (TCHAR)))
		{
			lRet = lNumber;

			if	(
					(pMaxDecimals < 0)
				||	(pMinDecimals < 0)
				||	(
						(pMinDecimals >= 0)
					&&	(pMinDecimals < pMaxDecimals)
					)
				)
			{
				int lDecPos = lRet.ReverseFind ('.');
				int	lMinDecimals = max (pMinDecimals, 0);

				if	(
						(lDecPos < 0)
					&&	(lMinDecimals > 0)
					)
				{
					TCHAR	lDecSep [3];

					lDecPos = lRet.GetLength ();

					if	(GetLocaleInfo (__Locale(pLangId), LOCALE_SDECIMAL, lDecSep, sizeof (lDecSep) / sizeof (TCHAR)))
					{
						lRet += lDecSep;
					}
					else
					{
						lRet += _T(".");
					}
				}

				if	(lDecPos >= 0)
				{
					while	(
								(lRet.GetLength () > (lDecPos + lMinDecimals))
							&&	(lRet [lRet.GetLength () - 1] == '0')
							)
					{
						lRet.Delete (lRet.GetLength () - 1);
					}

					if	(lMinDecimals > 0)
					{
						while	(lRet.GetLength () < (lDecPos + lMinDecimals + 1))
						{
							lRet += _T("0");
						}
					}
				}

				if	(lRet.GetLength () == lDecPos+1)
				{
					if	(
							(lDecPos == 0)
						&&	(pNumber == 0)
						)
					{
						lRet = _T("0");
					}
					else
					{
						lRet.Delete (lDecPos);
					}
				}
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

inline int PrepNumber (LPCTSTR pNumber, CString & pNumberStr)
{
	int		lRet = 0;
	TCHAR	lDecSep [3];
	TCHAR	lThouSep [3];
	int		lThouPos;
	int		lDecPos;

	pNumberStr = pNumber;
	pNumberStr.TrimLeft ();
	pNumberStr.TrimRight ();

	GetLocaleInfo (LOCALE_CURRENT, LOCALE_SDECIMAL, lDecSep, sizeof (lDecSep) / sizeof (TCHAR));
	GetLocaleInfo (LOCALE_CURRENT, LOCALE_STHOUSAND, lThouSep, sizeof (lThouSep) / sizeof (TCHAR));

	if	(
			(lDecSep [0])
		&&	((lDecPos = pNumberStr.Find (lDecSep)) >= 0)
		)
	{}
	else
	{
		lDecPos = pNumberStr.GetLength ();
	}

	if	(lThouSep [0])
	{
		int	lThouSepLen = (int)_tcslen (lThouSep);

		while ((lThouPos = pNumberStr.Find (lThouSep)) >= 0)
		{
			if	(lThouPos < lDecPos)
			{
				pNumberStr.Delete (lThouPos, lThouSepLen);
				lRet += lThouSepLen;
			}
			else
			{
				break;
			}
		}
	}

	return lRet;
}

double LclParseNumber (LPCTSTR pNumber, CString * pRemainder)
{
	CString	lNumber;
	LPTSTR	lEndPtr = NULL;
	INT_PTR	lRemoved;
	double	lValue;

	lRemoved = PrepNumber (pNumber, lNumber);
	lValue = _tcstod (lNumber, &lEndPtr);

	if	(pRemainder)
	{
		if	(
				(lEndPtr)
			&&	(*lEndPtr != 0)
			)
		{
			(*pRemainder) = pNumber;
			pRemainder->Delete (0, (int)(lEndPtr - (LPCTSTR) lNumber + lRemoved));
		}
		else
		{
			pRemainder->Empty ();
		}
	}

	return lValue;
}

bool LclParseNumber (LPCTSTR pNumber, double & pValue)
{
	CString	lNumber;
	LPTSTR	lEndPtr = NULL;
	double	lValue;

	PrepNumber (pNumber, lNumber);

	lValue = _tcstod (lNumber, &lEndPtr);

	if	(
			(lEndPtr)
		&&	(*lEndPtr == 0)
		)
	{
		pValue = lValue;
		return true;
	}
	return false;
}

bool LclParseNumber (LPCTSTR pNumber, long & pValue, int pBase)
{
	CString	lNumber;
	LPTSTR	lEndPtr = NULL;
	long	lValue;

	PrepNumber (pNumber, lNumber);

	lValue = _tcstol (lNumber, &lEndPtr, pBase);

	if	(
			(lEndPtr)
		&&	(*lEndPtr == 0)
		)
	{
		pValue = lValue;
		return true;
	}
	return false;
}

bool LclParseNumber (LPCTSTR pNumber, ULONG & pValue, int pBase)
{
	CString	lNumber;
	LPTSTR	lEndPtr = NULL;
	ULONG	lValue;

	PrepNumber (pNumber, lNumber);

	lValue = _tcstoul (lNumber, &lEndPtr, pBase);

	if	(
			(lEndPtr)
		&&	(*lEndPtr == 0)
		)
	{
		pValue = lValue;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

static LPCTSTR	sSuffixPercent = _T("%");

CString LclFormatPercent (double pNumber, int pRoundDigits, WORD pLangId)
{
	CString	lRet;

	if	(pNumber == 0.0)
	{
		lRet = LclFormatNumber (pNumber, false, false, 1, 1);
	}
	else
	{
		pNumber *= 100.0;
		pRoundDigits = max (pRoundDigits - (int) floor (log10 (fabs (pNumber))) - 1, 0);

		lRet = LclFormatNumber (pNumber, false, false, pRoundDigits, 1);
	}

	return lRet + sSuffixPercent;
}

CString LclFormatPercent (const CString & pNumber, WORD pLangId)
{
	return (LclFormatPercent (LclParseNumber (pNumber), 0, pLangId));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString	CLocalizeEx::mSizeSuffixBytes = _T(" bytes");
CString	CLocalizeEx::mSizeSuffixKBytes = _T(" KB");
CString	CLocalizeEx::mSizeSuffixMBytes = _T(" MB");
CString	CLocalizeEx::mSizeSuffixGBytes = _T(" GB");

//////////////////////////////////////////////////////////////////////

CString LclFormatSize (double pSize, UINT pFormat, WORD pLangId)
{
	CString	lRet;
    int		lDecimals;
	int		lMinDecimals = 0;
	int		lMaxDecimals = 0;
	int		lMagnitude = 0;
	bool	lGroupDigits;
	double	lSign = 1.0;

	if	(pFormat == LclSizeFormatDefault)
	{
		pFormat = LclSizeFormatFlexible;
	}
	if	(pSize < 0.0)
	{
		lSign = -1.0;
		pSize = -pSize;
	}

	lDecimals = (int) (signed char) (pFormat & LclSizeFormatDecimalsMask);
	lDecimals = min (max (lDecimals, -9), 8);
    lGroupDigits = (pFormat & LclSizeFormatSeparators) ? true : false;

    if	(pFormat & LclSizeFormatRoundedMask)
	{
		double lDispSize = pSize / 1024.0;

		if	(
				(pFormat & (LclSizeFormatBytes & LclSizeFormatTypeMask))
			&&	(lDispSize < 1.0)
			)
		{
			lRet = LclFormatNumber (pSize*lSign, false, !lGroupDigits, lMaxDecimals, lMinDecimals);
			lRet += CLocalizeEx::mSizeSuffixBytes;
		}
		else
        if	(
				((pFormat & LclSizeFormatRoundedMask) == (LclSizeFormatKBytes & LclSizeFormatTypeMask))
			||	(
					(pFormat & (LclSizeFormatKBytes & LclSizeFormatTypeMask))
				&&	(lDispSize < 1024.0)
				)
			)
		{
			lMagnitude = ((lDecimals > 0) || (lDispSize <= 0.0)) ? 0 : max (2 - (int) log10 (lDispSize), 0);
			lMinDecimals = (lDecimals >= -1) ? max (lDecimals, 0) : lMagnitude >= 4 ? 0 : min (lMagnitude, 2);
			lMaxDecimals = (lDecimals >= -1) ? lDecimals : min (lMagnitude, -1-lDecimals);

			lRet = LclFormatNumber (lDispSize*lSign, false, !lGroupDigits, lMaxDecimals, lMinDecimals);
			lRet += CLocalizeEx::mSizeSuffixKBytes;
        }
		else
		{
            lDispSize /= 1024.0;

            if	(
					((pFormat & LclSizeFormatRoundedMask) == (LclSizeFormatMBytes & LclSizeFormatTypeMask))
				||	(
						(pFormat & (LclSizeFormatMBytes & LclSizeFormatTypeMask))
					&&	(lDispSize < 1024.0)
					)
				)
			{
				lMagnitude = ((lDecimals > 0) || (lDispSize <= 0.0)) ? 0 : max (2 - (int) log10 (lDispSize), 0);
				lMinDecimals = (lDecimals >= -1) ? max (lDecimals, 0) : lMagnitude >= 4 ? 0 : min (lMagnitude, 2);
				lMaxDecimals = (lDecimals >= -1) ? lDecimals : min (lMagnitude, -1-lDecimals);

				lRet = LclFormatNumber (lDispSize*lSign, false, !lGroupDigits, lMaxDecimals, lMinDecimals);
				lRet += CLocalizeEx::mSizeSuffixMBytes;
			}
			else
            if	(pFormat & (LclSizeFormatGBytes & LclSizeFormatRoundedMask))
			{
                lDispSize /= 1024.0;

				lMagnitude = ((lDecimals > 0) || (lDispSize <= 0.0)) ? 0 : max (2 - (int) log10 (lDispSize), 0);
				lMinDecimals = (lDecimals >= -1) ? max (lDecimals, 0) : lMagnitude >= 4 ? 0 : min (lMagnitude, 2);
				lMaxDecimals = (lDecimals >= -1) ? lDecimals : min (lMagnitude, -1-lDecimals);

				lRet = LclFormatNumber (lDispSize*lSign, false, !lGroupDigits, lMaxDecimals, lMinDecimals);
				lRet += CLocalizeEx::mSizeSuffixGBytes;
			}
        }
	}
    else
	{
		lRet = LclFormatNumber (pSize*lSign, false, !lGroupDigits, lMaxDecimals, lMinDecimals);
		if	(pFormat & (LclSizeFormatBytes & LclSizeFormatTypeMask))
		{
			lRet += CLocalizeEx::mSizeSuffixBytes;
		}
    }

#ifdef	_DEBUG_NOT
	LogMessage (LogNormal|LogHighVolume, _T("Format [%s] for [%4.4X] as [%s] Dec [%d (%d %d) (%d)]"), FormatNumber (pSize, false, false), pFormat, lRet, lDecimals, lMinDecimals, lMaxDecimals, lMagnitude);
#endif

    return CString (lRet);
}

//////////////////////////////////////////////////////////////////////

bool LclParseSize (LPCTSTR pSizeStr, double & pSize, UINT * pFormat)
{
	bool	lRet = false;
	CString	lRemainder;
	UINT	lFormat = 0;

	pSize = LclParseNumber (pSizeStr, &lRemainder);

	lRemainder.TrimLeft ();
	lRemainder.TrimRight ();

	if	(lRemainder.IsEmpty ())
	{
		lFormat = LclSizeFormatBytes;
		lRet = true;
	}
	else
	{
		lRemainder.Insert (0, _T(" "));

		if	(lRemainder.CompareNoCase (CLocalizeEx::mSizeSuffixBytes) == 0)
		{
			lFormat = LclSizeFormatBytes;
			lRet = true;
		}
		else
		if	(lRemainder.CompareNoCase (CLocalizeEx::mSizeSuffixKBytes) == 0)
		{
			pSize *= 1024.0;
			lFormat = LclSizeFormatKBytes;
			lRet = true;
		}
		else
		if	(lRemainder.CompareNoCase (CLocalizeEx::mSizeSuffixMBytes) == 0)
		{
			pSize *= 1024.0*1024.0;
			lFormat = LclSizeFormatMBytes;
			lRet = true;
		}
		else
		if	(lRemainder.CompareNoCase (CLocalizeEx::mSizeSuffixGBytes) == 0)
		{
			pSize *= 1024.0*1024.0*1024.0;
			lFormat = LclSizeFormatGBytes;
			lRet = true;
		}
	}

	if	(pFormat)
	{
		(*pFormat) = lFormat;
	}
	return lRet;
}

double LclParseSize (LPCTSTR pSizeStr)
{
	double	lRet = 0.0;
	LclParseSize (pSizeStr, lRet);
	return lRet;
}
