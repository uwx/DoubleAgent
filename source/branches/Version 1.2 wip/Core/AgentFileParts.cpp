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
#include "AgentFile.h"
#ifdef	_M_CEE
#include "HandleTemplates.h"
#else
#include "DaCore.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "DebugProcess.h"
#endif
#endif
#include "AgentFileDefs.inl"

#ifndef	_M_CEE
IMPLEMENT_DLL_OBJECT(CAgentFileHeader)
IMPLEMENT_DLL_OBJECT(CAgentFileName)
IMPLEMENT_DLL_OBJECT(CAgentFileTts)
IMPLEMENT_DLL_OBJECT(CAgentFileBalloon)
IMPLEMENT_DLL_OBJECT(CAgentFileImage)
IMPLEMENT_DLL_OBJECT(CAgentFileFrameImage)
IMPLEMENT_DLL_OBJECT(CAgentFileFrameOverlay)
IMPLEMENT_DLL_OBJECT(CAgentFileFrame)
IMPLEMENT_DLL_OBJECT(CAgentFileAnimation)
IMPLEMENT_DLL_OBJECT(CAgentFileGestures)
IMPLEMENT_DLL_OBJECT(CAgentFileStates)
#endif

#ifndef	_M_CEE
#ifdef	_DEBUG
#define	_DEBUG_SEQUENCE_FRAMES	(GetProfileDebugInt(_T("DebugSequenceFrames"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#endif
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef	_M_CEE
CAgentFileAnimation* CAgentFileGestures::operator() (const CAtlString& pName) {return mAnimations (FindSortedString (mNames, pName));}
const CAgentFileAnimation* CAgentFileGestures::operator() (const CAtlString& pName) const {return mAnimations (FindSortedString (mNames, pName));}

CAtlStringArray* CAgentFileStates::operator() (const CAtlString& pName) {return mGestures (FindSortedString (mNames, pName));}
const CAtlStringArray* CAgentFileStates::operator() (const CAtlString& pName) const {return mGestures (FindSortedString (mNames, pName));}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentFileHeader::CAgentFileHeader ()
#ifndef	_M_CEE
:	mIcon (NULL)
#endif
{
	Empty ();
}

#ifdef	_M_CEE
CAgentFileHeader::CAgentFileHeader (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileHeader::Empty ()
{
#ifdef	_M_CEE
	if	(mIcon)
	{
		DestroyIcon ((HICON)(INT_PTR)mIcon->Handle);
	}
#else
	if	(mIcon)
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CharacterFile::DestroyIcon [%p]"), this, mIcon);
		}
#endif

		DestroyIcon (mIcon);

#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CharacterFile::DestroyIcon [%p] Done"), this, mIcon);
		}
#endif
	}
#endif

	mTransparency = 0;
	mStyle = 0;
#ifdef	_M_CEE
	mImageSize.Width = 0;
	mImageSize.Height = 0;
	mIcon = nullptr;
	mGuid = System::Guid::Empty;
	mPalette = nullptr;
#else
	mImageSize.cx = mImageSize.cy = 0;
	mIcon = NULL;
	mGuid = GUID_NULL;
	mPalette = NULL;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
AgentCharStyle CAgentFileHeader::Style::get ()
{
	return (AgentCharStyle)mStyle;
}

void CAgentFileHeader::Style::set (AgentCharStyle pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mStyle != (UInt32)pValue)
		)
	{
		mStyle = (UInt32)pValue;
		if	(
				(mStyle & (UInt32)AgentCharStyle::CharStyleBalloon)
			&&	(!mOwner->mBalloon)
			)
		{
			mOwner->mBalloon = gcnew CAgentFileBalloon (mOwner);
		}
		if	(
				(mStyle & (UInt32)AgentCharStyle::CharStyleTts)
			&&	(!mOwner->mTts)
			)
		{
			mOwner->mTts = gcnew CAgentFileTts (mOwner);
		}
		mOwner->IsDirty = true;
	}
}

System::Guid CAgentFileHeader::Guid::get ()
{
	return mGuid;
}

void CAgentFileHeader::Guid::set (System::Guid pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!pValue.Equals (System::Guid::Empty))
		&&	(!mGuid.Equals (pValue))
		)
	{
		mGuid = pValue;
		mOwner->IsDirty = true;
	}
}

System::Drawing::Icon^ CAgentFileHeader::Icon::get ()
{
	return mIcon;
}

System::Drawing::Size CAgentFileHeader::ImageSize::get ()
{
	return mImageSize;
}

void CAgentFileHeader::ImageSize::set (System::Drawing::Size pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(pValue.Width > 0)
		&&	(pValue.Height > 0)
		&&	(mImageSize != pValue)
		)
	{
		mImageSize = pValue;
		mOwner->IsDirty = true;
	}
}

System::Drawing::Imaging::ColorPalette^ CAgentFileHeader::Palette::get ()
{
	return mPalette;
}

Byte CAgentFileHeader::Transparency::get ()
{
	return mTransparency;
}

void CAgentFileHeader::Transparency::set (Byte pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mTransparency != pValue)
		)
	{
		mTransparency = pValue;
		mOwner->IsDirty = true;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileHeader::ToString ()
{
	String^	lStyle = String::Empty;

	if	((UInt32)Style & (UInt32)AgentCharStyle::CharStyleStandard)
	{
		lStyle += "Standard ";
	}
	if	((UInt32)Style & (UInt32)AgentCharStyle::CharStyleTts)
	{
		lStyle += "Tts ";
	}
	if	((UInt32)Style & (UInt32)AgentCharStyle::CharStyleBalloon)
	{
		lStyle += "Balloon ";
	}
	if	((UInt32)Style & (UInt32)AgentCharStyle::CharStyleSizeToText)
	{
		lStyle += "SizeToText ";
	}
	if	((UInt32)Style & (UInt32)AgentCharStyle::CharStyleNoAutoHide)
	{
		lStyle += "NoAutoHide ";
	}
	if	((UInt32)Style & (UInt32)AgentCharStyle::CharStyleNoAutoPace)
	{
		lStyle += "NoAutoPace ";
	}
    return String::Format ("Style [{0}] ImageSize {1},{2} Transparency {3} GUID {4}", lStyle, ImageSize.Width, ImageSize.Height, Transparency, Guid.ToString()->ToUpper());
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileName::CAgentFileName (LANGID pLanguage, System::String^ pName)
{
	Empty ();
	mLanguage = pLanguage;
	mName = pName;
}

CAgentFileName::CAgentFileName (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileName::Empty ()
{
	mLanguage = 0;
#ifdef	_M_CEE
	mName = String::Empty;
	mDesc1 = nullptr;
	mDesc2 = nullptr;
#else
	mName = NULL;
	mDesc1 = NULL;
	mDesc2 = NULL;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
LANGID CAgentFileName::Language::get ()
{
	return mLanguage;
}
#endif

#ifdef	_M_CEE
System::String^ CAgentFileName::Name::get ()
{
	return mName;
}

void CAgentFileName::Name::set (System::String^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pValue))
		&&	(
				(String::IsNullOrEmpty (mName))
			||	(!mName->Equals (pValue))
			)
		)
	{
		mName = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileName::put_Name (LPCTSTR pName)
{
	CAtlString	lName (pName);

	if	(!lName.IsEmpty())
	{
		mName = lName.AllocSysString ();
	}
}
#endif

#ifdef	_M_CEE
System::String^ CAgentFileName::Desc1::get ()
{
	return mDesc1;
}

void CAgentFileName::Desc1::set (System::String^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(
				(String::IsNullOrEmpty (mDesc1) != String::IsNullOrEmpty (pValue))
			||	(
					(mDesc1)
				&&	(pValue)
				&&	(!mDesc1->Equals (pValue))
				)
			)
		)
	{
		mDesc1 = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileName::put_Desc1 (LPCTSTR pDesc1)
{
	mDesc1 = CAtlString (pDesc1).AllocSysString ();
}
#endif

#ifdef	_M_CEE
System::String^ CAgentFileName::Desc2::get ()
{
	return mDesc2;
}

void CAgentFileName::Desc2::set (System::String^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(
				(String::IsNullOrEmpty (mDesc2) != String::IsNullOrEmpty (pValue))
			||	(
					(mDesc2)
				&&	(pValue)
				&&	(!mDesc2->Equals (pValue))
				)
			)
		)
	{
		mDesc2 = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileName::put_Desc2 (LPCTSTR pDesc2)
{
	mDesc2 = CAtlString (pDesc2).AllocSysString ();
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileName::ToString ()
{
    return String::Format ("Name \"{0}\" Language {1:X} {1:D} \"{2}\" \"{3}\" \"{4}\"", Name, Language, (gcnew System::Globalization::CultureInfo (Language))->EnglishName, Desc1, Desc2);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileNames::CAgentFileNames ()
{
}

CAgentFileNames::CAgentFileNames (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
}

LANGID CAgentFileNames::GetKeyForItem (CAgentFileName^ pItem)
{
	return pItem->Language;
}

CAgentFileName^ CAgentFileNames::Add (LANGID pLangID, System::String^ pName)
{
	CAgentFileName^	lFileName = nullptr;

	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!Contains (pLangID))
		&&	(!String::IsNullOrEmpty (pName))
		)
	{
		lFileName = gcnew CAgentFileName (mOwner);
		lFileName->mLanguage = pLangID;
		lFileName->mName = pName;

		__super::Add (lFileName);
		mOwner->IsDirty = true;
	}
	return lFileName;
}

bool CAgentFileNames::Remove (LANGID pLangID)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(__super::Remove (pLangID))
		)
	{
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileNames::Remove (CAgentFileName^ pItem)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(__super::Remove (pItem))
		)
	{
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileTts::CAgentFileTts (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileTts::Empty ()
{
	mGender = 0;
	mLanguage = 0;
#ifdef	_M_CEE
	mEngine = Guid::Empty;
	mMode = Guid::Empty;
	mModeId = String::Empty;
#else
	mEngine = GUID_NULL;
	mMode = GUID_NULL;
	mModeId = NULL;
	mStyle = NULL;
	mAge = 0;
	mSpeed = 0;
	mPitch = 0;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::Guid CAgentFileTts::Engine::get ()
{
	return mEngine;
}

void CAgentFileTts::Engine::set (System::Guid pValue)
{
	if	(
			(mOwner)
		&&	(!mEngine.Equals (pValue))
		)
	{
		mEngine = pValue;
		mOwner->IsDirty = true;
	}
}
#endif

#ifdef	_M_CEE
System::Guid CAgentFileTts::Mode::get ()
{
	return mMode;
}

void CAgentFileTts::Mode::set (System::Guid pValue)
{
	if	(
			(mOwner)
		&&	(!mMode.Equals (pValue))
		)
	{
		mMode = pValue;
		if	(mMode.Equals (Guid::Empty))
		{
			mModeId = nullptr;
		}
		else
		{
			mModeId = mMode.ToString()->ToUpper();
		}
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileTts::put_Mode (const GUID& pMode)
{
	mMode = pMode;

	if	(IsEqualGUID (mMode, GUID_NULL))
	{
		mModeId = NULL;
	}
	else
	{
		mModeId = ((CAtlString)CGuidStr(mMode)).AllocSysString ();
	}
}
#endif

#ifdef	_M_CEE
System::String^ CAgentFileTts::ModeId::get ()
{
	return mModeId;
}
#endif

#ifdef	_M_CEE
LANGID CAgentFileTts::Language::get ()
{
	return mLanguage;
}

void CAgentFileTts::Language::set (LANGID pValue)
{
	if	(
			(mOwner)
		&&	(pValue != 0)
		&&	(mLanguage != pValue)
		)
	{
		mLanguage = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileTts::put_Language (LANGID pLanguage)
{
	mLanguage = pLanguage;
}
#endif

#ifdef	_M_CEE
UInt16 CAgentFileTts::Gender::get ()
{
	return mGender;
}

void CAgentFileTts::Gender::set (UInt16 pValue)
{
	if	(
			(mOwner)
		&&	(pValue >= 0)
		&&	(pValue <= 2)
		&&	(mGender != pValue)
		)
	{
		mGender = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileTts::put_Gender (USHORT pGender)
{
	if	(
			(pGender >= 0)
		&&	(pGender <= 2)
		)
	{
		mGender = pGender;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileTts::ToString ()
{
    return String::Format ("Tts {0}", (ModeId ? ModeId : "<empty>"));
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileBalloon::CAgentFileBalloon (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileBalloon::Empty ()
{
	mLines = DefLines;
	mPerLine = DefPerLine;
#ifdef	_M_CEE
	mFont = nullptr;
#else
	mFont.Clear ();
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
UInt16 CAgentFileBalloon::Lines::get ()
{
	return mLines;
}

void CAgentFileBalloon::Lines::set (UInt16 pValue)
{
	pValue = min (max (pValue, MinLines), MaxLines);

	if	(
			(mOwner)
		&&	(mLines != pValue)
		)
	{
		mLines = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileBalloon::put_Lines (USHORT pLines)
{
	mLines = min (max (pLines, MinLines), MaxLines);
}
#endif

#ifdef	_M_CEE
UInt16 CAgentFileBalloon::PerLine::get ()
{
	return mPerLine;
}

void CAgentFileBalloon::PerLine::set (UInt16 pValue)
{
	pValue = min (max (pValue, MinPerLine), MaxPerLine);

	if	(
			(mOwner)
		&&	(mPerLine != pValue)
		)
	{
		mPerLine = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileBalloon::put_PerLine (USHORT pPerLine)
{
	mPerLine = min (max (pPerLine, MinPerLine), MaxPerLine);
}
#endif

#ifdef	_M_CEE
System::Drawing::Color CAgentFileBalloon::FgColor::get ()
{
	return mFgColor;
}

void CAgentFileBalloon::FgColor::set (System::Drawing::Color pValue)
{
	if	(
			(mOwner)
		&&	(mFgColor != pValue)
		)
	{
		mFgColor = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileBalloon::put_FgColor (COLORREF pFgColor)
{
	mFgColor = pFgColor;
}
#endif

#ifdef	_M_CEE
System::Drawing::Color CAgentFileBalloon::BkColor::get ()
{
	return mBkColor;
}

void CAgentFileBalloon::BkColor::set (System::Drawing::Color pValue)
{
	if	(
			(mOwner)
		&&	(mBkColor != pValue)
		)
	{
		mBkColor = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileBalloon::put_BkColor (COLORREF pBkColor)
{
	mBkColor = pBkColor;
}
#endif

#ifdef	_M_CEE
System::Drawing::Color CAgentFileBalloon::BrColor::get ()
{
	return mBrColor;
}

void CAgentFileBalloon::BrColor::set (System::Drawing::Color pValue)
{
	if	(
			(mOwner)
		&&	(mBrColor != pValue)
		)
	{
		mBrColor = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileBalloon::put_BrColor (COLORREF pBrColor)
{
	mBrColor = pBrColor;
}
#endif

#ifdef	_M_CEE
System::Drawing::Font^ CAgentFileBalloon::Font::get ()
{
	return mFont;
}

void CAgentFileBalloon::Font::set (System::Drawing::Font^ pValue)
{
	if	(
			(mOwner)
		&&	(
				((!mFont) != (!pValue))
			||	(
					(mFont)
				&&	(pValue)
				&&	(!mFont->Equals (pValue))
				)
			)
		)
	{
		mFont = pValue;
		mOwner->IsDirty = true;
	}
}
#else
void CAgentFileBalloon::put_Font (const LOGFONT& pFont)
{
	mFont = pFont;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileBalloon::ToString ()
{
    return String::Format ("Balloon {0:D} {1:D}", Lines, PerLine);
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
UInt16 CAgentFileBalloon::MinLines::get() {return 1;}
UInt16 CAgentFileBalloon::MaxLines::get() {return 128;}
UInt16 CAgentFileBalloon::DefLines::get() {return 2;}
UInt16 CAgentFileBalloon::MinPerLine::get() {return 8;}
UInt16 CAgentFileBalloon::MaxPerLine::get() {return 255;}
UInt16 CAgentFileBalloon::DefPerLine::get() {return 32;}
#else
const USHORT CAgentFileBalloon::MinLines = 1;
const USHORT CAgentFileBalloon::MaxLines = 128;
const USHORT CAgentFileBalloon::DefLines = 2;
const USHORT CAgentFileBalloon::MinPerLine = 8;
const USHORT CAgentFileBalloon::MaxPerLine = 255;
const USHORT CAgentFileBalloon::DefPerLine = 32;
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileImage::Empty ()
{
	mImageNum = 0;
	mIs32Bit = false;
#ifdef	_M_CEE
	mImageSize.Width = 0;
	mImageSize.Height = 0;
	mBits = nullptr;
#else
	mImageSize.cx = mImageSize.cy = 0;
	mBitsSize = 0;
	mBits = NULL;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
UInt32 CAgentFileImage::ImageNum::get()
{
	return mImageNum;
}

System::Drawing::Size CAgentFileImage::ImageSize::get()
{
	return mImageSize;
}

Boolean CAgentFileImage::Is32Bit::get()
{
	return mIs32Bit;
}

UInt32 CAgentFileImage::BitsSize::get()
{
	return (mBits) ? (UInt32)mBits->Length : 0;
}

array <BYTE>^ CAgentFileImage::Bits::get()
{
	return mBits;
}
#else
void CAgentFileImage::put_ImageSize (const CSize& pImageSize)
{
	mImageSize = pImageSize;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileImage::ToString ()
{
    return String::Format ("FileImage {0}", ImageSize.ToString());
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrameImage::CAgentFileFrameImage (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileFrameImage::Empty ()
{
	mImageNdx = 0;
#ifdef	_M_CEE
	mOffset.X = 0;
	mOffset.Y = 0;
#else
	mOffset.x = mOffset.y = 0;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
Int32 CAgentFileFrameImage::ImageNdx::get()
{
	return mImageNdx;
}

System::String^ CAgentFileFrameImage::ImageFilePath::get()
{
	if	(mOwner)
	{
		return mOwner->GetImageFilePath (mImageNdx);
	}
	return nullptr;
}

void CAgentFileFrameImage::ImageFilePath::set (System::String^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pValue))
		)
	{
		Int32	lImageNdx = mOwner->LoadImageFile (pValue);

		if	(
				(lImageNdx >= 0)
			&&	(mImageNdx != lImageNdx)
			)
		{
			mImageNdx = lImageNdx;
			mOwner->IsDirty = true;
		}
	}
}

System::Drawing::Point CAgentFileFrameImage::Offset::get()
{
	return mOffset;
}

void CAgentFileFrameImage::Offset::set (System::Drawing::Point pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mOffset != pValue)
		)
	{
		mOffset = pValue;
		mOwner->IsDirty = true;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
Boolean CAgentFileFrameImage::CopyTo (CAgentFileFrameImage^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mOwner == mOwner)
		)
	{
		pTarget->mImageNdx = mImageNdx;
		pTarget->mOffset = mOffset;
		return true;
	}
	return false;
}

System::String^ CAgentFileFrameImage::ToString ()
{
    return String::Format ("FrameImage {0:D} {%1}", ImageNdx, Offset.ToString());
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrameImages::CAgentFileFrameImages ()
{
}

CAgentFileFrameImages::CAgentFileFrameImages (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
}

CAgentFileFrameImage^ CAgentFileFrameImages::Add (System::String^ pFrameImagePath)
{
	return Insert (Count, pFrameImagePath);
}

CAgentFileFrameImage^ CAgentFileFrameImages::Insert (Int32 pNdx, System::String^ pFrameImagePath)
{
	CAgentFileFrameImage^	lImage = nullptr;

	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pFrameImagePath))
		)
	{
		lImage = gcnew CAgentFileFrameImage (mOwner);
		lImage->mImageNdx = mOwner->LoadImageFile (pFrameImagePath);

		if	(pNdx < 0)
		{
			pNdx = Count;
		}
		__super::Insert (min ((int)pNdx, Count), lImage);
		mOwner->IsDirty = true;
	}
	return lImage;
}

bool CAgentFileFrameImages::Remove (CAgentFileFrameImage^ pItem)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pItem))
		)
	{
		RemoveItem (IndexOf (pItem));
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileFrameImages::RemoveAt (Int32 pNdx)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(pNdx >= 0)
		&&	(pNdx < Count)
		)
	{
		RemoveItem (pNdx);
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileFrameImages::Move (Int32 pNdx, Int32 pNewNdx)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(pNdx >= 0)
		&&	((int)pNdx < Count)
		)
	{
		pNewNdx = (Int32)min (max ((int)pNewNdx, 0), Count-1);
		if	(pNewNdx != pNdx)
		{
			CAgentFileFrameImage^	lImage = this [pNdx];

			__super::Remove (lImage);
			__super::Insert (pNewNdx, lImage);
			mOwner->IsDirty = true;
			return true;
		}
	}
	return false;
}

bool CAgentFileFrameImages::Move (CAgentFileFrameImage^ pItem, Int32 pNewNdx)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pItem))
		)
	{
		return Move (IndexOf (pItem), pNewNdx);
	}
	return false;
}

bool CAgentFileFrameImages::Move (CAgentFileFrameImage^ pItem, CAgentFileFrameImage^ pBefore)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pItem))
		&&	(
				(!pBefore)
			||	(Contains (pBefore))
			)
		)
	{
		int	lNdx = IndexOf (pItem);
		int	lNewNdx = (pBefore) ? IndexOf (pBefore) : Count;

		if	(lNewNdx > lNdx)
		{
			lNewNdx--;
		}
		return Move (lNdx, lNewNdx);
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrameOverlay::CAgentFileFrameOverlay (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileFrameOverlay::Empty ()
{
	mOverlayType = (AgentMouthOverlay)0;
	mImageNdx = 0;
	mReplaceFlag = false;
#ifdef	_M_CEE
	mOffset.X = 0;
	mOffset.Y = 0;
#else
	mOffset.x = mOffset.y = 0;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
AgentMouthOverlay CAgentFileFrameOverlay::OverlayType::get()
{
	return mOverlayType;
}

void CAgentFileFrameOverlay::OverlayType::set (AgentMouthOverlay pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(pValue >= AgentMouthOverlay::MouthOverlayMin)
		&&	(pValue <= AgentMouthOverlay::MouthOverlayMax)
		&&	(mOverlayType != pValue)
		)
	{
		mOverlayType = pValue;
		mOwner->IsDirty = true;
	}
}

Int32 CAgentFileFrameOverlay::ImageNdx::get()
{
	return mImageNdx;
}

System::String^ CAgentFileFrameOverlay::ImageFilePath::get()
{
	if	(mOwner)
	{
		return mOwner->GetImageFilePath (mImageNdx);
	}
	return nullptr;
}

void CAgentFileFrameOverlay::ImageFilePath::set (System::String^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pValue))
		)
	{
		Int32	lImageNdx = mOwner->LoadImageFile (pValue);

		if	(
				(lImageNdx >= 0)
			&&	(mImageNdx != lImageNdx)
			)
		{
			mImageNdx = lImageNdx;
			mOwner->IsDirty = true;
		}
	}
}

Boolean CAgentFileFrameOverlay::ReplaceFlag::get()
{
	return mReplaceFlag;
}

void CAgentFileFrameOverlay::ReplaceFlag::set (Boolean pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mReplaceFlag != pValue)
		)
	{
		mReplaceFlag = pValue;
		mOwner->IsDirty = true;
	}
}

System::Drawing::Point CAgentFileFrameOverlay::Offset::get()
{
	return mOffset;
}

void CAgentFileFrameOverlay::Offset::set (System::Drawing::Point pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mOffset != pValue)
		)
	{
		mOffset = pValue;
		mOwner->IsDirty = true;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
Boolean CAgentFileFrameOverlay::CopyTo (CAgentFileFrameOverlay^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mOwner == mOwner)
		)
	{
		pTarget->mOverlayType = mOverlayType;
		pTarget->mImageNdx = mImageNdx;
		pTarget->mReplaceFlag = mReplaceFlag;
		pTarget->mOffset = mOffset;
		return true;
	}
	return false;
}

System::String^ CAgentFileFrameOverlay::ToString ()
{
    return String::Format ("FrameOverlay {0:D} {%1}", OverlayType, Offset.ToString());
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrameOverlays::CAgentFileFrameOverlays ()
{
}

CAgentFileFrameOverlays::CAgentFileFrameOverlays (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
}

AgentMouthOverlay CAgentFileFrameOverlays::GetKeyForItem (CAgentFileFrameOverlay^ pItem)
{
	return pItem->OverlayType;
}

CAgentFileFrameOverlay^ CAgentFileFrameOverlays::Add (AgentMouthOverlay pOverlayType, System::String^ pOverlayImagePath)
{
	CAgentFileFrameOverlay^	lOverlay = nullptr;

	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!Contains (pOverlayType))
		&&	(pOverlayType >= AgentMouthOverlay::MouthOverlayMin)
		&&	(pOverlayType <= AgentMouthOverlay::MouthOverlayMax)
		&&	(!String::IsNullOrEmpty (pOverlayImagePath))
		)
	{
		lOverlay = gcnew CAgentFileFrameOverlay (mOwner);
		lOverlay->mOverlayType = pOverlayType;
		lOverlay->mImageNdx = mOwner->LoadImageFile (pOverlayImagePath);

		__super::Add (lOverlay);
		mOwner->IsDirty = true;
	}
	return lOverlay;
}

bool CAgentFileFrameOverlays::Remove (AgentMouthOverlay pOverlayType)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(__super::Remove (pOverlayType))
		)
	{
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileFrameOverlays::Remove (CAgentFileFrameOverlay^ pItem)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(__super::Remove (pItem))
		)
	{
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrame::CAgentFileFrame (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileFrame::Empty ()
{
	mDuration = 0;
	mExitFrame = -2;
	mSoundNdx = -1;
#ifdef	_M_CEE
	mBranching = nullptr;
	if (mOwner)
	{
		mImages = gcnew CAgentFileFrameImages (mOwner);
		mOverlays = gcnew CAgentFileFrameOverlays (mOwner);
	}
	else
	{
		mImages = nullptr;
		mOverlays = nullptr;
	}
#else
	memset (mBranching, 0, sizeof(mBranching));
	mBranching [0].mFrameNdx = -1;
	mBranching [1].mFrameNdx = -1;
	mBranching [2].mFrameNdx = -1;
	mImageCount = 0;
	mImages = NULL;
	mOverlayCount = 0;
	mOverlays = NULL;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
UInt16 CAgentFileFrame::Duration::get()
{
	return mDuration;
}

void CAgentFileFrame::Duration::set (UInt16 pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mDuration != pValue)
		)
	{
		mDuration = pValue;
		mOwner->IsDirty = true;
	}
}

Int16 CAgentFileFrame::SoundNdx::get()
{
	return mSoundNdx;
}

System::String^ CAgentFileFrame::SoundFilePath::get()
{
	if	(
			(mOwner)
		&&	(mSoundNdx >= 0)
		)
	{
		return mOwner->GetSoundFilePath (mSoundNdx);
	}
	return nullptr;
}

void CAgentFileFrame::SoundFilePath::set (System::String^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		)
	{
		if	(String::IsNullOrEmpty (pValue))
		{
			if	(mSoundNdx >= 0)
			{
				mSoundNdx = -1;
				mOwner->IsDirty = true;
			}
		}
		else
		{
			Int32	lSoundNdx = mOwner->LoadSoundFile (pValue);

			if	(
					(lSoundNdx >= 0)
				&&	(mSoundNdx != lSoundNdx)
				)
			{
				mSoundNdx = lSoundNdx;
				mOwner->IsDirty = true;
			}
		}
	}
}

Int16 CAgentFileFrame::ExitFrame::get()
{
	return mExitFrame;
}

void CAgentFileFrame::ExitFrame::set (Int16 pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mExitFrame != pValue)
		)
	{
		mExitFrame = pValue;
		mOwner->IsDirty = true;
	}
}

array <CAgentFileFrameBranch>^ CAgentFileFrame::Branching::get()
{
	return mBranching;
}

void CAgentFileFrame::Branching::set (array <CAgentFileFrameBranch>^ pValue)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mBranching != pValue)
		)
	{
		// Validation?
		mBranching = pValue;
		mOwner->IsDirty = true;
	}
}

UInt16 CAgentFileFrame::ImageCount::get()
{
	return (mImages) ? (UInt16)mImages->Count : 0;
}

CAgentFileFrameImages^ CAgentFileFrame::Images::get()
{
	return mImages;
}

UInt16 CAgentFileFrame::OverlayCount::get()
{
	return (mOverlays) ? (UInt16)mOverlays->Count : 0;
}

CAgentFileFrameOverlays^ CAgentFileFrame::Overlays::get()
{
	return mOverlays;
}
#else
#ifdef	_DEBUG
USHORT CAgentFileFrame::get_Duration () const
{
#ifdef	_DEBUG_SEQUENCE_FRAMES
	if	(LogIsActive (_DEBUG_SEQUENCE_FRAMES))
	{
		return mDuration * 10;
	}
#endif
	return mDuration;
}
#endif
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
Boolean CAgentFileFrame::CopyTo (CAgentFileFrame^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mOwner == mOwner)
		)
	{
		pTarget->mDuration = mDuration;
		pTarget->mSoundNdx = mSoundNdx;
		pTarget->mExitFrame = mExitFrame;
		pTarget->mBranching = mBranching;
		pTarget->mImages = mImages;
		pTarget->mOverlays = mOverlays;
		return true;
	}
	return false;
}

System::String^ CAgentFileFrame::ToString ()
{
    return String::Format ("Frame {0:D} Images {1:D} Overlays {2:D} Sound {3:D}", Duration, ImageCount, OverlayCount, SoundNdx);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrames::CAgentFileFrames ()
{
}

CAgentFileFrames::CAgentFileFrames (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileFrame^ CAgentFileFrames::Add ()
{
	return Insert (Count);
}

CAgentFileFrame^ CAgentFileFrames::Insert (Int32 pNdx)
{
	CAgentFileFrame^	lFrame = nullptr;

	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		)
	{
		lFrame = gcnew CAgentFileFrame (mOwner);
		lFrame->mDuration = mOwner->NewFrameDuration;

		if	(pNdx < 0)
		{
			pNdx = Count;
		}
		__super::Insert (min ((int)pNdx, Count), lFrame);
		mOwner->IsDirty = true;
	}
	return lFrame;
}

bool CAgentFileFrames::Remove (CAgentFileFrame^ pItem)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pItem))
		)
	{
		__super::Remove (pItem);
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileFrames::RemoveAt (Int32 pNdx)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(pNdx >= 0)
		&&	((int)pNdx < Count)
		)
	{
		__super::RemoveAt (pNdx);
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileFrames::Move (Int32 pNdx, Int32 pNewNdx)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(pNdx >= 0)
		&&	((int)pNdx < Count)
		)
	{
		pNewNdx = (Int32)min (max ((int)pNewNdx, 0), Count-1);
		if	(pNewNdx != pNdx)
		{
			CAgentFileFrame^	lFrame = this [pNdx];

			__super::Remove (lFrame);
			__super::Insert (pNewNdx, lFrame);
			mOwner->IsDirty = true;
			return true;
		}
	}
	return false;
}

bool CAgentFileFrames::Move (CAgentFileFrame^ pItem, Int32 pNewNdx)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pItem))
		)
	{
		return Move (IndexOf (pItem), pNewNdx);
	}
	return false;
}

bool CAgentFileFrames::Move (CAgentFileFrame^ pItem, CAgentFileFrame^ pBefore)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pItem))
		&&	(
				(!pBefore)
			||	(Contains (pBefore))
			)
		)
	{
		int	lNdx = IndexOf (pItem);
		int	lNewNdx = (pBefore) ? IndexOf (pBefore) : Count;

		if	(lNewNdx > lNdx)
		{
			lNewNdx--;
		}
		return Move (lNdx, lNewNdx);
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileAnimation::CAgentFileAnimation (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
	Empty ();
}
#endif

void CAgentFileAnimation::Empty ()
{
	mAcaChksum = 0;
	mReturnType = 2;
#ifdef	_M_CEE
	mName = String::Empty;
	mReturnName = nullptr;
	mAcaFileName = nullptr;
	if	(mOwner)
	{
		mFrames = gcnew CAgentFileFrames (mOwner);
	}
	else
	{
		mFrames = nullptr;
	}
#else
	mName = NULL;
	mReturnName = NULL;
	mAcaFileName = NULL;
	mFrameCount = 0;
	mFrames = NULL;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileAnimation::Name::get()
{
	return mName;
}

void CAgentFileAnimation::Name::set (System::String^ pName)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pName))
		&&	(
				(String::IsNullOrEmpty (mName))
			||	(!mName->Equals (pName))
			)
		&&	(mOwner->RenameAnimation (this, pName))
		)
	{
		mOwner->IsDirty = true;
	}
}

Byte CAgentFileAnimation::ReturnType::get()
{
	return mReturnType;
}

void CAgentFileAnimation::ReturnType::set (Byte pReturnType)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(mReturnType != pReturnType)
		)
	{
		mReturnType = Math::Min (pReturnType, (Byte)2);
		if	(mReturnType > 0)
		{
			mReturnName = nullptr;
		}
		mOwner->IsDirty = true;
	}
}

System::String^ CAgentFileAnimation::ReturnName::get()
{
	return mReturnName;
}

void CAgentFileAnimation::ReturnName::set (System::String^ pReturnName)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(
				(String::IsNullOrEmpty (mReturnName) != String::IsNullOrEmpty (pReturnName))
			||	(
					(mReturnName)
				&&	(pReturnName)
				&&	(!mReturnName->Equals (pReturnName))
				)
			)
		)
	{
		mReturnName = pReturnName;
		if (String::IsNullOrEmpty (mReturnName))
		{
			if (mReturnType == 0)
			{
				mReturnType = 2;
			}
		}
		else
		{
			mReturnType = 0;
		}
		mOwner->IsDirty = true;
	}
}

UInt16 CAgentFileAnimation::FrameCount::get()
{
	return (mFrames) ? (UInt16)mFrames->Count : 0;
}

CAgentFileFrames^ CAgentFileAnimation::Frames::get()
{
	return mFrames;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
Boolean CAgentFileAnimation::CopyTo (CAgentFileAnimation^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mOwner == mOwner)
		)
	{
		pTarget->mName = mName;
		pTarget->mReturnType = mReturnType;
		pTarget->mReturnName = mReturnName;
		pTarget->mFrames = mFrames;
		pTarget->mAcaFileName = mAcaFileName;
		pTarget->mAcaChksum = mAcaChksum;
		return true;
	}
	return false;
}

System::String^ CAgentFileAnimation::ToString ()
{
    return String::Format ("Animation \"{0}\"", Name);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileGestures::CAgentFileGestures ()
:	KeyedCollection <System::String^, CAgentFileAnimation^> (StringComparer::InvariantCultureIgnoreCase)
{
}

CAgentFileGestures::CAgentFileGestures (CharacterFile^ pOwner)
:	KeyedCollection <System::String^, CAgentFileAnimation^> (StringComparer::InvariantCultureIgnoreCase),
	mOwner (pOwner)
{
}

System::String^ CAgentFileGestures::GetKeyForItem (CAgentFileAnimation^ pItem)
{
	return pItem->Name;
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileAnimation^ CAgentFileGestures::Add (System::String^ pAnimationName)
{
	CAgentFileAnimation^	lAnimation = nullptr;

	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!Contains (pAnimationName))
		)
	{
		lAnimation = gcnew CAgentFileAnimation (mOwner);
		lAnimation->mName = pAnimationName;
		Add (lAnimation);
		mOwner->IsDirty = true;
	}
	return lAnimation;
}

bool CAgentFileGestures::Remove (System::String^ pAnimationName)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(Contains (pAnimationName))
		)
	{
		__super::Remove (pAnimationName);
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

bool CAgentFileGestures::Remove (CAgentFileAnimation^ pItem)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!Contains (pItem))
		)
	{
		__super::Remove (pItem);
		mOwner->IsDirty = true;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFileGestures::Add (CAgentFileAnimation^ pItem)
{
	int	lNdx;

	for (lNdx = 0; lNdx < Count; lNdx++)
	{
		if	(String::Compare (this [lNdx]->Name, pItem->Name, true) > 0)
		{
			break;
		}
	}
	__super::Insert (lNdx, pItem);
}

void CAgentFileGestures::ChangeItemKey (CAgentFileAnimation^ pItem, String^ pNewKey)
{
//	__super::ChangeItemKey (pItem, pItem->Name);
//
//	Instead - Remove/Add to resort
//	
	__super::Remove (pItem);
	pItem->mName = pNewKey;
	Add (pItem);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileStates::CAgentFileStates ()
:	SortedList <System::String^, array <System::String^>^> (StringComparer::InvariantCultureIgnoreCase)
{
}

CAgentFileStates::CAgentFileStates (CharacterFile^ pOwner)
:	SortedList <System::String^, array <System::String^>^> (StringComparer::InvariantCultureIgnoreCase),
	mOwner (pOwner)
{
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileStates::AddStateAnimation (System::String^ pStateName, System::String^ pAnimationName)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pStateName))
		&&	(!String::IsNullOrEmpty (pAnimationName))
		)
	{
		array <String^>^	lAnimations = nullptr;

		if	(ContainsKey (pStateName))
		{
			lAnimations = this [pStateName];
		}
		else
		if	(lAnimations = gcnew array <String^> (0))
		{
			Add (pStateName, lAnimations);
		}
		if	(
				(lAnimations)
			&&	(!lAnimations->Find (lAnimations, gcnew Predicate <String^> (pAnimationName, &String::Equals)))
			)
		{
			try
			{
				List <String^>^	lList = gcnew List <String^> (lAnimations);
				int				lNdx;

				for (lNdx = 0; lNdx < lList->Count; lNdx++)
				{
					if	(String::Compare (lList [lNdx], pAnimationName) > 0)
					{
						break;
					}
				}
				lList->Insert (lNdx, gcnew String (pAnimationName));
				this [pStateName] = lList->ToArray();
				mOwner->IsDirty = true;
				return true;
			}
			catch AnyExceptionDebug
		}
	}
	return false;
}

bool CAgentFileStates::RemoveStateAnimation (System::String^ pStateName, System::String^ pAnimationName)
{
	if	(
			(mOwner)
		&&	(!mOwner->IsReadOnly)
		&&	(!String::IsNullOrEmpty (pStateName))
		&&	(!String::IsNullOrEmpty (pAnimationName))
		&&	(ContainsKey (pStateName))
		)
	{
		array <String^>^	lAnimations = this [pStateName];

		if	(
				(lAnimations)
			&&	(lAnimations->Find (lAnimations, gcnew Predicate <String^> (pAnimationName, &String::Equals)))
			)
		{
			try
			{
				List <String^>^	lList = gcnew List <String^> (lAnimations);

				lList->RemoveAt (lList->FindIndex (gcnew Predicate <String^> (pAnimationName, &String::Equals)));
				this [pStateName] = lList->ToArray();
				mOwner->IsDirty = true;
				return true;
			}
			catch AnyExceptionDebug
		}
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
