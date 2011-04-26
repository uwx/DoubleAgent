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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public partial class OverlayForm : UserControl
	{
		private CharacterFile		mCharacterFile = null;
		private FileAnimationFrame	mFrame = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public OverlayForm ()
		{
			InitializeComponent ();
			CausesValidation = Visible;
		}

		private void OverlayForm_VisibleChanged (object sender, EventArgs e)
		{
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.UpdateApplied -= new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				Program.MainForm.CanEdit -= new Global.EditEventHandler (MainForm_CanEdit);
				Program.MainForm.EditCopy -= new Global.EditEventHandler (MainForm_EditCopy);
				Program.MainForm.EditCut -= new Global.EditEventHandler (MainForm_EditCut);
				Program.MainForm.EditDelete -= new Global.EditEventHandler (MainForm_EditDelete);
				Program.MainForm.EditPaste -= new Global.EditEventHandler (MainForm_EditPaste);
				Program.MainForm.EditMenu -= new Global.ContextMenuEventHandler (MainForm_EditMenu);
				if (Visible)
				{
					Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					Program.MainForm.CanEdit += new Global.EditEventHandler (MainForm_CanEdit);
					Program.MainForm.EditCopy += new Global.EditEventHandler (MainForm_EditCopy);
					Program.MainForm.EditCut += new Global.EditEventHandler (MainForm_EditCut);
					Program.MainForm.EditDelete += new Global.EditEventHandler (MainForm_EditDelete);
					Program.MainForm.EditPaste += new Global.EditEventHandler (MainForm_EditPaste);
					Program.MainForm.EditMenu += new Global.ContextMenuEventHandler (MainForm_EditMenu);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				this.Frame = null;
			}
		}

		[System.ComponentModel.Browsable (false)]
		public FileAnimation Animation
		{
			get
			{
				return (mFrame == null) ? null : mFrame.Animation;
			}
		}
		[System.ComponentModel.Browsable (false)]
		public String FrameTitle
		{
			get
			{
				return Global.TitleFrame (mFrame);
			}
		}
		[System.ComponentModel.Browsable (false)]
		public FileAnimationFrame Frame
		{
			get
			{
				return mFrame;
			}
			set
			{
				mFrame = value;

				ShowFrameName ();
				ShowFrameOverlays ();
				if (!IsEmpty && SelectFirstOverlay ())
				{
					ShowSelectedOverlay ();
				}
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFrame == null));
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
				TextBoxFrameName.Text = Global.TitleFrameAnimation (mFrame);
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

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameOverlay (FileFrameOverlay pFrameOverlay)
		{
			System.Drawing.Bitmap	lBitmap = null;

			CausesValidation = false;
			TableLayoutSample.SuspendLayout ();

			if (pFrameOverlay == null)
			{
				NumericOffsetX.ResetText ();
				NumericOffsetY.ResetText ();
				CheckBoxReplace.Checked = false;

				NumericOffsetX.Enabled = false;
				NumericOffsetY.Enabled = false;
				CheckBoxReplace.Enabled = false;
				TableLayoutSample.Enabled = false;

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

				NumericOffsetX.Enabled = !Program.FileIsReadOnly;
				NumericOffsetY.Enabled = !Program.FileIsReadOnly;
				CheckBoxReplace.Enabled = !Program.FileIsReadOnly;
				TableLayoutSample.Enabled = !Program.FileIsReadOnly;

				lBitmap = mCharacterFile.GetFrameBitmap (mFrame, true, Color.Transparent, (Int16)pFrameOverlay.OverlayType);
			}

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

		private void ShowEditState (Global.EditEventArgs pEventArgs)
		{
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private RepeatingUpdate<UpdateFrameOverlay,FileFrameOverlay> mRepeatUpdateFrameOverlay = new RepeatingUpdate<UpdateFrameOverlay, FileFrameOverlay> ();

		///////////////////////////////////////////////////////////////////////////////

		private UndoableUpdate UpdateSelectedOverlay (FileFrameOverlay pFrameOverlay)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int		lOverlayNdx = ListViewOverlays.GetSelectedIndex (false);
				String	lFilePath = String.Empty;

				if (pFrameOverlay != null)
				{
					lFilePath = pFrameOverlay.ImageFilePath;
				}
				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (pFrameOverlay != null)
					{
						UpdateFrameOverlay	lUpdate = new UpdateFrameOverlay (pFrameOverlay, false);

						lUpdate.OverlayImagePath = lFilePath;
						if (UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this))
						{
							return lUpdate;
						}
					}
					else if ((lOverlayNdx >= 0) && !String.IsNullOrEmpty (lFilePath))
					{
						AddDeleteFrameOverlay	lUpdate = new AddDeleteFrameOverlay (mFrame, (MouthOverlay)lOverlayNdx, lFilePath, false);

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
				AddDeleteFrameOverlay	lUpdate = new AddDeleteFrameOverlay (pFrameOverlay, pForClipboard);

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
					int	lOverlayNdx = ListViewOverlays.GetSelectedIndex (false);

					if (lOverlayNdx >= 0)
					{
						AddDeleteFrameOverlay	lUpdate = new AddDeleteFrameOverlay (mFrame, (MouthOverlay)lOverlayNdx, String.Empty, true);

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
					UpdateFrameOverlay	lUpdate = new UpdateFrameOverlay (pFrameOverlay, true);

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

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateAnimation			lUpdateAnimation = sender as UpdateAnimation;
				AddDeleteFrameOverlay	lAddDeleteOverlay = sender as AddDeleteFrameOverlay;
				UpdateFrameOverlay		lUpdateOverlay = sender as UpdateFrameOverlay;
				UpdateAnimationFrame	lUpdateFrame = sender as UpdateAnimationFrame;

				if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == this.Animation) && lUpdateAnimation.NameChanged)
				{
					ShowFrameName ();
				}
				else if ((lAddDeleteOverlay != null) && (lAddDeleteOverlay.Frame == mFrame))
				{
					int	lSelectedNdx = ListViewOverlays.SelectedIndex;
					ShowFrameOverlays ();
					ListViewOverlays.SelectedIndex = lSelectedNdx;
					ShowSelectedOverlay ();
				}
				else if ((lUpdateOverlay != null) && (lUpdateOverlay.Frame == mFrame))
				{
					RefreshOverlayItem (lUpdateOverlay.Target);
					ShowSelectedOverlay ();
				}
				else if ((lUpdateFrame != null) && (lUpdateFrame.Target == mFrame) && lUpdateFrame.ForClipboard)
				{
					int	lSelectedNdx = ListViewOverlays.SelectedIndex;
					ShowFrameOverlays ();
					ListViewOverlays.SelectedIndex = lSelectedNdx;
					ShowSelectedOverlay ();
				}
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
				FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
				UpdateFrameOverlay	lUpdate;

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
				FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
				UpdateFrameOverlay	lUpdate;

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
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay	lUpdate;

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
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay	lUpdate;

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
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay	lUpdate;

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
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay	lUpdate;

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
			FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay	lUpdate;

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.ReplaceFlag = CheckBoxReplace.Checked;
				UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameOverlay, this);
			}
		}


		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewOverlays.ContainsFocus)
			{
				ShowEditState (e);
			}
		}

		internal void MainForm_EditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrameOverlay);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrameOverlay);
						DeleteSelectedOverlay (lFrameOverlay, true);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewOverlays.ContainsFocus)
			{
				FileFrameOverlay	lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					e.IsUsed = true;
					DeleteSelectedOverlay (lFrameOverlay, false);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewOverlays.ContainsFocus)
			{
				if (e.PasteObject is FileFrameOverlay)
				{
					e.IsUsed = true;
					PasteSelectedOverlay (GetSelectedOverlay (false), e.PasteObject as FileFrameOverlay);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal void MainForm_EditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e);

			if (!IsEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, ListViewOverlays))
			{
				ToolStripMenuItem	lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonChooseFile.Text, ButtonChooseFile.Image, ButtonOpen_Click));
				lMenuItem.Enabled = ButtonChooseFile.Enabled;
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;
			}
		}

		#endregion
	}
}
