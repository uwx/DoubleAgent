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
using System.Windows.Forms;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Global
{
	public class EditEventArgs : EventArgs
	{
		public EditEventArgs ()
		{
		}

		public virtual Boolean IsUsed
		{
			get;
			set;
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

	///////////////////////////////////////////////////////////////////////////////

	public class CanEditEventArgs : EditEventArgs
	{
		public CanEditEventArgs ()
		{
			PasteTypeTitle = AppResources.Resources.EditPasteThis;
		}

		public override Boolean IsUsed
		{
			get
			{
				return base.IsUsed || !String.IsNullOrEmpty (CopyObjectTitle) || !String.IsNullOrEmpty (CutObjectTitle) || !String.IsNullOrEmpty (DeleteObjectTitle) || !String.IsNullOrEmpty (PasteObjectTitle);
			}
			set
			{
				base.IsUsed = value;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public String CopyObjectTitle
		{
			get;
			protected set;
		}

		public String CutObjectTitle
		{
			get;
			protected set;
		}

		public String DeleteObjectTitle
		{
			get;
			protected set;
		}

		public String PasteObjectTitle
		{
			get;
			set;
		}
 		public String PasteTypeTitle
		{
			get; set;
		}

		///////////////////////////////////////////////////////////////////////////////

		public void PutCutTitle (String pObjectTitle)
		{
			CutObjectTitle = pObjectTitle;
		}

		public void PutCopyTitle (String pObjectTitle)
		{
			CopyObjectTitle = pObjectTitle;
		}

		public void PutDeleteTitle (String pObjectTitle)
		{
			DeleteObjectTitle = pObjectTitle;
		}

		public void PutPasteTitle (String pObjectTitle)
		{
			PasteObjectTitle = pObjectTitle;
		}
		public void PutPasteTitle (String pPasteTypeTitle, String pObjectTitle)
		{
			PasteTypeTitle = pPasteTypeTitle;
			PasteObjectTitle = pObjectTitle;
		}
	}

	public delegate void EditEventHandler (object sender, EditEventArgs e);
	public delegate void CanEditEventHandler (object sender, CanEditEventArgs e);

	///////////////////////////////////////////////////////////////////////////////

	public class ContextMenuEventArgs : CanEditEventArgs
	{
		public ContextMenuEventArgs (ContextMenuStrip pContextMenu, Control pActiveControl)
		{
			ContextMenu = pContextMenu;
			ActiveControl = pActiveControl;
		}

		public ContextMenuStrip ContextMenu
		{
			get;
			private set;
		}
		public Control ActiveControl
		{
			get;
			private set;
		}
	}

	public delegate void ContextMenuEventHandler (object sender, ContextMenuEventArgs e);
}
