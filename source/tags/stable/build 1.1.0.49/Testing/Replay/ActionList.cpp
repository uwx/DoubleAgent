#include "StdAfx.h"
#include "ReplayActions.h"
#include "ActionList.h"
#include "LocalizeEx.h"
#include "DebugStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CActionList::CActionList()
{
}

CActionList::~CActionList()
{
}

/////////////////////////////////////////////////////////////////////////////

void CActionList::PreSubclassWindow ()
{
	int	lColumnCount = 0;
	int	lColumnNdx;

	CListCtrl::PreSubclassWindow ();

	SetExtendedStyle (GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP);

	InsertColumn (lColumnCount, _T("Time"), LVCFMT_LEFT, -1, lColumnCount);
	lColumnCount++;
	InsertColumn (lColumnCount, _T("Action"), LVCFMT_LEFT, -1, lColumnCount);
	lColumnCount++;

	for	(lColumnNdx = 0; lColumnNdx < lColumnCount; lColumnNdx++)
	{
		SetColumnWidth (lColumnNdx, LVSCW_AUTOSIZE_USEHEADER);
	}
}

/////////////////////////////////////////////////////////////////////////////

int CActionList::ShowAction (CActionLine * pActionLine)
{
	int	lItemNdx = -1;

	if	(pActionLine)
	{
		tS <SYSTEMTIME>	lActionTime;
		CString			lActionTimeStr;
		int				lColumnCount;
		int				lColumnNdx;

		FileTimeToSystemTime (&pActionLine->mTime, &lActionTime);
		lActionTimeStr.Format (_T("%s %s.%3.3hu"), LclFormatDate (lActionTime, DATE_SHORTDATE), LclFormatTime (lActionTime, TIME_FORCE24HOURFORMAT|TIME_FORCETIMEMARKER|TIME_FORCESECONDS), lActionTime.wMilliseconds);
		lItemNdx = InsertItem (GetItemCount(), lActionTimeStr);
		SetItemText (lItemNdx, 1, pActionLine->mAction);

		lColumnCount = GetHeaderCtrl()->GetItemCount ();
		while	(lColumnCount-2 < pActionLine->mValues.GetSize())
		{
			InsertColumn (lColumnCount, _T(""), LVCFMT_LEFT, -1, lColumnCount);
			SetColumnWidth (lColumnCount, LOWORD(GetDialogBaseUnits()) * 20);
			lColumnCount++;
		}
		for	(lColumnNdx = 0; lColumnNdx < pActionLine->mValues.GetSize(); lColumnNdx++)
		{
			SetItemText (lItemNdx, lColumnNdx+2, DebugStr(pActionLine->mValues [lColumnNdx]));
		}

		if	(
				(GetItemCount() == 1)
			||	(lColumnCount <= 2)
			)
		{
			SetColumnWidth (0, LVSCW_AUTOSIZE);
			SetColumnWidth (0, GetColumnWidth (0) + 8);
			SetColumnWidth (1, LVSCW_AUTOSIZE_USEHEADER);
			SetColumnWidth (1, GetColumnWidth (1) * 3);
		}

		SetItemState (lItemNdx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		EnsureVisible (lItemNdx, FALSE);
	}
	return lItemNdx;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CActionList, CListCtrl)
	//{{AFX_MSG_MAP(CActionList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
