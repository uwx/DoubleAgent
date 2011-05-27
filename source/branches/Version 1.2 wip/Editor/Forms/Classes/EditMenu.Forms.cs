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
using System.ComponentModel;
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

		public void UpdateMenuItems (ToolStripMenuItem pCopyItem, ToolStripMenuItem pCutItem, ToolStripMenuItem pDeleteItem, ToolStripMenuItem pPasteItem)
		{
			if (pCopyItem != null)
			{
				if (!String.IsNullOrEmpty (CopyObjectTitle))
				{
					pCopyItem.Enabled = true;
					pCopyItem.Text = String.Format (AppResources.Resources.EditCopyThis.FixMenuPrefix (), CopyObjectTitle);
				}
				else
				{
					pCopyItem.Enabled = false;
					pCopyItem.Text = AppResources.Resources.EditCopy.FixMenuPrefix ();
				}
			}

			if (pCutItem != null)
			{
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (CutObjectTitle))
				{
					pCutItem.Enabled = true;
					pCutItem.Text = String.Format (AppResources.Resources.EditCutThis.FixMenuPrefix (), CutObjectTitle);
				}
				else
				{
					pCutItem.Enabled = false;
					pCutItem.Text = AppResources.Resources.EditCut.FixMenuPrefix ();
				}
			}

			if (pDeleteItem != null)
			{
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (DeleteObjectTitle))
				{
					pDeleteItem.Enabled = true;
					pDeleteItem.Text = String.Format (AppResources.Resources.EditDeleteThis.FixMenuPrefix (), DeleteObjectTitle);
				}
				else
				{
					pDeleteItem.Enabled = false;
					pDeleteItem.Text = AppResources.Resources.EditDelete.FixMenuPrefix ();
				}
			}

			if (pPasteItem != null)
			{
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (PasteObjectTitle))
				{
					if (!String.IsNullOrEmpty (PasteTypeTitle))
					{
						pPasteItem.Enabled = true;
						pPasteItem.Text = String.Format (PasteTypeTitle.FixMenuPrefix (), PasteObjectTitle);
					}
					else
					{
						pPasteItem.Enabled = false;
						pPasteItem.Text = PasteObjectTitle;
					}
				}
				else
				{
					pPasteItem.Enabled = false;
					pPasteItem.Text = AppResources.Resources.EditPaste.FixMenuPrefix ();
				}
			}
		}

		public void UpdateToolStripButtons (ToolStripItem pCopyItem, ToolStripItem pCutItem, ToolStripItem pDeleteItem, ToolStripItem pPasteItem)
		{
			if (pCopyItem != null)
			{
				if (!String.IsNullOrEmpty (CopyObjectTitle))
				{
					pCopyItem.Enabled = true;
					pCopyItem.Text = String.Format (AppResources.Resources.EditCopyThis.NoMenuPrefix (), CopyObjectTitle);
				}
				else
				{
					pCopyItem.Enabled = false;
					pCopyItem.Text = AppResources.Resources.EditCopy.NoMenuPrefix ();
				}
			}

			if (pCutItem != null)
			{
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (CutObjectTitle))
				{
					pCutItem.Enabled = true;
					pCutItem.Text = String.Format (AppResources.Resources.EditCutThis.NoMenuPrefix (), CutObjectTitle);
				}
				else
				{
					pCutItem.Enabled = false;
					pCutItem.Text = AppResources.Resources.EditCut.NoMenuPrefix ();
				}
			}

			if (pDeleteItem != null)
			{
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (DeleteObjectTitle))
				{
					pDeleteItem.Enabled = true;
					pDeleteItem.Text = String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), DeleteObjectTitle);
				}
				else
				{
					pDeleteItem.Enabled = false;
					pDeleteItem.Text = AppResources.Resources.EditDelete.NoMenuPrefix ();
				}
			}

			if (pPasteItem != null)
			{
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (PasteObjectTitle))
				{
					if (!String.IsNullOrEmpty (PasteTypeTitle))
					{
						pPasteItem.Enabled = true;
						pPasteItem.Text = String.Format (PasteTypeTitle.NoMenuPrefix (), PasteObjectTitle);
					}
					else
					{
						pPasteItem.Enabled = false;
						pPasteItem.Text = PasteObjectTitle;
					}
				}
				else
				{
					pPasteItem.Enabled = false;
					pPasteItem.Text = AppResources.Resources.EditPaste.NoMenuPrefix ();
				}
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
			get;
			set;
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

	public class ContextMenuEdit : ContextMenuStrip
	{
		public ContextMenuEdit (IContainer pContainer)
			: base (pContainer)
		{
			Name = "ContextMenuStub";
		}

		public ToolStripMenuItem AddMenuItemClone (ToolStripMenuItem pMenuItem)
		{
			ToolStripMenuItem lMenuItem = new ToolStripMenuItem ();

			lMenuItem.Name = pMenuItem.Name;
			lMenuItem.Text = pMenuItem.Text;
			lMenuItem.Image = pMenuItem.Image;
			lMenuItem.ImageTransparentColor = pMenuItem.ImageTransparentColor;
			lMenuItem.ShortcutKeys = lMenuItem.ShortcutKeys;
			lMenuItem.Enabled = pMenuItem.Enabled;

			Items.Add (lMenuItem);
			return lMenuItem;
		}
	}

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
