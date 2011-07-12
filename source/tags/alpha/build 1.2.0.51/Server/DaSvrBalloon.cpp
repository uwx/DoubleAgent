/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "AgentCharacterWnd.h"
#include "GuidStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Balloon"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Balloon"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED		MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#define	_DEBUG_ABANDONED	_LOG_ABANDONED
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrBalloon::DaSvrBalloon ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::DaSvrBalloon (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrBalloon::~DaSvrBalloon ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::~DaSvrBalloon (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrBalloon * DaSvrBalloon::CreateInstance (long pCharID, CInstanceAnchor* pAnchor, CAgentFile* pFile, CAgentCharacterWnd* pOwner, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrBalloon>*	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive|LogTime, CComObject<DaSvrBalloon>::CreateInstance (&lInstance))))
	{
		lInstance->CDaCmnBalloon::Initialize (pCharID, pAnchor, pFile, pOwner);
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrBalloon::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::Terminate [%u %u]"), this, max(m_dwRef,-1), pFinal, pAbandonned);
		}
#endif
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
 		if	(pAbandonned)
		{
			Disconnect (pAbandonned);
		}
		if	(pFinal)
		{
			Disconnect (pAbandonned);
		}
	}
}

void DaSvrBalloon::Disconnect (bool pAbandonned)
{
	if	(m_dwRef > 0)
	{
		if	(pAbandonned)
		{
			m_dwRef = SHRT_MIN;
		}
		__try
		{
			LogComErr (LogIfActive, CoDisconnectObject (GetUnknown(), 0));
		}
		__except (LogCrashCode (GetExceptionCode(), __FILE__, __LINE__, EXCEPTION_EXECUTE_HANDLER))
		{}
	}
	if	(pAbandonned)
	{
		m_dwRef = min (m_dwRef, SHRT_MIN);
	}
	else
	{
		m_dwRef = min (m_dwRef, 0);
	}
}

void DaSvrBalloon::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrBalloon::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, !_VerifyClientLifetime());
}

void DaSvrBalloon::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrBalloon::OnClientEnded"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false, true);
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

long DaSvrBalloon::GetCharID () const
{
	return mCharID;
}

LANGID DaSvrBalloon::GetLangID () const
{
	return CDaCmnBalloon::mLangID;
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
	HRESULT		lResult;
	OLE_COLOR	lTextColor;

	if	(ForeColor)
	{
		lResult = get_TextColor (&lTextColor);
		(*ForeColor) = CDaCmnBalloon::GetOleColor (lTextColor);
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetBackColor (long *BackColor)
{
	HRESULT		lResult;
	OLE_COLOR	lBackColor;

	if	(BackColor)
	{
		lResult = get_BackColor (&lBackColor);
		(*BackColor) = CDaCmnBalloon::GetOleColor (lBackColor);
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::GetBorderColor (long *BorderColor)
{
	HRESULT		lResult;
	OLE_COLOR	lBorderColor;

	if	(BorderColor)
	{
		lResult = get_BorderColor (&lBorderColor);
		(*BorderColor) = CDaCmnBalloon::GetOleColor (lBorderColor);
	}
	else
	{
		lResult = E_POINTER;
	}
	return lResult;
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_Enabled"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_Enabled (Enabled);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_Enabled"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_Style (long *Style)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_Style"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_Style (Style);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_Style"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_Style (long Style)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_Style [%8.8X]"), this, mCharID, max(m_dwRef,-1), Style);
#endif
	HRESULT	lResult = CDaCmnBalloon::put_Style (Style);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_Style"), _T("0x%8.8X"), Style);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_Style"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_Visible (VARIANT_BOOL *Visible)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_Visible"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_Visible (Visible);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_Visible"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_Visible (VARIANT_BOOL Visible)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_Visible"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_Visible (Visible);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_Visible"), _T("%d"), Visible);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_Visible"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_NumberOfLines (long *NumberOfLines)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_NumberOfLines"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_NumberOfLines (NumberOfLines);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_NumberOfLines"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_NumberOfLines (long NumberOfLines)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_NumberOfLines [%d]"), this, mCharID, max(m_dwRef,-1), NumberOfLines);
#endif
	HRESULT	lResult = CDaCmnBalloon::put_NumberOfLines (NumberOfLines);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_NumberOfLines"), _T("%d"), NumberOfLines);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_NumberOfLines [%d]"), this, mCharID, max(m_dwRef,-1), NumberOfLines);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_CharsPerLine (long *CharsPerLine)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_CharsPerLine"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_CharsPerLine (CharsPerLine);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_CharsPerLine"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_CharsPerLine (long CharsPerLine)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_CharsPerLine [%d]"), this, mCharID, max(m_dwRef,-1), CharsPerLine);
#endif
	HRESULT	lResult = CDaCmnBalloon::put_CharsPerLine (CharsPerLine);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_CharsPerLine"), _T("%d"), CharsPerLine);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_CharsPerLine [%d]"), this, mCharID, max(m_dwRef,-1), CharsPerLine);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_TextColor (OLE_COLOR *TextColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_TextColor"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_TextColor (TextColor);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_TextColor"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_TextColor (OLE_COLOR TextColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_TextColor"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_TextColor (TextColor);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_TextColor"), _T("%0x8.8X"), TextColor);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_TextColor"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_BackColor (OLE_COLOR *BackColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_BackColor"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_BackColor (BackColor);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_BackColor"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_BackColor (OLE_COLOR BackColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_BackColor"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_BackColor (BackColor);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_BackColor"), _T("%0x8.8X"), BackColor);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_BackColor"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_BorderColor (OLE_COLOR *BorderColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_BorderColor"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_BorderColor (BorderColor);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_BorderColor"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_BorderColor (OLE_COLOR BorderColor)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_BorderColor"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_BorderColor (BorderColor);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_BorderColor"), _T("%0x8.8X"), BorderColor);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_BorderColor"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontName (BSTR *FontName)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontName"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontName (FontName);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontName"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontName (BSTR FontName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontName"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontName (FontName);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontName"), _T("%ls"), FontName);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontName"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontSize (long *FontSize)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontSize"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontSize (FontSize);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontSize"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontSize (long FontSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontSize"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontSize (FontSize);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontSize"), _T("%d"), FontSize);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontSize"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontBold (VARIANT_BOOL *FontBold)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontBold"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontBold (FontBold);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontBold"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontBold (VARIANT_BOOL FontBold)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontBold"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontBold (FontBold);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontBold"), _T("%d"), FontBold);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontBold"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontItalic (VARIANT_BOOL *FontItalic)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontItalic"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontItalic (FontItalic);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontItalic"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontItalic (VARIANT_BOOL FontItalic)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontItalic"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontItalic (FontItalic);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontItalic"), _T("%d"), FontItalic);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontItalic"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontStrikethru (VARIANT_BOOL *FontStrikethru)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontStrikethru"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontStrikethru (FontStrikethru);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontStrikethru"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontStrikethru (VARIANT_BOOL FontStrikethru)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontStrikethru"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontStrikethru (FontStrikethru);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontStrikethru"), _T("%d"), FontStrikethru);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontStrikethru"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontUnderline (VARIANT_BOOL *FontUnderline)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontUnderline"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontUnderline (FontUnderline);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontUnderline"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontUnderline (VARIANT_BOOL FontUnderline)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontUnderline"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontUnderline (FontUnderline);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontUnderline"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrBalloon::get_FontCharSet (short *FontCharSet)
{
#ifdef	_DEBUG_INTERFACE_NOT
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontCharSet"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::get_FontCharSet (FontCharSet);

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::get_FontCharSet"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrBalloon::put_FontCharSet (short FontCharSet)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontCharSet"), this, mCharID, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnBalloon::put_FontCharSet (FontCharSet);

#ifdef	_TRACE_CHARACTER_ACTIONS
	_AtlModule.TraceCharacterAction (mCharID, _T("Balloon:put_FontCharSet"), _T("%hd"), FontCharSet);
#endif

	PutServerError (lResult, __uuidof(IDaSvrBalloon2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)(%d)] DaSvrBalloon::put_FontCharSet"), this, mCharID, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
