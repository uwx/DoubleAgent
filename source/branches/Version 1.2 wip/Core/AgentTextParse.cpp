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
#include "AgentTextParse.h"
#ifdef	__cplusplus_cli
using namespace System;
using namespace System::Collections::Generic;
#else	// __cplusplus_cli
#include "StringArrayEx.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_TEXT		(GetProfileDebugInt(_T("DebugSpeechText"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#define	_DEBUG_TAGS		(GetProfileDebugInt(_T("DebugSpeechTags"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
//#define	_DEBUG_SPLIT	LogDebugFast
//#define	_DEBUG_SAX		LogDebugFast
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

static CAgentTextParse::CAgentTextParse ()
{
	mTags = nullptr;
}

CAgentTextParse::CAgentTextParse ()
{
}

CAgentTextParse::CAgentTextParse (UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
}

CAgentTextParse::CAgentTextParse (System::String^ pText)
{
	operator= (pText);
}

CAgentTextParse::CAgentTextParse (System::String^ pText, UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	operator= (pText);
}

CAgentTextParse::CAgentTextParse (array<System::String^>^ pWords)
:	CAgentText (pWords)
{
}

CAgentTextParse::CAgentTextParse (array<System::String^>^ pWords, UINT pSapiVersion)
:	CAgentText (pWords, pSapiVersion)
{
}

CAgentTextParse::CAgentTextParse (CAgentText^ pText)
:	CAgentText (pText)
{
}

CAgentTextParse::CAgentTextParse (CAgentText^ pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
}

/////////////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentTextParse::CAgentTextParse (LPCTSTR pText, UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	operator= (pText);
}

CAgentTextParse::CAgentTextParse (const CAtlStringArray& pWords, UINT pSapiVersion)
:	CAgentText (pWords, pSapiVersion)
{
}

CAgentTextParse::CAgentTextParse (const CAgentText& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
}

CAgentTextParse::~CAgentTextParse ()
{
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentTextParse^ CAgentTextParse::operator= (System::String^ pText)
{
	mTextWords = nullptr;
	operator+= (pText);
	return this;
}

CAgentTextParse^ CAgentTextParse::operator+= (System::String^ pText)
{
	array<String^>^ lTextWords = nullptr;
	array<String^>^ lSpeechWords = nullptr;
	
	if	(!String::IsNullOrEmpty (pText))
	{
		lTextWords = gcnew array<String^> (0);
		lSpeechWords = gcnew array<String^> (0);
		ParseTags (pText, lTextWords, lSpeechWords, true);
	}
	Append (lTextWords, lSpeechWords);
	
	return this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextParse^ CAgentTextParse::operator= (array<System::String^>^ pWords)
{
	CAgentText::operator= (pWords);
	return this;
}

CAgentTextParse^ CAgentTextParse::operator+= (array<System::String^>^ pWords)
{
	CAgentText::operator+= (pWords);
	return this;
}

CAgentTextParse^ CAgentTextParse::operator= (CAgentText^ pText)
{
	CAgentText::operator= (pText);
	return this;
}

CAgentTextParse^ CAgentTextParse::operator+= (CAgentText^ pText)
{
	CAgentText::operator+= (pText);
	return this;
}

/////////////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentTextParse& CAgentTextParse::operator= (LPCTSTR pText)
{
	mTextWords.RemoveAll ();
	operator+= (pText);
	return *this;
}

CAgentTextParse& CAgentTextParse::operator+= (LPCTSTR pText)
{
#ifdef	DebugTimeStart
	DebugTimeStart
#endif
	CAtlStringArray	lTextWords;
	CAtlStringArray	lSpeechWords;

	if	(pText)
	{
		ParseTags (pText, lTextWords, lSpeechWords, true);
	}
	Append (lTextWords, lSpeechWords);

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogTimeMs|LogHighVolume, _T("%f     CAgentText::operator+="), DebugTimeElapsed);
#endif
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextParse& CAgentTextParse::operator= (const CAtlStringArray& pWords)
{
	CAgentText::operator= (pWords);
	return *this;
}

CAgentTextParse& CAgentTextParse::operator+= (const CAtlStringArray& pWords)
{
	CAgentText::operator+= (pWords);
	return *this;
}

CAgentTextParse& CAgentTextParse::operator= (const CAgentText& pText)
{
	CAgentText::operator= (pText);
	return *this;
}

CAgentTextParse& CAgentTextParse::operator+= (const CAgentText& pText)
{
	CAgentText::operator+= (pText);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
static const Char*	sTagChr = _T("\\chr=\0");
static const Char*	sTagCom = _T("\\com=\0");
static const Char*	sTagCtx = _T("\\ctx=\0");
static const Char*	sTagDem = _T("\\dem\\\0");
static const Char*	sTagEmp = _T("\\emp\\\0");
static const Char*	sTagEng = _T("\\eng;\0");
static const Char*	sTagLst = _T("\\lst\\\0");
static const Char*	sTagMap = _T("\\map=\0");
static const Char*	sTagMrk = _T("\\mrk=\0");
static const Char*	sTagPau = _T("\\pau=\0");
static const Char*	sTagPit = _T("\\pit=\0");
static const Char*	sTagPra = _T("\\pra=\0");
static const Char*	sTagPrn = _T("\\prn=\0");
static const Char*	sTagPro = _T("\\pro=\0");
static const Char*	sTagPrt = _T("\\prt=\0");
static const Char*	sTagRst = _T("\\rst\\\0");
static const Char*	sTagRmS = _T("\\rms=\0");
static const Char*	sTagRmW = _T("\\rmw=\0");
static const Char*	sTagRPit = _T("\\rpit=\0");
static const Char*	sTagRPrn = _T("\\rprn=\0");
static const Char*	sTagRSpd = _T("\\rspd=\0");
static const Char*	sTagSpd = _T("\\spd=\0");
static const Char*	sTagVol = _T("\\vol=\0");
#else
static LPCTSTR	sTagChr = _T("\\chr=");
static LPCTSTR	sTagCom = _T("\\com=");
static LPCTSTR	sTagCtx = _T("\\ctx=");
static LPCTSTR	sTagDem = _T("\\dem\\");
static LPCTSTR	sTagEmp = _T("\\emp\\");
static LPCTSTR	sTagEng = _T("\\eng;");
static LPCTSTR	sTagLst = _T("\\lst\\");
static LPCTSTR	sTagMap = _T("\\map=");
static LPCTSTR	sTagMrk = _T("\\mrk=");
static LPCTSTR	sTagPau = _T("\\pau=");
static LPCTSTR	sTagPit = _T("\\pit=");
static LPCTSTR	sTagPra = _T("\\pra=");
static LPCTSTR	sTagPrn = _T("\\prn=");
static LPCTSTR	sTagPro = _T("\\pro=");
static LPCTSTR	sTagPrt = _T("\\prt=");
static LPCTSTR	sTagRst = _T("\\rst\\");
static LPCTSTR	sTagRmS = _T("\\rms=");
static LPCTSTR	sTagRmW = _T("\\rmw=");
static LPCTSTR	sTagRPit = _T("\\rpit=");
static LPCTSTR	sTagRPrn = _T("\\rprn=");
static LPCTSTR	sTagRSpd = _T("\\rspd=");
static LPCTSTR	sTagSpd = _T("\\spd=");
static LPCTSTR	sTagVol = _T("\\vol=");
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
const System::Char* CAgentTextParse::MatchTag (System::String^ pText, int pStartAt)
{
	if	(
			(!mTags)
		&&	(mTags = gcnew array<const Char*> (23))
		)
	{
		int	lNdx = 0;
		mTags[lNdx++] = sTagChr;
		mTags[lNdx++] = sTagCom;
		mTags[lNdx++] = sTagCtx;
		mTags[lNdx++] = sTagDem;
		mTags[lNdx++] = sTagEmp;
		mTags[lNdx++] = sTagEng;
		mTags[lNdx++] = sTagLst;
		mTags[lNdx++] = sTagMap;
		mTags[lNdx++] = sTagMrk;
		mTags[lNdx++] = sTagPau;
		mTags[lNdx++] = sTagPit;
		mTags[lNdx++] = sTagPra;
		mTags[lNdx++] = sTagPrn;
		mTags[lNdx++] = sTagPro;
		mTags[lNdx++] = sTagPrt;
		mTags[lNdx++] = sTagRmS;
		mTags[lNdx++] = sTagRmW;
		mTags[lNdx++] = sTagRPit;
		mTags[lNdx++] = sTagRPrn;
		mTags[lNdx++] = sTagRSpd;
		mTags[lNdx++] = sTagRst;
		mTags[lNdx++] = sTagSpd;
		mTags[lNdx++] = sTagVol;
	}
	
	for each (const Char * lTag in mTags)
	{
		int	lTextNdx;
		int	lCharNdx;
		
		for (lTextNdx = pStartAt, lCharNdx = 0; lTextNdx < pText->Length; lTextNdx++, lCharNdx++)
		{
			if	(lTag[lCharNdx] == 0)
			{
				return lTag;
			}
			else
			if	(lTag[lCharNdx].CompareTo (Char::ToLower(pText[lTextNdx])) != 0)
			{
				break;
			}
		}
	}
	
	return NULL;
}
#else
CAtlTypeArray <LPCTSTR>	CAgentTextParse::mTags;

LPCTSTR CAgentTextParse::MatchTag (LPCTSTR pText)
{
	INT_PTR	lNdx;

	if	(mTags.GetCount() <= 0)
	{
		mTags.Add (sTagChr);
		mTags.Add (sTagCom);
		mTags.Add (sTagCtx);
		mTags.Add (sTagDem);
		mTags.Add (sTagEmp);
		mTags.Add (sTagEng);
		mTags.Add (sTagLst);
		mTags.Add (sTagMap);
		mTags.Add (sTagMrk);
		mTags.Add (sTagPau);
		mTags.Add (sTagPit);
		mTags.Add (sTagPra);
		mTags.Add (sTagPrn);
		mTags.Add (sTagPro);
		mTags.Add (sTagPrt);
		mTags.Add (sTagRmS);
		mTags.Add (sTagRmW);
		mTags.Add (sTagRPit);
		mTags.Add (sTagRPrn);
		mTags.Add (sTagRSpd);
		mTags.Add (sTagRst);
		mTags.Add (sTagSpd);
		mTags.Add (sTagVol);
	}

	for	(lNdx = 0; lNdx < (INT_PTR)mTags.GetCount(); lNdx++)
	{
		if	(_tcsnicmp (pText, mTags [lNdx], _tcslen (mTags [lNdx])) == 0)
		{
			return mTags [lNdx];
		}
	}
	return NULL;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
static void AddWord (array<System::String^>^% pWords, String^ pWord)
{
	Array::Resize (pWords, pWords->Length+1);
	pWords[pWords->Length-1] = pWord;
}
static void AddWord (array<System::String^>^% pWords, const Char * pWord)
{
	AddWord (pWords, gcnew String (pWord));
}

static void InsertWord (array<System::String^>^% pWords, int pInsertAt, String^ pWord)
{
	Array::Resize (pWords, pWords->Length+1);
	for (int lNdx = pWords->Length-1; lNdx > pInsertAt; lNdx--)
	{
		pWords[lNdx] = pWords[lNdx-1];
	}
	pWords[pInsertAt] = pWord;
}
static void InsertWord (array<System::String^>^% pWords, int pInsertAt, const Char * pWord)
{
	InsertWord (pWords, pInsertAt, gcnew String(pWord));
}

static void RemoveWord (array<System::String^>^% pWords, int pRemoveAt)
{
	for (int lNdx = pRemoveAt; lNdx < pWords->Length-2; lNdx++)
	{
		pWords[lNdx] = pWords[lNdx+1];
	}
	Array::Resize (pWords, pWords->Length-1);
}

static void ReplaceAll (array<System::String^>^ pWords, wchar_t pOldChar, wchar_t pNewChar)
{
	for (int lNdx = 0; lNdx < pWords->Length; lNdx++)
	{
		pWords[lNdx] = pWords[lNdx]->Replace (pOldChar, pNewChar);
	}
}
static void ReplaceAll (array<System::String^>^ pWords, String^ pOldString, String^ pNewString)
{
	for (int lNdx = 0; lNdx < pWords->Length; lNdx++)
	{
		pWords[lNdx] = pWords[lNdx]->Replace (pOldString, pNewString);
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentTextParse::ParseTags (System::String^ pText, array<System::String^>^% pTextWords, array<System::String^>^% pSpeechWords, System::Boolean pOuterParse)
#else
void CAgentTextParse::ParseTags (LPCTSTR pText, CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords, bool pOuterParse)
#endif
{
	int				lTextNdx;
	int				lTagStart = -1;
	int				lTagEnd = -1;
#ifdef	__cplusplus_cli
	const Char *	lTag = NULL;
#else	
	LPCTSTR			lTag = NULL;
#endif	

#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%sParseTags [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), DebugStr(pText), pOuterParse);
#endif

#ifdef	__cplusplus_cli
	for	(lTextNdx = 0; lTextNdx < pText->Length; lTextNdx++)
#else
	for	(lTextNdx = 0; pText [lTextNdx]; lTextNdx++)
#endif	
	{
		if	(pText [lTextNdx] == _T('\\'))
		{
			if	(lTagStart >= 0)
			{
#ifdef	_DEBUG_TAGS
				LogMessage (_DEBUG_TAGS, _T("%s  Tag [%s] end at [%d] [%s]"), (pOuterParse?_T(""):_T("  ")), lTag, lTextNdx, DebugStr(pText+lTextNdx));
#endif
#ifdef	__cplusplus_cli
				PutTag (lTag, pText->Substring (lTagStart, lTextNdx-lTagStart), pTextWords, pSpeechWords, pOuterParse);
#else
				PutTag (lTag, CAtlString (pText+lTagStart, lTextNdx-lTagStart), pTextWords, pSpeechWords, pOuterParse);
#endif				
				lTagEnd = lTextNdx;
				lTagStart = -1;
			}
			else
			{
				if	(pText [lTextNdx+1] == _T('\\'))
				{
					lTextNdx++;
					continue;
				}
				if	(lTextNdx > lTagEnd+1)
				{
					if	(
							(pOuterParse)
						&&	(mSapiVersion >= 5)
						)
					{
#ifdef	__cplusplus_cli
						ParseText (pText->Substring (lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords, pSpeechWords);
#else
						ParseText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords, pSpeechWords);
#endif						
					}
					else
					{
#ifdef	__cplusplus_cli
						SplitText (pText->Substring (lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords);
#else
						SplitText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords);
#endif						
					}
				}

#ifdef	__cplusplus_cli
				if	(lTag = MatchTag (pText, lTextNdx))
#else
				if	(lTag = MatchTag (pText+lTextNdx))
#endif				
				{
#ifdef	_DEBUG_TAGS
					LogMessage (_DEBUG_TAGS, _T("%s  Tag [%s] at [%d]"), (pOuterParse?_T(""):_T("  ")), lTag, lTextNdx);
#endif
					if	(lTag == sTagMap)
					{
						lTextNdx += (int)_tcslen (lTag);
						lTagStart = lTextNdx;
#ifdef	__cplusplus_cli
						lTextNdx += SplitMap (pText->Substring(lTagStart), gcnew String(""), gcnew String(""));
#else
						lTextNdx += SplitMap (pText+lTagStart, NULL, NULL);
#endif						
					}
					else
					{
						lTextNdx += (int)_tcslen (lTag)-1;
						if	(pText [lTextNdx] == _T('\\'))
						{
							lTextNdx--;
						}
						lTagStart = lTextNdx+1;
					}
				}
				else
				{
					lTagEnd = lTextNdx;
					lTagStart = -1;
				}
			}
		}
	}

	if	(
			(lTagStart < 0)
		&&	(lTextNdx > lTagEnd+1)
		)
	{
		if	(
				(pOuterParse)
			&&	(mSapiVersion >= 5)
			)
		{
#ifdef	__cplusplus_cli
			ParseText (pText->Substring (lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords, pSpeechWords);
#else
			ParseText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords, pSpeechWords);
#endif			
		}
		else
		{
#ifdef	__cplusplus_cli
			SplitText (pText->Substring (lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords);
#else
			SplitText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords);
#endif			
		}
	}

#ifdef	__cplusplus_cli
	if	(pSpeechWords->Length > 0)
#else
	if	(pSpeechWords.GetCount() > 0)
#endif	
	{
		SpeechFromText (pTextWords, pSpeechWords);
		FinishSpeech (pSpeechWords);
	}
	if	(pOuterParse)
	{
		SpeechFromText (pTextWords, pSpeechWords);
		FinishWords (pTextWords);
		FinishWords (pSpeechWords, mSapiVersion);
	}
#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%sParsed [%s] Text [%s] Speech [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), DebugStr(pText), DebugStr(JoinStringArray(pTextWords, _T("]["))), DebugStr(JoinStringArray(pSpeechWords, _T("]["))), pOuterParse);
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentTextParse::PutTag (const System::Char* pTag, System::String^ pText, array<System::String^>^% pTextWords, array<System::String^>^% pSpeechWords, bool pOuterParse)
#else
void CAgentTextParse::PutTag (LPCTSTR pTag, LPCTSTR pText, CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords, bool pOuterParse)
#endif
{
#ifdef	__cplusplus_cli
	UInt32		lNumVal;
	String^		lTagString;
#else
	ULONG		lNumVal;
	LPTSTR		lNumValEnd;
	CAtlString	lTagString;
#endif	

#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%s  PutTag [%s] [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), pTag, DebugStr(pText), pOuterParse);
#endif
	if	(pTag == sTagMap)
	{
#ifdef	__cplusplus_cli
		String^					lText = String::Empty;
		String^					lSpeech = String::Empty;
		array<System::String^>^	lTextWords = gcnew array<System::String^> (0);
		array<System::String^>^	lSpeechWords = gcnew array<System::String^> (0);
#else
		CAtlString		lText;
		CAtlString		lSpeech;
		CAtlStringArray	lTextWords;
		CAtlStringArray	lSpeechWords;
#endif		

#ifdef	__cplusplus_cli
		if	(SplitMap (pText, lText, lSpeech) > 0)
#else
		if	(SplitMap (pText, &lText, &lSpeech) > 0)
#endif		
		{
#ifdef	__cplusplus_cli
			if	(!String::IsNullOrEmpty (lSpeech))
#else
			if	(!lSpeech.IsEmpty())
#endif			
			{
				if	(
						(pOuterParse)
					&&	(mSapiVersion >= 5)
					)
				{
#ifdef	__cplusplus_cli
					lSpeech = lSpeech->Replace (_T("\\\\"), _T("\\"));
#else
					lSpeech.Replace (_T("\\\\"), _T("\\"));
#endif					
					ParseText (lSpeech, lTextWords, lSpeechWords);
					SpeechFromText (lTextWords, lSpeechWords);
#ifdef	__cplusplus_cli
					lTextWords = gcnew array<System::String^> (0);
#else					
					lTextWords.RemoveAll ();
#endif					
				}
				else
				if	(mSapiVersion >= 5)
				{
					SplitText (lSpeech, lSpeechWords);
				}
				else
				{
					ParseTags (lSpeech, lTextWords, lSpeechWords, false);
					SpeechFromText (lTextWords, lSpeechWords);
#ifdef	__cplusplus_cli
					lTextWords = gcnew array<System::String^> (0);
#else
					lTextWords.RemoveAll ();
#endif					
				}
				PadWords (lSpeechWords, mSapiVersion);
			}
#ifdef	__cplusplus_cli
			if	(!String::IsNullOrEmpty (lText))
#else
			if	(!lText.IsEmpty())
#endif			
			{
				SplitText (lText, lTextWords);
				PadWords (lTextWords);
			}

#ifdef	__cplusplus_cli
			if	(
					(lTextWords->Length > 0)
				||	(lSpeechWords->Length > 0)
				)
#else
			if	(
					(lTextWords.GetCount() > 0)
				||	(lSpeechWords.GetCount() > 0)
				)
#endif				
			{
				SpeechFromText (pTextWords, pSpeechWords);
				AppendWords (lTextWords, pTextWords);
				AppendWords (lSpeechWords, pSpeechWords, mSapiVersion);
#ifdef	__cplusplus_cli
				Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
				Array::Resize (pSpeechWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
				pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
				pSpeechWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif				
			}
		}
		else
		{
			SplitText (pText, pTextWords);
		}
	}
	else
	if	(pTag == sTagLst)
	{
#ifdef	__cplusplus_cli
		if	(
				(String::IsNullOrEmpty (pText))
			&&	(pTextWords->Length <= 0)
			&&	(pSpeechWords->Length <= 0)
			)
#else
		if	(
				(pText [0] == 0)
			&&	(pTextWords.GetCount() <= 0)
			&&	(pSpeechWords.GetCount() <= 0)
			)
#endif			
		{
			if	(mSapiVersion >= 5)
			{
#ifdef	__cplusplus_cli
				ParseText (SpeechText, pTextWords, pSpeechWords);
#else
				ParseText (GetSpeechText(), pTextWords, pSpeechWords);
#endif				
			}
			else
			{
#ifdef	__cplusplus_cli
				pTextWords = safe_cast <array<System::String^> > (mTextWords->Clone());
				pSpeechWords = safe_cast <array<System::String^> > (mSpeechWords->Clone());
#else
				pTextWords.Copy (mTextWords);
				pSpeechWords.Copy (mSpeechWords);
#endif		
			}
		}
	}
	else
	if	(pTag == sTagEmp)
	{
#ifdef	__cplusplus_cli
		if	(String::IsNullOrEmpty (pText))
#else
		if	(pText [0] == 0)
#endif		
		{
			SpeechFromText (pTextWords, pSpeechWords);
			if	(mSapiVersion >= 5)
			{
#ifdef	__cplusplus_cli
				AddWord (pSpeechWords, _T("<emph/>"));
#else
				pSpeechWords.Add (_T("<emph/>"));
#endif				
			}
			else
			{
#ifdef	__cplusplus_cli
				AddWord (pSpeechWords, sTagEmp);
#else
				pSpeechWords.Add (sTagEmp);
#endif				
			}
#ifdef	__cplusplus_cli
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
		}
	}
	else
	if	(pTag == sTagRst)
	{
#ifdef	__cplusplus_cli
		if	(String::IsNullOrEmpty (pText))
#else
		if	(pText [0] == 0)
#endif		
		{
			SpeechFromText (pTextWords, pSpeechWords);
			if	(mSapiVersion >= 5)
			{
#ifdef	__cplusplus_cli
				AddWord (pSpeechWords, _T("<volume level=\"100\"/>"));
				AddWord (pSpeechWords, _T("<rate absspeed=\"0\">"));
				AddWord (pSpeechWords, _T("<pitch absmiddle=\"0\">"));
#else
				pSpeechWords.Add (_T("<volume level=\"100\"/>"));
				pSpeechWords.Add (_T("<rate absspeed=\"0\">"));
				pSpeechWords.Add (_T("<pitch absmiddle=\"0\">"));
#endif				
			}
			else
			{
#ifdef	__cplusplus_cli
				AddWord (pSpeechWords, sTagRst);
#else
				pSpeechWords.Add (sTagRst);
#endif				
			}
#ifdef	__cplusplus_cli
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
		}
	}
	else
	if	(pTag == sTagCtx)
	{
		if	(mSapiVersion >= 5)
		{
#ifdef	_DEBUG
			LogMessage (LogDebugFast, _T("Unhandled tag [%s] [%s]"), pTag, pText);
#endif
		}
		else
		{
#ifdef	__cplusplus_cli
			lTagString = String::Format ("{0}{1}\\", gcnew String(pTag), pText);
			AddWord (pSpeechWords, lTagString);
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			lTagString.Format (_T("%s%s\\"), pTag, pText);
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif			
		}
	}
	else
	if	(pTag == sTagMrk)
	{
#ifdef	__cplusplus_cli
		if	(
				(!String::IsNullOrEmpty (pText))
			&&	(UInt32::TryParse (pText, lNumVal))
			)
#else
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
#endif			
		{
			SpeechFromText (pTextWords, pSpeechWords);
#ifdef	__cplusplus_cli
			if	(mSapiVersion >= 5)
			{
				lTagString = String::Format ("<bookmark mark=\"{0}\"/>", lNumVal);
			}
			else
			{
				lTagString = String::Format ("{0}{1}\\", gcnew String(pTag), lNumVal);
			}
			AddWord (pSpeechWords, lTagString);
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			if	(mSapiVersion >= 5)
			{
				lTagString.Format (_T("<bookmark mark=\"%u\"/>"), lNumVal);
			}
			else
			{
				lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
			}
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
		}
	}
	else
	if	(pTag == sTagPau)
	{
#ifdef	__cplusplus_cli
		if	(
				(!String::IsNullOrEmpty (pText))
			&&	(UInt32::TryParse (pText, lNumVal))
			)
#else
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
#endif		
		{
			SpeechFromText (pTextWords, pSpeechWords);
#ifdef	__cplusplus_cli
			if	(mSapiVersion >= 5)
			{
				lTagString = String::Format ("<silence msec=\"{0}\"/>", lNumVal);
			}
			else
			{
				lTagString = String::Format ("{0}{1}\\", gcnew String(pTag), lNumVal);
			}
			AddWord (pSpeechWords, lTagString);
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			if	(mSapiVersion >= 5)
			{
				lTagString.Format (_T("<silence msec=\"%u\"/>"), lNumVal);
			}
			else
			{
				lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
			}
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif			
		}
	}
	else
	if	(pTag == sTagPit)
	{
		if	(mSapiVersion >= 5)
		{
#ifdef	_DEBUG
			LogMessage (LogDebugFast, _T("Unhandled tag [%s] [%s]"), pTag, pText);
#endif
		}
		else
#ifdef	__cplusplus_cli
		if	(
				(!String::IsNullOrEmpty (pText))
			&&	(UInt32::TryParse (pText, lNumVal))
			)
#else
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
#endif		
		{
			SpeechFromText (pTextWords, pSpeechWords);
#ifdef	__cplusplus_cli
			lTagString = String::Format (_T("{0}{1}\\"), gcnew String(pTag), lNumVal);
			AddWord (pSpeechWords, lTagString);
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
		}
	}
	else
	if	(pTag == sTagSpd)
	{
		if	(mSapiVersion >= 5)
		{
#ifdef	_DEBUG
			LogMessage (LogDebugFast, _T("Unhandled tag [%s] [%s]"), pTag, pText);
#endif
		}
		else
#ifdef	__cplusplus_cli
		if	(
				(!String::IsNullOrEmpty (pText))
			&&	(UInt32::TryParse (pText, lNumVal))
			)
#else
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
#endif		
		{
			SpeechFromText (pTextWords, pSpeechWords);
#ifdef	__cplusplus_cli
			lTagString = String::Format (_T("{0}{1}\\"), gcnew String(pTag), lNumVal);
			AddWord (pSpeechWords, lTagString);
			Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
			lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
		}
	}
	else
	if	(pTag == sTagVol)
	{
#ifdef	__cplusplus_cli
		if	(!String::IsNullOrEmpty (pText))
#else
		if	(pText [0] != 0)
#endif		
		{
#ifdef	__cplusplus_cli
			if	(
					(UInt32::TryParse (pText, lNumVal))
				&&	(lNumVal <= USHRT_MAX)
				)
#else
			lNumVal = _tcstoul (pText, &lNumValEnd, 10);
			if	(
					(*lNumValEnd == 0)
				&&	(lNumVal <= USHRT_MAX)
				)
#endif		
			{
				SpeechFromText (pTextWords, pSpeechWords);
#ifdef	__cplusplus_cli
				if	(mSapiVersion >= 5)
				{
					lTagString = String::Format ("<volume level=\"{0}\"/>", Math::Min (Math::Max ((Int32)Math::Round ((Single)lNumVal * 99.0f / (Single)USHRT_MAX), 0), 99));
				}
				else
				{
					lTagString = String::Format (_T("{0}{1}\\"), gcnew String(pTag), lNumVal);
				}
				AddWord (pSpeechWords, lTagString);
				Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
				if	(mSapiVersion >= 5)
				{
					lTagString.Format (_T("<volume level=\"%d\"/>"), min (max (MulDiv (lNumVal, 99, USHRT_MAX), 0), 99));
				}
				else
				{
					lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
				}
				pSpeechWords.Add (lTagString);
				pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
			}
		}
	}
	else
	{
#ifdef	__cplusplus_cli
		SpeechFromText (pTextWords, pSpeechWords);
		if	(mSapiVersion < 5)
		{
			if	(pText [0] != 0)
			{
				lTagString = String::Format ("{0}{1}\\", gcnew String(pTag), pText);
				AddWord (pSpeechWords, lTagString);
			}
			else
			{
				AddWord (pSpeechWords, pTag);
			}
		}
		Array::Resize (pTextWords, Math::Max (pTextWords->Length, pSpeechWords->Length));
#else
#ifdef	_DEBUG_NOT
		LogMessage (LogDebugFast, _T("Other tag [%s] [%s]"), pTag, pText);
#endif
		SpeechFromText (pTextWords, pSpeechWords);
		if	(mSapiVersion < 5)
		{
			if	(pText [0] != 0)
			{
				lTagString.Format (_T("%s%s\\"), pTag, pText);
				pSpeechWords.Add (lTagString);
			}
			else
			{
				pSpeechWords.Add (pTag);
			}
		}
		pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
#endif		
	}

#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%s  PutTag [%s] [%s] Text [%s] Speech [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), pTag, DebugStr(pText), DebugStr(JoinStringArray(pTextWords, _T("]["))), DebugStr(JoinStringArray(pSpeechWords, _T("]["))), pOuterParse);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentTextParse::SplitText (System::String^ pText,array<System::String^>^% pTextWords)
#else
int CAgentTextParse::SplitText (LPCTSTR pText, CAtlStringArray& pTextWords)
#endif
{
	int		lTextNdx;
	int		lWordStart = 0;
	int		lWordEnd = -1;

#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitText [%s]"), DebugStr(pText));
#endif

#ifdef	__cplusplus_cli
	for	(lTextNdx = 0; lTextNdx < pText->Length; lTextNdx++)
#else
	for	(lTextNdx = 0; pText [lTextNdx]; lTextNdx++)
#endif	
	{
#ifdef	__cplusplus_cli
		if	(
				(
					(Char::IsControl (pText [lTextNdx]))
				||	(pText [lTextNdx] == 0x200B)
				)
			&&	(
					(lWordEnd < lWordStart)
				||	(!Char::IsWhiteSpace (pText [lWordEnd]))
				||	(Char::IsControl (pText [lWordEnd]))
				)
			)
#else
		if	(
				(
					(_istcntrl (pText [lTextNdx]))
				||	(pText [lTextNdx] == 0x200B)
				)
			&&	(
					(lWordEnd < lWordStart)
				||	(!_istspace (pText [lWordEnd]))
				||	(_istcntrl (pText [lWordEnd]))
				)
			)
#endif			
		{
			lWordEnd = lTextNdx;
			continue;
		}
		else
#ifdef	__cplusplus_cli
		if	(Char::IsWhiteSpace (pText [lTextNdx]))
#else
		if	(_istspace (pText [lTextNdx]))
#endif		
		{
			if	(lTextNdx <= lWordStart)
			{
				lWordStart = lTextNdx+1;
			}
			else
			if	(lWordEnd <= lWordStart)
			{
				lWordEnd = lTextNdx;
			}
		}

		if	(lWordEnd >= lWordStart)
		{
#ifdef	__cplusplus_cli
			AddWord (pTextWords, pText->Substring (lWordStart, lWordEnd-lWordStart+1));
#else
			pTextWords.Add (CAtlString (pText+lWordStart, lWordEnd-lWordStart+1));
#endif			
			lWordStart = lWordEnd+1;
		}
	}

	if	(lTextNdx > lWordStart)
	{
#ifdef	__cplusplus_cli
		AddWord (pTextWords, pText->Substring (lWordStart, lTextNdx-lWordStart));
#else
		pTextWords.Add (CAtlString (pText+lWordStart, lTextNdx-lWordStart));
#endif		
	}
#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitText [%s] Words [%s]"), DebugStr(CAtlString(pText,lTextNdx)), DebugStr(JoinStringArray(pTextWords,_T("]["),true)));
#endif
	return lTextNdx;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentTextParse::SplitMap (System::String^ pText, System::String^% pSpeechWords, System::String^% pTextWords)
#else
int CAgentTextParse::SplitMap (LPCTSTR pText, CAtlString* pSpeechWords, CAtlString* pTextWords)
#endif
{
	int	lRet = 0;
	int	lTextNdx;
	int	lQuoteEnd = -1;
	int	lQuoteStart = -1;
	int	lEquals = -1;

#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitMap [%s]"), DebugStr(pText));
#endif

#ifdef	__cplusplus_cli
	if	(pSpeechWords)
	{
		pSpeechWords = gcnew String("");
	}
	if	(pTextWords)
	{
		pTextWords = gcnew String("");
	}
#else
	if	(pSpeechWords)
	{
		pSpeechWords->Empty ();
	}
	if	(pTextWords)
	{
		pTextWords->Empty ();
	}
#endif	

#ifdef	__cplusplus_cli
	for	(lTextNdx = 0; lTextNdx < pText->Length; lTextNdx++)
#else
	for	(lTextNdx = 0; pText [lTextNdx]; lTextNdx++)
#endif	
	{
		if	(
				(lQuoteStart > 0)
			&&	(pText [lTextNdx] == _T('\"'))
			)
		{
			lQuoteEnd = lTextNdx;
			if	(pSpeechWords)
			{
#ifdef	__cplusplus_cli
				pSpeechWords = pText->Substring (lQuoteStart, lQuoteEnd-lQuoteStart+1);
#else
				(*pSpeechWords) = CAtlString (pText+lQuoteStart, lQuoteEnd-lQuoteStart+1);
#endif				
			}
			lRet = lTextNdx;
		}
		else
		if	(
				(lEquals < 0)
			&&	(pText [lTextNdx] == _T('\"'))
			)
		{
			lQuoteEnd = lTextNdx;
		}
		else
		if	(
				(lEquals > 0)
			&&	(pText [lTextNdx] == _T('\"'))
			)
		{
			lQuoteStart = lTextNdx;
		}
		else
		if	(
				(lQuoteEnd > 0)
			&&	(pText [lTextNdx] == _T('\\'))
			)
		{
			lRet = lTextNdx-1;
			break;
		}
		else
		if	(
				(lQuoteEnd > 0)
			&&	(lQuoteStart < 0)
			)
		{
			if	(pText [lTextNdx] == _T('='))
			{
				lEquals = lTextNdx;
				if	(pTextWords)
				{
#ifdef	__cplusplus_cli
					pTextWords = pText->Substring (0, lQuoteEnd+1);
#else
					(*pTextWords) = CAtlString (pText, lQuoteEnd+1);
#endif					
				}
			}
			else
#ifdef	__cplusplus_cli
			if	(!Char::IsWhiteSpace (pText [lTextNdx]))
#else
			if	(!_istspace (pText [lTextNdx]))
#endif			
			{
				lEquals = -1;
			}
		}
	}

	if	(pSpeechWords)
	{
#ifdef	__cplusplus_cli
		UnquoteMappedText (pSpeechWords);
#else
		UnquoteMappedText (*pSpeechWords);
#endif		
	}
	if	(pTextWords)
	{
#ifdef	__cplusplus_cli
		UnquoteMappedText (pTextWords);
#else
		UnquoteMappedText (*pTextWords);
#endif		
	}
#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitMap [%s] (%d) [%s] [%s]"), DebugStr(CAtlString(pText,lRet)), lRet, (pSpeechWords ? (LPCTSTR)(*pSpeechWords) : NULL), (pTextWords ? (LPCTSTR)(*pTextWords) : NULL));
#endif
	return lRet;
}

#ifdef	__cplusplus_cli
void CAgentTextParse::UnquoteMappedText (System::String^% pText)
#else
void CAgentTextParse::UnquoteMappedText (CAtlString& pText)
#endif
{
#ifdef	__cplusplus_cli
	if	(
			(pText->Length >= 2)
		&&	(pText [0] == _T('\"'))
		&&	(pText [pText->Length-1] == _T('\"'))
		)
	{
		pText = pText->Substring (1, pText->Length-2);
	}
	pText = pText->Replace (_T("\"\""), _T("\""));
#else
	if	(
			(pText.GetLength() >= 2)
		&&	(pText [0] == _T('\"'))
		&&	(pText [pText.GetLength()-1] == _T('\"'))
		)
	{
		pText.Delete (pText.GetLength()-1);
		pText.Delete (0);
	}
	pText.Replace (_T("\"\""), _T("\""));
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentTextParse::AppendWords (array<System::String^>^ pAppend, array<System::String^>^% pWords)
{
	AppendWords (pAppend, pWords, 0);
}
void CAgentTextParse::AppendWords (array<System::String^>^ pAppend, array<System::String^>^% pWords, UINT pSapiVersion)
#else
void CAgentTextParse::AppendWords (const CAtlStringArray& pAppend, CAtlStringArray& pWords, UINT pSapiVersion)
#endif
{
#ifdef	__cplusplus_cli
	if	(pAppend->Length > 0)
	{
		Array::Resize (pWords, pWords->Length + pAppend->Length);
		for	(int lNdx = 0; lNdx < pAppend->Length; lNdx++)
		{
			pWords[pWords->Length + lNdx] = pAppend[lNdx];
		}
		PadWords (pWords, pSapiVersion);
	}
#else
	if	(pAppend.GetCount() > 0)
	{
		pWords.Append (pAppend);
		PadWords (pWords, pSapiVersion);
	}
#endif
}

#ifdef	__cplusplus_cli
void CAgentTextParse::PadWords (array<System::String^>^% pWords)
{
	PadWords (pWords, 0);
}
void CAgentTextParse::PadWords (array<System::String^>^% pWords, UINT pSapiVersion)
#else
void CAgentTextParse::PadWords (CAtlStringArray& pWords, UINT pSapiVersion)
#endif
{
#ifdef	__cplusplus_cli
	int				lNdx;
	Boolean			lLastWordFound = false;
	array<wchar_t>^	lTrimChars = {' ', '\t', '\f'};
#else
	INT_PTR	lNdx;
	bool		lLastWordFound = false;
#endif	

#ifdef	__cplusplus_cli
	for	(lNdx = pWords->Length-1; lNdx >= 0; lNdx--)
#else
	for	(lNdx = (INT_PTR)pWords.GetCount()-1; lNdx >= 0; lNdx--)
#endif	
	{
#ifdef	__cplusplus_cli
		String^		lWord = pWords [lNdx];
#else
		CAtlString&	lWord = pWords [lNdx];
#endif		

#ifdef	__cplusplus_cli
		if	(!String::IsNullOrEmpty (lWord))
#else
		if	(!lWord.IsEmpty())
#endif		
		{
			if	(pSapiVersion != 0)
			{
#ifdef	__cplusplus_cli
				lWord = lWord->TrimEnd (lTrimChars);
#else
				lWord.TrimRight (_T(" \t\f"));
#endif				
			}
			else
			{
#ifdef	__cplusplus_cli
				lWord = lWord->TrimEnd ();
#else
				lWord.TrimRight ();
#endif				
			}
#ifdef	__cplusplus_cli
			if	(!String::IsNullOrEmpty (lWord))
#else
			if	(!lWord.IsEmpty ())
#endif			
			{
				if	(
						(pSapiVersion >= 5)
					&&	(lWord [0] == _T('<'))
#ifdef	__cplusplus_cli
					&&	(lWord [lWord->Length-1] == _T('>'))
#else
					&&	(lWord [lWord.GetLength()-1] == _T('>'))
#endif					
					)
				{
					continue;
				}
				if	(
						(pSapiVersion == 4)
					&&	(lWord [0] == _T('\\'))
#ifdef	__cplusplus_cli
					&&	(lWord [lWord->Length-1] == _T('\\'))
#else
					&&	(lWord [lWord.GetLength()-1] == _T('\\'))
#endif					
					)
				{
					continue;
				}
			}
#ifdef	__cplusplus_cli
			if	(!String::IsNullOrEmpty (lWord))
#else
			if	(!lWord.IsEmpty ())
#endif			
			{
				if	(
						(lLastWordFound)
					&&	(
							(pSapiVersion == 0)
#ifdef	__cplusplus_cli
						||	(
								(!Char::IsWhiteSpace (lWord [lWord->Length-1]))
							&&	(lWord [lWord->Length-1] != 0x200B)
							)
#else
						||	(
								(!_istspace (lWord [lWord.GetLength()-1]))
							&&	(lWord [lWord.GetLength()-1] != 0x200B)
							)
#endif							
						)
					)
				{
#ifdef	__cplusplus_cli
					lWord = lWord + _T(" ");
#else
					lWord += _T(" ");
#endif					
				}
				lLastWordFound = true;
			}
		}

#ifdef	__cplusplus_cli
		pWords [lNdx] = lWord;
#endif
	}
}

#ifdef	__cplusplus_cli
void CAgentTextParse::FinishWords (array<System::String^>^% pWords)
{
	FinishWords (pWords, 0);
}
void CAgentTextParse::FinishWords (array<System::String^>^% pWords, UINT pSapiVersion)
#else
void CAgentTextParse::FinishWords (CAtlStringArray& pWords, UINT pSapiVersion)
#endif
{
#ifdef	__cplusplus_cli
	if	(pWords->Length > 0)
#else	
	if	(pWords.GetCount() > 0)
#endif	
	{
		ReplaceAll (pWords, _T("\\\""), _T("\""));
		ReplaceAll (pWords, _T("\\\\"), _T("\\"));
		if	(pSapiVersion == 0)
		{
			ReplaceAll (pWords, _T('\r'), _T(' '));
			ReplaceAll (pWords, _T('\n'), _T(' '));
			ReplaceAll (pWords, (TCHAR)0x200B, _T(' '));
		}
		else
		{
			ReplaceAll (pWords, _T("\r\n"), _T("\r"));
			ReplaceAll (pWords, _T('\n'), _T('\r'));
		}

		PadWords (pWords, pSapiVersion);
	}
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentTextParse::SpeechFromText (array<System::String^>^ pTextWords, array<System::String^>^% pSpeechWords)
#else
void CAgentTextParse::SpeechFromText (const CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords)
#endif
{
#ifdef	__cplusplus_cli
	while	(pSpeechWords->Length < pTextWords->Length)
	{
		AddWord (pSpeechWords, pTextWords [pSpeechWords->Length]);
	}
#else
	while	(pSpeechWords.GetCount() < pTextWords.GetCount())
	{
		pSpeechWords.Add (pTextWords [pSpeechWords.GetCount()]);
	}
#endif
}

#ifdef	__cplusplus_cli
void CAgentTextParse::FinishSpeech (array<System::String^>^% pSpeechWords)
#else
void CAgentTextParse::FinishSpeech (CAtlStringArray& pSpeechWords)
#endif
{
	if	(mSapiVersion >= 5)
	{
#ifdef	__cplusplus_cli
		for	(int lNdx = pSpeechWords->Length-1; lNdx >= 0; lNdx--)
#else
		for	(INT_PTR lNdx = (INT_PTR)pSpeechWords.GetCount()-1; lNdx >= 0; lNdx--)
#endif		
		{
#ifdef	__cplusplus_cli
			if	(pSpeechWords [lNdx]->Equals (_T("<emph/>")))
			{
				if	(lNdx < pSpeechWords->Length-1)
				{
					pSpeechWords[lNdx] = gcnew String (_T("<emph>"));
					InsertWord (pSpeechWords, lNdx+2, _T("</emph>"));
				}
				else
				{
					RemoveWord (pSpeechWords, lNdx);
				}
			}
#else
			if	(pSpeechWords [lNdx].Compare (_T("<emph/>")) == 0)
			{
				if	(lNdx < (INT_PTR)pSpeechWords.GetCount()-1)
				{
					pSpeechWords.SetAt (lNdx, _T("<emph>"));
					pSpeechWords.InsertAt (lNdx+2, _T("</emph>"));
				}
				else
				{
					pSpeechWords.RemoveAt (lNdx);
				}
			}
#endif			
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentTextParse::ParseText (System::String^ pText, array<System::String^>^% pTextWords, array<System::String^>^% pSpeechWords)
{
	ParseTags (pText, pTextWords, pSpeechWords, false);
}
#else
static LPCTSTR	sSaxElementName = _T("sapi");
static LPCTSTR	sSaxVolumeName = _T("volume");
static LPCTSTR	sSaxRateName = _T("rate");
static LPCTSTR	sSaxSpeedName = _T("speed");
static LPCTSTR	sSaxPitchName = _T("pitch");
static LPCTSTR	sSaxContextName = _T("context");
static LPCTSTR	sSaxEmphName = _T("emph");

/////////////////////////////////////////////////////////////////////////////

void CAgentTextParse::ParseText (LPCTSTR pText, CAtlStringArray& pTextWords, CAtlStringArray& pSpeechWords)
{
	CAtlStringArray		lTextWords;
	CAtlStringArray		lSpeechWords;
	ISAXXMLReaderPtr	lSaveReader;
	ISAXLocatorPtr		lSaveLocator;
	CAtlStringArray		lSaveElements;
	CAtlStringArray		lSaveTextWords;
	CAtlStringArray		lSaveSpeechWords;
	static CAtlString	lXmlHeader (_T("<?xml version=\"1.0\" encoding=\"UTF-16\" ?>"));
	static CAtlString	lXmlStart (_T("<") + CAtlString(sSaxElementName) + _T(">"));
	static CAtlString	lXmlEnd (_T("</") + CAtlString(sSaxElementName) + _T(">"));

	lSaveReader.Attach (mSaxReader.Detach());
	lSaveLocator.Attach (mSaxLocator.Detach());
	lSaveElements.Copy (mSaxElements);
	lSaveTextWords.Copy (mSaxTextWords);
	lSaveSpeechWords.Copy (mSaxSpeechWords);

	m_dwRef = max (m_dwRef, 1);

	try
	{
		HRESULT		lResult;
		CAtlString	lText (pText);
		_variant_t	lXmlText;

		mSaxElements.RemoveAll ();
		mSaxTextWords.RemoveAll ();
		mSaxSpeechWords.RemoveAll ();

		lText.Replace (_T("&"), _T("$amp;"));
		lXmlText = lXmlHeader + lXmlStart + lText + lXmlEnd;

		if	(
				(SUCCEEDED (lResult = CoCreateInstance (__uuidof(SAXXMLReader30), NULL, CLSCTX_SERVER, __uuidof(ISAXXMLReader), (void**)&mSaxReader)))
			&&	(SUCCEEDED (lResult = mSaxReader->putContentHandler (this)))
			&&	(SUCCEEDED (lResult = mSaxReader->putErrorHandler (this)))
			)
		{
#ifdef	_DEBUG_TEXT
			LogMessage (_DEBUG_TEXT, _T("Parse  [%ls]"), DebugStr(CAtlString (V_BSTR(&lXmlText))));
#endif
			lResult = mSaxReader->parse (lXmlText);
			if	(
					(FAILED (lResult))
				&&	(LogIsActive ())
				)
			{
				LogComErr (LogIfActive|LogTime, lResult, _T("[%s]"),  DebugStr(CAtlString (V_BSTR(&lXmlText))));
			}
		}
		else
		if	(LogIsActive ())
		{
			LogComErr (LogNormal|LogTime, lResult);
		}

		// Use local copy to allow recursion
		if	(SUCCEEDED (lResult))
		{
			ReplaceAll (mSaxTextWords, _T("$amp;"), _T("&"));
			ReplaceAll (mSaxSpeechWords, _T("$amp;"), _T("&"));
			lTextWords.Copy (mSaxTextWords);
			lSpeechWords.Copy (mSaxSpeechWords);
		}
		else
		{
			SplitText (pText, lTextWords);
			lSpeechWords.Copy (lTextWords);
		}

		SafeFreeSafePtr (mSaxLocator);
		SafeFreeSafePtr (mSaxReader);
	}
	catch AnyExceptionDebug

	mSaxReader.Attach (lSaveReader.Detach());
	mSaxLocator.Attach (lSaveLocator.Detach());
	mSaxElements.Copy (lSaveElements);
	mSaxTextWords.Copy (lSaveTextWords);
	mSaxSpeechWords.Copy (lSaveSpeechWords);

	AppendWords (lTextWords, pTextWords);
	AppendWords (lSpeechWords, pSpeechWords, mSapiVersion);
#ifdef	_DEBUG_TEXT
	LogMessage (_DEBUG_TEXT, _T("Parsed [%s] Text [%s] Speech [%s]"), DebugStr(pText), DebugStr(JoinStringArray(pTextWords, _T("]["))), DebugStr(JoinStringArray(pSpeechWords, _T("]["))));
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentTextParse::putDocumentLocator (ISAXLocator *pLocator)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::putDocumentLocator [%p]"), this, max(m_dwRef,-1), pLocator);
#endif
	mSaxLocator = pLocator;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::startDocument ()
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::startDocument"), this, max(m_dwRef,-1));
#endif
	mSaxElements.RemoveAll ();
	mSaxTextWords.RemoveAll ();
	mSaxSpeechWords.RemoveAll ();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::endDocument (void)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::endDocument"), this, max(m_dwRef,-1));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::startPrefixMapping (const wchar_t *pwchPrefix, int cchPrefix, const wchar_t *pwchUri, int cchUri)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::startPrefixMapping [%s]"), this, max(m_dwRef,-1), CAtlString(pwchPrefix,cchPrefix));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::endPrefixMapping (const wchar_t *pwchPrefix, int cchPrefix)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::endPrefixMapping [%s]"), this, max(m_dwRef,-1), CAtlString(pwchPrefix,cchPrefix));
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentTextParse::startElement (const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName, ISAXAttributes *pAttributes)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::startElement [%s] [%s]"), this, max(m_dwRef,-1), CAtlString(pwchLocalName,cchLocalName), CAtlString(pwchQName,cchQName));
	if	(pAttributes)
	{
		int	lAttrCount = -1;
		int	lAttrNdx;

		pAttributes->getLength (&lAttrCount);
		LogMessage (_DEBUG_SAX, _T("  Attributes [%d]"), lAttrCount);
		if	(lAttrCount > 0)
		{
			for	(lAttrNdx = 0; lAttrNdx < lAttrCount; lAttrNdx++)
			{
				const wchar_t *	lAttrName = NULL;
				int				lAttrNameLen = 0;
				const wchar_t *	lAttrVal = NULL;
				int				lAttrValLen = 0;

				pAttributes->getLocalName (lAttrNdx, &lAttrName, &lAttrNameLen);
				pAttributes->getValue (lAttrNdx, &lAttrVal, &lAttrValLen);
				LogMessage (_DEBUG_SAX, _T("    [%s] [%s]"), CAtlString(lAttrName,lAttrNameLen), CAtlString(lAttrVal,lAttrValLen));
			}
		}
	}
#endif
	mSaxElements.SetCount (mSaxElements.GetCount() + 1);
	if	(mSaxElements.GetCount() > 1)
	{
		CAtlString	lElement;
		int			lAttrCount = -1;
		int			lAttrNdx;

		lElement.Format (_T("<%s"), CAtlString(pwchLocalName,cchLocalName));

		if	(
				(pAttributes)
			&&	(SUCCEEDED (pAttributes->getLength (&lAttrCount)))
			&&	(lAttrCount > 0)
			)
		{
			for	(lAttrNdx = 0; lAttrNdx < lAttrCount; lAttrNdx++)
			{
				const wchar_t *	lAttrName = NULL;
				int				lAttrNameLen = 0;
				const wchar_t *	lAttrVal = NULL;
				int				lAttrValLen = 0;

				if	(
						(SUCCEEDED (pAttributes->getLocalName (lAttrNdx, &lAttrName, &lAttrNameLen)))
					&&	(SUCCEEDED (pAttributes->getValue (lAttrNdx, &lAttrVal, &lAttrValLen)))
					)
				{
					lElement.Format (_T("%s %s=\"%s\""), CAtlString((LPCTSTR)lElement), CAtlString (lAttrName,lAttrNameLen), CAtlString(lAttrVal,lAttrValLen));
				}
			}
		}

		mSaxElements [(INT_PTR)mSaxElements.GetCount()-1] = lElement;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::endElement (const wchar_t *pwchNamespaceUri, int cchNamespaceUri, const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::endElement [%s] [%s]"), this, max(m_dwRef,-1), CAtlString(pwchLocalName,cchLocalName), CAtlString(pwchQName,cchQName));
#endif
	if	(mSaxElements.GetCount() > 1)
	{
		if	(mSaxElements [(INT_PTR)mSaxElements.GetCount()-1].IsEmpty())
		{
			mSaxElements [(INT_PTR)mSaxElements.GetCount()-1].Format (_T("</%s>"), CAtlString(pwchLocalName,cchLocalName));
		}
		else
		{
			mSaxElements [(INT_PTR)mSaxElements.GetCount()-1] += _T("/>");
		}
		SpeechFromText (mSaxTextWords, mSaxSpeechWords);
		mSaxSpeechWords.Add (mSaxElements [(INT_PTR)mSaxElements.GetCount()-1]);
		mSaxTextWords.SetCount (max (mSaxTextWords.GetCount(), mSaxSpeechWords.GetCount()));
#ifdef	_DEBUG_SAX
		LogMessage (_DEBUG_SAX, _T("  PutElement [%s]"), mSaxElements [(INT_PTR)mSaxElements.GetCount()-1]);
#endif
	}
	mSaxElements.SetCount (max (mSaxElements.GetCount() - 1, 0));

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentTextParse::characters (const wchar_t *pwchChars, int cchChars)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::characters [%s]"), this, max(m_dwRef,-1), DebugStr(CAtlString(pwchChars,cchChars)));
#endif
	if	(
			(mSaxElements.GetCount() > 1)
		&&	(!mSaxElements [(INT_PTR)mSaxElements.GetCount()-1].IsEmpty())
		)
	{
		mSaxElements [(INT_PTR)mSaxElements.GetCount()-1] += _T(">");
		SpeechFromText (mSaxTextWords, mSaxSpeechWords);
		mSaxSpeechWords.Add (mSaxElements [(INT_PTR)mSaxElements.GetCount()-1]);
		mSaxTextWords.SetCount (max (mSaxTextWords.GetCount(), mSaxSpeechWords.GetCount()));
#ifdef	_DEBUG_SAX
		LogMessage (_DEBUG_SAX, _T("  PutElement [%s]"), mSaxElements [(INT_PTR)mSaxElements.GetCount()-1]);
#endif
		mSaxElements [(INT_PTR)mSaxElements.GetCount()-1].Empty();
	}

	ParseTags (CAtlString (pwchChars, cchChars), mSaxTextWords, mSaxSpeechWords, false);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentTextParse::ignorableWhitespace (const wchar_t *pwchChars, int cchChars)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::ignorableWhitespace [%s]"), this, max(m_dwRef,-1), DebugStr(CAtlString(pwchChars,cchChars)));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::processingInstruction (const wchar_t *pwchTarget, int cchTarget, const wchar_t *pwchData, int cchData)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::processingInstruction"), this, max(m_dwRef,-1));
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::skippedEntity (const wchar_t *pwchName, int cchName)
{
#ifdef	_DEBUG_SAX
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxContentHandler::skippedEntity [%s]"), this, max(m_dwRef,-1), CAtlString(pwchName,cchName));
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentTextParse::error (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode)
{
#ifdef	_DEBUG_SAX
	int	lLine = -1;
	int	lColumn = -1;
	if	(mSaxLocator != NULL)
	{
		mSaxLocator->getLineNumber (&lLine);
		mSaxLocator->getColumnNumber (&lColumn);
	}
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxErrorHandler::error [%8.8X] [%ls] at [%d %d]"), this, max(m_dwRef,-1), hrErrorCode, DebugStr(pwchErrorMessage), lLine, lColumn);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::fatalError (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode)
{
#ifdef	_DEBUG_SAX
	int	lLine = -1;
	int	lColumn = -1;
	if	(mSaxLocator != NULL)
	{
		mSaxLocator->getLineNumber (&lLine);
		mSaxLocator->getColumnNumber (&lColumn);
	}
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxErrorHandler::fatalError [%8.8X] [%ls] at [%d %d]"), this, max(m_dwRef,-1), hrErrorCode, DebugStr(pwchErrorMessage), lLine, lColumn);
#endif
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAgentTextParse::ignorableWarning (ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode)
{
#ifdef	_DEBUG_SAX
	int	lLine = -1;
	int	lColumn = -1;
	if	(mSaxLocator != NULL)
	{
		mSaxLocator->getLineNumber (&lLine);
		mSaxLocator->getColumnNumber (&lColumn);
	}
	LogMessage (_DEBUG_SAX, _T("[%p(%d)] SaxErrorHandler::ignorableWarning [%8.8X] [%ls] at [%d %d]"), this, max(m_dwRef,-1), hrErrorCode, DebugStr(pwchErrorMessage), lLine, lColumn);
#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
