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
#include "TextWrap.h"

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentText
#else
class CAgentText
#endif
{
public:
#ifdef	__cplusplus_cli
	CAgentText ();
	CAgentText (UINT pSapiVersion);
	CAgentText (array<System::String^>^ pWords);
	CAgentText (array<System::String^>^ pWords, UINT pSapiVersion);
	CAgentText (CAgentText^ pText);
	CAgentText (CAgentText^ pText, UINT pSapiVersion);
#else
	CAgentText (UINT pSapiVersion = 5);
	CAgentText (const CAtlStringArray& pWords, UINT pSapiVersion = 5);
	CAgentText (const CAgentText& pText, UINT pSapiVersion = 5);
	virtual ~CAgentText ();
#endif	

// Attributes
public:
#ifdef	__cplusplus_cli
	property System::UInt32 SapiVersion {System::UInt32 get(); void set (System::UInt32 pSapiVersion);}
#else
	UINT GetSapiVersion () const;
	bool SetSapiVersion (UINT pSapiVersion);
#endif	

#ifdef	__cplusplus_cli
	property System::String^ FullText {System::String^ get();}
	property System::String^ SpeechText {System::String^ get();}
#else
	const CAtlString& GetFullText () const {return mText;}
	CAtlString GetSpeechText () const;
#endif	

// Operations
public:
#ifdef	__cplusplus_cli
	CAgentText^ operator= (array<System::String^>^ pWords);
	CAgentText^ operator+= (array<System::String^>^ pWords);
	CAgentText^ operator= (CAgentText^ pText);
	CAgentText^ operator+= (CAgentText^ pText);
#else
	CAgentText& operator= (const CAtlStringArray& pWords);
	CAgentText& operator+= (const CAtlStringArray& pWords);
	CAgentText& operator= (const CAgentText& pText);
	CAgentText& operator+= (const CAgentText& pText);
#endif	

// Implementation
public:
#ifdef	__cplusplus_cli
	void Append (CAgentText^ pText);
	void Append (CAgentText^ pText, System::Boolean pAppendSpeech);
	void Append (array<System::String^>^ pTextWords, array<System::String^>^ pSpeechWords);
	void Append (array<System::String^>^ pTextWords, array<System::String^>^ pSpeechWords, System::Boolean pAppendSpeech);
#else
	void Append (const CAgentText& pText, bool pAppendSpeech = false);
	void Append (const CAtlStringArray& pTextWords, const CAtlStringArray& pSpeechWords, bool pAppendSpeech = false);
#endif	

#ifdef	__cplusplus_cli
internal:
	System::UInt32			mSapiVersion;
	System::String^			mText;
	array<System::String^>^	mTextWords;
	array<System::String^>^	mSpeechWords;
#else
protected:
	UINT					mSapiVersion;
	CAtlString				mText;
	CAtlStringArray			mTextWords;
	CAtlStringArray			mSpeechWords;
#endif	
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentTextDraw : public CAgentText
#else
class CAgentTextDraw : public CTextWrap, public CAgentText
#endif
{
public:
#ifdef	__cplusplus_cli
	CAgentTextDraw ();
	CAgentTextDraw (UINT pSapiVersion);
	CAgentTextDraw (CAgentText^ pText);
	CAgentTextDraw (CAgentText^ pText, System::UInt32 pSapiVersion);
	CAgentTextDraw (CAgentTextDraw^ pText);
	CAgentTextDraw (CAgentTextDraw^ pText, System::UInt32 pSapiVersion);
#else
	CAgentTextDraw (UINT pSapiVersion = 5);
	CAgentTextDraw (const CAgentText& pText, UINT pSapiVersion = 5);
	CAgentTextDraw (const CAgentTextDraw& pText, UINT pSapiVersion = 5);
	virtual ~CAgentTextDraw ();
#endif	

// Attributes
public:
#ifdef	__cplusplus_cli
	property System::String^ DisplayText {System::String^ get();}
	System::String^ GetDisplayText (int pLookAhead);
	System::String^ GetDisplayWord (int pWordNdx);
#else
	CAtlString GetDisplayText (INT_PTR pLookAhead = 0) const;
	CAtlString GetDisplayWord (INT_PTR pWordNdx) const;
#endif	

#ifdef	__cplusplus_cli
	property int WordCount {int get();}
	property int WordDisplayed {int get();}
	property int LookAhead {int get();}
#else
	__declspec(property(get=get_WordCount))			INT_PTR	WordCount;
	__declspec(property(get=get_WordDisplayed))		INT_PTR	WordDisplayed;
	__declspec(property(get=get_LookAhead))			INT_PTR	LookAhead;

	INT_PTR get_WordCount () const {return mTextWords.GetCount();}
	INT_PTR get_WordDisplayed () const {return mWordDisplayed;}
	INT_PTR get_LookAhead () const {return min (WordCount - WordDisplayed - 1, mDefaultLookAhead);}
#endif

#ifdef	__cplusplus_cli
	property System::Boolean CanPace {System::Boolean get();}
	virtual property System::Boolean CanScroll {virtual System::Boolean get();}
	property System::Int32 ScrollPos {System::Int32 get();}
	property System::Int32 ScrollInc {System::Int32 get();}
	property System::Int32 ScrollMin {System::Int32 get();}
	property System::Int32 ScrollMax {System::Int32 get();}
	property System::UInt32 ScrollTime {System::UInt32 get();}
#else
	__declspec(property(get=get_TextBounds, put=put_TextBounds))		CRect	TextBounds;
	__declspec(property(get=get_CanPace))								bool	CanPace;
	__declspec(property(get=get_CanScroll))								bool	CanScroll;
	__declspec(property(get=get_ScrollBounds, put=put_ScrollBounds))	CRect	ScrollBounds;
	__declspec(property(get=get_ScrollPos))								long	ScrollPos;
	__declspec(property(get=get_ScrollInc))								long	ScrollInc;
	__declspec(property(get=get_ScrollMin))								long	ScrollMin;
	__declspec(property(get=get_ScrollMax))								long	ScrollMax;
	__declspec(property(get=get_ScrollTime))							DWORD	ScrollTime;

	CRect get_TextBounds () const {return mBounds;}
	void put_TextBounds (const CRect & pTextBounds) {mBounds = pTextBounds;}

	bool get_CanPace () const;
	bool get_CanScroll () const;
	CRect get_ScrollBounds () const {return mScrollBounds;}
	void put_ScrollBounds (const CRect & pScrollBounds) {mScrollBounds = pScrollBounds;}
	long get_ScrollPos () const {return mScrollPos;}
	long get_ScrollInc () const {return mScrollInc;}
	long get_ScrollMin () const {return mScrollMin;}
	long get_ScrollMax () const {return mScrollMax;}
	DWORD get_ScrollTime () const {return mScrollTime;}
#endif

#ifdef	__cplusplus_cli
	static property System::UInt32 DefaultAutoPaceTime {System::UInt32 get();}
	static property int DefaultLookAhead {int get();}
#else
	static const DWORD		mDefaultAutoPaceTime;
	static const INT_PTR	mDefaultLookAhead;
#endif	

// Operations
public:
#ifdef	__cplusplus_cli
	CAgentTextDraw^ operator= (CAgentText^ pText);
	CAgentTextDraw^ operator+= (CAgentText^ pText);
	CAgentTextDraw^ operator= (CAgentTextDraw^ pText);
	CAgentTextDraw^ operator+= (CAgentTextDraw^ pText);
#else
	CAgentTextDraw& operator= (const CAgentText& pText);
	CAgentTextDraw& operator+= (const CAgentText& pText);
	CAgentTextDraw& operator= (const CAgentTextDraw& pText);
	CAgentTextDraw& operator+= (const CAgentTextDraw& pText);
#endif

#ifdef	__cplusplus_cli
	System::Boolean DisplayFirstWord ();
	System::Boolean DisplayFirstWord (System::Boolean pForSpeech);
	System::Boolean DisplayNextWord ();
	System::Boolean DisplayNextWord (System::Boolean pForSpeech);
	System::Boolean DisplayThisWord (System::Int32 pWordPos, System::Int32 pWordLength);
	System::Boolean DisplayThisWord (System::Int32 pWordPos, System::Int32 pWordLength, System::Boolean pForSpeech);
	System::Boolean DisplayThisWord (int pWordNdx);
	System::Boolean DisplayThisWord (int pWordNdx, System::Boolean pForSpeech);
	System::Boolean DisplayAllWords ();
	System::Boolean DisplayAllWords (System::Boolean pForSpeech);
#else
	bool DisplayFirstWord (bool pForSpeech = false);
	bool DisplayNextWord (bool pForSpeech = false);
	bool DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech = true);
	bool DisplayThisWord (INT_PTR pWordNdx, bool pForSpeech = true);
	bool DisplayAllWords (bool pForSpeech = false);
#endif

#ifndef	__cplusplus_cli
	CSize CalcTextSize (HFONT pFont, USHORT pPerLine, USHORT pLines);
	CSize CalcTextSize (HFONT pFont, USHORT pPerLine);
	CSize CalcTextSize (HFONT pFont);
	CSize CalcTextSize (HDC pDC);
#endif

#ifdef	__cplusplus_cli
	virtual void ResetScroll ();
	virtual System::Boolean Scroll ();
#else
	virtual bool InitScroll (const CRect& pScrollBounds, bool pDelayed = false, DWORD pMaxLineTime = 0);
	virtual bool ApplyScroll ();
	virtual void ResetScroll ();
	virtual bool Scroll ();
#endif

// Implementation
public:
#ifdef	__cplusplus_cli
	virtual void ResetState ();
#else
	void ResetState ();
#endif
	void DisplayNoWords ();

protected:
#ifdef	__cplusplus_cli
	System::Boolean InitScroll (System::Int32 pScrollInc, System::Int32 pScrollMin, System::Int32 pScrollMax);
	System::UInt32 CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax);
	System::UInt32 CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed);
	virtual System::UInt32 CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed, System::UInt32 pMaxLineTime);
	System::UInt32 CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Double pTextHeight, System::Double pScrollHeight, int pLineCount, System::Boolean pDelayed, System::UInt32 pMaxLineTime);
#else
	bool InitScroll (long pScrollInc, long pScrollMin, long pScrollMax);
	DWORD CalcScroll (long& pScrollInc, long& pScrollMin, long& pScrollMax, bool pDelayed, DWORD pMaxLineTime) const;
	DWORD CalcScroll (long& pScrollInc, long& pScrollMin, long& pScrollMax, long pTextHeight, long pScrollHeight, INT_PTR pLineCount, bool pDelayed, DWORD pMaxLineTime) const;
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter);
#endif	

protected:
#ifdef	__cplusplus_cli
	int										mWordDisplayed;
	System::Int32							mScrollPos;
	System::Int32							mScrollInc;
	System::Int32							mScrollMin;
	System::Int32							mScrollMax;
	System::UInt32							mScrollTime;
#else
	INT_PTR									mWordDisplayed;
	CRect									mScrollBounds;
	long									mScrollPos;
	long									mScrollInc;
	long									mScrollMin;
	long									mScrollMax;
	DWORD									mScrollTime;
#endif	
private:
#ifdef	__cplusplus_cli
	mutable int														mTextCacheLimit;
	mutable int														mTextCacheStart;
	mutable System::Collections::Generic::List <System::String^>^	mTextCache;
#else
	mutable INT_PTR													mTextCacheLimit;
	mutable INT_PTR													mTextCacheStart;
	mutable CAtlOwnPtrArray <CAtlString>							mTextCache;
#endif	
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentTextDrawForms : public CAgentTextDraw
{
public:
	CAgentTextDrawForms (CAgentTextDraw^ pTextDraw);
	CAgentTextDrawForms (CAgentTextDrawForms^ pTextDraw);
	static CAgentTextDrawForms^ Downcast (CAgentTextDraw^ pTextDraw);

// Attributes
public:
	property CTextWrap^ TextWrap {CTextWrap^ get();}
	property System::Drawing::RectangleF ScrollBounds {System::Drawing::RectangleF get();}
	property System::Drawing::RectangleF TextBounds {System::Drawing::RectangleF get(); void set (System::Drawing::RectangleF pTextBounds);}
	virtual property System::Boolean CanScroll {virtual System::Boolean get() override;}

// Operations
public:
	CAgentTextDrawForms^ operator= (CAgentTextDrawForms^ pTextDraw);

	System::Drawing::SizeF CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine, System::UInt16 pLines);
	System::Drawing::SizeF CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine);
	System::Drawing::SizeF CalcTextSize (System::Drawing::Font^ pFont);
	System::Drawing::SizeF CalcTextSize (System::Drawing::Font^ pFont, System::Drawing::Graphics^ pGraphics);

	System::Boolean InitScroll (System::Drawing::RectangleF pScrollBounds);
	System::Boolean InitScroll (System::Drawing::RectangleF pScrollBounds, System::Boolean pDelayed);
	System::Boolean InitScroll (System::Drawing::RectangleF pScrollBounds, System::Boolean pDelayed, System::UInt32 pMaxLineTime);
	System::Boolean ApplyScroll ();
	virtual void ResetScroll () override;

// Implementation
public:
	virtual void ResetState () override;
protected:
	virtual System::UInt32 CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed, System::UInt32 pMaxLineTime) override;

protected:
	CTextWrap^					mTextWrap;
	System::Drawing::RectangleF	mScrollBounds;
};
#endif	

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentTextDrawWPF : public CAgentTextDraw
{
public:
	CAgentTextDrawWPF (CAgentTextDraw^ pTextDraw);
	CAgentTextDrawWPF (CAgentTextDrawWPF^ pTextDraw);
	static CAgentTextDrawWPF^ Downcast (CAgentTextDraw^ pTextDraw);

// Attributes
public:
	property System::Windows::Media::TextFormatting::TextFormatter^ TextFormatter {System::Windows::Media::TextFormatting::TextFormatter^ get();}
	property System::Windows::Rect TextBounds {System::Windows::Rect get(); void set(System::Windows::Rect pTextBounds);}
	property System::Windows::Rect ScrollBounds {System::Windows::Rect get();}
	virtual property System::Boolean CanScroll {virtual System::Boolean get() override;}

// Operations
public:
	CAgentTextDrawWPF^ operator= (CAgentTextDrawWPF^ pTextDraw);

	System::Windows::Size CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine, System::UInt16 pLines);
	System::Windows::Size CalcTextSize (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties, System::UInt16 pPerLine, System::UInt16 pLines);
	System::Windows::Size CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine);
	System::Windows::Size CalcTextSize (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties, System::UInt16 pPerLine);
	System::Windows::Size CalcTextSize (System::Drawing::Font^ pFont);
	System::Windows::Size CalcTextSize (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties);

	void CalcUsedHeight (System::Drawing::Font^ pFont);
	void CalcUsedHeight (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties);

	System::Boolean InitScroll (System::Windows::Rect pScrollBounds);
	System::Boolean InitScroll (System::Windows::Rect pScrollBounds, System::Boolean pDelayed);
	System::Boolean InitScroll (System::Windows::Rect pScrollBounds, System::Boolean pDelayed, System::UInt32 pMaxLineTime);
	System::Boolean ApplyScroll ();
	virtual void ResetScroll () override;

	void Draw (System::Windows::Media::DrawingGroup^ pDrawing, System::Drawing::Font^ pFont, System::Drawing::Color pColor, System::Drawing::RectangleF pClipRect, System::Boolean pClipPartialLines);
	void Draw (System::Windows::Media::DrawingGroup^ pDrawing, System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties, System::Windows::Rect pClipRect, System::Boolean pClipPartialLines);

// Implementation
public:
	virtual void ResetState () override;
protected:
	virtual System::UInt32 CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed, System::UInt32 pMaxLineTime) override;

protected:
	ref class TextDrawSource : System::Windows::Media::TextFormatting::TextSource
	{
	public:
		TextDrawSource (CAgentTextDrawWPF^ pTextDraw, System::Windows::Media::TextFormatting::TextRunProperties^ pTextProperties, int pWordCount);

		virtual System::Windows::Media::TextFormatting::TextSpan<System::Windows::Media::TextFormatting::CultureSpecificCharacterBufferRange^>^ GetPrecedingText (int pCharacterIndexLimit) override;
		virtual int GetTextEffectCharacterIndexFromTextSourceCharacterIndex (int pCharacterIndex) override;
		virtual System::Windows::Media::TextFormatting::TextRun^ GetTextRun (int pCharacterIndex) override;

		CAgentTextDrawWPF^											mTextDraw;
		System::Windows::Media::TextFormatting::TextRunProperties^	mTextProperties;
		int															mWordCount;
		System::Boolean												mComplete;
	};

	ref class TextStringSource : System::Windows::Media::TextFormatting::TextSource
	{
	public:
		TextStringSource (System::String^ pText, System::Windows::Media::TextFormatting::TextRunProperties^ pTextProperties);

		virtual System::Windows::Media::TextFormatting::TextSpan<System::Windows::Media::TextFormatting::CultureSpecificCharacterBufferRange^>^ GetPrecedingText (int pCharacterIndexLimit) override;
		virtual int GetTextEffectCharacterIndexFromTextSourceCharacterIndex (int pCharacterIndex) override;
		virtual System::Windows::Media::TextFormatting::TextRun^ GetTextRun (int pCharacterIndex) override;

		System::String^												mText;
		System::Windows::Media::TextFormatting::TextRunProperties^	mTextProperties;
	};

protected:
	System::Windows::Media::TextFormatting::TextFormatter^	mTextFormatter;
	System::Windows::Rect									mScrollBounds;
	System::Windows::Rect									mTextBounds;
	int														mLineCount;
};
#endif	

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
