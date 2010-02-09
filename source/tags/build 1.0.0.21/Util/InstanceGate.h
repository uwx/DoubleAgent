/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef INSTANCEGATE_H_INCLUDED_
#define INSTANCEGATE_H_INCLUDED_
#pragma once

#include "AfxTemplEx.h"

//////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
#include <typeinfo.h>
//#define	_TRACE_GATED_INSTANCE	LogNormal|LogHighVolume|LogTimeMs
#endif

//////////////////////////////////////////////////////////////////////

class CInstanceGate
{
public:
// Operations
	template <typename TYPE> static LPVOID PutGatedInstance (TYPE * pInstance);
	template <typename TYPE> static void NotGatedInstance (TYPE * pInstance);
	template <typename TYPE> static bool LockGatedInstance (LPVOID pLock, TYPE *& pInstance, DWORD pLockWait = INFINITE);
	template <typename TYPE> static bool FreeGatedInstance (LPVOID pLock, TYPE *& pInstance);
	template <typename TYPE> static bool GetGatedInstance (LPVOID pLock, TYPE *& pInstance);
	template <typename TYPE> static LPVOID FindGatedInstance (TYPE * pInstance);

// Implementation
private:
	class CInstanceLock : public CMutex
	{
	public:
		CInstanceLock () {}
		virtual ~CInstanceLock () {};
		void ShutTheGate ();
		void OpenTheGate ();
	protected:
		CCriticalSection mGateLock;
	};

	template <typename TYPE> class CTypeLock : public CInstanceLock
	{
	public:
		CTypeLock (TYPE * pInstance) : mInstance (pInstance) {}
		TYPE * GetInstance ();
		void NotInstance ();
	private:
		TYPE * mInstance;
		CCriticalSection mValueLock;
	};

private:
	static COwnPtrArray <CInstanceLock>	mInstances;
	static CCriticalSection				mThreadLock;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

template <typename TYPE> LPVOID CInstanceGate::PutGatedInstance (TYPE * pInstance)
{
	CTypeLock <TYPE> *	lLock = NULL;

	if	(pInstance)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			lLock = static_cast <CTypeLock <TYPE> *> (FindGatedInstance (pInstance));
			if	(!lLock)
			{
				mInstances.AddSortedQS (lLock = new CTypeLock <TYPE> (pInstance));
#ifdef	_TRACE_GATED_INSTANCE
				LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Gates  [%p] %hs"), lLock, pInstance, typeid(TYPE).name());
#endif
			}
		}
		catch AnyExceptionSilent
	}
	return lLock;
}

template <typename TYPE> void CInstanceGate::NotGatedInstance (TYPE * pInstance)
{
	CTypeLock <TYPE> *	lLock = NULL;

	if	(pInstance)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			lLock = static_cast <CTypeLock <TYPE> *> (FindGatedInstance (pInstance));
			if	(
					(lLock)
				&&	(mInstances.RemoveSortedQS (lLock) < 0)
				)
			{
				lLock = NULL;
			}
			else
			{
				lLock->ShutTheGate ();
			}
		}
		catch AnyExceptionSilent
	}

//
//	This is the core of the gate.  First gate through ShutTheGate and OpenTheGate.
//	Then lock and unlock it.  By the time it's deleted, it's sure to be empty and not in use.
//
	if	(lLock)
	{
		try
		{
#ifdef	_TRACE_GATED_INSTANCE
			LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Not    [%p] %hs"), lLock, lLock->GetInstance (), typeid(TYPE).name());
#endif
			lLock->NotInstance ();
			lLock->OpenTheGate ();
			lLock->Lock ();
			lLock->Unlock ();

#ifdef	_TRACE_GATED_INSTANCE
			LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Free   [%p] %hs"), lLock, lLock->GetInstance (), typeid(TYPE).name());
#endif
			delete lLock;
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

template <typename TYPE> bool CInstanceGate::LockGatedInstance (LPVOID pLock, TYPE *& pInstance, DWORD pLockWait)
{
	bool				lRet = false;
	CTypeLock <TYPE> *	lLock = NULL;

	pInstance = NULL;

	if	(pLock)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);

		try
		{
			if	(mInstances.FindSortedQS (static_cast <CTypeLock <TYPE> *> (pLock)) >= 0)
			{
				lLock = static_cast <CTypeLock <TYPE> *> (pLock);
				lLock->ShutTheGate ();
			}
		}
		catch AnyExceptionSilent
	}

//
//	This is the core of the gate.  ShutTheGate will stop the instance from being deleted while
//	still leaving our global ThreadLock unlocked.  By the time OpenTheGate is called, the
//	instance lock will have been emptied if it's being deleted.
//
	if	(lLock)
	{
		pInstance = lLock->GetInstance ();
		lLock->OpenTheGate ();
		if	(pInstance)
		{
			if	(lLock->Lock (pLockWait))
			{
				pInstance = lLock->GetInstance ();
#ifdef	_TRACE_GATED_INSTANCE
				LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Locked [%p] %hs"), lLock, pInstance, typeid(TYPE).name());
#endif
			}
			else
			{
				pInstance = NULL;
			}
		}
	}
	if	(pInstance)
	{
		lRet = true;
	}
	return lRet;
}

template <typename TYPE> bool CInstanceGate::FreeGatedInstance (LPVOID pLock, TYPE *& pInstance)
{
	bool	lRet = false;

	if	(pLock)
	{
		try
		{
			CTypeLock <TYPE> *	lLock = static_cast <CTypeLock <TYPE> *> (pLock);
#ifdef	_TRACE_GATED_INSTANCE
			TYPE *				lInstance = lLock->GetInstance ();
#endif
			if	(lLock->Unlock ())
			{
				lRet = true;
#ifdef	_TRACE_GATED_INSTANCE
				LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Freed  [%p] %hs [%p]"), lLock, pInstance, typeid(TYPE).name(), lInstance);
#endif
			}
//
//	We assume that pLock is a valid instance lock that is currently locked.  We
//	don't look it up because it may be removed from our list by NotGatedInstance.
//	After it's unlocked, it can be delete.
//
		}
		catch AnyExceptionSilent
	}

	pInstance = NULL;
	return lRet;
}

//////////////////////////////////////////////////////////////////////

template <typename TYPE> bool CInstanceGate::GetGatedInstance (LPVOID pLock, TYPE *& pInstance)
{
	bool				lRet = false;
	CTypeLock <TYPE> *	lLock;

	pInstance = NULL;

	if	(pLock)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			if	(
					(lLock = static_cast <CTypeLock <TYPE> *> (pLock))
				&&	(mInstances.FindSortedQS (lLock) >= 0)
				&&	(pInstance = lLock->GetInstance ())
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

template <typename TYPE> LPVOID CInstanceGate::FindGatedInstance (TYPE * pInstance)
{
	LPVOID				lRet = NULL;
	INT_PTR				lNdx;
	CTypeLock <TYPE> *	lLock;

	if	(pInstance)
	{
		for	(lNdx = 0; (lRet == NULL); lNdx++)
		{
			CSingleLock	lThreadLock (&mThreadLock, TRUE);

			try
			{
				if	(lNdx <= mInstances.GetUpperBound())
				{
					if	(
							(lLock = dynamic_cast <CTypeLock <TYPE> *> (mInstances [lNdx]))
						&&	(lLock->GetInstance () == pInstance)
						)
					{
						lRet = mInstances [lNdx];
					}
				}
				else
				{
					break;
				}
			}
			catch AnyExceptionSilent
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline void CInstanceGate::CInstanceLock::ShutTheGate ()
{
	if	(this)
	{
		mGateLock.Lock ();
	}
}

inline void CInstanceGate::CInstanceLock::OpenTheGate ()
{
	if	(this)
	{
		mGateLock.Unlock ();
	}
}

//////////////////////////////////////////////////////////////////////

template <typename TYPE> inline TYPE * CInstanceGate::CTypeLock<TYPE>::GetInstance ()
{
	TYPE * lRet = NULL;
	if	(this)
	{
		mValueLock.Lock ();
		lRet = mInstance;
		mValueLock.Unlock ();
	}
	return lRet;
}

template <typename TYPE> inline void CInstanceGate::CTypeLock<TYPE>::NotInstance ()
{
	if	(this)
	{
		mValueLock.Lock ();
		mInstance = NULL;
		mValueLock.Unlock ();
	}
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(INSTANCEGATE_H_INCLUDED_)
