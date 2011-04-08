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
#include "AgentFileBinary.h"

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
#define CAgentFileAcs AcsFile
public ref class CAgentFileAcs : public CAgentFileBinary
#else
class CAgentFileAcs : public CAgentFileBinary
#endif
{
public:
	CAgentFileAcs ();
	_DACORE_IMPEXP virtual ~CAgentFileAcs ();
#ifdef	_M_CEE
	static CAgentFileAcs^ CreateInstance ();
#else
	DECLARE_DLL_OBJECT_EX(CAgentFileAcs, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFileAcs* CreateInstance ();
#endif

// Overrides
public:
#ifdef	_M_CEE
	virtual property bool IsAcsFile {virtual bool get() override;}
	virtual bool Open (const System::String^ pPath, UINT pLogLevel) override;
	virtual bool Save (const System::String^ pPath, CAgentFile^ pSource, UINT pLogLevel) override;
	virtual void Close () override;
#else
	_DACORE_IMPEXP virtual bool get_IsAcsFile () const;
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath, UINT pLogLevel = 15);
	_DACORE_IMPEXP virtual void Close ();
#endif

#ifdef	_M_CEE
	virtual property int ImageCount {virtual int get() override;}
	virtual CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor, UINT pLogLevel) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetImageCount () const;
	_DACORE_IMPEXP virtual CAgentFileImage* GetImage (INT_PTR pImageNdx, bool p32Bit = false, const COLORREF* pBkColor = NULL, UINT pLogLevel = 15);
#endif

#ifdef	_M_CEE
	virtual property int SoundCount {virtual int get() override;}
	virtual int GetSoundSize (int pSoundNdx) override;
	virtual array <BYTE>^ GetSound (int pSoundNdx) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP virtual long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP virtual LPCVOID GetSound (INT_PTR pSoundNdx);
#endif

protected:
#ifdef	_M_CEE
	virtual bool LoadFile (System::String^, UINT pLogLevel) override;
	virtual bool ReadHeader (UINT pLogLevel) override;
	virtual bool ReadNames (bool pFirstLetterCaps, UINT pLogLevel) override;
	virtual bool ReadStates (UINT pLogLevel) override;
	virtual bool ReadGestures (UINT pLogLevel) override;
#else
	virtual HRESULT LoadFile (LPCTSTR pPath, UINT pLogLevel = 15);
	virtual HRESULT ReadHeader (UINT pLogLevel);
	virtual bool ReadNames (bool pFirstLetterCaps, UINT pLogLevel);
	virtual bool ReadStates (UINT pLogLevel);
	virtual bool ReadGestures (UINT pLogLevel);
#endif

// Implementation
protected:
	bool ReadAcsHeader ();
	bool ReadAcsHeader (UINT pLogLevel);
	LPCVOID ReadBufferHeader (LPCVOID pBuffer, UINT pLogLevel);
#ifdef	_M_CEE
	CAgentFileAnimation^ ReadAcsAnimation (DWORD pOffset, DWORD pSize, UINT pLogLevel);
	CAgentFileImage^ ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit, System::Drawing::Color pBkColor, UINT pLogLevel);
	array <BYTE>^ ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum, UINT pLogLevel);
#else
	CAgentFileAnimation* ReadAcsAnimation (DWORD pOffset, DWORD pSize, UINT pLogLevel = 15);
	CAgentFileImage* ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit = false, const COLORREF* pBkColor = NULL, UINT pLogLevel = 15);
	LPCVOID ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum, UINT pLogLevel = 15);
#endif

	bool ReadImageIndex ();
	bool ReadImageIndex (UINT pLogLevel);
	void FreeImageIndex ();

	bool ReadSoundIndex ();
	bool ReadSoundIndex (UINT pLogLevel);
	void FreeSoundIndex ();

#ifdef	_M_CEE
	DWORD WriteAcsHeader (DWORD pFileOffset, CAgentFile^ pSource);
	DWORD WriteAcsHeader (DWORD pFileOffset, CAgentFile^ pSource, UINT pLogLevel);
	LPVOID WriteBufferHeader (LPVOID pBuffer, CAgentFileHeader^ pHeader, UINT pLogLevel);
	DWORD WriteImageIndex (DWORD pFileOffset);
	DWORD WriteImageIndex (DWORD pFileOffset, UINT pLogLevel);
	DWORD WriteSoundIndex (DWORD pFileOffset);
	DWORD WriteSoundIndex (DWORD pFileOffset, UINT pLogLevel);

	DWORD WriteGestures (DWORD pFileOffset, CAgentFile^ pSource);
	DWORD WriteGestures (DWORD pFileOffset, CAgentFile^ pSource, UINT pLogLevel);
	LPVOID WriteAcsAnimation (LPVOID pBuffer, CAgentFileAnimation^ pAnimation, UINT pLogLevel);

	DWORD WriteImages (DWORD pFileOffset, CAgentFile^ pSource);
	DWORD WriteImages (DWORD pFileOffset, CAgentFile^ pSource, UINT pLogLevel);
	LPVOID WriteAcsImage (LPVOID pBuffer, CAgentFileImage^ pImage, UINT pLogLevel);

	DWORD WriteSounds (DWORD pFileOffset, CAgentFile^ pSource);
	DWORD WriteSounds (DWORD pFileOffset, CAgentFile^ pSource, UINT pLogLevel);
	LPVOID WriteAcsSound (LPVOID pBuffer, array <BYTE>^ pSound, UINT pLogLevel);
#endif

protected:
#ifndef	_M_CEE
	void DumpAcsImages (UINT pLogLevel);
	void DumpAcsImage (INT_PTR pImageNdx, UINT pLogLevel);
#endif

protected:
#ifdef	_M_CEE
	UInt32										mFileNamesOffset;
	UInt32										mFileNamesSize;
	UInt32										mFileStatesOffset;
	UInt32										mFileStatesSize;
	array <KeyValuePair <UInt32, UInt32> >^		mImageIndex;
	array <KeyValuePair <UInt32, UInt32> >^		mSoundIndex;
#else
	DWORD										mFileNamesOffset;
	DWORD										mFileNamesSize;
	DWORD										mFileStatesOffset;
	DWORD										mFileStatesSize;
	CAtlStructArray <ULARGE_INTEGER>			mImageIndex;
	CAtlStructArray <ULARGE_INTEGER>			mSoundIndex;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
