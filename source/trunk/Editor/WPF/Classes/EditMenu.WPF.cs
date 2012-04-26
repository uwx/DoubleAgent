/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Global
{
	public class EditEventArgs : EventArgs
	{
		public EditEventArgs (ExecutedRoutedEventArgs pRoutedEventArgs)
		{
			RoutedEventArgs = pRoutedEventArgs;
		}

		public ExecutedRoutedEventArgs RoutedEventArgs
		{
			get;
			protected set;
		}

		//=============================================================================

		public Boolean PutCopyObject (Object pCopyObject)
		{
			if (pCopyObject != null)
			{
				try
				{
					Clipboard.SetData (DataFormats.Serializable, pCopyObject);
					return true;
				}
				catch
				{
				}
			}
			return false;
		}

		public Object GetPasteObject ()
		{
			if (!PasteObjectRetrieved)
			{
				PasteObjectRetrieved = true;
				if (Clipboard.ContainsData (DataFormats.Serializable))
				{
					PasteObject = Clipboard.GetData (DataFormats.Serializable);
				}
			}
			return PasteObject;
		}
		private Object PasteObject
		{
			get;
			set;
		}
		private Boolean PasteObjectRetrieved
		{
			get;
			set;
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	public class CanEditEventArgs : EventArgs
	{
		public CanEditEventArgs (CanExecuteRoutedEventArgs pRoutedEventArgs)
		{
			RoutedEventArgs = pRoutedEventArgs;
		}

		public CanExecuteRoutedEventArgs RoutedEventArgs
		{
			get;
			protected set;
		}
		public Boolean IsContextMenu
		{
			get
			{
				if ((RoutedEventArgs.Parameter is MenuItem) && (FocusManager.GetFocusScope (RoutedEventArgs.Parameter as MenuItem) is ContextMenu))
				{
					return true;
				}
				return false;
			}
		}

		//=============================================================================

		public void PutCutTitle (String pObjectTitle)
		{
			RoutedEventArgs.PutCutTitle (pObjectTitle);
		}

		public void PutCopyTitle (String pObjectTitle)
		{
			RoutedEventArgs.PutCopyTitle (pObjectTitle);
		}

		public void PutDeleteTitle (String pObjectTitle)
		{
			RoutedEventArgs.PutDeleteTitle (pObjectTitle);
		}

		public void PutPasteTitle (String pObjectTitle)
		{
			RoutedEventArgs.PutPasteTitle (pObjectTitle);
		}
		public void PutPasteTitle (String pPasteTypeTitle, String pObjectTitle)
		{
			RoutedEventArgs.PutPasteTitle (pPasteTypeTitle, pObjectTitle);
		}

		//=============================================================================

		public Object GetPasteObject ()
		{
			if (!PasteObjectRetrieved)
			{
				PasteObjectRetrieved = true;
				if (Clipboard.ContainsData (DataFormats.Serializable))
				{
					PasteObject = Clipboard.GetData (DataFormats.Serializable);
				}
			}
			return PasteObject;
		}
		private Object PasteObject
		{
			get;
			set;
		}
		private Boolean PasteObjectRetrieved
		{
			get;
			set;
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class EditEventsExtensions
	{
		public static void SetTipText (this Button pButton, String pTipText)
		{
			pButton.ToolTip = pTipText;
		}

		public static void SetTitle (this Button pButton, String pTitle)
		{
			pButton.ToolTip = pTitle.NoMenuPrefix ();
		}

		public static void SetTitle (this MenuItem pMenuItem, String pTitle)
		{
			pMenuItem.Header = pTitle;
		}

		//=============================================================================

		public static void SetTipText (this Control pControl, String pTipText)
		{
			if (pControl is Button)
			{
				(pControl as Button).ToolTip = pTipText;
			}
		}

		public static void SetTitle (this Control pControl, String pTitle)
		{
			if (pControl is Button)
			{
				(pControl as Button).ToolTip = pTitle.NoMenuPrefix ();
			}
			if (pControl is MenuItem)
			{
				(pControl as MenuItem).Header = pTitle.FixMenuPrefix ();
			}
		}

		public static void SetChecked (this Control pControl, Boolean pChecked)
		{
			if (pControl is ToggleButton)
			{
				(pControl as ToggleButton).IsChecked = pChecked;
			}
			if (pControl is MenuItem)
			{
				(pControl as MenuItem).IsChecked = pChecked;
			}
		}

		//=============================================================================

		public static void SetTipText (this CanExecuteRoutedEventArgs pEventArgs, String pTipText)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = pTipText;
			}
		}

		public static void SetTitle (this CanExecuteRoutedEventArgs pEventArgs, String pTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = pTitle.NoMenuPrefix ();
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = pTitle.FixMenuPrefix ();
			}
		}

		static public void SetChecked (this CanExecuteRoutedEventArgs pEventArgs, Boolean pChecked)
		{
			if (pEventArgs.Parameter is ToggleButton)
			{
				(pEventArgs.Parameter as ToggleButton).IsChecked = pChecked;
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).IsChecked = pChecked;
			}
		}

		//=============================================================================

		static public void PutCutTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutCutTitle (String.Empty);
		}

		static public void PutCutTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditCutThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditCutThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		//=============================================================================

		static public void PutCopyTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutCopyTitle (String.Empty);
		}

		static public void PutCopyTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditCopyThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditCopyThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		//=============================================================================

		static public void PutDeleteTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutDeleteTitle (String.Empty);
		}

		static public void PutDeleteTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditDeleteThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		//=============================================================================

		static public void PutPasteTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutPasteTitle (String.Empty);
		}

		static public void PutPasteTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditPasteThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditPasteThis.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		static public void PutPasteTitle (this CanExecuteRoutedEventArgs pEventArgs, String pPasteTypeTitle, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (pPasteTypeTitle.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (pPasteTypeTitle.FixMenuPrefix (), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		//=============================================================================

		static public void PutUndoTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutUndoTitle (String.Empty);
		}

		static public void PutUndoTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					if ((pEventArgs.OriginalSource is TextBoxBase) && (pEventArgs.OriginalSource as TextBoxBase).CanUndo)
					{
						(pEventArgs.Parameter as Button).ToolTip = AppResources.Resources.EditUndoText.NoMenuPrefix ();
					}
					else
					{
						(pEventArgs.Parameter as Button).ToolTip = AppResources.Resources.EditUndo.NoMenuPrefix ();
					}
				}
				else
				{
					(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditUndoThis.NoMenuPrefix (), pObjectTitle);
				}
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					if ((pEventArgs.OriginalSource is TextBoxBase) && (pEventArgs.OriginalSource as TextBoxBase).CanUndo)
					{
						(pEventArgs.Parameter as MenuItem).Header = AppResources.Resources.EditUndoText.FixMenuPrefix ();
					}
					else
					{
						(pEventArgs.Parameter as MenuItem).Header = AppResources.Resources.EditUndo.FixMenuPrefix ();
					}
				}
				else
				{
					(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditUndoThis.FixMenuPrefix (), pObjectTitle);
				}
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		//=============================================================================

		static public void PutRedoTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutRedoTitle (String.Empty);
		}

		static public void PutRedoTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					if ((pEventArgs.OriginalSource is TextBoxBase) && (pEventArgs.OriginalSource as TextBoxBase).CanRedo)
					{
						(pEventArgs.Parameter as Button).ToolTip = AppResources.Resources.EditRedoText.NoMenuPrefix ();
					}
					else
					{
						(pEventArgs.Parameter as Button).ToolTip = AppResources.Resources.EditRedo.NoMenuPrefix ();
					}
				}
				else
				{
					(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditRedoThis.NoMenuPrefix (), pObjectTitle);
				}
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				if (String.IsNullOrEmpty (pObjectTitle))
				{
					if ((pEventArgs.OriginalSource is TextBoxBase) && (pEventArgs.OriginalSource as TextBoxBase).CanRedo)
					{
						(pEventArgs.Parameter as MenuItem).Header = AppResources.Resources.EditRedoText.FixMenuPrefix ();
					}
					else
					{
						(pEventArgs.Parameter as MenuItem).Header = AppResources.Resources.EditRedo.FixMenuPrefix ();
					}
				}
				else
				{
					(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditRedoThis.FixMenuPrefix (), pObjectTitle);
				}
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}
	}
}
