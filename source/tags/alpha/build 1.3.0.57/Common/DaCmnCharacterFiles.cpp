/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#include "DaCmnCharacterFiles.h"
#include "AgentFiles.h"
#include "StringArrayEx.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnCharacterFiles::CDaCmnCharacterFiles ()
:	mFilter (FilesFilter_PathDoubleAgent|FilesFilter_PathMsAgent|FilesFilter_NoValidateVersion)
{
}

CDaCmnCharacterFiles::~CDaCmnCharacterFiles ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnCharacterFiles::Initialize ()
{
	GetDefaultSearch ();
	GetFilePaths ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CDaCmnCharacterFiles::GetDefaultSearch ()
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

void CDaCmnCharacterFiles::GetFilePaths ()
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
						(mFilter & FilesFilter_ExcludeMask)
					||	(!(mFilter & FilesFilter_NoValidateVersion))
					)
				{
					try
					{
						tPtr <CAgentFile>	lFile = CAgentFile::CreateInstance (lFoundPath);

						if	(SUCCEEDED (lFile->Open (lFoundPath)))
						{
							if	(
									(
										(!(mFilter & FilesFilter_ExcludeNonCompliant))
									||	(lFile->Header.Style & CharStyleStandard)
									)
								&&	(
										(!(mFilter & FilesFilter_ExcludeCompliant))
									||	(!(lFile->Header.Style & CharStyleStandard))
									)
								&&	(
										(!(mFilter & FilesFilter_ExcludeNonSpeaking))
									||	(!IsEqualGUID (lFile->Tts.Mode, GUID_NULL))
									)
								&&	(
										(!(mFilter & FilesFilter_ExcludeSpeaking))
									||	(IsEqualGUID (lFile->Tts.Mode, GUID_NULL))
									)
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

void CDaCmnCharacterFiles::UpdateFilter (DWORD pNewFilter)
{
	DWORD	lOldFilter = mFilter;
	bool	lGetFiles = false;

	mFilter = pNewFilter & FilesFilter_ValidMask;

	if	((mFilter & FilesFilter_PathMask) == 0)
	{
		mFilter |= FilesFilter_PathDoubleAgent|FilesFilter_PathMsAgent;
	}

	if	((mFilter & FilesFilter_PathMask) != (lOldFilter & FilesFilter_PathMask))
	{
		GetDefaultSearch ();
		if	(mSearchPath.GetCount() <= 0)
		{
			lGetFiles = true;
		}
	}

	if	(mFilter & FilesFilter_ExcludeNonCompliant)
	{
		mFilter &= ~FilesFilter_ExcludeCompliant;
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacterFiles::get_FilePaths (SAFEARRAY**FilePaths)
{
	HRESULT	lResult = S_OK;

	if	(!FilePaths)
	{
		lResult = E_POINTER;
	}
	else
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
	return lResult;
}

HRESULT CDaCmnCharacterFiles::get_SearchPath (BSTR *SearchPath)
{
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
	return lResult;
}

HRESULT CDaCmnCharacterFiles::put_SearchPath (BSTR SearchPath)
{
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
	return lResult;
}

HRESULT CDaCmnCharacterFiles::get_DefaultSearchPath (BSTR *DefaultSearchPath)
{
	HRESULT	lResult = S_OK;

	if	(!DefaultSearchPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultSearchPath) = JoinStringArray (mDefaultSearchPath, _T(";")).AllocSysString ();
	}
	return lResult;
}

HRESULT CDaCmnCharacterFiles::get_Filter (long *Filter)
{
	HRESULT	lResult = S_OK;

	if	(!Filter)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*Filter) = (long)mFilter;
	}
	return lResult;
}

HRESULT CDaCmnCharacterFiles::put_Filter (long Filter)
{
	UpdateFilter (Filter);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCharacterFiles::get_DefaultFilePath (BSTR *DefaultFilePath)
{
	HRESULT	lResult = S_OK;

	if	(!DefaultFilePath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultFilePath) = CAgentFiles::GetDefCharPath().Detach();
	}
	return lResult;
}

HRESULT CDaCmnCharacterFiles::get_DefaultFileName (BSTR *DefaultFileName)
{
	HRESULT	lResult = S_OK;

	if	(!DefaultFileName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*DefaultFileName) = _bstr_t (PathFindFileName (CAtlString (CAgentFiles::GetDefCharPath()))).Detach();
	}
	return lResult;
}
