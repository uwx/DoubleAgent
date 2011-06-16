using System;
using System.Windows;

namespace DoubleAgent
{
	public class RepeatButtonEx : System.Windows.Controls.Primitives.RepeatButton, IDisposable
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private AsyncTimer mRepeatEndTimer = null;
		private int mRepeatEndDelay = -1;

		public RepeatButtonEx ()
		{
		}
		~RepeatButtonEx ()
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
			StopRepeatEndTimer ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public int RepeatEndDelay
		{
			get
			{
				return (mRepeatEndDelay < 0) ? Delay : mRepeatEndDelay;
			}
			set
			{
				mRepeatEndDelay = Math.Max (value, -1);
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsRepeating
		{
			get
			{
				return (mRepeatEndTimer != null);
			}
			set
			{
				if (!value)
				{
					StopRepeatEndTimer ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public static readonly RoutedEvent RepeatEndEvent = EventManager.RegisterRoutedEvent ("RepeatEnd", RoutingStrategy.Bubble, typeof (RoutedEventHandler), typeof (RepeatButtonEx));

		public event RoutedEventHandler RepeatEnd
		{
			add
			{
				AddHandler (RepeatEndEvent, value);
			}
			remove
			{
				RemoveHandler (RepeatEndEvent, value);
			}
		}

		protected void OnRepeatEnd ()
		{
			try
			{
				RaiseEvent (new RoutedEventArgs (RepeatEndEvent, this));
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		private void StartRepeatEndTimer ()
		{
			if (mRepeatEndTimer == null)
			{
				mRepeatEndTimer = new AsyncTimer ();
				mRepeatEndTimer.TimerPulse += new AsyncTimer.TimerPulseHandler (RepeatEndTimer_TimerPulse);
			}
			if (mRepeatEndTimer != null)
			{
				mRepeatEndTimer.Stop ();
				mRepeatEndTimer.Start (RepeatEndDelay);
			}
		}

		private void StopRepeatEndTimer ()
		{
			if (mRepeatEndTimer != null)
			{
				mRepeatEndTimer.Dispose ();
				mRepeatEndTimer = null;
			}
		}

		private void RepeatEndTimer_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			if (!IsPressed)
			{
				OnRepeatEnd ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void OnClick ()
		{
			StartRepeatEndTimer ();
			base.OnClick ();
		}

		protected override void OnGotFocus (RoutedEventArgs e)
		{
			StopRepeatEndTimer ();
			base.OnGotFocus (e);
		}

		protected override void OnLostFocus (RoutedEventArgs e)
		{
			StopRepeatEndTimer ();
			base.OnGotFocus (e);
		}

		#endregion
	}
}
