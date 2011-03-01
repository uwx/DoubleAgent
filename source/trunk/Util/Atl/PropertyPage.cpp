/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include "PropertyPage.h"
#include "Localize.h"

/////////////////////////////////////////////////////////////////////////////

CAtlPropertyPage::CAtlPropertyPage (UINT pTemplateId)
{
	mPsp.dwFlags = PSP_USECALLBACK;
	mPsp.hInstance = _AtlBaseModule.GetResourceInstance ();
	mPsp.pszTemplate = MAKEINTRESOURCE(pTemplateId);
	mPsp.pfnDlgProc = StartDialogProc;
	mPsp.pfnCallback = PropPageCallback;
	mPsp.lParam = (LPARAM)this;

	mCaption = CLocalize::LoadString (pTemplateId);
	if	(!mCaption.IsEmpty ())
	{
		mPsp.pszTitle = (LPCTSTR)mCaption;
		mPsp.dwFlags |= PSP_USETITLE;
	}
}

CAtlPropertyPage::~CAtlPropertyPage()
{
}

/////////////////////////////////////////////////////////////////////////////

UINT CALLBACK CAtlPropertyPage::PropPageCallback (HWND hwnd, UINT message, PROPSHEETPAGE * pPropPage)
{
	switch (message)
	{
		case PSPCB_CREATE:
		{
			CAtlPropertyPage *	lThis = (CAtlPropertyPage *) pPropPage->lParam;

			lThis->m_thunk.Init (NULL, NULL);
			_AtlWinModule.AddCreateWndData (&lThis->m_thunk.cd, lThis);
		}
		break;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAtlPropertyPage::HandleInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = DefWindowProc ();

	if	(!OnInitDialog ())
	{
		lResult = FALSE;
	}
	return lResult;
}
