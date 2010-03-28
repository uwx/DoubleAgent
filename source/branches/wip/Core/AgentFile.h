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
#ifndef AGENTFILE_H_INCLUDED_
#define AGENTFILE_H_INCLUDED_
#pragma once

#include "DaCoreExp.h"
#include "StringMap.h"

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4251 4275)

struct _DACORE_IMPEXP CAgentFileName
{
	LANGID					mLanguage;
	tBstrPtr				mName;
	tBstrPtr				mDesc1;
	tBstrPtr				mDesc2;

	CAgentFileName () {}
	~CAgentFileName () {}
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
};

struct _DACORE_IMPEXP CAgentFileBalloon
{
	BYTE					mLines;
	BYTE					mPerLine;
	COLORREF				mFgColor;
	COLORREF				mBkColor;
	COLORREF				mBrColor;
	tBstrPtr				mFontName;
	long					mFontHeight;
	USHORT					mFontWeight;
	BYTE					mFontItalic;
	BYTE					mFontUnderline;
	BYTE					mFontStrikethru;
	BYTE					mFontCharset;

	CAgentFileBalloon () {}
	~CAgentFileBalloon () {}
};

struct _DACORE_IMPEXP CAgentFileImage
{
	UINT					mImageNum;
	SIZE					mImageSize;
	DWORD					mBitsSize;
	tArrayPtr <BYTE>		mBits;

	CAgentFileImage () {}
	~CAgentFileImage () {}
};

struct _DACORE_IMPEXP CAgentFileFrameImage
{
	DWORD					mImageNdx;
	POINT					mOffset;

	CAgentFileFrameImage () {}
	~CAgentFileFrameImage () {}
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
};
typedef	CStringMap <tPtr <CAgentFileAnimation>, CAgentFileAnimation *> CAgentFileGestures;

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

class CFileDownload;

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CAgentFile : public CObject
{
protected:
	CAgentFile ();
public:
	virtual ~CAgentFile ();
	DECLARE_DYNCREATE (CAgentFile)

// Attributes
public:
	tBstrPtr GetPath () const;
	tBstrPtr GetFileName () const;

	bool IsAcsFile () const;
	bool IsAcfFile () const;

	const GUID & GetGuid () const;
	DWORD GetVersion () const;
	DWORD GetStyle () const;
	HICON GetIcon () const;
	const CAgentFileBalloon & GetBalloon () const;
	const CAgentFileTts & GetTts () const;

	const CPtrTypeArray <CAgentFileName> & GetNames () const;
	const CStringMap <CStringArray> & GetStates () const;
	const CAgentFileGestures & GetGestures () const;

	CSize GetImageSize () const;
	SHORT GetPaletteSize () const;
	LPCOLORREF GetPalette () const;
	BYTE GetTransparency () const;

// Operations
public:
	static bool IsProperFilePath (LPCTSTR pPath);
	static bool IsRelativeFilePath (LPCTSTR pPath);
	static CString ParseFilePath (LPCTSTR pPath);
	CString ParseRelativePath (LPCTSTR pRelativePath);

	HRESULT Open (LPCTSTR pPath, UINT pLogLevel = 15);
	void Close ();

	void SetDownloadMode (bool pRefresh = true, bool pReload = false, bool pSecure = true);
	HRESULT LoadAcf (CFileDownload * pDownload, UINT pLogLevel = 15);

	bool ReadNames (bool pFirstLetterCaps = true, UINT pLogLevel = 15);
	bool ReadStates (UINT pLogLevel = 15);
	bool ReadGestures (UINT pLogLevel = 15);
	bool ReadImageIndex (UINT pLogLevel = 15);
	bool ReadSoundIndex (UINT pLogLevel = 15);

	void FreeNames ();
	void FreeStates ();
	void FreeGestures ();
	void FreeImageIndex ();
	void FreeSoundIndex ();

	CAgentFileName * FindName (WORD pLangID = LANG_USER_DEFAULT);
	INT_PTR FindState (LPCTSTR pStateName);
	INT_PTR FindGesture (LPCTSTR pGestureName);
	INT_PTR FindAnimation (LPCTSTR pAnimationName);

	const CAgentFileAnimation * GetGesture (INT_PTR pGestureNdx);
	const CAgentFileAnimation * GetAnimation (INT_PTR pAnimationNdx);
	const CAgentFileAnimation * GetGesture (LPCTSTR pGestureName);
	const CAgentFileAnimation * GetAnimation (LPCTSTR pAnimationName);

	bool IsAnimationLoaded (INT_PTR pAnimationNdx);
	bool IsAnimationLoaded (LPCTSTR pAnimationName);
	HRESULT LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload * pDownload);
	HRESULT LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload * pDownload);
	tBstrPtr GetAnimationAcaPath (INT_PTR pAnimationNdx);
	tBstrPtr GetAnimationAcaPath (LPCTSTR pAnimationName);

	INT_PTR GetImageCount () const;
	CAgentFileImage * GetImage (INT_PTR pImageNdx, bool p32Bit = false, UINT pLogLevel = 15);
	UINT GetImageFormat (LPBITMAPINFO pImageInfo, const CAgentFileImage * pImage = NULL, bool p32Bit = false) const;
	UINT GetImageBits (LPBYTE pImageBits, const CAgentFileImage * pImage, bool p32Bit = false) const;
	UINT GetFrameBits (LPBYTE pImageBits, const CAgentFileFrame & pFrame, bool p32Bit = false, const COLORREF * pBkColor = NULL, SHORT pOverlayNdx = -1);

	INT_PTR GetSoundCount () const;
	long GetSoundSize (INT_PTR pSoundNdx);
	LPCVOID GetSound (INT_PTR pSoundNdx);

// Implementation
public:
	void Log (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogNames (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogStates (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	void LogGestures (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	static void LogTts (const CAgentFileTts & pTts, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	static void LogBalloon (const CAgentFileBalloon & pBalloon, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);

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

	CString GetAcaPath (CAgentFileAnimation * pAnimation);
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
	CFileHandle						mFileHandle;
	CGenericHandle					mFileMapping;
	CMappedHandle					mFileView;
	tPtr <CFileDownload>			mFileDownload;
	DWORD							mFileSize;
	DWORD							mFileNamesOffset;
	DWORD							mFileStatesOffset;
	CString							mPath;
	DWORD							mSignature;
	WORD							mVersionMajor;
	WORD							mVersionMinor;
	GUID							mGuid;
	CSize							mImageSize;
	BYTE							mTransparency;
	DWORD							mStyle;
	tS <CAgentFileTts>				mTts;
	tS <CAgentFileBalloon>			mBalloon;
	HICON							mIcon;
	tArrayPtr <COLORREF>			mPalette;

	COwnPtrArray <CAgentFileName>	mNames;
	CStringMap <CStringArray>		mStates;
	CAgentFileGestures				mGestures;
	CStructArray <ULARGE_INTEGER>	mImageIndex;
	CStructArray <ULARGE_INTEGER>	mSoundIndex;
	COwnPtrArray <CAgentFileImage>	mAcaImages;
	COwnPtrArray <CByteArray>		mAcaSounds;

// new and delete for DLL object
public:
#pragma auto_inline (off)
	void PASCAL operator delete (void* p) {CObject::operator delete (p);}
#ifdef	_DEBUG
	void PASCAL operator delete (void *p, LPCSTR lpszFileName, int nLine) {CObject::operator delete (p, lpszFileName, nLine);}
#endif
	void* PASCAL operator new(size_t nSize) {return CObject::operator new (nSize);}
#ifdef	_DEBUG
	void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine) {return CObject::operator new (nSize, lpszFileName, nLine);}
#endif
#pragma auto_inline ()
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // AGENTFILE_H_INCLUDED_
