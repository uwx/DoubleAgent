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
#include <AfxPriv.h>
#include "PropPageFix.h"
#include "Localize.h"
#include "MallocPtr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define	DataAlign(d,t)	((LPBYTE)(d) + ((sizeof(t) - ((ULONG_PTR)(d) % sizeof(t))) % sizeof(t)));

/////////////////////////////////////////////////////////////////////////////

CPropPageFix::CPropPageFix()
:	mWritableTemplate (NULL)
{
	mWritableTemplate = NULL;
}

CPropPageFix::~CPropPageFix()
{
	if	(mWritableTemplate)
	{
		GlobalFree (mWritableTemplate);
	}
}

/////////////////////////////////////////////////////////////////////////////

LPCDLGTEMPLATE CPropPageFix::GetWritableTemplate (UINT pTemplateId, WORD pLangId)
{
	CDialogTemplate	lTemplate;
	CString			lFontName;
	WORD			lFontSize;

	if	(mWritableTemplate)
	{
		GlobalFree (mWritableTemplate);
		mWritableTemplate = NULL;
	}

	if	(CLocalize::LoadDialog (lTemplate, pTemplateId, pLangId))
	{
		if	(
				(lTemplate.GetFont (lFontName, lFontSize))
			&&	(
					(lFontName.CompareNoCase (_T("MS Sans Serif")) == 0)
				||	(lFontName.CompareNoCase (_T("MS Shell Dlg")) == 0)
				)
			)
		{
			lTemplate.SetFont (_T("MS Shell Dlg"), lFontSize);
		}
		mWritableTemplate = lTemplate.Detach ();
		return (LPCDLGTEMPLATE)mWritableTemplate;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

CString CPropPageFix::GetTemplateCaption (LPCDLGTEMPLATE pTemplate)
{
	CString	lCaption;
	LPBYTE	lTemplateData;

	if	(lTemplateData = (LPBYTE) pTemplate)
	{
		if	(((LPWORD)lTemplateData)[1] == 0xFFFF)
		{
			lTemplateData += sizeof(DLGTEMPLATE) + sizeof(WORD)*2 + sizeof(DWORD);
		}
		else
		{
			lTemplateData += sizeof(DLGTEMPLATE);
		}
		if	(((LPWORD)lTemplateData)[0] == 0xFFFF)
		{
			lTemplateData += sizeof(WORD)*2;
		}
		else
		if	(((LPWORD)lTemplateData)[0] == 0)
		{
			lTemplateData += sizeof(WORD);
		}
		else
		{
			lTemplateData += sizeof(WORD);
			lTemplateData += wcslen ((LPCWSTR)lTemplateData) * sizeof(WCHAR) + sizeof(WCHAR);
		}
		if	(((LPWORD)lTemplateData)[0] == 0xFFFF)
		{
			lTemplateData += sizeof(WORD)*2;
		}
		else
		if	(((LPWORD)lTemplateData)[0] == 0)
		{
			lTemplateData += sizeof(WORD);
		}
		else
		{
			lTemplateData += sizeof(WORD);
			lTemplateData += wcslen ((LPCWSTR)lTemplateData) * sizeof(WCHAR) + sizeof(WCHAR);
		}

		lCaption = (LPCWSTR)lTemplateData;
	}

	return lCaption;
}

LPCDLGTEMPLATE CPropPageFix::SetTemplateCaption (LPCDLGTEMPLATE pTemplate, LPCTSTR pCaption)
{
	LPCDLGTEMPLATE		lRet = pTemplate;
	CDialogTemplate		lOldTemplate (pTemplate);
	CDialogTemplate		lNewTemplate;
	LPCWSTR				lOldCaption = NULL;
	CString				lNewCaption (pCaption);
	long				lOldCaptionSize;
	long				lNewCaptionSize;
	LPBYTE				lOldTemplateData;
	LPBYTE				lNewTemplateData;
	LPBYTE				lSrcByte;
	LPBYTE				lTrgByte;
	WORD				lSignature;
	DWORD				lDlgStyle;
	long				lFontSize;

	if	(lOldTemplateData = (LPBYTE) GlobalLock (lOldTemplate.m_hTemplate))
	{
		lSrcByte = lOldTemplateData;
		lSignature = ((LPWORD)lSrcByte)[1];

		if	(lSignature == 0xFFFF)
		{
			lDlgStyle = ((LPCDLGTEMPLATE) (lSrcByte+sizeof(WORD)*2+sizeof(DWORD)*2))->style;
			lSrcByte += sizeof(DLGTEMPLATE) + sizeof(WORD)*2 + sizeof(DWORD);
		}
		else
		{
			lDlgStyle = ((LPCDLGTEMPLATE) lSrcByte)->style;
			lSrcByte += sizeof(DLGTEMPLATE);
		}
		if	(((LPWORD)lSrcByte)[0] == 0xFFFF)
		{
			lSrcByte += sizeof(WORD)*2;
		}
		else
		if	(((LPWORD)lSrcByte)[0] == 0)
		{
			lSrcByte += sizeof(WORD);
		}
		else
		{
			lSrcByte += sizeof(WORD);
			lSrcByte += wcslen ((LPCWSTR)lSrcByte) * sizeof(WCHAR) + sizeof(WCHAR);
		}
		if	(((LPWORD)lSrcByte)[0] == 0xFFFF)
		{
			lSrcByte += sizeof(WORD)*2;
		}
		else
		if	(((LPWORD)lSrcByte)[0] == 0)
		{
			lSrcByte += sizeof(WORD);
		}
		else
		{
			lSrcByte += sizeof(WORD);
			lSrcByte += wcslen ((LPCWSTR)lSrcByte) * sizeof(WCHAR) + sizeof(WCHAR);
		}

		lOldCaption = (LPCWSTR)lSrcByte;

		if	(wcscmp (lNewCaption, lOldCaption) != 0)
		{
			lOldCaptionSize = (long)(wcslen(lOldCaption) + 1) * sizeof (WCHAR);
			lNewCaptionSize = (long)(wcslen(lNewCaption) + 1) * sizeof (WCHAR);

			lNewTemplate.m_dwTemplateSize = lOldTemplate.m_dwTemplateSize + lNewCaptionSize	- lOldCaptionSize + sizeof (DWORD);

			if	(
					(lNewTemplate.m_hTemplate = GlobalAlloc (GPTR, lNewTemplate.m_dwTemplateSize))
				&&	(lNewTemplateData = (LPBYTE) GlobalLock (lNewTemplate.m_hTemplate))
				)
			{
				lSrcByte = lOldTemplateData;
				lTrgByte = lNewTemplateData;
				memcpy (lTrgByte, lSrcByte, (LPBYTE)lOldCaption-lOldTemplateData);
				lSrcByte += (LPBYTE)lOldCaption-lOldTemplateData;
				lTrgByte += (LPBYTE)lOldCaption-lOldTemplateData;
				memcpy (lTrgByte, (LPCWSTR)lNewCaption, (wcslen(lNewCaption)+1) * sizeof(WCHAR));
				lSrcByte += lOldCaptionSize;
				lTrgByte += lNewCaptionSize;

				if	(lSignature == 0xFFFF)
				{
					if	(lDlgStyle & (DS_SETFONT|DS_SHELLFONT))
					{
						lFontSize = sizeof(WORD)*3 + (long)(wcslen((LPCWSTR)(lSrcByte+sizeof(WORD)*3)) + 1) * sizeof (WCHAR);
						memcpy (lTrgByte, lSrcByte, lFontSize);
						lSrcByte += lFontSize;
						lTrgByte += lFontSize;
						lSrcByte = DataAlign (lSrcByte, DWORD);
						lTrgByte = DataAlign (lTrgByte, DWORD);
					}
				}
				else
				{
					if	(lDlgStyle & DS_SETFONT)
					{
						lFontSize = sizeof(WORD) + (long)(wcslen((LPCWSTR)(lSrcByte+sizeof(WORD))) + 1) * sizeof (WCHAR);
						memcpy (lTrgByte, lSrcByte, lFontSize);
						lSrcByte += lFontSize;
						lTrgByte += lFontSize;
						lSrcByte = DataAlign (lSrcByte, DWORD);
						lTrgByte = DataAlign (lTrgByte, DWORD);
					}
				}

				memcpy (lTrgByte, lSrcByte, lOldTemplate.m_dwTemplateSize-(lSrcByte-lOldTemplateData));

				Swap (mWritableTemplate, lNewTemplate.m_hTemplate);
				GlobalUnlock (lNewTemplate.m_hTemplate);
				lRet = (LPCDLGTEMPLATE)lNewTemplateData;
			}
		}

		GlobalUnlock (lOldTemplate.m_hTemplate);
	}

	return lRet;
}
