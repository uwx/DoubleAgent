using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using AgentCharacterEditor.Global;
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
				if (IsPanelEmpty || (Frame.Images == null))
				{
					ImageFrameSample.Source = null;
				}
				else
				{
					ImageFrameSample.Source = CharacterFile.GetFrameBitmap (Frame, true, System.Drawing.Color.Transparent).MakeImageSource ();
				}
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
				ImageListItems.Frame = Frame;
				ImageListItems.Refresh ();
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
				System.Drawing.Bitmap lBitmap = null;

				if (pFrameImage == null)
				{
					PanelSample.IsEnabled = false;
				}
				else
				{
					PanelSample.IsEnabled = !Program.FileIsReadOnly;

					try
					{
						lBitmap = CharacterFile.GetImageBitmap ((int)pFrameImage.ImageNdx, true, (CheckBoxTransparent.IsChecked.Value ? System.Drawing.Color.Transparent : System.Drawing.Color.Empty));
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

				if (lBitmap == null)
				{
					ImageSample.RenderTransform = Transform.Identity;
				}
				else
				{
					ImageSample.UpdateLayout ();
					ImageSample.RenderTransform = new TranslateTransform (pFrameImage.Offset.X * ImageSample.DesiredSize.Width / lBitmap.Width, pFrameImage.Offset.Y * ImageSample.DesiredSize.Height / lBitmap.Height);
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
				MenuItemMoveUp.IsEnabled = CanMoveImageUp;
				MenuItemMoveDown.IsEnabled = CanMoveImageDown;

				MenuItemChooseFile.SetTitle (ChooseImageFileTitle);
				MenuItemMoveUp.SetTitle (MoveImageUpTitle);
				MenuItemMoveDown.SetTitle (MoveImageDownTitle);
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

		private void ButtonMoveUp_Click (object sender, RoutedEventArgs e)
		{
			MoveImageUp ();
		}

		private void ButtonMoveDown_Click (object sender, RoutedEventArgs e)
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

		private void CheckBoxTransparent_CheckChanged (object sender, RoutedEventArgs e)
		{
			ShowSelectedImage ();
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
		public ImageListItem (FileFrameImage pImage)
		{
			Image = pImage;
		}

		//=============================================================================

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
				if (Exists)
				{
					try
					{
						if (Image.Owner != null)
						{
							FileImage lFrameImage = Image.Owner.GetImage ((int)Image.ImageNdx);
							if (lFrameImage != null)
							{
								return String.Format ("{0}, {1}", lFrameImage.ImageSize.Width, lFrameImage.ImageSize.Height);
							}
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
		public FileAnimationFrame Frame
		{
			get;
			set;
		}

		public void Refresh ()
		{
			Clear ();

			if ((Frame != null) && (Frame.ImageCount > 0))
			{
				foreach (FileFrameImage lImage in Frame.Images)
				{
					Add (new ImageListItem (lImage));
				}
			}
		}
	}

	#endregion
}

