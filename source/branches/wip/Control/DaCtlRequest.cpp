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
#include "DaCtlRequest.h"
#include "DaControl.h"
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

DaCtlRequest::DaCtlRequest ()
:	mOwner (NULL),
	mStatus (RequestStatus_Pending),
	mCategory (DaRequestOther),
	mReqID (0),
	mResult (S_FALSE)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlRequest::DaCtlRequest (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X] Interface [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mReqID, StatusStr(), CategoryStr(), mResult, (IDaCtlRequest*)this);
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlRequest::~DaCtlRequest ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlRequest::~DaCtlRequest (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X] Interface [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mReqID, StatusStr(), CategoryStr(), mResult, (IDaCtlRequest*)this);
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
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlRequest::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlRequest::FinalRelease ReqID [%d] Status [%s] Category [%s]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mReqID, StatusStr(), CategoryStr());
	}
#endif
	Terminate (false);
}

void DaCtlRequest::Terminate (bool pFinal)
{
	if	(pFinal)
	{
		mOwner = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlRequest::SetOwner (DaControl * pOwner, DaRequestCategory pCategory, long pReqID, HRESULT pResult)
{
	mOwner = pOwner;
	mCategory = pCategory;
	mReqID = pReqID;
	mResult = pResult;

#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlRequest::SetOwner (%d) ReqID [%d] Status [%s] Category [%s] Result [%8.8X] Interface [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mReqID, StatusStr(), CategoryStr(), mResult, (IDaCtlRequest*)this);
	}
#endif
	mOwner->RequestCreated (this);
	return S_OK;
}

DaControl * DaCtlRequest::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlRequest::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlRequest::InterfaceSupportsErrorInfo(REFIID riid)
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

CAtlString DaCtlRequest::StatusStr () const
{
	if	(this)
	{
		return RequestStatusStr (mStatus);
	}
	return CAtlString();
}

CAtlString DaCtlRequest::CategoryStr () const
{
	if	(this)
	{
		return RequestCategoryStr (mCategory);
	}
	return CAtlString();
}

CAtlString RequestStatusStr (long pStatus)
{
	CAtlString	lStatusStr;

	switch (pStatus)
	{
		case RequestStatus_Success:		lStatusStr = _T("Success"); break;
		case RequestStatus_Failed:		lStatusStr = _T("Failed"); break;
		case RequestStatus_Pending:		lStatusStr = _T("Pending"); break;
		case RequestStatus_InProgress:	lStatusStr = _T("InProgress"); break;
		case RequestStatus_Interrupted:	lStatusStr = _T("Interrupted"); break;
		default:						lStatusStr.Format (_T("%d"), pStatus);
	}
	return lStatusStr;
}

CAtlString RequestCategoryStr (DaRequestCategory pCategory)
{
	CAtlString	lCategoryStr;

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

HRESULT STDMETHODCALLTYPE DaCtlRequest::get_ID (long *ID)
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

HRESULT STDMETHODCALLTYPE DaCtlRequest::get_Status (long *Status)
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

HRESULT STDMETHODCALLTYPE DaCtlRequest::get_Description (BSTR *Description)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRequestObj::get_Description"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT		lResult = S_OK;
	CAtlString	lDescription;

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

HRESULT STDMETHODCALLTYPE DaCtlRequest::get_Number (long *Number)
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
