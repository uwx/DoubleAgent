#include "StdAfx.h"

#ifdef	GetFirstChild
#undef	GetFirstChild
#endif
#import "C:\\Program Files (x86)\\Common Files\\microsoft shared\\Help\\hxds.dll" no_namespace

#include "HxRegHelper.h"

#ifdef	_DEBUG
#define	_DEBUG_TRANSACTION	LogNormal
#define	_DEBUG_REGISTER		LogNormal
#endif

//////////////////////////////////////////////////////////////////////

CHxRegHelper::CHxRegHelper ()
{
	LogComErr (LogNormal, mLastResult = CoCreateInstance (__uuidof(HxRegisterSession), NULL, CLSCTX_SERVER, __uuidof(IHxRegisterSession), (void**)&mRegisterSession));
}

CHxRegHelper::~CHxRegHelper ()
{
}

//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::GetLastResult () const
{
	return mLastResult;
}

LPCTSTR CHxRegHelper::GetSessionToken () const
{
	return mSessionToken;
}

HRESULT CHxRegHelper::SetSessionToken (LPCTSTR pSessionToken)
{
	mSessionToken = pSessionToken;
	return ContinueTransaction ();
}

//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::CreateTransaction (LPCTSTR pSessionToken)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			mSessionToken = pSessionToken;

			if	(mSessionToken.IsEmpty ())
			{
				lResult = E_INVALIDARG;
			}
			else
			{
#ifdef	_DEBUG_TRANSACTION
				LogMessage (_DEBUG_TRANSACTION, _T("  Create transaction    [%s]"), mSessionToken);
#endif

				mSessionToken = (BSTR) mRegisterSession->CreateTransaction (_bstr_t (mSessionToken));

#ifdef	_DEBUG_TRANSACTION
				LogMessage (_DEBUG_TRANSACTION, _T("  Created transaction   [%s]"), mSessionToken);
#endif
				if	(!mSessionToken.IsEmpty ())
				{
					lResult = S_OK;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

HRESULT CHxRegHelper::CommitTransaction ()
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(
				(mRegisterSession)
			&&	(!mSessionToken.IsEmpty ())
			)
		{
#ifdef	_DEBUG_TRANSACTION
			LogMessage (_DEBUG_TRANSACTION, _T("  Commit transaction    [%s]"), mSessionToken);
#endif

			LogComErr (LogNormal, lResult = mRegisterSession->CommitTransaction ());

#ifdef	_DEBUG_TRANSACTION
			LogComErrAnon (MinLogLevel(_DEBUG_TRANSACTION,LogAlways), lResult, _T("  Committed transaction [%s]"), mSessionToken);
#endif
			mSessionToken.Empty ();
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

HRESULT CHxRegHelper::RevertTransaction ()
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(
				(mRegisterSession)
			&&	(!mSessionToken.IsEmpty ())
			)
		{
#ifdef	_DEBUG_TRANSACTION
			LogMessage (_DEBUG_TRANSACTION, _T("  Revert transaction    [%s]"), mSessionToken);
#endif

			LogComErr (LogNormal, lResult = mRegisterSession->RevertTransaction ());

#ifdef	_DEBUG_TRANSACTION
			LogComErrAnon (MinLogLevel(_DEBUG_TRANSACTION,LogAlways), lResult, _T("  Reverted transaction [%s]"), mSessionToken);
#endif
			mSessionToken.Empty ();
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::PostponeTransaction ()
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(
				(mRegisterSession)
			&&	(!mSessionToken.IsEmpty ())
			)
		{
#ifdef	_DEBUG_TRANSACTION
			LogMessage (_DEBUG_TRANSACTION, _T("  Postpone transaction  [%s]"), mSessionToken);
#endif

			mSessionToken = (BSTR) mRegisterSession->PostponeTransaction ();

#ifdef	_DEBUG_TRANSACTION
			LogMessage (_DEBUG_TRANSACTION, _T("  Postponed transaction [%s]"), mSessionToken);
#endif
			if	(!mSessionToken.IsEmpty ())
			{
				lResult = S_OK;
			}
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

HRESULT CHxRegHelper::ContinueTransaction ()
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(
				(mRegisterSession)
			&&	(!mSessionToken.IsEmpty ())
			)
		{
#ifdef	_DEBUG_TRANSACTION
			LogMessage (_DEBUG_TRANSACTION, _T("  Continue transaction  [%s]"), mSessionToken);
#endif

			LogComErr (LogNormal, lResult = mRegisterSession->ContinueTransaction (_bstr_t(mSessionToken)));

#ifdef	_DEBUG_TRANSACTION
			LogComErrAnon (MinLogLevel(_DEBUG_TRANSACTION,LogAlways), lResult, _T("  Continued transaction [%s]"), mSessionToken);
#endif
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::RegisterNamespace (LPCTSTR pNamespace, LPCTSTR pDescription, LPCTSTR pCollection, LPCTSTR pFileName, LANGID pLangID)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxRegisterPtr	lRegister = mRegisterSession->GetRegistrationObject (HxRegisterSession_IHxRegister);
			CAtlString		lNamespaceName (pNamespace);
			CAtlString		lNamespaceDescription (pDescription);
			CAtlString		lNamespaceCollection (pCollection);
			CAtlString		lNamespaceFileName (PathFindFileName (pFileName));
			CAtlString		lNamespaceFileID (pNamespace);

			while (*PathFindExtension (lNamespaceFileID))
			{
				PathRemoveExtension (lNamespaceFileID.GetBuffer (lNamespaceFileID.GetLength ()));
				lNamespaceFileID.ReleaseBuffer ();
			}

			if	(lRegister)
			{
				CAtlString	lCurrPath;
				CAtlString	lFilePath (pFileName);

				GetCurrentDirectory (MAX_PATH, lCurrPath.GetBuffer (MAX_PATH));
				lCurrPath.ReleaseBuffer ();
				PathRemoveFileSpec (lFilePath.GetBuffer (lFilePath.GetLength ()));
				lFilePath.ReleaseBuffer ();
				SetCurrentDirectory (lFilePath);

				try
				{
#ifdef	_DEBUG_REGISTER
					LogMessage (_DEBUG_REGISTER, _T("  RegisterNamespace [%s] Collection [%s] File [%s] [%s] LangID [%u %4.4X] Description [%s]"), lNamespaceName, lNamespaceCollection, lNamespaceFileID, lNamespaceFileName, pLangID, pLangID, lNamespaceDescription);
#endif
					if	(SUCCEEDED (LogComErrAnon (LogAlways, lResult = lRegister->RegisterNamespace (_bstr_t(lNamespaceName), _bstr_t(lNamespaceCollection), _bstr_t(lNamespaceDescription)), _T("  RegisterNamespace"))))
					{
						LogComErrAnon (LogAlways, lResult = lRegister->RegisterHelpFileSet (_bstr_t(lNamespaceName), _bstr_t(lNamespaceFileID), pLangID, _bstr_t(lNamespaceFileName), _bstr_t(""), _bstr_t(""), _bstr_t(""), 0, 0, 0, 0), _T("  RegisterHelpFileSet"));
					}
				}
				catch AnyExceptionDebug

				SetCurrentDirectory (lCurrPath);
#if	FALSE
				if	(SUCCEEDED (lResult))
				{
					LogComErr (LogNormal, mRegisterSession->CommitTransaction ());
					mSessionToken = (BSTR) mRegisterSession->CreateTransaction (_bstr_t(mSessionToken));
				}
#endif
			}
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::UnregisterNamespace (LPCTSTR pNamespace)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxRegisterPtr	lRegister = mRegisterSession->GetRegistrationObject (HxRegisterSession_IHxRegister);
			CAtlString		lNamespaceName (pNamespace);
			CAtlString		lNamespaceFileID (pNamespace);

			while (*PathFindExtension (lNamespaceFileID))
			{
				PathRemoveExtension (lNamespaceFileID.GetBuffer (lNamespaceFileID.GetLength ()));
				lNamespaceFileID.ReleaseBuffer ();
			}

			if	(lRegister)
			{
#ifdef	_DEBUG_REGISTER
				LogMessage (_DEBUG_REGISTER, _T("  UnregisterNamespace [%s] File [%s]"), lNamespaceName, lNamespaceFileID);
#endif
				try
				{
					LogComErrAnon (LogAlways, lResult = lRegister->RemoveHelpFile (_bstr_t(lNamespaceName), _bstr_t(lNamespaceFileID), 0), _T("  RemoveHelpFile"));
				}
				catch AnyExceptionDebug
				try
				{
					LogComErrAnon (LogAlways, lResult = lRegister->RemoveNamespace (_bstr_t(lNamespaceName)), _T("  RemoveNamespace"));
				}
				catch AnyExceptionDebug

#if	FALSE
				if	(SUCCEEDED (lResult))
				{
					LogComErr (LogNormal, mRegisterSession->CommitTransaction ());
					mSessionToken = (BSTR) mRegisterSession->CreateTransaction (_bstr_t(mSessionToken));
				}
#endif
			}
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::RegisterFilter (LPCTSTR pNamespace, LPCTSTR pFilterName, LPCTSTR pFilterQuery)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxFiltersPtr	lFilters = mRegisterSession->GetRegistrationObject (HxRegisterSession_IHxFilters);

			if	(
					(lFilters)
				&&	(SUCCEEDED (LogComErr (LogNormal, lResult = lFilters->SetNamespace (_bstr_t (pNamespace)))))
				)
			{
#ifdef	_DEBUG_REGISTER
				LogMessage (_DEBUG_REGISTER, _T("  RegisterFilter      [%s] [%s] [%s]"), pNamespace, pFilterName, pFilterQuery);
#endif
				try
				{
					LogComErr (LogAlways, lResult = lFilters->RegisterFilter (pFilterName, pFilterQuery));
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

HRESULT CHxRegHelper::UnregisterFilter (LPCTSTR pNamespace, LPCTSTR pFilterName)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxFiltersPtr	lFilters = mRegisterSession->GetRegistrationObject (HxRegisterSession_IHxFilters);

			if	(
					(lFilters)
				&&	(SUCCEEDED (LogComErr (LogNormal, lResult = lFilters->SetNamespace (_bstr_t (pNamespace)))))
				)
			{
#ifdef	_DEBUG_REGISTER
				LogMessage (_DEBUG_REGISTER, _T("  RemoveFilter        [%s] [%s]"), pNamespace, pFilterName);
#endif
				try
				{
					LogComErr (LogAlways, lResult = lFilters->RemoveFilter (pFilterName));
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

	return (mLastResult = lResult);
}

//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::UnregisterNamespaceFilters (LPCTSTR pNamespace)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxFiltersPtr			lFilters;
#ifdef	_DEBUG_REGISTER
			LogMessage (_DEBUG_REGISTER, _T("  UnregisterFilters   [%s]"), pNamespace);
#endif
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::RegisterPlugin (LPCTSTR pParentNamespace, LPCTSTR pParentFile, LPCTSTR pChildNamespace, LPCTSTR pChildFile)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxPlugInPtr			lPlugIn;
			CAtlString				lParentNamespace (pParentNamespace);
			CAtlString				lParentFile (pParentFile);
			CAtlString				lChildNamespace (pChildNamespace);
			CAtlString				lChildFile (pChildFile);
			IHxRegNamespacePtr		lRegNamespace;

#ifdef	_DEBUG_REGISTER
			LogMessage (_DEBUG_REGISTER, _T("  RegisterPlugin      [%s] [%s] in [%s] [%s]"), pChildNamespace, pChildFile, pParentNamespace, pParentFile);
#endif
			if	(
					(lParentFile.IsEmpty ())
				&&	(GetNamespaceCollection (lParentNamespace, lParentFile, true))
				)
			{
				PathRemoveExtension (lParentFile.GetBuffer (MAX_PATH));
				PathAddExtension (lParentFile.GetBuffer (MAX_PATH), _T(".HxT"));
				lParentFile.ReleaseBuffer ();
			}

			if	(
					(lChildFile.IsEmpty ())
				&&	(GetNamespaceCollection (lChildNamespace, lChildFile, true))
				)
			{
				PathRemoveExtension (lChildFile.GetBuffer (MAX_PATH));
				PathAddExtension (lChildFile.GetBuffer (MAX_PATH), _T(".HxT"));
				lChildFile.ReleaseBuffer ();
			}

			if	(lPlugIn = mRegisterSession->GetRegistrationObject (HxRegisterSession_IHxPlugIn))
			{
				try
				{
					LogComErr (LogAlways, lResult = lPlugIn->RegisterHelpPlugIn (_bstr_t(lParentNamespace), _bstr_t(lParentFile), _bstr_t(lChildNamespace), _bstr_t(lChildFile), _bstr_t(""), 0), _T("  RegisterHelpPlugIn"));
#ifdef	_DEBUG_REGISTER
					LogMessage (_DEBUG_REGISTER, _T("  RegisterHelpPlugIn  [%s] [%s] in [%s] [%s]"), lChildNamespace, lChildFile, lParentNamespace, lParentFile);
#endif
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}
//////////////////////////////////////////////////////////////////////

HRESULT CHxRegHelper::UnregisterPlugin (LPCTSTR pParentNamespace, LPCTSTR pChildNamespace)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(mRegisterSession)
		{
			IHxPlugInPtr			lPlugIn;
			CAtlString				lParentNamespace (pParentNamespace);
			CAtlString				lChildNamespace (pChildNamespace);
			IHxRegNamespacePtr		lRegNamespace;

#ifdef	_DEBUG_REGISTER
			LogMessage (_DEBUG_REGISTER, _T("  UnregisterPlugin    [%s] in [%s]"), pChildNamespace, pParentNamespace);
#endif

			if	(lPlugIn = mRegisterSession->GetRegistrationObject (HxRegisterSession_IHxPlugIn))
			{
				try
				{
					LogComErr (LogAlways, lResult = lPlugIn->RemoveHelpPlugIn (_bstr_t(lParentNamespace), _bstr_t(""), _bstr_t(lChildNamespace), _bstr_t(""), _bstr_t("")), _T("RemoveHelpPlugIn"));
#ifdef	_DEBUG_REGISTER
					LogMessage (_DEBUG_REGISTER, _T("  RemoveHelpPlugIn    [%s] in [%s]"), lChildNamespace, lParentNamespace);
#endif
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

IHxRegNamespacePtr CHxRegHelper::GetRegNamespace (LPCTSTR pNamespace)
{
	IHxRegNamespacePtr	lRet;

	try
	{
		IHxRegNamespaceListPtr	lRegNamespaceList;
		IHxRegNamespacePtr		lRegNamespace;
		long					lCount;
		long					lNdx;

		if	(
				(
					(mRegistryWalker != NULL)
				||	(SUCCEEDED (LogComErr (LogNormal, CoCreateInstance (__uuidof(HxRegistryWalker), NULL, CLSCTX_SERVER, __uuidof(IHxRegistryWalker), (void**)&mRegistryWalker))))
				)
			&&	(mNamespaceList = mRegistryWalker->RegisteredNamespaceList [_bstr_t(pNamespace)])
			&&	(lCount = mNamespaceList->Count)
			)
		{
			for	(lNdx = 1; lNdx <= lCount; lNdx++)
			{
				if	(lRegNamespace = mNamespaceList->ItemAt (lNdx))
				{
					CAtlString	lNamespaceName ((BSTR)lRegNamespace->Name);

					if	(lNamespaceName.Compare (pNamespace) == 0)
					{
						lRet = lRegNamespace;
						break;
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

bool CHxRegHelper::GetNamespaceCollection (LPCTSTR pNamespace, CAtlString& pCollectionPath, bool pStripPath)
{
	bool	lRet = false;

	pCollectionPath.Empty ();

	try
	{
		IHxRegNamespacePtr	lRegNamespace;
		_variant_t			lProperty;

		if	(lRegNamespace = GetRegNamespace (pNamespace))
		{
			lProperty = lRegNamespace->GetProperty (HxRegNamespaceCollection);
			if	(V_VT (&lProperty) == VT_BSTR)
			{
				pCollectionPath = V_BSTR (&lProperty);
				if	(pStripPath)
				{
					PathStripPath (pCollectionPath.GetBuffer (pCollectionPath.GetLength ()));
					pCollectionPath.ReleaseBuffer ();
				}
				lRet = true;
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}
