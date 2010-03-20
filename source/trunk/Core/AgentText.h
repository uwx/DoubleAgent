/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef AGENTTEXT_H_INCLUDED_
#define AGENTTEXT_H_INCLUDED_
#pragma once

#include <msxml6.h>
#include "TextWrap.h"

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF(ISAXXMLReader, __uuidof(ISAXXMLReader));
_COM_SMARTPTR_TYPEDEF(ISAXLocator, __uuidof(ISAXLocator));

/////////////////////////////////////////////////////////////////////////////

class CAgentText
{
public:
	CAgentText (UINT pSapiVersion = 5);
	CAgentText (const CStringArray & pWords, UINT pSapiVersion = 5);
	CAgentText (const CAgentText & pText, UINT pSapiVersion = 5);
	virtual ~CAgentText ();

// Attributes
public:
	UINT GetSapiVersion () const;
	bool SetSapiVersion (UINT pSapiVersion);

	const CString & GetFullText () const {return mText;}
	CString GetSpeechText () const;

// Operations
public:
	CAgentText & operator= (const CStringArray & pWords);
	CAgentText & operator+= (const CStringArray & pWords);
	CAgentText & operator= (const CAgentText & pText);
	CAgentText & operator+= (const CAgentText & pText);

// Implementation
public:
	void Append (const CStringArray & pTextWords, const CStringArray & pSpeechWords);

protected:
	UINT			mSapiVersion;
	CString			mText;
	CStringArray	mTextWords;
	CStringArray	mSpeechWords;
};

/////////////////////////////////////////////////////////////////////////////

class CAgentTextParse : public CCmdTarget, public CAgentText
{
public:
	CAgentTextParse (LPCTSTR pText = NULL, UINT pSapiVersion = 5);
	CAgentTextParse (const CStringArray & pWords, UINT pSapiVersion = 5);
	CAgentTextParse (const CAgentText & pText, UINT pSapiVersion = 5);
	virtual ~CAgentTextParse ();
	DECLARE_DYNAMIC (CAgentTextParse)

// Attributes
public:

// Operations
public:
	CAgentTextParse & operator= (LPCTSTR pText);
	CAgentTextParse & operator+= (LPCTSTR pText);
	CAgentTextParse & operator= (const CStringArray & pWords);
	CAgentTextParse & operator+= (const CStringArray & pWords);
	CAgentTextParse & operator= (const CAgentText & pText);
	CAgentTextParse & operator+= (const CAgentText & pText);

// Overrides
	//{{AFX_VIRTUAL(CAgentTextParse)
	//}}AFX_VIRTUAL

// Implementation
public:
	static int SplitText (LPCTSTR pText, CStringArray & pTextWords);
protected:
	int SplitMap (LPCTSTR pText, CString * pSpeechWords, CString * pTextWords);

	void ParseText (LPCTSTR pText, CStringArray & pTextWords, CStringArray & pSpeechWords);
	void ParseTags (LPCTSTR pText, CStringArray & pTextWords, CStringArray & pSpeechWords, bool pOuterParse);
	void PutTag (LPCTSTR pTag, LPCTSTR pText, CStringArray & pTextWords, CStringArray & pSpeechWords, bool pOuterParse);

	void UnquoteMappedText (CString & pText);
	void AppendWords (const CStringArray & pAppend, CStringArray & pWords, UINT pSapiVersion = 0);
	void PadWords (CStringArray & pWords, UINT pSapiVersion = 0);
	void FinishWords (CStringArray & pWords, UINT pSapiVersion = 0);
	void SpeechFromText (const CStringArray & pTextWords, CStringArray & pSpeechWords);
	void FinishSpeech (CStringArray & pSpeechWords);

	BEGIN_INTERFACE_PART(SaxContentHandler, ISAXContentHandler)
		HRESULT STDMETHODCALLTYPE putDocumentLocator (ISAXLocator *pLocator);
		HRESULT STDMETHODCALLTYPE startDocument (void);
		HRESULT STDMETHODCALLTYPE endDocument (void);
		HRESULT STDMETHODCALLTYPE startPrefixMapping (const wchar_t *pwchPrefix, int cchPrefix, const wchar_t *pwchUri, int cchUri);
		HRESULT STDMETHODCALLTYPE endPrefixMapping (const wchar_t *pwchPrefix, int cchPrefix);
		HRESULT STDMETHODCALLTYPE startElement (const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName, ISAXAttributes *pAttributes);
		HRESULT STDMETHODCALLTYPE endElement (const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName);
		HRESULT STDMETHODCALLTYPE characters (const wchar_t *pwchChars, int cchChars);
		HRESULT STDMETHODCALLTYPE ignorableWhitespace (const wchar_t *pwchChars, int cchChars);
		HRESULT STDMETHODCALLTYPE processingInstruction (const wchar_t *pwchTarget, int cchTarget, const wchar_t *pwchData, int cchData);
		HRESULT STDMETHODCALLTYPE skippedEntity (const wchar_t *pwchName, int cchName);
	END_INTERFACE_PART(SaxContentHandler)

	BEGIN_INTERFACE_PART(SaxErrorHandler, ISAXErrorHandler)
		HRESULT STDMETHODCALLTYPE error (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
		HRESULT STDMETHODCALLTYPE fatalError (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
		HRESULT STDMETHODCALLTYPE ignorableWarning (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
	END_INTERFACE_PART(SaxErrorHandler)

	DECLARE_INTERFACE_MAP()

private:
	ISAXXMLReaderPtr	mSaxReader;
	ISAXLocatorPtr		mSaxLocator;
	CStringArray		mSaxElements;
	CStringArray		mSaxTextWords;
	CStringArray		mSaxSpeechWords;
};

/////////////////////////////////////////////////////////////////////////////

class CAgentTextDraw : public CTextWrap, public CAgentText
{
public:
	CAgentTextDraw (UINT pSapiVersion = 5);
	CAgentTextDraw (const CAgentText & pText, UINT pSapiVersion = 5);
	CAgentTextDraw (const CAgentTextDraw & pText, UINT pSapiVersion = 5);
	virtual ~CAgentTextDraw ();

// Attributes
public:
	CString GetDisplayText (INT_PTR pLookAhead = 0) const;
	CString GetDisplayWord (INT_PTR pWordNdx) const;

	INT_PTR GetWordCount () const {return mTextWords.GetSize();}
	INT_PTR GetWordDisplayed () const {return mWordDisplayed;}

	bool CanScroll (const CRect & pTextBounds) const;
	long GetScrollPos () const {return mScrollPos;}
	long GetScrollInc () const {return mScrollInc;}
	long GetScrollMin () const {return mScrollMin;}
	long GetScrollMax () const {return mScrollMax;}

// Operations
public:
	CAgentTextDraw & operator= (const CAgentText & pText);
	CAgentTextDraw & operator+= (const CAgentText & pText);
	CAgentTextDraw & operator= (const CAgentTextDraw & pText);
	CAgentTextDraw & operator+= (const CAgentTextDraw & pText);

	bool DisplayFirstWord (bool pForSpeech = false);
	bool DisplayNextWord (bool pForSpeech = false);
	bool DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech = true);
	bool DisplayAllWords (bool pForSpeech = false);

	CSize CalcTextSize (CFont * pFont, USHORT pPerLine, USHORT pLines);
	CSize CalcTextSize (CFont * pFont, USHORT pPerLine);

	DWORD CalcScroll (const CRect & pTextBounds, long & pScrollInc, long & pScrollMin, long & pScrollMax, bool pClipLines = false, DWORD pMaxLineTime = 0) const;
	DWORD InitScroll (const CRect & pTextBounds, bool pForceReinit = false, bool pClipLines = false, DWORD pMaxLineTime = 0);
	bool ApplyScroll (const CRect & pTextBounds, CRect * pClipRect = NULL);
	bool Scroll ();

// Overrides
	//{{AFX_VIRTUAL(CAgentTextDraw)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetState (bool pFullReset);
protected:
	virtual bool IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool & pBreakAfter);

protected:
	INT_PTR							mWordDisplayed;
	long							mScrollPos;
	long							mScrollInc;
	long							mScrollMin;
	long							mScrollMax;
private:	
	mutable INT_PTR					mTextCacheLimit;
	mutable INT_PTR					mTextCacheStart;
	mutable COwnPtrArray <CString>	mTextCache;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AGENTTEXT_H_INCLUDED_
