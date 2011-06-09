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

		public AnimationPanel ()
		{
			InitializeComponent ();
			InitAnimationStates ();

			if (Program.MainWindow != null)
			{
				Program.MainWindow.InitializeContextMenu (FramesView.ContextMenuFrames);
			}

			FramesView.Frames.ItemActivate += new System.EventHandler (FramesView_ItemActivate);
			FramesView.Frames.SelectedIndexChanged += new System.EventHandler (FramesView_SelectedIndexChanged);
			FramesView.ContextMenuFrames.Opening += new System.ComponentModel.CancelEventHandler (ContextMenuFrames_Opening);
			FramesView.MenuItemAdd.Click += new EventHandler (ButtonAdd_Click);
			FramesView.MenuItemMoveUp.Click += new EventHandler (ButtonMoveUp_Click);
			FramesView.MenuItemMoveDown.Click += new EventHandler (ButtonMoveDown_Click);

			AnimationPreview.AnimationStateChanged += new EventHandler (AnimationPreview_StateChanged);
			AnimationPreview.AnimationImageChanged += new EventHandler (AnimationPreview_ImageChanged);

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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

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

		private void ShowReturnAnimations ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
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
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowAnimationFrames ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					FramesView.Clear ();
					FramesView.Enabled = false;
				}
				else
				{
					using (CursorState lCursorState = new CursorState (Program.MainWindow))
					{
						lCursorState.ShowWait ();

						FramesView.ShowAnimationFrames (CharacterFile, Animation);
						FramesView.Enabled = true;

						ShowFramesState ();
						ShowSelectedFrame ();
					}
				}

				AnimationPreview.DeleteAnimation ();
			}
		}

		private void ShowSelectedFrame ()
		{
			FramesView.Frames.EnsureVisible (FramesView.Frames.SelectedOrFocusedIndex);

			ShowSelectionState ();

			AnimationPreview.StopAnimation ();
			AnimationPreview.ShowAnimationFrame (CharacterFile, GetSelectedFrame (true));
			ShowPreviewState ();
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
		#region Event Handlers

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if (TextBoxName.IsModified)
			{
				HandleAnimationNameChanged ();
			}
			TextBoxName.IsModified = false;
		}

		private void ComboBoxReturn_SelectionChangeCommitted (object sender, EventArgs e)
		{
			HandleReturnAnimationChanged ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void FramesView_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && !IsPanelFilling)
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

		void ContextMenuFrames_Opening (object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (IsPanelEmpty || Program.FileIsReadOnly)
			{
				e.Cancel = true;
			}
			else
			{
				Program.MainWindow.ShowEditState (FramesView.ContextMenuFrames);

				FramesView.MenuItemAdd.Enabled = CanAddFrame;
				FramesView.MenuItemMoveUp.Enabled = CanMoveFrameUp;
				FramesView.MenuItemMoveDown.Enabled = CanMoveFrameDown;

				FramesView.MenuItemMoveUp.SetTitle (MoveFrameUpTitle);
				FramesView.MenuItemMoveDown.SetTitle (MoveFrameDownTitle);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewStates_ItemActivate (object sender, EventArgs e)
		{
			if (!IsPanelEmpty && (Navigate != null))
			{
				ListViewItemCommon lItem = ListViewStates.SelectedItem as ListViewItemCommon;

				if (lItem != null)
				{
					try
					{
						Navigate (this, new NavigationEventArgs (new ResolveState (lItem.Tag as String)));
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
			HandleDefaultDurationChanged ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			HandleAddFrame ();
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			HandleDeleteFrame ();
		}

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			HandleMoveFrameUp ();
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			HandleMoveFrameDown ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShowBranching_Click (object sender, EventArgs e)
		{
			HandleShowBranching ();
		}

		private void ButtonShowExitBranching_Click (object sender, EventArgs e)
		{
			HandleShowExitBranching ();
		}

		private void ButtonViewSmall_Click (object sender, EventArgs e)
		{
			HandleViewSmall ();
		}

		private void ButtonViewMedium_Click (object sender, EventArgs e)
		{
			HandleViewMedium ();
		}

		private void ButtonViewLarge_Click (object sender, EventArgs e)
		{
			HandleViewLarge ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewButtonPlay_Click (object sender, EventArgs e)
		{
			if (!AnimationPreview.IsAnimated)
			{
				using (CursorState lCursorState = new CursorState (Program.MainWindow))
				{
					lCursorState.ShowWait ();
					AnimationPreview.CreateAnimation (CharacterFile, Animation, !PreviewButtonMute.Checked);
				}
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
