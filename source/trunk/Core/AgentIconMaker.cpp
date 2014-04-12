/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "ImageBuffer.h"
#include "ImageTools.h"
#include "ImageAlpha.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#include "DebugStr.h"
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

HICON CAgentIconMaker::MakeIcon (CAgentFile* pAgentFile, const CSize& pIconSize, const CRect* pClipRect)
{
	HICON			lIcon = NULL;
	HBITMAP			lFrameImage;
	CImageBuffer	lImageBuffer;
	CRect			lImageRect;
	CImageBuffer	lRenderBuffer;
	CPoint			lRenderPos;
	CSize			lRenderSize;
	tS <BITMAP>		lBitmapInfo;

	if	(
			(lImageBuffer.mImage = new ATL::CImage)
		&&	(lFrameImage = GetFrameImage (pAgentFile))
		)
	{
		lImageBuffer.mImage->Attach (lFrameImage);
	}
	if	(lImageBuffer.GetImage ())
	{
		lImageRect = CRect (CPoint (0,0), lImageBuffer.GetImageSize ());

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
			CImageDebugger::SaveBitmap (lImageBuffer.GetImage(), CAtlString(pAgentFile->FindName()->mName)+_T("- Generated"));
		}
#endif

		if	(
				(lImageBuffer.StartBuffer ())
			&&	(lRenderBuffer.CreateBuffer (lRenderSize, true, true, true))
			)
		{
			::BitBlt (*lRenderBuffer.mDC, lRenderPos.x, lRenderPos.y, lImageRect.Width(), lImageRect.Height(), *lImageBuffer.mDC, lImageRect.left, lImageRect.top, SRCCOPY);
			lImageBuffer.EndBuffer ();
			lRenderBuffer.EndBuffer ();

			if	(RemoveMargin (lRenderBuffer))
			{
#ifdef	_DEBUG_ICONS
				if	(LogIsActive (_DEBUG_ICONS))
				{
					LogMessage (_DEBUG_ICONS, _T("Reduced to [%s]"), FormatSize(lRenderBuffer.GetImageSize()));
				}
#endif
			}

			if	(
					(lImageRect.Size() != pIconSize)
				&&	(lImageBuffer.CreateBuffer (pIconSize, true, true))
				)
			{
				Gdiplus::Bitmap		lBitmap (lRenderBuffer.mImage->GetWidth(), lRenderBuffer.mImage->GetHeight(), abs(lRenderBuffer.mImage->GetPitch()), PixelFormat32bppPARGB, GetImageBits(*lRenderBuffer.mImage));
				Gdiplus::Graphics	lGraphics (*lImageBuffer.mDC);

				lGraphics.Clear (Gdiplus::Color (0,0,0,0));
				lGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
				lGraphics.SetCompositingQuality (Gdiplus::CompositingQualityDefault);
				lGraphics.SetInterpolationMode (Gdiplus::InterpolationModeHighQualityBilinear);
				lGraphics.DrawImage (&lBitmap, 0, 0, pIconSize.cx, pIconSize.cy);
			}
			else
			{
				lImageBuffer.mImage.Attach (lRenderBuffer.mImage.Detach ());
			}

			lImageBuffer.EndBuffer ();
			lRenderBuffer.EndBuffer (true);

			if	(
					(lRenderBuffer.CreateBuffer (pIconSize, true, true))
				&&	(lImageBuffer.StartBuffer ())
				)
			{
				BitBlt (*lRenderBuffer.mDC, 0, 0, pIconSize.cx, pIconSize.cy, *lImageBuffer.mDC, 0, 0, SRCCOPY);
				lRenderBuffer.EndBuffer ();
				CImageAlpha::AlphaSaturateImage (*lRenderBuffer.mImage);
				GdiFlush ();
			}

			lImageBuffer.EndBuffer ();
			lRenderBuffer.EndBuffer ();
#ifdef	_DEBUG_ICONS
			if	(LogIsActive (_DEBUG_ICONS))
			{
				CImageDebugger::SaveBitmap (lImageBuffer.GetImage(), CAtlString(pAgentFile->FindName()->mName)+_T("- Resized"));
				CImageDebugger::SaveBitmap (lRenderBuffer.GetImage(), CAtlString(pAgentFile->FindName()->mName)+_T("- Masked"));
			}
#endif
			if	(
					(lImageBuffer.GetImage ())
				&&	(lRenderBuffer.GetImage ())
				)
			{
				tS <ICONINFO>	lIconInfo;

				lIconInfo.fIcon = TRUE;
				lIconInfo.hbmColor = lImageBuffer.GetImage ();
				lIconInfo.hbmMask = lRenderBuffer.GetImage ();

				lIcon = CreateIconIndirect (&lIconInfo);

#ifdef	_DEBUG_ICONS
				if	(LogIsActive (_DEBUG_ICONS))
				{
					CImageDebugger::SaveIcon (lIcon, CAtlString(pAgentFile->FindName()->mName)+_T("- Generated"));
				}
#endif
			}
		}
	}

	return lIcon;
}

bool CAgentIconMaker::RemoveMargin (CImageBuffer & pBuffer)
{
	bool	lRet = false;
	LPBYTE	lImageBits;

	if	(
			(pBuffer.mImage)
		&&	(lImageBits = GetImageBits (*pBuffer.mImage))
		)
	{
		CPoint	lPixel;
		CSize	lImageSize (pBuffer.mImage->GetWidth(), pBuffer.mImage->GetHeight());
		int		lImagePitch = abs (pBuffer.mImage->GetPitch());
		long	lMarginMax = MulDiv (min (lImageSize.cx, lImageSize.cy), 1, 4);
		long	lMargin;
		long	lNdx;

		for	(lMargin = 0; lMargin < lMarginMax; lMargin++)
		{
			bool	lPixelFound = false;

			lPixel.y = lMargin;
			for	(lPixel.x = lMargin, lNdx = lPixel.y*lImagePitch; lPixel.x < lImageSize.cx-lMargin; lPixel.x++, lNdx += 4)
			{
				if	(*(DWORD*)(lImageBits + lNdx) != 0)
				{
					lPixelFound = true;
					break;
				}
			}
			if	(!lPixelFound)
			{
				lPixel.y = lImageSize.cy-lMargin-1;
				for	(lPixel.x = lMargin, lNdx = lPixel.y*lImagePitch; lPixel.x < lImageSize.cx-lMargin; lPixel.x++, lNdx += 4)
				{
					if	(*(DWORD*)(lImageBits + lNdx) != 0)
					{
						lPixelFound = true;
						break;
					}
				}
			}
			if	(!lPixelFound)
			{
				lPixel.x = lMargin;
				for	(lPixel.y = lMargin; lPixel.y < lImageSize.cy-lMargin; lPixel.y++)
				{
					lNdx = (lPixel.y * lImagePitch) + (lPixel.x * 4);
					if	(*(DWORD*)(lImageBits + lNdx) != 0)
					{
						lPixelFound = true;
						break;
					}
				}
			}
			if	(!lPixelFound)
			{
				lPixel.x = lImageSize.cx-lMargin-1;
				for	(lPixel.y = lMargin; lPixel.y < lImageSize.cy-lMargin; lPixel.y++)
				{
					lNdx = (lPixel.y * lImagePitch) + (lPixel.x * 4);
					if	(*(DWORD*)(lImageBits + lNdx) != 0)
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
			CRect			lTempRect (CPoint (0,0), pBuffer.GetImageSize ());
			CImageBuffer	lTempBuffer;

			lTempRect.DeflateRect (lMargin, lMargin);

			if	(
					(lTempBuffer.CreateBuffer (lTempRect.Size(), true, true, true))
				&&	(pBuffer.StartBuffer ())
				)
			{
				::BitBlt (*lTempBuffer.mDC, 0, 0, lTempRect.Width(), lTempRect.Height(), *pBuffer.mDC, lTempRect.left, lTempRect.top, SRCCOPY);
				lTempBuffer.EndBuffer ();
				pBuffer.EndBuffer ();

				pBuffer.mImage = lTempBuffer.mImage.Detach ();
				lRet = true;
			}
		}
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HBITMAP CAgentIconMaker::GetFrameImage (CAgentFile* pAgentFile)
{
	HBITMAP						lRet = NULL;
	const CAgentFileAnimation*	lGesture;

	if	(
			(pAgentFile)
		&&	(
				(
					(lGesture = pAgentFile->GetGesture (_T("RestPose")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Speak")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Speaking")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Hide")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Hiding")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Listen")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Listening")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Think")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			||	(
					(lGesture = pAgentFile->GetGesture (_T("Thinking")))
				&&	(lGesture->FrameCount > 0)
				&&	(lGesture->Frames [0].ImageCount > 0)
				)
			)
		)
	{
		CAgentFileImage		lImage;
		UINT				lImageFormatSize;
		tArrayPtr <BYTE>	lImageFormat;
		LPBITMAPINFO		lBitmapInfo;
		ATL::CImage			lBitmap;

		lImage.put_ImageSize (pAgentFile->Header.ImageSize);

		if	(
				(lImageFormatSize = pAgentFile->GetImageFormat (NULL, &lImage, true))
			&&	(lImageFormat = new BYTE [lImageFormatSize])
			&&	(lBitmapInfo = (LPBITMAPINFO) lImageFormat.Ptr())
			&&	(pAgentFile->GetImageFormat (lBitmapInfo, &lImage, true))
			)
		{
			lBitmap.Attach (CreateDIBSection (NULL, lBitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0));
			if	(
					(GetImageBits (lBitmap))
				&&	(pAgentFile->GetFrameBits (GetImageBits (lBitmap), &lGesture->Frames [0], true))
				)
			{
				GdiFlush ();
				lRet = (HBITMAP) lBitmap.Detach ();
			}
		}
	}

	return lRet;
}
