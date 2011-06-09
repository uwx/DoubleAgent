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
using System.IO;
using System.Media;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Global;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class FramePanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		private SoundPlayer mSoundPreview = null;

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				if (FilePart is ResolveAnimationFrame)
				{
					(FilePart as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeFrame;
				}
				Frame = (FilePart is ResolveAnimationFrame) ? (FilePart as ResolveAnimationFrame).Target : null;

				ShowFrameName ();
				ShowFrameDuration ();
				ShowFrameSample ();
				ShowFrameImages ();
				ShowFrameSound ();
			}
		}

		protected FileAnimationFrame Frame
		{
			get;
			set;
		}

		protected FileAnimation Animation
		{
			get
			{
				return (Frame == null) ? null : Frame.Animation;
			}
		}

		protected String FrameTitle
		{
			get
			{
				return Titles.Frame (Frame);
			}
		}

		public override Boolean IsPanelEmpty
		{
			get
			{
				return base.IsPanelEmpty || (Frame == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameName ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					TextBoxFrameName.Clear ();
					TextBoxFrameName.IsEnabled = false;
				}
				else
				{
					TextBoxFrameName.Text = Titles.FrameAnimation (Frame);
					TextBoxFrameName.IsEnabled = true;
				}
			}
		}

		private void ShowFrameDuration ()
		{
			if (IsPanelEmpty)
			{
				NumericDuration.Clear ();
				NumericDuration.IsEnabled = false;
			}
			else
			{
				NumericDuration.Value = Frame.Duration;
				NumericDuration.IsEnabled = !Program.FileIsReadOnly;
			}
			NumericDuration.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameImage (FileFrameImage pFrameImage)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (pFrameImage == null)
				{
					NumericOffsetX.Clear ();
					NumericOffsetY.Clear ();
					NumericOffsetX.IsEnabled = false;
					NumericOffsetY.IsEnabled = false;
					LabelOffsetX.IsEnabled = false;
					LabelOffsetY.IsEnabled = false;
				}
				else
				{
					NumericOffsetX.Value = pFrameImage.Offset.X;
					NumericOffsetY.Value = pFrameImage.Offset.Y;
					NumericOffsetX.IsEnabled = !Program.FileIsReadOnly;
					NumericOffsetY.IsEnabled = !Program.FileIsReadOnly;
					LabelOffsetX.IsEnabled = !Program.FileIsReadOnly;
					LabelOffsetY.IsEnabled = !Program.FileIsReadOnly;
				}
				NumericOffsetX.IsModified = false;
				NumericOffsetY.IsModified = false;

				ShowFrameImageSample (pFrameImage);
			}
		}

		private void ShowSelectedImage ()
		{
			ShowFrameImage (GetSelectedImage (true));

			ButtonAdd.IsEnabled = CanAddImage;
			ButtonDelete.IsEnabled = CanDeleteImage;
			ButtonChooseFile.IsEnabled = CanChooseImageFile;
			ButtonMoveUp.IsEnabled = CanMoveImageUp;
			ButtonMoveDown.IsEnabled = CanMoveImageDown;

			ButtonDelete.SetTipText (DeleteImageTitle);
			ButtonChooseFile.SetTipText (ChooseImageFileTitle);
			ButtonMoveUp.SetTipText (MoveImageUpTitle);
			ButtonMoveDown.SetTipText (MoveImageDownTitle);
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean CanAddImage
		{
			get
			{
				return !IsPanelEmpty && !Program.FileIsReadOnly;
			}
		}

		private Boolean CanDeleteImage
		{
			get
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);
				return !IsPanelEmpty && !Program.FileIsReadOnly && (lFrameImage != null);
			}
		}
		private String DeleteImageTitle
		{
			get
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);
				return CanDeleteImage ? String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Image (lFrameImage)) : AppResources.Resources.EditDelete.NoMenuPrefix ();
			}
		}

		private Boolean CanChooseImageFile
		{
			get
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);
				return !IsPanelEmpty && !Program.FileIsReadOnly && (lFrameImage != null);
			}
		}
		private String ChooseImageFileTitle
		{
			get
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);
				return CanChooseImageFile ? String.Format (AppResources.Resources.EditChooseThisFile.NoMenuPrefix (), Titles.Image (lFrameImage)) : AppResources.Resources.EditChooseFile.NoMenuPrefix ();
			}
		}

		private Boolean CanMoveImageUp
		{
			get
			{
				int lSelectedIndex = ListViewImages.SelectedIndex;
				return !IsPanelEmpty && !Program.FileIsReadOnly && (lSelectedIndex > 0) && (lSelectedIndex < ListViewImages.Items.Count);
			}
		}
		private String MoveImageUpTitle
		{
			get
			{
				FileFrameImage lFrameImage = CanMoveImageUp ? GetSelectedImage (false) : null;
				return String.Format (AppResources.Resources.EditMoveImageUp.NoMenuPrefix (), Titles.Image (lFrameImage));
			}
		}

		private Boolean CanMoveImageDown
		{
			get
			{
				int lSelectedIndex = ListViewImages.SelectedIndex;
				return !IsPanelEmpty && !Program.FileIsReadOnly && (lSelectedIndex >= 0) && (lSelectedIndex < ListViewImages.Items.Count - 1);
			}
		}
		private String MoveImageDownTitle
		{
			get
			{
				FileFrameImage lFrameImage = CanMoveImageDown ? GetSelectedImage (false) : null;
				return String.Format (AppResources.Resources.EditMoveImageDown.NoMenuPrefix (), Titles.Image (lFrameImage));
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private FileFrameImage GetSelectedImage (Boolean pIncludeFocus)
		{
			return GetSelectedImage (ListViewImages.GetSelectedIndex (pIncludeFocus));
		}
		private FileFrameImage GetSelectedImage (int pImageNdx)
		{
			FileFrameImage lFrameImage = null;

			if ((Frame != null) && (Frame.Images != null))
			{
				if ((pImageNdx >= 0) && (pImageNdx < Frame.Images.Count))
				{
					lFrameImage = Frame.Images[pImageNdx];
				}
			}
			return lFrameImage;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameSound ()
		{
			mSoundPreview = null;

			if (IsPanelEmpty)
			{
				TextBoxSoundFile.Clear ();
				TextBoxSoundFile.IsEnabled = false;
				ButtonSoundPreview.IsEnabled = false;
				ButtonSoundImport.IsEnabled = false;
				ButtonSoundExport.IsEnabled = false;
			}
			else
			{
				TextBoxSoundFile.Text = Frame.SoundFilePath;
				TextBoxSoundFile.IsEnabled = !Program.FileIsReadOnly;
				ButtonSoundImport.IsEnabled = !Program.FileIsReadOnly;
				ButtonSoundExport.IsEnabled = false;

				if (Frame.SoundNdx >= 0)
				{
					try
					{
						Byte[] lSound = CharacterFile.GetSound (Frame.SoundNdx);
						MemoryStream lStream = new MemoryStream (lSound);

						mSoundPreview = new SoundPlayer (lStream);
					}
					catch
					{
					}
				}
			}

			ButtonSoundPreview.IsEnabled = (mSoundPreview != null);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Updates

		private RepeatingUpdate<UpdateFrameImage, FileFrameImage> mRepeatUpdateFrameImage = new RepeatingUpdate<UpdateFrameImage, FileFrameImage> ();

		private UndoableUpdate UpdateSelectedImage (FileFrameImage pFrameImage)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly && (pFrameImage != null))
			{
				String lFilePath = pFrameImage.ImageFilePath;
				UpdateFrameImage lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					lUpdate = new UpdateFrameImage (pFrameImage, false);
					lUpdate.ImageFilePath = lFilePath;
					if (UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameImage, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		internal UndoableUpdate DeleteSelectedImage (FileFrameImage pFrameImage, Boolean pForClipboard)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly && (pFrameImage != null))
			{
				AddDeleteFrameImage lUpdate = new AddDeleteFrameImage (pFrameImage, pForClipboard);

				if (AddDeleteFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteFrameImage, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedImage (FileFrameImage pFrameImage, FileFrameImage pPasteImage)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly && (pPasteImage != null))
			{
				if (pFrameImage == null)
				{
					AddDeleteFrameImage lUpdate = new AddDeleteFrameImage (Frame, String.Empty, true);

					lUpdate = lUpdate.Apply () as AddDeleteFrameImage;
					if (lUpdate != null)
					{
						pPasteImage.CopyTo (lUpdate.Target);
						lUpdate = lUpdate.Apply () as AddDeleteFrameImage;
					}
					if ((lUpdate != null) && AddDeleteFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteFrameImage, this))
					{
						return lUpdate;
					}
				}
				else
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (pFrameImage, true);

					pPasteImage.CopyTo (pFrameImage);
					lUpdate = lUpdate.Apply () as UpdateFrameImage;
					if ((lUpdate != null) && UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameImage, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void AddNewImage ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				String lFilePath = String.Empty;
				AddDeleteFrameImage lUpdate;

				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (!String.IsNullOrEmpty (lFilePath))
					{
						lUpdate = new AddDeleteFrameImage (Frame, lFilePath, false);
						AddDeleteFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteFrameImage, this);
					}
				}
			}
		}

		private void MoveImageUp ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);
				int lSelNdx = ListViewImages.SelectedIndex;
				ReorderFrameImage lUpdate;

				if ((lFrameImage != null) && (lSelNdx > 0))
				{
					lUpdate = new ReorderFrameImage (lFrameImage, lSelNdx - 1);
					ReorderFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as ReorderFrameImage, this);
				}
			}
		}

		private void MoveImageDown ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);
				int lSelNdx = ListViewImages.SelectedIndex;
				ReorderFrameImage lUpdate;

				if ((lFrameImage != null) && (lSelNdx >= 0) && (lSelNdx < Frame.Images.Count - 1))
				{
					lUpdate = new ReorderFrameImage (lFrameImage, lSelNdx + 1);
					ReorderFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as ReorderFrameImage, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool HandleCanEditCopy (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs))
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					pEventArgs.PutCopyTitle (Titles.ImageFrameAnimation (lFrameImage).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCopy (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs))
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					pEventArgs.PutCopyObject (lFrameImage);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditCut (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					pEventArgs.PutCutTitle (Titles.ImageFrameAnimation (lFrameImage).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					if (pEventArgs.PutCopyObject (lFrameImage))
					{
						DeleteSelectedImage (lFrameImage, true);
					}
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditDelete (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					pEventArgs.PutDeleteTitle (Titles.ImageFrameAnimation (lFrameImage).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					DeleteSelectedImage (lFrameImage, false);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditPaste (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs) && !Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileFrameImage))
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage == null)
				{
					pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lFrameImage, AppResources.Resources.EditPasteImageSource));
				}
				else
				{
					pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lFrameImage, Titles.Image (lFrameImage), AppResources.Resources.EditPasteImageSource));
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewImages, pEventArgs) && !Program.FileIsReadOnly)
			{
				if (pEventArgs.GetPasteObject () is FileFrameImage)
				{
					PasteSelectedImage (GetSelectedImage (false), pEventArgs.GetPasteObject () as FileFrameImage);
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleDurationChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

				lUpdate.Duration = (UInt16)NumericDuration.Value;
				UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this);
			}
			NumericDuration.IsModified = false;
		}

		private void HandleOffsetXChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new System.Drawing.Point ((int)NumericOffsetX.Value, lFrameImage.Offset.Y);
					UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameImage, this);
				}
			}
			NumericOffsetX.IsModified = false;
		}

		private void HandleOffsetYChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new System.Drawing.Point (lFrameImage.Offset.X, (int)NumericOffsetY.Value);
					UpdateFrameImage.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameImage, this);
				}
			}
			NumericOffsetY.IsModified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleShiftUpClick (int pRepeatNum)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new System.Drawing.Point (lFrameImage.Offset.X, lFrameImage.Offset.Y - 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
					mRepeatUpdateFrameImage.PutUpdate (lUpdate, pRepeatNum);
				}
			}
		}

		private void HandleShiftDownClick (int pRepeatNum)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new System.Drawing.Point (lFrameImage.Offset.X, lFrameImage.Offset.Y + 1);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
					mRepeatUpdateFrameImage.PutUpdate (lUpdate, pRepeatNum);
				}
			}
		}

		private void HandleShiftLeftClick (int pRepeatNum)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new System.Drawing.Point (lFrameImage.Offset.X - 1, lFrameImage.Offset.Y);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
					mRepeatUpdateFrameImage.PutUpdate (lUpdate, pRepeatNum);
				}
			}
		}

		private void HandleShiftRightClick (int pRepeatNum)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				FileFrameImage lFrameImage = GetSelectedImage (false);

				if (lFrameImage != null)
				{
					UpdateFrameImage lUpdate = new UpdateFrameImage (lFrameImage, false);

					lUpdate.Offset = new System.Drawing.Point (lFrameImage.Offset.X + 1, lFrameImage.Offset.Y);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainWindow.OnUpdateApplied);
					mRepeatUpdateFrameImage.PutUpdate (lUpdate, pRepeatNum);
				}
			}
		}

		private void HandleShiftComplete ()
		{
			mRepeatUpdateFrameImage.EndUpdate ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleSoundFileChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

				lUpdate.SoundFilePath = TextBoxSoundFile.Text;
				UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this);
			}
			TextBoxSoundFile.IsModified = false;
		}

		private void HandleSoundImport ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				String lFilePath = Frame.SoundFilePath;

				if (OpenFileDialogEx.OpenSoundFile (ref lFilePath))
				{
					UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

					lUpdate.SoundFilePath = lFilePath;
					UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			UpdateAnimationFrame lUpdateFrame = pUpdate as UpdateAnimationFrame;
			AddDeleteFrameImage lAddDeleteImage = pUpdate as AddDeleteFrameImage;
			ReorderFrameImage lReorderImage = pUpdate as ReorderFrameImage;
			UpdateFrameImage lUpdateImage = pUpdate as UpdateFrameImage;

			if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == this.Animation) && lUpdateAnimation.NameChanged)
			{
				ShowFrameName ();
			}
			else if ((lUpdateFrame != null) && (lUpdateFrame.Target == Frame))
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
			else if ((lAddDeleteImage != null) && (lAddDeleteImage.Frame == Frame))
			{
				ShowFrameImages ();
				ListViewImages.SelectedIndex = lAddDeleteImage.ImageNdx;
				ShowSelectedImage ();
				ShowFrameSample ();
			}
			else if ((lReorderImage != null) && (lReorderImage.Frame == Frame))
			{
				ShowFrameImages ();
				ListViewImages.SelectedIndex = Frame.Images.IndexOf (lReorderImage.Target);
				ShowSelectedImage ();
				ShowFrameSample ();
			}
			else if ((lUpdateImage != null) && (lUpdateImage.Frame == Frame))
			{
				RefreshImageItem (lUpdateImage.Target, Frame.Images.IndexOf (lUpdateImage.Target));
				ShowSelectedImage ();
				ShowFrameSample ();
			}
		}

		#endregion
	}
}
