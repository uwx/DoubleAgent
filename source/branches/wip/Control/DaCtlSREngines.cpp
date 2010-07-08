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
#include "DaCtlSREngines.h"
#include "DaCtlSREngine.h"
#include "ErrorInfo.h"
#include "Registry.h"
#include "Sapi5Inputs.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaCtlSREngines::DaCtlSREngines ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::DaCtlSREngines (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

DaCtlSREngines::~DaCtlSREngines ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::~DaCtlSREngines (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlSREngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::FinalRelease (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), mServerObject.GetInterfacePtr(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	Terminate (false);
}

void DaCtlSREngines::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::Terminate [%u] [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
		}
#endif
#endif

		Disconnect (pFinal);
		mOwner = NULL;

#ifdef	_DEBUG_NOT
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::Terminate [%u] [%p] [%p] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), pFinal, mServerObject.GetInterfacePtr(), mLocalObject.Ptr(), _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

void DaCtlSREngines::Disconnect (bool pFinal)
{
	SafeFreeSafePtr (mCachedEnum);
	if	(pFinal)
	{
		mSREngines.RemoveAll ();
	}
	if	(pFinal)
	{
		mServerObject.Detach ();
	}
	else
	{
		SafeFreeSafePtr (mServerObject);
	}
	SafeFreeSafePtr (mLocalObject);
}

/////////////////////////////////////////////////////////////////////////////

HRESULT DaCtlSREngines::SetOwner (DaControl * pOwner)
{
	HRESULT	lResult = S_OK;

	if	(mOwner = pOwner)
	{
		if	(mOwner->mServer == NULL)
		{
			if	(!mLocalObject)
			{
				if	(mLocalObject = new CDaCmnSREngines)
				{
					lResult = mLocalObject->UseAllInputs ();
				}
				else
				{
					lResult = E_OUTOFMEMORY;
				}
			}
		}
		else
		if	(mServerObject == NULL)
		{
			lResult = mOwner->mServer->get_SREngines (&mServerObject);
		}
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogComErrAnon (MinLogLevel(_LOG_INSTANCE,LogAlways), lResult, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::SetOwner (%d) [%p] [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr(), mLocalObject.Ptr());
	}
#endif
	return lResult;
}

DaControl * DaCtlSREngines::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int DaCtlSREngines::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? max(mOwner->m_dwRef,-1) : -1;
}

/////////////////////////////////////////////////////////////////////////////

void DaCtlSREngines::InitializeObjects ()
{
	try
	{
		if	(mLocalObject)
		{
			INT_PTR								lNdx;
			tPtr <CComObject <DaCtlSREngine> >	lItemObject;
			IDaCtlSREnginePtr					lItemInterface;

			for	(lNdx = 0; lNdx < (INT_PTR)mLocalObject->mSapi5Inputs.GetCount(); lNdx++)
			{
				if	(
						(SUCCEEDED (CComObject <DaCtlSREngine>::CreateInstance (lItemObject.Free())))
					&&	(lItemObject->mLocalObject = new CDaCmnSREngine)
					&&	(SUCCEEDED (lItemObject->SetOwner (mOwner)))
					)
				{
					lItemObject->mLocalObject->Initialize (mLocalObject->mSapi5Inputs [lNdx]);
					lItemInterface = (LPDISPATCH)lItemObject.Detach();
					mSREngines.Add (lItemInterface);
				}
			}
		}
		else
		if	(mServerObject)
		{
			long								lNdx;
			IDaSvrSREnginePtr					lServerObject;
			tPtr <CComObject <DaCtlSREngine> >	lItemObject;
			IDaCtlSREnginePtr					lItemInterface;

			for	(lNdx = 0; (mServerObject->get_Item (lNdx, &lServerObject) == S_OK); lNdx++)
			{
				if	(SUCCEEDED (CComObject <DaCtlSREngine>::CreateInstance (lItemObject.Free())))
				{
					lItemObject->mServerObject = lServerObject;
					if	(SUCCEEDED (lItemObject->SetOwner (mOwner)))
					{
						lItemInterface = (LPDISPATCH)lItemObject.Detach();
						mSREngines.Add (lItemInterface);
					}
				}
			}
		}
	}
	catch AnyExceptionDebug
}

HRESULT DaCtlSREngines::InitEnumVariant (CEnumVARIANTImpl * pEnum)
{
	HRESULT					lResult = S_FALSE;
	tArrayPtr <CComVariant>	lArray;
	INT_PTR					lNdx;

	if	(lArray = new CComVariant [mSREngines.GetCount()+1])
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mSREngines.GetCount(); lNdx++)
		{
			lArray [lNdx] = (LPDISPATCH)(mSREngines [lNdx].p);
		}
		lResult = pEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mSREngines.GetCount()]), (LPDISPATCH)this, AtlFlagCopy);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaCtlSREngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaCtlSREngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlSREngines::get_Item (VARIANT Index, IDaCtlSREngine **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	INT_PTR				lItemNdx = -1;
	IDaCtlSREnginePtr	lInterface;

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
			tBstrPtr	lSRModeID;
			tBstrPtr	lDisplayName;

			if	(!lItemName.IsEmpty ())
			{
				for	(lItemNdx = 0; lItemNdx < (INT_PTR)mSREngines.GetCount(); lItemNdx++)
				{
					if	(
							(
								(SUCCEEDED (mSREngines [lItemNdx]->get_SRModeID (lSRModeID.Free())))
							&&	(lItemName.CompareNoCase (CAtlString (lSRModeID)) == 0)
							)
						||	(
								(SUCCEEDED (mSREngines [lItemNdx]->get_DisplayName (lDisplayName.Free())))
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
			&&	(lItemNdx < (INT_PTR)mSREngines.GetCount())
			)
		{
			lInterface = mSREngines [lItemNdx].p;
			(*Item) = lInterface.Detach ();
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSREngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaCtlSREngines::get_Count (long *Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mSREngines.GetCount();
	}

	PutControlError (lResult, __uuidof(IDaCtlSREngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaCtlSREngines::get__NewEnum (IUnknown **EnumVariant)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
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

	PutControlError (lResult, __uuidof(IDaCtlSREngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] DaCtlSREngines::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
