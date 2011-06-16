using System;
using System.Collections.Generic;
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
				if (!value)
				{
					UndoLimit = 0;
					UndoLimit = -1;
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
