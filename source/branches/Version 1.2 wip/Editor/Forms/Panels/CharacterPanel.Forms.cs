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
using System.Drawing;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;

namespace AgentCharacterEditor.Panels
{
	public partial class CharacterPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public CharacterPanel ()
		{
			InitializeComponent ();
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				UInt16 lSelectedLangID = (UInt16)lSettings.SelectedNameLanguage;

				if (lSelectedLangID > 0)
				{
					SelectLangIDItem (lSelectedLangID);
				}
			}
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			Settings lSettings = Settings.Default;

			lSettings.SelectedNameLanguage = (Int16)ListItemLangID (ListViewLanguage.SelectedOrFocusedItem);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowNameStates ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			foreach (ListViewItem lItem in ListViewLanguage.Items)
			{
				UInt16 lLangID = ListItemLangID (lItem);
				FileCharacterName lName = null;

				if (IsPanelEmpty)
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

			PopIsPanelShowing (lWasShowing);
		}

		private void ShowCharacterName (FileCharacterName pName, UInt16 pLangID)
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (pName == null)
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
				TextBoxName.Text = pName.Name;
				TextBoxDescription.Text = pName.Desc1;
				TextBoxExtra.Text = pName.Desc2;

				TextBoxName.ReadOnly = Program.FileIsReadOnly;
				TextBoxDescription.ReadOnly = Program.FileIsReadOnly || !pLangID.PrimaryLanguageEqual (pName);
				TextBoxExtra.ReadOnly = Program.FileIsReadOnly || !pLangID.PrimaryLanguageEqual (pName);

				TextBoxName.Enabled = true;
				TextBoxDescription.Enabled = true;
				TextBoxExtra.Enabled = true;
			}
			TextBoxName.Modified = false;
			TextBoxDescription.Modified = false;
			TextBoxExtra.Modified = false;

			PopIsPanelShowing (lWasShowing);
		}

		///////////////////////////////////////////////////////////////////////////////

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

		private UInt16 SelectedLangID (Boolean pUseDefault)
		{
			if (ListViewLanguage.SelectedItem != null)
			{
				return ListItemLangID (ListViewLanguage.SelectedItem);
			}
			return pUseDefault ? mLangDefault : (UInt16)0;
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
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (IsPanelEmpty)
			{
				PictureBoxIconSmall.Image = null;
				PictureBoxIconLarge.Image = null;
				TextBoxIconFile.ResetText ();
				TextBoxIconFile.Enabled = false;
				ToolStripIconFile.Enabled = false;
			}
			else
			{
				TextBoxIconFile.Text = CharacterFile.IconFilePath;

				if (CharacterFile.Header.Icon == null)
				{
					PictureBoxIconSmall.Image = null;
					PictureBoxIconLarge.Image = null;
				}
				else
				{
					try
					{
						PictureBoxIconSmall.Image = CharacterFile.Header.Icon.ToBitmap ();
						PictureBoxIconLarge.Image = CharacterFile.Header.Icon.ToBitmap ();
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

			PopIsPanelShowing (lWasShowing);
		}

		private void ShowCharacterGuid ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (IsPanelEmpty)
			{
				TextBoxGUID.ResetText ();
				ButtonNewGUID.Enabled = false;
			}
			else
			{
				if (CharacterFile.Header.Guid == null)
				{
					TextBoxGUID.ResetText ();
				}
				else
				{
					TextBoxGUID.Text = CharacterFile.Header.Guid.ToString ().ToUpper ();
				}
				ButtonNewGUID.Enabled = !Program.FileIsReadOnly;
			}

			PopIsPanelShowing (lWasShowing);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
			if (ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					String lObjectTitle = Titles.CharacterName (lName.Language);

					pEventArgs.PutCopyTitle (lObjectTitle);
					if (!Program.FileIsReadOnly && !mLangDefault.PrimaryLanguageEqual (lName))
					{
						pEventArgs.PutCutTitle (lObjectTitle);
						pEventArgs.PutDeleteTitle (lObjectTitle);
					}
				}
				if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileCharacterName))
				{
					pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lName, Titles.CharacterName (mLangCurrent), Titles.CharacterName ((pEventArgs.PasteObject as FileCharacterName).Language));
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override Boolean EditCopy (Global.EditEventArgs pEventArgs)
		{
			if (ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lName);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override Boolean EditCut (Global.EditEventArgs pEventArgs)
		{
			if (ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName lUpdate;

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lName);

						lUpdate = new AddDeleteCharacterName (lName, true, true);
						AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override Boolean EditDelete (Global.EditEventArgs pEventArgs)
		{
			if (ListViewLanguage.ContainsFocus)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName lUpdate;

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					lUpdate = new AddDeleteCharacterName (lName, true, false);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
					return true;
				}
			}
			return false;
		}

		protected override Boolean EditPaste (Global.EditEventArgs pEventArgs)
		{
			if (ListViewLanguage.ContainsFocus)
			{
				if (pEventArgs.PasteObject is FileCharacterName)
				{
					FileCharacterName lName = LangIDName (mLangCurrent, true);
					FileCharacterName lPasteName = new FileCharacterName (mLangCurrent, pEventArgs.PasteObject as FileCharacterName);

					if (lName == null)
					{
						AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lPasteName, false, true);

						AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (lName, true);

						lPasteName.CopyTo (lName);
						lUpdate = lUpdate.Apply () as UpdateCharacterName;
						if (lUpdate != null)
						{
							UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
						}
					}
					return true;
				}
			}
			return false;
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
							AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
						}
						else
						{
							TextBoxName.Text = lName.Name;
						}
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
						UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
					}
				}
				else if ((!String.IsNullOrEmpty (TextBoxName.Text)) && ((lName == null) || (TextBoxName.Text != lName.Name)))
				{
					lName = new FileCharacterName (mLangCurrent, TextBoxName.Text);
					AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lName, false, false);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
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
					UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
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
					UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
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
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
			TextBoxIconFile.Modified = false;
		}

		private void ButtonIconImport_Click (object sender, EventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				String lFilePath = CharacterFile.IconFilePath;
				UpdateCharacterHeader lUpdate;

				if (OpenFileDialogEx.OpenIconFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterHeader ();
					lUpdate.IconFilePath = lFilePath;
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
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
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		#endregion
	}
}
