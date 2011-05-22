﻿/////////////////////////////////////////////////////////////////////////////
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
using System.Windows.Controls;
using System.Windows.Input;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Global
{
	public static partial class StringExtensions
	{
		/// <summary>
		/// Updates the mnemonic prefix indicator ('&amp;') in a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix updated for the current application build.</returns>
		static public String FixMenuPrefix (this String pString)
		{
			return pString.Replace ("&", "_");
		}

		/// <summary>
		/// Strips the mnemonic prefix indicator ('&amp;') from a string.
		/// </summary>
		/// <param name="pString">The initial string value.</param>
		/// <returns>The string value with any mnemonic prefix removed.</returns>
		static public String NoMenuPrefix (this String pString)
		{
			return pString.Replace ("&", "");
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	public static class EditEventsExtensions
	{
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
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditCutThis.FixMenuPrefix(), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditCopyThis.FixMenuPrefix(), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditDeleteThis.FixMenuPrefix(), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditPasteThis.FixMenuPrefix(), pObjectTitle);
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
				(pEventArgs.Parameter as MenuItem).Header = String.Format (pPasteTypeTitle.FixMenuPrefix(), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutUndoTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutUndoTitle (String.Empty);
		}

		static public void PutUndoTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditUndoThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditUndoThis.FixMenuPrefix(), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		static public void PutRedoTitle (this CanExecuteRoutedEventArgs pEventArgs)
		{
			pEventArgs.PutRedoTitle (String.Empty);
		}

		static public void PutRedoTitle (this CanExecuteRoutedEventArgs pEventArgs, String pObjectTitle)
		{
			if (pEventArgs.Parameter is Button)
			{
				(pEventArgs.Parameter as Button).ToolTip = String.Format (AppResources.Resources.EditRedoThis.NoMenuPrefix (), pObjectTitle);
			}
			if (pEventArgs.Parameter is MenuItem)
			{
				(pEventArgs.Parameter as MenuItem).Header = String.Format (AppResources.Resources.EditRedoThis.FixMenuPrefix(), pObjectTitle);
			}
			if (!String.IsNullOrEmpty (pObjectTitle))
			{
				pEventArgs.CanExecute = true;
			}
		}
	}
}
