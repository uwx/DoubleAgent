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
ref class ScriptReader;
ref class ScriptWriter;
#define CAgentFileScript ScriptFile
public ref class CAgentFileScript : public CAgentFile
#else
class CAgentFileScript : public CAgentFile
#endif
{
protected:
	CAgentFileScript ();
public:
	_DACORE_IMPEXP virtual ~CAgentFileScript ();
#ifdef	_M_CEE
	static CAgentFileScript^ CreateInstance ();
#else
	DECLARE_DLL_OBJECT_EX(CAgentFileScript, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFileScript* CreateInstance ();
#endif

// Overrides
public:
#ifdef	_M_CEE
	virtual property bool IsAcdFile {virtual bool get() override;}
	virtual property bool IsOpen {virtual bool get() override;}
	virtual property bool IsReadOnly {virtual bool get() override;}
	virtual property bool IsDirty {virtual bool get() override; virtual void set (bool pIsDirty) override;}
#else
	_DACORE_IMPEXP virtual bool IsAcdFile () const;
	_DACORE_IMPEXP virtual bool IsOpen () const;
	_DACORE_IMPEXP virtual bool IsReadOnly () const;
#endif

#ifdef	_M_CEE
	virtual bool Open (const System::String^ pPath) override;
	virtual bool Save (const System::String^ pPath, CAgentFile^ pSource) override;
	virtual void Close () override;
#else
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath);
	_DACORE_IMPEXP virtual void Close ();
#endif

#ifdef	_M_CEE
	virtual property int ImageCount {virtual int get() override;}
	virtual System::String^ GetImageFilePath (int pImageNdx) override;
	virtual CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor) override;
	virtual System::Drawing::Bitmap^ GetImageBitmap (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetImageCount () const;
	_DACORE_IMPEXP virtual CAgentFileImage* GetImage (INT_PTR pImageNdx, bool p32Bit = false);
#endif

#ifdef	_M_CEE
	virtual property int SoundCount {virtual int get() override;}
	virtual int GetSoundSize (int pSoundNdx) override;
	virtual array <BYTE>^ GetSound (int pSoundNdx) override;
	virtual System::String^ GetSoundFilePath (int pSoundNdx) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP virtual long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP virtual LPCVOID GetSound (INT_PTR pSoundNdx);
#endif

#ifdef	_M_CEE
	virtual property System::String^ IconFilePath {virtual System::String^ get() override; virtual void set (System::String^ pValue) override;}
	virtual property System::String^ PaletteFilePath {virtual System::String^ get() override; virtual void set (System::String^ pValue) override;}
	virtual property UInt16 NewFrameDuration {virtual UInt16 get() override; virtual void set (UInt16 pValue) override;}
#endif

protected:
#ifdef	_M_CEE
	virtual void FreeNames () override;
	virtual void FreeStates () override;
	virtual void FreeGestures () override;
internal:
	virtual bool RenameAnimation (CAgentFileAnimation^ pAnimation, System::String^ pNewName) override;
	virtual Int32 LoadImageFile (System::String^ pImageFilePath) override;
	virtual Int32 LoadSoundFile (System::String^ pSoundFilePath) override;
#endif

// Implementation
protected:
#ifdef	_M_CEE
	bool ParseFile (ScriptReader^ pReader);
	bool ParseHeader (ScriptReader^ pReader);
	UInt32 ParseStyle (ScriptReader^ pReader, System::String^ pStyle);
	bool ParseTtsLine (ScriptReader^ pReader, System::String^ pToken, System::String^ pLine);
	bool ParseBalloon (ScriptReader^ pReader);
	bool ParseName (ScriptReader^ pReader, LANGID pLangID);
	bool ParseAnimation (ScriptReader^ pReader, System::String^ pAnimationName);
	bool ParseAnimationFrame (ScriptReader^ pReader, CAgentFileAnimation^ pAnimation);
	bool ParseFrameBranching (ScriptReader^ pReader, CAgentFileFrame^ pFrame);
	bool ParseFrameImage (ScriptReader^ pReader, CAgentFileFrame^ pFrame);
	bool ParseFrameOverlay (ScriptReader^ pReader, CAgentFileFrame^ pFrame);
	bool ParseState (ScriptReader^ pReader, System::String^ pStateName);

	bool WriteFile (ScriptWriter^ pWriter, CAgentFile^ pSource);
	bool WriteHeader (ScriptWriter^ pWriter, CAgentFile^ pSource);
	bool WriteStyle (ScriptWriter^ pWriter, UInt32 pStyle);
	bool WriteTts (ScriptWriter^ pWriter, CAgentFile^ pSource);
	bool WriteBalloon (ScriptWriter^ pWriter, CAgentFile^ pSource);
	bool WriteNames (ScriptWriter^ pWriter, CAgentFile^ pSource);
	bool WriteName (ScriptWriter^ pWriter, CAgentFileName^ pName);
	bool WriteAnimations (ScriptWriter^ pWriter, CAgentFile^ pSource);
	bool WriteAnimation (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileAnimation^ pAnimation);
	bool WriteAnimationFrame (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrame^ pFrame);
	bool WriteFrameBranching (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrame^ pFrame);
	bool WriteFrameImage (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrameImage^ pFrameImage);
	bool WriteFrameOverlay (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrameOverlay^ pFrameOverlay);
	bool WriteStates (ScriptWriter^ pWriter, CAgentFile^ pSource);
	
	String^ PrepareFileFolder ();
	String^ PrepareFileFolder (Boolean pEmptyFolder);
	String^ WriteIconFile (CAgentFileHeader^ pSource);
	String^ WritePaletteFile (CAgentFileHeader^ pSource);
	String^ WriteImageFile (CAgentFile^ pSource, int pImageNdx, Boolean pForOverlay);
	String^ WriteSoundFile (CAgentFile^ pSource, int pSoundNdx);
#endif

protected:
#ifdef	_M_CEE
	bool																						mIsDirty;
	System::IO::FileStream^																		mFileStream;
	ScriptReader^																				mFileReader;
	System::Collections::Generic::List <System::String^>^										mImageFilePaths;
	System::Collections::Generic::Dictionary <System::String^, System::Drawing::Bitmap^>^		mImages;
	System::Collections::Generic::List <System::String^>^										mSoundFilePaths;
	System::Collections::Generic::Dictionary <System::String^, System::Media::SoundPlayer^>^	mSounds;
	System::String^																				mIconFilePath;
	System::String^																				mPaletteFilePath;
	UInt16																						mNewFrameDuration;
#else
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
