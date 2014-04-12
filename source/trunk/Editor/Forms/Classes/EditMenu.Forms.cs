/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
	public class ContextMenuEdit : ContextMenuStrip
	{
		public ToolStripMenuItem MenuItemCut;
		public ToolStripMenuItem MenuItemCopy;
		public ToolStripMenuItem MenuItemPaste;
		public ToolStripMenuItem MenuItemDelete;

		public ContextMenuEdit (IContainer pContainer)
			: base (pContainer)
		{
			Name = "ContextMenuEdit";
		}

		public void InitializeComponent ()
		{
			this.MenuItemCut = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemCopy = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemPaste = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemDelete = new System.Windows.Forms.ToolStripMenuItem ();
			this.SuspendLayout ();
			// 
			// MenuItemCut
			// 
			this.MenuItemCut.Enabled = false;
			this.MenuItemCut.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditCut;
			this.MenuItemCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemCut.Name = "MenuItemCut";
			this.MenuItemCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
			this.MenuItemCut.Size = new System.Drawing.Size (181, 28);
			this.MenuItemCut.Text = "Cu&t";
			// 
			// MenuItemCopy
			// 
			this.MenuItemCopy.Enabled = false;
			this.MenuItemCopy.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditCopy;
			this.MenuItemCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemCopy.Name = "MenuItemCopy";
			this.MenuItemCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
			this.MenuItemCopy.Size = new System.Drawing.Size (181, 28);
			this.MenuItemCopy.Text = "&Copy";
			// 
			// MenuItemPaste
			// 
			this.MenuItemPaste.Enabled = false;
			this.MenuItemPaste.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditPaste;
			this.MenuItemPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemPaste.Name = "MenuItemPaste";
			this.MenuItemPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
			this.MenuItemPaste.Size = new System.Drawing.Size (181, 28);
			this.MenuItemPaste.Text = "&Paste";
			// 
			// MenuItemDelete
			// 
			this.MenuItemDelete.Enabled = false;
			this.MenuItemDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.MenuItemDelete.Name = "MenuItemDelete";
			this.MenuItemDelete.ShortcutKeyDisplayString = "";
			this.MenuItemDelete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
			this.MenuItemDelete.Size = new System.Drawing.Size (181, 28);
			this.MenuItemDelete.Text = "&Delete";
			// 
			// ContextMenuEdit
			// 
			this.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemCut,
            this.MenuItemCopy,
            this.MenuItemPaste,
            this.MenuItemDelete});
			this.Size = new System.Drawing.Size (234, 88);
			this.ResumeLayout (false);

		}
	}

	///////////////////////////////////////////////////////////////////////////////

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

	///////////////////////////////////////////////////////////////////////////////

	public class CanEditEventArgs : EditEventArgs
	{
		public CanEditEventArgs ()
		{
			PasteTypeTitle = AppResources.Resources.EditPasteThis;
		}
		public CanEditEventArgs (ContextMenuStrip pContextMenu)
		{
			ContextMenu = pContextMenu;
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
		public Boolean IsContextMenu
		{
			get
			{
				return ContextMenu != null;
			}
		}
		public ContextMenuStrip ContextMenu
		{
			get;
			protected set;
		}

		//=============================================================================

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

		//=============================================================================

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

		//=============================================================================

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

	public static partial class EditEventsExtensions
	{
		public static void SetTipText (this ToolStripButton pButton, String pTipText)
		{
			pButton.Text = pTipText;
		}

		public static void SetTitle (this ToolStripButton pButton, String pTitle)
		{
			pButton.Text = pTitle.NoMenuPrefix ();
		}

		public static void SetTitle (this ToolStripMenuItem pMenuItem, String pTitle)
		{
			pMenuItem.Text = pTitle;
		}

		public static void SetTipText (this ToolStripItem pItem, String pTipText)
		{
			pItem.Text = pTipText;
		}

		public static void SetTitle (this ToolStripItem pItem, String pTitle)
		{
			pItem.Text = pTitle.NoMenuPrefix ();
		}
	}
}
