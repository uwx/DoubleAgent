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

namespace AgentCharacterEditor
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
				lImageRect = new RectangleF (lItemRect.Left, lItemRect.Top, lItemRect.Width, lItemRect.Height);
				lImageRect.Width = lImage.Width * lImageRect.Height / (float)lImage.Height;
				lImageRect.Offset (((float)lItemRect.Width - lImageRect.Width) / 2.0f, 0.0f);

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
			try
			{
				if (pItem.Tag is Bitmap)
				{
					lImage = pItem.Tag as Bitmap;
				}
				else if ((pItem.Tag == null) && (CharacterFile != null) && (Animation != null))
				{
					pItem.Tag = Boolean.FalseString;
					lImage = CharacterFile.GetFrameBitmap (Animation.Frames[pItemIndex], true, Color.Transparent);
					pItem.Tag = lImage;
				}
			}
			catch
			{
			}
			return lImage;
		}

		#endregion
	}
}
