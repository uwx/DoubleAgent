/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "DaCtlTTSEngines.h"
#include "DaCtlTTSEngine.h"
#include "ErrorInfo.h"
#include "Registry.h"
#include "Sapi5Voices.h"
#ifndef	_WIN64
#include "Sapi4Voices.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

#ifdef	_DACORE_LOCAL
#define	LogServerPtr	(void*)NULL
#else
#define	LogServerPtr	mServerObject.GetInterfacePtr()
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlTTSEngines::DaCtlTTSEngines ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::DaCtlTTSEngines (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((IDaCtlTTSEngines*)this);
#endif
}

DaCtlTTSEngines::~DaCtlTTSEngines ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::~DaCtlTTSEngines (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((IDaCtlTTSEngines*)this);
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlTTSEngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::FinalRelease (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
	Terminate (false);
}

void DaCtlTTSEngines::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::Terminate [%u] [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, LogServerPtr, mLocalObject.Ptr());
		}
#endif
#endif

		Disconnect (pFinal);
		mOwner = NULL;

#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::Terminate [%u] [%p] [%p] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, LogServerPtr, mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

void DaCtlTTSEngines::Disconnect (bool pFinal)
{
	SafeFreeSafePtr (mCachedEnum);
	if	(pFinal)
	{
		mTTSEngines.RemoveAll ();
	}
#ifndef	_DACORE_LOCAL
	if	(pFinal)
	{
		mServerObject.Detach ();
	}
	else
	{
		SafeFreeSafePtr (mServerObject);
	}
#endif
	SafeFreeSafePtr (mLocalObject);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlTTSEngines::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
#ifndef	_DACORE_LOCAL
		if	(mOwner->mServer == NULL)
#endif
		{
			if	(!mLocalObject)
			{
				if	(mLocalObject = new CDaCmnTTSEngines)
				{
					lResult = mLocalObject->UseAllVoices ();
				}
				else
				{
					lResult = E_OUTOFMEMORY;
				}
			}
		}
#ifndef	_DACORE_LOCAL
		else
		if	(mServerObject == NULL)
		{
			lResult = mOwner->mServer->get_TTSEngines (&mServerObject);
		}
#endif
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::SetOwner (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), LogServerPtr, mLocalObject.Ptr());
	}
#endif
	return lResult;
}

DaControl * DaCtlTTSEngines::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlTTSEngines::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlTTSEngines::InitializeObjects ()
{
	try
	{
		if	(mLocalObject)
		{
			INT_PTR								lNdx;
			tPtr <CComObject <DaCtlTTSEngine> >	lItemObject;
			IDaCtlTTSEnginePtr					lItemInterface;
			CSapi5VoiceInfo *					lSapi5VoiceInfo;
#ifndef	_WIN64
			CSapi4VoiceInfo*					lSapi4VoiceInfo;
#endif

			for	(lNdx = 0; true; lNdx++)
			{
				if	(lSapi5VoiceInfo = mLocalObject->GetSapi5VoiceAt (lNdx))
				{
					if	(
							(SUCCEEDED (CComObject <DaCtlTTSEngine>::CreateInstance (lItemObject.Free())))
						&&	(lItemObject->mLocalObject = new CDaCmnTTSEngine)
						&&	(SUCCEEDED (lItemObject->SetOwner (mOwner)))
						)
					{
						lItemObject->mLocalObject->Initialize (lSapi5VoiceInfo);
						lItemInterface = (LPDISPATCH)lItemObject.Detach();
						mTTSEngines.Add (lItemInterface);
					}
				}
#ifndef	_WIN64
				else
				if	(lSapi4VoiceInfo = mLocalObject->GetSapi4VoiceAt (lNdx))
				{
					if	(
							(SUCCEEDED (CComObject <DaCtlTTSEngine>::CreateInstance (lItemObject.Free())))
						&&	(lItemObject->mLocalObject = new CDaCmnTTSEngine)
						&&	(SUCCEEDED (lItemObject->SetOwner (mOwner)))
						)
					{
						lItemObject->mLocalObject->Initialize (lSapi4VoiceInfo);
						lItemInterface = (LPDISPATCH)lItemObject.Detach();
						mTTSEngines.Add (lItemInterface);
					}
				}
#endif
				else
				{
					break;
				}
			}
		}
#ifndef	_DACORE_LOCAL
		else
		if	(mServerObject)
		{
			long								lNdx;
			IDaSvrTTSEnginePtr					lServerObject;
			tPtr <CComObject <DaCtlTTSEngine> >	lItemObject;
			IDaCtlTTSEnginePtr					lItemInterface;

			for	(lNdx = 0; (mServerObject->get_Item (lNdx, &lServerObject) == S_OK); lNdx++)
			{
				if	(SUCCEEDED (CComObject <DaCtlTTSEngine>::CreateInstance (lItemObject.Free())))
				{
					lItemObject->mServerObject = lServerObject;
					if	(SUCCEEDED (lItemObject->SetOwner (mOwner)))
					{
						lItemInterface = (LPDISPATCH)lItemObject.Detach();
						mTTSEngines.Add (lItemInterface);
					}
				}
			}
		}
#endif
	}
	catch AnyExceptionDebug
}

HRESULT DaCtlTTSEngines::InitEnumVariant (CEnumVARIANTImpl * pEnum)
{
	HRESULT					lResult = S_FALSE;
	tArrayPtr <CComVariant>	lArray;
	INT_PTR					lNdx;

	if	(lArray = new CComVariant [mTTSEngines.GetCount()+1])
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mTTSEngines.GetCount(); lNdx++)
		{
			lArray [lNdx] = (LPDISPATCH)(mTTSEngines [lNdx].p);
		}
		lResult = pEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mTTSEngines.GetCount()]), (LPDISPATCH)this, AtlFlagCopy);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlTTSEngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaCtlTTSEngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlTTSEngines::get_Item (VARIANT Index, IDaCtlTTSEngine **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	INT_PTR				lItemNdx = -1;
	IDaCtlTTSEnginePtr	lInterface;

	if	(!Item)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Item) = NULL;

		if	(V_VT (&Index) == VT_I4)
		{
			lItemNdx = V_I4 (&Index);
		}
		else
		if	(V_VT (&Index) == VT_I2)
		{
			lItemNdx = V_I2 (&Index);
		}
		else
		if	(V_VT (&Index) == VT_BSTR)
		{
			CAtlString	lItemName = V_BSTR (&Index);
			tBstrPtr	lTTSModeID;
			tBstrPtr	lDisplayName;

			if	(!lItemName.IsEmpty ())
			{
				for	(lItemNdx = 0; lItemNdx < (INT_PTR)mTTSEngines.GetCount(); lItemNdx++)
				{
					if	(
							(
								(SUCCEEDED (mTTSEngines [lItemNdx]->get_TTSModeID (lTTSModeID.Free())))
							&&	(lItemName.CompareNoCase (CAtlString (lTTSModeID)) == 0)
							)
						||	(
								(SUCCEEDED (mTTSEngines [lItemNdx]->get_DisplayName (lDisplayName.Free())))
							&&	(lItemName.CompareNoCase (CAtlString (lDisplayName)) == 0)
							)
						)
					{
						break;
					}
				}
			}
		}

		if	(
				(lItemNdx >= 0)
			&&	(lItemNdx < (INT_PTR)mTTSEngines.GetCount())
			)
		{
			lInterface = mTTSEngines [lItemNdx].p;
			(*Item) = lInterface.Detach ();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlTTSEngines::get_Count (long *Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mTTSEngines.GetCount();
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlTTSEngines::get__NewEnum (IUnknown **EnumVariant)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT								lResult = S_OK;
	tPtr <CComObject<CEnumVARIANT> >	lEnumVariant;
	IEnumVARIANTPtr						lInterface;

	if	(!EnumVariant)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*EnumVariant) = NULL;

		if	(
				(SUCCEEDED (lResult = CComObject<CEnumVARIANT>::CreateInstance (lEnumVariant.Free())))
			&&	(SUCCEEDED (lResult = InitEnumVariant (lEnumVariant)))
			)
		{
			lInterface = lEnumVariant.Detach();
			(*EnumVariant) = lInterface.Detach();
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlTTSEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlTTSEngines::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
