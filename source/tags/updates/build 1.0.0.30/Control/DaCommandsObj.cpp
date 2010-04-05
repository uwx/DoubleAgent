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
#include "DaCommandsObj.h"
#include "DaCommandsEnum.h"
#include "DaCommandObj.h"
#include "DaCharacterObj.h"
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

IMPLEMENT_DYNAMIC(CDaCommandsObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaCommandsObj, "")
IMPLEMENT_OLETYPELIB(CDaCommandsObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaCommandsObj::CDaCommandsObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Commands")))
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

BEGIN_INTERFACE_MAP(CDaCommandsObj, CCmdTarget)
	INTERFACE_PART(CDaCommandsObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaCommandsObj, __uuidof(IDaCtlCommands), Commands)
	INTERFACE_PART(CDaCommandsObj, __uuidof(IAgentCtlCommands), Commands)
	INTERFACE_PART(CDaCommandsObj, __uuidof(IAgentCtlCommandsEx), Commands)
	INTERFACE_PART(CDaCommandsObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaCommandsObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaCommandsObj, Commands)
IMPLEMENT_DISPATCH_IID(CDaCommandsObj, __uuidof(IDaCtlCommands))
IMPLEMENT_PROVIDECLASSINFO(CDaCommandsObj, __uuidof(CDaCommandsObj))

BEGIN_SUPPORTERRORINFO(CDaCommandsObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandsObj, __uuidof(IDaCtlCommands))
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandsObj, __uuidof(IAgentCtlCommands))
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandsObj, __uuidof(IAgentCtlCommandsEx))
END_SUPPORTERRORINFO(CDaCommandsObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaCommandsObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaCommandsObj)
	DISP_PROPERTY_PARAM_ID(CDaCommandsObj, "Item", DISPID_VALUE, DspGetItem, DspSetItem, VT_DISPATCH, VTS_BSTR)
	DISP_DEFVALUE(CDaCommandsObj, "Item")
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "Count", DISPID_IAgentCtlCommands_Count, DspGetCount, DspSetCount, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "Caption", DISPID_IAgentCtlCommands_Caption, DspGetCaption, DspSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "Voice", DISPID_IAgentCtlCommands_Voice, DspGetVoice, DspSetVoice, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "Visible", DISPID_IAgentCtlCommands_Visible, DspGetVisible, DspSetVisible, VT_BOOL)
	DISP_FUNCTION_ID(CDaCommandsObj, "Command", DISPID_IAgentCtlCommands_Command, DspCommand, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CDaCommandsObj, "Add", DISPID_IAgentCtlCommands_Add, DspAdd, VT_DISPATCH, VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCommandsObj, "Insert", DISPID_IAgentCtlCommands_Insert, DspInsert, VT_DISPATCH, VTS_BSTR VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCommandsObj, "Remove", DISPID_IAgentCtlCommands_Remove, DspRemove, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CDaCommandsObj, "RemoveAll", DISPID_IAgentCtlCommands_RemoveAll, DspRemoveAll, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "DefaultCommand", DISPID_IAgentCtlCommandsEx_DefaultCommand, DspGetDefaultCommand, DspSetDefaultCommand, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "HelpContextID", DISPID_IAgentCtlCommandsEx_HelpContextID, DspGetHelpContextID, DspSetHelpContextID, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "FontName", DISPID_IAgentCtlCommandsEx_FontName, DspGetFontName, DspSetFontName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "FontSize", DISPID_IAgentCtlCommandsEx_FontSize, DspGetFontSize, DspSetFontSize, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "VoiceCaption", DISPID_IAgentCtlCommandsEx_VoiceCaption, DspGetVoiceCaption, DspSetVoiceCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCommandsObj, "GlobalVoiceCommandsEnabled", DISPID_IAgentCtlCommandsEx_GlobalVoiceCommandsEnabled, DspGetGlobalVoiceCommandsEnabled, DspSetGlobalVoiceCommandsEnabled, VT_BOOL)
	DISP_FUNCTION_ID(CDaCommandsObj, "Enum", DISPID_NEWENUM, DspEnum, VT_UNKNOWN, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCommandsObj::CDaCommandsObj (CDaCharacterObj & pOwner)
:	mOwner (&pOwner),
	mServerObject (pOwner.mServerObject)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::CDaCommandsObj (%d) [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCommandsObj::~CDaCommandsObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::~CDaCommandsObj (%d) [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mCommands != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj Attached [%p] Owner [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mOwner->mCommands.GetInterfacePtr(), mOwner);
		}
	}
	catch AnyExceptionSilent
#endif

	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCommandsObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::Terminate [%u] (%d)"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mCommands.GetSize());
		}
#endif
#endif
		try
		{
			int				lNdx;
			CDaCommandObj *	lCommand;

			for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
			{
				try
				{
					if	(lCommand = (CDaCommandObj *) CCmdTarget::FromIDispatch (mCommands [lNdx]))
					{
						lCommand->Terminate (pFinal);
					}
				}
				catch AnyExceptionSilent

				if	(pFinal)
				{
					mCommands [lNdx] = NULL;
				}
			}
		}
		catch AnyExceptionDebug

		if	(pFinal)
		{
			mCommands.RemoveAll ();
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::Terminate [%u] Done [%d]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaCommandsObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::OnFinalRelease [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaCharacterObj * CDaCommandsObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCommandsObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCommandsObj::RemoveAll ()
{
	HRESULT	lResult;

	try
	{
		mCommands.RemoveAll ();
	}
	catch AnyExceptionDebug

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (mServerObject)))
	{
		try
		{
			lResult = mServerObject->RemoveAll ();
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (mServerObject);
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCommandObj * CDaCommandsObj::GetCommand (LPCTSTR pCommandName)
{
	CDaCommandObj *	lCommand = NULL;
	INT_PTR			lNdx = mCommands.FindKey (pCommandName);

	if	(lNdx >= 0)
	{
		lCommand = (CDaCommandObj *)CCmdTarget::FromIDispatch (mCommands [lNdx]);
	}
	return lCommand;
}

CDaCommandObj * CDaCommandsObj::GetCommand (long pCommandId)
{
	CDaCommandObj *	lCommand = NULL;
	int				lNdx;

	if	(pCommandId > 0)
	{
		for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
		{
			if	(
					(lCommand = (CDaCommandObj *)CCmdTarget::FromIDispatch (mCommands [lNdx]))
				&&	(lCommand->mServerId == pCommandId)
				)
			{
				break;
			}
			lCommand = NULL;
		}
	}
	return lCommand;
}

/////////////////////////////////////////////////////////////////////////////

CString CDaCommandsObj::GetCommandName (long pCommandId)
{
	CString			lCommandName;
	CDaCommandObj *	lCommand;
	int				lNdx;

	if	(pCommandId > 0)
	{
		for	(lNdx = 0; lNdx <= mCommands.GetUpperBound(); lNdx++)
		{
			if	(
					(lCommand = (CDaCommandObj *)CCmdTarget::FromIDispatch (mCommands [lNdx]))
				&&	(lCommand->mServerId == pCommandId)
				)
			{
				lCommandName = mCommands.KeyAt (lNdx);
				break;
			}
		}
	}
	return lCommandName;
}

long CDaCommandsObj::GetCommandId (LPCTSTR pCommandName)
{
	long			lCommandId = 0;
	CDaCommandObj *	lCommand;
	INT_PTR			lNdx = mCommands.FindKey (pCommandName);

	if	(
			(lNdx >= 0)
		&&	(lCommand = (CDaCommandObj *)CCmdTarget::FromIDispatch (mCommands [lNdx]))
		)
	{
		lCommandId = lCommand->mServerId;
	}
	return lCommandId;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPDISPATCH CDaCommandsObj::DspGetItem(LPCTSTR Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetItem"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCommand *	lRet = NULL;
	HRESULT			lResult = m_xCommands.get_Item (_bstr_t(Name), &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetItem(LPCTSTR Name, LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetItem"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaCommandsObj::DspCommand(LPCTSTR Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspCommand"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCommand *	lRet = NULL;
	HRESULT			lResult = m_xCommands.Command (_bstr_t(Name), &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

long CDaCommandsObj::DspGetCount()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetCount"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCommands.get_Count (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetCount(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetCount"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaCommandsObj::DspGetCaption()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommands.get_Caption (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetCaption(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_Caption (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandsObj::DspGetVoice()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommands.get_Voice (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetVoice(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetVoice"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_Voice (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCommandsObj::DspGetVisible()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetVisible"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCommands.get_Visible (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandsObj::DspSetVisible(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetVisible"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_Visible (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

LPDISPATCH CDaCommandsObj::DspAdd(LPCTSTR Name, const VARIANT & Caption, const VARIANT & Voice, const VARIANT & Enabled, const VARIANT & Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspAdd"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCommand *	lRet = NULL;
	HRESULT			lResult = m_xCommands.Add (_bstr_t(Name), Caption, Voice, Enabled, Visible, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPDISPATCH CDaCommandsObj::DspInsert(LPCTSTR Name, LPCTSTR RefName, const VARIANT & Before, const VARIANT & Caption, const VARIANT & Voice, const VARIANT & Enabled, const VARIANT & Visible)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspInsert"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCommand *	lRet = NULL;
	HRESULT			lResult = m_xCommands.Insert (_bstr_t(Name), _bstr_t(RefName), Before, Caption, Voice, Enabled, Visible, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspRemove(LPCTSTR Name)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspRemove"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.Remove (_bstr_t(Name));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

void CDaCommandsObj::DspRemoveAll()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspRemoveAll"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.RemoveAll ();
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandsObj::DspGetDefaultCommand()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetDefaultCommand"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommands.get_DefaultCommand (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetDefaultCommand(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetDefaultCommand"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_DefaultCommand (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaCommandsObj::DspGetHelpContextID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetHelpContextID"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCommands.get_HelpContextID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandsObj::DspSetHelpContextID(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetHelpContextID"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_HelpContextID (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandsObj::DspGetFontName()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetFontName"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommands.get_FontName (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetFontName(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetFontName"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_FontName (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaCommandsObj::DspGetFontSize()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetFontSize"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCommands.get_FontSize (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandsObj::DspSetFontSize(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetFontSize"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_FontSize (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCommandsObj::DspGetVoiceCaption()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetVoiceCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCommands.get_VoiceCaption (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsObj::DspSetVoiceCaption(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetVoiceCaption"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_VoiceCaption (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCommandsObj::DspGetGlobalVoiceCommandsEnabled()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspGetGlobalVoiceCommandsEnabled"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCommands.get_GlobalVoiceCommandsEnabled (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandsObj::DspSetGlobalVoiceCommandsEnabled(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspSetGlobalVoiceCommandsEnabled"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommands.put_GlobalVoiceCommandsEnabled (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

LPUNKNOWN CDaCommandsObj::DspEnum()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::DspEnum"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	LPUNKNOWN	lRet = NULL;
	HRESULT		lResult = m_xCommands.get_Enum (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_Item (BSTR Name, IDaCtlCommand **Item)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Item"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CString				lItemName (Name);
	IDispatchPtr		lItemDispatch;
	IDaCtlCommandPtr	lItem;

	if	(!Item)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Item) = NULL;

		if	(pThis->mCommands.Lookup (lItemName, lItemDispatch))
		{
			lItem = lItemDispatch;
			(*Item) = lItem.Detach();
		}
		else
		{
			lResult = AGENTERR_COMMANDNOTFOUND;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Item"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::Command (BSTR Name, IDaCtlCommand **Item)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Command"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	HRESULT	lResult = get_Item (Name, Item);

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Command"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_Count (long *Count)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Count"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Count)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Count) = (long)pThis->mCommands.GetSize();
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Count"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_Caption (BSTR *Caption)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Caption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Caption) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetCaption (Caption);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_Caption (BSTR Caption)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetCaption (Caption);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_Caption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_Voice (BSTR *Voice)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Voice)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Voice) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVoice (Voice);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_Voice (BSTR Voice)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVoice (Voice);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_Voice"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_Visible (VARIANT_BOOL *Visible)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lVisible = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetVisible (&lVisible);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(Visible)
	{
		(*Visible) = lVisible ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_Visible (VARIANT_BOOL Visible)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVisible (Visible!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_Enum (IUnknown **ppunkEnum)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Enum"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CDaCommandsEnum *	lEnum;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;

		if	(lEnum = new CDaCommandsEnum (*pThis))
		{
			(*ppunkEnum) = lEnum->GetControllingUnknown ();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_Enum"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::Add (BSTR Name, VARIANT Caption, VARIANT Voice, VARIANT Enabled, VARIANT Visible, IDaCtlCommand **Command)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Add"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	CString					lName (Name);
	tPtr <CDaCommandObj>	lCommand;
	_bstr_t					lCaption;
	_bstr_t					lVoice;
	long					lEnabled = TRUE;
	long					lVisible = TRUE;

	if	(IsEmptyParm (&Caption))
	{
		lCaption = lName;
	}
	else
	{
		try
		{
			lCaption = (bstr_t)_variant_t(Caption);
		}
		catch AnyExceptionSilent
	}
	if	(IsEmptyParm (&Voice))
	{
		lVoice = lName;
	}
	else
	{
		try
		{
			lVoice = (bstr_t)_variant_t(Voice);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Enabled))
	{
		try
		{
			lEnabled = (long)(bool)_variant_t(Enabled);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Visible))
	{
		try
		{
			lVisible = (long)(bool)_variant_t(Visible);
		}
		catch AnyExceptionSilent
	}

	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(pThis->mCommands.FindKey (lName) >= 0)
	{
		lResult = AGENTERR_COMMANDALREADYINUSE;
	}
	else
	if	(!Command)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(lCommand = new CDaCommandObj (*pThis->mOwner))
			{
				lResult = pThis->mServerObject->Add (lCaption, lVoice, lEnabled, lVisible, &lCommand->mServerId);
				if	(SUCCEEDED (lResult))
				{
					lResult = pThis->mServerObject->GetCommandEx (lCommand->mServerId, &lCommand->mServerObject);
				}
				if	(
						(lCommand->mServerObject != NULL)
					&&	(lCommand->mServerId != 0)
					)
				{
					pThis->mCommands.SetAt (lName, lCommand->GetIDispatch(FALSE));
					lCommand->InternalRelease ();
					lCommand.Detach ();
				}
				else
				{
					if	(lCommand->mServerId != 0)
					{
						pThis->mServerObject->Remove (lCommand->mServerId);
					}
					if	(SUCCEEDED (lResult))
					{
						lResult = E_FAIL;
					}
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Add"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::Insert (BSTR Name, BSTR RefName, VARIANT Before, VARIANT Caption, VARIANT Voice, VARIANT Enabled, VARIANT Visible, IDaCtlCommand **Command)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Insert"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	CString					lName (Name);
	CString					lRefName (RefName);
	CDaCommandObj *			lRefCommand = NULL;
	tPtr <CDaCommandObj>	lCommand;
	_bstr_t					lCaption;
	_bstr_t					lVoice;
	long					lEnabled = TRUE;
	long					lVisible = TRUE;
	long					lBefore = TRUE;

	if	(IsEmptyParm (&Caption))
	{
		lCaption = lName;
	}
	else
	{
		try
		{
			lCaption = (bstr_t)_variant_t(Caption);
		}
		catch AnyExceptionSilent
	}
	if	(IsEmptyParm (&Voice))
	{
		lVoice = lName;
	}
	else
	{
		try
		{
			lVoice = (bstr_t)_variant_t(Voice);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Enabled))
	{
		try
		{
			lEnabled = (long)(bool)_variant_t(Enabled);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Visible))
	{
		try
		{
			lVisible = (long)(bool)_variant_t(Visible);
		}
		catch AnyExceptionSilent
	}
	if	(!IsEmptyParm (&Before))
	{
		try
		{
			lBefore = (long)(bool)_variant_t(Before);
		}
		catch AnyExceptionSilent
	}

	if	(
			(lName.IsEmpty ())
		||	(lRefName.IsEmpty ())
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(pThis->mCommands.FindKey (lName) >= 0)
	{
		lResult = AGENTERR_COMMANDALREADYINUSE;
	}
	else
	if	((lRefCommand = pThis->GetCommand (lRefName)) == NULL)
	{
		lResult = AGENTERR_COMMANDNOTFOUND;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(lCommand = new CDaCommandObj (*pThis->mOwner))
			{
				lResult = pThis->mServerObject->Insert (lCaption, lVoice, lEnabled, lVisible, lRefCommand->mServerId, lBefore, &lCommand->mServerId);
				if	(SUCCEEDED (lResult))
				{
					lResult = pThis->mServerObject->GetCommandEx (lCommand->mServerId, &lCommand->mServerObject);
				}
				if	(
						(lCommand->mServerObject != NULL)
					&&	(lCommand->mServerId != 0)
					)
				{
					pThis->mCommands.SetAt (lName, lCommand->GetIDispatch(FALSE));
					lCommand->InternalRelease ();
					lCommand.Detach ();
				}
				else
				{
					if	(lCommand->mServerId != 0)
					{
						pThis->mServerObject->Remove (lCommand->mServerId);
					}
					if	(SUCCEEDED (lResult))
					{
						lResult = E_FAIL;
					}
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Insert"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::Remove (BSTR Name)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Remove"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT			lResult;
	CString			lName (Name);
	CDaCommandObj *	lCommand;

	if	(lName.IsEmpty ())
	{
		lResult = E_INVALIDARG;
	}
	else
	if	((lCommand = pThis->GetCommand (lName)) == NULL)
	{
		lResult = AGENTERR_COMMANDNOTFOUND;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			SafeFreeSafePtr (lCommand->mServerObject);
			lResult = pThis->mServerObject->Remove (lCommand->mServerId);
			pThis->mCommands.RemoveKey (lName);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::Remove"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::RemoveAll (void)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::RemoveAll"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = pThis->RemoveAll ();

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::RemoveAll"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_DefaultCommand (BSTR *Name)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_DefaultCommand"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lDefaultId = 0;
	CString	lDefaultName;

	if	(!Name)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetDefaultID (&lDefaultId);
				lDefaultName = pThis->GetCommandName (lDefaultId);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}

		(*Name) = lDefaultName.AllocSysString ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_DefaultCommand"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_DefaultCommand (BSTR Name)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_DefaultCommand"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lDefaultName (Name);
	long	lDefaultId;

	if	(lDefaultName.IsEmpty ())
	{
		lDefaultId = 0;
	}
	else
	{
		lDefaultId = pThis->GetCommandId (lDefaultName);
		if	(lDefaultId <= 0)
		{
			lResult = AGENTERR_COMMANDNOTFOUND;
		}
	}
	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		)
	{
		try
		{
			lResult = pThis->mServerObject->SetDefaultID (lDefaultId);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_DefaultCommand"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_HelpContextID (long *ID)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!ID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ID) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetHelpContextID (ID);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_HelpContextID (long ID)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetHelpContextID (ID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_HelpContextID"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_FontName (BSTR *FontName)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontName) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetFontName (FontName);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_FontName (BSTR FontName)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetFontName (FontName);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_FontSize (long *FontSize)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FontSize) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetFontSize (FontSize);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_FontSize (long FontSize)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetFontSize (FontSize);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_VoiceCaption (BSTR *VoiceCaption)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!VoiceCaption)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VoiceCaption) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVoiceCaption (VoiceCaption);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_VoiceCaption (BSTR VoiceCaption)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVoiceCaption (VoiceCaption);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_VoiceCaption"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::get_GlobalVoiceCommandsEnabled (VARIANT_BOOL *Enable)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_GlobalVoiceCommandsEnabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lEnable = FALSE;
	HRESULT	lResult;

	if	(!Enable)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetGlobalVoiceCommandsEnabled (&lEnable);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Enable) = lEnable ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::get_GlobalVoiceCommandsEnabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsObj::XCommands::put_GlobalVoiceCommandsEnabled (VARIANT_BOOL Enable)
{
	METHOD_PROLOGUE(CDaCommandsObj, Commands)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_GlobalVoiceCommandsEnabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetGlobalVoiceCommandsEnabled (Enable!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommands));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaCommandsObj::XCommands::put_GlobalVoiceCommandsEnabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
