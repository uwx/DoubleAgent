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

		private void ShowCharacterName (FileCharacterName pName, UInt16 pLangID)
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (pName == null)
			{
				TextBoxName.Clear ();
				TextBoxDescription.Clear ();
				TextBoxExtra.Clear ();

				TextBoxName.IsEnabled = false;
				TextBoxDescription.IsEnabled = false;
				TextBoxExtra.IsEnabled = false;
			}
			else
			{
				TextBoxName.Text = pName.Name;
				TextBoxDescription.Text = pName.Desc1;
				TextBoxExtra.Text = pName.Desc2;

				TextBoxName.IsReadOnly = Program.FileIsReadOnly;
				TextBoxDescription.IsReadOnly = Program.FileIsReadOnly || !pLangID.PrimaryLanguageEqual (pName);
				TextBoxExtra.IsReadOnly = Program.FileIsReadOnly || !pLangID.PrimaryLanguageEqual (pName);

				TextBoxName.IsEnabled = true;
				TextBoxDescription.IsEnabled = true;
				TextBoxExtra.IsEnabled = true;
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

		private void ShowCharacterGuid ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (IsPanelEmpty)
			{
				TextBoxGUID.Clear ();
				ButtonNewGUID.IsEnabled = false;
			}
			else
			{
				if (CharacterFile.Header.Guid == null)
				{
					TextBoxGUID.Clear ();
				}
				else
				{
					TextBoxGUID.Text = CharacterFile.Header.Guid.ToString ().ToUpper ();
				}
				ButtonNewGUID.IsEnabled = !Program.FileIsReadOnly;
			}

			PopIsPanelShowing (lWasShowing);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private void ListViewLanguage_CanEditCut (object sender, CanExecuteRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					e.PutCutTitle (Titles.CharacterName (lName.Language));
				}
			}
			e.Handled = true;
		}
		private void ListViewLanguage_OnEditCut (object sender, ExecutedRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lName);

						AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lName, true, true);
						AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					catch
					{
					}
				}
			}
			e.Handled = true;
		}

		private void ListViewLanguage_CanEditCopy (object sender, CanExecuteRoutedEventArgs e)
		{
			FileCharacterName lName = LangIDName (mLangCurrent, true);

			if (lName != null)
			{
				e.PutCopyTitle (Titles.CharacterName (lName.Language));
			}
			e.Handled = true;
		}
		private void ListViewLanguage_OnEditCopy (object sender, ExecutedRoutedEventArgs e)
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
			}
			e.Handled = true;
		}

		private void ListViewLanguage_CanEditDelete (object sender, CanExecuteRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					e.PutDeleteTitle (Titles.CharacterName (lName.Language));
				}
			}
			e.Handled = true;
		}
		private void ListViewLanguage_OnEditDelete (object sender, ExecutedRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lName, true, false);
					AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
				}
			}
			e.Handled = true;
		}

		private void ListViewLanguage_CanEditPaste (object sender, CanExecuteRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly && Clipboard.ContainsData (DataFormats.Serializable))
			{
				FileCharacterName lPasteObject = Clipboard.GetData (DataFormats.Serializable) as FileCharacterName;

				if (lPasteObject != null)
				{
					FileCharacterName lName = LangIDName (mLangCurrent, true);

					e.PutPasteTitle (Titles.PasteTypeTitle (lName, Titles.CharacterName (mLangCurrent), Titles.CharacterName (lPasteObject.Language)));
				}
			}
			e.Handled = true;
		}
		private void ListViewLanguage_OnEditPaste (object sender, ExecutedRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly && Clipboard.ContainsData (DataFormats.Serializable))
			{
				FileCharacterName lPasteObject = Clipboard.GetData (DataFormats.Serializable) as FileCharacterName;

				if (lPasteObject != null)
				{
					FileCharacterName lName = LangIDName (mLangCurrent, true);
					FileCharacterName lPasteName = new FileCharacterName (mLangCurrent, lPasteObject);

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
				}
			}
			e.Handled = true;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewLanguage_SelectionChanged (object sender, SelectionChangedEventArgs e)
		{
			mLangCurrent = ShowCharacterName ();
		}

		private void TextBoxName_TextChanged (object sender, TextChangedEventArgs e)
		{
			//if ((TextBoxName.Modified) && (!Program.FileIsReadOnly))
			if (!Program.FileIsReadOnly)
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
			//TextBoxName.Modified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxDescription_TextChanged (object sender, TextChangedEventArgs e)
		{
			//if ((TextBoxDescription.Modified) && (!Program.FileIsReadOnly))
			if (!Program.FileIsReadOnly)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				UpdateCharacterName lUpdate;

				if (lName != null)
				{
					lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
					UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
				}
			}
			//TextBoxDescription.Modified = false;
		}

		private void TextBoxExtra_TextChanged (object sender, TextChangedEventArgs e)
		{
			//if ((TextBoxExtra.Modified) && (!Program.FileIsReadOnly))
			if (!Program.FileIsReadOnly)
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				UpdateCharacterName lUpdate;

				if (lName != null)
				{
					lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
					UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
				}
			}
			//TextBoxExtra.Modified = false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void TextBoxIconFile_TextChanged (object sender, TextChangedEventArgs e)
		{
			//if ((TextBoxIconFile.Modified) && (!Program.FileIsReadOnly))
			if (!Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.IconFilePath = TextBoxIconFile.Text;
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
		}

		private void ButtonIconImport_Click (object sender, RoutedEventArgs e)
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

		private void ButtonNewGUID_Click (object sender, RoutedEventArgs e)
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
