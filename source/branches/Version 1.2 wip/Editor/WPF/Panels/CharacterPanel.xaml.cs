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

			lSettings.SelectedNameLanguage = (Int16)ListItemLangID (ListViewLanguage.SelectedItem as ListViewItem);
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
					lItem.Foreground = ListViewLanguage.Foreground;
				}
				else
				{
					lName = LangIDName (lLangID, true);
					if (lName == null)
					{
						lItem.Foreground = ListViewLanguage.Foreground;
					}
					else
					{
						lItem.Foreground = SystemColors.HotTrackBrush;
					}
				}
			}

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
				return ListItemLangID (ListViewLanguage.SelectedItem as ListViewItem);
			}
			return pUseDefault ? mLangDefault : (UInt16)0;
		}

		private Boolean SelectLangIDItem (UInt16 pLangID)
		{
			ListViewItem lItem = ListLangIDItem (pLangID);

			if (lItem != null)
			{
				lItem.IsSelected = true;
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
				ImageIconSmall.Source = null;
				ImageIconLarge.Source = null;
				TextBoxIconFile.Clear ();
				TextBoxIconFile.IsEnabled = false;
				ButtonIconImport.IsEnabled = false;
				ButtonIconExport.IsEnabled = false;
			}
			else
			{
				TextBoxIconFile.Text = CharacterFile.IconFilePath;

				if (CharacterFile.Header.Icon == null)
				{
					ImageIconSmall.Source = null;
					ImageIconLarge.Source = null;
				}
				else
				{
					try
					{
						ImageIconSmall.Source = CharacterFile.Header.Icon.ToBitmap ().MakeImageSource ();
						ImageIconLarge.Source = CharacterFile.Header.Icon.ToBitmap ().MakeImageSource ();
					}
					catch
					{
						ImageIconSmall.Source = null;
						ImageIconLarge.Source = null;
					}
				}
				TextBoxIconFile.IsEnabled = !Program.FileIsReadOnly;
				ButtonIconImport.IsEnabled = !Program.FileIsReadOnly;
				ButtonIconExport.IsEnabled = false;
			}

			try
			{
				if (ImageIconSmall.Source == null)
				{
					(ImageIconSmall.Parent as FrameworkElement).Effect = null;
				}
				else if ((ImageIconSmall.Parent as FrameworkElement).Effect == null)
				{
					(ImageIconSmall.Parent as FrameworkElement).Effect = new DropShadowEffect ();
				}
				if (ImageIconLarge.Source == null)
				{
					(ImageIconLarge.Parent as FrameworkElement).Effect = null;
				}
				else if ((ImageIconLarge.Parent as FrameworkElement).Effect == null)
				{
					(ImageIconLarge.Parent as FrameworkElement).Effect = new DropShadowEffect ();
				}

				if ((ImageIconSmall.Parent as FrameworkElement).Effect is DropShadowEffect)
				{
					((ImageIconSmall.Parent as FrameworkElement).Effect as DropShadowEffect).Opacity = 0.3;
				}
				if ((ImageIconLarge.Parent as FrameworkElement).Effect is DropShadowEffect)
				{
					((ImageIconLarge.Parent as FrameworkElement).Effect as DropShadowEffect).Opacity = 0.3;
				}
			}
			catch
			{
			}

			PopIsPanelShowing (lWasShowing);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewLanguage_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			if (!IsPanelShowing && !Program.FileIsReadOnly)
			{
				mLangCurrent = ShowCharacterName ();
			}
		}

		private void TextBoxName_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && TextBoxName.IsModified && !Program.FileIsReadOnly)
			{
				HandleNameChanged ();
				TextBoxName.IsModified = false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxDescription_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && TextBoxDescription.IsModified && !Program.FileIsReadOnly)
			{
				HandleDescriptionChanged ();
				TextBoxDescription.IsModified = false;
			}
		}

		private void TextBoxExtra_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && TextBoxExtra.IsModified && !Program.FileIsReadOnly)
			{
				HandleExtraChanged ();
				TextBoxExtra.IsModified = false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxIconFile_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && TextBoxIconFile.IsModified && !Program.FileIsReadOnly)
			{
				HandleIconFileChanged ();
			}
		}

		private void ButtonIconImport_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !Program.FileIsReadOnly)
			{
				HandleIconImport ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonNewGUID_Click (object sender, RoutedEventArgs e)
		{
			if (!IsPanelShowing && !Program.FileIsReadOnly)
			{
				HandleNewGUID ();
			}
		}

		#endregion
	}
}
