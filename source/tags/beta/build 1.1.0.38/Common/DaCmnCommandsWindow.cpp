/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include "DaCmnCommandsWindow.h"
#include "VoiceCommandsWnd.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnCommandsWindow::CDaCmnCommandsWindow ()
:	mVoiceCommandsWnd (NULL)
{
}

CDaCmnCommandsWindow::~CDaCmnCommandsWindow ()
{
}

/////////////////////////////////////////////////////////////////////////////

void CDaCmnCommandsWindow::Initialize (CVoiceCommandsWnd * pVoiceCommandsWnd)
{
	mVoiceCommandsWnd = pVoiceCommandsWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommandsWindow::GetPosition (long *Left, long *Top)
{
	HRESULT	lResult = S_OK;
	CRect	lWinRect (0,0,0,0);

	if	(
			(mVoiceCommandsWnd)
		&&	(mVoiceCommandsWnd->IsWindow())
		)
	{
		mVoiceCommandsWnd->GetWindowRect (&lWinRect);
	}
	else
	{
		lResult = S_FALSE;
	}
	if	(Left)
	{
		(*Left) = lWinRect.left;
	}
	if	(Top)
	{
		(*Top) = lWinRect.top;
	}
	return lResult;
}

HRESULT CDaCmnCommandsWindow::GetSize (long *Width, long *Height)
{
	HRESULT	lResult = S_OK;
	CRect	lWinRect (0,0,0,0);

	if	(
			(mVoiceCommandsWnd)
		&&	(mVoiceCommandsWnd->IsWindow())
		)
	{
		mVoiceCommandsWnd->GetWindowRect (&lWinRect);
	}
	else
	{
		lResult = S_FALSE;
	}
	if	(Width)
	{
		(*Width) = lWinRect.Width();
	}
	if	(Height)
	{
		(*Height) = lWinRect.Height();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommandsWindow::get_Visible (VARIANT_BOOL *Visible)
{
	HRESULT	lResult = S_FALSE;

	if	(
			(mVoiceCommandsWnd)
		&&	(mVoiceCommandsWnd->IsWindow())
		)
	{
		lResult = mVoiceCommandsWnd->IsWindowVisible () ? S_OK : S_FALSE;
	}
	if	(Visible)
	{
		(*Visible) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnCommandsWindow::put_Visible (VARIANT_BOOL Visible)
{
	HRESULT	lResult = S_OK;

	if	(Visible)
	{
		if	(
				(mVoiceCommandsWnd)
			&&	(mVoiceCommandsWnd->IsWindow())
			)
		{
			if	(mVoiceCommandsWnd->IsWindowVisible ())
			{
				lResult = S_FALSE;
			}
			else
			{
				mVoiceCommandsWnd->Show (true);
			}
		}
		else
		{
			lResult = E_FAIL;
		}
	}
	else
	{
		if	(
				(mVoiceCommandsWnd)
			&&	(mVoiceCommandsWnd->IsWindow())
			)
		{
			if	(mVoiceCommandsWnd->IsWindowVisible ())
			{
				mVoiceCommandsWnd->Hide ();
			}
			else
			{
				lResult = S_FALSE;
			}
		}
		else
		{
			lResult = S_FALSE;
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnCommandsWindow::get_Left (short *Left)
{
	long	lLeft = 0;
	HRESULT	lResult = GetPosition (&lLeft, NULL);

	if	(Left)
	{
		(*Left) = (short)lLeft;
	}
	return lResult;
}

HRESULT CDaCmnCommandsWindow::get_Top (short *Top)
{
	long	lTop = 0;
	HRESULT	lResult = GetPosition (NULL, &lTop);

	if	(Top)
	{
		(*Top) = (short)lTop;
	}
	return lResult;
}

HRESULT CDaCmnCommandsWindow::get_Height (short *Height)
{
	long	lHeight = 0;
	HRESULT	lResult = GetSize (NULL, &lHeight);

	if	(Height)
	{
		(*Height) = (short)lHeight;
	}
	return lResult;
}

HRESULT CDaCmnCommandsWindow::get_Width (short *Width)
{
	long	lWidth = 0;
	HRESULT	lResult = GetSize (&lWidth, NULL);

	if	(Width)
	{
		(*Width) = (short)lWidth;
	}
	return lResult;
}
