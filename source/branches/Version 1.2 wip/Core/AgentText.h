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

#ifdef	__cplusplus_cli__NOT
ref class CAgentText
#else
class CAgentText
#endif
{
public:
#ifdef	__cplusplus_cli__NOT
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
#ifdef	__cplusplus_cli__NOT
	property System::UInt32 SapiVersion {System::UInt32 get(); void set (System::UInt32 pSapiVersion);}
#else
	UINT GetSapiVersion () const;
	bool SetSapiVersion (UINT pSapiVersion);
#endif	

#ifdef	__cplusplus_cli__NOT
	property System::String^ FullText {System::String^ get();}
	property System::String^ SpeechText {System::String^ get();}
#else
	const CAtlString& GetFullText () const {return mText;}
	CAtlString GetSpeechText () const;
#endif	

// Operations
public:
#ifdef	__cplusplus_cli__NOT
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
#ifdef	__cplusplus_cli__NOT
	void Append (CAgentText^ pText);
	void Append (CAgentText^ pText, System::Boolean pAppendSpeech);
	void Append (array<System::String^>^ pTextWords, array<System::String^>^ pSpeechWords);
	void Append (array<System::String^>^ pTextWords, array<System::String^>^ pSpeechWords, System::Boolean pAppendSpeech);
#else
	void Append (const CAgentText& pText, bool pAppendSpeech = false);
	void Append (const CAtlStringArray& pTextWords, const CAtlStringArray& pSpeechWords, bool pAppendSpeech = false);
#endif	

#ifdef	__cplusplus_cli__NOT
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

#ifdef	__cplusplus_cli__NOT
ref class CAgentTextDraw : public CTextWrap
#else
class CAgentTextDraw : public CTextWrap, public CAgentText
#endif
{
public:
#ifdef	__cplusplus_cli__NOT
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
#ifdef	__cplusplus_cli__NOT
	System::String^ GetDisplayText ();
	System::String^ GetDisplayText (int pLookAhead);
	System::String^ GetDisplayWord (int pWordNdx);
#else
	CAtlString GetDisplayText (INT_PTR pLookAhead = 0) const;
	CAtlString GetDisplayWord (INT_PTR pWordNdx) const;
#endif	

#ifdef	__cplusplus_cli__NOT
	property int WordCount {int get();}
	property int WordDisplayed {int get();}
	property int LookAhead {int get();}
#else
	INT_PTR GetWordCount () const {return mTextWords.GetCount();}
	INT_PTR GetWordDisplayed () const {return mWordDisplayed;}
	INT_PTR GetLookAhead () const {return min (GetWordCount() - GetWordDisplayed() - 1, mDefaultLookAhead);}
#endif

#ifdef	__cplusplus_cli__NOT
	property System::Boolean CanPace {System::Boolean get();}
	System::Boolean CanScroll (System::Drawing::RectangleF pTextBounds);
	property System::Int32 ScrollPos {System::Int32 get();}
	property System::Int32 ScrollInc {System::Int32 get();}
	property System::Int32 ScrollMin {System::Int32 get();}
	property System::Int32 ScrollMax {System::Int32 get();}
#else
	bool CanPace () const;
	bool CanScroll (const CRect& pTextBounds) const;
	long GetScrollPos () const {return mScrollPos;}
	long GetScrollInc () const {return mScrollInc;}
	long GetScrollMin () const {return mScrollMin;}
	long GetScrollMax () const {return mScrollMax;}
#endif

#ifdef	__cplusplus_cli__NOT
#else
	static const DWORD		mDefaultAutoPaceTime;
	static const INT_PTR	mDefaultLookAhead;
#endif	

// Operations
public:
#ifdef	__cplusplus_cli__NOT
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

#ifdef	__cplusplus_cli__NOT
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

#ifdef	__cplusplus_cli__NOT
	System::Drawing::SizeF CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine, System::UInt16 pLines);
	System::Drawing::SizeF CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine);
#else
	CSize CalcTextSize (HFONT pFont, USHORT pPerLine, USHORT pLines);
	CSize CalcTextSize (HFONT pFont, USHORT pPerLine);
#endif

#ifdef	__cplusplus_cli__NOT
#else
	DWORD CalcScroll (const CRect& pTextBounds, long& pScrollInc, long& pScrollMin, long& pScrollMax, bool pClipLines = false, DWORD pMaxLineTime = 0) const;
	DWORD InitScroll (const CRect& pTextBounds, bool pForceReinit = false, bool pClipLines = false, DWORD pMaxLineTime = 0);
	bool ApplyScroll (const CRect& pTextBounds, CRect* pClipRect = NULL);
	bool Scroll ();
#endif

// Implementation
public:
#ifdef	__cplusplus_cli__NOT
	void ResetState (System::Boolean pFullReset);
#else
	void ResetState (bool pFullReset);
#endif	
protected:
#ifdef	__cplusplus_cli__NOT
	virtual System::Boolean IsBreakChar (System::String^ pText, int pNdx, UINT pPriority, System::Boolean% pBreakAfter) override;
#else
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter);
#endif	

protected:
#ifdef	__cplusplus_cli__NOT
	CAgentText^								mText;
	int										mWordDisplayed;
	System::Int32							mScrollPos;
	System::Int32							mScrollInc;
	System::Int32							mScrollMin;
	System::Int32							mScrollMax;
#else
	INT_PTR									mWordDisplayed;
	long									mScrollPos;
	long									mScrollInc;
	long									mScrollMin;
	long									mScrollMax;
#endif	
private:
#ifdef	__cplusplus_cli__NOT
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
