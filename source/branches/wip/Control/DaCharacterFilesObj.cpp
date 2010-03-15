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
#include "DaCharacterFilesObj.h"
#include "DaAgentCtl.h"
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

IMPLEMENT_DYNAMIC(CDaCharacterFilesObj, CCmdTarget)
IMPLEMENT_OLECREATE_UUID(CDaCharacterFilesObj, "")
IMPLEMENT_OLETYPELIB(CDaCharacterFilesObj, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

BOOL CDaCharacterFilesObj::CDaCharacterFilesObjFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::DoUpdateRegistry (bRegister, _T(_CONTROL_PROGID_NAME) _T(" Character Files")))
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

BEGIN_INTERFACE_MAP(CDaCharacterFilesObj, CCmdTarget)
	INTERFACE_PART(CDaCharacterFilesObj, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaCharacterFilesObj, __uuidof(IDaCtlCharacterFiles), CharacterFiles)
	INTERFACE_PART(CDaCharacterFilesObj, __uuidof(ISupportErrorInfo), SupportErrorInfo)
	INTERFACE_PART(CDaCharacterFilesObj, __uuidof(IProvideClassInfo), ProvideClassInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaCharacterFilesObj, CharacterFiles)
IMPLEMENT_DISPATCH_IID(CDaCharacterFilesObj, __uuidof(IDaCtlCharacterFiles))
IMPLEMENT_PROVIDECLASSINFO(CDaCharacterFilesObj, __uuidof(CDaCharacterFilesObj))

BEGIN_SUPPORTERRORINFO(CDaCharacterFilesObj)
	IMPLEMENT_SUPPORTERRORINFO(CDaCharacterFilesObj, __uuidof(IDaCtlCharacterFiles))
END_SUPPORTERRORINFO(CDaCharacterFilesObj)

/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaCharacterFilesObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CDaCharacterFilesObj)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "FilePaths", DISPID_VALUE, DspGetFilePaths, DspSetFilePaths, VT_UNKNOWN)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "SearchPath", DISPID_IDaCtlCharacterFiles_SearchPath, DspGetSearchPath, DspSetSearchPath, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "DefaultSearchPath", DISPID_IDaCtlCharacterFiles_DefaultSearchPath, DspGetDefaultSearchPath, DspSetDefaultSearchPath, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "DoubleAgentFiles", DISPID_IDaCtlCharacterFiles_DoubleAgentFiles, DspGetDoubleAgentFiles, DspSetDoubleAgentFiles, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "MsAgentFiles", DISPID_IDaCtlCharacterFiles_MsAgentFiles, DspGetMsAgentFiles, DspSetMsAgentFiles, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "MsOfficeFiles", DISPID_IDaCtlCharacterFiles_MsOfficeFiles, DspGetMsOfficeFiles, DspSetMsOfficeFiles, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "SpeakingCharacters", DISPID_IDaCtlCharacterFiles_SpeakingCharacters, DspGetSpeakingCharacters, DspSetSpeakingCharacters, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "NonSpeakingCharacters", DISPID_IDaCtlCharacterFiles_NonSpeakingCharacters, DspGetNonSpeakingCharacters, DspSetNonSpeakingCharacters, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDaCharacterFilesObj, "VerifyVersion", DISPID_IDaCtlCharacterFiles_VerifyVersion, DspGetVerifyVersion, DspSetVerifyVersion, VT_BOOL)
	DISP_DEFVALUE(CDaCharacterFilesObj, "FilePaths")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CDaCharacterFilesObj::CDaCharacterFilesObj (CDaAgentCtl & pOwner)
:	mOwner (&pOwner)
{
	if	(pOwner.mServer != NULL)
	{
		LogComErr (LogNormal, pOwner.mServer->GetCharacterFiles (&mServerObject));
	}
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::CDaCharacterFilesObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	AfxOleLockApp ();

	EnableAutomation ();
	EnableTypeLib ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Add (this);
#endif
}

CDaCharacterFilesObj::~CDaCharacterFilesObj ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::~CDaCharacterFilesObj (%d) [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (true);
	AfxOleUnlockApp ();
#ifdef	_DEBUG
	TheControlApp->mComObjects.Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CDaCharacterFilesObj::Terminate (bool pFinal)
{
	if	(this)
	{
#ifdef	_DEBUG
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::Terminate [%u] [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, mServerObject.GetInterfacePtr());
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::Terminate [%u] Done [%d]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, pFinal, AfxOleCanExitApp());
		}
#endif
#endif
	}
}

void CDaCharacterFilesObj::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::OnFinalRelease [%p]"), mOwner, SafeGetOwnerUsed(), this, m_dwRef, mServerObject.GetInterfacePtr());
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////

CDaAgentCtl * CDaCharacterFilesObj::SafeGetOwner () const
{
	return (this ? mOwner : NULL);
}

int CDaCharacterFilesObj::SafeGetOwnerUsed () const
{
	return ((this) && (mOwner)) ? mOwner->m_dwRef : -1;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

SAFEARRAY * CDaCharacterFilesObj::DspGetFilePaths()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetFilePaths"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	SAFEARRAY *	lRet = NULL;
	HRESULT		lResult = m_xCharacterFiles.get_FilePaths (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterFilesObj::DspSetFilePaths(SAFEARRAY * FilePaths)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetFilePaths"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaCharacterFilesObj::DspGetSearchPath()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetSearchPath"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacterFiles.get_SearchPath (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterFilesObj::DspSetSearchPath(LPCTSTR SearchPath)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetSearchPath"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_SearchPath (_bstr_t(SearchPath));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaCharacterFilesObj::DspGetDefaultSearchPath()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetDefaultSearchPath"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacterFiles.get_DefaultSearchPath (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaCharacterFilesObj::DspSetDefaultSearchPath(LPCTSTR DefaultSearchPath)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetDefaultSearchPath"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

/////////////////////////////////////////////////////////////////////////////

BOOL CDaCharacterFilesObj::DspGetDoubleAgentFiles()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetDoubleAgentFiles"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet;
	HRESULT			lResult = m_xCharacterFiles.get_DoubleAgentFiles (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterFilesObj::DspSetDoubleAgentFiles(BOOL DoubleAgentFiles)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetDoubleAgentFiles"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_DoubleAgentFiles (DoubleAgentFiles?VARIANT_TRUE:VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterFilesObj::DspGetMsAgentFiles()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetMsAgentFiles"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet;
	HRESULT			lResult = m_xCharacterFiles.get_MsAgentFiles (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterFilesObj::DspSetMsAgentFiles(BOOL MsAgentFiles)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetMsAgentFiles"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_MsAgentFiles (MsAgentFiles?VARIANT_TRUE:VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterFilesObj::DspGetMsOfficeFiles()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetMsOfficeFiles"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet;
	HRESULT			lResult = m_xCharacterFiles.get_MsOfficeFiles (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterFilesObj::DspSetMsOfficeFiles(BOOL MsOfficeFiles)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetMsOfficeFiles"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_MsOfficeFiles (MsOfficeFiles?VARIANT_TRUE:VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterFilesObj::DspGetSpeakingCharacters()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetSpeakingCharacters"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet;
	HRESULT			lResult = m_xCharacterFiles.get_SpeakingCharacters (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterFilesObj::DspSetSpeakingCharacters(BOOL SpeakingCharacters)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetSpeakingCharacters"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_SpeakingCharacters (SpeakingCharacters?VARIANT_TRUE:VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterFilesObj::DspGetNonSpeakingCharacters()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetNonSpeakingCharacters"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet;
	HRESULT			lResult = m_xCharacterFiles.get_NonSpeakingCharacters (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterFilesObj::DspSetNonSpeakingCharacters(BOOL NonSpeakingCharacters)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetNonSpeakingCharacters"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_NonSpeakingCharacters (NonSpeakingCharacters?VARIANT_TRUE:VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BOOL CDaCharacterFilesObj::DspGetVerifyVersion()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspGetVerifyVersion"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	VARIANT_BOOL	lRet;
	HRESULT			lResult = m_xCharacterFiles.get_VerifyVersion (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return (lRet!=VARIANT_FALSE);
}

void CDaCharacterFilesObj::DspSetVerifyVersion(BOOL VerifyVersion)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::DspSetVerifyVersion"), mOwner, SafeGetOwnerUsed(), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_VerifyVersion (VerifyVersion?VARIANT_TRUE:VARIANT_FALSE);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_FilePaths"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!FilePaths)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*FilePaths) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->get_FilePaths (FilePaths);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_FilePaths"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_SearchPath"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!SearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SearchPath) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->get_SearchPath (SearchPath);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_SearchPath"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_SearchPath (BSTR SearchPath)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_SearchPath"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			lResult = pThis->mServerObject->put_SearchPath (SearchPath);
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_SearchPath"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_DefaultSearchPath"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;

	if	(!DefaultSearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultSearchPath) = NULL;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				lResult = pThis->mServerObject->get_DefaultSearchPath (DefaultSearchPath);
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_DefaultSearchPath"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_DoubleAgentFiles (VARIANT_BOOL *DoubleAgentFiles)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_DoubleAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!DoubleAgentFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DoubleAgentFiles) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
				{
					(*DoubleAgentFiles) = (lFilter & FILES_PATH_DOUBLE_AGENT) ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_DoubleAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_DoubleAgentFiles (VARIANT_BOOL DoubleAgentFiles)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_DoubleAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
			{
				if	(DoubleAgentFiles)
				{
					lFilter |= FILES_PATH_DOUBLE_AGENT;
				}
				else
				{
					lFilter &= ~FILES_PATH_DOUBLE_AGENT;
				}
				lResult = pThis->mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_DoubleAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_MsAgentFiles (VARIANT_BOOL *MsAgentFiles)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_MsAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!MsAgentFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*MsAgentFiles) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
				{
					(*MsAgentFiles) = (lFilter & FILES_PATH_MS_AGENT) ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_MsAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_MsAgentFiles (VARIANT_BOOL MsAgentFiles)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_MsAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
			{
				if	(MsAgentFiles)
				{
					lFilter |= FILES_PATH_MS_AGENT;
				}
				else
				{
					lFilter &= ~FILES_PATH_MS_AGENT;
				}
				lResult = pThis->mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_MsAgentFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_MsOfficeFiles (VARIANT_BOOL *MsOfficeFiles)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_MsOfficeFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!MsOfficeFiles)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*MsOfficeFiles) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
				{
					(*MsOfficeFiles) = (lFilter & FILES_PATH_MS_OFFICE) ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_MsOfficeFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_MsOfficeFiles (VARIANT_BOOL MsOfficeFiles)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_MsOfficeFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
			{
				if	(MsOfficeFiles)
				{
					lFilter |= FILES_PATH_MS_OFFICE;
				}
				else
				{
					lFilter &= ~FILES_PATH_MS_OFFICE;
				}
				lResult = pThis->mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_MsOfficeFiles"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_SpeakingCharacters (VARIANT_BOOL *SpeakingCharacters)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_SpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!SpeakingCharacters)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SpeakingCharacters) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
				{
					(*SpeakingCharacters) = (lFilter & FILES_EXCLUDE_SPEAKING) ? VARIANT_FALSE : VARIANT_TRUE;
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_SpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_SpeakingCharacters (VARIANT_BOOL SpeakingCharacters)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_SpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
			{
				if	(SpeakingCharacters)
				{
					lFilter &= ~FILES_EXCLUDE_SPEAKING;
				}
				else
				{
					lFilter |= FILES_EXCLUDE_SPEAKING;
					lFilter &= ~FILES_EXCLUDE_NONSPEAKING;
				}
				lResult = pThis->mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_SpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_NonSpeakingCharacters (VARIANT_BOOL *NonSpeakingCharacters)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_NonSpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!NonSpeakingCharacters)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*NonSpeakingCharacters) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
				{
					(*NonSpeakingCharacters) = (lFilter & FILES_EXCLUDE_NONSPEAKING) ? VARIANT_FALSE : VARIANT_TRUE;
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_NonSpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_NonSpeakingCharacters (VARIANT_BOOL NonSpeakingCharacters)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_NonSpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
			{
				if	(NonSpeakingCharacters)
				{
					lFilter &= ~FILES_EXCLUDE_NONSPEAKING;
				}
				else
				{
					lFilter |= FILES_EXCLUDE_NONSPEAKING;
					lFilter &= ~FILES_EXCLUDE_SPEAKING;
				}
				lResult = pThis->mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_NonSpeakingCharacters"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::get_VerifyVersion (VARIANT_BOOL *VerifyVersion)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_VerifyVersion"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(!VerifyVersion)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*VerifyVersion) = VARIANT_TRUE;

		if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
		{
			try
			{
				if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
				{
					(*VerifyVersion) = (lFilter & FILES_NO_VALIDATE_VERSION) ? VARIANT_FALSE : VARIANT_TRUE;
				}
			}
			catch AnyExceptionDebug
			TheControlApp->PostServerCall (pThis->mServerObject);
		}
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::get_VerifyVersion"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaCharacterFilesObj::XCharacterFiles::put_VerifyVersion (VARIANT_BOOL VerifyVersion)
{
	METHOD_PROLOGUE(CDaCharacterFilesObj, CharacterFiles)
	ClearControlError ();
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_VerifyVersion"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult;
	long	lFilter = 0;

	if	(SUCCEEDED (lResult = TheControlApp->PreServerCall (pThis->mServerObject)))
	{
		try
		{
			if	(SUCCEEDED (lResult = pThis->mServerObject->get_Filter (&lFilter)))
			{
				if	(VerifyVersion)
				{
					lFilter &= ~FILES_NO_VALIDATE_VERSION;
				}
				else
				{
					lFilter |= FILES_NO_VALIDATE_VERSION;
					lFilter &= ~FILES_EXCLUDE_NONSPEAKING;
					lFilter &= ~FILES_EXCLUDE_SPEAKING;
				}
				lResult = pThis->mServerObject->put_Filter (lFilter);
			}
		}
		catch AnyExceptionDebug
		TheControlApp->PostServerCall (pThis->mServerObject);
	}

	PutControlError (lResult, __uuidof(IDaCtlCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] [%p(%u)] CDaCharacterFilesObj::XCommand::put_VerifyVersion"), pThis->mOwner, pThis->SafeGetOwnerUsed(), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
