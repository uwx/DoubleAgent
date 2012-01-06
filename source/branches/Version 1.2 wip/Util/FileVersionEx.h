/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
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
#if !defined(AFX_FILEVERSIONEX_H__749B12F5_A4FE_47C4_A7FB_5A71DE1AA298__INCLUDED_)
#define AFX_FILEVERSIONEX_H__749B12F5_A4FE_47C4_A7FB_5A71DE1AA298__INCLUDED_
#pragma once

#include "FileVersion.h"
#ifdef	__AFXCOLL_H__
#include "AfxTemplEx.h"
#endif

//////////////////////////////////////////////////////////////////////

class CFileVersionEx : public CFileVersion
{
public:
	CFileVersionEx (void* pVersionInfo, LPCTSTR pFileName = NULL);
	CFileVersionEx (LPCTSTR pFileName);
	CFileVersionEx (HMODULE pModule, LPCTSTR pFileName = NULL);
	CFileVersionEx (const CFileVersionEx& pSource);
	virtual ~CFileVersionEx ();

//----------------------------------------------------------------------

	CString FileVersionName (LPCTSTR pFormatString) const;
	CString ProductVersionName (LPCTSTR pFormatString) const;
	CString AppVersionName () const;

	DWORD FileType () const;
	DWORD FileFlags () const;
	DWORD OsType () const;
	DWORD OsSubType () const;
	DWORD DriverType () const;
	DWORD FontType () const;
	DWORD DeviceId () const;

	static CString FileTypeName (DWORD pFileType);
	CString FileFlagsNames (bool pUseStrings = true) const;
	static CString OsTypeName (DWORD pOsType);
	static CString OsSubTypeName (DWORD pOsType);
	static CString OsName (DWORD pOsType);
	static CString DriverTypeName (DWORD pDriverType);
	static CString FontTypeName (DWORD pFontType);

//----------------------------------------------------------------------

	bool FileFlagsValid (DWORD& pFileFlags, DWORD& pFileFlagsMask) const;
	bool OneCodePageValid () const;
	bool AllCodePagesValid () const;

	static CString MassageName (const CString& pName, bool pSingleLine = false);
	static CString MassageProductName (const CString& pProductName, UINT pStrength = 0);
	static CString MassageCompanyName (const CString& pCompanyName, UINT pStrength = 0);

//----------------------------------------------------------------------

	class CVersionString
	{
	public:
		CVersionString () {}
		~CVersionString () {}

		CVersionString (const CVersionString& pSource)
		:	mName (pSource.mName),
			mValue (pSource.mValue)
		{
			mValue.TrimLeft ();
			mValue.TrimRight ();
		}

		CVersionString (const CString& pSource)
		:	mName (pSource)
		{
		}

		operator const CString&() const {return mName;}

		CVersionString& operator= (const CVersionString& pSource)
		{
			mName = pSource.mName;
			mValue = pSource.mValue;
			return *this;
		}

		CVersionString& operator= (const CString& pSource)
		{
			mName = pSource;
			mValue.Empty ();
			return *this;
		}

		CString mName;
		CString mValue;
	};

//----------------------------------------------------------------------

	class CVersionStrings : public CClassArray <CVersionString>
	{
	public:
		CVersionStrings (LPCTSTR pKey);
		CVersionStrings (const CVersionStrings& pSource);
		~CVersionStrings () {}

		CVersionString* Find (const CString& pName) const;

		WORD GetLANGID () const {return MAKELANGID (mLanguage, SUBLANG_NEUTRAL);}
		DWORD GetLCID () const {return MAKELCID (MAKELANGID (mLanguage, SUBLANG_NEUTRAL), SORT_DEFAULT);}

		WORD	mLanguage;
		WORD	mCodePage;
		CString mLanguageName;
		CString mCountryName;
	};

//----------------------------------------------------------------------

	class CVersionLocale
	{
	public:
		CVersionLocale (DWORD pLocale);
		~CVersionLocale () {}

		WORD GetLANGID () const {return MAKELANGID (mLanguage, SUBLANG_NEUTRAL);}
		DWORD GetLCID () const {return MAKELCID (MAKELANGID (mLanguage, SUBLANG_NEUTRAL), SORT_DEFAULT);}

		WORD	mLanguage;
		WORD	mCodePage;
		CString mLanguageName;
		CString mCountryName;
	};

//----------------------------------------------------------------------

	class CVersionStringsList : public COwnPtrArray <CVersionStrings>
	{
	public:
		CVersionStringsList () {}
		~CVersionStringsList () {}

		CVersionStrings * FindLang (WORD pLanguage)	const;
		CVersionString* Find (const CString& pName) const;
	};

//----------------------------------------------------------------------

	class CVersionLocaleList : public COwnPtrArray <CVersionLocale>
	{
	public:
		CVersionLocaleList () {}
		~CVersionLocaleList () {}

		CVersionLocale * FindLang (WORD pLanguage) const;
	};

//----------------------------------------------------------------------

	CVersionLocaleList		mLocales;
	CVersionStringsList 	mStrings;

	static LPCTSTR			mFileVersionName;
	static LPCTSTR			mProductVersionName;
	static LPCTSTR			mCompanyNameName;
	static LPCTSTR			mCompanyNameAlt [2];
	static LPCTSTR			mProductNameName;
	static LPCTSTR			mProductNameAlt [2];
	static LPCTSTR			mFileDescriptionName;
	static LPCTSTR			mCopyrightName;
	static LPCTSTR			mTrademarkName;
	static LPCTSTR			mPrivateBuildName;
	static LPCTSTR			mSpecialBuildName;

	static LPCTSTR			mOsTypeDos;
	static LPCTSTR			mOsTypeNt;
	static LPCTSTR			mOsTypePm32;
	static LPCTSTR			mOsTypePm16;

	static LPCTSTR			mOsSubTypeWin16;
	static LPCTSTR			mOsSubTypeWin32;
	static LPCTSTR			mOsSubTypePm16;
	static LPCTSTR			mOsSubTypePm32;

	static LPCTSTR			mOsNameWin16;
	static LPCTSTR			mOsNameWin32;
	static LPCTSTR			mOsNameWinNt;
	static LPCTSTR			mOsNameWin16Cui;
	static LPCTSTR			mOsNameWin32Cui;
	static LPCTSTR			mOsNameWinNtCui;
	static LPCTSTR			mOsNameOs2;
	static LPCTSTR			mOsNameOs2Cui;

	static LPCTSTR			mOsNamePosixCui;
	static LPCTSTR			mOsNameWin9xDriver;
	static LPCTSTR			mOsNameWinCE;
	static LPCTSTR			mOsNameUnknown;

	static const CString	mFileTypeNameUnknown;
	static const CString	mFileTypeNameExe;
	static const CString	mFileTypeNameDll;
	static const CString	mFileTypeNameDrv;
	static const CString	mFileTypeNameFon;
	static const CString	mFileTypeNameVxd;
	static const CString	mFileTypeNameLib;
	static const CString	mFileTypeNameOcx;
//----------------------------------------------------------------------
private:
	virtual void GetVersionInfo (void* pVersionInfo);
	void GetVersionInfoW (void* pVersionInfo);
	void GetVersionInfoA (void* pVersionInfo);

private:
	CString mFileName;
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline CString GetLongPathNameEx (LPCTSTR pPathName, bool pQuoted = false)
{
	CString	lRet;
	CString	lPathName (pPathName);

	PathUnquoteSpaces (lPathName.GetBuffer (MAX_PATH));
	lPathName.ReleaseBuffer ();
	lPathName.TrimLeft ();
	lPathName.TrimRight ();
	if	(::GetLongPathName (lPathName, lRet.GetBuffer (MAX_PATH), MAX_PATH))
	{
		lRet.ReleaseBuffer ();
	}
	else
	{
		lRet = lPathName;
	}
	if	(pQuoted)
	{
		PathQuoteSpaces (lRet.GetBuffer (MAX_PATH));
		lRet.ReleaseBuffer ();
	}
	return lRet;
}

inline CString GetShortPathNameEx (LPCTSTR pPathName)
{
	CString	lRet;
	CString	lPathName (pPathName);

	PathUnquoteSpaces (lPathName.GetBuffer (MAX_PATH));
	lPathName.ReleaseBuffer ();
	lPathName.TrimLeft ();
	lPathName.TrimRight ();
	if	(::GetShortPathName (lPathName, lRet.GetBuffer (MAX_PATH), MAX_PATH))
	{
		lRet.ReleaseBuffer ();
	}
	else
	{
		lRet = lPathName;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_FILEVERSIONEX_H__749B12F5_A4FE_47C4_A7FB_5A71DE1AA298__INCLUDED_)
