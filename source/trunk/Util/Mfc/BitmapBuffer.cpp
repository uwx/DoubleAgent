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
#include "StdAfx.h"
#include "BitmapBuffer.h"
#include "BitmapTools.h"
#include "BitmapAlpha.h"
#include "Log.h"
#ifdef	_DEBUG
#include "BitmapDebugger.h"
#endif
#ifdef	_DEBUG_BITMAP_BUFFERS
#include "LocalizeEx.h"
#include "DebugTrace.h"
#endif

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#ifdef	_DEBUG
//#define	_DEBUG_BUFFERS	LogDebugFast
//#define	_DEBUG_SCALING	LogDebugFast
#endif

//////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
static inline DWORD BitmapBytes (const CBitmap& pBitmap)
{
	tS <BITMAP>	lBitmap;
	if	(
			(pBitmap.GetSafeHandle ())
		&&	(const_cast <CBitmap&> (pBitmap).GetBitmap (&lBitmap))
		)
	{
		return lBitmap.bmWidthBytes * lBitmap.bmHeight;
	}
	return 0;
}
#endif
//////////////////////////////////////////////////////////////////////

CBitmapBuffer::CBitmapBuffer (LPCTSTR pDebugName)
:	mOldBitmap (NULL),
	mBitmapBits (NULL),
	mOldFont (NULL)
{
#ifdef	_DEBUG_BITMAP_BUFFERS
	CSingleLock	lLock (&mUsageCriticalSection, TRUE);
	try
	{
		mDebugName = pDebugName;
		mCreated = 0;
		mDeleted = 0;
		mUsed = 0;
		mStartTime = 0;
		mInUseTime = 0.0;
		mInstances.Add (this);
		mMaxInstances = max (mMaxInstances, (UINT) mInstances.GetSize ());
		mTotalInstances++;
	}
	catch AnyExceptionSilent
#endif
}

CBitmapBuffer::~CBitmapBuffer ()
{
	EndBuffer (true, true);

#ifdef	_DEBUG_BITMAP_BUFFERS
	try
	{
		if	(mUsageCriticalSection.IsKindOf (RUNTIME_CLASS (CSyncObject)))
		{
			CSingleLock	lLock (&mUsageCriticalSection, TRUE);
			try
			{
				for	(int lNdx = mInstances.GetUpperBound(); lNdx >= 0; lNdx--)
				{
					if	(mInstances [lNdx] == this)
					{
						mInstances.RemoveAt (lNdx);
						break;
					}
				}
			}
			catch AnyExceptionSilent
		}
	}
	catch AnyExceptionSilent
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CBitmapBuffer::StartBuffer (CDC* pCompatibleDC)
{
#ifdef	_DEBUG_BITMAP_BUFFERS
	DebugTimeStart
	DebugTimePush (mStartTime);
#endif
	if	(mBitmap.GetSafeHandle ())
	{
		if	(mOldBitmap)
		{
			mDC.SelectObject (mOldBitmap);
		}

		if	(mDC.GetSafeHdc ())
		{
			mOldBitmap = mDC.SelectObject (mBitmap);
			return true;
		}
		else
		if	(mDC.CreateCompatibleDC (pCompatibleDC))
		{
			mDC.SetBkColor (RGB (0xFF, 0xFF, 0xFF));
			mDC.SetTextColor (RGB (0x00, 0x00, 0x00));
			mOldBitmap = mDC.SelectObject (mBitmap);
#ifdef	_DEBUG_BITMAP_BUFFERS
			mUsed++;
			mNowInUse++;
			mTotalInUse++;
			mMaxInUse = max (mMaxInUse, mNowInUse);
#endif
			return true;
		}
	}
	return false;
}

void CBitmapBuffer::EndBuffer (bool pReleaseBuffer, bool pReleaseDC)
{
	if	(mDC.GetSafeHdc ())
	{
		if	(mOldBitmap)
		{
			mDC.SelectObject (mOldBitmap);
			mOldBitmap = NULL;
		}
		if	(pReleaseDC)
		{
#ifdef	_DEBUG_BITMAP_BUFFERS
			if	(mNowInUse > 0)
			{
				mNowInUse--;
			}
#endif
			if	(mOldFont)
			{
				mDC.SelectObject (mOldFont);
				mOldFont = NULL;
			}
			mDC.DeleteDC ();
		}
		else
		{
			mDC.SetWindowOrg (0, 0);
		}
#ifdef	_DEBUG_BITMAP_BUFFERS
		{
			DebugTimeDefine
			DebugTimeStop
			DebugTimePop (mStartTime);
			mInUseTime += DebugTimeElapsed;
			mTotalInUseTime += DebugTimeElapsed;
		}
#endif
	}
	else
	{
		mOldFont = NULL;
		mOldBitmap = NULL;
	}

	if	(pReleaseBuffer)
	{
		if	(mBitmap.GetSafeHandle ())
		{
#ifdef	_DEBUG_BITMAP_BUFFERS
			DebugTimeStart
#endif
			mBitmap.DeleteObject ();
#ifdef	_DEBUG_BITMAP_BUFFERS
			DebugTimeStop
			mTotalDeleteTime += DebugTimeElapsed;
			mDeleted++;
			mTotalDeleted++;
#endif
		}
		mBitmapBits = NULL;
	}
}

bool CBitmapBuffer::PauseBuffer ()
{

	if	(
			(mDC.GetSafeHdc ())
		&&	(mOldBitmap)
		)
	{
		mDC.SelectObject (mOldBitmap);
		mOldBitmap = NULL;
		GdiFlush ();
		return true;
	}
	return false;
}

bool CBitmapBuffer::ResumeBuffer ()
{
	if	(
			(mDC.GetSafeHdc ())
		&&	(mBitmap.GetSafeHandle ())
		)
	{
		GdiFlush ();
		if	(mOldBitmap)
		{
			mDC.SelectObject (mOldBitmap);
		}
		mOldBitmap = mDC.SelectObject (mBitmap);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CBitmapBuffer::CreateScaledBuffer (const CSize& pSize, bool pForBlending, UINT pScale)
{
	CSize	lSize (pSize);

	EndBuffer ();

	lSize.cx *= pScale;
	lSize.cy *= pScale;

	mBitmapBits = NULL;

#ifdef	_DEBUG_BUFFERS
	if	(mDebugName.IsEmpty ())
	{
		LogMessage (_DEBUG_BUFFERS, _T("* CreateScaledBuffer Unnamed"));
	}
	else
	{
		LogMessage (_DEBUG_BUFFERS, _T("CreateScaledBuffer %s"), mDebugName);
	}
#endif

#ifdef	_DEBUG_BITMAP_BUFFERS
	DebugTimeStart
#endif

	if	(CBitmapTools::CreateColorBitmap (lSize, mBitmap, &mBitmapBits, pForBlending))
	{
#ifdef	_DEBUG_BITMAP_BUFFERS
		DebugTimeStop
		mTotalCreateTime += DebugTimeElapsed;
		mCreated++;
		mTotalCreated++;
#endif
		if	(StartBuffer ())
		{
			if	(pScale > 1)
			{
				mDC.SetMapMode (MM_ANISOTROPIC);
				mDC.ScaleViewportExt (pScale, 1, pScale, 1);
			}
			return true;
		}
	}

	EndBuffer ();

	if	(mBitmap.GetSafeHandle ())
	{
#ifdef	_DEBUG_BITMAP_BUFFERS
		DebugTimeStart
#endif
		mBitmap.DeleteObject ();
#ifdef	_DEBUG_BITMAP_BUFFERS
		DebugTimeStop
		mTotalDeleteTime += DebugTimeElapsed;
		mDeleted++;
		mTotalDeleted++;
#endif
	}
	mBitmapBits = NULL;

	return false;
}

bool CBitmapBuffer::UnscaleBuffer (UINT pScale, UINT pBlend)
{
	bool	lRet = false;

	if	(pScale > 1)
	{
		if	(
				(mBitmap.GetSafeHandle ())
			&&	(mBitmapBits)
			)
		{
			bool	lPaused = PauseBuffer ();

#ifdef	_DEBUG_SCALING
			{
				CBitmapDebugger	lDebugger;
				lDebugger.ShowBitmap (mBitmap, 1, _T("Unshrunk"));
				lDebugger.DoModal ();
			}
#endif
			lRet = CBitmapTools::ShrinkBitmap (mBitmap, mBitmapBits, pScale, pBlend);

#ifdef	_DEBUG_SCALING
			{
				CBitmapDebugger	lDebugger;
				lDebugger.ShowBitmap (mBitmap, 1, _T("shrunk"));
				lDebugger.DoModal ();
			}
#endif
			if	(
					(lPaused)
				&&	(ResumeBuffer ())
				)
			{
				mDC.SetMapMode (MM_TEXT);
			}
		}
	}
	else
	{
		lRet = true;
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CBitmapBuffer::NewBufferRequired (CBitmap& pBitmap, const CSize& pSize, bool pForBlending, int pSizeFactor)
{
	tS <BITMAP>	lBitmap;

	if	(
			(pBitmap.GetSafeHandle ())
		&&	(pBitmap.GetBitmap (&lBitmap))
		&&	(lBitmap.bmWidth >= pSize.cx)
		&&	(lBitmap.bmHeight >= pSize.cy)
		&&	(
				(lBitmap.bmWidth <= (pSize.cx * pSizeFactor))
			||	(lBitmap.bmHeight <= (pSize.cy * pSizeFactor))
			)
		&&	(
				(pForBlending)
			?	(lBitmap.bmBitsPixel == 32)
			:	(lBitmap.bmBitsPixel == 24)
			)
		)
	{
		return false;
	}

#ifdef	_DEBUG_BUFFERS
	if	(
			(pBitmap.GetSafeHandle ())
		&&	(!mDebugName.IsEmpty ())
		)
	{
		LogMessage (_DEBUG_BUFFERS, _T("NewBufferRequired Old [%d %d] New [%d %d] Factor [%d] For [%s]"), lBitmap.bmWidth, lBitmap.bmHeight, pSize.cx, pSize.cy, pSizeFactor, mDebugName);
	}
#endif
	return true;
}

bool CBitmapBuffer::CreateBuffer (const CSize& pBufferSize, bool pForBlending, bool pForceNewBuffer, bool pTopDown)
{
	bool	lRet = false;

	if	(
			(pBufferSize.cx > 0)
		&&	(pBufferSize.cy > 0)
		)
	{
		if	(
				(!pForceNewBuffer)
			&&	(!NewBufferRequired (mBitmap, pBufferSize, pForBlending))
			&&	(StartBuffer ())
			)
		{
			lRet = true;
		}
		else
		{
			CBitmap	lOldBitmap;

#ifdef	_DEBUG_BUFFERS
			if	(mDebugName.IsEmpty ())
			{
				LogMessage (_DEBUG_BUFFERS, _T("* CreateBuffer Unnamed"));
			}
			else
			{
				LogMessage (_DEBUG_BUFFERS, _T("CreateBuffer %s"), mDebugName);
			}
#endif

			lOldBitmap.Attach (mBitmap.Detach ());

#ifdef	_DEBUG_BITMAP_BUFFERS
			DebugTimeStart
#endif

			if	(CBitmapTools::CreateColorBitmap (pBufferSize, mBitmap, &mBitmapBits, pForBlending, pTopDown))
			{
#ifdef	_DEBUG_BITMAP_BUFFERS
				DebugTimeStop
				mTotalCreateTime += DebugTimeElapsed;
				mCreated++;
				mTotalCreated++;
#endif
				if	(StartBuffer ())
				{
					lRet = true;
				}
			}
		}
	}

	return lRet;
}

bool CBitmapBuffer::DrawBuffer (HDC pTargetDC, const CRect& pRect, bool pReleaseBuffer, bool pReleaseDC)
{
	bool	lRet = false;

	try
	{
		if	(
				(!pRect.IsRectEmpty ())
			&&	(mDC.GetSafeHdc ())
			&&	(mBitmap.GetSafeHandle ())
			&&	(pTargetDC)
			)
		{
			BitBlt (pTargetDC, pRect.left, pRect.top, pRect.Width (), pRect.Height (), mDC, pRect.left, pRect.top, SRCCOPY);
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	EndBuffer (pReleaseBuffer, pReleaseDC);

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CBitmapBufferScaled::CBitmapBufferScaled (UINT pScale, UINT pBlend, LPCTSTR pDebugName)
:	CBitmapBuffer (pDebugName),
	mScale (pScale),
	mBlend (pBlend),
	mScaledBits (NULL)
{
}

CBitmapBufferScaled::~CBitmapBufferScaled ()
{
}

//////////////////////////////////////////////////////////////////////

bool CBitmapBufferScaled::CreateBuffer (const CSize& pBufferSize, bool pForBlending, bool pForceNewBuffer, bool pTopDown)
{
	bool	lRet = false;
	CSize	lBufferSize (((pBufferSize.cx + 3) / 4) * 4, ((pBufferSize.cy + 3) / 4) * 4);
	CSize	lScaledSize (lBufferSize.cx * (long) mScale, lBufferSize.cy * (long) mScale);
	CBitmap	lScaledBuffer;
	LPBYTE	lScaledBits;
	CBitmap	lUnscaledBuffer;
	LPBYTE	lUnscaledBits;

#ifdef	_DEBUG_BITMAP_BUFFERS
	DebugTimeStart
	DebugTimePush (mStartTime);
#endif

	if	(
			(lBufferSize.cx > 0)
		&&	(lBufferSize.cy > 0)
		&&	(lScaledSize.cx > 0)
		&&	(lScaledSize.cy > 0)
		)
	{
		if	(
				(
					(
						(!pForceNewBuffer)
					&&	(!NewBufferRequired (mBitmap, lBufferSize, pForBlending, 10))
					&&	(lUnscaledBuffer.Attach (mBitmap.Detach ()))
					&&	(lUnscaledBits = mBitmapBits)
					)
				||	(CBitmapTools::CreateColorBitmap (lBufferSize, lUnscaledBuffer, &lUnscaledBits, pForBlending, pTopDown))
				)
			&&	(
					(
						(!pForceNewBuffer)
					&&	(!NewBufferRequired (mScaledBitmap, lScaledSize, pForBlending, 10))
					&&	(lScaledBuffer.Attach (mScaledBitmap.Detach ()))
					&&	(lScaledBits = mScaledBits)
					)
				||	(CBitmapTools::CreateColorBitmap (lScaledSize, lScaledBuffer, &lScaledBits, pForBlending))
				)
			)
		{
#ifdef	_DEBUG_BITMAP_BUFFERS
			if	(lUnscaledBits != mBitmapBits)
			{
				mCreated++;
				mTotalCreated++;
			}
			if	(lScaledBits != mScaledBits)
			{
				mCreated++;
				mTotalCreated++;
			}
#endif
#ifdef	_DEBUG_BUFFERS
			if	(
					(lUnscaledBits != mBitmapBits)
				||	(lScaledBits != mScaledBits)
				)
			{
				if	(mDebugName.IsEmpty ())
				{
					LogMessage (_DEBUG_BUFFERS, _T("* CreateBuffer Unnamed"));
				}
				else
				{
					LogMessage (_DEBUG_BUFFERS, _T("CreateBuffer %s"), mDebugName);
				}
			}
#endif
#ifdef	_DEBUG_SCALING
			LogMessage (LogNormal|LogTime, _T("StartScaled [%d %d] [%p] [%p] Unscaled [%d %d] [%p] [%p]"), lScaledSize.cx, lScaledSize.cy, lScaledBuffer.GetSafeHandle (), lScaledBits, lBufferSize.cx, lBufferSize.cy, lUnscaledBuffer.GetSafeHandle (), lUnscaledBits);
#endif

			if	(mDC.GetSafeHdc ())
			{
				if	(mOldBitmap)
				{
					mDC.SelectObject (mOldBitmap);
					mOldBitmap = NULL;
				}
				if	(mOldFont)
				{
					mDC.SelectObject (mOldFont);
					mOldFont = NULL;
				}
				mDC.DeleteDC ();
#ifdef	_DEBUG_BITMAP_BUFFERS
				if	(mNowInUse > 0)
				{
					mNowInUse--;
				}
#endif
#ifdef	_DEBUG_BITMAP_BUFFERS
				{
					DebugTimeDefine
					DebugTimeStop
					DebugTimePop (mStartTime);
					mInUseTime += DebugTimeElapsed;
					mTotalInUseTime += DebugTimeElapsed;
				}
#endif
			}
			else
			{
				mOldBitmap = NULL;
				mOldFont = NULL;
			}

			if	(mBitmap.GetSafeHandle ())
			{
#ifdef	_DEBUG_BITMAP_BUFFERS
				DebugTimeStart
#endif
				mBitmap.DeleteObject ();
#ifdef	_DEBUG_BITMAP_BUFFERS
				DebugTimeStop
				mTotalDeleteTime += DebugTimeElapsed;
				mDeleted++;
				mTotalDeleted++;
#endif
			}
			if	(mScaledBitmap.GetSafeHandle ())
			{
#ifdef	_DEBUG_BITMAP_BUFFERS
				DebugTimeStart
#endif
				mScaledBitmap.DeleteObject ();
#ifdef	_DEBUG_BITMAP_BUFFERS
				DebugTimeStop
				mTotalDeleteTime += DebugTimeElapsed;
				mDeleted++;
				mTotalDeleted++;
#endif
			}
			mBitmapBits = NULL;
			mScaledBits = NULL;

			if	(mDC.CreateCompatibleDC (0))
			{
				mBitmap.Attach (lScaledBuffer.Detach ());
				mBitmapBits = lScaledBits;
				mScaledBitmap.Attach (lUnscaledBuffer.Detach ());
				mScaledBits = lUnscaledBits;

				mDC.SetMapMode (MM_TEXT);
				mDC.SetWindowOrg (0, 0);
				mOldBitmap = mDC.SelectObject (mBitmap);
				mDC.SetMapMode (MM_ANISOTROPIC);
				mDC.ScaleViewportExt (mScale, 1, mScale, 1);

#ifdef	_DEBUG_BITMAP_BUFFERS
				mUsed++;
				mNowInUse++;
				mTotalInUse++;
				mMaxInUse = max (mMaxInUse, mNowInUse);
#endif
				lRet = true;
			}
		}
	}

	return lRet;
}

bool CBitmapBufferScaled::DrawBuffer (HDC pTargetDC, const CRect& pRect, bool pReleaseBuffer, bool pReleaseDC)
{
	bool	lRet = false;

	if	(
			(mBitmap.GetSafeHandle ())
		&&	(mBitmapBits)
		)
	{
		tS <BITMAP>	lBitmapDef;
		tS <BITMAP>	lScaledDef;

		if	(
				(mBitmap.GetSafeHandle ())
			&&	(mBitmap.GetBitmap (&lBitmapDef))
			&&	(mScaledBitmap.GetSafeHandle ())
			&&	(mScaledBitmap.GetBitmap (&lScaledDef))
			&&	(lScaledDef.bmWidth < lBitmapDef.bmWidth)
			)
		{
			lRet = UnscaleBuffer ();
		}

		if	(pTargetDC)
		{
			if	(
					(!pRect.IsRectEmpty ())
				&&	(mDC.GetSafeHdc ())
				)
			{
				try
				{
					if	(
							(lBitmapDef.bmBitsPixel < 32)
						||	(!CBitmapAlpha::AlphaBlend (pTargetDC, pRect, mDC, pRect))
						)
					{
						BitBlt (pTargetDC, pRect.left, pRect.top, pRect.Width (), pRect.Height (), mDC, pRect.left, pRect.top, SRCCOPY);
					}
				}
				catch AnyExceptionSilent
			}

			EndBuffer (pReleaseBuffer, pReleaseDC);

			if	(pReleaseBuffer)
			{
				if	(mScaledBitmap.GetSafeHandle ())
				{
#ifdef	_DEBUG_BITMAP_BUFFERS
					DebugTimeStart
#endif
					mScaledBitmap.DeleteObject ();
#ifdef	_DEBUG_BITMAP_BUFFERS
					DebugTimeStop
					mTotalDeleteTime += DebugTimeElapsed;
					mDeleted++;
					mTotalDeleted++;
#endif
				}
				mScaledBits = NULL;
			}
		}
	}

	return lRet;
}

void CBitmapBufferScaled::EraseBuffer (COLORREF pColor)
{
	tS <BITMAP>	lBitmapDef;

	if	(
			(mDC.GetSafeHdc ())
		&&	(mBitmap.GetBitmap (&lBitmapDef))
		)
	{
		COLORREF lBkColor = mDC.GetBkColor ();
		mDC.FillSolidRect (CRect (mDC.GetWindowOrg (), CSize (lBitmapDef.bmWidth, lBitmapDef.bmHeight)), pColor);
		mDC.SetBkColor (lBkColor);

		if	(lBitmapDef.bmBitsPixel >= 32)
		{
			PauseBuffer ();
			CBitmapAlpha::AlphaFillBitmap (mBitmap, mBitmapBits);
			ResumeBuffer ();
		}
	}
}

bool CBitmapBufferScaled::UnscaleBuffer ()
{
	bool	lRet = false;

#ifdef	_DEBUG_BITMAP_BUFFERS
	DebugTimeStart
#endif
	if	(
			(mBitmap.GetSafeHandle ())
		&&	(mBitmapBits)
		&&	(mScaledBitmap.GetSafeHandle ())
		&&	(mScaledBits)
		)
	{
		CPoint		lWindowOrg = mDC.GetWindowOrg ();
		bool		lPaused = PauseBuffer ();
		CBitmap		lBitmap;
		LPBYTE		lBits;
		tS <BITMAP>	lBitmapDef;

		lBitmap.Attach (mBitmap.Detach ());
		mBitmap.Attach (mScaledBitmap.Detach ());
		mScaledBitmap.Attach (lBitmap.Detach ());
		lBits = mBitmapBits;
		mBitmapBits = mScaledBits;
		mScaledBits = lBits;

//		LogMessage (LogNormal|LogTime, _T("EndScaled [%p] [%p] Unscaled [%p] [%p]"), mScaledBitmap.GetSafeHandle (), mScaledBits, mBitmap.GetSafeHandle (), mBitmapBits);

		if	(
				(mBitmap.GetBitmap (&lBitmapDef))
			&&	(lBitmapDef.bmBitsPixel >= 32)
			)
		{
			CBitmapAlpha::AlphaInvertBitmap (mScaledBitmap, mScaledBits);
		}

		try
		{
 			lRet = CBitmapTools::ShrinkBitmap (mScaledBitmap, mScaledBits, mBitmap, mBitmapBits, mBlend);
		}
		catch AnyExceptionSilent

#ifdef	_DEBUG_SCALING
		{
			CBitmapDebugger	lDebugger;
			lDebugger.ShowBitmap (mScaledBitmap, 1, _T("Unshrunk"));
			lDebugger.DoModal ();
			lDebugger.ShowBitmap (mBitmap, 1, _T("Shrunk"));
			lDebugger.DoModal ();
		}
#endif

		if	(
				(lPaused)
			&&	(ResumeBuffer ())
			)
		{
			mDC.SetMapMode (MM_TEXT);
			mDC.SetWindowOrg (lWindowOrg);
		}
	}

#ifdef	_DEBUG_BITMAP_BUFFERS
	DebugTimeLog
#endif
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSize CBitmapBuffer::GetBitmapSize () const
{
	tS <BITMAP>	lBitmapInfo;

	if	(
			(mBitmap.GetSafeHandle ())
		&&	(const_cast <CBitmapBuffer *> (this)->mBitmap.GetBitmap (&lBitmapInfo))
		)
	{
		return CSize (lBitmapInfo.bmWidth, lBitmapInfo.bmHeight);
	}
	return CSize (0, 0);
}

bool CBitmapBuffer::SaveBitmap (HBITMAP pBitmap, LPCTSTR pFileName)
{
	bool		lRet = false;
    tS <BITMAP>	lBitmap;
	CString 	lFileName (pFileName);

	lFileName.TrimLeft ();
	lFileName.TrimRight ();

	try
	{
		if	(
				(!lFileName.IsEmpty ())
			&&	(pBitmap)
			&&	(GetObject (pBitmap, sizeof (lBitmap), &lBitmap))
			&&	(lBitmap.bmBits)
			)
		{
			CFileHandle				lFile;
			tS <BITMAPFILEHEADER>	lBmpFileHdr;
			BITMAPINFO *			lBmpInfo;
			tArrayPtr <BYTE>		lBmpInfoBuffer;
			tArrayPtr <BYTE>		lBits;
			long					lInfoSize = sizeof (BITMAPINFOHEADER);
			ULONG					lBitsSize = lBitmap.bmHeight * lBitmap.bmWidthBytes;
			ULONG					lColorCount = (ULONG) 1L << lBitmap.bmBitsPixel;
			CDC						lDC;

			if	(lBitmap.bmBitsPixel < 24)
			{
				lInfoSize += lColorCount * sizeof (RGBQUAD);
			}

			if	(
					(lBmpInfoBuffer = new BYTE [lInfoSize])
				&&	(lBmpInfo = (BITMAPINFO *) (LPBYTE) lBmpInfoBuffer)
				&&	(lDC.CreateIC (_T("DISPLAY"), NULL, NULL, NULL))
				)
			{
				try
				{
					memset (lBmpInfo, 0, lInfoSize);
					lBmpInfo->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
					lBmpInfo->bmiHeader.biWidth = (long) lBitmap.bmWidth;
					lBmpInfo->bmiHeader.biHeight = (long) lBitmap.bmHeight;
					lBmpInfo->bmiHeader.biPlanes = 1;
					lBmpInfo->bmiHeader.biBitCount = lBitmap.bmBitsPixel;
					lBmpInfo->bmiHeader.biCompression = BI_RGB;
					lBmpInfo->bmiHeader.biSizeImage = lBitsSize;

					if	(
							(GetDIBits (lDC, pBitmap, 0, lBitmap.bmHeight, NULL, lBmpInfo, DIB_RGB_COLORS))
						&&	(lBitsSize = lBmpInfo->bmiHeader.biSizeImage)
						&&	(lBits = new BYTE [lBitsSize])
						&&	(GetDIBits (lDC, pBitmap, 0, lBitmap.bmHeight, (LPBYTE) lBits, lBmpInfo, DIB_RGB_COLORS))
						)
					{
						lFile = CreateFile (lFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

						if	(!lFile.SafeIsValid ())
						{
							LogWinErr (LogNormal|LogTime, GetLastError (), _T("CreateFile"));
						}
						else
						{
							try
							{
								lBmpFileHdr.bfType = 0x4D42;
								lBmpFileHdr.bfSize = sizeof (lBmpFileHdr) + lInfoSize + lBitsSize;
								lBmpFileHdr.bfOffBits = sizeof (lBmpFileHdr) + lInfoSize;

								DWORD	lWrite;
								DWORD	lWritten;

								if	(
										(WriteFile (lFile, &lBmpFileHdr, lWrite = sizeof (lBmpFileHdr), &(lWritten = 0), NULL))
									&&	(lWritten == lWrite)
									&&	(WriteFile (lFile, lBmpInfo, lWrite = lInfoSize, &(lWritten = 0), NULL))
									&&	(lWritten == lWrite)
									&&	(WriteFile (lFile, (LPBYTE) lBits, lWrite = lBitsSize, &(lWritten = 0), NULL))
									&&	(lWritten == lWrite)
									)
								{
									lRet = true;
								}
								else
								{
									LogWinErr (LogNormal|LogTime, GetLastError (), _T("WriteFile"));
								}
							}
							catch AnyExceptionSilent

							lFile.Close ();
						}
					}
					else
					{
						LogWinErr (LogNormal|LogTime, GetLastError (), _T("GetDIBits"));
					}
				}
				catch AnyExceptionSilent

				lDC.DeleteDC ();
			}
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG_BITMAP_BUFFERS
//////////////////////////////////////////////////////////////////////

UINT CBitmapBuffer::IsInUse () const
{
	return (mOldBitmap) ? 1 : 0;
}

UINT CBitmapBuffer::IsDcInUse () const
{
	return (mDC.GetSafeHdc ()) ? 1 : 0;
}

UINT CBitmapBuffer::IsBitmapUnUse (DWORD& pBitmapBytes) const
{
	if	(mBitmap.GetSafeHandle ())
	{
		tS <BITMAP>	lBitmap;

		const_cast <CBitmapBuffer *> (this)->mBitmap.GetBitmap (&lBitmap);
		pBitmapBytes = lBitmap.bmWidthBytes * lBitmap.bmHeight;

		return 1;
	}
	return 0;
}

void CBitmapBuffer::LogBitmapUnUse (UINT pLogLevel) const
{
	CString		lName = mDebugName.IsEmpty() ? _T("Unnamed") : mDebugName;
	tS <BITMAP>	lBitmap;

	if	(mBitmap.GetSafeHandle ())
	{
		const_cast <CBitmapBuffer *> (this)->mBitmap.GetBitmap (&lBitmap);
		LogMessage (pLogLevel, _T("    %-20.20s Used [%3I64u] [%f] Created [%3I64u] Deleted [%3I64u] Bitmap [%8.8lX] Size [%u x %u x %u] [%s]"), lName, mUsed, mInUseTime, mCreated, mDeleted, mBitmap.GetSafeHandle (), lBitmap.bmWidth, lBitmap.bmHeight, lBitmap.bmBitsPixel, LclFormatSize ((double)lBitmap.bmWidthBytes*(double)lBitmap.bmHeight));
	}
	else
	{
		LogMessage (pLogLevel, _T("    %-20.20s Used [%3I64u] [%f] Created [%3I64u] Deleted [%3I64u]"), lName, mUsed, mInUseTime, mCreated, mDeleted);
	}
}

//////////////////////////////////////////////////////////////////////

UINT CBitmapBufferScaled::IsBitmapUnUse (DWORD& pBitmapBytes) const
{
	return CBitmapBuffer::IsBitmapUnUse (pBitmapBytes);
}

void CBitmapBufferScaled::LogBitmapUnUse (UINT pLogLevel) const
{
	CBitmapBuffer::LogBitmapUnUse (pLogLevel);

	if	(mScaledBitmap.GetSafeHandle ())
	{
		tS <BITMAP>	lBitmap;

		const_cast <CBitmapBufferScaled *> (this)->mScaledBitmap.GetBitmap (&lBitmap);
		LogMessage (pLogLevel, _T("                         Scaled [%8.8lX] Size [%u x %u x %u] [%s]"), mScaledBitmap.GetSafeHandle (), lBitmap.bmWidth, lBitmap.bmHeight, lBitmap.bmBitsPixel, LclFormatSize ((double)lBitmap.bmWidthBytes*(double)lBitmap.bmHeight));
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CCriticalSection							CBitmapBuffer::mUsageCriticalSection;
CTypedPtrArray <CPtrArray, CBitmapBuffer *>	CBitmapBuffer::mInstances;
UINT										CBitmapBuffer::mMaxInstances = 0;
UINT										CBitmapBuffer::mTotalInstances = 0;
UINT										CBitmapBuffer::mNowInUse = 0;
UINT										CBitmapBuffer::mTotalInUse = 0;
UINT										CBitmapBuffer::mMaxInUse = 0;
ULONGLONG									CBitmapBuffer::mTotalCreated = 0;
ULONGLONG									CBitmapBuffer::mTotalDeleted = 0;
double										CBitmapBuffer::mTotalCreateTime = 0;
double										CBitmapBuffer::mTotalDeleteTime = 0;
double										CBitmapBuffer::mTotalInUseTime = 0;

//////////////////////////////////////////////////////////////////////

void CBitmapBuffer::DumpUsage (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		CBitmapBuffer *	lInstance;
		int				lNdx;
		UINT			lInUseCount = 0;
		UINT			lDcCount = 0;
		UINT			lBmpCount;
		DWORD			lBmpBytes;
		UINT			lTotalBmpCount = 0;
		ULONGLONG		lTotalBytes = 0;
		CSingleLock		lLock (&mUsageCriticalSection, TRUE);

		try
		{
			if	(!pTitle)
			{
				pTitle = _T("Usage");
			}
			LogMessage (pLogLevel, _T("CBitmapBuffer %s"), pTitle);

			for	(lNdx = 0; lNdx <= mInstances.GetUpperBound(); lNdx++)
			{
				lInstance = mInstances [lNdx];

				lInUseCount += lInstance->IsInUse ();
				lDcCount += lInstance->IsDcInUse ();

				if	(lBmpCount = lInstance->IsBitmapUnUse (lBmpBytes))
				{
					lTotalBmpCount += lBmpCount;
					lTotalBytes += lBmpBytes;
				}
			}

			LogMessage (pLogLevel, _T("  Instances [%d of Max %u Total %u]"), mInstances.GetSize (), mMaxInstances, mTotalInstances);
			LogMessage (pLogLevel, _T("  InUse     [%u (%u) of Max %u Total %u] Time [%f]"), lInUseCount, mNowInUse, mMaxInUse, mTotalInUse, mTotalInUseTime);
			LogMessage (pLogLevel, _T("    Created [%I64u] [%f] Deleted [%I64u] [%f]"), mTotalCreated, mTotalCreateTime, mTotalDeleted, mTotalDeleteTime);
			LogMessage (pLogLevel, _T("    DCs     [%u] Bitmaps [%u] Bytes [%s]"), lDcCount, lTotalBmpCount, LclFormatSize ((double)(LONGLONG)lTotalBytes));
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////

void CBitmapBuffer::DumpInstances (UINT pLogLevel, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		CBitmapBuffer *	lInstance;
		int				lNdx;
		UINT			lBmpCount;
		DWORD			lBmpBytes;
		UINT			lTotalBmpCount = 0;
		ULONGLONG		lTotalBytes = 0;
		CSingleLock		lLock (&mUsageCriticalSection, TRUE);

		try
		{
			if	(!pTitle)
			{
				pTitle = _T("Instances");
			}
			LogMessage (pLogLevel, _T("CBitmapBuffer %s"), pTitle);

			for	(lNdx = 0; lNdx <= mInstances.GetUpperBound(); lNdx++)
			{
				lInstance = mInstances [lNdx];

				if	(lBmpCount = lInstance->IsBitmapUnUse (lBmpBytes))
				{
					lTotalBmpCount += lBmpCount;
					lTotalBytes += lBmpBytes;
				}
				lInstance->LogBitmapUnUse (pLogLevel|LogHighVolume);
			}

			LogMessage (pLogLevel, _T("  Total Bitmaps [%u] Size [%s]"), lTotalBmpCount, LclFormatSize ((double)(LONGLONG)lTotalBytes));
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#endif // _DEBUG_BITMAP_BUFFERS
//////////////////////////////////////////////////////////////////////
