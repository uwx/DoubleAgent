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
using System.Windows.Forms;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class AnimationPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private int mReturnItemNone = -1;
		private int mReturnItemExit = -1;
		private List<ListViewItem> mStateItems;

		public AnimationPanel ()
		{
			InitializeComponent ();

			FramesView.Frames.ItemActivate += new System.EventHandler (FramesView_ItemActivate);
			FramesView.Frames.SelectedIndexChanged += new System.EventHandler (FramesView_SelectedIndexChanged);

			mStateItems = new List<ListViewItem> ();
			foreach (ListViewItem lListItem in ListViewStates.Items)
			{
				mStateItems.Add (lListItem);
			}
			ListViewStates.Items.Clear ();
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
				Animation = (FilePart is ResolveAnimation) ? (FilePart as ResolveAnimation).Target : null;

				ShowAnimationName ();
				ShowReturnAnimations ();
				ShowReturnAnimation ();
				ShowAnimationStates ();
				ShowAnimationPreview ();
			}
		}

		protected FileAnimation Animation
		{
			get;
			set;
		}

		protected override Boolean IsEmpty
		{
			get
			{
				return base.IsEmpty || (Animation == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event Global.NavigationEventHandler Navigate;

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
				TextBoxName.Text = Animation.Name;
				TextBoxName.Enabled = !Program.FileIsReadOnly;
				ToolStripFrames.Enabled = true;
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
				String[] lAnimations = CharacterFile.GetAnimationNames ();

				mReturnItemExit = ComboBoxReturn.Items.Add ("Use Exit Branching");
				foreach (String lAnimation in lAnimations)
				{
					if (String.Compare (lAnimation, Animation.Name, true) != 0)
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
			else if (Animation.ReturnType == 1)
			{
				ComboBoxReturn.SelectedIndex = mReturnItemExit;
			}
			else if ((Animation.ReturnType != 0) || (String.IsNullOrEmpty (Animation.ReturnName)))
			{
				ComboBoxReturn.SelectedIndex = mReturnItemNone;
			}
			else
			{
				ComboBoxReturn.SelectedIndex = ComboBoxReturn.FindStringExact (Animation.ReturnName);
				if (ComboBoxReturn.SelectedIndex < 0)
				{
					ComboBoxReturn.SelectedIndex = mReturnItemNone;
				}
			}
		}

		private void ShowAnimationStates ()
		{
			FileStates lFileStates = null;

			if (IsEmpty)
			{
				ListViewStates.Enabled = false;
			}
			else
			{
				ListViewStates.Enabled = true;//!Program.FileIsReadOnly;
				lFileStates = CharacterFile.States;
			}

			ListViewStates.Items.Clear ();

			foreach (ListViewItem lListItem in mStateItems)
			{
				String[] lStateAnimations = null;

				if (lFileStates != null)
				{
					if (lFileStates.Contains (lListItem.Text))
					{
						lStateAnimations = lFileStates[lListItem.Text].AnimationNames;
					}
					else if (lFileStates.Contains (lListItem.Text.ToUpper ()))
					{
						lStateAnimations = lFileStates[lListItem.Text.ToUpper ()].AnimationNames;
					}
				}
				if (
						(lStateAnimations != null)
					&& (
							(Array.IndexOf (lStateAnimations, Animation.Name) >= 0)
						|| (Array.IndexOf (lStateAnimations, Animation.Name.ToUpper ()) >= 0)
						)
					)
				{
					ListViewStates.Items.Add (lListItem);
				}
			}
		}

		private void ShowAnimationPreview ()
		{
			CausesValidation = false;

			if (CharacterFile == null)
			{
				NumericFrameDuration.ResetText ();
				NumericFrameDuration.Enabled = false;
			}
			else
			{
				NumericFrameDuration.Value = CharacterFile.NewFrameDuration;
				NumericFrameDuration.Enabled = !Program.FileIsReadOnly;
			}

			if (IsEmpty)
			{
				FramesView.Clear ();
				FramesView.Enabled = false;
			}
			else
			{
				CursorState lCursorState = new CursorState (Program.MainForm);

				lCursorState.ShowWait ();

				FramesView.ShowAnimationFrames (CharacterFile, Animation);
				FramesView.Enabled = true;

				ShowPreviewScale ();
				ShowSelectedFrame ();
				lCursorState.RestoreCursor ();
			}
			CausesValidation = Visible;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowPreviewScale ()
		{
			if (IsEmpty)
			{
				ButtonViewSmall.Checked = false;
				ButtonViewMedium.Checked = false;
				ButtonViewLarge.Checked = false;
				ButtonViewSmall.Enabled = false;
				ButtonViewMedium.Enabled = false;
				ButtonViewLarge.Enabled = false;
			}
			else
			{
				ButtonViewSmall.Checked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Small);
				ButtonViewMedium.Checked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Medium);
				ButtonViewLarge.Checked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Large);
				ButtonViewSmall.Enabled = FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Small).EitherLT (FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Medium));
				ButtonViewMedium.Enabled = true;
				ButtonViewLarge.Enabled = FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Large).EitherGT (FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Medium));
			}
		}

		private void ShowSelectedFrame ()
		{
			ShowSelectionState (GetSelectedFrame (false), FramesView.Frames.GetSelectedIndex (false));
		}

		private void ShowSelectionState (FileAnimationFrame pFrame, int pFrameNdx)
		{
			if ((pFrame == null) || IsEmpty)
			{
				ButtonAdd.Enabled = false;
				ButtonDelete.Enabled = false;
				ButtonMoveUp.Enabled = false;
				ButtonMoveDown.Enabled = false;
			}
			else
			{
				ButtonAdd.Enabled = !Program.FileIsReadOnly;
				ButtonDelete.Enabled = !Program.FileIsReadOnly && (pFrameNdx >= 0);
				ButtonMoveUp.Enabled = !Program.FileIsReadOnly && (pFrameNdx > 0);
				ButtonMoveDown.Enabled = !Program.FileIsReadOnly && (pFrameNdx >= 0) && (pFrameNdx < FramesView.Frames.Items.Count - 1);
			}

			ButtonDelete.Text = String.Format (Properties.Resources.EditDeleteThis.NoMenuPrefix (), Global.TitleFrame (ButtonDelete.Enabled ? pFrame : null));
			ButtonMoveUp.Text = String.Format (Properties.Resources.EditMoveFrameUp.NoMenuPrefix (), Global.TitleFrame (ButtonMoveUp.Enabled ? pFrame : null));
			ButtonMoveDown.Text = String.Format (Properties.Resources.EditMoveFrameDown.NoMenuPrefix (), Global.TitleFrame (ButtonMoveDown.Enabled ? pFrame : null));
		}

		///////////////////////////////////////////////////////////////////////////////

		private FileAnimationFrame GetSelectedFrame (Boolean pIncludeFocus)
		{
			return GetSelectedFrame (FramesView.Frames.GetSelectedIndex (pIncludeFocus));
		}
		private FileAnimationFrame GetSelectedFrame (ListViewItem pSelectedItem)
		{
			if (pSelectedItem != null)
			{
				return GetSelectedFrame (pSelectedItem.Index);
			}
			return null;
		}
		private FileAnimationFrame GetSelectedFrame (int pFrameNdx)
		{
			if ((Animation != null) && (pFrameNdx >= 0) && (pFrameNdx < Animation.FrameCount))
			{
				return Animation.Frames[pFrameNdx];
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
				AddDeleteAnimationFrame lUpdate = new AddDeleteAnimationFrame (pFrame, pForClipboard);
				if (AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteAnimationFrame, this))
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
					AddDeleteAnimationFrame lUpdate = new AddDeleteAnimationFrame (pAnimation, pAnimation.FrameCount, true);

					lUpdate = lUpdate.Apply () as AddDeleteAnimationFrame;
					if (lUpdate != null)
					{
						pPasteFrame.CopyTo (lUpdate.Target, true);
						lUpdate = lUpdate.Apply () as AddDeleteAnimationFrame;
					}
					if ((lUpdate != null) && AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteAnimationFrame, this))
					{
						return lUpdate;
					}
				}
				else
				{
					UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (pFrame, true);

					pPasteFrame.CopyTo (pFrame, true);
					lUpdate = lUpdate.Apply () as UpdateAnimationFrame;
					if ((lUpdate != null) && UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this))
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
			if (FramesView.Frames.ContainsFocus)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					pEventArgs.CopyObjectTitle = (pEventArgs is Global.ContextMenuEventArgs) ? Global.TitleFrame (lFrame) : Global.TitleFrameAnimation (lFrame).Quoted ();
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
						pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
					}
				}
				if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileAnimationFrame))
				{
					if (lFrame == null)
					{
						pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lFrame, Properties.Resources.EditPasteFrameSource);
					}
					else
					{
						pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lFrame, (pEventArgs is Global.ContextMenuEventArgs) ? Global.TitleFrame (lFrame) : Global.TitleFrameAnimation (lFrame).Quoted (), Properties.Resources.EditPasteFrameSource);
					}
				}
			}
		}

		protected override void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e);

			if (!IsEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, FramesView.Frames))
			{
				ToolStripMenuItem lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;

				e.ContextMenu.Items.Add (new ToolStripSeparator ());
				e.ContextMenu.Items.Add (lMenuItem = new ToolStripMenuItem (ButtonMoveUp.Text, ButtonMoveUp.Image, ButtonMoveUp_Click));
				lMenuItem.Enabled = ButtonMoveUp.Enabled;
				e.ContextMenu.Items.Add (lMenuItem = new ToolStripMenuItem (ButtonMoveDown.Text, ButtonMoveDown.Image, ButtonMoveDown_Click));
				lMenuItem.Enabled = ButtonMoveDown.Enabled;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool EditCopy (Global.EditEventArgs pEventArgs)
		{
			if (FramesView.Frames.ContainsFocus)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrame);
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
			if (FramesView.Frames.ContainsFocus)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrame);
						DeleteSelectedFrame (lFrame, true);
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
			if (FramesView.Frames.ContainsFocus)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					DeleteSelectedFrame (lFrame, false);
					return true;
				}
			}
			return false;
		}

		protected override bool EditPaste (Global.EditEventArgs pEventArgs)
		{
			if (FramesView.Frames.ContainsFocus)
			{
				if (pEventArgs.PasteObject is FileAnimationFrame)
				{
					FileAnimationFrame lFrame = GetSelectedFrame (false);

					PasteSelectedFrame (Animation, lFrame, pEventArgs.PasteObject as FileAnimationFrame);
					if (lFrame == null)
					{
						FramesView.Frames.SelectedItem = null;
					}
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (Object pUpdate)
		{
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			UpdateCharacterHeader lUpdateCharacter = pUpdate as UpdateCharacterHeader;
			AddDeleteAnimationFrame lAddDeleteFrame = pUpdate as AddDeleteAnimationFrame;
			ReorderAnimationFrame lReorderFrame = pUpdate as ReorderAnimationFrame;
			UpdateAnimationFrame lUpdateFrame = pUpdate as UpdateAnimationFrame;
			AddDeleteFrameImage lAddDeleteImage = pUpdate as AddDeleteFrameImage;
			ReorderFrameImage lReorderImage = pUpdate as ReorderFrameImage;
			UpdateFrameImage lUpdateImage = pUpdate as UpdateFrameImage;

			if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == Animation))
			{
				if ((lUpdateAnimation != null) && lUpdateAnimation.ForClipboard)
				{
					ShowAnimationName ();
					ShowReturnAnimation ();
					ShowAnimationPreview ();
				}
				else if (lUpdateAnimation.NameChanged)
				{
					ShowAnimationName ();
				}
				else
				{
					ShowReturnAnimation ();
				}
			}
			else if ((lUpdateCharacter != null) && (lUpdateCharacter.CharacterFile == CharacterFile))
			{
				NumericFrameDuration.Value = CharacterFile.NewFrameDuration;
			}
			else if ((lAddDeleteFrame != null) && (lAddDeleteFrame.Animation == Animation))
			{
				ShowAnimationPreview ();
				FramesView.Frames.SelectedIndex = lAddDeleteFrame.FrameNdx;
			}
			else if ((lReorderFrame != null) && (lReorderFrame.Animation == Animation))
			{
				ShowAnimationPreview ();
				FramesView.Frames.SelectedIndex = Animation.Frames.IndexOf (lReorderFrame.Target);
			}
			else if ((lUpdateFrame != null) && (lUpdateFrame.Animation == Animation))
			{
				if (lUpdateFrame.DurationChanged || lUpdateFrame.BranchingChanged)
				{
					ShowAnimationPreview ();
				}
			}
			else if ((lAddDeleteImage != null) && (lAddDeleteImage.Animation == Animation))
			{
				ShowAnimationPreview ();
			}
			else if ((lReorderImage != null) && (lReorderImage.Animation == Animation))
			{
				ShowAnimationPreview ();
			}
			else if ((lUpdateImage != null) && (lUpdateImage.Animation == Animation))
			{
				ShowAnimationPreview ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && TextBoxName.Modified && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!String.IsNullOrEmpty (TextBoxName.Text))
				{
					UpdateAnimation lUpdate = new UpdateAnimation (Animation, false);

					lUpdate.Name = TextBoxName.Text;
					UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimation, this);
				}
				ShowAnimationName ();
			}
			TextBoxName.Modified = false;
		}

		private void ComboBoxReturn_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimation lUpdate = new UpdateAnimation (Animation, false);

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

				UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimation, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void FramesView_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				ShowSelectedFrame ();
			}
		}

		private void FramesView_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (Navigate != null))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					try
					{
						Navigate (this, new Global.NavigationEventArgs (new ResolveAnimationFrame (lFrame)));
					}
					catch
					{
					}
				}
			}
		}

		private void ListViewStates_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (Navigate != null))
			{
				ListViewItem lItem = ListViewStates.SelectedItem;

				if (lItem != null)
				{
					try
					{
						Navigate (this, new Global.NavigationEventArgs (new ResolveState (lItem.Text)));
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
			if (CausesValidation && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.NewFrameDuration = (UInt16)NumericFrameDuration.Value;
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = FramesView.Frames.GetSelectedIndex (false);
				AddDeleteAnimationFrame lUpdate = new AddDeleteAnimationFrame (Animation, (lSelNdx >= 0) ? lSelNdx + 1 : Animation.FrameCount, false);

				AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteAnimationFrame, this);
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

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
				int lSelNdx = FramesView.Frames.GetSelectedIndex (false);
				FileAnimationFrame lFrame = GetSelectedFrame (false);
				ReorderAnimationFrame lUpdate;

				if ((lFrame != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderAnimationFrame (lFrame, lSelNdx - 1);
					ReorderAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as ReorderAnimationFrame, this);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = FramesView.Frames.GetSelectedIndex (false);
				FileAnimationFrame lFrame = GetSelectedFrame (false);
				ReorderAnimationFrame lUpdate;

				if ((lFrame != null) && (lSelNdx >= 0) && (lSelNdx < Animation.Frames.Count - 1))
				{
					lUpdate = new ReorderAnimationFrame (lFrame, lSelNdx + 1);
					ReorderAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as ReorderAnimationFrame, this);
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////

		private void ButtonViewSmall_Click (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Small);
				ShowPreviewScale ();
			}
		}

		private void ButtonViewMedium_Click (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Medium);
				ShowPreviewScale ();
			}
		}

		private void ButtonViewLarge_Click (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Large);
				ShowPreviewScale ();
			}
		}

		#endregion
	}
}
