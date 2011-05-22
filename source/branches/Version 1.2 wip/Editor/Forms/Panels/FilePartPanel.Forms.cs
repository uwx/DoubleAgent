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
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Navigation;

namespace AgentCharacterEditor.Panels
{
	public partial class FilePartPanel : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FilePartPanel ()
		{
			InitializeComponent ();
			InitializeCommon ();
		}

		protected virtual void OnVisibleChanged (object sender, EventArgs e)
		{
			if (Program.MainWindow != null)
			{
				Program.MainWindow.CanEdit -= new Global.CanEditEventHandler (OnCanEdit);
				Program.MainWindow.EditCopy -= new Global.EditEventHandler (OnEditCopy);
				Program.MainWindow.EditCut -= new Global.EditEventHandler (OnEditCut);
				Program.MainWindow.EditDelete -= new Global.EditEventHandler (OnEditDelete);
				Program.MainWindow.EditPaste -= new Global.EditEventHandler (OnEditPaste);
				Program.MainWindow.EditMenu -= new Global.ContextMenuEventHandler (OnEditMenu);
				if (Visible)
				{
					Program.MainWindow.CanEdit += new Global.CanEditEventHandler (OnCanEdit);
					Program.MainWindow.EditCopy += new Global.EditEventHandler (OnEditCopy);
					Program.MainWindow.EditCut += new Global.EditEventHandler (OnEditCut);
					Program.MainWindow.EditDelete += new Global.EditEventHandler (OnEditDelete);
					Program.MainWindow.EditPaste += new Global.EditEventHandler (OnEditPaste);
					Program.MainWindow.EditMenu += new Global.ContextMenuEventHandler (OnEditMenu);
				}
			}

			HandleVisibleChanged ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsPanelVisible
		{
			get
			{
				return this.Visible;
			}
			set
			{
				this.Visible = value;
			}
		}

		protected virtual Boolean IsPanelShowing
		{
			get
			{
				return !CausesValidation;
			}
			set
			{
				CausesValidation = !value && Visible;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public class PanelContext
		{
			public PanelContext (FilePartPanel pPanel)
			{
				FilePart = pPanel.FilePart;
				if (pPanel.ContainsFocus)
				{
					ActiveControl = Program.MainWindow.GetActiveControl ();
				}
			}
			public void RestoreContext (FilePartPanel pPanel)
			{
				pPanel.FilePart = FilePart;
				if (ActiveControl != null)
				{
					ActiveControl.Focus ();
				}
			}

			public ResolvePart FilePart
			{
				get;
				protected set;
			}
			public Control ActiveControl
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Edit Menu

		protected virtual void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
		}

		protected virtual Boolean EditCopy (Global.EditEventArgs pEventArgs)
		{
			return false;
		}
		protected virtual Boolean EditCut (Global.EditEventArgs pEventArgs)
		{
			return false;
		}
		protected virtual Boolean EditDelete (Global.EditEventArgs pEventArgs)
		{
			return false;
		}
		protected virtual Boolean EditPaste (Global.EditEventArgs pEventArgs)
		{
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void OnCanEdit (object sender, Global.CanEditEventArgs e)
		{
			if (!e.IsUsed && !IsPanelEmpty)
			{
				try
				{
					ShowEditState (e);
				}
				catch
				{
				}
			}
		}

		protected virtual void OnEditMenu (object sender, Global.ContextMenuEventArgs e)
		{
			if (!IsPanelEmpty)
			{
				try
				{
					ShowEditState (e);
				}
				catch
				{
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void OnEditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsPanelEmpty)
			{
				try
				{
					if (EditCopy (e))
					{
						e.IsUsed = true;
					}
				}
				catch
				{
				}
			}
		}

		private void OnEditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				try
				{
					if (EditCut (e))
					{
						e.IsUsed = true;
					}
				}
				catch
				{
				}
			}
		}

		private void OnEditDelete (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				try
				{
					if (EditDelete (e))
					{
						e.IsUsed = true;
					}
				}
				catch
				{
				}
			}
		}

		private void OnEditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				try
				{
					if (EditPaste (e))
					{
						e.IsUsed = true;
					}
				}
				catch
				{
				}
			}
		}

		#endregion
	}
}
