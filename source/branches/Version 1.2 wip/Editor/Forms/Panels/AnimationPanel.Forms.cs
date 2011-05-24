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
using System.Drawing;
using System.Windows.Forms;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Previews;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
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

			AnimationPreview.AnimationStateChanged += new EventHandler (AnimationPreview_StateChanged);
			AnimationPreview.AnimationImageChanged += new EventHandler (AnimationPreview_ImageChanged);

			InitAnimationStates ();
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				FramesView.ShowBranching = lSettings.FramesViewBranching;
				FramesView.ShowExitBranching = lSettings.FramesViewExitBranching;
				try
				{
					FramesView.RecalcLayout ((FramesPreview.ImageScaleType)lSettings.FramesViewScale);
				}
				catch
				{
				}
				SliderRate.Value = Math.Min (Math.Max (lSettings.AnimationPreviewRate, SliderRate.Minimum), SliderRate.Maximum);
				PreviewButtonRepeat.Checked = lSettings.AnimationPreviewRepeat;
				PreviewButtonMute.Checked = lSettings.AnimationPreviewMute;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings lSettings = Settings.Default;

			lSettings.FramesViewBranching = FramesView.ShowBranching;
			lSettings.FramesViewExitBranching = FramesView.ShowExitBranching;
			lSettings.FramesViewScale = (int)FramesView.ImageScale;
			lSettings.AnimationPreviewRate = SliderRate.Value;
			lSettings.AnimationPreviewRepeat = PreviewButtonRepeat.Checked;
			lSettings.AnimationPreviewMute = PreviewButtonMute.Checked;
		}

		protected override void OnVisibleChanged (object sender, EventArgs e)
		{
			base.OnVisibleChanged (sender, e);

			if (Visible)
			{
				ShowPreviewState ();
			}
			else
			{
				AnimationPreview.StopAnimation ();
			}
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
				ShowAnimationFrames ();
			}
		}

		protected FileAnimation Animation
		{
			get;
			set;
		}

		public override Boolean IsPanelEmpty
		{
			get
			{
				return base.IsPanelEmpty || (Animation == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event NavigationEventHandler Navigate;

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
			public PanelContext (AnimationPanel pPanel)
				: base (pPanel)
			{
				SelectedState = pPanel.ListViewStates.SelectedIndex;
				FocusedState = pPanel.ListViewStates.FocusedIndex;
				SelectedFrame = pPanel.FramesView.Frames.SelectedIndex;
				FocusedFrame = pPanel.FramesView.Frames.FocusedIndex;
			}

			public void RestoreContext (AnimationPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewStates.SelectedIndex = SelectedState;
				pPanel.ListViewStates.FocusedIndex = FocusedState;
				pPanel.FramesView.Frames.SelectedIndex = SelectedFrame;
				pPanel.FramesView.Frames.FocusedIndex = FocusedFrame;
			}

			public int SelectedState
			{
				get;
				protected set;
			}
			public int FocusedState
			{
				get;
				protected set;
			}
			public int SelectedFrame
			{
				get;
				protected set;
			}
			public int FocusedFrame
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowAnimationName ()
		{
			if (IsPanelEmpty)
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

			if (IsPanelEmpty)
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
			if (IsPanelEmpty)
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

		///////////////////////////////////////////////////////////////////////////////

		private void InitAnimationStates ()
		{
			mStateItems = new List<ListViewItem> ();
			foreach (ListViewItem lListItem in ListViewStates.Items)
			{
				mStateItems.Add (lListItem);
			}
			ListViewStates.Items.Clear ();
		}

		private void ShowAnimationStates ()
		{
			FileStates lFileStates = null;

			if (IsPanelEmpty)
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

		///////////////////////////////////////////////////////////////////////////////

		private void ShowAnimationFrames ()
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

			if (IsPanelEmpty)
			{
				FramesView.Clear ();
				FramesView.Enabled = false;
			}
			else
			{
				CursorState lCursorState = new CursorState (Program.MainWindow);
				lCursorState.ShowWait ();

				FramesView.ShowAnimationFrames (CharacterFile, Animation);
				FramesView.Enabled = true;

				ShowFramesState ();
				ShowSelectedFrame ();
				lCursorState.RestoreCursor ();
			}

			AnimationPreview.DeleteAnimation ();
			CausesValidation = Visible;
		}

		private void ShowSelectedFrame ()
		{
			FramesView.Frames.EnsureVisible (FramesView.Frames.SelectedOrFocusedIndex);

			ShowSelectionState (GetSelectedFrame (false), FramesView.Frames.SelectedIndex);

			AnimationPreview.StopAnimation ();
			AnimationPreview.ShowAnimationFrame (CharacterFile, GetSelectedFrame (true));
			ShowPreviewState ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFramesState ()
		{
			if (IsPanelEmpty)
			{
				ButtonShowBranching.Checked = true;
				ButtonShowExitBranching.Checked = true;
				ButtonShowBranching.Enabled = false;
				ButtonShowExitBranching.Enabled = false;

				ButtonViewSmall.Checked = false;
				ButtonViewMedium.Checked = false;
				ButtonViewLarge.Checked = false;
				ButtonViewSmall.Enabled = false;
				ButtonViewMedium.Enabled = false;
				ButtonViewLarge.Enabled = false;
			}
			else
			{
				ButtonShowBranching.Checked = FramesView.ShowBranching;
				ButtonShowExitBranching.Checked = FramesView.ShowExitBranching;
				ButtonShowBranching.Enabled = true;
				ButtonShowExitBranching.Enabled = true;

				ButtonViewSmall.Checked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Small);
				ButtonViewMedium.Checked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Medium);
				ButtonViewLarge.Checked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Large);
				ButtonViewSmall.Enabled = FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Small).EitherLT (FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Medium));
				ButtonViewMedium.Enabled = true;
				ButtonViewLarge.Enabled = FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Large).EitherGT (FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Medium));
			}
		}

		private void ShowSelectionState (FileAnimationFrame pFrame, int pFrameNdx)
		{
			if ((pFrame == null) || IsPanelEmpty)
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

			ButtonDelete.Text = String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Frame (ButtonDelete.Enabled ? pFrame : null));
			ButtonMoveUp.Text = String.Format (AppResources.Resources.EditMoveFrameUp.NoMenuPrefix (), Titles.Frame (ButtonMoveUp.Enabled ? pFrame : null));
			ButtonMoveDown.Text = String.Format (AppResources.Resources.EditMoveFrameDown.NoMenuPrefix (), Titles.Frame (ButtonMoveDown.Enabled ? pFrame : null));
		}

		///////////////////////////////////////////////////////////////////////////////

		private FileAnimationFrame GetSelectedFrame (Boolean pIncludeFocus)
		{
			if (pIncludeFocus)
			{
				return GetSelectedFrame (Math.Min (Math.Max (FramesView.Frames.SelectedOrFocusedIndex, 0), FramesView.Frames.Items.Count - 1));
			}
			else
			{
				return GetSelectedFrame (FramesView.Frames.SelectedIndex);
			}
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

		///////////////////////////////////////////////////////////////////////////////

		private int mLastSelectedFrameNdx = -1;

		private void PushSelectedFrame ()
		{
			mLastSelectedFrameNdx = FramesView.Frames.SelectedIndex;
		}

		private void PopSelectedFrame ()
		{
			if (mLastSelectedFrameNdx >= 0)
			{
				CausesValidation = false;
				FramesView.Frames.SelectedIndex = mLastSelectedFrameNdx;
				CausesValidation = Visible;
			}
			mLastSelectedFrameNdx = -1;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowPreviewState ()
		{
			ShowPreviewState (false);
		}

		private void ShowPreviewState (Boolean pUpdateNow)
		{
			PreviewButtonPlay.Enabled = !IsPanelEmpty && (Animation.Frames.Count > 1);
			PreviewButtonPause.Enabled = !IsPanelEmpty && AnimationPreview.IsPlaying;
			PreviewButtonStop.Enabled = !IsPanelEmpty && AnimationPreview.IsPlaying;
			PreviewButtonRepeat.Enabled = PreviewButtonPlay.Enabled && !AnimationPreview.IsPlaying;
			PreviewButtonMute.Enabled = PreviewButtonPlay.Enabled && !AnimationPreview.IsPlaying;

			if (AnimationPreview.IsAnimated)
			{
				PreviewButtonPause.Checked = PreviewButtonPause.Enabled && AnimationPreview.IsPaused;
				PreviewButtonRepeat.Checked = PreviewButtonPlay.Enabled && AnimationPreview.IsRepeating;
			}
			else
			{
				PreviewButtonPause.Checked = false;
			}
			ShowPreviewSkipState ();

			if (pUpdateNow)
			{
				ToolStripPreview.Update ();
			}
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("PreviewState Animating [{0}] Playing [{1}] Paused [{2}] Repeating [{3}] Rate [{4}]]", AnimationPreview.IsAnimating, AnimationPreview.AnimationIsPlaying, AnimationPreview.AnimationIsPaused, AnimationPreview.AnimationIsRepeating, AnimationPreview.AnimationRate);
			System.Diagnostics.Debug.Print ("             Play [{0}] Pause [{1} {2}] Stop [{3}] Repeat [{4} {5}] Back [{6}] Forward [{7}]", PreviewButtonPlay.Enabled, PreviewButtonPause.Enabled, PreviewButtonPause.Checked, PreviewButtonStop.Enabled, PreviewButtonRepeat.Enabled, PreviewButtonRepeat.Checked, PreviewButtonSkipBack.Enabled, PreviewButtonSkipForward.Enabled);
#endif
		}

		private void ShowPreviewSkipState ()
		{
			if (AnimationPreview.IsPlaying)
			{
				//ShowPreviewSkipState (AnimationPreview.CurrentFrameIndex);
				ShowPreviewSkipState (-1);
			}
			else
			{
				ShowPreviewSkipState (FramesView.Frames.SelectedIndex);
			}
		}

		private void ShowPreviewSkipState (int pFrameIndex)
		{
			PreviewButtonSkipBack.Enabled = PreviewButtonPlay.Enabled && (pFrameIndex > 0);
			PreviewButtonSkipForward.Enabled = PreviewButtonPlay.Enabled && (pFrameIndex >= 0) && (pFrameIndex < FramesView.Frames.Items.Count - 1);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate DeleteSelectedFrame (FileAnimationFrame pFrame, Boolean pForClipboard)
		{
			if ((pFrame != null) && !Program.FileIsReadOnly)
			{
				AddDeleteAnimationFrame lUpdate = new AddDeleteAnimationFrame (pFrame, pForClipboard);
				if (AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimationFrame, this))
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
					if ((lUpdate != null) && AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimationFrame, this))
					{
						return lUpdate;
					}
				}
				else
				{
					UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (pFrame, true);

					pPasteFrame.CopyTo (pFrame, true);
					lUpdate = lUpdate.Apply () as UpdateAnimationFrame;
					if ((lUpdate != null) && UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this))
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
					String lObjectTitle = (pEventArgs is Global.ContextMenuEventArgs) ? Titles.Frame (lFrame) : Titles.FrameAnimation (lFrame).Quoted ();

					pEventArgs.PutCopyTitle (lObjectTitle);
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.PutCutTitle (lObjectTitle);
						pEventArgs.PutDeleteTitle (lObjectTitle);
					}
				}
				if (!Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileAnimationFrame))
				{
					if (lFrame == null)
					{
						pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lFrame, AppResources.Resources.EditPasteFrameSource);
					}
					else
					{
						pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lFrame, (pEventArgs is Global.ContextMenuEventArgs) ? Titles.Frame (lFrame) : Titles.FrameAnimation (lFrame).Quoted (), AppResources.Resources.EditPasteFrameSource);
					}
				}
			}
		}

		protected override void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e);

			if (!IsPanelEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, FramesView.Frames))
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

		protected override bool HandleEditCopy (Global.EditEventArgs pEventArgs)
		{
			if (FramesView.Frames.ContainsFocus)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					pEventArgs.PutCopyObject (lFrame);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (FramesView.Frames.ContainsFocus)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					if (pEventArgs.PutCopyObject (lFrame))
					{
						DeleteSelectedFrame (lFrame, true);
					}
					return true;
				}
			}
			return false;
		}

		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
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

		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (FramesView.Frames.ContainsFocus)
			{
				if (pEventArgs.GetPasteObject () is FileAnimationFrame)
				{
					FileAnimationFrame lFrame = GetSelectedFrame (false);

					PasteSelectedFrame (Animation, lFrame, pEventArgs.GetPasteObject () as FileAnimationFrame);
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
					ShowAnimationFrames ();
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
				ShowAnimationFrames ();
				FramesView.Frames.SelectedIndex = lAddDeleteFrame.FrameNdx;
			}
			else if ((lReorderFrame != null) && (lReorderFrame.Animation == Animation))
			{
				ShowAnimationFrames ();
				FramesView.Frames.SelectedIndex = Animation.Frames.IndexOf (lReorderFrame.Target);
			}
			else if ((lUpdateFrame != null) && (lUpdateFrame.Animation == Animation))
			{
				if (lUpdateFrame.DurationChanged || lUpdateFrame.BranchingChanged)
				{
					ShowAnimationFrames ();
				}
			}
			else if ((lAddDeleteImage != null) && (lAddDeleteImage.Animation == Animation))
			{
				ShowAnimationFrames ();
			}
			else if ((lReorderImage != null) && (lReorderImage.Animation == Animation))
			{
				ShowAnimationFrames ();
			}
			else if ((lUpdateImage != null) && (lUpdateImage.Animation == Animation))
			{
				ShowAnimationFrames ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && TextBoxName.Modified && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				if (!String.IsNullOrEmpty (TextBoxName.Text))
				{
					UpdateAnimation lUpdate = new UpdateAnimation (Animation, false);

					lUpdate.Name = TextBoxName.Text;
					UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimation, this);
				}
				ShowAnimationName ();
			}
			TextBoxName.Modified = false;
		}

		private void ComboBoxReturn_SelectionChangeCommitted (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
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

				UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimation, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void FramesView_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && CausesValidation)
			{
				ShowSelectedFrame ();
			}
		}

		private void FramesView_ItemActivate (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && (Navigate != null))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					try
					{
						Navigate (this, new NavigationEventArgs (new ResolveAnimationFrame (lFrame)));
					}
					catch
					{
					}
				}
			}
		}

		private void ListViewStates_ItemActivate (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && (Navigate != null))
			{
				ListViewItem lItem = ListViewStates.SelectedItem;

				if (lItem != null)
				{
					try
					{
						Navigate (this, new NavigationEventArgs (new ResolveState (lItem.Text)));
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
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = FramesView.Frames.SelectedIndex;
				AddDeleteAnimationFrame lUpdate = new AddDeleteAnimationFrame (Animation, (lSelNdx >= 0) ? lSelNdx + 1 : Animation.FrameCount, false);

				AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimationFrame, this);
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
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
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = FramesView.Frames.SelectedIndex;
				FileAnimationFrame lFrame = GetSelectedFrame (false);
				ReorderAnimationFrame lUpdate;

				if ((lFrame != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderAnimationFrame (lFrame, lSelNdx - 1);
					ReorderAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as ReorderAnimationFrame, this);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = FramesView.Frames.SelectedIndex;
				FileAnimationFrame lFrame = GetSelectedFrame (false);
				ReorderAnimationFrame lUpdate;

				if ((lFrame != null) && (lSelNdx >= 0) && (lSelNdx < Animation.Frames.Count - 1))
				{
					lUpdate = new ReorderAnimationFrame (lFrame, lSelNdx + 1);
					ReorderAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as ReorderAnimationFrame, this);
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShowBranching_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty)
			{
				FramesView.ShowBranching = !FramesView.ShowBranching;
				FramesView.ShowBranchingGraphs ();
				FramesView.RecalcLayout ();
				ShowFramesState ();
			}
		}

		private void ButtonShowExitBranching_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty)
			{
				FramesView.ShowExitBranching = !FramesView.ShowExitBranching;
				FramesView.ShowBranchingGraphs ();
				FramesView.RecalcLayout ();
				ShowFramesState ();
			}
		}

		private void ButtonViewSmall_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Small);
				ShowFramesState ();
			}
		}

		private void ButtonViewMedium_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Medium);
				ShowFramesState ();
			}
		}

		private void ButtonViewLarge_Click (object sender, EventArgs e)
		{
			if (!IsPanelEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Large);
				ShowFramesState ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewButtonPlay_Click (object sender, EventArgs e)
		{
			if (!AnimationPreview.IsAnimated)
			{
				CursorState lCursorState = new CursorState (Program.MainWindow);
				lCursorState.ShowWait ();

				AnimationPreview.CreateAnimation (CharacterFile, Animation, !PreviewButtonMute.Checked);

				lCursorState.RestoreCursor ();
			}

			if (AnimationPreview.IsAnimated)
			{
				AnimationPreview.StopAnimation ();
				AnimationPreview.IsPaused = false;
				AnimationPreview.IsRepeating = PreviewButtonRepeat.Checked;
				AnimationPreview.AnimationRate = (double)SliderRate.Value;
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StartAnimation Repeat [{0}] Rate [{1}]", AnimationPreview.AnimationIsRepeating, AnimationPreview.AnimationRate);		
#endif
				PushSelectedFrame ();
				AnimationPreview.StartAnimation ();
			}
			ShowPreviewState (true);
		}

		private void PreviewButtonStop_Click (object sender, EventArgs e)
		{
			AnimationPreview.StopAnimation ();
			ShowPreviewState (true);
		}

		private void PreviewButtonPause_Click (object sender, EventArgs e)
		{
			PreviewButtonPause.Checked = !PreviewButtonPause.Checked;
			if (AnimationPreview.IsPlaying)
			{
				AnimationPreview.IsPaused = PreviewButtonPause.Checked;
				ShowPreviewSkipState ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewButtonSkipBack_Click (object sender, EventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				//AnimationPreview.CurrentFrameIndex--;
			}
			else if (!IsPanelEmpty)
			{
				if (FramesView.Frames.SelectedIndex > 0)
				{
					FramesView.Frames.SelectedIndex--;
				}
			}
		}

		private void PreviewButtonSkipForward_Click (object sender, EventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				//AnimationPreview.CurrentFrameIndex++;
			}
			else if (!IsPanelEmpty)
			{
				if ((FramesView.Frames.SelectedIndex >= 0) && (FramesView.Frames.SelectedIndex < FramesView.Frames.Items.Count - 1))
				{
					FramesView.Frames.SelectedIndex++;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewButtonRepeat_Click (object sender, EventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			else
			{
				PreviewButtonRepeat.Checked = !PreviewButtonRepeat.Checked;
				AnimationPreview.IsRepeating = PreviewButtonRepeat.Checked;
			}
		}

		private void PreviewButtonMute_Click (object sender, EventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			else
			{
				PreviewButtonMute.Checked = !PreviewButtonMute.Checked;
				AnimationPreview.DeleteAnimation ();
			}
		}

		private void TrackBarRate_Scroll (object sender, EventArgs e)
		{
			if (AnimationPreview.IsAnimated)
			{
				AnimationPreview.AnimationRate = (double)SliderRate.Value;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		void AnimationPreview_StateChanged (object sender, EventArgs e)
		{
#if DEBUG_NOT
			try
			{
				System.Windows.Media.Animation.ClockGroup lClock = sender as System.Windows.Media.Animation.ClockGroup;
				System.Diagnostics.Debug.Print ("PreviewState [{0}] Time [{1}] Progress [{2}] Iteration [{3}]", lClock.CurrentState, lClock.CurrentTime, lClock.CurrentProgress, lClock.CurrentIteration);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
#endif
			try
			{
				if (AnimationPreview.IsPlaying)
				{
					ShowPreviewState (true);
				}
				else
				{
					PopSelectedFrame ();
					ShowSelectedFrame ();
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		void AnimationPreview_ImageChanged (object sender, EventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				try
				{
					int lFrameNdx = AnimationPreview.CurrentFrameIndex;
					if (lFrameNdx >= 0)
					{
						CausesValidation = false;
						FramesView.Frames.SelectedIndex = lFrameNdx;
						FramesView.Frames.EnsureVisible (lFrameNdx, true);
						CausesValidation = Visible;
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		#endregion
	}
}
