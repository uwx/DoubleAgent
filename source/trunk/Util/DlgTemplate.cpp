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
#include "StdAfx.h"
#include "DlgTemplate.h"
#include "Log.h"

/////////////////////////////////////////////////////////////////////////////

#define	DataStringChars(d)	(wcslen((LPCWSTR)(d)) + 1)
#define	DataStringSize(d)	((wcslen((LPCWSTR)(d)) + 1) * sizeof(WCHAR))
#define	DataAlign(d,t)		((LPBYTE)(d) + ((sizeof(t) - ((ULONG_PTR)(d) % sizeof(t))) % sizeof(t)));

//////////////////////////////////////////////////////////////////////

CDlgTemplate::CDlgTemplate (LPCTSTR pTemplateName, HINSTANCE pResourceInstance)
:	mTemplateSize (0),
	mTemplate (*(LPDLGTEMPLATE*)(mPointers+0)),
	mSignature (*(LPWORD*)(mPointers+1)),
	mHelpId (*(LPDWORD*)(mPointers+2)),
	mStyle (*(LPDWORD*)(mPointers+3)),
	mExStyle (*(LPDWORD*)(mPointers+4)),
	mMenuId (*(LPWORD*)(mPointers+5)),
	mMenuName (*(LPWSTR*)(mPointers+6)),
	mClassId (*(LPWORD*)(mPointers+7)),
	mClassName (*(LPWSTR*)(mPointers+8)),
	mCaption (*(LPWSTR*)(mPointers+9)),
	mFontName (*(LPWSTR*)(mPointers+10)),
	mFontHeight (*(LPWORD*)(mPointers+11)),
	mFontWeight (*(LPWORD*)(mPointers+12)),
	mFontItalic (*(LPBYTE*)(mPointers+13)),
	mFontCharSet (*(LPBYTE*)(mPointers+14))
{
	memset (mPointers, 0, sizeof(mPointers));

	if	(pTemplateName)
	{
		LoadResource (pTemplateName, pResourceInstance);
	}
}

CDlgTemplate::~CDlgTemplate ()
{
}

//////////////////////////////////////////////////////////////////////

bool CDlgTemplate::LoadResource (LPCTSTR pTemplateName, HINSTANCE pResourceInstance)
{
	bool		lRet = false;
	HGLOBAL		lResHandle;
	DWORD		lResSize;
	HRSRC		lResource;
	HGLOBAL		lTemplateHandle;

	if	(
			(
				(lResource = ::FindResourceEx (pResourceInstance, RT_DIALOG, pTemplateName, LANGIDFROMLCID (GetThreadLocale())))
			||	(lResource = ::FindResourceEx (pResourceInstance, RT_DIALOG, pTemplateName, GetUserDefaultUILanguage()))
			||	(lResource = ::FindResourceEx (pResourceInstance, RT_DIALOG, pTemplateName, GetSystemDefaultUILanguage()))
			||	(lResource = ::FindResource (pResourceInstance, pTemplateName, RT_DIALOG))
			)
		&&	(lResSize = ::SizeofResource (pResourceInstance, lResource))
		&&	(lResHandle = ::LoadResource (pResourceInstance, lResource))
		)
	{
		if	(lTemplateHandle = ::GlobalAlloc (GPTR, lResSize))
		{
			memcpy (::GlobalLock(lTemplateHandle), ::LockResource(lResHandle), lResSize);
			::GlobalUnlock(lTemplateHandle);
			Attach (lTemplateHandle);
			lRet = true;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CDlgTemplate::Attach (HGLOBAL pTemplate)
{
	bool	lRet = false;

	RemoveAll ();
	memset (mPointers, 0, sizeof(mPointers));
	mTemplateHandle.Close ();
	mTemplateSize = 0;

	if	(mTemplateHandle.Attach (pTemplate))
	{
		WORD	lControlVersion = 0;
		WORD	lControlNdx;
		LPBYTE	lDataStart;
		LPBYTE	lDataByte;
		LPWORD	lDataWord;
		LPDWORD	lDataDWord;

		lDataByte = lDataStart = (LPBYTE)GlobalLock (mTemplateHandle);
		lDataWord = (LPWORD)lDataByte;
		mSignature = lDataWord;
		if	(IsDlgTemplateEx ())
		{
			lDataByte += sizeof(WORD)*2;
			lDataDWord = (LPDWORD)lDataByte;
			mHelpId = lDataDWord;
			mExStyle = lDataDWord+1;
			mStyle = lDataDWord+2;
			lDataByte += sizeof(DWORD);
			mTemplate = (LPDLGTEMPLATE)lDataByte;
			lControlVersion = 1;
		}
		else
		{
			lDataDWord = (LPDWORD)lDataByte;
			mStyle = lDataDWord;
			mExStyle = lDataDWord+1;
			mTemplate = (LPDLGTEMPLATE)lDataByte;
		}

		lDataByte += sizeof(DLGTEMPLATE);

		lDataByte = DataAlign (lDataByte, WORD);
		lDataWord = (LPWORD)lDataByte;
		lDataByte += sizeof(WORD);
		if	(lDataWord[0] == 0xFFFF)
		{
			mMenuId = lDataWord+1;
			lDataByte += sizeof(WORD)*2;
		}
		else
		if	(lDataWord[0] != 0)
		{
			mMenuName = (LPWSTR)lDataByte;
			lDataByte += DataStringSize (lDataByte);
		}

		lDataByte = DataAlign (lDataByte, WORD);
		lDataWord = (LPWORD)lDataByte;
		lDataByte += sizeof(WORD);
		if	(lDataWord[0] == 0xFFFF)
		{
			mClassId = lDataWord+1;
			lDataByte += sizeof(WORD)*2;
		}
		else
		if	(lDataWord[0] != 0)
		{
			mClassName = (LPWSTR)lDataByte;
			lDataByte += DataStringSize (lDataByte);
		}

		lDataByte = DataAlign (lDataByte, WORD);
		mCaption = (LPWSTR)lDataByte;
		lDataByte += DataStringSize (lDataByte);

		lDataByte = DataAlign (lDataByte, WORD);
		lDataWord = (LPWORD)lDataByte;
		if	(IsDlgTemplateEx ())
		{
			if	((*mStyle) & (DS_SETFONT|DS_SHELLFONT))
			{
				mFontHeight = lDataWord;
				mFontWeight = lDataWord+1;
				lDataByte += sizeof(WORD)*2;
				mFontItalic = lDataByte;
				mFontCharSet = lDataByte+1;
				lDataByte += 2;
				mFontName = (LPWSTR)lDataByte;
			}
		}
		else
		{
			if	((*mStyle) & DS_SETFONT)
			{
				mFontHeight = lDataWord;
				lDataByte += sizeof(WORD);
				mFontName = (LPWSTR)lDataByte;
				lDataByte += DataStringSize (lDataByte);
			}
		}

		lDataByte = DataAlign (lDataByte, DWORD);

		for	(lControlNdx = 0; lControlNdx < mTemplate->cdit; lControlNdx++)
		{
			LPDLGITEMTEMPLATE	lItemTemplate;

			if	(IsDlgTemplateEx ())
			{
				lItemTemplate = (LPDLGITEMTEMPLATE)(lDataByte + sizeof(DWORD));
			}
			else
			{
				lItemTemplate = (LPDLGITEMTEMPLATE)lDataByte;
			}
			Add (CDlgControl (this, lItemTemplate, lControlVersion, &lDataByte));
			lDataByte = DataAlign (lDataByte, DWORD);
		}

		mTemplateSize = lDataByte - lDataStart;
		lRet = true;
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

HGLOBAL CDlgTemplate::Detach ()
{
	RemoveAll ();
	memset (mPointers, 0, sizeof(mPointers));
	mTemplateSize = 0;
	return mTemplateHandle.Detach ();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CDlgTemplate::IsDlgTemplateEx () const
{
	return mSignature ? ((*mSignature) == 0xFFFF) : false;
}

CString CDlgTemplate::GetCaption () const
{
	if	(mCaption)
	{
		return CString (*mCaption);
	}
	return CString();
}

DWORD CDlgTemplate::GetStyle () const
{
	return mStyle ? *mStyle : 0;
}

DWORD CDlgTemplate::GetExStyle () const
{
	return mExStyle ? *mExStyle : 0;
}

DWORD CDlgTemplate::GetHelpId () const
{
	return mHelpId ? *mHelpId : 0;
}

WORD CDlgTemplate::GetClassId () const
{
	return mClassId ? *mClassId : 0;
}

CString CDlgTemplate::GetClassName () const
{
	if	(mClassName)
	{
		return CString (*mClassName);
	}
	return CString();
}

WORD CDlgTemplate::GetMenuId () const
{
	return mMenuId ? *mMenuId : 0;
}

CString CDlgTemplate::GetMenuName () const
{
	if	(mMenuName)
	{
		return CString (*mMenuName);
	}
	return CString();
}

CString CDlgTemplate::GetFontName () const
{
	if	(mFontName)
	{
		return CString (*mFontName);
	}
	return CString();
}

UINT CDlgTemplate::GetFontSize (bool pInPixels) const
{
	if	(mFontHeight)
	{
		if	(pInPixels)
		{
			CMemDCHandle	lDC;
			CFontHandle		lFont;
			tS <LOGFONT>	lLogFont;
			tS <TEXTMETRIC>	lTextMetrics;

			GetFont (lLogFont);
			lFont.Attach (::CreateFontIndirect (&lLogFont));
			lDC.Attach (CreateCompatibleDC (NULL));
			::SelectObject (lDC, lFont);
			::GetTextMetrics (lDC, &lTextMetrics);
			lDC.Close ();
			lFont.Close ();

			return lTextMetrics.tmAscent + lTextMetrics.tmDescent; // MFC uses lTextMetrics.tmHeight + lTextMetrics.tmExternalLeading
		}
		else
		{
			return *mFontHeight;
		}
	}
	return 0;
}

bool CDlgTemplate::GetFont (LOGFONT& pFont) const
{
	bool	lRet = false;

	memset (&pFont, 0, sizeof(LOGFONT));

	if	(mFontName)
	{
		wcsncpy (pFont.lfFaceName, mFontName, sizeof(pFont.lfFaceName)/sizeof(WCHAR));
		lRet = true;
	}
	else
	{
		::GetObject (GetStockFont (DEFAULT_GUI_FONT), sizeof(LOGFONT), &pFont);
	}
	if	(mFontWeight)
	{
		pFont.lfWeight = (*mFontWeight);
	}
	else
	{
		pFont.lfWeight = FW_NORMAL;
	}
	if	(mFontItalic)
	{
		pFont.lfItalic = (*mFontItalic);
	}
	if	(mFontCharSet)
	{
		pFont.lfCharSet = (*mFontCharSet);
	}
	else
	{
		pFont.lfCharSet = DEFAULT_CHARSET;
	}

	if	(mFontHeight)
	{
		CMemDCHandle	lDC;

		lDC.Attach (CreateCompatibleDC (NULL));
		pFont.lfHeight = -MulDiv ((long)(*mFontHeight), ::GetDeviceCaps (lDC, LOGPIXELSY), 72);
	}
	else
	if	(mFontName)
	{
		tS <LOGFONT>	lSysFont;

		::GetObject (GetStockFont (DEFAULT_GUI_FONT), sizeof(LOGFONT), &lSysFont);
		pFont.lfHeight = lSysFont.lfHeight;
	}

	pFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	pFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	pFont.lfPitchAndFamily = FF_DONTCARE;
	return lRet;
}

CPoint CDlgTemplate::GetPosition (bool pInPixels) const
{
	CPoint	lPosition (0,0);

	if	(mTemplate)
	{
		lPosition.x = mTemplate->x;
		lPosition.y = mTemplate->y;

		if	(pInPixels)
		{
			if	(mFontName)
			{
				lPosition = DlgSizeToPixels (lPosition);
			}
			else
			{
				DWORD	lDialogUnits = GetDialogBaseUnits();

				lPosition.x = (lPosition.x * LOWORD(lDialogUnits)) / 4;
				lPosition.y = (lPosition.y * HIWORD(lDialogUnits)) / 8;
			}
		}
	}
	return lPosition;
}

CSize CDlgTemplate::GetSize (bool pInPixels) const
{
	CSize	lSize (0,0);

	if	(mTemplate)
	{
		lSize.cx = mTemplate->cx;
		lSize.cy = mTemplate->cy;

		if	(pInPixels)
		{
			if	(mFontName)
			{
				lSize = DlgSizeToPixels (lSize);
			}
			else
			{
				DWORD	lDialogUnits = GetDialogBaseUnits();

				lSize.cx = (lSize.cx * LOWORD(lDialogUnits)) / 4;
				lSize.cy = (lSize.cy * HIWORD(lDialogUnits)) / 8;
			}
		}
	}
	return lSize;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSize CDlgTemplate::DlgSizeToPixels (const CSize& pSize) const
{
	CMemDCHandle	lDC;
	CFontHandle		lFont;
	tS <LOGFONT>	lLogFont;
	CSize			lTextSize;

	GetFont (lLogFont);
	lFont.Attach (::CreateFontIndirect (&lLogFont));
	lDC.Attach (CreateCompatibleDC (NULL));
	::SelectObject (lDC, lFont);

	::GetTextExtentPoint (lDC, _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52, &lTextSize);
	lTextSize.cx = (lTextSize.cx + 26) / 52;

	lDC.Close ();
	lFont.Close ();

	return CSize (MulDiv (pSize.cx, lTextSize.cx, 4), MulDiv (pSize.cy, lTextSize.cy, 8));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CDlgControl::CDlgControl (CDlgTemplate * pOwner, LPDLGITEMTEMPLATE pTemplate, WORD pVersion, LPBYTE* pTemplateData)
:	mOwner (pOwner),
	mTemplateSize (0),
	mTemplate (*(LPDLGITEMTEMPLATE*)(mPointers+0)),
	mHelpId (*(LPDWORD*)(mPointers+1)),
	mStyle (*(LPDWORD*)(mPointers+2)),
	mExStyle (*(LPDWORD*)(mPointers+3)),
	mClassId (*(LPWORD*)(mPointers+4)),
	mClassName (*(LPWSTR*)(mPointers+5)),
	mRefResId (*(LPWORD*)(mPointers+6)),
	mText (*(LPWSTR*)(mPointers+7)),
	mExtraSize (*(LPWORD*)(mPointers+8)),
	mExtraData (*(LPBYTE*)(mPointers+9))
{
	mTemplate = pTemplate;

	if	(pTemplateData)
	{
		ATLASSERT ((LPVOID)pTemplate==(LPVOID)(*pTemplateData));
	}
	if	(pTemplate)
	{
		LPBYTE	lDataStart;
		LPBYTE	lDataByte;
		LPWORD	lDataWord;
		LPDWORD	lDataDWord;

		lDataByte = lDataStart = (LPBYTE)pTemplate;
		lDataDWord = (LPDWORD)lDataByte;
		if	(pVersion > 0)
		{
			mHelpId = lDataDWord;
			mExStyle = lDataDWord+1;
			mStyle = lDataDWord+2;
			lDataByte += sizeof(DWORD);
			mTemplate = (LPDLGITEMTEMPLATE)lDataByte;
			lDataByte += sizeof(WORD);
		}
		else
		{
			mStyle = lDataDWord;
			mExStyle = lDataDWord+1;
		}

		lDataByte += sizeof(DLGITEMTEMPLATE);

		lDataByte = DataAlign (lDataByte, WORD);
		lDataWord = (LPWORD)lDataByte;
		if	(lDataWord[0] == 0xFFFF)
		{
			mClassId = lDataWord+1;
			lDataByte += sizeof(WORD)*2;
		}
		else
		{
			mClassName = (LPWSTR)lDataByte;
			lDataByte += DataStringSize (lDataByte);
		}

		lDataByte = DataAlign (lDataByte, WORD);
		lDataWord = (LPWORD)lDataByte;
		if	(lDataWord[0] == 0xFFFF)
		{
			mRefResId = lDataWord+1;
			lDataByte += sizeof(WORD)*2;
		}
		else
		{
			mText = (LPWSTR)lDataByte;
			lDataByte += DataStringSize (lDataByte);
		}

		lDataByte = DataAlign (lDataByte, WORD);
		lDataWord = (LPWORD)lDataByte;
		mExtraSize = lDataWord;
		lDataByte += sizeof(WORD);
		mExtraData = lDataByte;
		lDataByte += *mExtraSize;

		mTemplateSize = lDataByte - lDataStart;
		if	(pTemplateData)
		{
			(*pTemplateData) = lDataByte;
		}
	}
}

CDlgControl::CDlgControl (const CDlgControl& pSource)
:	mTemplate (*(LPDLGITEMTEMPLATE*)(mPointers+0)),
	mHelpId (*(LPDWORD*)(mPointers+1)),
	mStyle (*(LPDWORD*)(mPointers+2)),
	mExStyle (*(LPDWORD*)(mPointers+3)),
	mClassId (*(LPWORD*)(mPointers+4)),
	mClassName (*(LPWSTR*)(mPointers+5)),
	mRefResId (*(LPWORD*)(mPointers+6)),
	mText (*(LPWSTR*)(mPointers+7)),
	mExtraSize (*(LPWORD*)(mPointers+8)),
	mExtraData (*(LPBYTE*)(mPointers+9))
{
	operator= (pSource);
}

CDlgControl& CDlgControl::operator= (const CDlgControl& pSource)
{
	mOwner = pSource.mOwner;
	mTemplateSize = pSource.mTemplateSize;
	memcpy (mPointers, pSource.mPointers, sizeof(mPointers));
	return *this;
}

CDlgControl::~CDlgControl ()
{
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CDlgControl::GetText () const
{
	if	(mText)
	{
		return CString (*mText);
	}
	return CString();
}

DWORD CDlgControl::GetStyle () const
{
	return mStyle ? *mStyle : 0;
}

DWORD CDlgControl::GetExStyle () const
{
	return mExStyle ? *mExStyle : 0;
}

DWORD CDlgControl::GetHelpId () const
{
	return mHelpId ? *mHelpId : 0;
}

WORD CDlgControl::GetClassId () const
{
	return mClassId ? *mClassId : 0;
}

CString CDlgControl::GetClassName () const
{
	if	(mClassName)
	{
		return CString (*mClassName);
	}
	return CString();
}
