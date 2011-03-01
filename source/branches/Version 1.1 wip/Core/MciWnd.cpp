#include "StdAfx.h"
#include <MmSystem.h>
#include <MmReg.h>
#include <MsAcm.h>
#include <Vfw.h>
#include "MciWnd.h"
#include "MciError.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "vfw32.lib")
#ifdef	_DEBUG
#pragma comment(lib, "msacm32.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_NOTIFY	LogNormal|LogHighVolume
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMciWnd, CWnd)

BEGIN_MESSAGE_MAP(CMciWnd, CWnd)
	//{{AFX_MSG_MAP(CMciWnd)
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CMciWnd::CMciWnd()
{
}

CMciWnd::~CMciWnd()
{
	if	(IsWindow (m_hWnd))
	{
		try
		{
			DestroyWindow ();
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CMciWnd::Open (LPCTSTR pFileName)
{
	bool	lRet = false;

	try
	{
		Opening (pFileName);
	}
	catch AnyExceptionSilent

	//ShowCursor (FALSE);

	try
	{
		if	(
				(IsWindow (m_hWnd))
			&&	(LogMciErr (LogIfActive, MCIWndOpen (m_hWnd, pFileName, 0), _T("Open [%s]"), pFileName) == ERROR_SUCCESS)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	//ShowCursor (TRUE);

	if	(lRet)
	{
		try
		{
			Opened ();
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

void CMciWnd::Close ()
{
	try
	{
		Closing ();
	}
	catch AnyExceptionSilent

	if	(
			(IsWindow (m_hWnd))
		&&	(MCIWndGetDeviceID (m_hWnd))
		)
	{
		tS <MCI_GENERIC_PARMS>	lParms;
		lParms.dwCallback = (DWORD_PTR)m_hWnd;
		mciSendCommand (MCIWndGetDeviceID (m_hWnd), MCI_CLOSE, MCI_WAIT, (LPARAM)&lParms);
	}

	try
	{
		Closed ();
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

void CMciWnd::Opening (LPCTSTR pFileName)
{}

void CMciWnd::Opened ()
{
#ifdef	_DEBUG_NOT
	CRect	lSource;
	CRect	lDest;
	CRect	lWinRect;

	GetWindowRect (&lWinRect);
	LogMciErr (LogIfActive, MCIWndGetSource (m_hWnd, &lSource));
	LogMciErr (LogIfActive, MCIWndGetDest (m_hWnd, &lDest));
	LogMessage (LogDebug, _T("MciWnd [%s] [%d %d] Source [%d %d %d %d (%d %d)] Dest [%d %d %d %d (%d %d)]"), MciStyleStr (MCIWndGetStyles (m_hWnd)), lWinRect.Width(), lWinRect.Height(), lSource.left, lSource.top, lSource.right, lSource.bottom, lSource.Width(), lSource.Height(), lDest.left, lDest.top, lDest.right, lDest.bottom, lDest.Width(), lDest.Height());
	LogMciDevice (LogDebug);
	LogMciMode (LogDebug);
#endif
//
//	Autosize doesn't always work for some reason
//
//KLUDGE

	if	((MCIWndGetStyles (m_hWnd) & MCIWNDF_NOAUTOSIZEWINDOW) == 0)
	{
		CRect	lSource;
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		if	(
				(LogMciErr (LogIfActive, MCIWndGetSource (m_hWnd, &lSource)) == ERROR_SUCCESS)
			&&	(
					(lWinRect.Width() != lSource.Width())
				||	(lWinRect.Height () != lSource.Height())
				)
			)
		{
			if	(LogIsActive ())
			{
				LogMessage (LogNormal, _T("AutoSize MciWnd [%d %d] to Source [%d %d]]"), lWinRect.Width(), lWinRect.Height(), lSource.Width(), lSource.Height());
			}
			lWinRect.right = lWinRect.left + lSource.Width();
			lWinRect.bottom = lWinRect.top + lSource.Height();
			MoveWindow (lWinRect);
		}
	}

//END KLUDGE
}

void CMciWnd::Closing ()
{}

void CMciWnd::Closed ()
{}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CMciWnd::Start ()
{
	if	(
			(IsWindow (m_hWnd))
		&&	(LogMciErr (LogNormal, MCIWndPlayFrom (m_hWnd, 0)) == ERROR_SUCCESS)
		)
	{
		return true;
	}
	return false;
}

bool CMciWnd::Stop ()
{
	if	(
			(IsWindow (m_hWnd))
		&&	(MCIWndGetMode (m_hWnd, NULL, 0) != MCI_MODE_NOT_READY)
		&&	(LogMciErr (LogNormal, MCIWndStop (m_hWnd)) == ERROR_SUCCESS)
		)
	{
		return true;
	}
	return false;
}

bool CMciWnd::Pause ()
{
	if	(
			(IsWindow (m_hWnd))
		&&	(LogMciErr (LogNormal, MCIWndPause (m_hWnd)) == ERROR_SUCCESS)
		)
	{
		return true;
	}
	return false;
}

bool CMciWnd::Resume ()
{
	if	(
			(IsWindow (m_hWnd))
		&&	(LogMciErr (LogNormal, MCIWndResume (m_hWnd)) == ERROR_SUCCESS)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CMciWnd::IsPlaying ()
{
	bool	lRet = false;

	if	(IsWindow (m_hWnd))
	{
		long	lMode = MCIWndGetMode (m_hWnd, NULL, 0);

		if	(
				(lMode == MCI_MODE_PLAY)
			||	(lMode == MCI_MODE_SEEK)
			||	(lMode == MCI_MODE_PAUSE)
			)
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CMciWnd::OnDestroy()
{
	if	(MCIWndGetMode (m_hWnd, NULL, 0) != MCI_MODE_NOT_READY)
	{
		MCIWndStop (m_hWnd);
	}
	CWnd::OnDestroy ();
}

void CMciWnd::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	Default ();
	// Skip CWnd response - avoids MFC assertions for custom menus
}

/////////////////////////////////////////////////////////////////////////////

bool CMciWnd::HandleMciNotify (UINT pMsgId, WPARAM pWparam, LPARAM pLparam, LRESULT & pResult)
{
	if	((HWND) pWparam == m_hWnd)
	{
		switch (pMsgId)
		{
			case MCIWNDM_NOTIFYERROR:
			{
				if	(LogIsActive ())
				{
					try
					{
						CString	lErrorStr;
						long	lError = MCIWndGetError (m_hWnd, lErrorStr.GetBuffer (MAX_PATH*2), MAX_PATH*2*sizeof(TCHAR));
						lErrorStr.ReleaseBuffer ();
#ifdef	_DEBUG_NOTIFY
						LogMessage (_DEBUG_NOTIFY, _T("OnMciNotifyError [%u (%8.8X)] ([%u (%8.8X)] [%s])"), pLparam, pLparam, lError, lError, lErrorStr);
#else
						LogMessage (LogIfActive, _T("OnMciNotifyError ([%u (%8.8X)] [%s])"), lError, lError, lErrorStr);
#endif
					}
					catch AnyExceptionSilent
				}
				pResult = 0;
				return true;
			}	break;
			case MCIWNDM_NOTIFYMEDIA:
			{
#ifdef	_DEBUG_NOTIFY
				try
				{
					LogMessage (_DEBUG_NOTIFY, _T("OnMciNotifyMedia [%s]"), pLparam);
				}
				catch AnyExceptionSilent
#endif
				pResult = 0;
				return true;
			}	break;
			case MCIWNDM_NOTIFYMODE:
			{
#ifdef	_DEBUG_NOTIFY
				try
				{
					CString	lModeStr;
					long	lMode = MCIWndGetMode (m_hWnd, lModeStr.GetBuffer (MAX_PATH), MAX_PATH*sizeof(TCHAR));

					lModeStr.ReleaseBuffer ();
					LogMessage (_DEBUG_NOTIFY, _T("OnMciNotifyMode [%d] ([%d] [%s])"), pLparam, lMode, lModeStr);
				}
				catch AnyExceptionSilent
#endif
				pResult = 0;
				return true;
			}	break;
			case MCIWNDM_NOTIFYPOS:
			{
#ifdef	_DEBUG_NOTIFY
				try
				{
					LogMessage (_DEBUG_NOTIFY, _T("OnMciNotifyPos [%d]"), pLparam);
				}
				catch AnyExceptionSilent
#endif
				pResult = 0;
				return true;
			}	break;
			case MCIWNDM_NOTIFYSIZE:
			{
#ifdef	_DEBUG_NOTIFY
				try
				{
					CRect lRect;
					GetWindowRect (&lRect);
					LogMessage (_DEBUG_NOTIFY, _T("OnMciNotifySize [%d %d]"), lRect.Width (), lRect.Height ());
				}
				catch AnyExceptionSilent
#endif
				pResult = 0;
				return true;
			}	break;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CMciWnd::LogMciDevice (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			CString	lDeviceName;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("MCI Device");
			}

			if	(IsWindow (m_hWnd))
			{
				MCIWndGetDevice (m_hWnd, lDeviceName.GetBuffer (MAX_PATH), MAX_PATH*sizeof (TCHAR));
				lDeviceName.ReleaseBuffer ();
				LogMessage (pLogLevel, _T("%s [%u] [%s] Alias [%8.8X (%d)] Volume [%d]"), lTitle, MCIWndGetDeviceID (m_hWnd), lDeviceName, MCIWndGetAlias (m_hWnd), MCIWndGetAlias (m_hWnd), MCIWndGetVolume (m_hWnd));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s <no MCI window>"), lTitle);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}
/////////////////////////////////////////////////////////////////////////////

void CMciWnd::LogMciMode (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			CString	lModeName;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("MCI Mode");
			}

			if	(IsWindow (m_hWnd))
			{
				MCIWndGetMode (m_hWnd, lModeName.GetBuffer (MAX_PATH), MAX_PATH*sizeof (TCHAR));
				lModeName.ReleaseBuffer ();
				LogMessage (pLogLevel, _T("%s [%u] Mode [%s] CanPlay [%d] CanRecord [%d] CanSave [%d]"), lTitle, MCIWndGetDeviceID(m_hWnd), lModeName, MCIWndCanPlay(m_hWnd), MCIWndCanRecord(m_hWnd), MCIWndCanSave(m_hWnd));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s <no MCI window>"), lTitle);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CMciWnd::LogMciMetrics (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("ACM Metrics");
			}

			try
			{
				DWORD	lVersion;
				DWORD	lCodecCount = 0;
				DWORD	lConverterCount = 0;
				DWORD	lDisabledCount = 0;
				DWORD	lDriverCount = 0;
				DWORD	lFilterCount = 0;
				DWORD	lHardwareCount = 0;
				DWORD	lLocalCodecCount = 0;
				DWORD	lLocalConverterCount = 0;
				DWORD	lLocalDisabledCount = 0;
				DWORD	lLocalDriverCount = 0;
				DWORD	lLocalFilterCount = 0;

				lVersion = acmGetVersion ();

				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_CODECS, &lCodecCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_CONVERTERS, &lConverterCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_DISABLED, &lDisabledCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_DRIVERS, &lDriverCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_FILTERS, &lFilterCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_HARDWARE, &lHardwareCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_LOCAL_CODECS, &lLocalCodecCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_LOCAL_CONVERTERS, &lLocalConverterCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_LOCAL_DISABLED, &lLocalDisabledCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_LOCAL_DRIVERS, &lLocalDriverCount));
				LogMciErr (pLogLevel, acmMetrics (NULL, ACM_METRIC_COUNT_LOCAL_FILTERS, &lLocalFilterCount));

				LogMessage (pLogLevel, _T("%s [%u.%.02u.%04u]"), lTitle, HIBYTE (HIWORD (lVersion)), LOBYTE (HIWORD (lVersion)), LOWORD (lVersion));
				LogMessage (pLogLevel|LogHighVolume, _T("  Global Codecs     [%u]"), lCodecCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Global Converters [%u]"), lConverterCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Global Disabled   [%u]"), lDisabledCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Global Drivers    [%u]"), lDriverCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Global Filters    [%u]"), lFilterCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Global Hardware   [%u]"), lHardwareCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Local Codecs      [%u]"), lLocalCodecCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Local Converters  [%u]"), lLocalConverterCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Local Disabled    [%u]"), lLocalDisabledCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Local Drivers     [%u]"), lLocalDriverCount);
				LogMessage (pLogLevel|LogHighVolume, _T("  Local Filters     [%u]"), lLocalFilterCount);
			}
			catch AnyExceptionSilent
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
/////////////////////////////////////////////////////////////////////////////

static CString SupportStr (DWORD fdwSupport)
{
	CString	lSupport;

	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_ASYNC)
	{
		lSupport += _T("SUPPORTF_ASYNC, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_CODEC)
	{
		lSupport += _T("SUPPORTF_CODEC, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_CONVERTER)
	{
		lSupport += _T("SUPPORTF_CONVERTER, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_DISABLED)
	{
		lSupport += _T("SUPPORTF_DISABLED, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_FILTER)
	{
		lSupport += _T("SUPPORTF_FILTER, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_FILTER)
	{
		lSupport += _T("SUPPORTF_FILTER, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_HARDWARE)
	{
		lSupport += _T("SUPPORTF_HARDWARE, ");
	}
	if	(fdwSupport & ACMDRIVERDETAILS_SUPPORTF_LOCAL)
	{
		lSupport += _T("SUPPORTF_LOCAL, ");
	}
	lSupport.TrimRight (_T(" ,"));

	return lSupport;
}

/////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK FormatLogCallback (HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD_PTR dwInstance, DWORD fdwSupport)
{
	UINT	lLogLevel = (UINT) dwInstance | LogHighVolume;

	LogMessage (lLogLevel, _T("          Format             [%d]"), pafd->dwFormatIndex);
	LogMessage (lLogLevel, _T("            FormatTag        [%u]"), pafd->dwFormatTag);
	LogMessage (lLogLevel, _T("            Support          [%s] [%s]"), SupportStr (fdwSupport), SupportStr (pafd->fdwSupport));
	LogMessage (lLogLevel, _T("            Channels         [%hu]"), pafd->pwfx->nChannels);
	LogMessage (lLogLevel, _T("            Samples/sec      [%u]"), pafd->pwfx->nSamplesPerSec);
	LogMessage (lLogLevel, _T("            Avg. Bytes/sec   [%u]"), pafd->pwfx->nAvgBytesPerSec);
	LogMessage (lLogLevel, _T("            Bits/sample      [%hu]"), pafd->pwfx->wBitsPerSample);
	LogMessage (lLogLevel, _T("            Block alignment  [%hu]"), pafd->pwfx->nBlockAlign);

	return TRUE;
}

static BOOL CALLBACK FormatTagLogCallback (HACMDRIVERID hadid, LPACMFORMATTAGDETAILS paftd, DWORD_PTR dwInstance, DWORD fdwSupport)
{
	UINT	lLogLevel = (UINT) dwInstance | LogHighVolume;

	LogMessage (lLogLevel, _T("      FormatTag          [%d]"), paftd->dwFormatTagIndex);
	LogMessage (lLogLevel, _T("        Tag              [%u]"), paftd->dwFormatTag);
	LogMessage (lLogLevel, _T("        Name             [%s]"), paftd->szFormatTag);
	LogMessage (lLogLevel, _T("        Standard Formats [%u]"), paftd->cStandardFormats);
	LogMessage (lLogLevel, _T("        Support          [%s] [%s]"), SupportStr (fdwSupport), SupportStr (paftd->fdwSupport));

	try
	{
		HACMDRIVER						lDriver = 0;
		tSS <ACMFORMATDETAILS, DWORD>	lFormatDet;
		tArrayPtr <BYTE>				lFormat;

		if	(LogMciErr (lLogLevel, acmDriverOpen (&lDriver, hadid, 0)) == ERROR_SUCCESS)
		{
			LogMciErr (lLogLevel, acmMetrics ((HACMOBJ) lDriver, ACM_METRIC_MAX_SIZE_FORMAT, &lFormatDet.cbwfx));

			if	(lFormatDet.cbwfx > 0)
			{
				lFormatDet.pwfx = (LPWAVEFORMATEX) (LPBYTE) (lFormat = new BYTE [lFormatDet.cbwfx]);
				lFormatDet.dwFormatTag = paftd->dwFormatTag;
				lFormatDet.pwfx->wFormatTag = (WORD) paftd->dwFormatTag;

				LogMciErr (LogNormal, acmFormatEnum (lDriver, &lFormatDet, FormatLogCallback, dwInstance, ACM_FORMATENUMF_OUTPUT | ACM_FORMATENUMF_WFORMATTAG));
			}
			LogMciErr (lLogLevel, acmDriverClose (lDriver, 0));
		}
	}
	catch AnyExceptionDebug

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK DriverLogCallback (HACMDRIVERID hadid, DWORD_PTR dwInstance, DWORD fdwSupport)
{
	UINT							lLogLevel = (UINT) dwInstance | LogHighVolume;
	tSS <ACMDRIVERDETAILS, DWORD>	lDetails;
	CString							lDetailStr;
	DWORD							lPriority = -1;

	lDetails.fccType = ACMDRIVERDETAILS_FCCTYPE_AUDIOCODEC;
	lDetails.fccComp = ACMDRIVERDETAILS_FCCCOMP_UNDEFINED;
	acmDriverDetails (hadid, &lDetails, 0);

	acmMetrics ((HACMOBJ) hadid, ACM_METRIC_DRIVER_PRIORITY, &lPriority);

	LogMessage (lLogLevel, _T("  Driver      [%8.8X (%u)]"), hadid, hadid);
	LogMessage (lLogLevel, _T("    ShortName [%s]"), lDetails.szShortName);
	LogMessage (lLogLevel, _T("    LongName  [%s]"), lDetails.szLongName);
	LogMessage (lLogLevel, _T("    Copyright [%s]"), lDetails.szCopyright);
	LogMessage (lLogLevel, _T("    Licensing [%s]"), lDetails.szLicensing);
	LogMessage (lLogLevel, _T("    Feature   [%s]"), lDetails.szFeatures);
	LogMessage (lLogLevel, _T("    Formats   [%u]"), lDetails.cFormatTags);
	LogMessage (lLogLevel, _T("    Filters   [%u]"), lDetails.cFilterTags);
	LogMessage (lLogLevel, _T("    Priority  [%u]"), lPriority);
	LogMessage (lLogLevel, _T("    Version   [%u.%.02u.%04u]"), HIBYTE (HIWORD (lDetails.vdwDriver)), LOBYTE (HIWORD (lDetails.vdwDriver)), LOWORD (lDetails.vdwDriver));
	LogMessage (lLogLevel, _T("    ACM       [%u.%.02u.%04u]"), HIBYTE (HIWORD (lDetails.vdwACM)), LOBYTE (HIWORD (lDetails.vdwACM)), LOWORD (lDetails.vdwACM));
	LogMessage (lLogLevel, _T("    Support   [%s] [%s]"), SupportStr (fdwSupport), SupportStr (lDetails.fdwSupport));

	try
	{
		HACMDRIVER							lDriver = 0;
		tSS <ACMFORMATTAGDETAILS, DWORD>	lFormatTagDet;

		if	(LogMciErr (lLogLevel, acmDriverOpen (&lDriver, hadid, 0)) == ERROR_SUCCESS)
		{
			LogMciErr (lLogLevel, acmFormatTagEnum (lDriver, &lFormatTagDet, FormatTagLogCallback, dwInstance, 0));
			LogMciErr (lLogLevel, acmDriverClose (lDriver, 0));
		}
	}
	catch AnyExceptionSilent

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////

void CMciWnd::LogMciDrivers (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("MCI Drivers");
			}

			LogMessage (pLogLevel, _T("%s"), lTitle);
			acmDriverEnum (DriverLogCallback, (DWORD_PTR) pLogLevel, ACM_DRIVERENUMF_DISABLED);
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CMciWnd::LogAviFilters (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle;
			TCHAR	lFilter [MAX_PATH*10];
			LPCTSTR	lFilterPart;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("AVI Filters");
			}

			memset (lFilter, 0, sizeof (lFilter));

			if	(SUCCEEDED (LogComErr (pLogLevel, AVIBuildFilter (lFilter, sizeof (lFilter), FALSE))))
			{
				LogMessage (pLogLevel, _T("%s"), lTitle);

				for	(lFilterPart = lFilter; (*lFilterPart); lFilterPart += _tcslen (lFilterPart)+1)
				{
					LogMessage (pLogLevel, _T("  [%s]"), lFilterPart);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString MciStyleStr (DWORD pMciStyle)
{
	CString	lStyleStr;

	lStyleStr.Format (_T("%8.8X "), pMciStyle);
#ifdef	_DEBUG
	if	(pMciStyle & MCIWNDF_NOAUTOSIZEWINDOW)
	{
		lStyleStr += _T("NOAUTOSIZEWINDOW ");
	}
	if	(pMciStyle & MCIWNDF_NOPLAYBAR)
	{
		lStyleStr += _T("NOPLAYBAR ");
	}
	if	(pMciStyle & MCIWNDF_NOAUTOSIZEMOVIE)
	{
		lStyleStr += _T("NOAUTOSIZEMOVIE ");
	}
	if	(pMciStyle & MCIWNDF_NOMENU)
	{
		lStyleStr += _T("NOMENU ");
	}
	if	(pMciStyle & MCIWNDF_RECORD)
	{
		lStyleStr += _T("RECORD ");
	}
	if	(pMciStyle & MCIWNDF_NOERRORDLG)
	{
		lStyleStr += _T("NOERRORDLG ");
	}
	if	(pMciStyle & MCIWNDF_NOOPEN)
	{
		lStyleStr += _T("NOOPEN ");
	}
	if	((pMciStyle & MCIWNDF_SHOWALL) == MCIWNDF_SHOWALL)
	{
		lStyleStr += _T("SHOWALL ");
	}
	else
	{
		if	(pMciStyle & MCIWNDF_SHOWNAME)
		{
			lStyleStr += _T("SHOWNAME ");
		}
		if	(pMciStyle & MCIWNDF_SHOWPOS)
		{
			lStyleStr += _T("SHOWPOS ");
		}
		if	(pMciStyle & MCIWNDF_SHOWMODE)
		{
			lStyleStr += _T("SHOWMODE ");
		}
	}
	if	((pMciStyle & MCIWNDF_NOTIFYALL) == MCIWNDF_NOTIFYALL)
	{
		lStyleStr += _T("NOTIFYALL ");
	}
	else
	{
		if	(pMciStyle & MCIWNDF_NOTIFYMODE)
		{
			lStyleStr += _T("NOTIFYMODE ");
		}
		if	(pMciStyle & MCIWNDF_NOTIFYPOS)
		{
			lStyleStr += _T("NOTIFYPOS ");
		}
		if	(pMciStyle & MCIWNDF_NOTIFYSIZE)
		{
			lStyleStr += _T("NOTIFYSIZE ");
		}
		if	(pMciStyle & MCIWNDF_NOTIFYERROR)
		{
			lStyleStr += _T("NOTIFYERROR ");
		}
	}
	if	(pMciStyle & MCIWNDF_NOTIFYANSI)
	{
		lStyleStr += _T("NOTIFYANSI ");
	}
	if	(pMciStyle & MCIWNDF_NOTIFYMEDIAW)
	{
		lStyleStr += _T("NOTIFYMEDIA ");
	}
#endif
	lStyleStr.TrimRight();
	return lStyleStr;
}
