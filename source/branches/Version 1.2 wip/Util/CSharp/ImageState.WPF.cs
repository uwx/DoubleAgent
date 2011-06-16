using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;

namespace DoubleAgent
{
	public class ImageState : System.Windows.Controls.Image
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		public ImageState ()
		{
			try
			{
				Binding lBinding = new Binding ();
				lBinding.RelativeSource = new RelativeSource (RelativeSourceMode.FindAncestor, typeof (ButtonBase), 1);
				lBinding.Path = new PropertyPath (ButtonBase.IsMouseOverProperty);
				lBinding.Mode = BindingMode.OneWay;
				lBinding.NotifyOnSourceUpdated = true;
				lBinding.TargetNullValue = false;
				SetBinding (ImageState.IsHotProperty, lBinding);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}

			try
			{
				Binding lBinding = new Binding ();
				lBinding.RelativeSource = new RelativeSource (RelativeSourceMode.FindAncestor, typeof (ButtonBase), 1);
				lBinding.Path = new PropertyPath (ButtonBase.IsPressedProperty);
				lBinding.Mode = BindingMode.OneWay;
				lBinding.NotifyOnSourceUpdated = true;
				lBinding.TargetNullValue = false;
				SetBinding (ImageState.IsPressedProperty, lBinding);
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		protected override void OnPropertyChanged (DependencyPropertyChangedEventArgs e)
		{
			base.OnPropertyChanged (e);

			if ((e.Property == IsPressedProperty) || (e.Property == IsHotProperty) || (e.Property == IsCheckedProperty) || (e.Property == IsEnabledProperty) || (e.Property == NormalSourceProperty) || (e.Property == CheckedSourceProperty) || (e.Property == DisabledSourceProperty))
			{
				try
				{
					ImageSource lImage = null;
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Property [{0}] from [{1}] to [{2}]", e.Property, e.OldValue, e.NewValue);
					System.Diagnostics.Debug.Print ("  IsPressed [{0}] IsHot [{1}] IsChecked [{2}] IsEnabled [{3}]", IsPressed, IsHot, IsChecked, IsEnabled);
#endif
					if (IsEnabled)
					{
						if (IsPressed)
						{
							lImage = PressedSource;
						}
						else if (IsHot)
						{
							lImage = HotSource;
						}
						else if (IsChecked.HasValue && IsChecked.Value)
						{
							lImage = CheckedSource;
						}
					}
					else
					{
						lImage = DisabledSource;
					}
					if (lImage == null)
					{
						lImage = NormalSource;
					}
					if (lImage == null)
					{
						lImage = Source;
					}
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("  Image [{0}] Source [{1}]", lImage, Source);
#endif
					if (lImage != Source)
					{
						SetCurrentValue (SourceProperty, lImage);
					}
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("  Image [{0}] Source [{1}]", lImage, Source);
#endif
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Dependency Properties

		public static readonly DependencyProperty IsPressedProperty = DependencyProperty.Register ("IsPressed", typeof (Boolean), typeof (ImageState), new FrameworkPropertyMetadata (false, new PropertyChangedCallback (OnIsPressedPropertyChanged), new CoerceValueCallback (CoerceIsPressed)));
		public static readonly DependencyProperty IsHotProperty = DependencyProperty.Register ("IsHot", typeof (Boolean), typeof (ImageState), new FrameworkPropertyMetadata (false, new PropertyChangedCallback (OnIsHotPropertyChanged), new CoerceValueCallback (CoerceIsHot)));
		public static readonly DependencyProperty IsCheckedProperty = DependencyProperty.Register ("IsChecked", typeof (Boolean), typeof (ImageState), new FrameworkPropertyMetadata (false, new PropertyChangedCallback (OnIsCheckedPropertyChanged), new CoerceValueCallback (CoerceIsChecked)));

		public static readonly DependencyProperty NormalSourceProperty = DependencyProperty.Register ("NormalSource", typeof (ImageSource), typeof (ImageState), new FrameworkPropertyMetadata (null, new PropertyChangedCallback (OnNormalSourcePropertyChanged), new CoerceValueCallback (CoerceNormalSource)));
		public static readonly DependencyProperty HotSourceProperty = DependencyProperty.Register ("HotSource", typeof (ImageSource), typeof (ImageState), new FrameworkPropertyMetadata (null, new PropertyChangedCallback (OnHotSourcePropertyChanged), new CoerceValueCallback (CoerceHotSource)));
		public static readonly DependencyProperty PressedSourceProperty = DependencyProperty.Register ("PressedSource", typeof (ImageSource), typeof (ImageState), new FrameworkPropertyMetadata (null, new PropertyChangedCallback (OnPressedSourcePropertyChanged), new CoerceValueCallback (CoercePressedSource)));
		public static readonly DependencyProperty CheckedSourceProperty = DependencyProperty.Register ("CheckedSource", typeof (ImageSource), typeof (ImageState), new FrameworkPropertyMetadata (null, new PropertyChangedCallback (OnCheckedSourcePropertyChanged), new CoerceValueCallback (CoerceCheckedSource)));
		public static readonly DependencyProperty DisabledSourceProperty = DependencyProperty.RegisterAttached ("DisabledSource", typeof (ImageSource), typeof (ImageState), new FrameworkPropertyMetadata (null, new PropertyChangedCallback (OnDisabledSourcePropertyChanged), new CoerceValueCallback (CoerceDisabledSource)));

		///////////////////////////////////////////////////////////////////////////////
		#region IsPressed

		public Boolean IsPressed
		{
			get
			{
				return (Boolean)GetValue (IsPressedProperty);
			}
			set
			{
				SetValue (IsPressedProperty, value);
			}
		}

		private static object CoerceIsPressed (DependencyObject pObject, object pValue)
		{
			try
			{
				return (Boolean)pValue;
			}
			catch
			{
			}
			return false;
		}

		private static void OnIsPressedPropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region IsHot

		public Boolean IsHot
		{
			get
			{
				return (Boolean)GetValue (IsHotProperty);
			}
			set
			{
				SetValue (IsHotProperty, value);
			}
		}

		private static object CoerceIsHot (DependencyObject pObject, object pValue)
		{
			try
			{
				return (Boolean)pValue;
			}
			catch
			{
			}
			return false;
		}

		private static void OnIsHotPropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region IsChecked

		public Boolean? IsChecked
		{
			get
			{
				Object lValue = GetValue (IsCheckedProperty);
				return (lValue is Boolean) ? (Boolean?)lValue : (Boolean?)null;
			}
			set
			{
				if (value.HasValue)
				{
					SetValue (IsCheckedProperty, value.Value);
				}
				else
				{
					SetValue (IsCheckedProperty, null);
				}
			}
		}

		private static object CoerceIsChecked (DependencyObject pObject, object pValue)
		{
			try
			{
				if (pValue != null)
				{
					return (Boolean)pValue;
				}
			}
			catch
			{
			}
			return false;
		}

		private static void OnIsCheckedPropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region NormalSource

		public ImageSource NormalSource
		{
			get
			{
				return GetValue (NormalSourceProperty) as ImageSource;
			}
			set
			{
				SetValue (NormalSourceProperty, value);
			}
		}

		private static object CoerceNormalSource (DependencyObject pObject, object pValue)
		{
			if (pValue is ImageSource)
			{
				return pValue;
			}
			else
			{
				return null;
			}
		}

		private static void OnNormalSourcePropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region PressedSource

		public ImageSource PressedSource
		{
			get
			{
				return GetValue (PressedSourceProperty) as ImageSource;
			}
			set
			{
				SetValue (PressedSourceProperty, value);
			}
		}

		private static object CoercePressedSource (DependencyObject pObject, object pValue)
		{
			if (pValue is ImageSource)
			{
				return pValue;
			}
			else
			{
				return null;
			}
		}

		private static void OnPressedSourcePropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region HotSource

		public ImageSource HotSource
		{
			get
			{
				return GetValue (HotSourceProperty) as ImageSource;
			}
			set
			{
				SetValue (HotSourceProperty, value);
			}
		}

		private static object CoerceHotSource (DependencyObject pObject, object pValue)
		{
			if (pValue is ImageSource)
			{
				return pValue;
			}
			else
			{
				return null;
			}
		}

		private static void OnHotSourcePropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region CheckedSource

		public ImageSource CheckedSource
		{
			get
			{
				return GetValue (CheckedSourceProperty) as ImageSource;
			}
			set
			{
				SetValue (CheckedSourceProperty, value);
			}
		}

		private static object CoerceCheckedSource (DependencyObject pObject, object pValue)
		{
			if (pValue is ImageSource)
			{
				return pValue;
			}
			else
			{
				return null;
			}
		}

		private static void OnCheckedSourcePropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region DisabledSource

		[AttachedPropertyBrowsableForType (typeof (ImageState))]
		public static void SetDisabledSource (UIElement element, ImageSource value)
		{
			element.SetValue (DisabledSourceProperty, value);
		}
		[AttachedPropertyBrowsableForType (typeof (ImageState))]
		public static ImageSource GetDisabledSource (UIElement element)
		{
			return element.GetValue (DisabledSourceProperty) as ImageSource;
		}

		public ImageSource DisabledSource
		{
			get
			{
				return GetValue (DisabledSourceProperty) as ImageSource;
			}
			set
			{
				SetValue (DisabledSourceProperty, value);
			}
		}

		private static object CoerceDisabledSource (DependencyObject pObject, object pValue)
		{
			if (pValue is ImageSource)
			{
				return pValue;
			}
			else
			{
				return null;
			}
		}

		private static void OnDisabledSourcePropertyChanged (DependencyObject pObject, DependencyPropertyChangedEventArgs pEventArgs)
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#endregion
	}
}
