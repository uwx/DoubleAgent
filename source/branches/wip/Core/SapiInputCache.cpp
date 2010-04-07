/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCore.h"
#include "SapiInputCache.h"
#include "Sapi5Inputs.h"
#include "Sapi5Input.h"
#include "Registry.h"
#include "DebugStr.h"

#ifdef	_DEBUG
#define	_DEBUG_CACHE	(GetProfileDebugInt(_T("LogInputCache"),LogVerbose,true)&0xFFFF)
#endif

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapiInputCache)

CSapiInputCache::CSapiInputCache ()
{
}

CSapiInputCache::~CSapiInputCache ()
{
#ifdef	_DEBUG_CACHE
	try
	{
		LogMessage (_DEBUG_CACHE, _T("CSapiInputCache::~CSapiInputCache Inputs [%d] Clients [%d]"), mCachedInputs.GetSize(), mInputClients.GetCount());
	}
	catch AnyExceptionSilent
#endif
	try
	{
		mInputClients.RemoveAll ();
	}
	catch AnyExceptionSilent
	try
	{
		mCachedInputs.DeleteAll ();
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

CSapiInputCache * CSapiInputCache::GetStaticInstance ()
{
	return &_AtlModule;
}

CSapi5Inputs * CSapiInputCache::GetSapi5Inputs ()
{
	if	(!mSapi5Inputs)
	{
		mSapi5Inputs = CSapi5Inputs::CreateInstance();
	}
	return mSapi5Inputs;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSapi5Input * CSapiInputCache::GetAgentInput (LANGID pLangID, bool pUseDefaults, bool pCached)
{
	CSapi5Input *		lRet = NULL;
	CSapi5InputInfo *	lInputInfo;
	tPtr <CSapi5Input>	lInput;

	if	(GetSapi5Inputs())
	{
		if	(lInputInfo = mSapi5Inputs->GetInput (pLangID, pUseDefaults))
		{
			if	(pCached)
			{
				lRet = FindCachedInput (lInputInfo->mEngineIdLong);
			}
			if	(
					(!lRet)
				&&	(lInput = CSapi5Input::CreateInstance())
				&&	(SUCCEEDED (lInput->SetEngineId (lInputInfo->mEngineIdShort)))
				)
			{
				lRet = lInput.Detach();
			}
		}
#ifndef	_STRICT_COMPATIBILITY
		if	(
				(!lRet)
			&&	(lInputInfo = mSapi5Inputs->operator () (mSapi5Inputs->DefaultInputNdx()))
			)
		{
			if	(pCached)
			{
				lRet = FindCachedInput (lInputInfo->mEngineIdLong);
			}
			if	(
					(!lRet)
				&&	(lInput = (CSapi5Input *)CSapi5Input::CreateObject())
				)
			{
				lInput->SetEngineId (lInputInfo->mEngineIdShort);
				if	(lInput->SafeIsValid())
				{
					lRet = lInput.Detach ();
				}
			}
		}
#endif
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

CSapi5Input * CSapiInputCache::GetAgentInput (LPCTSTR pEngineName, LANGID pLangID, bool pUseDefaults, bool pCached)
{
	CSapi5Input *		lRet = NULL;
	CSapi5InputInfo *	lInputInfo;
	tPtr <CSapi5Input>	lInput;

	if	(GetSapi5Inputs())
	{
		if	(
				(
				(lInputInfo = mSapi5Inputs->GetEngineName (pEngineName))
				||	(lInputInfo = mSapi5Inputs->GetEngineId (pEngineName))
				)
			&&	(mSapi5Inputs->InputSupportsLanguage (lInputInfo, pLangID, pUseDefaults))
			)
		{
			if	(pCached)
			{
				lRet = FindCachedInput (lInputInfo->mEngineIdLong);
			}
			if	(
					(!lRet)
				&&	(lInput = CSapi5Input::CreateInstance())
				&&	(SUCCEEDED (lInput->SetEngineId (lInputInfo->mEngineIdShort)))
				)
			{
				lRet = lInput.Detach();
			}
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CSapiInputCache::CacheInput (CSapi5Input * pInput, CSapiInputClient * pClient)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(
				(pInput)
			&&	(pClient)
			)
		{
			INT_PTR											lInputNdx;
			tPtr <CAtlPtrTypeArray <CSapiInputClient> > &	lClients = mInputClients [pInput];

			lInputNdx = mCachedInputs.Find (pInput);
			if	(lInputNdx < 0)
			{
				mCachedInputs.Add (pInput);
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Cache Input [%p] [%s] for Client [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient));
#endif
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Duplicate Input [%p] [%s] for Client [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient));
			}
#endif

			if	(!lClients)
			{
				lClients = new CAtlPtrTypeArray <CSapiInputClient>;
			}
			if	(
					(lClients)
				&&	(lClients->Find (pClient) < 0)
				)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Input [%p] [%s] Client [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient));
#endif
				lClients->Add (pClient);
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Input [%p] [%s] Duplicate Client [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient));
			}
#endif
			lRet = true;
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("CacheInput failed for Input [%p] Client [%p] [%s]"), pInput, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CSapiInputCache::UncacheInput (CSapi5Input * pInput)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR	lInputNdx;

		if	(pInput)
		{
			lInputNdx = mCachedInputs.Find (pInput);

			if	(lInputNdx >= 0)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Uncache Input [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId());
#endif
				mCachedInputs.RemoveAt (lInputNdx);
				mInputClients.RemoveKey (pInput);
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CSapi5Input * CSapiInputCache::GetCachedInput (int pInputNdx)
{
	CSapi5Input *	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		lRet = mCachedInputs (pInputNdx);
	}
	catch AnyExceptionDebug

	return lRet;
}

CSapi5Input * CSapiInputCache::FindCachedInput (LPCTSTR pEngineId)
{
	CSapi5Input *	lRet = NULL;
	CLockCS			lLock (mCritSec);

	try
	{
		CSapi5Input *	lInput;
		CAtlString		lInputName (pEngineId);
		int				lNdx;

		for	(lNdx = 0; lInput = mCachedInputs (lNdx); lNdx++)
		{
			if	(lInputName.CompareNoCase ((BSTR)lInput->GetEngineId ()) == 0)
			{
				lRet = lInput;
				break;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CSapiInputCache::AddInputClient (CSapi5Input * pInput, CSapiInputClient * pClient)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		if	(
				(pInput)
			&&	(pClient)
			&&	(mCachedInputs.Find (pInput) >= 0)
			)
		{
			tPtr <CAtlPtrTypeArray <CSapiInputClient> > &	lClients = mInputClients [pInput];

			if	(
					(lClients)
				&&	(lClients->Find (pClient) < 0)
				)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Input [%p] [%s] Client [%p] [%s] Clients [%d]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient), lClients->GetSize());
#endif
				lClients->Add (pClient);
				lRet = true;
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Input [%p] [%s] Duplicate Client [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient));
			}
#endif
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("AddClient failed for Input [%p] Client [%p] [%s]"), pInput, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CSapiInputCache::RemoveInputClient (CSapi5Input * pInput, CSapiInputClient * pClient, bool pDeleteUnusedInput)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		INT_PTR	lInputNdx;

		if	(
				(pInput)
			&&	(pClient)
			&&	((lInputNdx = mCachedInputs.Find (pInput)) >= 0)
			)
		{
			tPtr <CAtlPtrTypeArray <CSapiInputClient> > &	lClients = mInputClients [pInput];

			if	(
					(lClients)
				&&	(lClients->Find (pClient) >= 0)
				)
			{
				lClients->Remove (pClient);
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("Input [%p] [%s] Remove client [%p] [%s] Clients [%d]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient), lClients->GetSize());
#endif
				lRet = true;

				if	(lClients->GetSize() <= 0)
				{
					mInputClients.RemoveKey (pInput);
					if	(pDeleteUnusedInput)
					{
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("Delete cached Input [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId());
#endif
						mCachedInputs.DeleteAt (lInputNdx);
					}
					else
					{
#ifdef	_DEBUG_CACHE
						LogMessage (_DEBUG_CACHE, _T("Unused cached Input [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId());
#endif
						mCachedInputs.RemoveAt (lInputNdx);
					}
				}
			}
#ifdef	_DEBUG_CACHE
			else
			{
				LogMessage (_DEBUG_CACHE, _T("Input [%p] [%s] No Client [%p] [%s]"), pInput, (BSTR)pInput->GetEngineId(), pClient, AtlTypeName(pClient));
			}
#endif
		}
#ifdef	_DEBUG_CACHE
		else
		{
			LogMessage (_DEBUG_CACHE, _T("RemoveClient failed for [%p] Client [%p] [%s]"), pInput, pClient, AtlTypeName(pClient));
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CSapiInputCache::GetInputClients (CSapi5Input * pInput, CAtlPtrTypeArray <CSapiInputClient> & pClients)
{
	bool	lRet = false;
	CLockCS	lLock (mCritSec);

	try
	{
		CAtlPtrTypeArray <CSapiInputClient> *	lClients = NULL;

		if	(
				(pInput)
			&&	(mInputClients.Lookup (pInput, lClients))
			&&	(lClients)
			)
		{
			pClients.Copy (*lClients);
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
