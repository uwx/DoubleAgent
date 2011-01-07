/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCmnSREngines.h"
#include "AgentFile.h"
#include "SapiInputCache.h"
#include "Sapi5Inputs.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnSREngines::CDaCmnSREngines()
{
}

CDaCmnSREngines::~CDaCmnSREngines()
{
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSREngines::UseAllInputs ()
{
	HRESULT				lResult = S_FALSE;
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;

	mSapi5Inputs.RemoveAll ();

	if	(
			(lInputCache = CSapiInputCache::GetStaticInstance ())
		&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs())
		)
	{
		mSapi5Inputs.Copy (*lSapi5Inputs);
		lResult = S_OK;
	}
	return lResult;
}

HRESULT CDaCmnSREngines::UseTheseInputs (CAgentFile * pFile, LANGID pLangId)
{
	HRESULT								lResult = S_FALSE;
	CSapiInputCache *					lInputCache;
	CSapi5Inputs *						lSapi5Inputs;
	tPtr <CSapi5InputInfoArray const>	lInputArray;

	if	(
			(pFile)
		&&	(pLangId == 0)
		)
	{
		pLangId = pFile->GetTts().mLanguage;
	}

	mSapi5Inputs.RemoveAll ();

	if	(
			(lInputCache = CSapiInputCache::GetStaticInstance ())
		&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs())
		&&	(lInputArray = lSapi5Inputs->GetInputs (pLangId, true))
		)
	{
		mSapi5Inputs.Copy (*lInputArray);
		lResult = S_OK;
	}
	return lResult;
}
