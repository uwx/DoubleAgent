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

#include "AgentText.h"

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
private ref class CAgentTextParse : public CAgentText
#else	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#include <msxml6.h>

_COM_SMARTPTR_TYPEDEF(ISAXXMLReader, __uuidof(ISAXXMLReader));
_COM_SMARTPTR_TYPEDEF(ISAXLocator, __uuidof(ISAXLocator));

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CAgentSaxParse : public  CComObjectRoot, public ISAXContentHandler, public ISAXErrorHandler
{
public:
	CAgentSaxParse () {}
	~CAgentSaxParse () {}

	BEGIN_COM_MAP(CAgentSaxParse)
		COM_INTERFACE_ENTRY(ISAXContentHandler)
		COM_INTERFACE_ENTRY(ISAXErrorHandler)
	END_COM_MAP()
};

class CAgentTextParse : public CComObjectNoLock<CAgentSaxParse>, public CAgentText
#endif
{
public:
#ifdef	__cplusplus_cli
	CAgentTextParse ();
	CAgentTextParse (UINT pSapiVersion);
	CAgentTextParse (System::String^ pText);
	CAgentTextParse (System::String^ pText, UINT pSapiVersion);
	CAgentTextParse (array<System::String^>^ pWords);
	CAgentTextParse (array<System::String^>^ pWords, UINT pSapiVersion);
	CAgentTextParse (CAgentText^ pText);
	CAgentTextParse (CAgentText^ pText, UINT pSapiVersion);
#else
	CAgentTextParse (LPCTSTR pText = NULL, UINT pSapiVersion = 5);
	CAgentTextParse (const CAtlStringArray& pWords, UINT pSapiVersion = 5);
	CAgentTextParse (const CAgentText& pText, UINT pSapiVersion = 5);
	virtual ~CAgentTextParse ();
#endif	

// Attributes
public:

// Operations
public:
#ifdef	__cplusplus_cli
	CAgentTextParse^ operator= (System::String^ pText);
	CAgentTextParse^ operator+= (System::String^ pText);
	CAgentTextParse^ operator= (array<System::String^>^ pWords);
	CAgentTextParse^ operator+= (array<System::String^>^ pWords);
	CAgentTextParse^ operator= (CAgentText^ pText);
	CAgentTextParse^ operator+= (CAgentText^ pText);
#else
	CAgentTextParse& operator= (LPCTSTR pText);
	CAgentTextParse& operator+= (LPCTSTR pText);
	CAgentTextParse& operator= (const CAtlStringArray& pWords);
	CAgentTextParse& operator+= (const CAtlStringArray& pWords);
	CAgentTextParse& operator= (const CAgentText& pText);
	CAgentTextParse& operator+= (const CAgentText& pText);
#endif	

// Overrides

// Implementation
public:
#ifdef	__cplusplus_cli
	static int SplitText (System::String^ pText,array<System::String^>^% pTextWords);
#else
	static int SplitText (LPCTSTR pText, CAtlStringArray& pTextWords);
#endif	
protected:
#ifdef	__cplusplus_cli
	static const System::Char* CAgentTextParse::MatchTag (System::String^ pText, int pStartAt);
	int SplitMap (System::String^ pText, System::String^% pSpeechWords, System::String^% pTextWords);

	void ParseText (System::String^ pText, array<System::String^>^% pTextWords, array<System::String^>^% pSpeechWords);
	void ParseTags (System::String^ pText, array<System::String^>^% pTextWords, array<System::String^>^% pSpeechWords, System::Boolean pOuterParse);
	void PutTag (const System::Char* pTag, System::String^ pText, array<System::String^>^% pTextWords, array<System::String^>^% pSpeechWords, bool pOuterParse);

	void UnquoteMappedText (System::String^% pText);
	void AppendWords (array<System::String^>^ pAppend, array<System::String^>^% pWords);
	void AppendWords (array<System::String^>^ pAppend, array<System::String^>^% pWords, UINT pSapiVersion);
	void PadWords (array<System::String^>^% pWords);
	void PadWords (array<System::String^>^% pWords, UINT pSapiVersion);
	void FinishWords (array<System::String^>^% pWords);
	void FinishWords (array<System::String^>^% pWords, UINT pSapiVersion);
	void SpeechFromText (array<System::String^>^ pTextWords, array<System::String^>^% pSpeechWords);
	void FinishSpeech (array<System::String^>^% pSpeechWords);
#else
	static LPCTSTR MatchTag (LPCTSTR pText);
	int SplitMap (LPCTSTR pText, CAtlString* pSpeechWords, CAtlString* pTextWords);

	void ParseText (LPCTSTR pText, CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords);
	void ParseTags (LPCTSTR pText, CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords, bool pOuterParse);
	void PutTag (LPCTSTR pTag, LPCTSTR pText, CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords, bool pOuterParse);

	void UnquoteMappedText (CAtlString& pText);
	void AppendWords (const CAtlStringArray& pAppend, CAtlStringArray& pWords, UINT pSapiVersion = 0);
	void PadWords (CAtlStringArray& pWords, UINT pSapiVersion = 0);
	void FinishWords (CAtlStringArray& pWords, UINT pSapiVersion = 0);
	void SpeechFromText (const CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords);
	void FinishSpeech (CAtlStringArray& pSpeechWords);
#endif	

#ifndef	__cplusplus_cli
// ISAXContentHandler
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

// ISAXErrorHandler
	HRESULT STDMETHODCALLTYPE error (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
	HRESULT STDMETHODCALLTYPE fatalError (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
	HRESULT STDMETHODCALLTYPE ignorableWarning (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
#endif	

private:
#ifdef	__cplusplus_cli
	static CAgentTextParse::CAgentTextParse ();
	static array<const System::Char*>^	mTags;
#else
	static CAtlTypeArray <LPCTSTR>		mTags;
	ISAXXMLReaderPtr					mSaxReader;
	ISAXLocatorPtr						mSaxLocator;
	CAtlStringArray						mSaxElements;
	CAtlStringArray						mSaxTextWords;
	CAtlStringArray						mSaxSpeechWords;
#endif	
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
