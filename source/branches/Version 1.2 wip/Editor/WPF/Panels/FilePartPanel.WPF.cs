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
			Fader = new PanelFader (this);
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
				return (Visibility == Visibility.Visible);
			}
			set
			{
				if (value)
				{
					Opacity = 1;
					Visibility = Visibility.Visible;
				}
				else
				{
					Visibility = Visibility.Collapsed;
				}
			}
		}

		protected virtual Boolean IsPanelFilling
		{
			get;
			set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public PanelFader Fader
		{
			get;
			protected set;
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

		///////////////////////////////////////////////////////////////////////////////

		protected Boolean IsControlFocused (Control pControl)
		{
			return (pControl == null) ? this.IsKeyboardFocusWithin : pControl.IsKeyboardFocusWithin;
		}

		protected Control GetFocusedControl ()
		{
			if (this.IsKeyboardFocusWithin)
			{
				return Keyboard.FocusedElement as Control;
			}
			return null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Edit Menu

		protected Boolean IsControlEditTarget (Control pControl, Global.CanEditEventArgs e)
		{
			return IsControlFocused (pControl) || pControl.IsAncestorOf (e.RoutedEventArgs.Source as DependencyObject);
		}

		protected Boolean IsControlEditTarget (Control pControl, Global.EditEventArgs e)
		{
			return IsControlFocused (pControl) || pControl.IsAncestorOf (e.RoutedEventArgs.Source as DependencyObject);
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void OnCanEditCut (object sender, CanExecuteRoutedEventArgs e)
		{
			if (HandleCanEditCut (new Global.CanEditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		protected void OnCanEditCopy (object sender, CanExecuteRoutedEventArgs e)
		{
			if (HandleCanEditCopy (new Global.CanEditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		protected void OnCanEditDelete (object sender, CanExecuteRoutedEventArgs e)
		{
			if (HandleCanEditDelete (new Global.CanEditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		protected void OnCanEditPaste (object sender, CanExecuteRoutedEventArgs e)
		{
			if (HandleCanEditPaste (new Global.CanEditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void OnEditCut (object sender, ExecutedRoutedEventArgs e)
		{
			if (HandleEditCut (new Global.EditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		protected void OnEditCopy (object sender, ExecutedRoutedEventArgs e)
		{
			if (HandleEditCopy (new Global.EditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		protected void OnEditDelete (object sender, ExecutedRoutedEventArgs e)
		{
			if (HandleEditDelete (new Global.EditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		protected void OnEditPaste (object sender, ExecutedRoutedEventArgs e)
		{
			if (HandleEditPaste (new Global.EditEventArgs (e)))
			{
				e.Handled = true;
			}
		}

		#endregion
	}
}
