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
#include "StdAfx.h"
#include "AgentText.h"
#ifdef	__cplusplus_cli
#include "FontProperties.h"
using namespace System;
using namespace System::Drawing;
using namespace System::Windows;
using namespace System::Windows::Media;
using namespace System::Windows::Media::TextFormatting;
#else
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif
#endif
#include "DebugStr.h"

#ifndef	__cplusplus_cli
#ifdef	_DEBUG
#define	_DEBUG_TEXT		(GetProfileDebugInt(_T("DebugSpeechText"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
//#define	_DEBUG_CACHE	LogDebugFast
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentText::CAgentText ()
:	mSapiVersion (0)
{
	SapiVersion = 5;
}
	
CAgentText::CAgentText (UINT pSapiVersion)
:	mSapiVersion (0)
{
	SapiVersion = pSapiVersion;
}

CAgentText::CAgentText (array<System::String^>^ pWords)
:	mSapiVersion (0)
{
	SapiVersion = 5;
	operator= (pWords);
}

CAgentText::CAgentText (array<System::String^>^ pWords, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SapiVersion = pSapiVersion;
	operator= (pWords);
}

CAgentText::CAgentText (CAgentText^ pText)
:	mSapiVersion (0)
{
	SapiVersion = 5;
	operator= (pText);
}

CAgentText::CAgentText (CAgentText^ pText, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SapiVersion = pSapiVersion;
	operator= (pText);
}

/////////////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentText::CAgentText (UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
}

CAgentText::CAgentText (const CAtlStringArray& pWords, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
	operator= (pWords);
}

CAgentText::CAgentText (const CAgentText& pText, UINT pSapiVersion)
:	mSapiVersion (0)
{
	SetSapiVersion (pSapiVersion);
	operator= (pText);
}

CAgentText::~CAgentText ()
{
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentText^ CAgentText::operator= (array<System::String^>^ pWords)
{
	mTextWords = nullptr;
	operator+= (pWords);
	return this;
}

CAgentText^ CAgentText::operator+= (array<System::String^>^ pWords)
{
	Append (pWords, pWords);
	return this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentText^ CAgentText::operator= (CAgentText^ pText)
{
	mSapiVersion = pText->mSapiVersion;
	mText = gcnew String (pText->mText);
	mTextWords = safe_cast <array<String^>^> (pText->mTextWords->Clone ());
	mSpeechWords = safe_cast <array<String^>^> (pText->mSpeechWords->Clone ());
	return this;
}

CAgentText^ CAgentText::operator+= (CAgentText^ pText)
{
	Append (pText->mTextWords, pText->mSpeechWords);
	return this;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentText::Append (CAgentText^ pText)
{
	Append (pText, false);
}

void CAgentText::Append (CAgentText^ pText, System::Boolean pAppendSpeech)
{
	Append (pText->mTextWords, pText->mSpeechWords, pAppendSpeech);
}

void CAgentText::Append (array<System::String^>^ pTextWords, array<System::String^>^ pSpeechWords)
{
	Append (pTextWords, pSpeechWords, false);
}

void CAgentText::Append (array<System::String^>^ pTextWords, array<System::String^>^ pSpeechWords, System::Boolean pAppendSpeech)
{
	if	(
			(mTextWords)
		&&	(mTextWords->Length > 0)
		&&	(pTextWords)
		&&	(pTextWords->Length > 0)
		)
	{
		int	lLastNdx = mTextWords->Length-1;

		mTextWords [lLastNdx] = mTextWords [lLastNdx]->TrimEnd();
		mTextWords->Resize (mTextWords, mTextWords->Length+pTextWords->Length+1);
		mTextWords [++lLastNdx] = gcnew String("\n");
		for each (String^ lWord in pTextWords)
		{
			mTextWords [++lLastNdx] = gcnew String (lWord);
		}
	}
	else
	if	(pTextWords)
	{
		mTextWords = safe_cast <array<String^>^> (pTextWords->Clone ());
	}

	if	(mTextWords)
	{
		mText = String::Join ("", mTextWords);
	}
	else
	{
		mText = String::Empty;
	}

	if	(
			(pSpeechWords)
		&&	(pSpeechWords->Length > 0)
		)
	{
		if	(
				(pAppendSpeech)
			&&	(mSpeechWords)
			&&	(mSpeechWords->Length > 0)
			)
		{
			int	lLastNdx = mSpeechWords->Length-1;

			mSpeechWords->Resize (mSpeechWords, mSpeechWords->Length+pSpeechWords->Length+1);
			mSpeechWords [++lLastNdx] = gcnew String ("\n");
			for each (String^ lWord in pSpeechWords)
			{
				mSpeechWords [++lLastNdx] = gcnew String (lWord);
			}
		}
		else
		{
			mSpeechWords = safe_cast <array<String^>^> (pSpeechWords->Clone ());
		}
	}
	else
	if	(
			(pTextWords)
		&&	(pTextWords->Length > 0)
		)
	{
		if	(
				(pAppendSpeech)
			&&	(mSpeechWords)
			&&	(mSpeechWords->Length > 0)
			)
		{
			int	lLastNdx = mSpeechWords->Length-1;

			mSpeechWords->Resize (mSpeechWords, mSpeechWords->Length+pTextWords->Length+1);
			mSpeechWords [++lLastNdx] = gcnew String ("\n");
			for each (String^ lWord in pTextWords)
			{
				mSpeechWords [++lLastNdx] = gcnew String (lWord);
			}
		}
		else
		{
			mSpeechWords = safe_cast <array<String^>^> (pTextWords->Clone ());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentText& CAgentText::operator= (const CAtlStringArray& pWords)
{
	mTextWords.RemoveAll ();
	operator+= (pWords);
	return *this;
}

CAgentText& CAgentText::operator+= (const CAtlStringArray& pWords)
{
	Append (pWords, pWords);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentText& CAgentText::operator= (const CAgentText& pText)
{
	mSapiVersion = pText.mSapiVersion;
	mText = pText.mText;
	mTextWords.Copy (pText.mTextWords);
	mSpeechWords.Copy (pText.mSpeechWords);
	return *this;
}

CAgentText& CAgentText::operator+= (const CAgentText& pText)
{
	Append (pText.mTextWords, pText.mSpeechWords);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentText::Append (const CAgentText& pText, bool pAppendSpeech)
{
	Append (pText.mTextWords, pText.mSpeechWords, pAppendSpeech);
}

void CAgentText::Append (const CAtlStringArray& pTextWords, const CAtlStringArray& pSpeechWords, bool pAppendSpeech)
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
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
UInt32 CAgentText::SapiVersion::get ()
{
	return mSapiVersion;
}

void CAgentText::SapiVersion::set (UInt32 pSapiVersion)
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
	}
}
#else
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
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentText::FullText::get()
{
	return mText;
}

System::String^ CAgentText::SpeechText::get()
{
	if	(
			(mSpeechWords)
		&&	(mSpeechWords->Length > 0)
		)
	{
		return String::Join ("", mSpeechWords);
	}
	else
	{
		return mText;
	}
}
#else
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
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::UInt32 CAgentTextDraw::DefaultAutoPaceTime::get()
{
	return 300;
}
int CAgentTextDraw::DefaultLookAhead::get()
{
	return 2;
}
#else
const DWORD CAgentTextDraw::mDefaultAutoPaceTime = 300;
const INT_PTR CAgentTextDraw::mDefaultLookAhead = 2;
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw::CAgentTextDraw ()
{
}

CAgentTextDraw::CAgentTextDraw (System::UInt32 pSapiVersion)
:	CAgentText (pSapiVersion)
{
}

CAgentTextDraw::CAgentTextDraw (CAgentText^ pText)
:	CAgentText (pText)
{
}

CAgentTextDraw::CAgentTextDraw (CAgentText^ pText, System::UInt32 pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
}

CAgentTextDraw::CAgentTextDraw (CAgentTextDraw^ pText)
:	CAgentText (pText)
{
}

CAgentTextDraw::CAgentTextDraw (CAgentTextDraw^ pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
}

/////////////////////////////////////////////////////////////////////////////

int CAgentTextDraw::WordCount::get()
{
	return (mTextWords) ? mTextWords->Length : 0;
}

int CAgentTextDraw::WordDisplayed::get()
{
	return mWordDisplayed;
}

int CAgentTextDraw::LookAhead::get()
{
	return Math::Min (WordCount - WordDisplayed - 1, DefaultLookAhead);
}

/////////////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw::CAgentTextDraw (UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	DisplayNoWords ();
}

CAgentTextDraw::CAgentTextDraw (const CAgentText& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	DisplayNoWords ();
}

CAgentTextDraw::CAgentTextDraw (const CAgentTextDraw& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	DisplayNoWords ();
}

CAgentTextDraw::~CAgentTextDraw ()
{
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli

CAgentTextDraw^ CAgentTextDraw::operator= (CAgentText^ pText)
{
	CAgentText::operator= (pText);
	DisplayNoWords ();
	DisplayFirstWord ();
	return this;
}

CAgentTextDraw^ CAgentTextDraw::operator+= (CAgentText^ pText)
{
	CAgentText::operator+= (pText);
	ResetState ();
	return this;
}

CAgentTextDraw^ CAgentTextDraw::operator= (CAgentTextDraw^ pText)
{
	CAgentText::operator= (pText);
	ResetState ();

	mWordDisplayed = pText->mWordDisplayed;
	mScrollPos = pText->mScrollPos;
	mScrollInc = pText->mScrollInc;
	mScrollMin = pText->mScrollMin;
	mScrollMax = pText->mScrollMax;
	mScrollTime = pText->mScrollTime;
	return this;
}

CAgentTextDraw^ CAgentTextDraw::operator+= (CAgentTextDraw^ pText)
{
	CAgentText::operator+= (pText);
	ResetState ();
	return this;
}

/////////////////////////////////////////////////////////////////////////////
#else	//   __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
CAgentTextDraw& CAgentTextDraw::operator= (const CAgentText& pText)
{
	CAgentText::operator= (pText);
	DisplayNoWords ();
	DisplayFirstWord ();
	return *this;
}

CAgentTextDraw& CAgentTextDraw::operator+= (const CAgentText& pText)
{
	CAgentText::operator+= (pText);
	ResetState ();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw& CAgentTextDraw::operator= (const CAgentTextDraw& pText)
{
	CTextWrap::operator= (pText);
	CAgentText::operator= (pText);
	ResetState ();

	mWordDisplayed = pText.mWordDisplayed;
	mScrollBounds = pText.mScrollBounds;
	mScrollPos = pText.mScrollPos;
	mScrollInc = pText.mScrollInc;
	mScrollMin = pText.mScrollMin;
	mScrollMax = pText.mScrollMax;
	mScrollTime = pText.mScrollTime;
	return *this;
}

CAgentTextDraw& CAgentTextDraw::operator+= (const CAgentTextDraw& pText)
{
	CAgentText::operator+= (pText);
	ResetState ();
	return *this;
}
/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentTextDraw::ResetState ()
{
	mTextCacheLimit = 10;
	mTextCacheStart = 0;
	mTextCache = gcnew System::Collections::Generic::List <System::String^> (mTextCacheLimit);
}
#else
void CAgentTextDraw::ResetState ()
{
	mTextCacheLimit = 10;
	mTextCacheStart = 0;
	mTextCache.DeleteAll ();

	mBounds.SetRectEmpty ();
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentTextDraw::DisplayText::get ()
{
	return GetDisplayText (0);
}

System::String^ CAgentTextDraw::GetDisplayText (int pLookAhead)
#else
CAtlString CAgentTextDraw::GetDisplayText (INT_PTR pLookAhead) const
#endif
{
#ifdef	__cplusplus_cli
	String^		lDisplayText;
#else	
	CAtlString	lDisplayText;
#endif	
//
//	The caching is primitive but does the job.  It assumes that display text will be accessed
//	with constantly growing length.
//
#ifdef	__cplusplus_cli
	if	(
			(mTextWords)
		&&	(mTextWords->Length > 0)
		&&	(mWordDisplayed	< mTextWords->Length)
		)
#else
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed	< (INT_PTR)mTextWords.GetCount())
		)
#endif		
	{
		if	(mWordDisplayed	>= 0)
		{
#ifdef	__cplusplus_cli
			int			lWordDisplayed = Math::Min (mWordDisplayed + pLookAhead, mTextWords->Length-1);
			int			lTextCacheNdx;
#else
			INT_PTR		lWordDisplayed = min (mWordDisplayed + pLookAhead, (INT_PTR)mTextWords.GetCount()-1);
			INT_PTR		lTextCacheNdx;
			CAtlString*	lTextCache;
#endif			

#ifdef	__cplusplus_cli
			if	(!mTextCache)
			{
				mTextCache = gcnew System::Collections::Generic::List <System::String^>;
			}
			
			while	(
						(mTextCacheStart < mWordDisplayed)
					&&	(mTextCache->Count > 0)
					)
			{
				mTextCache->RemoveAt (0);
				mTextCacheStart++;
			}
#else
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
#endif			

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
#ifdef	__cplusplus_cli
				if	(
						(lTextCacheNdx < mTextCache->Count)
					&&	(mTextCache [lTextCacheNdx])
					)
				{
					lDisplayText = gcnew String (mTextCache [lTextCacheNdx]);
				}
				else
				{
					lDisplayText = String::Join ("", mTextWords, 0, lWordDisplayed+1);
					while (mTextCache->Count < lTextCacheNdx+1)
					{
						mTextCache->Add ("");
					}
					mTextCache [lTextCacheNdx] = gcnew String (lDisplayText);
				}
#else
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
#endif				
			}
			else
			{
#ifdef	__cplusplus_cli
				lDisplayText = String::Join ("", mTextWords, 0, lWordDisplayed+1);
#else
				lDisplayText = JoinStringArray (mTextWords, _T(""), false, 0, lWordDisplayed);
#endif				
			}
		}
	}
	else
	{
#ifdef	__cplusplus_cli
		lDisplayText = gcnew String (mText);
#else
		lDisplayText = (LPCTSTR)mText;
#endif		
	}
	return lDisplayText;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentTextDraw::GetDisplayWord (int pWordNdx)
{
	if	(
			(mTextWords)
		&&	(pWordNdx >= 0)
		&&	(pWordNdx < mTextWords->Length)
		)
	{
		return mTextWords [pWordNdx];
	}
	return String::Empty;
}
#else
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
#endif


/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::DisplayFirstWord ()
{
	return DisplayFirstWord (false);
}

System::Boolean CAgentTextDraw::DisplayFirstWord (System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayFirstWord (bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli
	if	(mTextWords)
	{
		mWordDisplayed = Math::Min (0, mTextWords->Length-1);
	}
#else
	mWordDisplayed = min (0, (INT_PTR)mTextWords.GetCount()-1);
#endif

#ifdef	__cplusplus_cli
	if	(pForSpeech)
	{
		mWordDisplayed = Math::Max (mWordDisplayed-1, -1);
		if	(
				(mSpeechWords)
			&&	(mSpeechWords->Length > 0)
			)
		{
			mWordDisplayed = Math::Max (mWordDisplayed, mTextWords->Length - mSpeechWords->Length);
		}
	}
#else
	if	(pForSpeech)
	{
		mWordDisplayed = max (mWordDisplayed-1, -1);
		if	(mSpeechWords.GetCount() > 0)
		{
			mWordDisplayed = max (mWordDisplayed, mTextWords.GetCount() - mSpeechWords.GetCount());
		}
	}
#endif
	
#ifdef	__cplusplus_cli
	if	(mTextWords)
	{
		while	(
					(mWordDisplayed >= 0)
				&&	(mWordDisplayed < mTextWords->Length-1)
				&&	(String::IsNullOrEmpty (mTextWords [mWordDisplayed]))
				)
		{
			mWordDisplayed++;
		}
	}
#else	
	while	(
				(mWordDisplayed >= 0)
			&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
			&&	(mTextWords [mWordDisplayed].IsEmpty())
			)
	{
		mWordDisplayed++;
	}
#endif
	
	ResetState ();
	return (mWordDisplayed >= 0);
}

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::DisplayNextWord ()
{
	return DisplayNextWord (false);
}

System::Boolean CAgentTextDraw::DisplayNextWord (System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayNextWord (bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli
	if	(
			(mTextWords)
		&&	(mTextWords->Length > 0)
		&&	(mWordDisplayed < mTextWords->Length-1)
		)
#else
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
		)
#endif		
	{
#ifdef	__cplusplus_cli
		if	(
				(pForSpeech)
			&&	(mSpeechWords)
			&&	(mSpeechWords->Length > 0)
			)
		{
			mWordDisplayed = Math::Max (mWordDisplayed, mTextWords->Length - mSpeechWords->Length);
		}
#else
		if	(
				(pForSpeech)
			&&	(mSpeechWords.GetCount() > 0)
			)
		{
			mWordDisplayed = max (mWordDisplayed, mTextWords.GetCount() - mSpeechWords.GetCount());
		}
#endif		

		mWordDisplayed++;

#ifdef	__cplusplus_cli
		if	(mTextWords)
		{
			while	(
						(mWordDisplayed >= 0)
					&&	(mWordDisplayed < mTextWords->Length-1)
					&&	(String::IsNullOrEmpty (mTextWords [mWordDisplayed]))
					)
			{
				mWordDisplayed++;
			}
		}
#else
		while	(
					(mWordDisplayed >= 0)
				&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
				&&	(mTextWords [mWordDisplayed].IsEmpty())
				)
		{
			mWordDisplayed++;
		}
#endif		
		return true;
	}
	return false;
}

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::DisplayThisWord (System::Int32 pWordPos, System::Int32 pWordLength)
{
	return DisplayThisWord (pWordPos, pWordLength, true);
}

System::Boolean CAgentTextDraw::DisplayThisWord (System::Int32 pWordPos, System::Int32 pWordLength, System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli
	int				lWordNdx;
	System::Int32	lWordEnd = pWordPos + pWordLength;
#else
	INT_PTR			lWordNdx;
	long			lWordEnd = pWordPos + pWordLength;
#endif	

	if	(pForSpeech)
	{
#ifdef	__cplusplus_cli
		if	(mSpeechWords)
		{
			for	(lWordNdx = 0; lWordNdx < mSpeechWords->Length; lWordNdx++)
			{
				lWordEnd -= mSpeechWords [lWordNdx]->Length;
				if	(lWordEnd <= 0)
				{
					break;
				}
			}
			lWordNdx += mTextWords->Length - mSpeechWords->Length;
		}
		if	(mTextWords)
		{
			lWordNdx = Math::Min (lWordNdx, mTextWords->Length-1);
		}
#else
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
#endif		
	}
	else
	{
#ifdef	__cplusplus_cli
		if	(mTextWords)
		{
			for	(lWordNdx = 0; lWordNdx < mTextWords->Length; lWordNdx++)
			{
				lWordEnd -= mTextWords [lWordNdx]->Length;
				if	(lWordEnd <= 0)
				{
					break;
				}
			}
			lWordNdx = Math::Min (lWordNdx, mTextWords->Length-1);
		}
#else
		for	(lWordNdx = 0; lWordNdx < (INT_PTR)mTextWords.GetCount(); lWordNdx++)
		{
			lWordEnd -= mTextWords [lWordNdx].GetLength ();
			if	(lWordEnd <= 0)
			{
				break;
			}
		}
		lWordNdx = min (lWordNdx, (INT_PTR)mTextWords.GetCount()-1);
#endif
	}

	if	(mWordDisplayed != lWordNdx)
	{
		return DisplayThisWord (lWordNdx, pForSpeech);
	}
	return false;
}

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::DisplayThisWord (int pWordNdx)
{
	return DisplayThisWord (pWordNdx, true);
}

System::Boolean CAgentTextDraw::DisplayThisWord (int pWordNdx, System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayThisWord (INT_PTR pWordNdx, bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli
	if	(
			(mTextWords)
		&&	(pWordNdx >= 0)
		&&	(pWordNdx < mTextWords->Length)
		)
	{
		mWordDisplayed = pWordNdx;
		return true;
	}
#else
	if	(
			(pWordNdx >= 0)
		&&	(pWordNdx < (INT_PTR)mTextWords.GetCount())
		)
	{
		mWordDisplayed = pWordNdx;
		return true;
	}
#endif
	return false;
}

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::DisplayAllWords ()
{
	return DisplayAllWords (false);
}
System::Boolean CAgentTextDraw::DisplayAllWords (System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayAllWords (bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli
	if	(mWordDisplayed != ((mTextWords) ? mTextWords->Length : 0)-1)
	{
		mWordDisplayed = ((mTextWords) ? mTextWords->Length : 0)-1;
		return true;
	}
#else
	if	(mWordDisplayed != (INT_PTR)mTextWords.GetCount()-1)
	{
		mWordDisplayed = (INT_PTR)mTextWords.GetCount()-1;
		return true;
	}
#endif
	return false;
}

void CAgentTextDraw::DisplayNoWords ()
{
	mWordDisplayed = -1;
	ResetState ();
	ResetScroll ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
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

CSize CAgentTextDraw::CalcTextSize (HFONT pFont)
{
	return MeasureText (GetFullText(), NULL, pFont);
}

CSize CAgentTextDraw::CalcTextSize (HDC pDC)
{
	return MeasureText (GetFullText(), pDC);
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
Boolean CAgentTextDraw::CanPace::get ()
{
	return (WordCount > 0);
}
#else
bool CAgentTextDraw::get_CanPace () const
{
	return (WordCount > 0);
}
#endif

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::CanScroll::get ()
{
	return false;
}
#else
bool CAgentTextDraw::get_CanScroll () const
{
	if	(mScrollBounds.IsRectEmpty())
	{
		return false;
	}
	else
	{
		return (mBounds.bottom > mScrollBounds.bottom);
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::Int32 CAgentTextDraw::ScrollPos::get() {return mScrollPos;}
System::Int32 CAgentTextDraw::ScrollInc::get() {return mScrollInc;}
System::Int32 CAgentTextDraw::ScrollMin::get() {return mScrollMin;}
System::Int32 CAgentTextDraw::ScrollMax::get() {return mScrollMax;}
System::UInt32 CAgentTextDraw::ScrollTime::get() {return mScrollTime;}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::UInt32 CAgentTextDraw::CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax)
{
	return CalcScroll (pScrollInc, pScrollMin, pScrollMax, false);
}
System::UInt32 CAgentTextDraw::CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed)
{
	return CalcScroll (pScrollInc, pScrollMin, pScrollMax, pDelayed, 0);
}
System::UInt32 CAgentTextDraw::CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed, System::UInt32 pMaxLineTime)
{
	return 0;
}
#else
DWORD CAgentTextDraw::CalcScroll (long& pScrollInc, long& pScrollMin, long& pScrollMax, bool pDelayed, DWORD pMaxLineTime) const
{
	DWORD	lScrollTime = 0;

	if	(mBounds.bottom > mScrollBounds.bottom)
	{
		lScrollTime = CalcScroll (pScrollInc, pScrollMin, pScrollMax, mBounds.Height(), mScrollBounds.Height(), GetLineCount(), pDelayed, pMaxLineTime);
	}
	else
	{
		pScrollMin = mScrollMin;
		pScrollInc = 0;
		pScrollMax = 0;
	}
	return lScrollTime;
}
#endif

#ifdef	__cplusplus_cli
System::UInt32 CAgentTextDraw::CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Double pTextHeight, System::Double pScrollHeight, int pLineCount, System::Boolean pDelayed, System::UInt32 pMaxLineTime)
#else
DWORD CAgentTextDraw::CalcScroll (long& pScrollInc, long& pScrollMin, long& pScrollMax, long pTextHeight, long pScrollHeight, INT_PTR pLineCount, bool pDelayed, DWORD pMaxLineTime) const
#endif
{
#ifdef	__cplusplus_cli
	System::UInt32	lScrollTime = 0;
	Int32			lScrollScale;
	Int32			lScrollInc;
#else
	DWORD			lScrollTime = 0;
	long			lScrollScale;
	long			lScrollInc;
#endif	

#ifdef	__cplusplus_cli
	pScrollMin = 0;
	pScrollMax = (Int32)Math::Ceiling (pTextHeight - pScrollHeight);
	pScrollInc = Math::Max ((Int32)Math::Round(pTextHeight / ((Double)pLineCount*4.0)), 1);
#else
	pScrollMin = 0;
	pScrollMax = pTextHeight - pScrollHeight;
	pScrollInc = max (MulDiv (pTextHeight, 1, (long)pLineCount*4), 1);
#endif		

#ifdef	__cplusplus_cli
	lScrollTime = (UInt32)System::Windows::SystemParameters::MouseHoverTime.TotalMilliseconds/4;
	lScrollScale = ((Int32)lScrollTime + 9) / 10;
	lScrollInc = Math::Max ((Int32)Math::Round ((Single)pScrollInc / (Single)lScrollScale), 1);
	if	(lScrollInc < pScrollInc)
	{
		lScrollTime = Math::Max ((UInt32)Math::Round ((Double)lScrollTime * (Double)lScrollInc / (Double)pScrollInc), (UInt32)10);
		pScrollInc = lScrollInc;
	}
	if	(
			(pMaxLineTime > 0)
		&&	((UInt32)Math::Round (pTextHeight * (Double)lScrollTime / ((Double)pLineCount * (Double)pScrollInc)) > pMaxLineTime)
		)
	{
		lScrollTime = (UInt32)Math::Round ((Double)pLineCount * (Double)pMaxLineTime * (Double)pScrollInc / pTextHeight);
	}
#else
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
		&&	(MulDiv (pTextHeight, lScrollTime, (long)pLineCount * pScrollInc) > (long)pMaxLineTime)
		)
	{
		lScrollTime = MulDiv ((long)pLineCount, pMaxLineTime * pScrollInc, pTextHeight);
	}
#endif		

	if	(pDelayed)
	{
#ifdef	__cplusplus_cli
		pScrollMin = (Int32)Math::Round (Math::Max (pScrollHeight - (Double)Math::Max (mScrollPos, 0), 0.0) * -1.0 / (Double)pScrollInc);
#else
		pScrollMin = MulDiv (max (pScrollHeight - max (mScrollPos, 0), 0), -1, pScrollInc);
#endif			
	}

	return lScrollTime;
}

/////////////////////////////////////////////////////////////////////////////

#ifndef	__cplusplus_cli
bool CAgentTextDraw::InitScroll (const CRect& pScrollBounds, bool pDelayed, DWORD pMaxLineTime)
{
	long	lScrollInc = mScrollInc;
	long	lScrollMin = mScrollMin;
	long	lScrollMax = mScrollMax;

	mScrollBounds = pScrollBounds;
	mScrollTime = CalcScroll (lScrollInc, lScrollMin, lScrollMax, pDelayed, pMaxLineTime);
	return InitScroll (lScrollInc, lScrollMin, lScrollMax);
}
#endif

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::InitScroll (System::Int32 pScrollInc, System::Int32 pScrollMin, System::Int32 pScrollMax)
#else
bool CAgentTextDraw::InitScroll (long pScrollInc, long pScrollMin, long pScrollMax)
#endif
{
	if	(
			(mScrollInc != pScrollInc)
		||	(mScrollMin != pScrollMin)
		||	(mScrollMax != pScrollMax)
		||	(mScrollPos < pScrollMin)
		||	(mScrollPos > pScrollMax)
		)
	{
		mScrollInc = pScrollInc;
		mScrollMin = pScrollMin;
		mScrollMax = pScrollMax;
#ifdef	__cplusplus_cli
		mScrollPos = Math::Min (Math::Max (mScrollPos, mScrollMin), mScrollMax);
#else
		mScrollPos = min (max (mScrollPos, mScrollMin), mScrollMax);
#endif		
		return true;
	}
	return false;
}

void CAgentTextDraw::ResetScroll ()
{
#ifdef	__cplusplus_cli
	mScrollPos = Int32::MinValue;
#else
	mScrollBounds.SetRectEmpty ();
	mScrollPos = LONG_MIN;
#endif		
	mScrollInc = 0;
	mScrollMin = 0;
	mScrollMax = 0;
	mScrollTime = 0;
}

#ifdef	__cplusplus_cli
System::Boolean CAgentTextDraw::Scroll ()
#else
bool CAgentTextDraw::Scroll ()
#endif
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

#ifndef	__cplusplus_cli
bool CAgentTextDraw::ApplyScroll ()
{
	if	(mScrollPos > 0)
	{
		mBounds.OffsetRect (0, -min (mScrollPos, max (mBounds.bottom - mScrollBounds.bottom, 0)));
		return true;
	}
	return false;
}
#endif	//	__cplusplus_cli

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentTextDraw_TextWrap : public CTextWrap
{
public:
	CAgentTextDraw_TextWrap (CAgentTextDrawForms^ pOwner);
	
	CAgentTextDrawForms^ mOwner;

	virtual System::Boolean IsBreakChar (System::String^ pText, int pNdx, UINT pPriority, System::Boolean% pBreakAfter) override;
};

CAgentTextDraw_TextWrap::CAgentTextDraw_TextWrap (CAgentTextDrawForms^ pOwner)
:	mOwner (pOwner)
{
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean CAgentTextDraw_TextWrap::IsBreakChar (System::String^ pText, int pNdx, UINT pPriority, System::Boolean% pBreakAfter)
{
	if	(
			(mOwner->mTextWords)
		&&	(mOwner->mTextWords->Length > 0)
		)
	{
		int	lCharNdx = pNdx;
		int	lWordNdx;

		for	(lWordNdx = 0; (lCharNdx > 0) && (lWordNdx < mOwner->mTextWords->Length); lWordNdx++)
		{
			lCharNdx -= mOwner->mTextWords [lWordNdx]->Length;
		}
		pBreakAfter = false;
		return (lCharNdx == 0);
	}
	else
	{
		return CTextWrap::IsBreakChar (pText, pNdx, pPriority, pBreakAfter);
	}
}
#else
bool CAgentTextDraw::IsBreakChar (LPCTSTR pText, int pNdx, UINT pPriority, bool& pBreakAfter)
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
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

CAgentTextDrawForms::CAgentTextDrawForms (CAgentTextDraw^ pTextDraw)
:	CAgentTextDraw (pTextDraw)
{
	mTextWrap = gcnew CAgentTextDraw_TextWrap (this);
	ResetState ();
}

CAgentTextDrawForms::CAgentTextDrawForms (CAgentTextDrawForms^ pTextDraw)
{
	mTextWrap = gcnew CAgentTextDraw_TextWrap (this);
	operator= (pTextDraw);
}

CAgentTextDrawForms^ CAgentTextDrawForms::operator= (CAgentTextDrawForms^ pTextDraw)
{
	ResetState ();
	CAgentTextDraw::operator= (pTextDraw);
	mTextWrap->operator= (pTextDraw->mTextWrap);
	mScrollBounds = pTextDraw->mScrollBounds;
	return this;
}

/////////////////////////////////////////////////////////////////////////////
	
CAgentTextDrawForms^ CAgentTextDrawForms::Downcast (CAgentTextDraw^ pTextDraw)
{
	if	(
			(pTextDraw)
		&&	(CAgentTextDrawForms::typeid->IsAssignableFrom (pTextDraw->GetType()))
		)
	{
		return safe_cast <CAgentTextDrawForms^> (pTextDraw);
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentTextDrawForms::ResetState ()
{
	mTextWrap->mBounds = RectangleF::Empty;
	__super::ResetState ();
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::SizeF CAgentTextDrawForms::CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine, System::UInt16 pLines)
{
	System::Drawing::SizeF	lTextSize;
	HDC						lTempDC = CreateCompatibleDC (0);
	Graphics^				lGraphics = Graphics::FromHdc ((IntPtr)lTempDC);

	try
	{	
		lTextSize.Width = lGraphics->MeasureString ("x", pFont, Int16::MaxValue, System::Drawing::StringFormat::GenericTypographic).Width * (Single)pPerLine;
		lTextSize.Height = (Single)Math::Ceiling (lGraphics->MeasureString ("x", pFont, Int16::MaxValue, System::Drawing::StringFormat::GenericDefault).Height) * (Single)pLines;
	}
	catch AnyExceptionDebug
	
	lGraphics->~Graphics();
	DeleteDC (lTempDC);
	return lTextSize;
}

System::Drawing::SizeF CAgentTextDrawForms::CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine)
{
	System::Drawing::SizeF	lTextSize;
	HDC						lTempDC = CreateCompatibleDC (0);
	Graphics^				lGraphics = Graphics::FromHdc ((IntPtr)lTempDC);

	try
	{	
		lTextSize.Width = lGraphics->MeasureString ("x", pFont, Int16::MaxValue, System::Drawing::StringFormat::GenericTypographic).Width * (Single)pPerLine;
		mTextWrap->mBounds.X = 0;
		mTextWrap->mBounds.Y = 0;
		if	(String::IsNullOrEmpty (FullText))
		{
			mTextWrap->mBounds.Width = 0;
			mTextWrap->mBounds.Height = 0;
			lTextSize.Height = lGraphics->MeasureString ("x", pFont, Int16::MaxValue, System::Drawing::StringFormat::GenericDefault).Height;
		}
		else
		{
			mTextWrap->mBounds.Width = lTextSize.Width;
			mTextWrap->mBounds.Height = Single::MaxValue;
			lTextSize.Height = mTextWrap->MeasureText (FullText, lGraphics, pFont).Height;
			mTextWrap->mBounds.Width = 0;
		}
	}
	catch AnyExceptionDebug
	
	lGraphics->~Graphics();
	DeleteDC (lTempDC);
	return lTextSize;
}

/////////////////////////////////////////////////////////////////////////////

System::Drawing::SizeF CAgentTextDrawForms::CalcTextSize (System::Drawing::Font^ pFont)
{
	return CalcTextSize (pFont, nullptr);
}

System::Drawing::SizeF CAgentTextDrawForms::CalcTextSize (System::Drawing::Font^ pFont, System::Drawing::Graphics^ pGraphics)
{
	return mTextWrap->MeasureText (FullText, pGraphics, pFont);
}

/////////////////////////////////////////////////////////////////////////////

CTextWrap^ CAgentTextDrawForms::TextWrap::get ()
{
	return mTextWrap;
}

System::Drawing::RectangleF CAgentTextDrawForms::TextBounds::get()
{
	return mTextWrap->mBounds;
}

void CAgentTextDrawForms::TextBounds::set (System::Drawing::RectangleF pTextBounds)
{
	mTextWrap->mBounds = pTextBounds;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Drawing::RectangleF CAgentTextDrawForms::ScrollBounds::get()
{
	return mScrollBounds;
}

System::Boolean CAgentTextDrawForms::CanScroll::get ()
{
	if	(mScrollBounds.IsEmpty)
	{
		return false;
	}
	else
	{
		return (mTextWrap->mBounds.Bottom > mScrollBounds.Bottom);
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean CAgentTextDrawForms::InitScroll (System::Drawing::RectangleF pScrollBounds)
{
	return InitScroll (pScrollBounds, false);
}
System::Boolean CAgentTextDrawForms::InitScroll (System::Drawing::RectangleF pScrollBounds, System::Boolean pDelayed)
{
	return InitScroll (pScrollBounds, pDelayed, 0);
}
System::Boolean CAgentTextDrawForms::InitScroll (System::Drawing::RectangleF pScrollBounds, System::Boolean pDelayed, System::UInt32 pMaxLineTime)
{
	Int32	lScrollInc = mScrollInc;
	Int32	lScrollMin = mScrollMin;
	Int32	lScrollMax = mScrollMax;

	mScrollBounds = pScrollBounds;
	mScrollTime = CalcScroll (lScrollInc, lScrollMin, lScrollMax, pDelayed, pMaxLineTime);
	return InitScroll (lScrollInc, lScrollMin, lScrollMax);
}

System::UInt32 CAgentTextDrawForms::CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed, System::UInt32 pMaxLineTime)
{
	UInt32	lScrollTime = 0;

	if	(mTextWrap->mBounds.Bottom > mScrollBounds.Bottom)
	{
		lScrollTime = CalcScroll (pScrollInc, pScrollMin, pScrollMax, mTextWrap->mBounds.Height, mScrollBounds.Height, mTextWrap->LineCount, pDelayed, pMaxLineTime);
	}
	else
	{
		pScrollInc = 0;
		pScrollMax = 0;
	}
	return lScrollTime;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean CAgentTextDrawForms::ApplyScroll ()
{
	if	(mScrollPos > 0)
	{
		mTextWrap->mBounds.Offset (0.0f, -Math::Min ((Single)mScrollPos, Math::Max (mTextWrap->mBounds.Bottom - mScrollBounds.Bottom, 0.0f)));
		return true;
	}
	return false;
}

void CAgentTextDrawForms::ResetScroll ()
{
	__super::ResetScroll ();
	mScrollBounds = RectangleF::Empty;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentTextDrawWPF::CAgentTextDrawWPF (CAgentTextDraw^ pTextDraw)
:	CAgentTextDraw (pTextDraw)
{
	mTextFormatter = System::Windows::Media::TextFormatting::TextFormatter::Create ();
	ResetState ();
}

CAgentTextDrawWPF::CAgentTextDrawWPF (CAgentTextDrawWPF^ pTextDraw)
{
	operator= (pTextDraw);
}

CAgentTextDrawWPF^ CAgentTextDrawWPF::operator= (CAgentTextDrawWPF^ pTextDraw)
{
	ResetState ();
	CAgentTextDraw::operator= (pTextDraw);
	mTextFormatter = pTextDraw->mTextFormatter;
	mScrollBounds = pTextDraw->mScrollBounds;
	return this;
}

/////////////////////////////////////////////////////////////////////////////
	
CAgentTextDrawWPF^ CAgentTextDrawWPF::Downcast (CAgentTextDraw^ pTextDraw)
{
	if	(
			(pTextDraw)
		&&	(CAgentTextDrawWPF::typeid->IsAssignableFrom (pTextDraw->GetType()))
		)
	{
		return safe_cast <CAgentTextDrawWPF^> (pTextDraw);
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentTextDrawWPF::ResetState ()
{
	mTextBounds = System::Windows::Rect::Empty;
	mLineCount = 0;
	__super::ResetState ();
}

/////////////////////////////////////////////////////////////////////////////

System::Windows::Size CAgentTextDrawWPF::CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine, System::UInt16 pLines)
{
	return CalcTextSize (gcnew FontProperties (pFont), pPerLine, pLines);
}
System::Windows::Size CAgentTextDrawWPF::CalcTextSize (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties, System::UInt16 pPerLine, System::UInt16 pLines)
{
	System::Windows::Size	lTextSize;

	try
	{	
		TextParagraphProperties^	lParagraphProperties = gcnew FontParagraphProperties (pFontProperties);
		TextLine^					lTextLine;

		lTextLine = mTextFormatter->FormatLine (gcnew TextStringSource ("x", pFontProperties), 0, Int16::MaxValue, lParagraphProperties, nullptr);
		lTextSize.Width = lTextLine->Width * (Double)pPerLine;
		lTextSize.Height = Math::Ceiling (lTextLine->TextHeight) * (Double)pLines;
	}
	catch AnyExceptionDebug

	return lTextSize;
}

System::Windows::Size CAgentTextDrawWPF::CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine)
{
	return CalcTextSize (gcnew FontProperties (pFont), pPerLine);
}
System::Windows::Size CAgentTextDrawWPF::CalcTextSize (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties, System::UInt16 pPerLine)
{
	System::Windows::Size	lTextSize;

	try
	{	
		TextDrawSource^				lTextSource = gcnew TextDrawSource (this, pFontProperties, WordCount);
		TextParagraphProperties^	lParagraphProperties = gcnew FontParagraphProperties (pFontProperties);
		TextLine^					lTextLine;
		TextLineBreak^				lLineBreak = nullptr;
		int							lLineNdx;

		lTextLine = mTextFormatter->FormatLine (gcnew TextStringSource ("x", pFontProperties), 0, Int16::MaxValue, lParagraphProperties, nullptr);
		lTextSize.Width = lTextLine->Width * (Double)pPerLine;

		for	(lLineNdx = 0; (!lTextSource->mComplete) && (lTextLine = mTextFormatter->FormatLine (lTextSource, lLineNdx, lTextSize.Width, lParagraphProperties, lLineBreak)); lLineNdx += lTextLine->Length, lLineBreak = lTextLine->GetTextLineBreak ())
		{
			lTextSize.Height += Math::Ceiling (lTextLine->TextHeight);
		}
	}
	catch AnyExceptionDebug
	
	return lTextSize;
}

/////////////////////////////////////////////////////////////////////////////

System::Windows::Size CAgentTextDrawWPF::CalcTextSize (System::Drawing::Font^ pFont)
{
	return CalcTextSize (gcnew FontProperties (pFont));
}
System::Windows::Size CAgentTextDrawWPF::CalcTextSize (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties)
{
	System::Windows::Size	lTextSize;

	try
	{	
		TextDrawSource^				lTextSource = gcnew TextDrawSource (this, pFontProperties, WordCount);
		TextParagraphProperties^	lParagraphProperties = gcnew FontParagraphProperties (pFontProperties);
		TextLine^					lTextLine;
		TextLineBreak^				lLineBreak = nullptr;
		int							lLineNdx;

		mLineCount = 0;

		for	(lLineNdx = 0; (!lTextSource->mComplete) && (lTextLine = mTextFormatter->FormatLine (lTextSource, lLineNdx, mTextBounds.Width, lParagraphProperties, lLineBreak)); lLineNdx += lTextLine->Length, lLineBreak = lTextLine->GetTextLineBreak ())
		{
			//LogMessage (LogDebugFast, _T("Line [%d] [%d] [%f %f]"), lLineNdx, lTextLine->Length, lTextLine->Width, lTextLine->TextHeight);
			lTextSize.Width = Math::Max (lTextSize.Width, lTextLine->Width);
			lTextSize.Height += Math::Ceiling (lTextLine->TextHeight);
			mLineCount++;
		}

		mTextBounds.Width = lTextSize.Width;
		mTextBounds.Height = lTextSize.Height;
	}
	catch AnyExceptionDebug
	
	return lTextSize;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextDrawWPF::CalcUsedHeight (System::Drawing::Font^ pFont)
{
	CalcUsedHeight (gcnew FontProperties (pFont));
}
void CAgentTextDrawWPF::CalcUsedHeight (System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties)
{
	try
	{	
		TextDrawSource^				lTextSource = gcnew TextDrawSource (this, pFontProperties, WordDisplayed+1);
		TextParagraphProperties^	lParagraphProperties = gcnew FontParagraphProperties (pFontProperties);
		TextLine^					lTextLine;
		TextLineBreak^				lLineBreak = nullptr;
		int							lLineNdx;

		mTextBounds.Height = 0;

		for	(lLineNdx = 0; (!lTextSource->mComplete) && (lTextLine = mTextFormatter->FormatLine (lTextSource, lLineNdx, mTextBounds.Width, lParagraphProperties, lLineBreak)); lLineNdx += lTextLine->Length, lLineBreak = lTextLine->GetTextLineBreak ())
		{
			mTextBounds.Height += Math::Ceiling (lTextLine->TextHeight);
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void CAgentTextDrawWPF::Draw (System::Windows::Media::DrawingGroup^ pDrawing, System::Drawing::Font^ pFont, System::Drawing::Color pColor, System::Drawing::RectangleF pClipRect, System::Boolean pClipPartialLines)
{
	System::Windows::Media::Brush^	lBrush = gcnew System::Windows::Media::SolidColorBrush (System::Windows::Media::Color::FromArgb (pColor.A, pColor.R, pColor.G, pColor.B));
	TextRunProperties^				lFontProperties = gcnew FontProperties (pFont, lBrush);
	System::Windows::Rect			lClipRect (pClipRect.X, pClipRect.Y, pClipRect.Width, pClipRect.Height);

	Draw (pDrawing, lFontProperties, lClipRect, pClipPartialLines);
}

void CAgentTextDrawWPF::Draw (System::Windows::Media::DrawingGroup^ pDrawing, System::Windows::Media::TextFormatting::TextRunProperties^ pFontProperties, System::Windows::Rect pClipRect, System::Boolean pClipPartialLines)
{
	DrawingContext^		lDrawingContext = pDrawing->Append ();

	lDrawingContext->PushClip (gcnew System::Windows::Media::RectangleGeometry (pClipRect));
	try
	{
		TextDrawSource^				lTextSource = gcnew CAgentTextDrawWPF::TextDrawSource (this, pFontProperties, WordDisplayed+1);
		TextParagraphProperties^	lParagraphProperties = gcnew DoubleAgent::FontParagraphProperties (pFontProperties);
		TextLine^					lTextLine;
		TextLineBreak^				lLineBreak = nullptr;
		int							lLineNdx;
		System::Windows::Point		lLinePos = TextBounds.Location;
		Double						lLineHeight;

		for	(lLineNdx = 0; (!lTextSource->mComplete) && (lTextLine = mTextFormatter->FormatLine (lTextSource, lLineNdx, TextBounds.Width, lParagraphProperties, lLineBreak)); lLineNdx += lTextLine->Length, lLineBreak = lTextLine->GetTextLineBreak ())
		{
			lLineHeight = Math::Ceiling (lTextLine->TextHeight);
			if	(
					(pClipPartialLines)
				&&	(lLinePos.Y + lLineHeight > pClipRect.Bottom)
				)
			{
				break;
			}
			if	(lLinePos.Y + lLineHeight > pClipRect.Top)
			{
				lTextLine->Draw (lDrawingContext, lLinePos, InvertAxes::None);
			}
			lLinePos.Y += lLineHeight;
			if	(
					(lLinePos.Y > mTextBounds.Bottom)
				||	(lLinePos.Y > pClipRect.Bottom)
				)
			{
				break;
			}
		}
	}
	catch AnyExceptionDebug

	lDrawingContext->Pop ();
	lDrawingContext->Close ();
}

/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::TextFormatting::TextFormatter^ CAgentTextDrawWPF::TextFormatter::get ()
{
	return mTextFormatter;
}

System::Windows::Rect CAgentTextDrawWPF::TextBounds::get()
{
	return mTextBounds;
}

void CAgentTextDrawWPF::TextBounds::set(System::Windows::Rect pTextBounds)
{
	mTextBounds = pTextBounds;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Windows::Rect CAgentTextDrawWPF::ScrollBounds::get()
{
	return mScrollBounds;
}

System::Boolean CAgentTextDrawWPF::CanScroll::get ()
{
	if	(mScrollBounds.IsEmpty)
	{
		return false;
	}
	else
	{
		return (mTextBounds.Bottom > mScrollBounds.Bottom);
	}
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean CAgentTextDrawWPF::InitScroll (System::Windows::Rect pScrollBounds)
{
	return InitScroll (pScrollBounds, false);
}
System::Boolean CAgentTextDrawWPF::InitScroll (System::Windows::Rect pScrollBounds, System::Boolean pDelayed)
{
	return InitScroll (pScrollBounds, pDelayed, 0);
}
System::Boolean CAgentTextDrawWPF::InitScroll (System::Windows::Rect pScrollBounds, System::Boolean pDelayed, System::UInt32 pMaxLineTime)
{
	Int32	lScrollInc = mScrollInc;
	Int32	lScrollMin = mScrollMin;
	Int32	lScrollMax = mScrollMax;

	mScrollBounds = pScrollBounds;
	mScrollTime = CalcScroll (lScrollInc, lScrollMin, lScrollMax, pDelayed, pMaxLineTime);
	return InitScroll (lScrollInc, lScrollMin, lScrollMax);
}

System::UInt32 CAgentTextDrawWPF::CalcScroll (System::Int32% pScrollInc, System::Int32% pScrollMin, System::Int32% pScrollMax, System::Boolean pDelayed, System::UInt32 pMaxLineTime)
{
	UInt32	lScrollTime = 0;

	if	(mTextBounds.Bottom > mScrollBounds.Bottom)
	{
		lScrollTime = CalcScroll (pScrollInc, pScrollMin, pScrollMax, mTextBounds.Height, mScrollBounds.Height, mLineCount, pDelayed, pMaxLineTime); 
	}
	else
	{
		pScrollInc = 0;
		pScrollMax = 0;
	}
	return lScrollTime;
}

/////////////////////////////////////////////////////////////////////////////

System::Boolean CAgentTextDrawWPF::ApplyScroll ()
{
	if	(mScrollPos > 0)
	{
		mTextBounds.Offset (0.0, -Math::Min ((Double)mScrollPos, Math::Max (mTextBounds.Bottom - mScrollBounds.Bottom, 0.0)));
		return true;
	}
	return false;
}

void CAgentTextDrawWPF::ResetScroll ()
{
	__super::ResetScroll ();
	mScrollBounds = System::Windows::Rect::Empty;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentTextDrawWPF::TextDrawSource::TextDrawSource (CAgentTextDrawWPF^ pTextDraw, System::Windows::Media::TextFormatting::TextRunProperties^ pTextProperties, int pWordCount)
:	mTextDraw (pTextDraw),
	mTextProperties (pTextProperties),
	mWordCount (pWordCount),
	mComplete (false)
{
}

System::Windows::Media::TextFormatting::TextSpan<System::Windows::Media::TextFormatting::CultureSpecificCharacterBufferRange^>^ CAgentTextDrawWPF::TextDrawSource::GetPrecedingText (int pCharacterIndexLimit)
{
	LogMessage (LogDebug, _T("--- GetPrecedingText ---"));
	return nullptr;
}

int CAgentTextDrawWPF::TextDrawSource::GetTextEffectCharacterIndexFromTextSourceCharacterIndex (int pCharacterIndex)
{
	return pCharacterIndex;
}

System::Windows::Media::TextFormatting::TextRun^ CAgentTextDrawWPF::TextDrawSource::GetTextRun (int pCharacterIndex)
{
	if	(pCharacterIndex >= 0)
	{
		int		lWordNum = 0;
		int		lTextLength = 0;
		
		for each (String^ lWord in mTextDraw->mTextWords)
		{
			if	(++lWordNum > mWordCount)
			{
				break;
			}
			lTextLength += lWord->Length;
			if	(lTextLength > pCharacterIndex)
			{
				return gcnew TextCharacters (lWord, lTextLength-lWord->Length-pCharacterIndex, lTextLength-pCharacterIndex, mTextProperties);
			}
		}
	}
	
	mComplete = true;
	return gcnew TextEndOfParagraph (1);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentTextDrawWPF::TextStringSource::TextStringSource (System::String^ pText, System::Windows::Media::TextFormatting::TextRunProperties^ pTextProperties)
:	mText (pText),
	mTextProperties (pTextProperties)
{
}

System::Windows::Media::TextFormatting::TextSpan<System::Windows::Media::TextFormatting::CultureSpecificCharacterBufferRange^>^ CAgentTextDrawWPF::TextStringSource::GetPrecedingText (int pCharacterIndexLimit)
{
	LogMessage (LogDebug, _T("--- GetPrecedingText ---"));
	return nullptr;
#if	FALSE	
	CharacterBufferRange					lBufferRange (mText, 0, mText->Length);
	CultureSpecificCharacterBufferRange^	lCultureRange = gcnew CultureSpecificCharacterBufferRange (mTextProperties->CultureInfo, lBufferRange);
	return gcnew TextSpan <System::Windows::Media::TextFormatting::CultureSpecificCharacterBufferRange^> (mText->Length, lCultureRange);
#endif	
}

int CAgentTextDrawWPF::TextStringSource::GetTextEffectCharacterIndexFromTextSourceCharacterIndex (int pCharacterIndex)
{
	return pCharacterIndex;
}

System::Windows::Media::TextFormatting::TextRun^ CAgentTextDrawWPF::TextStringSource::GetTextRun (int pCharacterIndex)
{
	if	(
			(pCharacterIndex >= 0)
		&&	(pCharacterIndex < mText->Length)
		)
	{
		return gcnew TextCharacters (mText, pCharacterIndex, mText->Length-pCharacterIndex, mTextProperties);
	}
	return gcnew TextEndOfParagraph (1);
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
