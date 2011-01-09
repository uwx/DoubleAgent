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
#include "DaSvrFormattedText.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_ABANDONED			MinLogLevel(GetProfileDebugInt(_T("LogAbandoned"),LogDetails,true)&0xFFFF|LogTime,_LOG_INSTANCE)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrFormattedText::DaSvrFormattedText ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrFormattedText::DaSvrFormattedText (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

DaSvrFormattedText::~DaSvrFormattedText ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrFormattedText::~DaSvrFormattedText (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaSvrFormattedText::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}

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
	}
}

void DaSvrFormattedText::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrFormattedText::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
	Terminate (false);
}

void DaSvrFormattedText::OnClientEnded()
{
#ifdef	_LOG_ABANDONED
	if	(LogIsActive (_LOG_ABANDONED))
	{
		LogMessage (_LOG_ABANDONED, _T("[%p(%d)] DaSvrFormattedText::OnClientEnded"), this, max(m_dwRef,-1));
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

STDMETHODIMP DaSvrFormattedText::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrFormattedText), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_RawText (BSTR *RawText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_RawText"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_RawText (RawText);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_RawText"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::put_RawText (BSTR RawText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::put_RawText"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::put_RawText (RawText);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::put_RawText"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_ParsedText(BSTR *ParsedText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_ParsedText"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_ParsedText (ParsedText);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_ParsedText"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_SpeechText(BSTR *SpeechText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_SpeechText"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_SpeechText (SpeechText);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_SpeechText"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_WordCount (long *WordCount)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_WordCount"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_WordCount (WordCount);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_WordCount"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_DisplayWord (long WordIndex, BSTR *DisplayWord)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_DisplayWord"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_DisplayWord (WordIndex, DisplayWord);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_DisplayWord"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_WordIndex (long *WordIndex)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_WordIndex"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_WordIndex (WordIndex);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_WordIndex"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::get_DisplayText (BSTR *DisplayText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::get_DisplayText"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::get_DisplayText (DisplayText);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::get_DisplayText"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::Parse (BSTR RawText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::Parse"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = CDaCmnFormattedText::Parse (RawText);

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::Parse"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrFormattedText::Append (IDaSvrFormattedText *FormattedText)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrFormattedText::Append"), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = E_INVALIDARG;
	DaSvrFormattedText *	lFormattedText = NULL;

	try
	{
		lFormattedText = dynamic_cast <CComObject <DaSvrFormattedText> *> (FormattedText);
	}
	catch AnyExceptionSilent

	if	(lFormattedText)
	{
		lResult = CDaCmnFormattedText::Append (lFormattedText);
	}

	PutServerError (lResult, __uuidof(IDaSvrFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrFormattedText::Append"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
