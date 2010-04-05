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
#if !defined(AFX_LOCALIZE_H__C67392BF_D926_4DB8_BA5C_A24CAB2CA1DA__INCLUDED_)
#define AFX_LOCALIZE_H__C67392BF_D926_4DB8_BA5C_A24CAB2CA1DA__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CLocalize
{
public:
	static CString LoadString (UINT pId, WORD pLangId = LANG_USER_DEFAULT);
	static HACCEL LoadAccelerators (UINT pTableId, WORD pLangId = LANG_USER_DEFAULT);
	static HACCEL LoadAccelerators (LPCTSTR pTableName, WORD pLangId = LANG_USER_DEFAULT);
	static HANDLE LoadImage (UINT pImageId, UINT pImageType, int pCX, int pCY, UINT pFlags, WORD pLangId = LANG_USER_DEFAULT);
	static HANDLE LoadImage (UINT pImageId, UINT pImageType, const CSize & pSize, UINT pFlags, WORD pLangId = LANG_USER_DEFAULT);
	static HANDLE LoadImage (LPCTSTR pImageName, UINT pImageType, int pCX, int pCY, UINT pFlags, WORD pLangId = LANG_USER_DEFAULT);
	static HANDLE LoadImage (LPCTSTR pImageName, UINT pImageType, const CSize & pSize, UINT pFlags, WORD pLangId = LANG_USER_DEFAULT);
	static HBITMAP LoadBitmap (UINT pBitmapId, int pCX, int pCY, UINT pFlags = 0, WORD pLangId = LANG_USER_DEFAULT);
	static HBITMAP LoadBitmap (UINT pBitmapId, const CSize & pSize, UINT pFlags = 0, WORD pLangId = LANG_USER_DEFAULT);
	static HICON LoadIcon (UINT pIconId, WORD pLangId = LANG_USER_DEFAULT);
	static HICON LoadIcon (UINT pIconId, const CSize & pSize, UINT pFlags = LR_DEFAULTCOLOR, WORD pLangId = LANG_USER_DEFAULT);
	static HCURSOR LoadCursor (UINT pCursorId, WORD pLangId = LANG_USER_DEFAULT);
	static HMENU LoadMenu (UINT pMenuId, WORD pLangId = LANG_USER_DEFAULT);
	static bool LoadMenu (class CMenu & pMenu, UINT pMenuId, WORD pLangId = LANG_USER_DEFAULT);
	static bool LoadDialog (class CDialogTemplate & pTemplate, UINT pDialogId, WORD pLangId = LANG_USER_DEFAULT);

	static HINSTANCE FindResource (const class CResName & pResName, WORD pLangId = LANG_USER_DEFAULT, HINSTANCE pResModule = NULL);
	static LPCVOID LoadResource (const class CResName & pResName, ULONG & pResSize, WORD pLangId = LANG_USER_DEFAULT, HINSTANCE pResModule = NULL);

	friend CString FormatString (UINT pId, LPCTSTR pInsert);
	friend CString FormatString (UINT pId, LPCTSTR pInsert1, LPCTSTR pInsert2);
	friend CString FormatString (UINT pId, LPCTSTR pInsert1, LPCTSTR pInsert2, LPCTSTR pInsert3);
	friend CString FormatString (UINT pId, const CStringArray & pInsert);
	friend CString FormatString (LPCTSTR pFormat, LPCTSTR pInsert);
	friend CString FormatString (LPCTSTR pFormat, LPCTSTR pInsert1, LPCTSTR pInsert2);
	friend CString FormatString (LPCTSTR pFormat, LPCTSTR pInsert1, LPCTSTR pInsert2, LPCTSTR pInsert3);
	friend CString FormatString (LPCTSTR pFormat, const CStringArray & pInsert);

	friend CString ExtractSubString (UINT pId, int pSubString, TCHAR pSep = '\n');
	friend CString ExtractSubString (LPCTSTR pString, int pSubString, TCHAR pSep = '\n');
	friend CString ExpandEnvString (UINT pId);
	friend CString ExpandEnvString (LPCTSTR pString);

public:
	static HINSTANCE GetMuiModule (LANGID pLangId);
	static HINSTANCE GetMuiModule (LANGID pLangId, ULONG & pModuleNum);
	static void FreeMuiModules ();
	static void SetNlsSource (LPUNKNOWN pNlsServer);
	static HRESULT ApplyNlsLangId ();
	static HRESULT MarshalNlsSource (LPSTREAM * pStream);
	static HRESULT SetThreadNlsSource (LPSTREAM pStream);
	static const UINT mNlsChangeMsg;

private:
	static bool LoadMuiString (UINT pId, WORD pLangId, LPCWSTR & pString, ULONG & pStringSize);
	static bool LoadMuiMenu (UINT pMenuId, WORD pLangId, LPCVOID & pMenuTemplate, ULONG & pTemplateSize);
	static bool LoadMuiDialog (UINT pDialogId, WORD pLangId, LPCVOID & pDialogTemplate, ULONG & pTemplateSize);
	static bool LoadNlsString (UINT pId, WORD pLangId, BSTR & pString);
	static bool LoadNlsMenu (UINT pMenuId, WORD pLangId, CByteArray & pMenuTemplate);
	static bool LoadNlsDialog (UINT pDialogId, WORD pLangId, CByteArray & pDialogTemplate);
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline CString ExtractSubString (UINT pId, int pSubString, TCHAR pSep)
{
	return ExtractSubString (CLocalize::LoadString (pId), pSubString, pSep);
}

inline CString ExtractSubString (LPCTSTR pString, int pSubString, TCHAR pSep)
{
	CString	lRet;
	AfxExtractSubString (lRet, pString, pSubString, pSep);
	lRet.ReleaseBuffer ();
	lRet.Replace (_T('\r'), _T('\n'));
	return lRet;
}

//////////////////////////////////////////////////////////////////////

inline CString ExpandEnvString (UINT pId)
{
	return ExpandEnvString (CLocalize::LoadString (pId));
}

inline CString ExpandEnvString (LPCTSTR pString)
{
	CString lString;
	DWORD	lStringSize;

	if	(
			(pString)
		&&	(*pString)
		&&	(lStringSize = ExpandEnvironmentStrings (pString, NULL, 0))
		&&	(lStringSize)
		&&	(ExpandEnvironmentStrings (pString, lString.GetBuffer (lStringSize), lStringSize))
		)
	{
		lString.ReleaseBuffer ();
	}
	else
	{
		lString = pString;
	}

	return lString;
}

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_LOCALIZE_H__C67392BF_D926_4DB8_BA5C_A24CAB2CA1DA__INCLUDED_)
