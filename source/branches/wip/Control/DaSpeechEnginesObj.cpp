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
#include "DaSpeechEnginesObj.h"
#include "DaSpeechEnginesEnum.h"
#include "DaSpeechEngineObj.h"
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

IMPLEMENT_DYNAMIC(CDaSpeechEnginesObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaSpeechEnginesObj, "")
IMPLEMENT_OLETYPELIB(CDaSpeechEnginesObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaSpeechEnginesObj::CDaSpeechEnginesObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" SpeechEngines")))
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

BEGIN_INTERFACE_MAP(CDaSpeechEnginesObj, CCmdTarget)
	INTERFACE_PART(CDaSpeechEnginesObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSpeechEnginesObj, __uuidof(IDaCtlSpeechEngines), SpeechEngines)
	INTERFACE_PART(CDaSpeechEnginesObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaSpeechEnginesObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSpeechEnginesObj, SpeechEngines)
IMPLEMENT_DISPATCH_IID(CDaSpeechEnginesObj, __uuidof(IDaCtlSpeechEngines))
IMPLEMENT_PROVIDECLASSINFO(CDaSpeechEnginesObj, __uuidof(CDaSpeechEnginesObj))

BEGIN_SUPPORTERRORINFO(CDaSpeechEnginesObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaSpeechEnginesObj, __uuidof(IDaCtlSpeechEngines))
END_SUPPORTERRORINFO(CDaSpeechEnginesObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSpeechEnginesObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaSpeechEnginesObj)
	DISP_PROPERTY_PARAM_ID(CDaSpeechEnginesObj, "Item", DISPID_VALUE, DspGetItem, DspSetItem, VT_DISPATCH, VTS_VARIANT)
	DISP_PROPERTY_EX_ID(CDaSpeechEnginesObj, "Count", DISPID_COLLECT, DspGetCount, DspSetCount, VT_I4)
	DISP_FUNCTION_ID(CDaSpeechEnginesObj, "_NewEnum", DISPID_NEWENUM, Dsp_NewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_DEFVALUE(CDaSpeechEnginesObj, "Item")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaSpeechEnginesObj::CDaSpeechEnginesObj (IDaSvrSpeechEngines * pServerObject)
:	mServerObject (pServerObject)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEnginesObj::CDaSpeechEnginesObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
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

CDaSpeechEnginesObj::~CDaSpeechEnginesObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEnginesObj::~CDaSpeechEnginesObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaSpeechEnginesObj::Initialize ()
{
	long					lNdx;
	IDaSvrSpeechEnginePtr	lServerObject;
	CDaSpeechEngineObj *	lItemObject;
	IDaCtlSpeechEnginePtr	lItemInterface;

	for	(lNdx = 0; (mServerObject->get_Item (lNdx, &lServerObject) == S_OK); lNdx++)
	{
		if	(lItemObject = new CDaSpeechEngineObj (lServerObject))
		{
			lItemObject->m_dwRef = 0;
			lItemInterface = lItemObject->GetIDispatch (FALSE);
			mSpeechEngines.Add (lItemInterface);
		}
	}
}

void CDaSpeechEnginesObj::Terminate (bool pFinal)
{
	if	(this)
	{
		if	(pFinal)
		{
			mSpeechEngines.RemoveAll ();
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

void CDaSpeechEnginesObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSpeechEnginesObj::OnFinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaSpeechEnginesObj::DspGetItem(VARIANT Index)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::DspGetItem"), this, m_dwRef);
#endif
	IDaCtlSpeechEngine *	lRet = NULL;
	HRESULT					lResult = m_xSpeechEngines.get_Item (Index, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEnginesObj::DspSetItem(VARIANT Index, LPDISPATCH Item)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::DspSetItem"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaSpeechEnginesObj::DspGetCount()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::DspGetCount"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xSpeechEngines.get_Count (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSpeechEnginesObj::DspSetCount(long Count)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::DspSetCount"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPUNKNOWN CDaSpeechEnginesObj::Dsp_NewEnum()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::Dsp_NewEnum"), this, m_dwRef);
#endif
	LPUNKNOWN	lRet = NULL;
	HRESULT		lResult = m_xSpeechEngines.get__NewEnum (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSpeechEnginesObj::XSpeechEngines::get_Item (VARIANT Index, IDaCtlSpeechEngine **Item)
{
	METHOD_PROLOGUE(CDaSpeechEnginesObj, SpeechEngines)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::XSpeechEngines::get_Item"), pThis, pThis->m_dwRef);
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
			tBstrPtr	lTTSModeID;
			tBstrPtr	lDisplayName;

			if	(!lItemName.IsEmpty ())
			{
				for	(lItemNdx = 0; lItemNdx <= pThis->mSpeechEngines.GetUpperBound(); lItemNdx++)
				{
					if	(
							(
								(SUCCEEDED (pThis->mSpeechEngines [lItemNdx]->get_TTSModeID (lTTSModeID.Free())))
							&&	(lItemName.CompareNoCase (CString (lTTSModeID)) == 0)
							)
						||	(
								(SUCCEEDED (pThis->mSpeechEngines [lItemNdx]->get_DisplayName (lDisplayName.Free())))
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
			&&	(lItemNdx <= pThis->mSpeechEngines.GetUpperBound())
			)
		{
			(*Item) = pThis->mSpeechEngines [lItemNdx];
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEnginesObj::XSpeechEngines::get_Item"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSpeechEnginesObj::XSpeechEngines::get_Count (long *Count)
{
	METHOD_PROLOGUE(CDaSpeechEnginesObj, SpeechEngines)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::XSpeechEngines::get_Count"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)pThis->mSpeechEngines.GetSize();
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEnginesObj::XSpeechEngines::get_Count"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSpeechEnginesObj::XSpeechEngines::get__NewEnum (IUnknown **ppunkEnum)
{
	METHOD_PROLOGUE(CDaSpeechEnginesObj, SpeechEngines)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSpeechEnginesObj::XSpeechEngines::get__NewEnum"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CDaSpeechEnginesEnum *	lEnum;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(lEnum = new CDaSpeechEnginesEnum (*pThis))
		{
			(*ppunkEnum) = lEnum->GetControllingUnknown ();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlSpeechEngines));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSpeechEnginesObj::XSpeechEngines::get__NewEnum"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
