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
using System.Drawing;
using System.Media;
using System.Windows.Forms;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class FramePanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramePanel ()
		{
			InitializeComponent ();

			if (Program.MainWindow != null)
			{
				Program.MainWindow.InitializeContextMenu (ContextMenuImages);
			}
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			if (Settings.Default.IsValid)
			{
				CheckBoxTransparent.Checked = Settings.Default.ShowFrameTransparency;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings.Default.ShowFrameTransparency = CheckBoxTransparent.Checked;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public override object NavigationContext
		{
			get
			{
				return new PanelContext (this);
			}
			set
			{
				if (value is PanelContext)
				{
					(value as PanelContext).RestoreContext (this);
				}
				else
				{
					base.NavigationContext = value;
				}
			}
		}

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (FramePanel pPanel)
				: base (pPanel)
			{
				SelectedImage = pPanel.ListViewImages.SelectedIndex;
				FocusedImage = pPanel.ListViewImages.FocusedIndex;
			}

			public void RestoreContext (FramePanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewImages.SelectedIndex = SelectedImage;
				pPanel.ListViewImages.FocusedIndex = FocusedImage;
			}

			public int SelectedImage
			{
				get;
				protected set;
			}
			public int FocusedImage
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameSample ()
		{
			PanelMain.SuspendLayout ();
			if (IsPanelEmpty || (Frame.Images == null))
			{
				PictureBoxFrameSample.Image = null;
				PictureBoxFrameSample.ClientSize = FrameSample.DefaultImageSize;
			}
			else
			{
				PictureBoxFrameSample.Image = CharacterFile.GetFrameBitmap (Frame, true, Color.Transparent);
				PictureBoxFrameSample.ClientSize = CharacterFile.Header.ImageSize;
			}
			PanelMain.ResumeLayout (true);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameImages ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					BeginListUpdate ();
					ListViewImages.Items.Clear ();
					EndListUpdate ();
					ListViewImages.Enabled = false;
					ToolStripImages.Enabled = false;
					ShowFrameImage (null);
				}
				else
				{
					int lListNdx = 0;

					BeginListUpdate ();
					if (Frame.Images == null)
					{
						ListViewImages.Items.Clear ();
					}
					else
					{
						ListViewImages.UpdateItemCount (Frame.Images.Count);

						foreach (FileFrameImage lFrameImage in Frame.Images)
						{
							ShowImageItem (lFrameImage, lListNdx++);
						}
					}

					EndListUpdate ();
					ListViewImages.Enabled = true;
					ToolStripImages.Enabled = !Program.FileIsReadOnly;

					ListViewImages.SelectedIndex = Math.Max (ListViewImages.SelectedIndex, 0);
					ShowSelectedImage ();
				}
			}
		}

		private void ShowImageItem (FileFrameImage pFrameImage, int pListNdx)
		{
			FileImage lFrameImage = CharacterFile.GetImage ((int)pFrameImage.ImageNdx);
			ListViewItemCommon lImageItem = ((pListNdx < ListViewImages.Items.Count) ? ListViewImages.Items[pListNdx] : ListViewImages.Items.Add (new ListViewItemCommon (""))) as ListViewItemCommon;

			ListViewImages.UpdateSubItemCount (lImageItem);

			lImageItem.Text = (pListNdx + 1).ToString ();
			lImageItem.SubItems[ColumnHeaderPosition.Index].Text = pFrameImage.Offset.X.ToString () + ", " + pFrameImage.Offset.Y.ToString ();
			lImageItem.SubItems[ColumnHeaderSize.Index].Text = (lFrameImage == null) ? "" : (lFrameImage.ImageSize.Width.ToString () + " x " + lFrameImage.ImageSize.Height.ToString ());
			lImageItem.SubItems[ColumnHeaderPath.Index].Text = pFrameImage.ImageFilePath;
		}

		private void RefreshImageItem (FileFrameImage pFrameImage, int pListNdx)
		{
			if (pFrameImage != null)
			{
				ListViewImages.BeginUpdate ();
				ShowImageItem (pFrameImage, pListNdx);
				ListViewImages.AutoSizeColumn (ColumnHeaderPosition);
				ListViewImages.AutoSizeColumn (ColumnHeaderSize);
				ListViewImages.AutoSizeColumn (ColumnHeaderPath);
				ListViewImages.EndUpdate ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void BeginListUpdate ()
		{
			ListViewImages.BeginUpdate ();
		}

		private void EndListUpdate ()
		{
			ListViewImages.AutoSizeColumn (ColumnHeaderImage);
			ListViewImages.AutoSizeColumn (ColumnHeaderPosition);
			ListViewImages.AutoSizeColumn (ColumnHeaderSize);
			ListViewImages.AutoSizeColumn (ColumnHeaderPath);
			ListViewImages.EndUpdate ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameImageSample (FileFrameImage pFrameImage)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				FileImage lFrameImage = null;
				System.Drawing.Bitmap lBitmap = null;

				if (pFrameImage != null)
				{
					try
					{
						lFrameImage = CharacterFile.GetImage ((int)pFrameImage.ImageNdx);
					}
					catch
					{
					}
					try
					{
						lBitmap = CharacterFile.GetImageBitmap ((int)pFrameImage.ImageNdx, true, (CheckBoxTransparent.Checked ? Color.Transparent : Color.Empty));
					}
					catch
					{
					}
				}

				PanelSample.SuspendLayout ();

				if (lFrameImage == null)
				{
					PanelImageClip.ClientSize = FrameSample.DefaultImageSize;
					PictureBoxImageSample.SuspendLayout ();
					PictureBoxImageSample.ClientSize = FrameSample.DefaultImageSize;
					PictureBoxImageSample.Location = new Point (0, 0);
					PictureBoxImageSample.Image = null;
					PictureBoxImageSample.ResumeLayout (true);
					PanelSample.Enabled = false;
				}
				else
				{
					PanelImageClip.ClientSize = PictureBoxFrameSample.ClientSize;
					PictureBoxImageSample.SuspendLayout ();
					PictureBoxImageSample.ClientSize = PictureBoxFrameSample.ImageToClient (lBitmap.Size);
					PictureBoxImageSample.Location = PictureBoxFrameSample.ImageToClient (pFrameImage.Offset);
					PictureBoxImageSample.Image = lBitmap;
					PictureBoxImageSample.ResumeLayout (true);
					PanelSample.Enabled = !Program.FileIsReadOnly;
				}

				PanelSample.ResumeLayout (true);
				ButtonShiftUp.Width = ToolStripShiftUp.Width;
				ButtonShiftDown.Width = ToolStripShiftDown.Width;
				ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
				ButtonShiftRight.Height = ToolStripShiftRight.Height;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ContextMenuImages_Opening (object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (IsPanelEmpty || Program.FileIsReadOnly)
			{
				e.Cancel = true;
			}
			else
			{
				Program.MainWindow.ShowEditState (ContextMenuImages);

				MenuItemAdd.Enabled = CanAddImage;
				MenuItemChooseFile.Enabled = CanChooseImageFile;
				MenuItemMoveUp.Enabled = CanMoveImageUp;
				MenuItemMoveDown.Enabled = CanMoveImageDown;

				MenuItemChooseFile.SetTitle (ChooseImageFileTitle);
				MenuItemMoveUp.SetTitle (MoveImageUpTitle);
				MenuItemMoveDown.SetTitle (MoveImageDownTitle);
			}
		}

		private void NumericDuration_Validated (object sender, EventArgs e)
		{
			if (NumericDuration.IsModified)
			{
				HandleDurationChanged ();
			}
			NumericDuration.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxSoundFile_Validated (object sender, EventArgs e)
		{
			if (TextBoxSoundFile.IsModified)
			{
				HandleSoundFileChanged ();
			}
			TextBoxSoundFile.IsModified = false;
		}

		private void ButtonSoundImport_Click (object sender, EventArgs e)
		{
			HandleSoundImport ();
		}

		private void ButtonSoundPreview_Click (object sender, EventArgs e)
		{
			if (mSoundPreview != null)
			{
				mSoundPreview.Play ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewImages_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (ListViewImages.SelectedIndex >= 0)
			{
				ShowSelectedImage ();
			}
		}

		private void ListViewImages_MouseUp (object sender, MouseEventArgs e)
		{
			if (ListViewImages.SelectedIndex < 0)
			{
				ShowSelectedImage ();
			}
		}

		private void ListViewImages_ItemActivate (object sender, EventArgs e)
		{
			UpdateSelectedImage (GetSelectedImage (false));
		}

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedImage (GetSelectedImage (false));
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			AddNewImage ();
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			DeleteSelectedImage (GetSelectedImage (false), false);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			MoveImageUp ();
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			MoveImageDown ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			if (NumericOffsetX.IsModified)
			{
				HandleOffsetXChanged ();
			}
			NumericOffsetX.IsModified = false;
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			if (NumericOffsetY.IsModified)
			{
				HandleOffsetYChanged ();
			}
			NumericOffsetY.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			HandleShiftUpClick (ButtonShiftUp.ClickRepeatNum);
		}
		private void ButtonShiftUp_RepeatEnd (object sender, System.EventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftDown_Click (object sender, EventArgs e)
		{
			HandleShiftDownClick (ButtonShiftDown.ClickRepeatNum);
		}
		private void ButtonShiftDown_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftLeft_Click (object sender, EventArgs e)
		{
			HandleShiftLeftClick (ButtonShiftLeft.ClickRepeatNum);
		}
		private void ButtonShiftLeft_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		private void ButtonShiftRight_Click (object sender, EventArgs e)
		{
			HandleShiftRightClick (ButtonShiftRight.ClickRepeatNum);
		}
		private void ButtonShiftRight_RepeatEnd (object sender, EventArgs e)
		{
			HandleShiftComplete ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxTransparent_CheckedChanged (object sender, EventArgs e)
		{
			ShowSelectedImage ();
		}

		#endregion
	}
}
