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

#ifdef	_M_CEE
#ifdef	_DEBUG
//#define	_DEBUG_INSTANCE			LogNormal
//#define	_DEBUG_INSTANCE_OWNED	MinLogLevel(mOwner->mLogLevel,_DEBUG_INSTANCE)
//#define	_DEBUG_COPY				LogNormal
#endif
#else
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

#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
static gcroot<List<CAgentFileHeader^> ^>			sHeaderInstances = gcnew List<CAgentFileHeader^>;
static gcroot<List<CAgentFileTts^> ^>				sTtsInstances = gcnew List<CAgentFileTts^>;
static gcroot<List<CAgentFileBalloon^> ^>			sBalloonInstances = gcnew List<CAgentFileBalloon^>;
static gcroot<List<CAgentFileAnimation^> ^>			sAnimationInstances = gcnew List<CAgentFileAnimation^>;
static gcroot<List<CAgentFileFrame^> ^>				sFrameInstances = gcnew List<CAgentFileFrame^>;
static gcroot<List<CAgentFileFrames^> ^>			sFramesInstances = gcnew List<CAgentFileFrames^>;
static gcroot<List<CAgentFileFrameImage^> ^>		sFrameImageInstances = gcnew List<CAgentFileFrameImage^>;
static gcroot<List<CAgentFileFrameImages^> ^>		sFrameImagesInstances = gcnew List<CAgentFileFrameImages^>;
static gcroot<List<CAgentFileFrameOverlay^> ^>		sFrameOverlayInstances = gcnew List<CAgentFileFrameOverlay^>;
static gcroot<List<CAgentFileFrameOverlays^> ^>		sFrameOverlaysInstances = gcnew List<CAgentFileFrameOverlays^>;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileHeader::CAgentFileHeader ()
#ifndef	_M_CEE
:	mIcon (NULL)
#endif
{
#ifdef	_DEBUG_INSTANCE
	sHeaderInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileHeader::CAgentFileHeader"), sHeaderInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileHeader::CAgentFileHeader (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
#ifdef	_DEBUG_INSTANCE
	sHeaderInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileHeader::CAgentFileHeader (Owned)"), sHeaderInstances->IndexOf(this));
#endif
#endif
	Empty ();
}
#endif

CAgentFileHeader::~CAgentFileHeader ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileHeader::~CAgentFileHeader (Owned)"), sHeaderInstances->IndexOf(this));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileHeader::~CAgentFileHeader"), sHeaderInstances->IndexOf(this));
#endif
	sHeaderInstances->Remove (this);
#endif
#endif
}

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
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%d] CharacterFile::DestroyIcon [%p]"), this, mIcon);
		}
#endif

		DestroyIcon (mIcon);

#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%d] CharacterFile::DestroyIcon [%p] Done"), this, mIcon);
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
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileHeader::ToString ()
{
    return String::Format ("Style [{0}] ImageSize {1:D},{2:D} Transparency {3} GUID {4}", StyleString(Style), ImageSize.Width, ImageSize.Height, Transparency, Guid.ToString()->ToUpper());
}
#endif

#ifdef	_M_CEE
System::String^ CAgentFileHeader::StyleString (AgentCharStyle pStyle)
#else
CAtlString CAgentFileHeader::StyleString (AgentCharStyle pStyle)
#endif
{
#ifdef	_M_CEE
	String^		lStyle = String::Empty;
#else
	CAtlString	lStyle;
#endif

	if	((DWORD)pStyle & (DWORD)AgentCharStyle::CharStyleStandard)
	{
		lStyle += "Standard ";
	}
	if	((DWORD)pStyle & (DWORD)AgentCharStyle::CharStyleTts)
	{
		lStyle += "Tts ";
	}
	if	((DWORD)pStyle & (DWORD)AgentCharStyle::CharStyleBalloon)
	{
		lStyle += "Balloon ";
	}
	if	((DWORD)pStyle & (DWORD)AgentCharStyle::CharStyleSizeToText)
	{
		lStyle += "SizeToText ";
	}
	if	((DWORD)pStyle & (DWORD)AgentCharStyle::CharStyleNoAutoHide)
	{
		lStyle += "NoAutoHide ";
	}
	if	((DWORD)pStyle & (DWORD)AgentCharStyle::CharStyleNoAutoPace)
	{
		lStyle += "NoAutoPace ";
	}

#ifdef	_M_CEE
	return lStyle->Trim();
#else
	return lStyle.Trim();
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileName::CAgentFileName ()
{
	Empty ();
}

#ifdef	_M_CEE
CAgentFileName::CAgentFileName (LANGID pLanguage, System::String^ pName)
{
	Empty ();
	mLanguage = pLanguage;
	mName = pName;
}

CAgentFileName::CAgentFileName (LANGID pLanguage, CAgentFileName^ pSource)
{
	Empty ();
	mLanguage = pLanguage;
	pSource->CopyTo (this);
}

CAgentFileName::CAgentFileName (CharacterFile^ pOwner, FileCharacterNames^ pContainer)
:	mOwner (pOwner),
	mContainer (pContainer)
{
	Empty ();
}
#endif

CAgentFileName::~CAgentFileName ()
{
}

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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!String::IsNullOrEmpty (pValue))
		&&	(
				(String::IsNullOrEmpty (mName))
			||	(!mName->Equals (pValue))
			)
		)
	{
		mName = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
/////////////////////////////////////////////////////////////////////////////

Boolean CAgentFileName::CopyTo (CAgentFileName^ pTarget)
{
	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mName = (mName) ? gcnew String (mName) : nullptr;
		pTarget->mDesc1 = (mDesc1) ? gcnew String (mDesc1) : nullptr;
		pTarget->mDesc2 = (mDesc2) ? gcnew String (mDesc2) : nullptr;

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

Boolean CAgentFileName::Equals (CAgentFileName^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mLanguage == mLanguage)
		&&	(String::Compare (pTarget->mName, mName) == 0)
		&&	(String::Compare (pTarget->mDesc1, mDesc1) == 0)
		&&	(String::Compare (pTarget->mDesc2, mDesc2) == 0)
		)
	{
		return true;
	}
	return false;
}

System::String^ CAgentFileName::ToString ()
{
#ifdef	_DEBUG_NOT
    return String::Format (" Name {0:X} {0:D} \"{1}\" \"{2}\" \"{3}\" \"{4}\"", Language, (gcnew System::Globalization::CultureInfo (Language))->EnglishName, Name, Desc1, Desc2);
#else
    return String::Format (" Name {0:X} {0:D} \"{1}\"", Language, (gcnew System::Globalization::CultureInfo (Language))->EnglishName);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

CAgentFileNames::CAgentFileNames ()
{
}

CAgentFileNames::~CAgentFileNames ()
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!Contains (pLangID))
		&&	(!String::IsNullOrEmpty (pName))
		)
	{
		lFileName = gcnew CAgentFileName (mOwner, this);
		lFileName->mLanguage = pLangID;
		lFileName->mName = pName;

		__super::Add (lFileName);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
	return lFileName;
}

bool CAgentFileNames::Remove (LANGID pLangID)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(__super::Remove (pLangID))
		)
	{
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileNames::Remove (CAgentFileName^ pItem)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(__super::Remove (pItem))
		)
	{
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileTts::CAgentFileTts ()
{
#ifdef	_DEBUG_INSTANCE
	sTtsInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileTts::CAgentFileTts"), sTtsInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileTts::CAgentFileTts (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
#ifdef	_DEBUG_INSTANCE
	sTtsInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileTts::CAgentFileTts (Owned)"), sTtsInstances->IndexOf(this));
#endif
#endif
	Empty ();
}
#endif

CAgentFileTts::~CAgentFileTts ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileTts::~CAgentFileTts (Owned)"), sTtsInstances->IndexOf(this));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileTts::~CAgentFileTts"), sTtsInstances->IndexOf(this));
#endif
	sTtsInstances->Remove (this);
#endif
#endif
}

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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!mEngine.Equals (pValue))
		)
	{
		mEngine = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(pValue != 0)
		&&	(mLanguage != pValue)
		)
	{
		mLanguage = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(pValue >= 0)
		&&	(pValue <= 2)
		&&	(mGender != pValue)
		)
	{
		mGender = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
/////////////////////////////////////////////////////////////////////////////

Boolean CAgentFileTts::CopyTo (CAgentFileTts^ pTarget)
{
	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mEngine = mEngine;
		pTarget->mMode = mMode;
		pTarget->mModeId = (mModeId) ? gcnew String (mModeId) : nullptr;
		pTarget->mLanguage = mLanguage;
		pTarget->mGender = mGender;

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

Boolean CAgentFileTts::Equals (CAgentFileTts^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mEngine == mEngine)
		&&	(pTarget->mMode == mMode)
		&&	(String::Compare (pTarget->mModeId, mModeId) == 0)
		&&	(pTarget->mLanguage == mLanguage)
		&&	(pTarget->mGender == mGender)
		)
	{
		return true;
	}
	return false;
}

System::String^ CAgentFileTts::ToString ()
{
    return String::Format ("Tts {0}", (ModeId ? ModeId : "<empty>"));
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileBalloon::CAgentFileBalloon ()
{
#ifdef	_DEBUG_INSTANCE
	sBalloonInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileBalloon::CAgentFileBalloon"), sBalloonInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileBalloon::CAgentFileBalloon (CharacterFile^ pOwner)
:	mOwner (pOwner)
{
#ifdef	_DEBUG_INSTANCE
	sBalloonInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileBalloon::CAgentFileBalloon (Owned)"), sBalloonInstances->IndexOf(this));
#endif
#endif
	Empty ();
}
#endif

CAgentFileBalloon::~CAgentFileBalloon ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileBalloon::~CAgentFileBalloon (Owned)"), sBalloonInstances->IndexOf(this));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileBalloon::~CAgentFileBalloon"), sBalloonInstances->IndexOf(this));
#endif
	sBalloonInstances->Remove (this);
#endif
#endif
}

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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mLines != pValue)
		)
	{
		mLines = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mPerLine != pValue)
		)
	{
		mPerLine = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mFgColor != pValue)
		)
	{
		mFgColor = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mBkColor != pValue)
		)
	{
		mBkColor = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mBrColor != pValue)
		)
	{
		mBrColor = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
#ifdef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

Boolean CAgentFileBalloon::CopyTo (CAgentFileBalloon^ pTarget)
{
	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mLines = mLines;
		pTarget->mPerLine = mPerLine;
		pTarget->mFgColor = mFgColor;
		pTarget->mBkColor = mBkColor;
		pTarget->mBrColor = mBrColor;
		pTarget->mFont = (mFont) ? safe_cast <System::Drawing::Font^> (mFont->Clone()) : nullptr;

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

Boolean CAgentFileBalloon::Equals (CAgentFileBalloon^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mLines == mLines)
		&&	(pTarget->mPerLine == mPerLine)
		&&	(pTarget->mFgColor == mFgColor)
		&&	(pTarget->mBkColor == mBkColor)
		&&	(pTarget->mBrColor == mBrColor)
		&&	(
				((pTarget->mFont == nullptr) == (mFont == nullptr))
			&&	(
					(pTarget->mFont == nullptr)
				||	(pTarget->mFont->Equals (mFont))
				)
			)
		)
	{
		return true;
	}
	return false;
}

System::String^ CAgentFileBalloon::ToString ()
{
    return String::Format ("Balloon {0:D} {1:D}", Lines, PerLine);
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileFrameImage::CAgentFileFrameImage ()
{
#ifdef	_DEBUG_INSTANCE
	sFrameImageInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameImage::CAgentFileFrameImage"), sFrameImageInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileFrameImage::CAgentFileFrameImage (CharacterFile^ pOwner, FileAnimationFrame^ pFrame)
:	mOwner (pOwner),
	mContainer (pFrame->Images)
{
#ifdef	_DEBUG_INSTANCE
	sFrameImageInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileFrameImage::CAgentFileFrameImage (Owned) [%d]"), sFrameImageInstances->IndexOf(this), sFrameImagesInstances->IndexOf(mContainer));
#endif
#endif
	Empty ();
}
#endif

CAgentFileFrameImage::~CAgentFileFrameImage ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameImage::~CAgentFileFrameImage (Owned) [%d]"), sFrameImageInstances->IndexOf(this), sFrameImagesInstances->IndexOf(mContainer));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameImage::~CAgentFileFrameImage"), sFrameImageInstances->IndexOf(this));
#endif
	sFrameImageInstances->Remove (this);
#endif
#endif
}

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
/////////////////////////////////////////////////////////////////////////////

FileAnimationFrame^ CAgentFileFrameImage::Frame::get()
{
	return (mContainer) ? mContainer->Frame : nullptr;
}

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
	else
	{
		return mImageFilePath;
	}
}

void CAgentFileFrameImage::ImageFilePath::set (System::String^ pValue)
{
	if	(!String::IsNullOrEmpty (pValue))
	{
		if	(
				(mOwner)
			&&	(!mOwner->IsReadOnly)
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
		else
		if	(!mOwner)
		{
			mImageFilePath = gcnew String (pValue);
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mOffset != pValue)
		)
	{
		mOffset = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

Boolean CAgentFileFrameImage::CopyTo (CAgentFileFrameImage^ pTarget)
{
	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mOffset = mOffset;
		if	(
				(!mOwner)
			||	(!pTarget->mOwner)
			||	(!Object::ReferenceEquals (mOwner, pTarget->mOwner))
			)
		{
			pTarget->ImageFilePath = ImageFilePath;
		}
		else
		{
			pTarget->mImageNdx = mImageNdx;
		}

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

Boolean CAgentFileFrameImage::Equals (CAgentFileFrameImage^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mOffset == mOffset)
		)
	{
		if	(
				(!mOwner)
			||	(!pTarget->mOwner)
			||	(!Object::ReferenceEquals (mOwner, pTarget->mOwner))
			)
		{
			if	(String::Equals (ImageFilePath, pTarget->ImageFilePath))
			{
				return true;
			}
		}
		else
		if	(pTarget->mImageNdx == mImageNdx)
		{
			return true;
		}
	}
	return false;
}

System::String^ CAgentFileFrameImage::ToString ()
{
#ifdef	_DEBUG_NOT
    return String::Format ("FrameImage {0:D} file {1:D} {2} at {3} in {4}", (mContainer)?mContainer->IndexOf(this):-1, ImageNdx, ImageFilePath, Offset.ToString(), (mContainer && mContainer->mFrame)?mContainer->mFrame->ToString():"<unowned>");
#else
    return String::Format ("FrameImage {0:D} in {1}", (mContainer)?mContainer->IndexOf(this):-1, (mContainer && mContainer->mFrame)?mContainer->mFrame->ToString():"<unowned>");
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFileFrameImage::OnSerializing (StreamingContext pContext)
{
	if	(mOwner)
	{
		mImageFilePath = ImageFilePath;
		mImage = mOwner->GetImage (ImageNdx);
	}
	else
	{
		mImageFilePath = nullptr;
		mImage = nullptr;
	}
}

void CAgentFileFrameImage::OnSerialized (StreamingContext pContext)
{
	mImageFilePath = nullptr;
	mImage = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

CAgentFileFrameImages::CAgentFileFrameImages ()
{
#ifdef	_DEBUG_INSTANCE
	sFrameImagesInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameImages::CAgentFileFrameImages"), sFrameImagesInstances->IndexOf(this));
#endif
}

CAgentFileFrameImages::CAgentFileFrameImages (CharacterFile^ pOwner, FileAnimationFrame^ pFrame)
:	mOwner (pOwner),
	mFrame (pFrame)
{
#ifdef	_DEBUG_INSTANCE
	sFrameImagesInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileFrameImages::CAgentFileFrameImages (Owned) [%d]"), sFrameImagesInstances->IndexOf(this), sFrameInstances->IndexOf(mFrame));
#endif
#endif
}

CAgentFileFrameImages::~CAgentFileFrameImages ()
{
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameImages::~CAgentFileFrameImages (Owned) [%d]"), sFrameImagesInstances->IndexOf(this), sFrameInstances->IndexOf(mFrame));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameImages::~CAgentFileFrameImages"), sFrameImagesInstances->IndexOf(this));
#endif
	sFrameImagesInstances->Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileFrameImage^ CAgentFileFrameImages::Add (System::String^ pFrameImagePath)
{
	return Insert (Count, pFrameImagePath);
}

CAgentFileFrameImage^ CAgentFileFrameImages::Insert (Int32 pNdx, System::String^ pFrameImagePath)
{
	CAgentFileFrameImage^	lImage = nullptr;

	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(
				(mOwner)
			||	(String::IsNullOrEmpty (pFrameImagePath))
			)
		)
	{
		lImage = gcnew CAgentFileFrameImage (mOwner, mFrame);
		if	(
				(mOwner)
			&&	(!String::IsNullOrEmpty (pFrameImagePath))
			)
		{
			lImage->mImageNdx = mOwner->LoadImageFile (pFrameImagePath);
		}
		if	(pNdx < 0)
		{
			pNdx = Count;
		}
		__super::Insert (min ((int)pNdx, Count), lImage);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
	return lImage;
}

bool CAgentFileFrameImages::Remove (CAgentFileFrameImage^ pItem)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(Contains (pItem))
		)
	{
		RemoveItem (IndexOf (pItem));
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileFrameImages::RemoveAt (Int32 pNdx)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(pNdx >= 0)
		&&	(pNdx < Count)
		)
	{
		RemoveItem (pNdx);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileFrameImages::Move (Int32 pNdx, Int32 pNewNdx)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
			if	(mOwner)
			{
				mOwner->IsDirty = true;
			}
			return true;
		}
	}
	return false;
}

bool CAgentFileFrameImages::Move (CAgentFileFrameImage^ pItem, Int32 pNewNdx)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileFrameOverlay::CAgentFileFrameOverlay ()
{
#ifdef	_DEBUG_INSTANCE
	sFrameOverlayInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameOverlay::CAgentFileFrameOverlay"), sFrameOverlayInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileFrameOverlay::CAgentFileFrameOverlay (CharacterFile^ pOwner, FileAnimationFrame^ pFrame)
:	mOwner (pOwner),
	mContainer (pFrame->Overlays)
{
#ifdef	_DEBUG_INSTANCE
	sFrameOverlayInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileFrameOverlay::CAgentFileFrameOverlay (Owned) [%d]"), sFrameOverlayInstances->IndexOf(this), sFrameOverlaysInstances->IndexOf(mContainer));
#endif
#endif
	Empty ();
}
#endif

CAgentFileFrameOverlay::~CAgentFileFrameOverlay ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameOverlay::~CAgentFileFrameOverlay (Owned) [%d]"), sFrameOverlayInstances->IndexOf(this), sFrameOverlaysInstances->IndexOf(mContainer));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameOverlay::~CAgentFileFrameOverlay"), sFrameOverlayInstances->IndexOf(this));
#endif
	sFrameOverlayInstances->Remove (this);
#endif
#endif
}

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
/////////////////////////////////////////////////////////////////////////////

FileAnimationFrame^ CAgentFileFrameOverlay::Frame::get()
{
	return (mContainer) ? mContainer->Frame : nullptr;
}

AgentMouthOverlay CAgentFileFrameOverlay::OverlayType::get()
{
	return mOverlayType;
}

void CAgentFileFrameOverlay::OverlayType::set (AgentMouthOverlay pValue)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(pValue >= AgentMouthOverlay::MouthOverlayMin)
		&&	(pValue <= AgentMouthOverlay::MouthOverlayMax)
		&&	(mOverlayType != pValue)
		)
	{
		mOverlayType = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
	else
	{
		return mImageFilePath;
	}
}

void CAgentFileFrameOverlay::ImageFilePath::set (System::String^ pValue)
{
	if	(!String::IsNullOrEmpty (pValue))
	{
		if	(
				(mOwner)
			&&	(!mOwner->IsReadOnly)
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
		else
		if	(!mOwner)
		{
			mImageFilePath = gcnew String (pValue);
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mReplaceFlag != pValue)
		)
	{
		mReplaceFlag = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
}

System::Drawing::Point CAgentFileFrameOverlay::Offset::get()
{
	return mOffset;
}

void CAgentFileFrameOverlay::Offset::set (System::Drawing::Point pValue)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mOffset != pValue)
		)
	{
		mOffset = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

Boolean CAgentFileFrameOverlay::CopyTo (CAgentFileFrameOverlay^ pTarget)
{
	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mReplaceFlag = mReplaceFlag;
		pTarget->mOffset = mOffset;
		if	(
				(!mOwner)
			||	(!pTarget->mOwner)
			||	(!Object::ReferenceEquals (mOwner, pTarget->mOwner))
			)
		{
			pTarget->ImageFilePath = ImageFilePath;
		}
		else
		{
			pTarget->mImageNdx = mImageNdx;
		}

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

Boolean CAgentFileFrameOverlay::Equals (CAgentFileFrameOverlay^ pTarget)
{
	if	(
			(pTarget)
		&&	(pTarget->mReplaceFlag == mReplaceFlag)
		&&	(pTarget->mOffset == mOffset)
		)
	{
		if	(
				(!mOwner)
			||	(!pTarget->mOwner)
			||	(!Object::ReferenceEquals (mOwner, pTarget->mOwner))
			)
		{
			if	(String::Equals (ImageFilePath, pTarget->ImageFilePath))
			{
				return true;
			}
		}
		else
		if	(pTarget->mImageNdx == mImageNdx)
		{
			return true;
		}
	}
	return false;
}

System::String^ CAgentFileFrameOverlay::ToString ()
{
    return String::Format ("FrameOverlay {0:D} {1}", OverlayType, Offset.ToString());
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFileFrameOverlay::OnSerializing (StreamingContext pContext)
{
	if	(mOwner)
	{
		mImageFilePath = ImageFilePath;
		mImage = mOwner->GetImage (ImageNdx);
	}
	else
	{
		mImageFilePath = nullptr;
		mImage = nullptr;
	}
}

void CAgentFileFrameOverlay::OnSerialized (StreamingContext pContext)
{
	mImageFilePath = nullptr;
	mImage = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

CAgentFileFrameOverlays::CAgentFileFrameOverlays ()
{
#ifdef	_DEBUG_INSTANCE
	sFrameOverlaysInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameOverlays::CAgentFileFrameOverlays"), sFrameOverlaysInstances->IndexOf(this));
#endif
}

CAgentFileFrameOverlays::CAgentFileFrameOverlays (CharacterFile^ pOwner, FileAnimationFrame^ pFrame)
:	mOwner (pOwner),
	mFrame (pFrame)
{
#ifdef	_DEBUG_INSTANCE
	sFrameOverlaysInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileFrameOverlays::CAgentFileFrameOverlays (Owned) [%d]"), sFrameOverlaysInstances->IndexOf(this), sFrameInstances->IndexOf(mFrame));
#endif
#endif
}

CAgentFileFrameOverlays::~CAgentFileFrameOverlays ()
{
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameOverlays::~CAgentFileFrameOverlays (Owned) [%d]"), sFrameOverlaysInstances->IndexOf(this), sFrameInstances->IndexOf(mFrame));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrameOverlays::~CAgentFileFrameOverlays"), sFrameOverlaysInstances->IndexOf(this));
#endif
	sFrameOverlaysInstances->Remove (this);
#endif
}

/////////////////////////////////////////////////////////////////////////////

AgentMouthOverlay CAgentFileFrameOverlays::GetKeyForItem (CAgentFileFrameOverlay^ pItem)
{
	return pItem->OverlayType;
}

CAgentFileFrameOverlay^ CAgentFileFrameOverlays::Add (AgentMouthOverlay pOverlayType, System::String^ pOverlayImagePath)
{
	CAgentFileFrameOverlay^	lOverlay = nullptr;

	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!Contains (pOverlayType))
		&&	(pOverlayType >= AgentMouthOverlay::MouthOverlayMin)
		&&	(pOverlayType <= AgentMouthOverlay::MouthOverlayMax)
		)
	{
		lOverlay = gcnew CAgentFileFrameOverlay (mOwner, mFrame);
		lOverlay->mOverlayType = pOverlayType;
		if	(!String::IsNullOrEmpty (pOverlayImagePath))
		{
			lOverlay->mImageNdx = mOwner->LoadImageFile (pOverlayImagePath);
		}

		__super::Add (lOverlay);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
	return lOverlay;
}

bool CAgentFileFrameOverlays::Remove (AgentMouthOverlay pOverlayType)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(__super::Remove (pOverlayType))
		)
	{
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileFrameOverlays::Remove (CAgentFileFrameOverlay^ pItem)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(__super::Remove (pItem))
		)
	{
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileFrame::CAgentFileFrame ()
{
#ifdef	_DEBUG_INSTANCE
	sFrameInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrame::CAgentFileFrame"), sFrameInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileFrame::CAgentFileFrame (CharacterFile^ pOwner, FileFrames^ pContainer)
:	mOwner (pOwner),
	mContainer (pContainer)
{
#ifdef	_DEBUG_INSTANCE
	sFrameInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileFrame::CAgentFileFrame (Owned) [%d]"), sFrameInstances->IndexOf(this), sFramesInstances->IndexOf(mContainer));
#endif
#endif
	Empty ();
}
#endif

CAgentFileFrame::~CAgentFileFrame ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileFrame::~CAgentFileFrame (Owned) [%d]"), sFrameInstances->IndexOf(this), sFramesInstances->IndexOf(mContainer));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrame::~CAgentFileFrame"), sFrameInstances->IndexOf(this));
#endif
	sFrameInstances->Remove (this);
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFileFrame::Empty ()
{
	mDuration = 0;
	mExitFrame = -2;
	mSoundNdx = -1;
#ifdef	_M_CEE
	if	(mOwner)
	{
		mDuration = mOwner->NewFrameDuration;
	}
	mBranching = nullptr;
	if (mOwner)
	{
		mImages = gcnew CAgentFileFrameImages (mOwner, this);
		mOverlays = gcnew CAgentFileFrameOverlays (mOwner, this);
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
FileAnimation^ CAgentFileFrame::Animation::get()
{
	return (mContainer) ? mContainer->Animation : nullptr;
}

UInt16 CAgentFileFrame::Duration::get()
{
	return mDuration;
}

void CAgentFileFrame::Duration::set (UInt16 pValue)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mDuration != pValue)
		)
	{
		mDuration = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
}

Int16 CAgentFileFrame::SoundNdx::get()
{
	return mSoundNdx;
}

System::String^ CAgentFileFrame::SoundFilePath::get()
{
	if	(mOwner)
	{
		if	(mSoundNdx >= 0)
		{
			return mOwner->GetSoundFilePath (mSoundNdx);
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return mSoundFilePath;
	}
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
	else
	if	(!mOwner)
	{
		mSoundFilePath = gcnew String (pValue);
	}
}

Int16 CAgentFileFrame::ExitFrame::get()
{
	return mExitFrame;
}

void CAgentFileFrame::ExitFrame::set (Int16 pValue)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mExitFrame != pValue)
		)
	{
		mExitFrame = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
}

array <CAgentFileFrameBranch>^ CAgentFileFrame::Branching::get()
{
	return mBranching;
}

void CAgentFileFrame::Branching::set (array <CAgentFileFrameBranch>^ pValue)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mBranching != pValue)
		)
	{
//TODO Validation?
		mBranching = pValue;
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
/////////////////////////////////////////////////////////////////////////////

Boolean CAgentFileFrame::CopyTo (CAgentFileFrame^ pTarget)
{
	return CopyTo (pTarget, false);
}

Boolean CAgentFileFrame::CopyTo (CAgentFileFrame^ pTarget, Boolean pDeepCopy)
{
#ifdef	_DEBUG_COPY
	if	(pTarget)
	{
		LogMessage (_DEBUG_COPY, _T("Copy CAgentFileFrame [%s] [%d] to [%s] [%d]"), mOwner?_B(mOwner->FileName):_B(nullptr), mContainer?mContainer->IndexOf(this):-1, pTarget->mOwner?_B(pTarget->mOwner->FileName):_B(nullptr), pTarget->mContainer?pTarget->mContainer->IndexOf(pTarget):-1);
	}
#endif

	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mDuration = mDuration;
		pTarget->mExitFrame = mExitFrame;

		if	(
				(!mOwner)
			||	(!pTarget->mOwner)
			||	(!Object::ReferenceEquals (mOwner, pTarget->mOwner))
			)
		{
			pTarget->SoundFilePath = SoundFilePath;
		}
		else
		{
			pTarget->mSoundNdx = mSoundNdx;
		}

		if	(pDeepCopy)
		{
#ifdef	_DEBUG_COPY
			LogMessage (_DEBUG_COPY, _T("  Deep"));
#endif
			if	(mBranching)
			{
				pTarget->mBranching = gcnew array <CAgentFileFrameBranch> (mBranching->Length);
				for	(int lNdx = 0; lNdx < mBranching->Length; lNdx++)
				{
					pTarget->mBranching [lNdx] = mBranching [lNdx];
				}
#ifdef	_DEBUG_COPY
				LogMessage (_DEBUG_COPY, _T("  Branching [%d] [%d]"), mBranching->Length, pTarget->mBranching->Length);
#endif
			}
			else
			{
				pTarget->mBranching = nullptr;
			}

			if	(mImages)
			{
				pTarget->mImages = gcnew CAgentFileFrameImages (pTarget->mOwner, pTarget);

				for each (CAgentFileFrameImage^ lImage in mImages)
				{
					CAgentFileFrameImage^	lTargetImage = gcnew CAgentFileFrameImage (pTarget->mOwner, pTarget);

					lImage->CopyTo (lTargetImage);
					pTarget->mImages->Add (lTargetImage);
				}
#ifdef	_DEBUG_COPY
				LogMessage (_DEBUG_COPY, _T("  Images    [%d] [%d]"), mImages->Count, pTarget->mImages->Count);
#endif
			}
			else
			if	(pTarget->mOwner)
			{
				pTarget->mImages = gcnew CAgentFileFrameImages (pTarget->mOwner, pTarget);
			}
			else
			{
				pTarget->mImages = nullptr;
			}

			if	(mOverlays)
			{
				pTarget->mOverlays = gcnew CAgentFileFrameOverlays (pTarget->mOwner, pTarget);

				for each (CAgentFileFrameOverlay^ lOverlay in mOverlays)
				{
					CAgentFileFrameOverlay^	lTargetOverlay = gcnew CAgentFileFrameOverlay (pTarget->mOwner, pTarget);

					lTargetOverlay->mOverlayType = lOverlay->mOverlayType;
					lOverlay->CopyTo (lTargetOverlay);
					pTarget->mOverlays->Add (lTargetOverlay);
				}
			}
			else
			if	(pTarget->mOwner)
			{
				pTarget->mOverlays = gcnew CAgentFileFrameOverlays (pTarget->mOwner, pTarget);
			}
			else
			{
				pTarget->mOverlays = nullptr;
			}
#ifdef	_DEBUG_COPY
				LogMessage (_DEBUG_COPY, _T("  Overlays  [%d] [%d]"), mOverlays->Count, pTarget->mOverlays->Count);
#endif
		}
		else
		{
#ifdef	_DEBUG_COPY
			LogMessage (_DEBUG_COPY, _T("  Shallow"));
#endif
			pTarget->mBranching = mBranching;
			pTarget->mImages = mImages;
			pTarget->mOverlays = mOverlays;
		}

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

System::String^ CAgentFileFrame::ToString ()
{
    return String::Format ("Frame {0:D} Images {1:D} Overlays {2:D} Sound {3:D}", Duration, ImageCount, OverlayCount, SoundNdx);
}

/////////////////////////////////////////////////////////////////////////////

void CAgentFileFrame::OnSerializing (StreamingContext pContext)
{
	if	(mOwner)
	{
		mSoundFilePath = SoundFilePath;
		mSound = mOwner->GetSound (SoundNdx);
	}
	else
	{
		mSoundFilePath = nullptr;
		mSound = nullptr;
	}
}

void CAgentFileFrame::OnSerialized (StreamingContext pContext)
{
	mSoundFilePath = nullptr;
	mSound = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileFrames::CAgentFileFrames ()
{
#ifdef	_DEBUG_INSTANCE
	sFramesInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrames::CAgentFileFrames"), sFramesInstances->IndexOf(this));
#endif
}

CAgentFileFrames::CAgentFileFrames (CharacterFile^ pOwner, FileAnimation^ pAnimation)
:	mOwner (pOwner),
	mAnimation (pAnimation)
{
#ifdef	_DEBUG_INSTANCE
	sFramesInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileFrames::CAgentFileFrames (Owned) [%d]"), sFramesInstances->IndexOf(this), sAnimationInstances->IndexOf(mAnimation));
#endif
#endif
}

CAgentFileFrames::~CAgentFileFrames ()
{
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileFrames::CAgentFileFrames (Owned) [%d]"), sFramesInstances->IndexOf(this), sAnimationInstances->IndexOf(mAnimation));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileFrames::CAgentFileFrames"), sFramesInstances->IndexOf(this));
#endif
	sFramesInstances->Remove (this);
#endif
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
			(!mOwner)
		||	(!mOwner->IsReadOnly)
		)
	{
		lFrame = gcnew CAgentFileFrame (mOwner, this);

		if	(pNdx < 0)
		{
			pNdx = Count;
		}
		__super::Insert (min ((int)pNdx, Count), lFrame);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
	return lFrame;
}

bool CAgentFileFrames::Remove (CAgentFileFrame^ pItem)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(Contains (pItem))
		)
	{
		__super::Remove (pItem);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileFrames::RemoveAt (Int32 pNdx)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(pNdx >= 0)
		&&	((int)pNdx < Count)
		)
	{
		__super::RemoveAt (pNdx);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileFrames::Move (Int32 pNdx, Int32 pNewNdx)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
			if	(mOwner)
			{
				mOwner->IsDirty = true;
			}
			return true;
		}
	}
	return false;
}

bool CAgentFileFrames::Move (CAgentFileFrame^ pItem, Int32 pNewNdx)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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

CAgentFileAnimation::CAgentFileAnimation ()
{
#ifdef	_DEBUG_INSTANCE
	sAnimationInstances->Add (this);
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileAnimation::CAgentFileAnimation"), sAnimationInstances->IndexOf(this));
#endif
	Empty ();
}

#ifdef	_M_CEE
CAgentFileAnimation::CAgentFileAnimation (CharacterFile^ pOwner, FileGestures^ pContainer)
:	mOwner (pOwner),
	mContainer (pContainer)
{
#ifdef	_DEBUG_INSTANCE
	sAnimationInstances->Add (this);
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (_DEBUG_INSTANCE_OWNED, _T("[%d] CAgentFileAnimation::CAgentFileAnimation (Owned)"), sAnimationInstances->IndexOf(this));
#endif
#endif
	Empty ();
}
#endif

CAgentFileAnimation::~CAgentFileAnimation ()
{
#ifdef	_M_CEE
#ifdef	_DEBUG_INSTANCE
#ifdef	_DEBUG_INSTANCE_OWNED
	LogMessage (mOwner?_DEBUG_INSTANCE_OWNED:_DEBUG_INSTANCE, _T("[%d] CAgentFileAnimation::CAgentFileAnimation (Owned)"), sAnimationInstances->IndexOf(this));
#else
	LogMessage (_DEBUG_INSTANCE, _T("[%d] CAgentFileAnimation::CAgentFileAnimation"), sAnimationInstances->IndexOf(this));
#endif
	sAnimationInstances->Remove (this);
#endif
#endif
}

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
		mFrames = gcnew CAgentFileFrames (mOwner, this);
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!String::IsNullOrEmpty (pName))
		&&	(
				(String::IsNullOrEmpty (mName))
			||	(!mName->Equals (pName))
			)
		)
	{
		if	(mOwner)
		{
			if	(mOwner->RenameAnimation (this, pName))
			{
				mOwner->IsDirty = true;
			}
		}
		else
		{
			mName = gcnew String (pName);
		}
	}
}

Byte CAgentFileAnimation::ReturnType::get()
{
	return mReturnType;
}

void CAgentFileAnimation::ReturnType::set (Byte pReturnType)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(mReturnType != pReturnType)
		)
	{
		mReturnType = Math::Min (pReturnType, (Byte)2);
		if	(mReturnType > 0)
		{
			mReturnName = nullptr;
		}
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
}

System::String^ CAgentFileAnimation::ReturnName::get()
{
	return mReturnName;
}

void CAgentFileAnimation::ReturnName::set (System::String^ pReturnName)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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
	return CopyTo (pTarget, false);
}

Boolean CAgentFileAnimation::CopyTo (CAgentFileAnimation^ pTarget, Boolean pDeepCopy)
{
#ifdef	_DEBUG_COPY
	if	(pTarget)
	{
		LogMessage (_DEBUG_COPY, _T("Copy CAgentFileAnimation [%s] [%s] [%d] to [%s] [%s] [%d]"), mOwner?_B(mOwner->FileName):_B(nullptr), _B(Name), mContainer?mContainer->IndexOf(this):-1, pTarget->mOwner?_B(pTarget->mOwner->FileName):_B(nullptr), _B(pTarget->Name), pTarget->mContainer?pTarget->mContainer->IndexOf(pTarget):-1);
	}
#endif

	if	(
			(pTarget)
		&&	(
				(!pTarget->mOwner)
			||	(!pTarget->mOwner->IsReadOnly)
			)
		)
	{
		pTarget->mReturnType = mReturnType;
		pTarget->mReturnName = mReturnName;
		pTarget->mAcaFileName = mAcaFileName;
		pTarget->mAcaChksum = mAcaChksum;

		if	(pDeepCopy)
		{
#ifdef	_DEBUG_COPY
			LogMessage (_DEBUG_COPY, _T("  Deep"));
#endif
			if (mFrames)
			{
				pTarget->mFrames = gcnew CAgentFileFrames (pTarget->mOwner, pTarget);

				for each (CAgentFileFrame^ lFrame in mFrames)
				{
					CAgentFileFrame^	lTargetFrame = gcnew CAgentFileFrame (pTarget->mOwner, pTarget->mFrames);

					lFrame->CopyTo (lTargetFrame, true);
					pTarget->mFrames->Add (lTargetFrame);
				}
#ifdef	_DEBUG_COPY
				LogMessage (_DEBUG_COPY, _T("  Frames    [%d] [%d]"), mFrames->Count, pTarget->mFrames->Count);
#endif
			}
			else
			if	(pTarget->mOwner)
			{
				pTarget->mFrames = gcnew CAgentFileFrames (pTarget->mOwner, pTarget);
			}
			else
			{
				pTarget->mFrames = nullptr;
			}
		}
		else
		{
#ifdef	_DEBUG_COPY
			LogMessage (_DEBUG_COPY, _T("  Shallow"));
#endif
			pTarget->mFrames = mFrames;
		}

		if	(
				(pTarget->mOwner)
			&&	(!pTarget->mOwner->IsReadOnly)
			)
		{
			pTarget->mOwner->IsDirty = true;
		}
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

CAgentFileGestures::~CAgentFileGestures ()
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!Contains (pAnimationName))
		)
	{
		lAnimation = gcnew CAgentFileAnimation (mOwner, this);
		lAnimation->mName = pAnimationName;
		Add (lAnimation);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
	}
	return lAnimation;
}

bool CAgentFileGestures::Remove (System::String^ pAnimationName)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(Contains (pAnimationName))
		)
	{
		__super::Remove (pAnimationName);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
		return true;
	}
	return false;
}

bool CAgentFileGestures::Remove (CAgentFileAnimation^ pItem)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
		&&	(!Contains (pItem))
		)
	{
		__super::Remove (pItem);
		if	(mOwner)
		{
			mOwner->IsDirty = true;
		}
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

CAgentFileStates::~CAgentFileStates ()
{
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileStates::AddStateAnimation (System::String^ pStateName, System::String^ pAnimationName)
{
	if	(
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
				if	(mOwner)
				{
					mOwner->IsDirty = true;
				}
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
			(
				(!mOwner)
			||	(!mOwner->IsReadOnly)
			)
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
				if	(mOwner)
				{
					mOwner->IsDirty = true;
				}
				return true;
			}
			catch AnyExceptionDebug
		}
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileImage::CAgentFileImage ()
{
	Empty ();
}

CAgentFileImage::~CAgentFileImage ()
{
}

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
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
