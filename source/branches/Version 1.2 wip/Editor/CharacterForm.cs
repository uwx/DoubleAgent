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
		}

		public void LoadConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		public void SaveConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
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
				UInt16									lLangID = ListItemLangID (lItem);
				FileCharacterName	lName = null;

				if (mCharacterFile == null)
				{
					lItem.ForeColor = ListViewLanguage.ForeColor;
				}
				else
				{
					lName = mCharacterFile.FindName (lLangID);
					if ((lName != null) && ((Byte)lName.Language != (Byte)lLangID))
					{
						lName = null;
					}
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
				lLangID = ListItemLangID (ListViewLanguage.SelectedItems[0]);
			}
			if (mCharacterFile != null)
			{
				lName = mCharacterFile.FindName (lLangID);
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

				TextBoxName.ReadOnly = Program.MainForm.FileIsReadOnly;
				TextBoxDescription.ReadOnly = Program.MainForm.FileIsReadOnly || ((Byte)lName.Language != (Byte)lLangID);
				TextBoxExtra.ReadOnly = Program.MainForm.FileIsReadOnly || ((Byte)lName.Language != (Byte)lLangID);

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

		private bool SelectLangIDItem (UInt16 pLangID)
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
			if (mCharacterFile == null)
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

				TextBoxIconFile.Enabled = !Program.MainForm.FileIsReadOnly;
				ToolStripIconFile.Enabled = !Program.MainForm.FileIsReadOnly;
			}
		}

		private void ShowCharacterGuid ()
		{
			if (mCharacterFile == null)
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
				ButtonNewGUID.Enabled = !Program.MainForm.FileIsReadOnly;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class AddRemoveCharacterName : UndoableAddRemove<FileCharacterName>
		{
			public AddRemoveCharacterName (CharacterFile pCharacterFile, FileCharacterName pName, bool pRemove)
				: base (pCharacterFile, pName)
			{
				this.IsRemove = pRemove;
			}

			public override UndoUnit Apply ()
			{
				AddRemoveCharacterName	lApplied = null;

				if (IsRemove)
				{
					if (CharacterFile.Names.Remove (this.Target))
					{
						lApplied = new AddRemoveCharacterName (CharacterFile, this.Target, false);
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
						lApplied = new AddRemoveCharacterName (CharacterFile, lName, true);
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
				return String.Format ("{0} {1} name \"{2}\"", this.AddRemoveTitle, new System.Globalization.CultureInfo (this.Target.Language).EnglishName, this.Target.Name);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateCharacterName : UndoableUpdate
		{
			public UpdateCharacterName (CharacterFile pCharacterFile, UInt16 pLanguage, String pName, String pDesc1, String pDesc2)
				: base (pCharacterFile)
			{
				this.Language = pLanguage;
				this.Name = pName;
				this.Desc1 = pDesc1;
				this.Desc2 = pDesc2;
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

			public override UndoUnit Apply ()
			{
				UpdateCharacterName	lApplied = null;
				FileCharacterName	lName;
				String				lSwap;

				if (CharacterFile.Names.Contains (this.Language))
				{
					lName = CharacterFile.Names[this.Language];
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

			public override string ToString ()
			{
				return String.Format ("change {0} name", new System.Globalization.CultureInfo (this.Language).EnglishName);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateCharacterGuid : UndoableUpdate
		{
			public UpdateCharacterGuid (CharacterFile pCharacterFile, Guid pGuid)
				: base (pCharacterFile)
			{
				this.Guid = pGuid;
			}

			public Guid Guid
			{
				get;
				private set;
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

			public override string ToString ()
			{
				return "change unique identifier";
			}
		}

		internal class UpdateCharacterIcon : UndoableUpdate
		{
			public UpdateCharacterIcon (CharacterFile pCharacterFile, String pIconPath)
				: base (pCharacterFile)
			{
				this.IconPath = pIconPath;
			}

			public String IconPath
			{
				get;
				private set;
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

			public override string ToString ()
			{
				return "icon";
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
			if ((TextBoxName.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				FileCharacterName	lName = mCharacterFile.FindName (mLangCurrent);

				if ((lName != null) && ((Byte)lName.Language == (Byte)mLangCurrent))
				{
					if (String.IsNullOrEmpty (TextBoxName.Text))
					{
						if (String.IsNullOrEmpty (lName.Desc1) && String.IsNullOrEmpty (lName.Desc2))
						{
							AddRemoveCharacterName lUpdate = new AddRemoveCharacterName (mCharacterFile, lName, true);
							lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableNameStateUpdate_Applied);
							AddRemoveCharacterName.PutUndo (lUpdate.Apply () as AddRemoveCharacterName);
						}
						else
						{
							TextBoxName.Text = lName.Name;
						}
					}
					else
					{
						UpdateCharacterName lUpdate = new UpdateCharacterName (mCharacterFile, lName.Language, TextBoxName.Text, null, null);
						lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableNameTextUpdate_Applied);
						UpdateCharacterName.PutUndo (lUpdate.Apply () as UpdateCharacterName);
					}
				}
				else if ((!String.IsNullOrEmpty (TextBoxName.Text)) && ((lName == null) || (TextBoxName.Text != lName.Name)))
				{
					lName = new FileCharacterName (mLangCurrent, TextBoxName.Text);
					AddRemoveCharacterName lUpdate = new AddRemoveCharacterName (mCharacterFile, lName, false);
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableNameStateUpdate_Applied);
					AddRemoveCharacterName.PutUndo (lUpdate.Apply () as AddRemoveCharacterName);
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
			if ((TextBoxDescription.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				FileCharacterName	lName = mCharacterFile.FindName (mLangCurrent);
				UndoableUpdate		lUpdate;

				if ((lName != null) && ((Byte)lName.Language == (Byte)mLangCurrent))
				{
					lUpdate = new UpdateCharacterName (mCharacterFile, lName.Language, null, TextBoxDescription.Text, null);
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableNameTextUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
				}
			}
			TextBoxDescription.Modified = false;
		}

		private void TextBoxExtra_Validated (object sender, EventArgs e)
		{
			if ((TextBoxExtra.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				FileCharacterName	lName = mCharacterFile.FindName (mLangCurrent);
				UndoableUpdate		lUpdate;

				if ((lName != null) && ((Byte)lName.Language == (Byte)mLangCurrent))
				{
					lUpdate = new UpdateCharacterName (mCharacterFile, lName.Language, null, null, TextBoxExtra.Text);
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableNameTextUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
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
			if ((TextBoxIconFile.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				UndoableUpdate	lUpdate = new UpdateCharacterIcon (mCharacterFile, TextBoxIconFile.Text);

				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableIconUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
			}
		}

		private void ButtonIconImport_Click (object sender, EventArgs e)
		{
			if (!Program.MainForm.FileIsReadOnly)
			{
				String			lFilePath = mCharacterFile.IconFilePath;
				UndoableUpdate	lUpdate;

				if (OpenFileDialogEx.OpenIconFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterIcon (mCharacterFile, lFilePath);
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableIconUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
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
			if (!Program.MainForm.FileIsReadOnly)
			{
				UndoableUpdate	lUpdate = new UpdateCharacterGuid (mCharacterFile, System.Guid.NewGuid ());

				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableGuidUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
			}
		}

		private void UndoableGuidUpdate_Applied (object sender, EventArgs e)
		{
			ShowCharacterGuid ();
		}

		#endregion
	}
}
