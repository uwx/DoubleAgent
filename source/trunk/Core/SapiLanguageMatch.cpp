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
#include "SapiLanguageMatch.h"

//////////////////////////////////////////////////////////////////////

CSapiLanguageMatch::CSapiLanguageMatch ()
{
}

CSapiLanguageMatch::~CSapiLanguageMatch ()
{
}

//////////////////////////////////////////////////////////////////////

void CSapiLanguageMatch::MakeLanguageMatchList (LANGID pLanguageId, CAtlTypeArray <LANGID>& pLanguageIds, bool pUseDefaults) const
{
	pLanguageIds.RemoveAll ();

	if	(pLanguageId)
	{
		if	(IsValidLocale (MAKELCID (pLanguageId, SORT_DEFAULT), LCID_SUPPORTED))
		{
			pLanguageIds.AddUnique (pLanguageId);
		}

		if	(
				(SUBLANGID (pLanguageId) != SUBLANG_DEFAULT)
			&&	(IsValidLocale (MAKELCID (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_DEFAULT), SORT_DEFAULT), LCID_SUPPORTED))
			)
		{
			pLanguageIds.AddUnique (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_DEFAULT));
		}

		if	(
				(SUBLANGID (pLanguageId) != SUBLANG_DEFAULT)
			&&	(IsValidLocale (MAKELCID (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_SYS_DEFAULT), SORT_DEFAULT), LCID_SUPPORTED))
			)
		{
			pLanguageIds.AddUnique (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_SYS_DEFAULT));
		}
	}

	if	(pUseDefaults)
	{
		pLanguageIds.AddUnique (GetUserDefaultUILanguage ());
		pLanguageIds.AddUnique (GetSystemDefaultUILanguage ());
		pLanguageIds.AddUnique (GetUserDefaultLangID ());
		pLanguageIds.AddUnique (GetSystemDefaultLangID ());
		pLanguageIds.AddUnique (MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US));
		pLanguageIds.AddUnique (MAKELANGID (LANG_ENGLISH, SUBLANG_NEUTRAL));
	}
}

INT_PTR CSapiLanguageMatch::FindLanguageMatch (LANGID pLanguageId, const CAtlTypeArray <LANGID>& pLanguageIds) const
{
	INT_PTR	lLanguageNdx = pLanguageIds.Find (pLanguageId);

	if	(
			(lLanguageNdx < 0)
		&&	(SUBLANGID (pLanguageId) != SUBLANG_DEFAULT)
		)
	{
		lLanguageNdx = pLanguageIds.Find (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_DEFAULT));
	}
	if	(
			(lLanguageNdx < 0)
		&&	(SUBLANGID (pLanguageId) != SUBLANG_NEUTRAL)
		)
	{
		lLanguageNdx = pLanguageIds.Find (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_NEUTRAL));
	}
	return lLanguageNdx;
}

//////////////////////////////////////////////////////////////////////

void CSapiLanguageMatch::LogLanguageMatchList (UINT pLogLevel, LANGID pLanguageId, bool pUseDefaults, LPCTSTR pTitle, LPCTSTR pIndent) const
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString				lTitle (pTitle);
			CAtlString				lIndent (pIndent);
			CAtlTypeArray <LANGID>	lLanguageIds;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("LanguageMatching");
			}
			MakeLanguageMatchList (pLanguageId, lLanguageIds, pUseDefaults);
			LogMessage (pLogLevel, _T("%s%s [%u] for [%4.4X] is [%s] defaults [%4.4X] [%4.4X] [%4.4X] [%4.4X]"), lIndent, lTitle, pUseDefaults, pLanguageId, FormatArray (lLanguageIds, _T("%4.4X")), GetUserDefaultUILanguage (), GetSystemDefaultUILanguage (), GetUserDefaultLangID (), GetSystemDefaultLangID ());
		}
		catch AnyExceptionSilent
	}
}
