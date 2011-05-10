﻿/////////////////////////////////////////////////////////////////////////////
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
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class OverlayPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public OverlayPanel ()
		{
			InitializeComponent ();

			MouthImages.Images.Add (Properties.Resources.BmpMouthClosed);
			MouthImages.Images.Add (Properties.Resources.BmpMouthWide1);
			MouthImages.Images.Add (Properties.Resources.BmpMouthWide2);
			MouthImages.Images.Add (Properties.Resources.BmpMouthWide3);
			MouthImages.Images.Add (Properties.Resources.BmpMouthWide4);
			MouthImages.Images.Add (Properties.Resources.BmpMouthMedium);
			MouthImages.Images.Add (Properties.Resources.BmpMouthNarrow);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				if (FilePart is ResolveAnimationFrame)
				{
					(FilePart as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeOverlays;
				}
				Frame = (FilePart is ResolveAnimationFrame) ? (FilePart as ResolveAnimationFrame).Target : null;

				ShowFrameName ();
				ShowFrameOverlays ();
				if (!IsEmpty && SelectFirstOverlay ())
				{
					ShowSelectedOverlay ();
				}
			}
		}

		protected FileAnimationFrame Frame
		{
			get;
			set;
		}

		protected FileAnimation Animation
		{
			get
			{
				return (Frame == null) ? null : Frame.Animation;
			}
		}

		protected String FrameTitle
		{
			get
			{
				return Global.TitleFrame (Frame);
			}
		}

		public override Boolean IsEmpty
		{
			get
			{
				return base.IsEmpty || (Frame == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public override object NavigationContext
		{
			get
			{
				return new PanelContext (this);
			}
			set
			{
				if (value is PanelContext)
				{
					(value as PanelContext).RestoreContext (this);
				}
				else
				{
					base.NavigationContext = value;
				}
			}
		}

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (OverlayPanel pPanel)
				: base (pPanel)
			{
				SelectedOverlay = pPanel.ListViewOverlays.SelectedIndex;
			}

			public void RestoreContext (OverlayPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewOverlays.SelectedIndex = SelectedOverlay;
			}

			public int SelectedOverlay
			{
				get;
				protected set;
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
				TextBoxFrameName.Text = Global.TitleFrameAnimation (Frame);
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
					if ((Frame.Overlays != null) && Frame.Overlays.Contains (lOverlayType))
					{
						ShowOverlayItem (lOverlayType, Frame.Overlays[lOverlayType]);
					}
				}

				EndListUpdate ();
				ListViewOverlays.Enabled = true;
			}
		}

		private ListViewItem ShowOverlayItem (MouthOverlay pOverlayType, FileFrameOverlay pFrameOverlay)
		{
			ListViewItem lOverlayItem = ListViewOverlays.Items[(int)pOverlayType];

			if (pFrameOverlay != null)
			{
				ListViewOverlays.UpdateSubItemCount (lOverlayItem);

				lOverlayItem.SubItems[ColumnHeaderPosition.Index].Text = pFrameOverlay.Offset.X.ToString () + ", " + pFrameOverlay.Offset.Y.ToString ();
				lOverlayItem.SubItems[ColumnHeaderReplace.Index].Text = pFrameOverlay.ReplaceFlag ? "Yes" : "No";
				lOverlayItem.SubItems[ColumnHeaderPath.Index].Text = CharacterFile.GetImageFilePath ((int)pFrameOverlay.ImageNdx);
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
			int lListViewHeight = ListViewOverlays.Size.Height + ListViewOverlays.GetItemRect (6).Bottom - ListViewOverlays.ClientRectangle.Bottom;
			ListViewOverlays.Size = new Size (ListViewOverlays.Size.Width, lListViewHeight);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameOverlay (FileFrameOverlay pFrameOverlay)
		{
			System.Drawing.Bitmap lBitmap = null;

			CausesValidation = false;
			LayoutPanelSample.SuspendLayout ();

			if (pFrameOverlay == null)
			{
				NumericOffsetX.ResetText ();
				NumericOffsetY.ResetText ();
				CheckBoxReplace.Checked = false;

				NumericOffsetX.Enabled = false;
				NumericOffsetY.Enabled = false;
				CheckBoxReplace.Enabled = false;
				LayoutPanelSample.Enabled = false;

				if (!IsEmpty)
				{
					lBitmap = CharacterFile.GetFrameBitmap (Frame, true, Color.Transparent);
				}
			}
			else
			{
				NumericOffsetX.Value = pFrameOverlay.Offset.X;
				NumericOffsetY.Value = pFrameOverlay.Offset.Y;
				CheckBoxReplace.Checked = pFrameOverlay.ReplaceFlag;

				NumericOffsetX.Enabled = !Program.FileIsReadOnly;
				NumericOffsetY.Enabled = !Program.FileIsReadOnly;
				CheckBoxReplace.Enabled = !Program.FileIsReadOnly;
				LayoutPanelSample.Enabled = !Program.FileIsReadOnly;

				lBitmap = CharacterFile.GetFrameBitmap (Frame, true, Color.Transparent, (Int16)pFrameOverlay.OverlayType);
			}

			if (lBitmap == null)
			{
				PictureBoxImageSample.Image = null;
				PictureBoxImageSample.ClientSize = PictureBoxSample.DefaultImageSize;
			}
			else
			{
				PictureBoxImageSample.Image = lBitmap;
				PictureBoxImageSample.ClientSize = CharacterFile.Header.ImageSize;
			}

			LayoutPanelSample.ResumeLayout (true);
			ButtonShiftUp.Width = ToolStripShiftUp.Width;
			ButtonShiftDown.Width = ToolStripShiftDown.Width;
			ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
			ButtonShiftRight.Height = ToolStripShiftRight.Height;

			CausesValidation = true;
		}

		private void ShowSelectedOverlay ()
		{
			ShowFrameOverlay (GetSelectedOverlay (true));
			ShowSelectionState (GetSelectedOverlay (false), ListViewOverlays.GetSelectedIndex (false));
		}

		private void ShowSelectionState (FileFrameOverlay pFrameOverlay, int pOverlayNdx)
		{
			if (pFrameOverlay == null)
			{
				ButtonAdd.Enabled = !IsEmpty && !Program.FileIsReadOnly && (ListViewOverlays.GetSelectedIndex (false) >= 0);
				ButtonDelete.Enabled = false;
				ButtonChooseFile.Enabled = false;
			}
			else
			{
				ButtonAdd.Enabled = !Program.FileIsReadOnly && String.IsNullOrEmpty (pFrameOverlay.ImageFilePath);
				ButtonDelete.Enabled = !Program.FileIsReadOnly && !String.IsNullOrEmpty (pFrameOverlay.ImageFilePath);
				ButtonChooseFile.Enabled = !Program.FileIsReadOnly && !String.IsNullOrEmpty (pFrameOverlay.ImageFilePath);
			}

			ButtonAdd.Text = ButtonAdd.Enabled ? String.Format (Properties.Resources.EditAddThis.NoMenuPrefix (), Global.TitleOverlay ((MouthOverlay)ListViewOverlays.GetSelectedIndex (false))) : Properties.Resources.EditAdd.NoMenuPrefix ();
			ButtonDelete.Text = ButtonDelete.Enabled ? String.Format (Properties.Resources.EditDeleteThis.NoMenuPrefix (), Global.TitleOverlay (pFrameOverlay)) : Properties.Resources.EditDelete.NoMenuPrefix ();
			ButtonChooseFile.Text = ButtonChooseFile.Enabled ? String.Format (Properties.Resources.EditChooseThisFile.NoMenuPrefix (), Global.OverlayTypeName (pFrameOverlay.OverlayType)) : Properties.Resources.EditChooseFile.NoMenuPrefix ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean SelectFirstOverlay ()
		{
			if (ListViewOverlays.Items.Count > 0)
			{
				foreach (ListViewItem lItem in ListViewOverlays.Items)
				{
					if (lItem.SubItems.Count > 1)
					{
						lItem.Selected = true;
						return true;
					}
				}
			}
			return false;
		}

		private FileFrameOverlay GetSelectedOverlay (Boolean pIncludeFocus)
		{
			return GetSelectedOverlay (ListViewOverlays.GetSelectedIndex (pIncludeFocus));
		}
		private FileFrameOverlay GetSelectedOverlay (int pOverlayNdx)
		{
			FileFrameOverlay lFrameOverlay = null;

			if ((Frame != null) && (Frame.Overlays != null))
			{
				if (Frame.Overlays.Contains ((MouthOverlay)pOverlayNdx))
				{
					lFrameOverlay = Frame.Overlays[(MouthOverlay)pOverlayNdx];
				}
			}
			return lFrameOverlay;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private RepeatingUpdate<UpdateFrameOverlay, FileFrameOverlay> mRepeatUpdateFrameOverlay = new RepeatingUpdate<UpdateFrameOverlay, FileFrameOverlay> ();

		///////////////////////////////////////////////////////////////////////////////

		private UndoableUpdate UpdateSelectedOverlay (FileFrameOverlay pFrameOverlay)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int lOverlayNdx = ListViewOverlays.GetSelectedIndex (false);
				String lFilePath = String.Empty;

				if (pFrameOverlay != null)
				{
					lFilePath = pFrameOverlay.ImageFilePath;
				}
				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (pFrameOverlay != null)
					{
						UpdateFrameOverlay lUpdate = new UpdateFrameOverlay (pFrameOverlay, false);

						lUpdate.OverlayImagePath = lFilePath;
						if (UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this))
						{
							return lUpdate;
						}
					}
					else if ((lOverlayNdx >= 0) && !String.IsNullOrEmpty (lFilePath))
					{
						AddDeleteFrameOverlay lUpdate = new AddDeleteFrameOverlay (Frame, (MouthOverlay)lOverlayNdx, lFilePath, false);

						if (AddDeleteFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteFrameOverlay, this))
						{
							return lUpdate;
						}
					}
				}
			}
			return null;
		}

		internal UndoableUpdate DeleteSelectedOverlay (FileFrameOverlay pFrameOverlay, Boolean pForClipboard)
		{
			if (!IsEmpty && !Program.FileIsReadOnly && (pFrameOverlay != null))
			{
				AddDeleteFrameOverlay lUpdate = new AddDeleteFrameOverlay (pFrameOverlay, pForClipboard);

				if (AddDeleteFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteFrameOverlay, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedOverlay (FileFrameOverlay pFrameOverlay, FileFrameOverlay pPasteOverlay)
		{
			if (!IsEmpty && !Program.FileIsReadOnly && (pPasteOverlay != null))
			{
				if (pFrameOverlay == null)
				{
					int lOverlayNdx = ListViewOverlays.GetSelectedIndex (false);

					if (lOverlayNdx >= 0)
					{
						AddDeleteFrameOverlay lUpdate = new AddDeleteFrameOverlay (Frame, (MouthOverlay)lOverlayNdx, String.Empty, true);

						lUpdate = lUpdate.Apply () as AddDeleteFrameOverlay;
						if (lUpdate != null)
						{
							pPasteOverlay.CopyTo (lUpdate.Target);
							lUpdate = lUpdate.Apply () as AddDeleteFrameOverlay;
						}
						if ((lUpdate != null) && AddDeleteFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteFrameOverlay, this))
						{
							return lUpdate;
						}
					}
				}
				else
				{
					UpdateFrameOverlay lUpdate = new UpdateFrameOverlay (pFrameOverlay, true);

					pPasteOverlay.CopyTo (pFrameOverlay);
					lUpdate = lUpdate.Apply () as UpdateFrameOverlay;
					if ((lUpdate != null) && UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
			if (ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					pEventArgs.CopyObjectTitle = (pEventArgs is Global.ContextMenuEventArgs) ? Global.TitleOverlay (lFrameOverlay) : Global.TitleOverlayFrameAnimation (lFrameOverlay).Quoted ();
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
						pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
					}
				}
				if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileFrameOverlay))
				{
					pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lFrameOverlay, Global.TitleOverlay ((pEventArgs.PasteObject as FileFrameOverlay).OverlayType), Global.TitleOverlay ((MouthOverlay)ListViewOverlays.SelectedIndex));
				}
			}
		}

		protected override void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e);

			if (!IsEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, ListViewOverlays))
			{
				ToolStripMenuItem lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonChooseFile.Text, ButtonChooseFile.Image, ButtonOpen_Click));
				lMenuItem.Enabled = ButtonChooseFile.Enabled;
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool EditCopy (Global.EditEventArgs pEventArgs)
		{
			if (ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrameOverlay);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override bool EditCut (Global.EditEventArgs pEventArgs)
		{
			if (ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrameOverlay);
						DeleteSelectedOverlay (lFrameOverlay, true);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override bool EditDelete (Global.EditEventArgs pEventArgs)
		{
			if (ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					DeleteSelectedOverlay (lFrameOverlay, false);
					return true;
				}
			}
			return false;
		}

		protected override bool EditPaste (Global.EditEventArgs pEventArgs)
		{
			if (ListViewOverlays.ContainsFocus && (pEventArgs.PasteObject is FileFrameOverlay))
			{
				PasteSelectedOverlay (GetSelectedOverlay (false), pEventArgs.PasteObject as FileFrameOverlay);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			AddDeleteFrameOverlay lAddDeleteOverlay = pUpdate as AddDeleteFrameOverlay;
			UpdateFrameOverlay lUpdateOverlay = pUpdate as UpdateFrameOverlay;
			UpdateAnimationFrame lUpdateFrame = pUpdate as UpdateAnimationFrame;

			if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == this.Animation) && lUpdateAnimation.NameChanged)
			{
				ShowFrameName ();
			}
			else if ((lAddDeleteOverlay != null) && (lAddDeleteOverlay.Frame == Frame))
			{
				int lSelectedNdx = ListViewOverlays.SelectedIndex;
				ShowFrameOverlays ();
				ListViewOverlays.SelectedIndex = lSelectedNdx;
				ShowSelectedOverlay ();
			}
			else if ((lUpdateOverlay != null) && (lUpdateOverlay.Frame == Frame))
			{
				RefreshOverlayItem (lUpdateOverlay.Target);
				ShowSelectedOverlay ();
			}
			else if ((lUpdateFrame != null) && (lUpdateFrame.Target == Frame) && lUpdateFrame.ForClipboard)
			{
				int lSelectedNdx = ListViewOverlays.SelectedIndex;
				ShowFrameOverlays ();
				ListViewOverlays.SelectedIndex = lSelectedNdx;
				ShowSelectedOverlay ();
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
			if (CausesValidation)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
				UpdateFrameOverlay lUpdate;

				if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
				{
					lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
					lUpdate.Offset = new Point ((int)NumericOffsetX.Value, lFrameOverlay.Offset.Y);
					UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this);
				}
			}
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			if (CausesValidation)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
				UpdateFrameOverlay lUpdate;

				if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
				{
					lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
					lUpdate.Offset = new Point (lFrameOverlay.Offset.X, (int)NumericOffsetY.Value);
					UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X, lFrameOverlay.Offset.Y - 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X, lFrameOverlay.Offset.Y + 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X - 1, lFrameOverlay.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new Point (lFrameOverlay.Offset.X + 1, lFrameOverlay.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.ReplaceFlag = CheckBoxReplace.Checked;
				UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this);
			}
		}


		#endregion
	}
}
