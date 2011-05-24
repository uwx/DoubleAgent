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

		///////////////////////////////////////////////////////////////////////////////

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

		/////////////////////////////////////////////////////////////////////////////

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

		/////////////////////////////////////////////////////////////////////////////

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
}
