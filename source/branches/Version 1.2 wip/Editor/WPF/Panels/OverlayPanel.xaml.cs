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

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			if (Settings.Default.IsValid)
			{
				CheckBoxOverlayTransparent.IsChecked = Settings.Default.ShowOverlayTransparency;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings.Default.ShowOverlayTransparency = CheckBoxOverlayTransparent.IsChecked ?? false;
		}

		protected override Size ArrangeOverride (Size arrangeBounds)
		{
			MainRight.Width = GridLength.Auto;
			base.ArrangeOverride (arrangeBounds);
			MainRight.Width = new GridLength (Math.Max (MainGrid.ActualWidth - MainLeft.ActualWidth, 0));
			return base.ArrangeOverride (arrangeBounds);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameOverlays ()
		{
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

			using (PanelFillingState lFillingState = new PanelFillingState (this))
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

				if (lContext != null)
				{
					lContext.RestoreContext (this);
				}
				if (IsPanelEmpty)
				{
					ShowFrameOverlay (null);
				}
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

		//=============================================================================

		private void ShowOverlaySample (FileFrameOverlay pFrameOverlay)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				System.Drawing.Bitmap lFrameBitmap = null;
				System.Drawing.Bitmap lOverlayBitmap = null;

				if (pFrameOverlay == null)
				{
					PanelSample.IsEnabled = false;
					if (!IsPanelEmpty)
					{
						try
						{
							lFrameBitmap = CharacterFile.GetFrameBitmap (Frame, true, CheckBoxOverlayTransparent.IsChecked.Value ? System.Drawing.Color.Transparent : System.Drawing.Color.Empty);
						}
						catch (Exception pException)
						{
							System.Diagnostics.Debug.Print (pException.Message);
						}
					}
				}
				else
				{
					PanelSample.IsEnabled = !Program.FileIsReadOnly;
					try
					{
						lFrameBitmap = CharacterFile.GetFrameBitmap (Frame, true, CheckBoxOverlayTransparent.IsChecked.Value ? System.Drawing.Color.Transparent : System.Drawing.Color.Empty, (Int16)pFrameOverlay.OverlayType);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
					try
					{
						lOverlayBitmap = CharacterFile.GetImageBitmap ((int)pFrameOverlay.ImageNdx, true, CheckBoxOverlayTransparent.IsChecked.Value ? System.Drawing.Color.Transparent : System.Drawing.Color.Empty);
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
				}

				try
				{
					if (lFrameBitmap == null)
					{
						ImageSample.Source = null;
					}
					else
					{
						ImageSample.Source = lFrameBitmap.MakeImageSource ();
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

				try
				{
					if (lOverlayBitmap == null)
					{
						PanelOverlayImage.Visibility = Visibility.Hidden;
						OverlaySample.Source = null;
					}
					else
					{
						OverlaySample.MinWidth = 1;
						OverlaySample.MinHeight = 1;
						OverlaySample.Source = lOverlayBitmap.MakeImageSource ();
						PanelOverlayImage.Visibility = Visibility.Visible;
					}
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

		//=============================================================================

		private void ListViewOverlays_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowSelectedOverlay ();
			}
		}

		private void ListViewOverlays_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay ());
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

		//=============================================================================

		private void ButtonAdd_Click (object sender, RoutedEventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay ());
		}

		private void ButtonDelete_Click (object sender, RoutedEventArgs e)
		{
			DeleteSelectedOverlay (GetSelectedOverlay (), false);
		}

		private void ButtonChooseFile_Click (object sender, RoutedEventArgs e)
		{
			UpdateSelectedOverlay (GetSelectedOverlay ());
		}

		//=============================================================================

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

		//=============================================================================

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

		//=============================================================================

		private void CheckBoxOverlayTransparent_CheckChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowSelectedOverlay ();
			}
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

		//=============================================================================

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

		//=============================================================================

		public ImageSource OverlayTypeImage
		{
			get
			{
				if (Overlay != null)
				{
					switch (Overlay.OverlayType)
					{
						case MouthOverlay.Closed:
							return Application.Current.TryFindResource ("ImgMouthClosed") as BitmapImage;
						case MouthOverlay.Wide1:
							return Application.Current.TryFindResource ("ImgMouthWide1") as BitmapImage;
						case MouthOverlay.Wide2:
							return Application.Current.TryFindResource ("ImgMouthWide2") as BitmapImage;
						case MouthOverlay.Wide3:
							return Application.Current.TryFindResource ("ImgMouthWide3") as BitmapImage;
						case MouthOverlay.Wide4:
							return Application.Current.TryFindResource ("ImgMouthWide4") as BitmapImage;
						case MouthOverlay.Medium:
							return Application.Current.TryFindResource ("ImgMouthMedium") as BitmapImage;
						case MouthOverlay.Narrow:
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

		//=============================================================================

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
