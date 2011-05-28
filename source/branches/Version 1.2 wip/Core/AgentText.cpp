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
#include "AgentText.h"
#ifdef	__cplusplus_cli__NOT
using namespace System;
using namespace System::Drawing;
#else
#include "StringArrayEx.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif
#endif

#ifndef	__cplusplus_cli__NOT
#ifdef	_DEBUG
#define	_DEBUG_TEXT		(GetProfileDebugInt(_T("DebugSpeechText"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
//#define	_DEBUG_CACHE	LogDebugFast
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli__NOT
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
#else	// __cplusplus_cli__NOT
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
#endif	// __cplusplus_cli__NOT
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli__NOT
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
			mSpeechWords = safe_cast <array<String^>^> (mSpeechWords->Clone ());
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
#else	// __cplusplus_cli__NOT
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
#endif	// __cplusplus_cli__NOT
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
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

#ifdef	__cplusplus_cli__NOT
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

#ifdef	__cplusplus_cli__NOT
#else
const DWORD		CAgentTextDraw::mDefaultAutoPaceTime = 300;
const INT_PTR	CAgentTextDraw::mDefaultLookAhead = 2;
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli__NOT

CAgentTextDraw::CAgentTextDraw ()
:	mText (gcnew CAgentText ())
{
}

CAgentTextDraw::CAgentTextDraw (System::UInt32 pSapiVersion)
:	mText (gcnew CAgentText (pSapiVersion))
{
}

CAgentTextDraw::CAgentTextDraw (CAgentText^ pText)
:	mText (gcnew CAgentText (pText))
{
}

CAgentTextDraw::CAgentTextDraw (CAgentText^ pText, System::UInt32 pSapiVersion)
:	mText (gcnew CAgentText (pText, pSapiVersion))
{
}

CAgentTextDraw::CAgentTextDraw (CAgentTextDraw^ pText)
:	mText (gcnew CAgentText (pText->mText))
{
}

CAgentTextDraw::CAgentTextDraw (CAgentTextDraw^ pText, UINT pSapiVersion)
:	mText (gcnew CAgentText (pText->mText, pSapiVersion))
{
}

/////////////////////////////////////////////////////////////////////////////
#else	// __cplusplus_cli__NOT
/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw::CAgentTextDraw (UINT pSapiVersion)
:	CAgentText (pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::CAgentTextDraw (const CAgentText& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::CAgentTextDraw (const CAgentTextDraw& pText, UINT pSapiVersion)
:	CAgentText (pText, pSapiVersion)
{
	ResetState (true);
}

CAgentTextDraw::~CAgentTextDraw ()
{
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli__NOT
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli__NOT

CAgentTextDraw^ CAgentTextDraw::operator= (CAgentText^ pText)
{
	mText->operator= (pText);
	ResetState (true);
	DisplayFirstWord ();
	return this;
}

CAgentTextDraw^ CAgentTextDraw::operator+= (CAgentText^ pText)
{
	mText->operator+= (pText);
	ResetState (false);
	return this;
}

CAgentTextDraw^ CAgentTextDraw::operator= (CAgentTextDraw^ pText)
{
	CTextWrap::operator= (pText);
	mText->operator= (pText->mText);
	ResetState (true);

	mWordDisplayed = pText->mWordDisplayed;
	mScrollPos = pText->mScrollPos;
	mScrollInc = pText->mScrollInc;
	mScrollMin = pText->mScrollMin;
	mScrollMax = pText->mScrollMax;
	return this;
}

CAgentTextDraw^ CAgentTextDraw::operator+= (CAgentTextDraw^ pText)
{
	mText->operator+= (pText->mText);
	ResetState (false);
	return this;
}

/////////////////////////////////////////////////////////////////////////////
#else	//   __cplusplus_cli__NOT
/////////////////////////////////////////////////////////////////////////////
CAgentTextDraw& CAgentTextDraw::operator= (const CAgentText& pText)
{
	CAgentText::operator= (pText);
	ResetState (true);
	DisplayFirstWord ();
	return *this;
}

CAgentTextDraw& CAgentTextDraw::operator+= (const CAgentText& pText)
{
	CAgentText::operator+= (pText);
	ResetState (false);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

CAgentTextDraw& CAgentTextDraw::operator= (const CAgentTextDraw& pText)
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

CAgentTextDraw& CAgentTextDraw::operator+= (const CAgentTextDraw& pText)
{
	CAgentText::operator+= (pText);
	ResetState (false);
	return *this;
}
/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli__NOT
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
void CAgentTextDraw::ResetState (Boolean pFullReset)
#else
void CAgentTextDraw::ResetState (bool pFullReset)
#endif
{
#ifdef	__cplusplus_cli__NOT
	mBounds = RectangleF::Empty;
#else
	mBounds.SetRectEmpty ();
#endif	

	mTextCacheLimit = 5;
	mTextCacheStart = 0;
#ifdef	__cplusplus_cli__NOT
	mTextCache = gcnew System::Collections::Generic::List <System::String^> (mTextCacheLimit);
#else
	mTextCache.DeleteAll ();
#endif	

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

#ifdef	__cplusplus_cli__NOT
System::String^ CAgentTextDraw::GetDisplayText ()
{
	return GetDisplayText (0);
}

System::String^ CAgentTextDraw::GetDisplayText (int pLookAhead)
#else
CAtlString CAgentTextDraw::GetDisplayText (INT_PTR pLookAhead) const
#endif
{
#ifdef	__cplusplus_cli__NOT
	String^		lDisplayText;
#else	
	CAtlString	lDisplayText;
#endif	
//
//	The caching is primitive but does the job.  It assumes that display text will be accessed
//	with constantly growing length.
//
#ifdef	__cplusplus_cli__NOT
	if	(
			(mText->mTextWords)
		&&	(mText->mTextWords->Length > 0)
		&&	(mWordDisplayed	< mText->mTextWords->Length)
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
#ifdef	__cplusplus_cli__NOT
			int			lWordDisplayed = Math::Min (mWordDisplayed + pLookAhead, mText->mTextWords->Length-1);
			int			lTextCacheNdx;
#else
			INT_PTR		lWordDisplayed = min (mWordDisplayed + pLookAhead, (INT_PTR)mTextWords.GetCount()-1);
			INT_PTR		lTextCacheNdx;
			CAtlString*	lTextCache;
#endif			

#ifdef	__cplusplus_cli__NOT
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
#ifdef	__cplusplus_cli__NOT
				if	(
						(lTextCacheNdx < mTextCache->Count)
					&&	(mTextCache [lTextCacheNdx])
					)
				{
					lDisplayText = gcnew String (mTextCache [lTextCacheNdx]);
				}
				else
				{
					lDisplayText = String::Join ("", mText->mTextWords, 0, lWordDisplayed);
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
#ifdef	__cplusplus_cli__NOT
				lDisplayText = String::Join ("", mText->mTextWords, 0, lWordDisplayed);
#else
				lDisplayText = JoinStringArray (mTextWords, _T(""), false, 0, lWordDisplayed);
#endif				
			}
		}
	}
	else
	{
#ifdef	__cplusplus_cli__NOT
		lDisplayText = gcnew String (mText->mText);
#else
		lDisplayText = (LPCTSTR)mText;
#endif		
	}
	return lDisplayText;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
System::String^ CAgentTextDraw::GetDisplayWord (int pWordNdx)
{
	if	(
			(mText->mTextWords)
		&&	(pWordNdx >= 0)
		&&	(pWordNdx < mText->mTextWords->Length)
		)
	{
		return mText->mTextWords [pWordNdx];
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

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::DisplayFirstWord ()
{
	return DisplayFirstWord (false);
}

System::Boolean CAgentTextDraw::DisplayFirstWord (System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayFirstWord (bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli__NOT
	if	(mText->mTextWords)
	{
		mWordDisplayed = Math::Min (0, mText->mTextWords->Length-1);
	}
#else
	mWordDisplayed = min (0, (INT_PTR)mTextWords.GetCount()-1);
#endif

#ifdef	__cplusplus_cli__NOT
	if	(pForSpeech)
	{
		mWordDisplayed = Math::Max (mWordDisplayed-1, -1);
		if	(
				(mText->mSpeechWords)
			&&	(mText->mSpeechWords->Length > 0)
			)
		{
			mWordDisplayed = Math::Max (mWordDisplayed, mText->mTextWords->Length - mText->mSpeechWords->Length);
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
	
#ifdef	__cplusplus_cli__NOT
	if	(mText->mTextWords)
	{
		while	(
					(mWordDisplayed >= 0)
				&&	(mWordDisplayed < mText->mTextWords->Length-1)
				&&	(String::IsNullOrEmpty (mText->mTextWords [mWordDisplayed]))
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
	
#ifdef	__cplusplus_cli__NOT
	mBounds = RectangleF::Empty;
#else
	mBounds.SetRectEmpty ();
#endif
	return (mWordDisplayed >= 0);
}

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::DisplayNextWord ()
{
	return DisplayNextWord (false);
}

System::Boolean CAgentTextDraw::DisplayNextWord (System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayNextWord (bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli__NOT
	if	(
			(mText->mTextWords)
		&&	(mText->mTextWords->Length > 0)
		&&	(mWordDisplayed < mText->mTextWords->Length-1)
		)
#else
	if	(
			(mTextWords.GetCount() > 0)
		&&	(mWordDisplayed < (INT_PTR)mTextWords.GetCount()-1)
		)
#endif		
	{
#ifdef	__cplusplus_cli__NOT
		if	(
				(pForSpeech)
			&&	(mText->mSpeechWords)
			&&	(mText->mSpeechWords->Length > 0)
			)
		{
			mWordDisplayed = Math::Max (mWordDisplayed, mText->mTextWords->Length - mText->mSpeechWords->Length);
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

#ifdef	__cplusplus_cli__NOT
		if	(mText->mTextWords)
		{
			while	(
						(mWordDisplayed >= 0)
					&&	(mWordDisplayed < mText->mTextWords->Length-1)
					&&	(String::IsNullOrEmpty (mText->mTextWords [mWordDisplayed]))
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

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::DisplayThisWord (System::Int32 pWordPos, System::Int32 pWordLength)
{
	return DisplayThisWord (pWordPos, pWordLength, true);
}

System::Boolean CAgentTextDraw::DisplayThisWord (System::Int32 pWordPos, System::Int32 pWordLength, System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayThisWord (long pWordPos, long pWordLength, bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli__NOT
	int				lWordNdx;
	System::Int32	lWordEnd = pWordPos + pWordLength;
#else
	INT_PTR			lWordNdx;
	long			lWordEnd = pWordPos + pWordLength;
#endif	

	if	(pForSpeech)
	{
#ifdef	__cplusplus_cli__NOT
		if	(mText->mSpeechWords)
		{
			for	(lWordNdx = 0; lWordNdx < mText->mSpeechWords->Length; lWordNdx++)
			{
				lWordEnd -= mText->mSpeechWords [lWordNdx]->Length;
				if	(lWordEnd <= 0)
				{
					break;
				}
			}
			lWordNdx += mText->mTextWords->Length - mText->mSpeechWords->Length;
		}
		if	(mText->mTextWords)
		{
			lWordNdx = Math::Min (lWordNdx, mText->mTextWords->Length-1);
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
#ifdef	__cplusplus_cli__NOT
		if	(mText->mTextWords)
		{
			for	(lWordNdx = 0; lWordNdx < mText->mTextWords->Length; lWordNdx++)
			{
				lWordEnd -= mText->mTextWords [lWordNdx]->Length;
				if	(lWordEnd <= 0)
				{
					break;
				}
			}
			lWordNdx = Math::Min (lWordNdx, mText->mTextWords->Length-1);
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

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::DisplayThisWord (int pWordNdx)
{
	return DisplayThisWord (pWordNdx, true);
}

System::Boolean CAgentTextDraw::DisplayThisWord (int pWordNdx, System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayThisWord (INT_PTR pWordNdx, bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli__NOT
	if	(
			(mText->mTextWords)
		&&	(pWordNdx >= 0)
		&&	(pWordNdx < mText->mTextWords->Length)
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

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::DisplayAllWords ()
{
	return DisplayAllWords (false);
}
System::Boolean CAgentTextDraw::DisplayAllWords (System::Boolean pForSpeech)
#else
bool CAgentTextDraw::DisplayAllWords (bool pForSpeech)
#endif
{
#ifdef	__cplusplus_cli__NOT
	if	(mWordDisplayed != ((mText->mTextWords) ? mText->mTextWords->Length : 0)-1)
	{
		mWordDisplayed = ((mText->mTextWords) ? mText->mTextWords->Length : 0)-1;
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
Boolean CAgentTextDraw::CanPace::get ()
{
	return (WordCount > 0);
}
#else
bool CAgentTextDraw::CanPace () const
{
	return (GetWordCount() > 0);
}
#endif

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::CanScroll (System::Drawing::RectangleF pTextBounds)
{
	return (mBounds.Bottom > pTextBounds.Bottom);
}
#else
bool CAgentTextDraw::CanScroll (const CRect& pTextBounds) const
{
	return (mBounds.bottom > pTextBounds.bottom);
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
System::Int32 CAgentTextDraw::ScrollPos::get() {return mScrollPos;}
System::Int32 CAgentTextDraw::ScrollInc::get() {return mScrollInc;}
System::Int32 CAgentTextDraw::ScrollMin::get() {return mScrollMin;}
System::Int32 CAgentTextDraw::ScrollMax::get() {return mScrollMax;}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
//TODO
#else
DWORD CAgentTextDraw::CalcScroll (const CRect& pTextBounds, long& pScrollInc, long& pScrollMin, long& pScrollMax, bool pClipLines, DWORD pMaxLineTime) const
{
	DWORD	lScrollTime = 0;

	if	(mBounds.bottom > pTextBounds.bottom)
	{
		long	lScrollScale;
		long	lScrollInc;

		pScrollMin = pClipLines ? GetLineRect(0).bottom - pTextBounds.bottom : 0;
		pScrollMax = mBounds.Height() - pTextBounds.Height();
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
#endif

#ifdef	__cplusplus_cli__NOT
//TODO
#else
DWORD CAgentTextDraw::InitScroll (const CRect& pTextBounds, bool pForceReinit, bool pClipLines, DWORD pMaxLineTime)
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
#endif

#ifdef	__cplusplus_cli__NOT
//TODO
#else
bool CAgentTextDraw::ApplyScroll (const CRect& pTextBounds, CRect* pClipRect)
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
#endif

#ifdef	__cplusplus_cli__NOT
//TODO
#else
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
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
System::Drawing::SizeF CAgentTextDraw::CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine, System::UInt16 pLines)
{
	System::Drawing::SizeF	lTextSize;
	HDC						lTempDC = CreateCompatibleDC (0);
	Graphics^				lGraphics = Graphics::FromHdc ((IntPtr)lTempDC);

	try
	{	
		lTextSize = lGraphics->MeasureString ("x", pFont);
		lTextSize.Width *= (Single)pPerLine;
//TODO
//		lRet.cy = (long)(short)pLines * (lTextMetrics.tmAscent+lTextMetrics.tmDescent);
	}
	catch AnyExceptionSilent
	
	lGraphics->~Graphics();
	DeleteDC (lTempDC);
	return lTextSize;
}
#else
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
#endif

#ifdef	__cplusplus_cli__NOT
System::Drawing::SizeF CAgentTextDraw::CalcTextSize (System::Drawing::Font^ pFont, System::UInt16 pPerLine)
{
	System::Drawing::SizeF	lTextSize;
	HDC						lTempDC = CreateCompatibleDC (0);
	Graphics^				lGraphics = Graphics::FromHdc ((IntPtr)lTempDC);

	try
	{	
		lTextSize = lGraphics->MeasureString ("x", pFont);
		lTextSize.Width *= (Single)pPerLine;
		mBounds.X = 0;
		mBounds.Y = 0;
		mBounds.Width = lTextSize.Width;
		mBounds.Height = Single::MaxValue;
		lTextSize.Height = MeasureText (mText->FullText, lGraphics, pFont).Height;
		mBounds.Width = 0;
	}
	catch AnyExceptionSilent
	
	lGraphics->~Graphics();
	DeleteDC (lTempDC);
	return lTextSize;
}
#else
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
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli__NOT
System::Boolean CAgentTextDraw::IsBreakChar (System::String^ pText, int pNdx, UINT pPriority, System::Boolean% pBreakAfter)
{
	if	(
			(mText->mTextWords)
		&&	(mText->mTextWords->Length > 0)
		)
	{
		int	lCharNdx = pNdx;
		int	lWordNdx;

		for	(lWordNdx = 0; (lCharNdx > 0) && (lWordNdx < mText->mTextWords->Length); lWordNdx++)
		{
			lCharNdx -= mText->mTextWords [lWordNdx]->Length;
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