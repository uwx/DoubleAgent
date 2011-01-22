/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include "ServerLifetime.h"
#include "DaServerApp.h"
#include "GuidStr.h"
#include "Registry.h"
#include "Elapsed.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_LOG_LIFETIME	(GetProfileDebugInt(_T("LogInstance_Handler"),LogDetails,true)&0xFFFF|LogTime)
#endif

#ifndef	_LOG_LIFETIME
#define	_LOG_LIFETIME	LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

CSvrObjLifetime::CSvrObjLifetime ()
{
}

CSvrObjLifetime::~CSvrObjLifetime ()
{
}

/////////////////////////////////////////////////////////////////////////////

bool CSvrObjLifetime::VerifyClientLifetime ()
{
	if	(_VerifyClientLifetime())
	{
		return true;
	}
#ifdef	_LOG_LIFETIME
	if	(
			(mClientMutex)
		&&	(LogIsActive (_LOG_LIFETIME))
		)
	{
		LogMessage (_LOG_LIFETIME, _T("[%p] Verify %s ClientMutex [%p] [%s] ABANDONED"), this, AtlTypeName(this), mClientMutex->m_h, mClientMutexName);
	}
#endif
	return false;
}

bool CSvrObjLifetime::_VerifyClientLifetime ()
{
	if	(
			(mClientMutex)
		&&	(mClientMutex->m_h)
		&&	(mClientMutex->Lock (0))
		)
	{
		mClientMutex->Unlock ();
		return false;
	}
	return true;
}

void CSvrObjLifetime::OnClientEnded ()
{
#ifdef	_DEBUG
	LogMessage (LogIfActive|LogTime, _T("[%p] CSvrObjLifetime::OnClientEnded"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CSvrObjLifetime::ManageObjectLifetime (CComObjectRootBase * pObject, LPCTSTR pClientMutexName)
{
	bool	lRet = false;

	if	(mClientMutexName.IsEmpty ())
	{
		try
		{
			mClientMutexName = pClientMutexName;

			if	(mClientMutexName.IsEmpty ())
			{
				GUID	lThreadId;

				if	(SUCCEEDED (LogComErr (LogNormal|LogTime, CoGetCurrentLogicalThreadId (&lThreadId))))
				{
					mClientMutexName.Format (_T("Local\\%s"), (CString)CGuidStr (lThreadId));
				}
			}
			if	(!mClientMutexName.IsEmpty ())
			{
				try
				{
					mClientMutex = new CAutoMutex (NULL, FALSE, mClientMutexName);
				}
				catch AnyExceptionDebug

				if	(
						(!mClientMutex)
					||	(!mClientMutex->m_h)
					)
				{
					if	(LogIsActive ())
					{
						LogWinErr (LogAlways, GetLastError(), _T("ClientMutex [%s]"), mClientMutexName);
					}
					mClientMutex = NULL;
				}
				else
				if	(mClientMutex->Lock (0))
				{
					mClientMutex->Unlock ();
#ifdef	_LOG_LIFETIME
					if	(LogIsActive (_LOG_LIFETIME))
					{
						LogMessage (_LOG_LIFETIME, _T("[%p(%d)] Manage %s ClientMutex [%p] [%s] UNLOCKED Abandoned [%u]"), this, pObject->m_dwRef, AtlTypeName(this), mClientMutex->m_h, mClientMutexName, mClientMutex->IsAbandoned());
					}
#endif
					mClientMutex = NULL;
				}
				else
				{
					lRet = true;
#ifdef	_LOG_LIFETIME
					if	(LogIsActive (_LOG_LIFETIME))
					{
						LogMessage (_LOG_LIFETIME, _T("[%p(%d)] Manage %s ClientMutex [%p] [%s]"), this, pObject->m_dwRef, AtlTypeName(this), mClientMutex->m_h, mClientMutexName);
					}
#endif
				}
			}
		}
		catch AnyExceptionDebug
	}
	if	(lRet)
	{
		_AtlModule.ManageObjectLifetime (this);
	}
	return lRet;
}

void CSvrObjLifetime::UnmanageObjectLifetime (CComObjectRootBase * pObject)
{
	if	(mClientMutex)
	{
		try
		{
#ifdef	_LOG_LIFETIME
			if	(LogIsActive (_LOG_LIFETIME))
			{
				LogMessage (_LOG_LIFETIME, _T("[%p(%d)] Unmanage %s ClientMutex [%p] [%s]"), this, max(pObject->m_dwRef,0), AtlTypeName(this), mClientMutex->m_h, mClientMutexName);
			}
#endif
			_AtlModule.UnmanageObjectLifetime (this);
		}
		catch AnyExceptionSilent
	}

	mClientMutexName.Empty ();
	SafeFreeSafePtr (mClientMutex);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CServerLifetime::CServerLifetime ()
:	mObjectWasAbandoned (false),
	mLastVerifyTime (GetTickCount ()),
#ifdef	_DEBUG
	mVerifyInterval (3000)
#else
	mVerifyInterval (10000)
#endif
{
}

CServerLifetime::~CServerLifetime ()
{
#ifdef	_LOG_LIFETIME
	if	(
			(mObjectLifetimes.GetCount() > 0)
		&&	(LogIsActive (_LOG_LIFETIME))
		)
	{
		LogMessage (_LOG_LIFETIME, _T("CServerLifetime:~CServerLifetime Objects [%d]"), mObjectLifetimes.GetCount());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CServerLifetime::ManageObjectLifetime (CSvrObjLifetime * pObject)
{
	CLockCS	lLock (mCriticalSection);

	try
	{
		mObjectLifetimes.AddUnique (pObject);
#ifdef	_LOG_LIFETIME_NOT
		if	(LogIsActive (_LOG_LIFETIME))
		{
			LogMessage (_LOG_LIFETIME, _T("ManagedLifetimes [%d] Locks [%d]"), mObjectLifetimes.GetCount(), _AtlModule.GetLockCount());
		}
#endif
	}
	catch AnyExceptionSilent
}

void CServerLifetime::UnmanageObjectLifetime (CSvrObjLifetime * pObject)
{
	CLockCS	lLock (mCriticalSection);

	try
	{
		mObjectLifetimes.Remove (pObject);
#ifdef	_LOG_LIFETIME_NOT
		if	(LogIsActive (_LOG_LIFETIME))
		{
			LogMessage (_LOG_LIFETIME, _T("ManagedLifetimes [%d] Objects [%d]"), mObjectLifetimes.GetCount(), _AtlModule.GetLockCount());
		}
#endif
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////

void CServerLifetime::VerifyObjectLifetimes (bool pImmediate)
{
	if	(
			(TicksElapsed (mLastVerifyTime, mVerifyInterval))
		||	(pImmediate)
		)
	{
		CLockCS	lLock (mCriticalSection);

		try
		{
			INT_PTR				lNdx;
			CSvrObjLifetime *	lObject;
			bool				lCoCreateSuspended = false;

#ifdef	_LOG_LIFETIME
			if	(
					(pImmediate)
				&&	(LogIsActive (_LOG_LIFETIME))
				)
			{
				LogMessage (_LOG_LIFETIME, _T("VerifyObjectLifetimes [%d] Locks [%d] (start)"), mObjectLifetimes.GetCount(), _AtlModule.GetLockCount());
			}
#endif
			if	(pImmediate)
			{
				CoSuspendClassObjects ();
				lCoCreateSuspended = true;
			}

			for	(lNdx = 0; lNdx < (INT_PTR)mObjectLifetimes.GetCount(); lNdx++)
			{
				lObject = mObjectLifetimes [lNdx];

				try
				{
					if	(!lObject->VerifyClientLifetime ())
					{
						if	(!lCoCreateSuspended)
						{
#ifdef	_LOG_LIFETIME
							if	(
									(!pImmediate)
								&&	(LogIsActive (_LOG_LIFETIME))
								)
							{
								LogMessage (_LOG_LIFETIME, _T("VerifyObjectLifetimes [%d] Locks [%d] (start)"), mObjectLifetimes.GetCount(), _AtlModule.GetLockCount());
							}
#endif
							CoSuspendClassObjects ();
							lCoCreateSuspended = true;
						}
						mObjectLifetimes.RemoveAt (lNdx);
						lNdx = -1;
						lObject->OnClientEnded ();
					}
				}
				catch AnyExceptionDebug
			}

#ifdef	_LOG_LIFETIME
			if	(
					(
						(lCoCreateSuspended)
					||	(pImmediate)
					)
				&&	(LogIsActive (_LOG_LIFETIME))
				)
			{
				LogMessage (_LOG_LIFETIME, _T("VerifyObjectLifetimes [%d] Locks [%d] (end)"), mObjectLifetimes.GetCount(), _AtlModule.GetLockCount());
			}
#endif
			if	(lCoCreateSuspended)
			{
				CoResumeClassObjects ();
			}
		}
		catch AnyExceptionDebug
	}
}
