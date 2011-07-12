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
#include <shlwapi.h>
#include "Registry.h"
#include "StringArrayEx.h"
#include "Log.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////

CRegKeyEx::CRegKeyEx ()
:	mKey (NULL),
	mReadOnly (false)
{
}

CRegKeyEx::CRegKeyEx (const CRegKeyEx& pKey)
:	mKey (NULL),
	mReadOnly (pKey.mReadOnly)
{
	Reopen (pKey, mReadOnly);
}

CRegKeyEx::CRegKeyEx (const CRegKeyEx& pParent, LPCTSTR pName)
:	mKey (NULL),
	mReadOnly (pParent.mReadOnly)
{
	Open (pParent, pName, mReadOnly);
}

CRegKeyEx::CRegKeyEx (HKEY pParent, LPCTSTR pName, bool pReadOnly, bool pCreate, bool pAuthorize)
:	mKey (NULL),
	mReadOnly (pReadOnly)
{
	Open (pParent, pName, mReadOnly, pCreate, pAuthorize);
}

CRegKeyEx::~CRegKeyEx ()
{
	Close ();
}

//////////////////////////////////////////////////////////////////////

long CRegKeyEx::Open (HKEY pParent, LPCTSTR pName, bool pReadOnly, bool pCreate, bool pAuthorize)
{
	long		lRet = ERROR_INVALID_PARAMETER;
	CRegKeyEx	lOldKey;

	lOldKey.mKey = mKey;
	lOldKey.mName = mName;
	lOldKey.mReadOnly = mReadOnly;

	mKey = NULL;
	mName = pName;
	mReadOnly = false;

	if	(pParent)
	{
		if	(pReadOnly)
		{
			mReadOnly = true;
			lRet = RegOpenKeyEx (pParent, pName, 0, KEY_READ, &mKey);
		}
		else
		if	(
				(pCreate)
			&&	(pName)
			&&	(*pName)
			)
		{
			DWORD lDisposition;
			if	(pAuthorize)
			{
				lRet = RegCreateKeyEx (pParent, pName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE|WRITE_DAC|WRITE_OWNER, NULL, &mKey, &lDisposition);
				if	(lRet == ERROR_ACCESS_DENIED)
				{
					lRet = RegCreateKeyEx (pParent, pName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE|WRITE_DAC, NULL, &mKey, &lDisposition);
				}
				if	(lRet == ERROR_ACCESS_DENIED)
				{
					lRet = RegCreateKeyEx (pParent, pName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE|WRITE_OWNER, NULL, &mKey, &lDisposition);
				}
			}
			else
			{
				lRet = RegCreateKeyEx (pParent, pName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &mKey, &lDisposition);
			}
		}
		else
		{
			if	(pAuthorize)
			{
				lRet = RegOpenKeyEx (pParent, pName, 0, KEY_READ|KEY_WRITE|WRITE_DAC|WRITE_OWNER, &mKey);
				if	(lRet == ERROR_ACCESS_DENIED)
				{
					lRet = RegOpenKeyEx (pParent, pName, 0, KEY_READ|KEY_WRITE|WRITE_DAC, &mKey);
				}
				if	(lRet == ERROR_ACCESS_DENIED)
				{
					lRet = RegOpenKeyEx (pParent, pName, 0, KEY_READ|KEY_WRITE|WRITE_OWNER, &mKey);
				}
			}
			else
			{
				lRet = RegOpenKeyEx (pParent, pName, 0, KEY_READ|KEY_WRITE, &mKey);
			}
		}
	}

	return lRet;
}

long CRegKeyEx::Reopen (const CRegKeyEx& pKey, bool pReadOnly, bool pDeleteOnly)
{
	long		lRet = ERROR_INVALID_PARAMETER;
	HKEY		lKey = pKey;
	CRegKeyEx	lOldKey;

	lOldKey.mKey = mKey;
	lOldKey.mName = mName;
	lOldKey.mReadOnly = mReadOnly;

	mKey = NULL;
	mName = pKey.mName;
	mReadOnly = false;

	if	(
			(lKey)
		&&	(!mName.IsEmpty ())
		)
	{
		if	(pReadOnly)
		{
			mReadOnly = true;
			lRet = RegOpenKeyEx (lKey, NULL, 0, KEY_READ, &mKey);

			if	(lRet == ERROR_INVALID_HANDLE)
			{
				mKey = lKey;
				lRet = NOERROR;
			}
		}
		else
		if	(pDeleteOnly)
		{
			lRet = RegOpenKeyEx (lKey, NULL, 0, DELETE, &mKey);
		}
		else
		{
			lRet = RegOpenKeyEx (lKey, NULL, 0, KEY_READ | KEY_WRITE, &mKey);

			if	(lRet == ERROR_INVALID_HANDLE)
			{
				mKey = lKey;
				mReadOnly = true;
				lRet = NOERROR;
			}
			else
			if	(
					(lRet != NOERROR)
				&&	(RegOpenKeyEx (lKey, NULL, 0, KEY_READ, &mKey) == NOERROR)
				)
			{
				lRet = NOERROR;
				mReadOnly = true;
			}
		}
	}
	else
	{
		mKey = lKey;
		mReadOnly = pReadOnly;
	}

	return lRet;
}

long CRegKeyEx::Close ()
{
	if	(mKey)
	{
		HKEY lKey = mKey;
		mKey = NULL;
		return RegCloseKey (lKey);
	}
	else
	{
		return ERROR_SUCCESS;
	}
}

//////////////////////////////////////////////////////////////////////

bool CRegKeyEx::IsEmpty () const
{
	DWORD	lKeyCount = 0;
	DWORD	lValueCount = 0;

	if	(
			(mKey)
		&&	(RegQueryInfoKey (mKey, NULL, NULL, NULL, &lKeyCount, NULL, NULL, &lValueCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		&&	(
				(lKeyCount == 0)
			&&	(lValueCount == 0)
			)
		)
	{
		return true;
	}
	return false;
}

long CRegKeyEx::Delete ()
{
	long	lRet = ERROR_INVALID_HANDLE;

	if	(mKey)
	{
		if	(
				(mReadOnly)
			||	(mName.IsEmpty ())
			)
		{
			return ERROR_ACCESS_DENIED;
		}
		else
		{
			long	lError1;
			long	lError2;

			if	(
					((lError1 = SHDeleteKey (mKey, NULL)) == ERROR_SUCCESS)
				|	((lError2 = RegDeleteKey (mKey, _T(""))) == ERROR_SUCCESS)
				)
			{
				lRet = ERROR_SUCCESS;
				mKey = NULL;
			}
			else
			{
				lRet = (lError1 == ERROR_SUCCESS) ? lError2 : lError1;
				LogWinErr (LogNormal|LogTime, lRet, _T("DeleteKey"));
			}
		}
	}

	return	lRet;
}

long CRegKeyEx::Empty ()
{
	long	lRet = ERROR_INVALID_HANDLE;

	if	(mKey)
	{
		if	(
				(mReadOnly)
			||	(mName.IsEmpty ())
			)
		{
			return ERROR_ACCESS_DENIED;
		}
		else
		{
			if	((lRet = SHDeleteKey (mKey, NULL)) != ERROR_SUCCESS)
			{
				LogWinErr (LogNormal|LogTime, lRet, _T("SHDeleteKey"));
			}
		}
	}

	return	lRet;
}

//////////////////////////////////////////////////////////////////////

HKEY CRegKeyEx::Attach (HKEY pKey)
{
	Close ();
	mKey = pKey;
	mName.Empty ();
	return mKey;
}

HKEY CRegKeyEx::Detach ()
{
	HKEY lRet = mKey;
	mKey = NULL;
	mName.Empty ();
	return lRet;
}

//////////////////////////////////////////////////////////////////////

long CRegKeyEx::KeyCount () const
{
	DWORD	lKeyCount = 0;

	if	(
			(mKey)
		&&	(RegQueryInfoKey (mKey, NULL, NULL, NULL, &lKeyCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		)
	{
		return (long) lKeyCount;
	}
	return 0;
}

CRegKeyEx * CRegKeyEx::operator [] (long pNdx)
{
	tPtr <CRegKeyEx>	lRet;
	TCHAR				lSubKey [MAX_PATH+1];
	DWORD				lSubKeySize;

	if	(
			(mKey)
		&&	(RegEnumKeyEx (mKey, pNdx, lSubKey, &(lSubKeySize = sizeof (lSubKey) / sizeof (TCHAR)), 0, NULL, NULL, NULL) == ERROR_SUCCESS)
		&&	(lRet = new CRegKeyEx ())
		&&	(lRet->Open (mKey, lSubKey, mReadOnly) == ERROR_SUCCESS)
		)
	{
		return lRet.Detach ();
	}
	return NULL;
}

long CRegKeyEx::ValueCount () const
{
	DWORD	lValueCount = 0;

	if	(
			(mKey)
		&&	(RegQueryInfoKey (mKey, NULL, NULL, NULL, NULL, NULL, NULL, &lValueCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		)
	{
		return (long) lValueCount;
	}
	return 0;
}

CRegValue * CRegKeyEx::operator () (long pNdx)
{
	TCHAR	lValueName [MAX_PATH+1];
	DWORD	lValueNameSize;
	DWORD	lValueType = REG_NONE;
	DWORD	lValueSize;

	if	(
			(mKey)
		&&	(RegEnumValue (mKey, pNdx, lValueName, &(lValueNameSize = sizeof (lValueName) / sizeof (TCHAR)), 0, &lValueType, NULL, &(lValueSize = 0)) == ERROR_SUCCESS)
		)
	{
		switch (lValueType)
		{
			case REG_SZ:
			case REG_EXPAND_SZ:
			{
				return new CRegString (mKey, pNdx);
			}	break;
			case REG_MULTI_SZ:
			{
				return new CRegStrings (mKey, pNdx);
			}	break;
			case REG_DWORD:
			{
				return new CRegDWord (mKey, pNdx);
			}	break;
			case REG_QWORD:
			{
				return new CRegQWord (mKey, pNdx);
			}	break;
			case REG_BINARY:
			{
				if	(lValueSize == sizeof (DWORD))
				{
					return new CRegDWord (mKey, pNdx);
				}
				else
				{
					return new CRegBinary (mKey, pNdx);
				}
			}	break;
			default:
			{
				return new CRegValue (mKey, lValueName, lValueType);
			}
		}
	}
	return NULL;
}

CRegString CRegKeyEx::Value (bool pExpanded) const
{
	if	(pExpanded)
	{
		return CRegString (*this).Expand ();
	}
	else
	{
		return CRegString (*this);
	}
}

//////////////////////////////////////////////////////////////////////

void CRegKeyEx::LoadStrings (CStringArray& pStrings)
{
	if	(IsValid ())
	{
		long				lValueCount = ValueCount ();
		int					lValueNdx;
		tPtr <CRegValue>	lValue;
		CRegString *		lString;

		for	(lValueNdx = 0; lValueNdx < lValueCount; lValueNdx++)
		{
			if	(
					(lValue = operator () (lValueNdx))
#ifdef	__AFX_H__
				&&	(lString = DYNAMIC_DOWNCAST (CRegString, lValue.Ptr ()))
#else
				&&	(lString = dynamic_cast <CRegString *> (lValue.Ptr ()))
#endif
				&&	(!lString->Value ().IsEmpty ())
				)
			{
				pStrings.Add (lString->Value ());
			}
		}
	}
}

void CRegKeyEx::SaveStrings (const CStringArray& pStrings)
{
	if	(
			(!mReadOnly)
		&&	(IsValid ())
		)
	{
		TCHAR	lValueName [MAX_PATH+1];
		DWORD	lValueNameSize;
		DWORD	lValueType = REG_NONE;
		DWORD	lValueSize;
		int		lNdx;

		while	(RegEnumValue (mKey, 0, lValueName, &(lValueNameSize = sizeof (lValueName) / sizeof (TCHAR)), 0, &lValueType, NULL, &(lValueSize = 0)) == ERROR_SUCCESS)
		{
			if	(LogWinErr (LogNormal|LogTime, RegDeleteValue (mKey, lValueName)) != ERROR_SUCCESS)
			{
				break;
			}
		}

		for	(lNdx = 0; lNdx < (int)pStrings.GetCount(); lNdx++)
		{
			_stprintf (lValueName, _T("%d"), lNdx);
			CRegString (mKey, lValueName, true, pStrings [lNdx]).Update ();
		}
	}
}

//////////////////////////////////////////////////////////////////////

void CRegKeyEx::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString				lIndent (_T(' '), pIndent);
		int					lNdx;
		tPtr <CRegKeyEx>	lSubKey;
		tPtr <CRegValue>	lValue;

		if	(!pTitle)
		{
			pTitle = _T("RegKey");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\""), lIndent, pTitle, mName);

		for	(lNdx = 0; lValue = operator () (lNdx); lNdx++)
		{
			lValue->Dump (pLogLevel, _T("Value"), pIndent+2);
		}
		for	(lNdx = 0; lSubKey = operator[] (lNdx); lNdx++)
		{
			lSubKey->Dump (pLogLevel, _T("Key"), pIndent+2);
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CRegValue, CObject)
#endif

CRegValue::CRegValue (HKEY pKey, LPCTSTR pName, DWORD pValueType)
:	mKey (NULL),
	mName (pName),
	mValueType (pValueType)
{
	DWORD	lValueSize = 0;
	DWORD	lValueType = 0;

	if	(
			(pKey)
		&&	(!mName.IsEmpty())
		&&	(RegQueryValueEx (pKey, pName, NULL, &lValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(!mValueType)
			||	(mValueType == lValueType)
			)
		)
	{
		mKey = pKey;
		mValueType = lValueType;
	}
}

CRegValue::CRegValue (const CRegValue& pSource)
:	mKey (pSource.mKey),
	mName (pSource.mName),
	mValueType (pSource.mValueType)
{
}

CRegValue::~CRegValue ()
{
}

CRegValue& CRegValue::ExpandName ()
{
	DWORD	lNameSize;
	CString lName;

	if	(
			(lNameSize = ExpandEnvironmentStrings (mName, NULL, 0))
		&&	(lNameSize)
		&&	(ExpandEnvironmentStrings (mName, lName.GetBuffer (lNameSize), lNameSize))
		)
	{
		lName.ReleaseBuffer ();
		mName = lName;
	}

	return (*this);
}

long CRegValue::Delete ()
{
	if	(mKey)
	{
		return RegDeleteValue (mKey, mName.IsEmpty() ? NULL : (LPCTSTR) mName);
	}
	else
	{
		return ERROR_INVALID_HANDLE;
	}
}

void CRegValue::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (_T(' '), pIndent);

		if	(!pTitle)
		{
			pTitle = _T("Value");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\" (%d)"), lIndent, pTitle, mName, mValueType);
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CRegString, CRegValue)
#endif

CRegString::CRegString (HKEY pKey, LPCTSTR pName, bool pForCreate, LPCTSTR pValue)
:	CRegValue (NULL, pName),
	mValue (pValue)
{
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(mValueType == REG_SZ)
			||	(mValueType == REG_EXPAND_SZ)
			)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, (LPBYTE) mValue.GetBuffer (lValueSize / sizeof (TCHAR)), &lValueSize) == ERROR_SUCCESS)
		)
	{
		mValue.ReleaseBuffer ();
		mKey = pKey;
	}
	else
	{
		mValueType = REG_SZ;

		if	(pForCreate)
		{
			mKey = pKey;
			mValue = pValue;
		}
		else
		{
			mValue.Empty ();
		}
	}
}

CRegString::CRegString (HKEY pKey, long pIndex)
:	CRegValue (NULL)
{
	DWORD	lNameSize = MAX_PATH;
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegEnumValue (pKey, pIndex, mName.GetBuffer (lNameSize), &lNameSize, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(mValueType == REG_SZ)
			||	(mValueType == REG_EXPAND_SZ)
			)
		&&	(RegQueryValueEx (pKey, (LPCTSTR) mName, NULL, &mValueType, (LPBYTE) mValue.GetBuffer (lValueSize / sizeof (TCHAR)), &lValueSize) == ERROR_SUCCESS)
		)
	{
		mName.ReleaseBuffer ();
		mValue.ReleaseBuffer ();
		mKey = pKey;
	}
	else
	{
		mName.Empty ();
		mValue.Empty ();
		mValueType = REG_SZ;
	}
}

CRegString::CRegString (const CRegString& pSource)
:	CRegValue (pSource),
	mValue (pSource.mValue)
{
}

CRegString::~CRegString ()
{
}

//////////////////////////////////////////////////////////////////////

bool CRegString::CanExpand () const
{
	return (mValueType == REG_EXPAND_SZ);
}

void CRegString::CanExpand (bool pCanExpand)
{
	mValueType = pCanExpand ? REG_EXPAND_SZ : REG_SZ;
}

CRegString& CRegString::Expand (bool pIgnoreValueType)
{
	DWORD	lValueSize;
	CString lValue;

	if	(
			(
				(mValueType == REG_EXPAND_SZ)
			||	(pIgnoreValueType)
			)
		&&	(lValueSize = ExpandEnvironmentStrings (mValue, NULL, 0))
		&&	(lValueSize)
		&&	(ExpandEnvironmentStrings (mValue, lValue.GetBuffer (lValueSize), lValueSize))
		)
	{
		lValue.ReleaseBuffer ();
		mValue = lValue;
	}

	return (*this);
}

long CRegString::Update (LPCTSTR pValue)
{
	if	(mKey)
	{
		if	(pValue)
		{
			mValue = pValue;
		}

		return RegSetValueEx (mKey, mName.IsEmpty() ? NULL : (LPCTSTR) mName, 0, mValueType, (LPBYTE) (LPCTSTR) mValue, mValue.GetLength () * sizeof (TCHAR));
	}
	else
	{
		return ERROR_INVALID_HANDLE;
	}
}

//////////////////////////////////////////////////////////////////////

void CRegString::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (_T(' '), pIndent);

		if	(!pTitle)
		{
			pTitle = (mValueType == REG_EXPAND_SZ) ? _T("REG_EXPAND_SZ") : _T("REG_SZ");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\" [%s]"), lIndent, pTitle, mName, mValue);
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CRegStrings, CRegValue)
#endif

CRegStrings::CRegStrings (HKEY pKey, LPCTSTR pName, bool pForCreate, const CStringArray * pValue)
:	CRegValue (NULL, pName)
{
	DWORD	lValueSize = 0;
	CString	lValue;

	if	(
			(pKey)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(mValueType == REG_MULTI_SZ)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, (LPBYTE) lValue.GetBufferSetLength (lValueSize / sizeof (TCHAR)), &lValueSize) == ERROR_SUCCESS)
		)
	{
		mKey = pKey;
		MakeStringArray (lValue, mValue, NULL, false, false, lValue.GetLength ());
	}
	else
	{
		mValueType = REG_MULTI_SZ;

		if	(pForCreate)
		{
			mKey = pKey;
			if	(pValue)
			{
				mValue.Copy (*pValue);
			}
		}
	}
}

CRegStrings::CRegStrings (HKEY pKey, long pIndex)
:	CRegValue (NULL)
{
	DWORD	lNameSize = MAX_PATH;
	DWORD	lValueSize = 0;
	CString	lValue;

	if	(
			(pKey)
		&&	(RegEnumValue (pKey, pIndex, mName.GetBuffer (lNameSize), &lNameSize, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(mValueType == REG_MULTI_SZ)
		&&	(RegQueryValueEx (pKey, (LPCTSTR) mName, NULL, &mValueType, (LPBYTE) lValue.GetBufferSetLength (lValueSize / sizeof (TCHAR)), &lValueSize) == ERROR_SUCCESS)
		)
	{
		mName.ReleaseBuffer ();
		mKey = pKey;
		MakeStringArray (lValue, mValue, NULL, false, false, lValue.GetLength ());
	}
	else
	{
		mValueType = REG_MULTI_SZ;
		mName.Empty ();
	}
}

CRegStrings::CRegStrings (const CRegStrings& pSource)
:	CRegValue (pSource)
{
	mValue.Copy (pSource.mValue);
}

CRegStrings::~CRegStrings ()
{
}

//////////////////////////////////////////////////////////////////////

long CRegStrings::Update (const CStringArray * pValue)
{
	if	(mKey)
	{
		CString	lValue;

		if	(pValue)
		{
			mValue.Copy (*pValue);
		}

		if	(mValue.GetCount() > 0)
		{
			lValue = JoinStringArray (mValue, NULL, false);
		}
		else
		{
			lValue.GetBufferSetLength (1);
		}

		return RegSetValueEx (mKey, mName.IsEmpty() ? NULL : (LPCTSTR) mName, 0, mValueType, (LPBYTE) (LPCTSTR) lValue, (lValue.GetLength ()+1) * sizeof (TCHAR));
	}
	else
	{
		return ERROR_INVALID_HANDLE;
	}
}

//////////////////////////////////////////////////////////////////////

void CRegStrings::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (_T(' '), pIndent);

		if	(!pTitle)
		{
			pTitle = _T("REG_MULTI_SZ");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\" [%s]"), lIndent, pTitle, mName, JoinStringArray (mValue, _T("] [")));
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CRegDWord, CRegValue)
#endif

CRegDWord::CRegDWord (HKEY pKey, LPCTSTR pName, bool pForCreate, DWORD pValue)
:	CRegValue (NULL, pName),
	mValue (0)
{
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(mValueType == REG_DWORD)
			||	(mValueType == REG_BINARY)
			)
		&&	(lValueSize == sizeof (DWORD))
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, (LPBYTE) &mValue, &lValueSize) == ERROR_SUCCESS)
		)
	{
		mKey = pKey;
	}
	else
	{
		mValueType = REG_DWORD;

		if	(pForCreate)
		{
			mKey = pKey;
			mValue = pValue;
		}
		else
		{
			mValue = 0;
		}
	}
}

CRegDWord::CRegDWord (HKEY pKey, long pIndex)
:	CRegValue (NULL)
{
	DWORD	lNameSize = MAX_PATH;
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegEnumValue (pKey, pIndex, mName.GetBuffer (lNameSize), &lNameSize, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(mValueType == REG_DWORD)
			||	(mValueType == REG_BINARY)
			)
		&&	(lValueSize == sizeof (DWORD))
		&&	(RegQueryValueEx (pKey, (LPCTSTR) mName, NULL, &mValueType, (LPBYTE) &mValue, &lValueSize) == ERROR_SUCCESS)
		)
	{
		mName.ReleaseBuffer ();
		mKey = pKey;
	}
	else
	{
		mName.Empty ();
		mValueType = REG_DWORD;
		mValue = 0;
	}
}

CRegDWord::CRegDWord (const CRegDWord& pSource)
:	CRegValue (pSource),
	mValue (pSource.mValue)
{
}

CRegDWord::~CRegDWord ()
{
}

//////////////////////////////////////////////////////////////////////

long CRegDWord::Update (DWORD* pValue)
{
	if	(mKey)
	{
		if	(pValue)
		{
			mValue = (*pValue);
		}
		return RegSetValueEx (mKey, mName.IsEmpty() ? NULL : (LPCTSTR) mName, 0, mValueType, (LPBYTE) &mValue, sizeof (DWORD));
	}
	else
	{
		return ERROR_INVALID_HANDLE;
	}
}

bool CRegDWord::IsBinary () const
{
	return (mValueType == REG_BINARY);
}

void CRegDWord::IsBinary (bool pIsBinary)
{
	mValueType = pIsBinary ? REG_BINARY	: REG_DWORD;
}

//////////////////////////////////////////////////////////////////////

void CRegDWord::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (_T(' '), pIndent);

		if	(!pTitle)
		{
			pTitle = (mValueType == REG_BINARY) ? _T("REG_BINARY") : _T("REG_DWORD");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\" [%8.8X (%u) (%d)]"), lIndent, pTitle, mName, mValue, mValue, mValue);
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CRegQWord, CRegValue)
#endif

CRegQWord::CRegQWord (HKEY pKey, LPCTSTR pName, bool pForCreate, ULONGLONG pValue)
:	CRegValue (NULL, pName),
	mValue (0)
{
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(mValueType == REG_QWORD)
			||	(mValueType == REG_BINARY)
			)
		&&	(lValueSize == sizeof (ULONGLONG))
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, (LPBYTE) &mValue, &lValueSize) == ERROR_SUCCESS)
		)
	{
		mKey = pKey;
	}
	else
	{
		mValueType = REG_QWORD;

		if	(pForCreate)
		{
			mKey = pKey;
			mValue = pValue;
		}
		else
		{
			mValue = 0;
		}
	}
}

CRegQWord::CRegQWord (HKEY pKey, long pIndex)
:	CRegValue (NULL)
{
	DWORD	lNameSize = MAX_PATH;
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegEnumValue (pKey, pIndex, mName.GetBuffer (lNameSize), &lNameSize, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(
				(mValueType == REG_QWORD)
			||	(mValueType == REG_BINARY)
			)
		&&	(lValueSize == sizeof (ULONGLONG))
		&&	(RegQueryValueEx (pKey, (LPCTSTR) mName, NULL, &mValueType, (LPBYTE) &mValue, &lValueSize) == ERROR_SUCCESS)
		)
	{
		mName.ReleaseBuffer ();
		mKey = pKey;
	}
	else
	{
		mName.Empty ();
		mValueType = REG_QWORD;
		mValue = 0;
	}
}

CRegQWord::CRegQWord (const CRegQWord& pSource)
:	CRegValue (pSource),
	mValue (pSource.mValue)
{
}

CRegQWord::~CRegQWord ()
{
}

//////////////////////////////////////////////////////////////////////

long CRegQWord::Update (ULONGLONG * pValue)
{
	if	(mKey)
	{
		if	(pValue)
		{
			mValue = (*pValue);
		}
		return RegSetValueEx (mKey, mName.IsEmpty() ? NULL : (LPCTSTR) mName, 0, mValueType, (LPBYTE) &mValue, sizeof (ULONGLONG));
	}
	else
	{
		return ERROR_INVALID_HANDLE;
	}
}

bool CRegQWord::IsBinary () const
{
	return (mValueType == REG_BINARY);
}

void CRegQWord::IsBinary (bool pIsBinary)
{
	mValueType = pIsBinary ? REG_BINARY	: REG_QWORD;
}

//////////////////////////////////////////////////////////////////////

void CRegQWord::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (_T(' '), pIndent);

		if	(!pTitle)
		{
			pTitle = (mValueType == REG_BINARY) ? _T("REG_BINARY") : _T("REG_QWORD");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\" [%16.16I64X (%I64u) (%I64d)]"), lIndent, pTitle, mName, mValue, mValue, mValue);
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CRegBinary, CRegValue)
#endif

CRegBinary::CRegBinary (HKEY pKey, LPCTSTR pName, bool pForCreate, const CByteArray * pValue)
:	CRegValue (NULL, pName)
{
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegQueryValueEx (pKey, pName, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(mValueType == REG_BINARY)
		)
	{
		mKey = pKey;

		if	(lValueSize > 0)
		{
#ifdef	__AFXCOLL_H__
			mValue.SetSize (lValueSize);
#else
			mValue.SetCount (lValueSize);
#endif
			if	(RegQueryValueEx (pKey, pName, NULL, &mValueType, mValue.GetData(), &lValueSize) != ERROR_SUCCESS)
			{
				mKey = NULL;
				mValue.RemoveAll ();
			}
		}
	}
	else
	{
		mValueType = REG_BINARY;

		if	(pForCreate)
		{
			mKey = pKey;
			if	(pValue)
			{
				mValue.Copy (*pValue);
			}
		}
	}
}

CRegBinary::CRegBinary (HKEY pKey, long pIndex)
:	CRegValue (NULL)
{
	DWORD	lNameSize = MAX_PATH;
	DWORD	lValueSize = 0;

	if	(
			(pKey)
		&&	(RegEnumValue (pKey, pIndex, mName.GetBuffer (lNameSize), &lNameSize, NULL, &mValueType, NULL, &lValueSize) == ERROR_SUCCESS)
		&&	(mValueType == REG_BINARY)
		)
	{
		mKey = pKey;
		mName.ReleaseBuffer ();

		if	(lValueSize > 0)
		{
#ifdef	__AFXCOLL_H__
			mValue.SetSize (lValueSize);
#else
			mValue.SetCount (lValueSize);
#endif
			if	(RegQueryValueEx (pKey, (LPCTSTR) mName, NULL, &mValueType, mValue.GetData(), &lValueSize) != ERROR_SUCCESS)
			{
				mKey = NULL;
				mValue.RemoveAll ();
			}
		}
	}
	else
	{
		mValueType = REG_BINARY;
		mName.Empty ();
	}
}

CRegBinary::CRegBinary (const CRegBinary& pSource)
:	CRegValue (pSource)
{
	mValue.Copy (pSource.mValue);
}

CRegBinary::~CRegBinary ()
{
}

//////////////////////////////////////////////////////////////////////

long CRegBinary::Update (const CByteArray * pValue)
{
	if	(mKey)
	{
		if	(pValue)
		{
			mValue.Copy (*pValue);
		}

		return RegSetValueEx (mKey, mName.IsEmpty() ? NULL : (LPCTSTR) mName, 0, mValueType, (DWORD)mValue.GetCount() ? mValue.GetData() : NULL, (DWORD)mValue.GetCount());
	}
	else
	{
		return ERROR_INVALID_HANDLE;
	}
}

//////////////////////////////////////////////////////////////////////

void CRegBinary::Dump (UINT pLogLevel, LPCTSTR pTitle, UINT pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		CString	lIndent (_T(' '), pIndent);

		if	(!pTitle)
		{
			pTitle = _T("REG_BINARY");
		}
		LogMessage (pLogLevel, _T("%s%s \"%s\""), lIndent, pTitle, mName);
		LogDump (pLogLevel, mValue.GetData(), min (mValue.GetCount(), 1024), lIndent+_T("  "), true);
		if	(mValue.GetCount() > 1024)
		{
			LogMessage (pLogLevel, _T("%s..."), lIndent);
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifdef	__AFX_H__
//////////////////////////////////////////////////////////////////////

void SetAppProfileName (LPCTSTR pSubKeyName, bool pDeleteSubKey)
{
	try
	{
		CWinApp *	lApp;
		BOOL		lEnabled;
		TCHAR		lProfileName [MAX_PATH];

		if	(lApp = AfxGetApp ())
		{
			lEnabled = AfxEnableMemoryTracking (FALSE);

			_tcscpy (lProfileName, lApp->m_pszAppName);
			if	(
					(pSubKeyName)
				&&	(*pSubKeyName)
				)
			{
				PathAppend (lProfileName, pSubKeyName);
			}

			free ((void*) lApp->m_pszProfileName);
			lApp->m_pszProfileName = _tcsdup (lProfileName);

			AfxEnableMemoryTracking (lEnabled);

			if	(
					(pSubKeyName)
				&&	(*pSubKeyName)
				&&	(pDeleteSubKey)
				)
			{
				CRegKeyEx	lKey;

				if	(
						(lKey.Attach (lApp->GetAppRegistryKey ()))
					&&	(SHDeleteKey (lKey, NULL) == ERROR_SUCCESS)
					)
				{
					lKey.Attach (lApp->GetAppRegistryKey ());
				}
			}
		}
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////
#endif	// __AFX_H__
//////////////////////////////////////////////////////////////////////
