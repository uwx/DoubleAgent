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

		//protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		//{
		//    if (ListViewLanguage.ContainsFocus)
		//    {
		//        FileCharacterName lName = LangIDName (mLangCurrent, true);

		//        if (lName != null)
		//        {
		//            String lObjectTitle = Titles.CharacterName (lName.Language);

		//            pEventArgs.PutCopyTitle (lObjectTitle);
		////            if (!Program.FileIsReadOnly && !mLangDefault.PrimaryLanguageEqual (lName))
		////            {
		////                pEventArgs.PutCutTitle (lObjectTitle);
		////                pEventArgs.PutDeleteTitle (lObjectTitle);
		////            }
		////        }
		////        if (!Program.FileIsReadOnly && (pEventArgs.PasteObject is FileCharacterName))
		////        {
		////            pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lName, Titles.CharacterName (mLangCurrent), Titles.CharacterName ((pEventArgs.PasteObject as FileCharacterName).Language));
		////        }
		////    }
		////}

		/////////////////////////////////////////////////////////////////////////////////

		//protected override Boolean EditCopy (Global.EditEventArgs pEventArgs)
		//{
		//    if (ListViewLanguage.ContainsFocus)
		//    {
		//        FileCharacterName lName = LangIDName (mLangCurrent, true);

		//        if (lName != null)
		//        {
		//            try
		//            {
		//                Clipboard.SetData (DataFormats.Serializable, lName);
		//            }
		//            catch
		//            {
		//            }
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		//protected override Boolean EditCut (Global.EditEventArgs pEventArgs)
		//{
		//    if (ListViewLanguage.ContainsFocus)
		//    {
		//        FileCharacterName lName = LangIDName (mLangCurrent, true);
		//        AddDeleteCharacterName lUpdate;

		//        if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
		//        {
		//            try
		//            {
		//                Clipboard.SetData (DataFormats.Serializable, lName);

		//                lUpdate = new AddDeleteCharacterName (lName, true, true);
		//                AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
		//            }
		//            catch
		//            {
		//            }
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		//protected override Boolean EditDelete (Global.EditEventArgs pEventArgs)
		//{
		//    if (ListViewLanguage.ContainsFocus)
		//    {
		//        FileCharacterName lName = LangIDName (mLangCurrent, true);
		//        AddDeleteCharacterName lUpdate;

		//        if ((lName != null) && !mLangDefault.PrimaryLanguageEqual (lName))
		//        {
		//            lUpdate = new AddDeleteCharacterName (lName, true, false);
		//            AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		//protected override Boolean EditPaste (Global.EditEventArgs pEventArgs)
		//{
		//    if (ListViewLanguage.ContainsFocus)
		//    {
		//        if (pEventArgs.PasteObject is FileCharacterName)
		//        {
		//            FileCharacterName lName = LangIDName (mLangCurrent, true);
		//            FileCharacterName lPasteName = new FileCharacterName (mLangCurrent, pEventArgs.PasteObject as FileCharacterName);

		//            if (lName == null)
		//            {
		//                AddDeleteCharacterName lUpdate = new AddDeleteCharacterName (lPasteName, false, true);

		//                AddDeleteCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteCharacterName, this);
		//            }
		//            else
		//            {
		//                UpdateCharacterName lUpdate = new UpdateCharacterName (lName, true);

		//                lPasteName.CopyTo (lName);
		//                lUpdate = lUpdate.Apply () as UpdateCharacterName;
		//                if (lUpdate != null)
		//                {
		//                    UpdateCharacterName.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterName, this);
		//                }
		//            }
		//            return true;
		//        }
		//    }
		//    return false;
		//}

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