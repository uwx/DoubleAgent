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
#pragma managed(push,off)
#include "AgentBalloonShape.h"
#include "AgentText.h"
#include "AgentTextParse.h"
#include "DebugStr.h"
#pragma managed(pop)

using namespace System;
using namespace System::Drawing;

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

BalloonPreview::BalloonPreview ()
:	mStyle (CharacterStyle::None),
	mAutoRepeat (true),
	mAutoRepeatDelay (0),
	mAutoScrollTime (0),
	mShape (NULL),
	mTextDraw (NULL)
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
	if	(!Balloon)
	{
		return false;
	}
	return ((Style & CharacterStyle::Balloon) != CharacterStyle::None);
}

System::Boolean BalloonPreview::IsAutoSize::get()
{
	if	(
			(Balloon)
		&&	(Balloon->Lines < FileBalloon::MinLines)
		)
	{
		return true;
	}
	return ((Style & CharacterStyle::SizeToText) != CharacterStyle::None);
}

System::Boolean BalloonPreview::IsAutoPace::get()
{
	return ((Style & CharacterStyle::NoAutoPace) == CharacterStyle::None);
}

System::Boolean BalloonPreview::IsAutoHide::get()
{
	return ((Style & CharacterStyle::NoAutoHide) == CharacterStyle::None);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CharacterStyle BalloonPreview::Style::get()
{
	return mStyle;
}

void BalloonPreview::Style::set (CharacterStyle pStyle)
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
			mShape = new CAgentBalloonSpeak();
		}
		else
		{
			mShape = new CAgentBalloonThink();
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

FileBalloon^ BalloonPreview::Balloon::get()
{
	return mBalloon;
}

void BalloonPreview::Balloon::set (FileBalloon^ pBalloon)
{
	mBalloon = pBalloon;
	CalcShapeRect ();
}

/////////////////////////////////////////////////////////////////////////////

System::String^ BalloonPreview::Text::get ()
{
	return mText;
}

void BalloonPreview::Text::set (System::String^ pText)
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

System::String^ BalloonPreview::NextText::get ()
{
	return GetNextText ();
}

void BalloonPreview::NextText::set (System::String^ pText)
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

System::String^ BalloonPreview::DefaultText::get ()
{
	return "The quick red fox jumps over the lazy brown dog.";
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::AutoRepeat::get()
{
	return mAutoRepeat;
}

void BalloonPreview::AutoRepeat::set (System::Boolean pAutoRepeat)
{
	mAutoRepeat = pAutoRepeat;
}

System::Int32 BalloonPreview::AutoPaceTime::get()
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
		return DefaultAutoPaceTime;
	}
	return 0;
}

System::Int32 BalloonPreview::DefaultAutoPaceTime::get()
{
	return CAgentTextDraw::mDefaultAutoPaceTime;
}

System::Int32 BalloonPreview::AutoScrollTime::get()
{
	if	(
			(IsEnabled)
		&&	(!IsAutoSize)
		&&	(mShape)
		&&	(mTextDraw)
		)
	{
		return mAutoScrollTime;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::AutoPaceStarted ()
{
	return false;
}

System::Boolean BalloonPreview::AutoPaceStopped ()
{
	if	(
			(!IsAutoPace)
		&&	(mTextDraw)
		&&	(mTextDraw->DisplayAllWords())
		)
	{
		return true;
	}
	return false;
}

System::Boolean BalloonPreview::OnAutoPace (System::Boolean% pRefresh)
{
	pRefresh = false;

	if	(
			(mShape)
		&&	(mTextDraw)
		)
	{
		if	(mTextDraw->DisplayNextWord())
		{
			pRefresh = true;
			return true;
		}
		else
		if	(
				(IsEnabled)
			&&	(AutoRepeat)
			)
		{
			if	(
					(IsAutoSize)
				||	(mTextDraw->GetScrollMax() >= mShape->mTextRect.Height()*2)
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
					&&	(!mTextDraw->CanScroll (mShape->mTextRect))
					)
				||	(++mAutoRepeatDelay > 10)
				)
			{
				mAutoRepeatDelay = 0;
				NextText = NextText;
				pRefresh = true;
			}
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::AutoScrollStarted ()
{
	return false;
}

System::Boolean BalloonPreview::AutoScrollStopped ()
{
	if	(mAutoScrollTime > 0)
	{
		mAutoScrollTime = 0;
		return true;
	}
	return false;
}

System::Boolean BalloonPreview::OnAutoScroll (System::Boolean% pRefresh)
{
	pRefresh = false;

	if	(
			(mShape)
		&&	(mTextDraw)
		&&	(mTextDraw->CanScroll (mShape->mTextRect))
		)
	{
		if	(mTextDraw->Scroll ())
		{
			pRefresh = true;
		}
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Boolean BalloonPreview::Draw (System::Drawing::Graphics^ pGraphics)
{
	Boolean	lRet = false;

	if	(
			(mBalloon)
		&&	(mShape)
		)
	{
		Gdiplus::Graphics	lGraphics ((HDC)(INT_PTR)pGraphics->GetHdc());
		Gdiplus::Color		lBkColor ((Gdiplus::ARGB) mBalloon->BkColor.ToArgb());
		Gdiplus::Color		lBrColor ((Gdiplus::ARGB) mBalloon->BrColor.ToArgb());

		try
		{
			mShape->Draw (lGraphics, lBkColor, lBrColor);

			if	(mTextDraw)
			{
				HDC	lDC = lGraphics.GetHDC();

				try
				{
					HFONT		lFont = (mBalloon->Font) ? (HFONT)(INT_PTR)mBalloon->Font->ToHfont() : (HFONT)::GetStockObject (DEFAULT_GUI_FONT);
					COLORREF	lTextColor = mBalloon->FgColor.ToArgb();
					CRect		lTextBounds;
					CRect		lClipRect = mShape->mTextRect;
					bool		lClipPartialLines = true;

					::SetBkMode (lDC, TRANSPARENT);
					::SetTextColor (lDC, RGB (GetBValue(lTextColor), GetGValue(lTextColor), GetRValue(lTextColor)));

					if	(mTextDraw->mBounds.IsRectEmpty ())
					{
						mTextDraw->mBounds = mShape->mTextRect;
						mTextDraw->MeasureText (mTextDraw->GetFullText(), lDC, lFont);
					}
					lTextBounds = mTextDraw->mBounds;

					if	(
							(!IsAutoSize)
						&&	(mTextDraw->CanScroll (mShape->mTextRect))
						)
					{
						mTextDraw->mBounds = mTextDraw->GetUsedRect (false, mTextDraw->GetDisplayText ());
						mTextDraw->mBounds.left = lTextBounds.left;
						mTextDraw->mBounds.right = lTextBounds.right;
						mAutoScrollTime = mTextDraw->InitScroll (mShape->mTextRect, (mAutoScrollTime<=0), lClipPartialLines, DefaultAutoPaceTime);
						if	(lClipPartialLines)
						{
							mTextDraw->ApplyScroll (mShape->mTextRect, &lClipRect);
						}
						else
						{
							mTextDraw->ApplyScroll (mShape->mTextRect);
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

				lGraphics.ReleaseHDC (lDC);
			}
			lRet = true;
		}
		catch AnyExceptionDebug

		pGraphics->ReleaseHdc ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::Rectangle BalloonPreview::CalcShapeRect ()
{
	if	(
			(mBalloon)
		&&	(mShape)
		)
	{
		System::Drawing::Size	lTextSize = GetTextSize();
		CRect					lTextRect (CPoint (0, 0), CSize (lTextSize.Width, lTextSize.Height));
		CRect					lOwnerRect (CPoint (0, lTextSize.Height), CSize (lTextSize.Width, lTextSize.Height+40));
		CRect					lBounds (0, 0, SHRT_MAX, SHRT_MAX);
		CRect					lShapeRect;

		lShapeRect = mShape->RecalcLayout (lTextRect, lOwnerRect, lBounds);
		if	(mTextDraw)
		{
			mTextDraw->mBounds.OffsetRect (mShape->mTextRect.left - mTextDraw->mBounds.left, mShape->mTextRect.top - mTextDraw->mBounds.top);
		}

		return System::Drawing::Rectangle (0, 0, lShapeRect.Width(), lShapeRect.Height());
	}
	return System::Drawing::Rectangle (0,0,0,0);
}

System::Drawing::Size BalloonPreview::GetTextSize ()
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

		return System::Drawing::Size (lTextSize.cx, lTextSize.cy);
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
