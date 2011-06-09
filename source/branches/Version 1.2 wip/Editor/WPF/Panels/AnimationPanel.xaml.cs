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
using System.Windows.Media;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Previews;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class AnimationPanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationPanel ()
		{
			InitializeComponent ();
			InitAnimationStates ();

			if (Program.MainWindow != null)
			{
				Program.MainWindow.ViewChanged += new EventHandler (OnViewChanged);
			}

			FramesView.Frames.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler (FramesView_SelectionChanged);
			FramesView.Frames.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler (FramesView_MouseDoubleClick);
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
				PreviewButtonRepeat.IsChecked = lSettings.AnimationPreviewRepeat;
				PreviewButtonMute.IsChecked = lSettings.AnimationPreviewMute;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings lSettings = Settings.Default;

			lSettings.FramesViewBranching = FramesView.ShowBranching;
			lSettings.FramesViewExitBranching = FramesView.ShowExitBranching;
			lSettings.FramesViewScale = (int)FramesView.ImageScale;
			lSettings.AnimationPreviewRate = (int)SliderRate.Value;
			lSettings.AnimationPreviewRepeat = PreviewButtonRepeat.IsChecked.Value;
			lSettings.AnimationPreviewMute = PreviewButtonMute.IsChecked.Value;
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
				//SelectedFrame = pPanel.FramesView.Frames.SelectedIndex;
			}

			public void RestoreContext (AnimationPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewStates.SelectedIndex = SelectedState;
				//pPanel.FramesView.Frames.SelectedIndex = SelectedFrame;
			}

			public int SelectedState
			{
				get;
				protected set;
			}
			public int SelectedFrame
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
				ComboBoxReturn.Items.Clear ();
				mReturnItemNone = ComboBoxReturn.Items.Add ("None");
				mReturnItemExit = -1;

				if (IsPanelEmpty)
				{
					ComboBoxReturn.SelectedIndex = -1;
					ComboBoxReturn.IsEnabled = false;
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
					ComboBoxReturn.IsEnabled = !Program.FileIsReadOnly;
				}
			}
		}

		private void ShowAnimationFrames ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					FramesView.Clear ();
					FramesView.IsEnabled = false;
				}
				else
				{
					using (CursorState lCursorState = new CursorState (Program.MainWindow))
					{
						lCursorState.ShowWait ();

						FramesView.ShowAnimationFrames (CharacterFile, Animation);
						FramesView.IsEnabled = true;

						ShowFramesState ();
						ShowSelectedFrame ();
					}
				}

				//AnimationPreview.DeleteAnimation ();
			}
		}

		private void ShowSelectedFrame ()
		{
			//FramesView.Frames.EnsureVisible (FramesView.Frames.SelectedOrFocusedIndex);

			ShowSelectionState ();

			//AnimationPreview.StopAnimation ();
			//AnimationPreview.ShowAnimationFrame (CharacterFile, GetSelectedFrame (true));
			ShowPreviewState ();
		}

		private void ShowPreviewState ()
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		void OnViewChanged (object sender, EventArgs e)
		{
			ShowAnimationFrames ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxName_IsModifiedChanged (object sender, System.Windows.RoutedEventArgs e)
		{
			if (TextBoxName.IsModified)
			{
				HandleAnimationNameChanged ();
			}
			TextBoxName.IsModified = false;
		}

		private void ComboBoxReturn_SelectionChanged (object sender, System.Windows.Controls.SelectionChangedEventArgs e)
		{
			HandleReturnAnimationChanged ();
		}

		private void NumericFrameDuration_IsModifiedChanged (object sender, System.Windows.RoutedEventArgs e)
		{
			if (NumericFrameDuration.IsModified)
			{
				HandleDefaultDurationChanged ();
			}
			NumericFrameDuration.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		void FramesView_SelectionChanged (object sender, System.Windows.Controls.SelectionChangedEventArgs e)
		{
			if (!IsPanelEmpty && !IsPanelFilling)
			{
				ShowSelectedFrame ();
			}
		}

		void FramesView_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
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

		private void ListViewStates_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
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

		private void ButtonAdd_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleAddFrame ();
		}

		private void ButtonDelete_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleDeleteFrame ();
		}

		private void ButtonMoveUp_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleMoveFrameUp ();
		}

		private void ButtonMoveDown_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleMoveFrameDown ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShowBranching_CheckChanged (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleShowBranching ();
		}

		private void ButtonShowExitBranching_CheckChanged (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleShowExitBranching ();
		}

		private void ButtonViewSmall_Checked (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleViewSmall ();
		}

		private void ButtonViewMedium_Checked (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleViewMedium ();
		}

		private void ButtonViewLarge_Checked (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleViewLarge ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewButtonPlay_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void PreviewButtonPause_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void PreviewButtonStop_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void PreviewButtonSkipBack_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void PreviewButtonSkipForward_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void PreviewButtonRepeat_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void PreviewButtonMute_Click (object sender, System.Windows.RoutedEventArgs e)
		{

		}

		private void SliderRate_ValueChanged (object sender, System.Windows.RoutedPropertyChangedEventArgs<double> e)
		{

		}

		#endregion
	}
}
