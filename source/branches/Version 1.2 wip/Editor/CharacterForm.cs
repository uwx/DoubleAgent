using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
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
							if (mCharacterFile.Names.Remove (lName))
							{
								ShowNameStates ();
								ShowCharacterName ();
								Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
							}
						}
						else
						{
							TextBoxName.Text = lName.Name;
						}
					}
					else
					{
						lName.Name = TextBoxName.Text;
						Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
					}
				}
				else if ((!String.IsNullOrEmpty (TextBoxName.Text)) && ((lName == null) || (TextBoxName.Text != lName.Name)))
				{
					if (mCharacterFile.Names.Add (mLangCurrent, TextBoxName.Text) != null)
					{
						ShowNameStates ();
						ShowCharacterName ();
						Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
					}
				}
			}
		}

		private void TextBoxDescription_Validated (object sender, EventArgs e)
		{
			if ((TextBoxDescription.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				FileCharacterName	lName = mCharacterFile.FindName (mLangCurrent);

				if ((lName != null) && ((Byte)lName.Language == (Byte)mLangCurrent))
				{
					lName.Desc1 = TextBoxDescription.Text;
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void TextBoxExtra_Validated (object sender, EventArgs e)
		{
			if ((TextBoxExtra.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				FileCharacterName	lName = mCharacterFile.FindName (mLangCurrent);

				if ((lName != null) && ((Byte)lName.Language == (Byte)mLangCurrent))
				{
					lName.Desc2 = TextBoxExtra.Text;
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxIconFile_Validated (object sender, EventArgs e)
		{
			if ((TextBoxIconFile.Modified) && (!Program.MainForm.FileIsReadOnly))
			{
				mCharacterFile.IconFilePath = TextBoxIconFile.Text;
				ShowCharacterIcon ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void ButtonIconImport_Click (object sender, EventArgs e)
		{
			if (!Program.MainForm.FileIsReadOnly)
			{
				String	lFilePath = mCharacterFile.IconFilePath;

				if (OpenFileDialogEx.OpenIconFile (ref lFilePath))
				{
					mCharacterFile.IconFilePath = lFilePath;
					ShowCharacterIcon ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonNewGUID_Click (object sender, EventArgs e)
		{
			if (!Program.MainForm.FileIsReadOnly)
			{
				mCharacterFile.Header.Guid = System.Guid.NewGuid ();
				ShowCharacterGuid ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		#endregion
	}
}
