using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Animation;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Panels;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor
{
	public partial class MainWindow : Window
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public MainWindow ()
		{
			Program.MainWindow = this;
			InitializeComponent ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void LoadWindowState ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				if ((lSettings.MainFormSize.Width > 0) && (lSettings.MainFormSize.Height > 0))
				{
					System.Windows.Point lLocation = lSettings.MainFormLocation;
					System.Windows.Size lSize = lSettings.MainFormSize;

					lSize.Width = Math.Max (lSize.Width, MinWidth);
					lSize.Height = Math.Max (lSize.Height, MinHeight);
					Left = lLocation.X;
					Top = lLocation.Y;
					Width = lSize.Width;
					Height = lSize.Height;
				}
				WindowState = (lSettings.MainFormState == WindowState.Minimized) ? WindowState.Normal : lSettings.MainFormState;

				if (lSettings.MainFormSplit > 0)
				{
					MainGrid.ColumnDefinitions[0].Width = new GridLength (lSettings.MainFormSplit);
				}
			}
		}

		private void LoadRecentFiles ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				mRecentFiles.Clear ();
				if (lSettings.RecentFiles != null)
				{
					foreach (String lPath in lSettings.RecentFiles)
					{
						mRecentFiles.Add (lPath);
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("Load Recent [{0}]", lPath);
#endif
					}
				}
			}
			mRecentFiles.ShowPaths (MenuItemFile, MenuItemFilePrintPreview);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void SaveSettings ()
		{
			Settings.Default.Save ();
		}

		private void SaveWindowState ()
		{
			Settings lSettings = Settings.Default;

			if (this.WindowState == WindowState.Normal)
			{
				lSettings.MainFormLocation = new Point (Left, Top);
				lSettings.MainFormSize = new Size (Width, Height);
			}
			else
			{
				lSettings.MainFormLocation = RestoreBounds.TopLeft;
				lSettings.MainFormSize = RestoreBounds.Size;
			}
			lSettings.MainFormState = (WindowState == WindowState.Minimized) ? WindowState.Normal : WindowState;

			lSettings.MainFormSplit = MainGrid.ColumnDefinitions[0].Width.Value;
		}

		private void SaveRecentFiles ()
		{
			Settings lSettings = Settings.Default;

			lSettings.RecentFiles = new System.Collections.Specialized.StringCollection ();
			foreach (String lPath in mRecentFiles)
			{
				lSettings.RecentFiles.Add (lPath);
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("Save Recent [{0}]", lPath);
#endif
			}
		}

		private void UpdateRecentFiles ()
		{
			if ((mCharacterFile != null) && (!String.IsNullOrEmpty (mCharacterFile.Path)))
			{
				mRecentFiles.MostRecent = mCharacterFile.Path;
				mRecentFiles.ShowPaths (MenuItemFile, MenuItemFilePrintPreview);
				SaveRecentFiles ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		protected Boolean IsWindowShowing
		{
			get;
			set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region File

		private Boolean PromptSaveCharacterFile ()
		{
			Boolean lRet = true;
#if	!DEBUG
			if (mCharacterFile.IsDirty && !mCharacterFile.IsReadOnly)
			{
				String lMessage;

				if (String.IsNullOrEmpty (mCharacterFile.Path))
				{
					String lNewName = GetSaveNewName ();

					if (String.IsNullOrEmpty (lNewName))
					{
						lNewName = AppResources.Resources.TitleNewFile;
					}
					lMessage = String.Format (AppResources.Resources.MsgSaveNew, lNewName);
				}
				else
				{
					lMessage = String.Format (AppResources.Resources.MsgSaveFile, mCharacterFile.FileName);
				}

				switch (MessageBox.Show (lMessage, Program.AssemblyTitle, MessageBoxButton.YesNoCancel))
				{
					case MessageBoxResult.Yes:
						{
							lRet = SaveCharacterFile ();
						}
						break;
					case MessageBoxResult.Cancel:
						{
							lRet = false;
						}
						break;
				}
			}
#endif
			return lRet;
		}

		private Boolean PromptRemoveRecent (String pFilePath)
		{
			if (MessageBox.Show (String.Format ("Remove {0} from the recent files list?", pFilePath), Program.AssemblyTitle, MessageBoxButton.YesNo) == MessageBoxResult.Yes)
			{
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFileState ()
		{
			if (mCharacterFile == null)
			{
				this.Title = Program.AssemblyTitle;
			}
			else
			{
				this.Title = String.Format (mCharacterFile.IsDirty ? AppResources.Resources.TitleDirtyFile : AppResources.Resources.TitleOpenFile, Program.AssemblyTitle, String.IsNullOrEmpty (mCharacterFile.Path) ? AppResources.Resources.TitleNewFile : mCharacterFile.Path);
			}
		}

		private void ShowEditState ()
		{
		}

		private void ShowNavigateState ()
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		private void FadeShowSelectedPanel (FilePartPanel pSelectedPanel)
		{
#if false
			Storyboard lMainStoryboard = new Storyboard ();
			Storyboard lPanelStoryboard;

			if ((lPanelStoryboard = GetPanelFadeIn (pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}

			if ((lPanelStoryboard = GetPanelFadeOut (PanelCharacter, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelBalloon, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelTts, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelAnimations, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelAnimation, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelFrame, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelBranching, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelOverlays, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}
			if ((lPanelStoryboard = GetPanelFadeOut (PanelState, pSelectedPanel)) != null)
			{
				lMainStoryboard.Children.Add (lPanelStoryboard);
			}

			if ((lMainStoryboard.Children.Count > 0) && PanelFader.StartStoryboard (lMainStoryboard, this))
			{
				PutPanelFadeIn (lMainStoryboard, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelCharacter, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelBalloon, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelTts, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelAnimations, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelAnimation, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelFrame, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelBranching, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelOverlays, pSelectedPanel);
				PutPanelFadeOut (lMainStoryboard, PanelState, pSelectedPanel);
			}
			else
			{
				ShowSelectedPanel (pSelectedPanel);
			}
#else
			PanelCharacter.Fader.IsPanelVisible = Object.ReferenceEquals (PanelCharacter, pSelectedPanel);
			PanelBalloon.Fader.IsPanelVisible = Object.ReferenceEquals (PanelBalloon, pSelectedPanel);
			PanelTts.Fader.IsPanelVisible = Object.ReferenceEquals (PanelTts, pSelectedPanel);
			PanelAnimations.Fader.IsPanelVisible = Object.ReferenceEquals (PanelAnimations, pSelectedPanel);
			PanelAnimation.Fader.IsPanelVisible = Object.ReferenceEquals (PanelAnimation, pSelectedPanel);
			PanelFrame.Fader.IsPanelVisible = Object.ReferenceEquals (PanelFrame, pSelectedPanel);
			PanelBranching.Fader.IsPanelVisible = Object.ReferenceEquals (PanelBranching, pSelectedPanel);
			PanelOverlays.Fader.IsPanelVisible = Object.ReferenceEquals (PanelOverlays, pSelectedPanel);
			PanelState.Fader.IsPanelVisible = Object.ReferenceEquals (PanelState, pSelectedPanel);
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		private Storyboard GetPanelFadeIn (FilePartPanel pPanel)
		{
			Storyboard lPanelStoryboard = null;

			if ((pPanel != null) && (pPanel.Fader != null))
			{
				lPanelStoryboard = pPanel.Fader.GetPanelFadeIn ();
			}
			return lPanelStoryboard;
		}

		private Storyboard GetPanelFadeOut (FilePartPanel pPanel, FilePartPanel pSelectedPanel)
		{
			Storyboard lPanelStoryboard = null;

			if ((pPanel != null) && (pPanel.Fader != null) && !Object.ReferenceEquals (pPanel, pSelectedPanel))
			{
				lPanelStoryboard = pPanel.Fader.GetPanelFadeOut ();
			}
			return lPanelStoryboard;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PutPanelFadeIn (Storyboard pFadeIn, FilePartPanel pPanel)
		{
			if ((pFadeIn != null) && (pPanel != null) && (pPanel.Fader != null))
			{
				PanelFader.StopStoryboard (pPanel.Fader.FadeOutStoryboard, this);
				pPanel.Fader.StopStoryboard (pPanel.Fader.FadeOutStoryboard);
				pPanel.Fader.FadeOutStoryboard = null;
				pPanel.Fader.FadeInStoryboard = pFadeIn;
			}
		}

		private void PutPanelFadeOut (Storyboard pFadeOut, FilePartPanel pPanel, FilePartPanel pSelectedPanel)
		{
			if ((pFadeOut != null) && (pPanel != null) && (pPanel.Fader != null) && !Object.ReferenceEquals (pPanel, pSelectedPanel))
			{
				PanelFader.StopStoryboard (pPanel.Fader.FadeInStoryboard, this);
				pPanel.Fader.StopStoryboard (pPanel.Fader.FadeInStoryboard);
				pPanel.Fader.FadeInStoryboard = null;
				pPanel.Fader.FadeOutStoryboard = pFadeOut;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Window Event Handlers

		private void Window_Initialized (object sender, EventArgs e)
		{
			InitializeCommon ();
			LoadWindowState ();
		}

		private void Window_Loaded (object sender, RoutedEventArgs e)
		{
			try
			{
				FrameworkElement lOverflow = ToolBarMain.Template.FindName ("OverflowButton", ToolBarMain) as FrameworkElement;
				FrameworkElement lThumb = ToolBarMain.Template.FindName ("ToolBarThumb", ToolBarMain) as FrameworkElement;

				if (lOverflow != null)
				{
					lOverflow.Visibility = Visibility.Collapsed;
				}
				if (lThumb != null)
				{
					lThumb.Visibility = Visibility.Collapsed;
				}
			}
			catch
			{
			}

			LoadAllConfig ();

			if ((Program.CommandLineArgs != null) && (Program.CommandLineArgs.Length > 0))
			{
				OpenCharacterFile (Program.CommandLineArgs[0]);
			}
		}

		private void Window_Closing (object sender, System.ComponentModel.CancelEventArgs e)
		{
			SaveAllConfig ();
			if (!PromptSaveCharacterFile ())
			{
				e.Cancel = true;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Command Event Handlers

		private void OnFileNew (object sender, ExecutedRoutedEventArgs e)
		{
			HandleFileNew ();
		}

		private void OnFileOpen (object sender, ExecutedRoutedEventArgs e)
		{
			HandleFileOpen ();
		}

		private void CanFileSave (object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = CanHandleFileSave;
			e.Handled = true;
		}
		private void OnFileSave (object sender, ExecutedRoutedEventArgs e)
		{
			HandleFileSave ();
		}

		private void CanFileSaveAs (object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = CanHandleFileSaveAs;
			e.Handled = true;
		}
		private void OnFileSaveAs (object sender, ExecutedRoutedEventArgs e)
		{
			HandleFileSaveAs ();
		}

		private void OnFileClose (object sender, ExecutedRoutedEventArgs e)
		{
			Close ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CanBrowseBack (object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = CanHandleNavigateBack;
			e.Handled = true;
		}
		private void OnBrowseBack (object sender, ExecutedRoutedEventArgs e)
		{
			HandleNavigateBack ();
		}

		private void CanBrowseForward (object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = CanHandleNavigateForward;
			e.Handled = true;
		}
		private void OnBrowseForward (object sender, ExecutedRoutedEventArgs e)
		{
			HandleNavigateForward ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewCanEditCut (object sender, CanExecuteRoutedEventArgs e)
		{
			if ((e.Source is Button) || (e.Source is MenuItem))
			{
				e.PutCutTitle ();
			}
		}
		private void PreviewCanEditCopy (object sender, CanExecuteRoutedEventArgs e)
		{
			if ((e.Source is Button) || (e.Source is MenuItem))
			{
				e.PutCopyTitle ();
			}
		}
		private void PreviewCanEditDelete (object sender, CanExecuteRoutedEventArgs e)
		{
			if ((e.Source is Button) || (e.Source is MenuItem))
			{
				e.PutDeleteTitle ();
			}
		}
		private void PreviewCanEditPaste (object sender, CanExecuteRoutedEventArgs e)
		{
			if ((e.Source is Button) || (e.Source is MenuItem))
			{
				e.PutPasteTitle ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PreviewCanEditUndo (object sender, CanExecuteRoutedEventArgs e)
		{
			if ((e.Source is Button) || (e.Source is MenuItem))
			{
				e.PutUndoTitle ();
			}
		}
		private void CanEditUndo (object sender, CanExecuteRoutedEventArgs e)
		{
			if (Program.UndoManager.CanUndo)
			{
				e.CanExecute = true;
				e.PutUndoTitle (Program.UndoManager.UndoName);
			}
			else
			{
				e.CanExecute = false;
				e.PutUndoTitle ();
			}
			e.Handled = true;
		}
		private void OnEditUndo (object sender, ExecutedRoutedEventArgs e)
		{
			HandleEditUndo ();
		}

		private void PreviewCanEditRedo (object sender, CanExecuteRoutedEventArgs e)
		{
			if ((e.Source is Button) || (e.Source is MenuItem))
			{
				e.PutRedoTitle ();
			}
		}
		private void CanEditRedo (object sender, CanExecuteRoutedEventArgs e)
		{
			if (Program.UndoManager.CanRedo)
			{
				e.CanExecute = true;
				e.PutRedoTitle (Program.UndoManager.RedoName);
			}
			else
			{
				e.CanExecute = false;
				e.PutRedoTitle ();
			}
			e.Handled = true;
		}
		private void OnEditRedo (object sender, ExecutedRoutedEventArgs e)
		{
			HandleEditRedo ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void OnHelpAbout (object sender, RoutedEventArgs e)
		{
			AgentCharacterEditor.About.AboutBox lDialog = new AgentCharacterEditor.About.AboutBox ();

			lDialog.Owner = this;
			lDialog.ShowDialog ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		private void UndoManager_Undone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] Undone", e.UndoUnit.ToString ());
#endif
		}

		private void UndoManager_Redone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] Redone", e.UndoUnit.ToString ());
#endif
		}

		#endregion
	}
}
