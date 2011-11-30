/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "PropSheetOptions.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnPropertySheet : public CPropSheetOptions
{
public:
	CDaCmnPropertySheet (_AtlPropSheetOwner* pOwner, class CListeningGlobal& pListeningGlobal);
	virtual ~CDaCmnPropertySheet ();

// Interfaces
public:
	// ICDaPropertySheet2
	HRESULT GetPosition (long *Left, long *Top);
	HRESULT SetPosition (long Left, long Top);
	HRESULT GetSize (long *Width, long *Height);
	HRESULT put_Left (short Left);
	HRESULT get_Left (short *Left);
	HRESULT put_Top (short Top);
	HRESULT get_Top (short *Top);
	HRESULT get_Height (short *Height);
	HRESULT get_Width (short *Width);
	HRESULT put_Visible (VARIANT_BOOL Visible);
	HRESULT get_Visible (VARIANT_BOOL *Visible);
	HRESULT put_Page (BSTR Page);
	HRESULT get_Page (BSTR *Page);

// Implementation
protected:
	HWND GetPropSheetWnd (bool pCreate);

protected:
	_AtlPropSheetOwner*	mOwner;
};

/////////////////////////////////////////////////////////////////////////////
