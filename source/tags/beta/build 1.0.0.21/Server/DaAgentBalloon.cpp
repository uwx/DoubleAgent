/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaServer.h"
#include "DaAgentBalloon.h"
#include "AgentBalloonWnd.h"
#include "AgentPopupWnd.h"
#include "GuidStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Balloon"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Balloon"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaAgentBalloon, CCmdTarget)
IMPLEMENT_OLETYPELIB(CDaAgentBalloon, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaAgentBalloon::CDaAgentBalloon (long pCharID, CAgentPopupWnd * pOwner, CAgentFile * pFile, IDaNotify & pNotify)
:	mCharID (pCharID),
	mLangID (GetUserDefaultUILanguage ()),
	mOwner (pOwner),
	mOwnerRefHolder (pOwner->GetIDispatch (FALSE)),
	mFile (pFile),
	mNotify (pNotify)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentBalloon::CDaAgentBalloon (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	TheServerApp->AddFileClient (mFile, this);

	EnableAutomation();
	EnableTypeLib();
}

CDaAgentBalloon::~CDaAgentBalloon ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentBalloon::~CDaAgentBalloon (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	Terminate (true);
	AfxOleUnlockApp();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentBalloon::~CDaAgentBalloon (%d) Done"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
}

void CDaAgentBalloon::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					ExternalDisconnect ();
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		SafeFreeSafePtr (mOwnerRefHolder);

		try
		{
			TheServerApp->RemoveFileClient (mFile, this);
		}
		catch AnyExceptionSilent
	}
}

void CDaAgentBalloon::OnFinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAgentBalloon::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

LPUNKNOWN CDaAgentBalloon::GetInterfaceHook(const void* iid)
{
#ifdef	_DEBUG_COM
	LogMessage (_DEBUG_COM, _T("[%p(%u)] CDaAgentBalloon::QueryInterface [%s]"), this, m_dwRef, CGuidStr::GuidName(*(GUID*)iid));
#endif
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaAgentBalloon, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAgentBalloon)
	DISP_FUNCTION(CDaAgentBalloon, "GetEnabled", DspGetEnabled, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetNumLines", DspGetNumLines, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetNumCharsPerLine", DspGetNumCharsPerLine, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontName", DspGetFontName, VT_EMPTY, VTS_PBSTR)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontSize", DspGetFontSize, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontBold", DspGetFontBold, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontItalic", DspGetFontItalic, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontStrikethru", DspGetFontStrikethru, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontUnderline", DspGetFontUnderline, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetForeColor", DspGetForeColor, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetBackColor", DspGetBackColor, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "GetBorderColor", DspGetBorderColor, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "SetVisible", DspSetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CDaAgentBalloon, "GetVisible", DspGetVisible, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "SetFontName", DspSetFontName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDaAgentBalloon, "SetFontSize", DspSetFontSize, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CDaAgentBalloon, "SetFontCharSet", DspSetFontCharSet, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CDaAgentBalloon, "GetFontCharSet", DspGetFontCharSet, VT_EMPTY, VTS_PI2)
	DISP_FUNCTION(CDaAgentBalloon, "SetStyle", DspSetStyle, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CDaAgentBalloon, "GetStyle", DspGetStyle, VT_EMPTY, VTS_PI4)
	DISP_FUNCTION(CDaAgentBalloon, "SetNumLines", DspSetNumLines, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CDaAgentBalloon, "SetNumCharsPerLine", DspSetNumCharsPerLine, VT_EMPTY, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaAgentBalloon, CCmdTarget)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(IDaSvrBalloon), Balloon)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(IAgentBalloon), Balloon)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(IAgentBalloonEx), Balloon)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaAgentBalloon, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAgentBalloon, Balloon)
IMPLEMENT_DISPATCH_IID(CDaAgentBalloon, __uuidof(IDaSvrBalloon))
IMPLEMENT_PROVIDECLASSINFO(CDaAgentBalloon, __uuidof(IDaSvrBalloon))

BEGIN_SUPPORTERRORINFO(CDaAgentBalloon)
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentBalloon, __uuidof(IDaSvrBalloon))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentBalloon, __uuidof(IAgentBalloon))
	IMPLEMENT_SUPPORTERRORINFO(CDaAgentBalloon, __uuidof(IAgentBalloonEx))
END_SUPPORTERRORINFO(CDaAgentBalloon)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonWnd * CDaAgentBalloon::GetBalloonWnd (long pCharID)
{
	CAgentBalloonWnd *	lBalloonWnd = NULL;
	CAgentPopupWnd *	lOwner;

	if	(
			(lOwner = GetOwnerWnd ())
		&&	(lBalloonWnd = lOwner->GetBalloonWnd())
		&&	(lBalloonWnd->GetSafeHwnd())
		&&	(
				(pCharID <= 0)
			||	(lBalloonWnd->GetCharID () == pCharID)
			)
		)
	{
		return lBalloonWnd;
	}
	return NULL;
}

CAgentPopupWnd * CDaAgentBalloon::GetOwnerWnd ()
{
	if	(mOwner->GetSafeHwnd())
	{
		return mOwner;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileBalloon * CDaAgentBalloon::GetCustomConfig (bool pCreate)
{
	if	(
			(!mCustomConfig)
		&&	(pCreate)
		&&	(mCustomConfig = new CAgentFileBalloon)
		)
	{
		(*mCustomConfig) = const_cast <CAgentFileBalloon &> (mFile->GetBalloon());
	}
	return mCustomConfig;
}

/////////////////////////////////////////////////////////////////////////////

long CDaAgentBalloon::GetCharID () const
{
	return mCharID;
}

LANGID CDaAgentBalloon::GetLangID () const
{
	return mLangID;
}

bool CDaAgentBalloon::SetLangID (LANGID pLangID)
{
	if	(pLangID == LANG_USER_DEFAULT)
	{
#ifdef	_DEBUG
		pLangID = LANGIDFROMLCID (GetThreadLocale ());
#else
		pLangID = GetUserDefaultUILanguage();
#endif
	}
	else
	if	(pLangID == LANG_SYSTEM_DEFAULT)
	{
		pLangID = GetSystemDefaultUILanguage();
	}

	if	(mLangID != pLangID)
	{
		CAgentFileBalloon *	lCustomConfig;
		tS <LOGFONT>		lOldFont;
		tS <LOGFONT>		lNewFont;

		if	(
				(mFile)
			&&	(lCustomConfig = GetCustomConfig (true))
			)
		{
			if	(
					(CAgentBalloonWnd::CopyBalloonFont (*lCustomConfig, lOldFont))
				&&	(CAgentBalloonWnd::SetFontLangID (lOldFont, mLangID))
				&&	(CAgentBalloonWnd::CopyBalloonFont (*lCustomConfig, lNewFont))
				&&	(CAgentBalloonWnd::SetFontLangID (lNewFont, pLangID))
				&&	(lNewFont.lfCharSet == lOldFont.lfCharSet)
				&&	(CAgentBalloonWnd::GetActualFont (lNewFont, lNewFont, false))
				)
			{
				CAgentBalloonWnd::CopyBalloonFont (lNewFont, *lCustomConfig);
			}
			else
			{
				lOldFont.Clear ();
				lNewFont.Clear ();

				if	(
						(CAgentBalloonWnd::CopyBalloonFont (mFile->GetBalloon(), lOldFont))
					&&	(CAgentBalloonWnd::SetFontLangID (lOldFont, pLangID))
					&&	(CAgentBalloonWnd::GetActualFont (lOldFont, lNewFont, false))
					)
				{
					CAgentBalloonWnd::CopyBalloonFont (lNewFont, *lCustomConfig);
				}
			}
		}

		mLangID = pLangID;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetEnabled (long *pbEnabled)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetEnabled"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(pThis->mFile->GetStyle() & CharStyleBalloon)
		&&	(pThis->mGlobalConfig.LoadConfig().mEnabled)
		)
	{
		if	(pbEnabled)
		{
			(*pbEnabled) = TRUE;
		}
	}
	else
	{
		if	(pbEnabled)
		{
			(*pbEnabled) = FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetEnabled"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetNumLines (long *plLines)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetNumLines"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plLines)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*plLines) = (long)(short)pThis->mCustomConfig->mLines;
	}
	else
	{
		(*plLines) = (long)(short)pThis->mFile->GetBalloon().mLines;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetNumLines"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetNumCharsPerLine (long *plCharsPerLine)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetNumCharsPerLine"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plCharsPerLine)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*plCharsPerLine) = (long)(short)pThis->mCustomConfig->mPerLine;
	}
	else
	{
		(*plCharsPerLine) = (long)(short)pThis->mFile->GetBalloon().mPerLine;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetNumCharsPerLine"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetNumLines (long lLines)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetNumLines"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	long				lStyle;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	GetStyle (&lStyle);

	if	((pThis->mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lStyle & BALLOON_STYLE_SIZETOTEXT)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetNumLines"), _T("%d"), lLines);
#endif
		if	(
				(lLines < (long)CAgentBalloonWnd::mMinLines)
			||	(lLines > (long)CAgentBalloonWnd::mMaxLines)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(pThis->GetCustomConfig (true))
		{
			pThis->mCustomConfig->mLines = LOBYTE(lLines);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = pThis->GetBalloonWnd (pThis->mCharID))
		)
	{
		lBalloonWnd->SetOptions (pThis->mFile->GetBalloon(), this, pThis->mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetNumLines"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetNumCharsPerLine (long lCharsPerLine)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetNumCharsPerLine"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	long				lStyle;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	GetStyle (&lStyle);

	if	((pThis->mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lStyle & BALLOON_STYLE_SIZETOTEXT)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetNumCharsPerLine"), _T("%d"), lCharsPerLine);
#endif
		if	(
				(lCharsPerLine < (long)CAgentBalloonWnd::mMinPerLine)
			||	(lCharsPerLine > (long)CAgentBalloonWnd::mMaxPerLine)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(pThis->GetCustomConfig (true))
		{
			pThis->mCustomConfig->mPerLine = LOBYTE(lCharsPerLine);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = pThis->GetBalloonWnd (pThis->mCharID))
		)
	{
		lBalloonWnd->SetOptions (pThis->mFile->GetBalloon(), this, pThis->mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetNumCharsPerLine"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontName (BSTR *pbszFontName)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszFontName)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*pbszFontName) = _bstr_t (pThis->mGlobalConfig.mFont->lfFaceName).Detach ();
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*pbszFontName) = tBstrPtr (pThis->mCustomConfig->mFontName).Detach ();
	}
	else
	{
		(*pbszFontName) = tBstrPtr (pThis->mFile->GetBalloon().mFontName).Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontSize (long *plFontSize)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plFontSize)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*plFontSize) = pThis->mGlobalConfig.mFont->lfHeight;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*plFontSize) = pThis->mCustomConfig->mFontHeight;
	}
	else
	{
		(*plFontSize) = pThis->mFile->GetBalloon().mFontHeight;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontBold (long *pbFontBold)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontBold"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontBold)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontBold) = (pThis->mGlobalConfig.mFont->lfWeight >= FW_BOLD);
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*pbFontBold) = (pThis->mCustomConfig->mFontWeight >= FW_BOLD);
	}
	else
	{
		(*pbFontBold) = (pThis->mFile->GetBalloon().mFontWeight >= FW_BOLD);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontBold"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontItalic (long *pbFontItalic)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontItalic"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontItalic)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontItalic) = pThis->mGlobalConfig.mFont->lfItalic ? TRUE : FALSE;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*pbFontItalic) = pThis->mCustomConfig->mFontItalic ? TRUE : FALSE;
	}
	else
	{
		(*pbFontItalic) = pThis->mFile->GetBalloon().mFontItalic ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontItalic"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontStrikethru (long *pbFontStrikethru)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontStrikethru"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontStrikethru) = pThis->mGlobalConfig.mFont->lfStrikeOut ? TRUE : FALSE;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*pbFontStrikethru) = pThis->mCustomConfig->mFontStrikethru ? TRUE : FALSE;
	}
	else
	{
		(*pbFontStrikethru) = pThis->mFile->GetBalloon().mFontStrikethru ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontStrikethru"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontUnderline (long *pbFontUnderline)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontUnderline"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontUnderline) = pThis->mGlobalConfig.mFont->lfUnderline ? TRUE : FALSE;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*pbFontUnderline) = pThis->mCustomConfig->mFontUnderline ? TRUE : FALSE;
	}
	else
	{
		(*pbFontUnderline) = pThis->mFile->GetBalloon().mFontUnderline ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontUnderline"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetFontName (BSTR bszFontName)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((pThis->mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(
			(!bszFontName)
		||	(!bszFontName [0])
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetFontName"), _T("%ls"), bszFontName);
#endif
		if	(pThis->GetCustomConfig (true))
		{
			pThis->mCustomConfig->mFontName = _bstr_t(bszFontName, true).Detach();
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = pThis->GetBalloonWnd (pThis->mCharID))
		)
	{
		lBalloonWnd->SetOptions (pThis->mFile->GetBalloon(), this, pThis->mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetFontName"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetFontSize (long lFontSize)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((pThis->mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lFontSize == 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetFontSize"), _T("%d"), lFontSize);
#endif
		if	(pThis->GetCustomConfig (true))
		{
			pThis->mCustomConfig->mFontHeight = lFontSize;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = pThis->GetBalloonWnd (pThis->mCharID))
		)
	{
		lBalloonWnd->SetOptions (pThis->mFile->GetBalloon(), this, pThis->mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetFontSize"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetFontCharSet (short sFontCharSet)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetFontCharSet"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((pThis->mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(
			(sFontCharSet < 0)
		||	(sFontCharSet > 255)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetFontCharSet"), _T("%hd"), sFontCharSet);
#endif
		if	(pThis->GetCustomConfig (true))
		{
			pThis->mCustomConfig->mFontCharset = LOBYTE(sFontCharSet);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = pThis->GetBalloonWnd (pThis->mCharID))
		)
	{
		lBalloonWnd->SetOptions (pThis->mFile->GetBalloon(), this, pThis->mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetFontCharSet"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetFontCharSet (short *psFontCharSet)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontCharSet"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!psFontCharSet)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFont)
	{
		(*psFontCharSet) = pThis->mGlobalConfig.mFont->lfCharSet;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*psFontCharSet) = pThis->mCustomConfig->mFontCharset;
	}
	else
	{
		(*psFontCharSet) = pThis->mFile->GetBalloon().mFontCharset;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetFontCharSet"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetForeColor (long *plFGColor)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetForeColor"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plFGColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mFgColor)
	{
		(*plFGColor) = *pThis->mGlobalConfig.mFgColor;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*plFGColor) = pThis->mCustomConfig->mFgColor;
	}
	else
	{
		(*plFGColor) = pThis->mFile->GetBalloon().mFgColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetForeColor"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetBackColor (long *plBGColor)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetBackColor"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plBGColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mBkColor)
	{
		(*plBGColor) = *pThis->mGlobalConfig.mBkColor;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*plBGColor) = pThis->mCustomConfig->mBkColor;
	}
	else
	{
		(*plBGColor) = pThis->mFile->GetBalloon().mBkColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetBackColor"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetBorderColor (long *plBorderColor)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetBorderColor"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plBorderColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mGlobalConfig.LoadConfig().mBrColor)
	{
		(*plBorderColor) = *pThis->mGlobalConfig.mBrColor;
	}
	else
	if	(pThis->mCustomConfig)
	{
		(*plBorderColor) = pThis->mCustomConfig->mBrColor;
	}
	else
	{
		(*plBorderColor) = pThis->mFile->GetBalloon().mBrColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetBorderColor"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetVisible (long bVisible)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(lBalloonWnd = pThis->GetBalloonWnd ())
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetVisible"), _T("%d"), bVisible);
#endif
		if	(bVisible)
		{
			if	(!lBalloonWnd->ShowBalloonNow ())
			{
				lResult = S_FALSE;
			}
		}
		else
		{
			if	(!lBalloonWnd->HideBalloon ())
			{
				lResult = S_FALSE;
			}
		}
	}
	else
	{
		lResult = AGENTERR_NOBALLOON;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetVisible (long *pbVisible)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(
			(lBalloonWnd = pThis->GetBalloonWnd())
		&&	(lBalloonWnd->IsWindowVisible())
		)
	{
		if	(pbVisible)
		{
			(*pbVisible) = TRUE;
		}
	}
	else
	{
		if	(pbVisible)
		{
			(*pbVisible) = FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetVisible"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::SetStyle (long lStyle)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetStyle [%8.8X]"), pThis, pThis->m_dwRef, pThis->mCharID, lStyle);
#endif
	HRESULT				lResult = S_OK;
	DWORD				lCharStyle = pThis->mFile->GetStyle();
	CAgentBalloonWnd *	lBalloonWnd = NULL;

#ifdef	_TRACE_CHARACTER_ACTIONS
	TheServerApp->TraceCharacterAction (pThis->mCharID, _T("Balloon:SetStyle"), _T("0x%8.8X"), lStyle);
#endif
	if	(
			((lCharStyle & CharStyleBalloon) == 0)
		||	(!pThis->mGlobalConfig.LoadConfig().mEnabled)
		)
	{
		lResult = S_FALSE;
	}

	if	(lStyle & BALLOON_STYLE_BALLOON_ON)
	{
		lCharStyle |= CharStyleBalloon;
	}
	else
	{
		lCharStyle &= ~CharStyleBalloon;
	}
	if	(lStyle & BALLOON_STYLE_SIZETOTEXT)
	{
		lCharStyle |= CharStyleSizeToText;
	}
	else
	{
		lCharStyle &= ~CharStyleSizeToText;
		if	(pThis->mCustomConfig)
		{
			pThis->mCustomConfig->mLines = pThis->mFile->GetBalloon().mLines;
			pThis->mCustomConfig->mPerLine = pThis->mFile->GetBalloon().mPerLine;
		}
	}
	if	(lStyle & BALLOON_STYLE_AUTOHIDE)
	{
		lCharStyle &= ~CharStyleNoAutoHide;
	}
	else
	{
		lCharStyle |= CharStyleNoAutoHide;
	}
	if	(lStyle & BALLOON_STYLE_AUTOPACE)
	{
		lCharStyle &= ~CharStyleNoAutoPace;
	}
	else
	{
		lCharStyle |= CharStyleNoAutoPace;
	}

	if	(lCharStyle == pThis->mFile->GetStyle())
	{
		pThis->mCustomStyle = NULL;
	}
	else
	if	(pThis->mCustomStyle = new DWORD)
	{
		*(pThis->mCustomStyle) = lCharStyle;
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = pThis->GetBalloonWnd (pThis->mCharID))
		)
	{
		lBalloonWnd->SetOptions (pThis->mFile->GetBalloon(), this, pThis->mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::SetStyle"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaAgentBalloon::XBalloon::GetStyle (long *plStyle)
{
	METHOD_PROLOGUE(CDaAgentBalloon, Balloon)
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetStyle"), pThis, pThis->m_dwRef, pThis->mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plStyle)
	{
		lResult = E_POINTER;
	}
	else
	{
		DWORD	lCharStyle;

		if	(pThis->mCustomStyle)
		{
			lCharStyle = *(pThis->mCustomStyle);
		}
		else
		{
			lCharStyle = pThis->mFile->GetStyle();
		}

		(*plStyle) = BALLOON_STYLE_AUTOHIDE | BALLOON_STYLE_AUTOPACE;

		if	(lCharStyle & CharStyleBalloon)
		{
			if	(pThis->mGlobalConfig.LoadConfig().mEnabled)
			{
				(*plStyle) |= BALLOON_STYLE_BALLOON_ON;
			}
			if	(lCharStyle & CharStyleSizeToText)
			{
				(*plStyle) |= BALLOON_STYLE_SIZETOTEXT;
			}
			if	(lCharStyle & CharStyleNoAutoHide)
			{
				(*plStyle) &= ~BALLOON_STYLE_AUTOHIDE;
			}
			if	(lCharStyle & CharStyleNoAutoPace)
			{
				(*plStyle) &= ~BALLOON_STYLE_AUTOPACE;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%d] CDaAgentBalloon::XBalloon::GetStyle"), pThis, pThis->m_dwRef, pThis->mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaAgentBalloon::DspGetEnabled(long * Enabled)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetEnabled (Enabled);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetNumLines(long * Lines)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetNumLines"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetNumLines (Lines);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetNumCharsPerLine(long * CharsPerLine)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetNumCharsPerLine"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetNumCharsPerLine (CharsPerLine);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontName(BSTR * FontName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontName"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontName (FontName);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontSize(long * FontSize)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontSize"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontSize(FontSize);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontBold(long * FontBold)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontBold"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontBold (FontBold);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontItalic(long * FontItalic)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontItalic"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontItalic (FontItalic);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontStrikethru(long * FontStrikethru)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontStrikethru"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontStrikethru (FontStrikethru);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontUnderline(long * FontUnderline)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontUnderline"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontUnderline (FontUnderline);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetForeColor(long * ForeColor)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetForeColor"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetForeColor (ForeColor);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetBackColor(long * BackColor)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetBackColor"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetBackColor (BackColor);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetBorderColor(long * BorderColor)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetBorderColor"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetBorderColor (BorderColor);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetVisible(long Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetVisible(long * Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetVisible"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetVisible (Visible);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetFontName(LPCTSTR FontName)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetFontName"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetFontName (_bstr_t (FontName));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetFontSize(long FontSize)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetFontSize"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetFontSize (FontSize);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetFontCharSet(short FontCharSet)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetFontCharSet"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetFontCharSet (FontCharSet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetFontCharSet(short * FontCharSet)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetFontCharSet"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetFontCharSet (FontCharSet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetStyle(long Style)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetStyle"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetStyle (Style);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspGetStyle(long * Style)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspGetStyle"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.GetStyle (Style);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetNumLines(long Lines)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetNumLines"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetNumLines (Lines);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaAgentBalloon::DspSetNumCharsPerLine(long CharsPerLine)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAgentBalloon::DspSetNumCharsPerLine"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.SetNumCharsPerLine (CharsPerLine);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
