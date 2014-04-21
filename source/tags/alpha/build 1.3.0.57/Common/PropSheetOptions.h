/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "PropertySheet.h"

/////////////////////////////////////////////////////////////////////////////

class CPropSheetOptions : public CAtlPropertySheet
{
public:
	CPropSheetOptions (class CListeningGlobal& pListeningGlobal, HWND pParentWnd = NULL);
	virtual ~CPropSheetOptions();

// Attributes

// Operations
public:
	bool LoadLayout (CRect& pWinRect);
	int LoadStartPage ();
	void SaveLayout (const CRect& pWinRect);
	void SaveStartPage (int pStartPageNdx);

// Overrides
public:
	virtual INT_PTR DoModal ();
protected:
	virtual bool PreCreateSheet (bool pModal);
	virtual void PreShowSheet ();
	virtual void OnFinalMessage (HWND);
	virtual void LoadConfig ();
	virtual void SaveConfig (int pSheetResult);
	virtual void OnApplied ();

// Implementation
protected:
	CListeningGlobal& mListeningGlobal;
};

/////////////////////////////////////////////////////////////////////////////
