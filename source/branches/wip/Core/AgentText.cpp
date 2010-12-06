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
#include "StdAfx.h"
#include "DaCore.h"
#include "AgentText.h"
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
//#define	_DEBUG_CACHE	LogDebugFast
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentText::CAgentText (UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
}

CAgentText::CAgentText (const CAtlStringArray & pWords, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
	operator= (pWords);
}

CAgentText::CAgentText (const CAgentText & pText, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
	operator= (pText);
}

CAgentText::~CAgentText ()
{
}

/////////////////////////////////////////////////////////////////////////////

CAgentText & CAgentText::operator= (const CAtlStringArray & pWords)
{
	mTextWords.RemoveAll ();
	operator+= (pWords);
	return *this;
}

CAgentText & CAgentText::operator+= (const CAtlStringArray & pWords)
{
	Append (pWords, pWords);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentText & CAgentText::operator= (const CAgentText & pText)
{
	mSapiVersion = pText.mSapiVersion;
	mText = pText.mText;
	mTextWords.Copy (pText.mTextWords);
	mSpeechWords.Copy (pText.mSpeechWords);
	return *this;
}

CAgentText & CAgentText::operator+= (const CAgentText & pText)
{
	Append (pText.mTextWords, pText.mSpeechWords);
	return *this;
}

void CAgentText::Append (const CAgentText & pText, bool pAppendSpeech)
{
	Append (pText.mTextWords, pText.mSpeechWords, pAppendSpeech);
}

void CAgentText::Append (const CAtlStringArray & pTextWords, const CAtlStringArray & pSpeechWords, bool pAppendSpeech)
{
	if	(
			(mTextWords.GetCount() > 0)
		&&	(pTextWords.GetCount() > 0)
		)
	{
		mTextWords [(INT_PTR)mTextWords.GetCount()-1].TrimRight();
		mTextWords.Add (_T("\n"));
		mTextWords.Append (pTextWords);
	}
	else
	{
		mTextWords.Copy (pTextWords);
	}

	mText = JoinStringArray (mTextWords, _T(""));

	if	(pSpeechWords.GetCount() > 0)
	{
		if	(
				(pAppendSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mSpeechWords.Add (_T("\n"));
			mSpeechWords.Append (pSpeechWords);
		}
		else
		{
			mSpeechWords.Copy (pSpeechWords);
		}
	}
	else
	if	(pTextWords.GetCount() > 0)
	{
		if	(
				(pAppendSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mSpeechWords.Add (_T("\n"));
			mSpeechWords.Append (pTextWords);
		}
		else
		{
			mSpeechWords.Copy (pTextWords);
		}
	}

#ifdef	_DEBUG_TEXT
	if	(!mText.IsEmpty())
	{
		LogMessage (_DEBUG_TEXT, _T("Text   [%s]"), DebugStr(mText));
	}
	if	(mTextWords.GetCount() > 0)
	{
		LogMessage (_DEBUG_TEXT, _T("Words  [%s]"), DebugStr(JoinStringArray(mTextWords, _T(""), true)));
	}
	if	(mSpeechWords.GetCount() > 0)
	{
		LogMessage (_DEBUG_TEXT, _T("Speech [%s]"), DebugStr(JoinStringArray(mSpeechWords, _T(""), true)));
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

UINT CAgentText::GetSapiVersion () const
{
	return mSapiVersion;
}

bool CAgentText::SetSapiVersion (UINT pSapiVersion)
{
	if	(pSapiVersion == 0)
	{
		pSapiVersion = 5;
	}
	if	(
			(pSapiVersion == 4)
		||	(pSapiVersion == 5)
		)
	{
		mSapiVersion = pSapiVersion;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentText::GetSpeechText () const
{
	if	(mSpeechWords.GetCount() > 0)
	{
		return JoinStringArray (mSpeechWords, _T(""));
	}
	else
	{
		return mText;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentTextParse::CAgentTextParse (LPCTSTR pText, UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	operator= (pText);
}

CAgentTextParse::CAgentTextParse (const CAtlStringArray & pWords, UINT pSapiVersion)
:	CAgentText (pWords, pSapiVersion)
{
}

CAgentTextParse::CAgentTextParse (const CAgentText & pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
}

CAgentTextParse::~CAgentTextParse ()
{
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextParse & CAgentTextParse::operator= (LPCTSTR pText)
{
	mTextWords.RemoveAll ();
	operator+= (pText);
	return *this;
}

CAgentTextParse & CAgentTextParse::operator+= (LPCTSTR pText)
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

CAgentTextParse & CAgentTextParse::operator= (const CAtlStringArray & pWords)
{
	CAgentText::operator= (pWords);
	return *this;
}

CAgentTextParse & CAgentTextParse::operator+= (const CAtlStringArray & pWords)
{
	CAgentText::operator+= (pWords);
	return *this;
}

CAgentTextParse & CAgentTextParse::operator= (const CAgentText & pText)
{
	CAgentText::operator= (pText);
	return *this;
}

CAgentTextParse & CAgentTextParse::operator+= (const CAgentText & pText)
{
	CAgentText::operator+= (pText);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////

static LPCTSTR MatchTag (LPCTSTR pText)
{
	static CAtlTypeArray <LPCTSTR>	lTags;
	INT_PTR							lNdx;

	if	(lTags.GetCount() <= 0)
	{
		lTags.Add (sTagChr);
		lTags.Add (sTagCom);
		lTags.Add (sTagCtx);
		lTags.Add (sTagDem);
		lTags.Add (sTagEmp);
		lTags.Add (sTagEng);
		lTags.Add (sTagLst);
		lTags.Add (sTagMap);
		lTags.Add (sTagMrk);
		lTags.Add (sTagPau);
		lTags.Add (sTagPit);
		lTags.Add (sTagPra);
		lTags.Add (sTagPrn);
		lTags.Add (sTagPro);
		lTags.Add (sTagPrt);
		lTags.Add (sTagRmS);
		lTags.Add (sTagRmW);
		lTags.Add (sTagRPit);
		lTags.Add (sTagRPrn);
		lTags.Add (sTagRSpd);
		lTags.Add (sTagRst);
		lTags.Add (sTagSpd);
		lTags.Add (sTagVol);
	}

	for	(lNdx = 0; lNdx < (INT_PTR)lTags.GetCount(); lNdx++)
	{
		if	(_tcsnicmp (pText, lTags [lNdx], _tcslen (lTags [lNdx])) == 0)
		{
			return lTags [lNdx];
		}
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextParse::ParseTags (LPCTSTR pText, CAtlStringArray & pTextWords, CAtlStringArray & pSpeechWords, bool pOuterParse)
{
	int		lTextNdx;
	int		lTagStart = -1;
	int		lTagEnd = -1;
	LPCTSTR	lTag = NULL;

#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%sParseTags [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), DebugStr(pText), pOuterParse);
#endif

	for	(lTextNdx = 0; pText [lTextNdx]; lTextNdx++)
	{
		if	(pText [lTextNdx] == _T('\\'))
		{
			if	(lTagStart >= 0)
			{
#ifdef	_DEBUG_TAGS
				LogMessage (_DEBUG_TAGS, _T("%s  Tag [%s] end at [%d] [%s]"), (pOuterParse?_T(""):_T("  ")), lTag, lTextNdx, DebugStr(pText+lTextNdx));
#endif
				PutTag (lTag, CAtlString (pText+lTagStart, lTextNdx-lTagStart), pTextWords, pSpeechWords, pOuterParse);
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
						ParseText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords, pSpeechWords);
					}
					else
					{
						SplitText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords);
					}
				}

				if	(lTag = MatchTag (pText+lTextNdx))
				{
#ifdef	_DEBUG_TAGS
					LogMessage (_DEBUG_TAGS, _T("%s  Tag [%s] at [%d]"), (pOuterParse?_T(""):_T("  ")), lTag, lTextNdx);
#endif
					if	(lTag == sTagMap)
					{
						lTextNdx += (int)_tcslen (lTag);
						lTagStart = lTextNdx;
						lTextNdx += SplitMap (pText+lTagStart, NULL, NULL);
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
			ParseText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords, pSpeechWords);
		}
		else
		{
			SplitText (CAtlString (pText+lTagEnd+1, lTextNdx-lTagEnd-1), pTextWords);
		}
	}

	if	(pSpeechWords.GetCount() > 0)
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

void CAgentTextParse::PutTag (LPCTSTR pTag, LPCTSTR pText, CAtlStringArray & pTextWords, CAtlStringArray & pSpeechWords, bool pOuterParse)
{
	ULONG		lNumVal;
	LPTSTR		lNumValEnd;
	CAtlString	lTagString;

#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%s  PutTag [%s] [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), pTag, DebugStr(pText), pOuterParse);
#endif
	if	(pTag == sTagMap)
	{
		CAtlString		lText;
		CAtlString		lSpeech;
		CAtlStringArray	lTextWords;
		CAtlStringArray	lSpeechWords;

		if	(SplitMap (pText, &lText, &lSpeech) > 0)
		{
			if	(!lSpeech.IsEmpty())
			{
				if	(
						(pOuterParse)
					&&	(mSapiVersion >= 5)
					)
				{
					lSpeech.Replace (_T("\\\\"), _T("\\"));
					ParseText (lSpeech, lTextWords, lSpeechWords);
					SpeechFromText (lTextWords, lSpeechWords);
					lTextWords.RemoveAll ();
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
					lTextWords.RemoveAll ();
				}
				PadWords (lSpeechWords, mSapiVersion);
			}
			if	(!lText.IsEmpty())
			{
				SplitText (lText, lTextWords);
				PadWords (lTextWords);
			}

			if	(
					(lTextWords.GetCount() > 0)
				||	(lSpeechWords.GetCount() > 0)
				)
			{
				SpeechFromText (pTextWords, pSpeechWords);
				AppendWords (lTextWords, pTextWords);
				AppendWords (lSpeechWords, pSpeechWords, mSapiVersion);
				pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
				pSpeechWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
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
		if	(
				(pText [0] == 0)
			&&	(pTextWords.GetCount() <= 0)
			&&	(pSpeechWords.GetCount() <= 0)
			)
		{
			if	(mSapiVersion >= 5)
			{
				ParseText (GetSpeechText(), pTextWords, pSpeechWords);
			}
			else
			{
				pTextWords.Copy (mTextWords);
				pSpeechWords.Copy (mSpeechWords);
			}
		}
	}
	else
	if	(pTag == sTagEmp)
	{
		if	(pText [0] == 0)
		{
			SpeechFromText (pTextWords, pSpeechWords);
			if	(mSapiVersion >= 5)
			{
				pSpeechWords.Add (_T("<emph/>"));
			}
			else
			{
				pSpeechWords.Add (sTagEmp);
			}
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
		}
	}
	else
	if	(pTag == sTagRst)
	{
		if	(pText [0] == 0)
		{
			SpeechFromText (pTextWords, pSpeechWords);
			if	(mSapiVersion >= 5)
			{
				pSpeechWords.Add (_T("<volume level=\"100\"/>"));
				pSpeechWords.Add (_T("<rate absspeed=\"0\">"));
				pSpeechWords.Add (_T("<pitch absmiddle=\"0\">"));
			}
			else
			{
				pSpeechWords.Add (sTagRst);
			}
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
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
			lTagString.Format (_T("%s%s\\"), pTag, pText);
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
		}
	}
	else
	if	(pTag == sTagMrk)
	{
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
		{
			SpeechFromText (pTextWords, pSpeechWords);
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
		}
	}
	else
	if	(pTag == sTagPau)
	{
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
		{
			SpeechFromText (pTextWords, pSpeechWords);
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
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
		{
			SpeechFromText (pTextWords, pSpeechWords);
			lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
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
		if	(
				(pText [0] != 0)
			&&	(lNumVal = _tcstoul (pText, &lNumValEnd, 10))
			&&	(*lNumValEnd == 0)
			)
		{
			SpeechFromText (pTextWords, pSpeechWords);
			lTagString.Format (_T("%s%u\\"), pTag, lNumVal);
			pSpeechWords.Add (lTagString);
			pTextWords.SetCount (max (pTextWords.GetCount(), pSpeechWords.GetCount()));
		}
	}
	else
	if	(pTag == sTagVol)
	{
		if	(pText [0] != 0)
		{
			lNumVal = _tcstoul (pText, &lNumValEnd, 10);
			if	(
					(*lNumValEnd == 0)
				&&	(lNumVal <= USHRT_MAX)
				)
			{
				SpeechFromText (pTextWords, pSpeechWords);
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
			}
		}
	}
	else
	{
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
	}

#ifdef	_DEBUG_TAGS
	LogMessage (_DEBUG_TAGS, _T("%s  PutTag [%s] [%s] Text [%s] Speech [%s] (Outer %u)"), (pOuterParse?_T(""):_T("  ")), pTag, DebugStr(pText), DebugStr(JoinStringArray(pTextWords, _T("]["))), DebugStr(JoinStringArray(pSpeechWords, _T("]["))), pOuterParse);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CAgentTextParse::SplitText (LPCTSTR pText, CAtlStringArray & pTextWords)
{
	int		lTextNdx;
	int		lWordStart = 0;
	int		lWordEnd = -1;

#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitText [%s]"), DebugStr(pText));
#endif

	for	(lTextNdx = 0; pText [lTextNdx]; lTextNdx++)
	{
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
		{
			lWordEnd = lTextNdx;
			continue;
		}
		else
		if	(_istspace (pText [lTextNdx]))
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
			pTextWords.Add (CAtlString (pText+lWordStart, lWordEnd-lWordStart+1));
			lWordStart = lWordEnd+1;
		}
	}

	if	(lTextNdx > lWordStart)
	{
		pTextWords.Add (CAtlString (pText+lWordStart, lTextNdx-lWordStart));
	}
#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitText [%s] Words [%s]"), DebugStr(CAtlString(pText,lTextNdx)), DebugStr(JoinStringArray(pTextWords,_T("]["),true)));
#endif
	return lTextNdx;
}

/////////////////////////////////////////////////////////////////////////////

int CAgentTextParse::SplitMap (LPCTSTR pText, CAtlString * pSpeechWords, CAtlString * pTextWords)
{
	int	lRet = 0;
	int	lTextNdx;
	int	lQuoteEnd = -1;
	int	lQuoteStart = -1;
	int	lEquals = -1;

#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitMap [%s]"), DebugStr(pText));
#endif

	if	(pSpeechWords)
	{
		pSpeechWords->Empty ();
	}
	if	(pTextWords)
	{
		pTextWords->Empty ();
	}

	for	(lTextNdx = 0; pText [lTextNdx]; lTextNdx++)
	{
		if	(
				(lQuoteStart > 0)
			&&	(pText [lTextNdx] == _T('\"'))
			)
		{
			lQuoteEnd = lTextNdx;
			if	(pSpeechWords)
			{
				(*pSpeechWords) = CAtlString (pText+lQuoteStart, lQuoteEnd-lQuoteStart+1);
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
					(*pTextWords) = CAtlString (pText, lQuoteEnd+1);
				}
			}
			else
			if	(!_istspace (pText [lTextNdx]))
			{
				lEquals = -1;
			}
		}
	}

	if	(pSpeechWords)
	{
		UnquoteMappedText (*pSpeechWords);
	}
	if	(pTextWords)
	{
		UnquoteMappedText (*pTextWords);
	}
#ifdef	_DEBUG_SPLIT
	LogMessage (_DEBUG_SPLIT, _T("  SplitMap [%s] (%d) [%s] [%s]"), DebugStr(CAtlString(pText,lRet)), lRet, (pSpeechWords ? (LPCTSTR)(*pSpeechWords) : NULL), (pTextWords ? (LPCTSTR)(*pTextWords) : NULL));
#endif
	return lRet;
}

void CAgentTextParse::UnquoteMappedText (CAtlString & pText)
{
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
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextParse::AppendWords (const CAtlStringArray & pAppend, CAtlStringArray & pWords, UINT pSapiVersion)
{
	if	(pAppend.GetCount() > 0)
	{
		pWords.Append (pAppend);
		PadWords (pWords, pSapiVersion);
	}
}

void CAgentTextParse::PadWords (CAtlStringArray & pWords, UINT pSapiVersion)
{
	INT_PTR	lNdx;
	bool	lLastWordFound = false;

	for	(lNdx = (INT_PTR)pWords.GetCount()-1; lNdx >= 0; lNdx--)
	{
		CAtlString &	lWord = pWords [lNdx];

		if	(!lWord.IsEmpty())
		{
			if	(pSapiVersion != 0)
			{
				lWord.TrimRight (_T(" \t\f"));
			}
			else
			{
				lWord.TrimRight ();
			}
			if	(!lWord.IsEmpty ())
			{
				if	(
						(pSapiVersion >= 5)
					&&	(lWord [0] == _T('<'))
					&&	(lWord [lWord.GetLength()-1] == _T('>'))
					)
				{
					continue;
				}
				if	(
						(pSapiVersion == 4)
					&&	(lWord [0] == _T('\\'))
					&&	(lWord [lWord.GetLength()-1] == _T('\\'))
					)
				{
					continue;
				}
			}
			if	(!lWord.IsEmpty ())
			{
				if	(
						(lLastWordFound)
					&&	(
							(pSapiVersion == 0)
						||	(
								(!_istspace (lWord [lWord.GetLength()-1]))
							&&	(lWord [lWord.GetLength()-1] != 0x200B)
							)
						)
					)
				{
					lWord += _T(" ");
				}
				lLastWordFound = true;
			}
		}
	}
}

void CAgentTextParse::FinishWords (CAtlStringArray & pWords, UINT pSapiVersion)
{
	if	(pWords.GetCount() > 0)
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

void CAgentTextParse::SpeechFromText (const CAtlStringArray & pTextWords, CAtlStringArray & pSpeechWords)
{
	while	(pSpeechWords.GetCount() < pTextWords.GetCount())
	{
		pSpeechWords.Add (pTextWords [pSpeechWords.GetCount()]);
	}
}

void CAgentTextParse::FinishSpeech (CAtlStringArray & pSpeechWords)
{
	if	(mSapiVersion >= 5)
	{
		INT_PTR	lNdx;

		for	(lNdx = (INT_PTR)pSpeechWords.GetCount()-1; lNdx >= 0; lNdx--)
		{
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
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static LPCTSTR	sSaxElementName = _T("sapi");
static LPCTSTR	sSaxVolumeName = _T("volume");
static LPCTSTR	sSaxRateName = _T("rate");
static LPCTSTR	sSaxSpeedName = _T("speed");
static LPCTSTR	sSaxPitchName = _T("pitch");
static LPCTSTR	sSaxContextName = _T("context");
static LPCTSTR	sSaxEmphName = _T("emph");

/////////////////////////////////////////////////////////////////////////////

void CAgentTextParse::ParseText (LPCTSTR pText, CAtlStringArray & pTextWords, CAtlStringArray & pSpeechWords)
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw::CAgentTextDraw (UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::CAgentTextDraw (const CAgentText & pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::CAgentTextDraw (const CAgentTextDraw & pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::~CAgentTextDraw ()
{
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw & CAgentTextDraw::operator= (const CAgentText & pText)
{
	CAgentText::operator= (pText);
	ResetState (true);
	DisplayFirstWord ();
	return *this;
}

CAgentTextDraw & CAgentTextDraw::operator+= (const CAgentText & pText)
{
	CAgentText::operator+= (pText);
	ResetState (false);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw & CAgentTextDraw::operator= (const CAgentTextDraw & pText)
{
	CTextWrap::operator= (pText);
	CAgentText::operator= (pText);
	ResetState (true);

	mWordDisplayed = pText.mWordDisplayed;
	mScrollPos = pText.mScrollPos;
	mScrollInc = pText.mScrollInc;
	mScrollMin = pText.mScrollMin;
	mScrollMax = pText.mScrollMax;
	return *this;
}

CAgentTextDraw & CAgentTextDraw::operator+= (const CAgentTextDraw & pText)
{
	CAgentText::operator+= (pText);
	ResetState (false);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextDraw::ResetState (bool pFullReset)
{
	mBounds.SetRectEmpty ();

	mTextCacheLimit = 5;
	mTextCacheStart = 0;
	mTextCache.DeleteAll ();

	if	(pFullReset)
	{
		mWordDisplayed = -1;
		mScrollPos = LONG_MIN;
		mScrollInc = 0;
		mScrollMin = 0;
		mScrollMax = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAtlString CAgentTextDraw::GetDisplayText (INT_PTR pLookAhead) const
{
	CAtlString	lDisplayText;
//
//	The caching is primitive but does the job.  It assumes that display text will be accessed
//	with constantly growing length.
//
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed	< (INT_PTR)mTextWords.GetCount())
		)
	{
		if	(mWordDisplayed	>= 0)
		{
			INT_PTR			lWordDisplayed = min (mWordDisplayed + pLookAhead, (INT_PTR)mTextWords.GetCount()-1);
			INT_PTR			lTextCacheNdx;
			CAtlString *	lTextCache;

			while	(
						(mTextCacheStart < mWordDisplayed)
					&&	(mTextCache.GetCount() > 0)
					)
			{
#ifdef	_DEBUG_CACHE
				LogMessage (_DEBUG_CACHE, _T("  DeleteCached [%p] CacheStart [%d] CacheSize [%d]"), mTextCache (0), mTextCacheStart+1, mTextCache.GetCount()-1);
#endif
				mTextCache.DeleteAt (0);
				mTextCacheStart++;
			}

			mTextCacheStart = max (mTextCacheStart, mWordDisplayed);
			lTextCacheNdx = lWordDisplayed - mTextCacheStart;
#ifdef	_DEBUG_CACHE
			LogMessage (_DEBUG_CACHE, _T("GetDisplayText [%d] CacheStart [%d] CacheNdx [%d] CacheSize [%d]"), lWordDisplayed, mTextCacheStart, lTextCacheNdx, mTextCache.GetCount());
#endif

			if	(
					(lTextCacheNdx >= 0)
				&&	(lTextCacheNdx < mTextCacheLimit)
				)
			{
				if	(lTextCache = mTextCache (lTextCacheNdx))
				{
#ifdef	_DEBUG_CACHE
					LogMessage (_DEBUG_CACHE, _T("  UseCached [%p] [%d] at [%d]"), lTextCache, lWordDisplayed, lTextCacheNdx);
#endif
					lDisplayText = (LPCTSTR)(*lTextCache);
				}
				else
				{
					lDisplayText = JoinStringArray (mTextWords, _T(""), false, 0, lWordDisplayed);
					mTextCache.SetAtGrow (lTextCacheNdx, new CAtlString ((LPCTSTR)lDisplayText));
#ifdef	_DEBUG_CACHE
					LogMessage (_DEBUG_CACHE, _T("  AddCached [%p] [%d] at [%d] CacheSize [%d] Text [%s]"), mTextCache[lTextCacheNdx], lWordDisplayed, lTextCacheNdx, mTextCache.GetCount(), DebugStr(*(mTextCache[lTextCacheNdx])));
#endif
				}
			}
			else
			{
				lDisplayText = JoinStringArray (mTextWords, _T(""), false, 0, lWordDisplayed);
			}
		}
	}
	else
	{
		lDisplayText = (LPCTSTR)mText;
	}
	return lDisplayText;
}

CAtlString CAgentTextDraw::GetDisplayWord (INT_PTR pWordNdx) const
{
	if	(
			(pWordNdx >= 0)
		&&	(pWordNdx < (INT_PTR)mTextWords.GetCount())
		)
	{
		return mTextWords [pWordNdx];
	}
	return CAtlString();
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentTextDraw::DisplayFirstWord (bool pForSpeech)
{
	mWordDisplayed = min (0, (INT_PTR)mTextWords.GetCount()-1);
	if	(pForSpeech)
	{
		mWordDisplayed = max (mWordDisplayed-1, -1);
		if	(mSpeechWords.GetCount() > 0)
		{
			mWordDisplayed = max (mWordDisplayed, mTextWords.GetCount() - mSpeechWords.GetCount());
		}
	}
	while	(
				(mWordDisplayed >= 0)
			&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
			&&	(mTextWords [mWordDisplayed].IsEmpty())
			)
	{
		mWordDisplayed++;
	}
	mBounds.SetRectEmpty ();

	return (mWordDisplayed >= 0);
}

bool CAgentTextDraw::DisplayNextWord (bool pForSpeech)
{
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
		)
	{
		if	(
				(pForSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mWordDisplayed = max (mWordDisplayed, mTextWords.GetCount() - mSpeechWords.GetCount());
		}

		mWordDisplayed++;

		while	(
					(mWordDisplayed >= 0)
				&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
				&&	(mTextWords [mWordDisplayed].IsEmpty())
				)
		{
			mWordDisplayed++;
		}
		return true;
	}
	return false;
}

bool CAgentTextDraw::DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech)
{
	INT_PTR	lWordNdx;
	long	lWordEnd = pWordPos + pWordLength;

	if	(pForSpeech)
	{
		for	(lWordNdx = 0; lWordNdx < (INT_PTR)mSpeechWords.GetCount(); lWordNdx++)
		{
			lWordEnd -= mSpeechWords [lWordNdx].GetLength ();
			if	(lWordEnd <= 0)
			{
				break;
			}
		}
		lWordNdx += (int)(mTextWords.GetCount() - mSpeechWords.GetCount());
		lWordNdx = min (lWordNdx, (INT_PTR)mTextWords.GetCount()-1);
	}
	else
	{
		for	(lWordNdx = 0; lWordNdx < (INT_PTR)mTextWords.GetCount(); lWordNdx++)
		{
			lWordEnd -= mTextWords [lWordNdx].GetLength ();
			if	(lWordEnd <= 0)
			{
				break;
			}
		}
		lWordNdx = min (lWordNdx, (INT_PTR)mTextWords.GetCount()-1);
	}

	if	(mWordDisplayed != lWordNdx)
	{
		return DisplayThisWord (lWordNdx, pForSpeech);
	}
	return false;
}

bool CAgentTextDraw::DisplayThisWord (INT_PTR pWordNdx, bool pForSpeech)
{
	if	(
			(pWordNdx >= 0)
		&&	(pWordNdx < (INT_PTR)mTextWords.GetCount())
		)
	{
		mWordDisplayed = pWordNdx;
		return true;
	}
	return false;
}

bool CAgentTextDraw::DisplayAllWords (bool pForSpeech)
{
	if	(mWordDisplayed != (INT_PTR)mTextWords.GetCount()-1)
	{
		mWordDisplayed = (INT_PTR)mTextWords.GetCount()-1;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentTextDraw::CanScroll (const CRect & pTextBounds) const
{
	return (mBounds.bottom > pTextBounds.bottom);
}

DWORD CAgentTextDraw::CalcScroll (const CRect & pTextBounds, long & pScrollInc, long & pScrollMin, long & pScrollMax, bool pClipLines, DWORD pMaxLineTime) const
{
	DWORD	lScrollTime = 0;

	if	(mBounds.bottom > pTextBounds.bottom)
	{
		long	lScrollScale;
		long	lScrollInc;

		pScrollMin = pClipLines ? GetLineRect(0).bottom - pTextBounds.bottom : 0;
		pScrollMax = mBounds.bottom - pTextBounds.bottom;
		pScrollInc = max (MulDiv (GetSize().cy, 1, GetLineCount()*4), 1);

		lScrollTime = GetProfileInt (_T("windows"), _T("DragScrollInterval"), DD_DEFSCROLLINTERVAL) * 2;
		lScrollScale = ((long)lScrollTime + 9) / 10;
		lScrollInc = max (MulDiv (pScrollInc, 1, lScrollScale), 1);
		if	(lScrollInc < pScrollInc)
		{
			lScrollTime = max (MulDiv (lScrollTime, lScrollInc, pScrollInc), 10);
			pScrollInc = lScrollInc;
		}
		if	(
				(pMaxLineTime > 0)
			&&	(MulDiv (GetSize().cy, lScrollTime, GetLineCount() * pScrollInc) > (long)pMaxLineTime)
			)
		{
			lScrollTime = MulDiv (GetLineCount(), pMaxLineTime * pScrollInc, GetSize().cy);
		}
	}
	else
	{
		pScrollInc = 0;
		pScrollMax = 0;
	}
	return lScrollTime;
}

DWORD CAgentTextDraw::InitScroll (const CRect & pTextBounds, bool pForceReinit, bool pClipLines, DWORD pMaxLineTime)
{
	DWORD	lScrollTime;
	long	lScrollInc = mScrollInc;
	long	lScrollMin = mScrollMin;
	long	lScrollMax = mScrollMax;

	lScrollTime = CalcScroll (pTextBounds, lScrollInc, lScrollMin, lScrollMax, pClipLines, pMaxLineTime);
	if	(!lScrollTime)
	{
		mScrollPos = LONG_MIN;
	}

	if	(
			(mScrollInc != lScrollInc)
		||	(mScrollMin != lScrollMin)
		||	(mScrollMax != lScrollMax)
		||	(mScrollPos < lScrollMin)
		||	(mScrollPos > lScrollMax)
		||	(
				(pForceReinit)
			&&	(mScrollPos < lScrollMax)
			)
		)
	{
		mScrollInc = lScrollInc;
		mScrollMin = lScrollMin;
		mScrollMax = lScrollMax;
		mScrollPos = min (max (mScrollPos, mScrollMin), mScrollMax);
		return lScrollTime;
	}
	return 0;
}

bool CAgentTextDraw::ApplyScroll (const CRect & pTextBounds, CRect * pClipRect)
{
	if	(mScrollPos > 0)
	{
		mBounds.OffsetRect (0, -min (mScrollPos, max (mBounds.bottom - pTextBounds.bottom, 0)));

		if	(pClipRect)
		{
			int		lLineNdx;
			CRect	lLineRect;

			for	(lLineNdx = (int)mTextLines.GetCount()-1; lLineNdx >= 1; lLineNdx--)
			{
				lLineRect = GetLineRect (lLineNdx);
				if	(
						(lLineRect.bottom > pClipRect->bottom)
					&&	(lLineRect.top <= pClipRect->bottom)
					)
				{
					pClipRect->bottom = lLineRect.top;
					break;
				}
			}
		}
		return true;
	}
	return false;
}

bool CAgentTextDraw::Scroll ()
{
	if	(
			(mScrollPos < mScrollMax)
		&&	(mScrollInc > 0)
		)
	{
		mScrollPos = min (mScrollPos + mScrollInc, mScrollMax);
		return (mScrollPos > 0);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CSize CAgentTextDraw::CalcTextSize (HFONT pFont, USHORT pPerLine, USHORT pLines)
{
	CSize			lRet;
	CTextSize		lTextSize (pFont, false);
	tS <TEXTMETRIC>	lTextMetrics;

	lTextSize.GetTextMetrics (lTextMetrics);
	lRet = lTextSize.MeasureText (_T("x"), 1);
	lRet.cx *= (long)(short)pPerLine;
	lRet.cy = (long)(short)pLines * (lTextMetrics.tmAscent+lTextMetrics.tmDescent);

	return lRet;
}

CSize CAgentTextDraw::CalcTextSize (HFONT pFont, USHORT pPerLine)
{
	CSize		lRet;
	CTextSize	lTextSize (pFont, false);

	lRet = lTextSize.MeasureText (_T("x"), 1);
	lRet.cx *= (long)(short)pPerLine;
	mBounds.SetRect (0, 0, lRet.cx, INT_MAX);
	lRet.cy = MeasureText (GetFullText(), lTextSize.GetDC(), pFont).cy;
	mBounds.right = mBounds.left;

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentTextDraw::IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool & pBreakAfter)
{
	if	(mTextWords.GetCount() > 0)
	{
		int	lCharNdx = pNdx;
		int	lWordNdx;

		for	(lWordNdx = 0; (lCharNdx > 0) && (lWordNdx < (int)mTextWords.GetCount()); lWordNdx++)
		{
			lCharNdx -= mTextWords [lWordNdx].GetLength ();
		}
		pBreakAfter = false;
		return (lCharNdx == 0);
	}
	else
	{
		return CTextWrap::IsBreakChar (pText, pNdx, pPriority, pBreakAfter);
	}
}
