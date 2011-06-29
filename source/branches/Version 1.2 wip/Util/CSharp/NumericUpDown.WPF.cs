using System;
using System.Windows;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;

namespace DoubleAgent
{
	public class NumericUpDown : TextBoxEx, IDisposable
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private AsyncTimer mWheelTimer = null;
		private AsyncTimer mRepeatTimer = null;

		public NumericUpDown ()
		{
			this.DefaultBackground = SystemColors.WindowBrush;
			this.HighlightBackground = Brushes.Pink;
			this.MouseWheelSingle = true;
			this.Minimum = decimal.MinValue;
			this.Maximum = decimal.MaxValue;
			this.MaxLines = 1;

			this.CommandBindings.Add (new CommandBinding (ScrollBar.LineUpCommand, OnIncrement, OnCanIncrement));
			this.CommandBindings.Add (new CommandBinding (ScrollBar.LineDownCommand, OnDecrement, OnCanDecrement));
		}

		~NumericUpDown ()
		{
			Dispose (false);
		}

		public void Dispose ()
		{
			Dispose (true);
			GC.SuppressFinalize (this);
		}
		protected virtual void Dispose (bool disposing)
		{
			StopWheelTimer ();
			StopRepeatTimer ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public System.Windows.Controls.TextBox TextBox
		{
			get
			{
				return this;
			}
		}

		/// <summary>
		/// Indicates if mouse wheel increments and decrements should be limited to +/- 1.
		/// </summary>
		[System.ComponentModel.Category ("Behavior")]
		[System.ComponentModel.DefaultValue (true)]
		public Boolean MouseWheelSingle
		{
			get;
			set;
		}

		//=============================================================================

		/// <summary>
		/// The default background color.
		/// </summary>
		/// <seealso cref="HighlightBackground"/>
		public System.Windows.Media.Brush DefaultBackground
		{
			get;
			set;
		}

		/// <summary>
		/// The highlighted background color.
		/// </summary>
		/// <remarks>The highlighted background color is used when the control's value is not in range.</remarks>
		/// <seealso cref="IsHighlighted"/>
		/// <seealso cref="DefaultBackground"/>
		public System.Windows.Media.Brush HighlightBackground
		{
			get;
			set;
		}

		/// <summary>
		/// Indicates if the control is highlighted.
		/// </summary>
		/// <remarks>If the control is highlighted, it will use the <see cref="HighlightBackground"/>. Otherwise, it will use the <see cref="DefaultBackground"/></remarks>
		/// <seealso cref="HighlightBackground"/>
		/// <seealso cref="NumericUpDown.Minimum"/>
		/// <seealso cref="NumericUpDown.Maximum"/>
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsHighlighted
		{
			get
			{
				return (this.Background == this.HighlightBackground);
			}
			set
			{
				if (value)
				{
					if (this.Background != this.HighlightBackground)
					{
						this.Background = this.HighlightBackground;
					}
				}
				else
				{
					if (this.Background != this.DefaultBackground)
					{
						this.Background = this.DefaultBackground;
					}
				}
			}
		}

		//=============================================================================

		public Decimal Value
		{
			get
			{
				Decimal lValue = Decimal.Zero;
				Decimal.TryParse (base.Text, out lValue);
				return lValue;
			}
			set
			{
				if ((value < Minimum) || (value > Maximum))
				{
					//value = Math.Min (Math.Max (value, Minimum), Maximum);
					this.IsHighlighted = true;
				}
				else
				{
					this.IsHighlighted = false;
				}
				if (base.Text != value.ToString ())
				{
					base.Text = value.ToString ();
					CommandManager.InvalidateRequerySuggested ();
				}
			}
		}

		public Decimal Minimum
		{
			get;
			set;
		}

		public Decimal Maximum
		{
			get;
			set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		public new void Clear ()
		{
			base.Clear ();
			IsHighlighted = false;
		}

		protected override void OnMouseWheel (MouseWheelEventArgs e)
		{
			base.OnMouseWheel (e);
			StopRepeatTimer ();
			StopWheelTimer ();

#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("OnMouseWheel [{0}] [{1}] [{2} {3}] [{3}]", e.Delta, this.MouseWheelSingle, System.Windows.Forms.SystemInformation.MouseWheelScrollDelta, System.Windows.Forms.SystemInformation.MouseWheelScrollLines, this.Value);
#endif
			if (this.MouseWheelSingle)
			{
				this.Value = Math.Min (Math.Max (this.Value + (e.Delta / System.Windows.Forms.SystemInformation.MouseWheelScrollDelta), this.Minimum), this.Maximum);
			}
			else
			{
				this.Value = Math.Min (Math.Max (this.Value + (e.Delta / System.Windows.Forms.SystemInformation.MouseWheelScrollLines), this.Minimum), this.Maximum);
			}

			StartWheelTimer ();
		}

		private void StartWheelTimer ()
		{
			if (mWheelTimer == null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StartWheelTimer");
#endif
				mWheelTimer = new AsyncTimer ();
				mWheelTimer.TimerPulse += new AsyncTimer.TimerPulseHandler (WheelTimerComplete);
				mWheelTimer.Start (System.Windows.Forms.SystemInformation.DoubleClickTime);
			}
		}

		private void StopWheelTimer ()
		{
			if (mWheelTimer != null)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("StopWheelTimer");
#endif
				try
				{
					mWheelTimer.Dispose ();
				}
				catch
				{
				}
				mWheelTimer = null;
			}
		}

		private void WheelTimerComplete (object sender, AsyncTimer.TimerEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("WheelTimerComplete");
#endif
			try
			{
				if (HasChanged && !IsModified)
				{
					HasChanged = false;
					IsModified = true;
				}
			}
			catch
			{
			}
		}

		//=============================================================================

		protected override void OnGotFocus (RoutedEventArgs e)
		{
			StopWheelTimer ();
			StopRepeatTimer ();
			base.OnGotFocus (e);
		}

		protected override void OnLostFocus (RoutedEventArgs e)
		{
			StopWheelTimer ();
			StopRepeatTimer ();
			base.OnLostFocus (e);
		}

		//=============================================================================

		protected void OnCanIncrement (object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = (Value < Maximum);
			e.Handled = true;
		}
		protected void OnIncrement (object sender, ExecutedRoutedEventArgs e)
		{
			StopWheelTimer ();

			if (e.OriginalSource is RepeatButton)
			{
				StartRepeatTimer (e.OriginalSource as RepeatButton);
			}
			else
			{
				StopRepeatTimer ();
			}
			if (Value < Maximum)
			{
				Value++;
			}
			e.Handled = true;
		}

		protected void OnCanDecrement (object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = (Value > Minimum);
			e.Handled = true;
		}
		protected void OnDecrement (object sender, ExecutedRoutedEventArgs e)
		{
			StopWheelTimer ();

			if (e.OriginalSource is RepeatButton)
			{
				StartRepeatTimer (e.OriginalSource as RepeatButton);
			}
			else
			{
				StopRepeatTimer ();
			}
			if (Value > Minimum)
			{
				Value--;
			}
			e.Handled = true;
		}

		//=============================================================================

		private void StartRepeatTimer (RepeatButton pRepeatButton)
		{
			if (mRepeatTimer == null)
			{
				mRepeatTimer = new AsyncTimer ();
				mRepeatTimer.TimerPulse += new AsyncTimer.TimerPulseHandler (ShiftRepeatTimer_TimerPulse);
			}
			if (mRepeatTimer != null)
			{
				mRepeatTimer.Stop ();
				mRepeatTimer.Start (pRepeatButton.Delay, pRepeatButton);
			}
		}

		private void StopRepeatTimer ()
		{
			if (mRepeatTimer != null)
			{
				mRepeatTimer.Dispose ();
				mRepeatTimer = null;
			}
		}

		private void ShiftRepeatTimer_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			try
			{
				RepeatButton lRepeatButton = e.TimerId as RepeatButton;

				if (!lRepeatButton.IsPressed && HasChanged && !IsModified)
				{
					HasChanged = false;
					IsModified = true;
				}
			}
			catch
			{
			}
		}

		#endregion
	}
}
