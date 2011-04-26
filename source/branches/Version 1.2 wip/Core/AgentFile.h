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

#ifdef	_M_CEE
#ifndef	_DACORE_IMPEXP
#define	_DACORE_IMPEXP
#endif
#endif

#ifdef	_M_CEE
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
#else
#include "DaCoreExp.h"
#include "AtlCollEx.h"
#endif
#include "AgentFileParts.h"

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
#define CAgentFile CharacterFile
/// <summary>
///	The abstract base class for an Agent Character definition.
/// </summary>
/// <seealso cref="AcsFile"/>
/// <seealso cref="AcfFile"/>
/// <seealso cref="ScriptFile"/>
public ref class CAgentFile abstract
#else
class CAgentFile
#endif
{
protected:
	CAgentFile ();
public:
	_DACORE_IMPEXP virtual ~CAgentFile ();
#ifdef	_M_CEE
	static CAgentFile^ CreateInstance (const System::String^ pPath);
#else
	DECLARE_DLL_OBJECT_EX(CAgentFile, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFile* CreateInstance (LPCTSTR pPath);
#endif

// Attributes
public:
#ifdef	_M_CEE
	property System::String^ Path {System::String^ get();}
	property System::String^ FileName {System::String^ get();}

	virtual property bool IsOpen {virtual bool get();}
	virtual property bool IsReadOnly {virtual bool get();}
	virtual property bool IsDirty {virtual bool get(); virtual void set (bool pIsDirty);}
	virtual property bool IsAcsFile {virtual bool get();}
	virtual property bool IsAcfFile {virtual bool get();}
	virtual property bool IsAcdFile {virtual bool get();}

	property UInt32 Version {UInt32 get();}
	property FileHeader^ Header {FileHeader^ get();}
	property FileBalloon^ Balloon {FileBalloon^ get();}
	property FileTts^ Tts {FileTts^ get();}

	property UInt32 LogLevel {UInt32 get() {return (UInt32)mLogLevel;} void set (UInt32 pLogLevel) {mLogLevel = (UINT)pLogLevel;}}
	static property System::String^ AcsFileExt {System::String^ get ();}
	static property System::String^ AcfFileExt {System::String^ get ();}
	static property System::String^ AcdFileExt {System::String^ get ();}
#else
	__declspec(property(get=get_Path))		tBstrPtr	Path;
	__declspec(property(get=get_FileName))	tBstrPtr	FileName;

	_DACORE_IMPEXP tBstrPtr get_Path () const;
	_DACORE_IMPEXP tBstrPtr get_FileName () const;

	__declspec(property(get=get_IsOpen))						bool	IsOpen;
	__declspec(property(get=get_IsReadOnly))					bool	IsReadOnly;
	__declspec(property(get=get_IsDirty, put=put_IsDirty))		bool	IsDirty;
	__declspec(property(get=get_IsAcsFile))						bool	IsAcsFile;
	__declspec(property(get=get_IsAcfFile))						bool	IsAcfFile;
	__declspec(property(get=get_IsAcdFile))						bool	IsAcdFile;
	__declspec(property(get=get_LogLevel, put=put_LogLevel))	UINT	LogLevel;

	_DACORE_IMPEXP virtual bool get_IsOpen () const;
	_DACORE_IMPEXP virtual bool get_IsReadOnly () const;
	_DACORE_IMPEXP virtual bool get_IsAcsFile () const;
	_DACORE_IMPEXP virtual bool get_IsAcfFile () const;
	_DACORE_IMPEXP virtual bool get_IsAcdFile () const;

	__declspec(property(get=get_Version))	DWORD						Version;
	__declspec(property(get=get_Header))	const CAgentFileHeader&		Header;
	__declspec(property(get=get_Balloon))	const CAgentFileBalloon&	Balloon;
	__declspec(property(get=get_Tts))		const CAgentFileTts&		Tts;

	_DACORE_IMPEXP DWORD get_Version () const;
	_DACORE_IMPEXP const CAgentFileHeader& get_Header () const;
	_DACORE_IMPEXP const CAgentFileBalloon& get_Balloon () const;
	_DACORE_IMPEXP const CAgentFileTts& get_Tts () const;

	_DACORE_IMPEXP UINT get_LogLevel () const {return mLogLevel;}
	_DACORE_IMPEXP void put_LogLevel (UINT pLogLevel) {mLogLevel = pLogLevel;}
#endif

#ifdef	_M_CEE
	property CAgentFileNames^ Names {CAgentFileNames^ get();}
	property CAgentFileStates^ States {CAgentFileStates^ get();}
	property CAgentFileGestures^ Gestures {CAgentFileGestures^ get();}
#else
	__declspec(property(get=get_Names))		const CAgentFileNames&		Names;
	__declspec(property(get=get_States))	const CAgentFileStates&	States;
	__declspec(property(get=get_Gestures))	const CAgentFileGestures&	Gestures;

	_DACORE_IMPEXP const CAgentFileNames& get_Names () const;
	_DACORE_IMPEXP const CAgentFileStates& get_States () const;
	_DACORE_IMPEXP const CAgentFileGestures& get_Gestures () const;
#endif

#ifdef	_M_CEE
	virtual array <System::String^>^ GetStateNames ();
	virtual array <System::String^>^ GetGestureNames ();
	virtual array <System::String^>^ GetAnimationNames ();
#else
	_DACORE_IMPEXP virtual SAFEARRAY* GetStateNames ();
	_DACORE_IMPEXP virtual SAFEARRAY* GetGestureNames ();
	_DACORE_IMPEXP virtual SAFEARRAY* GetAnimationNames ();
#endif

#ifdef	_M_CEE
	/// <summary>
	/// Gets or sets the file that defines the character's <see cref="FileHeader.Icon"/>. 
	/// </summary>
	virtual property System::String^ IconFilePath {virtual System::String^ get(); virtual void set (System::String^ pValue);}
	/// <summary>
	/// Gets or sets the file that defines the character's <see cref="FileHeader.Palette"/>. 
	/// </summary>
	virtual property System::String^ PaletteFilePath {virtual System::String^ get(); virtual void set (System::String^ pValue);}
	/// <summary>
	/// Gets or sets default <see cref="FileAnimationFrame.Duration"/> used by the Agent Character Editor. 
	/// </summary>
	virtual property UInt16 NewFrameDuration {virtual UInt16 get(); virtual void set (UInt16 pValue);}
#endif

// Operations
public:
#ifdef	_M_CEE
	static bool IsProperFilePath (const System::String^ pPath);
	static bool IsRelativeFilePath (const System::String^ pPath);
	static System::String^ ParseFilePath (const System::String^ pPath);
#else
	_DACORE_IMPEXP static bool IsProperFilePath (LPCTSTR pPath);
	_DACORE_IMPEXP static bool IsRelativeFilePath (LPCTSTR pPath);
	_DACORE_IMPEXP static tBstrPtr ParseFilePath (LPCTSTR pPath);
	_DACORE_IMPEXP tBstrPtr ParseRelativePath (LPCTSTR pRelativePath);
#endif

#ifdef	_M_CEE
	virtual bool Open (const System::String^ pPath) = 0;
	virtual bool Save ();
	virtual bool Save (const System::String^ pPath);
	virtual bool Save (const System::String^ pPath, CAgentFile^ pSource);
	virtual void Close ();
#else
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath) = 0;
	_DACORE_IMPEXP virtual void Close ();
#endif

#ifdef	_M_CEE
	CAgentFileName^ GetDefaultName () {return FindName (LANG_USER_DEFAULT);}
	virtual CAgentFileName^ FindName (WORD pLangID);
#else
	_DACORE_IMPEXP CAgentFileName* GetDefaultName () {return FindName (LANG_USER_DEFAULT);}
	_DACORE_IMPEXP virtual CAgentFileName* FindName (WORD pLangID = LANG_USER_DEFAULT);
	_DACORE_IMPEXP virtual INT_PTR FindState (LPCTSTR pStateName);
	_DACORE_IMPEXP virtual INT_PTR FindGesture (LPCTSTR pGestureName);
	_DACORE_IMPEXP virtual INT_PTR FindAnimation (LPCTSTR pAnimationName);

	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetGesture (INT_PTR pGestureNdx);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetAnimation (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetGesture (LPCTSTR pGestureName);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetAnimation (LPCTSTR pAnimationName);
#endif

#ifdef	_M_CEE
	virtual property int ImageCount {virtual int get();}
	CAgentFileImage^ GetImage (int pImageNdx);
	CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit);
	virtual CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor);
	virtual System::String^ GetImageFilePath (int pImageNdx);
	UINT GetImageBits (LPBYTE pImageBits, int pImageNdx);
	UINT GetImageBits (LPBYTE pImageBits, int pImageNdx, bool p32Bit);
	UINT GetImageBits (LPBYTE pImageBits, CAgentFileImage^ pImage);
	UINT GetImageBits (LPBYTE pImageBits, CAgentFileImage^ pImage, bool p32Bit);
	UINT GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame);
	UINT GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame, bool p32Bit);
	UINT GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor);
	UINT GetFrameBits (LPBYTE pImageBits, CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor, Int16 pOverlayType);
	System::Drawing::Bitmap^ GetImageBitmap (int pImageNdx);
	System::Drawing::Bitmap^ GetImageBitmap (int pImageNdx, bool p32Bit);
	virtual System::Drawing::Bitmap^ GetImageBitmap (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor);
	System::Drawing::Bitmap^ GetImageBitmap (CAgentFileImage^ pImage);
	System::Drawing::Bitmap^ GetFrameBitmap (CAgentFileFrame^ pFrame);
	System::Drawing::Bitmap^ GetFrameBitmap (CAgentFileFrame^ pFrame, bool p32Bit);
	System::Drawing::Bitmap^ GetFrameBitmap (CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor);
	System::Drawing::Bitmap^ GetFrameBitmap (CAgentFileFrame^ pFrame, bool p32Bit, System::Drawing::Color pBkColor, Int16 pOverlayType);
#else
	_DACORE_IMPEXP virtual INT_PTR GetImageCount () const;
	_DACORE_IMPEXP virtual CAgentFileImage* GetImage (INT_PTR pImageNdx, bool p32Bit = false, const COLORREF* pBkColor = NULL);
	_DACORE_IMPEXP UINT GetImageFormat (LPBITMAPINFO pImageInfo, const CAgentFileImage* pImage = NULL, bool p32Bit = false) const;
	_DACORE_IMPEXP UINT GetImageBits (LPBYTE pImageBits, const CAgentFileImage* pImage, bool p32Bit = false) const;
	_DACORE_IMPEXP UINT GetFrameBits (LPBYTE pImageBits, const CAgentFileFrame* pFrame, bool p32Bit = false, const COLORREF* pBkColor = NULL, SHORT pOverlayType = -1);
#endif

#ifdef	_M_CEE
	virtual property int SoundCount {virtual int get();}
	virtual int GetSoundSize (int pSoundNdx);
	virtual array <BYTE>^ GetSound (int pSoundNdx);
	virtual System::String^ GetSoundFilePath (int pSoundNdx);
#else
	_DACORE_IMPEXP virtual INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP virtual long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP virtual LPCVOID GetSound (INT_PTR pSoundNdx);
#endif

// Implementation
public:
#ifndef	_M_CEE
	_DACORE_IMPEXP void Log (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP void LogNames (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP void LogStates (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP void LogGestures (UINT pLogLevel, LPCTSTR pFormat = NULL, ...) const;
	_DACORE_IMPEXP static void LogTts (const CAgentFileTts& pTts, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP static void LogBalloon (const CAgentFileBalloon& pBalloon, UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
#endif

protected:
	virtual bool ReadNames ();
	virtual bool ReadNames (bool pFirstLetterCaps);
	virtual bool ReadStates ();
	virtual bool ReadGestures ();

	virtual void FreeNames ();
	virtual void FreeStates ();
	virtual void FreeGestures ();

#ifdef	_M_CEE
internal:
	virtual bool RenameAnimation (CAgentFileAnimation^ pAnimation, System::String^ pNewName) {return false;}
	virtual Int32 LoadImageFile (System::String^ pImageFilePath) {return -1;}
	virtual Int32 LoadSoundFile (System::String^ pSoundFilePath) {return -1;}
#endif

protected:
	DWORD										mSignature;
	WORD										mVersionMajor;
	WORD										mVersionMinor;
#ifdef	_M_CEE
	System::String^								mPath;
internal:
	CAgentFileHeader^							mHeader;
	CAgentFileTts^								mTts;
	CAgentFileBalloon^							mBalloon;
	CAgentFileNames^							mNames;
	CAgentFileStates^							mStates;
	CAgentFileGestures^							mGestures;
#else
	CAtlString									mPath;
	CAgentFileHeader							mHeader;
	CAgentFileTts								mTts;
	CAgentFileBalloon							mBalloon;
	CAtlOwnPtrArray <CAgentFileName>			mNames;
	CAgentFileStates							mStates;
	CAgentFileGestures							mGestures;
#endif
public:
	UINT										mLogLevel;
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
