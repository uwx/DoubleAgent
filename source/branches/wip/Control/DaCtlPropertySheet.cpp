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
#include "DaCtlPropertySheet.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlPropertySheet::DaCtlPropertySheet ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::DaCtlPropertySheet (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlPropertySheet::~DaCtlPropertySheet ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::~DaCtlPropertySheet (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mPropertySheet != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet Attached [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mPropertySheet.GetInterfacePtr());
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

void DaCtlPropertySheet::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::FinalRelease (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	Terminate (false);
}

void DaCtlPropertySheet::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::Terminate [%u] [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
		}
#endif
#endif
		if	(pFinal)
		{
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}

		mOwner = NULL;
		SafeFreeSafePtr (mLocalObject);
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::Terminate [%u] [%p] [%p] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlPropertySheet::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
		if	(mOwner->mServer)
		{
			mServerObject = mOwner->mServer;
			if	(!mServerObject)
			{
				lResult = E_FAIL;
			}
		}
		else
		{
			mLocalObject = new CDaCmnPropertySheet (NULL);
			if	(!mLocalObject)
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::SetOwner (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	return lResult;
}

DaControl * DaCtlPropertySheet::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlPropertySheet::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlPropertySheet::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlPropertySheet), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaCtlPropertySheet2), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlPropertySheet), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::put_Left (short Left)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::put_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Left (Left);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Left (Left);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get_Left (short *Left)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Left)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Left) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Left (Left);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Left (Left);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get__Left (short *Left)
{
	return get_Left (Left);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::put_Top (short Top)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::put_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Top (Top);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Top (Top);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get_Top (short *Top)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Top)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Top) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Top (Top);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Top (Top);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get__Top (short *Top)
{
	return get_Top (Top);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get_Height (short *Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Height)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Height) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Height (Height);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Height (Height);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get_Width (short *Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Width) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Width (Width);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Width (Width);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::put_Visible (VARIANT_BOOL Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::put_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mLocalObject)
	{
		try
		{
			if	(
					(mOwner)
				&&	(mOwner->IsWindow ())
				)
			{
				if	(Visible)
				{
					mLocalObject->SetModalParent (mOwner->m_hWnd);
					lResult = (mLocalObject->DoModal () == IDOK) ? S_OK : S_FALSE;
				}
				else
				if	(mLocalObject->IsWindow ())
				{
					lResult = mLocalObject->put_Visible (Visible);
				}
			}
			else
			{
				lResult = mLocalObject->put_Visible (Visible);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			AllowSetForegroundWindow (ASFW_ANY);
			lResult = mServerObject->put_Visible (Visible);
			AllowSetForegroundWindow (GetCurrentProcessId());
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::put_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get_Visible (VARIANT_BOOL *Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Visible (Visible);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Visible (Visible);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::put_Page (BSTR Page)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::put_Page"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Page (Page);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Page (Page);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::put_Page"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlPropertySheet::get_Page (BSTR *Page)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Page"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Page)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Page) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Page (Page);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Page (Page);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlPropertySheet2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlPropertySheet::get_Page"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
