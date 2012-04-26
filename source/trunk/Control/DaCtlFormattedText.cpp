/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#include "DaCtlFormattedText.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

#ifdef	_DACORE_LOCAL
#define	LogServerPtr	(void*)NULL
#else
#define	LogServerPtr	mServerObject.GetInterfacePtr()
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlFormattedText::DaCtlFormattedText ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::DaCtlFormattedText (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlFormattedText::~DaCtlFormattedText ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::~DaCtlFormattedText (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlFormattedText::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::FinalRelease (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
	Terminate (false);
}

void DaCtlFormattedText::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::Terminate [%u] [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, LogServerPtr, mLocalObject.Ptr());
		}
#endif
#endif
		if	(pFinal)
		{
			mOwner = NULL;
		}
		SafeFreeSafePtr (mLocalObject);
#ifndef	_DACORE_LOCAL
		if	(pFinal)
		{
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#endif
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::Terminate [%u] [%p] [%p] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, LogServerPtr, mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlFormattedText::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_FALSE;

	if	(mOwner = pOwner)
	{
#ifndef	_DACORE_LOCAL
		if	(mOwner->mServer == NULL)
#endif
		{
#ifndef	_DACORE_LOCAL
			if	(mServerObject != NULL)
			{
				lResult = E_FAIL;
			}
			else
#endif
			if	(mLocalObject = new CDaCmnFormattedText ())
			{
				lResult = S_OK;
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
#ifndef	_DACORE_LOCAL
		else
		{
			if	(mServerObject == NULL)
			{
				lResult = E_FAIL;
			}
			else
			{
				lResult = S_OK;
			}
		}
#endif
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel (_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::SetOwner (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
	return lResult;
}

DaControl * DaCtlFormattedText::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlFormattedText::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlFormattedText::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaCtlFormattedText), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_RawText (BSTR *RawText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_RawText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!RawText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RawText) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_RawText (RawText);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_RawText (RawText);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_RawText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::put_RawText (BSTR RawText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::put_RawText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_RawText (RawText);
		}
		catch AnyExceptionDebug
	}
#ifndef	_DACORE_LOCAL
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_RawText (RawText);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
#endif

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::put_Text"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_ParsedText(BSTR *ParsedText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_ParsedText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!ParsedText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ParsedText) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_ParsedText (ParsedText);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_ParsedText (ParsedText);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_ParsedText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_SpeechText(BSTR *SpeechText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_SpeechText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!SpeechText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechText) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_SpeechText (SpeechText);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_SpeechText (SpeechText);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_SpeechText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_WordCount (long *WordCount)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_WordCount"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!WordCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*WordCount) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_WordCount (WordCount);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_WordCount (WordCount);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_WordCount"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_DisplayWord (long WordIndex, BSTR *DisplayWord)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_DisplayWord"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!DisplayWord)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DisplayWord) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DisplayWord (WordIndex, DisplayWord);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DisplayWord (WordIndex, DisplayWord);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_DisplayWord"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_WordIndex (long *WordIndex)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_WordIndex"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!WordIndex)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*WordIndex) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_WordIndex (WordIndex);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_WordIndex (WordIndex);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_WordIndex"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::get_DisplayText (BSTR *DisplayText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_DisplayText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(!DisplayText)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DisplayText) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_DisplayText (DisplayText);
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_DisplayText (DisplayText);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::get_DisplayText"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}


/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::Parse (BSTR RawText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::Parse"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Parse (RawText);
		}
		catch AnyExceptionDebug
	}
#ifndef	_DACORE_LOCAL
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Parse (RawText);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
#endif

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::Parse"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlFormattedText::Append (IDaCtlFormattedText *FormattedText)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::Append"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT					lResult = E_INVALIDARG;
	DaCtlFormattedText *	lFormattedText = NULL;

	try
	{
		lFormattedText = dynamic_cast <CComObject <DaCtlFormattedText>*> (FormattedText);
	}
	catch AnyExceptionSilent

	if	(lFormattedText)
	{
		if	(mLocalObject)
		{
			try
			{
				if	(lFormattedText->mLocalObject)
				{
					lResult = mLocalObject->Append (lFormattedText->mLocalObject);
				}
			}
			catch AnyExceptionDebug
		}
#ifndef	_DACORE_LOCAL
		else
		if	(
				(lFormattedText->mServerObject)
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				lResult = mServerObject->Append (lFormattedText->mServerObject);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
#endif
	}

	PutControlError (lResult, __uuidof(IDaCtlFormattedText));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlFormattedText::Append"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

