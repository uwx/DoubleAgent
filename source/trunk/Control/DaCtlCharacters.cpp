/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "GuidStr.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Characters"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Characters"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlCharacters::DaCtlCharacters ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::DaCtlCharacters (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::~DaCtlCharacters (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
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
			LogMessage (LogNormal|LogTime, _T("[%p(%d)] [%p(%d)] DaCtlCharacters Attached [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), mOwner->mCharacters.GetInterfacePtr());
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
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::FinalRelease (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), mCharacters.GetCount());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Terminate [%u] (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mCharacters.GetCount());
		}
#endif
#endif

		Disconnect (pFinal);
		mOwner = NULL;

#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

void DaCtlCharacters::Disconnect (bool pFinal)
{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Disconnect [%u] (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mCharacters.GetCount());
	}
#endif
#endif
	SafeFreeSafePtr (mCachedEnum);
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
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Disconnect [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, _AtlModule.GetLockCount());
	}
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlCharacters::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	mOwner = pOwner;
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::SetOwner (%d)"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
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
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
}

/////////////////////////////////////////////////////////////////////////////

DaCtlCharacter * DaCtlCharacters::FindCharacter (IDaCtlCharacter2 * pInterface)
{
	DaCtlCharacter *	lRet = NULL;

	if	(pInterface)
	{
		try
		{
			POSITION			lPos;
			IDaCtlCharacter2Ptr	lInterface;

			for	(lPos = mCharacters.GetStartPosition(); lPos;)
			{
				lInterface = mCharacters.GetNextValue (lPos);
				if	(lInterface == pInterface)
				{
					lRet = dynamic_cast <DaCtlCharacter *> (lInterface.GetInterfacePtr());
				}
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

HRESULT DaCtlCharacters::InitEnumVariant (CEnumVARIANTImpl * pEnum)
{
	HRESULT					lResult = S_FALSE;
	tArrayPtr <CComVariant>	lArray;
	POSITION				lPos;
	INT_PTR					lNdx;

	if	(lArray = new CComVariant [mCharacters.GetCount()+1])
	{
		for	(lPos = mCharacters.GetStartPosition(), lNdx = 0; lPos; lNdx++)
		{
			lArray [lNdx] = mCharacters.GetNextValue(lPos).GetInterfacePtr();
		}
		lResult = pEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mCharacters.GetCount()]), (LPDISPATCH)this, AtlFlagCopy);
	}
	return lResult;
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

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get_Count (long* Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get_Item (BSTR CharacterID, IDaCtlCharacter2 **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Item [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), CAtlString(CharacterID));
#endif
	HRESULT				lResult = S_OK;
	CAtlString			lCharacterId (CharacterID);
	IDispatchPtr		lCharacterDispatch;
	IDaCtlCharacter2Ptr	lCharacter;

	if	(!Item)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Item) = NULL;

		if	(mCharacters.Lookup (lCharacterId, lCharacterDispatch))
		{
			lCharacter = lCharacterDispatch.GetInterfacePtr();
			(*Item) = lCharacter.Detach();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Item [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), CAtlString(CharacterID));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get_Index (long Index, IDaCtlCharacter2 **Character)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Index"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get_Index"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::Character (BSTR CharacterID, IDaCtlCharacter2 **Character)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Character [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), CAtlString(CharacterID));
#endif

	HRESULT	lResult = get_Item (CharacterID, Character);

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Character [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), CAtlString(CharacterID));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacters::get__NewEnum (IUnknown **EnumVariant)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	tPtr <CComObject<CEnumVARIANT> >	lEnumVariant;
	IEnumVARIANTPtr						lInterface;

	if	(!EnumVariant)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*EnumVariant) = NULL;

		if	(
				(SUCCEEDED (lResult = CComObject<CEnumVARIANT>::CreateInstance (lEnumVariant.Free())))
			&&	(SUCCEEDED (lResult = InitEnumVariant (lEnumVariant)))
			)
		{
			lInterface = lEnumVariant.Detach();
			(*EnumVariant) = lInterface.Detach();
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlCharacters::Unload (BSTR CharacterID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Unload [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), DebugStr(CAtlString(CharacterID)));
#endif
	HRESULT				lResult = S_OK;
	CAtlString			lCharacterId (CharacterID);
	IDispatchPtr		lCharacterDispatch;
	DaCtlCharacter *	lCharacter = NULL;

	if	(mCharacters.Lookup (lCharacterId, lCharacterDispatch))
	{
		mCharacters.RemoveKey (lCharacterId);
		SafeFreeSafePtr (mCachedEnum);

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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Unload [%s] [%p] [%d] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), lCharacterId, lCharacter, (lCharacter?lCharacter->GetCharID():0), (lCharacter?lCharacter->m_dwRef:-1));
	}
#endif
	SafeFreeSafePtr (lCharacterDispatch);
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlCharacters::Load (BSTR CharacterID, VARIANT Provider, IDaCtlRequest **ppidRequest)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Load [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), CAtlString(CharacterID));
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
	SafeFreeSafePtr (mCachedEnum);

	if	(mCharacters.Lookup (lCharacterId))
	{
		lResult = AGENTERR_CHARACTERALREADYLOADED;
	}
	else
#ifndef	_DACORE_LOCAL
	if	(
			(!mOwner->mAutoConnect)
		&&	(mOwner->mServer == NULL)
		)
#endif
	{
		try
		{
			CAtlString			lFilePath;
			bool				lFilePathIsDefault;
			tPtr <CAgentFile>	lLoadFile;
			CAgentFile*		lAgentFile = NULL;

			if	(
					(SUCCEEDED (lResult = CDaCmnCharacter::GetLoadPath (Provider, lFilePath, mOwner->GetSearchPath(), &lFilePathIsDefault)))
				&&	(
						(mOwner->GetDefaultCharacter ())
					||	(
							(lFilePathIsDefault)
						&&	(mOwner->GetInstanceCharacter (-1))
						)
					)
				)
			{
				lResult = AGENTERR_DEFAULTCHARACTER;
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = CDaCmnCharacter::GetAgentFile (lFilePath, lLoadFile)))
				)
			{
				lAgentFile = mOwner->mAnchor.FindCachedFile (lLoadFile->Header.Guid);
				if	(!lAgentFile)
				{
					lAgentFile = lLoadFile;
				}
			}

			if	(
					(SUCCEEDED (lResult))
				&&	(SUCCEEDED (lResult = CComObject <DaCtlCharacter>::CreateInstance (lCharacter.Free())))
				&&	(SUCCEEDED (lResult = lCharacter->SetOwner (mOwner)))
				&&	(SUCCEEDED (lResult = lCharacter->mLocalObject->OpenFile (lAgentFile, lFilePathIsDefault)))
				&&	(SUCCEEDED (lResult = lCharacter->mLocalObject->RealizePopup (mOwner, mOwner->mLocalCharacterStyle, WS_EX_TOPMOST)))
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
#ifndef	_DACORE_LOCAL
	else
	if	(SUCCEEDED (lResult = _AtlModule.PreServerCall (mOwner->mServer)))
	{
		try
		{
			if	(
					(SUCCEEDED (lResult = CComObject <DaCtlCharacter>::CreateInstance (lCharacter.Free())))
				&&	(SUCCEEDED (lResult = lCharacter->SetOwner (mOwner)))
				&&	(SUCCEEDED (lResult = mOwner->mServer->Load (Provider, &lCharacter->mServerCharID, &lReqID)))
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
#endif
	if	(lReqID)
	{
		lRequest.Attach (mOwner->PutRequest (DaRequestLoad, lReqID, lResult)); // Create request even if not returned - load requests are handled specially.
		if	(ppidRequest)
		{
			(*ppidRequest) = lRequest.Detach();
		}
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlCharacters::Load [%s] [%s] [%p(%d)] [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), lCharacterId, VariantString(Provider), lCharacterLoaded, (lCharacterLoaded ? lCharacterLoaded->m_dwRef : 0), (lCharacterLoaded ? lCharacterLoaded->GetCharID() : -1));
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

/////////////////////////////////////////////////////////////////////////////
