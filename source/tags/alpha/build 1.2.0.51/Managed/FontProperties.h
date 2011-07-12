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

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

public ref class FontProperties : public System::Windows::Media::TextFormatting::TextRunProperties
{
public:
	FontProperties (System::Drawing::Font^ pFont);
	FontProperties (System::Drawing::Font^ pFont, System::Windows::Media::Brush^ pForegroundBrush);
	FontProperties (System::Drawing::Font^ pFont, System::Windows::Media::Brush^ pForegroundBrush, System::Windows::Media::Brush^ pBackgroundBrush);

	virtual property System::Double FontHintingEmSize {System::Double get() override;}
	virtual property System::Double FontRenderingEmSize {System::Double get() override;}
	virtual property System::Windows::BaselineAlignment BaselineAlignment {System::Windows::BaselineAlignment get() override;}
	virtual property System::Windows::Media::Typeface^ Typeface {System::Windows::Media::Typeface^ get() override;}
	virtual property System::Windows::TextDecorationCollection^ TextDecorations {System::Windows::TextDecorationCollection^ get() override;}
	virtual property System::Windows::Media::TextEffectCollection^ TextEffects {System::Windows::Media::TextEffectCollection^ get() override;}
	virtual property System::Windows::Media::TextFormatting::TextRunTypographyProperties^ TypographyProperties {System::Windows::Media::TextFormatting::TextRunTypographyProperties^ get() override;}
	virtual property System::Windows::Media::Brush^ ForegroundBrush {System::Windows::Media::Brush^ get() override;}
	virtual property System::Windows::Media::Brush^ BackgroundBrush {System::Windows::Media::Brush^ get() override;}
	virtual property System::Globalization::CultureInfo^ CultureInfo {System::Globalization::CultureInfo^ get() override;}
	
protected:
	void CommonConstruct (System::Drawing::Font^ pFont);
	
protected:
	System::Double								mHintingEmSize;
	System::Double								mRenderingEmSize;
	System::Windows::Media::Typeface^			mTypeface;
	System::Windows::TextDecorationCollection^	mDecorations;
	System::Windows::Media::Brush^				mForegroundBrush;
	System::Windows::Media::Brush^				mBackgroundBrush;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

private ref class FontParagraphProperties : System::Windows::Media::TextFormatting::TextParagraphProperties
{
public:
	FontParagraphProperties (System::Windows::Media::TextFormatting::TextRunProperties^ pTextRunProperties);
	FontParagraphProperties (System::Windows::Media::TextFormatting::TextRunProperties^ pTextRunProperties, System::Windows::Media::TextFormatting::TextMarkerProperties^ pTextMarkerProperties);

public:
	virtual property System::Windows::Media::TextFormatting::TextRunProperties^ DefaultTextRunProperties {System::Windows::Media::TextFormatting::TextRunProperties^ get() override;}
	virtual property System::Boolean FirstLineInParagraph {System::Boolean get() override;}
	virtual property System::Double Indent {System::Double get() override;}
	virtual property System::Double LineHeight {System::Double get() override;}
	virtual property System::Windows::FlowDirection FlowDirection {System::Windows::FlowDirection get() override;}
	virtual property System::Windows::TextAlignment TextAlignment {System::Windows::TextAlignment get() override;}
	virtual property System::Windows::TextWrapping TextWrapping {System::Windows::TextWrapping get() override;}
	virtual property System::Windows::TextDecorationCollection^ TextDecorations {System::Windows::TextDecorationCollection^ get() override;}
	virtual property System::Windows::Media::TextFormatting::TextMarkerProperties^ TextMarkerProperties {System::Windows::Media::TextFormatting::TextMarkerProperties^ get() override;}

protected:
	System::Windows::Media::TextFormatting::TextRunProperties^		mTextRunProperties;
	System::Windows::Media::TextFormatting::TextMarkerProperties^	mTextMarkerProperties;
	System::Boolean													mFirstLineInParagraph;
	System::Windows::FlowDirection									mFlowDirection;
	System::Windows::TextAlignment									mTextAlignment;
	System::Windows::TextWrapping									mTextWrapping;
};

/////////////////////////////////////////////////////////////////////////////

private ref class FontMarkerProperties : System::Windows::Media::TextFormatting::TextMarkerProperties
{
public:
	FontMarkerProperties (System::Windows::Media::TextFormatting::TextSource^ pTextSource);

	virtual property System::Double Offset {System::Double get() override;}
	virtual property System::Windows::Media::TextFormatting::TextSource^ TextSource {System::Windows::Media::TextFormatting::TextSource^ get() override;}

protected:
	System::Windows::Media::TextFormatting::TextSource^	mTextSource;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent
/////////////////////////////////////////////////////////////////////////////
