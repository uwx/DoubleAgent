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
#include "DaBalloonObj.h"
#include "DaCharacterObj.h"
#include "DaAgentCtl.h"
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
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Balloon"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Balloon"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif
#include "InterfaceMap.inl"

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDaBalloonObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaBalloonObj, "")
IMPLEMENT_OLETYPELIB(CDaBalloonObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaBalloonObj::CDaBalloonObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Balloon")))
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

BEGIN_INTERFACE_MAP(CDaBalloonObj, CCmdTarget)
	INTERFACE_PART(CDaBalloonObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaBalloonObj, __uuidof(IDaCtlBalloon), Balloon)
	INTERFACE_PART(CDaBalloonObj, __uuidof(IAgentCtlBalloon), Balloon)
	INTERFACE_PART(CDaBalloonObj, __uuidof(IAgentCtlBalloonEx), Balloon)
	INTERFACE_PART(CDaBalloonObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaBalloonObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaBalloonObj, Balloon)
IMPLEMENT_DISPATCH_IID(CDaBalloonObj, __uuidof(IDaCtlBalloon))
IMPLEMENT_PROVIDECLASSINFO(CDaBalloonObj, __uuidof(CDaBalloonObj))

BEGIN_SUPPORTERRORINFO(CDaBalloonObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaBalloonObj, __uuidof(IDaCtlBalloon))
	IMPLEMENT_SUPPORTERRORINFO(CDaBalloonObj, __uuidof(IAgentCtlBalloon))
	IMPLEMENT_SUPPORTERRORINFO(CDaBalloonObj, __uuidof(IAgentCtlBalloonEx))
END_SUPPORTERRORINFO(CDaBalloonObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaBalloonObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaBalloonObj)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "Enabled", DISPID_IAgentCtlBalloon_Enabled, DspGetEnabled, DspSetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "NumberOfLines", DISPID_IAgentCtlBalloon_NumberOfLines, DspGetNumberOfLines, DspSetNumberOfLines, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "CharsPerLine", DISPID_IAgentCtlBalloon_CharsPerLine, DspGetCharsPerLine, DspSetCharsPerLine, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontName", DISPID_IAgentCtlBalloon_FontName, DspGetFontName, DspSetFontName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontSize", DISPID_IAgentCtlBalloon_FontSize, DspGetFontSize, DspSetFontSize, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "ForeColor", DISPID_IAgentCtlBalloon_ForeColor, DspGetForeColor, DspSetForeColor, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "BackColor", DISPID_IAgentCtlBalloon_BackColor, DspGetBackColor, DspSetBackColor, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "BorderColor", DISPID_IAgentCtlBalloon_BorderColor, DspGetBorderColor, DspSetBorderColor, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontBold", DISPID_IAgentCtlBalloon_FontBold, DspGetFontBold, DspSetFontBold, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontItalic", DISPID_IAgentCtlBalloon_FontItalic, DspGetFontItalic, DspSetFontItalic, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontStrikethru", DISPID_IAgentCtlBalloon_FontStrikethru, DspGetFontStrikethru, DspSetFontStrikethru, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontUnderline", DISPID_IAgentCtlBalloon_FontUnderline, DspGetFontUnderline, DspSetFontUnderline, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "FontCharSet", DISPID_IAgentCtlBalloon_FontCharSet, DspGetFontCharSet, DspSetFontCharSet, VT_I2)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "Style", DISPID_IAgentCtlBalloonEx_Style, DspGetStyle, DspSetStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CDaBalloonObj, "Visible", DISPID_IAgentCtlBalloon_Visible, DspGetVisible, DspSetVisible, VT_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaBalloonObj::CDaBalloonObj (CDaCharacterObj & pOwner)
:	mOwner (&pOwner),
	mServerObject (pOwner.mServerObject)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::CDaBalloonObj (%d) [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaBalloonObj::~CDaBalloonObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::~CDaBalloonObj (%d) [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
#ifdef	_DEBUG
	try
	{
		if	(
				(mOwner)
			&&	(mOwner->mBalloon != NULL)
			)
		{
			LogMessage (LogNormal, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj Attached [%p] Owner [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mOwner->mBalloon.GetInterfacePtr(), mOwner);
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

void CDaBalloonObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::Terminate [%u] [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::Terminate [%u] Done [%d]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaBalloonObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::OnFinalRelease [%p]"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaCharacterObj * CDaBalloonObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaBalloonObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BOOL CDaBalloonObj::DspGetEnabled()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetEnabled"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xBalloon.get_Enabled (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaBalloonObj::DspSetEnabled(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetEnabled"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaBalloonObj::DspGetNumberOfLines()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetNumberOfLines"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_NumberOfLines (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetNumberOfLines(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetNumberOfLines"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaBalloonObj::DspGetCharsPerLine()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetCharsPerLine"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_CharsPerLine (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetCharsPerLine(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetCharsPerLine"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaBalloonObj::DspGetFontName()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontName"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xBalloon.get_FontName (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetFontName(LPCTSTR lpszNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontName"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.put_FontName (_bstr_t(lpszNewValue));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaBalloonObj::DspGetFontSize()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontSize"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_FontSize (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetFontSize(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontSize"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.put_FontSize (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaBalloonObj::DspGetForeColor()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetForeColor"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_ForeColor (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetForeColor(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetForeColor"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaBalloonObj::DspGetBackColor()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetBackColor"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_BackColor (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetBackColor(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetBackColor"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaBalloonObj::DspGetBorderColor()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetBorderColor"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_BorderColor (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetBorderColor(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetBorderColor"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaBalloonObj::DspGetFontBold()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontBold"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xBalloon.get_FontBold (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaBalloonObj::DspSetFontBold(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontBold"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaBalloonObj::DspGetFontItalic()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontItalic"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xBalloon.get_FontItalic (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaBalloonObj::DspSetFontItalic(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontItalic"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaBalloonObj::DspGetFontStrikethru()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontStrikethru"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xBalloon.get_FontStrikethru (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaBalloonObj::DspSetFontStrikethru(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontStrikethru"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BOOL CDaBalloonObj::DspGetFontUnderline()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontUnderline"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xBalloon.get_FontUnderline (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaBalloonObj::DspSetFontUnderline(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontUnderline"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

short CDaBalloonObj::DspGetFontCharSet()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetFontCharSet"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	short	lRet = DEFAULT_CHARSET;
	HRESULT	lResult = m_xBalloon.get_FontCharSet (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetFontCharSet(short nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetFontCharSet"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.put_FontCharSet (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

long CDaBalloonObj::DspGetStyle()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetStyle"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	long	lRet = 0;
	HRESULT	lResult = m_xBalloon.get_Style (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaBalloonObj::DspSetStyle(long nNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetStyle"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xBalloon.put_Style (nNewValue);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaBalloonObj::DspGetVisible()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspGetVisible"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet = VARIANT_FALSE;
	HRESULT			lResult = m_xBalloon.get_Visible (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaBalloonObj::DspSetVisible(BOOL bNewValue)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::DspSetVisible"), mOwner->SafeGetOwner(), mOwner->SafeGetOwnerUsed(), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT			lResult = m_xBalloon.put_Visible (bNewValue ? VARIANT_TRUE : VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_Enabled (VARIANT_BOOL *Enabled)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_Enabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lEnabled = 0;
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetEnabled (&lEnabled);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}
	if	(Enabled)
	{
		(*Enabled) = lEnabled ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_Enabled"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_NumberOfLines (long *Lines)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_NumberOfLines"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Lines)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Lines) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetNumLines (Lines);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_NumberOfLines"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_CharsPerLine (long *CharsPerLine)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_CharsPerLine"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!CharsPerLine)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*CharsPerLine) = 0;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetNumCharsPerLine (CharsPerLine);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_CharsPerLine"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontName (BSTR *FontName)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontSize (long *FontSize)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	CDC		lDC;

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

		if	(
				(SUCCEEDED (lResult))
			&&	(lDC.CreateCompatibleDC (NULL))
			)
		{
			CPoint	lFontSize (0, (*FontSize));
			CPoint	lOrigin (0, 0);

			lDC.LPtoDP (&lFontSize, 1);
			lDC.LPtoDP (&lOrigin, 1);
			lFontSize.y = abs(lOrigin.y - lFontSize.y);
			(*FontSize) = MulDiv (lFontSize.y, 72, lDC.GetDeviceCaps (LOGPIXELSY));
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontBold (VARIANT_BOOL *FontBold)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontBold"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lFontBold = FALSE;
	HRESULT	lResult;

	if	(!FontBold)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetFontBold (&lFontBold);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*FontBold) = lFontBold ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontBold"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontItalic (VARIANT_BOOL *FontItalic)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontItalic"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lFontItalic = FALSE;
	HRESULT	lResult;

	if	(!FontItalic)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetFontItalic (&lFontItalic);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*FontItalic) = lFontItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontItalic"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontStrikethru (VARIANT_BOOL *FontStrikethru)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontStrikethru"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lFontStrikethru = FALSE;
	HRESULT	lResult;

	if	(!FontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetFontStrikethru (&lFontStrikethru);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*FontStrikethru) = lFontStrikethru ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontStrikethru"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontUnderline (VARIANT_BOOL *FontUnderline)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontUnderline"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	long	lFontUnderline = FALSE;
	HRESULT	lResult;

	if	(!FontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->GetFontUnderline (&lFontUnderline);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
		(*FontUnderline) = lFontUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontUnderline"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_ForeColor (long *ForeColor)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_ForeColor"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!ForeColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetForeColor (ForeColor);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_ForeColor"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_BackColor (long *BackColor)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_BackColor"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!BackColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetBackColor (BackColor);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_BackColor"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_BorderColor (long *BorderColor)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_BorderColor"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!BorderColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetBorderColor (BorderColor);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_BorderColor"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::put_Visible (VARIANT_BOOL Visible)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_Visible (VARIANT_BOOL *Visible)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_Visible"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::put_FontName (BSTR FontName)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
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

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_FontName"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::put_FontSize (long FontSize)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	CDC		lDC;

	if	(lDC.CreateCompatibleDC (NULL))
	{
		CPoint	lFontSize (0, 0);
		CPoint	lOrigin (0, 0);

		lFontSize.y = MulDiv (FontSize, lDC.GetDeviceCaps (LOGPIXELSY), 72);
		lDC.DPtoLP (&lFontSize, 1);
		lDC.DPtoLP (&lOrigin, 1);
		FontSize = -abs(lFontSize.y - lOrigin.y);
	}

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetFontSize (FontSize);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_FontSize"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::put_FontCharSet (short FontCharSet)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_FontCharSet"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetFontCharSet (FontCharSet);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_FontCharSet"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_FontCharSet (short *FontCharSet)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontCharSet"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FontCharSet)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->GetFontCharSet (FontCharSet);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_FontCharSet"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::put_Style (long Style)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_Style [%8.8X]"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef, Style);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->SetStyle ((long)(ULONG)LOWORD(Style));
			if	(
					(SUCCEEDED (lResult))
				&&	((Style & BALLOON_STYLE_SIZETOTEXT) == 0)
				)
			{
				pThis->mServerObject->SetNumCharsPerLine ((long)(ULONG)LOBYTE(HIWORD(Style)));
				pThis->mServerObject->SetNumLines ((long)(ULONG)HIBYTE(HIWORD(Style)));
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::put_Style"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaBalloonObj::XBalloon::get_Style (long *Style)
{
	METHOD_PROLOGUE(CDaBalloonObj, Balloon)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_Style"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!Style)
	{
		lResult = E_POINTER;
	}
	else
	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			long	lCharsPerLine = 0;
			long	lNumLines = 0;
			
			lResult = pThis->mServerObject->GetStyle (Style);
			if	(SUCCEEDED (lResult))
			{
				pThis->mServerObject->GetNumCharsPerLine (&lCharsPerLine);
				pThis->mServerObject->GetNumLines (&lNumLines);
				(*Style) = MAKELONG (*Style, MAKEWORD (lCharsPerLine, lNumLines));
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlBalloon));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] [%p(%u)] [%p(%u)] CDaBalloonObj::XBalloon::get_Style"), pThis->mOwner->SafeGetOwner(), pThis->mOwner->SafeGetOwnerUsed(), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
