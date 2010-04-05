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

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetEnabled (long *pbEnabled)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetEnabled"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(mFile->GetStyle() & CharStyleBalloon)
		&&	(mGlobalConfig.LoadConfig().mEnabled)
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetEnabled"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetNumLines (long *plLines)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetNumLines"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plLines)
	{
		lResult = E_POINTER;
	}
	else
	if	(mCustomConfig)
	{
		(*plLines) = (long)(short)mCustomConfig->mLines;
	}
	else
	{
		(*plLines) = (long)(short)mFile->GetBalloon().mLines;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetNumLines"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetNumCharsPerLine (long *plCharsPerLine)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetNumCharsPerLine"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plCharsPerLine)
	{
		lResult = E_POINTER;
	}
	else
	if	(mCustomConfig)
	{
		(*plCharsPerLine) = (long)(short)mCustomConfig->mPerLine;
	}
	else
	{
		(*plCharsPerLine) = (long)(short)mFile->GetBalloon().mPerLine;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetNumCharsPerLine"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetNumLines (long lLines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetNumLines [%d]"), this, m_dwRef, mCharID, lLines);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetNumLines"), _T("%d"), lLines);
#endif
		if	(
				(lLines < (long)CAgentBalloonWnd::mMinLines)
			||	(lLines > (long)CAgentBalloonWnd::mMaxLines)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mLines = LOBYTE(lLines);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetNumLines [%d]"), this, m_dwRef, mCharID, lLines);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetNumCharsPerLine (long lCharsPerLine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetNumCharsPerLine [%d]"), this, m_dwRef, mCharID, lCharsPerLine);
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
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetNumCharsPerLine"), _T("%d"), lCharsPerLine);
#endif
		if	(
				(lCharsPerLine < (long)CAgentBalloonWnd::mMinPerLine)
			||	(lCharsPerLine > (long)CAgentBalloonWnd::mMaxPerLine)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mPerLine = LOBYTE(lCharsPerLine);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetNumCharsPerLine [%d]"), this, m_dwRef, mCharID, lCharsPerLine);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontName (BSTR *pbszFontName)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbszFontName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*pbszFontName) = _bstr_t (mGlobalConfig.mFont->lfFaceName).Detach ();
	}
	else
	if	(mCustomConfig)
	{
		(*pbszFontName) = tBstrPtr (mCustomConfig->mFontName).Detach ();
	}
	else
	{
		(*pbszFontName) = tBstrPtr (mFile->GetBalloon().mFontName).Detach ();
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontSize (long *plFontSize)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plFontSize)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*plFontSize) = mGlobalConfig.mFont->lfHeight;
	}
	else
	if	(mCustomConfig)
	{
		(*plFontSize) = mCustomConfig->mFontHeight;
	}
	else
	{
		(*plFontSize) = mFile->GetBalloon().mFontHeight;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontBold (long *pbFontBold)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontBold"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontBold)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontBold) = (mGlobalConfig.mFont->lfWeight >= FW_BOLD);
	}
	else
	if	(mCustomConfig)
	{
		(*pbFontBold) = (mCustomConfig->mFontWeight >= FW_BOLD);
	}
	else
	{
		(*pbFontBold) = (mFile->GetBalloon().mFontWeight >= FW_BOLD);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontBold"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontItalic (long *pbFontItalic)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontItalic"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontItalic)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontItalic) = mGlobalConfig.mFont->lfItalic ? TRUE : FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*pbFontItalic) = mCustomConfig->mFontItalic ? TRUE : FALSE;
	}
	else
	{
		(*pbFontItalic) = mFile->GetBalloon().mFontItalic ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontItalic"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontStrikethru (long *pbFontStrikethru)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontStrikethru"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontStrikethru) = mGlobalConfig.mFont->lfStrikeOut ? TRUE : FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*pbFontStrikethru) = mCustomConfig->mFontStrikethru ? TRUE : FALSE;
	}
	else
	{
		(*pbFontStrikethru) = mFile->GetBalloon().mFontStrikethru ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontStrikethru"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontUnderline (long *pbFontUnderline)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontUnderline"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!pbFontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*pbFontUnderline) = mGlobalConfig.mFont->lfUnderline ? TRUE : FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*pbFontUnderline) = mCustomConfig->mFontUnderline ? TRUE : FALSE;
	}
	else
	{
		(*pbFontUnderline) = mFile->GetBalloon().mFontUnderline ? TRUE : FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontUnderline"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetFontName (BSTR bszFontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetFontName"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
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
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetFontName"), _T("%ls"), bszFontName);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFontName = _bstr_t(bszFontName, true).Detach();
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetFontName"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetFontSize (long lFontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetFontSize"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
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
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetFontSize"), _T("%d"), lFontSize);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFontHeight = lFontSize;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetFontSize"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetFontCharSet (short sFontCharSet)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetFontCharSet"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd = NULL;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
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
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetFontCharSet"), _T("%hd"), sFontCharSet);
#endif
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFontCharset = LOBYTE(sFontCharSet);
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lBalloonWnd = GetBalloonWnd (mCharID))
		)
	{
		lBalloonWnd->SetOptions (mFile->GetBalloon(), this, mLangID);
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetFontCharSet"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetFontCharSet (short *psFontCharSet)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontCharSet"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!psFontCharSet)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*psFontCharSet) = mGlobalConfig.mFont->lfCharSet;
	}
	else
	if	(mCustomConfig)
	{
		(*psFontCharSet) = mCustomConfig->mFontCharset;
	}
	else
	{
		(*psFontCharSet) = mFile->GetBalloon().mFontCharset;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetFontCharSet"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetForeColor (long *plFGColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetForeColor"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plFGColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFgColor)
	{
		(*plFGColor) = *mGlobalConfig.mFgColor;
	}
	else
	if	(mCustomConfig)
	{
		(*plFGColor) = mCustomConfig->mFgColor;
	}
	else
	{
		(*plFGColor) = mFile->GetBalloon().mFgColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetForeColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetBackColor (long *plBGColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetBackColor"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plBGColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mBkColor)
	{
		(*plBGColor) = *mGlobalConfig.mBkColor;
	}
	else
	if	(mCustomConfig)
	{
		(*plBGColor) = mCustomConfig->mBkColor;
	}
	else
	{
		(*plBGColor) = mFile->GetBalloon().mBkColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetBackColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetBorderColor (long *plBorderColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetBorderColor"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plBorderColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mBrColor)
	{
		(*plBorderColor) = *mGlobalConfig.mBrColor;
	}
	else
	if	(mCustomConfig)
	{
		(*plBorderColor) = mCustomConfig->mBrColor;
	}
	else
	{
		(*plBorderColor) = mFile->GetBalloon().mBrColor;
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetBorderColor"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetVisible (long bVisible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetVisible"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(lBalloonWnd = GetBalloonWnd ())
	{
#ifdef	_TRACE_CHARACTER_ACTIONS
		_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetVisible"), _T("%d"), bVisible);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetVisible (long *pbVisible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetVisible"), this, m_dwRef, mCharID);
#endif
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(
			(lBalloonWnd = GetBalloonWnd())
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetVisible"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::SetStyle (long Style)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::SetStyle [%8.8X]"), this, m_dwRef, mCharID, Style);
#endif
	HRESULT				lResult = S_OK;
	DWORD				lCharStyle = mFile->GetStyle();
	CAgentBalloonWnd *	lBalloonWnd = NULL;

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:SetStyle"), _T("0x%8.8X"), Style);
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

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::SetStyle"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetStyle (long *plStyle)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%d] DaSvrBalloon::GetStyle"), this, m_dwRef, mCharID);
#endif
	HRESULT	lResult = S_OK;

	if	(!plStyle)
	{
		lResult = E_POINTER;
	}
	else
	{
		DWORD	lCharStyle;

		if	(mCustomStyle)
		{
			(*plStyle) = mCustomStyle->LowPart & ~BalloonStyle_Enabled;
			lCharStyle = mCustomStyle->HighPart;
		}
		else
		{
			(*plStyle) = BalloonStyle_AutoHide | BalloonStyle_AutoPace;
			lCharStyle = mFile->GetStyle();
		}

		if	(lCharStyle & CharStyleBalloon)
		{
			if	(mGlobalConfig.LoadConfig().mEnabled)
			{
				(*plStyle) |= BalloonStyle_Enabled;
			}
			if	(lCharStyle & CharStyleSizeToText)
			{
				(*plStyle) |= BalloonStyle_SizeToText;
			}
			if	(lCharStyle & CharStyleNoAutoHide)
			{
				(*plStyle) &= ~BalloonStyle_AutoHide;
			}
			if	(lCharStyle & CharStyleNoAutoPace)
			{
				(*plStyle) &= ~BalloonStyle_AutoPace;
			}
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%d] DaSvrBalloon::GetStyle"), this, m_dwRef, mCharID);
	}
#endif
	return lResult;
}
