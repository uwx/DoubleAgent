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
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Previews;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class AnimationPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private int mReturnItemNone = -1;
		private int mReturnItemExit = -1;
		private List<ListViewItemCommon> mStateItems;

		protected override void HandleVisibleChanged ()
		{
			base.HandleVisibleChanged ();

			if (IsPanelVisible)
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
				ShowDefaultFrameDuration ();
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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowAnimationName ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					TextBoxName.Clear ();
					TextBoxName.IsEnabled = false;
				}
				else
				{
					TextBoxName.Text = Animation.Name;
					TextBoxName.IsEnabled = !Program.FileIsReadOnly;
				}
				TextBoxName.IsModified = false;
			}
		}

		private void ShowReturnAnimation ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
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
		}

		private void ShowDefaultFrameDuration ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (CharacterFile == null)
				{
					NumericFrameDuration.Clear ();
					NumericFrameDuration.IsEnabled = false;
				}
				else
				{
					NumericFrameDuration.Value = CharacterFile.NewFrameDuration;
					NumericFrameDuration.IsEnabled = !Program.FileIsReadOnly;
				}
				NumericFrameDuration.IsModified = false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void InitAnimationStates ()
		{
			mStateItems = new List<ListViewItemCommon> ();

			mStateItems.Add (new ListViewItemCommon ("GesturingDown", "GesturingDown"));
			mStateItems.Add (new ListViewItemCommon ("GesturingLeft", "GesturingLeft"));
			mStateItems.Add (new ListViewItemCommon ("GesturingRight", "GesturingRight"));
			mStateItems.Add (new ListViewItemCommon ("GesturingUp", "GesturingUp"));
			mStateItems.Add (new ListViewItemCommon ("Listening", "Listening"));
			mStateItems.Add (new ListViewItemCommon ("Hearing", "Hearing"));
			mStateItems.Add (new ListViewItemCommon ("Hiding", "Hiding"));
			mStateItems.Add (new ListViewItemCommon ("IdlingLevel1", "IdlingLevel1"));
			mStateItems.Add (new ListViewItemCommon ("IdlingLevel2", "IdlingLevel2"));
			mStateItems.Add (new ListViewItemCommon ("IdlingLevel3", "IdlingLevel3"));
			mStateItems.Add (new ListViewItemCommon ("MovingDown", "MovingDown"));
			mStateItems.Add (new ListViewItemCommon ("MovingLeft", "MovingLeft"));
			mStateItems.Add (new ListViewItemCommon ("MovingRight", "MovingRight"));
			mStateItems.Add (new ListViewItemCommon ("MovingUp", "MovingUp"));
			mStateItems.Add (new ListViewItemCommon ("Showing", "Showing"));
			mStateItems.Add (new ListViewItemCommon ("Speaking", "Speaking"));
		}

		private void ShowAnimationStates ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				FileStates lFileStates = null;

				if (IsPanelEmpty)
				{
					ListViewStates.IsEnabled = false;
				}
				else
				{
					ListViewStates.IsEnabled = true;//!Program.FileIsReadOnly;
					lFileStates = CharacterFile.States;
				}

				ListViewStates.Items.Clear ();

				foreach (ListViewItemCommon lListItem in mStateItems)
				{
					String[] lStateAnimations = null;

					if (lFileStates != null)
					{
						if (lFileStates.Contains (lListItem.Tag as String))
						{
							lStateAnimations = lFileStates[lListItem.Tag as String].AnimationNames;
						}
						else if (lFileStates.Contains ((lListItem.Tag as String).ToUpper ()))
						{
							lStateAnimations = lFileStates[(lListItem.Tag as String).ToUpper ()].AnimationNames;
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
		}

		///////////////////////////////////////////////////////////////////////////////

		private FileAnimationFrame GetSelectedFrame (Boolean pIncludeFocus)
		{
			if (pIncludeFocus)
			{
				return GetSelectedFrame (Math.Min (Math.Max (FramesView.Frames.GetSelectedIndex (true), 0), FramesView.Frames.Items.Count - 1));
			}
			else
			{
				return GetSelectedFrame (FramesView.Frames.SelectedIndex);
			}
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
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					FramesView.Frames.SelectedIndex = mLastSelectedFrameNdx;
				}
			}
			mLastSelectedFrameNdx = -1;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFramesState ()
		{
			if (IsPanelEmpty)
			{
				ButtonShowBranching.IsChecked = true;
				ButtonShowExitBranching.IsChecked = true;
				ButtonShowBranching.IsEnabled = false;
				ButtonShowExitBranching.IsEnabled = false;

				ButtonViewSmall.IsChecked = false;
				ButtonViewMedium.IsChecked = false;
				ButtonViewLarge.IsChecked = false;
				ButtonViewSmall.IsEnabled = false;
				ButtonViewMedium.IsEnabled = false;
				ButtonViewLarge.IsEnabled = false;
			}
			else
			{
				ButtonShowBranching.IsChecked = FramesView.ShowBranching;
				ButtonShowExitBranching.IsChecked = FramesView.ShowExitBranching;
				ButtonShowBranching.IsEnabled = true;
				ButtonShowExitBranching.IsEnabled = true;

				ButtonViewSmall.IsChecked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Small);
				ButtonViewMedium.IsChecked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Medium);
				ButtonViewLarge.IsChecked = (FramesView.ImageScale == FramesPreview.ImageScaleType.Large);
				ButtonViewSmall.IsEnabled = FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Small).EitherLT (FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Medium));
				ButtonViewMedium.IsEnabled = true;
				ButtonViewLarge.IsEnabled = FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Large).EitherGT (FramesView.GetImageScaleSize (FramesPreview.ImageScaleType.Medium));
			}
		}

		private void ShowSelectionState ()
		{
			ButtonAdd.IsEnabled = CanAddFrame;
			ButtonDelete.IsEnabled = CanDeleteFrame;
			ButtonMoveUp.IsEnabled = CanMoveFrameUp;
			ButtonMoveDown.IsEnabled = CanMoveFrameDown;

			ButtonDelete.SetTipText (DeleteFrameTitle);
			ButtonMoveUp.SetTipText (MoveFrameUpTitle);
			ButtonMoveDown.SetTipText (MoveFrameDownTitle);
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean CanAddFrame
		{
			get
			{
				return !IsPanelEmpty && !Program.FileIsReadOnly;
			}
		}

		private Boolean CanDeleteFrame
		{
			get
			{
				return !IsPanelEmpty && !Program.FileIsReadOnly && (GetSelectedFrame (false) != null);
			}
		}
		private String DeleteFrameTitle
		{
			get
			{
				FileAnimationFrame lFrame = CanDeleteFrame ? GetSelectedFrame (false) : null;
				return String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Frame (lFrame));
			}
		}

		private Boolean CanMoveFrameUp
		{
			get
			{
				int lSelectedIndex = FramesView.Frames.SelectedIndex;
				return !IsPanelEmpty && !Program.FileIsReadOnly && (lSelectedIndex > 0) && (lSelectedIndex < FramesView.Frames.Items.Count);
			}
		}
		private String MoveFrameUpTitle
		{
			get
			{
				FileAnimationFrame lFrame = CanDeleteFrame ? GetSelectedFrame (false) : null;
				return String.Format (AppResources.Resources.EditMoveFrameUp.NoMenuPrefix (), Titles.Frame (lFrame));
			}
		}

		private Boolean CanMoveFrameDown
		{
			get
			{
				int lSelectedIndex = FramesView.Frames.SelectedIndex;
				return !IsPanelEmpty && !Program.FileIsReadOnly && (lSelectedIndex >= 0) && (lSelectedIndex < FramesView.Frames.Items.Count - 1);
			}
		}
		private String MoveFrameDownTitle
		{
			get
			{
				FileAnimationFrame lFrame = CanDeleteFrame ? GetSelectedFrame (false) : null;
				return String.Format (AppResources.Resources.EditMoveFrameDown.NoMenuPrefix (), Titles.Frame (lFrame));
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleShowBranching ()
		{
			if (!IsPanelFilling && !IsPanelEmpty)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					FramesView.ShowBranching = !FramesView.ShowBranching;
					FramesView.BuildBranchingGraphs ();
					FramesView.RecalcLayout ();
					ShowFramesState ();
				}
			}
		}

		private void HandleShowExitBranching ()
		{
			if (!IsPanelFilling && !IsPanelEmpty)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					FramesView.ShowExitBranching = !FramesView.ShowExitBranching;
					FramesView.BuildBranchingGraphs ();
					FramesView.RecalcLayout ();
					ShowFramesState ();
				}
			}
		}

		private void HandleViewSmall ()
		{
			if (!IsPanelFilling && !IsPanelEmpty)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					FramesView.RecalcLayout (FramesPreview.ImageScaleType.Small);
					ShowFramesState ();
				}
			}
		}

		private void HandleViewMedium ()
		{
			if (!IsPanelFilling && !IsPanelEmpty)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					FramesView.RecalcLayout (FramesPreview.ImageScaleType.Medium);
					ShowFramesState ();
				}
			}
		}

		private void HandleViewLarge ()
		{
			if (!IsPanelFilling && !IsPanelEmpty)
			{
				FramesView.RecalcLayout (FramesPreview.ImageScaleType.Large);
				ShowFramesState ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowPreviewState ()
		{
			PreviewButtonPlay.IsEnabled = !IsPanelEmpty && (Animation.Frames.Count > 1);
			PreviewButtonPause.IsEnabled = !IsPanelEmpty && AnimationPreview.IsPlaying;
			PreviewButtonStop.IsEnabled = !IsPanelEmpty && AnimationPreview.IsPlaying;
			PreviewButtonRepeat.IsEnabled = PreviewButtonPlay.IsEnabled && !AnimationPreview.IsPlaying;
			PreviewButtonMute.IsEnabled = PreviewButtonPlay.IsEnabled && !AnimationPreview.IsPlaying;

			if (AnimationPreview.IsAnimated)
			{
				PreviewButtonPause.IsChecked = PreviewButtonPause.IsEnabled && AnimationPreview.IsPaused;
				PreviewButtonRepeat.IsChecked = PreviewButtonPlay.IsEnabled && AnimationPreview.IsRepeating;
			}
			else
			{
				PreviewButtonPause.IsChecked = false;
			}
			ShowPreviewSkipState ();
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("PreviewState Animating [{0}] Playing [{1}] Paused [{2}] Repeating [{3}] Rate [{4}]]", AnimationPreview.IsAnimated, AnimationPreview.IsPlaying, AnimationPreview.IsPaused, AnimationPreview.IsRepeating, AnimationPreview.AnimationRate);
			System.Diagnostics.Debug.Print ("             Play [{0}] Pause [{1} {2}] Stop [{3}] Repeat [{4} {5}] Back [{6}] Forward [{7}]", PreviewButtonPlay.IsEnabled, PreviewButtonPause.IsEnabled, PreviewButtonPause.IsChecked, PreviewButtonStop.IsEnabled, PreviewButtonRepeat.IsEnabled, PreviewButtonRepeat.IsChecked, PreviewButtonSkipBack.IsEnabled, PreviewButtonSkipForward.IsEnabled);
#endif
		}

		private void ShowPreviewSkipState ()
		{
			if (AnimationPreview.IsPlaying)
			{
				ShowPreviewSkipState (-1);
			}
			else
			{
				ShowPreviewSkipState (FramesView.Frames.SelectedIndex);
			}
		}

		private void ShowPreviewSkipState (int pFrameIndex)
		{
			PreviewButtonSkipBack.IsEnabled = PreviewButtonPlay.IsEnabled && (pFrameIndex > 0);
			PreviewButtonSkipForward.IsEnabled = PreviewButtonPlay.IsEnabled && (pFrameIndex >= 0) && (pFrameIndex < FramesView.Frames.Items.Count - 1);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandlePreviewPlay ()
		{
			if (AnimationPreview.IsAnimated)
			{
				AnimationPreview.StopAnimation ();
				AnimationPreview.IsPaused = false;
				AnimationPreview.IsRepeating = PreviewButtonRepeat.IsChecked.Value;
				AnimationPreview.AnimationRate = (double)SliderRate.Value;
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StartAnimation Repeat [{0}] Rate [{1}]", AnimationPreview.AnimationIsRepeating, AnimationPreview.AnimationRate);		
#endif
				PushSelectedFrame ();
				AnimationPreview.StartAnimation ();
			}
			ShowPreviewState ();
		}

		private void HandlePreviewPause ()
		{
			if (AnimationPreview.IsPlaying)
			{
				AnimationPreview.IsPaused = PreviewButtonPause.IsChecked.Value;
				ShowPreviewSkipState ();
			}
		}

		private void HandlePreviewStop ()
		{
			AnimationPreview.StopAnimation ();
			ShowPreviewState ();
		}

		private void HandlePreviewSkipBack ()
		{
			if (AnimationPreview.IsPlaying)
			{
				//AnimationPreviewHost.CurrentFrameIndex--;
			}
			else if (!IsPanelEmpty)
			{
				if (FramesView.Frames.SelectedIndex > 0)
				{
					FramesView.Frames.SelectedIndex--;
				}
			}
		}

		private void HandlePreviewSkipForward ()
		{
			if (AnimationPreview.IsPlaying)
			{
				//AnimationPreviewHost.CurrentFrameIndex++;
			}
			else if (!IsPanelEmpty)
			{
				if ((FramesView.Frames.SelectedIndex >= 0) && (FramesView.Frames.SelectedIndex < FramesView.Frames.Items.Count - 1))
				{
					FramesView.Frames.SelectedIndex++;
				}
			}
		}

		private void HandlePreviewRepeat ()
		{
			if (!AnimationPreview.IsPlaying)
			{
				AnimationPreview.IsRepeating = PreviewButtonRepeat.IsChecked.Value;
			}
		}

		private void HandlePreviewMute ()
		{
			if (!AnimationPreview.IsPlaying)
			{
				AnimationPreview.DeleteAnimation ();
			}
		}

		private void HandlePreviewRateChanged ()
		{
			if (AnimationPreview.IsAnimated)
			{
				AnimationPreview.AnimationRate = (double)SliderRate.Value;
			}
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

		protected override bool HandleCanEditCopy (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (FramesView.Frames, pEventArgs))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					String lObjectTitle = /*(pEventArgs is Global.ContextMenuEventArgs) ? Titles.Frame (lFrame) :*/ Titles.FrameAnimation (lFrame).Quoted ();
					pEventArgs.PutCopyTitle (lObjectTitle);
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCopy (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (FramesView.Frames, pEventArgs))
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

		protected override bool HandleCanEditCut (CanEditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && IsControlEditTarget (FramesView.Frames, pEventArgs))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					pEventArgs.PutCutTitle (Titles.FrameAnimation (lFrame).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && IsControlEditTarget (FramesView.Frames, pEventArgs))
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

		protected override bool HandleCanEditDelete (CanEditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && IsControlEditTarget (FramesView.Frames, pEventArgs))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					pEventArgs.PutDeleteTitle (Titles.FrameAnimation (lFrame).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && IsControlEditTarget (FramesView.Frames, pEventArgs))
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

		protected override bool HandleCanEditPaste (CanEditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && IsControlEditTarget (FramesView.Frames, pEventArgs) && (pEventArgs.GetPasteObject () is FileAnimationFrame))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);
				if (lFrame == null)
				{
					pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lFrame, AppResources.Resources.EditPasteFrameSource));
				}
				else
				{
					pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lFrame, /*(pEventArgs is Global.ContextMenuEventArgs) ? Titles.Frame (lFrame) :*/ Titles.FrameAnimation (lFrame).Quoted (), AppResources.Resources.EditPasteFrameSource));
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && IsControlEditTarget (FramesView.Frames, pEventArgs) && (pEventArgs.GetPasteObject () is FileAnimationFrame))
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				PasteSelectedFrame (Animation, lFrame, pEventArgs.GetPasteObject () as FileAnimationFrame);
				if (lFrame == null)
				{
					FramesView.Frames.SelectedItem = null;
				}
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleAnimationNameChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				if (!String.IsNullOrEmpty (TextBoxName.Text))
				{
					UpdateAnimation lUpdate = new UpdateAnimation (Animation, false);

					lUpdate.Name = TextBoxName.Text;
					UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimation, this);
				}
				ShowAnimationName ();
			}
			TextBoxName.IsModified = false;
		}

		private void HandleReturnAnimationChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
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

		private void HandleDefaultDurationChanged ()
		{
			if (!IsPanelFilling && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.NewFrameDuration = (UInt16)NumericFrameDuration.Value;
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
			NumericFrameDuration.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleAddFrame ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = FramesView.Frames.SelectedIndex;
				AddDeleteAnimationFrame lUpdate = new AddDeleteAnimationFrame (Animation, (lSelNdx >= 0) ? lSelNdx + 1 : Animation.FrameCount, false);

				AddDeleteAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimationFrame, this);
			}
		}

		private void HandleDeleteFrame ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileAnimationFrame lFrame = GetSelectedFrame (false);

				if (lFrame != null)
				{
					DeleteSelectedFrame (lFrame, false);
				}
			}
		}

		private void HandleMoveFrameUp ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
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

		private void HandleMoveFrameDown ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
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
				ShowDefaultFrameDuration ();
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
	}
}
