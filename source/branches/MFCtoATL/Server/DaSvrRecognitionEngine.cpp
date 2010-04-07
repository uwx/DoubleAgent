/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
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
#include "DaSvrRecognitionEngine.h"
#include "Sapi5Inputs.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrRecognitionEngine::DaSvrRecognitionEngine ()
:	mSapi5Input (NULL)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngine::DaSvrRecognitionEngine (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrRecognitionEngine::~DaSvrRecognitionEngine ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngine::~DaSvrRecognitionEngine (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrRecognitionEngine * DaSvrRecognitionEngine::CreateInstance (CSapi5InputInfo * pInputInfo, LPCTSTR pClientMutexName)
{
	CComObject<DaSvrRecognitionEngine> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrRecognitionEngine>::CreateInstance (&lInstance))))
	{
		lInstance->mSapi5Input = pInputInfo;
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrRecognitionEngine::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void DaSvrRecognitionEngine::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngine::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrRecognitionEngine::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrRecognitionEngine::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrRecognitionEngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrRecognitionEngine), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetSRModeID (BSTR *SRModeID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetSRModeID"), this, m_dwRef);
#endif
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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetSRModeID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetDisplayName (BSTR *DisplayName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetDisplayName"), this, m_dwRef);
#endif
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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetDisplayName"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetManufacturer (BSTR *Manufacturer)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetManufacturer"), this, m_dwRef);
#endif
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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetManufacturer"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetVersion (short *MajorVersion, short *MinorVersion)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetVersion"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(MajorVersion)
	{
		(*MajorVersion) = 5;
	}
	if	(MinorVersion)
	{
		(*MinorVersion) = 0;
	}

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetVersion"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetLanguageID (long *LanguageID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageID"), this, m_dwRef);
#endif
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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageID"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetLanguageName (BSTR *LanguageName, boolean EnglishName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageName"), this, m_dwRef);
#endif
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
			LCTYPE	lInfoType = EnglishName ? LOCALE_SLANGUAGE : LOCALE_SNATIVELANGNAME;
			int 	lInfoSize;
			CString lInfoValue;

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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageName"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetLanguageIDs (SAFEARRAY **LanguageIDs)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageIDs"), this, m_dwRef);
#endif
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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageIDs"), this, m_dwRef);
	}
#endif
	return lResult;
}


HRESULT STDMETHODCALLTYPE DaSvrRecognitionEngine::GetLanguageNames (SAFEARRAY **LanguageNames, boolean EnglishNames)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageNames"), this, m_dwRef);
#endif
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
					int 	lInfoSize;
					CString lInfoValue;

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

	PutServerError (lResult, __uuidof(IDaSvrRecognitionEngine));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrRecognitionEngine::GetLanguageNames"), this, m_dwRef);
	}
#endif
	return lResult;
}
