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
#include "DaCommandsWindowObj.h"
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

IMPLEMENT_DYNAMIC(CDaCommandsWindowObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaCommandsWindowObj, "")
IMPLEMENT_OLETYPELIB(CDaCommandsWindowObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaCommandsWindowObj::CDaCommandsWindowObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Commands Window")))
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

BEGIN_INTERFACE_MAP(CDaCommandsWindowObj, CCmdTarget)
	INTERFACE_PART(CDaCommandsWindowObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaCommandsWindowObj, __uuidof(IDaCtlCommandsWindow), CommandsWindow)
	INTERFACE_PART(CDaCommandsWindowObj, __uuidof(IAgentCtlCommandsWindow), CommandsWindow)
	INTERFACE_PART(CDaCommandsWindowObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaCommandsWindowObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaCommandsWindowObj, CommandsWindow)
IMPLEMENT_DISPATCH_IID(CDaCommandsWindowObj, __uuidof(IDaCtlCommandsWindow))
IMPLEMENT_PROVIDECLASSINFO(CDaCommandsWindowObj, __uuidof(CDaCommandsWindowObj))

BEGIN_SUPPORTERRORINFO(CDaCommandsWindowObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandsWindowObj, __uuidof(IDaCtlCommandsWindow))
	IMPLEMENT_SUPPORTERRORINFO(CDaCommandsWindowObj, __uuidof(IAgentCtlCommandsWindow))
END_SUPPORTERRORINFO(CDaCommandsWindowObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaCommandsWindowObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaCommandsWindowObj)
	DISP_PROPERTY_EX_ID(CDaCommandsWindowObj, "Visible", DISPID_IAgentCtlCommandsWindow_Visible, DspGetVisible, DspSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCommandsWindowObj, "Left", DISPID_IAgentCtlCommandsWindow_Left, DspGetLeft, DspSetLeft, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCommandsWindowObj, "Top", DISPID_IAgentCtlCommandsWindow_Top, DspGetTop, DspSetTop, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCommandsWindowObj, "Height", DISPID_IAgentCtlCommandsWindow_Height, DspGetHeight, DspSetHeight, VT_I2)
	DISP_PROPERTY_EX_ID(CDaCommandsWindowObj, "Width", DISPID_IAgentCtlCommandsWindow_Width, DspGetWidth, DspSetWidth, VT_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCommandsWindowObj::CDaCommandsWindowObj (CDaAgentCtl & pOwner)
:	mOwner (&pOwner),
	mServerObject (pOwner.mServer)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::CDaCommandsWindowObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCommandsWindowObj::~CDaCommandsWindowObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::~CDaCommandsWindowObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mCommandsWindow != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj Attached [%p] Owner [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mOwner->mCommandsWindow.GetInterfacePtr(), mOwner);
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

void CDaCommandsWindowObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::Terminate [%u] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
		}
#endif
#endif
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaCommandsWindowObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::OnFinalRelease [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaCommandsWindowObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCommandsWindowObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CDaCommandsWindowObj::DspGetVisible()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspGetVisible"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xCommandsWindow.get_Visible (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCommandsWindowObj::DspSetVisible(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspSetVisible"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCommandsWindow.put_Visible (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

short CDaCommandsWindowObj::DspGetLeft()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspGetLeft"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCommandsWindow.get_Left (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsWindowObj::DspSetLeft(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspSetLeft"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCommandsWindowObj::DspGetTop()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspGetTop"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCommandsWindow.get_Top (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsWindowObj::DspSetTop(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspSetTop"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCommandsWindowObj::DspGetHeight()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspGetHeight"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCommandsWindow.get_Height (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsWindowObj::DspSetHeight(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspSetHeight"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaCommandsWindowObj::DspGetWidth()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspGetWidth"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = 0;
	HRESULT	lResult = m_xCommandsWindow.get_Width (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCommandsWindowObj::DspSetWidth(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::DspSetWidth"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsWindowObj::XCommandsWindow::get_Visible (VARIANT_BOOL *Visible)
{
	METHOD_PROLOGUE(CDaCommandsWindowObj, CommandsWindow)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Visible"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	long	lVisible = 0;

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

	PutControlError (lResult, __uuidof(IDaCtlCommandsWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Visible"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsWindowObj::XCommandsWindow::put_Visible (VARIANT_BOOL Visible)
{
	METHOD_PROLOGUE(CDaCommandsWindowObj, CommandsWindow)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::put_Visible"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetVisible (Visible!=VARIANT_FALSE);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCommandsWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::put_Visible"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCommandsWindowObj::XCommandsWindow::get_Left (short *Left)
{
	METHOD_PROLOGUE(CDaCommandsWindowObj, CommandsWindow)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Left"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
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

	PutControlError (lResult, __uuidof(IDaCtlCommandsWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Left"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsWindowObj::XCommandsWindow::get_Top (short *Top)
{
	METHOD_PROLOGUE(CDaCommandsWindowObj, CommandsWindow)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Top"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
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

	PutControlError (lResult, __uuidof(IDaCtlCommandsWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Top"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsWindowObj::XCommandsWindow::get_Height (short *Height)
{
	METHOD_PROLOGUE(CDaCommandsWindowObj, CommandsWindow)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Height"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
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

	PutControlError (lResult, __uuidof(IDaCtlCommandsWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Height"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCommandsWindowObj::XCommandsWindow::get_Width (short *Width)
{
	METHOD_PROLOGUE(CDaCommandsWindowObj, CommandsWindow)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Width"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
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

	PutControlError (lResult, __uuidof(IDaCtlCommandsWindow));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] CDaCommandsWindowObj::XCommandsWindow::get_Width"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
