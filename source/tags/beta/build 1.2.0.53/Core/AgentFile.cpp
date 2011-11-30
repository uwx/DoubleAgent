/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "AgentFile.h"
#ifdef	__cplusplus_cli
#include "AgtErr.h"
#include "AgentFileAcs.h"
#include "AgentFileScript.h"
#include "AgentFileV15.h"
#else
#include <shlwapi.h>
#include <wininet.h>
#include "DaCore.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#include "AgentFileBinary.h"
#include "AgentFileV15.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "DebugStr.h"
#include "ImageDebugger.h"
#include "ImageTools.h"
#endif
#endif
#include "AgentFileDefs.inl"

#ifndef	__cplusplus_cli
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon.lib")
IMPLEMENT_DLL_OBJECT(CAgentFile)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentFile::CAgentFile()
:	mLogLevel (LogVerbose+1),
	mSignature (0)
{
	Close ();
}

CAgentFile::~CAgentFile ()
{
	Close ();
}

#ifdef	__cplusplus_cli
CAgentFile^ CAgentFile::CreateInstance (const System::String^ pPath)
#else
CAgentFile* CAgentFile::CreateInstance (LPCTSTR pPath)
#endif
{
#ifdef	__cplusplus_cli
	if	(pPath)
	{
		String^	lPath = ParseFilePath (pPath);

		if	(String::Compare (System::IO::Path::GetExtension (lPath), AcdFileExt, true) == 0)
		{
			return CAgentFileScript::CreateInstance ();
		}
		else
		if	(
				(String::Compare (System::IO::Path::GetExtension (lPath), AcsFileExt, true) == 0)
			&&	(CAgentFileV15::CheckFileSignature (pPath))
			)
		{
			return CAgentFileV15::CreateInstance ();
		}
		else
		{
			return CAgentFileBinary::CreateInstance (lPath);
		}
	}
	else
	{
		return CAgentFileAcs::CreateInstance ();
	}
#else
	if	(
			(pPath)
		&&	(_tcsicmp (PathFindExtension (pPath), sAcsFileExt) == 0)
		&&	(CAgentFileV15::CheckFileSignature (pPath))
		)
	{
		return CAgentFileV15::CreateInstance ();
	}
	else
	{
		return CAgentFileBinary::CreateInstance (pPath);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFile::IsProperFilePath (const System::String^ pPath)
{
	String^	lPath = ParseFilePath (pPath);

	if	(!String::IsNullOrEmpty (lPath))
	{
		System::Uri^	lUri = gcnew System::Uri (lPath);

		if	(
				(lUri->Scheme == lUri->UriSchemeFile)
			||	(lUri->Scheme == lUri->UriSchemeHttp)
			)
		{
			return true;
		}
	}
	return false;
}
#else
bool CAgentFile::IsProperFilePath (LPCTSTR pPath)
{
	CAtlString	lPath (ParseFilePath (pPath));

	if	(!lPath.IsEmpty ())
	{
		if	(PathIsURL (lPath))
		{
			DWORD		lUrlLen = INTERNET_MAX_URL_LENGTH;
			CAtlString	lProtocol;

			if	(SUCCEEDED (UrlGetPart (lPath, lProtocol.GetBuffer(lUrlLen), &lUrlLen, URL_PART_SCHEME, 0)))
			{
				lProtocol.ReleaseBuffer ();
				if	(
						(lProtocol.CompareNoCase (_T("http")) == 0)
					||	(lProtocol.CompareNoCase (_T("file")) == 0)
					)
				{
					return true;
				}
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFile::IsRelativeFilePath (const System::String^ pPath)
{
	String^	lPath;

	if	(!pPath->IsNullOrEmpty (const_cast <System::String^> (pPath)))
	{
		lPath = String::Copy (const_cast <System::String^> (pPath));
		lPath->Trim ();

		if	(
				(!String::IsNullOrEmpty (lPath))
			&&	(!System::Uri::IsWellFormedUriString (lPath, System::UriKind::RelativeOrAbsolute))
			&&	(
					(!System::IO::Path::IsPathRooted (lPath))
				||	(System::IO::Path::GetPathRoot (lPath) == "\\")
				)
			)
		{
			return true;
		}
	}
	return false;
}
#else
bool CAgentFile::IsRelativeFilePath (LPCTSTR pPath)
{
	CAtlString	lPath (pPath);

	lPath.TrimLeft ();
	lPath.TrimRight ();
	PathUnquoteSpaces (lPath.GetBuffer (lPath.GetLength ()));
	lPath.ReleaseBuffer ();
	lPath.TrimLeft ();
	lPath.TrimRight ();

	if	(
			(!lPath.IsEmpty ())
		&&	(!PathIsURL (lPath))
		&&	(
				(PathIsFileSpec (lPath))
			||	(PathSkipRoot (lPath) == (LPCTSTR)lPath)
			)
		)
	{
		return true;
	}
	return false;
}
#endif

#ifdef	__cplusplus_cli
System::String^ CAgentFile::ParseFilePath (const System::String^ pPath)
{
	String^	lPath;

	if	(!pPath->IsNullOrEmpty (const_cast <String^> (pPath)))
	{
		lPath = String::Copy (const_cast <String^> (pPath));
		lPath->Trim ();
		if	(!String::IsNullOrEmpty (lPath))
		{
			try
			{
				lPath = System::IO::Path::GetFullPath (lPath);
			}
			catch AnyExceptionSilent
		}
		if	(String::IsNullOrEmpty (lPath))
		{
			lPath = nullptr;
		}
	}
	return lPath;
}
#else
tBstrPtr CAgentFile::ParseFilePath (LPCTSTR pPath)
{
	CAtlString	lPath (pPath);

	lPath.TrimLeft ();
	lPath.TrimRight ();
	PathUnquoteSpaces (lPath.GetBuffer (lPath.GetLength ()));
	lPath.ReleaseBuffer ();
	lPath.TrimLeft ();
	lPath.TrimRight ();

	if	(!lPath.IsEmpty ())
	{
		if	(PathIsURL (lPath))
		{
			DWORD		lUrlLen = INTERNET_MAX_URL_LENGTH;
			CAtlString	lCanonical;

			if	(SUCCEEDED (UrlCanonicalize (lPath, lCanonical.GetBuffer(lUrlLen), &lUrlLen, URL_ESCAPE_UNSAFE)))
			{
				lCanonical.ReleaseBuffer ();
				lPath = lCanonical;
			}
		}
		else
		{
			CAtlString	lLongPath;

			if	(PathSearchAndQualify (pPath, lLongPath.GetBuffer(MAX_PATH), MAX_PATH))
			{
				lLongPath.ReleaseBuffer ();
				lPath = lLongPath;
				if	(GetLongPathName (lPath, lLongPath.GetBuffer(MAX_PATH), MAX_PATH))
				{
					lLongPath.ReleaseBuffer ();
					lPath = lLongPath;
				}
			}
		}
	}
	return lPath.AllocSysString();
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentFile::ParseRelativePath (const System::String^ pPath)
{
	String^	lPath;

	if	(
			(!String::IsNullOrEmpty (mPath))
		&&	(!String::IsNullOrEmpty (const_cast <String^> (pPath)))
		)
	{
		lPath = String::Copy (const_cast <String^> (pPath));
		lPath->Trim ();

		if	(!lPath->IsNullOrEmpty (lPath))
		{
			try
			{
				lPath = System::IO::Path::Combine (System::IO::Path::GetDirectoryName (mPath), System::IO::Path::GetFileName (lPath));
			}
			catch AnyExceptionSilent
		}
		if	(String::IsNullOrEmpty (lPath))
		{
			lPath = nullptr;
		}
	}
	return lPath;
}
#else
tBstrPtr CAgentFile::ParseRelativePath (LPCTSTR pRelativePath)
{
	CAtlString	lRelativePath (pRelativePath);
	CAtlString	lPath;

	lRelativePath.TrimLeft ();
	lRelativePath.TrimRight ();
	PathUnquoteSpaces (lRelativePath.GetBuffer (lRelativePath.GetLength ()));
	lRelativePath.ReleaseBuffer ();
	lRelativePath.TrimLeft ();
	lRelativePath.TrimRight ();

	if	(PathIsURL (mPath))
	{
		CAtlString	lWorkStr;
		DWORD		lStrSize;

		CoInternetParseUrl (mPath, PARSE_SCHEMA, 0, lWorkStr.GetBuffer(INTERNET_MAX_URL_LENGTH), INTERNET_MAX_URL_LENGTH, &lStrSize, 0);
		lWorkStr.ReleaseBuffer ();
		lWorkStr = mPath.Mid (lWorkStr.GetLength()+1);
		lWorkStr.TrimLeft (_T('/'));
		lStrSize = mPath.GetLength() - lWorkStr.GetLength();
		lWorkStr.Replace (_T('/'), _T('\\'));
		PathRemoveFileSpec (lWorkStr.GetBuffer (INTERNET_MAX_URL_LENGTH));
		PathAddBackslash (lWorkStr.GetBuffer (INTERNET_MAX_URL_LENGTH));
		lWorkStr.ReleaseBuffer ();
		lWorkStr.Replace (_T('\\'), _T('/'));
		lWorkStr = mPath.Left (lWorkStr.GetLength() + lStrSize);

		lStrSize = INTERNET_MAX_URL_LENGTH;
		CoInternetCombineUrl (lWorkStr, lRelativePath, 0, lPath.GetBuffer(lStrSize), lStrSize, &lStrSize, 0);
		lPath.ReleaseBuffer ();
	}
	else
	{
		lPath = mPath;
		PathRemoveFileSpec (lPath.GetBuffer (lPath.GetLength ()));
		PathAppend (lPath.GetBuffer (MAX_PATH), lRelativePath);
		lPath.ReleaseBuffer ();
	}
	return ParseFilePath (lPath);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFile::Close ()
{
	FreeNames ();
	FreeStates ();
	FreeGestures ();

#ifdef	__cplusplus_cli
	mPath = nullptr;
	mHeader = gcnew CAgentFileHeader;
	mTts = nullptr;
	mBalloon = nullptr;
#else
	mPath.Empty ();
	mHeader.Empty ();
	mTts.Empty ();
	mBalloon.Empty ();
#endif
	mVersionMajor = 2;
	mVersionMinor = 1;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFile::Save ()
{
	return Save (nullptr);
}

bool CAgentFile::Save (const System::String^ pPath)
{
	return Save (pPath, this);
}

bool CAgentFile::Save (const System::String^ pPath, CAgentFile^ pSource)
{
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentFile::Path::get ()
{
	return (mPath) ? mPath->Copy (mPath) : nullptr;
}
#else
tBstrPtr CAgentFile::get_Path () const
{
	return mPath.AllocSysString();
}
#endif

#ifdef	__cplusplus_cli
System::String^ CAgentFile::FileName::get ()
{
	return (mPath) ? System::IO::Path::GetFileName (mPath) : nullptr;
}
#else
tBstrPtr CAgentFile::get_FileName () const
{
	CAtlString	lFileName (PathFindFileName (mPath));
	return lFileName.AllocSysString();
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFile::IsOpen::get ()
{
	return false;
}
#else
bool CAgentFile::get_IsOpen () const
{
	return false;
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFile::IsReadOnly::get ()
{
	return true;
}
#else
bool CAgentFile::get_IsReadOnly () const
{
	return true;
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFile::IsDirty::get ()
{
	return false;
}

void CAgentFile::IsDirty::set (bool pIsDirty)
{
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
Boolean CAgentFile::IsAcsFile::get ()
{
	return false;
}
#else
bool CAgentFile::get_IsAcsFile () const
{
	return false;
}
#endif

#ifdef	__cplusplus_cli
Boolean CAgentFile::IsAcfFile::get ()
{
	return false;
}
#else
bool CAgentFile::get_IsAcfFile () const
{
	return false;
}
#endif

#ifdef	__cplusplus_cli
Boolean CAgentFile::IsAcdFile::get ()
{
	return false;
}
#else
bool CAgentFile::get_IsAcdFile () const
{
	return false;
}
#endif

#ifdef	__cplusplus_cli
System::String^ CAgentFile::AcsFileExt::get ()
{
	return gcnew String (sAcsFileExt);
}

System::String^ CAgentFile::AcfFileExt::get ()
{
	return gcnew String (sAcfFileExt);
}

System::String^ CAgentFile::AcdFileExt::get ()
{
	return gcnew String (sAcdFileExt);
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
UInt32 CAgentFile::Version::get ()
{
	return MAKELONG (mVersionMinor, mVersionMajor);
}
#else
DWORD CAgentFile::get_Version () const
{
	return MAKELONG (mVersionMinor, mVersionMajor);
}
#endif

#ifdef	__cplusplus_cli
CAgentFileHeader^ CAgentFile::Header::get ()
{
	return mHeader;
}
#else
const CAgentFileHeader& CAgentFile::get_Header () const
{
	return mHeader;
}
#endif

#ifdef	__cplusplus_cli
CAgentFileBalloon^ CAgentFile::Balloon::get ()
{
	return mBalloon;
}
#else
const CAgentFileBalloon& CAgentFile::get_Balloon () const
{
	return mBalloon;
}
#endif

#ifdef	__cplusplus_cli
CAgentFileTts^ CAgentFile::Tts::get ()
{
	return mTts;
}
#else
const CAgentFileTts& CAgentFile::get_Tts () const
{
	return mTts;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
CAgentFileNames^ CAgentFile::Names::get ()
{
	if	(
			(
				(!mNames)
			||	(mNames->Count <= 0)
			)
		&&	(IsOpen)
		)
	{
		ReadNames ();
	}
	return mNames;
}
#else
const CAgentFileNames& CAgentFile::get_Names () const
{
	if	(
			(mNames.GetCount() <= 0)
		&&	(IsOpen)
		)
	{
		const_cast <CAgentFile*> (this)->ReadNames ();
	}
	return mNames;
}
#endif

#ifdef	__cplusplus_cli
CAgentFileStates^ CAgentFile::States::get ()
{
	if	(
			(
				(!mStates)
			||	(mStates->Count <= 0)
			)
		&&	(IsOpen)
		)
	{
		ReadStates ();
	}
	return mStates;
}
#else
const CAgentFileStates& CAgentFile::get_States () const
{
	if	(
			(mStates.mGestures.GetCount() <= 0)
		&&	(IsOpen)
		)
	{
		const_cast <CAgentFile*> (this)->ReadStates ();
	}
	return mStates;
}
#endif

#ifdef	__cplusplus_cli
CAgentFileGestures^ CAgentFile::Gestures::get ()
{
	if	(
			(
				(!mGestures)
			||	(mGestures->Count <= 0)
			)
		&&	(IsOpen)
		)
	{
		ReadGestures ();
	}
	return mGestures;
}
#else
const CAgentFileGestures& CAgentFile::get_Gestures () const
{
	if	(
			(mGestures.mNames.GetCount() <= 0)
		&&	(IsOpen)
		)
	{
		const_cast <CAgentFile*> (this)->ReadGestures ();
	}
	return mGestures;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
array <System::String^>^ CAgentFile::GetStateNames ()
{
	array <String^>^	lRet = nullptr;

	try
	{
		if	(
				(
					(!mStates)
				||	(mStates->Count <= 0)
				)
			&&	(IsOpen)
			)
		{
			ReadStates ();
		}

		if	(
				(mStates)
			&&	(mStates->Count > 0)
			&&	(lRet = gcnew array <String^> (mStates->Count))
			)
		{
			int	lNdx = 0;

			for each (CAgentFileState^ lState in mStates)
			{
				lRet [lNdx++] = lState->StateName;
			}
		}
		else
		{
			lRet = gcnew array <String^> (0);
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
#else
SAFEARRAY* CAgentFile::GetStateNames ()
{
	tSafeArrayPtr	lRet;

	try
	{
		long	lNdx;

		if	(
				(mStates.mGestures.GetCount() <= 0)
			&&	(IsOpen)
			)
		{
			ReadStates ();
		}

		if	(
				(mStates.mNames.GetCount() > 0)
			&&	(lRet = SafeArrayCreateVector (VT_BSTR, 0, (ULONG)mStates.mNames.GetCount()))
			)
		{
			for	(lNdx = 0; lNdx < (long)mStates.mNames.GetCount(); lNdx++)
			{
				SafeArrayPutElement (lRet, &lNdx, mStates.mNames [lNdx].AllocSysString());
			}
		}
	}
	catch AnyExceptionDebug

	return lRet.Detach();
}
#endif

#ifdef	__cplusplus_cli
array <System::String^>^ CAgentFile::GetGestureNames ()
{
	array <String^>^	lRet = nullptr;

	try
	{
		if	(
				(
					(!mGestures)
				||	(mGestures->Count <= 0)
				)
			&&	(IsOpen)
			)
		{
			ReadGestures ();
		}

		if	(
				(mGestures)
			&&	(mGestures->Count > 0)
			&&	(lRet = gcnew array <String^> (mGestures->Count))
			)
		{
			CAgentFileAnimation^	lAnimation;
			int						lNdx = 0;

			for each (lAnimation in mGestures)
			{
				lRet [lNdx++] = lAnimation->Name;
			}
		}
		else
		{
			lRet = gcnew array <String^> (0);
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
#else
SAFEARRAY* CAgentFile::GetGestureNames ()
{
	tSafeArrayPtr	lRet;

	try
	{
		long	lNdx;

		if	(
				(mGestures.mNames.GetCount() <= 0)
			&&	(IsOpen)
			)
		{
			ReadGestures ();
		}

		if	(
				(mGestures.mNames.GetCount() > 0)
			&&	(lRet = SafeArrayCreateVector (VT_BSTR, 0, (ULONG)mGestures.mNames.GetCount()))
			)
		{
			for	(lNdx = 0; lNdx < (long)mGestures.mNames.GetCount(); lNdx++)
			{
				SafeArrayPutElement (lRet, &lNdx, mGestures.mNames [lNdx].AllocSysString());
			}
		}
	}
	catch AnyExceptionDebug

	return lRet.Detach();
}
#endif

#ifdef	__cplusplus_cli
array <System::String^>^ CAgentFile::GetAnimationNames ()
{
	return GetGestureNames ();
}
#else
SAFEARRAY* CAgentFile::GetAnimationNames ()
{
	return GetGestureNames ();
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentFile::IconFilePath::get()
{
	return nullptr;
}

void CAgentFile::IconFilePath::set (System::String^ pValue)
{
}

System::String^ CAgentFile::PaletteFilePath::get()
{
	return nullptr;
}

void CAgentFile::PaletteFilePath::set (System::String^ pValue)
{
}

UInt16 CAgentFile::NewFrameDuration::get()
{
	return 0;
}

void CAgentFile::NewFrameDuration::set (UInt16 pValue)
{
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::FindState (LPCTSTR pStateName)
{
	CAtlString	lStateName (pStateName);

	if	(!lStateName.IsEmpty ())
	{
		return FindSortedString (mStates.mNames, lStateName);
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::FindGesture (LPCTSTR pGestureName)
{
	CAtlString	lGestureName (pGestureName);

	if	(!lGestureName.IsEmpty ())
	{
		return FindSortedString (mGestures.mNames, lGestureName);
	}
	return -1;
}

const CAgentFileAnimation* CAgentFile::GetGesture (INT_PTR pGestureNdx)
{
	CAgentFileAnimation*	lRet = NULL;

	if	(
			(pGestureNdx >= 0)
		&&	(pGestureNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		)
	{
		lRet = mGestures.mAnimations [pGestureNdx];
	}
	return lRet;
}

const CAgentFileAnimation* CAgentFile::GetGesture (LPCTSTR pGestureName)
{
	return GetGesture (FindGesture (pGestureName));
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::FindAnimation (LPCTSTR pAnimationName)
{
	CAtlString				lAnimationName (pAnimationName);
	CAgentFileAnimation*	lAnimation;
	INT_PTR					lNdx;

	if	(!lAnimationName.IsEmpty ())
	{
		for	(lNdx = 0; lNdx < (INT_PTR)mGestures.mAnimations.GetCount(); lNdx++)
		{
			if	(
					(lAnimation = mGestures.mAnimations [lNdx])
				&&	(lAnimationName.CompareNoCase (lAnimation->mName) == 0)
				)
			{
				return lNdx;
			}
		}
	}
	return -1;
}

const CAgentFileAnimation* CAgentFile::GetAnimation (INT_PTR pAnimationNdx)
{
	CAgentFileAnimation*	lRet = NULL;

	if	(
			(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		)
	{
		lRet = mGestures.mAnimations [pAnimationNdx];
	}
	return lRet;
}

const CAgentFileAnimation* CAgentFile::GetAnimation (LPCTSTR pAnimationName)
{
	return GetAnimation (FindAnimation (pAnimationName));
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentFile::ImageCount::get()
{
	return 0;
}

CAgentFileImage^ CAgentFile::GetImage (int pImageNdx)
{
	return GetImage (pImageNdx, false);
}

CAgentFileImage^ CAgentFile::GetImage (int pImageNdx, bool p32Bit)
{
	return GetImage (pImageNdx, p32Bit, Color::Transparent);
}

CAgentFileImage^ CAgentFile::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
{
	return nullptr;
}

System::String^ CAgentFile::GetImageFilePath (int pImageNdx)
{
	return nullptr;
}
#else
INT_PTR CAgentFile::GetImageCount () const
{
	return 0;
}

CAgentFileImage* CAgentFile::GetImage (INT_PTR pImageNdx, bool p32Bit, const COLORREF* pBkColor)
{
	return NULL;
}

UINT CAgentFile::GetImageFormat (LPBITMAPINFO pImageInfo, const CAgentFileImage* pImage, bool p32Bit) const
{
	UINT	lRet = sizeof(BITMAPINFOHEADER);

	if	(pImageInfo)
	{
		memset (pImageInfo, 0, sizeof(BITMAPINFOHEADER));

		pImageInfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
		pImageInfo->bmiHeader.biPlanes = 1;
		pImageInfo->bmiHeader.biBitCount = p32Bit ? 32 : 8;
		pImageInfo->bmiHeader.biCompression = BI_RGB;

		if	(pImage)
		{
			pImageInfo->bmiHeader.biWidth = pImage->ImageSize.cx;
			pImageInfo->bmiHeader.biHeight = pImage->ImageSize.cy;
		}
		else
		{
			pImageInfo->bmiHeader.biWidth = mHeader.ImageSize.cx;
			pImageInfo->bmiHeader.biHeight = mHeader.ImageSize.cy;
		}
		pImageInfo->bmiHeader.biSizeImage = GetImageBits (NULL, pImage, p32Bit);
	}
	if	(
			(!p32Bit)
		&&	(mHeader.Palette)
		)
	{
		lRet += sizeof(COLORREF) * 256;

		if	(pImageInfo)
		{
			pImageInfo->bmiHeader.biClrImportant = 256;
			pImageInfo->bmiHeader.biClrUsed = 256;
			memcpy (pImageInfo->bmiColors, mHeader.Palette, sizeof(COLORREF) * 256);
		}
	}
	return lRet;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
CAgentFileImage^ CAgentFile::Get32BitImage (CAgentFileImage^ p8BitImage)
{
	return Get32BitImage (p8BitImage, Color::Transparent);
}
CAgentFileImage^ CAgentFile::Get32BitImage (CAgentFileImage^ p8BitImage, System::Drawing::Color pBkColor)
#else	
CAgentFileImage* CAgentFile::Get32BitImage (const CAgentFileImage* p8BitImage, const COLORREF* pBkColor)
#endif
{
#ifdef	__cplusplus_cli
	CAgentFileImage^		l32BitImage = nullptr;
#else	
	tPtr <CAgentFileImage>	l32BitImage;
#endif
	if	(
			(p8BitImage)
		&&	(p8BitImage->Is32Bit)
#ifdef	__cplusplus_cli
		&&	(pBkColor == Color::Empty)
#else
		&&	(!pBkColor)
#endif		
		)
	{
#ifdef	__cplusplus_cli
		return p8BitImage;
#else
		return const_cast <CAgentFileImage*> (p8BitImage);
#endif
	}
	else
	if	(
			(p8BitImage)
		&&	(!p8BitImage->Is32Bit)
		&&	(p8BitImage->mBits)
#ifdef	__cplusplus_cli
		&&	(Header)
		&&	(Header->Palette)
#else
		&&	(Header.Palette)
#endif
		)
	{
		try
		{
			DWORD	lBitsSize;

			if	(
#ifdef	__cplusplus_cli
					(l32BitImage = gcnew CAgentFileImage)
				&&	(lBitsSize = p8BitImage->mImageSize.Width * p8BitImage->mImageSize.Height * 4)
				&&	(l32BitImage->mBits = gcnew array <BYTE> (lBitsSize))
#else
					(l32BitImage = new CAgentFileImage)
				&&	(lBitsSize = l32BitImage->mBitsSize = p8BitImage->mImageSize.cx * p8BitImage->mImageSize.cy * 4)
				&&	(l32BitImage->mBits = new BYTE [lBitsSize])
#endif
				)
			{
#ifdef	__cplusplus_cli
				pin_ptr <BYTE>					lImageBits;
				System::Drawing::Size			lImageSize = p8BitImage->mImageSize;
				System::Drawing::Point			lPixel;
				array <System::Drawing::Color>^	lPaletteEntries = Header->Palette->Entries;
				Byte							lTransparency = Header->Transparency;
				int								lSrcNdx;
				int								lTrgNdx;
#else
				CSize							lImageSize = p8BitImage->mImageSize;
				CPoint							lPixel;
				LPCOLORREF						lPalette = Header.Palette;
				BYTE							lTransparency = Header.Transparency;
				INT_PTR							lSrcNdx;
				INT_PTR							lTrgNdx;
#endif
				int								lSrcScanBytes;
				int								lTrgScanBytes;

				l32BitImage->mImageNum = p8BitImage->mImageNum;
				l32BitImage->mImageSize = p8BitImage->mImageSize;
				l32BitImage->mIs32Bit = true;

#ifdef	__cplusplus_cli
				lSrcScanBytes = ((lImageSize.Width + 3) / 4) * 4;
				lTrgScanBytes = lImageSize.Width * 4;
#else
				lSrcScanBytes = ((lImageSize.cx + 3) / 4) * 4;
				lTrgScanBytes = lImageSize.cx * 4;
#endif

#ifdef	__cplusplus_cli
				for	(lPixel.Y = 0; lPixel.Y < lImageSize.Height; lPixel.Y++)
#else
				for	(lPixel.y = 0; lPixel.y < lImageSize.cy; lPixel.y++)
#endif
				{
#ifdef	__cplusplus_cli
					lSrcNdx = lPixel.Y * lSrcScanBytes;
					lTrgNdx = lPixel.Y * lTrgScanBytes;
#else
					lSrcNdx = lPixel.y * lSrcScanBytes;
					lTrgNdx = lPixel.y * lTrgScanBytes;
#endif

#ifdef	__cplusplus_cli
					for	(lPixel.X = 0; lPixel.X < lImageSize.Width; lPixel.X++)
#else
					for	(lPixel.x = 0; lPixel.x < lImageSize.cx; lPixel.x++)
#endif
					{
#ifdef	__cplusplus_cli
						lImageBits = &l32BitImage->mBits[lTrgNdx];
						if	(p8BitImage->mBits [lSrcNdx] == lTransparency)
						{
							if	(pBkColor == Color::Transparent)
							{
								*(LPCOLORREF)(LPBYTE)lImageBits = 0;
							}
							else 
							if	(pBkColor == Color::Empty)
							{
								*(LPCOLORREF)(LPBYTE)lImageBits = lPaletteEntries [(long)p8BitImage->mBits [lSrcNdx]].ToArgb() | 0xFF000000;
							}
							else
							{
								*(LPCOLORREF)(LPBYTE)lImageBits = pBkColor.ToArgb() | 0xFF000000;
							}
						}
						else
						{
							*(LPCOLORREF)(LPBYTE)lImageBits = lPaletteEntries [(long)p8BitImage->mBits [lSrcNdx]].ToArgb() | 0xFF000000;
						}
#else
						if	(p8BitImage->mBits [lSrcNdx] == lTransparency)
						{
							if	(pBkColor)
							{
								*(LPCOLORREF)(l32BitImage->mBits+lTrgNdx) = *pBkColor | 0xFF000000;
							}
							else
							{
								*(LPCOLORREF)(l32BitImage->mBits+lTrgNdx) = 0;
							}
						}
						else
						{
							*(LPCOLORREF)(l32BitImage->mBits+lTrgNdx) = lPalette [(long)p8BitImage->mBits [lSrcNdx]] | 0xFF000000;
						}
#endif
						lSrcNdx++;
						lTrgNdx += 4;
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
#ifdef	__cplusplus_cli
	return l32BitImage;
#else	
	return l32BitImage.Detach();
#endif	
}	

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
UINT CAgentFile::GetImageBits (LPBYTE pImageBits, int pImageNdx)
{
	return GetImageBits (pImageBits, pImageNdx, false);
}

UINT CAgentFile::GetImageBits (LPBYTE pImageBits, int pImageNdx, bool p32Bit)
{
	return GetImageBits (pImageBits, GetImage (pImageNdx, p32Bit));
}

UINT CAgentFile::GetImageBits (LPBYTE pImageBits, CAgentFileImage^ pImage)
{
	return GetImageBits (pImageBits, pImage, false);
}

UINT CAgentFile::GetImageBits (LPBYTE pImageBits, CAgentFileImage^ pImage, bool p32Bit)
#else
UINT CAgentFile::GetImageBits (LPBYTE pImageBits, const CAgentFileImage* pImage, bool p32Bit) const
#endif
{
	UINT	lRet = 0;

	if	(
			(pImageBits)
		&&	(pImage)
		&&	(pImage->BitsSize)
		)
	{
		lRet = pImage->BitsSize;
#ifdef	__cplusplus_cli
		pin_ptr <BYTE>	lImageBits = &pImage->Bits[0];
		memcpy (pImageBits, (LPBYTE)lImageBits, pImage->BitsSize);
#else
		memcpy (pImageBits, pImage->Bits, pImage->BitsSize);
#endif
	}
	else
	{
#ifdef	__cplusplus_cli
		System::Drawing::Size	lImageSize (mHeader->ImageSize);
#else
		CSize					lImageSize (mHeader.ImageSize);
#endif

		if	(pImage)
		{
#ifdef	__cplusplus_cli
			lImageSize.Width = min (lImageSize.Width, pImage->ImageSize.Width);
			lImageSize.Height = min (lImageSize.Height, pImage->ImageSize.Height);
#else
			lImageSize.cx = min (lImageSize.cx, pImage->ImageSize.cx);
			lImageSize.cy = min (lImageSize.cy, pImage->ImageSize.cy);
#endif
			p32Bit = pImage->Is32Bit;
		}
		if	(p32Bit)
		{
#ifdef	__cplusplus_cli
			lRet = lImageSize.Width * lImageSize.Height * 4;
#else
			lRet = lImageSize.cx * lImageSize.cy * 4;
#endif
		}
		else
		{
#ifdef	__cplusplus_cli
			lRet = ((lImageSize.Width + 3) / 4) * 4 * lImageSize.Height;
#else
			lRet = ((lImageSize.cx + 3) / 4) * 4 * lImageSize.cy;
#endif
		}
		if	(
				(pImage)
			&&	(pImage->BitsSize)
			)
		{
			lRet = min (lRet, pImage->BitsSize);
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
UINT CAgentFile::GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame)
{
	return GetFrameBits (pImageBits, pFrame, false);
}

UINT CAgentFile::GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame, bool p32Bit)
{
	return GetFrameBits (pImageBits, pFrame, p32Bit, Color::Transparent);
}

UINT CAgentFile::GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor)
{
	return GetFrameBits (pImageBits, pFrame, p32Bit, pBkColor, -1);
}

UINT CAgentFile::GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor, Int16 pOverlayType)
#else
UINT CAgentFile::GetFrameBits (LPBYTE pImageBits, const CAgentFileFrame* pFrame, bool p32Bit, const COLORREF* pBkColor, SHORT pOverlayType)
#endif
{
	UINT	lRet = 0;

	if	(pFrame->ImageCount > 0)
	{
#ifdef	__cplusplus_cli
		lRet = GetImageBits (nullptr, nullptr, p32Bit);
#else
		lRet = GetImageBits (NULL, NULL, p32Bit);
#endif
	}

	if	(
			(pFrame->ImageCount > 0)
		&&	(pImageBits)
		)
	{
#ifdef	__cplusplus_cli
		CAgentFileImage^			lImage;
		System::Drawing::Point		lOffset;
		System::Drawing::Point		lPixel;
		ColorPalette^				lPalette = mHeader->Palette;
		BYTE						lTransparency = mHeader->Transparency;
		CAgentFileFrameOverlay^		lOverlay;
#else
		tPtr <CAgentFileImage>		lImage;
		CPoint						lOffset;
		CPoint						lPixel;
		LPCOLORREF					lPalette = mHeader.Palette;
		BYTE						lTransparency = mHeader.Transparency;
		USHORT						lOverlayNdx;
#endif
		INT_PTR						lMaxNdx = (long)((short)pFrame->ImageCount)-1;
		INT_PTR						lImageNdx = lMaxNdx + 1;
		INT_PTR						lSrcNdx;
		INT_PTR						lTrgNdx;
		int							lSrcScanBytes;
		int							lTrgScanBytes;
		bool						lOverlayReplace = false;

		if	(pOverlayType >= 0)
		{
#ifdef	__cplusplus_cli
			if	(lOverlay = pFrame->mOverlays [(AgentMouthOverlay)pOverlayType])
			{
				lOverlayReplace = lOverlay->ReplaceFlag;
			}
#else
			for	(lOverlayNdx = 0; lOverlayNdx < pFrame->OverlayCount; lOverlayNdx++)
			{
				if	(pFrame->Overlays [lOverlayNdx].OverlayType == (AgentMouthOverlay)pOverlayType)
				{
					lOverlayReplace = pFrame->Overlays [lOverlayNdx].ReplaceFlag;
					break;
				}
			}
#endif
		}

		if	(p32Bit)
		{
			memset (pImageBits, 0, lRet);
		}
		else
		{
			memset (pImageBits, lTransparency, lRet);
		}

		do
		{
			lImageNdx--;
#ifdef	__cplusplus_cli
			lImage = nullptr;
#else
			lImage = NULL;
#endif
			if	(
					(lImageNdx == 0)
				&&	(lOverlayReplace)
				)
			{
				lImageNdx--;
			}

			if	(lImageNdx >= 0)
			{
#ifdef	__cplusplus_cli
				lImage = GetImage (pFrame->Images [lImageNdx]->ImageNdx, false);
				lOffset = pFrame->Images [lImageNdx]->Offset;
#else
				lImage = GetImage (pFrame->Images [lImageNdx].ImageNdx, false);
				lOffset = pFrame->Images [lImageNdx].Offset;
#endif
			}
			else
			if	(
					(lImageNdx == -1)
				&&	(pOverlayType >= 0)
				)
			{
#ifdef	__cplusplus_cli
				if	(lOverlay = pFrame->Overlays [(AgentMouthOverlay)pOverlayType])
				{
					lImage = GetImage (lOverlay->ImageNdx, false);
					lOffset = lOverlay->Offset;
				}
#else
				for	(lOverlayNdx = 0; lOverlayNdx < pFrame->OverlayCount; lOverlayNdx++)
				{
					if	(pFrame->Overlays [lOverlayNdx].OverlayType == (AgentMouthOverlay)pOverlayType)
					{
						lImage = GetImage (pFrame->Overlays [lOverlayNdx].ImageNdx, false);
						lOffset = pFrame->Overlays [lOverlayNdx].Offset;
						break;
					}
				}
#endif
			}
			else
			{
				break;
			}

			if	(!lImage)
			{
				break;
			}

			//LogMessage (LogDebug, _T("ImageSize [%d %d] FrameSize [%d %d] Offset [%d %d]"), mImageSize.cx, mImageSize.cy, lImage->mImageSize.cx, lImage->mImageSize.cy, lOffset.x, lOffset.y);

			if	(
#ifdef	__cplusplus_cli
					(lOffset.X == 0)
				&&	(lOffset.Y == 0)
				&&	(lImage->mImageSize.Width == mHeader->ImageSize.Width)
				&&	(lImage->mImageSize.Height == mHeader->ImageSize.Height)
#else
					(lOffset.x == 0)
				&&	(lOffset.y == 0)
				&&	(lImage->ImageSize.cx == mHeader.ImageSize.cx)
				&&	(lImage->ImageSize.cy == mHeader.ImageSize.cy)
#endif
				&&	(!p32Bit)
				)
			{
#ifdef	__cplusplus_cli
				pin_ptr <BYTE>	lImageBits = &lImage->Bits[0];
#else
				const BYTE*	lImageBits = lImage->Bits;
#endif

				if	(lImageNdx == lMaxNdx)
				{
#ifdef	__cplusplus_cli
					memcpy (pImageBits, (LPBYTE)lImageBits, lImage->BitsSize);
#else
					memcpy (pImageBits, lImageBits, lImage->BitsSize);
#endif
					continue;
				}

				for	(lSrcNdx = 0; lSrcNdx < min ((long)lImage->BitsSize, (long)lRet); lSrcNdx++)
				{
					if	(lImageBits [lSrcNdx] != lTransparency)
					{
						pImageBits [lSrcNdx] = lImageBits [lSrcNdx];
					}
				}
			}
			else
			{
#ifdef	__cplusplus_cli
				pin_ptr <BYTE>	lImageBits = &lImage->Bits[0];
#else
				const BYTE*	lImageBits = lImage->Bits;
#endif

#ifdef	__cplusplus_cli
				lSrcScanBytes = ((lImage->mImageSize.Width + 3) / 4) * 4;
				if	(p32Bit)
				{
					lTrgScanBytes = mHeader->ImageSize.Width * 4;
				}
				else
				{
					lTrgScanBytes = ((mHeader->ImageSize.Width + 3) / 4) * 4;
				}
#else
				lSrcScanBytes = ((lImage->ImageSize.cx + 3) / 4) * 4;
				if	(p32Bit)
				{
					lTrgScanBytes = mHeader.ImageSize.cx * 4;
				}
				else
				{
					lTrgScanBytes = ((mHeader.ImageSize.cx + 3) / 4) * 4;
				}
#endif

#ifdef	__cplusplus_cli
				for	(lPixel.Y = max (-lOffset.Y, 0); lPixel.Y < min (lImage->ImageSize.Height, mHeader->ImageSize.Height - lOffset.Y); lPixel.Y++)
#else
				for	(lPixel.y = max (-lOffset.y, 0); lPixel.y < min (lImage->ImageSize.cy, mHeader.ImageSize.cy - lOffset.y); lPixel.y++)
#endif
				{
#ifdef	__cplusplus_cli
					lTrgNdx = (mHeader->ImageSize.Height - (lPixel.Y + lOffset.Y) - 1) * lTrgScanBytes;
					if	(p32Bit)
					{
						lTrgNdx += max (lOffset.X * 4, 0);
					}
					else
					{
						lTrgNdx += max (lOffset.X, 0);
					}
#else
					lTrgNdx = (mHeader.ImageSize.cy - (lPixel.y + lOffset.y) - 1) * lTrgScanBytes;
					if	(p32Bit)
					{
						lTrgNdx += max (lOffset.x * 4, 0);
					}
					else
					{
						lTrgNdx += max (lOffset.x, 0);
					}
#endif
					if	(lTrgNdx >= (long)lRet)
					{
						break;
					}

#ifdef	__cplusplus_cli
					lSrcNdx = ((lImage->ImageSize.Height - lPixel.Y - 1) * lSrcScanBytes) + max (-lOffset.X, 0);
#else
					lSrcNdx = ((lImage->ImageSize.cy - lPixel.y - 1) * lSrcScanBytes) + max (-lOffset.x, 0);
#endif

#ifdef	__cplusplus_cli
					for	(lPixel.X = max (-lOffset.X, 0); lPixel.X < min (lImage->ImageSize.Width, mHeader->ImageSize.Width - lOffset.X); lPixel.X++)
#else
					for	(lPixel.x = max (-lOffset.x, 0); lPixel.x < min (lImage->ImageSize.cx, mHeader.ImageSize.cx - lOffset.x); lPixel.x++)
#endif
					{
						if	(p32Bit)
						{
							if	(
									(lImageBits [lSrcNdx] == lTransparency)
#ifdef	__cplusplus_cli
								||	(lPalette->Entries [lImageBits [lSrcNdx]].Equals (lPalette->Entries [lTransparency]))
#else
								||	(lPalette [lImageBits [lSrcNdx]] == lPalette [lTransparency])
#endif
								)
							{
#ifdef	__cplusplus_cli
								if	(pBkColor == Color::Empty)
								{
									*(COLORREF*)(pImageBits + lTrgNdx) = lPalette->Entries [lImageBits [lSrcNdx]].ToArgb() | 0xFF000000;
								}
								else
								if	(
										(lImageNdx == lMaxNdx)
									&&	(pBkColor != Color::Transparent)
									)
								{
									*(COLORREF*)(pImageBits + lTrgNdx) = pBkColor.ToArgb() | 0xFF000000;
								}
#else
								if	(
										(lImageNdx == lMaxNdx)
									&&	(pBkColor)
									)
								{
									*(COLORREF*)(pImageBits + lTrgNdx) = *pBkColor | 0xFF000000;
								}
#endif
							}
							else
							{
#ifdef	__cplusplus_cli
								*(COLORREF*)(pImageBits + lTrgNdx) = lPalette->Entries [lImageBits [lSrcNdx]].ToArgb() | 0xFF000000;
#else
								*(COLORREF*)(pImageBits + lTrgNdx) = lPalette [lImageBits [lSrcNdx]] | 0xFF000000;
#endif
							}
							lSrcNdx++;
							lTrgNdx += 4;
						}
						else
						{
							if	(lImageBits [lSrcNdx] != lTransparency)
							{
								pImageBits [lTrgNdx] = lImageBits [lSrcNdx];
							}
							lSrcNdx++;
							lTrgNdx++;
						}
					}
				}
			}
		}
		while (lImage);
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

System::Drawing::Bitmap^ CAgentFile::GetImageBitmap (int pImageNdx)
{
	return GetImageBitmap (pImageNdx, false);
}

System::Drawing::Bitmap^ CAgentFile::GetImageBitmap (int pImageNdx, bool p32Bit)
{
	return GetImageBitmap (pImageNdx, p32Bit, Color::Transparent);
}

System::Drawing::Bitmap^ CAgentFile::GetImageBitmap (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
{
	return GetImageBitmap (GetImage (pImageNdx, p32Bit, pBkColor));
}

System::Drawing::Bitmap^ CAgentFile::GetImageBitmap (CAgentFileImage^ pImage)
{
	System::Drawing::Bitmap^	lBitmap = nullptr;
	pin_ptr <BYTE>				lImageBits;

	if	(
			(pImage)
		&&	(pImage->Bits)
		&&	(lImageBits = &pImage->Bits[0])
		)
	{
		if	(pImage->Is32Bit)
		{
			if	(lBitmap = gcnew System::Drawing::Bitmap (pImage->mImageSize.Width, pImage->mImageSize.Height, pImage->mImageSize.Width*4, PixelFormat::Format32bppPArgb, (IntPtr)(LPBYTE)lImageBits))
			{
				lBitmap->RotateFlip (RotateFlipType::RotateNoneFlipY);
			}
		}
		else
		{
			if	(lBitmap = gcnew System::Drawing::Bitmap (pImage->mImageSize.Width, pImage->mImageSize.Height, pImage->BitsSize/pImage->mImageSize.Height, PixelFormat::Format8bppIndexed, (IntPtr)(LPBYTE)lImageBits))
			{
				lBitmap->RotateFlip (RotateFlipType::RotateNoneFlipY);
				lBitmap->Palette = mHeader->Palette;
			}
		}
	}
	return lBitmap;
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::Bitmap^ CAgentFile::GetFrameBitmap (CAgentFileFrame^ pFrame)
{
	return GetFrameBitmap (pFrame, false);
}

System::Drawing::Bitmap^ CAgentFile::GetFrameBitmap (CAgentFileFrame^ pFrame, bool p32Bit)
{
	return GetFrameBitmap (pFrame, p32Bit, Color::Transparent);
}

System::Drawing::Bitmap^ CAgentFile::GetFrameBitmap (CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor)
{
	return GetFrameBitmap (pFrame, p32Bit, pBkColor, -1);
}

System::Drawing::Bitmap^ CAgentFile::GetFrameBitmap (CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor, Int16 pOverlayType)
{
	System::Drawing::Bitmap^	lBitmap = nullptr;
	array <BYTE>^				lBits;
	UINT						lBitsSize;
	pin_ptr <BYTE>				lImageBits;

	if	(
			(pFrame)
		&&	(lBitsSize = GetFrameBits (nullptr, pFrame, p32Bit))
		&&	(lBits = gcnew array <BYTE> (lBitsSize))
		&&	(lImageBits = &lBits[0])
		&&	(GetFrameBits (lImageBits, pFrame, p32Bit, pBkColor, pOverlayType))
		)
	{
		if	(p32Bit)
		{
			if	(lBitmap = gcnew System::Drawing::Bitmap (mHeader->ImageSize.Width, mHeader->ImageSize.Height, mHeader->ImageSize.Width*4, PixelFormat::Format32bppPArgb, (IntPtr)(LPBYTE)lImageBits))
			{
				lBitmap->RotateFlip (RotateFlipType::RotateNoneFlipY);
			}
		}
		else
		{
			if	(lBitmap = gcnew System::Drawing::Bitmap (mHeader->ImageSize.Width, mHeader->ImageSize.Height, lBitsSize/mHeader->ImageSize.Height, PixelFormat::Format8bppIndexed, (IntPtr)(LPBYTE)lImageBits))
			{
				lBitmap->RotateFlip (RotateFlipType::RotateNoneFlipY);
				lBitmap->Palette = mHeader->Palette;
			}
		}
	}
	return lBitmap;
}

/////////////////////////////////////////////////////////////////////////////
#endif	//	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentFile::SoundCount::get()
{
	return 0;
}

int CAgentFile::GetSoundSize (int pSoundNdx)
{
	return 0;
}

array <BYTE>^ CAgentFile::GetSound (int pSoundNdx)
{
	return nullptr;
}

System::String^ CAgentFile::GetSoundFilePath (int pSoundNdx)
{
	return nullptr;
}
#else
INT_PTR CAgentFile::GetSoundCount () const
{
	return 0;
}

long CAgentFile::GetSoundSize (INT_PTR pSoundNdx)
{
	return 0;
}

LPCVOID CAgentFile::GetSound (INT_PTR pSoundNdx)
{
	return NULL;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFile::FreeNames ()
{
#ifdef	__cplusplus_cli
	mNames = gcnew CAgentFileNames (this);
#else
	mNames.DeleteAll ();
#endif
}

bool CAgentFile::ReadNames ()
{
	return ReadNames (true);
}

bool CAgentFile::ReadNames (bool pFirstLetterCaps)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
CAgentFileName^ CAgentFile::FindName (WORD pLangID)
#else
CAgentFileName* CAgentFile::FindName (WORD pLangID)
#endif
{
#ifdef	__cplusplus_cli
	CAgentFileName^	lName = nullptr;
#else
	CAgentFileName*	lName = NULL;
	INT_PTR				lNdx;
#endif

#ifdef	__cplusplus_cli
	if	(
			(mNames)
		&&	(mNames->Count > 0)
		)
#else
	if	(mNames.GetCount() > 0)
#endif
	{
		if	(pLangID == LANG_USER_DEFAULT)
		{
#ifdef	_DEBUG
			pLangID = LANGIDFROMLCID (GetThreadLocale ());
#else
			pLangID = GetUserDefaultUILanguage();
#endif
		}
		else
		if	(pLangID == LANG_SYSTEM_DEFAULT)
		{
			pLangID = GetSystemDefaultUILanguage();
		}

#ifdef	__cplusplus_cli
		if	(mNames->Contains (pLangID))
		{
			lName = mNames [pLangID];
		}
#else
		for	(lNdx = 0; lName = mNames (lNdx); lNdx++)
		{
			if	(lName->Language == pLangID)
			{
				break;
			}
		}
#endif

		if	(!lName)
		{
#ifdef	__cplusplus_cli
			for each (CAgentFileName^ lEachName in mNames)
			{
				if	(
						(PRIMARYLANGID (lEachName->Language) == PRIMARYLANGID (pLangID))
					&&	(
							(SUBLANGID (lEachName->Language) == SUBLANG_DEFAULT)
						||	(SUBLANGID (lEachName->Language) == SUBLANG_NEUTRAL)
						)
					)
				{
					lName = lEachName;
					break;
				}
			}
#else
			for	(lNdx = 0; lName = mNames (lNdx); lNdx++)
			{
				if	(
						(PRIMARYLANGID (lName->Language) == PRIMARYLANGID (pLangID))
					&&	(
							(SUBLANGID (lName->Language) == SUBLANG_DEFAULT)
						||	(SUBLANGID (lName->Language) == SUBLANG_NEUTRAL)
						)
					)
				{
					break;
				}
			}
#endif
		}

		if	(!lName)
		{
#ifdef	__cplusplus_cli
			for each (CAgentFileName^ lEachName in mNames)
			{
				if	(PRIMARYLANGID (lEachName->Language) == PRIMARYLANGID (pLangID))
				{
					lName = lEachName;
					break;
				}
			}
#else
			for	(lNdx = 0; lName = mNames (lNdx); lNdx++)
			{
				if	(PRIMARYLANGID (lName->Language) == PRIMARYLANGID (pLangID))
				{
					break;
				}
			}
#endif
		}

		if	(
				(!lName)
			&&	(pLangID != MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT))
			)
		{
			lName = FindName (MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT));
		}

		if	(!lName)
		{
#ifdef	__cplusplus_cli
			if	(mNames->Count > 0)
			{
				lName = mNames [0];
			}
#else
			lName = mNames (0);
#endif
		}

#ifdef	_DEBUG_FIND_NAME
		if	(
				(lName)
			&&	(LogIsActive ())
			)
		{
			LogMessage (_DEBUG_FIND_NAME, _T("[%s] FindName [%4u %4.4X] Found [%4u %4.4X] [%s]"), mPath, pLangID, pLangID, lName->mLanguage, lName->mLanguage, lName->mName);
		}
#endif
	}
	return lName;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFile::FreeStates ()
{
#ifdef	__cplusplus_cli
	mStates = gcnew CAgentFileStates (this);
#else
	mStates.mGestures.RemoveAll ();
	mStates.mNames.RemoveAll ();
#endif
}

bool CAgentFile::ReadStates ()
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFile::FreeGestures ()
{
#ifdef	__cplusplus_cli
	mGestures = gcnew CAgentFileGestures (this);
#else
	mGestures.mAnimations.DeleteAll ();
	mGestures.mNames.RemoveAll ();
#endif
}

bool CAgentFile::ReadGestures ()
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

void CAgentFile::Log (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("Character");
			}
			LogMessage (pLogLevel, _T("%s [%s]"), lTitle, mPath);
			LogMessage (pLogLevel|LogHighVolume, _T("  Version         [%u.%2.2u]"), mVersionMajor, mVersionMinor);
			LogMessage (pLogLevel|LogHighVolume, _T("  Guid            [%s]"), (LPCTSTR) CGuidStr (mHeader.Guid));
			LogMessage (pLogLevel|LogHighVolume, _T("  Image Size      [%d %d]"), mHeader.ImageSize.cx, mHeader.ImageSize.cy);
			LogMessage (pLogLevel|LogHighVolume, _T("  Transparency    [%d]"), mHeader.Transparency);
			LogMessage (pLogLevel|LogHighVolume, _T("  Style           [%8.8X]"), mHeader.Style);
			LogMessage (pLogLevel|LogHighVolume, _T("  Icon            [%p]"), mHeader.Icon);
			LogTts (mTts, pLogLevel|LogHighVolume, _T("  Tts"));
			LogBalloon (mBalloon, pLogLevel|LogHighVolume, _T("  Balloon"));
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentFile::LogNames (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString				lTitle;
			INT_PTR					lNameNdx;
			const CAgentFileName*	lName;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("Names");
			}
			LogMessage (pLogLevel, _T("%s [%d] [%s]"), lTitle, mNames.GetCount(), mPath);

			for	(lNameNdx = 0; lNameNdx < (INT_PTR)mNames.GetCount(); lNameNdx++)
			{
				if	(lName = mNames (lNameNdx))
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  [%4.4X] [%ls] [%ls]"), lName->Language, (BSTR)lName->Name, DebugStr(CAtlString((BSTR)lName->Desc1)));
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentFile::LogStates (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			INT_PTR		lNdx;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("States");
			}
			LogMessage (pLogLevel, _T("%s [%d] [%s]"), lTitle, mStates.mGestures.GetCount(), mPath);

			for	(lNdx = 0; lNdx < (INT_PTR)mStates.mGestures.GetCount(); lNdx++)
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  %s [%s]"), mStates.mNames [lNdx], JoinStringArray (mStates.mGestures [lNdx], _T("|"), true));
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentFile::LogGestures (UINT pLogLevel, LPCTSTR pFormat, ...) const
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString					lTitle;
			INT_PTR						lNdx;
			const CAgentFileAnimation*	lAnimation;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();
			}
			else
			{
				lTitle = _T("Gestures");
			}
			LogMessage (pLogLevel, _T("%s [%d] [%s]"), lTitle, mGestures.mAnimations.GetCount(), mPath);

			for	(lNdx = 0; lNdx < (INT_PTR)mGestures.mAnimations.GetCount(); lNdx++)
			{
				if	(lAnimation = mGestures.mAnimations [lNdx])
				{
					CAtlString	lSuffix;

					if	(lAnimation->mFileName.Ptr())
					{
						lSuffix.Format (_T(" File [%ls] [%8.8X]"), (BSTR)lAnimation->mFileName, lAnimation->mFileCheckSum);
					}
					LogMessage (pLogLevel|LogHighVolume, _T("  %s Name [%ls] Frames [%hu] Return [%hu] [%ls]%s"), mGestures.mNames [lNdx], (BSTR)lAnimation->mName, lAnimation->mFrameCount, lAnimation->mReturnType, (BSTR)lAnimation->mReturnName, lSuffix);
				}
				else
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  %s"), mGestures.mNames [lNdx]);
				}
			}
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFile::LogTts (const CAgentFileTts& pTts, UINT pLogLevel, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			CAtlString	lIndent;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();

				lIndent = lTitle;
				lIndent.TrimLeft ();
				if	(lIndent.GetLength() < lTitle.GetLength())
				{
					lIndent = CAtlString (_T(' '), lTitle.GetLength() - lIndent.GetLength ());
				}
				else
				{
					lIndent.Empty ();
				}
			}
			else
			{
				lTitle = _T("Tts");
			}

			if	(!lTitle.IsEmpty ())
			{
				LogMessage (pLogLevel, _T("%s"), lTitle);
			}
			LogMessage (pLogLevel, _T("%s  Engine        [%s]"), lIndent, (LPCTSTR) CGuidStr (pTts.Engine));
			LogMessage (pLogLevel, _T("%s  Mode          [%s]"), lIndent, (LPCTSTR) CGuidStr (pTts.Mode));
			LogMessage (pLogLevel, _T("%s  Gender        [%hu]"), lIndent, pTts.Gender);
			LogMessage (pLogLevel, _T("%s  Age           [%hu]"), lIndent, pTts.Age);
			LogMessage (pLogLevel, _T("%s  Speed         [%d]"), lIndent, pTts.Speed);
			LogMessage (pLogLevel, _T("%s  Pitch         [%hd]"), lIndent, pTts.Pitch);
			LogMessage (pLogLevel, _T("%s  Language      [%4.4hu %4.4hX]"), lIndent, pTts.Language, pTts.Language);
			LogMessage (pLogLevel, _T("%s  Style         [%ls]"), lIndent, (BSTR)pTts.Style);
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentFile::LogBalloon (const CAgentFileBalloon& pBalloon, UINT pLogLevel, LPCTSTR pFormat, ...)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle;
			CAtlString	lIndent;

			if	(pFormat)
			{
				va_list lArgPtr;
				va_start (lArgPtr, pFormat);
				_vsntprintf (lTitle.GetBuffer(2048), 2048, pFormat, lArgPtr);
				lTitle.ReleaseBuffer ();

				lIndent = lTitle;
				lIndent.TrimLeft ();
				if	(lIndent.GetLength() < lTitle.GetLength())
				{
					lIndent = CAtlString (_T(' '), lTitle.GetLength() - lIndent.GetLength ());
				}
				else
				{
					lIndent.Empty ();
				}
			}
			else
			{
				lTitle = _T("Balloon");
			}

			if	(!lTitle.IsEmpty ())
			{
				LogMessage (pLogLevel, _T("%s"), lTitle);
			}
				LogMessage (pLogLevel, _T("%s  Lines         [%u]"), lIndent, pBalloon.Lines);
				LogMessage (pLogLevel, _T("%s  Per Line      [%u]"), lIndent, pBalloon.PerLine);
				LogMessage (pLogLevel, _T("%s  Foreground    [%8.8X]"), lIndent, pBalloon.FgColor);
				LogMessage (pLogLevel, _T("%s  Background    [%8.8X]"), lIndent, pBalloon.BkColor);
				LogMessage (pLogLevel, _T("%s  Border        [%8.8X]"), lIndent, pBalloon.BrColor);
				LogMessage (pLogLevel, _T("%s  Font Name     [%ls]"), lIndent, pBalloon.Font.lfFaceName);
				LogMessage (pLogLevel, _T("%s  Font Height   [%d]"), lIndent, pBalloon.Font.lfHeight);
				LogMessage (pLogLevel, _T("%s  Font Weight   [%hu]"), lIndent, pBalloon.Font.lfWeight);
				LogMessage (pLogLevel, _T("%s  Font Italic   [%hu]"), lIndent, pBalloon.Font.lfItalic);
				LogMessage (pLogLevel, _T("%s  Font Under    [%hu]"), lIndent, pBalloon.Font.lfUnderline);
				LogMessage (pLogLevel, _T("%s  Font Strike   [%hu]"), lIndent, pBalloon.Font.lfStrikeOut);
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFile::DumpPalette (LPVOID pPalette, DWORD pPaletteSize)
{
#ifdef	_DEBUG
	tArrayPtr <BYTE>	lInfoBuff;
	BITMAPINFO *		lInfo;
	ATL::CImage			lDumpBmp;
	LPBYTE				lDumpBits;

	lInfoBuff = new BYTE [sizeof (BITMAPINFOHEADER) + (sizeof(COLORREF) * pPaletteSize)];
	lInfo = (BITMAPINFO *) (LPBYTE) lInfoBuff;

	lInfo->bmiHeader.biSize = sizeof (lInfo->bmiHeader);
	lInfo->bmiHeader.biWidth = 256;
	lInfo->bmiHeader.biHeight = -256;
	lInfo->bmiHeader.biBitCount = 8;
	lInfo->bmiHeader.biPlanes = 1;
	lInfo->bmiHeader.biCompression = BI_RGB;
	lInfo->bmiHeader.biSizeImage = lInfo->bmiHeader.biWidth * abs (lInfo->bmiHeader.biHeight);
	lInfo->bmiHeader.biClrUsed = pPaletteSize;
	memcpy (lInfo->bmiColors, pPalette, sizeof(DWORD) * pPaletteSize);

	lDumpBmp.Attach (CreateDIBSection (NULL, lInfo, DIB_RGB_COLORS, NULL, NULL, 0));
	if	(lDumpBits = ::GetImageBits (lDumpBmp))
	{
		for	(long lNdx = 0; lNdx < (long) lInfo->bmiHeader.biSizeImage; lNdx++)
		{
			lDumpBits [lNdx] = (BYTE) ((lNdx >> 4) & 0x000F) | ((lNdx >> 8) & 0x00F0);
		}
		GdiFlush ();

#ifdef	_SAVE_IMAGE
		CImageDebugger::SaveBitmap (lDumpBmp, _T("Palette"));
#else
#ifdef	_SAVE_PALETTE
		lDumpName += _T(" - Palette");
		CImageDebugger::SaveBitmap (lDumpBmp, lDumpName);
#endif
#endif
#ifdef	_DUMP_PALETTE
		CImageDebugger::DumpBitmap (_DUMP_PALETTE, *lInfo, lDumpBits, _T("Palette"));
#endif
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFile::SaveImage (CAgentFileImage* pImage)
{
#ifdef	_DEBUG
	if	(pImage)
	{
		try
		{
			ATL::CImage			lBitmap;
			tArrayPtr <BYTE>	lInfoBuffer;
			LPBITMAPINFO		lInfo;
			LPVOID				lBits = NULL;

			if	(
					(lInfoBuffer = new BYTE [GetImageFormat (NULL)])
				&&	(lInfo = (LPBITMAPINFO) (LPBYTE) lInfoBuffer)
				)
			{
				GetImageFormat (lInfo, pImage, false);

				lBitmap.Attach (CreateDIBSection (NULL, lInfo, DIB_RGB_COLORS, NULL, NULL, 0));
				if	(lBits = ::GetImageBits (lBitmap))
				{
					CAtlString	lDumpName;

					memcpy (lBits, pImage->Bits, pImage->BitsSize);
					GdiFlush ();
					lDumpName.Format (_T("Image %.4u"), pImage->ImageNum);
					CImageDebugger::SaveBitmap (lBitmap, lDumpName);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
