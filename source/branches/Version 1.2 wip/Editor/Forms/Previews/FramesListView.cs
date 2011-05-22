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
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public class FramesListView : DoubleAgent.ListViewEx
	{
		#region Initialization

		public FramesListView ()
		{
			DoubleBuffered = true;
			InitializeComponent ();
		}

		private void InitializeComponent ()
		{
			this.SuspendLayout ();
			// 
			// AnimationFramesView
			// 
			this.Alignment = System.Windows.Forms.ListViewAlignment.Left;
			this.HideSelection = false;
			this.MultiSelect = false;
			this.OwnerDraw = true;
			this.ShowGroups = false;
			this.ResumeLayout (false);

		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get;
			set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FileAnimation Animation
		{
			get;
			set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public new void EnsureVisible (int pItemIndex)
		{
			EnsureVisible (pItemIndex, false);
		}

		public void EnsureVisible (int pItemIndex, Boolean pRedrawNow)
		{
			if ((pItemIndex >= 0) && (pItemIndex < Items.Count))
			{
				Rectangle lItemRect = GetItemRect (pItemIndex);
				Rectangle lVisibleRect = ClientRectangle;
				FramesPreview lContainer = (Parent as FramesPreview);

				lItemRect = Parent.RectangleToClient (RectangleToScreen (lItemRect));
				lVisibleRect = Parent.RectangleToClient (RectangleToScreen (lVisibleRect));
				lVisibleRect.Intersect (Parent.ClientRectangle);

				if (lItemRect.Left < lVisibleRect.Left)
				{
					lContainer.AutoScrollPosition = new Point (Math.Max (lContainer.HorizontalScroll.Value + (lItemRect.Left - lVisibleRect.Left), lContainer.HorizontalScroll.Minimum), 0);
				}
				else if (lItemRect.Right > lVisibleRect.Right)
				{
					lContainer.AutoScrollPosition = new Point (Math.Min (lContainer.HorizontalScroll.Value + (lItemRect.Right - lVisibleRect.Right), lContainer.HorizontalScroll.Maximum), 0);
				}

				if (pRedrawNow)
				{
					lContainer.Refresh ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Drawing

		protected override void OnDrawItem (DrawListViewItemEventArgs e)
		{
			Bitmap lImage = GetItemImage (e.Item, e.ItemIndex);
			Rectangle lItemRect;
			RectangleF lImageRect;

			e.DrawDefault = true;
			base.OnDrawItem (e);

			if (lImage != null)
			{
				lItemRect = GetItemRect (e.ItemIndex, ItemBoundsPortion.Icon);
				lItemRect.Inflate (-2,-2);
				lImageRect = new RectangleF (lItemRect.Left, lItemRect.Top, lItemRect.Width, lItemRect.Height);
				lImageRect.Width = lImage.Width * lImageRect.Height / (float)lImage.Height;
				lImageRect.Offset (((float)lItemRect.Width - lImageRect.Width) / 2.0f, 0.0f);
				//System.Diagnostics.Debug.Print ("Draw {0} in {1}", lImage.Size, lImageRect);

				e.Graphics.DrawImage (lImage, lImageRect);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public Bitmap GetItemImage (ListViewItem pItem)
		{
			return GetItemImage (pItem, pItem.Index);
		}

		public Bitmap GetItemImage (ListViewItem pItem, int pItemIndex)
		{
			Bitmap lImage = null;
			if (pItem.Tag is Bitmap)
			{
				lImage = pItem.Tag as Bitmap;
			}
			else if ((pItem.Tag == null) && (CharacterFile != null) && (Animation != null))
			{
				try
				{
					lImage = GetFrameImage (CharacterFile, Animation.Frames[pItemIndex]);
				}
				catch
				{
				}
				if (lImage != null)
				{
					pItem.Tag = lImage;
				}
				else
				{
					pItem.Tag = Boolean.FalseString;
				}
			}
			return lImage;
		}

		///////////////////////////////////////////////////////////////////////////////

		static public Bitmap GetFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			Bitmap lImage = null;

			if ((pCharacterFile != null) && (pFrame != null))
			{
				try
				{
					if (pFrame.ImageCount > 0)
					{
						lImage = pCharacterFile.GetFrameBitmap (pFrame, true, Color.Transparent);
					}
					else
					{
						lImage = new Bitmap (pCharacterFile.Header.ImageSize.Width, pCharacterFile.Header.ImageSize.Height);
						Graphics lGraphics = Graphics.FromImage (lImage);
						Pen lPen = new Pen (Color.Pink, 5.0f);

						lGraphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.None;
						lGraphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

						lGraphics.DrawRectangle (lPen, 3.0f, 3.0f, (float)lImage.Width - 7.0f, (float)lImage.Height - 7.0f);
						lGraphics.DrawLine (lPen, 3.0f, 3.0f, (float)lImage.Width - 4.0f, (float)lImage.Height - 4.0f);
						lGraphics.DrawLine (lPen, 3.0f, (float)lImage.Height - 4.0f, (float)lImage.Width - 4.0f, 3.0f);
						lGraphics.Dispose ();
					}
				}
				catch
				{
				}
			}
			return lImage;
		}

		#endregion
	}
}
