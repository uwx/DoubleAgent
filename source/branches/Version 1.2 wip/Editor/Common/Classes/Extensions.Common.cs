/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
using System;
using System.Drawing;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Global
{
	public static partial class StringExtensions
	{
		/// <summary>
		/// Surrounds a <see cref="System.String"/> value with quotes.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with quotes.</returns>
		/// <remarks>If the initial string value is empty or already quoted it is returned unchanged.</remarks>
		public static String Quoted (this String pString)
		{
			if (!String.IsNullOrEmpty (pString) && (pString[0] == '"') && (pString[pString.Length - 1] == '"'))
			{
				return pString;
			}
			else
			{
				return String.Format ("\"{0}\"", pString);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class LangIDExtensions
	{
		/// <summary>
		/// Determines if a <see cref="DoubleAgent.Character.FileCharacterName"/> has the specified Primary Language ID.
		/// </summary>
		/// <param name="pLangID">The Language ID to check.</param>
		/// <param name="pName">The <see cref="DoubleAgent.Character.FileCharacterName"/> to check.</param>
		/// <returns>True if the Primary Language ID's match.</returns>
		/// <see cref="DoubleAgent.Character.FileCharacterName.Language"/>
		public static Boolean PrimaryLanguageEqual (this UInt16 pLangID, FileCharacterName pName)
		{
			if (pName != null)
			{
				return pLangID.PrimaryLanguageEqual (pName.Language);
			}
			return false;
		}

		/// <summary>
		/// Determines of the Primary portion of two Language ID's is equal.
		/// </summary>
		/// <param name="pLangID1">The first Language ID</param>
		/// <param name="pLangID2">The second Language ID</param>
		/// <returns>True if the two Language ID's have the same Primary language.</returns>
		public static Boolean PrimaryLanguageEqual (this UInt16 pLangID1, UInt16 pLangID2)
		{
			return ((Byte)pLangID1 == (Byte)pLangID2);
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class SizeExtensions
	{

		/// <summary>
		/// Limits a <see cref="System.Drawing.Size"/> to a maximum value while maintaining the original aspect ratio.
		/// </summary>
		/// <param name="pSize">The original <see cref="System.Drawing.Size"/>.</param>
		/// <param name="pMaxSize">The maximum <see cref="System.Drawing.Size"/>.</param>
		/// <returns>The <see cref="System.Drawing.SizeF"/> with the aspect ratio of <paramref name="pSize"/> that is at most <paramref name="pMaxSize"/>.</returns>
		static public System.Drawing.SizeF Min (this System.Drawing.Size pSize, System.Drawing.Size pMaxSize)
		{
			SizeF lMaxSize = new SizeF ((float)pMaxSize.Width, (float)pMaxSize.Height);
			SizeF lScaledSize = new SizeF ((float)pSize.Width, (float)pSize.Height);
			PointF lScale = new PointF (lScaledSize.Width / lMaxSize.Width, lScaledSize.Height / lMaxSize.Height);

			if (lScale.X < lScale.Y)
			{
				lScaledSize.Width = lMaxSize.Width * lScale.X / lScale.Y;
				lScaledSize.Height = lMaxSize.Height;
			}
			else
			{
				lScaledSize.Width = lMaxSize.Width;
				lScaledSize.Height = lMaxSize.Height * lScale.Y / lScale.X;
			}
			return lScaledSize;
		}

		/// <summary>
		/// Limits a <see cref="System.Drawing.Size"/> to a minumim value while maintaining the original aspect ratio.
		/// </summary>
		/// <param name="pSize">The original <see cref="System.Drawing.Size"/>.</param>
		/// <param name="pMinSize">The minimum <see cref="System.Drawing.Size"/>.</param>
		/// <returns>The <see cref="System.Drawing.SizeF"/> with the aspect ratio of <paramref name="pSize"/> that is at least <paramref name="pMinSize"/>.</returns>
		static public System.Drawing.SizeF Max (this System.Drawing.Size pSize, System.Drawing.Size pMinSize)
		{
			SizeF lMinSize = new SizeF ((float)pMinSize.Width, (float)pMinSize.Height);
			SizeF lScaledSize = new SizeF ((float)pSize.Width, (float)pSize.Height);
			PointF lScale = new PointF (lScaledSize.Width / lMinSize.Width, lScaledSize.Height / lMinSize.Height);

			if (lScale.X > lScale.Y)
			{
				lScaledSize.Width = lMinSize.Width * lScale.X / lScale.Y;
				lScaledSize.Height = lMinSize.Height;
			}
			else
			{
				lScaledSize.Width = lMinSize.Width;
				lScaledSize.Height = lMinSize.Height * lScale.Y / lScale.X;
			}
			return lScaledSize;
		}

		///////////////////////////////////////////////////////////////////////////////

		static public Boolean EitherLT (this System.Drawing.Size pSize, System.Drawing.Size pMaxSize)
		{
			return (pSize.Width < pMaxSize.Width) || (pSize.Height < pMaxSize.Height);
		}
		static public Boolean EitherGT (this System.Drawing.Size pSize, System.Drawing.Size pMinSize)
		{
			return (pSize.Width > pMinSize.Width) || (pSize.Height > pMinSize.Height);
		}

		static public Boolean EitherLE (this System.Drawing.Size pSize, System.Drawing.Size pMaxSize)
		{
			return (pSize.Width <= pMaxSize.Width) || (pSize.Height <= pMaxSize.Height);
		}
		static public Boolean EitherGE (this System.Drawing.Size pSize, System.Drawing.Size pMinSize)
		{
			return (pSize.Width >= pMinSize.Width) || (pSize.Height >= pMinSize.Height);
		}

		static public Boolean BothLT (this System.Drawing.Size pSize, System.Drawing.Size pMaxSize)
		{
			return (pSize.Width < pMaxSize.Width) && (pSize.Height < pMaxSize.Height);
		}
		static public Boolean BothGT (this System.Drawing.Size pSize, System.Drawing.Size pMinSize)
		{
			return (pSize.Width > pMinSize.Width) && (pSize.Height > pMinSize.Height);
		}

		static public Boolean BothLE (this System.Drawing.Size pSize, System.Drawing.Size pMaxSize)
		{
			return (pSize.Width <= pMaxSize.Width) && (pSize.Height <= pMaxSize.Height);
		}
		static public Boolean BothGE (this System.Drawing.Size pSize, System.Drawing.Size pMinSize)
		{
			return (pSize.Width >= pMinSize.Width) && (pSize.Height >= pMinSize.Height);
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static partial class ImageExtensions
	{
		public static System.Drawing.SizeF GuessScreenResolution ()
		{
			System.Drawing.SizeF lResolution = new System.Drawing.SizeF (96.0f, 96.0f);
			GuessScreenResolution (ref lResolution);
			return lResolution;
		}

		public static void GuessScreenResolution (ref System.Drawing.SizeF pResolution)
		{
			if (pResolution == null)
			{
				pResolution = new System.Drawing.SizeF (96.0f, 96.0f);
			}
			try
			{
				System.Drawing.Bitmap lBitmap = new System.Drawing.Bitmap (1, 1);
				pResolution.Width = lBitmap.HorizontalResolution;
				pResolution.Height = lBitmap.VerticalResolution;
			}
			catch
			{
			}
		}

		static public System.Windows.Size PreserveAspectRatio (this System.Windows.Size pSize, System.Windows.Size pAspectRatio)
		{
			System.Windows.Size lRet = pSize;

			try
			{
				if (Math.Round (pSize.Width / pAspectRatio.Width * 1000.0) != Math.Round (pSize.Height / pAspectRatio.Height * 1000.0))
				{
					if ((pSize.Width / pAspectRatio.Width) > (pSize.Height / pAspectRatio.Height))
					{
						lRet = new System.Windows.Size (pSize.Height * pAspectRatio.Width / pAspectRatio.Height, pSize.Height);
					}
					else
					{
						lRet = new System.Windows.Size (pSize.Width, pSize.Width * pAspectRatio.Height / pAspectRatio.Width);
					}
				}
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("[{0} {1}] PreserveAspectRatio [{2} {3}] Return [{4} {5}]", pSize.Width, pSize.Height, pAspectRatio.Width, pAspectRatio.Height, lRet.Width, lRet.Height);
#endif
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		public static System.Windows.Media.ImageSource MakeImageSource (this System.Drawing.Bitmap pBitmap)
		{
			System.Drawing.SizeF lResolution = new System.Drawing.SizeF (pBitmap.HorizontalResolution, pBitmap.VerticalResolution);
			GuessScreenResolution (ref lResolution);
			return pBitmap.MakeImageSource (lResolution.Width, lResolution.Height);
		}

		public static System.Windows.Media.ImageSource MakeImageSource (this System.Drawing.Bitmap pBitmap, double pDpi)
		{
			return pBitmap.MakeImageSource (pDpi, pDpi);
		}

		public static System.Windows.Media.ImageSource MakeImageSource (this System.Drawing.Bitmap pBitmap, double pDpiX, double pDpiY)
		{
			System.Windows.Media.ImageSource lImageSource = null;

			if (pBitmap != null)
			{
				try
				{
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Bitmap Size [{0}] Resolution [{2} {3}] Flags [{1}] Dpi [{4} {5}]", pBitmap.Size, pBitmap.Flags, pBitmap.HorizontalResolution, pBitmap.VerticalResolution, pDpiX, pDpiY);
#endif
					System.Drawing.Imaging.BitmapData lBitmapData;
					lBitmapData = pBitmap.LockBits (new System.Drawing.Rectangle (0, 0, pBitmap.Width, pBitmap.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
					try
					{
						lImageSource = System.Windows.Media.Imaging.BitmapSource.Create (lBitmapData.Width, lBitmapData.Height, pDpiX, pDpiY, System.Windows.Media.PixelFormats.Bgra32, null, lBitmapData.Scan0, lBitmapData.Height * lBitmapData.Stride, lBitmapData.Stride);
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("Image Size [{0} {1}]", lImageSource.Width, lImageSource.Height);
#endif
					}
#if DEBUG
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
#else
					catch {}
#endif
					pBitmap.UnlockBits (lBitmapData);
				}
#if DEBUG
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
#else
				catch {}
#endif

			}
			return lImageSource;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
}
