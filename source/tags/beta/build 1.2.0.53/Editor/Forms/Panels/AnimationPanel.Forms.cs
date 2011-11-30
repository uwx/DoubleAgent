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

			ToolBarFrames.ButtonAdd.ButtonClick += new EventHandler (ButtonAdd_Click);
			ToolBarFrames.ButtonDelete.ButtonClick += new EventHandler (ButtonDelete_Click);
			ToolBarFrames.ButtonMovePrev.ButtonClick += new EventHandler (ButtonMovePrev_Click);
			ToolBarFrames.ButtonMoveNext.ButtonClick += new EventHandler (ButtonMoveNext_Click);

			FramesView.Frames.ItemActivate += new System.EventHandler (FramesView_ItemActivate);
			FramesView.Frames.SelectedIndexChanged += new System.EventHandler (FramesView_SelectedIndexChanged);
			FramesView.ContextMenuFrames.Opening += new System.ComponentModel.CancelEventHandler (ContextMenuFrames_Opening);
			FramesView.MenuItemAdd.Click += new EventHandler (ButtonAdd_Click);
			FramesView.MenuItemMovePrev.Click += new EventHandler (ButtonMovePrev_Click);
			FramesView.MenuItemMoveNext.Click += new EventHandler (ButtonMoveNext_Click);

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

				ToolBarFrames.LoadConfig ();
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

			ToolBarFrames.SaveConfig ();
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
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

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
						if (lContext != null)
						{
							lContext.RestoreContext (this);
						}
						FramesView.Frames.SelectedIndex = Math.Max (FramesView.Frames.SelectedIndex, 0);

						ShowFramesState ();
						ShowSelectedFrame ();
					}
				}

				AnimationPreview.DeleteAnimation ();
			}
		}

		private void ShowFrameSelection ()
		{
			FramesView.Frames.EnsureVisible (FramesView.Frames.SelectedIndex);
		}

		private System.Drawing.Size PreviewImageSize
		{
			get
			{
				return (CharacterFile != null) ? CharacterFile.Header.ImageSize : FrameSample.DefaultImageSize;
			}
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

		//=============================================================================

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
				FileAnimationFrame lFrame = GetSelectedFrame ();

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

				FramesView.MenuItemAdd.Enabled = ToolBarFrames.CanAddFrame;
				FramesView.MenuItemMovePrev.Enabled = ToolBarFrames.CanMoveFramePrev;
				FramesView.MenuItemMoveNext.Enabled = ToolBarFrames.CanMoveFrameNext;

				FramesView.MenuItemMovePrev.SetTitle (ToolBarFrames.MoveFramePrevTitle);
				FramesView.MenuItemMoveNext.SetTitle (ToolBarFrames.MoveFrameNextTitle);
			}
		}

		//=============================================================================

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

		//=============================================================================

		private void NumericFrameDuration_Validated (object sender, EventArgs e)
		{
			HandleDefaultDurationChanged ();
		}

		//=============================================================================

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			HandleAddFrame ();
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			HandleDeleteFrame ();
		}

		private void ButtonMovePrev_Click (object sender, EventArgs e)
		{
			HandleMoveFramePrev ();
		}

		private void ButtonMoveNext_Click (object sender, EventArgs e)
		{
			HandleMoveFrameNext ();
		}

		//=============================================================================

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

		//=============================================================================

		private void PreviewButtonPlay_Click (object sender, EventArgs e)
		{
			if (!AnimationPreview.IsAnimated)
			{
				using (CursorState lCursorState = new CursorState (Program.MainWindow))
				{
					lCursorState.ShowWait ();
					AnimationPreview.CreateAnimation (CharacterFile, Animation, PreviewImageSize, !PreviewButtonMute.Checked);
				}
			}
			HandlePreviewPlay ();
			ToolStripPreview.Update ();
		}

		private void PreviewButtonPause_Click (object sender, EventArgs e)
		{
			PreviewButtonPause.Checked = !PreviewButtonPause.Checked;
			HandlePreviewPause ();
		}

		private void PreviewButtonStop_Click (object sender, EventArgs e)
		{
			HandlePreviewStop ();
			ToolStripPreview.Update ();
		}

		private void PreviewButtonSkipBack_Click (object sender, EventArgs e)
		{
			HandlePreviewSkipBack ();
		}

		private void PreviewButtonSkipForward_Click (object sender, EventArgs e)
		{
			HandlePreviewSkipForward ();
		}

		//=============================================================================

		private void PreviewButtonRepeat_Click (object sender, EventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			else
			{
				PreviewButtonRepeat.Checked = !PreviewButtonRepeat.Checked;
				HandlePreviewRepeat ();
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
				HandlePreviewMute ();
			}
		}

		private void TrackBarRate_Scroll (object sender, EventArgs e)
		{
			HandlePreviewRateChanged ();
		}

		//=============================================================================

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
				if (!AnimationPreview.IsPlaying)
				{
					PopSelectedFrame ();
					ShowFrameSelection ();
					ShowFrameSelectionState ();
					// NOT ShowFramePreview ();
				}
				ShowPreviewState ();
				ToolStripPreview.Update ();
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
						using (PanelFillingState lFillingState = new PanelFillingState (this))
						{
							FramesView.Frames.SelectedIndex = lFrameNdx;
							FramesView.Frames.EnsureVisible (lFrameNdx, true);
						}
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
