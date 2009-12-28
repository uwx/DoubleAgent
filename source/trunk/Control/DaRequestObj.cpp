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
#include "DaRequestObj.h"
#include "DaAgentCtl.h"
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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Request"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Request"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogVerbose
#endif

#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaRequestObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaRequestObj, "")
IMPLEMENT_OLETYPELIB(CDaRequestObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaRequestObj::CDaRequestObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Request")))
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

BEGIN_INTERFACE_MAP(CDaRequestObj, CCmdTarget)
	INTERFACE_PART(CDaRequestObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaRequestObj, __uuidof(IDaCtlRequest), Request)
	INTERFACE_PART(CDaRequestObj, __uuidof(IAgentCtlRequest), Request)
	INTERFACE_PART(CDaRequestObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaRequestObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaRequestObj, Request)
IMPLEMENT_DISPATCH_IID(CDaRequestObj, __uuidof(IDaCtlRequest))
IMPLEMENT_PROVIDECLASSINFO(CDaRequestObj, __uuidof(CDaRequestObj))

BEGIN_SUPPORTERRORINFO(CDaRequestObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaRequestObj, __uuidof(IDaCtlRequest))
	IMPLEMENT_SUPPORTERRORINFO(CDaRequestObj, __uuidof(IAgentCtlRequest))
END_SUPPORTERRORINFO(CDaRequestObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaRequestObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaRequestObj)
	DISP_PROPERTY_EX_ID(CDaRequestObj, "ID", DISPID_VALUE, DspGetID, DspSetID, VT_I4)
	DISP_PROPERTY_EX_ID(CDaRequestObj, "Status", DISPID_IAgentCtlRequest_Status, DspGetStatus, DspSetStatus, VT_I4)
	DISP_PROPERTY_EX_ID(CDaRequestObj, "Description", DISPID_IAgentCtlRequest_Description, DspGetDescription, DspSetDescription, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaRequestObj, "Number", DISPID_IAgentCtlRequest_Number, DspGetNumber, DspSetNumber, VT_I4)
	DISP_DEFVALUE(CDaRequestObj, "ID")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaRequestObj::CDaRequestObj (CDaAgentCtl & pOwner, DaRequestCategory pCategory, long pReqID, HRESULT pResult)
:	mOwner (&pOwner),
	mCategory (pCategory),
	mReqID (pReqID),
	mStatus (RequestPending),
	mResult (pResult)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::CDaRequestObj (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mReqID, StatusStr(), CategoryStr(), mResult);
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
	mOwner->RequestCreated (this);
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaRequestObj::~CDaRequestObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::~CDaRequestObj (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mReqID, StatusStr(), CategoryStr(), mResult);
	}
#endif
	if	(mOwner)
	{
		try
		{
			mOwner->RequestDeleted (this);
		}
		catch AnyExceptionSilent
	}
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaRequestObj::Terminate (bool pFinal)
{
	if	(pFinal)
	{
		mOwner = NULL;
	}
}

void CDaRequestObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::OnFinalRelease ReqID [%d] Status [%s] Category [%s]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mReqID, StatusStr(), CategoryStr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaRequestObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaRequestObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString CDaRequestObj::StatusStr () const
{
	if	(this)
	{
		return RequestStatusStr (mStatus);
	}
	return CString();
}

CString CDaRequestObj::CategoryStr () const
{
	if	(this)
	{
		return RequestCategoryStr (mCategory);
	}
	return CString();
}

CString RequestStatusStr (long pStatus)
{
	CString	lStatusStr;

	switch (pStatus)
	{
		case RequestSuccess:		lStatusStr = _T("Success"); break;
		case RequestFailed:			lStatusStr = _T("Failed"); break;
		case RequestPending:		lStatusStr = _T("Pending"); break;
		case RequestInProgress:		lStatusStr = _T("InProgress"); break;
		case RequestInterrupted:	lStatusStr = _T("Interrupted"); break;
		default:					lStatusStr.Format (_T("%d"), pStatus);
	}
	return lStatusStr;
}

CString RequestCategoryStr (DaRequestCategory pCategory)
{
	CString	lCategoryStr;

	switch (pCategory & DaRequestCategoryMask)
	{
		case DaRequestLoad:		lCategoryStr = _T("Load"); break;
		case DaRequestShow:		lCategoryStr = _T("Show"); break;
		case DaRequestHide:		lCategoryStr = _T("Hide"); break;
		case DaRequestPlay:		lCategoryStr = _T("Play"); break;
		case DaRequestSpeak:	lCategoryStr = _T("Speak"); break;
		case DaRequestPrepare:	lCategoryStr = _T("Prepare"); break;
		case DaRequestOther:	lCategoryStr = _T("Other"); break;
		default:				lCategoryStr.Format (_T("%u"), (pCategory & DaRequestCategoryMask));
	}
	if	(pCategory & DaRequestNotifyStart)
	{
		lCategoryStr += _T(" Started");
	}
	if	(pCategory & DaRequestNotifyComplete)
	{
		lCategoryStr += _T(" Complete");
	}

	return lCategoryStr;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CDaRequestObj::DspGetID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspGetID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xRequest.get_ID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRequestObj::DspSetID(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspSetID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaRequestObj::DspGetStatus()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspGetStatus"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xRequest.get_Status (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRequestObj::DspSetStatus(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspSetStatus"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaRequestObj::DspGetDescription()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspGetDescription"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xRequest.get_Description (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRequestObj::DspSetDescription(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspSetDescription"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaRequestObj::DspGetNumber()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspGetNumber"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xRequest.get_Number (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRequestObj::DspSetNumber(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::DspSetNumber"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaRequestObj::XRequest::get_ID (long *ID)
{
	METHOD_PROLOGUE(CDaRequestObj, Request)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_ID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ID) = pThis->mReqID;
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_ID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRequestObj::XRequest::get_Status (long *Status)
{
	METHOD_PROLOGUE(CDaRequestObj, Request)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_Status"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Status)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Status) = pThis->mStatus;
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_Status"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRequestObj::XRequest::get_Description (BSTR *Description)
{
	METHOD_PROLOGUE(CDaRequestObj, Request)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_Description"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lDescription;

	if	(!Description)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(FAILED (pThis->mResult))
		{
			lDescription = DaErrorDescription (pThis->mResult);
			if	(lDescription.IsEmpty ())
			{
				lDescription = CErrorInfo (pThis->mResult).mDescription;
			}
			if	(lDescription.IsEmpty ())
			{
				lDescription = DaErrorDescription (E_FAIL);
			}
		}
		(*Description) = lDescription.AllocSysString ();
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_Description"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRequestObj::XRequest::get_Number (long *Number)
{
	METHOD_PROLOGUE(CDaRequestObj, Request)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_Number"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Number)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Number) = (long)pThis->mResult;
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaRequestObj::XRequestObj::get_Number"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
