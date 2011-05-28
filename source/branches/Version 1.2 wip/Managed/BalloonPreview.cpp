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
#pragma managed(push,off)
#include "AgentText.h"
#include "AgentTextParse.h"
#include "DebugStr.h"
#pragma managed(pop)

using namespace System;
using namespace System::Drawing;
using namespace System::Threading;

/////////////////////////////////////////////////////////////////////////////

static CRect RectangleFtoCRect (RectangleF pRect)
{
	return CRect ((long)Math::Floor(pRect.Left), (long)Math::Floor(pRect.Top), (long)Math::Ceiling(pRect.Right), (long)Math::Ceiling(pRect.Bottom));
}

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

BalloonPreview::BalloonPreview ()
:	mStyle (CharacterStyle::None),
	mAutoRepeat (true),
	mAutoRepeatDelay (0),
	mAutoScrollTime (0),
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
			mTextDraw = NULL;
		}
		if	(!String::IsNullOrEmpty (mText))
		{
			CAtlString		lText = CAtlString (mText);
			CAgentTextParse	lTextParse (lText);

			mTextDraw = new CAgentTextDraw (lTextParse);
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
			CAtlString		lText = CAtlString (mText);
			CAgentTextParse	lTextParse (lText);

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
					(mTextDraw->CanPace())
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
	return CAgentTextDraw::mDefaultAutoPaceTime;
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
			lRet = mAutoScrollTime;
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
					||	(mTextDraw->GetScrollMax() >= mShape->mTextRect.Height*2)
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
							(mTextDraw->GetScrollMax() > 0)
						&&	(!mTextDraw->CanScroll (RectangleFtoCRect (mShape->mTextRect)))
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
	Boolean	lRet = false;

	Monitor::Enter (this);
	try 
	{
		if	(mAutoScrollTime > 0)
		{
			mAutoScrollTime = 0;
			lRet = true;
		}
	}
    finally
    {
 		Monitor::Exit (this);
    }
	return lRet;
}

System::Boolean BalloonPreview::OnAutoScroll (System::Boolean% pRefresh)
{
	Boolean	lRet = false;
	
	pRefresh = false;

	Monitor::Enter (this);
	try 
	{
		if	(
				(mShape)
			&&	(mTextDraw)
			&&	(mTextDraw->CanScroll (RectangleFtoCRect (mShape->mTextRect)))
			)
		{
			if	(mTextDraw->Scroll ())
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
				mShape->Draw (pGraphics, mBalloon->BkColor, mBalloon->BrColor);

				if	(mTextDraw)
				{
					HDC	lDC = (HDC)(INT_PTR)pGraphics->GetHdc();

					try
					{
						HFONT		lFont = (mBalloon->Font) ? (HFONT)(INT_PTR)mBalloon->Font->ToHfont() : (HFONT)::GetStockObject (DEFAULT_GUI_FONT);
						COLORREF	lTextColor = mBalloon->FgColor.ToArgb();
						CRect		lTextBounds;
						CRect		lClipRect = RectangleFtoCRect (mShape->mTextRect);
						bool		lClipPartialLines = true;

						::SetBkMode (lDC, TRANSPARENT);
						::SetTextColor (lDC, RGB (GetBValue(lTextColor), GetGValue(lTextColor), GetRValue(lTextColor)));

						if	(mTextDraw->mBounds.IsRectEmpty ())
						{
							mTextDraw->mBounds = RectangleFtoCRect (mShape->mTextRect);
							mTextDraw->MeasureText (mTextDraw->GetFullText(), lDC, lFont);
						}
						lTextBounds = mTextDraw->mBounds;

						if	(
								(!IsAutoSize)
							&&	(mTextDraw->CanScroll (RectangleFtoCRect (mShape->mTextRect)))
							)
						{
							mTextDraw->mBounds = mTextDraw->GetUsedRect (false, mTextDraw->GetDisplayText ());
							mTextDraw->mBounds.left = lTextBounds.left;
							mTextDraw->mBounds.right = lTextBounds.right;
							mAutoScrollTime = mTextDraw->InitScroll (RectangleFtoCRect (mShape->mTextRect), (mAutoScrollTime<=0), lClipPartialLines, DefaultAutoPaceTime);
							if	(lClipPartialLines)
							{
								mTextDraw->ApplyScroll (RectangleFtoCRect (mShape->mTextRect), &lClipRect);
							}
							else
							{
								mTextDraw->ApplyScroll (RectangleFtoCRect (mShape->mTextRect));
							}
						}

						if	(
								(IsAutoPace)
							&&	(mTextDraw->GetWordDisplayed() < mTextDraw->GetWordCount())
							)
						{
							mTextDraw->DrawText (lDC, &mTextDraw->mBounds, mTextDraw->GetDisplayText(), lFont, &lClipRect);
						}
						else
						{
							mTextDraw->DrawText (lDC, lFont, &lClipRect);
						}
						mTextDraw->mBounds = lTextBounds;
					}
					catch AnyExceptionDebug

					pGraphics->ReleaseHdc ((IntPtr)lDC);
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#if	(__CLR_VER >= 40000000)
static void MakeDrawingText (System::Windows::Media::DrawingGroup^ pDrawing, CAgentTextDraw & pTextDraw, System::Drawing::Font^ pFont, System::Windows::Media::Brush^ pBrush, CRect & pClipRect)
{
	System::Windows::Media::DrawingContext^ lDrawingContext = pDrawing->Append ();

	System::Windows::Media::Typeface^	lFontFace = nullptr;
	Single								lFontSize = pFont->GetHeight (96.0);
	System::Windows::FontWeight			lFontWeight = pFont->Bold ? System::Windows::FontWeights::Bold : System::Windows::FontWeights::Normal;
	System::Windows::FontStyle			lFontStyle  = (((int)pFont->Style & (int)System::Drawing::FontStyle::Italic) != 0) ? System::Windows::FontStyles::Italic : System::Windows::FontStyles::Normal;
	System::Globalization::CultureInfo^	lTextCulture = System::Globalization::CultureInfo::CurrentUICulture;
	System::Windows::FlowDirection		lTextFlow = System::Windows::FlowDirection::LeftToRight;

	try
	{
		lFontFace = gcnew System::Windows::Media::Typeface (String::IsNullOrEmpty (pFont->OriginalFontName) ? pFont->Name : pFont->OriginalFontName);
		//LogMessage (LogNormal, _T("Font [%s] Style [%s] Weight [%s] Stretch [%s] Underline [%s %s] Strikethrough [%s %s]"), _B(lFontFace->FontFamily->ToString()), _B(lFontFace->Style.ToString()), _B(lFontFace->Weight.ToString()), _B(lFontFace->Stretch.ToString()), _B(lFontFace->UnderlinePosition.ToString()), _B(lFontFace->UnderlineThickness.ToString()), _B(lFontFace->StrikethroughPosition.ToString()), _B(lFontFace->StrikethroughThickness.ToString()));
	}
	catch AnyExceptionSilent

	try
	{
		int			lNdx;
		POLYTEXT *	lLine;
		CPoint		lLinePos;
		CRect		lLineRect;
		LPCTSTR		lLineText;
		int			lLineStart = 0;
		int			lLineLength;
		CAtlString	lText = pTextDraw.GetDisplayText();
		LPCTSTR		lTextEnd = (LPCTSTR)lText + lText.GetLength();

		//LogMessage (LogNormal, _T("Text [%s]"), DebugStr(lText));

		lDrawingContext->PushClip (gcnew System::Windows::Media::RectangleGeometry (System::Windows::Rect (pClipRect.left, pClipRect.top, pClipRect.Width(), pClipRect.Height())));

		for	(lNdx = 0; lLine = pTextDraw.GetLines() (lNdx); lNdx++)
		{
			lLineLength = lLine->n;

			lLineText = (LPCTSTR)lText + lLineStart;
			while	(
						(lLineText < lTextEnd)
					&&	(*lLineText == _T('\n'))
					)
			{
				lLineStart++;
				lLineText++;
			}
			if	(lLineText >= lTextEnd)
			{
				break;
			}
			lLineLength = min (lLineLength, lTextEnd - lLineText);

			lLinePos.x = pTextDraw.mBounds.left + lLine->x;
			lLinePos.y = pTextDraw.mBounds.top + lLine->rcl.top + lLine->y;
			lLineRect.SetRect (pTextDraw.mBounds.left, pTextDraw.mBounds.top + lLine->rcl.top, pTextDraw.mBounds.right, pTextDraw.mBounds.top + lLine->rcl.bottom);

			lDrawingContext->PushClip (gcnew System::Windows::Media::RectangleGeometry (System::Windows::Rect (lLineRect.left, lLineRect.top, lLineRect.Width(), lLineRect.Height())));
			
			System::Windows::Media::FormattedText^	lFormattedText = gcnew System::Windows::Media::FormattedText (gcnew String(lLineText, 0, lLineLength), lTextCulture, lTextFlow, lFontFace, lFontSize, pBrush);
			lDrawingContext->DrawText (lFormattedText, System::Windows::Point (lLinePos.x, lLinePos.y));
			//LogMessage (LogNormal, _T("Line [%s (%d %d)] at [%d %d]"), _B(lFormattedText->Text), lLineStart, lLineLength, lLinePos.x, lLinePos.y);

			lDrawingContext->Pop ();

			lLineStart += lLine->n;
		}

		lDrawingContext->Pop ();
	}
	catch AnyExceptionDebug

	lDrawingContext->Close ();
}

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

			lDrawingGroup->Children->Add (mShape->MakeDrawing (mBalloon->BkColor, mBalloon->BrColor));

			if	(mTextDraw)
			{
				try
				{
					System::Windows::Media::Brush^	lTextBrush = gcnew System::Windows::Media::SolidColorBrush (System::Windows::Media::Color::FromArgb (mBalloon->FgColor.A, mBalloon->FgColor.R, mBalloon->FgColor.G, mBalloon->FgColor.B));
					CRect							lTextBounds;
					CRect							lClipRect = RectangleFtoCRect (mShape->mTextRect);
					bool							lClipPartialLines = true;

					if	(mTextDraw->mBounds.IsRectEmpty ())
					{
						HDC		lDC = CreateCompatibleDC (0);
						HFONT	lFont = (mBalloon->Font) ? (HFONT)(INT_PTR)mBalloon->Font->ToHfont() : (HFONT)::GetStockObject (DEFAULT_GUI_FONT);

						mTextDraw->mBounds = RectangleFtoCRect (mShape->mTextRect);
						mTextDraw->MeasureText (mTextDraw->GetFullText(), lDC, lFont);

						DeleteDC (lDC);
					}
					lTextBounds = mTextDraw->mBounds;

					if	(
							(!IsAutoSize)
						&&	(mTextDraw->CanScroll (RectangleFtoCRect (mShape->mTextRect)))
						)
					{
						mTextDraw->mBounds = mTextDraw->GetUsedRect (false, mTextDraw->GetDisplayText ());
						mTextDraw->mBounds.left = lTextBounds.left;
						mTextDraw->mBounds.right = lTextBounds.right;
						mAutoScrollTime = mTextDraw->InitScroll (RectangleFtoCRect (mShape->mTextRect), (mAutoScrollTime<=0), lClipPartialLines, DefaultAutoPaceTime);
						if	(lClipPartialLines)
						{
							mTextDraw->ApplyScroll (RectangleFtoCRect (mShape->mTextRect), &lClipRect);
						}
						else
						{
							mTextDraw->ApplyScroll (RectangleFtoCRect (mShape->mTextRect));
						}
					}

					MakeDrawingText (lDrawingGroup, *mTextDraw, mBalloon->Font, lTextBrush, lClipRect);

					//if	(
					//		(IsAutoPace)
					//	&&	(mTextDraw->GetWordDisplayed() < mTextDraw->GetWordCount())
					//	)
					//{
					//	mTextDraw->DrawText (lDC, &mTextDraw->mBounds, mTextDraw->GetDisplayText(), lFont, &lClipRect);
					//}
					//else
					//{
					//	mTextDraw->DrawText (lDC, lFont, &lClipRect);
					//}
					mTextDraw->mBounds = lTextBounds;
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
#endif

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

		lShapeRect = mShape->RecalcLayout (lTextRect, lOwnerRect, lBounds);
		if	(mTextDraw)
		{
			mTextDraw->mBounds.OffsetRect (RectangleFtoCRect (mShape->mTextRect).left - mTextDraw->mBounds.left, RectangleFtoCRect (mShape->mTextRect).top - mTextDraw->mBounds.top);
		}
		return System::Drawing::RectangleF (lShapeRect.X, lShapeRect.Y, lShapeRect.Width, lShapeRect.Height);
	}
	return System::Drawing::RectangleF (0,0,0,0);
}

System::Drawing::SizeF BalloonPreview::GetTextSize ()
{
	if	(
			(mBalloon)
		&&	(mTextDraw)
		)
	{
		HFONT	lFont = (mBalloon->Font) ? (HFONT)(INT_PTR)mBalloon->Font->ToHfont() : (HFONT)::GetStockObject (DEFAULT_GUI_FONT);
		CSize	lTextSize;

		if	(
				(IsAutoSize)
			||	(Balloon->Lines < FileBalloon::MinLines)
			)
		{
			lTextSize = mTextDraw->CalcTextSize (lFont, Math::Min(Math::Max(mBalloon->PerLine,FileBalloon::MinPerLine),FileBalloon::MaxPerLine));
		}
		else
		{
			lTextSize = mTextDraw->CalcTextSize (lFont, Math::Min(Math::Max(mBalloon->PerLine,FileBalloon::MinPerLine),FileBalloon::MaxPerLine), Math::Min(Math::Max(mBalloon->Lines,CAgentFileBalloon::MinLines),CAgentFileBalloon::MaxLines));
		}
		mTextDraw->mBounds.SetRect (0, 0, lTextSize.cx, lTextSize.cy);
		mTextDraw->MeasureText (mTextDraw->GetFullText(), NULL, lFont);

		return System::Drawing::SizeF ((float)lTextSize.cx, (float)lTextSize.cy);
	}
	return System::Drawing::Size (32,16);
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
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
