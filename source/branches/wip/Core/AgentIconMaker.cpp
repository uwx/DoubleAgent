/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#include "DaCore.h"
#include "AgentIconMaker.h"
#include "AgentFile.h"
#include "BitmapBuffer.h"
#include "BitmapTools.h"
#include "BitmapAlpha.h"
#ifdef	_DEBUG
#include "BitmapDebugger.h"
#include "DebugStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_ICONS	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentIconMaker::CAgentIconMaker ()
{
}

CAgentIconMaker::~CAgentIconMaker ()
{
}

/////////////////////////////////////////////////////////////////////////////

HICON CAgentIconMaker::MakeIcon (CAgentFile * pAgentFile, const CSize & pIconSize, const CRect * pClipRect)
{
	HICON			lIcon = NULL;
	CBitmapBuffer	lImageBuffer;
	CRect			lImageRect;
	CBitmapBuffer	lRenderBuffer;
	CPoint			lRenderPos;
	CSize			lRenderSize;
	tS <BITMAP>		lBitmapInfo;

	if	(lImageBuffer.mBitmap.Attach (GetFrameImage (pAgentFile)))
	{
		lImageRect = CRect (CPoint (0,0), lImageBuffer.GetBitmapSize ());

		if	(
				(pClipRect)
			&&	(!pClipRect->IsRectEmpty ())
			)
		{
			lImageRect.IntersectRect (&lImageRect, pClipRect);
		}
		lRenderSize.cx = lRenderSize.cy = max (lImageRect.Width(), lImageRect.Height());
		lRenderPos.x = max (MulDiv (lImageRect.Height()-lImageRect.Width(),1,2), 0);
		lRenderPos.y = max (MulDiv (lImageRect.Width()-lImageRect.Height(),1,2), 0);

#ifdef	_DEBUG_ICONS
		if	(LogIsActive (_DEBUG_ICONS))
		{
			LogMessage (_DEBUG_ICONS, _T("Render [%s] in [%s] for [%s]"), FormatRect(lImageRect), FormatRect(CRect(lRenderPos,lRenderSize)), FormatSize(pIconSize));
			CBitmapDebugger::SaveBitmap (lImageBuffer.mBitmap, CString(pAgentFile->FindName()->mName)+_T("- Generated"));
		}
#endif

		if	(
				(lImageBuffer.StartBuffer ())
			&&	(lRenderBuffer.CreateBuffer (lRenderSize, true, true, true))
			)
		{
			lRenderBuffer.mDC.BitBlt (lRenderPos.x, lRenderPos.y, lImageRect.Width(), lImageRect.Height(), &lImageBuffer.mDC, lImageRect.left, lImageRect.top, SRCCOPY);
			lImageBuffer.EndBuffer ();
			lRenderBuffer.EndBuffer ();

			if	(RemoveMargin (lRenderBuffer))
			{
#ifdef	_DEBUG_ICONS
				if	(LogIsActive (_DEBUG_ICONS))
				{
					LogMessage (_DEBUG_ICONS, _T("Reduced to [%s]"), FormatSize(lRenderBuffer.GetBitmapSize()));
				}
#endif
			}
			lRenderBuffer.mBitmap.GetBitmap (&lBitmapInfo);

			if	(
					(lImageRect.Size() != pIconSize)
				&&	(lImageBuffer.CreateBuffer (pIconSize, true, true))
				)
			{
				Gdiplus::Bitmap		lBitmap (lBitmapInfo.bmWidth, lBitmapInfo.bmHeight, lBitmapInfo.bmWidthBytes, PixelFormat32bppPARGB, lRenderBuffer.mBitmapBits);
				Gdiplus::Graphics	lGraphics (lImageBuffer.mDC);

				lGraphics.Clear (Gdiplus::Color (0,0,0,0));
				lGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
				lGraphics.SetCompositingQuality (Gdiplus::CompositingQualityDefault);
				lGraphics.SetInterpolationMode (Gdiplus::InterpolationModeHighQualityBilinear);
				lGraphics.DrawImage (&lBitmap, 0, 0, pIconSize.cx, pIconSize.cy);
			}
			else
			{
				lImageBuffer.mBitmap.Attach (lRenderBuffer.mBitmap.Detach ());
				lImageBuffer.mBitmapBits = lRenderBuffer.mBitmapBits;
			}

			lImageBuffer.EndBuffer ();
			lRenderBuffer.EndBuffer (true);

			if	(
					(lRenderBuffer.CreateBuffer (pIconSize, true, true))
				&&	(lImageBuffer.StartBuffer ())
				)
			{
				lRenderBuffer.mDC.BitBlt (0, 0, pIconSize.cx, pIconSize.cy, &lImageBuffer.mDC, 0, 0, SRCCOPY);
				lRenderBuffer.EndBuffer ();
				CBitmapAlpha::AlphaSaturateBitmap (lRenderBuffer.mBitmap, lRenderBuffer.mBitmapBits);
				GdiFlush ();
			}

			lImageBuffer.EndBuffer ();
			lRenderBuffer.EndBuffer ();

#ifdef	_DEBUG_ICONS
			if	(LogIsActive (_DEBUG_ICONS))
			{
				CBitmapDebugger::SaveBitmap (lImageBuffer.mBitmap, CString(pAgentFile->FindName()->mName)+_T("- Resized"));
				CBitmapDebugger::SaveBitmap (lRenderBuffer.mBitmap, CString(pAgentFile->FindName()->mName)+_T("- Masked"));
			}
#endif
			if	(
					(lImageBuffer.mBitmap.GetSafeHandle ())
				&&	(lRenderBuffer.mBitmap.GetSafeHandle ())
				)
			{
				tS <ICONINFO>	lIconInfo;

				lIconInfo.fIcon = TRUE;
				lIconInfo.hbmColor = (HBITMAP) lImageBuffer.mBitmap;
				lIconInfo.hbmMask = (HBITMAP) lRenderBuffer.mBitmap;

				lIcon = CreateIconIndirect (&lIconInfo);

#ifdef	_DEBUG_ICONS
				if	(LogIsActive (_DEBUG_ICONS))
				{
					CBitmapDebugger::SaveIcon (lIcon, CString(pAgentFile->FindName()->mName)+_T("- Generated"));
				}
#endif
			}
		}
	}

	return lIcon;
}

bool CAgentIconMaker::RemoveMargin (CBitmapBuffer & pBuffer)
{
	bool			lRet = false;
	tS <BITMAP>		lBitmapInfo;

	if	(
			(pBuffer.mBitmapBits)
		&&	(pBuffer.mBitmap.GetBitmap (&lBitmapInfo))
		)
	{
		CPoint	lPixel;
		long	lMarginMax = MulDiv (min (lBitmapInfo.bmWidth, lBitmapInfo.bmHeight), 1, 4);
		long	lMargin;
		long	lNdx;

		for	(lMargin = 0; lMargin < lMarginMax; lMargin++)
		{
			bool	lPixelFound = false;

			lPixel.y = lMargin;
			for	(lPixel.x = lMargin, lNdx = lPixel.y*lBitmapInfo.bmWidthBytes; lPixel.x < lBitmapInfo.bmWidth-lMargin; lPixel.x++, lNdx += 4)
			{
				if	(*(DWORD *)(pBuffer.mBitmapBits + lNdx) != 0)
				{
					lPixelFound = true;
					break;
				}
			}
			if	(!lPixelFound)
			{
				lPixel.y = lBitmapInfo.bmHeight-lMargin-1;
				for	(lPixel.x = lMargin, lNdx = lPixel.y*lBitmapInfo.bmWidthBytes; lPixel.x < lBitmapInfo.bmWidth-lMargin; lPixel.x++, lNdx += 4)
				{
					if	(*(DWORD *)(pBuffer.mBitmapBits + lNdx) != 0)
					{
						lPixelFound = true;
						break;
					}
				}
			}
			if	(!lPixelFound)
			{
				lPixel.x = lMargin;
				for	(lPixel.y = lMargin; lPixel.y < lBitmapInfo.bmHeight-lMargin; lPixel.y++)
				{
					lNdx = (lPixel.y * lBitmapInfo.bmWidthBytes) + (lPixel.x * 4);
					if	(*(DWORD *)(pBuffer.mBitmapBits + lNdx) != 0)
					{
						lPixelFound = true;
						break;
					}
				}
			}
			if	(!lPixelFound)
			{
				lPixel.x = lBitmapInfo.bmWidth-lMargin-1;
				for	(lPixel.y = lMargin; lPixel.y < lBitmapInfo.bmHeight-lMargin; lPixel.y++)
				{
					lNdx = (lPixel.y * lBitmapInfo.bmWidthBytes) + (lPixel.x * 4);
					if	(*(DWORD *)(pBuffer.mBitmapBits + lNdx) != 0)
					{
						lPixelFound = true;
						break;
					}
				}
			}

			if	(lPixelFound)
			{
				break;
			}
		}

		if	(lMargin > 0)
		{
			CRect			lTempRect (CPoint (0,0), pBuffer.GetBitmapSize ());
			CBitmapBuffer	lTempBuffer;

			lTempRect.DeflateRect (lMargin, lMargin);

			if	(
					(lTempBuffer.CreateBuffer (lTempRect.Size(), true, true, true))
				&&	(pBuffer.StartBuffer ())
				)
			{
				lTempBuffer.mDC.BitBlt (0, 0, lTempRect.Width(), lTempRect.Height(), &pBuffer.mDC, lTempRect.left, lTempRect.top, SRCCOPY);
				lTempBuffer.EndBuffer ();
				pBuffer.EndBuffer ();

				pBuffer.mBitmap.DeleteObject ();
				pBuffer.mBitmap.Attach (lTempBuffer.mBitmap.Detach ());
				pBuffer.mBitmapBits = lTempBuffer.mBitmapBits;
				lRet = true;
			}
		}
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HBITMAP CAgentIconMaker::GetFrameImage (CAgentFile * pAgentFile)
{
	HBITMAP						lRet = NULL;
	const CAgentFileAnimation *	lGesture;

	if	(
			(pAgentFile)
		&&	(
				(
					(lGesture = pAgentFile->GetGesture (_T("RestPose")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Speak")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Speaking")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Hide")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Hiding")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Listen")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Listening")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Think")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Thinking")))
				&&	(lGesture->mFrameCount > 0)
				&&	(lGesture->mFrames [0].mImageCount > 0)
				)
			)
		)
	{
		tS <CAgentFileImage>	lImage;
		UINT					lImageFormatSize;
		tArrayPtr <BYTE>		lImageFormat;
		LPBYTE					lImageBits;
		LPBITMAPINFO			lBitmapInfo;
		CBitmap					lBitmap;

		lImage.mImageSize = pAgentFile->GetImageSize ();

		if	(
				(lImageFormatSize = pAgentFile->GetImageFormat (NULL, &lImage, true))
			&&	(lImageFormat = new BYTE [lImageFormatSize])
			&&	(lBitmapInfo = (LPBITMAPINFO) lImageFormat.Ptr())
			&&	(pAgentFile->GetImageFormat (lBitmapInfo, &lImage, true))
			)
		{
			if	(
					(lBitmap.Attach (CreateDIBSection (NULL, lBitmapInfo, DIB_RGB_COLORS, (void**)&lImageBits, NULL, 0)))
				&&	(pAgentFile->GetFrameBits (lImageBits, lGesture->mFrames [0], true))
				)
			{
				GdiFlush ();
				lRet = (HBITMAP) lBitmap.Detach ();
			}
		}
	}

	return lRet;
}
