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
#if !defined(AFX_FORMLAYOUT_H__78A4468B_926D_4AF5_8DFE_B6226583E75F__INCLUDED_)
#define AFX_FORMLAYOUT_H__78A4468B_926D_4AF5_8DFE_B6226583E75F__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CFormLayout
{
public:
	static bool AlignTo (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin = NULL);
	static bool AlignPos (CWnd * pAlign, CWnd * pAlignTo);
	static bool AlignSize (CWnd * pAlign, CWnd * pAlignTo, CSize * pMult = NULL);

	static bool AlignLeft (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);
	static bool AlignRight (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);
	static bool AlignTop (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);
	static bool AlignBottom (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);

	static bool AlignCenter (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin = NULL);
	static bool AlignVCenter (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin = NULL);
	static bool AlignWidth (CWnd * pAlign, CWnd * pAlignTo, float pMult = 0.0f);
	static bool AlignHeight (CWnd * pAlign, CWnd * pAlignTo, float pMult = 0.0f);

	static bool AlignAfter (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);
	static bool AlignBefore (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);
	static bool AlignBelow (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);
	static bool AlignAbove (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);

	static bool UpdateSize (CWnd * pWnd, const CSize & pSize);
	static bool UpdateWidth (CWnd * pWnd, int pWidth);
	static bool UpdateHeight (CWnd * pWnd, int pHeight);

	static bool AdjustRect (CWnd * pWnd, const CPoint & pOffset);
	static bool AdjustRect (CWnd * pWnd, const CRect & pMargin);

	static CRect ChildWndRect (CWnd & pWnd, CWnd * pParentWnd = NULL);
	static CPoint ChildWndPos (CWnd & pWnd, CWnd * pParentWnd = NULL);
	static CSize ChildWndSize (CWnd & pWnd);
	static CSize ChildWndGap (CWnd & pWnd1, CWnd & pWnd2);
	static CPoint ChildWndMid (CWnd & pWnd1, CWnd & pWnd2, CWnd * pParentWnd = NULL);

	static CRect ChildUsedRect (CWnd & pWnd, bool pCheckVisible = true);

	static bool OrderAfter (CWnd * pOrder, const CWnd * pOrderAfter);
	static bool OrderBefore (CWnd * pOrder, const CWnd * pOrderBefore);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_FORMLAYOUT_H__78A4468B_926D_4AF5_8DFE_B6226583E75F__INCLUDED_)
