/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesListView : System.Windows.Controls.ListView
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramesListView ()
		{
			RenderOptions.SetBitmapScalingMode (this, BitmapScalingMode.HighQuality);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.Size ImageSize
		{
			get
			{
				return mImageSize;
			}
			set
			{
				mImageSize = value.TransformToScreenResolution (Program.MainWindow.CurrentView.Inverse);

				foreach (FramesListItem lListItem in Items)
				{
					lListItem.Image.Width = mImageSize.Width;
					lListItem.Image.Height = mImageSize.Height;
				}
				InvalidateArrange ();
			}
		}
		private System.Windows.Size mImageSize = new System.Windows.Size (32, 32);

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public void ShowAnimationFrames ()
		{
			Items.Clear ();

			if (Animation != null)
			{
				foreach (FileAnimationFrame lFrame in Animation.Frames)
				{
					FramesListItem lListItem = new FramesListItem (this, CharacterFile, lFrame);

					lListItem.Image.Width = mImageSize.Width;
					lListItem.Image.Height = mImageSize.Height;
					Items.Add (lListItem);
				}
			}
			InvalidateArrange ();
		}

		protected void FrameImageChanged (FileAnimationFrame pFrame)
		{
			if (pFrame != null)
			{
				foreach (FramesListItem lListItem in Items)
				{
					if (lListItem.Frame == pFrame)
					{
						lListItem.Image.Source = lListItem.GetFrameImageSource ();
						lListItem.InvalidateVisual ();
						break;
					}
				}
			}
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	public class FramesListItem : ListViewItemCommon
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramesListItem (FramesListView pListView, CharacterFile pCharacterFile, FileAnimationFrame pFrame)
		{
			ListView = pListView;
			CharacterFile = pCharacterFile;
			Frame = pFrame;
			InitializeComponent ();
		}

		protected new void InitializeComponent ()
		{
			StackPanel lStackPanel = new StackPanel ();
			this.Content = lStackPanel;
			this.Padding = new Thickness (2);
			this.Margin = new Thickness (2, 0, 2, 0);

			Image = new Image ();
			Image.Margin = new Thickness (2, 2, 2, 0);
			Image.Width = 32;
			Image.Height = 32;
			Image.Stretch = System.Windows.Media.Stretch.Uniform;
			Image.Source = GetFrameImageSource ();

			Label = new Label ();
			Label.Margin = new Thickness (1);
			Label.HorizontalContentAlignment = System.Windows.HorizontalAlignment.Center;
			Label.VerticalContentAlignment = System.Windows.VerticalAlignment.Top;
			if (Frame != null)
			{
				Label.Content = String.Format ("{0} ({1:D})", Titles.Frame (Frame), Frame.Duration);
			}

			lStackPanel.Orientation = Orientation.Vertical;
			lStackPanel.Children.Add (Image);
			lStackPanel.Children.Add (Label);
		}

		public ImageSource GetFrameImageSource ()
		{
			System.Drawing.Bitmap lBitmap = null;
			if (ListView != null)
			{
			    lBitmap = ListView.GetFrameImageAsync (Frame);
			}
			return (lBitmap == null) ? null : lBitmap.MakeImageSource ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FramesListView ListView
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FileAnimationFrame Frame
		{
			get;
			protected set;
		}

		//=============================================================================

		public System.Windows.Controls.Image Image
		{
			get;
			set;
		}
		public System.Windows.Controls.Label Label
		{
			get;
			set;
		}

		#endregion
	}
}
