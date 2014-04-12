/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Effects;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class CharacterPanel : AgentCharacterEditor.Panels.FilePartPanel
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

			lSettings.SelectedNameLanguage = (Int16)ListItemLangID (ListViewLanguage.SelectedItem as ListViewItemCommon);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowNameItemState (ListViewItemCommon pItem, Boolean pPresent)
		{
			pItem.Foreground = pPresent ? SystemColors.HotTrackBrush : ListViewLanguage.Foreground;
		}

		private void ShowCharacterNameState (FileCharacterName pName, UInt16 pLangID)
		{
			System.Globalization.CultureInfo lCulture = (pName==null) ? new	System.Globalization.CultureInfo (mLangDefault)  : new	System.Globalization.CultureInfo (pName.Language);

			TextBoxName.FlowDirection =  lCulture.TextInfo.IsRightToLeft ? FlowDirection.RightToLeft : FlowDirection.LeftToRight;
			TextBoxDescription.FlowDirection =  lCulture.TextInfo.IsRightToLeft ? FlowDirection.RightToLeft : FlowDirection.LeftToRight;
			TextBoxExtra.FlowDirection =  lCulture.TextInfo.IsRightToLeft ? FlowDirection.RightToLeft : FlowDirection.LeftToRight;

			if ((pName == null) || pLangID.PrimaryLanguageEqual (pName))
			{
				TextBoxDescription.Background = SystemColors.WindowBrush;
				TextBoxExtra.Background = SystemColors.WindowBrush;
			}
			else
			{
				TextBoxDescription.Background = Program.MainWindow.Background;
				TextBoxExtra.Background = Program.MainWindow.Background;
			}
		}

		private Boolean SelectLangIDItem (UInt16 pLangID)
		{
			ListViewItemCommon lItem = ListLangIDItem (pLangID);

			if (lItem != null)
			{
				lItem.IsSelected = true;
				//ListViewLanguage.FocusedItem = lItem;
				return true;
			}
			return false;
		}

		//=============================================================================

		private void ShowSmallIcon (System.Drawing.Bitmap pIcon)
		{
			ImageIconSmall.Source = (pIcon == null) ? null : pIcon.MakeImageSource ();
		}

		private void ShowLargeIcon (System.Drawing.Bitmap pIcon)
		{
			ImageIconLarge.Source = (pIcon == null) ? null : pIcon.MakeImageSource ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewLanguage_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			if (!IsPanelFilling)
			{
				mLangCurrent = ShowCharacterName ();
			}
		}

		private void TextBoxName_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (TextBoxName.IsModified)
			{
				HandleNameChanged ();
			}
			TextBoxName.IsModified = false;
		}

		//=============================================================================

		private void TextBoxDescription_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (TextBoxDescription.IsModified)
			{
				HandleDescriptionChanged ();
			}
			TextBoxDescription.IsModified = false;
		}

		private void TextBoxExtra_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (TextBoxExtra.IsModified)
			{
				HandleExtraChanged ();
			}
			TextBoxExtra.IsModified = false;
		}

		//=============================================================================

		private void TextBoxIconFile_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (TextBoxIconFile.IsModified)
			{
				HandleIconFileChanged ();
			}
			TextBoxIconFile.IsModified = false;
		}

		private void ButtonIconImport_Click (object sender, RoutedEventArgs e)
		{
			HandleIconImport ();
		}

		//=============================================================================

		private void ButtonNewGUID_Click (object sender, RoutedEventArgs e)
		{
			HandleNewGUID ();
		}

		#endregion
	}
}
