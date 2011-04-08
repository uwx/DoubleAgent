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
using System.Collections.Generic;

namespace DoubleAgent
{
	public abstract class UndoUnit
	{
		#region Methods

#if DEBUG
		public override string ToString ()
		{
			return "UndoUnit";
		}
#endif

		public abstract UndoUnit Apply ();

		///////////////////////////////////////////////////////////////////////////////

		protected UndoUnit OnApplied (System.EventArgs pEventArgs, UndoUnit pRedoUnit)
		{
			OnApplied (pEventArgs);
			if ((pRedoUnit != null) && (pRedoUnit != this) && (this.Applied != null))
			{
				pRedoUnit.Applied = (AppliedEvent)this.Applied.Clone ();
			}
			return pRedoUnit;
		}

		protected virtual void OnApplied ()
		{
			OnApplied (System.EventArgs.Empty);
		}

		protected virtual void OnApplied (System.EventArgs pEventArgs)
		{
			if (this.Applied != null)
			{
				try
				{
					this.Applied (this, pEventArgs);
				}
				catch
				{
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public delegate void AppliedEvent (object sender, System.EventArgs e);

		public event AppliedEvent Applied;

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	public abstract class UndoUnit<T> : UndoUnit
	{
		public UndoUnit (T pTarget)
		{
			this.Target = pTarget;
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public T Target
		{
			get;
			protected set;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public class EventArgs : System.EventArgs
		{
			public EventArgs (T pTarget)
			{
				this.Target = pTarget;
			}

			public T Target
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

#if DEBUG
		public override string ToString ()
		{
			return "UndoUnit - " + typeof (T).Name;
		}
#endif

		protected override void OnApplied ()
		{
			base.OnApplied (new EventArgs (this.Target));
		}

		protected virtual UndoUnit OnApplied (UndoUnit pRedoUnit)
		{
			return base.OnApplied (new EventArgs (this.Target), pRedoUnit);
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	public class UndoManager
	{
		public UndoManager ()
		{
			Clear ();
		}

		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		private Stack <UndoUnit>	mUndoStack;
		private Stack <UndoUnit>	mRedoStack;

		public bool CanUndo
		{
			get
			{
				return (this.mUndoStack.Count > 0);
			}
		}

		public bool CanRedo
		{
			get
			{
				return (this.mRedoStack.Count > 0);
			}
		}

		public String UndoName
		{
			get
			{
				if (this.mUndoStack.Count > 0)
				{
					return this.mUndoStack.Peek ().ToString ();
				}
				return String.Empty;
			}
		}

		public String RedoName
		{
			get
			{
				if (this.mRedoStack.Count > 0)
				{
					return this.mRedoStack.Peek ().ToString ();
				}
				return String.Empty;
			}
		}

		public String UndoTip
		{
			get
			{
				String	lTitle = "Undo";

				if (this.mUndoStack.Count > 0)
				{
					lTitle = lTitle + " " + UndoName;
				}
				return lTitle;
			}
		}

		public String RedoTip
		{
			get
			{
				String	lTitle = "Redo";

				if (this.mRedoStack.Count > 0)
				{
					lTitle = lTitle + " " + RedoName;
				}
				return lTitle;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public class EventArgs : System.EventArgs
		{
			public EventArgs (UndoUnit pUndoUnit)
			{
				this.mUndoUnit = pUndoUnit;
			}

			private UndoUnit	mUndoUnit;

			public UndoUnit UndoUnit
			{
				get
				{
					return this.mUndoUnit;
				}
			}
		}

		public delegate void UndoneEvent (object sender, EventArgs e);
		public delegate void RedoneEvent (object sender, EventArgs e);

		public event UndoneEvent Undone;
		public event RedoneEvent Redone;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public void Clear ()
		{
			mUndoStack = new Stack<UndoUnit> ();
			mRedoStack = new Stack<UndoUnit> ();
		}

		public bool PutUndoUnit (UndoUnit pUndoUnit)
		{
			if (pUndoUnit != null)
			{
				pUndoUnit.Applied += new UndoUnit.AppliedEvent (UndoUnitApplied);
				mUndoStack.Push (pUndoUnit);
				mRedoStack.Clear ();
				return true;
			}
			return false;
		}

		public bool Undo ()
		{
			if (mUndoStack.Count > 0)
			{
				UndoUnit	lUndoUnit = mUndoStack.Pop ();
				UndoUnit	lRedoUnit = null;

				try
				{
					lRedoUnit = lUndoUnit.Apply ();
				}
				catch
				{
				}

				if (lRedoUnit != null)
				{
					mRedoStack.Push (lRedoUnit);
				}
				if (Undone != null)
				{
					Undone (this, new EventArgs (lUndoUnit));
				}
				return true;
			}
			return false;
		}

		public bool Redo ()
		{
			if (mRedoStack.Count > 0)
			{
				UndoUnit	lRedoUnit = mRedoStack.Pop ();
				UndoUnit	lUndoUnit = null;

				try
				{
					lUndoUnit = lRedoUnit.Apply ();
				}
				catch
				{
				}

				if (lUndoUnit != null)
				{
					mUndoStack.Push (lUndoUnit);
				}
				if (Redone != null)
				{
					Redone (this, new EventArgs (lRedoUnit));
				}
				return true;
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void UndoUnitApplied (object sender, System.EventArgs e)
		{
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] applied", sender.ToString ());
#endif
		}

		#endregion
	}
}
