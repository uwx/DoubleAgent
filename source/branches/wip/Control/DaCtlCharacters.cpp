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
#include "DaCtlCharacters.h"
#include "DaCtlCharacter.h"
#include "ErrorInfo.h"
#include "Registry.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlCharacters::DaCtlCharacters ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::DaCtlCharacters (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlCharacters::~DaCtlCharacters ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::~DaCtlCharacters (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mCharacters != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%d)] [%p(%d)] DaCtlCharacters Attached [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mOwner->mCharacters.GetInterfacePtr());
		}
	}
	catch AnyExceptionSilent
#endif

	Terminate (true);
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlCharacters::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::FinalRelease (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mCharacters.GetCount());
	}
#endif
	Terminate (false);
}

void DaCtlCharacters::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Terminate [%u] (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mCharacters.GetCount());
		}
#endif
#endif
		try
		{
			POSITION			lPos;
			DaCtlCharacter *	lCharacter;

			for	(lPos = mCharacters.GetStartPosition(); lPos;)
			{
				try
				{
					if	(lCharacter = dynamic_cast <DaCtlCharacter *> (mCharacters.GetValueAt (lPos).GetInterfacePtr()))
					{
						lCharacter->Terminate (pFinal);
					}
				}
				catch AnyExceptionSilent

				if	(pFinal)
				{
					mCharacters.SetValueAt (lPos, NULL);
				}
				mCharacters.GetNext (lPos);
			}
		}
		catch AnyExceptionDebug

		if	(pFinal)
		{
			mCharacters.RemoveAll ();
		}
		mOwner = NULL;
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlCharacters::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	mOwner = pOwner;
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::SetOwner (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	return lResult;
}

DaControl * DaCtlCharacters::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlCharacters::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlCharacters::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlCharacters), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlCharacters), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlAudioObjectEx), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get_Count (long * Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mCharacters.GetCount();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get_Item (BSTR CharacterID, IDaCtlCharacter2 **ppidItem)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CAtlString			lCharacterId (CharacterID);
	IDispatchPtr		lCharacterDispatch;
	IDaCtlCharacter2Ptr	lCharacter;

	if	(!ppidItem)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppidItem) = NULL;

		if	(mCharacters.Lookup (lCharacterId, lCharacterDispatch))
		{
			lCharacter = lCharacterDispatch.GetInterfacePtr();
			(*ppidItem) = lCharacter.Detach();
		}
		else
		{
			lResult = AGENTERR_CHARACTERINVALID;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get_Index (long Index, IDaCtlCharacter2 **Character)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Index"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDispatchPtr		lCharacterDispatch;
	IDaCtlCharacter2Ptr	lCharacter;

	if	(!Character)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Character) = NULL;

		if	(
				(Index >= 0)
			&&	(Index < (long)mCharacters.GetCount())
			)
		{
			POSITION	lPos;

			for	(lPos = mCharacters.GetStartPosition(); lPos;)
			{
				if	(Index-- <= 0)
				{
					lCharacterDispatch = mCharacters.GetValueAt (lPos);
					break;
				}
				mCharacters.GetNextValue (lPos);
			}
		}
		if	(lCharacterDispatch == NULL)
		{
			lResult = E_INVALIDARG;
		}
		else
		{
			lCharacter = lCharacterDispatch.GetInterfacePtr();
			(*Character) = lCharacter.Detach();
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Index"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::Character (BSTR CharacterID, IDaCtlCharacter2 **ppidItem)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Character"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif

	HRESULT	lResult = get_Item (CharacterID, ppidItem);

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Character"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <CComVariant>	lArray;
	IEnumVARIANTPtr			lInterface;
	POSITION				lPos;
	INT_PTR					lNdx;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(
				(lEnum = new CComObject <CEnumVARIANT>)
			&&	(lArray = new CComVariant [mCharacters.GetCount()+1])
			)
		{
			for	(lPos = mCharacters.GetStartPosition(), lNdx = 0; lPos; lNdx++)
			{
				lArray [lNdx] = mCharacters.GetNextValue(lPos).GetInterfacePtr();
			}
			if	(SUCCEEDED (lResult = lEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mCharacters.GetCount()]), (LPDISPATCH)this, AtlFlagCopy)))
			{
				lInterface = lEnum.Detach ();
				(*ppunkEnum) = lInterface.Detach ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacters::Unload (BSTR CharacterID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Unload [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, DebugStr(CAtlString(CharacterID)));
#endif
	HRESULT				lResult = S_OK;
	CAtlString			lCharacterId (CharacterID);
	IDispatchPtr		lCharacterDispatch;
	DaCtlCharacter *	lCharacter = NULL;

	if	(mCharacters.Lookup (lCharacterId, lCharacterDispatch))
	{
		mCharacters.RemoveKey (lCharacterId);

		try
		{
			if	(lCharacter = dynamic_cast <DaCtlCharacter *> (lCharacterDispatch.GetInterfacePtr()))
			{
				lResult = lCharacter->Terminate (false);
			}
		}
		catch AnyExceptionSilent

		SafeFreeSafePtr (lCharacterDispatch);

		if	(lCharacter)
		{
			mOwner->CharacterUnloaded ((int)mCharacters.GetCount(), lCharacter);
		}
	}
	else
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Unload [%s] [%p] [%d] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, lCharacterId, lCharacter, (lCharacter?lCharacter->GetCharID():0), (lCharacter?lCharacter->m_dwRef:-1));
	}
#endif
	SafeFreeSafePtr (lCharacterDispatch);
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::Load (BSTR CharacterID, VARIANT LoadKey, IDaCtlRequest **ppidRequest)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Load"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT								lResult = S_OK;
	CAtlString							lCharacterId (CharacterID);
	tPtr <CComObject <DaCtlCharacter> >	lCharacter;
	DaCtlCharacter *					lCharacterLoaded = NULL;
	long								lReqID = 0;
	IDaCtlRequestPtr					lRequest;

	if	(ppidRequest)
	{
		(*ppidRequest) = NULL;
	}
	mOwner->CompleteRequests ();

	if	(mCharacters.Lookup (lCharacterId))
	{
		lResult = AGENTERR_CHARACTERALREADYLOADED;
	}
	else
	if	(
			(!mOwner->mAutoConnect)
		&&	(mOwner->mServer == NULL)
		)
	{
		try
		{
			CString				lFilePath;
			tPtr <CAgentFile>	lLoadFile;
			CAgentFile *		lAgentFile = NULL;

			if	(
					(SUCCEEDED (lResult = CDaCmnCharacter::GetLoadPath (LoadKey, lFilePath)))
				&&	(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (lFilePath, lLoadFile)))
				)
			{
				lAgentFile = mOwner->FindCachedFile (lLoadFile->GetGuid());
				if	(!lAgentFile)
				{
					lAgentFile = lLoadFile;
				}
			}

			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCharacter>::CreateInstance (lCharacter.Free())))
				&&	(SUCCEEDED (lResult = lCharacter->SetOwner (mOwner)))
				&&	(SUCCEEDED (lResult = lCharacter->mLocalObject->OpenFile (lAgentFile, mOwner->mLocalCharacterStyle)))
				)
			{
				if	(lLoadFile == lAgentFile)
				{
					lLoadFile.Detach ();
				}

				mCharacters.SetAt (lCharacterId, (LPDISPATCH) lCharacter);
				lCharacterLoaded = lCharacter.Detach ();
			}
		}
		catch AnyExceptionDebug
	}
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mOwner->mServer)))
	{
		try
		{
			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCharacter>::CreateInstance (lCharacter.Free())))
				&&	(SUCCEEDED (lResult = lCharacter->SetOwner (mOwner)))
				&&	(SUCCEEDED (lResult = mOwner->mServer->Load (LoadKey, &lCharacter->mServerCharID, &lReqID)))
				&&	(SUCCEEDED (lResult = mOwner->mServer->get_Character (lCharacter->mServerCharID, &lCharacter->mServerObject)))
				)
			{
				if	(
						(lCharacter->mServerObject != NULL)
					&&	(lCharacter->GetCharID () > 0)
					)
				{
					mCharacters.SetAt (lCharacterId, (LPDISPATCH) lCharacter);
					lCharacterLoaded = lCharacter.Detach ();
				}
				else
				{
					lResult = E_FAIL;
				}
			}
		}
		catch AnyExceptionDebug
		_AtlModule.PostServerCall (mOwner->mServer);
	}

	if	(
			(lReqID)
		&&	(ppidRequest)
		)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestLoad, lReqID, lResult));
		(*ppidRequest) = lRequest.Detach();
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(lCharacterLoaded)
		)
	{
		mOwner->CharacterLoaded ((int)mCharacters.GetCount(), lCharacterLoaded);
	}
	
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Load [%s] [%s] [%p(%d)] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, lCharacterId, VariantString(LoadKey), lCharacterLoaded, (lCharacterLoaded ? lCharacterLoaded->m_dwRef : 0), (lCharacterLoaded ? lCharacterLoaded->mServerCharID : 0));
	}
#endif
	if	(
			(FAILED (lResult))
		&&	(!mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
	return lResult;
}
