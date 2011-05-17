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
using System.Windows.Forms;
using AgentCharacterEditor.Global;

namespace AgentCharacterEditor
{
	/// <summary>
	/// A <see cref="System.Windows.Forms.PictureBox"/> with extra functionality to support isometric scaling.
	/// </summary>
	/// <remarks>The <see cref="SizeMode"/> is initialized to <see cref="PictureBoxSizeMode.Zoom"/> and cannot be changed.</remarks>
	public class PictureBoxSample : System.Windows.Forms.PictureBox
	{
		/// <summary>
		/// Constructor
		/// </summary>
		public PictureBoxSample ()
		{
			base.SizeMode = PictureBoxSizeMode.Zoom;
		}

		/// <summary>
		/// Override of <see cref="System.Windows.Forms.PictureBox.SizeMode"/> that limits possible values to <see cref="PictureBoxSizeMode.Zoom"/> .
		/// </summary>
		public new PictureBoxSizeMode SizeMode
		{
			get
			{
				return base.SizeMode;
			}
			set
			{
				if (value == PictureBoxSizeMode.Zoom)
				{
					base.SizeMode = value;
				}
#if DEBUG
				else
				{
					System.Diagnostics.Debug.Print ("SizeMode change ignored");
				}
#endif
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Gets or sets the control's client area dimensions.<para>Override of <see cref="System.Windows.Forms.Control.ClientSize"/>.</para>
		/// </summary>
		/// <remarks>When the ClientSize is changed, it is constrained by the <see cref="System.Windows.Forms.Control.MinimumSize"/> and <see cref="System.Windows.Forms.Control.MaximumSize"/>.
		/// However, the constraints are not applied independently to the Width and Height.  Instead, the control's original aspect ratio
		/// is retained.
		/// </remarks>
		/// <seealso cref="MaximumImageSize"/>
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
					SizeF lScaledSize = value.Min (this.DisplayRectangle.Size);
					this.Size = this.Size + Size.Round (lScaledSize) - this.DisplayRectangle.Size;
				}
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("Image {0} Client {1} Display {2} Control {3} Scale {4}", value.ToString (), this.ClientSize.ToString(), this.DisplayRectangle.Size.ToString (), this.Size.ToString (), ImageScale.ToString());
#endif
			}
		}

		/// <summary>
		/// Gets the scaling (zoom) factor applied to the displayed image.
		/// </summary>
		public float ImageScale
		{
			get
			{
				try
				{
					Size lDisplaySize = this.DisplayRectangle.Size;
					Size lImageSize;
					PointF lScale;

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

		/// <summary>
		/// Converts a <see cref="System.Drawing.Point"/> from Image coordinates to Client coordinates.
		/// </summary>
		/// <param name="pPoint">The <see cref="System.Drawing.Point"/> in Image coordinates.</param>
		/// <returns>The <see cref="System.Drawing.Point"/> in Client coordinates</returns>
		/// <seealso cref="ImageScale"/>
		public System.Drawing.Point ImageToClient (System.Drawing.Point pPoint)
		{
			float lImageScale = this.ImageScale;
			PointF lScaledPoint = new PointF ((float)pPoint.X * lImageScale, (float)pPoint.Y * lImageScale);
			return Point.Round (lScaledPoint);
		}

		/// <summary>
		/// Converts a <see cref="System.Drawing.Point"/> from Client coordinates to Image coordinates.
		/// </summary>
		/// <param name="pPoint">The <see cref="System.Drawing.Point"/> in Client coordinates.</param>
		/// <returns>The <see cref="System.Drawing.Point"/> in Image coordinates</returns>
		/// <seealso cref="ImageScale"/>
		public System.Drawing.Point ClientToImage (System.Drawing.Point pPoint)
		{
			float lImageScale = this.ImageScale;
			PointF lScaledPoint = new PointF ((float)pPoint.X / lImageScale, (float)pPoint.Y / lImageScale);
			return Point.Round (lScaledPoint);
		}

		/// <summary>
		/// Converts a <see cref="System.Drawing.Size"/> from Image coordinates to Client coordinates.
		/// </summary>
		/// <param name="pSize">The <see cref="System.Drawing.Size"/> in Image coordinates.</param>
		/// <returns>The <see cref="System.Drawing.Size"/> in Client coordinates</returns>
		/// <seealso cref="ImageScale"/>
		public System.Drawing.Size ImageToClient (System.Drawing.Size pSize)
		{
			float lImageScale = this.ImageScale;
			SizeF lScaledSize = new SizeF ((float)pSize.Width * lImageScale, (float)pSize.Height * lImageScale);
			return Size.Round (lScaledSize);
		}

		/// <summary>
		/// Converts a <see cref="System.Drawing.Size"/> from Client coordinates to Image coordinates.
		/// </summary>
		/// <param name="pSize">The <see cref="System.Drawing.Size"/> in Client coordinates.</param>
		/// <returns>The <see cref="System.Drawing.Size"/> in Image coordinates</returns>
		/// <seealso cref="ImageScale"/>
		public System.Drawing.Size ClientToImage (System.Drawing.Size pSize)
		{
			float lImageScale = this.ImageScale;
			SizeF lScaledSize = new SizeF ((float)pSize.Width / lImageScale, (float)pSize.Height / lImageScale);
			return Size.Round (lScaledSize);
		}

		///////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// The suggested default dimensions of a sample image.
		/// </summary>
		public static Size DefaultImageSize
		{
			get
			{
				return new Size (128, 128);
			}
		}

		/// <summary>
		/// The suggested maximum dimensions of a sample image.
		/// </summary>
		public static Size MaximumImageSize
		{
			get
			{
				return new Size (256, 256);
			}
		}
	}
}
