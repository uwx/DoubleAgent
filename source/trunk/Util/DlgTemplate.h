/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#ifndef	DLGTEMPLATE_H_INCLUDED_
#define DLGTEMPLATE_H_INCLUDED_
#pragma once

#include "HandleTemplates.h"

//////////////////////////////////////////////////////////////////////

class CDlgControl
{
public:
	CDlgControl (class CDlgTemplate * pOwner = NULL, LPDLGITEMTEMPLATE pTemplate = NULL, WORD pVersion = 0, LPBYTE* pTemplateData = NULL);
	CDlgControl (const CDlgControl& pSource);
	CDlgControl& operator= (const CDlgControl& pSource);
	virtual ~CDlgControl ();

// Attributes

// Operations
	CString GetText () const;
	DWORD GetStyle () const;
	DWORD GetExStyle () const;
	DWORD GetHelpId () const;

	WORD GetClassId () const;
	CString GetClassName () const;

// Implementation
protected:
	CDlgTemplate *		mOwner;
	UINT_PTR			mTemplateSize;
	LPVOID				mPointers [10];
	LPDLGITEMTEMPLATE &	mTemplate;
	LPDWORD	&			mHelpId;
	LPDWORD &			mStyle;
	LPDWORD &			mExStyle;
	LPWORD &			mClassId;
	LPWSTR &			mClassName;
	LPWORD &			mRefResId;
	LPWSTR &			mText;
	LPWORD &			mExtraSize;
	LPBYTE &			mExtraData;
};

//////////////////////////////////////////////////////////////////////

class CDlgTemplate : public CClassArray <CDlgControl>
{
public:
	CDlgTemplate (LPCTSTR pTemplateName = NULL, HINSTANCE pResourceInstance = NULL);
	virtual ~CDlgTemplate ();

// Attributes
	bool IsDlgTemplateEx () const;

// Operations
	bool LoadResource (LPCTSTR pTemplateName, HINSTANCE pResourceInstance = NULL);
	bool LoadResource (UINT pTemplateID, HINSTANCE pResourceInstance = NULL) {return LoadResource (MAKEINTRESOURCE(pTemplateID), pResourceInstance);}
	bool Attach (HGLOBAL pTemplate);
	HGLOBAL Detach ();

	CString GetCaption () const;
	DWORD GetStyle () const;
	DWORD GetExStyle () const;
	DWORD GetHelpId () const;

	WORD GetClassId () const;
	CString GetClassName () const;
	WORD GetMenuId () const;
	CString GetMenuName () const;

	CString GetFontName () const;
	UINT GetFontSize (bool pInPixels = false) const;
	bool GetFont (LOGFONT& pFont) const;

	CPoint GetPosition (bool pInPixels = false) const;
	CSize GetSize (bool pInPixels = false) const;

// Implementation
protected:
	CSize DlgSizeToPixels (const CSize& pSize) const;

private:
	CGlobalHandle		mTemplateHandle;
	UINT_PTR			mTemplateSize;
	LPVOID				mPointers [15];
	LPDLGTEMPLATE &		mTemplate;
	LPWORD &			mSignature;
	LPDWORD	&			mHelpId;
	LPDWORD &			mStyle;
	LPDWORD &			mExStyle;
	LPWORD &			mMenuId;
	LPWSTR &			mMenuName;
	LPWORD &			mClassId;
	LPWSTR &			mClassName;
	LPWSTR &			mCaption;
	LPWSTR &			mFontName;
	LPWORD &			mFontHeight;
	LPWORD &			mFontWeight;
	LPBYTE &			mFontItalic;
	LPBYTE &			mFontCharSet;
};

//////////////////////////////////////////////////////////////////////

#endif // DLGTEMPLATE_H_INCLUDED_
