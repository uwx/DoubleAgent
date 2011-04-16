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
	public partial class CharacterForm : UserControl
	{
		private CharacterFile	mCharacterFile = null;
		private const UInt16	mLangDefault = 0x0409;
		private UInt16			mLangCurrent = 0;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public CharacterForm ()
		{
			InitializeComponent ();

			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.LoadConfig += new EventHandler (MainForm_LoadConfig);
				MainForm.Singleton.SaveConfig += new EventHandler (MainForm_SaveConfig);
			}
		}

		private void MainForm_LoadConfig (object sender, EventArgs e)
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		private void MainForm_SaveConfig (object sender, EventArgs e)
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		private void CharacterForm_VisibleChanged (object sender, EventArgs e)
		{
			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.CanEdit -= new EditEventHandler (MainForm_CanEdit);
				MainForm.Singleton.EditCopy -= new EditEventHandler (MainForm_EditCopy);
				MainForm.Singleton.EditCut -= new EditEventHandler (MainForm_EditCut);
				MainForm.Singleton.EditDelete -= new EditEventHandler (MainForm_EditDelete);
				MainForm.Singleton.EditPaste -= new EditEventHandler (MainForm_EditPaste);
				if (this.Visible)
				{
					MainForm.Singleton.CanEdit += new EditEventHandler (MainForm_CanEdit);
					MainForm.Singleton.EditCopy += new EditEventHandler (MainForm_EditCopy);
					MainForm.Singleton.EditCut += new EditEventHandler (MainForm_EditCut);
					MainForm.Singleton.EditDelete += new EditEventHandler (MainForm_EditDelete);
					MainForm.Singleton.EditPaste += new EditEventHandler (MainForm_EditPaste);
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
				UInt16				lLangID = ListItemLangID (lItem);
				FileCharacterName	lName = null;

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
			FileCharacterName	lName = null;
			UInt16				lLangID = mLangDefault;

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

				TextBoxName.ReadOnly = MainForm.Singleton.FileIsReadOnly;
				TextBoxDescription.ReadOnly = MainForm.Singleton.FileIsReadOnly || !PrimaryLanguageMatch (lName, lLangID);
				TextBoxExtra.ReadOnly = MainForm.Singleton.FileIsReadOnly || !PrimaryLanguageMatch (lName, lLangID);

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
			FileCharacterName	lName = null;

			if (!IsEmpty)
			{
				lName = mCharacterFile.FindName (pLangID);

				if (pExactMatch && !PrimaryLanguageMatch (lName, pLangID))
				{
					lName = null;
				}
			}
			return lName;
		}

		static private Boolean PrimaryLanguageMatch (FileCharacterName pName, UInt16 pLangID)
		{
			if (pName != null)
			{
				return PrimaryLanguageMatch (pName.Language, pLangID);
			}
			return false;
		}
		static private Boolean PrimaryLanguageMatch (UInt16 pLangID1, UInt16 pLangID2)
		{
			return ((Byte)pLangID1 == (Byte)pLangID2);
		}

		private UInt16 ListItemLangID (ListViewItem pListItem)
		{
			UInt16	lLangID = mLangDefault;
			String	lLanguage;

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
			UInt16	lLangID;

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
			ListViewItem	lItem = ListLangIDItem (pLangID);

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

				TextBoxIconFile.Enabled = !MainForm.Singleton.FileIsReadOnly;
				ToolStripIconFile.Enabled = !MainForm.Singleton.FileIsReadOnly;
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
				ButtonNewGUID.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class AddDeleteCharacterName : UndoableAddDelete<FileCharacterName>
		{
			public AddDeleteCharacterName (CharacterFile pCharacterFile, FileCharacterName pName, Boolean pRemove, Boolean pForClipboard)
				: base (pCharacterFile, pName, pForClipboard)
			{
				this.IsDelete = pRemove;
			}

			public override String TargetDescription
			{
				get
				{
					return Program.TitleCharacterName (this.Target);
				}
			}

			public override UndoUnit Apply ()
			{
				AddDeleteCharacterName	lApplied = null;

				if (IsDelete)
				{
					if (CharacterFile.Names.Remove (this.Target))
					{
						lApplied = new AddDeleteCharacterName (CharacterFile, this.Target, false, this.ForClipboard);
						lApplied.IsRedo = !IsRedo;
					}
				}
				else
				{
					FileCharacterName	lName = CharacterFile.Names.Add (this.Target.Language, this.Target.Name);

					if (lName != null)
					{
						lName.Desc1 = this.Target.Desc1;
						lName.Desc2 = this.Target.Desc2;
						lApplied = new AddDeleteCharacterName (CharacterFile, lName, true, this.ForClipboard);
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

		internal class UpdateCharacterName : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterName (CharacterFile pCharacterFile, UInt16 pLanguage, String pName, String pDesc1, String pDesc2)
				: base (pCharacterFile, pCharacterFile)
			{
				this.Language = pLanguage;
				this.Name = pName;
				this.Desc1 = pDesc1;
				this.Desc2 = pDesc2;
			}

			public UpdateCharacterName (CharacterFile pCharacterFile, FileCharacterName pName, Boolean pForClipboard)
				: base (pCharacterFile, pCharacterFile, pForClipboard)
			{
				this.Language = pName.Language;
				this.Name = pName.Name;
				this.Desc1 = pName.Desc1;
				this.Desc2 = pName.Desc2;
			}

			public UInt16 Language
			{
				get;
				private set;
			}
			public String Name
			{
				get;
				private set;
			}
			public String Desc1
			{
				get;
				private set;
			}
			public String Desc2
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Program.TitleCharacterName (this.Language);
				}
			}
			public override String ActionDescription
			{
				get
				{
					return this.ForClipboard ? "paste " : "change ";
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateCharacterName	lApplied = null;
				FileCharacterName	lName;
				String				lSwap;

				lName = CharacterFile.FindName (this.Language);
				if (!CharacterForm.PrimaryLanguageMatch (lName, this.Language))
				{
					lName = null;
				}
				if (lName != null)
				{
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Applying {0}", lName.ToString ());
#endif

					if ((this.Name != null) && (!this.Name.Equals (lName.Name)))
					{
						lSwap = lName.Name;
						lName.Name = this.Name;
						this.Name = lSwap;
						lApplied = this;
					}
					if ((this.Desc1 != null) && (!this.Desc1.Equals (lName.Desc1)))
					{
						lSwap = lName.Desc1;
						lName.Desc1 = this.Desc1;
						this.Desc1 = lSwap;
						lApplied = this;
					}
					if ((this.Desc2 != null) && (!this.Desc2.Equals (lName.Desc2)))
					{
						lSwap = lName.Desc2;
						lName.Desc2 = this.Desc2;
						this.Desc2 = lSwap;
						lApplied = this;
					}
#if DEBUG_NOT
				if (lApplied != null)
				{
					System.Diagnostics.Debug.Print ("Applyed  {0}", lName.ToString ());
				}
#endif
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateCharacterGuid : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterGuid (CharacterFile pCharacterFile, Guid pGuid)
				: base (pCharacterFile, pCharacterFile)
			{
				this.Guid = pGuid;
			}

			public Guid Guid
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return "unique identifier";
				}
			}

			public override UndoUnit Apply ()
			{
				Guid	lSwap;

				if (!CharacterFile.Header.Guid.Equals (this.Guid))
				{
					lSwap = CharacterFile.Header.Guid;
					CharacterFile.Header.Guid = this.Guid;
					this.Guid = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		internal class UpdateCharacterIcon : UndoableUpdate<CharacterFile>
		{
			public UpdateCharacterIcon (CharacterFile pCharacterFile, String pIconPath)
				: base (pCharacterFile, pCharacterFile)
			{
				this.IconPath = pIconPath;
			}

			public String IconPath
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return "icon";
				}
			}

			public override UndoUnit Apply ()
			{
				if (!CharacterFile.IconFilePath.Equals (this.IconPath))
				{
					String	lSwap = CharacterFile.IconFilePath;
					CharacterFile.IconFilePath = this.IconPath;
					this.IconPath = lSwap;

					return OnApplied (this);
				}
				return null;
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
			if ((TextBoxName.Modified) && (!MainForm.Singleton.FileIsReadOnly))
			{
				FileCharacterName	lName =LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					if (String.IsNullOrEmpty (TextBoxName.Text))
					{
						if (String.IsNullOrEmpty (lName.Desc1) && String.IsNullOrEmpty (lName.Desc2))
						{
							AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (mCharacterFile, lName, true, false);
							lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameStateUpdate_Applied);
							AddDeleteCharacterName.PutUndo (lUpdate.Apply () as AddDeleteCharacterName);
						}
						else
						{
							TextBoxName.Text = lName.Name;
						}
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (mCharacterFile, lName.Language, TextBoxName.Text, null, null);
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameTextUpdate_Applied);
						UpdateCharacterName.PutUndo (lUpdate.Apply () as UpdateCharacterName);
					}
				}
				else if ((!String.IsNullOrEmpty (TextBoxName.Text)) && ((lName == null) || (TextBoxName.Text != lName.Name)))
				{
					lName = new FileCharacterName (mLangCurrent, TextBoxName.Text);
					AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (mCharacterFile, lName, false, false);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameStateUpdate_Applied);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply () as AddDeleteCharacterName);
				}
			}
			TextBoxName.Modified = false;
		}

		private void UndoableNameStateUpdate_Applied (object sender, EventArgs e)
		{
			ShowNameStates ();
			ShowCharacterName ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxDescription_Validated (object sender, EventArgs e)
		{
			if ((TextBoxDescription.Modified) && (!MainForm.Singleton.FileIsReadOnly))
			{
				FileCharacterName	lName = LangIDName (mLangCurrent, true);
				UpdateCharacterName	lUpdate;

				if (lName != null)
				{
					lUpdate = new UpdateCharacterName (mCharacterFile, lName.Language, null, TextBoxDescription.Text, null);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameTextUpdate_Applied);
					UpdateCharacterName.PutUndo (lUpdate.Apply () as UpdateCharacterName);
				}
			}
			TextBoxDescription.Modified = false;
		}

		private void TextBoxExtra_Validated (object sender, EventArgs e)
		{
			if ((TextBoxExtra.Modified) && (!MainForm.Singleton.FileIsReadOnly))
			{
				FileCharacterName	lName = LangIDName (mLangCurrent, true);
				UpdateCharacterName	lUpdate;

				if (lName != null)
				{
					lUpdate = new UpdateCharacterName (mCharacterFile, lName.Language, null, null, TextBoxExtra.Text);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameTextUpdate_Applied);
					UpdateCharacterName.PutUndo (lUpdate.Apply () as UpdateCharacterName);
				}
			}
			TextBoxExtra.Modified = false;
		}

		private void UndoableNameTextUpdate_Applied (object sender, EventArgs e)
		{
			ShowCharacterName ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxIconFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxIconFile.Modified) && (!MainForm.Singleton.FileIsReadOnly))
			{
				UpdateCharacterIcon	lUpdate = new UpdateCharacterIcon (mCharacterFile, TextBoxIconFile.Text);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableIconUpdate_Applied);
				UpdateCharacterIcon.PutUndo (lUpdate.Apply () as UpdateCharacterIcon);
			}
		}

		private void ButtonIconImport_Click (object sender, EventArgs e)
		{
			if (!MainForm.Singleton.FileIsReadOnly)
			{
				String				lFilePath = mCharacterFile.IconFilePath;
				UpdateCharacterIcon	lUpdate;

				if (OpenFileDialogEx.OpenIconFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterIcon (mCharacterFile, lFilePath);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableIconUpdate_Applied);
					UpdateCharacterIcon.PutUndo (lUpdate.Apply () as UpdateCharacterIcon);
				}
			}
		}

		private void UndoableIconUpdate_Applied (object sender, EventArgs e)
		{
			ShowCharacterIcon ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonNewGUID_Click (object sender, EventArgs e)
		{
			if (!MainForm.Singleton.FileIsReadOnly)
			{
				UpdateCharacterGuid	lUpdate = new UpdateCharacterGuid (mCharacterFile, System.Guid.NewGuid ());

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableGuidUpdate_Applied);
				UpdateCharacterGuid.PutUndo (lUpdate.Apply () as UpdateCharacterGuid);
			}
		}

		private void UndoableGuidUpdate_Applied (object sender, EventArgs e)
		{
			ShowCharacterGuid ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName	lName =LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					e.CopyObjectTitle = Program.TitleCharacterName (lName.Language);
					if (!MainForm.Singleton.FileIsReadOnly && !PrimaryLanguageMatch (lName, mLangDefault))
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				if (!MainForm.Singleton.FileIsReadOnly && (e.PasteObject is FileCharacterName))
				{
					e.PasteObjectTitle = e.PasteTypeTitle (lName) + Program.TitleCharacterName (mLangCurrent);
				}
			}
		}

		internal void MainForm_EditCopy (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName	lName = LangIDName (mLangCurrent, true);

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

		internal void MainForm_EditCut (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !MainForm.Singleton.FileIsReadOnly && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName		lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName	lUpdate;

				if ((lName != null) && !PrimaryLanguageMatch (lName, mLangDefault))
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lName);

						lUpdate = new AddDeleteCharacterName (mCharacterFile, lName, true, true);
						lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameStateUpdate_Applied);
						AddDeleteCharacterName.PutUndo (lUpdate.Apply () as AddDeleteCharacterName);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !MainForm.Singleton.FileIsReadOnly && ListViewLanguage.ContainsFocus)
			{
				FileCharacterName		lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName	lUpdate;

				if ((lName != null) && !PrimaryLanguageMatch (lName, mLangDefault))
				{
					e.IsUsed = true;

					lUpdate = new AddDeleteCharacterName (mCharacterFile, lName, true, false);
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameStateUpdate_Applied);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply () as AddDeleteCharacterName);
				}
			}
		}

		internal void MainForm_EditPaste (object sender, EditEventArgs e)
		{
			if (e.IsUsed && !IsEmpty && !MainForm.Singleton.FileIsReadOnly && ListViewLanguage.ContainsFocus)
			{
				if (e.PasteObject is FileCharacterName)
				{
					FileCharacterName	lPasteName = new FileCharacterName (mLangCurrent, e.PasteObject as FileCharacterName);

					e.IsUsed = true;

					if (LangIDName (mLangCurrent, true) == null)
					{
						AddDeleteCharacterName	lUpdate = new AddDeleteCharacterName (mCharacterFile, lPasteName, false, true);

						lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameStateUpdate_Applied);
						AddDeleteCharacterName.PutUndo (lUpdate.Apply () as AddDeleteCharacterName);
					}
					else
					{
						UpdateCharacterName		lUpdate = new UpdateCharacterName (mCharacterFile, lPasteName, true);

						lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableNameTextUpdate_Applied);
						UpdateCharacterName.PutUndo (lUpdate.Apply () as UpdateCharacterName);
					}
				}
			}
		}

		#endregion
	}
}
