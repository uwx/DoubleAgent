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
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Collections;
using namespace System::Collections::Generic;
#else
#include "DaCoreExp.h"
#include "AtlCollEx.h"
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
namespace Character {
ref class CharacterFile;
ref class FileAnimationFrame;
ref class FileFrameImages;
ref class FileFrameOverlays;
ref class FileFrames;
ref class FileAnimation;
ref class FileGestures;
#else
#pragma warning (push)
#pragma warning (disable:4251)
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define AgentCharStyle CharacterStyle
public enum class AgentCharStyle : UInt32
#else
enum AgentCharStyle
#endif
{
	CharStyleTts = 0x00000020,
	CharStyleBalloon = 0x00000200,
	CharStyleSizeToText = 0x00010000,
	CharStyleNoAutoHide = 0x00020000,
	CharStyleNoAutoPace = 0x00040000,
	CharStyleStandard = 0x00100000
};

#ifdef	_M_CEE
#define AgentMouthOverlay MouthOverlay
public enum class AgentMouthOverlay
#else
enum AgentMouthOverlay
#endif
{
	MouthOverlayClosed = 0,
	MouthOverlayWide1 = 1,
	MouthOverlayWide2 = 2,
	MouthOverlayWide3 = 3,
	MouthOverlayWide4 = 4,
	MouthOverlayMedium = 5,
	MouthOverlayNarrow = 6,
	MouthOverlayMin = MouthOverlayClosed,
	MouthOverlayMax = MouthOverlayNarrow
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileHeader FileHeader
[Serializable] public ref class CAgentFileHeader
#else
class _DACORE_IMPEXP CAgentFileHeader
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}

	property AgentCharStyle Style {AgentCharStyle get(); void set (AgentCharStyle pValue);}
	property System::Guid Guid {System::Guid get(); void set (System::Guid pValue);}
	property System::Drawing::Icon^ Icon {System::Drawing::Icon^ get();}
	property System::Drawing::Size ImageSize {System::Drawing::Size get(); void set (System::Drawing::Size pValue);}
	property System::Drawing::Imaging::ColorPalette^ Palette {System::Drawing::Imaging::ColorPalette^ get();}
	property Byte Transparency {Byte get(); void set (Byte pValue);}
#else
	__declspec(property(get=get_Style))			DWORD			Style;
	__declspec(property(get=get_Guid))			const GUID&		Guid;
	__declspec(property(get=get_Icon))			HICON			Icon;
	__declspec(property(get=get_ImageSize))		const CSize&	ImageSize;
	__declspec(property(get=get_Palette))		LPCOLORREF		Palette;
	__declspec(property(get=get_Transparency))	BYTE			Transparency;

	DWORD get_Style () const {return mStyle;}
	const GUID& get_Guid () const {return mGuid;}
	HICON get_Icon () const {return mIcon;}
	const CSize& get_ImageSize () const {return mImageSize;}
	LPCOLORREF get_Palette () const {return (LPCOLORREF)mPalette.Ptr();}
	BYTE get_Transparency () const {return mTransparency;}
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^				mOwner;
	UInt32										mStyle;
	System::Guid								mGuid;
	System::Drawing::Size						mImageSize;
	System::Drawing::Icon^						mIcon;
	System::Drawing::Imaging::ColorPalette^		mPalette;
	Byte										mTransparency;
#else
protected:
	DWORD										mStyle;
	GUID										mGuid;
	CSize										mImageSize;
	HICON										mIcon;
	tArrayPtr <COLORREF>						mPalette;
	BYTE										mTransparency;
#endif

#ifdef	_M_CEE
internal:
#else
public:
#endif
	CAgentFileHeader ();
	virtual ~CAgentFileHeader ();
	void Empty ();

#ifdef	_M_CEE
public:
    virtual System::String^ ToString() override;
internal:
	CAgentFileHeader (CharacterFile^ pOwner);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileHeader)
	friend class CAgentFileBinary;
	friend class CAgentFileAcs;
	friend class CAgentFileAcf;
#endif
};

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileName FileCharacterName
[Serializable] public ref class CAgentFileName
#else
class _DACORE_IMPEXP CAgentFileName
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}

	property LANGID Language {LANGID get();}
	property System::String^ Name {System::String^ get(); void set (System::String^ pValue);}
	property System::String^ Desc1 {System::String^ get(); void set (System::String^ pValue);}
	property System::String^ Desc2 {System::String^ get(); void set (System::String^ pValue);}
#else
	__declspec(property(get=get_Language))	LANGID		Language;
	__declspec(property(get=get_Name))		tBstrPtr	Name;
	__declspec(property(get=get_Desc1))		tBstrPtr	Desc1;
	__declspec(property(get=get_Desc2))		tBstrPtr	Desc2;

	LANGID get_Language () const {return mLanguage;}
	tBstrPtr get_Name () const {return mName;}
	tBstrPtr get_Desc1 () const {return mDesc1;}
	tBstrPtr get_Desc2 () const {return mDesc2;}
	void put_Name (LPCTSTR pName);
	void put_Desc1 (LPCTSTR pDesc1);
	void put_Desc2 (LPCTSTR pDesc2);
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^	mOwner;
	LANGID							mLanguage;
	System::String^					mName;
	System::String^					mDesc1;
	System::String^					mDesc2;
#else
protected:
	LANGID							mLanguage;
	tBstrPtr						mName;
	tBstrPtr						mDesc1;
	tBstrPtr						mDesc2;
#endif

public:
	CAgentFileName ();
	virtual ~CAgentFileName ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	CAgentFileName (LANGID pLanguage, System::String^ pName);
	CAgentFileName (LANGID pLanguage, CAgentFileName^ pSource);
	Boolean CopyTo (CAgentFileName^ pTarget);
	Boolean Equals (CAgentFileName^ pTarget);
    virtual System::String^ ToString() override;
internal:
	CAgentFileName (CharacterFile^ pOwner);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileName)
	friend class CAgentFileBinary;
#endif
};

#ifdef	_M_CEE
#define CAgentFileNames FileCharacterNames
public ref class CAgentFileNames : public System::Collections::ObjectModel::KeyedCollection <LANGID, CAgentFileName^>
{
public:
	CAgentFileNames ();
	virtual ~CAgentFileNames ();
internal:
	CAgentFileNames (CharacterFile^ pOwner);

public:
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}

	CAgentFileName^ Add (LANGID pLangID, System::String^ pName);
	bool Remove (LANGID pLangID) new;
	bool Remove (CAgentFileName^ pItem) new;

internal:
	void Add (CAgentFileName^ pItem) new {__super::Add (pItem);}
	void Insert (Int32 pNdx, CAgentFileName^ pItem) new {__super::Insert (pNdx, pItem);}
	void RemoveAt (Int32 pNdx) new {__super::RemoveAt (pNdx);}
	void Clear () new {__super::Clear ();}

protected:
	virtual LANGID GetKeyForItem (CAgentFileName^ pItem) override;

internal:
	[NonSerialized]	CharacterFile^	mOwner;
};
#else
typedef	CAtlPtrTypeArray <CAgentFileName>	CAgentFileNames;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileTts FileTts
[Serializable] public ref class CAgentFileTts
#else
class _DACORE_IMPEXP CAgentFileTts
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}

	property System::Guid Engine {System::Guid get(); void set (System::Guid pValue);}
	property System::Guid Mode {System::Guid get(); void set (System::Guid pValue);}
	property System::String^ ModeId {System::String^ get();}
	property LANGID Language {LANGID get(); void set (LANGID pValue);}
	property UInt16 Gender {UInt16 get(); void set (UInt16 pValue);}
#else
	__declspec(property(get=get_Engine))	const GUID&		Engine;
	__declspec(property(get=get_Mode))		const GUID&		Mode;
	__declspec(property(get=get_ModeId))	tBstrPtr		ModeId;
	__declspec(property(get=get_Language))	LANGID			Language;
	__declspec(property(get=get_Gender))	USHORT			Gender;
	__declspec(property(get=get_Style))		tBstrPtr		Style;
	__declspec(property(get=get_Age))		USHORT			Age;
	__declspec(property(get=get_Speed))		long			Speed;
	__declspec(property(get=get_Pitch))		short			Pitch;

	const GUID& get_Engine () const {return mEngine;}
	const GUID& get_Mode () const {return mMode;}
	tBstrPtr get_ModeId () const {return mModeId;}
	LANGID get_Language () const {return mLanguage;}
	USHORT get_Gender () const {return mGender;}
	tBstrPtr get_Style () const {return mStyle;}
	USHORT get_Age () const {return mAge;}
	long get_Speed () const {return mSpeed;}
	short get_Pitch () const {return mPitch;}

	void put_Mode (const GUID& pMode);
	void put_Language (LANGID pLanguage);
	void put_Gender (USHORT pGender);
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^	mOwner;
	System::Guid					mEngine;
	System::Guid					mMode;
	System::String^					mModeId;
	LANGID							mLanguage;
	UInt16							mGender;
#else
protected:
	GUID							mEngine;
	GUID							mMode;
	tBstrPtr						mModeId;
	LANGID							mLanguage;
	USHORT							mGender;
	tBstrPtr						mStyle;
	USHORT							mAge;
	long							mSpeed;
	short							mPitch;
#endif

public:
	CAgentFileTts ();
	virtual ~CAgentFileTts ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	Boolean CopyTo (CAgentFileTts^ pTarget);
	Boolean Equals (CAgentFileTts^ pTarget);
    virtual System::String^ ToString() override;
internal:
	CAgentFileTts (CharacterFile^ pOwner);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileTts)
	friend class CAgentFileBinary;
#endif
};

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileBalloon FileBalloon
[Serializable] public ref class CAgentFileBalloon
#else
class _DACORE_IMPEXP CAgentFileBalloon
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}

	property UInt16 Lines {UInt16 get(); void set (UInt16 pValue);}
	property UInt16 PerLine {UInt16 get(); void set (UInt16 pValue);}
	property System::Drawing::Color FgColor {System::Drawing::Color get(); void set (System::Drawing::Color pValue);}
	property System::Drawing::Color BkColor {System::Drawing::Color get(); void set (System::Drawing::Color pValue);}
	property System::Drawing::Color BrColor {System::Drawing::Color get(); void set (System::Drawing::Color pValue);}
	property System::Drawing::Font^ Font {System::Drawing::Font^ get(); void set (System::Drawing::Font^ pValue);}

	static property UInt16 MinLines {UInt16 get();}
	static property UInt16 MaxLines {UInt16 get();}
	static property UInt16 DefLines {UInt16 get();}
	static property UInt16 MinPerLine {UInt16 get();}
	static property UInt16 MaxPerLine {UInt16 get();}
	static property UInt16 DefPerLine {UInt16 get();}
#else
	__declspec(property(get=get_Lines))		USHORT			Lines;
	__declspec(property(get=get_PerLine))	USHORT			PerLine;
	__declspec(property(get=get_FgColor))	COLORREF		FgColor;
	__declspec(property(get=get_BkColor))	COLORREF		BkColor;
	__declspec(property(get=get_BrColor))	COLORREF		BrColor;
	__declspec(property(get=get_Font))		const LOGFONT&	Font;

	USHORT get_Lines () const {return mLines;}
	USHORT get_PerLine () const {return mPerLine;}
	COLORREF get_FgColor () const {return mFgColor;}
	COLORREF get_BkColor () const {return mBkColor;}
	COLORREF get_BrColor () const {return mBrColor;}
	const LOGFONT& get_Font () const {return mFont;}

	void put_Lines (USHORT pLines);
	void put_PerLine (USHORT pPerLine);
	void put_FgColor (COLORREF pFgColor);
	void put_BkColor (COLORREF pBkColor);
	void put_BrColor (COLORREF pBrColor);
	void put_Font (const LOGFONT& pFont);

	static const USHORT	MinLines;
	static const USHORT	MaxLines;
	static const USHORT	DefLines;
	static const USHORT	MinPerLine;
	static const USHORT	MaxPerLine;
	static const USHORT	DefPerLine;
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^	mOwner;
	UInt16							mLines;
	UInt16							mPerLine;
	System::Drawing::Color			mFgColor;
	System::Drawing::Color			mBkColor;
	System::Drawing::Color			mBrColor;
	System::Drawing::Font^			mFont;
#else
protected:
	USHORT							mLines;
	USHORT							mPerLine;
	COLORREF						mFgColor;
	COLORREF						mBkColor;
	COLORREF						mBrColor;
	tS <LOGFONT>					mFont;
#endif

public:
	CAgentFileBalloon ();
	virtual ~CAgentFileBalloon ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	Boolean CopyTo (CAgentFileBalloon^ pTarget);
	Boolean Equals (CAgentFileBalloon^ pTarget);
    virtual System::String^ ToString() override;
internal:
	CAgentFileBalloon (CharacterFile^ pOwner);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileBalloon)
	friend class CAgentFileBinary;
	friend class CDaCmnBalloon;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileFrameImage FileFrameImage
[Serializable] public ref class CAgentFileFrameImage
#else
class _DACORE_IMPEXP CAgentFileFrameImage
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileFrameImages^ Container {FileFrameImages^ get() {return mContainer;}}
	property FileAnimationFrame^ Frame {FileAnimationFrame^ get();}

	property Int32 ImageNdx {Int32 get();}
	property System::String^ ImageFilePath {System::String^ get(); void set (System::String^ pValue);}
	property System::Drawing::Point Offset {System::Drawing::Point get(); void set (System::Drawing::Point pValue);}
#else
	__declspec(property(get=get_ImageNdx))	LONG			ImageNdx;
	__declspec(property(get=get_Offset))	const CPoint&	Offset;

	LONG get_ImageNdx () const {return mImageNdx;}
	const CPoint& get_Offset () const {return mOffset;}
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^		mOwner;
	[NonSerialized]	FileFrameImages^	mContainer;
	Int32								mImageNdx;
	System::Drawing::Point				mOffset;
#else
protected:
	LONG								mImageNdx;
	CPoint								mOffset;
#endif

public:
	CAgentFileFrameImage ();
	virtual ~CAgentFileFrameImage ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	Boolean CopyTo (CAgentFileFrameImage^ pTarget);
    virtual System::String^ ToString() override;
internal:
	CAgentFileFrameImage (CharacterFile^ pOwner, FileAnimationFrame^ pFrame);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileFrameImage)
	friend class CAgentFileAcs;
	friend class CAgentFileAcf;
#endif
};

#ifdef	_M_CEE
#define CAgentFileFrameImages FileFrameImages
[Serializable] public ref class CAgentFileFrameImages : public System::Collections::ObjectModel::Collection <CAgentFileFrameImage^>
{
public:
	CAgentFileFrameImages ();
	virtual ~CAgentFileFrameImages ();
internal:
	CAgentFileFrameImages (CharacterFile^ pOwner, FileAnimationFrame^ pFrame);

public:
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileAnimationFrame^ Frame {FileAnimationFrame^ get() {return mFrame;}}

public:
	CAgentFileFrameImage^ Add (System::String^ pFrameImagePath);
	CAgentFileFrameImage^ Insert (Int32 pNdx, System::String^ pFrameImagePath);
	bool Remove (CAgentFileFrameImage^ pItem) new;
	bool RemoveAt (Int32 pNdx) new;
	bool Move (Int32 pNdx, Int32 pNewNdx);
	bool Move (CAgentFileFrameImage^ pItem, Int32 pNewNdx);
	bool Move (CAgentFileFrameImage^ pItem, CAgentFileFrameImage^ pBefore);

internal:
	void Add (CAgentFileFrameImage^ pItem) new {__super::Add (pItem);}
	void Insert (Int32 pNdx, CAgentFileFrameImage^ pItem) new {__super::Insert (pNdx, pItem);}
	void Clear () new {__super::Clear ();}

internal:
	[NonSerialized]	CharacterFile^		mOwner;
	[NonSerialized]	FileAnimationFrame^	mFrame;
};
#else
typedef tArrayPtr <CAgentFileFrameImage> CAgentFileFrameImages;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileFrameOverlay FileFrameOverlay
[Serializable] public ref class CAgentFileFrameOverlay
#else
class _DACORE_IMPEXP CAgentFileFrameOverlay
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileFrameOverlays^ Container {FileFrameOverlays^ get() {return mContainer;}}
	property FileAnimationFrame^ Frame {FileAnimationFrame^ get();}

	property AgentMouthOverlay OverlayType {AgentMouthOverlay get(); void set (AgentMouthOverlay pValue);}
	property Int32 ImageNdx {Int32 get();}
	property System::String^ ImageFilePath {System::String^ get(); void set (System::String^ pValue);}
	property Boolean ReplaceFlag {Boolean get(); void set (Boolean pReplaceFlag);}
	property System::Drawing::Point Offset {System::Drawing::Point get(); void set (System::Drawing::Point pValue);}
#else
	__declspec(property(get=get_OverlayType))	AgentMouthOverlay	OverlayType;
	__declspec(property(get=get_ImageNdx))		LONG				ImageNdx;
	__declspec(property(get=get_ReplaceFlag))	bool				ReplaceFlag;
	__declspec(property(get=get_Offset))		const CPoint&		Offset;

	AgentMouthOverlay get_OverlayType () const {return mOverlayType;}
	LONG get_ImageNdx () const {return mImageNdx;}
	bool get_ReplaceFlag () const {return mReplaceFlag;}
	const CPoint& get_Offset () const {return mOffset;}
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^			mOwner;
	[NonSerialized]	FileFrameOverlays^		mContainer;
	AgentMouthOverlay						mOverlayType;
	Int32									mImageNdx;
	Boolean									mReplaceFlag;
	System::Drawing::Point					mOffset;
	[NonSerialized]	System::Drawing::Point	mSomething;
#else
protected:
	AgentMouthOverlay						mOverlayType;
	LONG									mImageNdx;
	bool									mReplaceFlag;
	CPoint									mOffset;
	CPoint									mSomething;
#endif

public:
	CAgentFileFrameOverlay ();
	virtual ~CAgentFileFrameOverlay ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	Boolean CopyTo (CAgentFileFrameOverlay^ pTarget);
    virtual System::String^ ToString() override;
internal:
	CAgentFileFrameOverlay (CharacterFile^ pOwner, FileAnimationFrame^ pFrame);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileFrameOverlay)
	friend class CAgentFileAcs;
	friend class CAgentFileAcf;
#endif
};

#ifdef	_M_CEE
#define CAgentFileFrameOverlays FileFrameOverlays
[Serializable] public ref class CAgentFileFrameOverlays : public System::Collections::ObjectModel::KeyedCollection <AgentMouthOverlay, CAgentFileFrameOverlay^>
{
public:
	CAgentFileFrameOverlays ();
	virtual ~CAgentFileFrameOverlays ();
internal:
	CAgentFileFrameOverlays (CharacterFile^ pOwner, FileAnimationFrame^ pFrame);

public:
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileAnimationFrame^ Frame {FileAnimationFrame^ get() {return mFrame;}}

public:
	CAgentFileFrameOverlay^ Add (AgentMouthOverlay pOverlayType, System::String^ pOverlayImagePath);
	bool Remove (AgentMouthOverlay pOverlayType) new;
	bool Remove (CAgentFileFrameOverlay^ pItem) new;

internal:
	void Add (CAgentFileFrameOverlay^ pItem) new {__super::Add (pItem);}
	void Insert (Int32 pNdx, CAgentFileFrameOverlay^ pItem) new {__super::Insert (pNdx, pItem);}
	void RemoveAt (Int32 pNdx) new {__super::RemoveAt (pNdx);}
	void Clear () new {__super::Clear ();}

protected:
	virtual AgentMouthOverlay GetKeyForItem (CAgentFileFrameOverlay^ pItem) override;

internal:
	[NonSerialized]	CharacterFile^		mOwner;
	[NonSerialized]	FileAnimationFrame^	mFrame;
};
#else
typedef tArrayPtr <CAgentFileFrameOverlay> CAgentFileFrameOverlays;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileFrameBranch FileFrameBranch
[Serializable] public value struct CAgentFileFrameBranch
{
	Int16	mFrameNdx;
	UInt16	mProbability;
};
#else
struct _DACORE_IMPEXP CAgentFileFrameBranch
{
	short	mFrameNdx;
	USHORT	mProbability;
};
#endif

#ifdef	_M_CEE
#define CAgentFileFrame FileAnimationFrame
[Serializable] public ref class CAgentFileFrame
#else
class _DACORE_IMPEXP CAgentFileFrame
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileFrames^ Container {FileFrames^ get() {return mContainer;}}
	property FileAnimation^ Animation {FileAnimation^ get();}

	property UInt16 Duration {UInt16 get(); void set (UInt16 pValue);}
	property Int16 SoundNdx {Int16 get();}
	property System::String^ SoundFilePath {System::String^ get(); void set (System::String^ pValue);}
	property Int16 ExitFrame {Int16 get(); void set (Int16 pValue);}
	property array <CAgentFileFrameBranch>^ Branching {array <CAgentFileFrameBranch>^ get(); void set (array <CAgentFileFrameBranch>^ pValue);}
	property UInt16 ImageCount {UInt16 get();}
	property CAgentFileFrameImages^ Images {CAgentFileFrameImages^ get();}
	property UInt16 OverlayCount {UInt16 get();}
	property CAgentFileFrameOverlays^ Overlays {CAgentFileFrameOverlays^ get();}
#else
	__declspec(property(get=get_Duration))		USHORT							Duration;
	__declspec(property(get=get_SoundNdx))		short							SoundNdx;
	__declspec(property(get=get_ExitFrame))		short							ExitFrame;
	__declspec(property(get=get_Branching))		const CAgentFileFrameBranch*	Branching;
	__declspec(property(get=get_ImageCount))	USHORT							ImageCount;
	__declspec(property(get=get_Images))		const CAgentFileFrameImage*		Images;
	__declspec(property(get=get_OverlayCount))	USHORT							OverlayCount;
	__declspec(property(get=get_Overlays))		const CAgentFileFrameOverlay*	Overlays;

#ifdef	_DEBUG
	USHORT get_Duration () const;
#else
	USHORT get_Duration () const {return mDuration;}
#endif
	short get_SoundNdx () const {return mSoundNdx;}
	short get_ExitFrame () const {return mExitFrame;}
	const CAgentFileFrameBranch* get_Branching () const {return mBranching;}
	USHORT get_ImageCount () const {return mImageCount;}
	const CAgentFileFrameImage* get_Images () const {return mImages;}
	USHORT get_OverlayCount () const {return mOverlayCount;}
	const CAgentFileFrameOverlay* get_Overlays () const {return mOverlays;}
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^	mOwner;
	[NonSerialized]	FileFrames^		mContainer;
	UInt16							mDuration;
	Int16							mSoundNdx;
	Int16							mExitFrame;
	array <CAgentFileFrameBranch>^	mBranching;
	CAgentFileFrameImages^			mImages;
	CAgentFileFrameOverlays^		mOverlays;
#else
protected:
	USHORT							mDuration;
	short							mSoundNdx;
	short							mExitFrame;
	CAgentFileFrameBranch			mBranching [3];
	USHORT							mImageCount;
	CAgentFileFrameImages			mImages;
	USHORT							mOverlayCount;
	CAgentFileFrameOverlays			mOverlays;
#endif

public:
	CAgentFileFrame ();
	virtual ~CAgentFileFrame ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	Boolean CopyTo (CAgentFileFrame^ pTarget);
	Boolean CopyTo (CAgentFileFrame^ pTarget, Boolean pDeepCopy);
    virtual System::String^ ToString() override;
internal:
	CAgentFileFrame (CharacterFile^ pOwner, FileFrames^ pContainer);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileFrame)
	friend class CAgentFileAcs;
	friend class CAgentFileAcf;
#endif
};

#ifdef	_M_CEE
#define CAgentFileFrames FileFrames
[Serializable] public ref class CAgentFileFrames : public System::Collections::ObjectModel::Collection <CAgentFileFrame^>
{
public:
	CAgentFileFrames ();
	virtual ~CAgentFileFrames ();
internal:
	CAgentFileFrames (CharacterFile^ pOwner, FileAnimation^ pAnimation);

public:
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileAnimation^ Animation {FileAnimation^ get() {return mAnimation;}}

public:
	CAgentFileFrame^ Add ();
	CAgentFileFrame^ Insert (Int32 pNdx);
	bool Remove (CAgentFileFrame^ pItem) new;
	bool RemoveAt (Int32 pNdx) new;
	bool Move (Int32 pNdx, Int32 pNewNdx);
	bool Move (CAgentFileFrame^ pItem, Int32 pNewNdx);
	bool Move (CAgentFileFrame^ pItem, CAgentFileFrame^ pBefore);

internal:
	void Add (CAgentFileFrame^ pItem) new {__super::Add (pItem);}
	void Insert (Int32 pNdx, CAgentFileFrame^ pItem) new {__super::Insert (pNdx, pItem);}
	void Clear () new {__super::Clear ();}

internal:
	[NonSerialized]	CharacterFile^	mOwner;
	[NonSerialized]	FileAnimation^	mAnimation;
};
#else
typedef	tArrayPtr <CAgentFileFrame> CAgentFileFrames;
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileAnimation FileAnimation
[Serializable] public ref class CAgentFileAnimation
#else
class _DACORE_IMPEXP CAgentFileAnimation
#endif
{
public:
#ifdef	_M_CEE
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
	property FileGestures^ Container {FileGestures^ get() {return mContainer;}}

	property System::String^ Name {System::String^ get(); void set (System::String^ pName);}
	property Byte ReturnType {Byte get(); void set (Byte pReturnType);}
	property System::String^ ReturnName {System::String^ get(); void set (System::String^ pReturnName);}
	property UInt16 FrameCount {UInt16 get();}
	property CAgentFileFrames^ Frames {CAgentFileFrames^ get();}
#else
	__declspec(property(get=get_Name))			tBstrPtr				Name;
	__declspec(property(get=get_ReturnType))	BYTE					ReturnType;
	__declspec(property(get=get_ReturnName))	tBstrPtr				ReturnName;
	__declspec(property(get=get_FrameCount))	USHORT					FrameCount;
	__declspec(property(get=get_Frames))		const CAgentFileFrame*	Frames;

	tBstrPtr get_Name () const {return mName;}
	BYTE get_ReturnType () const {return mReturnType;}
	tBstrPtr get_ReturnName () const {return mReturnName;}
	USHORT get_FrameCount () const {return mFrameCount;}
	const CAgentFileFrame* get_Frames () const {return mFrames;}
#endif

#ifdef	_M_CEE
internal:
	[NonSerialized]	CharacterFile^	mOwner;
	[NonSerialized]	FileGestures^	mContainer;
	System::String^					mName;
	Byte							mReturnType;
	System::String^					mReturnName;
	CAgentFileFrames^				mFrames;
	[NonSerialized]	System::String^	mAcaFileName;
	[NonSerialized]	DWORD			mAcaChksum;
#else
protected:
	tBstrPtr						mName;
	BYTE							mReturnType;
	tBstrPtr						mReturnName;
	USHORT							mFrameCount;
	CAgentFileFrames				mFrames;
	tBstrPtr						mAcaFileName;
	DWORD							mAcaChksum;
#endif

public:
	CAgentFileAnimation ();
	virtual ~CAgentFileAnimation ();
#ifdef	_M_CEE
internal:
#endif
	void Empty ();

#ifdef	_M_CEE
public:
	Boolean CopyTo (CAgentFileAnimation^ pTarget);
	Boolean CopyTo (CAgentFileAnimation^ pTarget, Boolean pDeepCopy);
    virtual System::String^ ToString() override;
internal:
	CAgentFileAnimation (CharacterFile^ pOwner, FileGestures^ pContainer);
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileAnimation)
	friend class CAgentFile;
	friend class CAgentFileAcs;
	friend class CAgentFileAcf;
#endif
};

#ifdef	_M_CEE
#define CAgentFileGestures FileGestures
public ref class CAgentFileGestures : public System::Collections::ObjectModel::KeyedCollection <System::String^, CAgentFileAnimation^>
{
public:
	CAgentFileGestures ();
	virtual ~CAgentFileGestures ();
internal:
	CAgentFileGestures (CharacterFile^ pOwner);

public:
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
public:
	CAgentFileAnimation^ Add (System::String^ pAnimationName);
	bool Remove (System::String^ pAnimationName) new;
	bool Remove (CAgentFileAnimation^ pItem) new;

internal:
	void Add (CAgentFileAnimation^ pItem) new;
	void RemoveAt (Int32 pNdx) new {__super::RemoveAt (pNdx);}
	void Clear () new {__super::Clear ();}
	void ChangeItemKey (CAgentFileAnimation^ pItem, String^ pNewKey);

protected:
	virtual System::String^ GetKeyForItem (CAgentFileAnimation^ pItem) override;

internal:
	[NonSerialized]	CharacterFile^	mOwner;
};
#else
struct _DACORE_IMPEXP CAgentFileGestures
{
	CAtlOwnPtrArray <CAgentFileAnimation>	mAnimations;
	CAtlStringArray							mNames;

	CAgentFileAnimation* operator() (const CAtlString& pName);
	const CAgentFileAnimation* operator() (const CAtlString& pName) const;
	DECLARE_DLL_OBJECT(CAgentFileGestures)
};
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileStates FileStates
public ref class CAgentFileStates : public System::Collections::Generic::SortedList <System::String^, array <System::String^>^>
{
public:
	CAgentFileStates ();
	virtual ~CAgentFileStates ();
internal:
	CAgentFileStates (CharacterFile^ pOwner);

public:
	property CharacterFile^ Owner {CharacterFile^ get() {return mOwner;}}
public:
	bool AddStateAnimation (System::String^ pStateName, System::String^ pAnimationName);
	bool RemoveStateAnimation (System::String^ pStateName, System::String^ pAnimationName);

internal:
	void Add (System::String^ pStateName, array <System::String^>^ pAnimations) new {__super::Add (pStateName, pAnimations);}

internal:
	[NonSerialized]	CharacterFile^	mOwner;
};
#else
struct _DACORE_IMPEXP CAgentFileStates
{
	CAtlClassArray <CAtlStringArray>	mGestures;
	CAtlStringArray						mNames;

	CAtlStringArray* operator() (const CAtlString& pName);
	const CAtlStringArray* operator() (const CAtlString& pName) const;
	DECLARE_DLL_OBJECT(CAgentFileStates)
};
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CAgentFileImage FileImage
public ref class CAgentFileImage
#else
class _DACORE_IMPEXP CAgentFileImage
#endif
{
public:
#ifdef	_M_CEE
	property UInt32 ImageNum {UInt32 get();}
	property System::Drawing::Size ImageSize {System::Drawing::Size get();}
	property Boolean Is32Bit {Boolean get();}
	property UInt32 BitsSize {UInt32 get();}
	property array <BYTE>^ Bits {array <BYTE>^ get();}
#else
	__declspec(property(get=get_ImageNum))		ULONG			ImageNum;
	__declspec(property(get=get_ImageSize))		const CSize&	ImageSize;
	__declspec(property(get=get_Is32Bit))		bool			Is32Bit;
	__declspec(property(get=get_BitsSize))		ULONG			BitsSize;
	__declspec(property(get=get_Bits))			const BYTE*		Bits;

	ULONG get_ImageNum () const {return mImageNum;}
	const CSize& get_ImageSize () const {return mImageSize;}
	bool get_Is32Bit () const {return mIs32Bit;}
	ULONG get_BitsSize () const {return mBitsSize;}
	const BYTE* get_Bits () const {return mBits;}

	void put_ImageSize (const CSize& pImageSize);
#endif

#ifdef	_M_CEE
internal:
	UInt32					mImageNum;
	System::Drawing::Size	mImageSize;
	Boolean					mIs32Bit;
	array <BYTE>^			mBits;
#else
protected:
	ULONG					mImageNum;
	CSize					mImageSize;
	bool					mIs32Bit;
	ULONG					mBitsSize;
	tArrayPtr <BYTE>		mBits;
#endif

#ifdef	_M_CEE
internal:
#else
public:
#endif
	CAgentFileImage ();
	virtual ~CAgentFileImage ();
	void Empty ();

#ifdef	_M_CEE
public:
    virtual System::String^ ToString() override;
#else
public:
	DECLARE_DLL_OBJECT(CAgentFileImage)
	friend class CAgentFileAcs;
	friend class CAgentFileAcf;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
