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
using namespace System;
using namespace System::IO;
using namespace System::Collections;

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
namespace Character {
/////////////////////////////////////////////////////////////////////////////

private	ref class AcdFileKeywords : public ArrayList
{
	public:
		int StartHeader;
		int EndHeader;
		int StartName;
		int EndName;
		int StartBalloon;
		int EndBalloon;
		int StartAnimation;
		int EndAnimation;
		int StartFrame;
		int EndFrame;
		int StartImage;
		int EndImage;
		int StartOverlay;
		int EndOverlay;
		int StartBranching;
		int EndBranching;
		int StartState;
		int EndState;

		int HeaderStyle;
		int HeaderGuid;
		int HeaderWidth;
		int HeaderHeight;
		int HeaderIcon;
		int HeaderPalette;
		int HeaderTransparency;
		int HeaderFrameDuration;

		int StyleSystem;
		int StyleTts;
		int StyleBalloon;
		int StyleBalloonNone;
		int StyleBalloonSizeToText;
		int StyleBalloonNoAutoHide;
		int StyleBalloonNoAutoPace;

		int NameName;
		int NameDesc1;
		int NameDesc2;

		int TtsEngine;
		int TtsMode;
		int TtsLanguage;
		int TtsDialect;
		int TtsGender;
		int TtsAge;
		int TtsStyle;

		int BalloonLines;
		int BalloonPerLine;
		int BalloonFgColor;
		int BalloonBkColor;
		int BalloonBrColor;
		int BalloonFontName;
		int BalloonFontHeight;
		int BalloonFontStyle;
		int BalloonFontBold;
		int BalloonFontItalic;

		int AnimationReturnType;
		int AnimationReturnName;
		int StateAnimation;

		int FrameDuration;
		int FrameExitBranch;
		int FrameSound;

		int BranchingTarget;
		int BranchingProbability;

		int ImageFile;
		int ImageOffsetX;
		int ImageOffsetY;

		int OverlayFile;
		int OverlayOffsetX;
		int OverlayOffsetY;
		int OverlayType;
		int OverlayTypeClosed;
		int OverlayTypeWide1;
		int OverlayTypeWide2;
		int OverlayTypeWide3;
		int OverlayTypeWide4;
		int OverlayTypeMedium;
		int OverlayTypeNarrow;

	public:
		AcdFileKeywords ();
};

/////////////////////////////////////////////////////////////////////////////

private ref class ScriptReader : public System::IO::StreamReader
{
public:
	ScriptReader (System::IO::Stream^ pStream);
	virtual ~ScriptReader ();

// Properties
public:
	property bool SkipComments {bool get(); void set (bool pValue);}
	property const AcdFileKeywords^ Keywords {const AcdFileKeywords^ get ();}
	property System::String^ Keyword [int] {System::String^ get (int pKeyword);}

// Operations
public:
	static System::String^ NextToken (System::String^% pLine);
	static System::String^ NextValue (System::String^ pLine);

	static System::String^ GetText (System::String^ pToken);
	static int GetInt (System::String^ pToken);
	static unsigned int GetHex (System::String^ pToken);
	static COLORREF GetColor (System::String^ pToken);
	static System::Guid GetGuid (System::String^ pToken);
	static array <System::String^>^ GetStyles (System::String^ pToken);
	static array <System::String^>^ GetFontStyles (System::String^ pToken);

	bool IsKeyword (System::String^ pToken, int pKeyword);

// Overrides
public:
	virtual System::String^ ReadLine () override;

// Implementation
protected:

protected:
	bool				mSkipComments;
	AcdFileKeywords^		mKeywords;
};

/////////////////////////////////////////////////////////////////////////////

private ref class ScriptWriter : public System::IO::StreamWriter
{
public:
	ScriptWriter (System::IO::Stream^ pStream);
	virtual ~ScriptWriter ();

// Properties
public:
	property int Indent {int get(); void set (int pValue);}
	property const AcdFileKeywords^ Keywords {const AcdFileKeywords^ get ();}
	property System::String^ Keyword [int] {System::String^ get (int pKeyword);}

// Operations
public:
	void WriteComments ();
	void WriteSeparator ();
	void WriteKeyword (int pKeyword);
	void WriteKeyword (int pKeyword, System::String^ pText);
	void WriteValue (int pKeyword, System::String^ pValue);

	static System::String^ FormatText (System::String^ pValue);
	static System::String^ FormatInt (int pValue);
	static System::String^ FormatHex (LANGID pValue);
	static System::String^ FormatHex (unsigned int pValue);
	static System::String^ FormatColor (COLORREF pValue);
	static System::String^ FormatGuid (System::Guid pValue);
	static System::String^ GetStyles (array <System::String^>^ pValue);
	static System::String^ GetFontStyles (array <System::String^>^ pValue);

protected:
	void WriteIndent ();

protected:
	int					mIndent;
	System::String^	mIndentString;
	AcdFileKeywords^	mKeywords;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace Character
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
