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
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;

namespace AgentCharacterEditor.Panels
{
	public partial class CharacterPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		internal const UInt16 mLangDefault = 0x0409;
		private UInt16 mLangCurrent = 0;

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
				SelectedName = pPanel.SelectedLangID (true);
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
			Boolean lWasShowing = PushIsPanelShowing (true);

			ShowCharacterIcon ();
			ShowCharacterGuid ();
			ShowNameStates ();
			if (ListViewLanguage.SelectedItems.Count <= 0)
			{
				SelectLangIDItem (mLangDefault);
			}
			mLangCurrent = ShowCharacterName ();

			PopIsPanelShowing (lWasShowing);
		}

		private UInt16 ShowCharacterName ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);
			FileCharacterName lName = null;
			UInt16 lLangID = SelectedLangID (true);

			if (!IsPanelEmpty)
			{
				lName = LangIDName (lLangID, false);
			}
			ShowCharacterName (lName, lLangID);

			PopIsPanelShowing (lWasShowing);
			return lLangID;
		}
 
		private void ShowCharacterName (FileCharacterName pName, UInt16 pLangID)
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			LabelName.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 
			LabelDescription.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 
			LabelExtra.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly; 

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
			TextBoxName.IsModified = false;
			TextBoxDescription.IsModified = false;
			TextBoxExtra.IsModified = false;

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

		///////////////////////////////////////////////////////////////////////////////

		private FileCharacterName LangIDName (UInt16 pLangID, Boolean pExactMatch)
		{
			FileCharacterName lName = null;

			if (!IsPanelEmpty)
			{
				lName = CharacterFile.FindName (pLangID);

				if (pExactMatch && !pLangID.PrimaryLanguageEqual (lName))
				{
					lName = null;
				}
			}
			return lName;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		protected override Boolean HandleCanEditCopy (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					pEventArgs.PutCopyTitle (Titles.CharacterName (lName.Language));
				}
				return true;
			}
			return false;
		}

		protected override Boolean HandleEditCopy (EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if (lName != null)
				{
					pEventArgs.PutCopyObject (lName);
				}
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override Boolean HandleCanEditCut (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					pEventArgs.PutCutTitle (Titles.CharacterName (lName.Language));
				}
				return true;
			}
			return false;
		}

		protected override Boolean HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);
				AddDeleteCharacterName lUpdate;

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					if (pEventArgs.PutCopyObject (lName))
					{
						lUpdate = new AddDeleteCharacterName (lName, true, true);
						AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
					}
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override Boolean HandleCanEditDelete (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lName = LangIDName (mLangCurrent, true);

				if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
				{
					pEventArgs.PutCutTitle (Titles.CharacterName (lName.Language));
				}
				return true;
			}
			return false;
		}

		protected override Boolean HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
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

		///////////////////////////////////////////////////////////////////////////////

		protected override Boolean HandleCanEditPaste (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lPasteObject = pEventArgs.GetPasteObject () as FileCharacterName;

				if (lPasteObject != null)
				{
					FileCharacterName lName = LangIDName (mLangCurrent, true);

					pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lName, Titles.CharacterName (mLangCurrent), Titles.CharacterName (lPasteObject.Language)));
				}
				return true;
			}
			return false;
		}

		protected override Boolean HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewLanguage, pEventArgs))
			{
				FileCharacterName lPasteObject = pEventArgs.GetPasteObject () as FileCharacterName;

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
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void HandleNameChanged ()
		{
			FileCharacterName lName = LangIDName (mLangCurrent, true);

			if (lName != null)
			{
				if (String.IsNullOrEmpty (TextBoxName.Text))
				{
					if (String.IsNullOrEmpty (lName.Desc1) && String.IsNullOrEmpty (lName.Desc2) && !mLangDefault.PrimaryLanguageEqual (lName))
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

		protected void HandleDescriptionChanged ()
		{
			FileCharacterName lName = LangIDName (mLangCurrent, true);
			UpdateCharacterName lUpdate;

			if (lName != null)
			{
				lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
				UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
			}
		}

		protected void HandleExtraChanged ()
		{
			FileCharacterName lName = LangIDName (mLangCurrent, true);
			UpdateCharacterName lUpdate;

			if (lName != null)
			{
				lUpdate = new UpdateCharacterName (lName.Language, TextBoxName.Text, TextBoxDescription.Text, TextBoxExtra.Text);
				UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void HandleIconFileChanged ()
		{
			UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

			lUpdate.IconFilePath = TextBoxIconFile.Text;
			UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
		}

		protected void HandleIconImport ()
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

		protected void HandleNewGUID ()
		{
			UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

			lUpdate.Guid = System.Guid.NewGuid ();
			UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
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
	}
}