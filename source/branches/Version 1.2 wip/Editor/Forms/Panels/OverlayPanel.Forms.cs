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

			MouthImages.Images.Add (Properties.Resources.ImgMouthClosed);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide1);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide2);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide3);
			MouthImages.Images.Add (Properties.Resources.ImgMouthWide4);
			MouthImages.Images.Add (Properties.Resources.ImgMouthMedium);
			MouthImages.Images.Add (Properties.Resources.ImgMouthNarrow);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (OverlayPanel pPanel)
				: base (pPanel)
			{
				SelectedOverlay = pPanel.ListViewOverlays.SelectedIndex;
				FocusedOverlay = pPanel.ListViewOverlays.FocusedIndex;
			}

			public void RestoreContext (OverlayPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewOverlays.SelectedIndex = SelectedOverlay;
				pPanel.ListViewOverlays.FocusedIndex = FocusedOverlay;
			}

			public int SelectedOverlay
			{
				get;
				protected set;
			}
			public int FocusedOverlay
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameOverlays ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
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
					if ((Frame.Overlays != null) && Frame.Overlays.Contains (lOverlayType))
					{
						ShowOverlayItem (lOverlayType, Frame.Overlays[lOverlayType]);
					}
				}

				EndListUpdate ();
				ListViewOverlays.Enabled = true;
			}

			PopIsPanelFilling (lWasFilling);
		}

		private ListViewItemCommon ShowOverlayItem (MouthOverlay pOverlayType, FileFrameOverlay pFrameOverlay)
		{
			ListViewItemCommon lOverlayItem = ListViewOverlays.Items[(int)pOverlayType] as ListViewItemCommon;

			if (pFrameOverlay != null)
			{
				Boolean lWasFilling = PushIsPanelFilling (true);

				ListViewOverlays.UpdateSubItemCount (lOverlayItem);
				lOverlayItem.SubItems[ColumnHeaderPosition.Index].Text = pFrameOverlay.Offset.X.ToString () + ", " + pFrameOverlay.Offset.Y.ToString ();
				lOverlayItem.SubItems[ColumnHeaderReplace.Index].Text = pFrameOverlay.ReplaceFlag ? "Yes" : "No";
				lOverlayItem.SubItems[ColumnHeaderPath.Index].Text = CharacterFile.GetImageFilePath ((int)pFrameOverlay.ImageNdx);

				PopIsPanelFilling (lWasFilling);
			}
			return lOverlayItem;
		}

		private void RefreshOverlayItem (FileFrameOverlay pFrameOverlay)
		{
			if (pFrameOverlay != null)
			{
				Boolean lWasFilling = PushIsPanelFilling (true);

				ListViewOverlays.BeginUpdate ();
				if (ShowOverlayItem (pFrameOverlay.OverlayType, pFrameOverlay) != null)
				{
					ListViewOverlays.AutoSizeColumn (ColumnHeaderPosition);
					ListViewOverlays.AutoSizeColumn (ColumnHeaderReplace);
					ListViewOverlays.AutoSizeColumn (ColumnHeaderPath);
				}
				ListViewOverlays.EndUpdate ();

				PopIsPanelFilling (lWasFilling);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameSample (FileFrameOverlay pFrameOverlay)
		{
			Boolean lWasFilling = PushIsPanelFilling (true);
			System.Drawing.Bitmap lBitmap = null;

			PanelSample.SuspendLayout ();

			if (pFrameOverlay == null)
			{
				PanelSample.Enabled = false;
				if (!IsPanelEmpty)
				{
					lBitmap = CharacterFile.GetFrameBitmap (Frame, true, Color.Transparent);
				}
			}
			else
			{
				PanelSample.Enabled = !Program.FileIsReadOnly;
				lBitmap = CharacterFile.GetFrameBitmap (Frame, true, Color.Transparent, (Int16)pFrameOverlay.OverlayType);
			}

			if (lBitmap == null)
			{
				PictureBoxImageSample.Image = null;
				PictureBoxImageSample.ClientSize = FrameSample.DefaultImageSize;
			}
			else
			{
				PictureBoxImageSample.Image = lBitmap;
				PictureBoxImageSample.ClientSize = CharacterFile.Header.ImageSize;
			}

			PanelSample.ResumeLayout (true);
			ButtonShiftUp.Width = ToolStripShiftUp.Width;
			ButtonShiftDown.Width = ToolStripShiftDown.Width;
			ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
			ButtonShiftRight.Height = ToolStripShiftRight.Height;

			PopIsPanelFilling (lWasFilling);
		}

		private void ShowSelectionState (FileFrameOverlay pFrameOverlay, int pOverlayNdx)
		{
			if (pFrameOverlay == null)
			{
				ButtonAdd.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly && (pOverlayNdx >= 0);
				ButtonDelete.IsEnabled = false;
				ButtonChooseFile.IsEnabled = false;
			}
			else
			{
				ButtonAdd.IsEnabled = !Program.FileIsReadOnly && String.IsNullOrEmpty (pFrameOverlay.ImageFilePath);
				ButtonDelete.IsEnabled = !Program.FileIsReadOnly && !String.IsNullOrEmpty (pFrameOverlay.ImageFilePath);
				ButtonChooseFile.IsEnabled = !Program.FileIsReadOnly && !String.IsNullOrEmpty (pFrameOverlay.ImageFilePath);
			}

			ButtonAdd.Text = ButtonAdd.IsEnabled ? String.Format (AppResources.Resources.EditAddThis.NoMenuPrefix (), Titles.Overlay ((MouthOverlay)pOverlayNdx)) : AppResources.Resources.EditAdd.NoMenuPrefix ();
			ButtonDelete.Text = ButtonDelete.IsEnabled ? String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Overlay (pFrameOverlay)) : AppResources.Resources.EditDelete.NoMenuPrefix ();
			ButtonChooseFile.Text = ButtonChooseFile.IsEnabled ? String.Format (AppResources.Resources.EditChooseThisFile.NoMenuPrefix (), Titles.OverlayTypeName (pFrameOverlay.OverlayType)) : AppResources.Resources.EditChooseFile.NoMenuPrefix ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs))
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					String lObjectTitle = (pEventArgs is Global.ContextMenuEventArgs) ? Titles.Overlay (lFrameOverlay) : Titles.OverlayFrameAnimation (lFrameOverlay).Quoted ();

					pEventArgs.PutCopyTitle (lObjectTitle);
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.PutCutTitle (lObjectTitle);
						pEventArgs.PutDeleteTitle (lObjectTitle);
					}
				}
				if (!Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileFrameOverlay))
				{
					pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lFrameOverlay, Titles.Overlay ((pEventArgs.GetPasteObject () as FileFrameOverlay).OverlayType), Titles.Overlay ((MouthOverlay)ListViewOverlays.SelectedIndex));
				}
			}
		}

		protected override void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e);

			if (!IsPanelEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, ListViewOverlays))
			{
				ToolStripMenuItem lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonChooseFile.Text, ButtonChooseFile.Image, ButtonOpen_Click));
				lMenuItem.Enabled = ButtonChooseFile.Enabled;
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;
			}
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
			UpdateSelectedOverlay (GetSelectedOverlay (false));
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay (false));
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			DeleteSelectedOverlay (GetSelectedOverlay (false), false);
		}

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay (false));
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !Program.FileIsReadOnly)
			{
				HandleNumericOffsetXChanged ();
			}
			NumericOffsetX.IsModified = false;
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !Program.FileIsReadOnly)
			{
				HandleNumericOffsetYChanged ();
			}
			NumericOffsetY.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X, lFrameOverlay.Offset.Y - 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
				mRepeatUpdateFrameOverlay.PutUpdate (lUpdate, ButtonShiftUp.ClickRepeatNum);
			}
		}
		private void ButtonShiftUp_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameOverlay.EndUpdate ();
		}

		private void ButtonShiftDown_Click (object sender, EventArgs e)
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X, lFrameOverlay.Offset.Y + 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
				mRepeatUpdateFrameOverlay.PutUpdate (lUpdate, ButtonShiftDown.ClickRepeatNum);
			}
		}
		private void ButtonShiftDown_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameOverlay.EndUpdate ();
		}

		private void ButtonShiftLeft_Click (object sender, EventArgs e)
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X - 1, lFrameOverlay.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
				mRepeatUpdateFrameOverlay.PutUpdate (lUpdate, ButtonShiftLeft.ClickRepeatNum);
			}
		}
		private void ButtonShiftLeft_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameOverlay.EndUpdate ();
		}

		private void ButtonShiftRight_Click (object sender, EventArgs e)
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X + 1, lFrameOverlay.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
				mRepeatUpdateFrameOverlay.PutUpdate (lUpdate, ButtonShiftRight.ClickRepeatNum);
			}
		}
		private void ButtonShiftRight_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameOverlay.EndUpdate ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxReplace_Click (object sender, EventArgs e)
		{
			HandleReplaceChanged ();
		}


		#endregion
	}
}
