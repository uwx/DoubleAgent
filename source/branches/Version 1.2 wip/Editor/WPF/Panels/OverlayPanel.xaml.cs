using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class OverlayPanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public OverlayPanel ()
		{
			InitializeComponent ();
			if (Program.MainWindow != null)
			{
				Program.MainWindow.ViewChanged += new EventHandler (OnViewChanged);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (OverlayPanel pPanel)
				: base (pPanel)
			{
				SelectedOverlay = pPanel.ListViewOverlays.SelectedIndex;
			}

			public void RestoreContext (OverlayPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewOverlays.SelectedIndex = SelectedOverlay;
			}

			public int SelectedOverlay
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameOverlays ()
		{
			foreach (ListViewItem lOverlayItem in ListViewOverlays.Items)
			{
				OverlayListItem lListItem = lOverlayItem.Content as OverlayListItem;
				MouthOverlay lOverlayType = lListItem.Overlay.OverlayType;

				if (!IsPanelEmpty && (Frame.Overlays != null) && (Frame.Overlays.Contains (lOverlayType)))
				{
					lListItem.Overlay = Frame.Overlays[lOverlayType];
				}
				else
				{
					lListItem.Overlay = new FileFrameOverlay ();
					lListItem.Overlay.OverlayType = lOverlayType;
				}

				lListItem.OverlayChanged ();
			}

			ArrangeOverlaysList ();
			ListViewOverlays.IsEnabled = !IsPanelEmpty;

			if (IsPanelEmpty)
			{
				ShowFrameOverlay (null);
			}
		}

		private void ArrangeOverlaysList ()
		{
			//
			//	Should work in XAML, but DevicePixel rounding messes it up
			//
			ListViewOverlays.MinWidth = OverlaysScroller.ViewportWidth;
			ListViewOverlays.UpdateLayout ();
			ScrollViewer.SetHorizontalScrollBarVisibility (OverlaysScroller, (ListViewOverlays.ActualWidth > ListViewOverlays.MinWidth) ? ScrollBarVisibility.Visible : ScrollBarVisibility.Hidden);
#if DEBUG_NOT
			Overlays.UpdateLayout ();
			System.Diagnostics.Debug.Print ("Scroller [{0}] Viewport [{1}]", OverlaysScroller.ActualWidth, OverlaysScroller.ViewportWidth);
			System.Diagnostics.Debug.Print ("ListViewOverlays Width [{0}] Desired [{1}] Min [{2}]", ListViewOverlays.ActualWidth, ListViewOverlays.DesiredSize.Width, ListViewOverlays.MinWidth);
			System.Diagnostics.Debug.Print ("OverlaysHeader Width [{0}] Desired [{1}]", OverlaysHeader.ActualWidth, OverlaysHeader.DesiredSize.Width);
#endif
		}

		private void RefreshOverlayItem (FileFrameOverlay pFrameOverlay)
		{
			if (pFrameOverlay != null)
			{
				foreach (ListViewItem lOverlayItem in ListViewOverlays.Items)
				{
					OverlayListItem lListItem = lOverlayItem.Content as OverlayListItem;

					if (lListItem.Overlay.OverlayType == pFrameOverlay.OverlayType)
					{
						lListItem.OverlayChanged ();
						ArrangeOverlaysList ();
						break;
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameSample (FileFrameOverlay pFrameOverlay)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				System.Drawing.Bitmap lBitmap = null;

				if (pFrameOverlay == null)
				{
					PanelSample.IsEnabled = false;
					if (!IsPanelEmpty)
					{
						try
						{
							lBitmap = CharacterFile.GetFrameBitmap (Frame, true, System.Drawing.Color.Transparent);
						}
						catch
						{
						}
					}
				}
				else
				{
					PanelSample.IsEnabled = !Program.FileIsReadOnly;
					try
					{
						lBitmap = CharacterFile.GetFrameBitmap (Frame, true, System.Drawing.Color.Transparent, (Int16)pFrameOverlay.OverlayType);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}

				try
				{
					if (lBitmap == null)
					{
						ImageSample.Source = null;
					}
					else
					{
						ImageSample.Source = lBitmap.MakeImageSource ();
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
				try
				{
					ImageSample.LayoutTransform = Program.MainWindow.CurrentView.Inverse as Transform;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void Overlays_LayoutUpdated (object sender, EventArgs e)
		{
			ArrangeOverlaysList ();
		}

		private void OnViewChanged (object sender, EventArgs e)
		{
			ShowSelectedOverlay ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewOverlays_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			if (ListViewOverlays.SelectedIndex >= 0)
			{
				ShowSelectedOverlay ();
			}
		}

		private void ListViewOverlays_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay (false));
		}

		private void ListViewOverlays_ContextMenuOpening (object sender, ContextMenuEventArgs e)
		{
			if (IsPanelEmpty)
			{
				e.Handled = true;
			}
			else
			{
				MenuItemAdd.IsEnabled = CanAddOverlay;
				MenuItemAdd.SetTitle (AddOverlayTitle);
				MenuItemChooseFile.IsEnabled = CanChooseOverlayFile;
				MenuItemChooseFile.SetTitle (ChooseOverlayFileTitle);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, RoutedEventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay (false));
		}

		private void ButtonDelete_Click (object sender, RoutedEventArgs e)
		{
			DeleteSelectedOverlay (GetSelectedOverlay (false), false);
		}

		private void ButtonChooseFile_Click (object sender, RoutedEventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay (false));
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericOffsetX.IsModified)
			{
				HandleOffsetXChanged ();
			}
			NumericOffsetX.IsModified = false;
		}

		private void NumericOffsetY_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericOffsetY.IsModified)
			{
				HandleOffsetYChanged ();
			}
			NumericOffsetY.IsModified = false;
		}

		private void CheckBoxReplace_CheckChanged (object sender, RoutedEventArgs e)
		{
			HandleReplaceChanged ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, RoutedEventArgs e)
		{
			HandleShiftUpClick (1);
		}
		private void ButtonShiftUp_RepeatEnd (object sender, RoutedEventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftDown_Click (object sender, RoutedEventArgs e)
		{
			HandleShiftDownClick (1);
		}
		private void ButtonShiftDown_RepeatEnd (object sender, RoutedEventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftLeft_Click (object sender, RoutedEventArgs e)
		{
			HandleShiftLeftClick (1);
		}
		private void ButtonShiftLeft_RepeatEnd (object sender, RoutedEventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftRight_Click (object sender, RoutedEventArgs e)
		{
			HandleShiftRightClick (1);
		}
		private void ButtonShiftRight_RepeatEnd (object sender, RoutedEventArgs e)
		{
			HandleShiftComplete ();
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////
	#region List Item Structure

	public class OverlayListItem : INotifyPropertyChanged
	{
		public OverlayListItem ()
		{
		}

		///////////////////////////////////////////////////////////////////////////////

		public FileFrameOverlay Overlay
		{
			get;
			set;
		}
		public Boolean Exists
		{
			get
			{
				return (Overlay == null) ? false : (Overlay.Container == null) ? false : Overlay.Container.Contains (Overlay);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public ImageSource OverlayTypeImage
		{
			get
			{
				if (Overlay != null)
				{
					switch (Overlay.OverlayType)
					{
						case MouthOverlay.MouthOverlayClosed:
							return Application.Current.TryFindResource ("ImgMouthClosed") as BitmapImage;
						case MouthOverlay.MouthOverlayWide1:
							return Application.Current.TryFindResource ("ImgMouthWide1") as BitmapImage;
						case MouthOverlay.MouthOverlayWide2:
							return Application.Current.TryFindResource ("ImgMouthWide2") as BitmapImage;
						case MouthOverlay.MouthOverlayWide3:
							return Application.Current.TryFindResource ("ImgMouthWide3") as BitmapImage;
						case MouthOverlay.MouthOverlayWide4:
							return Application.Current.TryFindResource ("ImgMouthWide4") as BitmapImage;
						case MouthOverlay.MouthOverlayMedium:
							return Application.Current.TryFindResource ("ImgMouthMedium") as BitmapImage;
						case MouthOverlay.MouthOverlayNarrow:
							return Application.Current.TryFindResource ("ImgMouthNarrow") as BitmapImage;
					}
				}
				return null;
			}
		}
		public String OverlayTypeName
		{
			get
			{
				return (Overlay != null) ? Titles.OverlayTypeName (Overlay.OverlayType) : "<empty>";
			}
		}
		public String ImageFilePath
		{
			get
			{
				return Exists ? Overlay.ImageFilePath : String.Empty;
			}
		}
		public String Offset
		{
			get
			{
				return Exists ? String.Format ("{0}, {1}", Overlay.Offset.X, Overlay.Offset.Y) : String.Empty;
			}
		}
		public Boolean ReplaceFlag
		{
			get
			{
				return Exists ? Overlay.ReplaceFlag : false;
			}
		}
		public Visibility ReplaceFlagVisibility
		{
			get
			{
				return (Exists && Overlay.ReplaceFlag) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public void OverlayChanged ()
		{
			if (PropertyChanged != null)
			{
				PropertyChanged (this, new PropertyChangedEventArgs ("ImageFilePath"));
				PropertyChanged (this, new PropertyChangedEventArgs ("Offset"));
				PropertyChanged (this, new PropertyChangedEventArgs ("ReplaceFlag"));
				PropertyChanged (this, new PropertyChangedEventArgs ("ReplaceFlagVisibility"));
			}
		}
		public event PropertyChangedEventHandler PropertyChanged;
	}

	#endregion
}
