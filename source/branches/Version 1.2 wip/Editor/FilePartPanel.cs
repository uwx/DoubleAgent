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
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class FilePartPanel : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FilePartPanel ()
		{
			InitializeComponent ();
			//DoubleBuffered = (System.Environment.OSVersion.Version.Major >= 6);
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.LoadConfig += new EventHandler (OnLoadConfig);
				Program.MainForm.SaveConfig += new EventHandler (OnSaveConfig);

				if (TrackUpdatesWhenHidden)
				{
					Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				}
			}
		}

		protected virtual void OnLoadConfig (object sender, EventArgs e)
		{
		}

		protected virtual void OnSaveConfig (object sender, EventArgs e)
		{
		}

		///////////////////////////////////////////////////////////////////////////////

		protected virtual void OnVisibleChanged (object sender, EventArgs e)
		{
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				if (!TrackUpdatesWhenHidden)
				{
					Program.MainForm.UpdateApplied -= new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				}
				Program.MainForm.CanEdit -= new Global.CanEditEventHandler (OnCanEdit);
				Program.MainForm.EditCopy -= new Global.EditEventHandler (OnEditCopy);
				Program.MainForm.EditCut -= new Global.EditEventHandler (OnEditCut);
				Program.MainForm.EditDelete -= new Global.EditEventHandler (OnEditDelete);
				Program.MainForm.EditPaste -= new Global.EditEventHandler (OnEditPaste);
				Program.MainForm.EditMenu -= new Global.ContextMenuEventHandler (OnEditMenu);
				if (Visible)
				{
					if (!TrackUpdatesWhenHidden)
					{
						Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					}
					Program.MainForm.CanEdit += new Global.CanEditEventHandler (OnCanEdit);
					Program.MainForm.EditCopy += new Global.EditEventHandler (OnEditCopy);
					Program.MainForm.EditCut += new Global.EditEventHandler (OnEditCut);
					Program.MainForm.EditDelete += new Global.EditEventHandler (OnEditDelete);
					Program.MainForm.EditPaste += new Global.EditEventHandler (OnEditPaste);
					Program.MainForm.EditMenu += new Global.ContextMenuEventHandler (OnEditMenu);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public virtual ResolvePart FilePart
		{
			get
			{
				return mFilePart;
			}
			set
			{
				mFilePart = value;
				CharacterFile = (mFilePart != null) ? mFilePart.CharacterFile : null;
			}
		}
		private ResolvePart mFilePart = null;

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get;
			protected set;
		}

		protected virtual Boolean IsEmpty
		{
			get
			{
				return (FilePart == null) || (CharacterFile == null);
			}
		}

		protected virtual Boolean TrackUpdatesWhenHidden
		{
			get
			{
				return false;
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
			if (!e.IsUsed && !IsEmpty)
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
			if (!IsEmpty)
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
			if (!e.IsUsed && !IsEmpty)
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
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly)
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
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly)
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
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly)
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
		///////////////////////////////////////////////////////////////////////////////
		#region Undoable Updates

		private void OnUpdateApplied (Object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				try
				{
					UpdateApplied (sender);
				}
				catch
				{
				}
			}
		}

		protected virtual void UpdateApplied (Object pUpdate)
		{
		}

		#endregion
	}
}
