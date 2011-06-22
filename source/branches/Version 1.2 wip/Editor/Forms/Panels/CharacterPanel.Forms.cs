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

			if (ListViewLanguage.SelectedItem != null)
			{
				lSettings.SelectedNameLanguage = (Int16)ListItemLangID (ListViewLanguage.SelectedItem as ListViewItemCommon);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowNameItemState (ListViewItemCommon pItem, Boolean pPresent)
		{
			pItem.ForeColor = pPresent ? SystemColors.HotTrack : ListViewLanguage.ForeColor;
		}

		private Boolean SelectLangIDItem (UInt16 pLangID)
		{
			ListViewItemCommon lItem = ListLangIDItem (pLangID);

			if (lItem != null)
			{
				lItem.IsSelected = true;
				ListViewLanguage.FocusedItem = lItem;
				return true;
			}
			return false;
		}

		//=============================================================================

		private void ShowSmallIcon (System.Drawing.Bitmap pIcon)
		{
			PictureBoxIconSmall.Image = pIcon;
		}

		private void ShowLargeIcon (System.Drawing.Bitmap pIcon)
		{
			PictureBoxIconLarge.Image = pIcon;
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
			if (TextBoxName.Modified)
			{
				HandleNameChanged ();
			}
			TextBoxName.Modified = false;
		}

		//=============================================================================

		private void TextBoxDescription_Validated (object sender, EventArgs e)
		{
			if (TextBoxDescription.Modified)
			{
				HandleDescriptionChanged ();
			}
			TextBoxDescription.Modified = false;
		}

		private void TextBoxExtra_Validated (object sender, EventArgs e)
		{
			if (TextBoxExtra.Modified)
			{
				HandleExtraChanged ();
			}
			TextBoxExtra.Modified = false;
		}

		//=============================================================================

		private void TextBoxIconFile_Validated (object sender, EventArgs e)
		{
			if (TextBoxIconFile.Modified)
			{
				HandleIconFileChanged ();
			}
			TextBoxIconFile.Modified = false;
		}

		private void ButtonIconImport_Click (object sender, EventArgs e)
		{
			HandleIconImport ();
		}

		//=============================================================================

		private void ButtonNewGUID_Click (object sender, EventArgs e)
		{
			HandleNewGUID ();
		}

		#endregion
	}
}
