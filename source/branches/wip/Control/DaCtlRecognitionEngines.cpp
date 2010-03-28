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
#include "DaCtlRecognitionEngines.h"
#include "DaCtlRecognitionEngine.h"
#include "ErrorInfo.h"
#include "Registry.h"

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

CDaCtlRecognitionEngines::CDaCtlRecognitionEngines ()
:	mOwner (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::CDaCtlRecognitionEngines (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Add ((LPDISPATCH)this);
#endif
}

CDaCtlRecognitionEngines::~CDaCtlRecognitionEngines ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::~CDaCtlRecognitionEngines (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	_AtlModule.mComObjects.Remove ((LPDISPATCH)this);
#endif

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlRecognitionEngines::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::FinalRelease [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
}

void CDaCtlRecognitionEngines::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::Terminate [%u] [%p(%u)]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), CoIsHandlerConnected(mServerObject));
		}
#endif
#endif
		if	(pFinal)
		{
			mRecognitionEngines.RemoveAll ();
		}
		if	(pFinal)
		{
			mOwner = NULL;
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::Terminate [%u] Done [%d]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, pFinal, _AtlModule.GetLockCount());
		}
#endif
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

void CDaCtlRecognitionEngines::SetOwner (CDaControlObj * pOwner)
{
	if	(mOwner = pOwner)
	{
		long									lNdx;
		IDaSvrRecognitionEnginePtr				lServerObject;
		CComObject <CDaCtlRecognitionEngine> *	lItemObject;
		IDaCtlRecognitionEnginePtr				lItemInterface;

		for	(lNdx = 0; (mServerObject->get_Item (lNdx, &lServerObject) == S_OK); lNdx++)
		{
			if	(SUCCEEDED (CComObject <CDaCtlRecognitionEngine>::CreateInstance (&(lItemObject=NULL))))
			{
				lItemObject->mServerObject = lServerObject;
				lItemObject->SetOwner (mOwner);
				lItemInterface = (LPDISPATCH) lItemObject;
				mRecognitionEngines.Add (lItemInterface);
			}
		}
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::SetOwner (%d) [%p]"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef, _AtlModule.GetLockCount(), mServerObject.GetInterfacePtr());
	}
#endif
}

CDaControlObj * CDaCtlRecognitionEngines::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCtlRecognitionEngines::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDaCtlRecognitionEngines::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaCtlRecognitionEngines), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlRecognitionEngines::get_Item (VARIANT Index, IDaCtlRecognitionEngine **Item)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	INT_PTR	lItemNdx = -1;

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
			CString		lItemName = V_BSTR (&Index);
			tBstrPtr	lSRModeID;
			tBstrPtr	lDisplayName;

			if	(!lItemName.IsEmpty ())
			{
				for	(lItemNdx = 0; lItemNdx < (INT_PTR)mRecognitionEngines.GetCount(); lItemNdx++)
				{
					if	(
							(
								(SUCCEEDED (mRecognitionEngines [lItemNdx]->get_SRModeID (lSRModeID.Free())))
							&&	(lItemName.CompareNoCase (CString (lSRModeID)) == 0)
							)
						||	(
								(SUCCEEDED (mRecognitionEngines [lItemNdx]->get_DisplayName (lDisplayName.Free())))
							&&	(lItemName.CompareNoCase (CString (lDisplayName)) == 0)
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
			&&	(lItemNdx < (INT_PTR)mRecognitionEngines.GetCount())
			)
		{
			(*Item) = mRecognitionEngines [lItemNdx];
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::get_Item"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCtlRecognitionEngines::get_Count (long *Count)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)mRecognitionEngines.GetCount();
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::get_Count"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCtlRecognitionEngines::get__NewEnum (IUnknown **ppunkEnum)
{
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tPtr <CEnumVARIANT>		lEnum;
	tArrayPtr <CComVariant>	lArray;
	IEnumVARIANTPtr			lInterface;
	INT_PTR					lNdx;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(
				(lEnum = new CComObject <CEnumVARIANT>)
			&&	(lArray = new CComVariant [mRecognitionEngines.GetCount()+1])
			)
		{
			for	(lNdx = 0; lNdx < (INT_PTR)mRecognitionEngines.GetCount(); lNdx++)
			{
				lArray [lNdx] = mRecognitionEngines [lNdx].p;
			}

			if	(SUCCEEDED (lResult = lEnum->Init (&(lArray[0]), &(lArray[(INT_PTR)mRecognitionEngines.GetCount()]), (LPDISPATCH)this, AtlFlagCopy)))
			{
				lInterface = lEnum.Detach ();
				(*ppunkEnum) = lInterface.Detach ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%d)] CDaCtlRecognitionEngines::get__NewEnum"), SafeGetOwner(), SafeGetOwnerUsed(), this, m_dwRef);
	}
#endif
	return lResult;
}
