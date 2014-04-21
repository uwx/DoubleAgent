/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
class CAgentFileV15Reader;

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
#define CAgentFileV15 V15File
public ref class V15File : public CAgentFile
#else
class CAgentFileV15 : public CAgentFile
#endif
{
protected:
	CAgentFileV15 ();
public:
	_DACORE_IMPEXP virtual ~CAgentFileV15 ();
#ifdef	__cplusplus_cli
	static CAgentFileV15^ CreateInstance ();
#else
	DECLARE_DLL_OBJECT_EX(CAgentFileV15, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFileV15* CreateInstance ();
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
	virtual property bool IsOpen {virtual bool get() override;}
#else
	_DACORE_IMPEXP virtual bool get_IsAcsFile () const;
	_DACORE_IMPEXP virtual bool get_IsOpen () const;
#endif

#ifndef	__cplusplus_cli
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetGesture (INT_PTR pGestureNdx);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetAnimation (INT_PTR pAnimationNdx);
#endif	

#ifdef	__cplusplus_cli
	virtual bool Open (const System::String^ pPath) override;
	virtual bool Save (const System::String^ pPath, CAgentFile^ pSource) override;
	virtual void Close () override;
#else
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath);
	_DACORE_IMPEXP virtual void Close ();
#endif

#ifdef	__cplusplus_cli
	virtual property int ImageCount {virtual int get() override;}
	virtual CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor) override;
	//virtual System::Drawing::Bitmap^ GetImageBitmap (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor) override;
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

// Implementation
protected:
#ifdef	__cplusplus_cli
	bool ReadHeaderStream ();
#else	
	HRESULT ReadHeaderStream ();
#endif	

	LPCVOID ReadBufferHeader (LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferIdentity (LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferTts (LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferBalloon (LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferPalette (LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferAnimations (LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferStates (LPCVOID pBuffer, DWORD pBufferSize);
#ifdef	__cplusplus_cli
	LPCVOID ReadBufferSounds (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferImages (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferFrames (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize, Int32 pFirstImage, Int32 pFirstSound);
#else
	LPCVOID ReadBufferSounds (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferImages (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferFrames (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize, long pFirstImage, long pFirstSound);
#endif	

#ifdef	__cplusplus_cli
	ref class V15FileAnimation^ GetV15Animation (int pAnimationNdx);
internal:	
	bool ReadAnimationStream (ref class V15FileAnimation^ pAnimation);
#else
	HRESULT ReadAnimationStream (CAgentFileAnimation* pAnimation);
#endif

protected:
#ifdef	__cplusplus_cli
	CAgentFileV15Reader*									mFileReader; 
	System::Collections::Generic::List <CAgentFileImage^>^	mStreamImages;
	System::Collections::Generic::List <array <BYTE>^>^		mStreamSounds;
#else
	tPtr <CAgentFileV15Reader>								mFileReader; 
	CAtlOwnPtrArray <CAgentFileImage>						mStreamImages;
	CAtlOwnPtrArray <CAtlByteArray>							mStreamSounds;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
