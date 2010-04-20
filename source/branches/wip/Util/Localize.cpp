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
#include "StdAfx.h"
#include "Localize.h"
#include "Log.h"

#ifdef	__AFX_H__
#include <afxpriv.h>
#define	_GetResourceHandle AfxGetResourceHandle
#else
#include <atlcore.h>
#define	_GetResourceHandle _AtlBaseModule.GetResourceInstance
#endif

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

CString CLocalize::LoadString (UINT pId, WORD pLangId)
{
	CString	lRet;

#ifdef	_UNICODE
	tBstrPtr	lNlsString;
	LPCWSTR		lMuiString;
	ULONG		lMuiStringSize;

	if	(LoadNlsString (pId, pLangId, *lNlsString.Free()))
	{
		lRet = (BSTR)lNlsString;
	}
	else
	if	(LoadMuiString (pId, pLangId, lMuiString, lMuiStringSize))
	{
		lRet = CString (lMuiString, lMuiStringSize);
	}
	else
#endif
	{
		::LoadString (_GetResourceHandle (), pId, lRet.GetBuffer (2048), 2048);
		lRet.ReleaseBuffer ();
		lRet.FreeExtra ();
	}
	return CString (lRet);
}

//////////////////////////////////////////////////////////////////////

HACCEL CLocalize::LoadAccelerators (LPCTSTR pTableName, WORD pLangId)
{
	return ::LoadAccelerators (_GetResourceHandle (), pTableName);
}

HACCEL CLocalize::LoadAccelerators (UINT pTableId, WORD pLangId)
{
	return LoadAccelerators (MAKEINTRESOURCE (pTableId), pLangId);
}

//////////////////////////////////////////////////////////////////////

HANDLE CLocalize::LoadImage (LPCTSTR pImageName, UINT pImageType, int pCX, int pCY, UINT pFlags, WORD pLangId)
{
	return ::LoadImage (_GetResourceHandle (), pImageName, pImageType, pCX, pCY, pFlags);
}

HANDLE CLocalize::LoadImage (LPCTSTR pImageName, UINT pImageType, const SIZE & pSize, UINT pFlags, WORD pLangId)
{
	return LoadImage (pImageName, pImageType, pSize.cx, pSize.cy, pFlags, pLangId);
}

HANDLE CLocalize::LoadImage (UINT pImageId, UINT pImageType, int pCX, int pCY, UINT pFlags, WORD pLangId)
{
	return LoadImage (MAKEINTRESOURCE (pImageId), pImageType, pCX, pCY, pFlags, pLangId);
}

HANDLE CLocalize::LoadImage (UINT pImageId, UINT pImageType, const SIZE & pSize, UINT pFlags, WORD pLangId)
{
	return LoadImage (MAKEINTRESOURCE (pImageId), pImageType, pSize.cx, pSize.cy, pFlags, pLangId);
}

//////////////////////////////////////////////////////////////////////

HBITMAP CLocalize::LoadBitmap (UINT pBitmapId, int pCX, int pCY, UINT pFlags, WORD pLangId)
{
	return (HBITMAP) LoadImage (MAKEINTRESOURCE (pBitmapId), IMAGE_BITMAP, pCX, pCY, pFlags, pLangId);
}

HBITMAP CLocalize::LoadBitmap (UINT pBitmapId, const SIZE & pSize, UINT pFlags, WORD pLangId)
{
	return (HBITMAP) LoadImage (MAKEINTRESOURCE (pBitmapId), IMAGE_BITMAP, pSize.cx, pSize.cy, pFlags, pLangId);
}

HICON CLocalize::LoadIcon (UINT pIconId, WORD pLangId)
{
	return (HICON) LoadImage (MAKEINTRESOURCE (pIconId), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR|LR_DEFAULTSIZE, pLangId);
}

HICON CLocalize::LoadIcon (UINT pIconId, const SIZE & pSize, UINT pFlags, WORD pLangId)
{
	return (HICON) LoadImage (MAKEINTRESOURCE (pIconId), IMAGE_ICON, pSize.cx, pSize.cy, pFlags, pLangId);
}

HCURSOR CLocalize::LoadCursor (UINT pCursorId, WORD pLangId)
{
	return ::LoadCursor (_GetResourceHandle (), MAKEINTRESOURCE (pCursorId));
	//return (HCURSOR) LoadImage (MAKEINTRESOURCE (pCursorId), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR|LR_DEFAULTSIZE, pLangId);
}

//////////////////////////////////////////////////////////////////////

HMENU CLocalize::LoadMenu (UINT pMenuId, WORD pLangId)
{
	HMENU		lMenu = NULL;
#ifdef	_UNICODE
	CByteArray	lNlsTemplate;
	LPCVOID		lMenuTemplate;
	ULONG		lMenuTemplateSize;

	if	(LoadNlsMenu (pMenuId, pLangId, lNlsTemplate))
	{
		lMenu = LoadMenuIndirect (lNlsTemplate.GetData());
	}
	else
	if	(LoadMuiMenu (pMenuId, pLangId, lMenuTemplate, lMenuTemplateSize))
	{
		lMenu = LoadMenuIndirect (lMenuTemplate);
	}
#endif
	return lMenu;
}

//////////////////////////////////////////////////////////////////////
#ifdef	__AFX_H__
//////////////////////////////////////////////////////////////////////

bool CLocalize::LoadMenu (CMenu & pMenu, UINT pMenuId, WORD pLangId)
{
	if	(pMenu.GetSafeHmenu ())
	{
		pMenu.DestroyMenu ();
	}
	if	(pMenu.Attach (LoadMenu (pMenuId, pLangId)))
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

class CDialogTemplateAccess : public CDialogTemplate
{
public:
	BOOL SetTemplate(const DLGTEMPLATE* pTemplate, UINT cb) {return CDialogTemplate::SetTemplate (pTemplate, cb);}
};

bool CLocalize::LoadDialog (CDialogTemplate & pTemplate, UINT pDialogId, WORD pLangId)
{
	bool		lRet = false;
#ifdef	_UNICODE
	CByteArray	lNlsTemplate;
	LPCVOID		lDialogTemplate;
	ULONG		lDialogTemplateSize;

	if	(LoadNlsDialog (pDialogId, pLangId, lNlsTemplate))
	{
		((CDialogTemplateAccess &)pTemplate).SetTemplate ((LPDLGTEMPLATE)lNlsTemplate.GetData(), (int)lNlsTemplate.GetSize());
		lRet = true;
	}
	else
	if	(LoadMuiDialog (pDialogId, pLangId, lDialogTemplate, lDialogTemplateSize))
	{
		((CDialogTemplateAccess &)pTemplate).SetTemplate ((LPDLGTEMPLATE)lDialogTemplate, lDialogTemplateSize);
		lRet = true;
	}
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#else	// __AFX_H__
//////////////////////////////////////////////////////////////////////

bool CLocalize::LoadMenu (CMenuHandle & pMenu, UINT pMenuId, WORD pLangId)
{
	if	(pMenu.GetSafeHandle ())
	{
		pMenu.Close ();
	}
	if	(pMenu.Attach (LoadMenu (pMenuId, pLangId)))
	{
		return true;
	}
	return false;
}

bool CLocalize::LoadDialog (CGlobalHandle & pTemplate, UINT pDialogId, WORD pLangId)
{
	bool		lRet = false;
	CByteArray	lNlsTemplate;
	LPCVOID		lDialogTemplate;
	ULONG		lDialogTemplateSize;

	if	(pTemplate.GetSafeHandle ())
	{
		pTemplate.Close ();
	}

	if	(LoadNlsDialog (pDialogId, pLangId, lNlsTemplate))
	{
		if	(pTemplate.Attach (GlobalAlloc (GPTR, lNlsTemplate.GetSize())))
		{
			memcpy (GlobalLock (pTemplate), lNlsTemplate.GetData(), lNlsTemplate.GetSize());
			GlobalUnlock (pTemplate);
			lRet = true;
		}
	}
	else
	if	(LoadMuiDialog (pDialogId, pLangId, lDialogTemplate, lDialogTemplateSize))
	{
		if	(pTemplate.Attach (GlobalAlloc (GPTR, lDialogTemplateSize)))
		{
			memcpy (GlobalLock (pTemplate), lDialogTemplate, lDialogTemplateSize);
			GlobalUnlock (pTemplate);
			lRet = true;
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#endif	// __AFX_H__
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#pragma optimize("",off)
//////////////////////////////////////////////////////////////////////

CString FormatString (UINT pId, LPCTSTR pInsert)
{
	return FormatString (CLocalize::LoadString (pId), pInsert);
}

CString FormatString (UINT pId, LPCTSTR pInsert1, LPCTSTR pInsert2)
{
	return FormatString (CLocalize::LoadString (pId), pInsert1, pInsert2);
}

CString FormatString (UINT pId, LPCTSTR pInsert1, LPCTSTR pInsert2, LPCTSTR pInsert3)
{
	return FormatString (CLocalize::LoadString (pId), pInsert1, pInsert2, pInsert3);
}

CString FormatString (UINT pId, const CStringArray & pInsert)
{
	return FormatString (CLocalize::LoadString (pId), pInsert);
}

CString FormatString (LPCTSTR pFormat, LPCTSTR pInsert)
{
	return FormatString (pFormat, &pInsert, 1);
}

CString FormatString (LPCTSTR pFormat, LPCTSTR pInsert1, LPCTSTR pInsert2)
{
	LPCTSTR lInserts [2];
	lInserts [0] = pInsert1;
	lInserts [1] = pInsert2;
	return FormatString (pFormat, lInserts, 2);
}

CString FormatString (LPCTSTR pFormat, LPCTSTR pInsert1, LPCTSTR pInsert2, LPCTSTR pInsert3)
{
	LPCTSTR lInserts [3];
	lInserts [0] = pInsert1;
	lInserts [1] = pInsert2;
	lInserts [2] = pInsert3;
	return FormatString (pFormat, lInserts, 3);
}

CString FormatString (LPCTSTR pFormat, const CStringArray & pInsert)
{
	CString		lRet;
	INT_PTR		lCount = pInsert.GetSize();
	INT_PTR		lNdx;
	LPCTSTR *	lInsert = new LPCTSTR [lCount];

	for	(lNdx = 0; lNdx < lCount; lNdx++)
	{
		lInsert [lNdx] = pInsert [lNdx];
	}
	lRet = FormatString (pFormat, lInsert, (int)lCount);
#if (_MSC_VER >= 1400)
	delete lInsert;
#else
	delete [] lInsert;
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString FormatString (LPCTSTR pFormat, LPCTSTR * pInsert, int pInsertCount)
{
	CString	lRet;

#ifdef	__AFX_H__
	AfxFormatStrings (lRet, pFormat, pInsert, pInsertCount);
	lRet.FreeExtra ();
#else
	LPTSTR	lBuffer = NULL;
	try
	{
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY|FORMAT_MESSAGE_MAX_WIDTH_MASK, pFormat, 0, 0, (LPTSTR)&lBuffer, 1, (va_list*)pInsert);
		lRet = lBuffer;
	}
	catch AnyExceptionSilent
	if	(lBuffer)
	{
		LocalFree (lBuffer);
	}
#endif

	return CString (lRet);
}

//////////////////////////////////////////////////////////////////////
#pragma optimize("",off)
//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
CString ExtractSubString (LPCTSTR pString, int pSubString, TCHAR pSep)
{
	CString	lRet;
	AfxExtractSubString (lRet, pString, pSubString, pSep);
	lRet.ReleaseBuffer ();
	lRet.Replace (_T('\r'), _T('\n'));
	return lRet;
}
#else
#include "StringArrayEx.h"
CString ExtractSubString (LPCTSTR pString, int pSubString, TCHAR pSep)
{
	CString			lRet;
	CStringArray	lParts;
	TCHAR			lDelims [2] = {pSep, 0};

	if	(MakeStringArray (pString, lParts, lDelims, true, false) > pSubString)
	{
		lRet = lParts [pSubString];
		lRet.Replace (_T('\r'), _T('\n'));
	}
	return lRet;
}
#endif
