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
		public TextBoxEx ()
		{
			TextChanged += new TextChangedEventHandler (TextBoxEx_TextChanged);
			LostFocus += new RoutedEventHandler (TextBoxEx_LostFocus);
			KeyDown += new System.Windows.Input.KeyEventHandler (TextBoxEx_KeyDown);
		}

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
			RaiseEvent (new RoutedEventArgs (TextBoxEx.IsModifiedChangedEvent));
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

		private Boolean HasChanged
		{
			get;
			set;
		}

		private void TextBoxEx_TextChanged (object sender, TextChangedEventArgs e)
		{
			HasChanged = true;
		}

		private void TextBoxEx_LostFocus (object sender, RoutedEventArgs e)
		{
			if (HasChanged && !IsModified)
			{
				IsModified = true;
			}
			HasChanged = false;
		}

		void TextBoxEx_KeyDown (object sender, System.Windows.Input.KeyEventArgs e)
		{
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
