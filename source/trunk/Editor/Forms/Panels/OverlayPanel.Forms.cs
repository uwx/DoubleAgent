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
using System;
using System.Drawing;
using System.Windows.Forms;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class OverlayPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public OverlayPanel ()
		{
			InitializeComponent ();

			if (Program.MainWindow != null)
			{
				Program.MainWindow.InitializeContextMenu (ContextMenuOverlaysList);
			}

			MouthImages.Images.Add (Properties.Resources.ImgMouthClosed);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide1);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide2);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide3);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide4);
			MouthImages.Images.Add (Properties.Resources.ImgMouthMedium);
			MouthImages.Images.Add (Properties.Resources.ImgMouthNarrow);

			ListViewOverlays.Items.Add (new ListViewItemCommon ("Closed", 0));
			ListViewOverlays.Items.Add (new ListViewItemCommon ("Wide 1", 1));
			ListViewOverlays.Items.Add (new ListViewItemCommon ("Wide 2", 2));
			ListViewOverlays.Items.Add (new ListViewItemCommon ("Wide 3", 3));
			ListViewOverlays.Items.Add (new ListViewItemCommon ("Wide 4", 4));
			ListViewOverlays.Items.Add (new ListViewItemCommon ("Medium", 5));
			ListViewOverlays.Items.Add (new ListViewItemCommon ("Narrow", 6));
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			if (Settings.Default.IsValid)
			{
				CheckBoxOverlayTransparent.Checked = Settings.Default.ShowOverlayTransparency;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings.Default.ShowOverlayTransparency = CheckBoxOverlayTransparent.Checked;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameOverlays ()
		{
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					BeginListUpdate ();
					EndListUpdate ();
					ListViewOverlays.Enabled = false;
				}
				else
				{
					BeginListUpdate ();

					for (MouthOverlay lOverlayType = MouthOverlay.MouthOverlayMin; lOverlayType <= MouthOverlay.MouthOverlayMax; lOverlayType++)
					{
						if ((Frame.Overlays != null) && Frame.Overlays.Contains (lOverlayType))
						{
							ShowOverlayItem (lOverlayType, Frame.Overlays[lOverlayType]);
						}
					}

					EndListUpdate ();
					ListViewOverlays.Enabled = true;
				}
				if (lContext != null)
				{
					lContext.RestoreContext (this);
				}
				if (IsPanelEmpty)
				{
					ShowFrameOverlay (null);
				}
			}
		}

		private ListViewItemCommon ShowOverlayItem (MouthOverlay pOverlayType, FileFrameOverlay pFrameOverlay)
		{
			ListViewItemCommon lOverlayItem = ListViewOverlays.Items[(int)pOverlayType] as ListViewItemCommon;

			if (pFrameOverlay != null)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					ListViewOverlays.UpdateSubItemCount (lOverlayItem);
					lOverlayItem.SubItems[ColumnHeaderPosition.Index].Text = pFrameOverlay.Offset.X.ToString () + ", " + pFrameOverlay.Offset.Y.ToString ();
					lOverlayItem.SubItems[ColumnHeaderReplace.Index].Text = pFrameOverlay.ReplaceFlag ? "Yes" : "No";
					lOverlayItem.SubItems[ColumnHeaderPath.Index].Text = CharacterFile.GetImageFilePath ((int)pFrameOverlay.ImageNdx);
				}
			}
			return lOverlayItem;
		}

		private void RefreshOverlayItem (FileFrameOverlay pFrameOverlay)
		{
			if (pFrameOverlay != null)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
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
		}

		//=============================================================================

		private void BeginListUpdate ()
		{
			ListViewOverlays.BeginUpdate ();

			foreach (ListViewItemCommon lOverlayItem in ListViewOverlays.Items)
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
			int lListViewHeight = ListViewOverlays.Size.Height + ListViewOverlays.GetItemRect (6).Bottom - ListViewOverlays.ClientRectangle.Bottom;
			ListViewOverlays.Size = new Size (ListViewOverlays.Size.Width, lListViewHeight);
		}

		//=============================================================================

		private void ShowOverlaySample (FileFrameOverlay pFrameOverlay)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				System.Drawing.Bitmap lFrameBitmap = null;
				System.Drawing.Bitmap lOverlayBitmap = null;

				GroupBoxImage.Text = Titles.Overlay (pFrameOverlay);
				PanelOverlayImage.SuspendLayout ();
				PanelSample.SuspendLayout ();

				if (pFrameOverlay == null)
				{
					ButtonShiftUp.Enabled = false;
					ButtonShiftDown.Enabled = false;
					ButtonShiftLeft.Enabled = false;
					ButtonShiftRight.Enabled = false;

					if (!IsPanelEmpty)
					{
						lFrameBitmap = CharacterFile.GetFrameBitmap (Frame, true, CheckBoxOverlayTransparent.Checked ? Color.Transparent : Color.Empty);
					}
				}
				else
				{
					ButtonShiftUp.Enabled = !Program.FileIsReadOnly;
					ButtonShiftDown.Enabled = !Program.FileIsReadOnly;
					ButtonShiftLeft.Enabled = !Program.FileIsReadOnly;
					ButtonShiftRight.Enabled = !Program.FileIsReadOnly;

					lFrameBitmap = CharacterFile.GetFrameBitmap (Frame, true, CheckBoxOverlayTransparent.Checked ? Color.Transparent : Color.Empty, (Int16)pFrameOverlay.OverlayType);
					lOverlayBitmap = CharacterFile.GetImageBitmap ((int)pFrameOverlay.ImageNdx, true, CheckBoxOverlayTransparent.Checked ? Color.Transparent : Color.Empty);
				}

				if (lFrameBitmap == null)
				{
					PictureBoxImageSample.Image = null;
					PictureBoxImageSample.ClientSize = FrameSample.DefaultImageSize;
				}
				else
				{
					PictureBoxImageSample.Image = lFrameBitmap;
					PictureBoxImageSample.ClientSize = CharacterFile.Header.ImageSize;
				}

				if (lOverlayBitmap == null)
				{
					PanelOverlayImage.Visible = false;
					PictureBoxOverlaySample.Image = null;
				}
				else
				{
					PictureBoxOverlaySample.Image = lOverlayBitmap;
					PictureBoxOverlaySample.ClientSize = lOverlayBitmap.Size;
					PanelOverlayImage.Visible = true;
				}

				PanelSample.ResumeLayout (true);
				PanelOverlayImage.ResumeLayout (true);

				ButtonShiftUp.Width = ToolStripShiftUp.Width;
				ButtonShiftDown.Width = ToolStripShiftDown.Width;
				ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
				ButtonShiftRight.Height = ToolStripShiftRight.Height;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OverlayForm_Layout (object sender, LayoutEventArgs e)
		{
			UpdateListHeight ();
		}

		private void ContextMenuOverlaysList_Opening (object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (IsPanelEmpty || Program.FileIsReadOnly)
			{
				e.Cancel = true;
			}
			else
			{
				Program.MainWindow.ShowEditState (ContextMenuOverlaysList);

				MenuItemAdd.Enabled = CanAddOverlay;
				MenuItemAdd.SetTitle (AddOverlayTitle);
				MenuItemChooseFile.Enabled = CanChooseOverlayFile;
				MenuItemChooseFile.SetTitle (ChooseOverlayFileTitle);
			}
		}

		//=============================================================================

		private void ListViewImages_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && (ListViewOverlays.SelectedIndex >= 0))
			{
				ShowSelectedOverlay ();
			}
		}

		private void ListViewOverlays_ItemActivate (object sender, EventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay ());
		}

		//=============================================================================

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay ());
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			DeleteSelectedOverlay (GetSelectedOverlay (), false);
		}

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay ());
		}

		//=============================================================================

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			HandleOffsetXChanged ();
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			HandleOffsetYChanged ();
		}

		//=============================================================================

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			HandleShiftUpClick (ButtonShiftUp.ClickRepeatNum);
		}
		private void ButtonShiftUp_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftDown_Click (object sender, EventArgs e)
		{
			HandleShiftDownClick (ButtonShiftDown.ClickRepeatNum);
		}
		private void ButtonShiftDown_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftLeft_Click (object sender, EventArgs e)
		{
			HandleShiftLeftClick (ButtonShiftLeft.ClickRepeatNum);
		}
		private void ButtonShiftLeft_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftRight_Click (object sender, EventArgs e)
		{
			HandleShiftRightClick (ButtonShiftRight.ClickRepeatNum);
		}
		private void ButtonShiftRight_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		//=============================================================================

		private void CheckBoxReplace_Click (object sender, EventArgs e)
		{
			HandleReplaceChanged ();
		}

		private void CheckBoxOverlayTransparent_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowSelectedOverlay ();
			}
		}

		#endregion
	}
}
