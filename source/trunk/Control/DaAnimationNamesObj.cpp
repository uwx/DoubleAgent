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
#include "DaAnimationNamesObj.h"
#include "DaCharacterObj.h"
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

IMPLEMENT_DYNAMIC(CDaAnimationNamesObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaAnimationNamesObj, "")
IMPLEMENT_OLETYPELIB(CDaAnimationNamesObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaAnimationNamesObj::CDaAnimationNamesObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Animation Names")))
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

BEGIN_INTERFACE_MAP(CDaAnimationNamesObj, CCmdTarget)
	INTERFACE_PART(CDaAnimationNamesObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaAnimationNamesObj, __uuidof(IDaCtlAnimationNames), AnimationNames)
	INTERFACE_PART(CDaAnimationNamesObj, __uuidof(IAgentCtlAnimationNames), AnimationNames)
	INTERFACE_PART(CDaAnimationNamesObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaAnimationNamesObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaAnimationNamesObj, AnimationNames)
IMPLEMENT_DISPATCH_IID(CDaAnimationNamesObj, __uuidof(IDaCtlAnimationNames))
IMPLEMENT_PROVIDECLASSINFO(CDaAnimationNamesObj, __uuidof(CDaAnimationNamesObj))

BEGIN_SUPPORTERRORINFO(CDaAnimationNamesObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaAnimationNamesObj, __uuidof(IDaCtlAnimationNames))
	IMPLEMENT_SUPPORTERRORINFO(CDaAnimationNamesObj, __uuidof(IAgentCtlAnimationNames))
END_SUPPORTERRORINFO(CDaAnimationNamesObj)

BEGIN_DISPATCH_MAP(CDaAnimationNamesObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaAnimationNamesObj)
	DISP_FUNCTION_ID(CDaAnimationNamesObj, "Enum", DISPID_NEWENUM, DspEnum, VT_UNKNOWN, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaAnimationNamesObj::CDaAnimationNamesObj (CDaCharacterObj & pOwner)
:	mOwner (&pOwner)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNamesObj::CDaAnimationNamesObj (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	if	(mOwner->mServerObject != NULL)
	{
		IUnknownPtr	lServerObject;
		mOwner->mServerObject->GetAnimationNames (&lServerObject);
		mServerObject = lServerObject;
	}

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaAnimationNamesObj::~CDaAnimationNamesObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNamesObj::~CDaAnimationNamesObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mAnimationNames != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%u)] CDaAnimationNamesObj Attached [%p] Owner [%p]"), this, m_dwRef, mOwner->mAnimationNames.GetInterfacePtr(), mOwner);
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

void CDaAnimationNamesObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNamesObj::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNamesObj::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaAnimationNamesObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaAnimationNamesObj::OnFinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaCharacterObj * CDaAnimationNamesObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaAnimationNamesObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPUNKNOWN CDaAnimationNamesObj::DspEnum()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaAnimationNamesObj::DspEnum"), this, m_dwRef);
#endif
	LPUNKNOWN	lRet = NULL;
	HRESULT		lResult = m_xAnimationNames.get_Enum (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaAnimationNamesObj::XAnimationNames::get_Enum (IUnknown **ppunkEnum)
{
	METHOD_PROLOGUE(CDaAnimationNamesObj, AnimationNames)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaAnimationNamesObj::XAnimationNames::get_Enum"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	if	(pThis->mServerObject == NULL)
	{
		lResult = AGENTCTLERROR_SERVERINIT;
	}
	else
	{
		pThis->mServerObject->AddRef ();
		(*ppunkEnum) = pThis->mServerObject;
	}

	PutControlError (lResult, __uuidof(IDaCtlAnimationNames));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaAnimationNamesObj::XAnimationNames::get_Enum"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
