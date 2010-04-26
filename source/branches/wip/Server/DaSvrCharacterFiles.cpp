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
#include "DaServerApp.h"
#include "DaSvrCharacterFiles.h"
#include "AgentFiles.h"
#include "StringarrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE		(GetProfileDebugInt(_T("DebugInterface_Other"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Other"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS			(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacterFiles::DaSvrCharacterFiles ()
:	mFilter (FilesFilter_PathDoubleAgent|FilesFilter_PathMsAgent|FilesFilter_NoValidateVersion)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::DaSvrCharacterFiles (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrCharacterFiles::~DaSvrCharacterFiles ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::~DaSvrCharacterFiles (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrCharacterFiles (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrCharacterFiles * DaSvrCharacterFiles::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrCharacterFiles> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrCharacterFiles>::CreateInstance (&lInstance))))
	{
		lInstance->GetDefaultSearch ();
		lInstance->GetFilePaths ();
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrCharacterFiles::Terminate (bool pFinal, bool pAbandonned)
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
					CoDisconnectObject (GetUnknown(), 0);
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

void DaSvrCharacterFiles::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrCharacterFiles::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrCharacterFiles::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void DaSvrCharacterFiles::GetDefaultSearch ()
{
	CAtlString	lFilePath;
	UINT		lPathNum;
	UINT		lPathNumFound;

	mDefaultSearchPath.RemoveAll ();

	if	(mFilter & FilesFilter_PathMsAgent)
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
	if	(mFilter & FilesFilter_PathDoubleAgent)
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
	if	(mFilter & FilesFilter_PathMsOffice)
	{
		lFilePath = CAgentFiles::GetOfficeCharsPath ();
		if	(!lFilePath.IsEmpty ())
		{
			mDefaultSearchPath.Add (lFilePath);
		}
	}
}

void DaSvrCharacterFiles::GetFilePaths ()
{
	CAtlStringArray	lSearchPath;
	INT_PTR			lPathNdx;

	if	(mSearchPath.GetCount() > 0)
	{
		lSearchPath.Copy (mSearchPath);
	}
	else
	{
		lSearchPath.Copy (mDefaultSearchPath);
	}

	mFilePaths.RemoveAll ();

	for	(lPathNdx = 0; lPathNdx < (INT_PTR)lSearchPath.GetCount(); lPathNdx++)
	{
		CAtlString		lFindPath (lSearchPath [lPathNdx]);
		CAtlString		lFoundPath;
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
						(mFilter & (FilesFilter_ExcludeNonSpeaking|FilesFilter_ExcludeSpeaking))
					||	(!(mFilter & FilesFilter_NoValidateVersion))
					)
				{
					try
					{
						tPtr <CAgentFile>	lFile = CAgentFile::CreateInstance();

						if	(SUCCEEDED (lFile->Open (lFoundPath)))
						{
							if	(
									(mFilter & FilesFilter_ExcludeNonSpeaking)
								?	(!IsEqualGUID (lFile->GetTts().mMode, GUID_NULL))
								:	(mFilter & FilesFilter_ExcludeSpeaking)
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

void DaSvrCharacterFiles::UpdateFilter (DWORD pNewFilter)
{
	DWORD	lOldFilter = mFilter;
	bool	lGetFiles = false;

	mFilter = pNewFilter & FilesFilter_ValidMask;

	if	((mFilter & FilesFilter_PathMask) != (lOldFilter & FilesFilter_PathMask))
	{
		GetDefaultSearch ();
		if	(mSearchPath.GetCount() <= 0)
		{
			lGetFiles = true;
		}
	}

	if	(mFilter & FilesFilter_ExcludeNonSpeaking)
	{
		mFilter &= ~FilesFilter_ExcludeSpeaking;
	}
	if	(mFilter & FilesFilter_ExcludeMask)
	{
		mFilter &= ~FilesFilter_NoValidateVersion;
	}

	if	((mFilter & FilesFilter_ExcludeMask) != (lOldFilter & FilesFilter_ExcludeMask))
	{
		lGetFiles = true;
	}
	if	((mFilter & FilesFilter_NoValidateVersion) != (lOldFilter & FilesFilter_NoValidateVersion))
	{
		lGetFiles = true;
	}

	if	(lGetFiles)
	{
		GetFilePaths ();
	}
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrCharacterFiles::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrCharacterFiles), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get__NewEnum (IUnknown **ppunkEnum)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_NewEnum"), this, m_dwRef);
#endif
	HRESULT					lResult = S_OK;
	tArrayPtr <CComVariant>	lFilePaths;
	tPtr <CEnumVARIANT>		lEnum;
	IEnumVARIANTPtr			lInterface;
	INT_PTR					lNdx;

	if	(!ppunkEnum)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*ppunkEnum) = NULL;
		
		if	(lFilePaths = new CComVariant [mFilePaths.GetCount()+1])
		{
			for	(lNdx = 0; lNdx < (INT_PTR)mFilePaths.GetCount(); lNdx++)
			{
				lFilePaths [lNdx] = mFilePaths [lNdx].AllocSysString ();
			}
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
		
		if	(
				(SUCCEEDED (lResult))
			&&	(SUCCEEDED (lEnum->Init (&(lFilePaths[0]), &(lFilePaths[(INT_PTR)mFilePaths.GetCount()]), NULL)))
			)
		{
			lInterface = lEnum;
			(*ppunkEnum) = lInterface.Detach ();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_FilePaths"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_FilePaths (SAFEARRAY **FilePaths)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_FilePaths"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!FilePaths)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	((*FilePaths) = SafeArrayCreateVector (VT_BSTR, 0, (long)mFilePaths.GetCount()))
		{
			long		lNdx;
			tBstrPtr	lFilePath;

			for	(lNdx = 0; lNdx < (INT_PTR)mFilePaths.GetCount(); lNdx++)
			{
				lFilePath = mFilePaths [lNdx].AllocSysString();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_FilePaths"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_SearchPath"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!SearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		if	(mSearchPath.GetCount() > 0)
		{
			(*SearchPath) = JoinStringArray (mSearchPath, _T(";")).AllocSysString ();
		}
		else
		{
			(*SearchPath) = JoinStringArray (mDefaultSearchPath, _T(";")).AllocSysString ();
		}
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_SearchPath"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::put_SearchPath (BSTR SearchPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::put_SearchPath"), this, m_dwRef);
#endif
	HRESULT		lResult = S_OK;
	CAtlString	lSearchPath (SearchPath);

	lSearchPath.TrimLeft ();
	lSearchPath.TrimRight ();
	if	(lSearchPath.IsEmpty ())
	{
		mSearchPath.RemoveAll();
	}
	else
	{
		MakeStringArray (lSearchPath, mSearchPath, _T(";"));
		GetFilePaths ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::put_SearchPath"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultSearchPath"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DefaultSearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultSearchPath) = JoinStringArray (mDefaultSearchPath, _T(";")).AllocSysString ();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultSearchPath"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_Filter (long *Filter)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_Filter"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!Filter)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Filter) = (long)mFilter;
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_Filter"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::put_Filter (long Filter)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::put_Filter"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	UpdateFilter (Filter);

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::put_Filter"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_DefaultFilePath (BSTR *DefaultFilePath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFilePath"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DefaultFilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultFilePath) = CAgentFiles::GetDefCharPath().Detach();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFilePath"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrCharacterFiles::get_DefaultFileName (BSTR *DefaultFileName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFileName"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!DefaultFileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultFileName) = _bstr_t (PathFindFileName (CAtlString (CAgentFiles::GetDefCharPath()))).Detach();
	}

	PutServerError (lResult, __uuidof(IDaSvrCharacterFiles));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrCharacterFiles::get_DefaultFileName"), this, m_dwRef);
	}
#endif
	return lResult;
}
