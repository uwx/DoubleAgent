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
#ifdef	__cplusplus_cli
namespace DoubleAgent {
namespace Character {
#else
#pragma warning (push)
#pragma warning (disable:4251)
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
#define CAgentFileAcs AcsFile
/// <summary>
///	An Agent Character definition stored in a single binary-formatted file.
/// </summary>
public ref class AcsFile : public CAgentFileBinary
#else
class CAgentFileAcs : public CAgentFileBinary
#endif
{
public:
	CAgentFileAcs ();
	_DACORE_IMPEXP virtual ~CAgentFileAcs ();
#ifdef	__cplusplus_cli
	static CAgentFileAcs^ CreateInstance ();
#else
	DECLARE_DLL_OBJECT_EX(CAgentFileAcs, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFileAcs* CreateInstance ();
#endif

// Methods
#ifdef	__cplusplus_cli
	static bool CheckFileSignature (const System::String^ pPath);	
#else
	static bool CheckFileSignature (LPCTSTR pPath);	
#endif	

// Overrides
public:
#ifdef	__cplusplus_cli
	virtual property bool IsAcsFile {virtual bool get() override;}
	virtual bool Open (const System::String^ pPath) override;
	virtual bool Save (const System::String^ pPath, CAgentFile^ pSource) override;
	virtual void Close () override;
#else
	_DACORE_IMPEXP virtual bool get_IsAcsFile () const;
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath);
	_DACORE_IMPEXP virtual void Close ();
#endif

#ifdef	__cplusplus_cli
	virtual property int ImageCount {virtual int get() override;}
	virtual CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetImageCount () const;
	_DACORE_IMPEXP virtual CAgentFileImage* GetImage (INT_PTR pImageNdx, bool p32Bit = false, const COLORREF* pBkColor = NULL);
#endif

#ifdef	__cplusplus_cli
	virtual property int SoundCount {virtual int get() override;}
	virtual int GetSoundSize (int pSoundNdx) override;
	virtual array <BYTE>^ GetSound (int pSoundNdx) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP virtual long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP virtual LPCVOID GetSound (INT_PTR pSoundNdx);
#endif

protected:
#ifdef	__cplusplus_cli
	virtual bool LoadFile (System::String^) override;
	virtual bool ReadHeader () override;
	virtual bool ReadNames (bool pFirstLetterCaps) override;
	virtual bool ReadStates () override;
	virtual bool ReadGestures () override;
#else
	virtual HRESULT LoadFile (LPCTSTR pPath);
	virtual HRESULT ReadHeader ();
	virtual bool ReadNames (bool pFirstLetterCaps);
	virtual bool ReadStates ();
	virtual bool ReadGestures ();
#endif

// Implementation
protected:
	bool ReadAcsHeader ();
	LPCVOID ReadBufferHeader (LPCVOID pBuffer);
#ifdef	__cplusplus_cli
	CAgentFileAnimation^ ReadAcsAnimation (DWORD pOffset, DWORD pSize);
	CAgentFileImage^ ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum);
	array <BYTE>^ ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum);
#else
	CAgentFileAnimation* ReadAcsAnimation (DWORD pOffset, DWORD pSize);
	CAgentFileImage* ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum);
	LPCVOID ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum);
#endif

	bool ReadImageIndex ();
	void FreeImageIndex ();

	bool ReadSoundIndex ();
	void FreeSoundIndex ();

#ifdef	__cplusplus_cli
	void FreeUnusedImages ();
	void FreeUnusedSounds ();

	DWORD WriteAcsHeader (DWORD pFileOffset, CAgentFile^ pSource);
	LPVOID WriteBufferHeader (LPVOID pBuffer, CAgentFileHeader^ pHeader);
	DWORD WriteImageIndex (DWORD pFileOffset, CAgentFile^ pSource);
	DWORD WriteSoundIndex (DWORD pFileOffset, CAgentFile^ pSource);

	DWORD WriteGestures (DWORD pFileOffset, CAgentFile^ pSource);
	LPVOID WriteAcsAnimation (LPVOID pBuffer, CAgentFileAnimation^ pAnimation);

	DWORD WriteImages (DWORD pFileOffset, CAgentFile^ pSource);
	LPVOID WriteAcsImage (LPVOID pBuffer, CAgentFileImage^ pImage);

	DWORD WriteSounds (DWORD pFileOffset, CAgentFile^ pSource);
	LPVOID WriteAcsSound (LPVOID pBuffer, array <BYTE>^ pSound);
#endif

protected:
#ifndef	__cplusplus_cli
	void DumpAcsImages (UINT pLogLevel);
	void DumpAcsImage (INT_PTR pImageNdx, UINT pLogLevel);
#endif

protected:
#ifdef	__cplusplus_cli
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
#ifdef	__cplusplus_cli
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
