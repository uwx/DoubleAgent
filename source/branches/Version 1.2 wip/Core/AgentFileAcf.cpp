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
#include "AgentFileAcf.h"
#include <shlwapi.h>
#include <wininet.h>
#include "DaCore.h"
#include "FileDownload.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "DebugStr.h"
#endif
#include "AgentFileDefs.inl"

#ifndef _M_CEE
IMPLEMENT_DLL_OBJECT(CAgentFileAcf)
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentFileAcf::CAgentFileAcf()
{
	Close ();
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileAcf::CAgentFileAcf"), this);
#endif
}

CAgentFileAcf::~CAgentFileAcf ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileAcf::~CAgentFileAcf"), this);
#endif
	Close ();
}

CAgentFileAcf* CAgentFileAcf::CreateInstance ()
{
	return new CAgentFileAcf;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcf::get_IsAcfFile () const
{
	if	(IsOpen)
	{
		return (mSignature == sAcfFileSignature);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileAcf::Open (LPCTSTR pPath, UINT pLogLevel)
{
	HRESULT		lResult = S_OK;
	CAtlString	lPath = ParseFilePath (pPath);

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif

	Close ();
	mPath = lPath;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Open [%s]"), lPath);
	}

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

void CAgentFileAcf::Close ()
{
	mAcaImages.DeleteAll ();
	mAcaSounds.DeleteAll ();
	__super::Close ();
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcf::SetDownloadMode (bool pRefresh, bool pReload, bool pSecure)
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

HRESULT CAgentFileAcf::LoadAcf (CFileDownload* pDownload, UINT pLogLevel)
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

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcf::IsAnimationLoaded (INT_PTR pAnimationNdx)
{
	bool					lRet = false;
	CAgentFileAnimation*	lAnimation = NULL;

	if	(
			(IsOpen)
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		&&	(lAnimation->mAcaChksum == (DWORD)-1)
		)
	{
		lRet = true;
	}
	return lRet;
}

HRESULT CAgentFileAcf::LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload* pDownload)
{
	HRESULT					lResult = S_OK;
	CAgentFileAnimation*	lAnimation = NULL;

	if	(!pDownload)
	{
		lResult = E_POINTER;
	}
	else
	if	(!IsOpen)
	{
		lResult = AGENTPROVERROR_CHARACTERINVALID;
	}
	else
	if	(
			(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
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

tBstrPtr CAgentFileAcf::GetAnimationAcaPath (INT_PTR pAnimationNdx)
{
	CAtlString				lPath;
	CAgentFileAnimation*	lAnimation = NULL;

	if	(
			(IsOpen)
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		)
	{
		lPath = GetAcaPath (lAnimation);
	}
	return lPath.AllocSysString();
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcf::IsAnimationLoaded (LPCTSTR pAnimationName)
{
	return IsAnimationLoaded (FindAnimation (pAnimationName));
}

HRESULT CAgentFileAcf::LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload* pDownload)
{
	return LoadAnimationAca (FindAnimation (pAnimationName), pDownload);
}

tBstrPtr CAgentFileAcf::GetAnimationAcaPath (LPCTSTR pAnimationName)
{
	return GetAnimationAcaPath (FindAnimation (pAnimationName));
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

const CAgentFileAnimation* CAgentFileAcf::GetGesture (INT_PTR pGestureNdx)
{
	CAgentFileAnimation*	lRet = const_cast <CAgentFileAnimation*> (__super::GetGesture (pGestureNdx));

	if	(lRet)
	{
		ReadAcaFile (lRet);
	}
	return lRet;
}

const CAgentFileAnimation* CAgentFileAcf::GetAnimation (INT_PTR pAnimationNdx)
{
	CAgentFileAnimation*	lRet = const_cast <CAgentFileAnimation*> (__super::GetAnimation (pAnimationNdx));

	if	(lRet)
	{
		ReadAcaFile (lRet);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFileAcf::GetImageCount () const
{
	return mAcaImages.GetCount();
}

CAgentFileImage* CAgentFileAcf::GetImage (INT_PTR pImageNdx, bool p32Bit, UINT pLogLevel)
{
	tPtr <CAgentFileImage>	lImage;
	const CAgentFileImage*	lAcfImage;

	if	(
			(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mAcaImages.GetCount())
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
	return lImage.Detach ();
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFileAcf::GetSoundCount () const
{
	return mAcaSounds.GetCount();
}

long CAgentFileAcf::GetSoundSize (INT_PTR pSoundNdx)
{
	long			lRet = -1;
	CAtlByteArray *	lAcaSound;

	if	(lAcaSound = mAcaSounds (pSoundNdx))
	{
		lRet = (long)lAcaSound->GetCount();
	}
	return lRet;
}

LPCVOID CAgentFileAcf::GetSound (INT_PTR pSoundNdx)
{
	LPCVOID			lSound = NULL;
	CAtlByteArray *	lAcaSound;

	if	(lAcaSound = mAcaSounds (pSoundNdx))
	{
		lSound = lAcaSound->GetData();
	}
	return lSound;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileAcf::ReadHeader (UINT pLogLevel)
{
	HRESULT	lResult = __super::ReadHeader (pLogLevel);

	if	(
			(SUCCEEDED (lResult))
		&&	(
				(mSignature != sAcfFileSignature)
			||	(!ReadAcfHeader (pLogLevel))
			)
		)
	{
		lResult = AGENTPROVERROR_MAGIC;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcf::ReadAcfHeader (UINT pLogLevel)
{
	bool				lRet = false;
	DWORD				lUncompressedSize;
	DWORD				lCompressedSize;
	LPCBYTE				lHeaderData = NULL;
	tArrayPtr <BYTE>	lUncompressedData;

	FreeNames ();
	FreeGestures ();
	FreeStates ();

	if	(
			(IsOpen)
		&&	(IsAcfFile)
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

				mHeader.mGuid = *(LPCGUID)lByte;
				lByte += sizeof (GUID);
				lByte = (LPCBYTE)ReadBufferNames (lByte, false, true, pLogLevel);

				mHeader.mImageSize.cx = *(LPCWORD)lByte;
				lByte += sizeof (WORD);
				mHeader.mImageSize.cy = *(LPCWORD)lByte;
				lByte += sizeof (WORD);
				mHeader.mTransparency = *lByte;
				lByte++;
				mHeader.mStyle = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lByte += sizeof(DWORD); // Unknown - always 0x0000002

				if	(mHeader.mStyle & CharStyleTts)
				{
					lByte = (LPCBYTE)ReadBufferTts (lByte, false, pLogLevel);
				}
				if	(mHeader.mStyle & CharStyleBalloon)
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

/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentFileAcf::GetAcaPath (CAgentFileAnimation* pAnimation)
{
	if	(pAnimation)
	{
		return CAtlString (ParseRelativePath (CAtlString(pAnimation->mAcaFileName)));
	}
	return CAtlString();
}

HRESULT CAgentFileAcf::ReadAcaFile (CAgentFileAnimation* pAnimation, bool p32Bit, UINT pLogLevel)
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

HRESULT CAgentFileAcf::ReadAcaFile (CAgentFileAnimation* pAnimation, LPCTSTR pPath, bool p32Bit, UINT pLogLevel)
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
					WORD	lImageStart = (WORD)mAcaImages.GetCount();

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
				LogWinErr (LogNormal|LogTime, GetLastError(), _T("Open [%s]"), pPath);
			}
		}

		lFileView.Close ();
		lFileMapping.Close ();
		lFileHandle.Close ();
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileAcf::ReadAcaFrames (CAgentFileAnimation* pAnimation, LPCVOID& pBuffer, DWORD& pBufferSize, WORD pImageStart, bool p32Bit, UINT pLogLevel)
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
				CAgentFileFrame*	lFrame = &pAnimation->mFrames [(short)(long)lFrameNum];
				BYTE				lBranchCount;
				BYTE				lOverlayCount;
				BYTE				lOverlayReplace;
				INT_PTR				lNdx;

				//LogDump (LogDebugFast, (LPBYTE)lByte, sizeof(WORD)*7);

				lFrame->mImageCount = 1;
				lFrame->mImages = new tS <CAgentFileFrameImage> [1];
				lFrame->mImages [0].mImageNdx = *(LPCWORD)lByte + pImageStart;
				lByte += sizeof(WORD);

				lFrame->mSoundNdx = *(LPCWORD)lByte;
				lByte += sizeof(WORD);
				lFrame->mDuration = *(LPCWORD)lByte;
				lByte += sizeof(WORD);
				DWORD lSomething = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lFrame->mExitFrame = *(LPCWORD)lByte;
				lByte += sizeof(WORD);

#ifdef	_DEBUG_ANIMATION
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
				{
					LogMessage (_DEBUG_ANIMATION, _T("  Frame [%u] Image [%hu] Duration [%hu] ExitFrame [%hd] Sound [%2hd] Something [%8.8X]"), lFrameNum, lFrame->mImages [0].mImageNdx, lFrame->mDuration, lFrame->mExitFrame, lFrame->mSoundNdx, lSomething);
				}
#endif
				lBranchCount = *lByte;
				lByte++;
				if	(lBranchCount > 0)
				{
					lFrame->mBranching [0].mFrameNdx = LOWORD(((LPCDWORD)lByte) [0]);
					lFrame->mBranching [0].mProbability = HIWORD(((LPCDWORD)lByte) [0]);
				}
				if	(lBranchCount > 1)
				{
					lFrame->mBranching [1].mFrameNdx = LOWORD(((LPCDWORD)lByte) [1]);
					lFrame->mBranching [1].mProbability = LOWORD(((LPCDWORD)lByte) [1]);
				}
				if	(lBranchCount > 2)
				{
					lFrame->mBranching [2].mFrameNdx = LOWORD(((LPCDWORD)lByte) [2]);
					lFrame->mBranching [2].mProbability = LOWORD(((LPCDWORD)lByte) [2]);
				}
				lByte += sizeof(WORD)*lBranchCount*2;

#ifdef	_DEBUG_ANIMATION
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
				{
					for	(lNdx = 0; lNdx < min ((INT_PTR)lBranchCount, 3); lNdx++)
					{
						LogMessage (_DEBUG_ANIMATION, _T("    Branch [%d] [%u]"), lFrame->mBranching [lNdx].mFrameNdx, lFrame->mBranching [lNdx].mProbability);
					}
				}
#endif
				lOverlayCount = *lByte;
				lByte++;
				if	(lOverlayCount > 0)
				{
					if	(lFrame->mOverlays = new tS <CAgentFileFrameOverlay> [lOverlayCount])
					{
						lFrame->mOverlayCount = lOverlayCount;
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

					for	(lNdx = 0; lNdx < (INT_PTR)lOverlayCount; lNdx++)
					{
						CAgentFileFrameOverlay*	lOverlay = &lFrame->mOverlays [lNdx];
						tPtr <CAgentFileImage>		lOverlayImage = new tS <CAgentFileImage>;

						//LogDump (LogDebugFast, (LPBYTE)lByte, 15);

						lOverlay->mOverlayType = (AgentMouthOverlay)*lByte;
						lByte += sizeof(BYTE);
						lOverlayImage->mBitsSize = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						lByte += sizeof(BYTE); // Unknown
						lOverlay->mReplaceFlag = (*lByte) ? true : false;
						lByte += sizeof(BYTE);
						lOverlay->mOffset.x = (long)(short)*(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lOverlay->mOffset.y = (long)(short)*(LPCWORD)lByte;
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
							lOverlay->mImageNdx = (DWORD)(mAcaImages.GetCount()-1);
						}
						else
						{
							lResult = E_OUTOFMEMORY;
							break;
						}

						if	(lOverlay->mReplaceFlag)
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
							LogMessage (_DEBUG_ANIMATION, _T("    Overlay [%hu] Replace [%u] Image [%u] [%d] [%d] of [%4u] at [%d %d]"), lOverlay->OverlayType, lOverlay->ReplaceFlag, lOverlay->ImageNdx, mAcaImages[(long)lOverlay->ImageNdx]->ImageSize.cx, mAcaImages[(long)lOverlay->ImageNdx]->ImageSize.cy, mAcaImages[(long)lOverlay->ImageNdx]->BitsSize, lOverlay->Offset.x, lOverlay->Offset.y);
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

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileAcf::ReadAcaImages (CAgentFileAnimation* pAnimation, LPCVOID& pBuffer, DWORD& pBufferSize, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_OK;

	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		WORD					lImageStart = (WORD)mAcaImages.GetCount();
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
				lImage->mImageSize = mHeader.ImageSize;
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
					DumpAcaImage ((INT_PTR)mAcaImages.GetCount()-1, MaxLogLevel (pLogLevel, _DUMP_IMAGE));
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

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileAcf::ReadAcaSounds (CAgentFileAnimation* pAnimation, LPCVOID& pBuffer, DWORD& pBufferSize, bool p32Bit, UINT pLogLevel)
{
	HRESULT	lResult = S_OK;

	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		WORD					lSoundStart = (WORD)mAcaSounds.GetCount();
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
				lSound->SetCount (lByteCount);
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcf::DumpAcaImages (UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			INT_PTR	lImageNdx;

			LogMessage (pLogLevel, _T("[%s] Images [%d]"), mPath, mAcaImages.GetCount());

			for	(lImageNdx = 0; lImageNdx < (INT_PTR)mAcaImages.GetCount(); lImageNdx++)
			{
				DumpAcaImage (lImageNdx, pLogLevel);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void CAgentFileAcf::DumpAcaImage (INT_PTR pImageNdx, UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(
			(LogIsActive (pLogLevel))
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mAcaImages.GetCount())
		)
	{
		try
		{
			CAgentFileImage*	lImage;

			if	(lImage = mAcaImages [pImageNdx])
			{
				LogMessage (pLogLevel, _T("  [%.3d] Image [%.3u] [%d x %d] Bits [%u]"), pImageNdx, lImage->mImageNum, lImage->mImageSize.cx, lImage->mImageSize.cy, lImage->mBitsSize);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
