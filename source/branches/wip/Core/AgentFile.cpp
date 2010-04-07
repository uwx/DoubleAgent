/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include <shlwapi.h>
#include <wininet.h>
#include "DaCore.h"
#include "AgentFile.h"
#include "FileDownload.h"
#include "GuidStr.h"
#include "MmSysError.h"
#include "ImageTools.h"
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "ImageDebugger.h"
#include "DebugStr.h"
#include "DebugProcess.h"
#endif
#if	(_MFC_VER < 0x0800)
#define	genericException generic
#endif

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon.lib")

#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_DUMP			LogDebugFast
//#define	_DEBUG_FIND_NAME	LogDebugFast
//#define	_DEBUG_LOAD			LogDebugFast
//#define	_DEBUG_BLOCKS		LogDebugFast
//#define	_DEBUG_INDEX		LogDebugFast

//#define	_DEBUG_ANIMATION	LogDebugFast
//#define	_SAVE_PALETTE		LogDebugFast
//#define	_SAVE_ICON			LogDebugFast
//#define	_SAVE_IMAGE			LogDebugFast

//#define	_DUMP_ANIMATION		LogDebugFast
//#define	_DUMP_PALETTE		LogDebugFast
//#define	_DUMP_ICON			LogDebugFast
//#define	_DUMP_IMAGE			LogDebugFast
//#define	_SHOW_IMAGE			LogDebugFast
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_LOAD_ERRS
#define	_LOG_LOAD_ERRS		LogDetails
#endif

//////////////////////////////////////////////////////////////////////

typedef const BYTE * LPCBYTE;
typedef const WORD * LPCWORD;
typedef const DWORD * LPCDWORD;
typedef const GUID * LPCGUID;
typedef const ULARGE_INTEGER * LPCULARGE_INTEGER;

static const DWORD	sAcsFileSignature = 0xABCDABC3;
static const DWORD	sAcfFileSignature = 0xABCDABC4;

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentFileName)
IMPLEMENT_DLL_OBJECT(CAgentFileTts)
IMPLEMENT_DLL_OBJECT(CAgentFileBalloon)
IMPLEMENT_DLL_OBJECT(CAgentFileImage)
IMPLEMENT_DLL_OBJECT(CAgentFileFrameImage)
IMPLEMENT_DLL_OBJECT(CAgentFileFrameOverlay)
IMPLEMENT_DLL_OBJECT(CAgentFileFrame)
IMPLEMENT_DLL_OBJECT(CAgentFileAnimation)
IMPLEMENT_DLL_OBJECT(CAgentFileGestures)
IMPLEMENT_DLL_OBJECT(CAgentFileStates)
IMPLEMENT_DLL_OBJECT(CAgentFile)

//////////////////////////////////////////////////////////////////////

CAgentFileAnimation * CAgentFileGestures::operator() (const CAtlString & pName) {return mAnimations (FindSortedString (mNames, pName));}
const CAgentFileAnimation * CAgentFileGestures::operator() (const CAtlString & pName) const {return mAnimations (FindSortedString (mNames, pName));}

CAtlStringArray * CAgentFileStates::operator() (const CAtlString & pName) {return mGestures (FindSortedString (mNames, pName));}
const CAtlStringArray * CAgentFileStates::operator() (const CAtlString & pName) const {return mGestures (FindSortedString (mNames, pName));}

//////////////////////////////////////////////////////////////////////

CAgentFile::CAgentFile()
:	mFileNamesOffset (0),
	mFileStatesOffset (0),
	mSignature (0),
	mVersionMajor (0),
	mVersionMinor (0),
	mStyle (0),
	mTransparency (10),
	mImageSize (0,0),
	mIcon (NULL),
	mGuid (GUID_NULL)
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFile::CAgentFile"), this);
#endif
}

CAgentFile::~CAgentFile ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFile::~CAgentFile"), this);
#endif
	Close ();
}

CAgentFile * CAgentFile::CreateInstance ()
{
	return new CAgentFile;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CAgentFile::Open (LPCTSTR pPath, UINT pLogLevel)
{
	HRESULT		lResult = S_OK;
	CAtlString	lPath = ParseFilePath (pPath);

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif

	Close ();

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Open [%s]"), lPath);
	}

	mPath = lPath;

	if	(PathIsURL (lPath))
	{
		tPtr <CFileDownload>	lOldDownload = mFileDownload.Detach();

		mFileDownload = CFileDownload::CreateInstance (lPath);
		if	(lOldDownload)
		{
			mFileDownload->SetBindFlags (lOldDownload->GetBindFlags());
		}
		SafeFreeSafePtr (lOldDownload);

		if	(SUCCEEDED (lResult = mFileDownload->Download ()))
		{
			lResult = LoadFile (CAtlString ((BSTR) mFileDownload->GetCacheName()), pLogLevel);
		}
	}
	else
	{
		lResult = LoadFile (lPath, pLogLevel);
	}
	if	(FAILED (lResult))
	{
		mPath.Empty ();
	}
	return lResult;
}

void CAgentFile::Close ()
{
	FreeNames ();
	FreeStates ();
	FreeGestures ();

	mFileView.Close ();
	mFileMapping.Close ();
	mFileHandle.Close ();
	mFileSize = 0;

	if	(mIcon)
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::DestroyIcon [%p]"), this, mIcon);
		}
#endif

		DestroyIcon (mIcon);
		mIcon = NULL;

#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::DestroyIcon [%p] Done"), this, mIcon);
		}
#endif
	}
	mGuid = GUID_NULL;
}

//////////////////////////////////////////////////////////////////////

void CAgentFile::SetDownloadMode (bool pRefresh, bool pReload, bool pSecure)
{
	if	(!mFileDownload)
	{
		mFileDownload = CFileDownload::CreateInstance (NULL);
	}
	if	(pReload)
	{
		mFileDownload->SetReloadMode (pReload);
	}
	else
	{
		mFileDownload->SetResynchronizeMode (pRefresh);
	}
	mFileDownload->SetSecurityMode (pSecure);
}

HRESULT CAgentFile::LoadAcf (CFileDownload * pDownload, UINT pLogLevel)
{
	HRESULT	lResult = E_FAIL;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif
	Close ();

	if	(
			(pDownload)
		&&	(pDownload->IsDownloadComplete () == S_OK)
		)
	{
		lResult = LoadFile (CAtlString ((BSTR) pDownload->GetCacheName()), pLogLevel);
		if	(SUCCEEDED (lResult))
		{
			mPath = ParseFilePath (CAtlString ((BSTR) pDownload->GetURL ()));
		}
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CAgentFile::LoadFile (LPCTSTR pPath, UINT pLogLevel)
{
	HRESULT	lResult = E_FAIL;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Load [%s]"), pPath);
	}

	mFileHandle = CreateFile (pPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if	(
			(mFileHandle.SafeIsValid ())
		&&	(mFileSize = GetFileSize (mFileHandle, NULL))
		&&	((mFileMapping = CreateFileMapping (mFileHandle, NULL, PAGE_READONLY, 0, 0, NULL)).SafeIsValid())
		&&	((mFileView = MapViewOfFile (mFileMapping, FILE_MAP_READ, 0, 0, mFileSize)).SafeIsValid())
		)
	{
		lResult = S_OK;
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (GetLastError());
		if	(SUCCEEDED (lResult))
		{
			lResult = E_FAIL;
		}
	}

	if	(SUCCEEDED (lResult))
	{
		lResult = ReadHeader (pLogLevel);

		if	(SUCCEEDED (lResult))
		{
			if	(LogIsActive (pLogLevel))
			{
				ReadNames (true, pLogLevel);
				ReadStates (pLogLevel);
				ReadGestures (pLogLevel);
			}
#ifdef	_DUMP_IMAGE
			if	(
					(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_IMAGE)))
				&&	(ReadImageIndex (pLogLevel))
				)
			{
				DumpAcsImages (pLogLevel);
			}
#else
#ifdef	_SAVE_IMAGE
			if	(
					(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_IMAGE)))
				&&	(ReadImageIndex (pLogLevel))
				)
			{
				DumpAcsImages (pLogLevel);
			}
#else
#ifdef	_DEBUG_INDEX
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_INDEX)))
#else
			if	(LogIsActive (pLogLevel))
#endif
			{
				ReadImageIndex (pLogLevel);
			}
#endif
#endif
#ifdef	_DEBUG_INDEX
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_INDEX)))
#else
			if	(LogIsActive (pLogLevel))
#endif
			{
				ReadSoundIndex (pLogLevel);
			}
#ifdef	_DEBUG_DUMP
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_DUMP)))
			{
				LogDump (_DEBUG_DUMP, mFileView, mFileSize, 0, true);
			}
#endif
#ifdef	_DEBUG_BLOCKS
			if	(IsAcsFile ())
			{
				DumpBlocks (MaxLogLevel (pLogLevel,_DEBUG_BLOCKS));
			}
#endif
		}
	}
	if	(LogIsActive())
	{
		LogComErr ((lResult==AGENTPROVERROR_MAGIC)?LogDetails:LogNormal, lResult, _T("Load [%s]"), pPath);
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

tBstrPtr CAgentFile::GetPath () const
{
	return mPath.AllocSysString();
}

tBstrPtr CAgentFile::GetFileName () const
{
	CAtlString	lFileName (PathFindFileName (mPath));
	return lFileName.AllocSysString();
}

//////////////////////////////////////////////////////////////////////

bool CAgentFile::IsAcsFile () const
{
	if	(mFileView.SafeIsValid ())
	{
		return (mSignature == sAcsFileSignature);
	}
	return false;
}

bool CAgentFile::IsAcfFile () const
{
	if	(mFileView.SafeIsValid ())
	{
		return (mSignature == sAcfFileSignature);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

const GUID & CAgentFile::GetGuid () const
{
	return mGuid;
}

DWORD CAgentFile::GetVersion () const
{
	return MAKELONG (mVersionMinor, mVersionMajor);
}

DWORD CAgentFile::GetStyle () const
{
	return mStyle;
}

HICON CAgentFile::GetIcon () const
{
	return mIcon;
}

const CAgentFileBalloon & CAgentFile::GetBalloon () const
{
	return mBalloon;
}

const CAgentFileTts & CAgentFile::GetTts () const
{
	return mTts;
}

//////////////////////////////////////////////////////////////////////

const CAtlPtrTypeArray <CAgentFileName> & CAgentFile::GetNames () const
{
	return mNames;
}

const CAgentFileStates & CAgentFile::GetStates () const
{
	return mStates;
}

const CAgentFileGestures & CAgentFile::GetGestures () const
{
	return mGestures;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::FindGesture (LPCTSTR pGestureName)
{
	CAtlString	lGestureName (pGestureName);

	if	(!lGestureName.IsEmpty ())
	{
		if	(
				(mGestures.mAnimations.GetCount () <= 0)
			&&	(IsAcsFile ())
			)
		{
			ReadGestures ();
		}

		return FindSortedString (mGestures.mNames, lGestureName);
	}
	return -1;
}

const CAgentFileAnimation * CAgentFile::GetGesture (INT_PTR pGestureNdx)
{
	CAgentFileAnimation *	lRet = NULL;

	if	(
			(mGestures.mAnimations.GetCount () <= 0)
		&&	(IsAcsFile ())
		)
	{
		ReadGestures ();
	}

	if	(
			(pGestureNdx >= 0)
		&&	(pGestureNdx < (INT_PTR)mGestures.mAnimations.GetCount ())
		)
	{
		lRet = mGestures.mAnimations [pGestureNdx];

		if	(
				(lRet)
			&&	(IsAcfFile ())
			)
		{
			ReadAcaFile (lRet);
		}
	}
	return lRet;
}

const CAgentFileAnimation * CAgentFile::GetGesture (LPCTSTR pGestureName)
{
	return GetGesture (FindGesture (pGestureName));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::FindAnimation (LPCTSTR pAnimationName)
{
	CAtlString				lAnimationName (pAnimationName);
	CAgentFileAnimation *	lAnimation;
	INT_PTR					lNdx;

	if	(!lAnimationName.IsEmpty ())
	{
		if	(
				(mGestures.mAnimations.GetCount() <= 0)
			&&	(IsAcsFile ())
			)
		{
			ReadGestures ();
		}

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

const CAgentFileAnimation * CAgentFile::GetAnimation (INT_PTR pAnimationNdx)
{
	CAgentFileAnimation *	lRet = NULL;

	if	(
			(mGestures.mAnimations.GetCount() <= 0)
		&&	(IsAcsFile ())
		)
	{
		ReadGestures ();
	}

	if	(
			(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount ())
		)
	{
		lRet = mGestures.mAnimations [pAnimationNdx];

		if	(
				(lRet)
			&&	(IsAcfFile ())
			)
		{
			ReadAcaFile (lRet);
		}
	}
	return lRet;
}

const CAgentFileAnimation * CAgentFile::GetAnimation (LPCTSTR pAnimationName)
{
	return GetAnimation (FindAnimation (pAnimationName));
}

//////////////////////////////////////////////////////////////////////

bool CAgentFile::IsAnimationLoaded (INT_PTR pAnimationNdx)
{
	bool					lRet = false;
	CAgentFileAnimation *	lAnimation = NULL;

	if	(
			(IsAcfFile ())
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount ())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		&&	(lAnimation->mAcaChksum == (DWORD)-1)
		)
	{
		lRet = true;
	}
	return lRet;
}

HRESULT CAgentFile::LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload * pDownload)
{
	HRESULT					lResult = S_OK;
	CAgentFileAnimation *	lAnimation = NULL;

	if	(!pDownload)
	{
		lResult = E_POINTER;
	}
	else
	if	(!IsAcfFile ())
	{
		lResult = AGENTPROVERROR_CHARACTERINVALID;
	}
	else
	if	(
			(pAnimationNdx >= 0)
		&&	(pAnimationNdx <= (INT_PTR)mGestures.mAnimations.GetUpperBound ())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		)
	{
		if	(GetAcaPath (lAnimation).CompareNoCase (CAtlString ((BSTR) pDownload->GetURL ())) != 0)
		{
			lResult = AGENTERR_INVALIDANIMATION;
		}
		else
		if	(lAnimation->mAcaChksum == (DWORD)-1)
		{
			lResult = S_FALSE;
		}
		else
		{
			lResult = ReadAcaFile (lAnimation, CAtlString ((BSTR) pDownload->GetCacheName()));
			lAnimation->mAcaChksum = (DWORD)-1;
		}
	}
	else
	{
		lResult = AGENTERR_ANIMATIONNOTFOUND;
	}
	return lResult;
}

tBstrPtr CAgentFile::GetAnimationAcaPath (INT_PTR pAnimationNdx)
{
	CAtlString				lPath;
	CAgentFileAnimation *	lAnimation = NULL;

	if	(
			(IsAcfFile ())
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx <= (INT_PTR)mGestures.mAnimations.GetUpperBound ())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		)
	{
		lPath = GetAcaPath (lAnimation);
	}
	return lPath.AllocSysString();
}

//////////////////////////////////////////////////////////////////////

bool CAgentFile::IsAnimationLoaded (LPCTSTR pAnimationName)
{
	return IsAnimationLoaded (FindAnimation (pAnimationName));
}

HRESULT CAgentFile::LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload * pDownload)
{
	return LoadAnimationAca (FindAnimation (pAnimationName), pDownload);
}

tBstrPtr CAgentFile::GetAnimationAcaPath (LPCTSTR pAnimationName)
{
	return GetAnimationAcaPath (FindAnimation (pAnimationName));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSize CAgentFile::GetImageSize () const
{
	return mImageSize;
}

BYTE CAgentFile::GetTransparency () const
{
	return mTransparency;
}

SHORT CAgentFile::GetPaletteSize () const
{
	return (mPalette.Ptr() ? 0 : 256);
}

LPCOLORREF CAgentFile::GetPalette () const
{
	return (LPCOLORREF)(const COLORREF *)mPalette;
}

//////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::GetImageCount () const
{
	if	(IsAcsFile ())
	{
		return mImageIndex.GetSize ();
	}
	else
	if	(IsAcfFile ())
	{
		return mAcaImages.GetSize ();
	}
	return 0;
}

CAgentFileImage * CAgentFile::GetImage (INT_PTR pImageNdx, bool p32Bit, UINT pLogLevel)
{
	tPtr <CAgentFileImage>	lImage;

	if	(IsAcsFile ())
	{
		if	(
				(pImageNdx >= 0)
			&&	(pImageNdx <= (INT_PTR)mImageIndex.GetUpperBound ())
			)
		{
			lImage = ReadAcsImage (mImageIndex [pImageNdx].LowPart, mImageIndex [pImageNdx].HighPart, (UINT)pImageNdx, p32Bit, pLogLevel);
		}
	}
	else
	if	 (IsAcfFile ())
	{
		const CAgentFileImage *	lAcfImage;

		if	(
				(pImageNdx >= 0)
			&&	(pImageNdx <= (INT_PTR)mAcaImages.GetUpperBound ())
			&&	(lAcfImage = mAcaImages [pImageNdx])
			&&	(lImage = new CAgentFileImage)
			&&	(lImage->mBits = new BYTE [lAcfImage->mBitsSize])
			)
		{
			lImage->mImageNum = lAcfImage->mImageNum;
			lImage->mImageSize = lAcfImage->mImageSize;
			lImage->mBitsSize = lAcfImage->mBitsSize;
			memcpy (lImage->mBits.Ptr(), lAcfImage->mBits.Ptr(), lImage->mBitsSize);
		}
		else
		{
			lImage = NULL;
		}
	}

	return lImage.Detach ();
}

UINT CAgentFile::GetImageFormat (LPBITMAPINFO pImageInfo, const CAgentFileImage * pImage, bool p32Bit) const
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
			pImageInfo->bmiHeader.biWidth = pImage->mImageSize.cx;
			pImageInfo->bmiHeader.biHeight = pImage->mImageSize.cy;
		}
		else
		{
			pImageInfo->bmiHeader.biWidth = mImageSize.cx;
			pImageInfo->bmiHeader.biHeight = mImageSize.cy;
		}
		pImageInfo->bmiHeader.biSizeImage = GetImageBits (NULL, pImage, p32Bit);
	}
	if	(
			(!p32Bit)
		&&	(mPalette.Ptr())
		)
	{
		lRet += sizeof(COLORREF) * 256;

		if	(pImageInfo)
		{
			pImageInfo->bmiHeader.biClrImportant = 256;
			pImageInfo->bmiHeader.biClrUsed = 256;
			memcpy (pImageInfo->bmiColors, (const COLORREF *)mPalette, sizeof(COLORREF) * 256);
		}
	}
	return lRet;
}

UINT CAgentFile::GetImageBits (LPBYTE pImageBits, const CAgentFileImage * pImage, bool p32Bit) const
{
	UINT	lRet = 0;

	if	(
			(pImageBits)
		&&	(pImage)
		&&	(pImage->mBitsSize)
		)
	{
		lRet = pImage->mBitsSize;
		memcpy (pImageBits, (LPBYTE)pImage->mBits.Ptr(), pImage->mBitsSize);
	}
	else
	{
		CSize	lImageSize (mImageSize);

		if	(pImage)
		{
			lImageSize.cx = min (lImageSize.cx, pImage->mImageSize.cx);
			lImageSize.cy = min (lImageSize.cy, pImage->mImageSize.cy);
		}
		if	(p32Bit)
		{
			lRet = lImageSize.cx * lImageSize.cy * 4;
		}
		else
		{
			lRet = ((lImageSize.cx + 3) / 4) * 4 * lImageSize.cy;
		}
		if	(
				(pImage)
			&&	(pImage->mBitsSize)
			)
		{
			lRet = min (lRet, pImage->mBitsSize);
		}
	}
	return lRet;
}

UINT CAgentFile::GetFrameBits (LPBYTE pImageBits, const CAgentFileFrame & pFrame, bool p32Bit, const COLORREF * pBkColor, SHORT pOverlayNdx)
{
	UINT	lRet = 0;

	if	(pFrame.mImageCount > 0)
	{
		lRet = GetImageBits (NULL, NULL, p32Bit);
	}

	if	(
			(pFrame.mImageCount > 0)
		&&	(pImageBits)
		)
	{
		tPtr <CAgentFileImage>	lImage;
		BYTE					lTransparency = GetTransparency();
		const COLORREF *		lPalette = GetPalette();
		INT_PTR					lMaxNdx = (long)((short)pFrame.mImageCount)-1;
		INT_PTR					lImageNdx = lMaxNdx + 1;
		INT_PTR					lSrcNdx;
		INT_PTR					lTrgNdx;
		int						lSrcScanBytes;
		int						lTrgScanBytes;
		CPoint					lOffset;
		CPoint					lPixel;
		WORD					lOverlayNdx;
		bool					lOverlayReplace = false;

		if	(pOverlayNdx >= 0)
		{
			for	(lOverlayNdx = 0; lOverlayNdx < pFrame.mOverlayCount; lOverlayNdx++)
			{
				if	(pFrame.mOverlays [lOverlayNdx].mOverlayNdx == (WORD)pOverlayNdx)
				{
					lOverlayReplace = (pFrame.mOverlays [lOverlayNdx].mReplaceFlag!=0);
					break;
				}
			}
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
			lImage = NULL;

			if	(
					(lImageNdx == 0)
				&&	(lOverlayReplace)
				)
			{
				lImageNdx--;
			}

			if	(lImageNdx >= 0)
			{
				lImage = GetImage (pFrame.mImages [lImageNdx].mImageNdx, false);
				lOffset = pFrame.mImages [lImageNdx].mOffset;
			}
			else
			if	(
					(lImageNdx == -1)
				&&	(pOverlayNdx >= 0)
				)
			{
				for	(lOverlayNdx = 0; lOverlayNdx < pFrame.mOverlayCount; lOverlayNdx++)
				{
					if	(pFrame.mOverlays [lOverlayNdx].mOverlayNdx == (WORD)pOverlayNdx)
					{
						lImage = GetImage (pFrame.mOverlays [lOverlayNdx].mImageNdx, false);
						lOffset = pFrame.mOverlays [lOverlayNdx].mOffset;
						break;
					}
				}
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
					(lOffset.x == 0)
				&&	(lOffset.y == 0)
				&&	(lImage->mImageSize.cx == mImageSize.cx)
				&&	(lImage->mImageSize.cy == mImageSize.cy)
				&&	(!p32Bit)
				)
			{
				if	(lImageNdx == lMaxNdx)
				{
					memcpy (pImageBits, (LPBYTE)lImage->mBits, lImage->mBitsSize);
					continue;
				}

				for	(lSrcNdx = 0; lSrcNdx < min ((long)lImage->mBitsSize, (long)lRet); lSrcNdx++)
				{
					if	(lImage->mBits [lSrcNdx] != lTransparency)
					{
						pImageBits [lSrcNdx] = lImage->mBits [lSrcNdx];
					}
				}
			}
			else
			{
				lSrcScanBytes = ((lImage->mImageSize.cx + 3) / 4) * 4;
				if	(p32Bit)
				{
					lTrgScanBytes = mImageSize.cx * 4;
				}
				else
				{
					lTrgScanBytes = ((mImageSize.cx + 3) / 4) * 4;
				}

				for	(lPixel.y = max (-lOffset.y, 0); lPixel.y < min (lImage->mImageSize.cy, mImageSize.cy - max (lOffset.y, 0)); lPixel.y++)
				{
					lTrgNdx = (mImageSize.cy - (lPixel.y + lOffset.y) - 1) * lTrgScanBytes;
					if	(lTrgNdx >= (long)lRet)
					{
						break;
					}

					if	(p32Bit)
					{
						lTrgNdx += max (lOffset.x * 4, 0);
					}
					else
					{
						lTrgNdx += max (lOffset.x, 0);
					}
					lSrcNdx = (lImage->mImageSize.cy - lPixel.y - 1) * lSrcScanBytes;

					for	(lPixel.x = max (-lOffset.x, 0); lPixel.x < min (lImage->mImageSize.cx, mImageSize.cx - max (lOffset.x, 0)); lPixel.x++)
					{
						if	(p32Bit)
						{
							if	(
									(lImage->mBits [lSrcNdx] == lTransparency)
								||	(lPalette [lImage->mBits [lSrcNdx]] == lPalette [lTransparency])
								)
							{
								if	(pBkColor)
								{
									*(COLORREF *)(pImageBits + lTrgNdx) = *pBkColor & 0x00FFFFFF;
								}
							}
							else
							{
								*(COLORREF *)(pImageBits + lTrgNdx) = lPalette [lImage->mBits [lSrcNdx]] | 0xFF000000;
							}
							lSrcNdx++;
							lTrgNdx += 4;
						}
						else
						{
							if	(lImage->mBits [lSrcNdx] != lTransparency)
							{
								pImageBits [lTrgNdx] = lImage->mBits [lSrcNdx];
							}
							lSrcNdx++;
							lTrgNdx++;
						}
					}
				}
			}
		}
		while (lImage.Ptr() != NULL);
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

INT_PTR CAgentFile::GetSoundCount () const
{
	if	(IsAcsFile ())
	{
		return mSoundIndex.GetSize ();
	}
	else
	if	(IsAcfFile ())
	{
		return mAcaSounds.GetSize ();
	}
	return 0;
}

long CAgentFile::GetSoundSize (INT_PTR pSoundNdx)
{
	long	lRet = -1;

	if	(IsAcsFile ())
	{
		if	(
				(pSoundNdx >= 0)
			&&	(pSoundNdx <= (INT_PTR)mSoundIndex.GetUpperBound ())
			)
		{
			lRet = (long) mSoundIndex [pSoundNdx].HighPart;
		}
	}
	else
	if	(IsAcfFile ())
	{
		CAtlByteArray *	lAcaSound;

		if	(lAcaSound = mAcaSounds (pSoundNdx))
		{
			lRet = (long)lAcaSound->GetSize ();
		}
	}
	return lRet;
}

LPCVOID CAgentFile::GetSound (INT_PTR pSoundNdx)
{
	LPCVOID	lSound = NULL;

	if	(IsAcsFile ())
	{
		if	(
				(pSoundNdx >= 0)
			&&	(pSoundNdx <= (INT_PTR)mSoundIndex.GetUpperBound ())
			)
		{
			lSound = ReadAcsSound (mSoundIndex [pSoundNdx].LowPart, mSoundIndex [pSoundNdx].HighPart, (UINT)pSoundNdx+1);
		}
	}
	else
	if	(IsAcfFile ())
	{
		CAtlByteArray *	lAcaSound;

		if	(lAcaSound = mAcaSounds (pSoundNdx))
		{
			lSound = lAcaSound->GetData ();
		}
	}
	return lSound;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CAgentFile::ReadHeader (UINT pLogLevel)
{
	LPCDWORD	lSignature;

	mSignature = 0;

	if	(
			(mFileView.SafeIsValid ())
		&&	(lSignature = (LPCDWORD)(LPVOID)mFileView)
		)
	{
		if	(
				(*lSignature == sAcsFileSignature)
			||	(*lSignature == sAcfFileSignature)
			)
		{
			mSignature = *lSignature;
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%s] Signature [%8.8X]"), mPath, *lSignature);
			}
		}
		else
		{
#ifdef	_LOG_LOAD_ERRS
			if	(LogIsActive ())
			{
				LogMessage (_LOG_LOAD_ERRS, _T("  [%s] Invalid Signature [%8.8X] != [%8.8X]"), mPath, *lSignature, sAcsFileSignature);
			}
#endif
			return AGENTPROVERROR_MAGIC;
		}
	}
	else
	{
#ifdef	_LOG_LOAD_ERRS
		if	(LogIsActive ())
		{
			LogMessage (_LOG_LOAD_ERRS, _T("  [%s] Failed to get signature"), mPath);
		}
#endif
		return AGENTPROVERROR_MAGIC;
	}

#ifdef	_DEBUG_LOAD
	LPCULARGE_INTEGER	lBlockDefs;

	if	(
			(LogIsActive (MinLogLevel (pLogLevel, _DEBUG_LOAD)))
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		)
	{
		LogMessage (pLogLevel, _T("  [%s] Blocks"), mPath);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[0].LowPart, lBlockDefs[0].LowPart, lBlockDefs[0].HighPart, lBlockDefs[0].HighPart);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[1].LowPart, lBlockDefs[1].LowPart, lBlockDefs[1].HighPart, lBlockDefs[1].HighPart);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[2].LowPart, lBlockDefs[2].LowPart, lBlockDefs[2].HighPart, lBlockDefs[2].HighPart);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[3].LowPart, lBlockDefs[3].LowPart, lBlockDefs[3].HighPart, lBlockDefs[3].HighPart);
	}
#endif

	if	(IsAcsFile ())
	{
		if	(!ReadAcsHeader (pLogLevel))
		{
			return AGENTPROVERROR_MAGIC;
		}
	}
	else
	if	(IsAcfFile ())
	{
		if	(!ReadAcfHeader (pLogLevel))
		{
			return AGENTPROVERROR_MAGIC;
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////

bool CAgentFile::ReadAcsHeader (UINT pLogLevel)
{
	bool				lRet = false;
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(
			(IsAcsFile ())
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[0].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
		)
	{
		if	(LogIsActive (pLogLevel))
		{
			LogMessage (pLogLevel, _T("  [%s] Read header of [%u] at [%8.8X (%u)]"), mPath, lBlockLength, lBlockOffset, lBlockOffset);
		}

		try
		{
			LPCBYTE	lByte = lBlock;

			mVersionMinor = *(LPCWORD)lByte;
			lByte += sizeof (WORD);
			mVersionMajor = *(LPCWORD)lByte;
			lByte += sizeof (WORD);
			mFileNamesOffset = (*(LPCDWORD)lByte) - lBlockOffset;
			lByte += sizeof(DWORD);	// Names offset
			lByte += sizeof(DWORD);	// Names size

			mGuid = *(LPCGUID)lByte;
			lByte += sizeof (GUID);
			mImageSize.cx = *(LPCWORD)lByte;
			lByte += sizeof (WORD);
			mImageSize.cy = *(LPCWORD)lByte;
			lByte += sizeof (WORD);
			mTransparency = *lByte;
			lByte++;
			mStyle = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lByte += sizeof(DWORD); // Unknown - always 0x0000002

			if	(mStyle & CharStyleTts)
			{
				lByte = (LPCBYTE)ReadBufferTts (lByte, true, pLogLevel);
			}
			if	(mStyle & CharStyleBalloon)
			{
				lByte = (LPCBYTE)ReadBufferBalloon (lByte, true, pLogLevel);
			}
			lByte = (LPCBYTE)ReadBufferPalette (lByte, pLogLevel);
			lByte = (LPCBYTE)ReadBufferIcon (lByte, pLogLevel);

			mFileStatesOffset = (DWORD)(lByte - lBlock);
			lRet = true;
		}
		catch AnyExceptionDebug

		if	(LogIsActive (pLogLevel))
		{
			Log (pLogLevel);
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CAgentFile::ReadAcfHeader (UINT pLogLevel)
{
	bool				lRet = false;
	DWORD				lUncompressedSize;
	DWORD				lCompressedSize;
	LPCBYTE				lHeaderData = NULL;
	tArrayPtr <BYTE>	lUncompressedData;

	if	(IsAcfFile ())
	{
		FreeNames ();
		FreeGestures ();
		FreeStates ();
	}

	if	(
			(IsAcfFile ())
		&&	(mFileView.SafeIsValid ())
		)
	{
		lUncompressedSize = ((LPCDWORD)(LPVOID)mFileView) [1];
		lCompressedSize = ((LPCDWORD)(LPVOID)mFileView) [2];

		if	(LogIsActive (pLogLevel))
		{
			LogMessage (pLogLevel, _T("  [%s] UncompressedSize [%u] CompressedSize [%u]"), mPath, lUncompressedSize, lCompressedSize);
		}

		if	(lCompressedSize == 0)
		{
			lHeaderData = ((LPCBYTE)(LPVOID)mFileView) + sizeof(DWORD)*3;
		}
		else
		if	(
				(lUncompressedData = new BYTE [lUncompressedSize])
			&&	(DecodeData (((LPCBYTE)(LPVOID)mFileView) + sizeof(DWORD)*3, lCompressedSize, (LPBYTE)lUncompressedData, lUncompressedSize) == lUncompressedSize)
			)
		{
			lHeaderData = lUncompressedData;
		}

		if	(lHeaderData)
		{
			try
			{
				LPCBYTE						lByte = lHeaderData;
				DWORD						lStrLen;
				LPCWSTR						lStr;
				WORD						lGestureCount;
				tPtr <CAgentFileAnimation>	lGesture;

				//LogDump (LogDebug, (LPBYTE)lHeaderData, lUncompressedSize);

				mVersionMinor = *(LPCWORD)lByte;
				lByte += sizeof (WORD);
				mVersionMajor = *(LPCWORD)lByte;
				lByte += sizeof (WORD);
				lGestureCount = *(LPWORD)lByte;
				lByte += sizeof (WORD);

				while (lGestureCount > 0)
				{
					lGesture = new tS <CAgentFileAnimation>;

					lStrLen = *(LPCDWORD)lByte;
					lByte += sizeof(DWORD);
					if	(lStrLen)
					{
						lStr = (LPCWSTR) lByte;
						lByte += lStrLen * sizeof (WCHAR);
						lGesture->mName = (CAtlString (lStr, lStrLen)).AllocSysString ();
					}

					lStrLen = *(LPCDWORD)lByte;
					lByte += sizeof(DWORD);
					if	(lStrLen)
					{
						lStr = (LPCWSTR) lByte;
						lByte += lStrLen * sizeof (WCHAR);
						lGesture->mAcaFileName = (CAtlString (lStr, lStrLen)).AllocSysString ();
					}

					lStrLen = *(LPCDWORD)lByte;
					lByte += sizeof(DWORD);
					if	(lStrLen)
					{
						lStr = (LPCWSTR) lByte;
						lByte += lStrLen * sizeof (WCHAR);
						lGesture->mReturnName = (CAtlString (lStr, lStrLen)).AllocSysString ();
					}

					lGesture->mAcaChksum = *(LPDWORD)lByte;
					lByte += sizeof(DWORD);

					mGestures.mAnimations.InsertAt (AddSortedString (mGestures.mNames, CAtlString ((BSTR)lGesture->mName)), lGesture);
					lGesture.Detach ();
					lGestureCount--;
				}

				mGuid = *(LPCGUID)lByte;
				lByte += sizeof (GUID);
				lByte = (LPCBYTE)ReadBufferNames (lByte, false, true, pLogLevel);

				mImageSize.cx = *(LPCWORD)lByte;
				lByte += sizeof (WORD);
				mImageSize.cy = *(LPCWORD)lByte;
				lByte += sizeof (WORD);
				mTransparency = *lByte;
				lByte++;
				mStyle = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lByte += sizeof(DWORD); // Unknown - always 0x0000002

				if	(mStyle & CharStyleTts)
				{
					lByte = (LPCBYTE)ReadBufferTts (lByte, false, pLogLevel);
				}
				if	(mStyle & CharStyleBalloon)
				{
					lByte = (LPCBYTE)ReadBufferBalloon (lByte, false, pLogLevel);
				}
				lByte = (LPCBYTE)ReadBufferPalette (lByte, pLogLevel);
				lByte = (LPCBYTE)ReadBufferIcon (lByte, pLogLevel);
				lByte = (LPCBYTE)ReadBufferStates (lByte, false, lHeaderData+lUncompressedSize, pLogLevel);

				lRet = true;
			}
			catch AnyExceptionDebug
		}

		if	(LogIsActive (pLogLevel))
		{
			Log (pLogLevel);
			LogNames (pLogLevel);
			LogStates (pLogLevel);
			LogGestures (pLogLevel);
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

LPCVOID CAgentFile::ReadBufferTts (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel)
{
	LPCBYTE		lByte = (LPCBYTE)pBuffer;
	const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;
	DWORD		lStrLen;
	LPCWSTR		lStr;

	mTts.mEngine = *(LPCGUID)lByte;
	lByte += sizeof (GUID);
	mTts.mMode = *(LPCGUID)lByte;
	lByte += sizeof (GUID);

	mTts.mSpeed = * (long *) lByte;
	lByte += sizeof (long);
	mTts.mPitch = * (short *) lByte;
	lByte += sizeof (short);

	if	(*lByte)
	{
		lByte += sizeof (BYTE);	// TRUE
		mTts.mLanguage = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		lStrLen = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lStr = (LPCWSTR) lByte;

		if	(
				(lStrLen > 2048)
			||	(IsBadStringPtr (lStr, 2048))
			)
		{
			AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
		}
#ifdef	_DEBUG_NOT
		CAtlString	lUnknownStr (lStr, lStrLen);
		if	(!lUnknownStr.IsEmpty ())
		{
			LogMessage (LogDebug, _T("Unknown TTS string [%s]"), lUnknownStr);
		}
#endif
		if	(lStrLen > 0)
		{
			lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
		}

		mTts.mGender = *(LPCWORD)lByte;
		lByte += sizeof (WORD);
		mTts.mAge = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		lStrLen = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lStr = (LPCWSTR) lByte;
		if	(
				(lStrLen > MAX_PATH)
			||	(IsBadStringPtr (lStr, MAX_PATH))
			)
		{
			AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
		}
		mTts.mStyle = CAtlString (lStr, lStrLen).AllocSysString();
		if	(lStrLen > 0)
		{
			lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
		}
	}
	else
	{
		lByte += sizeof (BYTE);	// FALSE
	}

	if	(IsEqualGUID (mTts.mMode, GUID_NULL))
	{
		mTts.mModeId = NULL;
	}
	else
	{
		mTts.mModeId = ((CString)CGuidStr (mTts.mMode)).AllocSysString();
	}
	return lByte;
}

LPCVOID CAgentFile::ReadBufferBalloon (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel)
{
	LPCBYTE		lByte = (LPCBYTE)pBuffer;
	const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;
	DWORD		lStrLen;
	LPCWSTR		lStr;

	mBalloon.mLines = *lByte;
	lByte++;
	mBalloon.mPerLine = *lByte;
	lByte++;
	mBalloon.mFgColor = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	mBalloon.mBkColor = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	mBalloon.mBrColor = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);

	lStrLen = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	lStr = (LPCWSTR) lByte;
	if	(
			(lStrLen > MAX_PATH)
		||	(IsBadStringPtr (lStr, MAX_PATH))
		)
	{
		AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
	}
	mBalloon.mFontName = CAtlString (lStr, lStrLen).AllocSysString();
	if	(lStrLen > 0)
	{
		lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
	}
	mBalloon.mFontHeight = * (long *) lByte;
	lByte += sizeof (long);
	mBalloon.mFontWeight = *(LPCWORD)lByte;
	lByte += sizeof (WORD);
	mBalloon.mFontStrikethru = *lByte; // Unsure, and where is underline?
	lByte += sizeof (WORD);
	mBalloon.mFontItalic = *lByte;
	lByte += sizeof (WORD);
	mBalloon.mFontCharset = DEFAULT_CHARSET;

	return lByte;
}

LPCVOID CAgentFile::ReadBufferPalette (LPCVOID pBuffer, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;
	DWORD	lPaletteSize;

	lPaletteSize = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	if	(lPaletteSize > 0)
	{
		if	(mPalette = new COLORREF [256])
		{
			memcpy ((LPCOLORREF)mPalette, lByte, min (lPaletteSize, 256) * sizeof(COLORREF));
		}
		lByte += lPaletteSize * sizeof(COLORREF);
	}

#ifdef	_SAVE_PALETTE
	if	(
			(mPalette.Ptr())
		&&	(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_PALETTE)))
		)
	{
		DumpPalette ((LPCOLORREF)mPalette);
	}
#else
#ifdef	_DUMP_PALETTE
	if	(
			(mPalette.Ptr())
		&&	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_PALETTE)))
		)
	{
		DumpPalette ((LPCOLORREF)mPalette);
	}
#endif
#endif
	return lByte;
}

LPCVOID CAgentFile::ReadBufferIcon (LPCVOID pBuffer, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	if	(*(lByte++))
	{
		DWORD			lIconSize = *(LPCDWORD)lByte;
		LPBITMAPINFO	lMaskBitmapInfo;
		LPBITMAPINFO	lColorBitmapInfo;
		LPCBYTE			lMaskBytes;
		LPCBYTE			lColorBytes;
		tS <ICONINFO>	lIconInfo;
		CMemDCHandle	lDC;

		lByte += sizeof(DWORD);
		lMaskBitmapInfo = (LPBITMAPINFO)lByte;
		lMaskBytes = lByte + lMaskBitmapInfo->bmiHeader.biSize;
		lMaskBytes += 2 * sizeof(DWORD);
		lByte += lIconSize;

		lIconSize = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lColorBitmapInfo = (LPBITMAPINFO)lByte;
		lColorBytes = lByte + lColorBitmapInfo->bmiHeader.biSize;
		lColorBytes += 16 * sizeof(DWORD);
		lByte += lIconSize;

#ifdef	_DUMP_ICON
		if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_ICON)))
		{
			CImageDebugger::DumpBitmap (_DUMP_ICON, *lMaskBitmapInfo, (LPBYTE)lMaskBytes, _T("Icon Mask"));
			CImageDebugger::DumpBitmap (_DUMP_ICON, *lColorBitmapInfo, (LPBYTE)lColorBytes, _T("Icon Color"));
		}
#endif

#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::CreateIcon [%p]"), this, mIcon);
		}
#endif
		if	(lDC.Attach (CreateDC (_T("DISPLAY"), NULL, NULL, NULL)))
		{
			lIconInfo.hbmMask = CreateDIBitmap (lDC, &lMaskBitmapInfo->bmiHeader, CBM_INIT, lMaskBytes, lMaskBitmapInfo, DIB_RGB_COLORS);
			lIconInfo.hbmColor = CreateDIBitmap (lDC, &lColorBitmapInfo->bmiHeader, CBM_INIT, lColorBytes, lColorBitmapInfo, DIB_RGB_COLORS);

			lIconInfo.fIcon = TRUE;
			if	(mIcon)
			{
				DestroyIcon (mIcon);
			}
			mIcon = CreateIconIndirect (&lIconInfo);
			lDC.Close ();
		}
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::CreateIcon [%p] Done"), this, mIcon);
		}
#endif
		if	(lIconInfo.hbmMask)
		{
			DeleteObject (lIconInfo.hbmMask);
		}
		if	(lIconInfo.hbmColor)
		{
			DeleteObject (lIconInfo.hbmColor);
		}
	}

#ifdef	_SAVE_ICON
	if	(
			(mIcon)
		&&	(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_ICON)))
		)
	{
		CAtlString	lDumpName (mPath);

		PathStripPath (lDumpName.GetBuffer (lDumpName.GetLength ()));
		PathRemoveExtension (lDumpName.GetBuffer (lDumpName.GetLength ()));
		lDumpName.ReleaseBuffer ();
		CImageDebugger::SaveIcon (mIcon, lDumpName);
	}
#endif
	return lByte;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::FreeNames ()
{
	mNames.DeleteAll ();
}

bool CAgentFile::ReadNames (bool pFirstLetterCaps, UINT pLogLevel)
{
	bool				lRet = false;
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadNames [%s]"), mPath);
	}
	if	(IsAcsFile ())
	{
		FreeNames ();
	}

	if	(
			(IsAcsFile ())
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[0].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
		)
	{
		try
		{
			DWORD	lNamesOffset = ((LPCDWORD)lBlock) [1] - lBlockOffset;
			LPCBYTE	lByte = lBlock+lNamesOffset;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Names [%u] at [%8.8X]"), *(LPCWORD)lByte, lBlockLength-lNamesOffset, lByte-lBlock);
			}

			lByte = (LPCBYTE)ReadBufferNames (lByte, true, pFirstLetterCaps, pLogLevel);

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD, _T("    Left [%u]"), (long) lBlockLength - (long) (lByte-lBlock));
			}
#endif
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] Names [%s]"), mNames.GetSize (), mPath);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

LPCVOID CAgentFile::ReadBufferNames (LPCVOID pBuffer, bool pNullTerminated, bool pFirstLetterCaps, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD					lNameCount;
		const DWORD				lStrPad = (pNullTerminated) ? 1 : 0;
		DWORD					lStrLen;
		LPCWSTR					lStr;
		tPtr <CAgentFileName>	lName;

		lNameCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		while (lNameCount > 0)
		{
			lName = new CAgentFileName;

			lName->mLanguage = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
				lName->mName = CAtlString (lStr, lStrLen).AllocSysString();
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD|LogHighVolume, _T("    StrLen [%u] Str [%ls]"), lStrLen, (lStrLen) ? lStr : NULL);
			}
#endif

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
				lName->mDesc1 = CAtlString (lStr, lStrLen).AllocSysString();
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD|LogHighVolume, _T("    StrLen [%u] Str [%ls]"), lStrLen, (lStrLen) ? lStr : NULL);
			}
#endif

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
				lName->mDesc2 = CAtlString (lStr, lStrLen).AllocSysString();
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD|LogHighVolume, _T("    StrLen [%u] Str [%ls]"), lStrLen, (lStrLen) ? lStr : NULL);
			}
#endif

			if	(pFirstLetterCaps)
			{
				CAtlString	lCharName ((BSTR) lName->mName);

				if	(
						(!lCharName.IsEmpty ())
					&&	(_istlower (lCharName [0]))
					)
				{
					lCharName.SetAt (0, _totupper (lCharName [0]));
					lName->mName = lCharName.AllocSysString();
				}
			}

			if	(LogIsActive(pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  NameCount [%hu] Language [%4.4hX] Name [%ls]"), lNameCount, lName->mLanguage, (BSTR) lName->mName);
			}

			lNameCount--;
			mNames.Add (lName.Detach ());
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

//////////////////////////////////////////////////////////////////////

CAgentFileName * CAgentFile::FindName (WORD pLangID)
{
	INT_PTR				lNdx;
	CAgentFileName *	lName = NULL;

	if	(mFileView.SafeIsValid ())
	{
		if	(mNames.GetSize() <= 0)
		{
			ReadNames ();
		}

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

		for	(lNdx = 0; lName = mNames (lNdx); lNdx++)
		{
			if	(lName->mLanguage == pLangID)
			{
				break;
			}
		}

		if	(!lName)
		{
			for	(lNdx = 0; lName = mNames (lNdx); lNdx++)
			{
				if	(
						(PRIMARYLANGID (lName->mLanguage) == PRIMARYLANGID (pLangID))
					&&	(
							(SUBLANGID (lName->mLanguage) == SUBLANG_DEFAULT)
						||	(SUBLANGID (lName->mLanguage) == SUBLANG_NEUTRAL)
						)
					)
				{
					break;
				}
			}
		}

		if	(!lName)
		{
			for	(lNdx = 0; lName = mNames (lNdx); lNdx++)
			{
				if	(PRIMARYLANGID (lName->mLanguage) == PRIMARYLANGID (pLangID))
				{
					break;
				}
			}
		}

		if	(!lName)
		{
			lName = FindName (MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT));
		}

		if	(!lName)
		{
			lName =  mNames (0);
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

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::FreeStates ()
{
	mStates.mGestures.RemoveAll ();
	mStates.mNames.RemoveAll ();
}

bool CAgentFile::ReadStates (UINT pLogLevel)
{
	bool				lRet = false;
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadStates [%s]"), mPath);
	}
	if	(IsAcsFile ())
	{
		FreeStates ();
	}

	if	(
			(IsAcsFile ())
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[0].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
		)
	{
		try
		{
			LPCBYTE	lByte = lBlock;
			LPCBYTE	lNamesStart = lBlock + mFileNamesOffset;

			lByte += mFileStatesOffset;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] States [%u] at [%8.8X]"), *(LPCWORD)lByte, lNamesStart-lByte, lByte-lBlock);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lByte, lNamesStart-lByte, _T("    "), true);
			}
#endif

			lByte = (LPCBYTE)ReadBufferStates (lByte, true, lNamesStart, pLogLevel);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] States [%s]"), mStates.mGestures.GetCount (), mPath);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

LPCVOID CAgentFile::ReadBufferStates (LPCVOID pBuffer, bool pNullTerminated, LPCVOID pBufferEnd, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD		lStateCount;
		const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;
		DWORD		lStrLen;
		LPCWSTR		lStr;

		lStateCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		while	(
					(lStateCount > 0)
				&&	(
						(pBufferEnd == NULL)
					||	(lByte < pBufferEnd)
					)
				)
		{
			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lStr = (LPCWSTR) lByte;
			lByte += (lStrLen + lStrPad) * sizeof (WCHAR);

			CAtlString		lState (lStr, lStrLen);
			INT_PTR			lGestureNdx = FindSortedString (mStates.mNames, lState);

			if	(lGestureNdx < 0)
			{
				mStates.mGestures.InsertAt (lGestureNdx = AddSortedString (mStates.mNames, lState), CAtlStringArray());
			}

			CAtlStringArray &	lGestures  = mStates.mGestures [lGestureNdx];
			WORD				lGestureCount;

			lGestureCount = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  State [%s] Gestures [%u]"), lState, lGestureCount);
			}

			while	(
						(lGestureCount > 0)
					&&	(
							(pBufferEnd == NULL)
						||	(lByte < pBufferEnd)
						)
					)
			{
				lStrLen = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);

				CAtlString	lGesture (lStr, lStrLen);
				lGestures.Add (lGesture);

				if	(LogIsActive (pLogLevel))
				{
					LogMessage (pLogLevel|LogHighVolume, _T("    Gesture [%s]"), lGesture);
				}
				lGestureCount--;
			}
			lStateCount--;
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

INT_PTR CAgentFile::FindState (LPCTSTR pStateName)
{
	INT_PTR		lRet = -1;
	CAtlString	lStateName (pStateName);

	if	(
			(!lStateName.IsEmpty ())
		&&	(mFileView.SafeIsValid ())
		)
	{
		if	(mStates.mGestures.GetSize() <= 0)
		{
			ReadStates ();
		}

		lRet = FindSortedString (mStates.mNames, lStateName);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::FreeGestures ()
{
	mGestures.mAnimations.DeleteAll ();
	mGestures.mNames.RemoveAll ();
}

bool CAgentFile::ReadGestures (UINT pLogLevel)
{
	bool				lRet = false;
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadGestures [%s]"), mPath);
	}
	if	(IsAcsFile ())
	{
		FreeGestures ();
	}

	if	(
			(IsAcsFile ())
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[1].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[1].LowPart))
		)
	{
		try
		{
			LPCBYTE		lByte = lBlock;
			DWORD		lCount = *(LPCDWORD)lByte;
			DWORD		lStrLen;
			LPCWSTR		lStr;
			CAtlString	lGesture;
			LPCDWORD	lAnimation;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Gestures [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

			while	(lCount > 0)
			{
				lStrLen = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + 1) * sizeof (WCHAR);
				lGesture = CAtlString (lStr, lStrLen);

				lAnimation = (LPCDWORD)lByte;
				lByte += sizeof(DWORD)*2;

#ifdef	_DEBUG_LOAD
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
				{
					LogMessage (_DEBUG_LOAD, _T("  Gesture [%s] at [%8.8X (%u)] of [%8.8X (%u)]"), lGesture, lAnimation[0], lAnimation[0], lAnimation[1], lAnimation[1]);
				}
#else
				if	(LogIsActive (pLogLevel))
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  Gesture [%s]"), lGesture);
				}
#endif
				mGestures.mAnimations.InsertAt (AddSortedString (mGestures.mNames, lGesture), ReadAcsAnimation (lAnimation[0], lAnimation[1], pLogLevel));
				lCount--;
			}

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] Gestures [%s]"), mGestures.mAnimations.GetSize (), mPath);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

CAgentFileAnimation * CAgentFile::ReadAcsAnimation (DWORD pOffset, DWORD pSize, UINT pLogLevel)
{
	tPtr <CAgentFileAnimation>	lRet;

	try
	{
		LPCBYTE	lBlock;

		if	(
				(IsAcsFile ())
			&&	(mFileView.SafeIsValid ())
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
			LPCBYTE		lByte = lBlock;
			DWORD		lStrLen;
			LPCWSTR		lStr;
			CAtlString	lName;
			CAtlString	lReturnName;
			BYTE		lReturnType;
			WORD		lFrameCount = 0;
			WORD		lFrameNdx;

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lStr = (LPCWSTR) lByte;
			lByte += (lStrLen + 1) * sizeof (WCHAR);
			lName = CAtlString (lStr, lStrLen);

			lReturnType = *lByte;
			lByte++;
			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + 1) * sizeof (WCHAR);
				lReturnName = CAtlString (lStr, lStrLen);
			}
			lFrameCount = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

#ifdef	_DEBUG_ANIMATION
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
			{
				CAtlString	lReturnString = (lReturnType == 1) ? _T("<exit branching>") : (lReturnType == 2) ? _T("<none>") : lReturnName;
				LogMessage (_DEBUG_ANIMATION, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), lName, lReturnType, lReturnString, lFrameCount);
			}
#else
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), lName, lReturnType, lReturnName, lFrameCount);
			}
#endif
#ifdef	_DUMP_ANIMATION
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_ANIMATION)))
			{
				LogDump (_DUMP_ANIMATION, (LPBYTE)lBlock, pSize, _T("        "));
			}
#endif
			if	(
					(lFrameCount > 0)
				&&	(lRet = new tS <CAgentFileAnimation>)
				)
			{
				lRet->mName = lName.AllocSysString();
				lRet->mReturnType = lReturnType;
				if	(!lReturnName.IsEmpty())
				{
					lRet->mReturnName = lReturnName.AllocSysString();
				}

				if	(
						(lFrameCount > 0)
					&&	(lRet->mFrames = new tS <CAgentFileFrame> [lFrameCount])
					)
				{
					lRet->mFrameCount = lFrameCount;

					for	(lFrameNdx = 0; lFrameNdx < lFrameCount; lFrameNdx++)
					{
						CAgentFileFrame &	lFrame = lRet->mFrames [(int)lFrameNdx];
						LPCBYTE				lFrameStart = lByte;
						WORD				lImageCount = *(LPCWORD)lByte;
						BYTE				lBranchCount;
						BYTE				lOverlayCount;
						int					lNdx;

						lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("      Frame [%hu] Images [%hu]"), lFrameNdx, lImageCount);
						}
#endif
						if	(
								(lImageCount > 0)
							&&	(lFrame.mImages = new tS <CAgentFileFrameImage> [lImageCount])
							)
						{
							lFrame.mImageCount = lImageCount;

							for	(lNdx = 0; lNdx < (int)lImageCount; lNdx++)
							{
								CAgentFileFrameImage &	lFrameImage = lFrame.mImages [lNdx];

								lFrameImage.mImageNdx = *(LPCDWORD)lByte;
								lByte += sizeof(DWORD);
								lFrameImage.mOffset.x = (long)(short)*(LPCWORD)lByte;
								lByte += sizeof(WORD);
								lFrameImage.mOffset.y = (long)(short)*(LPCWORD)lByte;
								lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
								if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
								{
									LogMessage (_DEBUG_ANIMATION, _T("        Image [%4u] at [%d %d]"), lFrameImage.mImageNdx, lFrameImage.mOffset.x, lFrameImage.mOffset.y);
								}
#endif
							}
						}

						lFrame.mSoundNdx = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lFrame.mDuration = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lFrame.mExitFrame = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("        Duration [%hu] ExitFrame [%hd] Sound [%2hd]"), lFrame.mDuration, lFrame.mExitFrame, lFrame.mSoundNdx);
						}
#endif
						lBranchCount = *lByte;
						lByte++;
						if	(lBranchCount > 0)
						{
							lFrame.mBranching [0] = ((LPCDWORD)lByte) [0];
						}
						if	(lBranchCount > 1)
						{
							lFrame.mBranching [1] = ((LPCDWORD)lByte) [1];
						}
						if	(lBranchCount > 2)
						{
							lFrame.mBranching [2] = ((LPCDWORD)lByte) [2];
						}
						lByte += sizeof(WORD)*lBranchCount*2;

#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							for	(lNdx = 0; lNdx < min ((int)lBranchCount, 3); lNdx++)
							{
								LogMessage (_DEBUG_ANIMATION, _T("        Branch [%u] [%u]"), LOWORD(lFrame.mBranching [lNdx]), HIWORD(lFrame.mBranching [lNdx]));
							}
						}
#endif

						lOverlayCount = *lByte;
						lByte++;
						if	(
								(lOverlayCount > 0)
							&&	(lFrame.mOverlays = new tS <CAgentFileFrameOverlay> [lOverlayCount])
							)
						{
							lFrame.mOverlayCount = lOverlayCount;

							for	(lNdx = 0; lNdx < (int)lOverlayCount; lNdx++)
							{
								CAgentFileFrameOverlay &	lOverlay = lFrame.mOverlays [lNdx];
								BYTE						lRgnFlag;

								lOverlay.mOverlayNdx = *lByte;
								lByte += sizeof(BYTE);
								lOverlay.mReplaceFlag = *lByte;
								lByte += sizeof(BYTE);
								lOverlay.mImageNdx = *(LPCWORD)lByte;
								lByte += sizeof(WORD);
								lByte += sizeof(BYTE); // Unknown
								lRgnFlag = *lByte;
								lByte += sizeof(BYTE);
								lOverlay.mOffset.x = (long)(short)*(LPCWORD)lByte;
								lByte += sizeof(WORD);
								lOverlay.mOffset.y = (long)(short)*(LPCWORD)lByte;
								lByte += sizeof(WORD);
								lOverlay.mSomething.x = (long)(short)*(LPCWORD)lByte;
								lByte += sizeof(WORD);
								lOverlay.mSomething.y = (long)(short)*(LPCWORD)lByte;
								lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
								if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
								{
									LogMessage (_DEBUG_ANIMATION, _T("        Overlay [%hu] Replace [%u] Image [%4u] at [%d %d] something [%d] [%d]"), lOverlay.mOverlayNdx, lOverlay.mReplaceFlag, lOverlay.mImageNdx, lOverlay.mOffset.x, lOverlay.mOffset.y, lOverlay.mSomething.x, lOverlay.mSomething.y);
								}
#endif
							}
						}

#ifdef	_DUMP_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_ANIMATION)))
						{
							LogDump (_DUMP_ANIMATION, (LPBYTE)lFrameStart, min(lByte-lFrameStart, pSize-(lFrameStart-lBlock)), _T("          "));
						}
#endif
						if	(lByte > lBlock+pSize)
						{
#ifdef	_DEBUG
							LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
							break;
						}
					}
#ifdef	_DEBUG
					if	(lByte < lBlock+pSize)
					{
						LogMessage (LogDebug, _T("!!! Extra !!! [%u]"), pSize-(lByte-lBlock));
					}
#endif
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet.Detach ();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CAtlString CAgentFile::GetAcaPath (CAgentFileAnimation * pAnimation)
{
	if	(pAnimation)
	{
		return CAtlString (ParseRelativePath (CAtlString(pAnimation->mAcaFileName)));
	}
	return CAtlString();
}

HRESULT CAgentFile::ReadAcaFile (CAgentFileAnimation * pAnimation, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_FALSE;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif

	if	(
			(pAnimation)
		&&	(pAnimation->mAcaChksum != (DWORD)-1)
		&&	(pAnimation->mAcaFileName.Ptr ())
		)
	{
		try
		{
			CAtlString	lPath = GetAcaPath (pAnimation);

			if	(PathIsURL (mPath))
			{
				tPtr <CFileDownload>	lOldDownload = mFileDownload.Detach();

				mFileDownload = CFileDownload::CreateInstance (lPath);
				if	(lOldDownload)
				{
					mFileDownload->SetBindFlags (lOldDownload->GetBindFlags());
				}
				SafeFreeSafePtr (lOldDownload);

				if	(SUCCEEDED (lResult = mFileDownload->Download ()))
				{
					lResult = ReadAcaFile (pAnimation, CAtlString ((BSTR) mFileDownload->GetCacheName()), p32Bit, pLogLevel);
				}
			}
			else
			{
				lResult = ReadAcaFile (pAnimation, lPath, p32Bit, pLogLevel);
			}
		}
		catch AnyExceptionDebug
	}

	if	(pAnimation)
	{
		pAnimation->mAcaChksum = (DWORD)-1;
	}
	return lResult;
}

HRESULT CAgentFile::ReadAcaFile (CAgentFileAnimation * pAnimation, LPCTSTR pPath, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_FALSE;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif

	try
	{
		CFileHandle	lFileHandle;
		CGenericHandle	lFileMapping;
		CMappedHandle	lFileView;
		DWORD			lFileSize;

		lFileHandle = CreateFile (pPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if	(
				(lFileHandle.SafeIsValid ())
			&&	(lFileSize = GetFileSize (lFileHandle, NULL))
			&&	((lFileMapping = CreateFileMapping (lFileHandle, NULL, PAGE_READONLY, 0, 0, NULL)).SafeIsValid())
			&&	((lFileView = MapViewOfFile (lFileMapping, FILE_MAP_READ, 0, 0, lFileSize)).SafeIsValid())
			)
		{
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T(""));
				LogMessage (pLogLevel, _T("Opened [%ls] [%ls] [%s] [%u]"), (BSTR)pAnimation->mName, (BSTR)pAnimation->mAcaFileName, pPath, lFileSize);
			}

			try
			{
				LPCBYTE				lByte = (LPCBYTE)(LPVOID)lFileView;
				DWORD				lVersion;
				DWORD				lChksum;
				DWORD				lUncompressedSize;
				DWORD				lCompressedSize;
				LPCBYTE				lAnimationData = NULL;
				tArrayPtr <BYTE>	lUncompressedData;

				lVersion = MAKELONG (((LPCWORD)lByte) [0], ((LPCWORD)lByte) [1]);
				lByte += sizeof(WORD) * 2;
				lChksum = *(LPDWORD)lByte;
				lByte += sizeof(DWORD);

				if	(
						(HIWORD (lVersion) >= 2)
					&&	(lChksum == pAnimation->mAcaChksum)
					)
				{
					if	(*lByte)
					{
						lByte++;
						lUncompressedSize = *(LPDWORD)lByte;
						lByte += sizeof(DWORD);
						lCompressedSize = *(LPDWORD)lByte;
						lByte += sizeof(DWORD);

						if	(
								(lCompressedSize <= lFileSize - (lByte - (LPCBYTE)(LPVOID)lFileView))
							&&	(lUncompressedData = new BYTE [lUncompressedSize])
							&&	(DecodeData (lByte, lCompressedSize, (LPBYTE)lUncompressedData, lUncompressedSize) == lUncompressedSize)
							)
						{
							lAnimationData = lUncompressedData;
						}
					}
					else
					{
						lByte++;
						lAnimationData = lByte;
						lUncompressedSize = lFileSize - (DWORD)(lByte - (LPCBYTE)(LPVOID)lFileView);
					}
				}

				if	(lAnimationData)
				{
					WORD	lImageStart = (WORD)mAcaImages.GetSize ();

					lByte = lAnimationData;
					lResult = ReadAcaSounds (pAnimation, (LPCVOID&)lByte, lUncompressedSize, p32Bit, pLogLevel);
					if	(SUCCEEDED (lResult))
					{
						lResult = ReadAcaImages (pAnimation, (LPCVOID&)lByte, lUncompressedSize, p32Bit, pLogLevel);
					}
					if	(SUCCEEDED (lResult))
					{
#ifdef	_DEBUG_LOAD
						LogMessage (_DEBUG_LOAD, _T("Unknown [%2.2X]"), *lByte);
#endif
						lByte += sizeof(BYTE); // Unknown
						lUncompressedSize -= sizeof(BYTE);
						lResult = ReadAcaFrames (pAnimation, (LPCVOID&)lByte, lUncompressedSize, lImageStart, p32Bit, pLogLevel);
					}

#ifdef	_DEBUG_LOAD
					LogMessage (_DEBUG_LOAD, _T("Leftover [%u]"), lUncompressedSize );
					LogDump (_DEBUG_LOAD, (LPBYTE)lByte, lUncompressedSize);
#endif
				}
			}
			catch AnyExceptionDebug
		}
		else
		{
			lResult = HRESULT_FROM_WIN32 (GetLastError());
			if	(SUCCEEDED (lResult))
			{
				lResult = E_FAIL;
			}
			if	(LogIsActive ())
			{
				LogWinErr (LogNormal, GetLastError(), _T("Open [%s]"), pPath);
			}
		}

		lFileView.Close ();
		lFileMapping.Close ();
		lFileHandle.Close ();
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CAgentFile::ReadAcaFrames (CAgentFileAnimation * pAnimation, LPCVOID & pBuffer, DWORD & pBufferSize, WORD pImageStart, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_OK;

	try
	{
		LPCBYTE	lByte = (LPCBYTE)pBuffer;
		WORD	lFrameCount;
		WORD	lFrameNum;

		//LogDump (LogDebugFast, (LPBYTE)pBuffer, pBufferSize);

		lFrameCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
		if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
		{
			LogMessage (_DEBUG_ANIMATION, _T("[%ls] Aca Frames [%hu]"), (BSTR)pAnimation->mName, lFrameCount);
		}
#endif

		if	(pAnimation->mFrames = new tS <CAgentFileFrame> [lFrameCount])
		{
			pAnimation->mFrameCount = lFrameCount;
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}

		if	(SUCCEEDED (lResult))
		{
			for	(lFrameNum = 0; lFrameNum < lFrameCount; lFrameNum++)
			{
				CAgentFileFrame &	lFrame = pAnimation->mFrames [(short)(long)lFrameNum];
				BYTE				lBranchCount;
				BYTE				lOverlayCount;
				BYTE				lOverlayReplace;
				int					lNdx;

				//LogDump (LogDebugFast, (LPBYTE)lByte, sizeof(WORD)*7);

				lFrame.mImageCount = 1;
				lFrame.mImages = new tS <CAgentFileFrameImage> [1];
				lFrame.mImages [0].mImageNdx = *(LPCWORD)lByte + pImageStart;
				lByte += sizeof(WORD);

				lFrame.mSoundNdx = *(LPCWORD)lByte;
				lByte += sizeof(WORD);
				lFrame.mDuration = *(LPCWORD)lByte;
				lByte += sizeof(WORD);
				DWORD lSomething = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lFrame.mExitFrame = *(LPCWORD)lByte;
				lByte += sizeof(WORD);

				if	((short)lFrame.mExitFrame >= 0)
				{
					lFrame.mExitFrame++; // 0-based to 1-based
				}

#ifdef	_DEBUG_ANIMATION
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
				{
					LogMessage (_DEBUG_ANIMATION, _T("  Frame [%u] Image [%hu] Duration [%hu] ExitFrame [%hd] Sound [%2hd] Something [%8.8X]"), lFrameNum, lFrame.mImages [0].mImageNdx, lFrame.mDuration, lFrame.mExitFrame, lFrame.mSoundNdx, lSomething);
				}
#endif
				lBranchCount = *lByte;
				lByte++;
				if	(lBranchCount > 0)
				{
					lFrame.mBranching [0] = ((LPCDWORD)lByte) [0];
					lFrame.mBranching [0] = MAKELONG (LOWORD (lFrame.mBranching [0]+1), HIWORD (lFrame.mBranching [0])); // 0-based to 1-based
				}
				if	(lBranchCount > 1)
				{
					lFrame.mBranching [1] = ((LPCDWORD)lByte) [1];
					lFrame.mBranching [1] = MAKELONG (LOWORD (lFrame.mBranching [1]+1), HIWORD (lFrame.mBranching [1])); // 0-based to 1-based
				}
				if	(lBranchCount > 2)
				{
					lFrame.mBranching [2] = ((LPCDWORD)lByte) [2];
					lFrame.mBranching [2] = MAKELONG (LOWORD (lFrame.mBranching [2]+1), HIWORD (lFrame.mBranching [2])); // 0-based to 1-based
				}
				lByte += sizeof(WORD)*lBranchCount*2;

#ifdef	_DEBUG_ANIMATION
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
				{
					for	(lNdx = 0; lNdx < min ((int)lBranchCount, 3); lNdx++)
					{
						LogMessage (_DEBUG_ANIMATION, _T("    Branch [%u] [%u]"), LOWORD(lFrame.mBranching [lNdx]), HIWORD(lFrame.mBranching [lNdx]));
					}
				}
#endif
				lOverlayCount = *lByte;
				lByte++;
				if	(lOverlayCount > 0)
				{
					if	(lFrame.mOverlays = new tS <CAgentFileFrameOverlay> [lOverlayCount])
					{
						lFrame.mOverlayCount = lOverlayCount;
					}
					else
					{
						lResult = E_OUTOFMEMORY;
					}
				}

				if	(
						(lOverlayCount > 0)
					&&	(SUCCEEDED (lResult))
					)
				{
					lOverlayReplace = *lByte;
					lByte += sizeof(BYTE);
#ifdef	_DEBUG_ANIMATION
					if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
					{
						LogMessage (_DEBUG_ANIMATION, _T("  Overlays [%hu] Replace [%2.2X]"), lOverlayCount, lOverlayReplace);
					}
#endif
					if	(lOverlayReplace)
					{
						LogMessage (LogDebug, _T("  ??? [%u]"), *(LPCDWORD)lByte);
						lByte += sizeof (DWORD);
					}

					for	(lNdx = 0; lNdx < (int)lOverlayCount; lNdx++)
					{
						CAgentFileFrameOverlay &	lOverlay = lFrame.mOverlays [lNdx];
						tPtr <CAgentFileImage>		lOverlayImage = new tS <CAgentFileImage>;

						//LogDump (LogDebugFast, (LPBYTE)lByte, 15);

						lOverlay.mOverlayNdx = *lByte;
						lByte += sizeof(BYTE);
						lOverlayImage->mBitsSize = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						lByte += sizeof(BYTE); // Unknown
						lOverlay.mReplaceFlag = *lByte;
						lByte += sizeof(BYTE);
						lOverlay.mOffset.x = (long)(short)*(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lOverlay.mOffset.y = (long)(short)*(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lOverlayImage->mImageSize.cx = (long)(short)*(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lOverlayImage->mImageSize.cy = (long)(short)*(LPCWORD)lByte;
						lByte += sizeof(WORD);

						if	(lOverlayImage->mBits = new BYTE [lOverlayImage->mBitsSize])
						{
							memcpy (lOverlayImage->mBits.Ptr(), lByte, lOverlayImage->mBitsSize);
							lByte += lOverlayImage->mBitsSize;

							mAcaImages.Add (lOverlayImage.Detach ());
							lOverlay.mImageNdx = (DWORD)mAcaImages.GetUpperBound ();
						}
						else
						{
							lResult = E_OUTOFMEMORY;
							break;
						}

						if	(lOverlay.mReplaceFlag)
						{
							DWORD lRgnSize = *(LPCDWORD)lByte;
							lByte += sizeof(DWORD);
#ifdef	_DEBUG_LOAD_NOT
							LogMessage (_DEBUG_LOAD, _T("    Region [%hu]"), lRgnSize);
							LogDump (_DEBUG_LOAD, (LPBYTE)lByte, lRgnSize);
#endif
							lByte += lRgnSize;
						}
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("    Overlay [%hu] Replace [%u] Image [%u] [%d] [%d] of [%4u] at [%d %d]"), lOverlay.mOverlayNdx, lOverlay.mReplaceFlag, lOverlay.mImageNdx, mAcaImages[(long)lOverlay.mImageNdx]->mImageSize.cx, mAcaImages[(long)lOverlay.mImageNdx]->mImageSize.cy, mAcaImages[(long)lOverlay.mImageNdx]->mBitsSize, lOverlay.mOffset.x, lOverlay.mOffset.y);
						}
#endif
					}
				}

				if	(FAILED (lResult))
				{
					break;
				}
				if	(lByte > (LPCBYTE)pBuffer + pBufferSize)
				{
#ifdef	_DEBUG
					LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
					break;
				}
			}
		}

		pBufferSize -= (DWORD)(lByte - (LPCBYTE)pBuffer);
		pBuffer = (LPCVOID)lByte;
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CAgentFile::ReadAcaImages (CAgentFileAnimation * pAnimation, LPCVOID & pBuffer, DWORD & pBufferSize, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_OK;

	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		WORD					lImageStart = (WORD)mAcaImages.GetSize();
		WORD					lImageCount;
		WORD					lImageNum;
		tPtr <CAgentFileImage>	lImage;
		DWORD					lByteCount;

		lImageCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);

#ifdef	_DEBUG_LOAD
		LogMessage (_DEBUG_LOAD, _T("[%ls] Aca Images [%hu] Starting [%u]"), (BSTR)pAnimation->mName, lImageCount, lImageStart);
#endif
		for	(lImageNum = 0; lImageNum < lImageCount; lImageNum++)
		{
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lByte += sizeof(BYTE); // Unknown
#ifdef	_DEBUG_LOAD
			LogMessage (_DEBUG_LOAD, _T("  Image [%hu (%hu)] of [%hu]"), lImageNum, lImageNum + lImageStart, lByteCount);
#endif
			if	(
					(lImage = new CAgentFileImage)
				&&	(lImage->mBits = new BYTE [lByteCount])
				)
			{
				lImage->mImageNum = lImageNum + lImageStart;
				lImage->mImageSize = mImageSize;
				lImage->mBitsSize = lByteCount;
				memcpy (lImage->mBits, lByte, lByteCount);

#ifdef	_SAVE_IMAGE
				if	(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_IMAGE)))
				{
					SaveImage (lImage);
				}
#endif
				mAcaImages.Add (lImage.Detach ());

#ifdef	_DUMP_IMAGE
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_IMAGE)))
				{
					DumpAcaImage ((INT_PTR)mAcaImages.GetUpperBound(), MaxLogLevel (pLogLevel, _DUMP_IMAGE));
				}
#endif
			}
			else
			{
				lResult = E_OUTOFMEMORY;
				break;
			}

			lByte += lByteCount;
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
#ifdef	_DEBUG_LOAD_NOT
			LogMessage (_DEBUG_LOAD, _T("  Region [%hu]"), lByteCount);
			LogDump (_DEBUG_LOAD, (LPBYTE)lByte, lByteCount);
#endif
			lByte += lByteCount;
			if	(lByte > (LPCBYTE)pBuffer + pBufferSize)
			{
#ifdef	_DEBUG
				LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
				break;
			}
		}

		pBufferSize -= (DWORD)(lByte - (LPCBYTE)pBuffer);
		pBuffer = (LPCVOID)lByte;
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////

HRESULT CAgentFile::ReadAcaSounds (CAgentFileAnimation * pAnimation, LPCVOID & pBuffer, DWORD & pBufferSize, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_OK;

	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		WORD					lSoundStart = (WORD)mAcaSounds.GetSize();
		WORD					lSoundCount;
		WORD					lSoundNum;
		tPtr <CAtlByteArray>	lSound;
		DWORD					lByteCount;

		lSoundCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);

#ifdef	_DEBUG_LOAD
		LogMessage (_DEBUG_LOAD, _T("[%ls] Aca Sounds [%hu] Starting [%u]"), (BSTR)pAnimation->mName, lSoundCount, lSoundStart);
#endif
		for	(lSoundNum = 0; lSoundNum < lSoundCount; lSoundNum++)
		{
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
#ifdef	_DEBUG_LOAD
			LogMessage (_DEBUG_LOAD, _T("  Sound [%hu (%hu)] of [%hu]"), lSoundNum, lSoundNum + lSoundStart, lByteCount);
#endif
			if	(lSound = new CAtlByteArray)
			{
				lSound->SetSize (lByteCount);
				memcpy (lSound->GetData(), lByte, lByteCount);
				mAcaSounds.SetAtGrow (lSoundNum+lSoundStart, lSound.Detach());
			}
			lByte += lByteCount;
		}

		pBufferSize -= (DWORD)(lByte - (LPCBYTE)pBuffer);
		pBuffer = (LPCVOID)lByte;
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::FreeImageIndex ()
{
	mImageIndex.RemoveAll ();
}

bool CAgentFile::ReadImageIndex (UINT pLogLevel)
{
	bool				lRet = false;
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadImageIndex [%s]"), mPath);
	}
	if	(IsAcsFile ())
	{
		FreeImageIndex ();
	}

	if	(
			(IsAcsFile ())
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[2].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[2].LowPart))
		)
	{
		try
		{
			LPCBYTE			lByte = lBlock;
			DWORD			lEntry;
			DWORD			lCount = *(LPCDWORD)lByte;
			UINT			lFrameNum = 0;
			LPCDWORD		lIndex;
			ULARGE_INTEGER	lIndexEntry;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] ImageIndex [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

			for	(lEntry = 0; lEntry < lCount; lEntry++)
			{
				lIndex = (LPCDWORD) lByte;
				lByte += sizeof(DWORD) * 3;

				lIndexEntry.LowPart = lIndex [0];
				lIndexEntry.HighPart = lIndex [1];
				mImageIndex.Add (lIndexEntry);
#ifdef	_DEBUG_INDEX
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_INDEX)))
#else
				if	(LogIsActive (pLogLevel))
#endif
				{
					LogMessage (pLogLevel, _T("    Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lEntry, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
				}
			}
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Images"), mImageIndex.GetSize());
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

void CAgentFile::FreeSoundIndex ()
{
	mSoundIndex.RemoveAll ();
}

bool CAgentFile::ReadSoundIndex (UINT pLogLevel)
{
	bool				lRet = false;
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadSoundIndex [%s]"), mPath);
	}
	if	(IsAcsFile ())
	{
		FreeSoundIndex ();
	}

	if	(
			(IsAcsFile ())
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[3].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[3].LowPart))
		)
	{
		try
		{
			LPCBYTE			lByte = lBlock;
			DWORD			lEntry;
			DWORD			lCount = *(LPCDWORD)lByte;
			LPCDWORD		lIndex;
			ULARGE_INTEGER	lIndexEntry;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] SoundIndex [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

			for	(lEntry = 0; lEntry < lCount; lEntry++)
			{
				lIndex = (LPCDWORD) lByte;
				lByte += sizeof(DWORD) * 3;

				lIndexEntry.LowPart = lIndex [0];
				lIndexEntry.HighPart = lIndex [1];
				mSoundIndex.Add (lIndexEntry);
#ifdef	_DEBUG_INDEX
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_INDEX)))
#else
				if	(LogIsActive (pLogLevel))
#endif
				{
					LogMessage (pLogLevel, _T("    Sound [%2u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lEntry, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
				}
			}

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Sounds"), mSoundIndex.GetSize());
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CAgentFileImage * CAgentFile::ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit, UINT pLogLevel)
{
	tPtr <CAgentFileImage>	lRet;
	tPtr <CAgentFileImage>	l32BitImage;

	try
	{
		LPCBYTE	lBlock;

		if	(
				(IsAcsFile ())
			&&	(mFileView.SafeIsValid ())
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
			LPCBYTE	lByte = lBlock;
			BYTE	lFirstByte;
			BYTE	lImageCompressed;
			CSize	lImageSize;
			DWORD	lByteCount;

			lFirstByte = *lByte;
			lByte++;
			lImageSize.cx = (int)*(LPCWORD)lByte;
			lByte += sizeof(WORD);
			lImageSize.cy = (int)*(LPCWORD)lByte;
			lByte += sizeof(WORD);
			lImageCompressed = *lByte;
			lByte++;
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  Image [%4u] of [%5u] at [%8.8X] thru [%8.8X] Size [%d %d] @1 [%u] Compressed [%u] Bytes [%u]"), pImageNum, pSize, lBlock, lBlock+pSize-1, lImageSize.cx, lImageSize.cy, lFirstByte, lImageCompressed, lByteCount);
			}

#ifdef	_DUMP_IMAGE
			if	(LogIsActive (MaxLogLevel (pLogLevel,_DUMP_IMAGE)))
			{
				LogDump (_DUMP_IMAGE, (LPBYTE)lBlock, lByteCount+10, _T("    "), true);
				if	(lByteCount < pSize)
				{
					LPCDWORD	lPart2 = (LPCDWORD)(lBlock+lByteCount+10);
					LogMessage (pLogLevel, _T("    Part 2 Compressed [%u] Decoded [%u]"), lPart2[0], lPart2[1]);
					LogDump (_DUMP_IMAGE, (LPBYTE)lBlock+lByteCount+10, pSize-lByteCount-10, _T("      "), true);
#if FALSE
					if	(lPart2 [0])
					{
						tArrayPtr <BYTE>	lPart2Decoded;

						if	(
								(lPart2Decoded = new BYTE [lPart2[1]])
							&&	(DecodeImage ((LPCBYTE)(lPart2+2), lPart2[0], (LPCBYTE)lPart2Decoded, lPart2[1], CSize(0,0)))
							)
						{
							LogMessage (pLogLevel, _T("    Part 2 Decoded [%u]"), lPart2[1]);
							LogDump (_DUMP_IMAGE, (LPCBYTE)lPart2Decoded, lPart2[1], _T("      "), true);
						}
					}
#endif
				}
			}
#endif
			if	(
					(lImageSize.cx > 0)
				&&	(lImageSize.cy > 0)
				&&	(lFirstByte > 0)
				&&	(lRet = new tS <CAgentFileImage>)
				)
			{
				lRet->mImageNum = pImageNum;
				lRet->mImageSize = lImageSize;

				if	(lImageCompressed)
				{
					lRet->mBitsSize = (((lImageSize.cx + 3) / 4) * 4) * lImageSize.cy;

					if	(lRet->mBits = new BYTE [lRet->mBitsSize])
					{
						if	(!DecodeImage (lByte, lByteCount, lRet->mBits, lRet->mBitsSize, lImageSize))
						{
							LogMessage (LogNormal, _T("*** [%s] Decode image [%u] failed ***"), mPath, pImageNum);
							//lRet = NULL;
						}
					}
					else
					{
						lRet = NULL;
					}
				}
				else
				{
					if	(lRet->mBits = new BYTE [lByteCount])
					{
						lRet->mBitsSize = lByteCount;
						memcpy ((LPBYTE)lRet->mBits, lByte, lByteCount);
					}
					else
					{
						lRet = NULL;
					}
				}
			}

			if	(
					(p32Bit)
				&&	(lRet)
				&&	(mPalette)
				&&	(l32BitImage = new tS <CAgentFileImage>)
				&&	(l32BitImage->mBitsSize = lRet->mImageSize.cx * lRet->mImageSize.cy * 4)
				&&	(l32BitImage->mBits = new BYTE [l32BitImage->mBitsSize])
				)
			{
				CPoint	lPixel;
				INT_PTR	lSrcNdx;
				INT_PTR	lTrgNdx;
				int		lSrcScanBytes;
				int		lTrgScanBytes;

				l32BitImage->mImageNum = pImageNum;
				l32BitImage->mImageSize = lImageSize;

				lSrcScanBytes = ((lImageSize.cx + 3) / 4) * 4;
				lTrgScanBytes = lImageSize.cx * 4;

				for	(lPixel.y = 0; lPixel.y < lImageSize.cy; lPixel.y++)
				{
					lSrcNdx = lPixel.y * lSrcScanBytes;
					lTrgNdx = lPixel.y * lTrgScanBytes;

					for	(lPixel.x = 0; lPixel.x < lImageSize.cx; lPixel.x++)
					{
						if	(lRet->mBits [lSrcNdx] == mTransparency)
						{
							*(COLORREF *)(l32BitImage->mBits+lTrgNdx) = 0;
						}
						else
						{
							*(COLORREF *)(l32BitImage->mBits+lTrgNdx) = mPalette [(long)lRet->mBits [lSrcNdx]] | 0xFF000000;
						}
						lSrcNdx++;
						lTrgNdx += 4;
					}
				}

				lRet = l32BitImage.Detach();
			}
		}
	}
	catch AnyExceptionDebug

	return lRet.Detach ();
}

//////////////////////////////////////////////////////////////////////

LPCVOID CAgentFile::ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum, UINT pLogLevel)
{
	LPCVOID	lRet = NULL;

	try
	{
		LPCBYTE	lBlock;

		if	(
				(IsAcsFile ())
			&&	(mFileView.SafeIsValid ())
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  Sound [%4u] of [%5u] at [%8.8X] thru [%8.8X]"), pSoundNum, pSize, lBlock, lBlock+pSize-1);
			}
			lRet = (LPCVOID) lBlock;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CAgentFile::DecodeImage (LPCVOID pSrcBits, ULONG pSrcCount, LPBYTE pTrgBits, ULONG pTrgCount, const CSize & pTrgSize)
{
	bool	lRet;

#ifdef	_SHOW_IMAGE
	CImageDebugger		lDebugger;
	tArrayPtr <BYTE>	lDebugInfoBuffer;
	LPBITMAPINFO		lDebugInfo = NULL;

	if	(
			(pTrgSize.cx > 0)
		&&	(pTrgSize.cy > 0)
		&&	(lDebugInfoBuffer = new BYTE [GetImageFormat (NULL)])
		&&	(lDebugInfo = (LPBITMAPINFO) (LPBYTE) lDebugInfoBuffer)
		)
	{
		GetImageFormat (lDebugInfo);
		lDebugInfo->bmiHeader.biWidth = pTrgSize.cx;
		lDebugInfo->bmiHeader.biHeight = pTrgSize.cy;
		lDebugInfo->bmiHeader.biSizeImage = pTrgCount;
	}
	memset (pTrgBits, 0xFF, pTrgCount);
#endif

	if	(DecodeData (pSrcBits, pSrcCount, pTrgBits, pTrgCount) == pTrgCount)
	{
		lRet = true;
	}

#ifdef	_SHOW_IMAGE
	if	(
			(lRet)
		&&	(lDebugInfo)
		)
	{
		CBitmap	lBitmap;
		LPVOID	lBits = NULL;

		if	(lBitmap.Attach (CreateDIBSection (NULL, lDebugInfo, DIB_RGB_COLORS, &lBits, NULL, 0)))
		{
			memcpy (lBits, pTrgBits, pTrgCount);
			GdiFlush ();

			lDebugger.ShowBitmap (lBitmap, (pTrgSize.cy<=32)?4:(pTrgSize.cy<=64)?2:1, _T("Decode"), false);
			switch	(lDebugger.DoModal ())
			{
				case IDCANCEL:	lDebugInfo = NULL; break;
				case IDCLOSE:	ExitProcess (IDCANCEL);
			}
		}
	}
#endif

	return lRet;
}

ULONG CAgentFile::DecodeData (LPCVOID pSrcData, ULONG pSrcSize, LPVOID pTrgData, ULONG pTrgSize)
{
	LPCBYTE	lSrcPtr = (LPCBYTE)pSrcData;
	LPCBYTE	lSrcEnd = lSrcPtr + pSrcSize;
	LPBYTE	lTrgPtr = (LPBYTE)pTrgData;
	LPBYTE	lTrgEnd = lTrgPtr + pTrgSize;
	DWORD	lSrcQuad;
	BYTE	lTrgByte;
	DWORD	lBitCount = 0;
	DWORD	lSrcOffset;
	DWORD	lRunLgth;
	DWORD	lRunCount;

	if	(
			(pSrcSize <= 7)
		||	(*lSrcPtr != 0)
		)
	{
		return 0;
	}

	for	(lBitCount = 1; (*(lSrcEnd-lBitCount) == 0xFF); lBitCount++)
	{
		if	(lBitCount > 6)
		{
			break;
		}
	}
	if	(lBitCount < 6)
	{
		return 0;
	}

	lBitCount = 0;
	lSrcPtr += 5;

	while	(
				(lSrcPtr < lSrcEnd)
			&&	(lTrgPtr < lTrgEnd)
			)
	{
		lSrcQuad = *(LPCDWORD)(lSrcPtr - sizeof(DWORD));

		if	(lSrcQuad & (1 << LOWORD(lBitCount)))
		{
			lSrcOffset = 1;

			if	(lSrcQuad & (1 << LOWORD(lBitCount+1)))
			{
				if	(lSrcQuad & (1 << LOWORD(lBitCount+2)))
				{
					if	(lSrcQuad & (1 << LOWORD(lBitCount+3)))
					{
						lSrcQuad >>= LOWORD(lBitCount+4);
						lSrcQuad &= 0x000FFFFF;
						if	(lSrcQuad == 0x000FFFFF)
						{
							break;
						}
						lSrcQuad += 4673;
						lBitCount += 24;

						lSrcOffset = 2;
					}
					else
					{
						lSrcQuad >>= LOWORD(lBitCount+4);
						lSrcQuad &= 0x00000FFF;
						lSrcQuad += 577;
						lBitCount += 16;
					}
				}
				else
				{
					lSrcQuad >>= LOWORD(lBitCount+3);
					lSrcQuad &= 0x000001FF;
					lSrcQuad += 65;
					lBitCount += 12;
				}
			}
			else
			{
				lSrcQuad >>= LOWORD(lBitCount+2);
				lSrcQuad &= 0x0000003F;
				lSrcQuad += 1;
				lBitCount += 8;
			}

			lSrcPtr += (lBitCount/8);
			lBitCount &= 7;
			lRunLgth = *(LPCDWORD)(lSrcPtr - sizeof(DWORD));
			lRunCount = 0;
			while	(lRunLgth & (1 << LOWORD(lBitCount+lRunCount)))
			{
				lRunCount++;
				if	(lRunCount > 11)
				{
					break;
				}
			}

			lRunLgth >>= LOWORD(lBitCount+lRunCount+1);
			lRunLgth &= (1 << lRunCount) - 1;
			lRunLgth += 1 << lRunCount;
			lRunLgth += lSrcOffset;
			lBitCount += lRunCount*2+1;

			if	(lTrgPtr + lRunLgth > lTrgEnd)
			{
				break;
			}
			if	(lTrgPtr - lSrcQuad < pTrgData)
			{
				break;
			}
			while	((long)lRunLgth > 0)
			{
				lTrgByte = *(lTrgPtr - lSrcQuad);
				*(lTrgPtr++) = lTrgByte;
				lRunLgth--;
			}
		}
		else
		{
			lSrcQuad >>= LOWORD(lBitCount+1);
			lBitCount += 9;

			lTrgByte = LOBYTE(lSrcQuad);
			*(lTrgPtr++) = lTrgByte;
		}

		lSrcPtr += lBitCount/8;
		lBitCount &= 7;
	}

#ifdef	_DEBUG
	if	(lTrgPtr - (LPBYTE)pTrgData != pTrgSize)
	{
		LogMessage (LogDebug, _T("Decoded [%u] of [%u]"), lTrgPtr - (LPBYTE)pTrgData, pTrgSize);
	}
#endif
	return (ULONG)(lTrgPtr - (LPBYTE)pTrgData);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

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
			LogMessage (pLogLevel|LogHighVolume, _T("  Guid            [%s]"), (LPCTSTR) CGuidStr (mGuid));
			LogMessage (pLogLevel|LogHighVolume, _T("  Image Size      [%d %d]"), mImageSize.cx, mImageSize.cy);
			LogMessage (pLogLevel|LogHighVolume, _T("  Transparency    [%d]"), mTransparency);
			LogMessage (pLogLevel|LogHighVolume, _T("  Style           [%8.8X]"), mStyle);
			LogMessage (pLogLevel|LogHighVolume, _T("  Icon            [%p]"), mIcon);
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
			const CAgentFileName *	lName;

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
			LogMessage (pLogLevel, _T("%s [%d] [%s]"), lTitle, mNames.GetSize(), mPath);

			for	(lNameNdx = 0; lNameNdx <= (INT_PTR)mNames.GetUpperBound(); lNameNdx++)
			{
				if	(lName = mNames (lNameNdx))
				{
					LogMessage (pLogLevel|LogHighVolume, _T("  [%4.4X] [%ls] [%ls]"), lName->mLanguage, (BSTR)lName->mName, DebugStr(CAtlString((BSTR)lName->mDesc1)));
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
			LogMessage (pLogLevel, _T("%s [%d] [%s]"), lTitle, mStates.mGestures.GetSize(), mPath);

			for	(lNdx = 0; lNdx <= (INT_PTR)mStates.mGestures.GetUpperBound(); lNdx++)
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
			const CAgentFileAnimation *	lAnimation;

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
			LogMessage (pLogLevel, _T("%s [%d] [%s]"), lTitle, mGestures.mAnimations.GetSize(), mPath);

			for	(lNdx = 0; lNdx <= (INT_PTR)mGestures.mAnimations.GetUpperBound(); lNdx++)
			{
				if	(lAnimation = mGestures.mAnimations [lNdx])
				{
					CAtlString	lSuffix;

					if	(lAnimation->mAcaFileName.Ptr())
					{
						lSuffix.Format (_T(" File [%ls] [%8.8X]"), (BSTR)lAnimation->mAcaFileName, lAnimation->mAcaChksum);
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

//////////////////////////////////////////////////////////////////////

void CAgentFile::LogTts (const CAgentFileTts & pTts, UINT pLogLevel, LPCTSTR pFormat, ...)
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
			LogMessage (pLogLevel, _T("%s  Engine        [%s]"), lIndent, (LPCTSTR) CGuidStr (pTts.mEngine));
			LogMessage (pLogLevel, _T("%s  Mode          [%s]"), lIndent, (LPCTSTR) CGuidStr (pTts.mMode));
			LogMessage (pLogLevel, _T("%s  Gender        [%hu]"), lIndent, pTts.mGender);
			LogMessage (pLogLevel, _T("%s  Age           [%hu]"), lIndent, pTts.mAge);
			LogMessage (pLogLevel, _T("%s  Speed         [%d]"), lIndent, pTts.mSpeed);
			LogMessage (pLogLevel, _T("%s  Pitch         [%hd]"), lIndent, pTts.mPitch);
			LogMessage (pLogLevel, _T("%s  Language      [%4.4hu %4.4hX]"), lIndent, pTts.mLanguage, pTts.mLanguage);
			LogMessage (pLogLevel, _T("%s  Style         [%ls]"), lIndent, (BSTR)pTts.mStyle);
		}
		catch AnyExceptionDebug
	}
#endif
}

void CAgentFile::LogBalloon (const CAgentFileBalloon & pBalloon, UINT pLogLevel, LPCTSTR pFormat, ...)
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
				LogMessage (pLogLevel, _T("%s  Lines         [%u]"), lIndent, pBalloon.mLines);
				LogMessage (pLogLevel, _T("%s  Per Line      [%u]"), lIndent, pBalloon.mPerLine);
				LogMessage (pLogLevel, _T("%s  Foreground    [%8.8X]"), lIndent, pBalloon.mFgColor);
				LogMessage (pLogLevel, _T("%s  Background    [%8.8X]"), lIndent, pBalloon.mBkColor);
				LogMessage (pLogLevel, _T("%s  Border        [%8.8X]"), lIndent, pBalloon.mBrColor);
				LogMessage (pLogLevel, _T("%s  Font Name     [%ls]"), lIndent, (BSTR)pBalloon.mFontName);
				LogMessage (pLogLevel, _T("%s  Font Height   [%d]"), lIndent, pBalloon.mFontHeight);
				LogMessage (pLogLevel, _T("%s  Font Weight   [%hu]"), lIndent, pBalloon.mFontWeight);
				LogMessage (pLogLevel, _T("%s  Font Italic   [%hu]"), lIndent, pBalloon.mFontItalic);
				LogMessage (pLogLevel, _T("%s  Font Under    [%hu]"), lIndent, pBalloon.mFontUnderline);
				LogMessage (pLogLevel, _T("%s  Font Strike   [%hu]"), lIndent, pBalloon.mFontStrikethru);
		}
		catch AnyExceptionDebug
	}
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::DumpBlocks (UINT pLogLevel, UINT pMaxBlockSize)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			DWORD				lFileSize;
			CGenericHandle		lFileMapping;
			CMappedHandle		lFileView;
			LPCULARGE_INTEGER	lBlockDefs;
			DWORD				lBlockOffset;
			DWORD				lBlockLength;
			LPCBYTE				lBlock;

			lFileSize = GetFileSize (mFileHandle, NULL);
			lFileMapping = CreateFileMapping (mFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
			lFileView = MapViewOfFile (lFileMapping, FILE_MAP_READ, 0, 0, lFileSize);

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[0].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
					)
				{
					LPCBYTE	lByte = lBlock;
					LPCBYTE	lPrevByte = lBlock;
					DWORD	lStrLen;
					DWORD	lPaletteSize;
					DWORD	lIconSize;
					WORD	lStateCount;
					LPCBYTE	lNamesStart = ((LPCDWORD)lBlock) [1] - lBlockOffset + lBlock;
					WORD	lNameCount;
					WORD	lNameNdx;

					LogMessage (pLogLevel, _T("Blocks [%u] at [%8.8X] (%u) of (%u)"), lBlockLength, lBlockOffset, lBlockOffset, lFileSize);

					lByte += 41;
					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Header Block [%u] at [%8.8X] [%8.8X]"), lByte-lPrevByte, lPrevByte-lBlock, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lByte-lPrevByte,pMaxBlockSize), _T("    "), true);
					lPrevByte = lByte;

					if	(mStyle & CharStyleTts)
					{
						lByte += sizeof (GUID) + sizeof (GUID) + sizeof (long) + sizeof (short) + sizeof (BYTE) + sizeof (WORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lByte += sizeof(DWORD) + sizeof (WORD) + sizeof (WORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lByte += sizeof(DWORD);

						LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Tts Block [%u] at [%8.8X]"), lByte-lPrevByte, lPrevByte-lBlock);
						LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lByte-lPrevByte,pMaxBlockSize), _T("    "), true);
						lPrevByte = lByte;
					}

					if	(mStyle & CharStyleBalloon)
					{
						lByte += sizeof (BYTE) + sizeof (BYTE) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lByte += sizeof(DWORD) + sizeof (long) + sizeof (WORD) + sizeof(DWORD);

						LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Balloon Block [%u] at [%8.8X]"), lByte-lPrevByte, lPrevByte-lBlock);
						LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lByte-lPrevByte,pMaxBlockSize), _T("    "), true);
						lPrevByte = lByte;
					}

					lPaletteSize = *(LPCDWORD)lByte;
					lByte += sizeof(DWORD);
					lPrevByte = lByte;
					lByte += lPaletteSize * sizeof(DWORD);

					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Palette [%u] at [%8.8X]"), lPaletteSize, lPrevByte-lBlock);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, lPaletteSize*sizeof(DWORD), _T("    "), true);
					lPrevByte = lByte;

					if	(*(lByte++))
					{
						lIconSize = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						lPrevByte = lByte;
						lByte += lIconSize;
						lIconSize += *(LPCDWORD)lByte;
						lIconSize += sizeof(DWORD);
						lByte += *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);

						LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Icon [%u] at [%8.8X]"), lIconSize, lPrevByte-lBlock);
						LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, lIconSize, _T("    "), true);
					}
					lPrevByte = lByte;

					lStateCount = *(LPCWORD)lByte;
					lByte += sizeof (WORD);

					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  [%u] States [%u] at [%8.8X]"), lStateCount, lNamesStart-lPrevByte, lPrevByte-lBlock);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lNamesStart-lPrevByte,pMaxBlockSize), _T("    "), true);
					lPrevByte = lNamesStart;

					lByte = lNamesStart;
					lNameCount = *(LPCWORD)lByte;
					lByte += sizeof (WORD);

					for (lNameNdx = 0; lNameNdx < lNameCount; lNameNdx++)
					{
						lByte += sizeof (WORD); // Language

						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
					}

					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  [%u] Names [%u] at [%8.8X] [%8.8X]"), lNameCount, lByte-lNamesStart, lNamesStart-lBlock, lNamesStart+lBlockOffset-lBlock);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lNamesStart, min(lByte-lNamesStart,pMaxBlockSize), _T("    "), true);
					lPrevByte = lByte;
				}
			}
			catch AnyExceptionDebug

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[1].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[1].LowPart))
					)
				{
					LogMessage (pLogLevel, _T("  [%u] Gestures [%u] at [%8.8X] (%u)"), *(LPCDWORD)lBlock, lBlockLength, lBlockOffset, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lBlock, min(lBlockLength,pMaxBlockSize), _T("    "), true);
				}
			}
			catch AnyExceptionDebug

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[2].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[2].LowPart))
					)
				{
					LogMessage (pLogLevel, _T("  [%u] Images [%u] at [%8.8X] (%u)"), *(LPCDWORD)lBlock, lBlockLength, lBlockOffset, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lBlock, min(lBlockLength,pMaxBlockSize), _T("    "), true);
				}
			}
			catch AnyExceptionDebug

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[3].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[3].LowPart))
					)
				{
					LogMessage (pLogLevel, _T("  [%u] Sounds [%u] at [%8.8X] (%u)"), *(LPCDWORD)lBlock, lBlockLength, lBlockOffset, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lBlock, min(lBlockLength,pMaxBlockSize), _T("    "), true);
				}
			}
			catch AnyExceptionDebug

			lFileView.Close ();
			lFileMapping.Close ();
		}
		catch AnyExceptionDebug
	}
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::DumpPalette (LPVOID pPalette)
{
#ifdef	_DEBUG
	tArrayPtr <BYTE>	lInfoBuff;
	BITMAPINFO *		lInfo;
	ATL::CImage			lDumpBmp;
	LPBYTE				lDumpBits;

	lInfoBuff = new BYTE [sizeof (BITMAPINFOHEADER) + (sizeof(COLORREF) * 256)];
	lInfo = (BITMAPINFO *) (LPBYTE) lInfoBuff;

	lInfo->bmiHeader.biSize = sizeof (lInfo->bmiHeader);
	lInfo->bmiHeader.biWidth = 256;
	lInfo->bmiHeader.biHeight = -256;
	lInfo->bmiHeader.biBitCount = 8;
	lInfo->bmiHeader.biPlanes = 1;
	lInfo->bmiHeader.biCompression = BI_RGB;
	lInfo->bmiHeader.biSizeImage = lInfo->bmiHeader.biWidth * abs (lInfo->bmiHeader.biHeight);
	lInfo->bmiHeader.biClrUsed = 256;
	memcpy (lInfo->bmiColors, pPalette, sizeof(DWORD) * 256);

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

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CAgentFile::DumpAcsImages (UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			tPtr <CAgentFileImage>	lImage;
			INT_PTR					lImageNdx;

			LogMessage (pLogLevel, _T("[%s] Images [%d]"), mPath, mImageIndex.GetSize());

			for	(lImageNdx = 0; lImageNdx <= (INT_PTR)mImageIndex.GetUpperBound(); lImageNdx++)
			{
				if	(lImage = ReadAcsImage (mImageIndex [lImageNdx].LowPart, mImageIndex [lImageNdx].HighPart, (UINT)lImageNdx, false, pLogLevel))
				{
#ifdef	_SAVE_IMAGE
					SaveImage (lImage);
#endif
				}
				else
				{
					LogMessage (pLogLevel, _T("*** [%s] Load image [%d] failed ***"), mPath, lImageNdx);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void CAgentFile::DumpAcsImage (INT_PTR pImageNdx, UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(
			(LogIsActive (pLogLevel))
		&&	(pImageNdx >= 0)
		&&	(pImageNdx <= (INT_PTR)mImageIndex.GetUpperBound ())
		)
	{
		try
		{
			tPtr <CAgentFileImage>	lImage;

			if	(lImage = ReadAcsImage (mImageIndex [pImageNdx].LowPart, mImageIndex [pImageNdx].HighPart, (UINT)pImageNdx, false, pLogLevel))
			{
#ifdef	_SAVE_IMAGE
				SaveImage (lImage);
#endif
			}
			else
			{
				LogMessage (pLogLevel, _T("*** [%s] Load image [%d] failed ***"), mPath, pImageNdx);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}
//////////////////////////////////////////////////////////////////////

void CAgentFile::DumpAcaImages (UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			INT_PTR	lImageNdx;

			LogMessage (pLogLevel, _T("[%s] Images [%d]"), mPath, mAcaImages.GetSize());

			for	(lImageNdx = 0; lImageNdx <= (INT_PTR)mAcaImages.GetUpperBound(); lImageNdx++)
			{
				DumpAcsImage (lImageNdx, pLogLevel);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void CAgentFile::DumpAcaImage (INT_PTR pImageNdx, UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(
			(LogIsActive (pLogLevel))
		&&	(pImageNdx >= 0)
		&&	(pImageNdx <= (INT_PTR)mAcaImages.GetUpperBound ())
		)
	{
		try
		{
			CAgentFileImage *	lImage;

			if	(lImage = mAcaImages [pImageNdx])
			{
				LogMessage (pLogLevel, _T("  [%.3d] Image [%.3u] [%d x %d] Bits [%u]"), pImageNdx, lImage->mImageNum, lImage->mImageSize.cx, lImage->mImageSize.cy, lImage->mBitsSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

//////////////////////////////////////////////////////////////////////

void CAgentFile::SaveImage (CAgentFileImage * pImage)
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

					memcpy (lBits, (LPBYTE)pImage->mBits, pImage->mBitsSize);
					GdiFlush ();
					lDumpName.Format (_T("Image %.4u"), pImage->mImageNum);
					CImageDebugger::SaveBitmap (lBitmap, lDumpName);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}
