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
#include "DaCtlCharacter.h"
#include "DaCtlBalloon.h"
#include "DaCtlCommands.h"
#include "DaCtlAnimationNames.h"
#include "DaCtlTTSEngine.h"
#include "DaCtlTTSEngines.h"
#include "DaCtlSREngine.h"
#include "DaCtlSREngines.h"
#include "ErrorInfo.h"
#include "Registry.h"
#include "StringArrayEx.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Character"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaCtlCharacter::DaCtlCharacter ()
:	mOwner (NULL),
	mServerCharID (0),
	mBalloon (NULL),
	mCommands (NULL),
	mAnimationNames (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::DaCtlCharacter (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlCharacter::~DaCtlCharacter ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::~DaCtlCharacter (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlCharacter::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::FinalRelease [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

HRESULT DaCtlCharacter::Terminate (bool pFinal)
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Terminate [%u] [%p(%u)] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject), mServerCharID);
		}
#endif
#endif
		try
		{
			DaCtlBalloon *			lBalloon;
			DaCtlCommands *			lCommands;
			DaCtlAnimationNames *	lAnimationNames;

			if	(
					(mBalloon != NULL)
				&&	(lBalloon = dynamic_cast <DaCtlBalloon *> (mBalloon.GetInterfacePtr()))
				)
			{
				lBalloon->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mBalloon = NULL;
			}

			if	(
					(mCommands != NULL)
				&&	(lCommands = dynamic_cast <DaCtlCommands *> (mCommands.GetInterfacePtr()))
				)
			{
				lCommands->Terminate (false);
			}
			if	(pFinal)
			{
				mCommands = NULL;
			}

			if	(
					(mAnimationNames != NULL)
				&&	(lAnimationNames = dynamic_cast <DaCtlAnimationNames *> (mAnimationNames.GetInterfacePtr()))
				)
			{
				lAnimationNames->Terminate (false);
			}
			if	(pFinal)
			{
				mAnimationNames = NULL;
			}

			if	(pFinal)
			{
				mServerObject.Detach ();
			}
			else
			{
				SafeFreeSafePtr (mServerObject);

				if	(
						(mServerCharID)
					&&	(SUCCEEDED (_AtlModule.PreServerCall (mOwner->mServer)))
					)
				{
					try
					{
						lResult = mOwner->mServer->Unload (mServerCharID);
					}
					catch AnyExceptionDebug
					_AtlModule.PostServerCall (mOwner->mServer);
				}
			}

			SafeFreeSafePtr (mLocalObject);
			mOwner = NULL;
			mServerCharID = 0;
		}
		catch AnyExceptionDebug

#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlCharacter::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
		if	(mOwner->mServer == NULL)
		{
			if	(mLocalObject = new CLocalWrapper (*this))
			{
				mLocalObject->Initialize (mOwner->mLocalNextCharID++, &mOwner->mLocalEventNotify, mOwner);
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
	}
	else
	{
		lResult = E_FAIL;
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::SetOwner (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
	return lResult;
}

DaControl * DaCtlCharacter::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlCharacter::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlCharacter::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlCharacter2), riid))
		||	(InlineIsEqualGUID (__uuidof(IDaCtlCharacter), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlCharacter), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlCharacterEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

DaCtlBalloon * DaCtlCharacter::GetBalloon ()
{
	CComObject <DaCtlBalloon> *	lRet = NULL;

	if	(mBalloon == NULL)
	{
		tPtr <CComObject <DaCtlBalloon> >	lBalloon;

		if	(mLocalObject)
		{
			try
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlBalloon>::CreateInstance (lBalloon.Free())))
					&&	(SUCCEEDED (lBalloon->SetOwner (this)))
					)
				{
					mBalloon = (LPDISPATCH) lBalloon.Detach();
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlBalloon>::CreateInstance (lBalloon.Free())))
					&&	(SUCCEEDED (lBalloon->SetOwner (this)))
					)
				{
					mBalloon = (LPDISPATCH) lBalloon.Detach();
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(mBalloon != NULL)
	{
		lRet = dynamic_cast <CComObject <DaCtlBalloon> *> (mBalloon.GetInterfacePtr());
	}
	return lRet;
}

DaCtlCommands * DaCtlCharacter::GetCommands ()
{
	CComObject <DaCtlCommands> *	lRet = NULL;

	if	(mCommands == NULL)
	{
		tPtr <CComObject <DaCtlCommands> >	lCommands;
		
		if	(mLocalObject)
		{
			try
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlCommands>::CreateInstance (lCommands.Free())))
					&&	(SUCCEEDED (lCommands->SetOwner (this)))
					)
				{
					mCommands = (LPDISPATCH) lCommands.Detach();
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlCommands>::CreateInstance (lCommands.Free())))
					&&	(SUCCEEDED (lCommands->SetOwner (this)))
					)
				{
					mCommands = (LPDISPATCH) lCommands.Detach();
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(mCommands != NULL)
	{
		lRet = dynamic_cast <CComObject <DaCtlCommands> *> (mCommands.GetInterfacePtr());
	}
	return lRet;
}

DaCtlAnimationNames * DaCtlCharacter::GetAnimationNames ()
{
	CComObject <DaCtlAnimationNames> *	lRet = NULL;

	if	(mAnimationNames == NULL)
	{
		tPtr <CComObject <DaCtlAnimationNames> >	lAnimationNames;

		if	(mLocalObject)
		{
			try
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlAnimationNames>::CreateInstance (lAnimationNames.Free())))
					&&	(SUCCEEDED (lAnimationNames->SetOwner (this)))
					)
				{
					mAnimationNames = (LPDISPATCH) lAnimationNames.Detach();
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlAnimationNames>::CreateInstance (lAnimationNames.Free())))
					&&	(SUCCEEDED (lAnimationNames->SetOwner (this)))
					)
				{
					mAnimationNames = (LPDISPATCH) lAnimationNames.Detach();
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(mAnimationNames != NULL)
	{
		lRet = dynamic_cast <CComObject <DaCtlAnimationNames> *> (mAnimationNames.GetInterfacePtr());
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CDaCmnCommands * DaCtlCharacter::CLocalWrapper::GetCommands (bool pCreateObject)
{
	CDaCmnCommands *	lRet = NULL;
	DaCtlCommands *		lCommands;
	
	if	(lCommands = mCharacter.GetCommands ())
	{
		lRet = lCommands->mLocalObject;
	}
	return lRet;
}

CDaCmnBalloon * DaCtlCharacter::CLocalWrapper::GetBalloon (bool pCreateObject)
{
	CDaCmnBalloon *	lRet = NULL;
	DaCtlBalloon *	lBalloon;
	
	if	(lBalloon = mCharacter.GetBalloon ())
	{
		lRet = lBalloon->mLocalObject;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Balloon (IDaCtlBalloon2 **Balloon)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Balloon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDaCtlBalloon2Ptr	lInterface;

	if	(!Balloon)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!GetBalloon ())
		{
			lResult = E_OUTOFMEMORY;
		}
		lInterface = mBalloon;
		(*Balloon) = lInterface.Detach ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Balloon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Commands (IDaCtlCommands2 **Commands)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Commands"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDaCtlCommands2Ptr	lInterface;

	if	(!Commands)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!GetCommands ())
		{
			lResult = E_OUTOFMEMORY;
		}
		lInterface = mCommands;
		(*Commands) = lInterface.Detach();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Commands"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Visible (VARIANT_BOOL *Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Left (short Left)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Left (short *Left)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Left)
	{
		lResult = E_POINTER;
	}
	else
	{
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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Top (short Top)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Top (short *Top)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Top)
	{
		lResult = E_POINTER;
	}
	else
	{
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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Height (short Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Height (Height);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Height (Height);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Height (short *Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Height)
	{
		lResult = E_POINTER;
	}
	else
	{
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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Width (short Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Width (Width);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Width (Width);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Width (short *Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Speed (long *Speed)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Speed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Speed)
	{
		lResult = E_POINTER;
	}
	else
	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->GetTTSSpeed (Speed);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetTTSSpeed (Speed);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Speed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Pitch (long *Pitch)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Pitch"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	short	lPitch = 0;

	if	(!Pitch)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->GetTTSPitch (&lPitch);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetTTSPitch (&lPitch);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Pitch) = lPitch;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Pitch"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_ActiveState (ActiveStateType *ActiveState)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_ActiveState"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult;
	ActiveStateType	lState = ActiveState_Inactive;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_ActiveState (&lState);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_ActiveState (&lState);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(ActiveState)
	{
		(*ActiveState) = (ActiveStateType)lState;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_ActiveState"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_ActiveState (ActiveStateType ActiveState)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_ActiveState"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_ActiveState (ActiveState);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_ActiveState (ActiveState);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_ActiveState"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IdleState (VARIANT_BOOL *IdleState)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_IdleState"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!IdleState)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IdleState) = VARIANT_FALSE;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_IdleState (IdleState);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_IdleState (IdleState);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IdleOn (VARIANT_BOOL *On)
{
	return get_IdleEnabled (On);
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IdleEnabled (VARIANT_BOOL *Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_IdleEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Style (&lStyle);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Style (&lStyle);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Enabled)
	{
		(*Enabled) = (lStyle & CharacterStyle_IdleEnabled) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_IdleEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_IdleOn (VARIANT_BOOL On)
{
	return put_IdleEnabled (On);
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_IdleEnabled (VARIANT_BOOL Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_IdleEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Style (&lStyle)))
			{
				if	(Enabled)
				{
					lStyle |= CharacterStyle_IdleEnabled;
				}
				else
				{
					lStyle &= ~CharacterStyle_IdleEnabled;
				}
				lResult = mLocalObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Style (&lStyle)))
			{
				if	(Enabled)
				{
					lStyle |= CharacterStyle_IdleEnabled;
				}
				else
				{
					lStyle &= ~CharacterStyle_IdleEnabled;
				}
				lResult = mServerObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_IdleEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Activate (VARIANT State, VARIANT_BOOL *Success)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Activate"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult;
	ActiveStateType	lState = ActiveState_InputActive;

	if	(!IsEmptyParm (&State))
	{
		try
		{
			lState = (ActiveStateType)(short)_variant_t(State);
		}
		catch AnyExceptionSilent
	}

	lResult = put_ActiveState (lState);
	if	(Success)
	{
		(*Success) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Activate"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Play (BSTR Animation, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Play [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, DebugStr(CAtlString(Animation)));
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Play (Animation, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Play (Animation, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestPlay, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Play"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Get (BSTR Type, BSTR Name, VARIANT Queue, IDaCtlRequest **Request)
{
	HRESULT				lResult = S_OK;
	PrepareResourceType	lResourceType;

	if	(_wcsicmp (Type, L"Animation") == 0)
	{
		lResourceType = PrepareResource_Animation;
	}
	else
	if	(_wcsicmp (Type, L"State") == 0)
	{
		lResourceType = PrepareResource_State;
	}
	else
	if	(_wcsicmp (Type, L"WaveFile") == 0)
	{
		lResourceType = PrepareResource_Wave;
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	if	(SUCCEEDED (lResult))
	{
		lResult = Prepare (lResourceType, Name, Queue, Request);
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Prepare (PrepareResourceType ResourceType, BSTR Name, VARIANT Queue, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Get"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	long				lPrepareQueue = TRUE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(
			(ResourceType != PrepareResource_Animation)
		&&	(ResourceType != PrepareResource_State)
		&&	(ResourceType != PrepareResource_Wave)
		)
	{
		lResult = E_INVALIDARG;
	}
	if	(!IsEmptyParm (&Queue))
	{
		try
		{
			lPrepareQueue = (long)(bool)_variant_t(Queue);
		}
		catch AnyExceptionSilent;
	}

	if	(SUCCEEDED (lResult))
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->Prepare (ResourceType, Name, lPrepareQueue, &lReqID);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->Prepare (ResourceType, Name, lPrepareQueue, &lReqID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestPrepare, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Get"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Stop (VARIANT Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Stop [%u] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, V_VT (&Request), V_DISPATCH (&Request));
#endif

	HRESULT				lResult;
	IDaCtlRequestPtr	lRequest;
	long				lReqID = 0;

	if	(V_VT (&Request) == VT_DISPATCH)
	{
		lRequest = V_DISPATCH (&Request);
	}
	if	(lRequest)
	{
		lResult = lRequest->get_ID (&lReqID);
	}
	else
	if	(
			(V_VT (&Request) != VT_EMPTY)
		&&	(V_VT (&Request) != VT_ERROR)
		)
	{
		lResult = E_INVALIDARG;
	}

	if	(SUCCEEDED (lResult))
	{
		if	(mLocalObject)
		{
			try
			{
				if	(lRequest == NULL)
				{
					lResult = mLocalObject->StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare);
				}
				else
				{
					lResult = mLocalObject->Stop (lReqID);
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(lRequest == NULL)
				{
					lResult = mServerObject->StopAll (StopAll_Play|StopAll_Move|StopAll_Speak|StopAll_QueuedPrepare);
				}
				else
				{
					lResult = mServerObject->Stop (lReqID);
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Stop"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Wait (IDaCtlRequest *WaitForRequest, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	{
		IDaCtlRequestPtr	lWaitForRequest ((LPDISPATCH)WaitForRequest);
		long				lWaitForReqID = -1;
		if	(lWaitForRequest)
		{
			LogComErr (_DEBUG_INTERFACE, lWaitForRequest->get_ID (&lWaitForReqID));
		}
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Wait [%p] [%p] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, WaitForRequest, lWaitForRequest.GetInterfacePtr(), lWaitForReqID);
	}
#endif
	HRESULT				lResult;
	long				lWaitForReqID = 0;
	IDaCtlRequestPtr	lWaitForRequest ((LPDISPATCH)WaitForRequest);
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(lWaitForRequest == NULL)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(SUCCEEDED (lResult = lWaitForRequest->get_ID (&lWaitForReqID)))
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->Wait (lWaitForReqID, &lReqID);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->Wait (lWaitForReqID, &lReqID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Wait"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Interrupt (IDaCtlRequest *InterruptRequest, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Interrupt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT				lResult;
	long				lInterruptReqID = 0;
	IDaCtlRequestPtr	lInterruptRequest ((LPDISPATCH)InterruptRequest);
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(lInterruptRequest == NULL)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(SUCCEEDED (lResult = lInterruptRequest->get_ID (&lInterruptReqID)))
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->Interrupt (lInterruptReqID, &lReqID);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->Interrupt (lInterruptReqID, &lReqID);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Interrupt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Speak (VARIANT Text, VARIANT Url, IDaCtlRequest **Request)
{
	ClearControlError ();

	HRESULT				lResult = S_OK;
	_bstr_t				lText;
	_bstr_t				lUrl;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Text))
	{
		try
		{
			lText = (_bstr_t)_variant_t(Text);
		}
		catch AnyExceptionSilent
	}

	if	(!IsEmptyParm (&Url))
	{
		try
		{
			lUrl = (_bstr_t)_variant_t(Url);
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Speak [%s] [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, DebugStr(CAtlString((BSTR)lText)), DebugStr(CAtlString((BSTR)lUrl)));
#endif

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Speak (lText, lUrl, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Speak (lText, lUrl, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestSpeak, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Speak"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::GestureAt (short x, short y, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::GestureAt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->GestureAt (x, y, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GestureAt (x, y, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::GestureAt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::MoveTo (short x, short y, VARIANT Speed, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::MoveTo"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult;
	long				lSpeed = 1000;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Speed))
	{
		try
		{
			lSpeed = (long)_variant_t(Speed);
		}
		catch AnyExceptionSilent
	}

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->MoveTo (x, y, lSpeed, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->MoveTo (x, y, lSpeed, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::MoveTo"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Hide (VARIANT Fast, IDaCtlRequest **Request)
{
	ClearControlError ();

	HRESULT				lResult;
	long				lFast = FALSE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Fast))
	{
		try
		{
			lFast = (long)(bool)_variant_t(Fast);
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Hide [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, lFast);
#endif

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Hide (lFast, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Hide (lFast, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestHide, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Hide"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Show (VARIANT Fast, IDaCtlRequest **Request)
{
	ClearControlError ();

	HRESULT				lResult;
	long				lFast = FALSE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Fast))
	{
		try
		{
			lFast = (long)(bool)_variant_t(Fast);
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Show [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, lFast);
#endif

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Show (lFast, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Show (lFast, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestShow, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Show"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::StopAll (VARIANT Types)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::StopAll"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult = S_OK;
	long			lStopTypes = 0;
	CAtlStringArray	lTypeNames;

	if	(IsEmptyParm (&Types))
	{
		lStopTypes = StopAll_Everything;
	}
	else
	{
		try
		{
			MakeStringArray (CAtlString ((BSTR)(_bstr_t)_variant_t(Types)), lTypeNames, _T(","));
		}
		catch AnyExceptionSilent

		if	(lTypeNames.GetCount() > 0)
		{
			for	(INT_PTR lNdx = 0; lNdx < (int)lTypeNames.GetCount(); lNdx++)
			{
				if	(lTypeNames [lNdx].CompareNoCase (_T("Get")) == 0)
				{
					lStopTypes |= StopAll_QueuedPrepare;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("NonQueuedGet")) == 0)
				{
					lStopTypes |= StopAll_ImmediatePrepare;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Move")) == 0)
				{
					lStopTypes |= StopAll_Move;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Play")) == 0)
				{
					lStopTypes |= StopAll_Play;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Speak")) == 0)
				{
					lStopTypes |= StopAll_Speak;
				}
				else
				{
					lResult = E_INVALIDARG;
					break;
				}
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	if	(SUCCEEDED (lResult))
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->StopAll (lStopTypes);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->StopAll (lStopTypes);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::StopAll"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_MoveCause (MoveCauseType *MoveCause)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_MoveCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!MoveCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_MoveCause (MoveCause);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_MoveCause (MoveCause);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_MoveCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_VisibilityCause (VisibilityCauseType *VisibilityCause)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_VisibilityCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!VisibilityCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_VisibilityCause (VisibilityCause);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_VisibilityCause (VisibilityCause);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_VisibilityCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_HasOtherClients (VARIANT_BOOL *HasOtherClients)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HasOtherClients"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT	lResult;
	long	lOtherClients = 0;

	if	(!HasOtherClients)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_OtherClientCount (&lOtherClients);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_OtherClientCount (&lOtherClients);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*HasOtherClients) = (lOtherClients > 0) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HasOtherClients"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_SoundEffectsOn (VARIANT_BOOL On)
{
	return put_SoundEffectsEnabled (On);
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_SoundEffectsEnabled (VARIANT_BOOL Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_SoundEffectsEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Style (&lStyle)))
			{
				if	(Enabled)
				{
					lStyle |= CharacterStyle_SoundEffects;
				}
				else
				{
					lStyle &= ~CharacterStyle_SoundEffects;
				}
				lResult = mLocalObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Style (&lStyle)))
			{
				if	(Enabled)
				{
					lStyle |= CharacterStyle_SoundEffects;
				}
				else
				{
					lStyle &= ~CharacterStyle_SoundEffects;
				}
				lResult = mServerObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_SoundEffectsEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_SoundEffectsOn (VARIANT_BOOL *On)
{
	return get_SoundEffectsEnabled (On);
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_SoundEffectsEnabled (VARIANT_BOOL *Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SoundEffectsEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Style (&lStyle);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Style (&lStyle);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Enabled)
	{
		(*Enabled) = (lStyle & CharacterStyle_SoundEffects) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SoundEffectsEnabled"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Name (BSTR *Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Name (Name);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Name (Name);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Name (BSTR Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Name (Name);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Name (Name);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Description (BSTR *Description)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Description (Description);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Description (Description);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Description (BSTR Description)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Description (Description);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Description (Description);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_ExtraData (BSTR *ExtraData)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_ExtraData"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_ExtraData (ExtraData);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_ExtraData (ExtraData);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_ExtraData"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::ShowPopupMenu (short x, short y, VARIANT_BOOL *Showed)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::ShowPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->ShowPopupMenu (x, y);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->ShowPopupMenu (x, y);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Showed)
	{
		(*Showed) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::ShowPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_AutoPopupMenu (VARIANT_BOOL Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Style (&lStyle)))
			{
				if	(Enabled)
				{
					lStyle |= CharacterStyle_AutoPopupMenu;
				}
				else
				{
					lStyle &= ~CharacterStyle_AutoPopupMenu;
				}
				lResult = mLocalObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Style (&lStyle)))
			{
				if	(Enabled)
				{
					lStyle |= CharacterStyle_AutoPopupMenu;
				}
				else
				{
					lStyle &= ~CharacterStyle_AutoPopupMenu;
				}
				lResult = mServerObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_AutoPopupMenu (VARIANT_BOOL *Enabled)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_Style (&lStyle);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_Style (&lStyle);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Enabled)
	{
		(*Enabled) = (lStyle & CharacterStyle_AutoPopupMenu) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_HelpModeOn (VARIANT_BOOL On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_HelpModeOn (VARIANT_BOOL *On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(On)
	{
		(*On) = VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_HelpContextID (long ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_HelpContextID (long *ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(ID)
	{
		(*ID) = 0;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Active (short *State)
{
	return get_ActiveState ((ActiveStateType*)State);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Listen (VARIANT_BOOL Listen, VARIANT_BOOL *StartedListening)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Listen"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Listen (Listen!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Listen (Listen!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(StartedListening)
	{
		(*StartedListening) = (lResult == S_OK) ? Listen : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Listen"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_LanguageID (long LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_LanguageID [%4.4X]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, LanguageID);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_LanguageID (LanguageID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_LanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_LanguageID (long *LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_LanguageID (LanguageID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_LanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_SRModeID (BSTR *EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_SRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_SRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_SRModeID (BSTR EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_SRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_SRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_TTSModeID (BSTR *EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_TTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_TTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_TTSModeID (BSTR EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_TTSModeID [%ls]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, EngineModeId);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_TTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_TTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_HelpFile (BSTR *File)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(File)
	{
		(*File) = NULL;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_HelpFile (BSTR File)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::put_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_GUID (BSTR *CharGUID)
{
	return get_UniqueID (CharGUID);
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_OriginalHeight (short *Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_OriginalHeight"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Height)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_OriginalHeight (Height);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_OriginalHeight (Height);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_OriginalHeight"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_OriginalWidth (short *Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_OriginalWidth"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_OriginalWidth (Width);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_OriginalWidth (Width);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_OriginalWidth"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::Think (BSTR Text, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Think"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->Think (Text, &lReqID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Think (Text, &lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestSpeak, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::Think"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Version (BSTR *Version)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Version"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT		lResult;
	short		lVerMaj = 0;
	short		lVerMin = 0;
	CAtlString	lVerStr;

	if	(!Version)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->GetVersion (&lVerMaj, &lVerMin);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetVersion (&lVerMaj, &lVerMin);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		lVerStr.Format (_T("%hd.%hd"), lVerMaj, lVerMin);
		(*Version) = lVerStr.AllocSysString ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Version"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_AnimationNames (IDaCtlAnimationNames **Names)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_AnimationNames"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	IDaCtlAnimationNamesPtr	lInterface;

	if	(!Names)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!GetAnimationNames ())
		{
			lResult = E_OUTOFMEMORY;
		}
		lInterface = mAnimationNames;
		(*Names) = lInterface.Detach();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_AnimationNames"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_SRStatus (long *Status)
{
	ListeningStatusType	lStatus = ListeningStatus_Error;
	HRESULT				lResult = get_ListeningStatus (&lStatus);

	if	(Status)
	{
		(*Status) = (long)lStatus;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_ListeningStatus (ListeningStatusType *ListeningStatus)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SRStatus"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_ListeningStatus (ListeningStatus);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_ListeningStatus (ListeningStatus);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SRStatus"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Style (long *Style)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Style)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Style) = 0;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Style (Style);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Style (Style);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_Style (long Style)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_Style (Style);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_Style (Style);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_HasIcon (VARIANT_BOOL *HasIcon)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_HasIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!HasIcon)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*HasIcon) = VARIANT_FALSE;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_HasIcon (HasIcon);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_HasIcon (HasIcon);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_HasIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::GenerateIcon (long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::GenerateIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->GenerateIcon (ClipLeft, ClipTop, ClipWidth, ClipHeight);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GenerateIcon (ClipLeft, ClipTop, ClipWidth, ClipHeight);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::GenerateIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IconShown (VARIANT_BOOL *IconShown)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(!IconShown)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconShown) = VARIANT_FALSE;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Style (&lStyle);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Style (&lStyle);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*IconShown) = (lStyle & CharacterStyle_IconShown) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_IconShown (VARIANT_BOOL IconShown)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lStyle = 0;

	if	(mLocalObject)
	{
		try
		{
			if	(SUCCEEDED (lResult = mLocalObject->get_Style (&lStyle)))
			{
				if	(IconShown)
				{
					lStyle |= CharacterStyle_IconShown;
				}
				else
				{
					lStyle &= ~CharacterStyle_IconShown;
				}
				lResult = mLocalObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->get_Style (&lStyle)))
			{
				if	(IconShown)
				{
					lStyle |= CharacterStyle_IconShown;
				}
				else
				{
					lStyle &= ~CharacterStyle_IconShown;
				}
				lResult = mServerObject->put_Style (lStyle);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IconVisible (VARIANT_BOOL *IconVisible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconVisible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconVisible)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconVisible) = VARIANT_FALSE;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_IconVisible (IconVisible);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_IconVisible (IconVisible);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconVisible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IconIdentity (BSTR *IconIdentity)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconIdentity)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconIdentity) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_IconIdentity (IconIdentity);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_IconIdentity (IconIdentity);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_IconIdentity (BSTR IconIdentity)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_IconIdentity (IconIdentity);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_IconIdentity (IconIdentity);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_IconTip (BSTR *IconTip)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconTip)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconTip) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_IconTip (IconTip);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_IconTip (IconTip);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::get_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::put_IconTip (BSTR IconTip)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->put_IconTip (IconTip);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->put_IconTip (IconTip);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] DaCtlCharacter::put_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_TTSEngine (VARIANT GetDefault, IDaCtlTTSEngine **TTSEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_TTSEngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	VARIANT_BOOL					lGetDefault = VARIANT_FALSE;
	IDaSvrTTSEnginePtr				lServerObject;
	CComObject <DaCtlTTSEngine> *	lObject = NULL;
	IDaCtlTTSEnginePtr				lInterface;

	if	(!TTSEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngine) = NULL;

		if	(!IsEmptyParm (&GetDefault))
		{
			try
			{
				lGetDefault = (VARIANT_BOOL)_variant_t(GetDefault);
			}
			catch AnyExceptionSilent
		}

		if	(mLocalObject)
		{
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_TTSEngine (lGetDefault, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngine>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (mOwner);
						lInterface = (LPDISPATCH) lObject;
						(*TTSEngine) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_TTSEngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::FindTTSEngines (VARIANT LanguageID, IDaCtlTTSEngines **TTSEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::FindTTSEngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	long							lLanguageID = 0;
	IDaSvrTTSEnginesPtr				lServerObject;
	CComObject <DaCtlTTSEngines> *	lObject = NULL;
	IDaCtlTTSEnginesPtr				lInterface;

	if	(!TTSEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSEngines) = NULL;

		if	(V_VT (&LanguageID) == VT_I4)
		{
			lLanguageID = V_I4 (&LanguageID);
		}
		else
		if	(V_VT (&LanguageID) == VT_I2)
		{
			lLanguageID = V_I2 (&LanguageID);
		}
		else
		if	(!IsEmptyParm (&LanguageID))
		{
			lResult = E_INVALIDARG;
		}

		if	(SUCCEEDED (lResult))
		{
			if	(mLocalObject)
			{
			}
			else
			if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
			{
				try
				{
					if	(SUCCEEDED (lResult = mServerObject->FindTTSEngines (lLanguageID, &lServerObject)))
					{
						if	(SUCCEEDED (lResult = CComObject <DaCtlTTSEngines>::CreateInstance (&lObject)))
						{
							lObject->mServerObject = lServerObject;
							lObject->SetOwner (mOwner);
							lInterface = (LPDISPATCH) lObject;
							(*TTSEngines) = lInterface.Detach();
						}
						else
						{
							lResult = E_OUTOFMEMORY;
						}
					}
				}
				catch AnyExceptionDebug
				_AtlModule.PostServerCall (mServerObject);
			}
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::FindTTSEngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_SREngine (VARIANT GetDefault, IDaCtlSREngine **SREngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SREngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	VARIANT_BOOL					lGetDefault = VARIANT_FALSE;
	IDaSvrSREnginePtr				lServerObject;
	CComObject <DaCtlSREngine> *	lObject = NULL;
	IDaCtlSREnginePtr				lInterface;

	if	(!SREngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngine) = NULL;

		if	(!IsEmptyParm (&GetDefault))
		{
			try
			{
				lGetDefault = (VARIANT_BOOL)_variant_t(GetDefault);
			}
			catch AnyExceptionSilent
		}

		if	(mLocalObject)
		{
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->get_SREngine (lGetDefault, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <DaCtlSREngine>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (mOwner);
						lInterface = (LPDISPATCH) lObject;
						(*SREngine) = lInterface.Detach();
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SREngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::FindSREngines (VARIANT LanguageID, IDaCtlSREngines **SREngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::FindSREngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT							lResult = S_OK;
	long							lLanguageID = 0;
	IDaSvrSREnginesPtr				lServerObject;
	CComObject <DaCtlSREngines> *	lObject = NULL;
	IDaCtlSREnginesPtr				lInterface;

	if	(!SREngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SREngines) = NULL;

		if	(V_VT (&LanguageID) == VT_I4)
		{
			lLanguageID = V_I4 (&LanguageID);
		}
		else
		if	(V_VT (&LanguageID) == VT_I2)
		{
			lLanguageID = V_I2 (&LanguageID);
		}
		else
		if	(!IsEmptyParm (&LanguageID))
		{
			lResult = E_INVALIDARG;
		}

		if	(SUCCEEDED (lResult))
		{
			if	(mLocalObject)
			{
			}
			else
			if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
			{
				try
				{
					if	(SUCCEEDED (lResult = mServerObject->FindSREngines (lLanguageID, &lServerObject)))
					{
						if	(SUCCEEDED (lResult = CComObject <DaCtlSREngines>::CreateInstance (&lObject)))
						{
							lObject->mServerObject = lServerObject;
							lObject->SetOwner (mOwner);
							lInterface = (LPDISPATCH) lObject;
							(*SREngines) = lInterface.Detach();
						}
						else
						{
							lResult = E_OUTOFMEMORY;
						}
					}
				}
				catch AnyExceptionDebug
				_AtlModule.PostServerCall (mServerObject);
			}
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::FindSREngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_UniqueID (BSTR *CharGUID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_UniqueID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(mLocalObject)
	{
		try
		{
			lResult = mLocalObject->get_UniqueID (CharGUID);
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->get_UniqueID (CharGUID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_UniqueID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_CharacterID (BSTR *CharacterID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_CharacterID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!CharacterID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharacterID) = NULL;

		if	(mOwner)
		{
			(*CharacterID) = mOwner->GetControlCharacterID (mServerCharID).AllocSysString();
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_FileName (BSTR *FileName)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_FileName"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!FileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FileName) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_FileName (FileName);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FileName (FileName);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_FilePath (BSTR *FilePath)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_FilePath"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!FilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePath) = NULL;

		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_FilePath (FilePath);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_FilePath (FilePath);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_Smoothed (VARIANT_BOOL *Smoothed)
{
	HRESULT	lResult;
	long	lStyle = 0;

	if	(!Smoothed)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Style (&lStyle);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Style (&lStyle);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*Smoothed) = ((lStyle & CharacterStyle_Smoothed) == CharacterStyle_Smoothed) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_Smoothed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacter::get_SmoothEdges (VARIANT_BOOL *SmoothEdges)
{
	HRESULT	lResult;
	long	lStyle = 0;

	if	(!SmoothEdges)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mLocalObject)
		{
			try
			{
				lResult = mLocalObject->get_Style (&lStyle);
			}
			catch AnyExceptionDebug
		}
		else
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->get_Style (&lStyle);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}

		(*SmoothEdges) = (lStyle & CharacterStyle_SmoothEdges) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacter::get_SmoothEdges"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
