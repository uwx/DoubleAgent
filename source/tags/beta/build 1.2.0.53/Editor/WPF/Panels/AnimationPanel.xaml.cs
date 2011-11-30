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
using System.ComponentModel;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using AgentCharacterEditor.Global;
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

			ToolBarFrames.ButtonAdd.Click += new RoutedEventHandler (ButtonAdd_Click);
			ToolBarFrames.ButtonDelete.Click += new RoutedEventHandler (ButtonDelete_Click);
			ToolBarFrames.ButtonMovePrev.Click += new RoutedEventHandler (ButtonMovePrev_Click);
			ToolBarFrames.ButtonMoveNext.Click += new RoutedEventHandler (ButtonMoveNext_Click);

			FramesView.Frames.SelectionChanged += new System.Windows.Controls.SelectionChangedEventHandler (FramesView_SelectionChanged);
			FramesView.Frames.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler (FramesView_MouseDoubleClick);
			FramesView.Frames.ContextMenuOpening += new System.Windows.Controls.ContextMenuEventHandler (FramesView_ContextMenuOpening);
			FramesView.MenuItemAdd.Click += new RoutedEventHandler (ButtonAdd_Click);
			FramesView.MenuItemMovePrev.Click += new RoutedEventHandler (ButtonMovePrev_Click);
			FramesView.MenuItemMoveNext.Click += new RoutedEventHandler (ButtonMoveNext_Click);

			AnimationPreview.AnimationStateChanged += new EventHandler (AnimationPreview_StateChanged);
			AnimationPreview.AnimationImageChanged += new EventHandler (AnimationPreview_ImageChanged);
		}

		protected override Size ArrangeOverride (Size arrangeBounds)
		{
			MainRight.Width = GridLength.Auto;
			base.ArrangeOverride (arrangeBounds);
			MainRight.Width = new GridLength (Math.Max (MainGrid.ActualWidth - MainLeft.ActualWidth, 0));
			return base.ArrangeOverride (arrangeBounds);
		}

		//=============================================================================

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

				ToolBarFrames.LoadConfig ();
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

			ToolBarFrames.SaveConfig ();
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
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

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
			FramesView.Frames.ScrollIntoView (FramesView.Frames.SelectedItem);
		}

		private System.Windows.Size PreviewImageSize
		{
			get
			{
				Size lImageSize = (CharacterFile != null) ? CharacterFile.Header.ImageSize.ToWPF () : FrameSample.DefaultImageSize.ToWPF ();
				return lImageSize.TransformToScreenResolution (Program.MainWindow.CurrentView.Inverse);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void OnViewChanged (object sender, EventArgs e)
		{
			ShowAnimationFrames ();
			ShowSelectedFrame ();
		}

		//=============================================================================

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

		//=============================================================================

		private void FramesView_PreviewMouseLeftButtonDown (object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			for (UIElement lHitTest = FramesViewContainer.InputHitTest (e.GetPosition (FramesViewContainer)) as UIElement; lHitTest != null; lHitTest = lHitTest = VisualTreeHelper.GetParent (lHitTest) as UIElement)
			{
				if (lHitTest is ListViewItem)
				{
					break;
				}
				else if ((lHitTest != FramesView) && FramesView.IsAncestorOf (lHitTest))
				{
					break;
				}
				else
				{
					FramesView.Frames.SelectedItem = null;
					break;
				}
			}
		}

		private void FramesView_SelectionChanged (object sender, System.Windows.Controls.SelectionChangedEventArgs e)
		{
			if (!IsPanelEmpty && !IsPanelFilling)
			{
				ShowSelectedFrame ();
			}
		}

		private void FramesView_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
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

		private void FramesView_ContextMenuOpening (object sender, System.Windows.Controls.ContextMenuEventArgs e)
		{
			if (IsPanelEmpty)
			{
				e.Handled = true;
			}
			else
			{
				FramesView.MenuItemAdd.IsEnabled = ToolBarFrames.CanAddFrame;
				FramesView.MenuItemMovePrev.IsEnabled = ToolBarFrames.CanMoveFramePrev;
				FramesView.MenuItemMoveNext.IsEnabled = ToolBarFrames.CanMoveFrameNext;

				FramesView.MenuItemMovePrev.SetTitle (ToolBarFrames.MoveFramePrevTitle);
				FramesView.MenuItemMoveNext.SetTitle (ToolBarFrames.MoveFrameNextTitle);
			}
		}

		//=============================================================================

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

		//=============================================================================

		private void ButtonAdd_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleAddFrame ();
		}

		private void ButtonDelete_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleDeleteFrame ();
		}

		private void ButtonMovePrev_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleMoveFramePrev ();
		}

		private void ButtonMoveNext_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandleMoveFrameNext ();
		}

		//=============================================================================

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

		//=============================================================================

		private void PreviewButtonPlay_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			if (!AnimationPreview.IsAnimated)
			{
				using (CursorState lCursorState = new CursorState (Program.MainWindow))
				{
					lCursorState.ShowWait ();
					AnimationPreview.CreateAnimation (CharacterFile, Animation, PreviewImageSize, !PreviewButtonMute.IsChecked.Value);
				}
			}
			HandlePreviewPlay ();
		}

		private void PreviewButtonPause_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandlePreviewPause ();
		}

		private void PreviewButtonStop_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandlePreviewStop ();
		}

		private void PreviewButtonSkipBack_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandlePreviewSkipBack ();
		}

		private void PreviewButtonSkipForward_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			HandlePreviewSkipForward ();
		}

		private void PreviewButtonRepeat_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			else
			{
				HandlePreviewRepeat ();
			}
		}

		private void PreviewButtonMute_Click (object sender, System.Windows.RoutedEventArgs e)
		{
			if (AnimationPreview.IsPlaying)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			else
			{
				HandlePreviewMute ();
			}
		}

		private void SliderRate_ValueChanged (object sender, System.Windows.RoutedPropertyChangedEventArgs<double> e)
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
							FramesView.Frames.ScrollIntoView (FramesView.Frames.SelectedItem);
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
