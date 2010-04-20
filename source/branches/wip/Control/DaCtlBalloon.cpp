/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaControlMod.h"
#include "DaCtlBalloon.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Balloon"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Balloon"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlBalloon::DaCtlBalloon ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::DaCtlBalloon (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlBalloon::~DaCtlBalloon ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::~DaCtlBalloon (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mBalloon != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon Attached [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mBalloon.GetInterfacePtr());
		}
	}
	catch AnyExceptionSilent
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlBalloon::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::FinalRelease (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void DaCtlBalloon::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::Terminate [%u] [%p(%u)]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
		}
#endif
#endif
		if	(pFinal)
		{
			mOwner = NULL;
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::Terminate [%u] Done [%d]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlBalloon::SetOwner (DaCtlCharacter * pOwner)
{
	if	(mOwner = pOwner)
	{
		mServerObject = mOwner->mServerObject;
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::SetOwner (%d) [%p]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

DaCtlCharacter * DaCtlBalloon::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlBalloon::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlBalloon::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlBalloon), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlBalloon), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlBalloonEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_Enabled (VARIANT_BOOL *Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_Enabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lEnabled = VARIANT_FALSE;
	HRESULT			lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Enabled (&lEnabled);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Enabled)
	{
		(*Enabled) = lEnabled;
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_Enabled"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__NumberOfLines (long *Lines)
{
	return get_NumberOfLines (Lines);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_NumberOfLines (long *NumberOfLines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_NumberOfLines"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!NumberOfLines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*NumberOfLines) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_NumberOfLines (NumberOfLines);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_NumberOfLines"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_NumberOfLines (long NumberOfLines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_NumberOfLines"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_NumberOfLines (NumberOfLines);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_NumberOfLines"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__CharsPerLine (long *CharsPerLine)
{
	return get_CharsPerLine (CharsPerLine);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_CharsPerLine (long *CharsPerLine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_CharsPerLine"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!CharsPerLine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharsPerLine) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_CharsPerLine (CharsPerLine);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_CharsPerLine"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_CharsPerLine (long CharsPerLine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_CharsPerLine"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_CharsPerLine (CharsPerLine);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_CharsPerLine"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontName (BSTR *FontName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontName) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontName (FontName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontName (BSTR FontName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontName (FontName);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontName"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontSize (long *FontSize)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult;
	CMemDCHandle	lDC;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontSize) = 0;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontSize (FontSize);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		if	(
				(SUCCEEDED (lResult))
			&&	(lDC.Attach (CreateCompatibleDC (NULL)))
			)
		{
			CPoint	lFontSize (0, (*FontSize));
			CPoint	lOrigin (0, 0);

			LPtoDP (lDC, &lFontSize, 1);
			LPtoDP (lDC, &lOrigin, 1);
			lFontSize.y = abs(lOrigin.y - lFontSize.y);
			(*FontSize) = MulDiv (lFontSize.y, 72, GetDeviceCaps (lDC, LOGPIXELSY));
		}
		lDC.Close ();
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontSize (long FontSize)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult;
	CMemDCHandle	lDC;

	if	(lDC.Attach (CreateCompatibleDC (NULL)))
	{
		CPoint	lFontSize (0, 0);
		CPoint	lOrigin (0, 0);

		lFontSize.y = MulDiv (FontSize, GetDeviceCaps (lDC, LOGPIXELSY), 72);
		DPtoLP (lDC, &lFontSize, 1);
		DPtoLP (lDC, &lOrigin, 1);
		FontSize = -abs(lFontSize.y - lOrigin.y);
	}
	lDC.Close ();

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontSize (FontSize);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontSize"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__FontBold (VARIANT_BOOL *FontBold)
{
	return get_FontBold (FontBold);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontBold (VARIANT_BOOL *FontBold)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontBold"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontBold)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontBold (FontBold);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontBold"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontBold (VARIANT_BOOL FontBold)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontBold"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontBold (FontBold);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontBold"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__FontItalic (VARIANT_BOOL *FontItalic)
{
	return get_FontItalic (FontItalic);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontItalic (VARIANT_BOOL *FontItalic)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontItalic"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontItalic)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontItalic (FontItalic);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontItalic"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontItalic (VARIANT_BOOL FontItalic)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontItalic"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontItalic (FontItalic);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontItalic"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__FontStrikethru (VARIANT_BOOL *FontStrikethru)
{
	return get_FontStrikethru (FontStrikethru);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontStrikethru (VARIANT_BOOL *FontStrikethru)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontStrikethru"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontStrikethru (FontStrikethru);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontStrikethru"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontStrikethru (VARIANT_BOOL FontStrikethru)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontStrikethru"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontStrikethru (FontStrikethru);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontStrikethru"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__FontUnderline (VARIANT_BOOL *FontUnderline)
{
	return get_FontUnderline (FontUnderline);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontUnderline (VARIANT_BOOL *FontUnderline)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontUnderline"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FontUnderline (FontUnderline);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontUnderline"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontUnderline (VARIANT_BOOL FontUnderline)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontUnderline"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontUnderline (FontUnderline);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontUnderline"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_FontCharSet (short *FontCharSet)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontCharSet"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontCharSet)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_FontCharSet (FontCharSet);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_FontCharSet"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_FontCharSet (short FontCharSet)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontCharSet"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_FontCharSet (FontCharSet);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_FontCharSet"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_ForeColor (long *ForeColor)
{
	return get_TextColor (ForeColor);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_TextColor (long *TextColor)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_TextColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!TextColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_TextColor (TextColor);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_TextColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_TextColor (long TextColor)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_TextColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_TextColor (TextColor);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_TextColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__BackColor (long *BackColor)
{
	return get_BackColor (BackColor);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_BackColor (long *BackColor)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_BackColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!BackColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_BackColor (BackColor);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_BackColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_BackColor (long BackColor)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_BackColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_BackColor (BackColor);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_BackColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get__BorderColor (long *BorderColor)
{
	return get_BorderColor (BorderColor);
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_BorderColor (long *BorderColor)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_BorderColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!BorderColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_BorderColor (BorderColor);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_BorderColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_BorderColor (long BorderColor)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_BorderColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_BorderColor (BorderColor);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_BorderColor"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_Visible (VARIANT_BOOL *Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Visible (Visible);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_Visible (VARIANT_BOOL Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Visible (Visible);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_Visible"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlBalloon::get_Style (long *Style)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_Style"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Style)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			long	lCharsPerLine = 0;
			long	lNumLines = 0;

			lResult = mServerObject->get_Style (Style);
			if	(SUCCEEDED (lResult))
			{
				mServerObject->get_CharsPerLine (&lCharsPerLine);
				mServerObject->get_NumberOfLines (&lNumLines);
				(*Style) = MAKELONG (*Style, MAKEWORD (lCharsPerLine, lNumLines));
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::get_Style"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlBalloon::put_Style (long Style)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_Style [%8.8X]"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, Style);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Style ((long)(ULONG)LOWORD(Style));
			if	(
					(SUCCEEDED (lResult))
				&&	((Style & BalloonStyle_SizeToText) == 0)
				)
			{
				mServerObject->put_CharsPerLine ((long)(ULONG)LOBYTE(HIWORD(Style)));
				mServerObject->put_NumberOfLines ((long)(ULONG)HIBYTE(HIWORD(Style)));
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] [%p(%d)] DaCtlBalloon::put_Style"), SafeGetOwner()->SafeGetOwner(), SafeGetOwner()->SafeGetOwnerUsed(), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
