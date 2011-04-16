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
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class AnimationsForm : UserControl
	{
		private CharacterFile	mCharacterFile = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationsForm ()
		{
			InitializeComponent ();

			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.InitializationComplete += new EventHandler (MainForm_InitializationComplete);
			}
		}

		void MainForm_InitializationComplete (object sender, EventArgs e)
		{
			MainForm.Singleton.PanelAnimation.AnimationNameChanged += new AnimationNameChangedEventHandler (PanelAnimation_AnimationNameChanged);
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
				ShowAnimationNames ();
				ShowFrameProperties ();
				ShowPalette ();
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return (mCharacterFile == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event AnimationAddedEventHandler AnimationAdded;
		public event AnimationRemovedEventHandler AnimationRemoved;
		public event GoToAnimationEventHandler GoToAnimation;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowAnimationNames ()
		{
			if (IsEmpty)
			{
				ListViewAnimations.Items.Clear ();
				ListViewAnimations.Enabled = false;
				TextBoxNewName.ResetText ();
				TextBoxNewName.Enabled = false;
				ToolStripAnimations.Enabled = false;
			}
			else
			{
				String[]	lAnimations = mCharacterFile.GetAnimationNames ();
				int			lNdx;

				ListViewAnimations.BeginUpdate ();
				if ((lAnimations == null) || (lAnimations.Length <= 0))
				{
					ListViewAnimations.Items.Clear ();
				}
				else
				{
					ListViewAnimations.UpdateItemCount (lAnimations.Length);

					for (lNdx = 0; lNdx < lAnimations.Length; lNdx++)
					{
						ListViewAnimations.Items[lNdx].Text = lAnimations[lNdx];
					}
				}
				ListViewAnimations.EndUpdate ();
				ListViewAnimations.ArrangeIcons ();

				ListViewAnimations.Enabled = true;
				TextBoxNewName.Enabled = !MainForm.Singleton.FileIsReadOnly;
				ToolStripAnimations.Enabled = !MainForm.Singleton.FileIsReadOnly;
				ButtonAdd.Enabled = !MainForm.Singleton.FileIsReadOnly && (!String.IsNullOrEmpty (TextBoxNewName.Text));
				ButtonDelete.Enabled = !MainForm.Singleton.FileIsReadOnly && (ListViewAnimations.SelectedIndex >= 0);
			}
		}

		private void ShowFrameProperties ()
		{
			if (IsEmpty)
			{
				NumericFrameWidth.ResetText ();
				NumericFrameHeight.ResetText ();

				NumericFrameWidth.Enabled = false;
				NumericFrameHeight.Enabled = false;
			}
			else
			{
				NumericFrameWidth.Value = mCharacterFile.Header.ImageSize.Width;
				NumericFrameHeight.Value = mCharacterFile.Header.ImageSize.Height;

				NumericFrameWidth.Enabled = !MainForm.Singleton.FileIsReadOnly;
				NumericFrameHeight.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}
		}

		private void ShowPalette ()
		{
			System.Drawing.Imaging.ColorPalette	lPalette = null;

			if (mCharacterFile == null)
			{
				TextBoxPaletteFile.ResetText ();
				TextBoxPaletteFile.Enabled = false;
				ToolStripPaletteFile.Enabled = false;
			}
			else
			{
				TextBoxPaletteFile.Text = mCharacterFile.PaletteFilePath;
				TextBoxPaletteFile.Enabled = !MainForm.Singleton.FileIsReadOnly;
				ToolStripPaletteFile.Enabled = !MainForm.Singleton.FileIsReadOnly;
				lPalette = mCharacterFile.Header.Palette;
			}
			if ((lPalette != null) && (lPalette.Entries.Length == 256))
			{
				System.Drawing.Bitmap				lBitmap = new System.Drawing.Bitmap (256, 256, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
				System.Drawing.Color[]				lColors = lPalette.Entries;
				int									lColorNdx = 0;
				System.Drawing.Imaging.BitmapData	lData;
				Byte[]								lBytes;

				try
				{
					lBitmap.Palette = lPalette;
					lData = lBitmap.LockBits (new Rectangle (0, 0, lBitmap.Width, lBitmap.Height), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
					lBytes = new byte[lData.Stride * lData.Height];
					System.Runtime.InteropServices.Marshal.Copy (lData.Scan0, lBytes, 0, lData.Stride * lData.Height);

					for (lColorNdx = 0; lColorNdx < lData.Stride * lData.Height; lColorNdx++)
					{
						lBytes[lColorNdx] = (Byte)((lColorNdx / (lData.Stride * 16) * 16) + ((lColorNdx % lData.Stride) / 16));
					}

					System.Runtime.InteropServices.Marshal.Copy (lBytes, 0, lData.Scan0, lData.Stride * lData.Height);
					lBitmap.UnlockBits (lData);
				}
				catch
				{
				}

				PictureBoxPalette.Image = lBitmap;
				LabelTransparencySample.BackColor = lColors[mCharacterFile.Header.Transparency];
				LabelTransparencySample.ForeColor = (LabelTransparencySample.BackColor.GetBrightness () > 0.5) ? Color.Black : Color.White;
				LabelTransparencySample.Text = mCharacterFile.Header.Transparency.ToString ();
				LabelTransparency.Enabled = true;
			}
			else
			{
				PictureBoxPalette.Image = null;
				LabelTransparencySample.BackColor = BackColor;
				LabelTransparencySample.ResetText ();
				LabelTransparency.Enabled = false;
			}
			LabelColorSample.Visible = false;
			LabelTransparencyClick.Visible = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowPaletteMouseColor (Point pMousePos)
		{
			System.Drawing.Imaging.ColorPalette	lPalette = null;
			int									lColorNdx;

			if (mCharacterFile != null)
			{
				lPalette = mCharacterFile.Header.Palette;
			}
			if (lPalette != null)
			{
				lColorNdx = PaletteMouseColorNdx (pMousePos);

				LabelColorSample.BackColor = lPalette.Entries[lColorNdx];
				LabelColorSample.ForeColor = (LabelColorSample.BackColor.GetBrightness () > 0.5) ? Color.Black : Color.White;
				LabelColorSample.Text = lColorNdx.ToString ();
				LabelColorSample.Visible = true;
				LabelTransparencyClick.Visible = !MainForm.Singleton.FileIsReadOnly;
			}
			else
			{
				LabelColorSample.Visible = false;
				LabelTransparencyClick.Visible = false;
			}
		}

		private int PaletteMouseColorNdx (Point pMousePos)
		{
			System.Drawing.Imaging.ColorPalette	lPalette = null;
			System.Drawing.Point				lColorPos;

			if (mCharacterFile != null)
			{
				lPalette = mCharacterFile.Header.Palette;
			}
			if (lPalette != null)
			{
				lColorPos = new System.Drawing.Point ();
				lColorPos.X = pMousePos.X * 16 / PictureBoxPalette.Width;
				lColorPos.Y = pMousePos.Y * 16 / PictureBoxPalette.Height;
				lColorPos.X = Math.Min (Math.Max (lColorPos.X, 0), 16);
				lColorPos.Y = Math.Min (Math.Max (lColorPos.Y, 0), 16);

				return (lColorPos.Y * 16) + lColorPos.X;
			}
			else
			{
				return -1;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class AddDeleteAnimation : UndoableAddDelete<FileAnimation>
		{
			public AddDeleteAnimation (CharacterFile pCharacterFile, String pAnimationName)
				: base (pCharacterFile)
			{
				this.AnimationName = pAnimationName;
			}

			public AddDeleteAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation, Boolean pForClipboard)
				: base (pCharacterFile, pAnimation, pForClipboard)
			{
				this.AnimationName = pAnimation.Name;
			}

			public String AnimationName
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Program.TitleAnimation (this.AnimationName);
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteAnimation	lApplied = null;
				FileAnimation		lAnimation = null;

				if (IsDelete)
				{
					if (this.CharacterFile.Gestures.Contains (this.AnimationName))
					{
						lAnimation = this.CharacterFile.Gestures[this.AnimationName];

						if (this.CharacterFile.Gestures.Remove (this.AnimationName))
						{
							lApplied = new AddDeleteAnimation (this.CharacterFile, lAnimation, this.ForClipboard);
							lApplied.IsDelete = false;
							lApplied.IsRedo = !IsRedo;
						}
					}
				}
				else
				{
					lAnimation = this.CharacterFile.Gestures.Add (this.AnimationName);

					if (lAnimation != null)
					{
						if (this.Target != null)
						{
							this.Target.CopyTo (lAnimation);
						}
						lApplied = new AddDeleteAnimation (this.CharacterFile, lAnimation, this.ForClipboard);
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

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateImageSize : UndoableUpdate<CharacterFile>
		{
			public UpdateImageSize (CharacterFile pCharacterFile, Size pImageSize)
				: base (pCharacterFile, pCharacterFile)
			{
				this.ImageSize = pImageSize;
			}

			public Size ImageSize
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Program.TitleAnimation ((FileAnimation)null);
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return " " + Properties.Resources.UndoAnimationSize;
				}
			}

			public override UndoUnit Apply ()
			{
				if (CharacterFile.Header.ImageSize != this.ImageSize)
				{
					Size	lSwap = CharacterFile.Header.ImageSize;
					CharacterFile.Header.ImageSize = this.ImageSize;
					this.ImageSize = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		internal class UpdateCharacterPalette : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterPalette (CharacterFile pCharacterFile, String pPaletteFilePath)
				: base (pCharacterFile, pCharacterFile)
			{
				this.PaletteFilePath = pPaletteFilePath;
			}

			public String PaletteFilePath
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Properties.Resources.UndoPalette;
				}
			}

			public override UndoUnit Apply ()
			{
				if (!CharacterFile.PaletteFilePath.Equals (this.PaletteFilePath))
				{
					String	lSwap = CharacterFile.PaletteFilePath;
					CharacterFile.PaletteFilePath = this.PaletteFilePath;
					this.PaletteFilePath = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal Boolean DeleteSelectedAnimation (FileAnimation pAnimation, Boolean pForClipboard)
		{
			Boolean	lRet = false;

			if ((pAnimation != null) && !MainForm.Singleton.FileIsReadOnly)
			{
				AddDeleteAnimation	lUpdate = new AddDeleteAnimation (mCharacterFile, pAnimation, pForClipboard);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableAnimation_Applied);
				lRet = AddDeleteAnimation.PutUndo (lUpdate.Apply () as AddDeleteAnimation);
			}
			return lRet;
		}

		internal Boolean PasteSelectedAnimation (FileAnimationFrame pPasteAnimation)
		{
			Boolean	lRet = false;

			if ((mCharacterFile != null) && (pPasteAnimation != null) && !MainForm.Singleton.FileIsReadOnly)
			{
			}
			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxNewName_TextChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				ButtonAdd.Enabled = !String.IsNullOrEmpty (TextBoxNewName.Text);
			}
		}

		private void ListViewAnimations_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				ButtonDelete.Enabled = (ListViewAnimations.SelectedIndex >= 0);
			}
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToAnimation != null))
			{
				ListViewItem	lItem = ListViewAnimations.SelectedItem;
				FileAnimation	lAnimation = null;

				if ((lItem != null) && (mCharacterFile.Gestures.Contains (lItem.Text)))
				{
					lAnimation = mCharacterFile.Gestures[lItem.Text];
				}
				if (lAnimation != null)
				{
					try
					{
						GoToAnimation (this, new AnimationEventArgs (lAnimation));
					}
					catch
					{
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				String				lAnimationName = TextBoxNewName.Text.Trim ();
				AddDeleteAnimation	lUpdate;

				if (String.IsNullOrEmpty (lAnimationName))
				{
					System.Media.SystemSounds.Beep.Play ();
				}
				else if (mCharacterFile.Gestures.Contains (lAnimationName))
				{
					MessageBox.Show (String.Format (Properties.Resources.MsgDuplicateAnimation, lAnimationName), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
				else
				{
					lUpdate = new AddDeleteAnimation (mCharacterFile, lAnimationName);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableAnimation_Applied);
					lUpdate = lUpdate.Apply () as AddDeleteAnimation;
					if (lUpdate != null)
					{
						AddDeleteAnimation.PutUndo (lUpdate);
						ListViewAnimations.SelectedIndex = mCharacterFile.Gestures.IndexOf (lUpdate.Target);
					}
					else
					{
						System.Media.SystemSounds.Beep.Play ();
					}
				}
			}
		}

		private void ButtonDelete_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				ListViewItem	lSelItem = ListViewAnimations.SelectedItem;

				if ((lSelItem != null) && mCharacterFile.Gestures.Contains (lSelItem.Text))
				{
					int				lSelNdx = lSelItem.Index;
					FileAnimation	lAnimation = mCharacterFile.Gestures[lSelItem.Text];

					if (DeleteSelectedAnimation (lAnimation, false))
					{
						ListViewAnimations.SelectedIndex = lSelNdx;
					}
					else
					{
						System.Media.SystemSounds.Beep.Play ();
					}
				}
			}
		}

		private void UndoableAnimation_Applied (object sender, EventArgs e)
		{
			AddDeleteAnimation	lUpdate = sender as AddDeleteAnimation;

			if (!IsEmpty)
			{
				int	lSelNdx = ListViewAnimations.SelectedIndex;
				ShowAnimationNames ();
				ListViewAnimations.SelectedIndex = lSelNdx;
			}
			if (lUpdate != null)
			{
				if ((lUpdate.IsDelete) && (this.AnimationRemoved != null))
				{
					AnimationRemoved (this, new AnimationEventArgs (lUpdate.AnimationName));
				}
				if ((!lUpdate.IsDelete) && (this.AnimationAdded != null))
				{
					AnimationAdded (this, new AnimationEventArgs (lUpdate.AnimationName));
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameWidth_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateImageSize	lUpdate = new UpdateImageSize (mCharacterFile, new System.Drawing.Size ((int)NumericFrameWidth.Value, mCharacterFile.Header.ImageSize.Height));

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableFrameUpdate_Applied);
				UpdateImageSize.PutUndo (lUpdate.Apply () as UpdateImageSize);
			}
		}

		private void NumericFrameHeight_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateImageSize	lUpdate = new UpdateImageSize (mCharacterFile, new System.Drawing.Size (mCharacterFile.Header.ImageSize.Width, (int)NumericFrameHeight.Value));

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableFrameUpdate_Applied);
				UpdateImageSize.PutUndo (lUpdate.Apply () as UpdateImageSize);
			}
		}

		private void UndoableFrameUpdate_Applied (object sender, EventArgs e)
		{
			ShowFrameProperties ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxPaletteFile.Modified) && !IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateCharacterPalette	lUpdate = new UpdateCharacterPalette (mCharacterFile, TextBoxPaletteFile.Text);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoablePaletteUpdate_Applied);
				UpdateCharacterPalette.PutUndo (lUpdate.Apply () as UpdateCharacterPalette);
			}
		}

		private void ButtonPaletteImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				String					lFilePath = mCharacterFile.PaletteFilePath;
				UpdateCharacterPalette	lUpdate;

				if (OpenFileDialogEx.OpenPaletteFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterPalette (mCharacterFile, lFilePath);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoablePaletteUpdate_Applied);
					UpdateCharacterPalette.PutUndo (lUpdate.Apply () as UpdateCharacterPalette);
				}
			}
		}

		private void UndoablePaletteUpdate_Applied (object sender, EventArgs e)
		{
			ShowPalette ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PictureBoxPalette_MouseMove (object sender, MouseEventArgs e)
		{
			ShowPaletteMouseColor (e.Location);
		}

		private void PictureBoxPalette_MouseLeave (object sender, EventArgs e)
		{
			LabelColorSample.Visible = false;
			LabelTransparencyClick.Visible = false;
		}

		private void PictureBoxPalette_MouseClick (object sender, MouseEventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				int	lColorNdx = PaletteMouseColorNdx (e.Location);

				if (lColorNdx >= 0)
				{
					mCharacterFile.Header.Transparency = (Byte)lColorNdx;
					ShowPalette ();
					MainForm.Singleton.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal void PanelAnimation_AnimationNameChanged (object sender, AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowAnimationNames ();
			}
		}

		#endregion
	}
}
