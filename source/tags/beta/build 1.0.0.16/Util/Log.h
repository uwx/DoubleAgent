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

#ifdef	__cplusplus
extern "C"
{
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
}
#endif

#ifdef	_LOG_DISABLED

static inline DWORD LogStart (bool pNewLogFile = false, LPCTSTR pLogFileName = NULL, UINT pLogLevel = 0) {return 0;}
static inline void LogStop (unsigned int pPutLogEnd = 15) {}
static inline void LogControl (LPTSTR pLogFileName, UINT & pLogLevel) {}
static inline bool LogLock () {return false;}
static inline bool LogUnlock () {return false;}
static inline bool LogIsActive (UINT pLogLevel = LogIfActive) {return false;}
static inline bool LogMessage (UINT pLogLevel, LPCTSTR pFormat, ...) {return true;}
static inline void LogWrite (LPCTSTR pStr, LPCTSTR pLogFileName = NULL) {}
static inline int LogWriteCache (LPCTSTR pLogFileName = NULL) {}
static inline bool LogDump (UINT pLogLevel, LPVOID pBuffer, UINT pBufferSize, LPCTSTR pPrefix = NULL, bool pDumpOffsets = false) {return true;}
static inline bool LogDumpBits (UINT pLogLevel, LPVOID pBuffer, UINT pBufferSize, LPCTSTR pPrefix = NULL, UINT pBytesPerLine = 8) {return true;}
static inline DWORD LogWinErr (UINT pLogLevel, DWORD pError, LPCTSTR pFormat = NULL, ...) {return pError;}
static inline HRESULT LogComErr (UINT pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...) {return pError;}
static inline void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false) {}
static inline int LogDebugMemory (int pDbgFlag = 0) {return 0;}
#define	LogWinErrAnon LogWinErr
#define	LogComErrAnon LogComErr

#else	// _LOG_DISABLED

#ifdef	__cplusplus
extern "C"
{
DWORD LogStart (bool pNewLogFile = false, LPCTSTR pLogFileName = NULL, UINT pLogLevel = 0);
void LogStop (unsigned int pPutLogEnd = 15);
void LogControl (LPTSTR pLogFileName, UINT & pLogLevel);
bool LogLock ();
bool LogUnlock ();
bool LogIsActive (UINT pLogLevel = LogIfActive);
bool LogMessage (UINT pLogLevel, LPCTSTR pFormat, ...);
void LogWrite (LPCTSTR pStr, LPCTSTR pLogFileName = NULL);
int LogWriteCache (LPCTSTR pLogFileName = NULL);
bool LogDump (UINT pLogLevel, LPVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix = NULL, bool pDumpOffsets = false);
bool LogDumpBits (UINT pLogLevel, LPVOID pBuffer, SIZE_T pBufferSize, LPCTSTR pPrefix = NULL, UINT pBytesPerLine = 8);
#ifdef	_CRTDBG_ALLOC_MEM_DF
void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false);
int LogDebugMemory (int pDbgFlag = _CRTDBG_ALLOC_MEM_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#else
static inline void LogDebugRuntime (bool pDebugRuntime = true, bool pAsserts = true, bool pErrors = true, bool pWarnings = false) {}
static inline int LogDebugMemory (int pDbgFlag = 0) {return 0;}
#endif
}

#ifdef	_LOG_NOFILELINE
extern "C" DWORD LogWinErr (UINT pLogLevel, DWORD pError, LPCTSTR pFormat = NULL, ...);
extern "C" HRESULT LogComErr (UINT pLogLevel, HRESULT pError, LPCTSTR pFormat = NULL, ...);
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

#else	// __cplusplus

extern DWORD LogStart (int pNewLogFile = false, LPCTSTR pLogFileName = NULL, UINT pLogLevel = 0);
extern void LogStop (unsigned int pPutLogEnd = 15);
extern void LogControl (LPTSTR pLogFileName, UINT & pLogLevel);
extern bool LogLock ();
extern bool LogUnlock ();
extern int LogIsActive (unsigned int pLogLevel = LogIfActive);
extern bool LogMessage (unsigned int pLogLevel, LPCTSTR pFormat, ...);
extern void LogWrite (LPCTSTR pStr, LPCTSTR pLogFileName = NULL);
extern int LogWriteCache (LPCTSTR pLogFileName = NULL);
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
#endif	// _LOG_DISABLED

#ifdef	__AFX_H__
static inline void LogException (UINT pLogLevel, CException * pException, LPCSTR pFile, UINT pLine)
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

////////////////////////////////////////////////////////////////////////

inline UINT MinLogLevel (UINT a, UINT b)
{
	return (((a & 0x00FF) <= (b & 0x00FF)) ? a : b) | (a & 0xFFFFFF00) | (b & 0xFFFFFF00);
}

inline UINT MaxLogLevel (UINT a, UINT b)
{
	return (((a & 0x00FF) >= (b & 0x00FF)) ? a : b) | (a & 0xFFFFFF00) | (b & 0xFFFFFF00);
}

////////////////////////////////////////////////////////////////////////
#ifdef	__AFX_H__
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

static inline CString DebugStr (LPCTSTR pString)
{
	CString	lString (pString);
	lString.Replace (_T("\r"), _T("\\r"));
	lString.Replace (_T("\n"), _T("\\n"));
	lString.Replace (_T("\t"), _T("\\t"));
	lString.Replace (_T("\f"), _T("\\f"));
	return lString;
}

////////////////////////////////////////////////////////////////////////

static CString WndClassName (HWND pWnd)
{
	CString	lClassName;
	if	(pWnd)
	{
		if	(IsWindow (pWnd))
		{
			GetClassName (pWnd, lClassName.GetBuffer(MAX_PATH), MAX_PATH);
			lClassName.ReleaseBuffer ();
		}
		else
		{
			lClassName = _T("---Not a window---");
		}
	}
	return lClassName;
}

static CString ObjClassName (CObject * const pObject)
{
	if	(pObject)
	{
		return CString (pObject->GetRuntimeClass()->m_lpszClassName);
	}
	return CString();
}

#ifdef	__AFXWIN_H__

static CString WndClassName (CWnd * const pWnd)
{
	if	(pWnd)
	{
		return CString (pWnd->GetRuntimeClass()->m_lpszClassName);
	}
	return CString();
}

#endif	//	__AFXWIN_H__
////////////////////////////////////////////////////////////////////////

static CString DrawTextFlags (UINT pFlags, bool pIncludeDefaults = true)
{
	CString	lRet;

	if	(
			(pIncludeDefaults)
		&&	((pFlags & (DT_CENTER|DT_RIGHT)) == 0)
		)
	{
		lRet += _T("LEFT ");
	}
	if	(pFlags & DT_CENTER)
	{
		lRet += _T("CENTER ");
	}
	if	(pFlags & DT_RIGHT)
	{
		lRet += _T("RIGHT ");
	}

	if	(
			(pIncludeDefaults)
		&&	((pFlags & (DT_VCENTER|DT_BOTTOM)) == 0)
		)
	{
		lRet += _T("TOP ");
	}
	if	(pFlags & DT_VCENTER)
	{
		lRet += _T("VCENTER ");
	}
	if	(pFlags & DT_BOTTOM)
	{
		lRet += _T("BOTTOM ");
	}

	if	(pFlags & DT_WORDBREAK)
	{
		lRet += _T("WORDBREAK ");
	}
	if	(pFlags & DT_SINGLELINE)
	{
		lRet += _T("SINGLELINE ");
	}
	if	(pFlags & DT_EXPANDTABS)
	{
		lRet += _T("EXPANDTABS ");
	}
	if	(pFlags & DT_TABSTOP)
	{
		lRet += _T("TABSTOP ");
	}
	if	(pFlags & DT_NOCLIP)
	{
		lRet += _T("NOCLIP ");
	}
	if	(pFlags & DT_EXTERNALLEADING)
	{
		lRet += _T("EXTERNALLEADING ");
	}
	if	(pFlags & DT_CALCRECT)
	{
		lRet += _T("CALCRECT ");
	}
	if	(pFlags & DT_NOPREFIX)
	{
		lRet += _T("NOPREFIX ");
	}
	if	(pFlags & DT_INTERNAL)
	{
		lRet += _T("INTERNAL ");
	}
#ifdef	DT_EDITCONTROL
	if	(pFlags & DT_EDITCONTROL)
	{
		lRet += _T("EDITCONTROL ");
	}
	if	(pFlags & DT_PATH_ELLIPSIS)
	{
		lRet += _T("PATH_ELLIPSIS ");
	}
	if	(pFlags & DT_END_ELLIPSIS)
	{
		lRet += _T("END_ELLIPSIS ");
	}
	if	(pFlags & DT_MODIFYSTRING)
	{
		lRet += _T("MODIFYSTRING ");
	}
	if	(pFlags & DT_RTLREADING)
	{
		lRet += _T("RTLREADING ");
	}
	if	(pFlags & DT_WORD_ELLIPSIS)
	{
		lRet += _T("WORD_ELLIPSIS ");
	}
#endif
#ifdef	DT_NOFULLWIDTHCHARBREAK
	if	(pFlags & DT_NOFULLWIDTHCHARBREAK)
	{
		lRet += _T("NOFULLWIDTHCHARBREAK ");
	}
#endif
#ifdef	DT_HIDEPREFIX
	if	(pFlags & DT_HIDEPREFIX)
	{
		lRet += _T("HIDEPREFIX ");
	}
	if	(pFlags & DT_PREFIXONLY)
	{
		lRet += _T("PREFIXONLY ");
	}
#endif

	lRet.TrimRight ();
	return lRet;
}

////////////////////////////////////////////////////////////////////////

static CString ItemDrawType (UINT pCtlType)
{
	CString	lRet;

	switch (pCtlType)
	{
		case ODT_MENU:		lRet = _T("MENU"); break;
		case ODT_LISTBOX:	lRet = _T("LISTBOX"); break;
		case ODT_COMBOBOX:	lRet = _T("COMBOBOX"); break;
		case ODT_BUTTON:	lRet = _T("BUTTON"); break;
		case ODT_STATIC:	lRet = _T("STATIC"); break;
		default:			lRet.Format (_T("-%u-"), pCtlType);
	}
	return lRet;
}

static CString ItemDrawType (LPDRAWITEMSTRUCT pDIS)
{
	if	(pDIS)
	{
		return ItemDrawType (pDIS->CtlType);
	}
	return CString();
}

static CString ItemDrawAction (UINT pAction)
{
	CString	lRet;

	if	(pAction & ODA_DRAWENTIRE)
	{
		lRet += _T("DRAWENTIRE ");
	}
	if	(pAction & ODA_SELECT)
	{
		lRet += _T("SELECT ");
	}
	if	(pAction & ODA_FOCUS)
	{
		lRet += _T("FOCUS ");
	}

	lRet.TrimRight ();
	return lRet;
}

static CString ItemDrawAction (LPDRAWITEMSTRUCT pDIS)
{
	if	(pDIS)
	{
		return ItemDrawAction (pDIS->itemAction);
	}
	return CString();
}

static CString ItemDrawState (UINT pState)
{
	CString	lRet;

	if	(pState & ODS_SELECTED)
	{
		lRet += _T("SELECTED ");
	}
	if	(pState & ODS_GRAYED)
	{
		lRet += _T("GRAYED ");
	}
	if	(pState & ODS_DISABLED)
	{
		lRet += _T("DISABLED ");
	}
	if	(pState & ODS_CHECKED)
	{
		lRet += _T("CHECKED ");
	}
	if	(pState & ODS_FOCUS)
	{
		lRet += _T("FOCUS ");
	}
	if	(pState & ODS_DEFAULT)
	{
		lRet += _T("DEFAULT ");
	}
#ifdef	ODS_HOTLIGHT
	if	(pState & ODS_HOTLIGHT)
	{
		lRet += _T("HOTLIGHT ");
	}
#endif
#ifdef	ODS_INACTIVE
	if	(pState & ODS_INACTIVE)
	{
		lRet += _T("INACTIVE ");
	}
#endif
#ifdef	ODS_NOACCEL
	if	(pState & ODS_NOACCEL)
	{
		lRet += _T("NOACCEL ");
	}
#endif
#ifdef	ODS_NOFOCUSRECT
	if	(pState & ODS_NOFOCUSRECT)
	{
		lRet += _T("NOFOCUSRECT ");
	}
#endif
	if	(pState & ODS_COMBOBOXEDIT)
	{
		lRet += _T("COMBOBOXEDIT ");
	}

	lRet.TrimRight ();
	return lRet;
}

static CString ItemDrawState (LPDRAWITEMSTRUCT pDIS)
{
	if	(pDIS)
	{
		return ItemDrawState (pDIS->itemState);
	}
	return CString();
}

////////////////////////////////////////////////////////////////////////

static CString CustomDrawStage (DWORD pStage)
{
	CString	lRet;

	switch (pStage & ~CDDS_ITEM & ~CDDS_SUBITEM)
	{
		case CDDS_PREPAINT:		lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPREPAINT") : (pStage & CDDS_ITEM) ? _T("ITEMPREPAINT") : _T("PREPAINT"); break;
		case CDDS_POSTPAINT:	lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPOSTPAINT") : (pStage & CDDS_ITEM) ? _T("ITEMPOSTPAINT") : _T("POSTPAINT"); break;
		case CDDS_PREERASE:		lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPREERASE") : (pStage & CDDS_ITEM) ? _T("ITEMPREERASE") : _T("PREERASE"); break;
		case CDDS_POSTERASE:	lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPOSTERASE") : (pStage & CDDS_ITEM) ? _T("ITEMPOSTERASE") : _T("POSTERASE"); break;
		default:					lRet.Format (_T("-%8.8X-"), pStage);
	}
	return lRet;
}

static CString CustomDrawStage (LPNMCUSTOMDRAW pCustomDraw)
{
	if	(pCustomDraw)
	{
		return CustomDrawStage (pCustomDraw->dwDrawStage);
	}
	return CString();
}

static CString CustomDrawState (UINT pState)
{
	CString	lRet;

	if	(pState & CDIS_SELECTED)
	{
		lRet += _T("SELECTED ");
	}
	if	(pState & CDIS_GRAYED)
	{
		lRet += _T("GRAYED ");
	}
	if	(pState & CDIS_DISABLED)
	{
		lRet += _T("DISABLED ");
	}
	if	(pState & CDIS_CHECKED)
	{
		lRet += _T("CHECKED ");
	}
	if	(pState & CDIS_FOCUS)
	{
		lRet += _T("FOCUS ");
	}
	if	(pState & CDIS_DEFAULT)
	{
		lRet += _T("DEFAULT ");
	}
	if	(pState & CDIS_HOT)
	{
		lRet += _T("HOT ");
	}
	if	(pState & CDIS_MARKED)
	{
		lRet += _T("MARKED ");
	}
	if	(pState & CDIS_INDETERMINATE)
	{
		lRet += _T("INDETERMINATE ");
	}
#ifdef	CDIS_SHOWKEYBOARDCUES
	if	(pState & CDIS_SHOWKEYBOARDCUES)
	{
		lRet += _T("SHOWKEYBOARDCUES ");
	}
#endif
#ifdef	CDIS_NEARHOT
	if	(pState & CDIS_NEARHOT)
	{
		lRet += _T("NEARHOT ");
	}
#endif
#ifdef	CDIS_OTHERSIDEHOT
	if	(pState & CDIS_OTHERSIDEHOT)
	{
		lRet += _T("OTHERSIDEHOT ");
	}
#endif
#ifdef	CDIS_DROPHILITED
	if	(pState & CDIS_DROPHILITED)
	{
		lRet += _T("DROPHILITED ");
	}
#endif

	lRet.TrimRight ();
	return lRet;
}

static CString CustomDrawState (LPNMCUSTOMDRAW pCustomDraw)
{
	if	(pCustomDraw)
	{
		return CustomDrawState (pCustomDraw->uItemState);
	}
	return CString();
}

static CString CustomDrawResult (LRESULT pResult)
{
	CString	lRet;

	if	(pResult & CDRF_NEWFONT)
	{
		lRet += _T("NEWFONT ");
	}
	if	(pResult & CDRF_SKIPDEFAULT)
	{
		lRet += _T("SKIPDEFAULT ");
	}
#ifdef	CDRF_DOERASE
	if	(pResult & CDRF_DOERASE)
	{
		lRet += _T("DOERASE ");
	}
#endif
#ifdef	CDRF_SKIPPOSTPAINT
	if	(pResult & CDRF_SKIPPOSTPAINT)
	{
		lRet += _T("SKIPPOSTPAINT ");
	}
#endif
	if	(pResult & CDRF_NOTIFYPOSTPAINT)
	{
		lRet += _T("NOTIFYPOSTPAINT ");
	}
	if	(pResult & CDRF_NOTIFYITEMDRAW)
	{
		lRet += _T("NOTIFYITEMDRAW ");
	}
	if	(pResult & CDRF_NOTIFYSUBITEMDRAW)
	{
		lRet += _T("NOTIFYSUBITEMDRAW ");
	}
	if	(pResult & CDRF_NOTIFYPOSTERASE)
	{
		lRet += _T("NOTIFYPOSTERASE ");
	}
	if	(lRet.IsEmpty ())
	{
		lRet = _T("DODEFAULT");
	}

	lRet.TrimRight ();
	return lRet;
}

////////////////////////////////////////////////////////////////////////
#endif	//	__AFX_H__
////////////////////////////////////////////////////////////////////////
#endif	// _LOG_H
