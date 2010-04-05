/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#if !defined(AFX_REGISTRY_H__89CE033F_B3FF_4D9A_8049_F71DBA461660__INCLUDED_)
#define AFX_REGISTRY_H__89CE033F_B3FF_4D9A_8049_F71DBA461660__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CRegKeyEx
{
public:
	CRegKeyEx ();
	CRegKeyEx (const CRegKeyEx & pKey);
	CRegKeyEx (const CRegKeyEx & pKey, LPCTSTR pName);
	CRegKeyEx (HKEY pParent, LPCTSTR pName, bool pReadOnly = false, bool pCreate = false, bool pAuthorize = false);
	virtual ~CRegKeyEx ();

// Attributes
	CString & Name () {return mName;}
	const CString & Name () const {return mName;}

	operator HKEY () const {return mKey;}
	operator bool () const {return (mKey != NULL);}
	bool operator !() const {return (mKey == NULL);}

	bool IsValid () const {return (mKey != NULL);}
	bool IsReadOnly () const {return mReadOnly;}
	bool IsEmpty () const;

// Operations
	long Open (HKEY pParent, LPCTSTR pName, bool pReadOnly = false, bool pCreate = false, bool pAuthorize = false);
	long Reopen (const CRegKeyEx & pKey, bool pReadOnly = false, bool pDeleteOnly = false);
	long Close ();
	long Delete ();
	long Empty ();

	HKEY Attach (HKEY pKey);
	HKEY Detach ();

	long KeyCount () const;
	CRegKeyEx * operator [] (long pNdx);

	long ValueCount () const;
	class CRegValue * operator () (long pNdx);
	class CRegString Value (bool pExpanded = true) const;

	void LoadStrings (CStringArray & pStrings);
	void SaveStrings (const CStringArray & pStrings);

	void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);
#ifdef	__AFX_H__
	friend void SetAppProfileName (LPCTSTR pSubKeyName = NULL, bool pDeleteSubKey = false);
#endif

// Implementation
private:
	HKEY	mKey;
	CString mName;
	bool	mReadOnly;
};

//////////////////////////////////////////////////////////////////////

class CRegValue
#ifdef	__AFX_H__
	: public CObject
#endif
{
public:
	CRegValue (HKEY pKey, LPCTSTR pName = NULL, DWORD pValueType = 0);
	CRegValue (const CRegValue & pSource);
	virtual ~CRegValue ();
#ifdef	__AFX_H__
	DECLARE_DYNAMIC (CRegValue)
#endif
// Attributes
	CString & Name () {return mName;}
	const CString & Name () const {return mName;}

	operator bool () const {return (mKey != NULL);}
	bool operator !() const {return (mKey == NULL);}
	bool IsValid () const {return (mKey != NULL);}
	ULONG ValueType () const {return mValueType;}

// Operations
	CRegValue & ExpandName ();
	long Delete ();
	virtual void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);

// Implementation
protected:
	HKEY	mKey;
	CString	mName;
    ULONG   mValueType;
};

//////////////////////////////////////////////////////////////////////

class CRegString : public CRegValue
{
public:
	CRegString (HKEY pKey, LPCTSTR pName = NULL, bool pForCreate = false, LPCTSTR pValue = NULL);
	CRegString (HKEY pKey, long pIndex);
	CRegString (const CRegString & pSource);
	virtual ~CRegString ();
#ifdef	__AFX_H__
	DECLARE_DYNAMIC (CRegString)
#endif
// Attributes
	const CString & Value () const {return mValue;}
	CString & Value () {return mValue;}

// Operations
	bool CanExpand () const;
	void CanExpand (bool pCanExpand);
	CRegString & Expand (bool pIgnoreValueType = false);

	long Update (LPCTSTR pValue = NULL);
	void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);

// Implementation
private:
	CString	mValue;
};

//////////////////////////////////////////////////////////////////////

class CRegStrings : public CRegValue
{
public:
	CRegStrings (HKEY pKey, LPCTSTR pName = NULL, bool pForCreate = false, const CStringArray * pValue = NULL);
	CRegStrings (HKEY pKey, long pIndex);
	CRegStrings (const CRegStrings & pSource);
	virtual ~CRegStrings ();
#ifdef	__AFX_H__
	DECLARE_DYNAMIC (CRegStrings)
#endif
// Attributes
	const CStringArray & Value () const {return mValue;}
	CStringArray & Value () {return mValue;}

// Operations
	long Update (const CStringArray * pValue = NULL);
	void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);

// Implementation
private:
	CStringArray	mValue;
};

//////////////////////////////////////////////////////////////////////

class CRegDWord : public CRegValue
{
public:
	CRegDWord (HKEY pKey, LPCTSTR pName = NULL, bool pForCreate = false, DWORD pValue = 0);
	CRegDWord (HKEY pKey, long pIndex);
	CRegDWord (const CRegDWord & pSource);
	virtual ~CRegDWord ();
#ifdef	__AFX_H__
	DECLARE_DYNAMIC (CRegDWord)
#endif
// Attributes
	const DWORD & Value () const {return mValue;}
	DWORD & Value () {return mValue;}
	bool IsBinary () const;
	void IsBinary (bool pIsBinary);

// Operations
	CRegDWord & SetValue (DWORD pValue) {mValue = pValue; return *this;}
	long Update (DWORD * pValue = NULL);
	void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);

// Implementation
private:
	DWORD	mValue;
};

//////////////////////////////////////////////////////////////////////

class CRegQWord : public CRegValue
{
public:
	CRegQWord (HKEY pKey, LPCTSTR pName = NULL, bool pForCreate = false, ULONGLONG pValue = 0);
	CRegQWord (HKEY pKey, long pIndex);
	CRegQWord (const CRegQWord & pSource);
	virtual ~CRegQWord ();
#ifdef	__AFX_H__
	DECLARE_DYNAMIC (CRegQWord)
#endif
// Attributes
	const ULONGLONG & Value () const {return mValue;}
	ULONGLONG & Value () {return mValue;}
	bool IsBinary () const;
	void IsBinary (bool pIsBinary);

// Operations
	long Update (ULONGLONG * pValue = NULL);
	void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);

// Implementation
private:
	ULONGLONG	mValue;
};

//////////////////////////////////////////////////////////////////////

class CRegBinary : public CRegValue
{
public:
	CRegBinary (HKEY pKey, LPCTSTR pName = NULL, bool pForCreate = false, const CByteArray * pValue = NULL);
	CRegBinary (HKEY pKey, long pIndex);
	CRegBinary (const CRegBinary & pSource);
	virtual ~CRegBinary ();
#ifdef	__AFX_H__
	DECLARE_DYNAMIC (CRegBinary)
#endif
// Attributes
	const CByteArray & Value () const {return mValue;}
	CByteArray & Value () {return mValue;}

// Operations
	long Update (const CByteArray * pValue = NULL);
	void Dump (UINT pLogLevel, LPCTSTR pTitle = NULL, UINT pIndent = 0);

// Implementation
private:
	CByteArray	mValue;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#include <shlobj.h>
#include <shlwapi.h>

#ifdef __AFXWIN_H__
static inline int GetProfileDebugInt (LPCTSTR pProfileKey, int pDefault = 0, bool pIgnoreNegative = false)
{
#if defined (_DEBUG) || defined (_LOG_H)
	int	lRet = pDefault;
#ifndef _DEBUG
	if	(LogIsActive())
#endif
	{
		CWinApp *	lApp = AfxGetApp ();

		if	(lApp)
		{
			try
			{
				CString	lIniPath;

				SHGetSpecialFolderPath (NULL, lIniPath.GetBuffer(MAX_PATH), CSIDL_COMMON_APPDATA, FALSE);
				PathAppend (lIniPath.GetBuffer(MAX_PATH), lApp->m_pszRegistryKey);
				PathAppend (lIniPath.GetBuffer(MAX_PATH), _T("Debug.ini"));
				lIniPath.ReleaseBuffer ();

				lRet = (int) ::GetPrivateProfileInt (lApp->m_pszProfileName, pProfileKey, pDefault, lIniPath);
				if	(
						(pIgnoreNegative)
					&&	(lRet < 0)
					)
				{
					lRet = pDefault;
				}
			}
			catch (...)
			{}
		}
	}
	return lRet;
#else
	return pDefault;
#endif
}
#else
__if_exists(_AtlProfileName)
{
	static inline int GetProfileDebugInt (LPCTSTR pProfileKey, int pDefault = 0, bool pIgnoreNegative = false)
	{
#if defined (_DEBUG) || defined (_LOG_H)
		int	lRet = pDefault;
#ifndef _DEBUG
		if	(LogIsActive())
#endif
		{
			try
			{
				CString	lIniPath;
				CString	lRootName (_AtlProfilePath);

				PathRemoveBackslash (lRootName.GetBuffer(lRootName.GetLength()));
				PathStripPath (lRootName.GetBuffer(lRootName.GetLength()));
				lRootName.ReleaseBuffer ();

				SHGetSpecialFolderPath (NULL, lIniPath.GetBuffer(MAX_PATH), CSIDL_COMMON_APPDATA, FALSE);
				PathAppend (lIniPath.GetBuffer(MAX_PATH), lRootName);
				PathAppend (lIniPath.GetBuffer(MAX_PATH), _T("Debug.ini"));
				lIniPath.ReleaseBuffer ();

				lRet = (int) ::GetPrivateProfileInt (_AtlProfileName, pProfileKey, pDefault, lIniPath);
				if	(
						(pIgnoreNegative)
					&&	(lRet < 0)
					)
				{
					lRet = pDefault;
				}
			}
			catch (...)
			{}
		}
		return lRet;
#else
		return pDefault;
#endif
	}
}
#endif	// __AFXWIN_H__
//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_REGISTRY_H__89CE033F_B3FF_4D9A_8049_F71DBA461660__INCLUDED_)
