/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "FontProperties.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows;
using namespace System::Windows::Media;
using namespace System::Windows::Media::TextFormatting;

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

FontProperties::FontProperties (System::Drawing::Font^ pFont)
:	mForegroundBrush (System::Windows::Media::Brushes::Black),
	mBackgroundBrush (System::Windows::Media::Brushes::Transparent)
{
	CommonConstruct (pFont);
}

FontProperties::FontProperties (System::Drawing::Font^ pFont, System::Windows::Media::Brush^ pForegroundBrush)
:	mForegroundBrush (pForegroundBrush),
	mBackgroundBrush (System::Windows::Media::Brushes::Transparent)
{
	CommonConstruct (pFont);
}

FontProperties::FontProperties (System::Drawing::Font^ pFont, System::Windows::Media::Brush^ pForegroundBrush, System::Windows::Media::Brush^ pBackgroundBrush)
:	mForegroundBrush (pForegroundBrush),
	mBackgroundBrush (pBackgroundBrush)
{
	CommonConstruct (pFont);
}

void FontProperties::CommonConstruct (System::Drawing::Font^ pFont)
{
	if	(pFont)
	{
		mHintingEmSize = pFont->SizeInPoints;
		mRenderingEmSize = pFont->GetHeight (96.0);
		mTypeface = nullptr;
		mDecorations = nullptr;

		try
		{
			System::Windows::FontStyle			lFontStyle  = (((int)pFont->Style & (int)System::Drawing::FontStyle::Italic) != 0) ? System::Windows::FontStyles::Italic : System::Windows::FontStyles::Normal;
			System::Windows::FontWeight			lFontWeight = pFont->Bold ? System::Windows::FontWeights::Bold : System::Windows::FontWeights::Normal;
			System::Windows::Media::Typeface^	lTypeface;

			if	(mTypeface = gcnew System::Windows::Media::Typeface (String::IsNullOrEmpty (pFont->OriginalFontName) ? pFont->Name : pFont->OriginalFontName))
			{
				if	(lTypeface = gcnew System::Windows::Media::Typeface (mTypeface->FontFamily, lFontStyle, lFontWeight, mTypeface->Stretch))
				{
					mTypeface = lTypeface;
				}
				if	(
						(
							(pFont->Underline)
						||	(pFont->Strikeout)
						)
					&&	(mDecorations = gcnew System::Windows::TextDecorationCollection)
					)
				{
					if	(pFont->Underline)
					{
						mDecorations->Add (gcnew System::Windows::TextDecoration (TextDecorationLocation::Underline, nullptr, 0, TextDecorationUnit::FontRecommended, TextDecorationUnit::FontRecommended));
					}
					if	(pFont->Strikeout)
					{
						mDecorations->Add (gcnew System::Windows::TextDecoration (TextDecorationLocation::Strikethrough, nullptr, 0, TextDecorationUnit::FontRecommended, TextDecorationUnit::FontRecommended));
					}
				}
			}
		}
		catch AnyExceptionSilent
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Double FontProperties::FontHintingEmSize::get()
{
	return mHintingEmSize;
}

System::Double FontProperties::FontRenderingEmSize::get()
{
	return mRenderingEmSize;
}

System::Windows::BaselineAlignment FontProperties::BaselineAlignment::get()
{
	return __super::BaselineAlignment;
}

System::Windows::Media::Typeface^ FontProperties::Typeface::get()
{
	return mTypeface;
}

System::Windows::TextDecorationCollection^ FontProperties::TextDecorations::get()
{
	return mDecorations;
}

System::Windows::Media::TextEffectCollection^ FontProperties::TextEffects::get()
{
	return nullptr;
}

System::Windows::Media::TextFormatting::TextRunTypographyProperties^ FontProperties::TypographyProperties::get()
{
	return __super::TypographyProperties;
}

System::Windows::Media::Brush^ FontProperties::ForegroundBrush::get()
{
	return mForegroundBrush;
}

System::Windows::Media::Brush^ FontProperties::BackgroundBrush::get()
{
	return mBackgroundBrush;
}

System::Globalization::CultureInfo^ FontProperties::CultureInfo::get()
{
	return System::Globalization::CultureInfo::CurrentUICulture;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

FontParagraphProperties::FontParagraphProperties (System::Windows::Media::TextFormatting::TextRunProperties^ pTextRunProperties)
:	mTextRunProperties (pTextRunProperties),
	mTextMarkerProperties (nullptr),
	mFirstLineInParagraph (true),
	mFlowDirection (System::Windows::FlowDirection::LeftToRight),
	mTextAlignment (System::Windows::TextAlignment::Left),
	mTextWrapping (System::Windows::TextWrapping::Wrap)
{
}

FontParagraphProperties::FontParagraphProperties (System::Windows::Media::TextFormatting::TextRunProperties^ pTextRunProperties, System::Windows::Media::TextFormatting::TextMarkerProperties^ pTextMarkerProperties)
:	mTextRunProperties (pTextRunProperties),
	mTextMarkerProperties (pTextMarkerProperties),
	mFirstLineInParagraph (true),
	mFlowDirection (System::Windows::FlowDirection::LeftToRight),
	mTextAlignment (System::Windows::TextAlignment::Left),
	mTextWrapping (System::Windows::TextWrapping::Wrap)
{
}

/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::TextFormatting::TextRunProperties^ FontParagraphProperties::DefaultTextRunProperties::get()
{
	return mTextRunProperties;
}

System::Boolean FontParagraphProperties::FirstLineInParagraph::get()
{
	return mFirstLineInParagraph;
}

System::Double FontParagraphProperties::Indent::get()
{
	return ParagraphIndent;
}

System::Double FontParagraphProperties::LineHeight::get()
{
	return mTextRunProperties->FontRenderingEmSize;
}

System::Windows::FlowDirection FontParagraphProperties::FlowDirection::get()
{
	return mFlowDirection;
}

System::Windows::TextAlignment FontParagraphProperties::TextAlignment::get()
{
	return mTextAlignment;
}

System::Windows::TextWrapping FontParagraphProperties::TextWrapping::get()
{
	return mTextWrapping;
}

System::Windows::TextDecorationCollection^ FontParagraphProperties::TextDecorations::get()
{
	return mTextRunProperties->TextDecorations;
}

System::Windows::Media::TextFormatting::TextMarkerProperties^ FontParagraphProperties::TextMarkerProperties::get()
{
	return mTextMarkerProperties;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

FontMarkerProperties::FontMarkerProperties (System::Windows::Media::TextFormatting::TextSource^ pTextSource)
:	mTextSource (pTextSource)
{
}

System::Double FontMarkerProperties::Offset::get()
{
	return 0;
}

System::Windows::Media::TextFormatting::TextSource^ FontMarkerProperties::TextSource::get()
{
	return mTextSource;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
