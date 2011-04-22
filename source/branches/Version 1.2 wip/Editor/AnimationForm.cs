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
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public partial class AnimationForm : UserControl
	{
		private CharacterFile			mCharacterFile = null;
		private FileAnimation			mAnimation = null;
		private int						mReturnItemNone = -1;
		private int						mReturnItemExit = -1;
		private List <ListViewItem>		mStateItems;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationForm ()
		{
			InitializeComponent ();

			mStateItems = new List<ListViewItem> ();
			foreach (ListViewItem lListItem in ListViewStates.Items)
			{
				mStateItems.Add (lListItem);
			}
			ListViewStates.Items.Clear ();
		}

		private void AnimationForm_VisibleChanged (object sender, EventArgs e)
		{
			if (Program.MainForm != null)
			{
				Program.MainForm.UpdateApplied -= new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				Program.MainForm.CanEdit -= new Global.EditEventHandler (MainForm_CanEdit);
				Program.MainForm.EditCopy -= new Global.EditEventHandler (MainForm_EditCopy);
				Program.MainForm.EditCut -= new Global.EditEventHandler (MainForm_EditCut);
				Program.MainForm.EditDelete -= new Global.EditEventHandler (MainForm_EditDelete);
				Program.MainForm.EditPaste -= new Global.EditEventHandler (MainForm_EditPaste);
				if (this.Visible)
				{
					Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					Program.MainForm.CanEdit += new Global.EditEventHandler (MainForm_CanEdit);
					Program.MainForm.EditCopy += new Global.EditEventHandler (MainForm_EditCopy);
					Program.MainForm.EditCut += new Global.EditEventHandler (MainForm_EditCut);
					Program.MainForm.EditDelete += new Global.EditEventHandler (MainForm_EditDelete);
					Program.MainForm.EditPaste += new Global.EditEventHandler (MainForm_EditPaste);
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
				this.Animation = null;
			}
		}

		[System.ComponentModel.Browsable (false)]
		public FileAnimation Animation
		{
			get
			{
				return mAnimation;
			}
			set
			{
				mAnimation = value;

				ShowAnimationName ();
				ShowReturnAnimations ();
				ShowReturnAnimation ();
				ShowAnimationStates ();
				ShowAnimationPreview ();
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mAnimation == null));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event Global.GoToFrameEventHandler GoToFrame;
		public event Global.GoToStateEventHandler GoToState;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowAnimationName ()
		{
			if (IsEmpty)
			{
				TextBoxName.ResetText ();
				TextBoxName.Enabled = false;
				ToolStripFrames.Enabled = false;
			}
			else
			{
				TextBoxName.Text = mAnimation.Name;
				TextBoxName.Enabled = !Program.FileIsReadOnly;
				ToolStripFrames.Enabled = !Program.FileIsReadOnly;
			}
			TextBoxName.Modified = false;
		}

		private void ShowReturnAnimations ()
		{
			ComboBoxReturn.BeginUpdate ();
			ComboBoxReturn.Items.Clear ();
			mReturnItemNone = ComboBoxReturn.Items.Add ("None");
			mReturnItemExit = -1;

			if (IsEmpty)
			{
				ComboBoxReturn.SelectedIndex = -1;
				ComboBoxReturn.Enabled = false;
			}
			else
			{
				String[]	lAnimations = mCharacterFile.GetAnimationNames ();

				mReturnItemExit = ComboBoxReturn.Items.Add ("Use Exit Branching");
				foreach (String lAnimation in lAnimations)
				{
					if (String.Compare (lAnimation, mAnimation.Name, true) != 0)
					{
						ComboBoxReturn.Items.Add (lAnimation);
					}
				}
				ComboBoxReturn.Enabled = !Program.FileIsReadOnly;
			}

			ComboBoxReturn.EndUpdate ();
		}

		private void ShowReturnAnimation ()
		{
			if (IsEmpty)
			{
				ComboBoxReturn.SelectedIndex = mReturnItemNone;
			}
			else if (mAnimation.ReturnType == 1)
			{
				ComboBoxReturn.SelectedIndex = mReturnItemExit;
			}
			else if ((mAnimation.ReturnType != 0) || (String.IsNullOrEmpty (mAnimation.ReturnName)))
			{
				ComboBoxReturn.SelectedIndex = mReturnItemNone;
			}
			else
			{
				ComboBoxReturn.SelectedIndex = ComboBoxReturn.FindStringExact (mAnimation.ReturnName);
				if (ComboBoxReturn.SelectedIndex < 0)
				{
					ComboBoxReturn.SelectedIndex = mReturnItemNone;
				}
			}
		}

		private void ShowAnimationStates ()
		{
			FileStates	lFileStates = null;

			if (IsEmpty)
			{
				ListViewStates.Enabled = false;
			}
			else
			{
				ListViewStates.Enabled = true;//!Program.FileIsReadOnly;
				lFileStates = mCharacterFile.States;
			}

			ListViewStates.Items.Clear ();

			foreach (ListViewItem lListItem in mStateItems)
			{
				String[]	lStateAnimations = null;

				if (lFileStates != null)
				{
					if (lFileStates.ContainsKey (lListItem.Text))
					{
						lStateAnimations = lFileStates[lListItem.Text];
					}
					else if (lFileStates.ContainsKey (lListItem.Text.ToUpper ()))
					{
						lStateAnimations = lFileStates[lListItem.Text.ToUpper ()];
					}
				}
				if (
						(lStateAnimations != null)
					&& (
							(Array.IndexOf (lStateAnimations, mAnimation.Name) >= 0)
						|| (Array.IndexOf (lStateAnimations, mAnimation.Name.ToUpper ()) >= 0)
						)
					)
				{
					ListViewStates.Items.Add (lListItem);
				}
			}
		}

		private void ShowAnimationPreview ()
		{
			NumericFrameDuration.Validated -= new System.EventHandler (NumericFrameDuration_Validated);
			if (mCharacterFile == null)
			{
				NumericFrameDuration.ResetText ();
				NumericFrameDuration.Enabled = false;
			}
			else
			{
				NumericFrameDuration.Value = mCharacterFile.NewFrameDuration;
				NumericFrameDuration.Enabled = !Program.FileIsReadOnly;
			}
			NumericFrameDuration.Validated += new System.EventHandler (NumericFrameDuration_Validated);

			if (IsEmpty)
			{
				ListViewPreview.Clear ();
				ListViewPreview.Enabled = false;
			}
			else
			{
				CursorState	lCursorState = new CursorState (Program.MainForm);
				ImageList	lImageList = new ImageList ();
				int			lImageNdx = 0;
				int			lItemNdx = 0;
				int			lListHeight;

				lCursorState.ShowWait ();
				ListViewPreview.BeginUpdate ();
				ListViewPreview.Items.Clear ();

				lImageList.ColorDepth = ColorDepth.Depth32Bit;
				lImageList.ImageSize = new Size (Math.Min (mCharacterFile.Header.ImageSize.Width, 256), Math.Min (mCharacterFile.Header.ImageSize.Height, 256));
				lListHeight = lImageList.ImageSize.Height;

				foreach (FileAnimationFrame lFrame in mAnimation.Frames)
				{
					Bitmap			lImage = null;
					ListViewItem	lListItem;

					try
					{
						lImage = mCharacterFile.GetFrameBitmap (lFrame, true, Color.Transparent);
					}
					catch
					{
					}

					lListItem = new ListViewItem (String.Format ("{0} ({1:D})", Global.TitleFrame (lItemNdx), lFrame.Duration));
					lListItem.Tag = lFrame;
					if (lImage != null)
					{
						lImageList.Images.Add (lImage);
						lListItem.ImageIndex = lImageNdx++;
					}
					ListViewPreview.Items.Add (lListItem);
					//lListHeight = Math.Max (lListHeight, ListViewPreview.GetItemRect (lListItem.Index).Height);
					lItemNdx++;
				}
				ListViewPreview.LargeImageList = lImageList;
				lListHeight += (System.Windows.Forms.SystemInformation.HorizontalScrollBarHeight * 3);
				ListViewPreview.Height = Math.Max (lListHeight, ListViewPreview.MinimumSize.Height);

				ListViewPreview.Enabled = true;
				ListViewPreview.EndUpdate ();
				ShowSelectedFrame ();
				lCursorState.RestoreCursor ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowSelectedFrame ()
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int	lSelNdx = ListViewPreview.GetSelectedIndex (false);

				ButtonDelete.Enabled = (lSelNdx >= 0);
				ButtonMoveUp.Enabled = (lSelNdx > 0);
				ButtonMoveDown.Enabled = (lSelNdx >= 0) && (lSelNdx < ListViewPreview.Items.Count - 1);
			}
		}

		private FileAnimationFrame GetSelectedFrame (Boolean pIncludeFocus)
		{
			return GetSelectedFrame (ListViewPreview.GetSelectedItem (pIncludeFocus));
		}

		private FileAnimationFrame GetSelectedFrame (ListViewItem pSelectedItem)
		{
			if (pSelectedItem != null)
			{
				return pSelectedItem.Tag as FileAnimationFrame;
			}
			return null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate DeleteSelectedFrame (FileAnimationFrame pFrame, Boolean pForClipboard)
		{
			if ((pFrame != null) && !Program.FileIsReadOnly)
			{
				AddDeleteAnimationFrame	lUpdate = new AddDeleteAnimationFrame (mCharacterFile, pFrame, pForClipboard);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				if (AddDeleteAnimationFrame.PutUndo (lUpdate.Apply () as AddDeleteAnimationFrame, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedFrame (FileAnimation pAnimation, FileAnimationFrame pFrame, FileAnimationFrame pPasteFrame)
		{
			if ((pAnimation != null) && (pPasteFrame != null) && !Program.FileIsReadOnly)
			{
				if (pFrame == null)
				{
					AddDeleteAnimationFrame	lUpdate = new AddDeleteAnimationFrame (mCharacterFile, pAnimation, pAnimation.FrameCount, true);

					lUpdate = lUpdate.Apply () as AddDeleteAnimationFrame;
					if (lUpdate != null)
					{
						pPasteFrame.CopyTo (lUpdate.Target, true);
						lUpdate = lUpdate.Apply () as AddDeleteAnimationFrame;
					}
					if (lUpdate != null)
					{
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
						if (AddDeleteAnimationFrame.PutUndo (lUpdate.Apply () as AddDeleteAnimationFrame, this))
						{
							return lUpdate;
						}
					}
				}
				else
				{
					UpdateAnimationFrame	lUpdate = new UpdateAnimationFrame (mCharacterFile, pFrame, true);

					pPasteFrame.CopyTo (pFrame, true);
					lUpdate = lUpdate.Apply () as UpdateAnimationFrame;
					if (lUpdate != null)
					{
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
						if (!UpdateAnimationFrame.PutUndo (lUpdate.Apply () as UpdateAnimationFrame, this))
						{
							lUpdate = null;
						}
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
				UpdateCharacterHeader	lUpdateCharacter = sender as UpdateCharacterHeader;
				AddDeleteAnimationFrame	lAddDeleteFrame = sender as AddDeleteAnimationFrame;
				ReorderAnimationFrame	lReorderFrame = sender as ReorderAnimationFrame;
				UpdateAnimationFrame	lUpdateFrame = sender as UpdateAnimationFrame;
				AddDeleteFrameImage		lAddDeleteImage = sender as AddDeleteFrameImage;
				ReorderFrameImage		lReorderImage = sender as ReorderFrameImage;
				UpdateFrameImage		lUpdateImage = sender as UpdateFrameImage;

				if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == mAnimation))
				{
					if (lUpdateAnimation.NameChanged)
					{
						ShowAnimationName ();
					}
					else
					{
						ShowReturnAnimation ();
					}
				}
				else if ((lUpdateCharacter != null) && (lUpdateCharacter.CharacterFile == mCharacterFile))
				{
					NumericFrameDuration.Value = mCharacterFile.NewFrameDuration;
				}
				else if ((lAddDeleteFrame != null) && (lAddDeleteFrame.Animation == mAnimation))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = lAddDeleteFrame.FrameNdx;
				}
				else if ((lReorderFrame != null) && (lReorderFrame.Animation == mAnimation))
				{
					ShowAnimationPreview ();
					ListViewPreview.SelectedIndex = mAnimation.Frames.IndexOf (lReorderFrame.Target);
				}
				else if ((lUpdateFrame != null) && (lUpdateFrame.Animation == mAnimation))
				{
					if (lUpdateFrame.DurationChanged)
					{
						ShowAnimationPreview ();
					}
				}
				else if ((lAddDeleteImage != null) && (lAddDeleteImage.Animation == mAnimation))
				{
					ShowAnimationPreview ();
				}
				else if ((lReorderImage != null) && (lReorderImage.Animation == mAnimation))
				{
					ShowAnimationPreview ();
				}
				else if ((lUpdateImage != null) && (lUpdateImage.Animation == mAnimation))
				{
					ShowAnimationPreview ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if (TextBoxName.Modified && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!String.IsNullOrEmpty (TextBoxName.Text))
				{
					UpdateAnimation	lUpdate = new UpdateAnimation (mCharacterFile, mAnimation, false);

					lUpdate.Name = TextBoxName.Text;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
					UpdateAnimation.PutUndo (lUpdate.Apply () as UpdateAnimation, this);
				}
				ShowAnimationName ();
			}
			TextBoxName.Modified = false;
		}

		private void ComboBoxReturn_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimation	lUpdate = new UpdateAnimation (mCharacterFile, mAnimation, false);

				if (ComboBoxReturn.SelectedIndex == mReturnItemNone)
				{
					lUpdate.ReturnType = 2;
					lUpdate.ReturnName = String.Empty;
				}
				else if (ComboBoxReturn.SelectedIndex == mReturnItemExit)
				{
					lUpdate.ReturnType = 1;
					lUpdate.ReturnName = String.Empty;
				}
				else if (ComboBoxReturn.SelectedIndex >= 0)
				{
					lUpdate.ReturnType = 0;
					lUpdate.ReturnName = ComboBoxReturn.SelectedItem.ToString ();
				}

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				UpdateAnimation.PutUndo (lUpdate.Apply () as UpdateAnimation, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewPreview_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				ShowSelectedFrame ();
			}
		}

		private void ListViewPreview_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToFrame != null))
			{
				FileAnimationFrame	lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					try
					{
						GoToFrame (this, new Global.AnimationFrameEventArgs (mAnimation, lFrame));
					}
					catch
					{
					}
				}
			}
		}

		private void ListViewStates_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToState != null))
			{
				ListViewItem	lItem = ListViewStates.SelectedItem;

				if (lItem != null)
				{
					try
					{
						GoToState (this, new Global.StateEventArgs (lItem.Text));
					}
					catch
					{
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameDuration_Validated (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader	lUpdate = new UpdateCharacterHeader (mCharacterFile);

				lUpdate.NewFrameDuration = (UInt16)NumericFrameDuration.Value;
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply () as UpdateCharacterHeader, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int						lSelNdx = ListViewPreview.GetSelectedIndex (false);
				AddDeleteAnimationFrame	lUpdate = new AddDeleteAnimationFrame (mCharacterFile, mAnimation, (lSelNdx >= 0) ? lSelNdx + 1 : mAnimation.FrameCount, false);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				AddDeleteAnimationFrame.PutUndo (lUpdate.Apply () as AddDeleteAnimationFrame, this);
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				FileAnimationFrame	lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					DeleteSelectedFrame (lFrame, false);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int						lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame		lFrame = GetSelectedFrame (false);
				ReorderAnimationFrame	lUpdate;

				if ((lFrame != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderAnimationFrame (mCharacterFile, mAnimation, lFrame, lSelNdx - 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
					ReorderAnimationFrame.PutUndo (lUpdate.Apply () as ReorderAnimationFrame, this);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int						lSelNdx = ListViewPreview.GetSelectedIndex (false);
				FileAnimationFrame		lFrame = GetSelectedFrame (false);
				ReorderAnimationFrame	lUpdate;

				if ((lFrame != null) && (lSelNdx >= 0) && (lSelNdx < mAnimation.Frames.Count - 1))
				{
					lUpdate = new ReorderAnimationFrame (mCharacterFile, mAnimation, lFrame, lSelNdx + 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
					ReorderAnimationFrame.PutUndo (lUpdate.Apply () as ReorderAnimationFrame, this);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewPreview.ContainsFocus)
			{
				FileAnimationFrame		lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					e.CopyObjectTitle = Global.QuotedTitle (Global.TitleFrameAnimation (lFrame));
					if (!Program.FileIsReadOnly)
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				if (!Program.FileIsReadOnly && (e.PasteObject is FileAnimationFrame))
				{
					if (lFrame == null)
					{
						e.PasteObjectTitle = e.PasteTypeTitle (lFrame, Properties.Resources.EditPasteFrameSource);
					}
					else
					{
						e.PasteObjectTitle = e.PasteTypeTitle (lFrame, Global.TitleFrameAnimation (lFrame), Properties.Resources.EditPasteFrameSource);
					}
				}
			}
		}

		internal void MainForm_EditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewPreview.ContainsFocus)
			{
				FileAnimationFrame		lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrame);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewPreview.ContainsFocus)
			{
				FileAnimationFrame		lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrame);
						DeleteSelectedFrame (lFrame, true);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewPreview.ContainsFocus)
			{
				FileAnimationFrame		lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					e.IsUsed = true;
					DeleteSelectedFrame (lFrame, false);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewPreview.ContainsFocus)
			{
				if (e.PasteObject is FileAnimationFrame)
				{
					FileAnimationFrame	lFrame = GetSelectedFrame (false);

					e.IsUsed = true;
					PasteSelectedFrame (mAnimation, lFrame, e.PasteObject as FileAnimationFrame);
					if (lFrame == null)
					{
						ListViewPreview.SelectedItem = null;
					}
				}
			}
		}

		#endregion
	}
}
