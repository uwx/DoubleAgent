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
#include "ScriptStream.h"
#pragma warning (disable: 4395)
using namespace System::Globalization;

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
namespace Character {
/////////////////////////////////////////////////////////////////////////////

ScriptReader::ScriptReader (System::IO::Stream^ pStream)
:	StreamReader (pStream),
	mSkipComments (true)
{
	mKeywords = gcnew AcdFileKeywords;
}

ScriptReader::~ScriptReader ()
{
}

/////////////////////////////////////////////////////////////////////////////

bool ScriptReader::SkipComments::get ()
{
	return mSkipComments;
}

void ScriptReader::SkipComments::set (bool pValue)
{
	mSkipComments = pValue;
}

const AcdFileKeywords^ ScriptReader::Keywords::get ()
{
	return mKeywords;
}

System::String^ ScriptReader::Keyword::get (int pKeyword)
{
	if	(
			(pKeyword >= 0)
		&&	(pKeyword < mKeywords->Count)
		)
	{
		return mKeywords [pKeyword]->ToString ();
	}
	return String::Empty;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::String^ ScriptReader::ReadLine ()
{
	String^	lLine;

	while	(lLine = __super::ReadLine ())
	{
		lLine = lLine->Trim ();

		if	(
				(mSkipComments)
			&&	(lLine->StartsWith (_T("//")))
			)
		{
			continue;
		}
		break;
	}
	return lLine;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::String^ ScriptReader::NextToken (System::String^% pLine)
{
	String^				lToken = nullptr;
	array <wchar_t>^	lWhitespace = {' ', '\t'};
	int					lNdx;

	pLine = pLine->Trim (lWhitespace);
	lNdx = pLine->IndexOfAny (lWhitespace);
	if	(lNdx > 0)
	{
		lToken = pLine->Substring (0, lNdx);
		pLine = pLine->Substring (lNdx)->TrimStart (lWhitespace);
	}
	else
	if	(pLine->Length > 0)
	{
		lToken = pLine;
		pLine = String::Empty;
	}
	return lToken;
}

System::String^ ScriptReader::NextValue (System::String^ pLine)
{
	String^	lToken = nullptr;

	if	(
			(lToken = NextToken (pLine))
		&&	(lToken == _T("="))
		)
	{
		return GetText (pLine);
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////

System::String^ ScriptReader::GetText (System::String^ pToken)
{
	if	(
			(pToken->StartsWith (_T("\"")))
		&&	(pToken->EndsWith (_T("\"")))
		)
	{
		return pToken->Substring (1, pToken->Length-2);
	}
	return pToken;
}

int ScriptReader::GetInt (System::String^ pToken)
{
	int	lValue = 0;

	try
	{
		lValue = int::Parse (pToken, NumberStyles::Integer);
	}
	catch AnyExceptionDebug

	return lValue;
}

unsigned int ScriptReader::GetHex (System::String^ pToken)
{
	unsigned int	lValue = 0;

	try
	{
		if	(
				(pToken->StartsWith (_T("0x")))
			||	(pToken->StartsWith (_T("0X")))
			)
		{
			pToken = pToken->Substring(2);
		}
		lValue = (unsigned int)int::Parse (pToken, NumberStyles::HexNumber);
	}
	catch AnyExceptionDebug

	return lValue;
}

COLORREF ScriptReader::GetColor (System::String^ pToken)
{
	return (COLORREF)GetHex (pToken) | 0xFF000000;
}

System::Guid ScriptReader::GetGuid (System::String^ pToken)
{
	System::Guid	lGuid = Guid::Empty;

	try
	{
		lGuid = System::Guid (pToken);
	}
	catch AnyExceptionDebug

	return lGuid;
}

/////////////////////////////////////////////////////////////////////////////

array <System::String^>^ ScriptReader::GetStyles (System::String^ pToken)
{
	array <String^>^	lParts = nullptr;
	array <wchar_t>^	lDelims = {_T('|')};

	try
	{
		lParts = pToken->Split (lDelims, System::StringSplitOptions::RemoveEmptyEntries);
		for	(int lNdx = 0; lNdx < lParts->Length; lNdx++)
		{
			lParts[lNdx] = lParts[lNdx]->Trim();
		}
	}
	catch AnyExceptionSilent

	return lParts;
}

array <System::String^>^ ScriptReader::GetFontStyles (System::String^ pToken)
{
	array <String^>^	lParts = nullptr;
	array <wchar_t>^	lDelims = {_T('|')};

	try
	{
		lParts = pToken->Split (lDelims, System::StringSplitOptions::RemoveEmptyEntries);
		for	(int lNdx = 0; lNdx < lParts->Length; lNdx++)
		{
			lParts[lNdx] = lParts[lNdx]->Trim();
		}
	}
	catch AnyExceptionSilent

	return lParts;
}

/////////////////////////////////////////////////////////////////////////////

bool ScriptReader::IsKeyword (System::String^ pToken, int pKeyword)
{
	int				lCount = mKeywords->Count;

	if	(
			(pToken)
		&&	(pKeyword >= 0)
		&&	(pKeyword < mKeywords->Count)
		&&	(String::Compare (pToken, safe_cast <System::String^> (mKeywords[pKeyword]), true) == 0)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

ScriptWriter::ScriptWriter (System::IO::Stream^ pStream)
:	StreamWriter (pStream),
	mIndent (0),
	mIndentString (nullptr)
{
	mKeywords = gcnew AcdFileKeywords;
}

ScriptWriter::~ScriptWriter ()
{
}

/////////////////////////////////////////////////////////////////////////////

int ScriptWriter::Indent::get ()
{
	return mIndent;
}

void ScriptWriter::Indent::set (int pValue)
{
	mIndent = max (pValue, 0);
	if	(mIndent > 0)
	{
		mIndentString = gcnew String ('\t', mIndent);
	}
	else
	{
		mIndentString = nullptr;
	}
}

const AcdFileKeywords^ ScriptWriter::Keywords::get ()
{
	return mKeywords;
}

System::String^ ScriptWriter::Keyword::get (int pKeyword)
{
	if	(
			(pKeyword >= 0)
		&&	(pKeyword < mKeywords->Count)
		)
	{
		return mKeywords [pKeyword]->ToString ();
	}
	return String::Empty;
}

/////////////////////////////////////////////////////////////////////////////

void ScriptWriter::WriteComments ()
{
	WriteLine ("//-------------------------------------------------------------------//");
	WriteLine ("//");
	WriteLine ("//	Microsoft Agent Character Description");
	WriteLine ("//");
	WriteLine ("//  Character Version 2.1");
	WriteLine ("//");
	WriteLine ("//-------------------------------------------------------------------//");
}

void ScriptWriter::WriteSeparator ()
{
	WriteLine ();
	WriteLine ("//-------------------------------------------------------------------//");
}

void ScriptWriter::WriteKeyword (int pKeyword)
{
	WriteIndent ();
	WriteLine (Keyword [pKeyword]);
}

void ScriptWriter::WriteKeyword (int pKeyword, System::String^ pText)
{
	WriteIndent ();
	WriteLine (Keyword [pKeyword] + " " + pText);
}

void ScriptWriter::WriteValue (int pKeyword, System::String^ pValue)
{
	WriteIndent ();
	Write (Keyword [pKeyword]);
	Write (" = ");
	WriteLine (pValue);
}

void ScriptWriter::WriteIndent ()
{
	if	(mIndentString)
	{
		Write (mIndentString);
	}
}

/////////////////////////////////////////////////////////////////////////////

System::String^ ScriptWriter::FormatText (System::String^ pValue)
{
	if	(String::IsNullOrEmpty (pValue))
	{
		return gcnew String ("\"\"");
	}
	else
	{
		return String::Concat ("\"", pValue, "\"");
	}
}

System::String^ ScriptWriter::FormatInt (int pValue)
{
	return String::Format ("{0:D}", pValue);
}

System::String^ ScriptWriter::FormatHex (LANGID pValue)
{
	return String::Format ("0x{0:X4}", pValue);
}

System::String^ ScriptWriter::FormatHex (unsigned int pValue)
{
	return String::Format ("0x{0:X}", pValue);
}

System::String^ ScriptWriter::FormatColor (COLORREF pValue)
{
	return String::Format ("{0:X8}", pValue & 0x00FFFFFF);
}

System::String^ ScriptWriter::FormatGuid (System::Guid pValue)
{
	String^	lGuid;

	lGuid = pValue.ToString()->ToUpper();
	return String::Concat ("{", lGuid, "}");
}

System::String^ ScriptWriter::GetStyles (array <System::String^>^ pValue)
{
	if	(
			(pValue)
		&&	(pValue->Length > 0)
		)
	{
		return String::Join (" | ", pValue);
	}
	return String::Empty;
}

System::String^ ScriptWriter::GetFontStyles (array <System::String^>^ pValue)
{
	if	(
			(pValue)
		&&	(pValue->Length > 0)
		)
	{
		return String::Join (" | ", pValue);
	}
	return String::Empty;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

AcdFileKeywords::AcdFileKeywords ()
{
	StartHeader = Add ("DefineCharacter");
	EndHeader = Add ("EndCharacter");
	StartName = Add ("DefineInfo");
	EndName = Add ("EndInfo");
	StartBalloon = Add ("DefineBalloon");
	EndBalloon = Add ("EndBalloon");
	StartAnimation = Add ("DefineAnimation");
	EndAnimation = Add ("EndAnimation");
	StartFrame = Add ("DefineFrame");
	EndFrame = Add ("EndFrame");
	StartImage = Add ("DefineImage");
	EndImage = Add ("EndImage");
	StartOverlay = Add ("DefineMouth");
	EndOverlay = Add ("EndMouth");
	StartBranching = Add ("DefineBranching");
	EndBranching = Add ("EndBranching");
	StartState = Add ("DefineState");
	EndState = Add ("EndState");

	HeaderStyle = Add ("Style");
	HeaderGuid = Add ("GUID");
	HeaderWidth = Add ("Width");
	HeaderHeight = Add ("Height");
	HeaderIcon = Add ("Icon");
	HeaderPalette = Add ("ColorTable");
	HeaderTransparency = Add ("Transparency");
	HeaderFrameDuration = Add ("DefaultFrameDuration");

	StyleSystem = Add ("AXS_SYSTEM_CHAR");
	StyleTts = Add ("AXS_VOICE_TTS");
	StyleBalloon = Add ("AXS_BALLOON_ROUNDRECT");
	StyleBalloonNone = Add ("AXS_BALLOON_NONE");
	StyleBalloonSizeToText = Add ("AXS_BALLOON_SIZETOTEXT");
	StyleBalloonNoAutoHide = Add ("AXS_BALLOON_NOAUTOHIDE");
	StyleBalloonNoAutoPace = Add ("AXS_BALLOON_NOAUTOPACE");

	NameName = Add ("Name");
	NameDesc1 = Add ("Description");
	NameDesc2 = Add ("ExtraData");

	TtsEngine = Add ("TTSEngineID");
	TtsMode = Add ("TTSModeID");
	TtsLanguage = Add ("TTSLangID");
	TtsDialect = Add ("TTSLangDialect");
	TtsGender = Add ("TTSGender");
	TtsAge = Add ("TTSAge");
	TtsStyle = Add ("TTSStyle");

	BalloonLines = Add ("NumLines");
	BalloonPerLine = Add ("CharsPerLine");
	BalloonFgColor = Add ("ForeColor");
	BalloonBkColor = Add ("BackColor");
	BalloonBrColor = Add ("BorderColor");
	BalloonFontName = Add ("FontName");
	BalloonFontHeight = Add ("FontHeight");
	BalloonFontStyle = Add ("FontStyle");
	BalloonFontBold = Add ("BOLD");
	BalloonFontItalic = Add ("ITALIC");

	AnimationReturnType = Add ("TransitionType");
	AnimationReturnName = Add ("Return");
	StateAnimation = Add ("Animation");

	FrameDuration = Add ("Duration");
	FrameExitBranch = Add ("ExitBranch");
	FrameSound = Add ("SoundEffect");

	BranchingTarget = Add ("BranchTo");
	BranchingProbability = Add ("Probability");

	ImageFile = Add ("Filename");
	ImageOffsetX = Add ("OffsetX");
	ImageOffsetY = Add ("OffsetY");

	OverlayFile = Add ("Filename");
	OverlayOffsetX = Add ("OffsetX");
	OverlayOffsetY = Add ("OffsetY");
	OverlayType = Add ("Type");
	OverlayTypeClosed = Add ("Closed");
	OverlayTypeWide1 = Add ("OpenWide1");
	OverlayTypeWide2 = Add ("OpenWide2");
	OverlayTypeWide3 = Add ("OpenWide3");
	OverlayTypeWide4 = Add ("OpenWide4");
	OverlayTypeMedium = Add ("OpenMedium");
	OverlayTypeNarrow = Add ("OpenNarrow");
};

/////////////////////////////////////////////////////////////////////////////
} // namespace Character
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
