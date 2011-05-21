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

namespace AgentCharacterEditor.Panels
{
	public partial class CharacterPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		internal const UInt16 mLangDefault = 0x0409;
		private UInt16 mLangCurrent = 0;

		public CharacterPanel ()
		{
			InitializeComponent ();
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			Properties.Settings lSettings = Properties.Settings.Default;

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
			Properties.Settings lSettings = Properties.Settings.Default;

			lSettings.SelectedNameLanguage = (Int16)ListItemLangID (ListViewLanguage.SelectedOrFocusedItem);
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
					(FilePart as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeCharacter;
				}
				ShowFileProperties ();
			}
		}

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
			public PanelContext (CharacterPanel pPanel)
				: base (pPanel)
			{
				SelectedName = pPanel.ListItemLangID (pPanel.ListViewLanguage.SelectedItem);
			}

			public void RestoreContext (CharacterPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.SelectLangIDItem (SelectedName);
			}

			public UInt16 SelectedName
			{
				get;
				protected set;
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

			if (ListViewLanguage.SelectedItem != null)
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
				lName = CharacterFile.FindName (pLangID);

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
					pEventArgs.CopyObjectTitle = Properties.Titles.CharacterName (lName.Language);
					if (!Program.FileIsReadOnly && !mLangDefault.PrimaryLanguageEqual (lName))
					{
						pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
						pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
					}
				}
				if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileCharacterName))
				{
					pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lName, Properties.Titles.CharacterName (mLangCurrent), Properties.Titles.CharacterName ((pEventArgs.PasteObject as FileCharacterName).Language));
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
						AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteCharacterName, this);
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
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteCharacterName, this);
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

						AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (lName, true);

						lPasteName.CopyTo (lName);
						lUpdate = lUpdate.Apply () as UpdateCharacterName;
						if (lUpdate != null)
						{
							UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterName, this);
						}
					}
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (Object pUpdate)
		{
			UpdateCharacterHeader lHeaderUpdate = pUpdate as UpdateCharacterHeader;
			AddDeleteCharacterName lAddDeleteName = pUpdate as AddDeleteCharacterName;
			UpdateCharacterName lUpdateName = pUpdate as UpdateCharacterName;

			if ((lAddDeleteName != null) && (lAddDeleteName.CharacterFile == CharacterFile))
			{
				ShowNameStates ();
				ShowCharacterName ();
			}
			else if ((lUpdateName != null) && (lUpdateName.CharacterFile == CharacterFile))
			{
				ShowCharacterName ();
			}
			else if ((lHeaderUpdate != null) && (lHeaderUpdate.CharacterFile == CharacterFile))
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
							AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteCharacterName, this);
						}
						else
						{
							TextBoxName.Text = lName.Name;
						}
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
						UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterName, this);
					}
				}
				else if ((!String.IsNullOrEmpty (TextBoxName.Text)) && ((lName == null) || (TextBoxName.Text != lName.Name)))
				{
					lName = new FileCharacterName (mLangCurrent, TextBoxName.Text);
					AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lName, false, false);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteCharacterName, this);
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
					UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterName, this);
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
					UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterName, this);
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
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
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
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
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
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		#endregion
	}
}
