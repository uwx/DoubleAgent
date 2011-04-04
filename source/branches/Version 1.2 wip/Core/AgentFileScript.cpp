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
#include "AgentFileScript.h"
#ifdef	_M_CEE
#include "HandleTemplates.h"
#include "AgtErr.h"
#include "ScriptStream.h"
#else
#endif
#include "AgentFileDefs.inl"

#ifndef	_M_CEE
IMPLEMENT_DLL_OBJECT(CAgentFileScript)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentFileScript::CAgentFileScript()
{
	Close ();
}

CAgentFileScript::~CAgentFileScript ()
{
	Close ();
}

#ifdef	_M_CEE
CAgentFileScript^ CAgentFileScript::CreateInstance ()
#else
CAgentFileScript* CAgentFileScript::CreateInstance ()
#endif
{
#ifdef	_M_CEE
	LogStart (true);
	return gcnew CAgentFileScript;
#else
	return new CAgentFileScript;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
bool CAgentFileScript::IsAcdFile::get ()
{
	return true;
}
#else
bool CAgentFileScript::get_IsAcdFile () const
{
	return true;
}
#endif	

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
bool CAgentFileScript::IsOpen::get ()
{
	if	(mFileStream)
	{
		return true;
	}
	return false;
}
#else
bool CAgentFileScript::get_IsOpen () const
{
	return false;
}
#endif	

#ifdef	_M_CEE
bool CAgentFileScript::IsReadOnly::get ()
{
	if	(mFileStream)
	{
		return false;
	}
	return true;
}
#else
bool CAgentFileScript::get_IsReadOnly () const
{
	return false;
}
#endif

#ifdef	_M_CEE
bool CAgentFileScript::IsDirty::get ()
{
	return mIsDirty;
}

void CAgentFileScript::IsDirty::set (bool pIsDirty)
{
	if	(!IsReadOnly)
	{
		mIsDirty = pIsDirty;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
void CAgentFileScript::FreeNames ()
{
	__super::FreeNames ();
	mNames = gcnew CAgentFileNames (this);
}

void CAgentFileScript::FreeGestures ()
{
	__super::FreeGestures ();
	mGestures = gcnew CAgentFileGestures (this);
}

void CAgentFileScript::FreeStates ()
{
	__super::FreeStates ();
	mStates = gcnew CAgentFileStates (this);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
HRESULT CAgentFileScript::Open (const System::String^ pPath, UINT pLogLevel)
{
	HRESULT			lResult = E_FAIL;
	String^		lPath = ParseFilePath (pPath);
	ScriptReader^	lReader;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif
	Close ();
	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Open [%s]"), _B(lPath));
	}
	
	try
	{
		mFileStream = gcnew FileStream (lPath, FileMode::Open, FileAccess::ReadWrite, FileShare::Read);
	}
	catch (FileNotFoundException^)
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_FILE_NOT_FOUND);
	}
	catch AnyExceptionDebug
	
	if	(
			(mFileStream)
		&&	(lReader = gcnew ScriptReader (mFileStream))
		)
	{
		mPath = lPath;
		lResult = ParseFile (lReader);
		lReader->Close ();
	}
	if	(FAILED (lResult))
	{
		Close ();
	}
	
	return lResult;
}
#else
HRESULT CAgentFileScript::Open (LPCTSTR pPath, UINT pLogLevel)
{
	return E_NOTIMPL;
}
#endif

HRESULT CAgentFileScript::Save ()
{
	HRESULT				lResult = E_FAIL;
	ScriptWriter^		lWriter;
	
	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		)
	{
		try
		{
			if	(mFileStream)
			{
				try
				{				
					mFileStream->Close ();
				}
				catch AnyExceptionDebug
			}
			mFileStream = nullptr;
						
			try
			{
				mFileStream = gcnew FileStream (mPath, FileMode::Create, FileAccess::Write, FileShare::Read);
			}
			catch AnyExceptionDebug
			
			if	(
					(mFileStream)
				&&	(lWriter = gcnew ScriptWriter (mFileStream))
				)
			{
				lResult = WriteFile (lWriter);
				lWriter->Close ();
			}
			if	(SUCCEEDED (lResult))
			{
				IsDirty = false;
			}
			try
			{
				mFileStream = gcnew FileStream (mPath, FileMode::Open, FileAccess::ReadWrite, FileShare::Read);
			}
			catch AnyExceptionDebug
		}
		catch AnyExceptionDebug
	}
	return lResult;
}

void CAgentFileScript::Close ()
{
#ifdef	_M_CEE
	mIsDirty = false;
	if	(mFileStream)
	{
		try
		{
			mFileStream->Close ();
		}
		catch AnyExceptionDebug
	}
	mFileStream = nullptr;
	mImageFilePaths = gcnew System::Collections::Generic::List <String^>;
	mImages = gcnew System::Collections::Generic::Dictionary <String^, System::Drawing::Bitmap^>;
	mSoundFilePaths = gcnew System::Collections::Generic::List <String^>;
	mSounds = gcnew System::Collections::Generic::Dictionary <String^, System::Media::SoundPlayer^>;
	mIconFilePath = nullptr;
	mPaletteFilePath = nullptr;
	mNewFrameDuration = 10;
#else
#endif
	__super::Close ();

#ifdef	_M_CEE
	mHeader = gcnew CAgentFileHeader (this);
#endif	
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseFile (ScriptReader^ pReader)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^	lLine;
	String^	lToken;
	
	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartHeader))
			{
				lResult = ParseHeader (pReader);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartBalloon))
			{
				lResult = ParseBalloon (pReader);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartAnimation))
			{
				lResult = ParseAnimation (pReader, pReader->GetText (lLine));
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartState))
			{
				lResult = ParseState (pReader, pReader->GetText (lLine));
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
		if	(FAILED (lResult))
		{
			break;
		}
	}
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::WriteFile (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteComments ();
	WriteHeader (pWriter);
	WriteBalloon (pWriter);
	WriteAnimations (pWriter);
	WriteStates (pWriter);
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseHeader (ScriptReader^ pReader)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^	lLine;
	String^	lToken;

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndHeader))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartName))
			{
				if	(lToken = pReader->NextToken (lLine))
				{
					lResult = ParseName (pReader, pReader->GetHex (lToken));
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderGuid))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mHeader->mGuid = pReader->GetGuid (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderStyle))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mHeader->mStyle = ParseStyle (pReader, lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderWidth))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mHeader->mImageSize.Width = pReader->GetInt (lToken); 
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderHeight))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mHeader->mImageSize.Height = pReader->GetInt (lToken); 
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderIcon))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mIconFilePath = lToken;
					try
					{
						mHeader->mIcon = gcnew System::Drawing::Icon (lToken, 16, 16);
					}
					catch AnyExceptionDebug
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderTransparency))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mHeader->mTransparency = (Byte)pReader->GetInt (lToken); 
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderPalette))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mPaletteFilePath = lToken;
					try
					{
						System::Drawing::Bitmap^	lBitmap = gcnew System::Drawing::Bitmap (lToken);
						
						if	(
								(lBitmap)
							&&	(lBitmap->Palette)
							)
						{
							mHeader->mPalette = lBitmap->Palette;
						}
					}
					catch AnyExceptionDebug
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->HeaderFrameDuration))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mNewFrameDuration = (Byte)pReader->GetInt (lToken); 
				}
			}
			else
			if	(ParseTtsLine (pReader, lToken, lLine))
			{
				continue;
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
		if	(FAILED (lResult))
		{
			break;
		}
	}
#else
#endif
	return lResult;
}

UInt32 CAgentFileScript::ParseStyle (ScriptReader^ pReader, System::String^ pStyle)
{
	UInt32				lStyle = 0;
	array <String^>^	lParts = pReader->GetStyles (pStyle);
	int					lNdx;

	for (lNdx = 0; lNdx	< lParts->Length; lNdx++)
	{
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleSystem))
		{
			lStyle |= (UInt32)AgentCharStyle::CharStyleStandard;
		}
		else
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleTts))
		{
			lStyle |= (UInt32)AgentCharStyle::CharStyleTts;
		}
		else
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleBalloon))
		{
			lStyle |= (UInt32)AgentCharStyle::CharStyleBalloon;
		}
		else
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleBalloonNone))
		{
			lStyle &= ~(UInt32)AgentCharStyle::CharStyleBalloon;
		}
		else
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleBalloonSizeToText))
		{
			lStyle |= (UInt32)AgentCharStyle::CharStyleSizeToText;
		}
		else
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleBalloonNoAutoHide))
		{
			lStyle |= (UInt32)AgentCharStyle::CharStyleNoAutoHide;
		}
		else
		if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->StyleBalloonNoAutoPace))
		{
			lStyle |= (UInt32)AgentCharStyle::CharStyleNoAutoPace;
		}
		else
		{
			LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lParts[lNdx]));
		}
	}
	return lStyle;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::WriteHeader (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteLine ();
	pWriter->WriteKeyword (pWriter->Keywords->StartHeader);
	pWriter->WriteLine ();
	pWriter->Indent++;
	
	WriteNames (pWriter);
	pWriter->WriteValue (pWriter->Keywords->HeaderGuid, pWriter->FormatGuid (mHeader->Guid));
	if	(!String::IsNullOrEmpty (IconFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->HeaderIcon, pWriter->FormatText (IconFilePath));
	}
	pWriter->WriteValue (pWriter->Keywords->HeaderWidth, pWriter->FormatInt (mHeader->ImageSize.Width));
	pWriter->WriteValue (pWriter->Keywords->HeaderHeight, pWriter->FormatInt (mHeader->ImageSize.Height));
	pWriter->WriteValue (pWriter->Keywords->HeaderTransparency, pWriter->FormatInt (mHeader->Transparency));
	pWriter->WriteValue (pWriter->Keywords->HeaderFrameDuration, pWriter->FormatInt (NewFrameDuration));
	WriteStyle (pWriter, (UInt32)mHeader->Style);
	WriteTts (pWriter);
	if	(!String::IsNullOrEmpty (PaletteFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->HeaderPalette, pWriter->FormatText (PaletteFilePath));
	}
	
	pWriter->Indent--;
	pWriter->WriteLine ();
	pWriter->WriteKeyword (pWriter->Keywords->EndHeader);
	pWriter->WriteSeparator ();
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteStyle (ScriptWriter^ pWriter, UInt32 pStyle)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	List <System::String^>^	lStyle = gcnew List <String^>;
	
	if	(pStyle & (UInt32)AgentCharStyle::CharStyleStandard)
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleSystem]);		
	}
	if	(pStyle & (UInt32)AgentCharStyle::CharStyleTts)
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleTts]);		
	}
	if	(pStyle & (UInt32)AgentCharStyle::CharStyleBalloon)
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleBalloon]);		
	}
	else
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleBalloon]);		
	}
	if	(pStyle & (UInt32)AgentCharStyle::CharStyleSizeToText)
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleBalloonSizeToText]);		
	}
	if	(pStyle & (UInt32)AgentCharStyle::CharStyleNoAutoHide)
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleBalloonNoAutoHide]);		
	}
	if	(pStyle & (UInt32)AgentCharStyle::CharStyleNoAutoPace)
	{
		lStyle->Add (pWriter->Keyword [pWriter->Keywords->StyleBalloonNoAutoPace]);		
	}
	pWriter->WriteValue (pWriter->Keywords->HeaderStyle, pWriter->GetStyles (lStyle->ToArray()));
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseTtsLine (ScriptReader^ pReader, System::String^ pToken, System::String^ pLine)
{
	String^	lToken;
	
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsEngine))
	{
		if	(!mTts)
		{
			mTts = gcnew CAgentFileTts (this);
		}
		if	(lToken = pReader->NextValue (pLine))
		{
			mTts->mEngine = pReader->GetGuid (lToken);
		}
	}
	else
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsMode))
	{
		if	(!mTts)
		{
			mTts = gcnew CAgentFileTts (this);
		}
		if	(lToken = pReader->NextValue (pLine))
		{
			mTts->mMode = pReader->GetGuid (lToken);
			if	(mTts->mMode.Equals (Guid::Empty))
			{
				mTts->mModeId = nullptr;
			}
			else
			{
				mTts->mModeId = mTts->mMode.ToString()->ToUpper();
			}
		}
	}
	else
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsLanguage))
	{
		if	(!mTts)
		{
			mTts = gcnew CAgentFileTts (this);
		}
		if	(lToken = pReader->NextValue (pLine))
		{
			mTts->mLanguage = pReader->GetHex (lToken);
		}
	}
	else
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsDialect))
	{
	}
	else
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsGender))
	{
		if	(!mTts)
		{
			mTts = gcnew CAgentFileTts (this);
		}
		if	(lToken = pReader->NextValue (pLine))
		{
			mTts->mGender = pReader->GetInt (lToken);
		}
	}
	else
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsAge))
	{
#if	FALSE	
		if	(!mTts)
		{
			mTts = gcnew CAgentFileTts (this);
		}
		if	(lToken = pReader->NextValue (pLine))
		{
			mTts->mAge = pReader->GetInt (lToken);
		}
#endif		
	}
	else
	if	(pReader->IsKeyword (pToken, pReader->Keywords->TtsStyle))
	{
#if	FALSE	
		if	(!mTts)
		{
			mTts = gcnew CAgentFileTts (this);
		}
		if	(lToken = pReader->NextValue (pLine))
		{
			mTts->mStyle = lToken;
		}
#endif		
	}
	else
	{
		return false;
	}
	return true;
}

HRESULT CAgentFileScript::WriteTts (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	if	(mTts)
	{
		pWriter->WriteValue (pWriter->Keywords->TtsEngine, pWriter->FormatGuid (mTts->Engine));
		pWriter->WriteValue (pWriter->Keywords->TtsMode, pWriter->FormatGuid (mTts->Mode));
		pWriter->WriteValue (pWriter->Keywords->TtsLanguage, pWriter->FormatHex (mTts->Language));
		pWriter->WriteValue (pWriter->Keywords->TtsGender, pWriter->FormatInt (mTts->Gender));
		//pWriter->WriteValue (pWriter->Keywords->TtsAge, pWriter->FormatInt (mTts->Age));
		//pWriter->WriteValue (pWriter->Keywords->TtsStyle, pWriter->FormatText (mTts->Style));
	}
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseBalloon (ScriptReader^ pReader)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^					lLine;
	String^					lToken;
	String^					lFontName = nullptr;
	int							lFontSize = 0;
	System::Drawing::FontStyle	lFontStyle = FontStyle::Regular;

	mBalloon = gcnew CAgentFileBalloon (this);
	
	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndBalloon))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonLines))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mBalloon->mLines = pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonPerLine))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mBalloon->mPerLine = pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonFgColor))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mBalloon->mFgColor = System::Drawing::Color::FromArgb (pReader->GetColor (lToken));
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonBkColor))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mBalloon->mBkColor = System::Drawing::Color::FromArgb (pReader->GetColor (lToken));
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonBrColor))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					mBalloon->mBrColor = System::Drawing::Color::FromArgb (pReader->GetColor (lToken));
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonFontName))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lFontName = lToken;
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonFontHeight))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lFontSize = pReader->GetInt (lToken); 
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BalloonFontStyle))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					try
					{
						array <String^>^	lParts = pReader->GetFontStyles (lToken);
						int					lNdx;
						
						for	(lNdx = 0; lNdx < lParts->Length; lNdx++)
						{
							lParts[lNdx] = lParts[lNdx]->Trim();
							
							if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->BalloonFontBold))
							{
								lFontStyle = (FontStyle)((int)lFontStyle | (int)FontStyle::Bold);
							}
							else
							if	(pReader->IsKeyword (lParts[lNdx], pReader->Keywords->BalloonFontItalic))
							{
								lFontStyle = (FontStyle)((int)lFontStyle | (int)FontStyle::Italic);
							}
							else
							{
								LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lParts[lNdx]));
							}
						}
					}
					catch AnyExceptionDebug
				}
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
	}
	
	if	(lFontName)
	{
		mBalloon->mFont = gcnew System::Drawing::Font (lFontName, (float)lFontSize, lFontStyle);
	}
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteBalloon (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	if	(mBalloon)
	{
		pWriter->WriteLine ();
		pWriter->WriteKeyword (pWriter->Keywords->StartBalloon);
		pWriter->Indent++;

		pWriter->WriteValue (pWriter->Keywords->BalloonLines, pWriter->FormatInt (mBalloon->Lines));
		pWriter->WriteValue (pWriter->Keywords->BalloonPerLine, pWriter->FormatInt (mBalloon->PerLine));
		if	(mBalloon->Font)
		{
			List <String^>^	lStyle = gcnew List <String^>;
			
			if	(mBalloon->Font->Bold)
			{
				lStyle->Add (pWriter->Keyword [pWriter->Keywords->BalloonFontBold]);
			}
			if	(mBalloon->Font->Italic)
			{
				lStyle->Add (pWriter->Keyword [pWriter->Keywords->BalloonFontItalic]);
			}
			pWriter->WriteValue (pWriter->Keywords->BalloonFontName, pWriter->FormatText (mBalloon->Font->OriginalFontName ? mBalloon->Font->OriginalFontName : mBalloon->Font->Name));
			pWriter->WriteValue (pWriter->Keywords->BalloonFontHeight, pWriter->FormatInt ((int)(mBalloon->Font->SizeInPoints + 0.49f)));
			pWriter->WriteValue (pWriter->Keywords->BalloonFontStyle, pWriter->GetFontStyles (lStyle->ToArray()));
			pWriter->WriteValue (pWriter->Keywords->BalloonFgColor, pWriter->FormatColor (mBalloon->FgColor.ToArgb()));
			pWriter->WriteValue (pWriter->Keywords->BalloonBkColor, pWriter->FormatColor (mBalloon->BkColor.ToArgb()));
			pWriter->WriteValue (pWriter->Keywords->BalloonBrColor, pWriter->FormatColor (mBalloon->BrColor.ToArgb()));
		}

		pWriter->Indent--;
		pWriter->WriteKeyword (pWriter->Keywords->EndBalloon);
		pWriter->WriteSeparator ();
	}
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseName (ScriptReader^ pReader, LANGID pLangID)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^			lLine;
	String^			lToken;
	CAgentFileName^	lName = gcnew CAgentFileName (this);
	
	lName->mLanguage = pLangID;

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndName))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->NameName))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lName->mName = lToken;
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->NameDesc1))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lName->mDesc1 = lToken;
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->NameDesc2))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lName->mDesc2 = lToken;
				}
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
		if	(FAILED (lResult))
		{
			break;
		}
	}
	
	if	(!String::IsNullOrEmpty (lName->mName))
	{
		try
		{
			mNames->Add (lName);
		}
		catch AnyExceptionDebug
	}
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteNames (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	if	(mNames)
	{
		for each (CAgentFileName^ lName in mNames)
		{
			WriteName (pWriter, lName);
		}
	}
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteName (ScriptWriter^ pWriter, CAgentFileName^ pName)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteKeyword (pWriter->Keywords->StartName, pWriter->FormatHex (pName->Language));
	pWriter->Indent++;

	pWriter->WriteValue (pWriter->Keywords->NameName, pWriter->FormatText (pName->Name));
	pWriter->WriteValue (pWriter->Keywords->NameDesc1, pWriter->FormatText (pName->Desc1));
	pWriter->WriteValue (pWriter->Keywords->NameDesc2, pWriter->FormatText (pName->Desc2));

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndName);
	pWriter->WriteLine ();
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseAnimation (ScriptReader^ pReader, System::String^ pAnimationName)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^				lLine;
	String^				lToken;
	CAgentFileAnimation^	lAnimation = gcnew CAgentFileAnimation (this);
	
	lAnimation->mName = pAnimationName;

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndAnimation))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->AnimationReturnType))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lAnimation->mReturnType = (Byte)pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->AnimationReturnName))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					try
					{
						lAnimation->mReturnName = lToken;
						lAnimation->mReturnType = 0;
					}
					catch AnyExceptionDebug
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartFrame))
			{
				lResult = ParseAnimationFrame (pReader, lAnimation);
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
	}
	
	try
	{
		mGestures->Add (lAnimation);
	}
	catch AnyExceptionDebug
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteAnimations (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	if	(mGestures)
	{
		for each (CAgentFileAnimation^ lAnimation in mGestures)
		{
			WriteAnimation (pWriter, lAnimation);
		}
	}
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteAnimation (ScriptWriter^ pWriter, CAgentFileAnimation^ pAnimation)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteLine ();
	pWriter->WriteKeyword (pWriter->Keywords->StartAnimation, pWriter->FormatText (pAnimation->Name));
	pWriter->WriteLine ();
	pWriter->Indent++;

	if	(pAnimation->ReturnType > 0)
	{
		pWriter->WriteValue (pWriter->Keywords->AnimationReturnType, pWriter->FormatInt (pAnimation->ReturnType));
		pWriter->WriteLine ();
	}
	else
	if	(!String::IsNullOrEmpty (pAnimation->ReturnName))
	{
		pWriter->WriteValue (pWriter->Keywords->AnimationReturnName, pWriter->FormatText (pAnimation->ReturnName));
		pWriter->WriteLine ();
	}
	
	if	(pAnimation->mFrames)
	{
		for each (CAgentFileFrame^ lFrame in pAnimation->mFrames)
		{
			WriteAnimationFrame (pWriter, lFrame);
		}
	}

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndAnimation);
	pWriter->WriteSeparator ();
#else
#endif
	return lResult;
}

bool CAgentFileScript::RenameAnimation (CAgentFileAnimation^ pAnimation, System::String^ pNewName)
{
	bool	lRet = false;

	if	(
			(pAnimation)	
		&&	(!String::IsNullOrEmpty (pNewName))
		&&	(
				(String::IsNullOrEmpty (pAnimation->mName))
			||	(!pAnimation->mName->Equals (pNewName))
			)
		&&	(!mGestures->Contains (pNewName))
		)
	{
		for each (CAgentFileAnimation^ lAnimation in mGestures)
		{
			if	(String::Compare (lAnimation->mReturnName, pAnimation->mName, true) == 0)
			{
				lAnimation->mReturnName = String::Copy (pNewName);
			}
		}
		if	(mStates)
		{		
			for each (KeyValuePair <String^, array <String^>^> lState in mStates)
			{
				for (int lNdx = 0; lNdx < lState.Value->Length; lNdx++)
				{
					if	(String::Compare (lState.Value[lNdx], pAnimation->mName, true) == 0)
					{
						lState.Value[lNdx] = String::Copy (pNewName);
					}
				}
			}
		}

		pAnimation->mName = pNewName;
		mGestures->ChangeItemKey (pAnimation);
		return true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseAnimationFrame (ScriptReader^ pReader, CAgentFileAnimation^ pAnimation)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^			lLine;
	String^			lToken;
	CAgentFileFrame^	lFrame = gcnew CAgentFileFrame (this);

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndFrame))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->FrameDuration))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lFrame->mDuration = (UInt16)pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->FrameExitBranch))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lFrame->mExitFrame = (Int16)pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->FrameSound))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lFrame->mSoundNdx = LoadSoundFile (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartImage))
			{
				lResult = ParseFrameImage (pReader, lFrame);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartOverlay))
			{
				lResult = ParseFrameOverlay (pReader, lFrame);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartBranching))
			{
				lResult = ParseFrameBranching (pReader, lFrame);
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
	}
	
	if (!pAnimation->mFrames)
	{
		pAnimation->mFrames = gcnew CAgentFileFrames (this);
	}
	pAnimation->mFrames->Add (lFrame);
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteAnimationFrame (ScriptWriter^ pWriter, CAgentFileFrame^ pFrame)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteKeyword (pWriter->Keywords->StartFrame);
	pWriter->Indent++;

	pWriter->WriteValue (pWriter->Keywords->FrameDuration, pWriter->FormatInt (pFrame->Duration));
	if	(pFrame->ExitFrame >= 0)
	{
		pWriter->WriteValue (pWriter->Keywords->FrameExitBranch, pWriter->FormatInt (pFrame->ExitFrame));
	}
	if	(!String::IsNullOrEmpty (pFrame->SoundFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->FrameSound, pWriter->FormatText (pFrame->SoundFilePath));
	}
	WriteFrameBranching (pWriter, pFrame);
	
	if	(pFrame->mOverlays)
	{
		for each (CAgentFileFrameOverlay^ lOverlay in pFrame->mOverlays)
		{
			WriteFrameOverlay (pWriter, lOverlay);
		}
	}
	if	(pFrame->mImages)
	{
		for each (CAgentFileFrameImage^ lImage in pFrame->mImages)
		{
			WriteFrameImage (pWriter, lImage);
		}
	}

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndFrame);
	pWriter->WriteLine ();
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseFrameBranching (ScriptReader^ pReader, CAgentFileFrame^ pFrame)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^	lLine;
	String^	lToken;
	Int16		lBranchTarget = -1;
	UInt16		lBranchProbability = 0;
	int			lBranchNdx = 0;

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndBranching))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BranchingTarget))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lBranchTarget = pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->BranchingProbability))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lBranchProbability = pReader->GetInt (lToken);
				}
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
		
		if	(
				(lBranchTarget >= 0)
			&&	(lBranchProbability > 0)
			&&	(lBranchNdx < 3)
			)
		{
			if	(pFrame->mBranching)
			{
				pFrame->mBranching->Resize (pFrame->mBranching, lBranchNdx+1);
			}
			else
			{
				pFrame->mBranching = gcnew array <CAgentFileFrameBranch> (1);
			}
			pFrame->mBranching[lBranchNdx].mFrameNdx = lBranchTarget;
			pFrame->mBranching[lBranchNdx].mProbability = lBranchProbability;
			lBranchNdx++;
		}
	}
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteFrameBranching (ScriptWriter^ pWriter, CAgentFileFrame^ pFrame)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	if	(
			(pFrame->mBranching)
		&&	(pFrame->mBranching->Length > 0)
		)
	{
		pWriter->WriteKeyword (pWriter->Keywords->StartBranching);
		pWriter->Indent++;
		
		for (int lNdx = 0; lNdx < pFrame->mBranching->Length; lNdx++)
		{
			if	(pFrame->mBranching [lNdx].mProbability)
			{
				pWriter->WriteValue (pWriter->Keywords->BranchingTarget, pWriter->FormatInt (pFrame->mBranching [lNdx].mFrameNdx));
				pWriter->WriteValue (pWriter->Keywords->BranchingProbability, pWriter->FormatInt (pFrame->mBranching [lNdx].mProbability));
			}
		}

		pWriter->Indent--;
		pWriter->WriteKeyword (pWriter->Keywords->EndBranching);
	}
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseFrameImage (ScriptReader^ pReader, CAgentFileFrame^ pFrame)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^				lLine;
	String^				lToken;
	CAgentFileFrameImage^	lImage = gcnew CAgentFileFrameImage (this);

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndImage))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->ImageFile))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lImage->mImageNdx = LoadImageFile (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->ImageOffsetX))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lImage->mOffset.X = pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->ImageOffsetY))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lImage->mOffset.Y = pReader->GetInt (lToken);
				}
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
	}

	if (!pFrame->mImages)
	{
		pFrame->mImages = gcnew CAgentFileFrameImages (this);
	}
	pFrame->mImages->Add (lImage);
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteFrameImage (ScriptWriter^ pWriter, CAgentFileFrameImage^ pFrameImage)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteKeyword (pWriter->Keywords->StartImage);
	pWriter->Indent++;

	if	(!String::IsNullOrEmpty (pFrameImage->ImageFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->ImageFile, pWriter->FormatText (pFrameImage->ImageFilePath));
	}
	if	(
			(pFrameImage->Offset.X != 0)
		||	(pFrameImage->Offset.Y != 0)
		)
	{
		pWriter->WriteValue (pWriter->Keywords->ImageOffsetX, pWriter->FormatInt (pFrameImage->Offset.X));
		pWriter->WriteValue (pWriter->Keywords->ImageOffsetY, pWriter->FormatInt (pFrameImage->Offset.Y));
	}

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndImage);
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseFrameOverlay (ScriptReader^ pReader, CAgentFileFrame^ pFrame)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^					lLine;
	String^					lToken;
	CAgentFileFrameOverlay^	lOverlay = gcnew CAgentFileFrameOverlay (this);

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndOverlay))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayFile))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lOverlay->mImageNdx = LoadImageFile (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayType))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeClosed))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayClosed;
					}
					else
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeWide1))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayWide1;
					}
					else
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeWide2))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayWide2;
					}
					else
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeWide3))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayWide3;
					}
					else
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeWide4))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayWide4;
					}
					else
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeMedium))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayMedium;
					}
					else
					if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayTypeNarrow))
					{
						lOverlay->mOverlayType = AgentMouthOverlay::MouthOverlayNarrow;
					}
					else
					{
						LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
						//lResult = E_INVALIDARG;
					}
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->OverlayOffsetX))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lOverlay->mOffset.X = pReader->GetInt (lToken);
				}
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->ImageOffsetY))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lOverlay->mOffset.Y = pReader->GetInt (lToken);
				}
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
			}
		}
	}

	if (!pFrame->mOverlays)
	{
		pFrame->mOverlays = gcnew CAgentFileFrameOverlays (this);
	}
	pFrame->mOverlays->Add (lOverlay);
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteFrameOverlay (ScriptWriter^ pWriter, CAgentFileFrameOverlay^ pFrameOverlay)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	pWriter->WriteKeyword (pWriter->Keywords->StartOverlay);
	pWriter->Indent++;
	
	switch (pFrameOverlay->OverlayType)
	{
		case MouthOverlay::MouthOverlayClosed:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeClosed]);
		}	break;
		case MouthOverlay::MouthOverlayWide1:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeWide1]);
		}	break;
		case MouthOverlay::MouthOverlayWide2:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeWide2]);
		}	break;
		case MouthOverlay::MouthOverlayWide3:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeWide3]);
		}	break;
		case MouthOverlay::MouthOverlayWide4:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeWide4]);
		}	break;
		case MouthOverlay::MouthOverlayMedium:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeMedium]);
		}	break;
		case MouthOverlay::MouthOverlayNarrow:
		{
			pWriter->WriteValue (pWriter->Keywords->OverlayType, pWriter->Keyword [pWriter->Keywords->OverlayTypeNarrow]);
		}	break;
	}
	if	(!String::IsNullOrEmpty (pFrameOverlay->ImageFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->OverlayFile, pWriter->FormatText (pFrameOverlay->ImageFilePath));
	}
	if	(
			(pFrameOverlay->Offset.X != 0)
		||	(pFrameOverlay->Offset.Y != 0)
		)
	{
		pWriter->WriteValue (pWriter->Keywords->OverlayOffsetX, pWriter->FormatInt (pFrameOverlay->Offset.X));
		pWriter->WriteValue (pWriter->Keywords->OverlayOffsetY, pWriter->FormatInt (pFrameOverlay->Offset.Y));
	}

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndOverlay);
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileScript::ParseState (ScriptReader^ pReader, System::String^ pStateName)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	String^			lLine;
	String^			lToken;
	List <String^>^		lAnimations = gcnew List <String^>;

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->EndState))
			{
				break;
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StateAnimation))
			{
				if	(lToken = pReader->NextValue (lLine))
				{
					lAnimations->Add (lToken);
				}
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lResult = E_INVALIDARG;
			}
		}
	}
	
	if	(lAnimations->Count > 0)
	{
		try
		{
			mStates->Add (pStateName, lAnimations->ToArray());
		}
		catch AnyExceptionDebug
	}
#else
#endif
	return lResult;
}

HRESULT CAgentFileScript::WriteStates (ScriptWriter^ pWriter)
{
	HRESULT	lResult = S_OK;
#ifdef	_M_CEE
	if	(mStates)
	{		
		for each (KeyValuePair <String^, array <String^>^> lState in mStates)
		{
			pWriter->WriteLine ();
			pWriter->WriteKeyword (pWriter->Keywords->StartState, pWriter->FormatText (lState.Key));
			pWriter->Indent++;
			
			for (int lNdx = 0; lNdx < lState.Value->Length; lNdx++)
			{
				pWriter->WriteValue (pWriter->Keywords->StateAnimation, pWriter->FormatText (lState.Value[lNdx]));
			}

			pWriter->Indent--;
			pWriter->WriteKeyword (pWriter->Keywords->EndState);
			pWriter->WriteSeparator ();
		}
	}
#else
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CAgentFileScript::ImageCount::get()
{
	return (mImages) ? mImages->Count : 0;
}

System::String^ CAgentFileScript::GetImageFilePath (int pImageNdx)
{
	if	(
			(mImageFilePaths)
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < mImageFilePaths->Count)
		)
	{
		return mImageFilePaths [pImageNdx];
	}
	return nullptr;
}

Int32 CAgentFileScript::LoadImageFile (System::String^ pImageFilePath)
{
	Int32	lImageNdx = -1;
	
	try
	{
		String^	lPath = System::IO::Path::GetFullPath (pImageFilePath);

		if	(!mImages->ContainsKey (lPath))
		{
			System::Drawing::Bitmap^	lBitmap = nullptr;
			
			try							
			{
				lBitmap = gcnew System::Drawing::Bitmap (lPath);
			}
			catch AnyExceptionDebug
			
			mImages->Add (lPath, lBitmap);
		}

		lImageNdx = (Int32)mImageFilePaths->Count;
		mImageFilePaths->Add (lPath);
	}
	catch AnyExceptionDebug
	
	return lImageNdx;
}

Int32 CAgentFileScript::LoadSoundFile (System::String^ pSoundFilePath)
{
	Int32	lSoundNdx = -1;
	
	try
	{
		String^	lPath = System::IO::Path::GetFullPath (pSoundFilePath);

		if	(!mSounds->ContainsKey (lPath))
		{
			System::Media::SoundPlayer^	lPlayer = nullptr;
			
			try							
			{
				lPlayer = gcnew System::Media::SoundPlayer (lPath);
			}
			catch AnyExceptionDebug
			
			mSounds->Add (lPath, lPlayer);
		}

		lSoundNdx = (Int16)mSoundFilePaths->Count;
		mSoundFilePaths->Add (lPath);
	}
	catch AnyExceptionDebug
	
	return lSoundNdx;
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileImage^ CAgentFileScript::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor, UINT pLogLevel)
{
	CAgentFileImage^			lFileImage = nullptr;
	System::Drawing::Bitmap^	lBitmap;

	if	(
			(lBitmap = GetImageBitmap (pImageNdx, p32Bit, pBkColor))
		&&	(lFileImage = gcnew CAgentFileImage)
		)
	{
		lFileImage->mImageNum = pImageNdx;
		lFileImage->mImageSize = lBitmap->Size;
		lFileImage->mIs32Bit = p32Bit;
		
		try
		{
			BitmapData^		lBitmapData;
			pin_ptr <BYTE>	lBits;

			lBitmap->RotateFlip (RotateFlipType::RotateNoneFlipY);
			
			if	(lBitmapData = lBitmap->LockBits (System::Drawing::Rectangle (0, 0, lBitmap->Width, lBitmap->Height), ImageLockMode::ReadOnly, lBitmap->PixelFormat))
			{
				if	(
						(lFileImage->mBits = gcnew array <BYTE> (lBitmapData->Height * lBitmapData->Stride))
					&&	(lBits = &lFileImage->mBits[0])
					)
				{
					memcpy (lBits, (LPVOID)lBitmapData->Scan0, lFileImage->mBits->Length);
				}
				lBitmap->UnlockBits (lBitmapData);
			}
		}
		catch AnyExceptionDebug
	}
	return lFileImage;			
}

System::Drawing::Bitmap^ CAgentFileScript::GetImageBitmap (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
{
	System::Drawing::Bitmap^	lBitmap = nullptr;
	
	if	(
			(mImageFilePaths)
		&&	(mImages)
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < mImageFilePaths->Count)
		)
	{
		try
		{
			lBitmap = mImages [mImageFilePaths [pImageNdx]];

			if	(lBitmap)
			{
				if	(p32Bit)
				{
//TODO convert to 8-bit on load?				
#if	FALSE				
					if	(
							(lBitmap->PixelFormat != PixelFormat::Format8bppIndexed)
						&&	(mHeader->mPalette)
						)
					{
						lBitmap->Palette = mHeader->mPalette;
						lBitmap = lBitmap->Clone (System::Drawing::Rectangle (0, 0, lBitmap->Width, lBitmap->Height), PixelFormat::Format8bppIndexed);	
					}
#endif
					if	(
							(lBitmap->PixelFormat == PixelFormat::Format8bppIndexed)
						&&	(mHeader->Palette)
						)
					{
						lBitmap->Palette = mHeader->Palette;
//TODO the following should work but it does not						
#if	FALSE				
						if	(
								(pBkColor != Color::Empty)
							&&	(pBkColor != Color::Transparent)
							)
						{
							lBitmap->Palette->Entries [mHeader->Transparency] = pBkColor;
						}
#endif						
					}

					lBitmap = lBitmap->Clone (System::Drawing::Rectangle (0, 0, lBitmap->Width, lBitmap->Height), PixelFormat::Format32bppPArgb);
					
					if	(
							(lBitmap)
						&&	(mHeader->Palette)
						&&	(pBkColor == Color::Transparent)
						)
					{
						try
						{
							lBitmap->MakeTransparent (mHeader->Palette->Entries [mHeader->Transparency]);
						}
						catch AnyExceptionDebug
					}
				}
				else
				{
					lBitmap = lBitmap->Clone (System::Drawing::Rectangle (0, 0, lBitmap->Width, lBitmap->Height), PixelFormat::Format8bppIndexed);	

					if	(
							(lBitmap)
						&&	(mHeader->Palette)
						)
					{
						lBitmap->Palette = mHeader->Palette;
						if	(
								(pBkColor != Color::Transparent)
							&&	(pBkColor != Color::Empty)
							)
						{
							lBitmap->Palette->Entries [mHeader->Transparency] = pBkColor;
						}
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
	
	return lBitmap;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CAgentFileScript::SoundCount::get()
{
	return (mSounds) ? mSounds->Count : 0;
}

int CAgentFileScript::GetSoundSize (int pSoundNdx)
{
	int	lSoundSize = 0;
	
	if	(
			(mSoundFilePaths)
		&&	(mSounds)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < mSoundFilePaths->Count)
		)
	{
		try
		{
			System::Media::SoundPlayer^	lPlayer = mSounds [mSoundFilePaths [pSoundNdx]];
			
			if	(
					(lPlayer)
				&&	(lPlayer->Stream)
				)
			{
				lSoundSize = (int)lPlayer->Stream->Length;
			}
		}
		catch AnyExceptionDebug
	}
	
	return lSoundSize;
}

System::String^ CAgentFileScript::GetSoundFilePath (int pSoundNdx)
{
	if	(
			(mSoundFilePaths)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < mSoundFilePaths->Count)
		)
	{
		return mSoundFilePaths [pSoundNdx];
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CAgentFileScript::IconFilePath::get()
{
	return mIconFilePath;
}

void CAgentFileScript::IconFilePath::set (System::String^ pValue)
{
	if	(!IsReadOnly)
	{
		String^				lFullPath = nullptr;
		System::Drawing::Icon^	lIcon = nullptr;

		if	(String::IsNullOrEmpty (pValue))
		{
			if	(mIconFilePath)
			{
				mIconFilePath = nullptr;
				IsDirty = true;
			}
			if	(mHeader->mIcon)
			{
				DestroyIcon ((HICON)(INT_PTR)mHeader->mIcon->Handle);
				mHeader->mIcon = lIcon;
				IsDirty = true;
			}
		}
		else
		{
			try
			{
				lFullPath = System::IO::Path::GetFullPath (pValue);
			}
			catch AnyExceptionDebug
					
			if	(
					(!String::IsNullOrEmpty (lFullPath))
				&&	(
						(!mIconFilePath)
					||	(!mIconFilePath->Equals (lFullPath))
					)
				)
			{
				try
				{
					lIcon = gcnew System::Drawing::Icon (lFullPath, 16, 16);
				}
				catch AnyExceptionDebug
				
				if	(lIcon)
				{
					mIconFilePath = lFullPath;
					if	(mHeader->mIcon)
					{
						DestroyIcon ((HICON)(INT_PTR)mHeader->mIcon->Handle);
					}
					mHeader->mIcon = lIcon;
					IsDirty = true;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

System::String^ CAgentFileScript::PaletteFilePath::get()
{
	return mPaletteFilePath;
}

void CAgentFileScript::PaletteFilePath::set (System::String^ pValue)
{
	if	(
			(!IsReadOnly)
		&&	(!String::IsNullOrEmpty (pValue))
		)
	{
		String^					lFullPath = nullptr;
		System::Drawing::Bitmap^	lBitmap = nullptr;
		
		try
		{
			lFullPath = System::IO::Path::GetFullPath (pValue);
		}
		catch AnyExceptionDebug
		
		if	(String::IsNullOrEmpty (lFullPath))
		{
			if	(mPaletteFilePath)
			{
				mPaletteFilePath = nullptr;
				IsDirty = true;
			}
		}
		else
		if	(
				(!mPaletteFilePath)
			||	(!mPaletteFilePath->Equals (lFullPath))
			)
		{
			try
			{
				lBitmap = gcnew System::Drawing::Bitmap (lFullPath);
			}
			catch AnyExceptionDebug
			
			if	(
					(lBitmap)
				&&	(lBitmap->Palette)
				)
			{
				mPaletteFilePath = lFullPath;
				mHeader->mPalette = lBitmap->Palette;
				IsDirty = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

UInt16 CAgentFileScript::NewFrameDuration::get()
{
	return mNewFrameDuration;
}

void CAgentFileScript::NewFrameDuration::set (UInt16 pValue)
{
	if	(
			(!IsReadOnly)
		&&	(mNewFrameDuration != pValue)
		)
	{
		mNewFrameDuration = pValue;
		IsDirty = true;
	}
}
#endif	

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
