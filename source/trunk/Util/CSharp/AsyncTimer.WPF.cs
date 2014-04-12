/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
using System.ComponentModel;
using System.Threading;

namespace DoubleAgent
{
	public class AsyncTimer : IDisposable
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private Timer mTimer = null;
		private AsyncOperation mAsyncOperation = null;

		public AsyncTimer ()
		{
		}

		~AsyncTimer ()
		{
			Dispose (false);
		}

		public void Dispose ()
		{
			Monitor.Enter (this);
			try
			{
				Dispose (true);
			}
			catch
			{
			}
			Monitor.Exit (this);
			GC.SuppressFinalize (this);
		}

		protected virtual void Dispose (bool disposing)
		{
			if (mTimer != null)
			{
				try
				{
					mTimer.Dispose ();
				}
				catch
				{
				}
				mTimer = null;
			}
			if (mAsyncOperation != null)
			{
				try
				{
					mAsyncOperation.OperationCompleted ();
				}
				catch
				{
				}
				mAsyncOperation = null;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public Boolean IsRunning
		{
			get
			{
				Boolean lRet = false;

				Monitor.Enter (this);
				lRet = (mTimer != null);
				Monitor.Exit (this);

				return lRet;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public class TimerEventArgs : EventArgs
		{
			public Object TimerId
			{
				get;
				set;
			}
		}

		public delegate void TimerPulseHandler (object sender, TimerEventArgs e);
		public event TimerPulseHandler TimerPulse;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean Start (int pInitialDelay)
		{
			return Start (pInitialDelay, Timeout.Infinite, null);
		}

		public Boolean Start (int pInitialDelay, Object pTimerId)
		{
			return Start (pInitialDelay, Timeout.Infinite, pTimerId);
		}

		public Boolean Start (int pInitialDelay, int pRepeatInterval)
		{
			return Start (pInitialDelay, pRepeatInterval, null);
		}

		public Boolean Start (int pInitialDelay, int pRepeatInterval, Object pTimerId)
		{
			Boolean lRet = false;
			Monitor.Enter (this);

			try
			{
				if (mTimer == null)
				{
					Stop ();

					try
					{
						if (pRepeatInterval == Timeout.Infinite)
						{
							mTimer = new Timer (TimerCallback, AsyncOperationManager.CreateOperation (pTimerId), pInitialDelay, pRepeatInterval);
						}
						else
						{
							mTimer = new Timer (TimerCallback, mAsyncOperation = AsyncOperationManager.CreateOperation (pTimerId), pInitialDelay, pRepeatInterval);
						}
						lRet = true;
					}
					catch
					{
					}
				}
			}
			catch
			{
			}

			Monitor.Exit (this);
			return lRet;
		}

		public Boolean Stop ()
		{
			Boolean lRet = false;
			Monitor.Enter (this);

			try
			{
				lRet = (mTimer != null);
				Dispose (false);
			}
			catch
			{
			}

			Monitor.Exit (this);
			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		private void TimerCallback (Object state)
		{
			Monitor.Enter (this);

			try
			{
				if (state == mAsyncOperation)
				{
					(state as AsyncOperation).Post (TimerTick, (state as AsyncOperation).UserSuppliedState);
				}
				else
				{
					(state as AsyncOperation).PostOperationCompleted (TimerComplete, (state as AsyncOperation).UserSuppliedState);
				}
			}
			catch
			{
			}

			Monitor.Exit (this);
		}

		private void TimerComplete (Object state)
		{
			Monitor.Enter (this);

			try
			{
				if (TimerPulse != null)
				{
					TimerEventArgs lEventArgs = new TimerEventArgs ();
					lEventArgs.TimerId = state;
					TimerPulse (this, lEventArgs);
				}
			}
			catch
			{
			}

			Monitor.Exit (this);
		}

		private void TimerTick (Object state)
		{
			Monitor.Enter (this);

			try
			{
				if (TimerPulse != null)
				{
					TimerEventArgs lEventArgs = new TimerEventArgs ();
					lEventArgs.TimerId = state;
					TimerPulse (this, lEventArgs);
				}
			}
			catch
			{
			}

			Monitor.Exit (this);
		}

		#endregion
	}
}
