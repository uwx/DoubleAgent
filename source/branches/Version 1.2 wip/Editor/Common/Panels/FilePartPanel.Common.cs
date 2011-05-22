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
using System.Windows.Controls;
using System.Windows.Input;
using AgentCharacterEditor.Navigation;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		protected virtual void InitializeCommon ()
		{
			IsPanelShowing = false;

			if (Program.MainWindow != null)
			{
				Program.MainWindow.LoadConfig += new EventHandler (OnLoadConfig);
				Program.MainWindow.SaveConfig += new EventHandler (OnSaveConfig);

				if (TrackUpdatesWhenHidden)
				{
					Program.MainWindow.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				}
			 }
		}

		protected virtual void HandleVisibleChanged ()
		{
			IsPanelShowing = false;

			if (Program.MainWindow != null)
			{
				if (!TrackUpdatesWhenHidden)
				{
					Program.MainWindow.UpdateApplied -= new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				}
				if (IsPanelVisible)
				{
					if (!TrackUpdatesWhenHidden)
					{
						Program.MainWindow.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected virtual void OnLoadConfig (object sender, EventArgs e)
		{
		}

		protected virtual void OnSaveConfig (object sender, EventArgs e)
		{
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
		public virtual Object NavigationContext
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
				else if (value is ResolvePart)
				{
					FilePart = value as ResolvePart;
				}
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public CharacterFile CharacterFile
		{
			get;
			protected set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public virtual Boolean IsPanelEmpty
		{
			get
			{
				return (FilePart == null) || (CharacterFile == null);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected virtual Boolean TrackUpdatesWhenHidden
		{
			get
			{
				return false;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		protected Boolean PushIsPanelShowing (Boolean pIsPanelShowing)
		{
			Boolean lRet = IsPanelShowing;
			IsPanelShowing = pIsPanelShowing;
			return lRet;
		}

		protected void PopIsPanelShowing (Boolean pWasPanelShowing)
		{
			IsPanelShowing = pWasPanelShowing;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Undoable Updates

		private void OnUpdateApplied (Object sender, EventArgs e)
		{
			if (!IsPanelEmpty)
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
