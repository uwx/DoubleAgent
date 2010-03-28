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

//////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
#include <typeinfo.h>
//#define	_TRACE_GATED_INSTANCE	LogNormal|LogHighVolume|LogTimeMs
#endif

//////////////////////////////////////////////////////////////////////
#pragma auto_inline(off)
#pragma optimize("", off)
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
	class CGenericLock
	{
	public:
		CGenericLock () {}
		virtual ~CGenericLock () {}
	};
	
	template <typename aType> class CTypeLock : public CGenericLock
	{
	public:
		CTypeLock (aType * pInstance) : mInstance (pInstance) {}
		virtual ~CTypeLock () {}
		bool Lock (DWORD pTimeout);
		bool Unlock ();
		void ShutTheGate ();
		void OpenTheGate ();
		aType * GetInstance ();
		void NotInstance ();
	private:
		aType *				mInstance;
		CMutex				mInUseLock;
		::CCriticalSection	mGateLock;
		::CCriticalSection	mValueLock;
	};

private:
	static COwnPtrArray <CGenericLock>	mInstances;
	static ::CCriticalSection			mThreadLock;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

template <typename TYPE> LPVOID CInstanceGate::PutGatedInstance (TYPE * pInstance)
{
	CTypeLock <TYPE> *	lTypedLock = NULL;

	if	(pInstance)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			lTypedLock = static_cast <CTypeLock <TYPE> *> (FindGatedInstance<TYPE> (pInstance));
			if	(!lTypedLock)
			{
				mInstances.AddSortedQS (lTypedLock = new CTypeLock <TYPE> (pInstance));
#ifdef	_TRACE_GATED_INSTANCE
				LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Gates  [%p] %hs"), lTypedLock, pInstance, typeid(TYPE).name());
#endif
			}
		}
		catch AnyExceptionSilent
	}
	return lTypedLock;
}

template <typename TYPE> void CInstanceGate::NotGatedInstance (TYPE * pInstance)
{
	CTypeLock <TYPE> *	lTypedLock = NULL;

	if	(pInstance)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			if	(lTypedLock = static_cast <CTypeLock <TYPE> *> (FindGatedInstance<TYPE> (pInstance)))
			{
				if	(mInstances.RemoveSortedQS (lTypedLock) < 0)
				{
					lTypedLock = NULL;
				}
				else
				{
					lTypedLock->ShutTheGate ();
				}
			}
		}
		catch AnyExceptionSilent
	}

//
//	This is the core of the gate.  First gate through ShutTheGate and OpenTheGate.
//	Then lock and unlock it.  By the time it's deleted, it's sure to be empty and not in use.
//
	if	(lTypedLock)
	{
		try
		{
#ifdef	_TRACE_GATED_INSTANCE
			LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Not    [%p] %hs"), lTypedLock, lTypedLock->GetInstance (), typeid(TYPE).name());
#endif
			lTypedLock->NotInstance ();
			lTypedLock->OpenTheGate ();
			lTypedLock->Lock (INFINITE);
			lTypedLock->Unlock ();

#ifdef	_TRACE_GATED_INSTANCE
			LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Free   [%p] %hs"), lTypedLock, lTypedLock->GetInstance (), typeid(TYPE).name());
#endif
			delete lTypedLock;
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

template <typename TYPE> bool CInstanceGate::LockGatedInstance (LPVOID pLock, TYPE *& pInstance, DWORD pLockWait)
{
	bool				lRet = false;
	CGenericLock *		lInstanceLock;
	CTypeLock <TYPE> *	lTypedLock = NULL;

	pInstance = NULL;

	if	(pLock)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			if	(
					(lInstanceLock = static_cast <CGenericLock *> (pLock))
				&&	(mInstances.FindSortedQS (lInstanceLock) >= 0)
				&&	(lTypedLock = dynamic_cast <CTypeLock <TYPE> *> (lInstanceLock))
				)
			{
				lTypedLock->ShutTheGate ();
			}
		}
		catch AnyExceptionSilent
	}

//
//	This is the core of the gate.  ShutTheGate will stop the instance from being deleted while
//	still leaving our global ThreadLock unlocked.  By the time OpenTheGate is called, the
//	instance lock will have been emptied if it's being deleted.
//
	if	(lTypedLock)
	{
		pInstance = lTypedLock->GetInstance ();
		lTypedLock->OpenTheGate ();
		if	(pInstance)
		{
			if	(lTypedLock->Lock (pLockWait))
			{
				pInstance = lTypedLock->GetInstance ();
#ifdef	_TRACE_GATED_INSTANCE
				LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Locked [%p] %hs"), lTypedLock, pInstance, typeid(TYPE).name());
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
	bool				lRet = false;
	CGenericLock *		lInstanceLock;
	CTypeLock <TYPE> *	lTypedLock;

	if	(pLock)
	{
		try
		{
			if	(
					(lInstanceLock = static_cast <CGenericLock *> (pLock))
				&&	(lTypedLock = dynamic_cast <CTypeLock <TYPE> *> (lInstanceLock))
				)
			{
#ifdef	_TRACE_GATED_INSTANCE
				TYPE *	lInstance = lTypedLock->GetInstance ();
#endif
				if	(lTypedLock->Unlock ())
				{
					lRet = true;
#ifdef	_TRACE_GATED_INSTANCE
					LogMessage (_TRACE_GATED_INSTANCE, _T("[%p] Freed  [%p] %hs [%p]"), lTypedLock, pInstance, typeid(TYPE).name(), lInstance);
#endif
				}
			}
//
//	We assume that pLock is a valid instance lock that is currently locked.  We
//	don't look it up because it may be removed from our list by NotGatedInstance.
//	After it's unlocked, it can be deleted.
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
	CGenericLock *		lInstanceLock;
	CTypeLock <TYPE> *	lTypedLock;

	pInstance = NULL;

	if	(pLock)
	{
		CSingleLock	lThreadLock (&mThreadLock, TRUE);
		try
		{
			if	(
					(lInstanceLock = static_cast <CGenericLock *> (pLock))
				&&	(mInstances.FindSortedQS (lTypedLock) >= 0)
				&&	(lTypedLock = dynamic_cast <CTypeLock <TYPE> *> (lInstanceLock))
				&&	(pInstance = lTypedLock->GetInstance ())
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
	CTypeLock <TYPE> *	lTypedLock;

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
							(lTypedLock = dynamic_cast <CTypeLock <TYPE> *> (mInstances [lNdx]))
						&&	(lTypedLock->GetInstance () == pInstance)
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

template <typename aType> bool CInstanceGate::CTypeLock<aType>::Lock (DWORD pTimeout)
{
	if	(this)
	{
		if	(mInUseLock.Lock (pTimeout))
		{
			return true;
		}
	}
	return false;
}

template <typename aType> bool CInstanceGate::CTypeLock<aType>::Unlock ()
{
	if	(this)
	{
		if	(mInUseLock.Unlock ())
		{
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////

template <typename aType> void CInstanceGate::CTypeLock<aType>::ShutTheGate ()
{
	if	(this)
	{
		mGateLock.Lock ();
	}
}

template <typename aType> void CInstanceGate::CTypeLock<aType>::OpenTheGate ()
{
	if	(this)
	{
		mGateLock.Unlock ();
	}
}

//////////////////////////////////////////////////////////////////////

template <typename aType> aType * CInstanceGate::CTypeLock<aType>::GetInstance ()
{
	aType * lRet = NULL;
	if	(this)
	{
		try
		{
			mValueLock.Lock ();
			try
			{
				lRet = mInstance;
			}
			catch AnyExceptionSilent
			mValueLock.Unlock ();
		}
		catch AnyExceptionSilent
	}
	return lRet;
}

template <typename aType> void CInstanceGate::CTypeLock<aType>::NotInstance ()
{
	if	(this)
	{
		try
		{
			mValueLock.Lock ();
			try
			{
				mInstance = NULL;
			}
			catch AnyExceptionSilent
			mValueLock.Unlock ();
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#pragma optimize("", on)
#pragma auto_inline()
//////////////////////////////////////////////////////////////////////

#endif // !defined(INSTANCEGATE_H_INCLUDED_)
