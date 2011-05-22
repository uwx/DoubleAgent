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
	public partial class FilePartPanel : UserControl
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FilePartPanel ()
		{
			InitializeCommon ();
			if (Program.MainWindow != null)
			{
				IsVisibleChanged += new DependencyPropertyChangedEventHandler (OnIsVisibleChanged);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected virtual void OnIsVisibleChanged (object sender, DependencyPropertyChangedEventArgs e)
		{
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
				return (this.Visibility == Visibility.Visible);
			}
			set
			{
				this.Visibility = value ? Visibility.Visible : Visibility.Collapsed;
			}
		}

		protected virtual Boolean IsPanelShowing
		{
			get; set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public class PanelContext
		{
			public PanelContext (FilePartPanel pPanel)
			{
				FilePart = pPanel.FilePart;
				if (pPanel.IsKeyboardFocusWithin)
				{
					FocusedElement = Keyboard.FocusedElement;
				}
			}
			public void RestoreContext (FilePartPanel pPanel)
			{
				pPanel.FilePart = FilePart;
				if (FocusedElement != null)
				{
					FocusedElement.Focus ();
				}
			}

			public ResolvePart FilePart
			{
				get;
				protected set;
			}
			public IInputElement FocusedElement
			{
				get;
				protected set;
			}
		}

		#endregion
	}
}
