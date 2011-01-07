/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "AfxODlgs.h"
#include "OleMessageFilterEx.h"
#include "Elapsed.h"
#include "Log.h"
#include "DebugStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC (COleMessageFilterEx, CCmdTarget)

COleMessageFilterEx::COleMessageFilterEx (DWORD pNotRespondingTimeout, DWORD pBusyTimeout, UINT pLogLevelDlg, UINT pLogLevelMsg)
:	mLogLevelDlg (pLogLevelDlg),
	mLogLevelMsg (pLogLevelMsg)
{
	SafeEnableNotResponding (pNotRespondingTimeout);
	SafeEnableBusy (pBusyTimeout);
}

COleMessageFilterEx::COleMessageFilterEx (const COleMessageFilter & pSource)
:	mLogLevelDlg (LogVerbose|LogTime),
	mLogLevelMsg (LogVerbose+1)
{
	operator= (pSource);
}

COleMessageFilterEx::COleMessageFilterEx (const CWinThread * pThread)
:	mLogLevelDlg (LogVerbose|LogTime),
	mLogLevelMsg (LogVerbose+1)
{
	if	(
			(pThread)
		&&	(pThread->m_pMessageFilter)
		)
	{
		operator= (*(pThread->m_pMessageFilter));
	}
}

COleMessageFilterEx & COleMessageFilterEx::operator= (const COleMessageFilter & pSource)
{
	m_nBusyCount			= static_cast <const COleMessageFilterEx &> (pSource).m_nBusyCount;
	m_bEnableBusy			= static_cast <const COleMessageFilterEx &> (pSource).m_bEnableBusy;
	m_bEnableNotResponding	= static_cast <const COleMessageFilterEx &> (pSource).m_bEnableNotResponding;
	m_bUnblocking			= static_cast <const COleMessageFilterEx &> (pSource).m_bUnblocking;
	m_nRetryReply			= static_cast <const COleMessageFilterEx &> (pSource).m_nRetryReply;
	m_nBusyReply			= static_cast <const COleMessageFilterEx &> (pSource).m_nBusyReply;
	m_nTimeout				= static_cast <const COleMessageFilterEx &> (pSource).m_nTimeout;

	return *this;
}

COleMessageFilterEx::~COleMessageFilterEx ()
{
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

DWORD COleMessageFilterEx::NotRespondingTimeout () const
{
	return (m_bEnableNotResponding > 1) ? 0 : (m_bEnableNotResponding) ? m_nTimeout : INFINITE;
}

COleMessageFilterEx * COleMessageFilterEx::SafeEnableNotResponding (DWORD pNotRespondingTimeout)
{
	if	(this)
	{
		try
		{
			if	(pNotRespondingTimeout == INFINITE)
			{
				m_nTimeout = ULONG_MAX;
				m_bEnableNotResponding = FALSE;
			}
			else
			if	(pNotRespondingTimeout == 0)
			{
				m_nTimeout = 8000;
				m_bEnableNotResponding = TRUE+1;
			}
			else
			{
				m_nTimeout = pNotRespondingTimeout;
				m_bEnableNotResponding = TRUE;
			}
		}
		catch AnyExceptionSilent
	}
	return this;
}

DWORD COleMessageFilterEx::BusyTimeout () const
{
	return (m_bEnableBusy > 1) ? 0 : (m_bEnableBusy) ? m_nRetryReply : INFINITE;
}

COleMessageFilterEx * COleMessageFilterEx::SafeEnableBusy (DWORD pBusyTimeout)
{
	if	(this)
	{
		try
		{
			if	(pBusyTimeout == INFINITE)
			{
				m_nRetryReply = 10000;
				m_bEnableBusy = FALSE;
			}
			else
			if	(pBusyTimeout == 0)
			{
				m_nRetryReply = 10000;
				m_bEnableBusy = TRUE+1;
			}
			else
			{
				m_nRetryReply = pBusyTimeout;
				m_bEnableBusy = TRUE;
			}
		}
		catch AnyExceptionSilent
	}
	return this;
}

//////////////////////////////////////////////////////////////////////

bool COleMessageFilterEx::SafeBeginBusyState (CWinThread * pThread)
{
	bool				lRet = false;
	CWinThread *		lThread;
	COleMessageFilter *	lMessageFilter;

	if	(
			(
				(lThread = pThread)
			||	(lThread = AfxGetThread ())
			)
		&&	(lMessageFilter = lThread->m_pMessageFilter)
		)
	{
		lMessageFilter->BeginBusyState ();
		lRet = true;
	}
	return lRet;
}

bool COleMessageFilterEx::SafeEndBusyState (CWinThread * pThread)
{
	bool				lRet = false;
	CWinThread *		lThread;
	COleMessageFilter *	lMessageFilter;

	if	(
			(
				(lThread = pThread)
			||	(lThread = AfxGetThread ())
			)
		&&	(lMessageFilter = lThread->m_pMessageFilter)
		)
	{
		lMessageFilter->EndBusyState ();
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

COleMessageFilterEx * COleMessageFilterEx::SetAppFilter (UINT pLogLevelDlg, UINT pLogLevelMsg)
{
	CWinThread *			lThread = AfxGetApp ();
	COleMessageFilterEx *	lMessageFilter;

	if	(lMessageFilter = new COleMessageFilterEx (lThread))
	{
		lMessageFilter->mLogLevelDlg = pLogLevelDlg|LogTimeMs;
		lMessageFilter->mLogLevelMsg = pLogLevelMsg|LogTimeMs;
		return SetThreadFilter (lThread, lMessageFilter);
	}
	return NULL;
}

COleMessageFilterEx * COleMessageFilterEx::SetThreadFilter (UINT pLogLevelDlg, UINT pLogLevelMsg)
{
	CWinThread *			lThread = AfxGetThread ();
	COleMessageFilterEx *	lMessageFilter;

	if	(lMessageFilter = new COleMessageFilterEx (lThread))
	{
		lMessageFilter->mLogLevelDlg = pLogLevelDlg|LogTimeMs;
		lMessageFilter->mLogLevelMsg = pLogLevelMsg|LogTimeMs;
		return SetThreadFilter (lThread, lMessageFilter);
	}
	return NULL;
}

COleMessageFilterEx * COleMessageFilterEx::SetThreadFilter (CWinThread * pThread, COleMessageFilterEx * pMessageFilter)
{
	COleMessageFilterEx *		lRet = NULL;
	tPtr <COleMessageFilterEx>	lMessageFilter (pMessageFilter);

	try
	{
		if	(
				(pThread)
			&&	(pMessageFilter)
			)
		{
			try
			{
				if	(pThread->m_pMessageFilter)
				{
					delete pThread->m_pMessageFilter;
					pThread->m_pMessageFilter = NULL;
				}
				if	(pThread->m_lpfnOleTermOrFreeLib == NULL)
				{
					pThread->m_lpfnOleTermOrFreeLib = AfxOleTermOrFreeLib;
				}

				lMessageFilter->Register ();
				pThread->m_pMessageFilter = lMessageFilter;
#ifdef	_DEBUG
				if	(LogIsActive ())
				{
					LogMessage (MinLogLevel(lMessageFilter->mLogLevelDlg,lMessageFilter->mLogLevelMsg), _T("SetThreadFilter [%u]"), pThread->m_nThreadID);
				}
#endif
				lRet = lMessageFilter.Detach ();
			}
			catch AnyExceptionSilent
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool COleMessageFilterEx::ResetAppFilter (bool pUseDefaultFilter)
{
	return ResetThreadFilter (AfxGetApp (), pUseDefaultFilter);
}

bool COleMessageFilterEx::ResetThreadFilter (bool pUseDefaultFilter)
{
	return ResetThreadFilter (AfxGetThread (), pUseDefaultFilter);
}

bool COleMessageFilterEx::ResetThreadFilter (CWinThread * pThread, bool pUseDefaultFilter)
{
	bool	lRet = false;

	try
	{
		if	(pThread)
		{
			COleMessageFilterEx *	lMessageFilter;

			if	(lMessageFilter = DYNAMIC_DOWNCAST (COleMessageFilterEx, pThread->m_pMessageFilter))
			{
#ifdef	_DEBUG
				if	(LogIsActive ())
				{
					LogMessage (MinLogLevel(lMessageFilter->mLogLevelDlg,lMessageFilter->mLogLevelMsg), _T("ResetThreadFilter [%u]"), pThread->m_nThreadID);
				}
#endif
				delete pThread->m_pMessageFilter;
				pThread->m_pMessageFilter = NULL;
				lRet = true;

				if	(pUseDefaultFilter)
				{
					if	(pThread->m_lpfnOleTermOrFreeLib == NULL)
					{
						pThread->m_lpfnOleTermOrFreeLib = AfxOleTermOrFreeLib;
					}
					if	(pThread->m_pMessageFilter = new COleMessageFilter)
					{
						pThread->m_pMessageFilter->Register ();
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

BOOL COleMessageFilterEx::OnMessagePending (const MSG* pMsg)
{
	if	(LogIsActive (mLogLevelMsg))
	{
		LogMessage (mLogLevelMsg, _T("COleMessageFilterEx::OnMessagePending [%p] [%8.8X] [%s]"), pMsg->hwnd, pMsg->message, WndClassName (pMsg->hwnd));
	}
	return COleMessageFilter::OnMessagePending (pMsg);
}

BOOL COleMessageFilterEx::IsSignificantMessage(MSG* pMsg)
{
	if	(LogIsActive (mLogLevelMsg))
	{
		LogMessage (mLogLevelMsg, _T("COleMessageFilterEx::IsSignificantMessage [%p] [%8.8X] [%s]"), pMsg->hwnd, pMsg->message, WndClassName (pMsg->hwnd));
	}
	return COleMessageFilter::IsSignificantMessage (pMsg);
}

//////////////////////////////////////////////////////////////////////

int COleMessageFilterEx::OnBusyDialog (HTASK htaskBusy)
{
	if	(LogIsActive (mLogLevelDlg))
	{
		LogMessage (mLogLevelDlg, _T("COleMessageFilterEx::OnBusyDialog [%p] [%u %u]"), htaskBusy, LOWORD(htaskBusy), HIWORD(htaskBusy));
	}
	if	(m_bEnableBusy > TRUE)
	{
		return 1;
	}
	return COleMessageFilter::OnBusyDialog (htaskBusy);
}

int COleMessageFilterEx::OnNotRespondingDialog (HTASK htaskBusy)
{
	if	(LogIsActive (mLogLevelDlg))
	{
		LogMessage (mLogLevelDlg, _T("COleMessageFilterEx::OnNotRespondingDialog [%p] [%u %u]"), htaskBusy, LOWORD(htaskBusy), HIWORD(htaskBusy));
	}
	if	(m_bEnableNotResponding > TRUE)
	{
		return 1;
	}
	return COleMessageFilter::OnNotRespondingDialog (htaskBusy);
}
