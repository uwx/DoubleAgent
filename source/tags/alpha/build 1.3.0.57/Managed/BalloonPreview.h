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
#include "AgentFileParts.h"

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

using namespace Character;
ref class CAgentBalloonShape;
ref class CAgentTextDraw;
ref class CAgentTextDrawForms;
ref class CAgentTextDrawWPF;

public ref class BalloonPreview
{
public:
	BalloonPreview ();
	virtual ~BalloonPreview ();

// Properties
public:
	property CharacterStyle Style {CharacterStyle get(); void set (CharacterStyle pStyle);}
	property FileBalloon^ Balloon {FileBalloon^ get(); void set (FileBalloon^ pBalloon);}

	property System::String^ Text {System::String^ get(); void set (System::String^ pText);}
	property System::String^ NextText {System::String^ get(); void set (System::String^ pText);}
	static property System::String^ DefaultText {System::String^ get();}

	property System::Boolean AutoRepeat {System::Boolean get(); void set (System::Boolean pAutoRepeat);}
	property System::Int32 AutoPaceTime {System::Int32 get();}
	static property System::Int32 DefaultAutoPaceTime {System::Int32 get();}
	property System::Int32 AutoScrollTime {System::Int32 get();}

	property System::Boolean IsEnabled {System::Boolean get();}
	property System::Boolean IsAutoSize {System::Boolean get();}
	property System::Boolean IsAutoPace {System::Boolean get();}
	property System::Boolean IsAutoHide {System::Boolean get();}
	property System::Boolean ClipPartialLines {System::Boolean get();}

// Methods
public:
	System::Boolean Draw (System::Drawing::Graphics^ pGraphics);
	System::Windows::Media::Drawing^ MakeDrawing ();
	System::Windows::Media::Visual^ MakeVisual ();

	System::Boolean AutoPaceStarted ();
	System::Boolean AutoPaceStopped ();
	System::Boolean OnAutoPace (System::Boolean% pRefresh);

	System::Boolean AutoScrollStarted ();
	System::Boolean AutoScrollStopped ();
	System::Boolean OnAutoScroll (System::Boolean% pRefresh);

// Implementation
protected:
	System::String^ GetNextText ();
	System::Drawing::RectangleF CalcShapeRect ();
	System::Drawing::SizeF GetTextSize ();
	System::Boolean InitTextLayout (System::Drawing::Graphics^ pGraphics);
	System::Boolean InitTextLayout ();
	
	CAgentTextDrawForms^ GetTextDrawForms (System::Boolean pCreate);
	CAgentTextDrawWPF^ GetTextDrawWPF (System::Boolean pCreate);

private:
	CharacterStyle			mStyle;
	FileBalloon^			mBalloon;
	System::String^			mText;
	System::Boolean			mAutoRepeat;
	System::Int32			mAutoRepeatDelay;
	CAgentBalloonShape^		mShape;
	CAgentTextDraw^			mTextDraw;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
