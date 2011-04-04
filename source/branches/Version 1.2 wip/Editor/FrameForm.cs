using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Media;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class FrameForm : UserControl
	{
		private CharacterFile		mCharacterFile = null;
		private FileAnimationFrame	mFrame = null;
		private String				mFrameName = null;
		private String				mAnimationName = null;
		private	SoundPlayer			mSoundPreview = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FrameForm ()
		{
			InitializeComponent ();
		}

		public void LoadConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;

			CheckBoxTransparent.Checked = lSettings.ShowFrameTransparency;
		}

		public void SaveConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;

			lSettings.ShowFrameTransparency = CheckBoxTransparent.Checked;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				FileFrame = null;
				FrameName = null;
				AnimationName = null;
			}
		}

		public FileAnimationFrame FileFrame
		{
			get
			{
				return mFrame;
			}
			set
			{
				mFrame = value;
				FrameName = null;
				mAnimationName = null;
			}
		}

		public String FrameName
		{
			get
			{
				return mFrameName;
			}
			set
			{
				mFrameName = value;
			}
		}

		public String AnimationName
		{
			get
			{
				return mAnimationName;
			}
			set
			{
				mAnimationName = value;
				ShowFrameName ();
				ShowFrameDuration ();
				ShowFrameSample ();
				ShowFrameImages ();
				ShowFrameSound ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFrame == null) || String.IsNullOrEmpty (mFrameName) || String.IsNullOrEmpty (mAnimationName));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameName ()
		{
			if (IsEmpty)
			{
				TextBoxFrameName.ResetText ();
				TextBoxFrameName.Enabled = false;
			}
			else
			{
				TextBoxFrameName.Text = mAnimationName + " - " + mFrameName;
				TextBoxFrameName.Enabled = true;
			}
		}

		private void ShowFrameDuration ()
		{
			if (IsEmpty)
			{
				NumericDuration.ResetText ();
				NumericDuration.Enabled = false;
			}
			else
			{
				NumericDuration.Value = mFrame.Duration;
				NumericDuration.Enabled = !Program.MainForm.FileIsReadOnly;
			}
		}

		private void ShowFrameSample ()
		{
			TableLayoutMain.SuspendLayout();
			if (IsEmpty || (mFrame.Images == null))
			{
				PictureBoxFrameSample.Image = null;
				PictureBoxFrameSample.ClientSize = PictureBoxSample.DefaultImageSize;
			}
			else
			{
				PictureBoxFrameSample.Image = mCharacterFile.GetFrameBitmap (mFrame, true, Color.Transparent);
				PictureBoxFrameSample.ClientSize = mCharacterFile.Header.ImageSize;
			}
			TableLayoutMain.ResumeLayout (true);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameImages ()
		{
			if (IsEmpty)
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
				int	lListNdx = 0;

				BeginListUpdate ();
				if (mFrame.Images == null)
				{
					ListViewImages.Items.Clear ();
				}
				else
				{
					ListViewImages.UpdateItemCount (mFrame.Images.Count);

					foreach (FileFrameImage lFrameImage in mFrame.Images)
					{
						ShowImageItem (lFrameImage, lListNdx++);
					}
				}

				EndListUpdate ();
				ListViewImages.Enabled = true;
				ToolStripImages.Enabled = !Program.MainForm.FileIsReadOnly;

				ListViewImages.SelectedIndex = Math.Max (ListViewImages.SelectedIndex, 0);
				ShowSelectedImage ();
			}
		}


		private void ShowImageItem (FileFrameImage pFrameImage, int pListNdx)
		{
			FileImage		lImage = mCharacterFile.GetImage ((int)pFrameImage.ImageNdx);
			ListViewItem	lImageItem = (pListNdx < ListViewImages.Items.Count) ? ListViewImages.Items[pListNdx] : ListViewImages.Items.Add ("");

			ListViewImages.UpdateSubItemCount (lImageItem);

			lImageItem.Text = (pListNdx + 1).ToString ();
			lImageItem.SubItems[ColumnHeaderPosition.Index].Text = pFrameImage.Offset.X.ToString () + ", " + pFrameImage.Offset.Y.ToString ();
			lImageItem.SubItems[ColumnHeaderSize.Index].Text = (lImage == null) ? "" : (lImage.ImageSize.Width.ToString () + " x " + lImage.ImageSize.Height.ToString ());
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

		private FileFrameImage GetSelectedImage (bool pIncludeFocus)
		{
			return GetSelectedImage (ListViewImages.GetSelectedIndex (pIncludeFocus));
		}

		private FileFrameImage GetSelectedImage (int pImageNdx)
		{
			FileFrameImage	lFrameImage = null;

			if ((mFrame != null) && (mFrame.Images != null))
			{
				if ((pImageNdx >= 0) && (pImageNdx < mFrame.Images.Count))
				{
					lFrameImage = mFrame.Images[pImageNdx];
				}
			}
			return lFrameImage;
		}

		private void ShowSelectedImage ()
		{
			ShowFrameImage (GetSelectedImage (true));
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameImage (FileFrameImage pFrameImage)
		{
			FileImage				lImage = null;
			System.Drawing.Bitmap	lBitmap = null;

			if (pFrameImage != null)
			{
				try
				{
					lImage = mCharacterFile.GetImage ((int)pFrameImage.ImageNdx);
				}
				catch
				{
				}
				try
				{
					lBitmap = mCharacterFile.GetImageBitmap ((int)pFrameImage.ImageNdx, true, (CheckBoxTransparent.Checked ? Color.Transparent : Color.Empty));
				}
				catch
				{
				}
			}

			TableLayoutSample.SuspendLayout ();
			if (lImage == null)
			{
				NumericOffsetX.ResetText ();
				NumericOffsetY.ResetText ();
				NumericOffsetX.Enabled = false;
				NumericOffsetY.Enabled = false;

				PanelImageClip.ClientSize = PictureBoxSample.DefaultImageSize;
				PictureBoxImageSample.SuspendLayout ();
				PictureBoxImageSample.ClientSize = PictureBoxSample.DefaultImageSize;
				PictureBoxImageSample.Location = new Point (0, 0);
				PictureBoxImageSample.Image = null;
				PictureBoxImageSample.ResumeLayout (true);
				TableLayoutSample.Enabled = false;
			}
			else
			{
				NumericOffsetX.Value = pFrameImage.Offset.X;
				NumericOffsetY.Value = pFrameImage.Offset.Y;
				NumericOffsetX.Enabled = !Program.MainForm.FileIsReadOnly;
				NumericOffsetY.Enabled = !Program.MainForm.FileIsReadOnly;

				PanelImageClip.ClientSize = PictureBoxFrameSample.ClientSize;
				PictureBoxImageSample.SuspendLayout ();
				PictureBoxImageSample.ClientSize = PictureBoxFrameSample.ScaledSize (lBitmap.Size);
				PictureBoxImageSample.Location = PictureBoxFrameSample.ScaledPoint (pFrameImage.Offset);
				PictureBoxImageSample.Image = lBitmap;
				PictureBoxImageSample.ResumeLayout (true);
				TableLayoutSample.Enabled = !Program.MainForm.FileIsReadOnly;
			}
			TableLayoutSample.ResumeLayout (true);

			ButtonShiftUp.Width = ToolStripShiftUp.Width;
			ButtonShiftDown.Width = ToolStripShiftDown.Width;
			ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
			ButtonShiftRight.Height = ToolStripShiftRight.Height;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameSound ()
		{
			mSoundPreview = null;

			if (IsEmpty)
			{
				TextBoxSoundFile.ResetText ();
				TextBoxSoundFile.Enabled = false;
				ToolStripSound.Enabled = false;
				ToolStripSoundFile.Enabled = false;
			}
			else
			{
				TextBoxSoundFile.Text = mFrame.SoundFilePath;
				TextBoxSoundFile.Enabled = !Program.MainForm.FileIsReadOnly;
				ToolStripSound.Enabled = !Program.MainForm.FileIsReadOnly;
				ToolStripSoundFile.Enabled = !Program.MainForm.FileIsReadOnly;

				if (!String.IsNullOrEmpty (mFrame.SoundFilePath))
				{
					try
					{
						mSoundPreview = new SoundPlayer (mFrame.SoundFilePath);
					}
					catch
					{
					}
				}
			}

			ButtonSoundPreview.Enabled = (mSoundPreview != null);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void NumericDuration_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mFrame.Duration = (UInt16)NumericDuration.Value;
				ShowFrameDuration ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ListViewImages_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (ListViewImages.SelectedIndex >= 0)
			{
				ShowSelectedImage ();
			}
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int	lSelNdx = ListViewImages.GetSelectedIndex (false);

				ButtonRemove.Enabled = (lSelNdx >= 0);
				ButtonOpen.Enabled = (lSelNdx >= 0);
				ButtonMoveUp.Enabled = (ListViewImages.Items.Count > 1) && (lSelNdx >= 1);
				ButtonMoveDown.Enabled = (ListViewImages.Items.Count > 1) && (lSelNdx >= 0) && (lSelNdx < ListViewImages.Items.Count - 1);
			}
		}

		private void ListViewImages_ItemActivate (object sender, EventArgs e)
		{
			UpdateSelectedImage ();
		}

		private void UpdateSelectedImage ()
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);
				String			lFilePath;

				if (lFrameImage != null)
				{
					lFilePath = lFrameImage.ImageFilePath;
					if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
					{
						lFrameImage.ImageFilePath = lFilePath;
						ShowFrameImages ();
						ShowSelectedImage ();
						ShowFrameSample ();
						Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage	lFrameImage = null;
				String			lFilePath = String.Empty;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					lFrameImage = mFrame.Images.Add (lFilePath);
					if (lFrameImage != null)
					{
						ShowFrameImages ();
						ListViewImages.SelectedIndex = ListViewImages.Items.Count - 1;
						ShowSelectedImage ();
						ShowFrameSample ();
						Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
					}
				}
			}
		}

		private void ButtonRemove_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				int	lSelNdx = ListViewImages.GetSelectedIndex (false);

				if (mFrame.Images.Remove (lFrameImage))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = lSelNdx;
					ShowSelectedImage ();
					ShowFrameSample ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedImage ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);
				int				lSelNdx = ListViewImages.GetSelectedIndex (false);

				if ((lFrameImage != null) && (lSelNdx > 0) && (mFrame.Images.Move (lFrameImage, lSelNdx - 1)))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = mFrame.Images.IndexOf (lFrameImage);
					ShowSelectedImage ();
					ShowFrameSample ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);
				int				lSelNdx = ListViewImages.GetSelectedIndex (false);

				if ((lFrameImage != null) && (lSelNdx >= 0) && (lSelNdx < mFrame.Images.Count - 1) && (mFrame.Images.Move (lFrameImage, lSelNdx + 1)))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = mFrame.Images.IndexOf (lFrameImage);
					ShowSelectedImage ();
					ShowFrameSample ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lFrameImage.Offset = new Point ((int)NumericOffsetX.Value, lFrameImage.Offset.Y);
				RefreshImageItem (lFrameImage, mFrame.Images.IndexOf (lFrameImage));
				ShowSelectedImage ();
				ShowFrameSample ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lFrameImage.Offset = new Point (lFrameImage.Offset.X, (int)NumericOffsetY.Value);
				RefreshImageItem (lFrameImage, mFrame.Images.IndexOf (lFrameImage));
				ShowSelectedImage ();
				ShowFrameSample ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lFrameImage.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y - 1);
				RefreshImageItem (lFrameImage, mFrame.Images.IndexOf (lFrameImage));
				ShowSelectedImage ();
				ShowFrameSample ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftDown_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lFrameImage.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y + 1);
				RefreshImageItem (lFrameImage, mFrame.Images.IndexOf (lFrameImage));
				ShowSelectedImage ();
				ShowFrameSample ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftLeft_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lFrameImage.Offset = new Point (lFrameImage.Offset.X - 1, lFrameImage.Offset.Y);
				RefreshImageItem (lFrameImage, mFrame.Images.IndexOf (lFrameImage));
				ShowSelectedImage ();
				ShowFrameSample ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonShiftRight_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lFrameImage.Offset = new Point (lFrameImage.Offset.X + 1, lFrameImage.Offset.Y);
				RefreshImageItem (lFrameImage, mFrame.Images.IndexOf (lFrameImage));
				ShowSelectedImage ();
				ShowFrameSample ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxTransparent_CheckedChanged (object sender, EventArgs e)
		{
			ShowSelectedImage ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxSoundFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxSoundFile.Modified) && !IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mFrame.SoundFilePath = TextBoxSoundFile.Text;
				ShowFrameSound ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonSoundImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				String	lFilePath = mFrame.SoundFilePath;

				if (OpenFileDialogEx.OpenSoundFile (ref lFilePath))
				{
					mFrame.SoundFilePath = lFilePath;
					ShowFrameSound ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonSoundPreview_Click (object sender, EventArgs e)
		{
			if (mSoundPreview != null)
			{
				mSoundPreview.Play ();
			}
		}

		#endregion
	}
}
