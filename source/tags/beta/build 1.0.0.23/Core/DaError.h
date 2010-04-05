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
#ifndef DAERROR_H_INCLUDED_
#define DAERROR_H_INCLUDED_
#pragma once

#include <AgtErr.h>
#include "ErrorInfo.h"

//////////////////////////////////////////////////////////////////////

class CServerError : public CErrorInfo
{
public:
	CServerError (HRESULT pResult, const GUID & pGuid = GUID_NULL);

	friend CString DaErrorDescription (HRESULT pResult);
};

//////////////////////////////////////////////////////////////////////

inline HRESULT PutServerError (HRESULT pResult, const GUID & pGuid = GUID_NULL)
{
	if	(FAILED (pResult))
	{
		CServerError (pResult, pGuid).PutErrorInfo ();
	}
	else
	{
		CServerError::ClearErrorInfo ();
	}
	return pResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class CControlError : public CErrorInfo
{
public:
	CControlError (HRESULT pResult, const GUID & pGuid = GUID_NULL);

	friend CString DaErrorDescription (HRESULT pResult);
};

//////////////////////////////////////////////////////////////////////

inline void ClearControlError ()
{
	CControlError::ClearErrorInfo ();
}

inline HRESULT PutControlError (HRESULT pResult, const GUID & pGuid = GUID_NULL)
{
	if	(FAILED (pResult))
	{
		CControlError (pResult, pGuid).PutErrorInfo ();
	}
	return pResult;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline COleDispatchException * DaDispatchException (HRESULT pResult)
{
	COleDispatchException * lException;
	CString					lDescription = DaErrorDescription (pResult);

	if	(lDescription.IsEmpty ())
	{
		lDescription = CErrorInfo::GetSysDescription (pResult);
	}
	lException = new COleDispatchException (DaErrorDescription (pResult), 0, 0);
	lException->m_scError = (SCODE)pResult;
	return lException;
}

//////////////////////////////////////////////////////////////////////

#endif // DAERROR_H_INCLUDED_
