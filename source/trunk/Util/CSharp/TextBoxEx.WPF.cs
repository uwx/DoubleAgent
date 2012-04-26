/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace DoubleAgent
{
	/// <summary>
	/// Compatibility with System.Windows.Forms.Control.TextBox
	/// </summary>
	public class TextBoxEx : System.Windows.Controls.TextBox
	{
		///////////////////////////////////////////////////////////////////////////////
		#region IsModified Property

		public static readonly DependencyProperty IsModifiedProperty = DependencyProperty.Register ("IsModified", typeof (Boolean), typeof (TextBoxEx), new FrameworkPropertyMetadata (false, new PropertyChangedCallback (OnIsModifiedPropertyChanged), new CoerceValueCallback (CoerceIsModified)));
		public static readonly RoutedEvent IsModifiedChangedEvent = EventManager.RegisterRoutedEvent ("IsModified", RoutingStrategy.Bubble, typeof (RoutedEventHandler), typeof (TextBoxEx));

		public Boolean IsModified
		{
			get
			{
				return (Boolean)GetValue (IsModifiedProperty);
			}
			set
			{
				SetValue (IsModifiedProperty, value);
				if (!IsModified)
				{
					UndoLimit = 0;
					UndoLimit = -1;
					HasChanged = false;
				}
			}
		}

		public event RoutedEventHandler IsModifiedChanged
		{
			add
			{
				AddHandler (IsModifiedChangedEvent, value);
			}
			remove
			{
				RemoveHandler (IsModifiedChangedEvent, value);
			}
		}

		protected void OnIsModifiedChanged ()
		{
			RaiseEvent (new RoutedEventArgs (TextBoxEx.IsModifiedChangedEvent, this));
		}

		private static object CoerceIsModified (DependencyObject pObject, object pValue)
		{
			Boolean lIsModified = false;
			try
			{
				lIsModified = (Boolean)pValue;
			}
			catch
			{
			}
			return lIsModified;
		}

		private static void OnIsModifiedPropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
			(pObject as TextBox).RaiseEvent (new RoutedPropertyChangedEventArgs<Boolean> ((Boolean)pEventArgs.OldValue, (Boolean)pEventArgs.NewValue, IsModifiedChangedEvent));
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		protected Boolean HasChanged
		{
			get; set;
		}

		protected override void OnTextChanged (TextChangedEventArgs e)
		{
			base.OnTextChanged (e);
			HasChanged = true;
		}

		protected override void OnLostFocus (RoutedEventArgs e)
		{
			base.OnLostFocus (e);
			if (HasChanged && !IsModified)
			{
				IsModified = true;
			}
			HasChanged = false;
		}

		protected override void OnKeyDown (KeyEventArgs e)
		{
			base.OnKeyDown (e);
			if ((e.Key == Key.Return) && !AcceptsReturn)
			{
				if (HasChanged && !IsModified)
				{
					IsModified = true;
				}
				HasChanged = false;
			}
		}

		#endregion
	}
}
