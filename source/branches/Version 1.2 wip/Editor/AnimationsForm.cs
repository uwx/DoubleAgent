using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
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
		#region Event Delegates

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
		#region Event Handlers

		private void NumericFrameWidth_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mCharacterFile.Header.ImageSize = new System.Drawing.Size ((int)NumericFrameWidth.Value, mCharacterFile.Header.ImageSize.Height);
				NumericFrameWidth.Value = mCharacterFile.Header.ImageSize.Width;
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void NumericFrameHeight_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mCharacterFile.Header.ImageSize = new System.Drawing.Size (mCharacterFile.Header.ImageSize.Width, (int)NumericFrameHeight.Value);
				NumericFrameHeight.Value = mCharacterFile.Header.ImageSize.Height;
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
				String			lAnimationName = TextBoxNewName.Text.Trim ();
				FileAnimation	lAnimation = null;

				if (String.IsNullOrEmpty (lAnimationName))
				{
					System.Media.SystemSounds.Beep.Play ();
				}
				else if (mCharacterFile.Gestures.Contains (lAnimationName))
				{
					MessageBox.Show (String.Format (Properties.Resources.MsgDuplicateAnimation, lAnimationName), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
				else if ((lAnimation = mCharacterFile.Gestures.Add (lAnimationName)) != null)
				{
					ShowAnimationNames ();
					ListViewAnimations.SelectedIndex = mCharacterFile.Gestures.IndexOf (lAnimation);
					if (AnimationAdded != null)
					{
						AnimationAdded (this, new AnimationEventArgs (lAnimation.Name));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
				else
				{
					System.Media.SystemSounds.Beep.Play ();
				}
			}
		}

		private void ButtonRemove_Click (object sender, EventArgs e)
		{
			ListViewItem	lSelItem = ListViewAnimations.SelectedItem;

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly && (lSelItem != null))
			{
				int		lSelNdx = lSelItem.Index;
				String	lAnimationName = lSelItem.Text;

				if (mCharacterFile.Gestures.Remove (lAnimationName))
				{
					ShowAnimationNames ();
					ListViewAnimations.SelectedIndex = lSelNdx;
					if (AnimationRemoved != null)
					{
						AnimationRemoved (this, new AnimationEventArgs (lAnimationName));
					}
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxPaletteFile.Modified) && !IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mCharacterFile.PaletteFilePath = TextBoxPaletteFile.Text;
				ShowPalette ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonPaletteImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				String	lFilePath = mCharacterFile.PaletteFilePath;

				if (OpenFileDialogEx.OpenPaletteFile (ref lFilePath))
				{
					mCharacterFile.PaletteFilePath = lFilePath;
					ShowPalette ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

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
