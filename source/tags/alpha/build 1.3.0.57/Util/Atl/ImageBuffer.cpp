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
#include "ImageBuffer.h"
#include "ImageTools.h"
#include "ImageAlpha.h"
#include "Log.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_SCALING	LogDebugFast
#endif

//////////////////////////////////////////////////////////////////////

CImageBuffer::CImageBuffer ()
:	mOldFont (NULL)
{
}

CImageBuffer::~CImageBuffer ()
{
	EndBuffer (true);
}

//////////////////////////////////////////////////////////////////////

HDC CImageBuffer::GetDC () const
{
	if	(
			(this)
		&&	(mDC)
		)
	{
		return (HDC)(*mDC);
	}
	return NULL;
}

HBITMAP CImageBuffer::GetImage () const
{
	if	(
			(this)
		&&	(mImage)
		)
	{
		return (HBITMAP)(*mImage);
	}
	return NULL;
}

CSize CImageBuffer::GetImageSize () const
{
	if	(
			(this)
		&&	(mImage)
		)
	{
		return CSize (mImage->GetWidth(), mImage->GetHeight());
	}
	return CSize (0, 0);
}

//////////////////////////////////////////////////////////////////////

bool CImageBuffer::StartBuffer ()
{
	if	(mImage)
	{
		if	(
				(mDC)
			||	(mDC = new ATL::CImageDC (*mImage))
			)
		{
			SetBkColor (*mDC, RGB (0xFF, 0xFF, 0xFF));
			SetTextColor (*mDC, RGB (0x00, 0x00, 0x00));
			return true;
		}
	}
	return false;
}

void CImageBuffer::EndBuffer (bool pReleaseBuffer)
{
	if	(mDC)
	{
		if	(mOldFont)
		{
			SelectObject (*mDC, mOldFont);
			mOldFont = NULL;
		}
		mDC = NULL;
	}
	else
	{
		mOldFont = NULL;
	}

	if	(pReleaseBuffer)
	{
		mImage = NULL;
	}
}

bool CImageBuffer::PauseBuffer ()
{
	if	(mDC)
	{
		mDC = NULL;
		GdiFlush ();
		return true;
	}
	return false;
}

bool CImageBuffer::ResumeBuffer ()
{
	if	(
			(mImage)
		&&	(
				(mDC)
			||	(mDC = new ATL::CImageDC (*mImage))
			)
		)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CImageBuffer::CreateScaledBuffer (const CSize& pSize, bool pForBlending, UINT pScale)
{
	CSize	lSize (pSize);

	lSize.cx *= pScale;
	lSize.cy *= pScale;

	EndBuffer (true);

	if	(
			(mImage = new ATL::CImage ())
		&&	(CImageTools::CreateColorImage (lSize, *mImage, pForBlending))
		&&	(StartBuffer ())
		)
	{
		if	(pScale > 1)
		{
			SetMapMode (*mDC, MM_ANISOTROPIC);
			ScaleViewportExtEx (*mDC, pScale, 1, pScale, 1, NULL);
		}
		return true;
	}

	EndBuffer (true);
	return false;
}

bool CImageBuffer::UnscaleBuffer (UINT pScale, UINT pBlend)
{
	bool	lRet = false;

	if	(pScale > 1)
	{
		if	(
				(mImage)
			&&	(mImage->IsDIBSection ())
			)
		{
			bool	lPaused = PauseBuffer ();

#ifdef	_DEBUG_SCALING
			{
				CImageDebugger	lDebugger;
				lDebugger.ShowBitmap (*mImage, 1, _T("Unshrunk"));
				lDebugger.DoModal ();
			}
#endif

			lRet = CImageTools::ShrinkImage (*mImage, pScale, pBlend);

#ifdef	_DEBUG_SCALING
			{
				CImageDebugger	lDebugger;
				lDebugger.ShowBitmap (*mImage, 1, _T("shrunk"));
				lDebugger.DoModal ();
			}
#endif
			if	(
					(lPaused)
				&&	(ResumeBuffer ())
				)
			{
				SetMapMode (*mDC, MM_TEXT);
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

bool CImageBuffer::NewBufferRequired (ATL::CImage* pImage, const CSize& pSize, bool pForBlending, int pSizeFactor)
{
	if	(
			(pImage)
		&&	(pImage->GetWidth() >= pSize.cx)
		&&	(pImage->GetHeight() >= pSize.cy)
		&&	(
				(pImage->GetWidth() <= (pSize.cx * pSizeFactor))
			||	(pImage->GetHeight() <= (pSize.cy * pSizeFactor))
			)
		&&	(
				(pForBlending)
			?	(pImage->GetBPP() == 32)
			:	(pImage->GetBPP() == 24)
			)
		)
	{
		return false;
	}
	return true;
}

bool CImageBuffer::CreateBuffer (const CSize& pBufferSize, bool pForBlending, bool pForceNewBuffer, bool pTopDown)
{
	bool	lRet = false;

	if	(
			(pBufferSize.cx > 0)
		&&	(pBufferSize.cy > 0)
		)
	{
		if	(
				(!pForceNewBuffer)
			&&	(!NewBufferRequired (mImage, pBufferSize, pForBlending))
			&&	(StartBuffer ())
			)
		{
			lRet = true;
		}
		else
		{
			tPtr <ATL::CImage>	lOldImage;

			mDC = NULL;
			lOldImage.Attach (mImage.Detach ());

			if	(
					(mImage = new ATL::CImage ())
				&&	(CImageTools::CreateColorImage (pBufferSize, *mImage, pForBlending, pTopDown))
				&&	(StartBuffer ())
				)
			{
				lRet = true;
			}
			else
			{
				mImage.Attach (lOldImage.Detach ());
			}
		}
	}

	return lRet;
}

bool CImageBuffer::DrawBuffer (HDC pTargetDC, const CRect& pRect, bool pReleaseBuffer, bool pReleaseDC)
{
	bool	lRet = false;

	try
	{
		if	(
				(!pRect.IsRectEmpty ())
			&&	(mImage)
			&&	(mDC)
			&&	(pTargetDC)
			)
		{
			mImage->BitBlt (pTargetDC, pRect.left, pRect.top, pRect.Width (), pRect.Height (), pRect.left, pRect.top, SRCCOPY);
			lRet = true;
		}
	}
	catch AnyExceptionSilent

	if	(
			(pReleaseBuffer)
		||	(pReleaseDC)
		)
	{
		EndBuffer (pReleaseBuffer);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CImageBufferScaled::CImageBufferScaled (UINT pScale, UINT pBlend)
:	mScale (pScale),
	mBlend (pBlend)
{
}

CImageBufferScaled::~CImageBufferScaled ()
{
}

//////////////////////////////////////////////////////////////////////

bool CImageBufferScaled::CreateBuffer (const CSize& pBufferSize, bool pForBlending, bool pForceNewBuffer, bool pTopDown)
{
	bool				lRet = false;
	CSize				lBufferSize (((pBufferSize.cx + 3) / 4) * 4, ((pBufferSize.cy + 3) / 4) * 4);
	CSize				lScaledSize (lBufferSize.cx * (long) mScale, lBufferSize.cy * (long) mScale);
	tPtr <ATL::CImage>	lUnscaledImage;
	tPtr <ATL::CImage>	lScaledImage;

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
					&&	(!NewBufferRequired (mImage, lBufferSize, pForBlending, 10))
					&&	(lUnscaledImage.Attach (mImage.Detach ()))
					)
				||	(
						(lUnscaledImage = new ATL::CImage ())
					&&	(CImageTools::CreateColorImage (lBufferSize, *lUnscaledImage, pForBlending, pTopDown))
					)
				)
			&&	(
					(
						(!pForceNewBuffer)
					&&	(!NewBufferRequired (mScaledImage, lScaledSize, pForBlending, 10))
					&&	(lScaledImage.Attach (mScaledImage.Detach ()))
					)
				||	(
						(lScaledImage = new ATL::CImage ())
					&&	(CImageTools::CreateColorImage (lScaledSize, *lScaledImage, pForBlending, pTopDown))
					)
				)
			)
		{
#ifdef	_DEBUG_SCALING
			LogMessage (LogNormal|LogTime, _T("StartScaled [%d %d] [%p] [%p] Unscaled [%d %d] [%p] [%p]"), lScaledSize.cx, lScaledSize.cy, (HBITMAP)(*lScaledImage), GetImageBits(*lScaledImage), lBufferSize.cx, lBufferSize.cy, (HBITMAP)(*lUnscaledImage), GetImageBits(*lUnscaledImage));
#endif
			EndBuffer ();
			mImage = lUnscaledImage;
			mScaledImage = lScaledImage;

			if	(mDC = new CImageDC (*mImage))
			{
				SetMapMode (*mDC, MM_TEXT);
				SetWindowOrgEx (*mDC, 0, 0, NULL);
				SetMapMode (*mDC, MM_ANISOTROPIC);
				ScaleViewportExtEx (*mDC, mScale, 1, mScale, 1, NULL);
				lRet = true;
			}
		}
	}
	return lRet;
}

bool CImageBufferScaled::DrawBuffer (HDC pTargetDC, const CRect& pRect, bool pReleaseBuffer, bool pReleaseDC)
{
	bool	lRet = false;

	if	(
			(mImage)
		&&	(mImage->IsDIBSection ())
		)
	{
		if	(
				(mScaledImage)
			&&	(mScaledImage->IsDIBSection ())
			&&	(mScaledImage->GetWidth() < mImage->GetWidth ())
			)
		{
			lRet = UnscaleBuffer ();
		}

		if	(pTargetDC)
		{
			if	(
					(!pRect.IsRectEmpty ())
				&&	(mDC)
				)
			{
				try
				{
					if	(
							(mImage->GetBPP () < 32)
						||	(!mImage->AlphaBlend (pTargetDC, pRect, pRect))
						)
					{
						mImage->BitBlt (pTargetDC, pRect.left, pRect.top, pRect.Width (), pRect.Height (), pRect.left, pRect.top, SRCCOPY);
					}
				}
				catch AnyExceptionSilent
			}

			if	(
					(pReleaseBuffer)
				||	(pReleaseDC)
				)
			{
				EndBuffer (pReleaseBuffer);
			}
			if	(pReleaseBuffer)
			{
				mScaledImage = NULL;
			}
		}
	}

	return lRet;
}

void CImageBufferScaled::EraseBuffer (COLORREF pColor)
{
	if	(
			(mImage)
		&&	(mDC)
		)
	{
		CPoint			lWindowOrg;
		CSize			lImageSize (mImage->GetWidth(), mImage->GetHeight());
		COLORREF		lBkColor;
		CBrushHandle	lBrush;

		GetWindowOrgEx (*mDC, &lWindowOrg);
		lBkColor = GetBkColor (*mDC);

		if	(mImage->GetBPP() >= 32)
		{
			CRgnHandle	lRgn;
			if	(
					(lRgn.Attach (CreateRectRgn (lWindowOrg.x, lWindowOrg.y, lWindowOrg.x+lImageSize.cx, lWindowOrg.y+lImageSize.cy)))
				&&	(lBrush.Attach (CImageAlpha::GetAlphaBrush (lBkColor, 255)))
				)
			{
				FillRgn (*mDC, lRgn, lBrush);
			}
		}
		else
		{
			if	(lBrush.Attach (CreateSolidBrush (lBkColor)))
			{
				FillRect (*mDC, CRect (lWindowOrg, lImageSize), lBrush);
			}
		}
	}
}

bool CImageBufferScaled::UnscaleBuffer ()
{
	bool	lRet = false;

	if	(
			(mImage)
		&&	(mScaledImage)
		)
	{
		bool				lPaused;
		CPoint				lWindowOrg;
		tPtr <ATL::CImage>	lImage;

		GetWindowOrgEx (*mDC, &lWindowOrg);
		lPaused = PauseBuffer ();

		lImage.Attach (mImage.Detach ());
		mImage.Attach (mScaledImage.Detach ());
		mScaledImage.Attach (lImage.Detach ());

		if	(mImage->GetBPP () >= 32)
		{
			CImageAlpha::AlphaInvertImage (*mScaledImage);
		}

		try
		{
 			lRet = CImageTools::ShrinkImage (*mScaledImage, *mImage, mBlend);
		}
		catch AnyExceptionSilent

		if	(
				(lPaused)
			&&	(ResumeBuffer ())
			)
		{
			SetMapMode (*mDC, MM_TEXT);
			SetWindowOrgEx (*mDC, lWindowOrg.x, lWindowOrg.y, NULL);
		}
	}
	return lRet;
}
