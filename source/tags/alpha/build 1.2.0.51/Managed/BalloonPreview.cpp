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
#include "BalloonPreview.h"
#include "AgentBalloonShape.h"
#include "AgentText.h"
#include "AgentTextParse.h"
#include "FontProperties.h"
#include "DebugStr.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows;
using namespace System::Windows::Media;
using namespace System::Windows::Media::Effects;
using namespace System::Windows::Media::TextFormatting;
using namespace System::Threading;

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

BalloonPreview::BalloonPreview ()
:	mStyle (CharacterStyle::None),
	mAutoRepeat (true),
	mAutoRepeatDelay (0),
	mShape (nullptr),
	mTextDraw (nullptr)
{
	Style = CharacterStyle::Balloon;
	Text = GetNextText();
}

BalloonPreview::~BalloonPreview ()
{
	if	(mShape)
	{
		delete mShape;
	}
	if	(mTextDraw)
	{
		delete mTextDraw;
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::IsEnabled::get()
{
	Boolean lRet = false;
	
	Monitor::Enter (this);
	try 
	{
		if	(Balloon)
		{
			lRet = ((Style & CharacterStyle::Balloon) != CharacterStyle::None);
		}
		else
		{
			lRet = true;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::IsAutoSize::get()
{
	Boolean lRet = false;
	
	Monitor::Enter (this);
	try 
	{
		if	(
				(Balloon)
			&&	(Balloon->Lines < FileBalloon::MinLines)
			)
		{
			lRet = true;
		}
		else
		{
			lRet = ((Style & CharacterStyle::SizeToText) != CharacterStyle::None);
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::IsAutoPace::get()
{
	Boolean lRet = false;
	
	Monitor::Enter (this);
	try 
	{
		lRet = ((Style & CharacterStyle::NoAutoPace) == CharacterStyle::None);
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::IsAutoHide::get()
{
	Boolean lRet = false;
	
	Monitor::Enter (this);
	try 
	{
		lRet = ((Style & CharacterStyle::NoAutoHide) == CharacterStyle::None);
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::ClipPartialLines::get()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CharacterStyle BalloonPreview::Style::get()
{
	CharacterStyle lRet = CharacterStyle::None;
	
	Monitor::Enter (this);
	try 
	{
		lRet = mStyle;
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

void BalloonPreview::Style::set (CharacterStyle pStyle)
{
	Monitor::Enter (this);
	try 
	{
		CharacterStyle	lPrevStyle = mStyle;

		mStyle = pStyle;

		if	(
				((mStyle & CharacterStyle::Tts) != (lPrevStyle & CharacterStyle::Tts))
			||	(!mShape)
			)
		{
			if	((mStyle & CharacterStyle::Tts) != CharacterStyle::None)
			{
				mShape = gcnew CAgentBalloonSpeak();
			}
			else
			{
				mShape = gcnew CAgentBalloonThink();
			}
			CalcShapeRect ();
		}
		else
		if	((mStyle & CharacterStyle::SizeToText) != (lPrevStyle & CharacterStyle::SizeToText))
		{
			if	((mStyle & CharacterStyle::SizeToText) != CharacterStyle::None)
			{
				Text = NextText;
			}
			CalcShapeRect ();
		}

		if	((mStyle & CharacterStyle::NoAutoPace) != (lPrevStyle & CharacterStyle::NoAutoPace))
		{
			if (IsAutoPace)
			{
				mTextDraw->DisplayFirstWord ();
			}
			else
			{
				mTextDraw->DisplayAllWords ();
			}
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
}

FileBalloon^ BalloonPreview::Balloon::get()
{
	FileBalloon^ lRet = nullptr;
	
	Monitor::Enter (this);
	try 
	{
		lRet = mBalloon;
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

void BalloonPreview::Balloon::set (FileBalloon^ pBalloon)
{
	Monitor::Enter (this);
	try 
	{
		mBalloon = pBalloon;
		CalcShapeRect ();
	}
    finally
    {
 		Monitor::Exit (this);
    }
}

/////////////////////////////////////////////////////////////////////////////

System::String^ BalloonPreview::Text::get ()
{
	String^	lRet = String::Empty;
	
	Monitor::Enter (this);
	try 
	{
		lRet = mText;
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

void BalloonPreview::Text::set (System::String^ pText)
{
	Monitor::Enter (this);
	try 
	{
		mText = gcnew String (pText);

		if	(mTextDraw)
		{
			delete mTextDraw;
			mTextDraw = nullptr;
		}
		if	(!String::IsNullOrEmpty (mText))
		{
			String^				lText = gcnew String (mText);
			CAgentTextParse^	lTextParse = gcnew CAgentTextParse (lText);

			mTextDraw = gcnew CAgentTextDraw (lTextParse);
#if	(__CLR_VER >= 40000000)
			GetTextDrawWPF (true);
#else
			GetTextDrawForms (true);
#endif			
			if (IsAutoPace)
			{
				mTextDraw->DisplayFirstWord ();
			}
			else
			{
				mTextDraw->DisplayAllWords ();
			}
		}
		CalcShapeRect ();
	}
    finally
    {
 		Monitor::Exit (this);
    }
}

System::String^ BalloonPreview::NextText::get ()
{
	String^	lRet = String::Empty;

	Monitor::Enter (this);
	try 
	{
		lRet = GetNextText ();
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

void BalloonPreview::NextText::set (System::String^ pText)
{
	Monitor::Enter (this);
	try 
	{
		mText = gcnew String (pText);

		if	(!mTextDraw)
		{
			Text = NextText;
		}
		else
		if	(!String::IsNullOrEmpty (pText))
		{
			String^				lText = gcnew String (mText);
			CAgentTextParse^	lTextParse = gcnew CAgentTextParse (lText);

			if (IsAutoSize)
			{
				(*mTextDraw) = lTextParse;
				if (IsAutoPace)
				{
					mTextDraw->DisplayFirstWord ();
				}
				else
				{
					mTextDraw->DisplayAllWords ();
				}
			}
			else
			{
				(*mTextDraw) += lTextParse;
				if (!IsAutoPace)
				{
					mTextDraw->DisplayAllWords ();
				}
			}
			CalcShapeRect ();
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
}

System::String^ BalloonPreview::DefaultText::get ()
{
	return "The quick red fox jumps over the lazy brown dog.";
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::AutoRepeat::get()
{
	Boolean lRet = false;

	Monitor::Enter (this);
	try 
	{
		lRet = mAutoRepeat;
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

void BalloonPreview::AutoRepeat::set (System::Boolean pAutoRepeat)
{
	Monitor::Enter (this);
	try 
	{
		mAutoRepeat = pAutoRepeat;
	}
    finally
    {
 		Monitor::Exit (this);
    }
}

System::Int32 BalloonPreview::AutoPaceTime::get()
{
	Int32 lRet = 0;

	Monitor::Enter (this);
	try 
	{
		if	(
				(IsEnabled)
			&&	(IsAutoPace)
			&&	(mTextDraw)
			&&	(
					(mTextDraw->CanPace)
				||	(AutoRepeat)
				)
			)
		{
			lRet = DefaultAutoPaceTime;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Int32 BalloonPreview::DefaultAutoPaceTime::get()
{
	return CAgentTextDraw::DefaultAutoPaceTime;
}

System::Int32 BalloonPreview::AutoScrollTime::get()
{
	Int32 lRet = 0;

	Monitor::Enter (this);
	try 
	{
		if	(
				(IsEnabled)
			&&	(!IsAutoSize)
			&&	(mShape)
			&&	(mTextDraw)
			)
		{
			lRet = (Int32)mTextDraw->ScrollTime;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::AutoPaceStarted ()
{
	return false;
}

System::Boolean BalloonPreview::AutoPaceStopped ()
{
	Boolean	lRet = false;

	Monitor::Enter (this);
	try 
	{
		if	(
				(!IsAutoPace)
			&&	(mTextDraw)
			&&	(mTextDraw->DisplayAllWords())
			)
		{
			lRet = true;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::OnAutoPace (System::Boolean% pRefresh)
{
	Boolean	lRet = false;

	pRefresh = false;

	Monitor::Enter (this);
	try 
	{
		if	(
				(mShape)
			&&	(mTextDraw)
			)
		{
			if	(mTextDraw->DisplayNextWord())
			{
				pRefresh = true;
				lRet = true;
			}
			else
			if	(
					(IsEnabled)
				&&	(AutoRepeat)
				)
			{
				if	(
						(IsAutoSize)
					||	(mTextDraw->ScrollMax >= mShape->mTextRect.Height*2)
					)
				{
					if	(++mAutoRepeatDelay > 10)
					{
						mAutoRepeatDelay = 0;
						Text = NextText;
						pRefresh = true;
					}
				}
				else
				if	(
						(
							(mTextDraw->ScrollMax > 0)
						&&	(!mTextDraw->CanScroll)
						)
					||	(++mAutoRepeatDelay > 10)
					)
				{
					mAutoRepeatDelay = 0;
					NextText = NextText;
					pRefresh = true;
				}
				lRet = true;
			}
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::AutoScrollStarted ()
{
	return false;
}

System::Boolean BalloonPreview::AutoScrollStopped ()
{
	return false;
}

System::Boolean BalloonPreview::OnAutoScroll (System::Boolean% pRefresh)
{
	Boolean	lRet = false;
	
	pRefresh = false;

	Monitor::Enter (this);
	try 
	{
		CAgentTextDrawForms^	lTextDrawForms;
		CAgentTextDrawWPF^		lTextDrawWPF;
		
		if	(
				(mShape)
			&&	(lTextDrawForms = GetTextDrawForms (false))
			&&	(lTextDrawForms->CanScroll)
			)
		{
			if	(lTextDrawForms->Scroll ())
			{
				pRefresh = true;
			}
			lRet = true;
		}
		else
		if	(
				(mShape)
			&&	(lTextDrawWPF = GetTextDrawWPF (false))
			&&	(lTextDrawWPF->CanScroll)
			)
		{
			if	(lTextDrawWPF->Scroll ())
			{
				pRefresh = true;
			}
			lRet = true;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::Draw (System::Drawing::Graphics^ pGraphics)
{
	Boolean	lRet = false;

	Monitor::Enter (this);
	try 
	{
		if	(
				(mBalloon)
			&&	(mShape)
			)
		{
			try
			{
				CAgentTextDrawForms^	lTextDrawForms;

				mShape->Draw (pGraphics, mBalloon->BkColor, mBalloon->BrColor);

				if	(lTextDrawForms = GetTextDrawForms (true))
				{
					try
					{
						RectangleF	lTextBounds;

						InitTextLayout (pGraphics);
						lTextBounds = lTextDrawForms->TextBounds;

						if	(
								(!IsAutoSize)
							&&	(lTextDrawForms->CanScroll)
							)
						{
							lTextDrawForms->TextBounds = lTextDrawForms->TextWrap->GetUsedRect (false, lTextDrawForms->DisplayText);
							lTextDrawForms->TextBounds = RectangleF (lTextBounds.X, lTextDrawForms->TextBounds.Y, lTextBounds.Width, lTextDrawForms->TextBounds.Height);
							lTextDrawForms->InitScroll (mShape->mTextRect, !IsAutoPace, DefaultAutoPaceTime);
							lTextDrawForms->ApplyScroll ();
						}

						if	(
								(IsAutoPace)
							&&	(lTextDrawForms->WordDisplayed < mTextDraw->WordCount)
							)
						{
							lTextDrawForms->TextWrap->DrawText (pGraphics, lTextDrawForms->TextBounds, lTextDrawForms->DisplayText, mBalloon->FgColor, mBalloon->Font, mShape->mTextRect);
						}
						else
						{
							lTextDrawForms->TextWrap->DrawText (pGraphics, mBalloon->FgColor, mBalloon->Font, mShape->mTextRect);
						}

						lTextDrawForms->TextBounds = lTextBounds;
					}
					catch AnyExceptionDebug
				}
				lRet = true;
			}
			catch AnyExceptionDebug
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::InitTextLayout (System::Drawing::Graphics^ pGraphics)
{
	System::Boolean			lRet = false;
	CAgentTextDrawForms^	lTextDrawForms;
	
	if	(
			(mBalloon)
		&&	(lTextDrawForms = GetTextDrawForms (true))
		)
	{
		try
		{
			lTextDrawForms->TextWrap->mClipPartialLines = ClipPartialLines;
			
			if	(lTextDrawForms->TextBounds.IsEmpty)
			{
				lTextDrawForms->TextBounds = mShape->mTextRect;
				lTextDrawForms->CalcTextSize (mBalloon->Font, pGraphics);
				lRet = true;
			}
			if	(
					(!IsAutoSize)
				&&	(
						(lRet)
					||	(lTextDrawForms->ScrollBounds.IsEmpty)
					)
				&&	(lTextDrawForms->InitScroll (mShape->mTextRect, !IsAutoPace, DefaultAutoPaceTime))
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::Drawing^ BalloonPreview::MakeDrawing ()
{
	System::Windows::Media::Drawing^	lRet = nullptr;

	Monitor::Enter (this);
	try 
	{
		if	(
				(mBalloon)
			&&	(mShape)
			)
		{
			System::Windows::Media::DrawingGroup^	lDrawingGroup = gcnew System::Windows::Media::DrawingGroup;
			CAgentTextDrawWPF^						lTextDrawWPF;

			lDrawingGroup->Children->Add (mShape->MakeDrawing (mBalloon->BkColor, mBalloon->BrColor));

			if	(lTextDrawWPF = GetTextDrawWPF (true))
			{
				try
				{
					System::Windows::Rect			lTextBounds;
					System::Windows::Rect			lTextRect (mShape->mTextRect.X, mShape->mTextRect.Y, mShape->mTextRect.Width, mShape->mTextRect.Height);
					System::Windows::Media::Brush^	lBrush = gcnew System::Windows::Media::SolidColorBrush (System::Windows::Media::Color::FromArgb (mBalloon->FgColor.A, mBalloon->FgColor.R, mBalloon->FgColor.G, mBalloon->FgColor.B));
					TextRunProperties^				lFontProperties = gcnew FontProperties (mBalloon->Font, lBrush);

					InitTextLayout ();
					lTextBounds = lTextDrawWPF->TextBounds;

					if	(
							(!IsAutoSize)
						&&	(lTextDrawWPF->CanScroll)
						)
					{
						lTextDrawWPF->CalcUsedHeight (lFontProperties);
						lTextDrawWPF->InitScroll (lTextRect, !IsAutoPace, DefaultAutoPaceTime);
						lTextDrawWPF->ApplyScroll ();
					}

					lTextDrawWPF->Draw (lDrawingGroup, lFontProperties, lTextRect, ClipPartialLines);
					lTextDrawWPF->TextBounds = lTextBounds;
				}
				catch AnyExceptionDebug
			}

			lRet = lDrawingGroup;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::InitTextLayout ()
{
	System::Boolean		lRet = false;
	CAgentTextDrawWPF^	lTextDrawWPF;
	
	if	(
			(mBalloon)
		&&	(lTextDrawWPF = GetTextDrawWPF (true))
		)
	{
		try
		{
			System::Windows::Rect	lTextRect (mShape->mTextRect.X, mShape->mTextRect.Y, mShape->mTextRect.Width, mShape->mTextRect.Height);

			if	(lTextDrawWPF->TextBounds.IsEmpty)
			{
				lTextDrawWPF->TextBounds = lTextRect;
				lTextDrawWPF->CalcTextSize (mBalloon->Font);
				lRet = true;
			}
			if	(
					(!IsAutoSize)
				&&	(
						(lRet)
					||	(lTextDrawWPF->ScrollBounds.IsEmpty)
					)
				&&	(lTextDrawWPF->InitScroll (lTextRect, !IsAutoPace, DefaultAutoPaceTime))
				)
			{
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}
 
/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::Visual^ BalloonPreview::MakeVisual ()
{
	System::Windows::Media::Visual^	lRet = nullptr;

	Monitor::Enter (this);
	try 
	{
		System::Windows::Media::Drawing^		lDrawing;
		System::Windows::Media::DrawingVisual^	lDrawingVisual;
		DrawingContext^							lDrawingContext;
#if	(__CLR_VER >= 40000000)
		DropShadowEffect^						lDropShadow;
#endif		
		
		if	(
				(lDrawing = MakeDrawing ())
			&&	(lDrawingVisual = gcnew DrawingVisual ())
			&&	(lDrawingContext = lDrawingVisual->RenderOpen ())
			)
		{
			lDrawingContext->DrawDrawing (lDrawing);
			lDrawingContext->Close ();

#if	(__CLR_VER >= 40000000)
			if	(lDropShadow = gcnew DropShadowEffect ())
			{
				lDropShadow->Opacity = 0.25;
				lDrawingVisual->Effect = lDropShadow;
			}
#endif
			lRet = lDrawingVisual;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}
 
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Drawing::RectangleF BalloonPreview::CalcShapeRect ()
{
	if	(
			(mBalloon)
		&&	(mShape)
		)
	{
		System::Drawing::SizeF		lTextSize = GetTextSize();
		System::Drawing::RectangleF	lTextRect (0, 0, lTextSize.Width, lTextSize.Height);
		System::Drawing::RectangleF	lOwnerRect (0, lTextRect.Bottom+mShape->LayoutMargin.Height*2, lTextSize.Width, mShape->LayoutMargin.Height*4);
		System::Drawing::RectangleF	lBounds (0, 0, Single::MaxValue, Single::MaxValue);
		System::Drawing::RectangleF	lShapeRect;
		CAgentTextDrawForms^		lTextDrawForms;
		CAgentTextDrawWPF^			lTextDrawWPF;

		lShapeRect = mShape->RecalcLayout (lTextRect, lOwnerRect, lBounds);
		if	(lTextDrawForms = GetTextDrawForms (false))
		{
			lTextDrawForms->TextBounds = System::Drawing::RectangleF (mShape->mTextRect.Location, lTextDrawForms->TextBounds.Size);
		}
		if	(lTextDrawWPF = GetTextDrawWPF (false))
		{
			lTextDrawWPF->TextBounds = System::Windows::Rect (mShape->mTextRect.X, mShape->mTextRect.Y, lTextDrawWPF->TextBounds.Width, lTextDrawWPF->TextBounds.Height);
		}
		return System::Drawing::RectangleF (lShapeRect.X, lShapeRect.Y, lShapeRect.Width, lShapeRect.Height);
	}
	return System::Drawing::RectangleF (0,0,0,0);
}

System::Drawing::SizeF BalloonPreview::GetTextSize ()
{
	CAgentTextDrawForms^	lTextDrawForms;
	CAgentTextDrawWPF^		lTextDrawWPF;

	if	(
			(mBalloon)
#if	(__CLR_VER >= 40000000)
		&&	(lTextDrawForms = GetTextDrawForms (false))
#else
		&&	(lTextDrawForms = GetTextDrawForms (true))
#endif		
		)
	{
		System::Drawing::SizeF	lTextSize;
		
		if	(
				(IsAutoSize)
			||	(Balloon->Lines < FileBalloon::MinLines)
			)
		{
			lTextSize = lTextDrawForms->CalcTextSize (mBalloon->Font, Math::Min(Math::Max(mBalloon->PerLine,FileBalloon::MinPerLine),FileBalloon::MaxPerLine));
		}
		else
		{
			lTextSize = lTextDrawForms->CalcTextSize (mBalloon->Font, Math::Min(Math::Max(mBalloon->PerLine,FileBalloon::MinPerLine),FileBalloon::MaxPerLine), Math::Min(Math::Max(mBalloon->Lines,CAgentFileBalloon::MinLines),CAgentFileBalloon::MaxLines));
		}
		lTextDrawForms->TextBounds = System::Drawing::RectangleF (System::Drawing::PointF (0,0), lTextSize);
		lTextDrawForms->CalcTextSize (mBalloon->Font);

		return lTextSize;
	}
	else
	if	(
			(mBalloon)
#if	(__CLR_VER >= 40000000)
		&&	(lTextDrawWPF = GetTextDrawWPF (true))
#else
		&&	(lTextDrawWPF = GetTextDrawWPF (false))
#endif		
		)
	{
		System::Windows::Size	lTextSize;
		
		if	(
				(IsAutoSize)
			||	(Balloon->Lines < FileBalloon::MinLines)
			)
		{
			lTextSize = lTextDrawWPF->CalcTextSize (mBalloon->Font, Math::Min(Math::Max(mBalloon->PerLine,FileBalloon::MinPerLine),FileBalloon::MaxPerLine));
		}
		else
		{
			lTextSize = lTextDrawWPF->CalcTextSize (mBalloon->Font, Math::Min(Math::Max(mBalloon->PerLine,FileBalloon::MinPerLine),FileBalloon::MaxPerLine), Math::Min(Math::Max(mBalloon->Lines,CAgentFileBalloon::MinLines),CAgentFileBalloon::MaxLines));
		}
		lTextDrawWPF->TextBounds = System::Windows::Rect (System::Windows::Point (0,0), lTextSize);
		lTextDrawWPF->CalcTextSize (mBalloon->Font);

		return System::Drawing::SizeF ((Single)lTextSize.Width, (Single)lTextSize.Height);
	}
	return System::Drawing::SizeF (32,16);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCSTR	sAdjectives [] =	{
									"athletic","artistic","awesome","bold","beautiful","bashful",
									"cautious","cute","cuddly","delightful","delicious","delectable","dangerous",
									"edgy","ecstatic","frightened","frivolous","fabulous","famous",
									"gorgeous","grumpy","grateful","hilarious","happy","hopeful",
									"ingenious","idiotic","jealous","kind","lumpy","ludicrous",
									"motherly","nervous","obese","pretty","quick","rotund","rueful",
									"satisified","silly","spontaneous","tired","ugly",
									"voracious","voluptuous","wary","wonderful","wild","zealous"
									};
static LPCSTR	sColors [] =		{
									"blue ","beige","emerald","fuchsia","green","gold","indigo",
									"mauve","marroon","orange","pink","purple","red","ruby",
									"sapphire","tangerine","violet","yellow"
									};
static LPCSTR	sNouns [] =			{
									"albatross","antelope","banshee","bandersnatch","brontosaurus","bear",
									"cat","cougar","cow","chicken","dog","dingo","dinosaur","dolphin","deer","duck",
									"elephant","elk","frog","fox","giraffe","groundhog","goose","jaguar","kitten",
									"leopard","lion","meerkat","moose","mud lark","orang-utan","pig","porcupine",
									"platypus","quark","rat","rabbit","snake","shrew","tyrannosaur","tiger",
									"warthog","wildebeest","wolf","yak","zebra"
									};
static LPCSTR	sVerbs [] =			{
									"bangs:into","bounces","careens:into",
									"dives","drives","dances:with","flies","gambles:with","hops",
									"jumps","lunges:at","navigates","pounces:on","pole-vaults",
									"runs","races","skydives","staggers","tumbles","trips",
									"sits:on","walks"
									};
static LPCSTR	sPrepositions [] =	{
									"above","around","beneath","over","under","through","behind","in front of"
									};

/////////////////////////////////////////////////////////////////////////////

System::String^ BalloonPreview::GetNextText ()
{
	srand (GetTickCount());

#ifdef	_DEBUG
	if	((rand() % 5) == 0)
#else
	if	((rand() % 30) == 0)
#endif
	{
		array<String^>^	lAdjective = {gcnew String (sAdjectives [rand() % (sizeof(sAdjectives)/sizeof(LPCSTR))]), gcnew String (sAdjectives [rand() % (sizeof(sAdjectives)/sizeof(LPCSTR))])};
		array<String^>^	lColor = {gcnew String (sColors [rand() % (sizeof(sColors)/sizeof(LPCSTR))]), gcnew String (sColors [rand() % (sizeof(sColors)/sizeof(LPCSTR))])};
		array<String^>^	lNoun = {gcnew String (sNouns [rand() % (sizeof(sNouns)/sizeof(LPCSTR))]), gcnew String (sNouns [rand() % (sizeof(sNouns)/sizeof(LPCSTR))])};
		String^			lVerb = gcnew String (sVerbs [rand() % (sizeof(sVerbs)/sizeof(LPCSTR))]);
		String^			lPreposition = gcnew String (sPrepositions [rand() % (sizeof(sPrepositions)/sizeof(LPCSTR))]);

		while (lAdjective[1] == lAdjective[0])
		{
			lAdjective[1] = gcnew String (sAdjectives [rand() % (sizeof(sAdjectives)/sizeof(LPCSTR))]);
		}
		while (lColor[1] == lColor[0])
		{
			lColor[1] = gcnew String (sColors [rand() % (sizeof(sColors)/sizeof(LPCSTR))]);
		}
		while (lNoun[1] == lNoun[0])
		{
			lNoun[1] = gcnew String (sNouns [rand() % (sizeof(sNouns)/sizeof(LPCSTR))]);
		}
		if	(lVerb->Contains (":"))
		{
			lPreposition = lVerb->Substring (lVerb->IndexOf (':')+1);
			lVerb = lVerb->Substring (0, lVerb->IndexOf (':'));
		}
		return String::Format ("The {0} {1} {2} {3} {4} the {5} {6} {7}.", lAdjective[0], lColor[0], lNoun[0], lVerb, lPreposition, lAdjective[1], lColor[1], lNoun[1]);
	}
	return DefaultText;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentTextDrawForms^ BalloonPreview::GetTextDrawForms (System::Boolean pCreate)
{
	CAgentTextDrawForms^	lTextDrawForms = CAgentTextDrawForms::Downcast (mTextDraw);
	
	if	(
			(!lTextDrawForms)
		&&	(pCreate)
		&&	(mTextDraw)
		)
	{
		mTextDraw = lTextDrawForms = gcnew CAgentTextDrawForms (mTextDraw);
	}
	return lTextDrawForms;
}

CAgentTextDrawWPF^ BalloonPreview::GetTextDrawWPF (System::Boolean pCreate)
{
	CAgentTextDrawWPF^	lTextDrawWPF = CAgentTextDrawWPF::Downcast (mTextDraw);
	
	if	(
			(!lTextDrawWPF)
		&&	(pCreate)
		&&	(mTextDraw)
		)
	{
		mTextDraw = lTextDrawWPF = gcnew CAgentTextDrawWPF (mTextDraw);
	}
	return lTextDrawWPF;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
