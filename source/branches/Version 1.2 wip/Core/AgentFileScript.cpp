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
	LogStart (false);
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
	if	(
			(mFileStream)
		||	(String::IsNullOrEmpty (mPath))
		)
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
bool CAgentFileScript::Open (const System::String^ pPath)
{
	bool			lRet = false;
	String^			lPath = ParseFilePath (pPath);
	ScriptReader^	lReader;

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	Close ();
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Open [%s]"), _B(lPath));
	}

	try
	{
		mFileStream = gcnew FileStream (lPath, FileMode::Open, FileAccess::ReadWrite, FileShare::Read);
	}
	catch (Exception^ pException)
	{
		__LogCliException (LogIfActive);
		throw pException;
	}

	if	(
			(mFileStream)
		&&	(lReader = gcnew ScriptReader (mFileStream))
		)
	{
		mPath = lPath;

		try
		{
			lRet = ParseFile (lReader);
		}
		catch (Exception^ pException)
		{
			__LogCliException (LogIfActive);

			try
			{
				lReader->Close ();
				lReader = nullptr;
				Close ();
			}
			catch AnyExceptionDebug

			throw pException;
		}

		lReader->Close ();
	}
	if	(!lRet)
	{
		Close ();
	}

#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif	
	return lRet;
}
#else
HRESULT CAgentFileScript::Open (LPCTSTR pPath)
{
	return E_NOTIMPL;
}
#endif

#ifdef	_M_CEE
bool CAgentFileScript::Save (const System::String^ pPath, CAgentFile^ pSource)
{
	bool				lRet = false;
	String^				lOldPath = mPath;
	String^				lNewPath = nullptr;
	ScriptWriter^		lWriter;

	if	(!String::IsNullOrEmpty (const_cast <String^> (pPath)))
	{
		lNewPath = ParseFilePath (pPath);
	}
	if	(LogIsActive (mLogLevel))
	{
		if	(pSource == this)
		{
			LogMessage (mLogLevel, _T("Save [%s] as [%s]"), _B(mPath), _B(lNewPath));
		}
		else
		if	(!String::IsNullOrEmpty (lNewPath))
		{
			LogMessage (mLogLevel, _T("Save [%s] as [%s]"), _B(pSource->Path), _B(lNewPath));
		}
		else
		{
			LogMessage (mLogLevel, _T("Save [%s]"), _B(mPath));
		}
	}

	if	(
			(
				(IsOpen)
			&&	(!IsReadOnly)
			)
		||	(!String::IsNullOrEmpty (lNewPath))
		||	(
				(pSource != this)
			&&	(pSource->IsOpen)
			)
		)
	{
		try
		{
			if	(!String::IsNullOrEmpty (lNewPath))
			{
				mPath = lNewPath;
			}
			if	(mFileStream)
			{
				try
				{
					mFileStream->Close ();
				}
				catch AnyExceptionDebug
				try
				{
					mFileStream->~FileStream ();
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
				FreeUnusedImages ();
				FreeUnusedSounds ();
				if	(!pSource->IsAcdFile)
				{
					PrepareFileFolder (true);
				}
				lRet = WriteFile (lWriter, pSource);
				lWriter->Close ();
			}
			if	(lRet)
			{
				IsDirty = false;
			}
			else
			{
				mPath = lOldPath;
			}
			try
			{
				mFileStream = gcnew FileStream (mPath, FileMode::Open, FileAccess::ReadWrite, FileShare::Read);
			}
			catch AnyExceptionDebug
		}
		catch AnyExceptionDebug
	}
	return lRet;
}
#endif

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
		try
		{
			mFileStream->~FileStream ();
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

bool CAgentFileScript::ParseFile (ScriptReader^ pReader)
{
	bool	lRet = true;
#ifdef	_M_CEE
	String^	lLine;
	String^	lToken;

	while (lLine = pReader->ReadLine ())
	{
		if	(lToken = pReader->NextToken (lLine))
		{
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartHeader))
			{
				lRet = ParseHeader (pReader);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartBalloon))
			{
				lRet = ParseBalloon (pReader);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartAnimation))
			{
				lRet = ParseAnimation (pReader, pReader->GetText (lLine));
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartState))
			{
				lRet = ParseState (pReader, pReader->GetText (lLine));
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lRet = false;
			}
		}
		if	(!lRet)
		{
			break;
		}
	}
#else
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::WriteFile (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	pWriter->WriteComments ();
	WriteHeader (pWriter, pSource);
	WriteBalloon (pWriter, pSource);
	WriteAnimations (pWriter, pSource);
	WriteStates (pWriter, pSource);
#else
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseHeader (ScriptReader^ pReader)
{
	bool	lRet = true;
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
					lRet = ParseName (pReader, pReader->GetHex (lToken));
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
				//lRet = false;
			}
		}
		if	(!lRet)
		{
			break;
		}
	}
#else
#endif
	return lRet;
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

bool CAgentFileScript::WriteHeader (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	String^	lIconFilePath = pSource->IconFilePath;
	String^	lPaletteFilePath = pSource->PaletteFilePath;

	pWriter->WriteLine ();
	pWriter->WriteKeyword (pWriter->Keywords->StartHeader);
	pWriter->WriteLine ();
	pWriter->Indent++;

	WriteNames (pWriter, pSource);
	pWriter->WriteValue (pWriter->Keywords->HeaderGuid, pWriter->FormatGuid (pSource->Header->Guid));
	if	(
			(pSource->Header->Icon)
		&&	(String::IsNullOrEmpty (lIconFilePath))
		)
	{
		lIconFilePath = WriteIconFile (pSource->Header);
	}
	if	(!String::IsNullOrEmpty (lIconFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->HeaderIcon, pWriter->FormatText (lIconFilePath));
	}
	pWriter->WriteValue (pWriter->Keywords->HeaderWidth, pWriter->FormatInt (pSource->Header->ImageSize.Width));
	pWriter->WriteValue (pWriter->Keywords->HeaderHeight, pWriter->FormatInt (pSource->Header->ImageSize.Height));
	pWriter->WriteValue (pWriter->Keywords->HeaderTransparency, pWriter->FormatInt (pSource->Header->Transparency));
	pWriter->WriteValue (pWriter->Keywords->HeaderFrameDuration, pWriter->FormatInt (NewFrameDuration));
	WriteStyle (pWriter, (UInt32)pSource->Header->Style);
	WriteTts (pWriter, pSource);
	if	(
			(pSource->Header->Palette)
		&&	(String::IsNullOrEmpty (lPaletteFilePath))
		)
	{
		lPaletteFilePath = WritePaletteFile (pSource->Header);
	}
	if	(!String::IsNullOrEmpty (lPaletteFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->HeaderPalette, pWriter->FormatText (lPaletteFilePath));
	}

	pWriter->Indent--;
	pWriter->WriteLine ();
	pWriter->WriteKeyword (pWriter->Keywords->EndHeader);
	pWriter->WriteSeparator ();
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteStyle (ScriptWriter^ pWriter, UInt32 pStyle)
{
	bool	lRet = true;
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
	return lRet;
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

bool CAgentFileScript::WriteTts (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	if	(pSource->Tts)
	{
		pWriter->WriteValue (pWriter->Keywords->TtsEngine, pWriter->FormatGuid (pSource->Tts->Engine));
		pWriter->WriteValue (pWriter->Keywords->TtsMode, pWriter->FormatGuid (pSource->Tts->Mode));
		pWriter->WriteValue (pWriter->Keywords->TtsLanguage, pWriter->FormatHex (pSource->Tts->Language));
		pWriter->WriteValue (pWriter->Keywords->TtsGender, pWriter->FormatInt (pSource->Tts->Gender));
		//pWriter->WriteValue (pWriter->Keywords->TtsAge, pWriter->FormatInt (pSource->Tts->Age));
		//pWriter->WriteValue (pWriter->Keywords->TtsStyle, pWriter->FormatText (pSource->Tts->Style));
	}
#else
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseBalloon (ScriptReader^ pReader)
{
	bool						lRet = true;
#ifdef	_M_CEE
	String^						lLine;
	String^						lToken;
	String^						lFontName = nullptr;
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
				//lRet = false;
			}
		}
	}

	if	(lFontName)
	{
		mBalloon->mFont = gcnew System::Drawing::Font (lFontName, (float)lFontSize, lFontStyle);
	}
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteBalloon (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	if	(pSource->Balloon)
	{
		pWriter->WriteLine ();
		pWriter->WriteKeyword (pWriter->Keywords->StartBalloon);
		pWriter->Indent++;

		pWriter->WriteValue (pWriter->Keywords->BalloonLines, pWriter->FormatInt (pSource->Balloon->Lines));
		pWriter->WriteValue (pWriter->Keywords->BalloonPerLine, pWriter->FormatInt (pSource->Balloon->PerLine));
		if	(pSource->Balloon->Font)
		{
			List <String^>^	lStyle = gcnew List <String^>;

			if	(pSource->Balloon->Font->Bold)
			{
				lStyle->Add (pWriter->Keyword [pWriter->Keywords->BalloonFontBold]);
			}
			if	(pSource->Balloon->Font->Italic)
			{
				lStyle->Add (pWriter->Keyword [pWriter->Keywords->BalloonFontItalic]);
			}
			pWriter->WriteValue (pWriter->Keywords->BalloonFontName, pWriter->FormatText (pSource->Balloon->Font->OriginalFontName ? pSource->Balloon->Font->OriginalFontName : pSource->Balloon->Font->Name));
			pWriter->WriteValue (pWriter->Keywords->BalloonFontHeight, pWriter->FormatInt ((int)(pSource->Balloon->Font->SizeInPoints + 0.49f)));
			pWriter->WriteValue (pWriter->Keywords->BalloonFontStyle, pWriter->GetFontStyles (lStyle->ToArray()));
			pWriter->WriteValue (pWriter->Keywords->BalloonFgColor, pWriter->FormatColor (pSource->Balloon->FgColor.ToArgb()));
			pWriter->WriteValue (pWriter->Keywords->BalloonBkColor, pWriter->FormatColor (pSource->Balloon->BkColor.ToArgb()));
			pWriter->WriteValue (pWriter->Keywords->BalloonBrColor, pWriter->FormatColor (pSource->Balloon->BrColor.ToArgb()));
		}

		pWriter->Indent--;
		pWriter->WriteKeyword (pWriter->Keywords->EndBalloon);
		pWriter->WriteSeparator ();
	}
#else
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseName (ScriptReader^ pReader, LANGID pLangID)
{
	bool			lRet = true;
#ifdef	_M_CEE
	String^			lLine;
	String^			lToken;
	CAgentFileName^	lName = gcnew CAgentFileName (this, Names);

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
				//lRet = false;
			}
		}
		if	(!lRet)
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
	return lRet;
}

bool CAgentFileScript::WriteNames (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	if	(pSource->Names)
	{
		for each (CAgentFileName^ lName in pSource->Names)
		{
			WriteName (pWriter, lName);
		}
	}
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteName (ScriptWriter^ pWriter, CAgentFileName^ pName)
{
	bool	lRet = true;
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
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseAnimation (ScriptReader^ pReader, System::String^ pAnimationName)
{
	bool					lRet = true;
#ifdef	_M_CEE
	String^					lLine;
	String^					lToken;
	CAgentFileAnimation^	lAnimation = gcnew CAgentFileAnimation (this, mGestures);

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
				lRet = ParseAnimationFrame (pReader, lAnimation);
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lRet = false;
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
	return lRet;
}

bool CAgentFileScript::WriteAnimations (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	if	(pSource->Gestures)
	{
		for each (CAgentFileAnimation^ lAnimation in pSource->Gestures)
		{
			WriteAnimation (pWriter, pSource, lAnimation);
		}
	}
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteAnimation (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileAnimation^ pAnimation)
{
	bool	lRet = true;
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
			WriteAnimationFrame (pWriter, pSource, lFrame);
		}
	}

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndAnimation);
	pWriter->WriteSeparator ();
#else
#endif
	return lRet;
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

		mGestures->ChangeItemKey (pAnimation, String::Copy (pNewName));
		return true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseAnimationFrame (ScriptReader^ pReader, CAgentFileAnimation^ pAnimation)
{
	bool				lRet = true;
#ifdef	_M_CEE
	String^				lLine;
	String^				lToken;
	CAgentFileFrame^	lFrame = gcnew CAgentFileFrame (this, pAnimation->Frames);

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
				lRet = ParseFrameImage (pReader, lFrame);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartOverlay))
			{
				lRet = ParseFrameOverlay (pReader, lFrame);
			}
			else
			if	(pReader->IsKeyword (lToken, pReader->Keywords->StartBranching))
			{
				lRet = ParseFrameBranching (pReader, lFrame);
			}
			else
			{
				LogComErr (LogNormal, E_INVALIDARG, _T("[%s]"), _B(lToken));
				//lRet = false;
			}
		}
	}

	if (!pAnimation->mFrames)
	{
		pAnimation->mFrames = gcnew CAgentFileFrames (this, pAnimation);
	}
	pAnimation->mFrames->Add (lFrame);
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteAnimationFrame (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrame^ pFrame)
{
	bool	lRet = true;
#ifdef	_M_CEE
	String^	lSoundFilePath = pFrame->SoundFilePath;
	
	pWriter->WriteKeyword (pWriter->Keywords->StartFrame);
	pWriter->Indent++;

	pWriter->WriteValue (pWriter->Keywords->FrameDuration, pWriter->FormatInt (pFrame->Duration));
	if	(pFrame->ExitFrame >= 0)
	{
		pWriter->WriteValue (pWriter->Keywords->FrameExitBranch, pWriter->FormatInt (pFrame->ExitFrame));
	}
	if	(
			(pFrame->SoundNdx >= 0)
		&&	(String::IsNullOrEmpty (lSoundFilePath))
		)
	{
		lSoundFilePath = WriteSoundFile (pSource, pFrame->SoundNdx);
	}
	if	(!String::IsNullOrEmpty (lSoundFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->FrameSound, pWriter->FormatText (lSoundFilePath));
	}
	WriteFrameBranching (pWriter, pSource, pFrame);

	if	(pFrame->mOverlays)
	{
		for each (CAgentFileFrameOverlay^ lOverlay in pFrame->mOverlays)
		{
			WriteFrameOverlay (pWriter, pSource, lOverlay);
		}
	}
	if	(pFrame->mImages)
	{
		for each (CAgentFileFrameImage^ lImage in pFrame->mImages)
		{
			WriteFrameImage (pWriter, pSource, lImage);
		}
	}

	pWriter->Indent--;
	pWriter->WriteKeyword (pWriter->Keywords->EndFrame);
	pWriter->WriteLine ();
#else
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseFrameBranching (ScriptReader^ pReader, CAgentFileFrame^ pFrame)
{
	bool	lRet = true;
#ifdef	_M_CEE
	String^	lLine;
	String^	lToken;
	Int16	lBranchTarget = -1;
	UInt16	lBranchProbability = 0;
	int		lBranchNdx = 0;

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
				//lRet = false;
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
	return lRet;
}

bool CAgentFileScript::WriteFrameBranching (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrame^ pFrame)
{
	bool	lRet = true;
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
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseFrameImage (ScriptReader^ pReader, CAgentFileFrame^ pFrame)
{
	bool					lRet = true;
#ifdef	_M_CEE
	String^					lLine;
	String^					lToken;
	CAgentFileFrameImage^	lImage = gcnew CAgentFileFrameImage (this, pFrame);

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
				//lRet = false;
			}
		}
	}

	if (!pFrame->mImages)
	{
		pFrame->mImages = gcnew CAgentFileFrameImages (this, pFrame);
	}
	pFrame->mImages->Add (lImage);
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteFrameImage (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrameImage^ pFrameImage)
{
	bool	lRet = true;
#ifdef	_M_CEE
	String^	lImageFilePath = pFrameImage->ImageFilePath;
	
	pWriter->WriteKeyword (pWriter->Keywords->StartImage);
	pWriter->Indent++;

	if	(
			(pFrameImage->ImageNdx >= 0)
		&&	(String::IsNullOrEmpty (lImageFilePath))
		)
	{
		lImageFilePath = WriteImageFile (pSource, pFrameImage->ImageNdx, false);
	}
	if	(!String::IsNullOrEmpty (lImageFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->ImageFile, pWriter->FormatText (lImageFilePath));
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
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseFrameOverlay (ScriptReader^ pReader, CAgentFileFrame^ pFrame)
{
	bool					lRet = true;
#ifdef	_M_CEE
	String^					lLine;
	String^					lToken;
	CAgentFileFrameOverlay^	lOverlay = gcnew CAgentFileFrameOverlay (this, pFrame);

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
						//lRet = false;
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
		pFrame->mOverlays = gcnew CAgentFileFrameOverlays (this, pFrame);
	}
	pFrame->mOverlays->Add (lOverlay);
#else
#endif
	return lRet;
}

bool CAgentFileScript::WriteFrameOverlay (ScriptWriter^ pWriter, CAgentFile^ pSource, CAgentFileFrameOverlay^ pFrameOverlay)
{
	bool	lRet = true;
#ifdef	_M_CEE
	String^	lImageFilePath = pFrameOverlay->ImageFilePath;

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
	
	if	(
			(pFrameOverlay->ImageNdx >= 0)
		&&	(String::IsNullOrEmpty (lImageFilePath))
		)
	{
		lImageFilePath = WriteImageFile (pSource, pFrameOverlay->ImageNdx, true);
	}
	if	(!String::IsNullOrEmpty (lImageFilePath))
	{
		pWriter->WriteValue (pWriter->Keywords->OverlayFile, pWriter->FormatText (lImageFilePath));
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
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileScript::ParseState (ScriptReader^ pReader, System::String^ pStateName)
{
	bool			lRet = true;
#ifdef	_M_CEE
	String^			lLine;
	String^			lToken;
	List <String^>^	lAnimations = gcnew List <String^>;

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
				//lRet = false;
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
	return lRet;
}

bool CAgentFileScript::WriteStates (ScriptWriter^ pWriter, CAgentFile^ pSource)
{
	bool	lRet = true;
#ifdef	_M_CEE
	if	(pSource->States)
	{
		for each (KeyValuePair <String^, array <String^>^> lState in pSource->States)
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
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

void CAgentFileScript::FreeUnusedImages ()
{
	if	(
			(mImageFilePaths)
		&&	(mImageFilePaths->Count > 0)
		)
	{
		array <int>^	lImageUsage = gcnew array <int> (mImageFilePaths->Count);
		int				lImageNdx;
		int				lUnusedCount = 0;
		
		for (lImageNdx = 0; lImageNdx < lImageUsage->Length; lImageNdx++)
		{
			lImageUsage[lImageNdx] = 0;
		}
		
		for each (CAgentFileAnimation^ lAnimation in Gestures)
		{
			if	(lAnimation->Frames)
			{
				for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
				{
					if	(lFrame->Images)
					{
						for each (CAgentFileFrameImage^ lImage in lFrame->Images)
						{
							if	(
									(lImage->mImageNdx >= 0)
								&&	(lImage->mImageNdx < lImageUsage->Length)
								)
							{
								lImageUsage[lImage->mImageNdx]++;
							}
						}
					}
					if	(lFrame->Overlays)
					{
						for each (CAgentFileFrameOverlay^ lOverlay in lFrame->Overlays)
						{
							if	(
									(lOverlay->mImageNdx >= 0)
								&&	(lOverlay->mImageNdx < lImageUsage->Length)
								)
							{
								lImageUsage[lOverlay->mImageNdx]++;
							}
						}
					}
				}
			}
		}

		for (lImageNdx = 0; lImageNdx < lImageUsage->Length; lImageNdx++)
		{
			if	(lImageUsage[lImageNdx] > 0)
			{
				lImageUsage[lImageNdx] = lUnusedCount;
			}
			else
			{
				if	(LogIsActive (LogNormal))
				{
					LogMessage (LogNormal, _T("  [%s] Image [%d] [%s] unused"), _B(mPath), lImageNdx, _B(mImageFilePaths[lImageNdx-lUnusedCount])); 
				}
				if	(
						(mImages)
					&&	(mImages->ContainsKey (mImageFilePaths[lImageNdx-lUnusedCount]))
					)
				{
					mImages->Remove (mImageFilePaths[lImageNdx-lUnusedCount]);
				}
				mImageFilePaths->RemoveAt (lImageNdx-lUnusedCount);
				lUnusedCount++;
			}
		}
		
		if	(lUnusedCount > 0)
		{
			for each (CAgentFileAnimation^ lAnimation in Gestures)
			{
				if	(lAnimation->Frames)
				{
					for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
					{
						if	(lFrame->Images)
						{
							for each (CAgentFileFrameImage^ lImage in lFrame->Images)
							{
								if	(
										(lImage->ImageNdx >= 0)
									&&	(lImage->ImageNdx < lImageUsage->Length)
									)
								{
									lImage->mImageNdx -= lImageUsage[lImage->mImageNdx];
								}
							}
						}
						if	(lFrame->Overlays)
						{
							for each (CAgentFileFrameOverlay^ lOverlay in lFrame->Overlays)
							{
								if	(
										(lOverlay->mImageNdx >= 0)
									&&	(lOverlay->mImageNdx < lImageUsage->Length)
									)
								{
									lOverlay->mImageNdx -= lImageUsage[lOverlay->mImageNdx];
								}
							}
						}
					}
				}
			}
		}
	}
}

void CAgentFileScript::FreeUnusedSounds ()	
{
	if	(
			(mSoundFilePaths)
		&&	(mSoundFilePaths->Count > 0)
		)
	{
		array <int>^	lSoundUsage = gcnew array <int> (mSoundFilePaths->Count);
		int				lSoundNdx;
		int				lUnusedCount = 0;
		
		for (lSoundNdx = 0; lSoundNdx < lSoundUsage->Length; lSoundNdx++)
		{
			lSoundUsage[lSoundNdx] = 0;
		}
		
		for each (CAgentFileAnimation^ lAnimation in Gestures)
		{
			if	(lAnimation->Frames)
			{
				for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
				{
					if	(
							(lFrame->mSoundNdx >= 0)
						&&	(lFrame->mSoundNdx < lSoundUsage->Length)
						)
					{
						lSoundUsage[lFrame->mSoundNdx]++;
					}
				}
			}
		}

		for (lSoundNdx = 0; lSoundNdx < lSoundUsage->Length; lSoundNdx++)
		{
			if	(lSoundUsage[lSoundNdx] > 0)
			{
				lSoundUsage[lSoundNdx] = lUnusedCount;
			}
			else
			{
				if	(LogIsActive (LogNormal))
				{
					LogMessage (LogNormal, _T("  [%s] Sound [%d] [%s] unused"), _B(mPath), lSoundNdx, _B(mSoundFilePaths[lSoundNdx-lUnusedCount])); 
				}
				if	(
						(mSounds)
					&&	(mSounds->ContainsKey (mSoundFilePaths[lSoundNdx-lUnusedCount]))
					)
				{
					mSounds->Remove (mSoundFilePaths[lSoundNdx-lUnusedCount]);
				}
				mSoundFilePaths->RemoveAt (lSoundNdx-lUnusedCount);
				lUnusedCount++;
			}
		}
		
		if	(lUnusedCount > 0)
		{
			for each (CAgentFileAnimation^ lAnimation in Gestures)
			{
				if	(lAnimation->Frames)
				{
					for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
					{
						if	(
								(lFrame->mSoundNdx >= 0)
							&&	(lFrame->mSoundNdx < lSoundUsage->Length)
							)
						{
							lFrame->mSoundNdx -= lSoundUsage[lFrame->mSoundNdx];
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

String^ CAgentFileScript::PrepareFileFolder ()
{
	return PrepareFileFolder (false);
}

String^ CAgentFileScript::PrepareFileFolder (Boolean pEmptyFolder)
{
	String^			lPath = System::IO::Path::Combine (System::IO::Path::GetDirectoryName (mPath), System::IO::Path::GetFileNameWithoutExtension (mPath) + " files");
	DirectoryInfo^	lDirectoryInfo = gcnew System::IO::DirectoryInfo (lPath);

	if	(lDirectoryInfo)
	{	
		if	(
				(lDirectoryInfo->Exists)
			&&	(pEmptyFolder)
			)
		{
			lDirectoryInfo->Delete (true);
			lDirectoryInfo = gcnew System::IO::DirectoryInfo (lPath);
		}
		if	(!lDirectoryInfo->Exists)
		{
			lDirectoryInfo->Create ();
			lDirectoryInfo = gcnew System::IO::DirectoryInfo (lPath);
		}
		if	(
				(lDirectoryInfo)
			&&	(lDirectoryInfo->Exists)
			)
		{
			return lPath;
		}
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////

String^ CAgentFileScript::WriteIconFile (CAgentFileHeader^ pSource)
{
	String^	lRet = nullptr;

	if	(
			(pSource)	
		&&	(pSource->Icon)
		)
	{
		try
		{
			String^	lPath = PrepareFileFolder ();
			
			if	(lPath)
			{
				lPath = System::IO::Path::Combine (lPath, System::IO::Path::GetFileNameWithoutExtension (mPath) + ".ico");
			}
			if	(lPath)
			{
				FileStream^	lStream = gcnew FileStream (lPath, FileMode::Create);
				
				if	(lStream)
				{
					pSource->Icon->Save (lStream);
					lStream->Close ();
					lRet = lPath;
				}
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

String^ CAgentFileScript::WritePaletteFile (CAgentFileHeader^ pSource)
{
	String^	lRet = nullptr;

	if	(
			(pSource)	
		&&	(pSource->Palette)
		&&	(pSource->Palette->Entries->Length == 256)
		)
	{
		try
		{
			String^	lPath = PrepareFileFolder ();
			
			if	(lPath)
			{
				lPath = System::IO::Path::Combine (lPath, System::IO::Path::GetFileNameWithoutExtension (mPath) + ".bmp");
			}
			if	(lPath)
			{
				Bitmap^	lBitmap = gcnew Bitmap (16, 16, PixelFormat::Format8bppIndexed);
				
				lBitmap->Palette = pSource->Palette;

				try
				{
					int			lColorNdx;
					BitmapData^	lData;
					LPBYTE		lBytes;

					lData = lBitmap->LockBits (System::Drawing::Rectangle (0, 0, lBitmap->Width, lBitmap->Height), ImageLockMode::ReadWrite, PixelFormat::Format8bppIndexed);
					lBytes = (LPBYTE)(INT_PTR)lData->Scan0;

					for (lColorNdx = 0; lColorNdx < lData->Stride * lData->Height; lColorNdx++)
					{
						lBytes[lColorNdx] = (Byte)((lColorNdx / lData->Stride * 16) + (lColorNdx % lData->Stride));
					}

					lBitmap->UnlockBits (lData);
				}
				catch AnyExceptionDebug
				
				lBitmap->Save (lPath, ImageFormat::Bmp);
				lRet = lPath;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

String^ CAgentFileScript::WriteImageFile (CAgentFile^ pSource, int pImageNdx, Boolean pForOverlay)
{
	String^	lRet = nullptr;
	Bitmap^	lBitmap;

	if	(
			(pSource)	
		&&	(lBitmap = pSource->GetImageBitmap (pImageNdx))
		)
	{
		try
		{
			String^	lPath = PrepareFileFolder ();
			
			if	(lPath)
			{
				lPath = System::IO::Path::Combine (lPath, String::Format ("{0} {1:D}", (pForOverlay ? "Overlay" : "Image"), pImageNdx+1) + ".bmp");
			}
			if	(lPath)
			{
				lBitmap->Save (lPath, ImageFormat::Bmp);
				lRet = lPath;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

String^ CAgentFileScript::WriteSoundFile (CAgentFile^ pSource, int pSoundNdx)
{
	String^			lRet = nullptr;
	array <BYTE>^	lSound;

	if	(
			(pSource)	
		&&	(lSound = pSource->GetSound (pSoundNdx))
		)
	{
		try
		{
			String^	lPath = PrepareFileFolder ();
			
			if	(lPath)
			{
				lPath = System::IO::Path::Combine (lPath, String::Format ("Sound {0:D}", pSoundNdx+1) + ".wav");
			}
			if	(lPath)
			{
				FileStream^		lFileStream = nullptr;
				BinaryWriter^	lFileWriter = nullptr;
				
				lFileStream = gcnew FileStream (lPath, FileMode::Create);
				if	(lFileStream)
				{
					lFileWriter = gcnew BinaryWriter (lFileStream);
				}
				if	(lFileWriter)
				{
					lFileWriter->Write (lSound);
					lFileWriter->Close ();
					lRet = lPath;
				}
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
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

		if	(mImages->ContainsKey (lPath))
		{
			lImageNdx = mImageFilePaths->FindIndex (gcnew Predicate <String^> (lPath, &String::Equals));
		}
		else
		{
			System::Drawing::Bitmap^	lBitmap = nullptr;

			try
			{
				lBitmap = gcnew System::Drawing::Bitmap (lPath);
			}
			catch AnyExceptionDebug

			lImageNdx = (Int32)mImageFilePaths->Count;
			mImages->Add (lPath, lBitmap);
			mImageFilePaths->Add (lPath);
		}
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

		if	(mSounds->ContainsKey (lPath))
		{
			lSoundNdx = mSoundFilePaths->FindIndex (gcnew Predicate <String^> (lPath, &String::Equals));
		}
		else
		{
			System::Media::SoundPlayer^	lPlayer = nullptr;

			try
			{
				lPlayer = gcnew System::Media::SoundPlayer (lPath);
			}
			catch AnyExceptionDebug

			lSoundNdx = (Int16)mSoundFilePaths->Count;
			mSounds->Add (lPath, lPlayer);
			mSoundFilePaths->Add (lPath);
		}
	}
	catch AnyExceptionDebug

	return lSoundNdx;
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileImage^ CAgentFileScript::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
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
	return (mSoundFilePaths) ? mSoundFilePaths->Count : 0;
}

int CAgentFileScript::GetSoundSize (int pSoundNdx)
{
	int	lSoundSize = 0;

	if	(
			(mSoundFilePaths)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < mSoundFilePaths->Count)
		)
	{
		try
		{
			lSoundSize = (int)System::IO::FileInfo::FileInfo (mSoundFilePaths [pSoundNdx]).Length;
		}
		catch AnyExceptionDebug
	}

	return lSoundSize;
}

array <BYTE>^ CAgentFileScript::GetSound (int pSoundNdx)
{
	array <BYTE>^	lSound = nullptr;
	
	if	(
			(mSoundFilePaths)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < mSoundFilePaths->Count)
		)
	{
		try
		{
			System::IO::FileStream^		lFileStream = nullptr;
			System::IO::BinaryReader^	lFileReader = nullptr;
			
			lFileStream = gcnew FileStream (mSoundFilePaths [pSoundNdx], FileMode::Open, FileAccess::Read, FileShare::ReadWrite);
			if	(
					(lFileStream)
				&&	(lFileStream->Length > 0)
				)
			{
				lFileReader = gcnew System::IO::BinaryReader (lFileStream);
			}
			if	(lFileReader)
			{
				lSound = lFileReader->ReadBytes ((int)lFileStream->Length);
			}
		}
		catch AnyExceptionDebug
	}
	return lSound;
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
