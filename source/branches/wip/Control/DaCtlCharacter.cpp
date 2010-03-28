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
#include "DaControl.h"
#include "DaCtlCharacter.h"
#include "DaCtlBalloon.h"
#include "DaCtlCommands.h"
#include "DaCtlAnimationNames.h"
#include "DaCtlSpeechEngine.h"
#include "DaCtlSpeechEngines.h"
#include "DaCtlRecognitionEngine.h"
#include "DaCtlRecognitionEngines.h"
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

CDaCtlCharacter::CDaCtlCharacter ()
:	mOwner (NULL),
	mServerCharID (0),
	mBalloon (NULL),
	mCommands (NULL),
	mAnimationNames (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::CDaCtlCharacter (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

CDaCtlCharacter::~CDaCtlCharacter ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::~CDaCtlCharacter (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlCharacter::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::FinalRelease [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

HRESULT CDaCtlCharacter::Terminate (bool pFinal)
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Terminate [%u] [%p(%u)] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject), mServerCharID);
		}
#endif
#endif
		try
		{
			CDaCtlBalloon *			lBalloon;
			CDaCtlCommands *		lCommands;
			CDaCtlAnimationNames *	lAnimationNames;

			if	(
					(mBalloon != NULL)
				&&	(lBalloon = dynamic_cast <CDaCtlBalloon *> (mBalloon.GetInterfacePtr()))
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
				&&	(lCommands = dynamic_cast <CDaCtlCommands *> (mCommands.GetInterfacePtr()))
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
				&&	(lAnimationNames = dynamic_cast <CDaCtlAnimationNames *> (mAnimationNames.GetInterfacePtr()))
				)
			{
				lAnimationNames->Terminate (false);
				if	(pFinal)
				{
					mAnimationNames = NULL;
				}
			}

			if	(pFinal)
			{
				mOwner = NULL;
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

			mServerCharID = 0;
		}
		catch AnyExceptionDebug

#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlCharacter::SetOwner (CDaControlObj * pOwner)
{
	mOwner = pOwner;
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::SetOwner (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

CDaControlObj * CDaCtlCharacter::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCtlCharacter::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlCharacter::InterfaceSupportsErrorInfo(REFIID riid)
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

CDaCtlBalloon * CDaCtlCharacter::GetBalloon (IDaCtlBalloonPtr & pInterface)
{
	CComObject <CDaCtlBalloon> *	lBalloon = NULL;

	if	(mBalloon == NULL)
	{
		if	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (CComObject <CDaCtlBalloon>::CreateInstance (&lBalloon)))
				{
					lBalloon->SetOwner (this);
					mBalloon = (LPDISPATCH) lBalloon;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(mBalloon != NULL)
	{
		lBalloon = dynamic_cast <CComObject <CDaCtlBalloon> *> (mBalloon.GetInterfacePtr());
	}
	pInterface = mBalloon;

	return lBalloon;
}

CDaCtlCommands * CDaCtlCharacter::GetCommands (IDaCtlCommandsPtr & pInterface)
{
	CComObject <CDaCtlCommands> *	lCommands = NULL;

	if	(mCommands == NULL)
	{
		if	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (CComObject <CDaCtlCommands>::CreateInstance (&lCommands)))
				{
					lCommands->SetOwner (this);
					mCommands = (LPDISPATCH) lCommands;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(mCommands != NULL)
	{
		lCommands = dynamic_cast <CComObject <CDaCtlCommands> *> (mCommands.GetInterfacePtr());
	}
	pInterface = mCommands;

	return lCommands;
}

CDaCtlAnimationNames * CDaCtlCharacter::GetAnimationNames (IDaCtlAnimationNamesPtr & pInterface)
{
	CComObject <CDaCtlAnimationNames> *	lAnimationNames = NULL;

	if	(mAnimationNames == NULL)
	{
		if	(SUCCEEDED (_AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (CComObject <CDaCtlAnimationNames>::CreateInstance (&lAnimationNames)))
				{
					lAnimationNames->SetOwner (this);
					mAnimationNames = (LPDISPATCH) lAnimationNames;
				}
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}
	if	(mAnimationNames != NULL)
	{
		lAnimationNames = dynamic_cast <CComObject <CDaCtlAnimationNames> *> (mAnimationNames.GetInterfacePtr());
	}
	pInterface = mAnimationNames;

	return lAnimationNames;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Balloon (IDaCtlBalloon **ppidBalloon)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Balloon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDaCtlBalloonPtr	lInterface;

	if	(!ppidBalloon)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!GetBalloon (lInterface))
		{
			lResult = E_OUTOFMEMORY;
		}
		(*ppidBalloon) = lInterface.Detach ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Balloon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Commands (IDaCtlCommands **ppidCommands)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Commands"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDaCtlCommandsPtr	lInterface;

	if	(!ppidCommands)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!GetCommands (lInterface))
		{
			lResult = E_OUTOFMEMORY;
		}
		(*ppidCommands) = lInterface.Detach();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Commands"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Name (BSTR *Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetName (Name);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Description (BSTR *Description)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetDescription (Description);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Visible (VARIANT_BOOL *Visible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lVisible = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetVisible (&lVisible);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Visible)
	{
		(*Visible) = lVisible ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Visible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Left (short Left)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft;
	long	lTop;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->GetPosition (&lLeft, &lTop)))
			{
				lResult = mServerObject->SetPosition (Left, lTop);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Left (short *Left)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft = 0;
	long	lTop = 0;

	if	(!Left)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetPosition (&lLeft, &lTop);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Left) = (short)lLeft;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Left"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Top (short Top)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft;
	long	lTop;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->GetPosition (&lLeft, &lTop)))
			{
				lResult = mServerObject->SetPosition (lLeft, Top);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Top (short *Top)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft = 0;
	long	lTop = 0;

	if	(!Top)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetPosition (&lLeft, &lTop);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Top) = (short)lTop;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Top"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Height (short Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth;
	long	lHeight;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->GetSize (&lWidth, &lHeight)))
			{
				lResult = mServerObject->SetSize (lWidth, Height);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Height (short *Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Height)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Height) = (short)lHeight;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Height"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Width (short Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth;
	long	lHeight;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = mServerObject->GetSize (&lWidth, &lHeight)))
			{
				lResult = mServerObject->SetSize (Width, lHeight);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Width (short *Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Width) = (short)lWidth;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Width"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Speed (long *Speed)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Speed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Speed"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Pitch (long *Pitch)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Pitch"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	short	lPitch = 0;

	if	(!Pitch)
	{
		lResult = E_POINTER;
	}
	else
	{
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Pitch"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_IdleOn (VARIANT_BOOL On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_IdleOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetIdleOn (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_IdleOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_IdleOn (VARIANT_BOOL *On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_IdleOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	long	lIdleOn = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetIdleOn (&lIdleOn);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(On)
	{
		(*On) = lIdleOn ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_IdleOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Activate (VARIANT State, VARIANT_BOOL *Success)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Activate"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT	lResult;
	short	lState = 2;

	if	(!IsEmptyParm (&State))
	{
		try
		{
			lState = (short)_variant_t(State);
		}
		catch AnyExceptionSilent
	}

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->Activate (lState);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(Success)
	{
		(*Success) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Activate"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Play (BSTR Animation, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Play [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, DebugStr(CString(Animation)));
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Play"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Get (BSTR Type, BSTR Name, VARIANT Queue, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Get"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	long				lPrepareType;
	long				lPrepareQueue = TRUE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(_wcsicmp (Type, L"Animation") == 0)
	{
		lPrepareType = PrepareType_Animation;
	}
	else
	if	(_wcsicmp (Type, L"State") == 0)
	{
		lPrepareType = PrepareType_State;
	}
	else
	if	(_wcsicmp (Type, L"WaveFile") == 0)
	{
		lPrepareType = PrepareType_Wave;
	}
	else
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

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		)
	{
		try
		{
			lResult = mServerObject->Prepare (lPrepareType, Name, lPrepareQueue,	&lReqID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Get"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Stop (VARIANT Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Stop [%u] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, V_VT (&Request), V_DISPATCH (&Request));
#endif

	HRESULT				lResult;
	IDaCtlRequestPtr	lRequest;
	long				lReqID = 0;

	if	(V_VT (&Request) == VT_DISPATCH)
	{
		lRequest = V_DISPATCH (&Request);
	}

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(lRequest == NULL)
			{
				if	(
						(V_VT (&Request) == VT_EMPTY)
					||	(V_VT (&Request) == VT_ERROR)
					)
				{
					lResult = mServerObject->StopAll (StopType_Play|StopType_Move|StopType_Speak|StopType_QueuedPrepare);
				}
				else
				{
					lResult = E_INVALIDARG;
				}
			}
			else
			if	(SUCCEEDED (lResult = lRequest->get_ID (&lReqID)))
			{
				lResult = mServerObject->Stop (lReqID);
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Stop"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Wait (IDaCtlRequest *WaitForRequest, IDaCtlRequest **Request)
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
		LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Wait [%p] [%p] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, WaitForRequest, lWaitForRequest.GetInterfacePtr(), lWaitForReqID);
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
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = lWaitForRequest->get_ID (&lWaitForReqID)))
			{
				lResult = mServerObject->Wait (lWaitForReqID, &lReqID);
			}
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Wait"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Interrupt (IDaCtlRequest *InterruptRequest, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Interrupt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = lInterruptRequest->get_ID (&lInterruptReqID)))
			{
				lResult = mServerObject->Interrupt (lInterruptReqID, &lReqID);
			}
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Interrupt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Speak (VARIANT Text, VARIANT Url, IDaCtlRequest **Request)
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Speak [%s] [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, DebugStr(CString((BSTR)lText)), DebugStr(CString((BSTR)lUrl)));
#endif

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Speak"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::GestureAt (short x, short y, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::GestureAt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::GestureAt"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::MoveTo (short x, short y, VARIANT Speed, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::MoveTo"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::MoveTo"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Hide (VARIANT Fast, IDaCtlRequest **Request)
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Hide [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, lFast);
#endif

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Hide"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Show (VARIANT Fast, IDaCtlRequest **Request)
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
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Show [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, lFast);
#endif

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Show"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::StopAll (VARIANT Types)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::StopAll"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult = S_OK;
	long			lStopTypes = 0;
	CStringArray	lTypeNames;

	if	(IsEmptyParm (&Types))
	{
		lStopTypes = StopType_All;
	}
	else
	{
		try
		{
			MakeStringArray (CString ((BSTR)(_bstr_t)_variant_t(Types)), lTypeNames, _T(","));
		}
		catch AnyExceptionSilent

		if	(lTypeNames.GetSize () > 0)
		{
			for	(int lNdx = 0; lNdx <= lTypeNames.GetUpperBound(); lNdx++)
			{
				if	(lTypeNames [lNdx].CompareNoCase (_T("Get")) == 0)
				{
					lStopTypes |= StopType_QueuedPrepare;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("NonQueuedGet")) == 0)
				{
					lStopTypes |= StopType_ImmediatePrepate;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Move")) == 0)
				{
					lStopTypes |= StopType_Move;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Play")) == 0)
				{
					lStopTypes |= StopType_Play;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Speak")) == 0)
				{
					lStopTypes |= StopType_Speak;
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

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		)
	{
		try
		{
			lResult = mServerObject->StopAll (lStopTypes);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::StopAll"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_MoveCause (short *MoveCause)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_MoveCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lMoveCause = 0;

	if	(!MoveCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetMoveCause (&lMoveCause);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*MoveCause) = (short)lMoveCause;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_MoveCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_VisibilityCause (short *VisibilityCause)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_VisibilityCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lVisibilityCause = 0;

	if	(!VisibilityCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetVisibilityCause (&lVisibilityCause);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*VisibilityCause) = (short)lVisibilityCause;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_VisibilityCause"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_HasOtherClients (VARIANT_BOOL *HasOtherClients)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HasOtherClients"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT	lResult;
	long	lOtherClients = 0;

	if	(!HasOtherClients)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->HasOtherClients (&lOtherClients);
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HasOtherClients"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_SoundEffectsOn (VARIANT_BOOL On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_SoundEffectsOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetSoundEffectsOn (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_SoundEffectsOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_SoundEffectsOn (VARIANT_BOOL *On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SoundEffectsOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	long	lSoundOn = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetSoundEffectsOn (&lSoundOn);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(On)
	{
		(*On) = lSoundOn ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SoundEffectsOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Name (BSTR Name)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetName (Name);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Name"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Description (BSTR Description)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetDescription (Description);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_ExtraData (BSTR *ExtraData)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_ExtraData"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetExtraData (ExtraData);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_ExtraData"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::ShowPopupMenu (short x, short y, VARIANT_BOOL *Showed)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::ShowPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::ShowPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_AutoPopupMenu (VARIANT_BOOL On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetAutoPopupMenu (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_AutoPopupMenu (VARIANT_BOOL *On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lAutoPopupMenu = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetAutoPopupMenu (&lAutoPopupMenu);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(On)
	{
		(*On) = lAutoPopupMenu ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_AutoPopupMenu"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_HelpModeOn (VARIANT_BOOL On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetHelpModeOn (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_HelpModeOn (VARIANT_BOOL *On)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lHelpModeOn = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetHelpModeOn (&lHelpModeOn);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}
	if	(On)
	{
		(*On) = lHelpModeOn ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HelpModeOn"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_HelpContextID (long ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetHelpContextID (ID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_HelpContextID (long *ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetHelpContextID (ID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HelpContextID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Active (short *State)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Active"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetActive (State);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Active"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Listen (VARIANT_BOOL Listen, VARIANT_BOOL *StartedListening)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Listen"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Listen"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_LanguageID (long LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_LanguageID [%4.4X]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, LanguageID);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetLanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_LanguageID (long *LanguageID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetLanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_LanguageID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_SRModeID (BSTR *EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetSRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_SRModeID (BSTR EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetSRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_SRModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_TTSModeID (BSTR *EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetTTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_TTSModeID (BSTR EngineModeId)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_TTSModeID [%ls]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, EngineModeId);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetTTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_TTSModeID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_HelpFile (BSTR *File)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetHelpFileName (File);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_HelpFile (BSTR File)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->SetHelpFileName (File);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::put_HelpFile"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_GUID (BSTR *GUID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_GUID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetGUID (GUID);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_GUID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_OriginalHeight (short *Height)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_OriginalHeight"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Height)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetOriginalSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Height) = (short)lHeight;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_OriginalHeight"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_OriginalWidth (short *Width)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_OriginalWidth"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				lResult = mServerObject->GetOriginalSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
		(*Width) = (short)lWidth;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_OriginalWidth"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::Think (BSTR Text, IDaCtlRequest **Request)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Think"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	mOwner->CompleteRequests ();

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::Think"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Version (BSTR *Version)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Version"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;
	short	lVerMaj = 0;
	short	lVerMin = 0;
	CString	lVerStr;

	if	(!Version)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_Version"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_AnimationNames (IDaCtlAnimationNames **Names)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_AnimationNames"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	IDaCtlAnimationNamesPtr	lInterface;

	if	(!Names)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!GetAnimationNames (lInterface))
		{
			lResult = E_OUTOFMEMORY;
		}
		(*Names) = lInterface.Detach();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_AnimationNames"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_SRStatus (long *Status)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SRStatus"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->GetSRStatus (Status);
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SRStatus"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_Style (long *Style)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Style)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Style) = 0;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_Style (long Style)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_Style"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_HasIcon (VARIANT_BOOL *HasIcon)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_HasIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!HasIcon)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*HasIcon) = VARIANT_FALSE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				boolean	lHasIcon = FALSE;

				lResult = mServerObject->get_HasIcon (&lHasIcon);
				(*HasIcon) = lHasIcon?VARIANT_TRUE:VARIANT_FALSE;
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_HasIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::GenerateIcon (long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::GenerateIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::GenerateIcon"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_IconShown (VARIANT_BOOL *IconShown)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconShown)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconShown) = VARIANT_FALSE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				long	lStyle = 0;

				lResult = mServerObject->get_Style (&lStyle);
				(*IconShown) = (lStyle & CharacterStyle_IconShown) ? VARIANT_TRUE : VARIANT_FALSE;
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_IconShown (VARIANT_BOOL IconShown)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
	{
		try
		{
			long	lStyle = 0;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_IconShown"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_IconVisible (VARIANT_BOOL *IconVisible)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconVisible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconVisible)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconVisible) = VARIANT_FALSE;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				boolean	lIconVisible = FALSE;

				lResult = mServerObject->get_IconVisible (&lIconVisible);
				(*IconVisible) = lIconVisible?VARIANT_TRUE:VARIANT_FALSE;
			}
			catch AnyExceptionDebug
			_AtlModule.PostServerCall (mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter2));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconVisible"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_IconIdentity (BSTR *IconIdentity)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconIdentity)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconIdentity) = NULL;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_IconIdentity (BSTR IconIdentity)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_IconIdentity"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_IconTip (BSTR *IconTip)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

	if	(!IconTip)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*IconTip) = NULL;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::get_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::put_IconTip (BSTR IconTip)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult;

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCtlCharacter::put_IconTip"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_SpeechEngine (VARIANT_BOOL GetDefault, IDaCtlSpeechEngine **SpeechEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SpeechEngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	IDaSvrSpeechEnginePtr				lServerObject;
	CComObject <CDaCtlSpeechEngine> *	lObject = NULL;
	IDaCtlSpeechEnginePtr				lInterface;
	
	if	(!SpeechEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngine) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->GetSpeechEngine ((GetDefault!=VARIANT_FALSE), &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechEngine>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (mOwner);
						lInterface = (LPDISPATCH) lObject;
						(*SpeechEngine) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_SpeechEngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::FindSpeechEngines (VARIANT LanguageID, IDaCtlSpeechEngines **SpeechEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::FindSpeechEngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	long								lLanguageID = 0;
	IDaSvrSpeechEnginesPtr				lServerObject;
	CComObject <CDaCtlSpeechEngines> *	lObject = NULL;
	IDaCtlSpeechEnginesPtr				lInterface;
	
	if	(!SpeechEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeechEngines) = NULL;

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

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->FindSpeechEngines (lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlSpeechEngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (mOwner);
						lInterface = (LPDISPATCH) lObject;
						(*SpeechEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::FindSpeechEngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::get_RecognitionEngine (VARIANT_BOOL GetDefault, IDaCtlRecognitionEngine **RecognitionEngine)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_RecognitionEngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT									lResult = S_OK;
	IDaSvrRecognitionEnginePtr				lServerObject;
	CComObject <CDaCtlRecognitionEngine> *	lObject = NULL;
	IDaCtlRecognitionEnginePtr				lInterface;
	
	if	(!RecognitionEngine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngine) = NULL;

		if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->GetRecognitionEngine ((GetDefault!=VARIANT_FALSE), &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlRecognitionEngine>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (mOwner);
						lInterface = (LPDISPATCH) lObject;
						(*RecognitionEngine) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::get_RecognitionEngine"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlCharacter::FindRecognitionEngines (VARIANT LanguageID, IDaCtlRecognitionEngines **RecognitionEngines)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::FindRecognitionEngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT									lResult = S_OK;
	long									lLanguageID = 0;
	IDaSvrRecognitionEnginesPtr				lServerObject;
	CComObject <CDaCtlRecognitionEngines> *	lObject = NULL;
	IDaCtlRecognitionEnginesPtr				lInterface;
	
	if	(!RecognitionEngines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*RecognitionEngines) = NULL;

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

		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mServerObject)))
			)
		{
			try
			{
				if	(SUCCEEDED (lResult = mServerObject->FindRecognitionEngines (lLanguageID, &lServerObject)))
				{
					if	(SUCCEEDED (lResult = CComObject <CDaCtlRecognitionEngines>::CreateInstance (&lObject)))
					{
						lObject->mServerObject = lServerObject;
						lObject->SetOwner (mOwner);
						lInterface = (LPDISPATCH) lObject;
						(*RecognitionEngines) = lInterface.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlCharacter::FindRecognitionEngines"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
