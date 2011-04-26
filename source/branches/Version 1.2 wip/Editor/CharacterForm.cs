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
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public partial class CharacterForm : UserControl
	{
		private CharacterFile mCharacterFile = null;
		internal const UInt16 mLangDefault = 0x0409;
		private UInt16 mLangCurrent = 0;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public CharacterForm ()
		{
			InitializeComponent ();
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.LoadConfig += new EventHandler (MainForm_LoadConfig);
				Program.MainForm.SaveConfig += new EventHandler (MainForm_SaveConfig);
			}
		}

		private void MainForm_LoadConfig (object sender, EventArgs e)
		{
			Properties.Settings lSettings = Properties.Settings.Default;
		}

		private void MainForm_SaveConfig (object sender, EventArgs e)
		{
			Properties.Settings lSettings = Properties.Settings.Default;
		}

		private void CharacterForm_VisibleChanged (object sender, EventArgs e)
		{
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.UpdateApplied -= new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				Program.MainForm.CanEdit -= new Global.EditEventHandler (MainForm_CanEdit);
				Program.MainForm.EditCopy -= new Global.EditEventHandler (MainForm_EditCopy);
				Program.MainForm.EditCut -= new Global.EditEventHandler (MainForm_EditCut);
				Program.MainForm.EditDelete -= new Global.EditEventHandler (MainForm_EditDelete);
				Program.MainForm.EditPaste -= new Global.EditEventHandler (MainForm_EditPaste);
				if (Visible)
				{
					Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
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

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				ShowFileProperties ();
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
		#region Display

		private void ShowFileProperties ()
		{
			ShowCharacterIcon ();
			ShowCharacterGuid ();
			ShowNameStates ();
			if (ListViewLanguage.SelectedItems.Count <= 0)
			{
				SelectLangIDItem (mLangDefault);
			}
			mLangCurrent = ShowCharacterName ();
		}

		private void ShowNameStates ()
		{
			foreach (ListViewItem lItem in ListViewLanguage.Items)
			{
				UInt16 lLangID = ListItemLangID (lItem);
				FileCharacterName lName = null;

				if (IsEmpty)
				{
					lItem.ForeColor = ListViewLanguage.ForeColor;
				}
				else
				{
					lName = LangIDName (lLangID, true);
					if (lName == null)
					{
						lItem.ForeColor = ListViewLanguage.ForeColor;
					}
					else
					{
						lItem.ForeColor = SystemColors.HotTrack;
					}
				}
			}
		}

		private UInt16 ShowCharacterName ()
		{
			FileCharacterName lName = null;
			UInt16 lLangID = mLangDefault;

			if (ListViewLanguage.SelectedItems.Count > 0)
			{
				lLangID = ListItemLangID (ListViewLanguage.SelectedItem);
			}
			if (!IsEmpty)
			{
				lName = LangIDName (lLangID, false);
			}
			if (lName == null)
			{
				TextBoxName.ResetText ();
				TextBoxDescription.ResetText ();
				TextBoxExtra.ResetText ();

				TextBoxName.Enabled = false;
				TextBoxDescription.Enabled = false;
				TextBoxExtra.Enabled = false;
			}
			else
			{
				TextBoxName.Text = lName.Name;
				TextBoxDescription.Text = lName.Desc1;
				TextBoxExtra.Text = lName.Desc2;

				TextBoxName.ReadOnly = Program.FileIsReadOnly;
				TextBoxDescription.ReadOnly = Program.FileIsReadOnly || !lLangID.PrimaryLanguageEqual (lName);
				TextBoxExtra.ReadOnly = Program.FileIsReadOnly || !lLangID.PrimaryLanguageEqual (lName);

				TextBoxName.Enabled = true;
				TextBoxDescription.Enabled = true;
				TextBoxExtra.Enabled = true;
			}
			TextBoxName.Modified = false;
			TextBoxDescription.Modified = false;
			TextBoxExtra.Modified = false;

			return lLangID;
		}

		///////////////////////////////////////////////////////////////////////////////

		private FileCharacterName LangIDName (UInt16 pLangID, Boolean pExactMatch)
		{
			FileCharacterName lName = null;

			if (!IsEmpty)
			{
				lName = mCharacterFile.FindName (pLangID);

				if (pExactMatch && !pLangID.PrimaryLanguageEqual (lName))
				{
					lName = null;
				}
			}
			return lName;
		}

		private UInt16 ListItemLangID (ListViewItem pListItem)
		{
			UInt16 lLangID = mLangDefault;
			String lLanguage;

			if (pListItem != null)
			{
				lLanguage = pListItem.Tag.ToString ();
				if (!UInt16.TryParse (lLanguage.Substring (lLanguage.LastIndexOf ('-') + 1), System.Globalization.NumberStyles.HexNumber, null, out lLangID))
				{
					lLangID = mLangDefault;
				}
			}
			return lLangID;
		}

		private ListViewItem ListLangIDItem (UInt16 pLangID)
		{
			UInt16 lLangID;

			foreach (ListViewItem lItem in ListViewLanguage.Items)
			{
				lLangID = ListItemLangID (lItem);
				if (
						(lLangID == pLangID)
					|| (
							((UInt16)(Byte)pLangID == pLangID)
						&& ((Byte)lLangID == (Byte)pLangID)
						)
					)
				{
					return lItem;
				}
			}
			return null;
		}

		private Boolean SelectLangIDItem (UInt16 pLangID)
		{
			ListViewItem lItem = ListLangIDItem (pLangID);

			if (lItem != null)
			{
				lItem.Selected = true;
				ListViewLanguage.FocusedItem = lItem;
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowCharacterIcon ()
		{
			if (IsEmpty)
			{
				PictureBoxIconSmall.Image = null;
				PictureBoxIconLarge.Image = null;
				TextBoxIconFile.ResetText ();
				TextBoxIconFile.Enabled = false;
				ToolStripIconFile.Enabled = false;
			}
			else
			{
				TextBoxIconFile.Text = mCharacterFile.IconFilePath;

				if (mCharacterFile.Header.Icon == null)
				{
					PictureBoxIconSmall.Image = null;
					PictureBoxIconLarge.Image = null;
				}
				else
				{
					try
					{
						PictureBoxIconSmall.Image = mCharacterFile.Header.Icon.ToBitmap ();
						PictureBoxIconLarge.Image = mCharacterFile.Header.Icon.ToBitmap ();
					}
					catch
					{
						PictureBoxIconSmall.Image = null;
						PictureBoxIconLarge.Image = null;
					}
				}

				TextBoxIconFile.Enabled = !Program.FileIsReadOnly;
				ToolStripIconFile.Enabled = !Program.FileIsReadOnly;
			}
		}

		private void ShowCharacterGuid ()
		{
			if (IsEmpty)
			{
				TextBoxGUID.ResetText ();
				ButtonNewGUID.Enabled = false;
			}
			else
			{
				if (mCharacterFile.Header.Guid == null)
				{
					TextBoxGUID.ResetText ();
				}
				else
				{
					TextBoxGUID.Text = mCharacterFile.Header.Guid.ToString ().ToUpper ();
				}
				ButtonNewGUID.Enabled = !Program.FileIsReadOnly;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateCharacterHeader lHeaderUpdate = sender as UpdateCharacterHeader;
				AddDeleteCharacterName lAddDeleteName = sender as AddDeleteCharacterName;
				UpdateCharacterName lUpdateName = sender as UpdateCharacterName;

				if ((lAddDeleteName != null) && (lAddDeleteName.CharacterFile == mCharacterFile))
				{
					ShowNameStates ();
					ShowCharacterName ();
				}
				else if ((lUpdateName != null) && (lUpdateName.CharacterFile == mCharacterFile))
				{
					ShowCharacterName ();
				}
				else if ((lHeaderUpdate != null) && (lHeaderUpdate.CharacterFile == mCharacterFile))
				{
					if (lHeaderUpdate.GuidChanged)
					{
						ShowCharacterGuid ();
					}
					if (lHeaderUpdate.IconChanged)
					{
						ShowCharacterIcon ();
					}
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewLanguage_SelectedIndexChanged (object sender, EventArgs e)
		{
			mLangCurrent = ShowCharacterName ();
		}

		private void TextBoxName_Validated (object sender, EventArgs e)
		{
			if ((TextBoxName.Modified) && (!Program.FileIsReadOnly))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					if (String.IsNullOrEmpty (TextBoxName.Text))
					{
						if (String.IsNullOrEmpty (lName.Desc1) && String.IsNullOrEmpty (lName.Desc2))
						{
							AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lName, true, false);
							AddDeleteCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as AddDeleteCharacterName, this);
						}
						else
						{
							TextBoxName.Text = lName.Name;
						}
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
						UpdateCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterName, this);
					}
				}
				else if ((!String.IsNullOrEmpty (TextBoxName.Text)) && ((lName == null) || (TextBoxName.Text != lName.Name)))
				{
					lName = new FileCharacterName (mLangCurrent, TextBoxName.Text);
					AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lName, false, false);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as AddDeleteCharacterName, this);
				}
			}
			TextBoxName.Modified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxDescription_Validated (object sender, EventArgs e)
		{
			if ((TextBoxDescription.Modified) && (!Program.FileIsReadOnly))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				UpdateCharacterName lUpdate;

				if (lName != null)
				{
					lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
					UpdateCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterName, this);
				}
			}
			TextBoxDescription.Modified = false;
		}

		private void TextBoxExtra_Validated (object sender, EventArgs e)
		{
			if ((TextBoxExtra.Modified) && (!Program.FileIsReadOnly))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				UpdateCharacterName lUpdate;

				if (lName != null)
				{
					lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
					UpdateCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterName, this);
				}
			}
			TextBoxExtra.Modified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxIconFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxIconFile.Modified) && (!Program.FileIsReadOnly))
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.IconFilePath = TextBoxIconFile.Text;
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		private void ButtonIconImport_Click (object sender, EventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				String lFilePath = mCharacterFile.IconFilePath;
				UpdateCharacterHeader lUpdate;

				if (OpenFileDialogEx.OpenIconFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterHeader ();
					lUpdate.IconFilePath = lFilePath;
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterHeader, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonNewGUID_Click (object sender, EventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.Guid = System.Guid.NewGuid ();
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					e.CopyObjectTitle = Global.TitleCharacterName (lName.Language);
					if (!Program.FileIsReadOnly && !mLangDefault.PrimaryLanguageEqual (lName))
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				if (!Program.FileIsReadOnly && (e.PasteObject is FileCharacterName))
				{
					e.PasteObjectTitle = e.PasteTypeTitle (lName, Global.TitleCharacterName (mLangCurrent), Global.TitleCharacterName ((e.PasteObject as FileCharacterName).Language));
				}
			}
		}

		internal void MainForm_EditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lName);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName lUpdate;

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lName);

						lUpdate = new AddDeleteCharacterName (lName, true, true);
						AddDeleteCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName lUpdate;

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					e.IsUsed = true;

					lUpdate = new AddDeleteCharacterName (lName, true, false);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as AddDeleteCharacterName, this);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && ListViewLanguage.ContainsFocus)
			{
				if (e.PasteObject is FileCharacterName)
				{
					FileCharacterName lName = LangIDName (mLangCurrent, true);
					FileCharacterName lPasteName = new FileCharacterName (mLangCurrent, e.PasteObject as FileCharacterName);

					e.IsUsed = true;

					if (lName == null)
					{
						AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lPasteName, false, true);

						AddDeleteCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (lName, true);

						lPasteName.CopyTo (lName);
						lUpdate = lUpdate.Apply () as UpdateCharacterName;
						if (lUpdate != null)
						{
							UpdateCharacterName.PutUndo (lUpdate.Apply (OnUpdateApplied) as UpdateCharacterName, this);
						}
					}
				}
			}
		}

		#endregion
	}
}
