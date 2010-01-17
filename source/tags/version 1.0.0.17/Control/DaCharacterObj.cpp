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
#include "DaCharacterObj.h"
#include "DaAgentCtl.h"
#include "DaBalloonObj.h"
#include "DaCommandsObj.h"
#include "DaAnimationNamesObj.h"
#include "ErrorInfo.h"
#include "OleVariantEx.h"
#include "StringArrayEx.h"
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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Character"),LogVerbose,true)&0xFFFF)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Character"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaCharacterObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaCharacterObj, "")
IMPLEMENT_OLETYPELIB(CDaCharacterObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaCharacterObj::CDaCharacterObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Character")))
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

BEGIN_INTERFACE_MAP(CDaCharacterObj, CCmdTarget)
	INTERFACE_PART(CDaCharacterObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaCharacterObj, __uuidof(IDaCtlCharacter), Character)
	INTERFACE_PART(CDaCharacterObj, __uuidof(IAgentCtlCharacter), Character)
	INTERFACE_PART(CDaCharacterObj, __uuidof(IAgentCtlCharacterEx), Character)
	INTERFACE_PART(CDaCharacterObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaCharacterObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaCharacterObj, Character)
IMPLEMENT_DISPATCH_IID(CDaCharacterObj, __uuidof(IDaCtlCharacter))
IMPLEMENT_PROVIDECLASSINFO(CDaCharacterObj, __uuidof(CDaCharacterObj))

BEGIN_SUPPORTERRORINFO(CDaCharacterObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaCharacterObj, __uuidof(IDaCtlCharacter))
	IMPLEMENT_SUPPORTERRORINFO(CDaCharacterObj, __uuidof(IAgentCtlCharacter))
	IMPLEMENT_SUPPORTERRORINFO(CDaCharacterObj, __uuidof(IAgentCtlCharacterEx))
END_SUPPORTERRORINFO(CDaCharacterObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaCharacterObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaCharacterObj)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Visible", DISPID_IAgentCtlCharacter_Visible, DspGetVisible, DspSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Left", DISPID_IAgentCtlCharacter_Left, DspGetLeft, DspSetLeft, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Top", DISPID_IAgentCtlCharacter_Top, DspGetTop, DspSetTop, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Height", DISPID_IAgentCtlCharacter_Height, DspGetHeight, DspSetHeight, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Width", DISPID_IAgentCtlCharacter_Width, DspGetWidth, DspSetWidth, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Speed", DISPID_IAgentCtlCharacter_Speed, DspGetSpeed, DspSetSpeed, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Pitch", DISPID_IAgentCtlCharacter_Pitch, DspGetPitch, DspSetPitch, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "IdleOn", DISPID_IAgentCtlCharacter_IdleOn, DspGetIdleOn, DspSetIdleOn, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Balloon", DISPID_IAgentCtlCharacter_Balloon, DspGetBalloon, DspSetBalloon, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Commands", DISPID_IAgentCtlCharacter_Commands, DspGetCommands, DspSetCommands, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Name", DISPID_IAgentCtlCharacter_Name, DspGetName, DspSetName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Description", DISPID_IAgentCtlCharacter_Description, DspGetDescription, DspSetDescription, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "MoveCause", DISPID_IAgentCtlCharacter_MoveCause, DspGetMoveCause, DspSetMoveCause, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "VisibilityCause", DISPID_IAgentCtlCharacter_VisibilityCause, DspGetVisibilityCause, DspSetVisibilityCause, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "HasOtherClients", DISPID_IAgentCtlCharacter_HasOtherClients, DspGetHasOtherClients, DspSetHasOtherClients, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "SoundEffectsOn", DISPID_IAgentCtlCharacter_SoundEffectsOn, DspGetSoundEffectsOn, DspSetSoundEffectsOn, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "ExtraData", DISPID_IAgentCtlCharacter_ExtraData, DspGetExtraData, DspSetExtraData, VT_BSTR)
	DISP_FUNCTION_ID(CDaCharacterObj, "Play", DISPID_IAgentCtlCharacter_Play, DspPlay, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CDaCharacterObj, "Stop", DISPID_IAgentCtlCharacter_Stop, DspStop, VT_EMPTY, VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "Speak", DISPID_IAgentCtlCharacter_Speak, DspSpeak, VT_DISPATCH, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "GestureAt", DISPID_IAgentCtlCharacter_GestureAt, DspGestureAt, VT_DISPATCH, VTS_I2 VTS_I2)
	DISP_FUNCTION_ID(CDaCharacterObj, "MoveTo", DISPID_IAgentCtlCharacter_MoveTo, DspMoveTo, VT_DISPATCH, VTS_I2 VTS_I2 VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "Hide", DISPID_IAgentCtlCharacter_Hide, DspHide, VT_DISPATCH, VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "Show", DISPID_IAgentCtlCharacter_Show, DspShow, VT_DISPATCH, VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "StopAll", DISPID_IAgentCtlCharacter_StopAll, DspStopAll, VT_EMPTY, VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "Interrupt", DISPID_IAgentCtlCharacter_Interrupt, DspInterrupt, VT_DISPATCH, VTS_DISPATCH)
	DISP_FUNCTION_ID(CDaCharacterObj, "Wait", DISPID_IAgentCtlCharacter_Wait, DspWait, VT_DISPATCH, VTS_DISPATCH)
	DISP_FUNCTION_ID(CDaCharacterObj, "Activate", DISPID_IAgentCtlCharacter_Activate, DspActivate, VT_BOOL, VTS_VARIANT)
	DISP_FUNCTION_ID(CDaCharacterObj, "Get", DISPID_IAgentCtlCharacter_Get, DspGet, VT_DISPATCH, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "AutoPopupMenu", DISPID_IAgentCtlCharacterEx_AutoPopupMenu, DspGetAutoPopupMenu, DspSetAutoPopupMenu, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "HelpModeOn", DISPID_IAgentCtlCharacterEx_HelpModeOn, DspGetHelpModeOn, DspSetHelpModeOn, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "HelpContextID", DISPID_IAgentCtlCharacterEx_HelpContextID, DspGetHelpContextID, DspSetHelpContextID, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Active", DISPID_IAgentCtlCharacterEx_Active, DspGetActive, DspSetActive, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "LanguageID", DISPID_IAgentCtlCharacterEx_LanguageID, DspGetLanguageID, DspSetLanguageID, VT_I4)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "SRModeID", DISPID_IAgentCtlCharacterEx_SRModeID, DspGetSRModeID, DspSetSRModeID, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "TTSModeID", DISPID_IAgentCtlCharacterEx_TTSModeID, DspGetTTSModeID, DspSetTTSModeID, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "HelpFile", DISPID_IAgentCtlCharacterEx_HelpFile, DspGetHelpFile, DspSetHelpFile, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "GUID", DISPID_IAgentCtlCharacterEx_GUID, DspGetGUID, DspSetGUID, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "OriginalHeight", DISPID_IAgentCtlCharacterEx_OriginalHeight, DspGetOriginalHeight, DspSetOriginalHeight, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "OriginalWidth", DISPID_IAgentCtlCharacterEx_OriginalWidth, DspGetOriginalWidth, DspSetOriginalWidth, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "Version", DISPID_IAgentCtlCharacterEx_Version, DspGetVersion, DspSetVersion, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "AnimationNames", DISPID_IAgentCtlCharacterEx_AnimationNames, DspGetAnimationNames, DspSetAnimationNames, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CDaCharacterObj, "SRStatus", DISPID_IAgentCtlCharacterEx_SRStatus, DspGetSRStatus, DspSetSRStatus, VT_I4)
	DISP_FUNCTION_ID(CDaCharacterObj, "ShowPopupMenu", DISPID_IAgentCtlCharacterEx_ShowPopupMenu, DspShowPopupMenu, VT_BOOL, VTS_I2 VTS_I2)
	DISP_FUNCTION_ID(CDaCharacterObj, "Listen", DISPID_IAgentCtlCharacterEx_Listen, DspListen, VT_BOOL, VTS_BOOL)
	DISP_FUNCTION_ID(CDaCharacterObj, "Think", DISPID_IAgentCtlCharacterEx_Think, DspThink, VT_DISPATCH, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCharacterObj::CDaCharacterObj (CDaAgentCtl & pOwner)
:	mOwner (&pOwner),
	mServerCharID (0),
	mBalloon (NULL),
	mCommands (NULL),
	mAnimationNames (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::CDaCharacterObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCharacterObj::~CDaCharacterObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::~CDaCharacterObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCharacterObj::Terminate (bool pFinal)
{
	HRESULT	lResult = S_FALSE;

	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::Terminate [%u] [%p] [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), mServerCharID);
		}
#endif
#endif
		try
		{
			CDaBalloonObj *			lBalloon;
			CDaCommandsObj *		lCommands;
			CDaAnimationNamesObj *	lAnimationNames;

			if	(
					(mBalloon != NULL)
				&&	(lBalloon = (CDaBalloonObj *)CCmdTarget::FromIDispatch (mBalloon))
				)
			{
				lBalloon->Terminate (pFinal);
			}
			if	(pFinal)
			{
				mBalloon = NULL;
			}

			if	(
					(mCommands != NULL)
				&&	(lCommands = (CDaCommandsObj *)CCmdTarget::FromIDispatch (mCommands))
				)
			{
				lCommands->Terminate (false);
			}
			if	(pFinal)
			{
				mCommands = NULL;
			}

			if	(
					(mAnimationNames != NULL)
				&&	(lAnimationNames = (CDaAnimationNamesObj *)CCmdTarget::FromIDispatch (mAnimationNames))
				)
			{
				lAnimationNames->Terminate (false);
				if	(pFinal)
				{
					mAnimationNames = NULL;
				}
			}

			if	(pFinal)
			{
				mOwner = NULL;
				mServerObject.Detach ();
			}
			else
			{
				SafeFreeSafePtr (mServerObject);

				if	(
						(mServerCharID)
					&&	(SUCCEEDED (TheControlApp->PreServerCall (mOwner->mServer)))
					)
				{
					try
					{
						lResult = mOwner->mServer->Unload (mServerCharID);
					}
					catch AnyExceptionDebug
					TheControlApp->PostServerCall (mOwner->mServer);
				}
			}

			mServerCharID = 0;
		}
		catch AnyExceptionDebug

#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr(), AfxOleCanExitApp());
		}
#endif
#endif
	}
	return lResult;
}

void CDaCharacterObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::OnFinalRelease [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaCharacterObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCharacterObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaBalloonObj * CDaCharacterObj::GetBalloon (IDaCtlBalloonPtr & pInterface)
{
	CDaBalloonObj *	lBalloon = NULL;

	if	(mBalloon == NULL)
	{
		if	(SUCCEEDED (TheControlApp->PreServerCall (mServerObject)))
		{
			try
			{
				if	(lBalloon = new CDaBalloonObj (*this))
				{
					mBalloon.Attach (lBalloon->GetIDispatch (FALSE));
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (mServerObject);
		}
	}
	if	(mBalloon != NULL)
	{
		lBalloon = (CDaBalloonObj *)CCmdTarget::FromIDispatch (mBalloon);
	}
	pInterface = mBalloon;

	return lBalloon;
}

CDaCommandsObj * CDaCharacterObj::GetCommands (IDaCtlCommandsPtr & pInterface)
{
	CDaCommandsObj *	lCommands = NULL;

	if	(mCommands == NULL)
	{
		if	(SUCCEEDED (TheControlApp->PreServerCall (mServerObject)))
		{
			try
			{
				if	(lCommands = new CDaCommandsObj (*this))
				{
					mCommands.Attach (lCommands->GetIDispatch (FALSE));
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (mServerObject);
		}
	}
	if	(mCommands != NULL)
	{
		lCommands = (CDaCommandsObj *)CCmdTarget::FromIDispatch (mCommands);
	}
	pInterface = mCommands;

	return lCommands;
}

CDaAnimationNamesObj * CDaCharacterObj::GetAnimationNames (IDaCtlAnimationNamesPtr & pInterface)
{
	CDaAnimationNamesObj *	lAnimationNames = NULL;

	if	(mAnimationNames == NULL)
	{
		if	(SUCCEEDED (TheControlApp->PreServerCall (mServerObject)))
		{
			try
			{
				if	(lAnimationNames = new CDaAnimationNamesObj (*this))
				{
					mAnimationNames.Attach (lAnimationNames->GetIDispatch (FALSE));
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (mServerObject);
		}
	}
	if	(mAnimationNames != NULL)
	{
		lAnimationNames = (CDaAnimationNamesObj *)CCmdTarget::FromIDispatch (mAnimationNames);
	}
	pInterface = mAnimationNames;

	return lAnimationNames;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CDaCharacterObj::DspGetVisible()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetVisible"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.get_Visible (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterObj::DspSetVisible(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetVisible"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCharacterObj::DspGetLeft()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetLeft"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Left (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetLeft(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetLeft"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_Left (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

short CDaCharacterObj::DspGetTop()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetTop"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Top (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetTop(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetTop"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_Top (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

short CDaCharacterObj::DspGetHeight()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetHeight"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Height (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetHeight(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetHeight"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_Height (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

short CDaCharacterObj::DspGetWidth()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetWidth"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Width (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetWidth(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetWidth"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_Width (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaCharacterObj::DspGetSpeed()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetSpeed"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Speed (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetSpeed(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetSpeed"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaCharacterObj::DspGetPitch()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetPitch"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Pitch (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetPitch(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetPitch"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaCharacterObj::DspPlay(LPCTSTR Animation)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspPlay"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.Play (_bstr_t(Animation), &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspStop(const VARIANT & Request)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspStop"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.Stop (Request);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

LPDISPATCH CDaCharacterObj::DspSpeak(const VARIANT & Text, const VARIANT & Url)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSpeak"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.Speak (Text, Url, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

BOOL CDaCharacterObj::DspGetIdleOn()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetIdleOn"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.get_IdleOn (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterObj::DspSetIdleOn(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetIdleOn"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_IdleOn (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

LPDISPATCH CDaCharacterObj::DspGestureAt(short x, short y)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGestureAt"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.GestureAt (x, y, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPDISPATCH CDaCharacterObj::DspMoveTo(short x, short y, const VARIANT & Speed)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspMoveTo"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.MoveTo (x, y, Speed, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPDISPATCH CDaCharacterObj::DspHide(const VARIANT & Fast)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspHide"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.Hide (Fast, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPDISPATCH CDaCharacterObj::DspShow(const VARIANT & Fast)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspShow"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.Show (Fast, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspStopAll(const VARIANT & Types)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspStopAll"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.StopAll (Types);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

LPDISPATCH CDaCharacterObj::DspInterrupt(LPDISPATCH InterruptRequest)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspInterrupt"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *		lRet = NULL;
	IDaCtlRequestPtr	lInterruptRequest (InterruptRequest);
	HRESULT				lResult = m_xCharacter.Interrupt (lInterruptRequest, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPDISPATCH CDaCharacterObj::DspWait(LPDISPATCH WaitForRequest)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspWait"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *		lRet = NULL;
	IDaCtlRequestPtr	lWaitForRequest (WaitForRequest);
	HRESULT				lResult = m_xCharacter.Wait (lWaitForRequest, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

LPDISPATCH CDaCharacterObj::DspGetBalloon()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetBalloon"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlBalloon *	lRet = NULL;
	HRESULT					lResult = m_xCharacter.get_Balloon (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetBalloon(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetBalloon"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaCharacterObj::DspGetCommands()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetCommands"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlCommands *	lRet = NULL;
	HRESULT				lResult = m_xCharacter.get_Commands (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetCommands(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetCommands"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaCharacterObj::DspGetName()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetName"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_Name (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetName(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetName"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_Name (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterObj::DspGetDescription()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetDescription"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_Description (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetDescription(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetDescription"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_Description (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterObj::DspActivate(const VARIANT & State)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspActivate"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.Activate (State ,&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

LPDISPATCH CDaCharacterObj::DspGet(LPCTSTR Type, LPCTSTR Name, const VARIANT & Queue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGet"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.Get (_bstr_t(Type), _bstr_t(Name), Queue, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

short CDaCharacterObj::DspGetMoveCause()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetMoveCause"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_MoveCause (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetMoveCause(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetMoveCause"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCharacterObj::DspGetVisibilityCause()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetVisibilityCause"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_VisibilityCause (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetVisibilityCause(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetVisibilityCause"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaCharacterObj::DspGetHasOtherClients()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetHasOtherClients"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.get_HasOtherClients (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterObj::DspSetHasOtherClients(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetHasOtherClients"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaCharacterObj::DspGetSoundEffectsOn()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetSoundEffectsOn"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.get_SoundEffectsOn (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterObj::DspSetSoundEffectsOn(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetSoundEffectsOn"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_SoundEffectsOn (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterObj::DspGetExtraData()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetExtraData"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_ExtraData (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetExtraData(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetExtraData"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDaCharacterObj::DspShowPopupMenu(short x, short y)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspShowPopupMenu"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.ShowPopupMenu (x, y, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

BOOL CDaCharacterObj::DspGetAutoPopupMenu()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetAutoPopupMenu"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.get_AutoPopupMenu (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterObj::DspSetAutoPopupMenu(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetAutoPopupMenu"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_AutoPopupMenu (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterObj::DspGetHelpModeOn()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetHelpModeOn"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.get_HelpModeOn (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterObj::DspSetHelpModeOn(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetHelpModeOn"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_HelpModeOn (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaCharacterObj::DspGetHelpContextID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetHelpContextID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_HelpContextID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetHelpContextID(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetHelpContextID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_HelpContextID (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

short CDaCharacterObj::DspGetActive()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetActive"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_Active (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetActive(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetActive"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaCharacterObj::DspListen(BOOL Listen)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspListen"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCharacter.Listen (Listen ? VARIANT_TRUE : VARIANT_FALSE, &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

long CDaCharacterObj::DspGetLanguageID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetLanguageID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_LanguageID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetLanguageID(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetLanguageID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_LanguageID (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterObj::DspGetSRModeID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetSRModeID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_SRModeID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetSRModeID(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetSRModeID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_SRModeID (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterObj::DspGetTTSModeID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetTTSModeID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_TTSModeID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetTTSModeID(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetTTSModeID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_TTSModeID (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterObj::DspGetHelpFile()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetHelpFile"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_HelpFile (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetHelpFile(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetHelpFile"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacter.put_HelpFile (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterObj::DspGetGUID()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetGUID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_GUID (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetGUID(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetGUID"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCharacterObj::DspGetOriginalHeight()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetOriginalHeight"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_OriginalHeight (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetOriginalHeight(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetOriginalHeight"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCharacterObj::DspGetOriginalWidth()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetOriginalWidth"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_OriginalWidth (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetOriginalWidth(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetOriginalWidth"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaCharacterObj::DspThink(LPCTSTR Text)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspThink"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlRequest *	lRet = NULL;
	HRESULT			lResult = m_xCharacter.Think (_bstr_t(Text), &lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

BSTR CDaCharacterObj::DspGetVersion()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetVersion"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacter.get_Version (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetVersion(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetVersion"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

LPDISPATCH CDaCharacterObj::DspGetAnimationNames()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetAnimationNames"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	IDaCtlAnimationNames *	lRet;
	HRESULT					lResult = m_xCharacter.get_AnimationNames (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetAnimationNames(LPDISPATCH newValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetAnimationNames"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaCharacterObj::DspGetSRStatus()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspGetSRStatus"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xCharacter.get_SRStatus (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterObj::DspSetSRStatus(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::DspSetSRStatus"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Balloon (IDaCtlBalloon **ppidBalloon)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Balloon"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDaCtlBalloonPtr	lInterface;

	if	(!ppidBalloon)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!pThis->GetBalloon (lInterface))
		{
			lResult = E_OUTOFMEMORY;
		}
		(*ppidBalloon) = lInterface.Detach ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Balloon"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Commands (IDaCtlCommands **ppidCommands)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Commands"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	IDaCtlCommandsPtr	lInterface;

	if	(!ppidCommands)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!pThis->GetCommands (lInterface))
		{
			lResult = E_OUTOFMEMORY;
		}
		(*ppidCommands) = lInterface.Detach();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Commands"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Name (BSTR *Name)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Name"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetName (Name);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Name"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Description (BSTR *Description)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Description"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetDescription (Description);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Description"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Visible (VARIANT_BOOL *Visible)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Visible"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Visible"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_Left (short Left)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Left"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft;
	long	lTop;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->GetPosition (&lLeft, &lTop)))
			{
				lResult = pThis->mServerObject->SetPosition (Left, lTop);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Left"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Left (short *Left)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Left"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft = 0;
	long	lTop = 0;

	if	(!Left)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetPosition (&lLeft, &lTop);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Left) = (short)lLeft;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Left"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_Top (short Top)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Top"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft;
	long	lTop;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->GetPosition (&lLeft, &lTop)))
			{
				lResult = pThis->mServerObject->SetPosition (lLeft, Top);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Top"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Top (short *Top)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Top"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lLeft = 0;
	long	lTop = 0;

	if	(!Top)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetPosition (&lLeft, &lTop);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Top) = (short)lTop;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Top"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_Height (short Height)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Height"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth;
	long	lHeight;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->GetSize (&lWidth, &lHeight)))
			{
				lResult = pThis->mServerObject->SetSize (lWidth, Height);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Height"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Height (short *Height)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Height"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Height)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Height) = (short)lHeight;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Height"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_Width (short Width)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Width"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth;
	long	lHeight;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->GetSize (&lWidth, &lHeight)))
			{
				lResult = pThis->mServerObject->SetSize (Width, lHeight);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Width"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Width (short *Width)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Width"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Width) = (short)lWidth;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Width"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Speed (long *Speed)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Speed"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetTTSSpeed (Speed);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Speed"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Pitch (long *Pitch)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Pitch"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	short	lPitch = 0;

	if	(!Pitch)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetTTSPitch (&lPitch);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Pitch) = lPitch;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Pitch"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_IdleOn (VARIANT_BOOL On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_IdleOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetIdleOn (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_IdleOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_IdleOn (VARIANT_BOOL *On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_IdleOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	long	lIdleOn = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetIdleOn (&lIdleOn);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(On)
	{
		(*On) = lIdleOn ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_IdleOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Activate (VARIANT State, VARIANT_BOOL *Success)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Activate"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	HRESULT	lResult;
	short	lState = 2;

	if	(!IsEmptyParm (&State))
	{
		try
		{
			lState = (short)_variant_t(State);
		}
		catch AnyExceptionSilent
	}

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Activate (lState);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(Success)
	{
		(*Success) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Activate"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Play (BSTR Animation, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Play [%s]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, DebugStr(CString(Animation)));
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Play (Animation, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestPlay, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Play"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Get (BSTR Type, BSTR Name, VARIANT Queue, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Get"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	long				lPrepareType;
	long				lPrepareQueue = TRUE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(_wcsicmp (Type, L"Animation") == 0)
	{
		lPrepareType = PREPARE_ANIMATION;
	}
	else
	if	(_wcsicmp (Type, L"State") == 0)
	{
		lPrepareType = PREPARE_STATE;
	}
	else
	if	(_wcsicmp (Type, L"WaveFile") == 0)
	{
		lPrepareType = PREPARE_WAVE;
	}
	else
	{
		lResult = E_INVALIDARG;
	}
	if	(!IsEmptyParm (&Queue))
	{
		try
		{
			lPrepareQueue = (long)(bool)_variant_t(Queue);
		}
		catch AnyExceptionSilent;
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		)
	{
		try
		{
			lResult = pThis->mServerObject->Prepare (lPrepareType, Name, lPrepareQueue,	&lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestPrepare, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Get"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Stop (VARIANT Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Stop [%u] [%p]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, V_VT (&Request), V_DISPATCH (&Request));
#endif

	HRESULT				lResult;
	IDaCtlRequestPtr	lRequest;
	long				lReqID = 0;

	if	(V_VT (&Request) == VT_DISPATCH)
	{
		lRequest = V_DISPATCH (&Request);
	}

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(lRequest == NULL)
			{
				if	(
						(V_VT (&Request) == VT_EMPTY)
					||	(V_VT (&Request) == VT_ERROR)
					)
				{
					lResult = pThis->mServerObject->StopAll (STOP_TYPE_PLAY|STOP_TYPE_MOVE|STOP_TYPE_SPEAK|STOP_TYPE_PREPARE);
				}
				else
				{
					lResult = E_INVALIDARG;
				}
			}
			else
			if	(SUCCEEDED (lResult = lRequest->get_ID (&lReqID)))
			{
				lResult = pThis->mServerObject->Stop (lReqID);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Stop"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Wait (IDaCtlRequest *WaitForRequest, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Wait"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult;
	long				lWaitForReqID = 0;
	IDaCtlRequestPtr	lWaitForRequest (WaitForRequest);
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(lWaitForRequest == NULL)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = lWaitForRequest->get_ID (&lWaitForReqID)))
			{
				lResult = pThis->mServerObject->Wait (lWaitForReqID, &lReqID);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Wait"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Interrupt (IDaCtlRequest *InterruptRequest, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Interrupt"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	HRESULT				lResult;
	long				lInterruptReqID = 0;
	IDaCtlRequestPtr	lInterruptRequest (InterruptRequest);
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(lInterruptRequest == NULL)
	{
		lResult = E_INVALIDARG;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = lInterruptRequest->get_ID (&lInterruptReqID)))
			{
				lResult = pThis->mServerObject->Interrupt (lInterruptReqID, &lReqID);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Interrupt"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Speak (VARIANT Text, VARIANT Url, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();

	HRESULT				lResult = S_OK;
	_bstr_t				lText;
	_bstr_t				lUrl;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Text))
	{
		try
		{
			lText = (_bstr_t)_variant_t(Text);
		}
		catch AnyExceptionSilent
	}

	if	(!IsEmptyParm (&Url))
	{
		try
		{
			lUrl = (_bstr_t)_variant_t(Url);
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Speak [%s] [%s]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, DebugStr(CString((BSTR)lText)), DebugStr(CString((BSTR)lUrl)));
#endif

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Speak (lText, lUrl, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestSpeak, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Speak"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::GestureAt (short x, short y, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::GestureAt"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GestureAt (x, y, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::GestureAt"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::MoveTo (short x, short y, VARIANT Speed, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::MoveTo"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult;
	long				lSpeed = 1000;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Speed))
	{
		try
		{
			lSpeed = (long)_variant_t(Speed);
		}
		catch AnyExceptionSilent
	}

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->MoveTo (x, y, lSpeed, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestOther, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::MoveTo"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Hide (VARIANT Fast, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();

	HRESULT				lResult;
	long				lFast = FALSE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Fast))
	{
		try
		{
			lFast = (long)(bool)_variant_t(Fast);
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Hide [%d]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, lFast);
#endif

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Hide (lFast, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestHide, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Hide"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Show (VARIANT Fast, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();

	HRESULT				lResult;
	long				lFast = FALSE;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(!IsEmptyParm (&Fast))
	{
		try
		{
			lFast = (long)(bool)_variant_t(Fast);
		}
		catch AnyExceptionSilent
	}
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Show [%d]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, lFast);
#endif

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Show (lFast, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestShow, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Show"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::StopAll (VARIANT Types)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::StopAll"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT			lResult = S_OK;
	long			lStopTypes = 0;
	CStringArray	lTypeNames;

	if	(IsEmptyParm (&Types))
	{
		lStopTypes = STOP_TYPE_ALL;
	}
	else
	{
		try
		{
			MakeStringArray (CString ((BSTR)(_bstr_t)_variant_t(Types)), lTypeNames, _T(","));
		}
		catch AnyExceptionSilent

		if	(lTypeNames.GetSize () > 0)
		{
			for	(int lNdx = 0; lNdx <= lTypeNames.GetUpperBound(); lNdx++)
			{
				if	(lTypeNames [lNdx].CompareNoCase (_T("Get")) == 0)
				{
					lStopTypes |= STOP_TYPE_PREPARE;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("NonQueuedGet")) == 0)
				{
					lStopTypes |= STOP_TYPE_NONQUEUEDPREPARE;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Move")) == 0)
				{
					lStopTypes |= STOP_TYPE_MOVE;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Play")) == 0)
				{
					lStopTypes |= STOP_TYPE_PLAY;
				}
				else
				if	(lTypeNames [lNdx].CompareNoCase (_T("Speak")) == 0)
				{
					lStopTypes |= STOP_TYPE_SPEAK;
				}
				else
				{
					lResult = E_INVALIDARG;
					break;
				}
			}
		}
		else
		{
			lResult = E_INVALIDARG;
		}
	}

	if	(
			(SUCCEEDED (lResult))
		&&	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		)
	{
		try
		{
			lResult = pThis->mServerObject->StopAll (lStopTypes);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::StopAll"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_MoveCause (short *MoveCause)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_MoveCause"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lMoveCause = 0;

	if	(!MoveCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetMoveCause (&lMoveCause);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*MoveCause) = (short)lMoveCause;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_MoveCause"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_VisibilityCause (short *VisibilityCause)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_VisibilityCause"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lVisibilityCause = 0;

	if	(!VisibilityCause)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVisibilityCause (&lVisibilityCause);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*VisibilityCause) = (short)lVisibilityCause;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_VisibilityCause"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_HasOtherClients (VARIANT_BOOL *HasOtherClients)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HasOtherClients"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	HRESULT	lResult;
	long	lOtherClients = 0;

	if	(!HasOtherClients)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->HasOtherClients (&lOtherClients);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*HasOtherClients) = (lOtherClients > 0) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HasOtherClients"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_SoundEffectsOn (VARIANT_BOOL On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_SoundEffectsOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetSoundEffectsOn (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_SoundEffectsOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_SoundEffectsOn (VARIANT_BOOL *On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_SoundEffectsOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif

	long	lSoundOn = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetSoundEffectsOn (&lSoundOn);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(On)
	{
		(*On) = lSoundOn ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_SoundEffectsOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_Name (BSTR Name)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Name"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetName (Name);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Name"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_Description (BSTR Description)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Description"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetDescription (Description);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_Description"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_ExtraData (BSTR *ExtraData)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_ExtraData"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetExtraData (ExtraData);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_ExtraData"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::ShowPopupMenu (short x, short y, VARIANT_BOOL *Showed)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::ShowPopupMenu"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->ShowPopupMenu (x, y);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(Showed)
	{
		(*Showed) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::ShowPopupMenu"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_AutoPopupMenu (VARIANT_BOOL On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_AutoPopupMenu"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetAutoPopupMenu (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_AutoPopupMenu"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_AutoPopupMenu (VARIANT_BOOL *On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_AutoPopupMenu"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lAutoPopupMenu = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetAutoPopupMenu (&lAutoPopupMenu);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(On)
	{
		(*On) = lAutoPopupMenu ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_AutoPopupMenu"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_HelpModeOn (VARIANT_BOOL On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_HelpModeOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetHelpModeOn (On!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_HelpModeOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_HelpModeOn (VARIANT_BOOL *On)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HelpModeOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lHelpModeOn = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetHelpModeOn (&lHelpModeOn);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(On)
	{
		(*On) = lHelpModeOn ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HelpModeOn"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_HelpContextID (long ID)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_HelpContextID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_HelpContextID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_HelpContextID (long *ID)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HelpContextID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetHelpContextID (ID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HelpContextID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Active (short *State)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Active"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetActive (State);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Active"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Listen (VARIANT_BOOL Listen, VARIANT_BOOL *StartedListening)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Listen"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Listen (Listen!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(StartedListening)
	{
		(*StartedListening) = (lResult == S_OK) ? Listen : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Listen"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_LanguageID (long LanguageID)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_LanguageID [%4.4X]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, LanguageID);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetLanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_LanguageID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_LanguageID (long *LanguageID)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_LanguageID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetLanguageID (LanguageID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_LanguageID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_SRModeID (BSTR *EngineModeId)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_SRModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetSRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_SRModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_SRModeID (BSTR EngineModeId)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_SRModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetSRModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_SRModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_TTSModeID (BSTR *EngineModeId)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_TTSModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetTTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_TTSModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_TTSModeID (BSTR EngineModeId)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_TTSModeID [%ls]"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, EngineModeId);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetTTSModeID (EngineModeId);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_TTSModeID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_HelpFile (BSTR *File)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HelpFile"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetHelpFileName (File);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_HelpFile"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::put_HelpFile (BSTR File)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_HelpFile"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetHelpFileName (File);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::put_HelpFile"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_GUID (BSTR *GUID)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_GUID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetGUID (GUID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_GUID"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_OriginalHeight (short *Height)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_OriginalHeight"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Height)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetOriginalSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Height) = (short)lHeight;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_OriginalHeight"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_OriginalWidth (short *Width)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_OriginalWidth"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lWidth = 0;
	long	lHeight = 0;

	if	(!Width)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetOriginalSize (&lWidth, &lHeight);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*Width) = (short)lWidth;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_OriginalWidth"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::Think (BSTR Text, IDaCtlRequest **Request)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Think"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult;
	long				lReqID = 0;
	IDaCtlRequestPtr	lRequest;

	if	(Request)
	{
		(*Request) = NULL;
	}
	pThis->mOwner->CompleteRequests ();

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->Think (Text, &lReqID);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	if	(
			(lReqID)
		&&	(Request)
		)
	{
		lRequest.Attach (pThis->mOwner->PutRequest (DaRequestSpeak, lReqID, lResult));
		(*Request) = lRequest.Detach();
	}
	if	(
			(FAILED (lResult))
		&&	(!pThis->mOwner->mRaiseRequestErrors)
		)
	{
		lResult = S_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::Think"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_Version (BSTR *Version)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Version"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	short	lVerMaj = 0;
	short	lVerMin = 0;
	CString	lVerStr;

	if	(!Version)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetVersion (&lVerMaj, &lVerMin);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		lVerStr.Format (_T("%hd.%hd"), lVerMaj, lVerMin);
		(*Version) = lVerStr.AllocSysString ();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_Version"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_AnimationNames (IDaCtlAnimationNames **Names)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_AnimationNames"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	IDaCtlAnimationNamesPtr	lInterface;

	if	(!Names)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(!pThis->GetAnimationNames (lInterface))
		{
			lResult = E_OUTOFMEMORY;
		}
		(*Names) = lInterface.Detach();
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_AnimationNames"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterObj::XCharacter::get_SRStatus (long *Status)
{
	METHOD_PROLOGUE(CDaCharacterObj, Character)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_SRStatus"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetSRStatus (Status);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacter));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCharacterObj::XCharacter::get_SRStatus"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
