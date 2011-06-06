using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using DoubleAgent.Character;
using AgentCharacterEditor;
using AgentCharacterEditor.Global;

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
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameSample ()
		{
			if (IsPanelEmpty || (Frame.Images == null))
			{
				ImageFrameSample.Source = null;
			}
			else
			{
				ImageFrameSample.Source = CharacterFile.GetFrameBitmap (Frame, true, System.Drawing.Color.Transparent).MakeImageSource ();
			}

			Window.GetWindow (this);
		}

		private void ShowFrameImages ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			ImageListItems.Frame = Frame;
			ImageListItems.Refresh ();
			ArrangeImagesList ();

			PopIsPanelFilling (lWasFilling);
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

		private void ShowFrameSound ()
		{
		}

		public ImageListItems ImageListItems
		{
			get;
			protected set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void Images_LayoutUpdated (object sender, EventArgs e)
		{
			ArrangeImagesList ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewImages_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
		}

		private void ListViewImages_MouseDoubleClick (object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
		}

		private void NumericOffsetY_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonDelete_Click (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonChooseFile_Click (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonMoveUp_Click (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonMoveDown_Click (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonShiftUp_Click (object sender, RoutedEventArgs e)
		{
		}

		private void CheckBoxTransparent_CheckChanged (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonSoundPreview_Click (object sender, RoutedEventArgs e)
		{
		}

		private void ButtonSoundImport_Click (object sender, RoutedEventArgs e)
		{
		}

		private void TextBoxSoundFile_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
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

		///////////////////////////////////////////////////////////////////////////////

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

		///////////////////////////////////////////////////////////////////////////////

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

		///////////////////////////////////////////////////////////////////////////////

		public void OverlayChanged ()
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

