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
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;

namespace AgentCharacterEditor
{
	public class PictureBoxSample : System.Windows.Forms.PictureBox
	{
		public PictureBoxSample ()
		{
			this.SizeMode = PictureBoxSizeMode.Zoom;
		}

		///////////////////////////////////////////////////////////////////////////////

		public new System.Drawing.Size ClientSize
		{
			get
			{
				return base.ClientSize;
			}
			set
			{
				this.Size = this.Size + value - this.DisplayRectangle.Size;

				if (this.DisplayRectangle.Size != value)   // Adjust for Min/Max size
				{
					SizeF	lDisplaySize = new SizeF ((float)this.DisplayRectangle.Width, (float)this.DisplayRectangle.Height);
					SizeF	lScaledSize = new SizeF ((float)value.Width, (float)value.Height);
					PointF	lScale = new PointF (lScaledSize.Width / lDisplaySize.Width, lScaledSize.Height / lDisplaySize.Height);

					if (lScale.X < lScale.Y)
					{
						lScaledSize.Width = lDisplaySize.Width * lScale.X / lScale.Y;
						lScaledSize.Height = lDisplaySize.Height;
					}
					else
					{
						lScaledSize.Width = lDisplaySize.Width;
						lScaledSize.Height = lDisplaySize.Height * lScale.Y / lScale.X;
					}

#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Scale {0} by {1} to {2}", value.ToString (), lScale.ToString (), lScaledSize.ToString ());
#endif
					this.Size = this.Size + Size.Round (lScaledSize) - this.DisplayRectangle.Size;
				}
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("Image {0} Client {1} Display {2} Control {3} Scale {4}", value.ToString (), this.ClientSize.ToString(), this.DisplayRectangle.Size.ToString (), this.Size.ToString (), ImageScale.ToString());
#endif
			}
		}

		public float ImageScale
		{
			get
			{
				try
				{
					Size	lDisplaySize = this.DisplayRectangle.Size;
					Size	lImageSize;
					PointF	lScale;

					if (this.SizeMode == PictureBoxSizeMode.Zoom)
					{
						if (this.Image != null)
						{
							lImageSize = this.Image.Size;
						}
						else
						{
							lImageSize = lDisplaySize;
						}
					}
					else
					{
						lImageSize = this.ClientSize;
					}

					lScale = new PointF ((float)lDisplaySize.Width / (float)lImageSize.Width, (float)lDisplaySize.Height / (float)lImageSize.Height);
					return Math.Min (lScale.X, lScale.Y);
				}
				catch
				{
					return 1.0F;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public System.Drawing.Point ScaledPoint (System.Drawing.Point pPoint)
		{
			float	lImageScale = this.ImageScale;
			PointF	lScaledPoint = new PointF ((float)pPoint.X * lImageScale, (float)pPoint.Y * lImageScale);
			return Point.Round (lScaledPoint);
		}

		public System.Drawing.Point UnscaledPoint (System.Drawing.Point pPoint)
		{
			float	lImageScale = this.ImageScale;
			PointF	lScaledPoint = new PointF ((float)pPoint.X / lImageScale, (float)pPoint.Y / lImageScale);
			return Point.Round (lScaledPoint);
		}

		public System.Drawing.Size ScaledSize (System.Drawing.Size pSize)
		{
			float	lImageScale = this.ImageScale;
			SizeF	lScaledSize = new SizeF ((float)pSize.Width * lImageScale, (float)pSize.Height * lImageScale);
			return Size.Round (lScaledSize);
		}

		public System.Drawing.Size UnscaledSize (System.Drawing.Size pSize)
		{
			float	lImageScale = this.ImageScale;
			SizeF	lScaledSize = new SizeF ((float)pSize.Width / lImageScale, (float)pSize.Height / lImageScale);
			return Size.Round (lScaledSize);
		}

		///////////////////////////////////////////////////////////////////////////////
		
		public static Size DefaultImageSize 
		{
			get
			{
				return new Size (128,128);
			}
		}

		public static Size MaxImageSize
		{
			get
			{
				return new Size (256, 256);
			}
		}
	}
}
