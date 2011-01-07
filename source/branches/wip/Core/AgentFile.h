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
#pragma once
#include "DaCoreExp.h"
#include "AtlCollEx.h"
#include "FileDownload.h"

//////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable:4251)
//////////////////////////////////////////////////////////////////////

struct _DACORE_IMPEXP CAgentFileName
{
	LANGID					mLanguage;
	tBstrPtr				mName;
	tBstrPtr				mDesc1;
	tBstrPtr				mDesc2;

	CAgentFileName () {}
	~CAgentFileName () {}
	DECLARE_DLL_OBJECT(CAgentFileName)
};

struct _DACORE_IMPEXP CAgentFileTts
{
	GUID					mEngine;
	GUID					mMode;
	tBstrPtr				mModeId;
	USHORT					mGender;
	USHORT					mAge;
	long					mSpeed;
	short					mPitch;
	LANGID					mLanguage;
	tBstrPtr				mStyle;

	CAgentFileTts () {}
	~CAgentFileTts () {}
	DECLARE_DLL_OBJECT(CAgentFileTts)
};

struct _DACORE_IMPEXP CAgentFileBalloon
{
	BYTE					mLines;
	BYTE					mPerLine;
	COLORREF				mFgColor;
	COLORREF				mBkColor;
	COLORREF				mBrColor;
	tS <LOGFONT>			mFont;

	CAgentFileBalloon () {}
	~CAgentFileBalloon () {}
	DECLARE_DLL_OBJECT(CAgentFileBalloon)
};

struct _DACORE_IMPEXP CAgentFileImage
{
	UINT					mImageNum;
	SIZE					mImageSize;
	DWORD					mBitsSize;
	tArrayPtr <BYTE>		mBits;

	CAgentFileImage () {}
	~CAgentFileImage () {}
	DECLARE_DLL_OBJECT(CAgentFileImage)
};

struct _DACORE_IMPEXP CAgentFileFrameImage
{
	DWORD					mImageNdx;
	POINT					mOffset;

	CAgentFileFrameImage () {}
	~CAgentFileFrameImage () {}
	DECLARE_DLL_OBJECT(CAgentFileFrameImage)
};
typedef tArrayPtr <CAgentFileFrameImage> CAgentFileFrameImages;

struct _DACORE_IMPEXP CAgentFileFrameOverlay
{
	WORD					mOverlayNdx;
	DWORD					mImageNdx;
	BYTE					mReplaceFlag;
	POINT					mOffset;
	POINT					mSomething;

	CAgentFileFrameOverlay () {}
	~CAgentFileFrameOverlay () {}
	DECLARE_DLL_OBJECT(CAgentFileFrameOverlay)
};
typedef tArrayPtr <CAgentFileFrameOverlay> CAgentFileFrameOverlays;

struct _DACORE_IMPEXP CAgentFileFrame
{
	WORD					mDuration;
	WORD					mSoundNdx;
	DWORD					mBranching [3];
	WORD					mExitFrame;
	WORD					mImageCount;
	CAgentFileFrameImages	mImages;
	WORD					mOverlayCount;
	CAgentFileFrameOverlays	mOverlays;

	CAgentFileFrame () {}
	~CAgentFileFrame () {}
	DECLARE_DLL_OBJECT(CAgentFileFrame)
};
typedef	tArrayPtr <CAgentFileFrame> CAgentFileFrames;

struct _DACORE_IMPEXP CAgentFileAnimation
{
	tBstrPtr				mName;
	tBstrPtr				mReturnName;
	tBstrPtr				mAcaFileName;
	DWORD					mAcaChksum;
	BYTE					mReturnType;
	WORD					mFrameCount;
	CAgentFileFrames		mFrames;

	CAgentFileAnimation () {}
	~CAgentFileAnimation () {}
	DECLARE_DLL_OBJECT(CAgentFileAnimation)
};

//////////////////////////////////////////////////////////////////////

struct _DACORE_IMPEXP CAgentFileGestures
{
	CAtlOwnPtrArray <CAgentFileAnimation>	mAnimations;
	CAtlStringArray							mNames;

	CAgentFileAnimation * operator() (const CAtlString & pName);
	const CAgentFileAnimation * operator() (const CAtlString & pName) const;
	DECLARE_DLL_OBJECT(CAgentFileGestures)
};

struct _DACORE_IMPEXP CAgentFileStates
{
	CAtlClassArray <CAtlStringArray>	mGestures;
	CAtlStringArray						mNames;

	CAtlStringArray * operator() (const CAtlString & pName);
	const CAtlStringArray * operator() (const CAtlString & pName) const;
	DECLARE_DLL_OBJECT(CAgentFileStates)
};

//////////////////////////////////////////////////////////////////////

enum AgentCharStyle
{
	CharStyleTts = 0x00000020,
	CharStyleBalloon = 0x00000200,
	CharStyleSizeToText = 0x00010000,
	CharStyleNoAutoHide = 0x00020000,
	CharStyleNoAutoPace = 0x00040000,
	CharStyleStandard = 0x00100000
};

enum AgentMouthOverlay
{
	MouthOverlayClosed = 0,
	MouthOverlayWide1 = 1,
	MouthOverlayWide2 = 2,
	MouthOverlayWide3 = 3,
	MouthOverlayWide4 = 4,
	MouthOverlayMedium = 5,
	MouthOverlayNarrow = 6
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class CAgentFile
{
	DECLARE_DLL_OBJECT_EX(CAgentFile, _DACORE_IMPEXP)
protected:
	CAgentFile ();
public:
	_DACORE_IMPEXP virtual ~CAgentFile ();
	_DACORE_IMPEXP static CAgentFile * CreateInstance ();

// Attributes
public:
	_DACORE_IMPEXP tBstrPtr GetPath () const;
	_DACORE_IMPEXP tBstrPtr GetFileName () const;

	_DACORE_IMPEXP bool IsAcsFile () const;
	_DACORE_IMPEXP bool IsAcfFile () const;

	_DACORE_IMPEXP const GUID & GetGuid () const;
	_DACORE_IMPEXP DWORD GetVersion () const;
	_DACORE_IMPEXP DWORD GetStyle () const;
	_DACORE_IMPEXP HICON GetIcon () const;
	_DACORE_IMPEXP const CAgentFileBalloon & GetBalloon () const;
	_DACORE_IMPEXP const CAgentFileTts & GetTts () const;

	_DACORE_IMPEXP const CAtlPtrTypeArray <CAgentFileName> & GetNames () const;
	_DACORE_IMPEXP const CAgentFileStates & GetStates () const;
	_DACORE_IMPEXP const CAgentFileGestures & GetGestures () const;

	_DACORE_IMPEXP SAFEARRAY * GetStateNames ();
	_DACORE_IMPEXP SAFEARRAY * GetGestureNames ();
	_DACORE_IMPEXP SAFEARRAY * GetAnimationNames ();

	_DACORE_IMPEXP CSize GetImageSize () const;
	_DACORE_IMPEXP SHORT GetPaletteSize () const;
	_DACORE_IMPEXP LPCOLORREF GetPalette () const;
	_DACORE_IMPEXP BYTE GetTransparency () const;

// Operations
public:
	_DACORE_IMPEXP static bool IsProperFilePath (LPCTSTR pPath);
	_DACORE_IMPEXP static bool IsRelativeFilePath (LPCTSTR pPath);
	_DACORE_IMPEXP static tBstrPtr ParseFilePath (LPCTSTR pPath);
	_DACORE_IMPEXP tBstrPtr ParseRelativePath (LPCTSTR pRelativePath);

	_DACORE_IMPEXP HRESULT Open (LPCTSTR pPath, UINT pLogLevel = 15);
	_DACORE_IMPEXP void Close ();

	_DACORE_IMPEXP void SetDownloadMode (bool pRefresh = true, bool pReload = false, bool pSecure = true);
	_DACORE_IMPEXP HRESULT LoadAcf (CFileDownload * pDownload, UINT pLogLevel = 15);

	_DACORE_IMPEXP bool ReadNames (bool pFirstLetterCaps = true, UINT pLogLevel = 15);
	_DACORE_IMPEXP bool ReadStates (UINT pLogLevel = 15);
	_DACORE_IMPEXP bool ReadGestures (UINT pLogLevel = 15);
	_DACORE_IMPEXP bool ReadImageIndex (UINT pLogLevel = 15);
	_DACORE_IMPEXP bool ReadSoundIndex (UINT pLogLevel = 15);

	_DACORE_IMPEXP void FreeNames ();
	_DACORE_IMPEXP void FreeStates ();
	_DACORE_IMPEXP void FreeGestures ();
	_DACORE_IMPEXP void FreeImageIndex ();
	_DACORE_IMPEXP void FreeSoundIndex ();

	_DACORE_IMPEXP CAgentFileName * FindName (WORD pLangID = LANG_USER_DEFAULT);
	_DACORE_IMPEXP INT_PTR FindState (LPCTSTR pStateName);
	_DACORE_IMPEXP INT_PTR FindGesture (LPCTSTR pGestureName);
	_DACORE_IMPEXP INT_PTR FindAnimation (LPCTSTR pAnimationName);

	_DACORE_IMPEXP const CAgentFileAnimation * GetGesture (INT_PTR pGestureNdx);
	_DACORE_IMPEXP const CAgentFileAnimation * GetAnimation (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP const CAgentFileAnimation * GetGesture (LPCTSTR pGestureName);
	_DACORE_IMPEXP const CAgentFileAnimation * GetAnimation (LPCTSTR pAnimationName);

	_DACORE_IMPEXP bool IsAnimationLoaded (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP bool IsAnimationLoaded (LPCTSTR pAnimationName);
	_DACORE_IMPEXP HRESULT LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload * pDownload);
	_DACORE_IMPEXP HRESULT LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload * pDownload);
	_DACORE_IMPEXP tBstrPtr GetAnimationAcaPath (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP tBstrPtr GetAnimationAcaPath (LPCTSTR pAnimationName);

	_DACORE_IMPEXP INT_PTR GetImageCount () const;
	_DACORE_IMPEXP CAgentFileImage * GetImage (INT_PTR pImageNdx, bool p32Bit = false, UINT pLogLevel = 15);
	_DACORE_IMPEXP UINT GetImageFormat (LPBITMAPINFO pImageInfo, const CAgentFileImage * pImage = NULL, bool p32Bit = false) const;
	_DACORE_IMPEXP UINT GetImageBits (LPBYTE pImageBits, const CAgentFileImage * pImage, bool p32Bit = false) const;
	_DACORE_IMPEXP UINT GetFrameBits (LPBYTE pImageBits, const CAgentFileFrame & pFrame, bool p32Bit = false, const COLORREF * pBkColor = NULL, SHORT pOverlayNdx = -1);

	_DACORE_IMPEXP INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP LPCVOID GetSound (INT_PTR pSoundNdx);

// Implementation
public:
	_DACORE_IMPEXP void Log (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP void LogNames (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP void LogStates (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP void LogGestures (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP static void LogTts (const CAgentFileTts & pTts, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP static void LogBalloon (const CAgentFileBalloon & pBalloon, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);

protected:
	HRESULT LoadFile (LPCTSTR pPath, UINT pLogLevel = 15);
	HRESULT ReadHeader (UINT pLogLevel = 15);
	bool ReadAcsHeader (UINT pLogLevel = 15);
	bool ReadAcfHeader (UINT pLogLevel = 15);
	LPCVOID ReadBufferNames (LPCVOID pBuffer, bool pNullTerminated, bool pFirstLetterCaps = true, UINT pLogLevel = 15);
	LPCVOID ReadBufferStates (LPCVOID pBuffer, bool pNullTerminated, LPCVOID pBufferEnd = NULL, UINT pLogLevel = 15);
	LPCVOID ReadBufferTts (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel = 15);
	LPCVOID ReadBufferBalloon (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel = 15);
	LPCVOID ReadBufferPalette (LPCVOID pBuffer, UINT pLogLevel = 15);
	LPCVOID ReadBufferIcon (LPCVOID pBuffer, UINT pLogLevel = 15);

	CAgentFileAnimation * ReadAcsAnimation (DWORD pOffset, DWORD pSize, UINT pLogLevel = 15);
	CAgentFileImage * ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit = false, UINT pLogLevel = 15);
	LPCVOID ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum, UINT pLogLevel = 15);

	CAtlString GetAcaPath (CAgentFileAnimation * pAnimation);
	HRESULT ReadAcaFile (CAgentFileAnimation * pAnimation, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaFile (CAgentFileAnimation * pAnimation, LPCTSTR pPath, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaFrames (CAgentFileAnimation * pAnimation, LPCVOID & pBuffer, DWORD & pBufferSize, WORD pImageStart, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaImages (CAgentFileAnimation * pAnimation, LPCVOID & pBuffer, DWORD & pBufferSize, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaSounds (CAgentFileAnimation * pAnimation, LPCVOID & pBuffer, DWORD & pBufferSize, bool p32Bit = false, UINT pLogLevel = 15);

	bool DecodeImage (LPCVOID pSrcBits, ULONG pSrcCount, LPBYTE pTrgBits, ULONG pTrgCount, const CSize & pTrgSize);
	ULONG DecodeData (LPCVOID pSrcData, ULONG pSrcSize, LPVOID pTrgData, ULONG pTrgSize);

protected:
	void DumpBlocks (UINT pLogLevel, UINT pMaxBlockSize=512);
	void DumpPalette (LPVOID pPalette);
	void DumpAcsImages (UINT pLogLevel);
	void DumpAcsImage (INT_PTR pImageNdx, UINT pLogLevel);
	void DumpAcaImages (UINT pLogLevel);
	void DumpAcaImage (INT_PTR pImageNdx, UINT pLogLevel);
	void SaveImage (CAgentFileImage * pImage);

protected:
	CFileHandle							mFileHandle;
	CGenericHandle						mFileMapping;
	CMappedHandle						mFileView;
	tPtr <CFileDownload>				mFileDownload;
	DWORD								mFileSize;
	DWORD								mFileNamesOffset;
	DWORD								mFileStatesOffset;
	CAtlString							mPath;
	DWORD								mSignature;
	WORD								mVersionMajor;
	WORD								mVersionMinor;
	GUID								mGuid;
	CSize								mImageSize;
	BYTE								mTransparency;
	DWORD								mStyle;
	tS <CAgentFileTts>					mTts;
	tS <CAgentFileBalloon>				mBalloon;
	HICON								mIcon;
	tArrayPtr <COLORREF>				mPalette;

	CAtlOwnPtrArray <CAgentFileName>	mNames;
	CAgentFileStates					mStates;
	CAgentFileGestures					mGestures;
	CAtlStructArray <ULARGE_INTEGER>	mImageIndex;
	CAtlStructArray <ULARGE_INTEGER>	mSoundIndex;
	CAtlOwnPtrArray <CAgentFileImage>	mAcaImages;
	CAtlOwnPtrArray <CAtlByteArray>		mAcaSounds;
};

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////
