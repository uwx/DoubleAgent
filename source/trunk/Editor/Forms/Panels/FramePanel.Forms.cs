/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
				CheckBoxImageTransparent.Checked = Settings.Default.ShowImageTransparency;
				CheckBoxFrameTransparent.Checked = Settings.Default.ShowFrameTransparency;
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings.Default.ShowImageTransparency = CheckBoxImageTransparent.Checked;
			Settings.Default.ShowFrameTransparency = CheckBoxFrameTransparent.Checked;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		private Control ImagePasteTarget
		{
			get
			{
				return ListViewImages;
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
				PictureBoxFrameSample.Image = CharacterFile.GetFrameBitmap (Frame, true, CheckBoxFrameTransparent.Checked ? Color.Transparent : Color.Empty);
				PictureBoxFrameSample.ClientSize = CharacterFile.Header.ImageSize;
			}
			PanelMain.ResumeLayout (true);
		}

		//=============================================================================

		private void ShowFrameImages ()
		{
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					BeginListUpdate ();
					ListViewImages.Items.Clear ();
					EndListUpdate ();
					ListViewImages.Enabled = false;
					ToolStripImages.Enabled = false;
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
				}

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

		//=============================================================================

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

		//=============================================================================

		private void ShowFrameImageSample (FileFrameImage pFrameImage)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				FileImage lFrameImage = null;
				System.Drawing.Bitmap lBitmap = null;

				GroupBoxImage.Text = Titles.Image (pFrameImage);

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
						lBitmap = CharacterFile.GetImageBitmap ((int)pFrameImage.ImageNdx, true, CheckBoxImageTransparent.Checked ? Color.Transparent : Color.Empty);
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

					ButtonShiftUp.Enabled = false;
					ButtonShiftDown.Enabled = false;
					ButtonShiftLeft.Enabled = false;
					ButtonShiftRight.Enabled = false;
				}
				else
				{
					PanelImageClip.ClientSize = PictureBoxFrameSample.ClientSize;
					PictureBoxImageSample.SuspendLayout ();
					PictureBoxImageSample.ClientSize = PictureBoxFrameSample.ImageToClient (lBitmap.Size);
					PictureBoxImageSample.Location = PictureBoxFrameSample.ImageToClient (pFrameImage.Offset);
					PictureBoxImageSample.Image = lBitmap;
					PictureBoxImageSample.ResumeLayout (true);

					ButtonShiftUp.Enabled = !Program.FileIsReadOnly;
					ButtonShiftDown.Enabled = !Program.FileIsReadOnly;
					ButtonShiftLeft.Enabled = !Program.FileIsReadOnly;
					ButtonShiftRight.Enabled = !Program.FileIsReadOnly;
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
				MenuItemMovePrev.Enabled = CanMoveImageUp;
				MenuItemMoveNext.Enabled = CanMoveImageDown;

				MenuItemChooseFile.SetTitle (ChooseImageFileTitle);
				MenuItemMovePrev.SetTitle (MoveImageUpTitle);
				MenuItemMoveNext.SetTitle (MoveImageDownTitle);
			}
		}

		private void NumericDuration_Validated (object sender, EventArgs e)
		{
			HandleDurationChanged ();
		}

		//=============================================================================

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

		//=============================================================================

		private void ListViewImages_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && (ListViewImages.SelectedIndex >= 0))
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
			UpdateSelectedImage (GetSelectedImage ());
		}

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedImage (GetSelectedImage ());
		}

		//=============================================================================

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			AddNewImage ();
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			DeleteSelectedImage (GetSelectedImage (), false);
		}

		//=============================================================================

		private void ButtonMovePrev_Click (object sender, EventArgs e)
		{
			MoveImageUp ();
		}

		private void ButtonMoveNext_Click (object sender, EventArgs e)
		{
			MoveImageDown ();
		}

		//=============================================================================

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			HandleOffsetXChanged ();
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			HandleOffsetYChanged ();
		}

		//=============================================================================

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

		//=============================================================================

		private void CheckBoxImageTransparent_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowSelectedImage ();
			}
		}

		private void CheckBoxFrameTransparent_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling)
			{
				ShowFrameSample ();
			}
		}

		#endregion
	}
}
