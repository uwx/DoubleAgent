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
using AgentCharacterEditor.Updates;

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
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.LoadConfig += new EventHandler (MainForm_LoadConfig);
				Program.MainForm.SaveConfig += new EventHandler (MainForm_SaveConfig);
			}
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
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.UpdateApplied -= new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				Program.MainForm.CanEdit -= new Global.CanEditEventHandler (MainForm_CanEdit);
				Program.MainForm.EditCopy -= new Global.EditEventHandler (MainForm_EditCopy);
				Program.MainForm.EditCut -= new Global.EditEventHandler (MainForm_EditCut);
				Program.MainForm.EditDelete -= new Global.EditEventHandler (MainForm_EditDelete);
				Program.MainForm.EditPaste -= new Global.EditEventHandler (MainForm_EditPaste);
				Program.MainForm.EditMenu -= new Global.ContextMenuEventHandler (MainForm_EditMenu);
				if (Visible)
				{
					Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					Program.MainForm.CanEdit += new Global.CanEditEventHandler (MainForm_CanEdit);
					Program.MainForm.EditCopy += new Global.EditEventHandler (MainForm_EditCopy);
					Program.MainForm.EditCut += new Global.EditEventHandler (MainForm_EditCut);
					Program.MainForm.EditDelete += new Global.EditEventHandler (MainForm_EditDelete);
					Program.MainForm.EditPaste += new Global.EditEventHandler (MainForm_EditPaste);
					Program.MainForm.EditMenu += new Global.ContextMenuEventHandler (MainForm_EditMenu);
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
				return Global.TitleFrame (mFrame);
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
				TextBoxFrameName.Text = Global.TitleFrameAnimation (mFrame);
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
				NumericDuration.Enabled = !Program.FileIsReadOnly;
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
				ToolStripImages.Enabled = !Program.FileIsReadOnly;

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

		private void ShowSelectedImage ()
		{
			ShowFrameImage (GetSelectedImage (true));
			ShowSelectionState (GetSelectedImage (false), ListViewImages.GetSelectedIndex (false));
		}

		private void ShowSelectionState (FileFrameImage pFrameImage, int pImageNdx)
		{
			ButtonAdd.Enabled = !IsEmpty && !Program.FileIsReadOnly;

			if ((pFrameImage == null) || IsEmpty)
			{
				ButtonDelete.Enabled = false;
				ButtonChooseFile.Enabled = false;
				ButtonMoveUp.Enabled = false;
				ButtonMoveDown.Enabled = false;
			}
			else
			{
				ButtonDelete.Enabled = !Program.FileIsReadOnly;
				ButtonChooseFile.Enabled = !Program.FileIsReadOnly;
				ButtonMoveUp.Enabled = !Program.FileIsReadOnly && (ListViewImages.Items.Count > 1) && (pImageNdx >= 1);
				ButtonMoveDown.Enabled = !Program.FileIsReadOnly && (ListViewImages.Items.Count > 1) && (pImageNdx >= 0) && (pImageNdx < ListViewImages.Items.Count - 1);
			}

			ButtonDelete.Text = ButtonDelete.Enabled ? String.Format (Properties.Resources.EditDeleteThis.NoMenuPrefix (), Global.TitleImage (pFrameImage)) : Properties.Resources.EditDelete.NoMenuPrefix ();
			ButtonChooseFile.Text = ButtonChooseFile.Enabled ? String.Format (Properties.Resources.EditChooseThisFile.NoMenuPrefix (), Global.TitleImage (pFrameImage)) : Properties.Resources.EditChooseFile.NoMenuPrefix ();
			ButtonMoveUp.Text = String.Format (Properties.Resources.EditMoveImageUp.NoMenuPrefix (), Global.TitleImage (pFrameImage));
			ButtonMoveDown.Text = String.Format (Properties.Resources.EditMoveImageDown.NoMenuPrefix (), Global.TitleImage (pFrameImage));
		}

		private void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (lFrameImage != null)
			{
				pEventArgs.CopyObjectTitle = (pEventArgs is Global.ContextMenuEventArgs) ? Global.TitleImage (lFrameImage) : Global.TitleImageFrameAnimation (lFrameImage).Quoted ();
				if (!Program.FileIsReadOnly)
				{
					pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
					pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
				}
			}
			if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileFrameImage))
			{
				if (lFrameImage == null)
				{
					pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lFrameImage, Properties.Resources.EditPasteImageSource);
				}
				else
				{
					pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lFrameImage, Global.TitleImage (lFrameImage), Properties.Resources.EditPasteImageSource);
				}
			}
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

			CausesValidation = false;
			TableLayoutSample.SuspendLayout ();

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
				NumericOffsetX.Enabled = !Program.FileIsReadOnly;
				NumericOffsetY.Enabled = !Program.FileIsReadOnly;

				PanelImageClip.ClientSize = PictureBoxFrameSample.ClientSize;
				PictureBoxImageSample.SuspendLayout ();
				PictureBoxImageSample.ClientSize = PictureBoxFrameSample.ImageToClient (lBitmap.Size);
				PictureBoxImageSample.Location = PictureBoxFrameSample.ImageToClient (pFrameImage.Offset);
				PictureBoxImageSample.Image = lBitmap;
				PictureBoxImageSample.ResumeLayout (true);
				TableLayoutSample.Enabled = !Program.FileIsReadOnly;
			}

			TableLayoutSample.ResumeLayout (true);
			ButtonShiftUp.Width = ToolStripShiftUp.Width;
			ButtonShiftDown.Width = ToolStripShiftDown.Width;
			ButtonShiftLeft.Height = ToolStripShiftLeft.Height;
			ButtonShiftRight.Height = ToolStripShiftRight.Height;

			CausesValidation = Visible;
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
				TextBoxSoundFile.Enabled = !Program.FileIsReadOnly;
				ToolStripSound.Enabled = true;
				ToolStripSoundFile.Enabled = !Program.FileIsReadOnly;

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

		private RepeatingUpdate<UpdateFrameImage,FileFrameImage> mRepeatUpdateFrameImage = new RepeatingUpdate<UpdateFrameImage, FileFrameImage> ();

		private UndoableUpdate UpdateSelectedImage (FileFrameImage pFrameImage)
		{
			if (!IsEmpty && !Program.FileIsReadOnly && (pFrameImage != null))
			{
				String				lFilePath = pFrameImage.ImageFilePath;
				UpdateFrameImage	lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					lUpdate = new UpdateFrameImage (pFrameImage, false);
					lUpdate.ImageFilePath = lFilePath;
					if (UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameImage, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		internal UndoableUpdate DeleteSelectedImage (FileFrameImage pFrameImage, Boolean pForClipboard)
		{
			if (!IsEmpty && !Program.FileIsReadOnly && (pFrameImage != null))
			{
				AddDeleteFrameImage	lUpdate = new AddDeleteFrameImage (pFrameImage, pForClipboard);

				if (AddDeleteFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteFrameImage, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedImage (FileFrameImage pFrameImage, FileFrameImage pPasteImage)
		{
			if (!IsEmpty && !Program.FileIsReadOnly && (pPasteImage != null))
			{
				if (pFrameImage == null)
				{
					AddDeleteFrameImage	lUpdate = new AddDeleteFrameImage (mFrame, String.Empty, true);

					lUpdate = lUpdate.Apply () as AddDeleteFrameImage;
					if (lUpdate != null)
					{
						pPasteImage.CopyTo (lUpdate.Target);
						lUpdate = lUpdate.Apply () as AddDeleteFrameImage;
					}
					if ((lUpdate != null) && AddDeleteFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteFrameImage, this))
					{
						return lUpdate;
					}
				}
				else
				{
					UpdateFrameImage	lUpdate = new UpdateFrameImage (pFrameImage, true);

					pPasteImage.CopyTo (pFrameImage);
					lUpdate = lUpdate.Apply () as UpdateFrameImage;
					if ((lUpdate != null) && UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameImage, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateAnimation			lUpdateAnimation = sender as UpdateAnimation;
				UpdateAnimationFrame	lUpdateFrame = sender as UpdateAnimationFrame;
				AddDeleteFrameImage		lAddDeleteImage = sender as AddDeleteFrameImage;
				ReorderFrameImage		lReorderImage = sender as ReorderFrameImage;
				UpdateFrameImage		lUpdateImage = sender as UpdateFrameImage;

				if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == this.Animation) && lUpdateAnimation.NameChanged)
				{
					ShowFrameName ();
				}
				else if ((lUpdateFrame != null) && (lUpdateFrame.Target == mFrame))
				{
					ShowFrameDuration ();
					ShowFrameSound ();
					if (lUpdateFrame.ForClipboard)
					{
						ShowFrameImages ();
						ShowSelectedImage ();
						ShowFrameSample ();
					}
				}
				else if ((lAddDeleteImage != null) && (lAddDeleteImage.Frame == mFrame))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = lAddDeleteImage.ImageNdx;
					ShowSelectedImage ();
					ShowFrameSample ();
				}
				else if ((lReorderImage != null) && (lReorderImage.Frame == mFrame))
				{
					ShowFrameImages ();
					ListViewImages.SelectedIndex = mFrame.Images.IndexOf (lReorderImage.Target);
					ShowSelectedImage ();
					ShowFrameSample ();
				}
				else if ((lUpdateImage != null) && (lUpdateImage.Frame == mFrame))
				{
					RefreshImageItem (lUpdateImage.Target, mFrame.Images.IndexOf (lUpdateImage.Target));
					ShowSelectedImage ();
					ShowFrameSample ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void NumericDuration_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame	lUpdate = new UpdateAnimationFrame (mFrame, false);

				lUpdate.Duration = (UInt16)NumericDuration.Value;
				UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxSoundFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxSoundFile.Modified) && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame	lUpdate = new UpdateAnimationFrame (mFrame, false);

				lUpdate.SoundFilePath = TextBoxSoundFile.Text;
				UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this);
			}
		}

		private void ButtonSoundImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				String	lFilePath = mFrame.SoundFilePath;

				if (OpenFileDialogEx.OpenSoundFile (ref lFilePath))
				{
					UpdateAnimationFrame	lUpdate = new UpdateAnimationFrame (mFrame, false);

					lUpdate.SoundFilePath = lFilePath;
					UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				String				lFilePath = String.Empty;
				AddDeleteFrameImage	lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (!String.IsNullOrEmpty (lFilePath))
					{
						lUpdate = new AddDeleteFrameImage (mFrame, lFilePath, false);
						AddDeleteFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteFrameImage, this);
					}
				}
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			DeleteSelectedImage (GetSelectedImage (false), false);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonMoveUp_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				int					lSelNdx = ListViewImages.GetSelectedIndex (false);
				ReorderFrameImage	lUpdate;

				if ((lFrameImage != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderFrameImage (lFrameImage, lSelNdx - 1);
					ReorderFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as ReorderFrameImage, this);
				}
			}
		}

		private void ButtonMoveDown_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage		lFrameImage = GetSelectedImage (false);
				int					lSelNdx = ListViewImages.GetSelectedIndex (false);
				ReorderFrameImage	lUpdate;

				if ((lFrameImage != null) && (lSelNdx >= 0) && (lSelNdx < mFrame.Images.Count - 1))
				{
					lUpdate = new ReorderFrameImage (lFrameImage, lSelNdx + 1);
					ReorderFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as ReorderFrameImage, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericOffsetX_Validated (object sender, EventArgs e)
		{
			if (CausesValidation)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (!IsEmpty && !Program.FileIsReadOnly && (lFrameImage != null))
				{
					UpdateFrameImage	lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new Point ((int)NumericOffsetX.Value, lFrameImage.Offset.Y);
					UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameImage, this);
				}
			}
		}

		private void NumericOffsetY_Validated (object sender, EventArgs e)
		{
			if (CausesValidation)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (!IsEmpty && !Program.FileIsReadOnly && (lFrameImage != null))
				{
					UpdateFrameImage	lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new Point (lFrameImage.Offset.X, (int)NumericOffsetY.Value);
					UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateFrameImage, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonShiftUp_Click (object sender, EventArgs e)
		{
			FileFrameImage	lFrameImage = GetSelectedImage (false);

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y - 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X, lFrameImage.Offset.Y + 1);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X - 1, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
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

			if (!IsEmpty && !Program.FileIsReadOnly && (lFrameImage != null))
			{
				UpdateFrameImage	lUpdate = new UpdateFrameImage (lFrameImage, false);

				lUpdate.Offset = new Point (lFrameImage.Offset.X + 1, lFrameImage.Offset.Y);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				mRepeatUpdateFrameImage.PutUpdate (lUpdate, ButtonShiftRight.ClickRepeatNum);
			}
		}
		private void ButtonShiftRight_RepeatEnd (object sender, EventArgs e)
		{
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxTransparent_CheckedChanged (object sender, EventArgs e)
		{
			ShowSelectedImage ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, Global.CanEditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewImages.ContainsFocus)
			{
				ShowEditState (e);
			}
		}

		internal void MainForm_EditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewImages.ContainsFocus)
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

		internal void MainForm_EditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewImages.ContainsFocus)
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

		internal void MainForm_EditDelete (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewImages.ContainsFocus)
			{
				FileFrameImage	lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					e.IsUsed = true;
					DeleteSelectedImage (lFrameImage, false);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewImages.ContainsFocus)
			{
				if (e.PasteObject is FileFrameImage)
				{
					e.IsUsed = true;
					PasteSelectedImage (GetSelectedImage (false), e.PasteObject as FileFrameImage);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal void MainForm_EditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e);

			if (!IsEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, ListViewImages))
			{
				ToolStripMenuItem	lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonChooseFile.Text, ButtonChooseFile.Image, ButtonOpen_Click));
				lMenuItem.Enabled = ButtonChooseFile.Enabled;
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;

				e.ContextMenu.Items.Add (new ToolStripSeparator ());
				e.ContextMenu.Items.Add (lMenuItem = new ToolStripMenuItem (ButtonMoveUp.Text, ButtonMoveUp.Image, ButtonMoveUp_Click));
				lMenuItem.Enabled = ButtonMoveUp.Enabled;
				e.ContextMenu.Items.Add (lMenuItem = new ToolStripMenuItem (ButtonMoveDown.Text, ButtonMoveDown.Image, ButtonMoveDown_Click));
				lMenuItem.Enabled = ButtonMoveDown.Enabled;
			}
		}

		#endregion
	}
}
