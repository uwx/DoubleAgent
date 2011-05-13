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
	public partial class AnimationsPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationsPanel ()
		{
			InitializeComponent ();
		}

		protected override Boolean TrackUpdatesWhenHidden
		{
			get
			{
				return true;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				if (FilePart is ResolveCharacter)
				{
					(FilePart as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeAnimations;
				}
				ShowAnimationNames ();
				ShowFrameProperties ();
				ShowPalette ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event Global.NavigationEventHandler Navigate;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public override object NavigationContext
		{
			get
			{
				return new PanelContext (this);
			}
			set
			{
				if (value is PanelContext)
				{
					(value as PanelContext).RestoreContext (this);
				}
				else
				{
					base.NavigationContext = value;
				}
			}
		}

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (AnimationsPanel pPanel)
				: base (pPanel)
			{
				SelectedAnimation = pPanel.ListViewAnimations.SelectedIndex;
				FocusedAnimation = pPanel.ListViewAnimations.FocusedIndex;
			}

			public void RestoreContext (AnimationsPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewAnimations.SelectedIndex = SelectedAnimation;
				pPanel.ListViewAnimations.FocusedIndex = FocusedAnimation;
			}

			public int SelectedAnimation
			{
				get;
				protected set;
			}
			public int FocusedAnimation
			{
				get;
				protected set;
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
				String[] lAnimations = CharacterFile.GetAnimationNames ();
				int lNdx;

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
			}
			ShowAddState ();
			ShowDeleteState ();
		}

		private void ShowAddState ()
		{
			if (IsEmpty || Program.FileIsReadOnly)
			{
				ButtonAdd.Enabled = false;
				ButtonAdd.Text = Properties.Resources.EditAddAnimation;
			}
			else if (HasNewAnimationName () && !CharacterFile.Gestures.Contains (GetNewAnimationName ()))
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

		private void ShowDeleteState ()
		{
			if (IsEmpty || Program.FileIsReadOnly)
			{
				ButtonDelete.Enabled = false;
				ButtonDelete.Text = String.Format (Properties.Resources.EditDeleteThis.NoMenuPrefix (), Global.TitleAnimation ((FileAnimation)null));
			}
			else
			{
				ButtonDelete.Enabled = (GetSelectedAnimation (false) != null);
				ButtonDelete.Text = String.Format (Properties.Resources.EditDeleteThis.NoMenuPrefix (), Global.TitleAnimation (GetSelectedAnimation (false)));
			}
		}

		private void ShowEditState (Global.CanEditEventArgs pEventArgs, Boolean pListIsFocused, Boolean pNewNameIsFocused)
		{
			if (!pEventArgs.IsUsed && pListIsFocused)
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					pEventArgs.CopyObjectTitle = Global.TitleAnimation (lAnimation);
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
						pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
					}
				}
			}

			if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileAnimation))
			{
				FileAnimation lAnimation = null;

				if (pListIsFocused)
				{
					lAnimation = GetSelectedAnimation (false);
				}
				if ((lAnimation != null) && pListIsFocused)
				{
					pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lAnimation, Global.TitleAnimation (lAnimation), Global.TitleAnimation (pEventArgs.PasteObject as FileAnimation));
				}
				else if (pListIsFocused || pNewNameIsFocused)
				{
					if (HasNewAnimationName () && !CharacterFile.Gestures.Contains (GetNewAnimationName ()))
					{
						pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (null, GetNewAnimationName ().Quoted (), Global.TitleAnimation (pEventArgs.PasteObject as FileAnimation));
					}
					else
					{
						pEventArgs.PasteTitle = null;
						pEventArgs.PasteObjectTitle = Properties.Resources.EditPasteAnimation0;
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
			ShowEditState (pEventArgs, ListViewAnimations.ContainsFocus, TextBoxNewName.ContainsFocus);
		}

		protected override void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			ShowEditState (e, true, false);

			if (!IsEmpty && !Program.FileIsReadOnly && Object.ReferenceEquals (e.ActiveControl, ListViewAnimations))
			{
				ToolStripMenuItem lMenuItem;

				e.ContextMenu.Items.Insert (0, new ToolStripSeparator ());
				e.ContextMenu.Items.Insert (0, lMenuItem = new ToolStripMenuItem (ButtonAdd.Text, ButtonAdd.Image, ButtonAdd_Click));
				lMenuItem.Enabled = ButtonAdd.Enabled;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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

		private FileAnimation GetSelectedAnimation (Boolean pIncludeFocus)
		{
			return GetSelectedAnimation (ListViewAnimations.GetSelectedItem (pIncludeFocus));
		}
		private FileAnimation GetSelectedAnimation (ListViewItem pItem)
		{
			FileAnimation lAnimation = null;

			if ((pItem != null) && (CharacterFile.Gestures.Contains (pItem.Text)))
			{
				lAnimation = CharacterFile.Gestures[pItem.Text];
			}
			return lAnimation;
		}

		internal String GetNewAnimationName ()
		{
			return TextBoxNewName.Text.Trim ();
		}
		internal Boolean HasNewAnimationName ()
		{
			return !String.IsNullOrEmpty (GetNewAnimationName ());
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameProperties ()
		{
			CausesValidation = false;

			if (IsEmpty)
			{
				NumericFrameWidth.ResetText ();
				NumericFrameHeight.ResetText ();

				NumericFrameWidth.Enabled = false;
				NumericFrameHeight.Enabled = false;
			}
			else
			{
				NumericFrameWidth.Value = CharacterFile.Header.ImageSize.Width;
				NumericFrameHeight.Value = CharacterFile.Header.ImageSize.Height;

				NumericFrameWidth.Enabled = !Program.FileIsReadOnly;
				NumericFrameHeight.Enabled = !Program.FileIsReadOnly;
			}

			CausesValidation = Visible;
		}

		private void ShowPalette ()
		{
			System.Drawing.Imaging.ColorPalette lPalette = null;

			if (CharacterFile == null)
			{
				TextBoxPaletteFile.ResetText ();
				TextBoxPaletteFile.Enabled = false;
				ToolStripPaletteFile.Enabled = false;
			}
			else
			{
				TextBoxPaletteFile.Text = CharacterFile.PaletteFilePath;
				TextBoxPaletteFile.Enabled = !Program.FileIsReadOnly;
				ToolStripPaletteFile.Enabled = !Program.FileIsReadOnly;
				lPalette = CharacterFile.Header.Palette;
			}
			if ((lPalette != null) && (lPalette.Entries.Length == 256))
			{
				System.Drawing.Bitmap lBitmap = new System.Drawing.Bitmap (256, 256, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
				System.Drawing.Color[] lColors = lPalette.Entries;
				int lColorNdx = 0;
				System.Drawing.Imaging.BitmapData lData;
				Byte[] lBytes;

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
				LabelTransparencySample.BackColor = lColors[CharacterFile.Header.Transparency];
				LabelTransparencySample.ForeColor = (LabelTransparencySample.BackColor.GetBrightness () > 0.5) ? Color.Black : Color.White;
				LabelTransparencySample.Text = CharacterFile.Header.Transparency.ToString ();
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
			System.Drawing.Imaging.ColorPalette lPalette = null;
			int lColorNdx;

			if (CharacterFile != null)
			{
				lPalette = CharacterFile.Header.Palette;
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
			System.Drawing.Imaging.ColorPalette lPalette = null;
			System.Drawing.Point lColorPos;

			if (CharacterFile != null)
			{
				lPalette = CharacterFile.Header.Palette;
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
				AddDeleteAnimation lUpdate = new AddDeleteAnimation (pAnimation, pForClipboard);

				if (AddDeleteAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteAnimation, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedAnimation (FileAnimation pAnimation, FileAnimation pPasteAnimation)
		{
			if ((CharacterFile != null) && (pPasteAnimation != null) && !Program.FileIsReadOnly)
			{
				String lAnimationName = GetNewAnimationName ();

				if (pAnimation == null)
				{
					if (String.IsNullOrEmpty (lAnimationName))
					{
						System.Media.SystemSounds.Beep.Play ();
					}
					else if (CharacterFile.Gestures.Contains (lAnimationName))
					{
						MessageBox.Show (String.Format (Properties.Resources.MsgDuplicateAnimation, lAnimationName), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
					}
					else
					{
						AddDeleteAnimation lUpdate = new AddDeleteAnimation (lAnimationName, true);

						lUpdate = lUpdate.Apply () as AddDeleteAnimation;
						if (lUpdate != null)
						{
							pPasteAnimation.CopyTo (lUpdate.Target, true);
							lUpdate = lUpdate.Apply () as AddDeleteAnimation;
						}
						if (lUpdate != null)
						{
							if (AddDeleteAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteAnimation, this))
							{
								SelectAnimation (lAnimationName);
								return lUpdate;
							}
						}
					}
				}
				else
				{
					UpdateAnimation lUpdate = new UpdateAnimation (pAnimation, true);

					pPasteAnimation.CopyTo (pAnimation, true);
					lUpdate = lUpdate.Apply () as UpdateAnimation;
					if ((lUpdate != null) && !UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimation, this))
					{
						lUpdate = null;
					}
				}
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool EditCopy (Global.EditEventArgs pEventArgs)
		{
			if (ListViewAnimations.ContainsFocus)
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lAnimation);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override bool EditCut (Global.EditEventArgs pEventArgs)
		{
			if (ListViewAnimations.ContainsFocus)
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lAnimation);
						DeleteSelectedAnimation (lAnimation, true);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override bool EditDelete (Global.EditEventArgs pEventArgs)
		{
			if (ListViewAnimations.ContainsFocus)
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					DeleteSelectedAnimation (lAnimation, false);
					return true;
				}
			}
			return false;
		}

		protected override bool EditPaste (Global.EditEventArgs pEventArgs)
		{
			if (pEventArgs.PasteObject is FileAnimation)
			{
				if (ListViewAnimations.ContainsFocus)
				{
					FileAnimation lAnimation = GetSelectedAnimation (false);

					PasteSelectedAnimation (lAnimation, pEventArgs.PasteObject as FileAnimation);
					return true;
				}
				else if (HasNewAnimationName ())
				{
					PasteSelectedAnimation (null, pEventArgs.PasteObject as FileAnimation);
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (Object pUpdate)
		{
			UpdateCharacterHeader lUpdateCharacter = pUpdate as UpdateCharacterHeader;
			AddDeleteAnimation lAddDeleteAnimation = pUpdate as AddDeleteAnimation;
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;

			if ((lUpdateCharacter != null) && (lUpdateCharacter.CharacterFile == CharacterFile))
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

			if ((lAddDeleteAnimation != null) && (lAddDeleteAnimation.CharacterFile == CharacterFile))
			{
				int lSelNdx = ListViewAnimations.SelectedIndex;
				ShowAnimationNames ();
				ListViewAnimations.SelectedIndex = lSelNdx;
			}

			if ((lUpdateAnimation != null) && lUpdateAnimation.NameChanged)
			{
				ShowAnimationNames ();
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
			ShowDeleteState ();
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (Navigate != null))
			{
				FileAnimation lAnimation = GetSelectedAnimation (false);

				if (lAnimation != null)
				{
					try
					{
						Navigate (this, new Global.NavigationEventArgs (new ResolveAnimation (lAnimation)));
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
				String lAnimationName = GetNewAnimationName ();
				AddDeleteAnimation lUpdate;

				if (String.IsNullOrEmpty (lAnimationName))
				{
					System.Media.SystemSounds.Beep.Play ();
				}
				else if (CharacterFile.Gestures.Contains (lAnimationName))
				{
					MessageBox.Show (String.Format (Properties.Resources.MsgDuplicateAnimation, lAnimationName), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
				else
				{
					lUpdate = new AddDeleteAnimation (lAnimationName, false);
					if (AddDeleteAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteAnimation, this))
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
				ListViewItem lSelItem = ListViewAnimations.SelectedItem;

				if ((lSelItem != null) && CharacterFile.Gestures.Contains (lSelItem.Text))
				{
					int lSelNdx = lSelItem.Index;
					FileAnimation lAnimation = CharacterFile.Gestures[lSelItem.Text];

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
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.ImageSize = new System.Drawing.Size ((int)NumericFrameWidth.Value, CharacterFile.Header.ImageSize.Height);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		private void NumericFrameHeight_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.ImageSize = new System.Drawing.Size (CharacterFile.Header.ImageSize.Width, (int)NumericFrameHeight.Value);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxPaletteFile_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && TextBoxPaletteFile.Modified && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.PaletteFilePath = TextBoxPaletteFile.Text;
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		private void ButtonPaletteImport_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				String lFilePath = CharacterFile.PaletteFilePath;
				UpdateCharacterHeader lUpdate;

				if (OpenFileDialogEx.OpenPaletteFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterHeader ();
					lUpdate.PaletteFilePath = lFilePath;
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
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
				int lColorNdx = PaletteMouseColorNdx (e.Location);
				UpdateCharacterHeader lUpdate;

				if (lColorNdx >= 0)
				{
					lUpdate = new UpdateCharacterHeader ();
					lUpdate.PaletteTransparency = (Byte)lColorNdx;
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
				}
			}
		}

		#endregion
	}
}
