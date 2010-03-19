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
#include "DaRecognitionEnginesObj.h"
#include "DaRecognitionEnginesEnum.h"
#include "DaRecognitionEngineObj.h"
#include "DaAgentCtl.h"
#include "OleVariantEx.h"
#include "ErrorInfo.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
#define	_DEBUG_DSPINTERFACE		(GetProfileDebugInt(_T("DebugDspInterface"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaRecognitionEnginesObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaRecognitionEnginesObj, "")
IMPLEMENT_OLETYPELIB(CDaRecognitionEnginesObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaRecognitionEnginesObj::CDaRecognitionEnginesObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" RecognitionEngines")))
	{
		if	(bRegister)
		{
			RegisterApartmentThreaded ();
			RegisterTypeLib (gDaTypeLibId);
			RegisterDefCategory ();
			RegisterCategory (m_clsid, CATID_Programmable);
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CDaRecognitionEnginesObj, CCmdTarget)
	INTERFACE_PART(CDaRecognitionEnginesObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaRecognitionEnginesObj, __uuidof(IDaCtlRecognitionEngines), RecognitionEngines)
	INTERFACE_PART(CDaRecognitionEnginesObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaRecognitionEnginesObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaRecognitionEnginesObj, RecognitionEngines)
IMPLEMENT_DISPATCH_IID(CDaRecognitionEnginesObj, __uuidof(IDaCtlRecognitionEngines))
IMPLEMENT_PROVIDECLASSINFO(CDaRecognitionEnginesObj, __uuidof(CDaRecognitionEnginesObj))

BEGIN_SUPPORTERRORINFO(CDaRecognitionEnginesObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaRecognitionEnginesObj, __uuidof(IDaCtlRecognitionEngines))
END_SUPPORTERRORINFO(CDaRecognitionEnginesObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaRecognitionEnginesObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaRecognitionEnginesObj)
	DISP_PROPERTY_PARAM_ID(CDaRecognitionEnginesObj, "Item", DISPID_VALUE, DspGetItem, DspSetItem, VT_DISPATCH, VTS_VARIANT)
	DISP_PROPERTY_EX_ID(CDaRecognitionEnginesObj, "Count", DISPID_COLLECT, DspGetCount, DspSetCount, VT_I4)
	DISP_FUNCTION_ID(CDaRecognitionEnginesObj, "_NewEnum", DISPID_NEWENUM, Dsp_NewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_DEFVALUE(CDaRecognitionEnginesObj, "Item")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaRecognitionEnginesObj::CDaRecognitionEnginesObj (IDaSvrRecognitionEngines * pServerObject)
:	mServerObject (pServerObject)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEnginesObj::CDaRecognitionEnginesObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	Initialize ();
	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaRecognitionEnginesObj::~CDaRecognitionEnginesObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEnginesObj::~CDaRecognitionEnginesObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaRecognitionEnginesObj::Initialize ()
{
	long						lNdx;
	IDaSvrRecognitionEnginePtr	lServerObject;
	CDaRecognitionEngineObj *	lItemObject;
	IDaCtlRecognitionEnginePtr	lItemInterface;

	for	(lNdx = 0; (mServerObject->get_Item (lNdx, &lServerObject) == S_OK); lNdx++)
	{
		if	(lItemObject = new CDaRecognitionEngineObj (lServerObject))
		{
			lItemObject->m_dwRef = 0;
			lItemInterface = lItemObject->GetIDispatch (FALSE);
			mRecognitionEngines.Add (lItemInterface);
		}
	}
}

void CDaRecognitionEnginesObj::Terminate (bool pFinal)
{
	if	(this)
	{
		if	(pFinal)
		{
			mRecognitionEngines.RemoveAll ();
		}
		if	(pFinal)
		{
			mServerObject.Detach ();
		}
		else
		{
			SafeFreeSafePtr (mServerObject);
		}
	}
}

void CDaRecognitionEnginesObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaRecognitionEnginesObj::OnFinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaRecognitionEnginesObj::DspGetItem(VARIANT Index)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::DspGetItem"), this, m_dwRef);
#endif
	IDaCtlRecognitionEngine *	lRet = NULL;
	HRESULT					lResult = m_xRecognitionEngines.get_Item (Index, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEnginesObj::DspSetItem(VARIANT Index, LPDISPATCH Item)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::DspSetItem"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaRecognitionEnginesObj::DspGetCount()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::DspGetCount"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xRecognitionEngines.get_Count (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaRecognitionEnginesObj::DspSetCount(long Count)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::DspSetCount"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPUNKNOWN CDaRecognitionEnginesObj::Dsp_NewEnum()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::Dsp_NewEnum"), this, m_dwRef);
#endif
	LPUNKNOWN	lRet = NULL;
	HRESULT		lResult = m_xRecognitionEngines.get__NewEnum (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaRecognitionEnginesObj::XRecognitionEngines::get_Item (VARIANT Index, IDaCtlRecognitionEngine **Item)
{
	METHOD_PROLOGUE(CDaRecognitionEnginesObj, RecognitionEngines)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::XRecognitionEngines::get_Item"), pThis, pThis->m_dwRef);
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
				for	(lItemNdx = 0; lItemNdx <= pThis->mRecognitionEngines.GetUpperBound(); lItemNdx++)
				{
					if	(
							(
								(SUCCEEDED (pThis->mRecognitionEngines [lItemNdx]->get_SRModeID (lSRModeID.Free())))
							&&	(lItemName.CompareNoCase (CString (lSRModeID)) == 0)
							)
						||	(
								(SUCCEEDED (pThis->mRecognitionEngines [lItemNdx]->get_DisplayName (lDisplayName.Free())))
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
			&&	(lItemNdx <= pThis->mRecognitionEngines.GetUpperBound())
			)
		{
			(*Item) = pThis->mRecognitionEngines [lItemNdx];
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEnginesObj::XRecognitionEngines::get_Item"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaRecognitionEnginesObj::XRecognitionEngines::get_Count (long *Count)
{
	METHOD_PROLOGUE(CDaRecognitionEnginesObj, RecognitionEngines)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::XRecognitionEngines::get_Count"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)pThis->mRecognitionEngines.GetSize();
	}

	PutControlError (lResult, __uuidof(IDaCtlRecognitionEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEnginesObj::XRecognitionEngines::get_Count"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaRecognitionEnginesObj::XRecognitionEngines::get__NewEnum (IUnknown **ppunkEnum)
{
	METHOD_PROLOGUE(CDaRecognitionEnginesObj, RecognitionEngines)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaRecognitionEnginesObj::XRecognitionEngines::get__NewEnum"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CDaRecognitionEnginesEnum *	lEnum;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(lEnum = new CDaRecognitionEnginesEnum (*pThis))
		{
			(*ppunkEnum) = lEnum->GetControllingUnknown ();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaRecognitionEnginesObj::XRecognitionEngines::get__NewEnum"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
