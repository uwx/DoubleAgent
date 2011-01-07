/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ErrorInfo.h"

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

CErrorInfo::CErrorInfo ()
:	mGuid (GUID_NULL),
	mHelpContext (0)
{
}

CErrorInfo::CErrorInfo (LPCTSTR pDescription, LPCTSTR pSource, const GUID & pGuid, LPCTSTR pHelpFile, DWORD pHelpContext)
:	mDescription (pDescription),
	mSource (pSource),
	mGuid (pGuid),
	mHelpFile (pHelpFile),
	mHelpContext (pHelpContext)
{
}

CErrorInfo::CErrorInfo (HRESULT pError, LPCTSTR pSource, const GUID & pGuid, LPCTSTR pHelpFile, DWORD pHelpContext)
:	mSource (pSource),
	mGuid (pGuid),
	mHelpFile (pHelpFile),
	mHelpContext (pHelpContext)
{
	if	(
			(pError == S_OK)
		||	(pError == S_FALSE)
		)
	{
		mSource.Empty ();
	}
	else
	{
		mDescription = GetSysDescription (pError);
	}
}

CString CErrorInfo::GetSysDescription (HRESULT pError)
{
	CString	lRet;
	LPTSTR	lMessage = NULL;

	FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, pError, 0, (LPTSTR) &lMessage, 0, NULL);
	if	(lMessage)
	{
		lRet = lMessage;
		LocalFree (lMessage);
		lRet.TrimRight (_T("\r\n"));
		lRet.Replace (_T("\r\n"), _T("\n"));
		lRet.Replace (_T('\r'), _T(' '));
		lRet.Replace (_T('\n'), _T(' '));
	}
	else
	{
		lRet.Format (_T("Error 0x%8.8X"), pError);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

HRESULT CErrorInfo::GetErrorInfo ()
{
	HRESULT	lResult;

	try
	{
		IErrorInfoPtr	lErrorInfo;
		tBstrPtr		lBstr;

		if	(
				(SUCCEEDED (lResult = ::GetErrorInfo (0, &lErrorInfo)))
			&&	(lErrorInfo != NULL)
			)
		{
			lErrorInfo->GetGUID (&mGuid);
			lErrorInfo->GetDescription (lBstr.Free());
			mDescription = (BSTR)lBstr;
			lErrorInfo->GetSource (lBstr.Free());
			mSource = (BSTR)lBstr;
			lErrorInfo->GetHelpFile (lBstr.Free());
			mHelpFile = (BSTR)lBstr;
			lErrorInfo->GetHelpContext (&(mHelpContext=0));
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

HRESULT	CErrorInfo::PutErrorInfo ()
{
	HRESULT	lResult;

	try
	{
		if	(
				(mSource.IsEmpty ())
			&&	(mDescription.IsEmpty ())
			)
		{
			lResult = ::SetErrorInfo (0, NULL);
		}
		else
		{
			ICreateErrorInfoPtr	lCreateErrorInfo;
			IErrorInfoPtr		lErrorInfo;

			if	(
					(SUCCEEDED (lResult = ::CreateErrorInfo (&lCreateErrorInfo)))
				&&	(SUCCEEDED (lResult = lCreateErrorInfo->SetGUID (mGuid)))
				&&	(SUCCEEDED (lResult = lCreateErrorInfo->SetDescription (_bstr_t(mDescription))))
				&&	(SUCCEEDED (lResult = lCreateErrorInfo->SetSource (_bstr_t(mSource))))
				)
			{
				lCreateErrorInfo->SetHelpFile (_bstr_t(mHelpFile));
				lCreateErrorInfo->SetHelpContext (mHelpContext);

				lErrorInfo = lCreateErrorInfo;
				lResult = ::SetErrorInfo (0, lErrorInfo);
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

//////////////////////////////////////////////////////////////////////

bool CErrorInfo::HasErrorInfo ()
{
	IErrorInfoPtr	lErrorInfo;

	if	(
			(SUCCEEDED (::GetErrorInfo (0, &lErrorInfo)))
		&&	(lErrorInfo != NULL)
		)
	{
		return true;
	}
	return false;
}

void CErrorInfo::ClearErrorInfo ()
{
	::SetErrorInfo (0, NULL);
}
