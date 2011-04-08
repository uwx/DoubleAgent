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
				ShowAnimationNames ();
				ShowFrameProperties ();
				ShowPalette ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return (mCharacterFile == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public class AnimationEventArgs : EventArgs
		{
			internal AnimationEventArgs (String pAnimationName)
			{
				mAnimationName = pAnimationName;
			}

			public System.String AnimationName
			{
				get
				{
					return mAnimationName;
				}
			}

			private String	mAnimationName;
		}

		public delegate void AnimationAddedEvent (object sender, AnimationEventArgs e);
		public delegate void AnimationRemovedEvent (object sender, AnimationEventArgs e);

		public event AnimationAddedEvent AnimationAdded;
		public event AnimationRemovedEvent AnimationRemoved;

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
				TextBoxNewName.Enabled = !Program.MainForm.FileIsReadOnly;
				ToolStripAnimations.Enabled = !Program.MainForm.FileIsReadOnly;
				ButtonAdd.Enabled = !Program.MainForm.FileIsReadOnly && (!String.IsNullOrEmpty (TextBoxNewName.Text));
				ButtonRemove.Enabled = !Program.MainForm.FileIsReadOnly && (ListViewAnimations.SelectedIndex >= 0);
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

				NumericFrameWidth.Enabled = !Program.MainForm.FileIsReadOnly;
				NumericFrameHeight.Enabled = !Program.MainForm.FileIsReadOnly;
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
				TextBoxPaletteFile.Enabled = !Program.MainForm.FileIsReadOnly;
				ToolStripPaletteFile.Enabled = !Program.MainForm.FileIsReadOnly;
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
				LabelTransparencyClick.Visible = !Program.MainForm.FileIsReadOnly;
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

		internal class AddRemoveAnimation : UndoableAddRemove<FileAnimation>
		{
			public AddRemoveAnimation (CharacterFile pCharacterFile, String pAnimationName)
				: base (pCharacterFile)
			{
				this.AnimationName = pAnimationName;
			}

			public AddRemoveAnimation (CharacterFile pCharacterFile, FileAnimation pAnimation)
				: base (pCharacterFile, pAnimation)
			{
				this.AnimationName = pAnimation.Name;
			}

			public String AnimationName
			{
				get;
				private set;
			}

			public override UndoUnit Apply ()
			{
				AddRemoveAnimation	lApplied = null;
				FileAnimation		lAnimation = null;

				if (IsRemove)
				{
					if (this.CharacterFile.Gestures.Contains (this.AnimationName))
					{
						lAnimation = this.CharacterFile.Gestures[this.AnimationName];

						if (this.CharacterFile.Gestures.Remove (this.AnimationName))
						{
							lApplied = new AddRemoveAnimation (this.CharacterFile, lAnimation);
							lApplied.IsRemove = false;
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
						lApplied = new AddRemoveAnimation (this.CharacterFile, lAnimation);
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
				return String.Format ("{0} animation \"{1}\"", this.AddRemoveTitle, this.AnimationName);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateImageSize : UndoableUpdate
		{
			public UpdateImageSize (CharacterFile pCharacterFile, Size pImageSize)
				: base (pCharacterFile)
			{
				this.ImageSize = pImageSize;
			}

			public Size ImageSize
			{
				get;
				private set;
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

			public override string ToString ()
			{
				return "animation size";
			}
		}

		internal class UpdateCharacterPalette : UndoableUpdate
		{
			public UpdateCharacterPalette (CharacterFile pCharacterFile, String pPaletteFilePath)
				: base (pCharacterFile)
			{
				this.PaletteFilePath = pPaletteFilePath;
			}

			public String PaletteFilePath
			{
				get;
				private set;
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

			public override string ToString ()
			{
				return "palette";
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxNewName_TextChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				ButtonAdd.Enabled = !String.IsNullOrEmpty (TextBoxNewName.Text);
			}
		}

		private void ListViewAnimations_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				ButtonRemove.Enabled = (ListViewAnimations.SelectedIndex >= 0);
			}
		}

		private void ButtonAdd_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				String				lAnimationName = TextBoxNewName.Text.Trim ();
				AddRemoveAnimation	lUpdate;

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
					lUpdate = new AddRemoveAnimation (mCharacterFile, lAnimationName);
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableAnimation_Applied);
					lUpdate = lUpdate.Apply () as AddRemoveAnimation;
					if (lUpdate != null)
					{
						AddRemoveAnimation.PutUndo (lUpdate);
						ListViewAnimations.SelectedIndex = mCharacterFile.Gestures.IndexOf (lUpdate.Target);
					}
					else
					{
						System.Media.SystemSounds.Beep.Play ();
					}
				}
			}
		}

		private void ButtonRemove_Click (object sender, EventArgs e)
		{
			ListViewItem	lSelItem = ListViewAnimations.SelectedItem;

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lSelItem != null))
			{
				if (mCharacterFile.Gestures.Contains (lSelItem.Text))
				{
					int					lSelNdx = lSelItem.Index;
					FileAnimation		lAnimation = mCharacterFile.Gestures[lSelItem.Text];
					AddRemoveAnimation	lUpdate = new AddRemoveAnimation (mCharacterFile, lAnimation);

					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableAnimation_Applied);
					lUpdate = lUpdate.Apply () as AddRemoveAnimation;
					if (lUpdate != null)
					{
						AddRemoveAnimation.PutUndo (lUpdate);
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
			if (!IsEmpty)
			{
				int	lSelNdx = ListViewAnimations.SelectedIndex;

				ShowAnimationNames ();

				ListViewAnimations.SelectedIndex = lSelNdx;

				try
				{
					AddRemoveAnimation	lSender = (AddRemoveAnimation)sender;

					if (lSender != null)
					{
						if ((lSender.IsRemove) && (this.AnimationRemoved != null))
						{
							AnimationRemoved (this, new AnimationEventArgs (lSender.AnimationName));
						}
						if ((!lSender.IsRemove) && (this.AnimationAdded != null))
						{
							AnimationAdded (this, new AnimationEventArgs (lSender.AnimationName));
						}
					}
				}
				catch
				{
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameWidth_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UndoableUpdate	lUpdate = new UpdateImageSize (mCharacterFile, new System.Drawing.Size ((int)NumericFrameWidth.Value, mCharacterFile.Header.ImageSize.Height));

				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableFrameUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
			}
		}

		private void NumericFrameHeight_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UndoableUpdate	lUpdate = new UpdateImageSize (mCharacterFile, new System.Drawing.Size (mCharacterFile.Header.ImageSize.Width, (int)NumericFrameHeight.Value));

				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableFrameUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
			}
		}

		private void UndoableFrameUpdate_Applied (object sender, EventArgs e)
		{
			ShowFrameProperties ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxPaletteFile.Modified) && !IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UndoableUpdate	lUpdate = new UpdateCharacterPalette (mCharacterFile, TextBoxPaletteFile.Text);

				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoablePaletteUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
			}
		}

		private void ButtonPaletteImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				String			lFilePath = mCharacterFile.PaletteFilePath;
				UndoableUpdate	lUpdate;

				if (OpenFileDialogEx.OpenPaletteFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterPalette (mCharacterFile, lFilePath);
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoablePaletteUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
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
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				int	lColorNdx = PaletteMouseColorNdx (e.Location);

				if (lColorNdx >= 0)
				{
					mCharacterFile.Header.Transparency = (Byte)lColorNdx;
					ShowPalette ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public void PanelAnimation_AnimationNameChanged (object sender, AnimationForm.AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowAnimationNames ();
			}
		}

		#endregion
	}
}
