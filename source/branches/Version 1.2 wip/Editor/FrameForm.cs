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
using System.IO;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class FrameForm : UserControl
	{
		private CharacterFile		mCharacterFile = null;
		private FileAnimationFrame	mFrame = null;
		private	SoundPlayer			mSoundPreview = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FrameForm ()
		{
			InitializeComponent ();

			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.InitializationComplete += new EventHandler (MainForm_InitializationComplete);
				MainForm.Singleton.LoadConfig += new EventHandler (MainForm_LoadConfig);
				MainForm.Singleton.SaveConfig += new EventHandler (MainForm_SaveConfig);
			}
		}

		private void MainForm_InitializationComplete (object sender, EventArgs e)
		{
			MainForm.Singleton.PanelAnimation.AnimationNameChanged += new AnimationNameChangedEventHandler (PanelAnimation_AnimationNameChanged);
		}

		void MainForm_LoadConfig (object sender, EventArgs e)
		{
			CheckBoxTransparent.Checked = Properties.Settings.Default.ShowFrameTransparency;
		}

		void MainForm_SaveConfig (object sender, EventArgs e)
		{
			Properties.Settings.Default.ShowFrameTransparency = CheckBoxTransparent.Checked;
		}

		private void FrameForm_VisibleChanged (object sender, EventArgs e)
		{
			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.CanEdit -= new EditEventHandler (MainForm_CanEdit);
				MainForm.Singleton.EditCopy -= new EditEventHandler (MainForm_EditCopy);
				MainForm.Singleton.EditCut -= new EditEventHandler (MainForm_EditCut);
				MainForm.Singleton.EditDelete -= new EditEventHandler (MainForm_EditDelete);
				MainForm.Singleton.EditPaste -= new EditEventHandler (MainForm_EditPaste);
				if (this.Visible)
				{
					MainForm.Singleton.CanEdit += new EditEventHandler (MainForm_CanEdit);
					MainForm.Singleton.EditCopy += new EditEventHandler (MainForm_EditCopy);
					MainForm.Singleton.EditCut += new EditEventHandler (MainForm_EditCut);
					MainForm.Singleton.EditDelete += new EditEventHandler (MainForm_EditDelete);
					MainForm.Singleton.EditPaste += new EditEventHandler (MainForm_EditPaste);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				this.Frame = null;
			}
		}

		[System.ComponentModel.Browsable (false)]
		public FileAnimation Animation
		{
			get
			{
				return (mFrame == null) ? null : mFrame.Animation;
			}
		}
		[System.ComponentModel.Browsable (false)]
		public String FrameTitle
		{
			get
			{
				return Program.TitleFrame (mFrame);
			}
		}
		[System.ComponentModel.Browsable (false)]
		public FileAnimationFrame Frame
		{
			get
			{
				return mFrame;
			}
			set
			{
				mFrame = value;

				ShowFrameName ();
				ShowFrameDuration ();
				ShowFrameSample ();
				ShowFrameImages ();
				ShowFrameSound ();
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFrame == null));
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
				TextBoxFrameName.Text = Program.TitleFrameAnimation (mFrame);
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
				NumericDuration.Enabled = !MainForm.Singleton.FileIsReadOnly;
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
				ToolStripImages.Enabled = !MainForm.Singleton.FileIsReadOnly;

				ListViewImages.SelectedIndex = Math.Max (ListViewImages.SelectedIndex, 0);
				ShowSelectedImage ();
			}
		}


		private void ShowImageItem (FileFrameImage pFrameImage, int pListNdx)
		{
			FileImage		lFrameImage = mCharacterFile.GetImage ((int)pFrameImage.ImageNdx);
			ListViewItem	lImageItem = (pListNdx < ListViewImages.Items.Count) ? ListViewImages.Items[pListNdx] : ListViewImages.Items.Add ("");

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

		private FileFrameImage GetSelectedImage (Boolean pIncludeFocus)
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
			FileImage				lFrameImage = null;
			System.Drawing.Bitmap	lBitmap = null;

			if (pFrameImage != null)
			{
				try
				{
					lFrameImage = mCharacterFile.GetImage ((int)pFrameImage.ImageNdx);
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
			NumericOffsetX.Validated -= new System.EventHandler (NumericOffsetX_Validated);
			NumericOffsetY.Validated -= new System.EventHandler (NumericOffsetY_Validated);

			if (lFrameImage == null)
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
				NumericOffsetX.Enabled = !MainForm.Singleton.FileIsReadOnly;
				NumericOffsetY.Enabled = !MainForm.Singleton.FileIsReadOnly;

				PanelImageClip.ClientSize = PictureBoxFrameSample.ClientSize;
				PictureBoxImageSample.SuspendLayout ();
				PictureBoxImageSample.ClientSize = PictureBoxFrameSample.ScaledSize (lBitmap.Size);
				PictureBoxImageSample.Location = PictureBoxFrameSample.ScaledPoint (pFrameImage.Offset);
				PictureBoxImageSample.Image = lBitmap;
				PictureBoxImageSample.ResumeLayout (true);
				TableLayoutSample.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}

			TableLayoutSample.ResumeLayout (true);
			NumericOffsetX.Validated += new System.EventHandler (NumericOffsetX_Validated);
			NumericOffsetY.Validated += new System.EventHandler (NumericOffsetY_Validated);

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
				TextBoxSoundFile.Enabled = !MainForm.Singleton.FileIsReadOnly;
				ToolStripSound.Enabled = true;
				ToolStripSoundFile.Enabled = !MainForm.Singleton.FileIsReadOnly;

				if (mFrame.SoundNdx >= 0)
				{
					try
					{
						Byte[]			lSound = mCharacterFile.GetSound (mFrame.SoundNdx);
						MemoryStream	lStream = new MemoryStream (lSound);

						mSoundPreview = new SoundPlayer (lStream);
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
			public UpdateFrame (CharacterFile pCharacterFile, FileAnimationFrame pFrame, Boolean pForClipboard)
				: base (pCharacterFile, pFrame, pForClipboard)
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

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleFrameAnimation (this.Target));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					if (this.ForClipboard)
					{
						return base.ChangeDescription;
					}
					else
					{
						return (Target.Duration != this.Duration) ? " duration" : (!String.Equals (Target.SoundFilePath, this.SoundFilePath)) ? " sound" : String.Empty;
					}
				}
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
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class AddDeleteFrameImage : UndoableAddDelete<FileFrameImage>
		{
			public AddDeleteFrameImage (CharacterFile pCharacterFile, FileAnimationFrame pFrame, String pImageFilePath, Boolean pForClipboard)
				: base (pCharacterFile, pForClipboard)
			{
				this.Frame = pFrame;
				this.ImageFilePath = pImageFilePath;
			}

			public AddDeleteFrameImage (CharacterFile pCharacterFile, FileFrameImage pImage, Boolean pForClipboard)
				: base (pCharacterFile, pImage, pForClipboard)
			{
				this.Frame = pImage.Frame;
				this.ImageFilePath = this.Target.ImageFilePath;
				this.ImageNdx = pImage.Container.IndexOf (this.Target);
			}

			public FileAnimation Animation
			{
				get
				{
					return this.Frame.Animation;
				}
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
			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleImageFrameAnimation (this.ImageNdx, this.Frame));
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteFrameImage	lApplied = null;

				if (IsDelete)
				{
					if (this.Frame.Images.Contains (this.Target))
					{
						this.ImageNdx = this.Frame.Images.IndexOf (this.Target);
						lApplied = new AddDeleteFrameImage (this.CharacterFile, this.Target, this.ForClipboard);
						if (this.Frame.Images.Remove (this.Target))
						{
							lApplied.IsDelete = false;
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
						lApplied = new AddDeleteFrameImage (this.CharacterFile, this.Target, this.ForClipboard);
						lApplied.IsDelete = true;
						lApplied.IsRedo = !IsRedo;
					}
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		internal class ReorderFrameImage : UndoableUpdate<FileFrameImage>
		{
			public ReorderFrameImage (CharacterFile pCharacterFile, FileFrameImage pImage, int pImageNdxTo)
				: base (pCharacterFile, pImage)
			{
				this.ImageNdxFrom = this.Frame.Images.IndexOf (this.Target);
				this.ImageNdxTo = pImageNdxTo;
			}

			public FileAnimation Animation
			{
				get
				{
					return this.Frame.Animation;
				}
			}
			public FileAnimationFrame Frame
			{
				get
				{
					return this.Target.Frame;
				}
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

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleFrameAnimation (this.Frame));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return " image order";
				}
			}

			public override UndoUnit Apply ()
			{
				int	lSwap = this.Frame.Images.IndexOf (this.Target);
				if (this.Frame.Images.Move (this.Target, this.ImageNdxTo))
				{
					this.ImageNdxFrom = this.Frame.Images.IndexOf (this.Target);
					this.ImageNdxTo = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		internal class UpdateFrameImage : UndoableUpdate<FileFrameImage>
		{
			public UpdateFrameImage (CharacterFile pCharacterFile, FileFrameImage pImage, Boolean pForClipboard)
				: base (pCharacterFile, pImage, pForClipboard)
			{
				this.Offset = Target.Offset;
				this.ImageFilePath = Target.ImageFilePath;
			}

			public FileAnimation Animation
			{
				get
				{
					return this.Frame.Animation;
				}
			}
			public FileAnimationFrame Frame
			{
				get
				{
					return this.Target.Frame;
				}
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

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleImageFrameAnimation (this.Target));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					if (this.ForClipboard)
					{
						return base.ChangeDescription;
					}
					else
					{
						return (Target.Offset != this.Offset) ? " offset" : (!String.Equals (Target.ImageFilePath, this.ImageFilePath)) ? " path" : String.Empty;
					}
				}
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
#if DEBUG
			public override String DebugString ()
			{
				return String.Format ("UndoOffset {2:D},{3:D} TargetOffset {0:D},{1:D}", this.Offset.X, this.Offset.Y, Target.Offset.X, Target.Offset.Y);
			}
#endif
		}

		private RepeatingUpdate<UpdateFrameImage,FileFrameImage> mRepeatUpdateFrameImage = new RepeatingUpdate<UpdateFrameImage, FileFrameImage> ();

		///////////////////////////////////////////////////////////////////////////////

		private Boolean UpdateSelectedImage (FileFrameImage pFrameImage)
		{
			Boolean	lRet = false;

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (pFrameImage != null))
			{
				String				lFilePath = pFrameImage.ImageFilePath;
				UpdateFrameImage	lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					lUpdate = new UpdateFrameImage (mCharacterFile, pFrameImage, false);
					lUpdate.ImageFilePath = lFilePath;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
					lRet = UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
				}
			}
			return lRet;
		}

		internal Boolean DeleteSelectedImage (FileFrameImage pFrameImage, Boolean pForClipboard)
		{
			Boolean	lRet = false;

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (pFrameImage != null))
			{
				AddDeleteFrameImage	lUpdate = new AddDeleteFrameImage (mCharacterFile, pFrameImage, pForClipboard);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (AddDeleteFrameImage_Applied);
				lRet = AddDeleteFrameImage.PutUndo (lUpdate.Apply () as AddDeleteFrameImage);
			}
			return lRet;
		}

		internal Boolean PasteSelectedImage (FileFrameImage pFrameImage, FileFrameImage pPasteImage)
		{
			Boolean	lRet = false;

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (pPasteImage != null))
			{
				if (pFrameImage == null)
				{
					AddDeleteFrameImage	lUpdate = new AddDeleteFrameImage (mCharacterFile, mFrame, String.Empty, true);

					lUpdate = lUpdate.Apply () as AddDeleteFrameImage;
					if (lUpdate != null)
					{
						pPasteImage.CopyTo (lUpdate.Target);
						lUpdate = lUpdate.Apply () as AddDeleteFrameImage;
					}
					if (lUpdate != null)
					{
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (AddDeleteFrameImage_Applied);
						lRet = AddDeleteFrameImage.PutUndo (lUpdate.Apply () as AddDeleteFrameImage);
					}
				}
				else
				{
					UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, pFrameImage, true);

					pPasteImage.CopyTo (pFrameImage);
					lUpdate = lUpdate.Apply () as UpdateFrameImage;
					if (lUpdate != null)
					{
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
						lRet = UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
					}
				}
			}
			return lRet;
		}

		internal Boolean PasteSelectedFrame (FileAnimationFrame pFrame, FileAnimationFrame pPasteFrame)
		{
			Boolean	lRet = false;

			if ((pFrame != null) && (pPasteFrame != null) && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, pFrame, true);

				pPasteFrame.CopyTo (pFrame);
				lUpdate = lUpdate.Apply () as UpdateFrame;
				if (lUpdate != null)
				{
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrame_Applied);
					lRet = UpdateFrame.PutUndo (lUpdate.Apply () as UpdateFrame);
				}
			}
			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void NumericDuration_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, mFrame, false);

				lUpdate.Duration = (UInt16)NumericDuration.Value;
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrame_Applied);
				UpdateFrame.PutUndo (lUpdate.Apply () as UpdateFrame);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxSoundFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxSoundFile.Modified) && !IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, mFrame, false);

				lUpdate.SoundFilePath = TextBoxSoundFile.Text;
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrame_Applied);
				UpdateFrame.PutUndo (lUpdate.Apply () as UpdateFrame);
			}
		}

		private void ButtonSoundImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				String	lFilePath = mFrame.SoundFilePath;

				if (OpenFileDialogEx.OpenSoundFile (ref lFilePath))
				{
					UpdateFrame	lUpdate = new UpdateFrame (mCharacterFile, mFrame, false);

					lUpdate.SoundFilePath = lFilePath;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrame_Applied);
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
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int	lSelNdx = ListViewImages.GetSelectedIndex (false);

				ButtonDelete.Enabled = (lSelNdx >= 0);
				ButtonOpen.Enabled = (lSelNdx >= 0);
				ButtonMoveUp.Enabled = (ListViewImages.Items.Count > 1) && (lSelNdx >= 1);
				ButtonMoveDown.Enabled = (ListViewImages.Items.Count > 1) && (lSelNdx >= 0) && (lSelNdx < ListViewImages.Items.Count - 1);
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
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				String				lFilePath = String.Empty;
				AddDeleteFrameImage	lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (!String.IsNullOrEmpty (lFilePath))
					{
						lUpdate = new AddDeleteFrameImage (mCharacterFile, mFrame, lFilePath, false);
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (AddDeleteFrameImage_Applied);
						AddDeleteFrameImage.PutUndo (lUpdate.Apply () as AddDeleteFrameImage);
					}
				}
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			DeleteSelectedImage (GetSelectedImage (false), false);
		}

		private void AddDeleteFrameImage_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				AddDeleteFrameImage	lUpdate = sender as AddDeleteFrameImage;

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
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				int					lSelNdx = ListViewImages.GetSelectedIndex (false);
				ReorderFrameImage	lUpdate;

				if ((lFrameImage != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderFrameImage (mCharacterFile, lFrameImage, lSelNdx - 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (ReorderFrameImage_Applied);
					ReorderFrameImage.PutUndo (lUpdate.Apply () as ReorderFrameImage);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				int					lSelNdx = ListViewImages.GetSelectedIndex (false);
				ReorderFrameImage	lUpdate;

				if ((lFrameImage != null) && (lSelNdx >= 0) && (lSelNdx < mFrame.Images.Count - 1))
				{
					lUpdate = new ReorderFrameImage (mCharacterFile, lFrameImage, lSelNdx + 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (ReorderFrameImage_Applied);
					ReorderFrameImage.PutUndo (lUpdate.Apply () as ReorderFrameImage);
				}
			}
		}

		private void ReorderFrameImage_Applied (object sender, EventArgs e)
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

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, lFrameImage, false);

				lUpdate.Offset = new Point ((int)NumericOffsetX.Value, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
				UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
			}
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, (int)NumericOffsetY.Value);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
				UpdateFrameImage.PutUndo (lUpdate.Apply () as UpdateFrameImage);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y - 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
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

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y + 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
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

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X - 1, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
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

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (mCharacterFile, lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X + 1, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateFrameImage_Applied);
				mRepeatUpdateFrameImage.PutUpdate (lUpdate, ButtonShiftRight.ClickRepeatNum);
			}
		}
		private void ButtonShiftRight_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void UpdateFrameImage_Applied (object sender, EventArgs e)
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
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void PanelAnimation_AnimationNameChanged (object sender, AnimationEventArgs e)
		{
			if (!IsEmpty && e.Animation == this.Animation)
			{
				ShowFrameName ();
			}
		}

		internal void MainForm_CanEdit (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && ListViewImages.ContainsFocus)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					e.CopyObjectTitle = Program.QuotedTitle (Program.TitleImageFrameAnimation (lFrameImage));
					if (!MainForm.Singleton.FileIsReadOnly)
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				if (!MainForm.Singleton.FileIsReadOnly && (e.PasteObject is FileFrameImage))
				{
					e.PasteObjectTitle = e.PasteTypeTitle (lFrameImage) + Program.TitleImage (e.PasteObject as FileFrameImage);
				}
			}
		}

		internal void MainForm_EditCopy (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && ListViewImages.ContainsFocus)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrameImage);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditCut (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !MainForm.Singleton.FileIsReadOnly && ListViewImages.ContainsFocus)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lFrameImage);
						DeleteSelectedImage (lFrameImage, true);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !MainForm.Singleton.FileIsReadOnly && ListViewImages.ContainsFocus)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					e.IsUsed = true;
					DeleteSelectedImage (lFrameImage, false);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !MainForm.Singleton.FileIsReadOnly && ListViewImages.ContainsFocus)
			{
				if (e.PasteObject is FileFrameImage)
				{
					e.IsUsed = true;
					PasteSelectedImage (GetSelectedImage (false), e.PasteObject as FileFrameImage);
				}
			}
		}

		#endregion
	}
}
