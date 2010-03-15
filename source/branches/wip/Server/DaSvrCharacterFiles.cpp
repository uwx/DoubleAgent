/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include <shlwapi.h>
#include "DaServer.h"
#include "DaSvrCharacterFiles.h"
#include "AgentFiles.h"
#include "StringarrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
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

IMPLEMENT_DYNAMIC(CDaSvrCharacterFiles, CEnumVariant)
IMPLEMENT_OLETYPELIB(CDaSvrCharacterFiles, gDaTypeLibId, gDaTypeLibVerMajor, gDaTypeLibVerMinor)

CDaSvrCharacterFiles::CDaSvrCharacterFiles (LPCTSTR pClientMutexName)
:	mFilter (FILES_PATH_DOUBLE_AGENT|FILES_PATH_MS_AGENT|FILES_NO_VALIDATE_VERSION)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrCharacterFiles::CDaSvrCharacterFiles (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	ManageObjectLifetime (this, pClientMutexName);

	GetDefaultSearch ();
	GetFilePaths ();
}

CDaSvrCharacterFiles::CDaSvrCharacterFiles (const CDaSvrCharacterFiles & pSource)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrCharacterFiles::CDaSvrCharacterFiles (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();

	EnableAutomation();
	EnableTypeLib();
	ManageObjectLifetime (this, mClientMutexName);

	mSearchPath.Copy (pSource.mSearchPath);
	mDefaultSearchPath.Copy (pSource.mDefaultSearchPath);
	mFilePaths.Copy (pSource.mFilePaths);
}

CDaSvrCharacterFiles::~CDaSvrCharacterFiles ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrCharacterFiles::~CDaSvrCharacterFiles (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	try
	{
		TheServerApp->OnDeleteSvrCharacterFiles (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
	AfxOleUnlockApp();
}

void CDaSvrCharacterFiles::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					ExternalDisconnect ();
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void CDaSvrCharacterFiles::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrCharacterFiles::OnFinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
	CCmdTarget::OnFinalRelease();
}

void CDaSvrCharacterFiles::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CDaSvrCharacterFiles::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaSvrCharacterFiles::GetDefaultSearch ()
{
	CString	lFilePath;
	UINT	lPathNum;
	UINT	lPathNumFound;

	mDefaultSearchPath.RemoveAll ();

	if	(mFilter & FILES_PATH_MS_AGENT)
	{
		lFilePath = CAgentFiles::GetSystemCharsPath (lPathNum=0, &lPathNumFound);
		if	(
				(!lFilePath.IsEmpty ())
			&&	(lPathNumFound == lPathNum)
			)
		{
			mDefaultSearchPath.Add (lFilePath);
		}
	}
	if	(mFilter & FILES_PATH_DOUBLE_AGENT)
	{
		lFilePath = CAgentFiles::GetSystemCharsPath (lPathNum=1, &lPathNumFound);
		if	(
				(!lFilePath.IsEmpty ())
			&&	(lPathNumFound == lPathNum)
			)
		{
			mDefaultSearchPath.Add (lFilePath);
		}
		lFilePath = CAgentFiles::GetSystemCharsPath (lPathNum=2, &lPathNumFound);
		if	(
				(!lFilePath.IsEmpty ())
			&&	(lPathNumFound == lPathNum)
			)
		{
			mDefaultSearchPath.Add (lFilePath);
		}
	}
	if	(mFilter & FILES_PATH_MS_OFFICE)
	{
		lFilePath = CAgentFiles::GetOfficeCharsPath ();
		if	(!lFilePath.IsEmpty ())
		{
			mDefaultSearchPath.Add (lFilePath);
		}
	}
}

void CDaSvrCharacterFiles::GetFilePaths ()
{
	CStringArray	lSearchPath;
	int				lPathNdx;

	if	(mSearchPath.GetSize() > 0)
	{
		lSearchPath.Copy (mSearchPath);
	}
	else
	{
		lSearchPath.Copy (mDefaultSearchPath);
	}

	mFilePaths.RemoveAll ();

	for	(lPathNdx = 0; lPathNdx <= lSearchPath.GetUpperBound (); lPathNdx++)
	{
		CString			lFindPath (lSearchPath [lPathNdx]);
		CString			lFoundPath;
		CFindHandle		lFindHandle;
		WIN32_FIND_DATA	lFindData;

		PathAddBackslash (lFindPath.GetBuffer (MAX_PATH));
		PathAppend (lFindPath.GetBuffer (MAX_PATH), _T("*.acs"));
		lFindPath.ReleaseBuffer ();

		lFindHandle = FindFirstFile (lFindPath, &lFindData);
		if	(lFindHandle.SafeIsValid ())
		{
			do
			{
				lFoundPath = lFindPath;
				PathRemoveFileSpec (lFoundPath.GetBuffer (MAX_PATH));
				PathAppend (lFoundPath.GetBuffer (MAX_PATH), lFindData.cFileName);
				lFoundPath.ReleaseBuffer ();

				if	(
						(mFilter & (FILES_EXCLUDE_NONSPEAKING|FILES_EXCLUDE_SPEAKING))
					||	(!(mFilter & FILES_NO_VALIDATE_VERSION))
					)
				{
					try
					{
						tPtr <CAgentFile>	lFile = (CAgentFile *)CAgentFile::CreateObject();

						if	(SUCCEEDED (lFile->Open (lFoundPath)))
						{
							if	(
									(mFilter & FILES_EXCLUDE_NONSPEAKING)
								?	(!IsEqualGUID (lFile->GetTts().mMode, GUID_NULL))
								:	(mFilter & FILES_EXCLUDE_SPEAKING)
								?	(IsEqualGUID (lFile->GetTts().mMode, GUID_NULL))
								:	(true)
								)
							{
								mFilePaths.Add (lFoundPath);
							}
						}
					}
					catch AnyExceptionDebug
				}
				else
				{
					mFilePaths.Add (lFoundPath);
				}
			}
			while (FindNextFile (lFindHandle, &lFindData));
		}
	}
}

void CDaSvrCharacterFiles::UpdateFilter (DWORD pNewFilter)
{
	DWORD	lOldFilter = mFilter;
	bool	lGetFiles = false;

	mFilter = pNewFilter & FILES_FLAGS_MASK;

	if	((mFilter & FILES_PATH_MASK) != (lOldFilter & FILES_PATH_MASK))
	{
		GetDefaultSearch ();
		if	(mSearchPath.GetSize() <= 0)
		{
			lGetFiles = true;
		}
	}

	if	(mFilter & FILES_EXCLUDE_NONSPEAKING)
	{
		mFilter &= ~FILES_EXCLUDE_SPEAKING;
	}
	if	(mFilter & FILES_EXCLUDE_MASK)
	{
		mFilter &= ~FILES_NO_VALIDATE_VERSION;
	}

	if	((mFilter & FILES_EXCLUDE_MASK) != (lOldFilter & FILES_EXCLUDE_MASK))
	{
		lGetFiles = true;
	}
	if	((mFilter & FILES_NO_VALIDATE_VERSION) != (lOldFilter & FILES_NO_VALIDATE_VERSION))
	{
		lGetFiles = true;
	}

	if	(lGetFiles)
	{
		GetFilePaths ();
	}
}

/////////////////////////////////////////////////////////////////////////////

long CDaSvrCharacterFiles::GetItemCount ()
{
	return (long)mFilePaths.GetSize();
}

void CDaSvrCharacterFiles::PutItem (long pItemNdx, VARIANT & pItem)
{
	V_VT (&pItem) = VT_BSTR;
	V_BSTR (&pItem) = _bstr_t (mFilePaths [pItemNdx]).Detach();
}

CEnumVariant * CDaSvrCharacterFiles::Clone ()
{
	return new CDaSvrCharacterFiles (*this);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CDaSvrCharacterFiles, CEnumVariant)
	//{{AFX_DISPATCH_MAP(CDaSvrCharacterFiles)
	DISP_PROPERTY_EX_ID(CDaSvrCharacterFiles, "FilePaths", DISPID_VALUE, DspGetFilePaths, DspSetFilePaths, VT_UNKNOWN)
	DISP_PROPERTY_EX_ID(CDaSvrCharacterFiles, "SearchPath", DISPID_IDaSvrCharacterFiles_SearchPath, DspGetSearchPath, DspSetSearchPath, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSvrCharacterFiles, "DefaultSearchPath", DISPID_IDaSvrCharacterFiles_DefaultSearchPath, DspGetDefaultSearchPath, DspSetDefaultSearchPath, VT_BSTR)
	DISP_PROPERTY_EX_ID(CDaSvrCharacterFiles, "Filter", DISPID_IDaSvrCharacterFiles_Filter, DspGetFilter, DspSetFilter, VT_I4)
	DISP_DEFVALUE(CDaSvrCharacterFiles, "FilePaths")
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CDaSvrCharacterFiles, CEnumVariant)
	INTERFACE_PART(CDaSvrCharacterFiles, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CDaSvrCharacterFiles, __uuidof(IDispatch), Dispatch)
	INTERFACE_PART(CDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles), CharacterFiles)
	INTERFACE_PART(CDaSvrCharacterFiles, __uuidof(IProvideClassInfo), ProvideClassInfo)
	INTERFACE_PART(CDaSvrCharacterFiles, __uuidof(ISupportErrorInfo), SupportErrorInfo)
END_INTERFACE_MAP()

IMPLEMENT_IDISPATCH(CDaSvrCharacterFiles, CharacterFiles)
IMPLEMENT_DISPATCH_IID(CDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles))
IMPLEMENT_PROVIDECLASSINFO(CDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles))

BEGIN_SUPPORTERRORINFO(CDaSvrCharacterFiles)
	IMPLEMENT_SUPPORTERRORINFO(CDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles))
END_SUPPORTERRORINFO(CDaSvrCharacterFiles)

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CDaSvrCharacterFiles::XCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
	METHOD_PROLOGUE(CDaSvrCharacterFiles, CharacterFiles)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_FilePaths"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!FilePaths)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	((*FilePaths) = SafeArrayCreateVector (VT_BSTR, 0, (long)pThis->mFilePaths.GetSize()))
		{
			long		lNdx;
			tBstrPtr	lFilePath;

			for	(lNdx = 0; lNdx <= pThis->mFilePaths.GetUpperBound(); lNdx++)
			{
				lFilePath = pThis->mFilePaths [lNdx].AllocSysString();
				SafeArrayPutElement (*FilePaths, &lNdx, lFilePath);
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_FilePaths"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrCharacterFiles::XCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
	METHOD_PROLOGUE(CDaSvrCharacterFiles, CharacterFiles)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_SearchPath"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!SearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(pThis->mSearchPath.GetSize() > 0)
		{
			(*SearchPath) = JoinStringArray (pThis->mSearchPath, _T(";")).AllocSysString ();
		}
		else
		{
			(*SearchPath) = JoinStringArray (pThis->mDefaultSearchPath, _T(";")).AllocSysString ();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_SearchPath"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrCharacterFiles::XCharacterFiles::put_SearchPath (BSTR SearchPath)
{
	METHOD_PROLOGUE(CDaSvrCharacterFiles, CharacterFiles)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::put_SearchPath"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;
	CString	lSearchPath (SearchPath);

	lSearchPath.TrimLeft ();
	lSearchPath.TrimRight ();
	if	(lSearchPath.IsEmpty ())
	{
		pThis->mSearchPath.RemoveAll();
	}
	else
	{
		MakeStringArray (lSearchPath, pThis->mSearchPath, _T(";"));
		pThis->GetFilePaths ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::put_SearchPath"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrCharacterFiles::XCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
	METHOD_PROLOGUE(CDaSvrCharacterFiles, CharacterFiles)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_DefaultSearchPath"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DefaultSearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultSearchPath) = JoinStringArray (pThis->mDefaultSearchPath, _T(";")).AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_DefaultSearchPath"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrCharacterFiles::XCharacterFiles::get_Filter (long *Filter)
{
	METHOD_PROLOGUE(CDaSvrCharacterFiles, CharacterFiles)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_Filter"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Filter)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Filter) = (long)pThis->mFilter;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::get_Filter"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CDaSvrCharacterFiles::XCharacterFiles::put_Filter (long Filter)
{
	METHOD_PROLOGUE(CDaSvrCharacterFiles, CharacterFiles)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::put_Filter"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	pThis->UpdateFilter (Filter);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CDaSvrCharacterFiles::XCharacterFiles::put_Filter"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

SAFEARRAY * CDaSvrCharacterFiles::DspGetFilePaths()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspSetFilePaths"), this, m_dwRef);
#endif
	SAFEARRAY *	lRet = NULL;
	HRESULT		lResult = m_xCharacterFiles.get_FilePaths (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSvrCharacterFiles::DspSetFilePaths(SAFEARRAY * FilePaths)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspSetFilePaths"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

BSTR CDaSvrCharacterFiles::DspGetSearchPath()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspGetSearchPath"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacterFiles.get_SearchPath (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSvrCharacterFiles::DspSetSearchPath(LPCTSTR SearchPath)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspSetSearchPath"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_SearchPath (_bstr_t(SearchPath));
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}

BSTR CDaSvrCharacterFiles::DspGetDefaultSearchPath()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspGetDefaultSearchPath"), this, m_dwRef);
#endif
	BSTR	lRet = NULL;
	HRESULT	lResult = m_xCharacterFiles.get_DefaultSearchPath (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSvrCharacterFiles::DspSetDefaultSearchPath(LPCTSTR DefaultSearchPath)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspSetDefaultSearchPath"), this, m_dwRef);
#endif
	throw DaDispatchException (E_ACCESSDENIED);
}

long CDaSvrCharacterFiles::DspGetFilter()
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspGetFilter"), this, m_dwRef);
#endif
	long	lRet = NULL;
	HRESULT	lResult = m_xCharacterFiles.get_Filter (&lRet);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
	return lRet;
}

void CDaSvrCharacterFiles::DspSetFilter(long Filter)
{
#ifdef	_DEBUG_DSPINTERFACE
	LogMessage (_DEBUG_DSPINTERFACE, _T("[%p(%d)] CDaSvrCharacterFiles::DspSetFilter"), this, m_dwRef);
#endif
	HRESULT	lResult = m_xCharacterFiles.put_Filter (Filter);
	if	(FAILED (lResult))
	{
		throw DaDispatchException (lResult);
	}
}
