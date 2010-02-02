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
#include "DaCharactersObj.h"
#include "DaCharacterObj.h"
#include "DaCharactersEnum.h"
#include "ErrorInfo.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaCharactersObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaCharactersObj, "")
IMPLEMENT_OLETYPELIB(CDaCharactersObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaCharactersObj::CDaCharactersObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Characters")))
	{
		if	(bRegister)
		{
			RegisterApartmentThreaded ();
			RegisterTypeLib (gDaTypeLibId);
			RegisterDefCategory ();
			RegisterCategory (m_clsid, CATID_Programmable);
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaCharactersObj, CCmdTarget)
	INTERFACE_PART(CDaCharactersObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaCharactersObj, __uuidof(IDaCtlCharacters), Characters)
	INTERFACE_PART(CDaCharactersObj, __uuidof(IAgentCtlCharacters), Characters)
	INTERFACE_PART(CDaCharactersObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaCharactersObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaCharactersObj, Characters)
IMPLEMENT_DISPATCH_IID(CDaCharactersObj, __uuidof(IDaCtlCharacters))
IMPLEMENT_PROVIDECLASSINFO(CDaCharactersObj, __uuidof(CDaCharactersObj))

BEGIN_SUPPORTERRORINFO(CDaCharactersObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaCharactersObj, __uuidof(IDaCtlCharacters))
	IMPLEMENT_SUPPORTERRORINFO(CDaCharactersObj, __uuidof(IAgentCtlCharacters))
END_SUPPORTERRORINFO(CDaCharactersObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaCharactersObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaCharactersObj)
	DISP_PROPERTY_PARAM_ID(CDaCharactersObj, "Item", DISPID_VALUE, DspGetItem, DspSetItem, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CDaCharactersObj, "Character", DISPID_IAgentCtlCharacters_CharacterID, DspCharacter, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CDaCharactersObj, "Unload", DISPID_IAgentCtlCharacters_Unload, DspUnload, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaCharactersObj, "Load", DISPID_IAgentCtlCharacters_Load, DspLoad, VT_DISPATCH, VTS_BSTR VTS_VARIANT)
	DISP_DEFVALUE(CDaCharactersObj, "Item")
	DISP_FUNCTION_ID(CDaCharactersObj, "Enum", DISPID_NEWENUM, DspEnum, VT_UNKNOWN, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCharactersObj::CDaCharactersObj (CDaAgentCtl & pOwner)
:	mOwner (&pOwner)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::CDaCharactersObj (%d)"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCharactersObj::~CDaCharactersObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::~CDaCharactersObj (%d)"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
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
			LogMessage (LogNormal, _T("[%p(%u)] [%p(%u)] CDaCharactersObj Attached [%p] Owner [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mOwner->mCharacters.GetInterfacePtr(), mOwner);
		}
	}
	catch AnyExceptionSilent
#endif

	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCharactersObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::Terminate [%u] (%d)"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mCharacters.GetSize());
		}
#endif
#endif
		try
		{
			int					lNdx;
			CDaCharacterObj *	lCharacter;

			for	(lNdx = 0; lNdx <= mCharacters.GetUpperBound(); lNdx++)
			{
				try
				{
					if	(lCharacter = (CDaCharacterObj *)CCmdTarget::FromIDispatch (mCharacters [lNdx]))
					{
						lCharacter->Terminate (pFinal);
					}
				}
				catch AnyExceptionSilent

				if	(pFinal)
				{
					mCharacters [lNdx] = NULL;
				}
			}
		}
		catch AnyExceptionDebug

		if	(pFinal)
		{
			mOwner = NULL;
			mCharacters.RemoveAll ();
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaCharactersObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::OnFinalRelease [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mCharacters.GetSize());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaCharactersObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCharactersObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaCharactersObj::DspGetItem(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::DspGetItem"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCharacter *	lRet = NULL;
	HRESULT						lResult = m_xCharacters.get_Item (_bstr_t(CharacterID), &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharactersObj::DspSetItem(LPCTSTR CharacterID, LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::DspSetItem"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaCharactersObj::DspCharacter(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::DspCharacter"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCharacter *	lRet = NULL;
	HRESULT						lResult = m_xCharacters.Character (_bstr_t(CharacterID), &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharactersObj::DspUnload(LPCTSTR CharacterID)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::DspUnload"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacters.Unload (_bstr_t(CharacterID));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

LPDISPATCH CDaCharactersObj::DspLoad(LPCTSTR CharacterID, const VARIANT & LoadKey)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::DspLoad"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT					lResult = m_xCharacters.Load (_bstr_t(CharacterID), LoadKey, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPUNKNOWN CDaCharactersObj::DspEnum()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::DspEnum"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	LPUNKNOWN	lRet = NULL;
	HRESULT		lResult = m_xCharacters.get_Enum (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharactersObj::XCharacters::get_Item (BSTR CharacterID, IDaCtlCharacter **ppidItem)
{
	METHOD_PROLOGUE(CDaCharactersObj, Characters)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::get_Item"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lCharacterId (CharacterID);
	IDispatchPtr		lCharacterDispatch;
	IDaCtlCharacterPtr	lCharacter;

	if	(!ppidItem)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppidItem) = NULL;

		if	(pThis->mCharacters.Lookup (lCharacterId, lCharacterDispatch))
		{
			lCharacter = lCharacterDispatch;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::get_Item"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharactersObj::XCharacters::Character (BSTR CharacterID, IDaCtlCharacter **ppidItem)
{
	METHOD_PROLOGUE(CDaCharactersObj, Characters)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::Character"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	HRESULT	lResult = get_Item (CharacterID, ppidItem);

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::Character"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharactersObj::XCharacters::get_Enum (IUnknown **ppunkEnum)
{
	METHOD_PROLOGUE(CDaCharactersObj, Characters)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::get_Enum"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CDaCharactersEnum *	lEnum;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(lEnum = new CDaCharactersEnum (*pThis))
		{
			(*ppunkEnum) = lEnum->GetControllingUnknown ();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::get_Enum"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharactersObj::XCharacters::Unload (BSTR CharacterID)
{
	METHOD_PROLOGUE(CDaCharactersObj, Characters)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::Unload [%s]"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, DebugStr(CString(CharacterID)));
#endif
	HRESULT				lResult = S_OK;
	CString				lCharacterId (CharacterID);
	IDispatchPtr		lCharacterDispatch;
	CDaCharacterObj *	lCharacter = NULL;

	if	(pThis->mCharacters.Lookup (lCharacterId, lCharacterDispatch))
	{
		pThis->mCharacters.RemoveKey (lCharacterId);

		try
		{
			if	(lCharacter = (CDaCharacterObj *)CCmdTarget::FromIDispatch (lCharacterDispatch))
			{
				lResult = lCharacter->Terminate (false);
			}
		}
		catch AnyExceptionSilent

		SafeFreeSafePtr (lCharacterDispatch);
	}
	else
	{
		lResult = AGENTERR_CHARACTERINVALID;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::Unload [%s] [%p] [%d] [%d]"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, lCharacterId, lCharacter, (lCharacter?lCharacter->mServerCharID:0), (lCharacter?lCharacter->m_dwRef:-1));
	}
#endif
	SafeFreeSafePtr (lCharacterDispatch);
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharactersObj::XCharacters::Load (BSTR CharacterID, VARIANT LoadKey, IDaCtlRequest **ppidRequest)
{
	METHOD_PROLOGUE(CDaCharactersObj, Characters)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::Load"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	CString					lCharacterId (CharacterID);
	tPtr <CDaCharacterObj>	lCharacter;
	CDaCharacterObj *		lCharacterLoaded = NULL;
	IDispatchPtr			lDispatch;
	long					lReqID = 0;
	IDaCtlRequestPtr		lRequest;

	if	(ppidRequest)
	{
		(*ppidRequest) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(pThis->mCharacters.FindKey (lCharacterId) >= 0)
	{
		lResult = AGENTERR_CHARACTERALREADYLOADED;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mOwner->mServer)))
	{
		try
		{
			if	(lCharacter = new CDaCharacterObj (*pThis->mOwner))
			{
				lResult = pThis->mOwner->mServer->Load (LoadKey, &lCharacter->mServerCharID, &lReqID);
				if	(SUCCEEDED (lResult))
				{
					lResult = pThis->mOwner->mServer->GetCharacter (lCharacter->mServerCharID, &lDispatch);
				}
				if	(SUCCEEDED (lResult))
				{
					lCharacter->mServerObject = lDispatch;
				}
				if	(
						(lCharacter->mServerObject != NULL)
					&&	(lCharacter->mServerCharID > 0)
					)
				{
					lCharacter->m_dwRef = 0;
					pThis->mCharacters.SetAt (lCharacterId, lCharacter->GetIDispatch(FALSE));
					lCharacterLoaded = lCharacter.Detach ();
				}
				else
				if	(SUCCEEDED (lResult))
				{
					lResult = E_FAIL;
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mOwner->mServer);
	}

	if	(
			(lReqID)
		&&	(ppidRequest)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestLoad, lReqID, lResult));
		(*ppidRequest) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacters));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (MinLogLevel(_LOG_RESULTS,LogAlways), lResult, _T("[%p(%u)] [%p(%u)] CDaCharactersObj::XCharacters::Load [%s] [%p] [%d]"), &pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, lCharacterId, lCharacterLoaded, (lCharacterLoaded ? lCharacterLoaded->mServerCharID : 0));
	}
#endif
	return lResult;
}
