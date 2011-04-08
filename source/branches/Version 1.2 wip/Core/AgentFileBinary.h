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
#include "AgentFile.h"

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
namespace Character {
#else
#pragma warning (push)
#pragma warning (disable:4251)
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
#define CAgentFileBinary BinaryFile
public ref class CAgentFileBinary abstract : public CAgentFile
#else
class CAgentFileBinary : public CAgentFile
#endif
{
protected:
	CAgentFileBinary ();
public:
	_DACORE_IMPEXP virtual ~CAgentFileBinary ();
#ifdef	_M_CEE
	static CAgentFileBinary^ CreateInstance (const System::String^ pPath);
#else
	DECLARE_DLL_OBJECT_EX(CAgentFileBinary, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFileBinary* CreateInstance (LPCTSTR pPath);
#endif

// Overrides
public:
#ifdef	_M_CEE
	virtual property bool IsOpen {virtual bool get() override;}
	virtual property bool IsReadOnly {virtual bool get() override;}
#else
	_DACORE_IMPEXP virtual bool get_IsOpen () const;
	_DACORE_IMPEXP virtual bool get_IsReadOnly () const;
#endif

#ifdef	_M_CEE
	virtual CAgentFileName^ FindName (WORD pLangID) override;
#else
	_DACORE_IMPEXP virtual CAgentFileName* FindName (WORD pLangID = LANG_USER_DEFAULT);
	_DACORE_IMPEXP virtual INT_PTR FindState (LPCTSTR pStateName);
	_DACORE_IMPEXP virtual INT_PTR FindGesture (LPCTSTR pGestureName);
	_DACORE_IMPEXP virtual INT_PTR FindAnimation (LPCTSTR pAnimationName);

	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetGesture (INT_PTR pGestureNdx);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetAnimation (INT_PTR pAnimationNdx);
#endif

#ifdef	_M_CEE
	virtual void Close () override;
#else
	_DACORE_IMPEXP virtual void Close ();
#endif

// Implementation
protected:
#ifdef	_M_CEE
	virtual void CloseFile ();
	virtual bool LoadFile (System::String^, UINT pLogLevel);
	bool ReadHeader ();
	virtual bool ReadHeader (UINT pLogLevel);
#else
	virtual void CloseFile ();
	virtual HRESULT LoadFile (LPCTSTR pPath, UINT pLogLevel = 15);
	HRESULT ReadHeader ();
	virtual HRESULT ReadHeader (UINT pLogLevel);
#endif

	LPCVOID ReadBufferNames (LPCVOID pBuffer, DWORD pBufferSize, bool pNullTerminated, bool pFirstLetterCaps, UINT pLogLevel);
	LPCVOID ReadBufferStates (LPCVOID pBuffer, DWORD pBufferSize, bool pNullTerminated, UINT pLogLevel);
	LPCVOID ReadBufferTts (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel);
	LPCVOID ReadBufferBalloon (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel);
	LPCVOID ReadBufferPalette (LPCVOID pBuffer, UINT pLogLevel);
	LPCVOID ReadBufferIcon (LPCVOID pBuffer, UINT pLogLevel);

#ifdef	_M_CEE
	LPVOID WriteBufferNames (LPVOID pBuffer, CAgentFileNames^ pNames, bool pNullTerminated, UINT pLogLevel);
	LPVOID WriteBufferStates (LPVOID pBuffer, CAgentFileStates^ pStates, bool pNullTerminated, UINT pLogLevel);
	LPVOID WriteBufferTts (LPVOID pBuffer, CAgentFileTts^ pTts, bool pNullTerminated, UINT pLogLevel);
	LPVOID WriteBufferBalloon (LPVOID pBuffer, CAgentFileBalloon^ pBalloon, bool pNullTerminated, UINT pLogLevel);
	LPVOID WriteBufferPalette (LPVOID pBuffer, CAgentFileHeader^ pHeader, UINT pLogLevel);
	LPVOID WriteBufferIcon (LPVOID pBuffer, CAgentFileHeader^ pHeader, UINT pLogLevel);
#endif

#ifdef	_M_CEE
	static LPCVOID ReadBufferString (LPCVOID pBuffer, bool pNullTerminated, System::String^% pString);
	static LPVOID WriteBufferString (LPVOID pBuffer, bool pNullTerminated, System::String^ pString);
#else
	static LPCVOID ReadBufferString (LPCVOID pBuffer, bool pNullTerminated, CAtlString& pString);
	static LPCVOID ReadBufferString (LPCVOID pBuffer, bool pNullTerminated, tBstrPtr& pString);
#endif

	bool DecodeImage (LPCVOID pSrcBits, ULONG pSrcCount, LPBYTE pTrgBits, ULONG pTrgCount, long pTrgWidth, long pTrgHeight);
	ULONG DecodeData (LPCVOID pSrcData, ULONG pSrcSize, LPVOID pTrgData, ULONG pTrgSize);

protected:
#ifndef	_M_CEE
	void DumpBlocks (UINT pLogLevel, UINT pMaxBlockSize=512);
	void DumpPalette (LPVOID pPalette);
	void SaveImage (CAgentFileImage* pImage);
#endif

protected:
#ifdef	_M_CEE
	System::IO::FileStream^						mFileStream;
	System::IO::BinaryReader^					mFileReader;
	System::IO::BinaryWriter^					mFileWriter;
	UInt32										mFileSize;
#else
	CFileHandle									mFileHandle;
	CGenericHandle								mFileMapping;
	CMappedHandle								mFileView;
	DWORD										mFileSize;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
