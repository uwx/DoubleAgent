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
#include "DaSvrBalloon.h"
#include "AgentBalloonWnd.h"
#include "AgentPopupWnd.h"
#include "GuidStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Balloon"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Balloon"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrBalloon::DaSvrBalloon ()
:	mCharID (0),
	mLangID (GetUserDefaultUILanguage ()),
	mOwner (NULL),
	mFile (NULL),
	mNotify (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::DaSvrBalloon (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrBalloon::~DaSvrBalloon ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::~DaSvrBalloon (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrBalloon * DaSvrBalloon::CreateInstance (long pCharID, CAgentPopupWnd * pOwner, CAgentFile * pFile, _IServerNotify * pNotify)
{
	CComObject<DaSvrBalloon> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrBalloon>::CreateInstance (&lInstance))))
	{
		lInstance->mCharID = pCharID;
		lInstance->mOwner = pOwner;
		lInstance->mOwnerRefHolder = pOwner->GetControllingUnknown();
		lInstance->mFile = pFile;
		lInstance->mNotify = pNotify;

		_AtlModule.AddFileClient (lInstance->mFile, lInstance);
	}
	return lInstance;
}

void DaSvrBalloon::Terminate (bool pFinal, bool pAbandonned)
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
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		SafeFreeSafePtr (mOwnerRefHolder);

		try
		{
			_AtlModule.RemoveFileClient (mFile, this);
		}
		catch AnyExceptionSilent
	}
}

void DaSvrBalloon::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrBalloon::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaSvrBalloon), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaSvrBalloon2), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentBalloon), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentBalloonEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonWnd * DaSvrBalloon::GetBalloonWnd (long pCharID)
{
	CAgentBalloonWnd *	lBalloonWnd = NULL;
	CAgentPopupWnd *	lOwner;

	if	(
			(lOwner = GetOwnerWnd ())
		&&	(lBalloonWnd = lOwner->GetBalloonWnd())
		&&	(lBalloonWnd->IsWindow ())
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

CAgentPopupWnd * DaSvrBalloon::GetOwnerWnd ()
{
	if	(
			(mOwner)
		&&	(mOwner->IsWindow ())
		)
	{
		return mOwner;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileBalloon * DaSvrBalloon::GetCustomConfig (bool pCreate)
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

long DaSvrBalloon::GetCharID () const
{
	return mCharID;
}

LANGID DaSvrBalloon::GetLangID () const
{
	return mLangID;
}

bool DaSvrBalloon::SetLangID (LANGID pLangID)
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

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetEnabled (long *Enabled)
{
	VARIANT_BOOL	lEnabled;
	HRESULT			lResult = get_Enabled (&lEnabled);

	if	(Enabled)
	{
		(*Enabled) = lEnabled?TRUE:FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetNumLines (long *Lines)
{
	return get_NumberOfLines (Lines);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetNumCharsPerLine (long *CharsPerLine)
{
	return get_CharsPerLine (CharsPerLine);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetNumLines (long Lines)
{
	return put_NumberOfLines (Lines);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetNumCharsPerLine (long CharsPerLine)
{
	return put_CharsPerLine (CharsPerLine);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontName (BSTR *FontName)
{
	return get_FontName (FontName);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontSize (long *FontSize)
{
	return get_FontSize (FontSize);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontBold (long *FontBold)
{
	VARIANT_BOOL	lFontBold;
	HRESULT			lResult = S_OK;

	if	(!FontBold)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = get_FontBold (&lFontBold);
		(*FontBold) = lFontBold ? TRUE : FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontItalic (long *FontItalic)
{
	VARIANT_BOOL	lFontItalic;
	HRESULT			lResult = S_OK;

	if	(!FontItalic)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = get_FontItalic (&lFontItalic);
		(*FontItalic) = lFontItalic ? TRUE : FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontStrikethru (long *FontStrikethru)
{
	VARIANT_BOOL	lFontStrikethru;
	HRESULT			lResult = S_OK;

	if	(!FontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = get_FontStrikethru (&lFontStrikethru);
		(*FontStrikethru) = lFontStrikethru ? TRUE : FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontUnderline (long *FontUnderline)
{
	VARIANT_BOOL	lFontUnderline;
	HRESULT			lResult = S_OK;

	if	(!FontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	{
		lResult = get_FontUnderline (&lFontUnderline);
		(*FontUnderline) = lFontUnderline ? TRUE : FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetFontName (BSTR FontName)
{
	return put_FontName (FontName);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetFontSize (long FontSize)
{
	return put_FontSize (FontSize);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetFontCharSet (short FontCharSet)
{
	return put_FontCharSet (FontCharSet);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontCharSet (short *FontCharSet)
{
	return get_FontCharSet (FontCharSet);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetForeColor (long *ForeColor)
{
	return get_TextColor (ForeColor);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetBackColor (long *BackColor)
{
	return get_BackColor (BackColor);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetBorderColor (long *BorderColor)
{
	return get_BorderColor (BorderColor);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetVisible (long Visible)
{
	return put_Visible (Visible ? VARIANT_TRUE : VARIANT_FALSE);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetVisible (long *Visible)
{
	VARIANT_BOOL	lVisible;
	HRESULT			lResult = get_Visible (&lVisible);
	
	if	(Visible)
	{
		(*Visible) = lVisible ? TRUE : FALSE;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetStyle (long Style)
{
	return put_Style (Style);
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetStyle (long *Style)
{
	return get_Style (Style);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_Enabled (VARIANT_BOOL *Enabled)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_Enabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(mFile->GetStyle() & CharStyleBalloon)
		&&	(mGlobalConfig.LoadConfig().mEnabled)
		)
	{
		if	(Enabled)
		{
			(*Enabled) = VARIANT_TRUE;
		}
	}
	else
	{
		if	(Enabled)
		{
			(*Enabled) = VARIANT_FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_Enabled"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_Style (long *Style)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_Style"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!Style)
	{
		lResult = E_POINTER;
	}
	else
	{
		DWORD	lCharStyle;

		if	(mCustomStyle)
		{
			(*Style) = mCustomStyle->LowPart & ~BalloonStyle_Enabled;
			lCharStyle = mCustomStyle->HighPart;
		}
		else
		{
			(*Style) = BalloonStyle_AutoHide | BalloonStyle_AutoPace;
			lCharStyle = mFile->GetStyle();
		}

		if	(lCharStyle & CharStyleBalloon)
		{
			if	(mGlobalConfig.LoadConfig().mEnabled)
			{
				(*Style) |= BalloonStyle_Enabled;
			}
			if	(lCharStyle & CharStyleSizeToText)
			{
				(*Style) |= BalloonStyle_SizeToText;
			}
			if	(lCharStyle & CharStyleNoAutoHide)
			{
				(*Style) &= ~BalloonStyle_AutoHide;
			}
			if	(lCharStyle & CharStyleNoAutoPace)
			{
				(*Style) &= ~BalloonStyle_AutoPace;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_Style"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_Style (long Style)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_Style [%8.8X]"), this, m_dwRef, mCharID, Style);
#endif
	HRESULT				lResult = S_OK;
	DWORD				lCharStyle = mFile->GetStyle();
	CAgentBalloonWnd *	lBalloonWnd = NULL;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_Style"), _T("0x%8.8X"), Style);
#endif
	if	(
			((lCharStyle & CharStyleBalloon) == 0)
		||	(!mGlobalConfig.LoadConfig().mEnabled)
		)
	{
		lResult = S_FALSE;
	}

	if	(Style & BalloonStyle_Enabled)
	{
		lCharStyle |= CharStyleBalloon;
	}
	else
	{
		lCharStyle &= ~CharStyleBalloon;
	}
	if	(Style & BalloonStyle_SizeToText)
	{
		lCharStyle |= CharStyleSizeToText;
	}
	else
	{
		lCharStyle &= ~CharStyleSizeToText;

		if	(
				(
					(!mCustomStyle)
				||	(mCustomStyle->HighPart & CharStyleSizeToText)
				)
			&&	(mCustomConfig)
			)
		{
			mCustomConfig->mLines = mFile->GetBalloon().mLines;
			mCustomConfig->mPerLine = mFile->GetBalloon().mPerLine;
		}
	}
	if	(Style & BalloonStyle_AutoHide)
	{
		lCharStyle &= ~CharStyleNoAutoHide;
	}
	else
	{
		lCharStyle |= CharStyleNoAutoHide;
	}
	if	(Style & BalloonStyle_AutoPace)
	{
		lCharStyle &= ~CharStyleNoAutoPace;
	}
	else
	{
		lCharStyle |= CharStyleNoAutoPace;
	}

	if	(mCustomStyle = new ULARGE_INTEGER)
	{
		mCustomStyle->LowPart = Style;
		mCustomStyle->HighPart = lCharStyle;
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_Style"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_Visible"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(
			(lBalloonWnd = GetBalloonWnd())
		&&	(lBalloonWnd->IsWindowVisible())
		)
	{
		if	(Visible)
		{
			(*Visible) = VARIANT_TRUE;
		}
	}
	else
	{
		if	(Visible)
		{
			(*Visible) = VARIANT_FALSE;
		}
		lResult = S_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_Visible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_Visible"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(lBalloonWnd = GetBalloonWnd ())
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_Visible"), _T("%d"), Visible);
#endif
		if	(Visible)
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

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_Visible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_NumberOfLines (long *NumberOfLines)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_NumberOfLines"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!NumberOfLines)
	{
		lResult = E_POINTER;
	}
	else
	if	(mCustomConfig)
	{
		(*NumberOfLines) = (long)(short)mCustomConfig->mLines;
	}
	else
	{
		(*NumberOfLines) = (long)(short)mFile->GetBalloon().mLines;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_NumberOfLines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_NumberOfLines (long NumberOfLines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_NumberOfLines [%d]"), this, m_dwRef, mCharID, NumberOfLines);
#endif
	HRESULT				lResult = S_OK;
	long				lStyle;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	GetStyle (&lStyle);

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lStyle & BalloonStyle_SizeToText)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_NumberOfLines"), _T("%d"), NumberOfLines);
#endif
		if	(
				(NumberOfLines < (long)CAgentBalloonWnd::mMinLines)
			||	(NumberOfLines > (long)CAgentBalloonWnd::mMaxLines)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mLines = LOBYTE(NumberOfLines);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_NumberOfLines [%d]"), this, m_dwRef, mCharID, NumberOfLines);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_CharsPerLine (long *CharsPerLine)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_CharsPerLine"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!CharsPerLine)
	{
		lResult = E_POINTER;
	}
	else
	if	(mCustomConfig)
	{
		(*CharsPerLine) = (long)(short)mCustomConfig->mPerLine;
	}
	else
	{
		(*CharsPerLine) = (long)(short)mFile->GetBalloon().mPerLine;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_CharsPerLine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_CharsPerLine (long CharsPerLine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_CharsPerLine [%d]"), this, m_dwRef, mCharID, CharsPerLine);
#endif
	HRESULT				lResult = S_OK;
	long				lStyle;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	GetStyle (&lStyle);

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lStyle & BalloonStyle_SizeToText)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_CharsPerLine"), _T("%d"), CharsPerLine);
#endif
		if	(
				(CharsPerLine < (long)CAgentBalloonWnd::mMinPerLine)
			||	(CharsPerLine > (long)CAgentBalloonWnd::mMaxPerLine)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mPerLine = LOBYTE(CharsPerLine);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_CharsPerLine [%d]"), this, m_dwRef, mCharID, CharsPerLine);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_TextColor (long *TextColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_TextColor"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!TextColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFgColor)
	{
		(*TextColor) = *mGlobalConfig.mFgColor;
	}
	else
	if	(mCustomConfig)
	{
		(*TextColor) = mCustomConfig->mFgColor;
	}
	else
	{
		(*TextColor) = mFile->GetBalloon().mFgColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_TextColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_TextColor (long TextColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_TextColor"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_TextColor"), _T("%0x8.8X"), TextColor);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFgColor = TextColor;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_TextColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_BackColor (long *BackColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_BackColor"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!BackColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mBkColor)
	{
		(*BackColor) = *mGlobalConfig.mBkColor;
	}
	else
	if	(mCustomConfig)
	{
		(*BackColor) = mCustomConfig->mBkColor;
	}
	else
	{
		(*BackColor) = mFile->GetBalloon().mBkColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_BackColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_BackColor (long BackColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_BackColor"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_BackColor"), _T("%0x8.8X"), BackColor);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mBkColor = BackColor;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_BackColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_BorderColor (long *BorderColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_BorderColor"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!BorderColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mBrColor)
	{
		(*BorderColor) = *mGlobalConfig.mBrColor;
	}
	else
	if	(mCustomConfig)
	{
		(*BorderColor) = mCustomConfig->mBrColor;
	}
	else
	{
		(*BorderColor) = mFile->GetBalloon().mBrColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_BorderColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_BorderColor (long BorderColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_BorderColor"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_BorderColor"), _T("%0x8.8X"), BorderColor);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mBrColor = BorderColor;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_BorderColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontName (BSTR *FontName)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontName) = _bstr_t (mGlobalConfig.mFont->lfFaceName).Detach ();
	}
	else
	if	(mCustomConfig)
	{
		(*FontName) = _bstr_t (mCustomConfig->mFont.lfFaceName).Detach ();
	}
	else
	{
		(*FontName) = _bstr_t (mFile->GetBalloon().mFont.lfFaceName).Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontName (BSTR FontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontName"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(
			(!FontName)
		||	(!FontName [0])
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontName"), _T("%ls"), FontName);
#endif
		if	(GetCustomConfig (true))
		{
			_tcsncpy (mCustomConfig->mFont.lfFaceName, CAtlString(FontName), (sizeof(mCustomConfig->mFont.lfFaceName)/sizeof(WCHAR))-1);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontSize (long *FontSize)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontSize) = mGlobalConfig.mFont->lfHeight;
	}
	else
	if	(mCustomConfig)
	{
		(*FontSize) = mCustomConfig->mFont.lfHeight;
	}
	else
	{
		(*FontSize) = mFile->GetBalloon().mFont.lfHeight;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontSize (long FontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(FontSize == 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontSize"), _T("%d"), FontSize);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfHeight = FontSize;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontBold (VARIANT_BOOL *FontBold)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontBold"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontBold)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontBold) = (mGlobalConfig.mFont->lfWeight >= FW_BOLD) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontBold) = (mCustomConfig->mFont.lfWeight >= FW_BOLD) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontBold) = (mFile->GetBalloon().mFont.lfWeight >= FW_BOLD) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontBold"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontBold (VARIANT_BOOL FontBold)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontBold"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontBold"), _T("%d"), FontBold);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfWeight = FontBold ? FW_BOLD : FW_NORMAL;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontBold"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontItalic (VARIANT_BOOL *FontItalic)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontItalic"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontItalic)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontItalic) = mGlobalConfig.mFont->lfItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontItalic) = mCustomConfig->mFont.lfItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontItalic) = mFile->GetBalloon().mFont.lfItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontItalic"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontItalic (VARIANT_BOOL FontItalic)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontItalic"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontItalic"), _T("%d"), FontItalic);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfItalic = FontItalic ? TRUE : FALSE;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontItalic"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontStrikethru (VARIANT_BOOL *FontStrikethru)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontStrikethru"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontStrikethru) = mGlobalConfig.mFont->lfStrikeOut ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontStrikethru) = mCustomConfig->mFont.lfStrikeOut ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontStrikethru) = mFile->GetBalloon().mFont.lfStrikeOut ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontStrikethru"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontStrikethru (VARIANT_BOOL FontStrikethru)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontStrikethru"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontStrikethru"), _T("%d"), FontStrikethru);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfStrikeOut = FontStrikethru ? TRUE : FALSE;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontStrikethru"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontUnderline (VARIANT_BOOL *FontUnderline)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontUnderline"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontUnderline) = mGlobalConfig.mFont->lfUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontUnderline) = mCustomConfig->mFont.lfUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontUnderline) = mFile->GetBalloon().mFont.lfUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontUnderline"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontUnderline (VARIANT_BOOL FontUnderline)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontUnderline"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontUnderline"), _T("%d"), FontUnderline);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfUnderline = FontUnderline ? TRUE : FALSE;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontUnderline"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontCharSet (short *FontCharSet)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontCharSet"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!FontCharSet)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontCharSet) = mGlobalConfig.mFont->lfCharSet;
	}
	else
	if	(mCustomConfig)
	{
		(*FontCharSet) = mCustomConfig->mFont.lfCharSet;
	}
	else
	{
		(*FontCharSet) = mFile->GetBalloon().mFont.lfCharSet;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::get_FontCharSet"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontCharSet (short FontCharSet)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontCharSet"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(
			(FontCharSet < 0)
		||	(FontCharSet > 255)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontCharSet"), _T("%hd"), FontCharSet);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfCharSet = LOBYTE(FontCharSet);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::put_FontCharSet"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
