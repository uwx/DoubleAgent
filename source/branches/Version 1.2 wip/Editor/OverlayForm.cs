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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class OverlayForm : UserControl
	{
		private CharacterFile		mCharacterFile = null;
		private FileAnimationFrame	mFrame = null;
		private String				mFrameName = null;
		private String				mAnimationName = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public OverlayForm ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				FileFrame = null;
				FrameName = null;
				AnimationName = null;
			}
		}

		public FileAnimationFrame FileFrame
		{
			get
			{
				return mFrame;
			}
			set
			{
				mFrame = value;
				FrameName = null;
				mAnimationName = null;
			}
		}

		public String FrameName
		{
			get
			{
				return mFrameName;
			}
			set
			{
				mFrameName = value;
			}
		}

		public String AnimationName
		{
			get
			{
				return mAnimationName;
			}
			set
			{
				mAnimationName = value;
				ShowFrameName ();
				ShowFrameOverlays ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFrame == null) || String.IsNullOrEmpty (mFrameName) || String.IsNullOrEmpty (mAnimationName));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameName ()
		{
			if (IsEmpty)
			{
				TextBoxFrameName.ResetText ();
				TextBoxFrameName.Enabled = false;
			}
			else
			{
				TextBoxFrameName.Text = mAnimationName + " - " + mFrameName;
				TextBoxFrameName.Enabled = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameOverlays ()
		{
			if (IsEmpty)
			{
				BeginListUpdate ();
				EndListUpdate ();
				ListViewOverlays.Enabled = false;
				ShowFrameOverlay (null);
			}
			else
			{
				BeginListUpdate ();

				for (MouthOverlay lOverlayType = MouthOverlay.MouthOverlayMin; lOverlayType <= MouthOverlay.MouthOverlayMax; lOverlayType++)
				{
					if ((mFrame.Overlays != null) && mFrame.Overlays.Contains (lOverlayType))
					{
						ShowOverlayItem (lOverlayType, mFrame.Overlays[lOverlayType]);
					}
				}

				EndListUpdate ();
				ListViewOverlays.Enabled = true;

				SelectFirstOverlay ();
				ShowSelectedOverlay ();
			}
		}

		private ListViewItem ShowOverlayItem (MouthOverlay pOverlayType, FileFrameOverlay pFrameOverlay)
		{
			ListViewItem	lOverlayItem =  ListViewOverlays.Items[(int)pOverlayType];

			if (pFrameOverlay != null)
			{
				ListViewOverlays.UpdateSubItemCount (lOverlayItem);

				lOverlayItem.SubItems[ColumnHeaderPosition.Index].Text = pFrameOverlay.Offset.X.ToString () + ", " + pFrameOverlay.Offset.Y.ToString ();
				lOverlayItem.SubItems[ColumnHeaderReplace.Index].Text = pFrameOverlay.ReplaceFlag ? "Yes" : "No";
				lOverlayItem.SubItems[ColumnHeaderPath.Index].Text = mCharacterFile.GetImageFilePath ((int)pFrameOverlay.ImageNdx);
			}
			return lOverlayItem;
		}

		private void RefreshOverlayItem (FileFrameOverlay pFrameOverlay)
		{
			if (pFrameOverlay != null)
			{
				ListViewOverlays.BeginUpdate ();
				if (ShowOverlayItem (pFrameOverlay.OverlayType, pFrameOverlay) != null)
				{
					ListViewOverlays.AutoSizeColumn (ColumnHeaderPosition);
					ListViewOverlays.AutoSizeColumn (ColumnHeaderReplace);
					ListViewOverlays.AutoSizeColumn (ColumnHeaderPath);
				}
				ListViewOverlays.EndUpdate ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void BeginListUpdate ()
		{
			ListViewOverlays.BeginUpdate ();

			foreach (ListViewItem lOverlayItem in ListViewOverlays.Items)
			{
				lOverlayItem.SubItems.Clear ();
			}
		}

		private void EndListUpdate ()
		{
			ListViewOverlays.AutoSizeColumn (ColumnHeaderMouth);
			ListViewOverlays.AutoSizeColumn (ColumnHeaderPosition);
			ListViewOverlays.AutoSizeColumn (ColumnHeaderReplace);
			ListViewOverlays.AutoSizeColumn (ColumnHeaderPath);
			ListViewOverlays.EndUpdate ();
			UpdateListHeight ();
		}

		private void UpdateListHeight ()
		{
			int	lListViewHeight = ListViewOverlays.Size.Height + ListViewOverlays.GetItemRect (6).Bottom - ListViewOverlays.ClientRectangle.Bottom;
			ListViewOverlays.Size = new Size (ListViewOverlays.Size.Width, lListViewHeight);
		}

		private void SelectFirstOverlay ()
		{
			if (ListViewOverlays.Items.Count > 0)
			{
				foreach (ListViewItem lItem in ListViewOverlays.Items)
				{
					if (lItem.SubItems.Count > 1)
					{
						lItem.Selected = true;
						break;
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private FileFrameOverlay GetSelectedOverlay (bool pIncludeFocus)
		{
			return GetSelectedOverlay (ListViewOverlays.GetSelectedIndex (pIncludeFocus));
		}

		private FileFrameOverlay GetSelectedOverlay (int pOverlayNdx)
		{
			FileFrameOverlay	lFrameOverlay = null;

			if ((mFrame != null) && (mFrame.Overlays != null))
			{
				if (mFrame.Overlays.Contains ((MouthOverlay)pOverlayNdx))
				{
					lFrameOverlay = mFrame.Overlays[(MouthOverlay)pOverlayNdx];
				}
			}
			return lFrameOverlay;
		}

		private void ShowSelectedOverlay ()
		{
			ShowFrameOverlay (GetSelectedOverlay (true));
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameOverlay (FileFrameOverlay pFrameOverlay)
		{
			System.Drawing.Bitmap	lBitmap = null;

			if (pFrameOverlay == null)
			{
				NumericOffsetX.ResetText ();
				NumericOffsetY.ResetText ();
				CheckBoxReplace.Checked = false;

				NumericOffsetX.Enabled = false;
				NumericOffsetY.Enabled = false;
				CheckBoxReplace.Enabled = false;
				TableLayoutSample.Enabled = false;

				ButtonAdd.Enabled = (ListViewOverlays.GetSelectedIndex (false) >= 0) && (!IsEmpty) && (!Program.MainForm.FileIsReadOnly);
				ButtonRemove.Enabled = false;
				ButtonOpen.Enabled = false;

				if (!IsEmpty)
				{
					lBitmap = mCharacterFile.GetFrameBitmap (mFrame, true, Color.Transparent);
				}
			}
			else
			{
				NumericOffsetX.Value = pFrameOverlay.Offset.X;
				NumericOffsetY.Value = pFrameOverlay.Offset.Y;
				CheckBoxReplace.Checked = pFrameOverlay.ReplaceFlag;

				NumericOffsetX.Enabled = !Program.MainForm.FileIsReadOnly;
				NumericOffsetY.Enabled = !Program.MainForm.FileIsReadOnly;
				CheckBoxReplace.Enabled = !Program.MainForm.FileIsReadOnly;
				TableLayoutSample.Enabled = !Program.MainForm.FileIsReadOnly;

				ButtonAdd.Enabled = (String.IsNullOrEmpty (pFrameOverlay.ImageFilePath)) && (!Program.MainForm.FileIsReadOnly);
				ButtonRemove.Enabled = (!String.IsNullOrEmpty (pFrameOverlay.ImageFilePath)) && (!Program.MainForm.FileIsReadOnly);
				ButtonOpen.Enabled = (!String.IsNullOrEmpty (pFrameOverlay.ImageFilePath)) && (!Program.MainForm.FileIsReadOnly);

				lBitmap = mCharacterFile.GetFrameBitmap (mFrame, true, Color.Transparent, (Int16)pFrameOverlay.OverlayType);
			}

			TableLayoutSample.SuspendLayout ();
			if (lBitmap == null)
			{
				PictureBoxImageSample.Image = null;
				PictureBoxImageSample.ClientSize = PictureBoxSample.DefaultImageSize;
			}
			else
			{
				PictureBoxImageSample.Image = lBitmap;
				PictureBoxImageSample.ClientSize = mCharacterFile.Header.ImageSize;
			}
			TableLayoutSample.ResumeLayout (true);

			ButtonShiftUp.Width = ToolStripShiftUp.Width;
			ButtonShiftDown.Width = ToolStripShiftDown.Width;
			ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
			ButtonShiftRight.Height = ToolStripShiftRight.Height;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OverlayForm_Layout (object sender, LayoutEventArgs e)
		{
			UpdateListHeight ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewImages_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (ListViewOverlays.SelectedIndex >= 0)
			{
				ShowSelectedOverlay ();
			}
		}

		private void ListViewOverlays_ItemActivate (object sender, EventArgs e)
		{
			UpdateSelectedOverlay ();
		}

		private void UpdateSelectedOverlay ()
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int					lOverlayNdx = ListViewOverlays.GetSelectedIndex (false);
				FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
				String				lFilePath = String.Empty;

				if (lFrameOverlay != null)
				{
					lFilePath = lFrameOverlay.ImageFilePath;
				}
				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (lFrameOverlay != null)
					{
						lFrameOverlay.ImageFilePath = lFilePath;
						RefreshOverlayItem (lFrameOverlay);
						ShowSelectedOverlay ();
						Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
					}
					else if (lOverlayNdx >= 0)
					{
						mFrame.Overlays.Add ((MouthOverlay)lOverlayNdx, lFilePath);
						RefreshOverlayItem (GetSelectedOverlay (false));
						ShowSelectedOverlay ();
						Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			UpdateSelectedOverlay ();
		}

		private void ButtonRemove_Click (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
			}
		}

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedOverlay ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.Offset = new Point ((int)NumericOffsetX.Value, lFrameOverlay.Offset.Y);
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.Offset = new Point (lFrameOverlay.Offset.X, (int)NumericOffsetY.Value);
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.Offset = new Point (lFrameOverlay.Offset.X, lFrameOverlay.Offset.Y - 1);
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftDown_Click (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.Offset = new Point (lFrameOverlay.Offset.X, lFrameOverlay.Offset.Y + 1);
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftLeft_Click (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.Offset = new Point (lFrameOverlay.Offset.X - 1, lFrameOverlay.Offset.Y);
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftRight_Click (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.Offset = new Point (lFrameOverlay.Offset.X + 1, lFrameOverlay.Offset.Y);
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxReplace_Click (object sender, EventArgs e)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameOverlay != null))
			{
				lFrameOverlay.ReplaceFlag = CheckBoxReplace.Checked;
				RefreshOverlayItem (lFrameOverlay);
				ShowSelectedOverlay ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		#endregion
	}
}
