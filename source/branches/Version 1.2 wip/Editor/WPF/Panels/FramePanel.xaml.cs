using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class FramePanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramePanel ()
		{
			ImageListItems = new ImageListItems ();
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
				CheckBoxImageTransparent.IsChecked = Settings.Default.ShowImageTransparency;
				CheckBoxFrameTransparent.IsChecked = Settings.Default.ShowFrameTransparency;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings.Default.ShowImageTransparency = CheckBoxImageTransparent.IsChecked ?? false;
			Settings.Default.ShowFrameTransparency = CheckBoxFrameTransparent.IsChecked ?? false;
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
		#region Properties

		private Control ImagePasteTarget
		{
			get
			{
				return ImagesContainer;
			}
		}

		public ImageListItems ImageListItems
		{
			get;
			protected set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameSample ()
		{
			try
			{
				System.Drawing.Bitmap lBitmap = null;

				if (!IsPanelEmpty && (Frame.Images != null))
				{
					lBitmap = CharacterFile.GetFrameBitmap (Frame, true, CheckBoxFrameTransparent.IsChecked.Value ? System.Drawing.Color.Transparent : System.Drawing.Color.Empty);
				}
				ImageFrameSample.Source = (lBitmap == null) ? null : lBitmap.MakeImageSource ();
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			try
			{
				ImageFrameSample.LayoutTransform = Program.MainWindow.CurrentView.Inverse as Transform;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		private void ShowFrameImages ()
		{
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				ImageListItems.Refresh (CharacterFile, Frame);
				ArrangeImagesList ();

				if (lContext != null)
				{
					lContext.RestoreContext (this);
				}
				if (ListViewImages.Items.Count > 0)
				{
					ListViewImages.SelectedIndex = Math.Max (ListViewImages.SelectedIndex, 0);
				}
			}
		}

		private void RefreshImageItem (FileFrameImage pFrameImage, int pListNdx)
		{
			if ((pListNdx >= 0) && (pListNdx < ImageListItems.Count) && (pFrameImage == ImageListItems[pListNdx].Image))
			{
				ImageListItems[pListNdx].ImageChanged ();
			}
		}

		private void ArrangeImagesList ()
		{
			//
			//	Should work in XAML, but DevicePixel rounding messes it up
			//
			ListViewImages.MinWidth = ImagesScroller.ViewportWidth;
			ListViewImages.UpdateLayout ();
			ScrollViewer.SetHorizontalScrollBarVisibility (ImagesScroller, (ListViewImages.ActualWidth > ListViewImages.MinWidth) ? ScrollBarVisibility.Visible : ScrollBarVisibility.Hidden);
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("Header  [{0} {1}] [{2} {3}]", ImagesHeader.DesiredSize.Width, ImagesHeader.ActualWidth, ImagesHeaderItems.DesiredSize.Width, ImagesHeaderItems.ActualWidth);
			System.Diagnostics.Debug.Print ("        [{0} {1} {2}]", ImagesHeaderIndex.ActualWidth, ImagesHeaderOffset.ActualWidth, ImagesHeaderDimensions.ActualWidth);
			System.Diagnostics.Debug.Print ("Columns [{0} {1}] [{2} {3}]", ImagesColumns.DesiredSize.Width, ImagesColumns.ActualWidth, ImagesColumnLines.DesiredSize.Width, ImagesColumnLines.ActualWidth);
			System.Diagnostics.Debug.Print ("        [{0} {1} {2}]", ImagesColumnIndex.ActualWidth, ImagesColumnOffset.ActualWidth, ImagesColumnDimensions.ActualWidth);
			System.Diagnostics.Debug.Print ("        [{0} {1}] [{2} {3}] [{4} {5}]", ImagesLineIndex.ActualWidth, ImagesLineIndex.ActualHeight, ImagesLineOffset.ActualWidth, ImagesLineOffset.ActualHeight, ImagesLineDimensions.ActualWidth, ImagesLineDimensions.ActualHeight);
#endif
		}

		//=============================================================================

		private void ShowFrameImageSample (FileFrameImage pFrameImage)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				Size lImageSize;
				Size lContainerSize;
				System.Drawing.Bitmap lBitmap = null;

				if (pFrameImage == null)
				{
					lImageSize = FrameSample.DefaultImageSize.ToWPF ();
				}
				else
				{
					lImageSize = CharacterFile.Header.ImageSize.ToWPF ();
				}

				lContainerSize = new Size (Math.Min (Math.Max (lImageSize.Width, ImageSample.MinWidth), ImageSample.MaxWidth), Math.Min (Math.Max (lImageSize.Height, ImageSample.MinHeight), ImageSample.MaxHeight));
				lContainerSize = lContainerSize.ScaleToScreenResolution ().PreserveAspectRatio (lImageSize);
				ImageContainer.Width = lContainerSize.Width;
				ImageContainer.Height = lContainerSize.Height;

				if (pFrameImage == null)
				{
					PanelSample.IsEnabled = false;
				}
				else
				{
					PanelSample.IsEnabled = !Program.FileIsReadOnly;

					try
					{
						lBitmap = CharacterFile.GetImageBitmap ((int)pFrameImage.ImageNdx, true, CheckBoxImageTransparent.IsChecked.Value ? System.Drawing.Color.Transparent : System.Drawing.Color.Empty);
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
						ImageSample.RenderTransform = Transform.Identity;
					}
					else
					{
						ImageSample.Source = lBitmap.MakeImageSource ();
						ImageSample.RenderTransform = new TranslateTransform (pFrameImage.Offset.X * lContainerSize.Width / lImageSize.Width, pFrameImage.Offset.Y * lContainerSize.Height / lImageSize.Height);
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}

				try
				{
					ImageContainer.LayoutTransform = Program.MainWindow.CurrentView.Inverse as Transform;
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

		private void Images_LayoutUpdated (object sender, EventArgs e)
		{
			ArrangeImagesList ();
		}

		private void OnViewChanged (object sender, EventArgs e)
		{
			ShowFrameSample ();
			ShowSelectedImage ();
		}

		//=============================================================================

		private void ListViewImages_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowSelectedImage ();
			}
		}

		private void ListViewImages_PreviewMouseLeftButtonDown (object sender, MouseButtonEventArgs e)
		{
			for (UIElement lHitTest = ListViewImages.InputHitTest (e.GetPosition (ListViewImages)) as UIElement; lHitTest != null; lHitTest = lHitTest = VisualTreeHelper.GetParent (lHitTest) as UIElement)
			{
				if (lHitTest is ListViewItem)
				{
					break;
				}
				else if (!ListViewImages.IsAncestorOf (lHitTest))
				{
					ListViewImages.SelectedItem = null;
					break;
				}
			}
		}

		private void ListViewImages_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			UpdateSelectedImage (GetSelectedImage ());
		}

		private void ListViewImages_ContextMenuOpening (object sender, ContextMenuEventArgs e)
		{
			if (IsPanelEmpty)
			{
				e.Handled = true;
			}
			else
			{
				MenuItemAdd.IsEnabled = CanAddImage;
				MenuItemChooseFile.IsEnabled = CanChooseImageFile;
				MenuItemMovePrev.IsEnabled = CanMoveImageUp;
				MenuItemMoveNext.IsEnabled = CanMoveImageDown;

				MenuItemChooseFile.SetTitle (ChooseImageFileTitle);
				MenuItemMovePrev.SetTitle (MoveImageUpTitle);
				MenuItemMoveNext.SetTitle (MoveImageDownTitle);
			}
		}

		//=============================================================================

		private void NumericDuration_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericDuration.IsModified)
			{
				HandleDurationChanged ();
			}
			NumericDuration.IsModified = false;
		}

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

		//=============================================================================

		private void ButtonAdd_Click (object sender, RoutedEventArgs e)
		{
			AddNewImage ();
		}

		private void ButtonDelete_Click (object sender, RoutedEventArgs e)
		{
			DeleteSelectedImage (GetSelectedImage (), false);
		}

		private void ButtonChooseFile_Click (object sender, RoutedEventArgs e)
		{
			UpdateSelectedImage (GetSelectedImage ());
		}

		private void ButtonMovePrev_Click (object sender, RoutedEventArgs e)
		{
			MoveImageUp ();
		}

		private void ButtonMoveNext_Click (object sender, RoutedEventArgs e)
		{
			MoveImageDown ();
		}

		//=============================================================================

		private void TextBoxSoundFile_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (TextBoxSoundFile.IsModified)
			{
				HandleSoundFileChanged ();
			}
			TextBoxSoundFile.IsModified = false;
		}

		private void ButtonSoundImport_Click (object sender, RoutedEventArgs e)
		{
			HandleSoundImport ();
		}

		private void ButtonSoundPreview_Click (object sender, RoutedEventArgs e)
		{
			if (mSoundPreview != null)
			{
				mSoundPreview.Play ();
			}
		}

		//=============================================================================

		private void ButtonShiftUp_Click (object sender, RoutedEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("ButtonShiftUp_Click [{0} {1}]", ButtonShiftUp.Delay, ButtonShiftUp.Interval);
#endif
			HandleShiftUpClick (1);
		}
		private void ButtonShiftUp_RepeatEnd (object sender, RoutedEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("ButtonShiftUp_RepeatEnd [{0} {1}]", ButtonShiftUp.Delay, ButtonShiftUp.Interval);
#endif
			HandleShiftComplete ();
		}

		private void ButtonShiftDown_Click (object sender, RoutedEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("ButtonShiftDown_Click [{0} {1}]", ButtonShiftUp.Delay, ButtonShiftUp.Interval);
#endif
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

		private void CheckBoxImageTransparent_CheckChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowSelectedImage ();
			}
		}

		private void CheckBoxFrameTransparent_CheckChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowFrameSample ();
			}
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////
	#region List Item Structure

	public class ImageListItem : INotifyPropertyChanged
	{
		public ImageListItem ()
		{
		}
		public ImageListItem (CharacterFile pCharacterFile, FileFrameImage pImage)
		{
			CharacterFile = pCharacterFile;
			Image = pImage;
		}

		//=============================================================================

		public CharacterFile CharacterFile
		{
			get;
			protected set;
		}
		public FileFrameImage Image
		{
			get;
			protected set;
		}
		public Boolean Exists
		{
			get
			{
				return (Image == null) ? false : (Image.Container == null) ? false : Image.Container.Contains (Image);
			}
		}

		//=============================================================================

		public String ImageIndex
		{
			get
			{
				return Exists ? Image.Container.IndexOf (Image).ToString () : String.Empty;
			}
		}
		public String ImageFilePath
		{
			get
			{
				return Exists ? Image.ImageFilePath : String.Empty;
			}
		}
		public String Offset
		{
			get
			{
				return Exists ? String.Format ("{0}, {1}", Image.Offset.X, Image.Offset.Y) : String.Empty;
			}
		}
		public String Dimensions
		{
			get
			{
				if (Exists && (CharacterFile != null))
				{
					try
					{
						FileImage lFrameImage = CharacterFile.GetImage ((int)Image.ImageNdx);
						if (lFrameImage != null)
						{
							return String.Format ("{0}, {1}", lFrameImage.ImageSize.Width, lFrameImage.ImageSize.Height);
						}
					}
					catch
					{
					}
				}
				return String.Empty;
			}
		}

		//=============================================================================

		public void ImageChanged ()
		{
			if (PropertyChanged != null)
			{
				PropertyChanged (this, new PropertyChangedEventArgs ("FrameIndex"));
				PropertyChanged (this, new PropertyChangedEventArgs ("ImageFilePath"));
				PropertyChanged (this, new PropertyChangedEventArgs ("Offset"));
				PropertyChanged (this, new PropertyChangedEventArgs ("Dimensions"));
			}
		}
		public event PropertyChangedEventHandler PropertyChanged;
	}

	#endregion
	///////////////////////////////////////////////////////////////////////////////
	#region List Items Collection

	public class ImageListItems : ObservableCollection<ImageListItem>
	{
		public ImageListItems ()
		{
		}

		public void Refresh (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			Clear ();

			if ((pFrame != null) && (pFrame.ImageCount > 0))
			{
				foreach (FileFrameImage lImage in pFrame.Images)
				{
					Add (new ImageListItem (pCharacterFile, lImage));
				}
			}
		}
	}

	#endregion
}

