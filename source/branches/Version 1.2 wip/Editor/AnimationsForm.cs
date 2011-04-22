﻿/////////////////////////////////////////////////////////////////////////////
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
using AgentCharacterEditor.Updates;

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

			if (Program.MainForm != null)
			{
				Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
			}
		}

		private void AnimationsForm_VisibleChanged (object sender, EventArgs e)
		{
			if (Program.MainForm != null)
			{
				Program.MainForm.CanEdit -= new Global.EditEventHandler (MainForm_CanEdit);
				Program.MainForm.EditCopy -= new Global.EditEventHandler (MainForm_EditCopy);
				Program.MainForm.EditCut -= new Global.EditEventHandler (MainForm_EditCut);
				Program.MainForm.EditDelete -= new Global.EditEventHandler (MainForm_EditDelete);
				Program.MainForm.EditPaste -= new Global.EditEventHandler (MainForm_EditPaste);
				if (this.Visible)
				{
					Program.MainForm.CanEdit += new Global.EditEventHandler (MainForm_CanEdit);
					Program.MainForm.EditCopy += new Global.EditEventHandler (MainForm_EditCopy);
					Program.MainForm.EditCut += new Global.EditEventHandler (MainForm_EditCut);
					Program.MainForm.EditDelete += new Global.EditEventHandler (MainForm_EditDelete);
					Program.MainForm.EditPaste += new Global.EditEventHandler (MainForm_EditPaste);
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

		public event Global.GoToAnimationEventHandler GoToAnimation;

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
				TextBoxNewName.Enabled = !Program.FileIsReadOnly;
				ToolStripAnimations.Enabled = !Program.FileIsReadOnly;
				ButtonDelete.Enabled = !Program.FileIsReadOnly && (ListViewAnimations.SelectedIndex >= 0);
			}
			ShowAddState ();
		}

		private FileAnimation GetSelectedAnimation (Boolean pIncludeFocus)
		{
			return GetSelectedAnimation (ListViewAnimations.GetSelectedItem (pIncludeFocus));
		}
		private FileAnimation GetSelectedAnimation (ListViewItem pItem)
		{
			FileAnimation	lAnimation = null;

			if ((pItem != null) && (mCharacterFile.Gestures.Contains (pItem.Text)))
			{
				lAnimation = mCharacterFile.Gestures[pItem.Text];
			}
			return lAnimation;
		}
		private ListViewItem SelectAnimation (String pAnimationName)
		{
			foreach (ListViewItem lItem in ListViewAnimations.Items)
			{
				if (String.Compare (lItem.Text, pAnimationName, false) == 0)
				{
					lItem.Selected = true;
					return lItem;
				}
			}
			return null;
		}

		internal String GetNewAnimationName ()
		{
			return TextBoxNewName.Text.Trim ();
		}
		internal Boolean HasNewAnimationName ()
		{
			return !String.IsNullOrEmpty (GetNewAnimationName ());
		}

		private void ShowAddState ()
		{
			if (IsEmpty || Program.FileIsReadOnly)
			{
				ButtonAdd.Enabled = false;
				ButtonAdd.Text = Properties.Resources.EditAddAnimation;
			}
			else if (HasNewAnimationName () && !mCharacterFile.Gestures.Contains (GetNewAnimationName ()))
			{
				ButtonAdd.Enabled = true;
				ButtonAdd.Text = String.Format (Properties.Resources.EditAddAnimation1, GetNewAnimationName ());
			}
			else
			{
				ButtonAdd.Enabled = false;
				ButtonAdd.Text = Properties.Resources.EditAddAnimation0;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameProperties ()
		{
			NumericFrameWidth.Validated -= new System.EventHandler (NumericFrameWidth_Validated);
			NumericFrameHeight.Validated -= new System.EventHandler (NumericFrameHeight_Validated);

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

				NumericFrameWidth.Enabled = !Program.FileIsReadOnly;
				NumericFrameHeight.Enabled = !Program.FileIsReadOnly;
			}

			NumericFrameWidth.Validated += new System.EventHandler (NumericFrameWidth_Validated);
			NumericFrameHeight.Validated += new System.EventHandler (NumericFrameHeight_Validated);
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
				TextBoxPaletteFile.Enabled = !Program.FileIsReadOnly;
				ToolStripPaletteFile.Enabled = !Program.FileIsReadOnly;
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
				LabelTransparencyClick.Visible = !Program.FileIsReadOnly;
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

		internal UndoableUpdate DeleteSelectedAnimation (FileAnimation pAnimation, Boolean pForClipboard)
		{
			if ((pAnimation != null) && !Program.FileIsReadOnly)
			{
				AddDeleteAnimation	lUpdate = new AddDeleteAnimation (mCharacterFile, pAnimation, pForClipboard);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				if (AddDeleteAnimation.PutUndo (lUpdate.Apply () as AddDeleteAnimation, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedAnimation (FileAnimation pAnimation, FileAnimation pPasteAnimation)
		{
			if ((mCharacterFile != null) && (pPasteAnimation != null) && !Program.FileIsReadOnly)
			{
				String	lAnimationName = GetNewAnimationName ();

				if (pAnimation == null)
				{
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
						AddDeleteAnimation	lUpdate = new AddDeleteAnimation (mCharacterFile, lAnimationName, true);

						lUpdate = lUpdate.Apply () as AddDeleteAnimation;
						if (lUpdate != null)
						{
							pPasteAnimation.CopyTo (lUpdate.Target, true);
							lUpdate = lUpdate.Apply () as AddDeleteAnimation;
						}
						if (lUpdate != null)
						{
							lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
							if (AddDeleteAnimation.PutUndo (lUpdate.Apply () as AddDeleteAnimation, this))
							{
								SelectAnimation (lAnimationName);
								return lUpdate;
							}
						}
					}
				}
				else
				{
					UpdateAnimation	lUpdate = new UpdateAnimation (mCharacterFile, pAnimation, true);

					pPasteAnimation.CopyTo (pAnimation, true);
					lUpdate = lUpdate.Apply () as UpdateAnimation;
					if (lUpdate != null)
					{
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
						if (!UpdateAnimation.PutUndo (lUpdate.Apply () as UpdateAnimation, this))
						{
							lUpdate = null;
						}
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
				UpdateCharacterHeader	lUpdateCharacter = sender as UpdateCharacterHeader;
				AddDeleteAnimation		lAddDeleteAnimation = sender as AddDeleteAnimation;
				UpdateAnimation			lUpdateAnimation = sender as UpdateAnimation;

				if ((lUpdateCharacter != null) && (lUpdateCharacter.CharacterFile == mCharacterFile))
				{
					if (lUpdateCharacter.ImageSizeChanged)
					{
						ShowFrameProperties ();
					}
					if (lUpdateCharacter.PaletteChanged || lUpdateCharacter.PaletteTransparencyChanged)
					{
						ShowPalette ();
					}
				}

				if ((lAddDeleteAnimation != null) && (lAddDeleteAnimation.CharacterFile == mCharacterFile))
				{
					int	lSelNdx = ListViewAnimations.SelectedIndex;
					ShowAnimationNames ();
					ListViewAnimations.SelectedIndex = lSelNdx;
				}

				if ((lUpdateAnimation != null) && lUpdateAnimation.NameChanged)
				{
					ShowAnimationNames ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TextBoxNewName_TextChanged (object sender, EventArgs e)
		{
			ShowAddState ();
		}

		private void ListViewAnimations_SelectedIndexChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				ButtonDelete.Enabled = (ListViewAnimations.SelectedIndex >= 0);
			}
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToAnimation != null))
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					try
					{
						GoToAnimation (this, new Global.AnimationEventArgs (lAnimation));
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
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				String				lAnimationName = GetNewAnimationName ();
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
					lUpdate = new AddDeleteAnimation (mCharacterFile, lAnimationName, false);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
					if (AddDeleteAnimation.PutUndo (lUpdate.Apply () as AddDeleteAnimation, this))
					{
						SelectAnimation (lAnimationName);
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
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				ListViewItem	lSelItem = ListViewAnimations.SelectedItem;

				if ((lSelItem != null) && mCharacterFile.Gestures.Contains (lSelItem.Text))
				{
					int				lSelNdx = lSelItem.Index;
					FileAnimation	lAnimation = mCharacterFile.Gestures[lSelItem.Text];

					if (DeleteSelectedAnimation (lAnimation, false) != null)
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

		///////////////////////////////////////////////////////////////////////////////

		private void NumericFrameWidth_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader	lUpdate = new UpdateCharacterHeader (mCharacterFile);

				lUpdate.ImageSize = new System.Drawing.Size ((int)NumericFrameWidth.Value, mCharacterFile.Header.ImageSize.Height);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply () as UpdateCharacterHeader, this);
			}
		}

		private void NumericFrameHeight_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader	lUpdate = new UpdateCharacterHeader (mCharacterFile);

				lUpdate.ImageSize = new System.Drawing.Size (mCharacterFile.Header.ImageSize.Width, (int)NumericFrameHeight.Value);
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply () as UpdateCharacterHeader, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxPaletteFile.Modified) && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader	lUpdate = new UpdateCharacterHeader (mCharacterFile);

				lUpdate.PaletteFilePath = TextBoxPaletteFile.Text;
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply () as UpdateCharacterHeader, this);
			}
		}

		private void ButtonPaletteImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				String					lFilePath = mCharacterFile.PaletteFilePath;
				UpdateCharacterHeader	lUpdate;

				if (OpenFileDialogEx.OpenPaletteFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterHeader (mCharacterFile);
					lUpdate.PaletteFilePath = lFilePath;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
					UpdateCharacterHeader.PutUndo (lUpdate.Apply () as UpdateCharacterHeader, this);
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
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				int						lColorNdx = PaletteMouseColorNdx (e.Location);
				UpdateCharacterHeader	lUpdate;

				if (lColorNdx >= 0)
				{
					lUpdate = new UpdateCharacterHeader (mCharacterFile);
					lUpdate.PaletteTransparency = (Byte)lColorNdx;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (Program.MainForm.OnUpdateApplied);
					UpdateCharacterHeader.PutUndo (lUpdate.Apply () as UpdateCharacterHeader, this);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, Global.EditEventArgs e)
		{
			if (!IsEmpty)
			{
				if (!e.IsUsed && ListViewAnimations.ContainsFocus)
				{
					FileAnimation	lAnimation = GetSelectedAnimation (false);

					if (lAnimation != null)
					{
						e.CopyObjectTitle = Global.QuotedTitle (Global.TitleAnimation (lAnimation));
						if (!Program.FileIsReadOnly)
						{
							e.CutObjectTitle = e.CopyObjectTitle;
							e.DeleteObjectTitle = e.CopyObjectTitle;
						}
					}
				}

				if (!Program.FileIsReadOnly && (e.PasteObject is FileAnimation))
				{
					FileAnimation	lAnimation = null;

					if (ListViewAnimations.ContainsFocus)
					{
						lAnimation = GetSelectedAnimation (false);
					}
					if ((lAnimation != null) && (ListViewAnimations.ContainsFocus))
					{
						e.PasteObjectTitle = e.PasteTypeTitle (lAnimation, Global.TitleAnimation (lAnimation), Global.TitleAnimation (e.PasteObject as FileAnimation));
					}
					else if (ListViewAnimations.ContainsFocus || TextBoxNewName.ContainsFocus)
					{
						if (HasNewAnimationName () && !mCharacterFile.Gestures.Contains (GetNewAnimationName ()))
						{
							e.PasteObjectTitle = e.PasteTypeTitle (null, Global.QuotedTitle (GetNewAnimationName ()), Global.TitleAnimation (e.PasteObject as FileAnimation));
						}
						else
						{
							e.PasteTitle = null;
							e.PasteObjectTitle = Properties.Resources.EditPasteAnimation0;
						}
					}
				}
			}
		}

		internal void MainForm_EditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewAnimations.ContainsFocus)
			{
				FileAnimation	lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lAnimation);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewAnimations.ContainsFocus)
			{
				FileAnimation	lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lAnimation);
						DeleteSelectedAnimation (lAnimation, true);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, Global.EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewAnimations.ContainsFocus)
			{
				FileAnimation	lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					e.IsUsed = true;
					DeleteSelectedAnimation (lAnimation, false);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && (e.PasteObject is FileAnimation))
			{
				if (ListViewAnimations.ContainsFocus)
				{
					FileAnimation	lAnimation = GetSelectedAnimation (false);

					e.IsUsed = true;
					PasteSelectedAnimation (lAnimation, e.PasteObject as FileAnimation);
				}
				else if (HasNewAnimationName ())
				{
					e.IsUsed = true;
					PasteSelectedAnimation (null, e.PasteObject as FileAnimation);
				}
			}
		}

		#endregion
	}
}
