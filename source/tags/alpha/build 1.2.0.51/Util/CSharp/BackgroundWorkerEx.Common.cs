/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
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
	internal class BackgroundWorkerEx : System.ComponentModel.BackgroundWorker
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private UInt32 mWaitCycles = 100;
		private UInt32 mWaitTime = 10;
		private Boolean mIsWorking = false;
		private UInt32 mWorkCycle = 0;
		private Object mWaitLock = new Object ();

		public BackgroundWorkerEx ()
		{
			WorkerReportsProgress = true;
			WorkerSupportsCancellation = true;
		}
		public BackgroundWorkerEx (UInt32 pWaitCycles, UInt32 pWaitTime)
		{
			mWaitCycles = Math.Max (pWaitCycles, 1);
			mWaitTime = Math.Max (pWaitTime, 10);
			WorkerReportsProgress = true;
			WorkerSupportsCancellation = true;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Work cycling

		protected virtual Boolean StartWork ()
		{
			Boolean lRet = false;
#if DEBUG
			Boolean lThreadEnding = false;
			if (IsBusy && !HasWorkCycles)
			{
				System.Diagnostics.Debug.Print ("!!! Worker thread ending");
				lThreadEnding = true;
			}
#endif
			try
			{
				HasWorkCycles = true;
				RunWorkerAsync ();
				lRet = true;
			}
#if DEBUG
			catch (Exception pException)
			{
				if (lThreadEnding)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
#else
			catch
			{
			}
#endif
			return lRet;
		}

		protected virtual Boolean StopWork ()
		{
			Boolean lRet = HasWorkCycles;
			HasWorkCycles = false;
			return lRet;
		}

		//=============================================================================

		public Boolean IsWorking
		{
			get
			{
				Boolean lRet = false;
				lock (mWaitLock)
				{
					lRet = mIsWorking;
				}
				return lRet;
			}
			protected set
			{
				lock (mWaitLock)
				{
					mIsWorking = value;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected Boolean HasWorkCycles
		{
			get
			{
				return (WorkCycle > 0);
			}
			set
			{
				WorkCycle = value ? mWaitCycles : 0;
			}
		}

		protected UInt32 StartWorkCycle ()
		{
			return --WorkCycle;
		}

		protected Boolean CancelWorkCycle (DoWorkEventArgs e)
		{
			if (e.Cancel || CancellationPending)
			{
				e.Cancel = true;
				return true;
			}
			return false;
		}

		protected Boolean HasNewWork (UInt32 pCurrentWorkCycle)
		{
			return (WorkCycle > pCurrentWorkCycle);
		}

		protected Boolean WaitForWork (UInt32 pCurrentWorkCycle, DoWorkEventArgs e)
		{
			if (HasNewWork (pCurrentWorkCycle))
			{
				return true;
			}
			return WaitForWork (e);
		}
		protected Boolean WaitForWork (DoWorkEventArgs e)
		{
			if (HasWorkCycles && !CancelWorkCycle (e))
			{
				Thread.Sleep ((int)mWaitTime);

				if (HasWorkCycles && !CancelWorkCycle (e))
				{
					return true;
				}
			}
			return false;
		}

		//=============================================================================

		private UInt32 WorkCycle
		{
			get
			{
				UInt32 lRet = 0;
				lock (mWaitLock)
				{
					lRet = mWorkCycle;
				}
				return lRet;
			}
			set
			{
				lock (mWaitLock)
				{
					mWorkCycle = value;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		//	
		//	Sample worker method - override in derived class
		//
		///////////////////////////////////////////////////////////////////////////////

		protected override void OnDoWork (DoWorkEventArgs e)
		{
			UInt32 lWorkCycle;

			do
			{
				lWorkCycle = StartWorkCycle ();

				if ((lWorkCycle > 0) && !CancelWorkCycle (e))
				{
					IsWorking = true;
					//
					// Do work...
					//
					IsWorking = false;
				}
			}
			while (WaitForWork (lWorkCycle, e));
		}

		#endregion
	}
}
