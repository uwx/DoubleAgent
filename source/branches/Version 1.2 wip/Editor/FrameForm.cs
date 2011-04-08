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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Media;
using DoubleAgent;
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
			TableLayoutMain.SuspendLayout ();
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
		#region Updates

		internal class UpdateFrame : UndoableUpdate<FileAnimationFrame>
		{
			public UpdateFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame)
				: base (pCharacterFile, pFrame)
			{
				this.Duration = Target.Duration;
				this.SoundFilePath = Target.SoundFilePath;
			}

			public UInt16 Duration
			{
				get;
				set;
			}
			public String SoundFilePath
			{
				get;
				set;
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if (Target.Duration != this.Duration)
				{
					UInt16	lSwap = Target.Duration;
					Target.Duration = this.Duration;
					this.Duration = lSwap;
					lApplied = this;
				}
				if (!String.Equals (Target.SoundFilePath, this.SoundFilePath))
				{
					String	lSwap = Target.SoundFilePath;
					Target.SoundFilePath = this.SoundFilePath;
					this.SoundFilePath = lSwap;
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public override string ToString ()
			{
				if (Target.Duration != this.Duration)
				{
					return "frame duration";
				}
				else if (!String.Equals (Target.SoundFilePath, this.SoundFilePath))
				{
					return "frame sound";
				}
				else
				{
					return "frame";
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddRemoveFrameImage : UndoableAddRemove<FileFrameImage>
		{
			public AddRemoveFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame, String pImageFilePath)
				: base (pCharacterFile)
			{
				this.Frame = pFrame;
				this.ImageFilePath = pImageFilePath;
			}

			public AddRemoveFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame, FileFrameImage pImage)
				: base (pCharacterFile, pImage)
			{
				this.Frame = pFrame;
				this.ImageFilePath = this.Target.ImageFilePath;
				this.ImageNdx = this.Frame.Images.IndexOf (this.Target);
			}

			public FileAnimationFrame Frame
			{
				get;
				set;
			}
			public String ImageFilePath
			{
				get;
				private set;
			}
			public int ImageNdx
			{
				get;
				private set;
			}

			public override UndoUnit Apply ()
			{
				AddRemoveFrameImage	lApplied = null;

				if (IsRemove)
				{
					if (this.Frame.Images.Contains (this.Target))
					{
						this.ImageNdx = this.Frame.Images.IndexOf (this.Target);
						lApplied = new AddRemoveFrameImage (this.CharacterFile, this.Frame, this.Target);
						if (this.Frame.Images.Remove (this.Target))
						{
							lApplied.IsRemove = false;
							lApplied.IsRedo = !IsRedo;
						}
						else
						{
							lApplied = null;
						}
					}
				}
				else
				{
					FileFrameImage	lFrameImage = this.Frame.Images.Add (this.ImageFilePath);

					if (lFrameImage != null)
					{
						if (this.Target != null)
						{
							this.Target.CopyTo (lFrameImage);
							this.Frame.Images.Move (lFrameImage, this.ImageNdx);
						}
						this.Target = lFrameImage;
						this.ImageNdx = this.Frame.Images.IndexOf (this.Target);
						lApplied = new AddRemoveFrameImage (this.CharacterFile, this.Frame, this.Target);
						lApplied.IsRemove = true;
						lApplied.IsRedo = !IsRedo;
					}
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public override string ToString ()
			{
				return String.Format ("{0} image {1:D}", this.AddRemoveTitle, this.ImageNdx + 1);
			}
		}

		internal class ReorderFrameImage : UndoableUpdate<FileFrameImage>
		{
			public ReorderFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame, FileFrameImage pImage, int pImageNdxTo)
				: base (pCharacterFile, pImage)
			{
				this.Frame = pFrame;
				this.ImageNdxFrom = pFrame.Images.IndexOf (this.Target);
				this.ImageNdxTo = pImageNdxTo;
			}

			public FileAnimationFrame Frame
			{
				get;
				private set;
			}
			public int ImageNdxFrom
			{
				get;
				private set;
			}
			public int ImageNdxTo
			{
				get;
				private set;
			}

			public override UndoUnit Apply ()
			{
				int	lSwap = Frame.Images.IndexOf (Target);
				if (Frame.Images.Move (Target, this.ImageNdxTo))
				{
					this.ImageNdxFrom = Frame.Images.IndexOf (Target);
					this.ImageNdxTo = lSwap;

					return OnApplied (this);
				}
				return null;
			}

			public override string ToString ()
			{
				return "image order";
			}
		}

		internal class UpdateFrameImage : UndoableUpdate<FileFrameImage>
		{
			public UpdateFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame, FileFrameImage pImage)
				: base (pCharacterFile, pImage)
			{
				this.Frame = pFrame;
				this.Offset = Target.Offset;
				this.ImageFilePath = Target.ImageFilePath;
			}

			public FileAnimationFrame Frame
			{
				get;
				set;
			}
			public Point Offset
			{
				get;
				set;
			}
			public String ImageFilePath
			{
				get;
				set;
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if (Target.Offset != this.Offset)
				{
					Point	lSwap = Target.Offset;
					Target.Offset = this.Offset;
					this.Offset = lSwap;
					lApplied = this;
				}
				if (!String.Equals (Target.ImageFilePath, this.ImageFilePath))
				{
					String	lSwap = Target.ImageFilePath;
					Target.ImageFilePath = this.ImageFilePath;
					this.ImageFilePath = lSwap;
					lApplied = this;
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public override string ToString ()
			{
				String	lType = String.Empty;
				if (Target.Offset != this.Offset)
				{
					lType = " offset";
				}
				else if (!String.Equals (Target.ImageFilePath, this.ImageFilePath))
				{
					lType = " file";
				}
				return String.Format ("image {0:D}{1}", this.Frame.Images.IndexOf (this.Target) + 1, lType);
			}
#if DEBUG
			public override string DebugString ()
			{
				return String.Format ("UndoOffset {2:D},{3:D} TargetOffset {0:D},{1:D}", this.Offset.X, this.Offset.Y, Target.Offset.X, Target.Offset.Y);
			}
#endif
		}

		private RepeatingUpdate<UpdateFrameImage,FileFrameImage> mRepeatUpdateFrameImage = new RepeatingUpdate<UpdateFrameImage, FileFrameImage> ();

		private void UpdateSelectedImage ()
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				String				lFilePath;
				UpdateFrameImage	lUpdate;

				if (lFrameImage != null)
				{
					lFilePath = lFrameImage.ImageFilePath;
					if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
					{
						lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);
						lUpdate.ImageFilePath = lFilePath;
						lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
						UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
					}
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void NumericDuration_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, mFrame);

				lUpdate.Duration = (UInt16)NumericDuration.Value;
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrame_Applied);
				UpdateFrame.PutUndo (lUpdate.Apply () as UpdateFrame);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxSoundFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxSoundFile.Modified) && !IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, mFrame);

				lUpdate.SoundFilePath = TextBoxSoundFile.Text;
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrame_Applied);
				UpdateFrame.PutUndo (lUpdate.Apply () as UpdateFrame);
			}
		}

		private void ButtonSoundImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				String	lFilePath = mFrame.SoundFilePath;

				if (OpenFileDialogEx.OpenSoundFile (ref lFilePath))
				{
					UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, mFrame);

					lUpdate.SoundFilePath = lFilePath;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrame_Applied);
					UpdateFrame.PutUndo (lUpdate.Apply () as UpdateFrame);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void UpdateFrame_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateFrame	lUpdate = sender as UpdateFrame;

				if ((lUpdate != null) && (lUpdate.Target == mFrame))
				{
					ShowFrameDuration ();
					ShowFrameSound ();
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

		private void ButtonOpen_Click (object sender, EventArgs e)
		{
			UpdateSelectedImage ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				String				lFilePath = String.Empty;
				AddRemoveFrameImage	lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					lUpdate = new AddRemoveFrameImage (mCharacterFile, mFrame, lFilePath);
					lUpdate.Applied += new UndoUnit.AppliedEvent (AddRemoveFrameImage_Applied);
					AddRemoveFrameImage.PutUndo (lUpdate.Apply () as AddRemoveFrameImage);
				}
			}
		}

		private void ButtonRemove_Click (object sender, EventArgs e)
		{
			FileFrameImage		lFrameImage = GetSelectedImage (false);
			AddRemoveFrameImage	lUpdate;

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				lUpdate = new AddRemoveFrameImage (mCharacterFile, mFrame, lFrameImage);
				lUpdate.Applied += new UndoUnit.AppliedEvent (AddRemoveFrameImage_Applied);
				AddRemoveFrameImage.PutUndo (lUpdate.Apply () as AddRemoveFrameImage);
			}
		}

		void AddRemoveFrameImage_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				AddRemoveFrameImage	lUpdate = sender as AddRemoveFrameImage;

				if ((lUpdate != null) && (lUpdate.Frame == mFrame))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = lUpdate.ImageNdx;
					ShowSelectedImage ();
					ShowFrameSample ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				int					lSelNdx = ListViewImages.GetSelectedIndex (false);
				ReorderFrameImage	lUpdate;

				if ((lFrameImage != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderFrameImage (mCharacterFile, mFrame, lFrameImage, lSelNdx - 1);
					lUpdate.Applied += new UndoUnit.AppliedEvent (ReorderFrameImage_Applied);
					ReorderFrameImage.PutUndo (lUpdate.Apply () as ReorderFrameImage);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				int					lSelNdx = ListViewImages.GetSelectedIndex (false);
				ReorderFrameImage	lUpdate;

				if ((lFrameImage != null) && (lSelNdx >= 0) && (lSelNdx < mFrame.Images.Count - 1))
				{
					lUpdate = new ReorderFrameImage (mCharacterFile, mFrame, lFrameImage, lSelNdx + 1);
					lUpdate.Applied += new UndoUnit.AppliedEvent (ReorderFrameImage_Applied);
					ReorderFrameImage.PutUndo (lUpdate.Apply () as ReorderFrameImage);
				}
			}
		}

		void ReorderFrameImage_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				ReorderFrameImage	lUpdate = sender as ReorderFrameImage;

				if ((lUpdate != null) && (lUpdate.Frame == mFrame))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = mFrame.Images.IndexOf (lUpdate.Target);
					ShowSelectedImage ();
					ShowFrameSample ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);

				lUpdate.Offset = new Point ((int)NumericOffsetX.Value, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
				UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
			}
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, (int)NumericOffsetY.Value);
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
				UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y - 1);
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("Click Repeat {0}", ButtonShiftUp.ClickRepeatNum.ToString ());
#endif
				mRepeatUpdateFrameImage.PutUpdate (lUpdate, ButtonShiftUp.ClickRepeatNum);
			}
		}
		private void ButtonShiftUp_RepeatEnd (object sender, System.EventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("RepeatEnd Repeat {0}", ButtonShiftUp.ClickRepeatNum.ToString ());
#endif
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		private void ButtonShiftDown_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y + 1);
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
				mRepeatUpdateFrameImage.PutUpdate (lUpdate, ButtonShiftDown.ClickRepeatNum);
			}
		}
		private void ButtonShiftDown_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		private void ButtonShiftLeft_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);

				lUpdate.Offset = new Point (lFrameImage.Offset.X - 1, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
				mRepeatUpdateFrameImage.PutUpdate (lUpdate, ButtonShiftLeft.ClickRepeatNum);
			}
		}
		private void ButtonShiftLeft_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		private void ButtonShiftRight_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, mFrame, lFrameImage);

				lUpdate.Offset = new Point (lFrameImage.Offset.X + 1, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEvent (UpdateFrameImage_Applied);
				mRepeatUpdateFrameImage.PutUpdate (lUpdate, ButtonShiftRight.ClickRepeatNum);
			}
		}
		private void ButtonShiftRight_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		///////////////////////////////////////////////////////////////////////////////

		void UpdateFrameImage_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateFrameImage	lUpdate = sender as UpdateFrameImage;

				if ((lUpdate != null) && (lUpdate.Frame == mFrame))
				{
					RefreshImageItem (lUpdate.Target, mFrame.Images.IndexOf (lUpdate.Target));
					ShowSelectedImage ();
					ShowFrameSample ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxTransparent_CheckedChanged (object sender, EventArgs e)
		{
			ShowSelectedImage ();
		}

		#endregion
	}
}
