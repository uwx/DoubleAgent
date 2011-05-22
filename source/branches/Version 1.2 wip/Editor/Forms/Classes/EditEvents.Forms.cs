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
using AgentCharacterEditor.Properties;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Global
{
	public class EditEventArgs : EventArgs
	{
		public EditEventArgs ()
		{
		}
		public EditEventArgs (String pClipboardFormat)
		{
			if (Clipboard.ContainsData (pClipboardFormat))
			{
				PasteObject = Clipboard.GetData (pClipboardFormat);
			}
		}

		public Object PasteObject
		{
			get;
			private set;
		}

		public virtual Boolean IsUsed
		{
			get
			{
				return mIsUsed;
			}
			set
			{
				mIsUsed = value;
			}
		}
		private Boolean mIsUsed = false;
	}

	public class CanEditEventArgs : EditEventArgs
	{
		public CanEditEventArgs ()
		{
		}
		public CanEditEventArgs (String pClipboardFormat)
			: base (pClipboardFormat)
		{
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

		public String PasteTitle
		{
			get
			{
				return mPasteTitle;
			}
			set
			{
				mPasteTitle = value;
			}
		}
		public String PasteObjectTitle
		{
			get;
			set;
		}
		private String mPasteTitle = AppResources.Resources.EditPasteThis;

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

		public void PutPasteTitle (String pPasteTypeTitle, String pObjectTitle)
		{
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
		public ContextMenuEventArgs (ContextMenuStrip pContextMenu, Control pActiveControl, String pClipboardFormat)
			: base (pClipboardFormat)
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
