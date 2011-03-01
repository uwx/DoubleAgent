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
#include "DaCmnSREngine.h"
#include "AgentFile.h"
#include "Sapi5Inputs.h"
#include "Sapi5Input.h"
#include "SapiInputCache.h"
#include "MallocPtr.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnSREngine::CDaCmnSREngine ()
:	mSapi5Input (NULL)
{
}

CDaCmnSREngine::~CDaCmnSREngine ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnSREngine::Initialize (CSapi5InputInfo * pInputInfo)
{
	mSapi5Input = pInputInfo;
}

bool CDaCmnSREngine::Initialize (CSapi5Input * pInput)
{
	bool				lRet = false;
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;
	CSapi5InputInfo *	lSapi5Input;

	if	(
			(pInput)
		&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
		&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs ())
		&&	(lSapi5Input = lSapi5Inputs->GetEngineId (CAtlString (pInput->GetEngineId ())))
		)
	{
		Initialize (lSapi5Input);
		lRet = true;
	}
	return lRet;
}

bool CDaCmnSREngine::Initialize (CAgentFile * pFile)
{
	bool				lRet = false;
	CSapiInputCache *	lInputCache;
	CSapi5Inputs *		lSapi5Inputs;
	CSapi5InputInfo *	lSapi5Input;

	if	(
			(pFile)
		&&	(lInputCache = CSapiInputCache::GetStaticInstance ())
		)
	{
		tPtr <CSapi5Input>	lSapiInput;

		if	(
				(lSapiInput = lInputCache->GetAgentInput (pFile->GetTts().mLanguage, true, false))
			&&	(lSapi5Inputs = lInputCache->GetSapi5Inputs ())
			&&	(lSapi5Input = lSapi5Inputs->GetEngineId (CAtlString (lSapiInput->GetEngineId ())))
			)
		{
			Initialize (lSapi5Input);
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSREngine::get_SRModeID (BSTR *SRModeID)
{
	HRESULT	lResult = S_OK;

	if	(!SRModeID)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Input)
	{
		(*SRModeID) = SysAllocString (mSapi5Input->mEngineIdShort);
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnSREngine::get_DisplayName (BSTR *DisplayName)
{
	HRESULT	lResult = S_OK;

	if	(!DisplayName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Input)
	{
		(*DisplayName) = SysAllocString (mSapi5Input->mEngineName);
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnSREngine::get_Manufacturer (BSTR *Manufacturer)
{
	HRESULT	lResult = S_OK;

	if	(!Manufacturer)
	{
		lResult = E_POINTER;
	}
	else
	if	(mSapi5Input)
	{
		(*Manufacturer) = SysAllocString (mSapi5Input->mManufacturer);
	}
	else
	{
		lResult = E_FAIL;
	}
	return lResult;
}

HRESULT CDaCmnSREngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
	HRESULT	lResult = S_OK;

	if	(MajorVersion)
	{
		(*MajorVersion) = 5;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = 0;
	}
	return lResult;
}

HRESULT CDaCmnSREngine::get_LanguageID (long *LanguageID)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageID)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageID) = 0;

		if	(mSapi5Input)
		{
			(*LanguageID) = (long)mSapi5Input->mLangId;
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	return lResult;
}

HRESULT CDaCmnSREngine::get_LanguageName (VARIANT_BOOL EnglishName, BSTR *LanguageName)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageName)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageName) = NULL;

		if	(mSapi5Input)
		{
			LCTYPE		lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
			int			lInfoSize;
			CAtlString	lInfoValue;

			if	(lInfoSize = GetLocaleInfo (MAKELCID (mSapi5Input->mLangId, SORT_DEFAULT), lInfoType, NULL, 0))
			{
				GetLocaleInfo (MAKELCID (mSapi5Input->mLangId, SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
			}
			else
			{
				lResult = S_FALSE;
			}
			lInfoValue.ReleaseBuffer ();
			(*LanguageName) = lInfoValue.AllocSysString ();
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnSREngine::get_LanguageIDs (SAFEARRAY **LanguageIDs)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageIDs)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageIDs) = NULL;

		if	(mSapi5Input)
		{
			if	((*LanguageIDs) = SafeArrayCreateVector (VT_I4, 0, mSapi5Input->mLangIdCount))
			{
				long	lNdx;
				long	lLanguageID;

				for	(lNdx = 0; lNdx < (long)mSapi5Input->mLangIdCount; lNdx++)
				{
					lLanguageID = (long)mSapi5Input->mLangIdSupported [lNdx];
					SafeArrayPutElement (*LanguageIDs, &lNdx, &lLanguageID);
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	return lResult;
}


HRESULT CDaCmnSREngine::get_LanguageNames (VARIANT_BOOL EnglishNames, SAFEARRAY **LanguageNames)
{
	HRESULT	lResult = S_OK;

	if	(!LanguageNames)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*LanguageNames) = NULL;

		if	(mSapi5Input)
		{
			if	((*LanguageNames) = SafeArrayCreateVector (VT_BSTR, 0, mSapi5Input->mLangIdCount))
			{
				LCTYPE	lInfoType = EnglishNames ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
				long	lNdx;

				for	(lNdx = 0; lNdx < (long)mSapi5Input->mLangIdCount; lNdx++)
				{
					int			lInfoSize;
					CAtlString	lInfoValue;

					if	(lInfoSize = GetLocaleInfo (MAKELCID (mSapi5Input->mLangIdSupported [lNdx], SORT_DEFAULT), lInfoType, NULL, 0))
					{
						GetLocaleInfo (MAKELCID (mSapi5Input->mLangIdSupported [lNdx], SORT_DEFAULT), lInfoType, lInfoValue.GetBuffer (lInfoSize), lInfoSize);
					}
					lInfoValue.ReleaseBuffer ();
					SafeArrayPutElement (*LanguageNames, &lNdx, tBstrPtr(lInfoValue.AllocSysString()));
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	return lResult;
}
