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
#include <math.h>
#include "FormLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

bool CFormLayout::AlignTo (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}
		if	(pMargin)
		{
			lRect2.DeflateRect (*pMargin);
		}

		if	(!lRect2.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect2);
			}
			pAlign->MoveWindow (&lRect2);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignPos (CWnd * pAlign, CWnd * pAlignTo)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		lRect3.OffsetRect (lRect2.left - lRect1.left, lRect2.top - lRect1.top);

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignSize (CWnd * pAlign, CWnd * pAlignTo, CSize * pMult)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		pAlignTo->GetWindowRect (&lRect2);

		lRect3 = lRect1;
		if	(pMult)
		{
			lRect2.OffsetRect (-lRect2.TopLeft ());
			lRect2.right *= pMult->cx;
			lRect2.bottom *= pMult->cy;
		}
		lRect3 = CRect (lRect1.TopLeft (), lRect2.Size ());

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CFormLayout::AlignLeft (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.left = lRect2.left + pOffset;
		}
		else
		{
			lRect3.OffsetRect (lRect2.left - lRect1.left + pOffset, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignRight (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.right = lRect2.right + pOffset;
		}
		else
		{
			lRect3.OffsetRect (lRect2.right - lRect1.right + pOffset, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignTop (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.top = lRect2.top + pOffset;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.top - lRect1.top + pOffset);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignBottom (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.bottom = lRect2.bottom + pOffset;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.bottom - lRect1.bottom + pOffset);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CFormLayout::AlignCenter (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}
		if	(pMargin)
		{
			lRect2.DeflateRect (*pMargin);
		}

		lRect3 = lRect1;
		lRect3.OffsetRect (lRect2.CenterPoint ().x - lRect1.CenterPoint ().x, 0);

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignVCenter (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}
		if	(pMargin)
		{
			lRect2.DeflateRect (*pMargin);
		}

		lRect3 = lRect1;
		lRect3.OffsetRect (0, lRect2.CenterPoint ().y - lRect1.CenterPoint ().y);

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignWidth (CWnd * pAlign, CWnd * pAlignTo, float pMult)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		if	(pMult > 0.0f)
		{
			lRect3.right = lRect3.left + (long) floor ((float) lRect2.Width () * pMult);
		}
		else
		{
			lRect3.right = lRect3.left + lRect2.Width ();
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignHeight (CWnd * pAlign, CWnd * pAlignTo, float pMult)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		if	(pAlignTo->IsChild (pAlign))
		{
			pAlignTo->GetClientRect (&lRect2);
			pAlignTo->ClientToScreen (&lRect2);
		}
		else
		{
			pAlignTo->GetWindowRect (&lRect2);
		}

		lRect3 = lRect1;
		if	(pMult > 0.0f)
		{
			lRect3.bottom = lRect3.top + (long) floor ((float) lRect2.Height () * pMult);
		}
		else
		{
			lRect3.bottom = lRect3.top + lRect2.Height ();
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CFormLayout::AlignAfter (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		pAlignTo->GetWindowRect (&lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.left = lRect2.right + pGap;
		}
		else
		{
			lRect3.OffsetRect (lRect2.right - lRect1.left + pGap, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignBefore (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		pAlignTo->GetWindowRect (&lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.right = lRect2.left - pGap;
		}
		else
		{
			lRect3.OffsetRect (lRect2.left - lRect1.right - pGap, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignBelow (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		pAlignTo->GetWindowRect (&lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.top = lRect2.bottom + pGap;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.bottom - lRect1.top + pGap);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

bool CFormLayout::AlignAbove (CWnd * pAlign, CWnd * pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign->GetSafeHwnd ())
		&&	(pAlignTo->GetSafeHwnd ())
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;
		CWnd *	lParent;

		pAlign->GetWindowRect (&lRect1);
		pAlignTo->GetWindowRect (&lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.bottom = lRect2.top - pGap;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.top - lRect1.bottom - pGap);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			if	(lParent = pAlign->GetParent ())
			{
				lParent->ScreenToClient (&lRect3);
			}
			pAlign->MoveWindow (&lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CFormLayout::UpdateSize (CWnd * pWnd, const CSize & pSize)
{
	if	(pWnd->GetSafeHwnd ())
	{
		CRect	lRect;
		CWnd *	lParent;

		pWnd->GetWindowRect (&lRect);

		if	(lRect.Size() != pSize)
		{
			lRect.right = lRect.left + pSize.cx;
			lRect.bottom = lRect.top + pSize.cy;
			if	(lParent = pWnd->GetParent ())
			{
				lParent->ScreenToClient (&lRect);
			}
			pWnd->MoveWindow (&lRect);
			return true;
		}
	}
	return false;
}

bool CFormLayout::UpdateWidth (CWnd * pWnd, int pWidth)
{
	if	(pWnd->GetSafeHwnd ())
	{
		CRect	lRect;
		CWnd *	lParent;

		pWnd->GetWindowRect (&lRect);

		if	(lRect.Width() != pWidth)
		{
			lRect.right = lRect.left + pWidth;
			if	(lParent = pWnd->GetParent ())
			{
				lParent->ScreenToClient (&lRect);
			}
			pWnd->MoveWindow (&lRect);
			return true;
		}
	}
	return false;
}

bool CFormLayout::UpdateHeight (CWnd * pWnd, int pHeight)
{
	if	(pWnd->GetSafeHwnd ())
	{
		CRect	lRect;
		CWnd *	lParent;

		pWnd->GetWindowRect (&lRect);

		if	(lRect.Height() != pHeight)
		{
			lRect.bottom = lRect.top + pHeight;
			if	(lParent = pWnd->GetParent ())
			{
				lParent->ScreenToClient (&lRect);
			}
			pWnd->MoveWindow (&lRect);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CFormLayout::AdjustRect (CWnd * pWnd, const CPoint & pOffset)
{
	if	(pWnd->GetSafeHwnd ())
	{
		CRect	lRect;
		CWnd *	lParent;

		pWnd->GetWindowRect (&lRect);
		if	(lParent = pWnd->GetParent ())
		{
			lParent->ScreenToClient (&lRect);
		}
		lRect.OffsetRect (pOffset);
		pWnd->MoveWindow (&lRect);

		return true;
	}
	return false;
}

bool CFormLayout::AdjustRect (CWnd * pWnd, const CRect & pMargin)
{
	if	(pWnd->GetSafeHwnd ())
	{
		CRect	lRect;
		CWnd *	lParent;

		pWnd->GetWindowRect (&lRect);
		if	(lParent = pWnd->GetParent ())
		{
			lParent->ScreenToClient (&lRect);
		}
		lRect.InflateRect (pMargin);
		pWnd->MoveWindow (&lRect);

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

CRect CFormLayout::ChildWndRect (CWnd & pWnd, CWnd * pParentWnd)
{
	CRect	lRect;
	CWnd *	lParentWnd = pParentWnd ? pParentWnd : pWnd.GetParent ();

	pWnd.GetWindowRect (&lRect);
	lParentWnd->ScreenToClient (&lRect);
	return lRect;
}

CPoint CFormLayout::ChildWndPos (CWnd & pWnd, CWnd * pParentWnd)
{
	CRect	lRect;
	CWnd *	lParentWnd = pParentWnd ? pParentWnd : pWnd.GetParent ();

	pWnd.GetWindowRect (&lRect);
	lParentWnd->ScreenToClient (&lRect);
	return lRect.TopLeft ();
}

CSize CFormLayout::ChildWndSize (CWnd & pWnd)
{
	CRect	lRect;

	pWnd.GetWindowRect (&lRect);
	return lRect.Size ();
}

CSize CFormLayout::ChildWndGap (CWnd & pWnd1, CWnd & pWnd2)
{
	CRect	lRect1;
	CRect	lRect2;

	pWnd1.GetWindowRect (&lRect1);
	pWnd2.GetWindowRect (&lRect2);

	return CSize (max (lRect1.left-lRect2.right, lRect2.left-lRect1.right), max (lRect1.top-lRect2.bottom, lRect2.top-lRect1.bottom));
}

CPoint CFormLayout::ChildWndMid (CWnd & pWnd1, CWnd & pWnd2, CWnd * pParentWnd)
{
	CPoint	lMidPoint;
	CRect	lRect1;
	CRect	lRect2;
	CWnd *	lParentWnd = pParentWnd ? pParentWnd : pWnd1.GetParent ();

	pWnd1.GetWindowRect (&lRect1);
	pWnd2.GetWindowRect (&lRect2);

	lMidPoint.x = lRect1.right + (lRect2.left - lRect1.right) / 2;
	lMidPoint.y = lRect1.bottom + (lRect2.top - lRect1.bottom) / 2;

	lParentWnd->ScreenToClient (&lMidPoint);
	return lMidPoint;
}

//////////////////////////////////////////////////////////////////////

CRect CFormLayout::ChildUsedRect (CWnd & pWnd, bool pCheckVisible)
{
	CRect	lRect (-1,-1,-1,-1);
	CRect	lChildRect;
	CWnd *	lChildWnd;
	bool	lFirst = true;

	for	(lChildWnd = pWnd.GetWindow (GW_CHILD); lChildWnd; lChildWnd = lChildWnd->GetWindow (GW_HWNDNEXT))
	{
		if	(
				(!pCheckVisible)
			||	(lChildWnd->GetStyle () & WS_VISIBLE)
			)
		{
			lChildWnd->GetWindowRect (lChildRect);
			pWnd.ScreenToClient (lChildRect);

			if	(lFirst)
			{
				lRect = lChildRect;
				lFirst = false;
			}
			else
			{
				lRect |= lChildRect;
			}
		}
	}

	return lRect;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CFormLayout::OrderAfter (CWnd * pOrder, const CWnd * pOrderAfter)
{
	HWND	lOrder;
	HWND	lOrderAfter;

	if	(
			(lOrder = pOrder->GetSafeHwnd ())
		&&	(::IsWindow (lOrder))
		&&	(
				(
					(pOrderAfter == &CWnd::wndBottom)
				&&	((lOrderAfter = HWND_BOTTOM) || true)
				)
			||	(
					(pOrderAfter == &CWnd::wndTop)
				&&	((lOrderAfter = HWND_TOP) || true)
				)
			||	(
					(lOrderAfter = pOrderAfter->GetSafeHwnd ())
				&&	(::IsWindow (lOrderAfter))
				&&	(::GetParent (lOrder) == ::GetParent (lOrderAfter))
				)
			)
		&&	(::SetWindowPos (lOrder, lOrderAfter, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE))
		)
	{
		return true;
	}
	return false;
}

bool CFormLayout::OrderBefore (CWnd * pOrder, const CWnd * pOrderBefore)
{
	HWND	lOrder;
	HWND	lOrderBefore;

	if	(
			(lOrder = pOrder->GetSafeHwnd ())
		&&	(lOrderBefore = pOrderBefore->GetSafeHwnd ())
		&&	(::IsWindow (lOrder))
		&&	(::IsWindow (lOrderBefore))
		&&	(::GetParent (lOrder) == ::GetParent (lOrderBefore))
		&&	(::SetWindowPos (lOrder, ::GetWindow (lOrderBefore, GW_HWNDPREV), 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE))
		)
	{
		return true;
	}
	return false;
}
