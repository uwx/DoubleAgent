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
#include "DaCtlRequest.h"
#include "DaControlObj.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Request"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Request"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogVerbose
#endif
/////////////////////////////////////////////////////////////////////////////

CDaCtlRequest::CDaCtlRequest ()
:	mOwner (NULL),
	mStatus (RequestPending),
	mCategory (DaRequestOther),
	mReqID (0),
	mResult (S_FALSE)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRequest::CDaCtlRequest (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X] Interface [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mReqID, StatusStr(), CategoryStr(), mResult, (IDaCtlRequest*)this);
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add (this);
#endif
}

CDaCtlRequest::~CDaCtlRequest ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRequest::~CDaCtlRequest (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X] Interface [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mReqID, StatusStr(), CategoryStr(), mResult, (IDaCtlRequest*)this);
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
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlRequest::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRequest::FinalRelease ReqID [%d] Status [%s] Category [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mReqID, StatusStr(), CategoryStr());
	}
#endif
	Terminate (false);
}

void CDaCtlRequest::Terminate (bool pFinal)
{
	if	(pFinal)
	{
		mOwner = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlRequest::SetOwner (CDaControlObj * pOwner, DaRequestCategory pCategory, long pReqID, HRESULT pResult)
{
	mOwner = pOwner;
	mCategory = pCategory;
	mReqID = pReqID;
	mResult = pResult;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRequest::SetOwner (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X] Interface [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mReqID, StatusStr(), CategoryStr(), mResult, (IDaCtlRequest*)this);
	}
#endif
	mOwner->RequestCreated (this);
}

CDaControlObj * CDaCtlRequest::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCtlRequest::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlRequest::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(
			(InlineIsEqualGUID (__uuidof(IDaCtlRequest), riid))
		||	(InlineIsEqualGUID (__uuidof(IAgentCtlRequest), riid))
		)
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CString CDaCtlRequest::StatusStr () const
{
	if	(this)
	{
		return RequestStatusStr (mStatus);
	}
	return CString();
}

CString CDaCtlRequest::CategoryStr () const
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
	if	(pCategory & DaRequestNotifyEnabled)
	{
		lCategoryStr += _T(" Enabled");
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

HRESULT STDMETHODCALLTYPE CDaCtlRequest::get_ID (long *ID)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_ID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!ID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ID) = mReqID;
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_ID"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlRequest::get_Status (long *Status)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Status [%d] [%d] [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mReqID, mStatus, RequestStatusStr(mStatus));
#endif
	HRESULT	lResult = S_OK;

	if	(!Status)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Status) = mStatus;
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Status"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlRequest::get_Description (BSTR *Description)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lDescription;

	if	(!Description)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(FAILED (mResult))
		{
			lDescription = DaErrorDescription (mResult);
			if	(lDescription.IsEmpty ())
			{
				lDescription = CErrorInfo (mResult).mDescription;
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlRequest::get_Number (long *Number)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Number"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Number)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Number) = (long)mResult;
	}

	PutControlError (lResult, __uuidof(IDaCtlRequest));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Number"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
