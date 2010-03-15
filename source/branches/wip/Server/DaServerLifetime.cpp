/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaServerLifetime.h"
#include "DaServer.h"
#include "GuidStr.h"
#include "Registry.h"
#include "Elapsed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	_LOG_LIFETIME	(GetProfileDebugInt(_T("LogInstance_Handler"),LogDetails,true)&0xFFFF)
#endif

#ifndef	_LOG_LIFETIME
#define	_LOG_LIFETIME	LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

CDaObjectLifetime::CDaObjectLifetime ()
{
}

CDaObjectLifetime::~CDaObjectLifetime ()
{
}

/////////////////////////////////////////////////////////////////////////////

bool CDaObjectLifetime::VerifyClientLifetime ()
{
	bool	lRet = true;

	if	(
			(mClientMutex)
		&&	(mClientMutex->m_hObject)
		&&	(mClientMutex->Lock (0))
		)
	{
		mClientMutex->Unlock ();
		lRet = false;
	}
	return lRet;
}

void CDaObjectLifetime::OnClientEnded ()
{
#ifdef	_DEBUG
	LogMessage (LogIfActive, _T("[%0] CDaObjectLifetime::OnClientEnded"), this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CDaObjectLifetime::ManageObjectLifetime (CCmdTarget * pObject, LPCTSTR pClientMutexName)
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

				if	(SUCCEEDED (LogComErr (LogNormal, CoGetCurrentLogicalThreadId (&lThreadId))))
				{
					mClientMutexName.Format (_T("Local\\%s"), (CString)CGuidStr (lThreadId));
				}
			}
			if	(!mClientMutexName.IsEmpty ())
			{
				try
				{
					mClientMutex = new CMutex (FALSE, mClientMutexName);
				}
				catch AnyExceptionDebug

				if	(
						(!mClientMutex)
					||	(!mClientMutex->m_hObject)
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
					mClientMutex = NULL;
#ifdef	_LOG_LIFETIME
					if	(LogIsActive (_LOG_LIFETIME))
					{
						LogMessage (_LOG_LIFETIME, _T("[%p(%d)] Manage %s ClientMutex [%s] UNLOCKED"), pObject, pObject->m_dwRef, ObjClassName(pObject), mClientMutexName);
					}
#endif
				}
				else
				{
					lRet = true;
#ifdef	_LOG_LIFETIME
					if	(LogIsActive (_LOG_LIFETIME))
					{
						LogMessage (_LOG_LIFETIME, _T("[%p(%d)] Manage %s ClientMutex [%p] [%s]"), pObject, pObject->m_dwRef, ObjClassName(pObject), mClientMutex->m_hObject, mClientMutexName);
					}
#endif
				}
			}
		}
		catch AnyExceptionDebug
	}
	if	(lRet)
	{
		TheServerApp->ManageObjectLifetime (this);
	}
	return lRet;
}

void CDaObjectLifetime::UnmanageObjectLifetime (CCmdTarget * pObject)
{
	if	(mClientMutex)
	{
		try
		{
#ifdef	_LOG_LIFETIME
			if	(LogIsActive (_LOG_LIFETIME))
			{
				LogMessage (_LOG_LIFETIME, _T("[%p(%d)] Unmanage %s"), pObject, pObject->m_dwRef, ObjClassName(pObject));
			}
#endif
			TheServerApp->UnmanageObjectLifetime (this);
		}
		catch AnyExceptionSilent
	}

	mClientMutexName.Empty ();
	SafeFreeSafePtr (mClientMutex);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaServerLifetime::CDaServerLifetime ()
:	mLastVerifyTime (GetTickCount ()),
#ifdef	_DEBUG
	mVerifyInterval (3000)
#else
	mVerifyInterval (10000)
#endif
{
}

CDaServerLifetime::~CDaServerLifetime ()
{
#ifdef	_LOG_LIFETIME
	if	(
			(mObjectLifetimes.GetSize () > 0)
		&&	(LogIsActive (_LOG_LIFETIME))
		)
	{
		LogMessage (_LOG_LIFETIME, _T("CDaServerLifetime:~CDaServerLifetime Objects [%d]"), mObjectLifetimes.GetSize ());
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaServerLifetime::ManageObjectLifetime (CDaObjectLifetime * pObject)
{
	CSingleLock	lLock (&mCriticalSection, TRUE);

	try
	{
		mObjectLifetimes.AddUnique (pObject);
	}
	catch AnyExceptionSilent
}

void CDaServerLifetime::UnmanageObjectLifetime (CDaObjectLifetime * pObject)
{
	CSingleLock	lLock (&mCriticalSection, TRUE);

	try
	{
		mObjectLifetimes.Remove (pObject);
	}
	catch AnyExceptionSilent
}

void CDaServerLifetime::VerifyObjectLifetimes ()
{
	if	(TicksElapsed (mLastVerifyTime, mVerifyInterval))
	{
		CSingleLock	lLock (&mCriticalSection, TRUE);

		try
		{
			INT_PTR				lNdx;
			CDaObjectLifetime *	lObject;

			for	(lNdx = mObjectLifetimes.GetUpperBound (); lNdx >= 0; lNdx--)
			{
				lObject = mObjectLifetimes [lNdx];

				try
				{
					if	(!lObject->VerifyClientLifetime ())
					{
						mObjectLifetimes.RemoveAt (lNdx);
						lObject->OnClientEnded ();
					}
				}
				catch AnyExceptionDebug
			}
		}
		catch AnyExceptionDebug
	}
}
