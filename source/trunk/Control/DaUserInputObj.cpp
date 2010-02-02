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
#include "DaUserInputObj.h"
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

IMPLEMENT_DYNAMIC(CDaUserInputObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaUserInputObj, "")
IMPLEMENT_OLETYPELIB(CDaUserInputObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaUserInputObj::CDaUserInputObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" User Input")))
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

BEGIN_INTERFACE_MAP(CDaUserInputObj, CCmdTarget)
	INTERFACE_PART(CDaUserInputObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaUserInputObj, __uuidof(IDaCtlUserInput), UserInput)
	INTERFACE_PART(CDaUserInputObj, __uuidof(IAgentCtlUserInput), UserInput)
	INTERFACE_PART(CDaUserInputObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaUserInputObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaUserInputObj, UserInput)
IMPLEMENT_DISPATCH_IID(CDaUserInputObj, __uuidof(IDaCtlUserInput))
IMPLEMENT_PROVIDECLASSINFO(CDaUserInputObj, __uuidof(CDaUserInputObj))

BEGIN_SUPPORTERRORINFO(CDaUserInputObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaUserInputObj, __uuidof(IDaCtlUserInput))
	IMPLEMENT_SUPPORTERRORINFO(CDaUserInputObj, __uuidof(IAgentCtlUserInput))
END_SUPPORTERRORINFO(CDaUserInputObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaUserInputObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaUserInputObj)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Count", DISPID_IAgentCtlUserInput_Count, DspGetCount, DspSetCount, VT_I2)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Name", DISPID_IAgentCtlUserInput_Name, DspGetName, DspSetName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "CharacterID", DISPID_IAgentCtlUserInput_CharacterID, DspGetCharacterID, DspSetCharacterID, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Confidence", DISPID_IAgentCtlUserInput_Confidence, DspGetConfidence, DspSetConfidence, VT_I4)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Voice", DISPID_IAgentCtlUserInput_Voice, DspGetVoice, DspSetVoice, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Alt1Name", DISPID_IAgentCtlUserInput_Alt1Name, DspGetAlt1Name, DspSetAlt1Name, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Alt1Confidence", DISPID_IAgentCtlUserInput_Alt1Confidence, DspGetAlt1Confidence, DspSetAlt1Confidence, VT_I4)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Alt1Voice", DISPID_IAgentCtlUserInput_Alt1Voice, DspGetAlt1Voice, DspSetAlt1Voice, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Alt2Name", DISPID_IAgentCtlUserInput_Alt2Name, DspGetAlt2Name, DspSetAlt2Name, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Alt2Confidence", DISPID_IAgentCtlUserInput_Alt2Confidence, DspGetAlt2Confidence, DspSetAlt2Confidence, VT_I4)
	DISP_PROPERTY_EX_ID(CDaUserInputObj, "Alt2Voice", DISPID_IAgentCtlUserInput_Alt2Voice, DspGetAlt2Voice, DspSetAlt2Voice, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaUserInputObj::CDaUserInputObj()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaUserInputObj::CDaUserInputObj (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaUserInputObj::~CDaUserInputObj()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaUserInputObj::~CDaUserInputObj (%d) [%p]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaUserInputObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaUserInputObj::Terminate [%u] [%p]"), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
		}
#endif
#endif
		if	(pFinal)
		{
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaUserInputObj::Terminate [%u] Done [%d]"), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaUserInputObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CDaUserInputObj::OnFinalRelease [%p]"), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

short CDaUserInputObj::DspGetCount()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetCount"), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xUserInput.get_Count (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetCount(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetCount"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetName()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetName"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_Name (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetName(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetName"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetCharacterID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetCharacterID"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_CharacterID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetCharacterID(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetCharacterID"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaUserInputObj::DspGetConfidence()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetConfidence"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xUserInput.get_Confidence (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetConfidence(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetConfidence"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetVoice()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetVoice"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_Voice (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetVoice(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetVoice"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetAlt1Name()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetAlt1Name"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_Alt1Name (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetAlt1Name(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetAlt1Name"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaUserInputObj::DspGetAlt1Confidence()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetAlt1Confidence"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xUserInput.get_Alt1Confidence (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetAlt1Confidence(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetAlt1Confidence"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetAlt1Voice()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetAlt1Voice"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_Alt1Voice (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetAlt1Voice(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetAlt1Voice"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetAlt2Name()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetAlt2Name"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_Alt2Name (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetAlt2Name(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetAlt2Name"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaUserInputObj::DspGetAlt2Confidence()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetAlt2Confidence"), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xUserInput.get_Alt2Confidence (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetAlt2Confidence(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetAlt2Confidence"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaUserInputObj::DspGetAlt2Voice()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspGetAlt2Voice"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xUserInput.get_Alt2Voice (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaUserInputObj::DspSetAlt2Voice(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] CDaUserInputObj::DspSetAlt2Voice"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Count (short *pCount)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Count"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lCount = 0;

	if	(!pCount)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetCount (&lCount);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}

		(*pCount) = (short)lCount;
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Count"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Name (BSTR *pName)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Name"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pName) = NULL;

		if	(
				(pThis->mServerCommands != NULL)
			&&	(SUCCEEDED (TheControlApp->PreServerCall (pThis->mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommandPtr	lCommand;

				if	(
						(SUCCEEDED (pThis->mServerObject->GetItemID (0, &lCommandId)))
					&&	(SUCCEEDED (pThis->mServerCommands->GetCommandEx (lCommandId, &lCommand)))
					)
				{
					lCommand->GetCaption (pName);
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Name"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_CharacterID (BSTR *pCharacterID)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_CharacterID"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pCharacterID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pCharacterID) = pThis->mCharacterID.AllocSysString();
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_CharacterID"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Confidence (long *pConfidence)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Confidence"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pConfidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pConfidence) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetItemConfidence (0, pConfidence);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Confidence"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Voice (BSTR *pVoice)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Voice"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pVoice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pVoice) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetItemText (0, pVoice);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Voice"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Alt1Name (BSTR *pAlt1Name)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt1Name"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pAlt1Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt1Name) = NULL;

		if	(
				(pThis->mServerCommands != NULL)
			&&	(SUCCEEDED (TheControlApp->PreServerCall (pThis->mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommandPtr	lCommand;

				if	(
						(SUCCEEDED (pThis->mServerObject->GetItemID (1, &lCommandId)))
					&&	(SUCCEEDED (pThis->mServerCommands->GetCommandEx (lCommandId, &lCommand)))
					)
				{
					lCommand->GetCaption (pAlt1Name);
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt1Name"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Alt1Confidence (long *pAlt1Confidence)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt1Confidence"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pAlt1Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt1Confidence) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetItemConfidence (1, pAlt1Confidence);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt1Confidence"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Alt1Voice (BSTR *pAlt1Voice)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt1Voice"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pAlt1Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt1Voice) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetItemText (1, pAlt1Voice);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt1Voice"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Alt2Name (BSTR *pAlt2Name)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt2Name"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!pAlt2Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt2Name) = NULL;

		if	(
				(pThis->mServerCommands != NULL)
			&&	(SUCCEEDED (TheControlApp->PreServerCall (pThis->mServerObject)))
			)
		{
			try
			{
				long				lCommandId = -1;
				IDaSvrCommandPtr	lCommand;

				if	(
						(SUCCEEDED (pThis->mServerObject->GetItemID (2, &lCommandId)))
					&&	(SUCCEEDED (pThis->mServerCommands->GetCommandEx (lCommandId, &lCommand)))
					)
				{
					lCommand->GetCaption (pAlt2Name);
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt2Name"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Alt2Confidence (long *pAlt2Confidence)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt2Confidence"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!pAlt2Confidence)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt2Confidence) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetItemConfidence (2, pAlt2Confidence);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt2Confidence"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaUserInputObj::XUserInput::get_Alt2Voice (BSTR *pAlt2Voice)
{
	METHOD_PROLOGUE(CDaUserInputObj, UserInput)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt2Voice"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;

	if	(!pAlt2Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pAlt2Voice) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetItemText (2, pAlt2Voice);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		else
		{
			lResult = S_FALSE;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlUserInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CDaUserInputObj::XUserInput::get_Alt2Voice"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
