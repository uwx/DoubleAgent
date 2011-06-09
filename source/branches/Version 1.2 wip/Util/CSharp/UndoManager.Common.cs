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
		#region Properties

		public abstract String TargetDescription
		{
			get;
		}
		public virtual String ActionDescription
		{
			get
			{
				return String.Empty;
			}
		}
		public virtual String ChangeDescription
		{
			get
			{
				return String.Empty;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public delegate void AppliedEventHandler (object sender, System.EventArgs e);
		public event AppliedEventHandler Applied;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public abstract UndoUnit Apply ();

		public virtual UndoUnit Apply (AppliedEventHandler pEventHandler)
		{
			Applied += pEventHandler;
			return Apply ();
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
#if DEBUG
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
#else
				catch {}
#endif
			}
		}

		protected virtual UndoUnit OnApplied (UndoUnit pRedoUnit)
		{
			return OnApplied (pRedoUnit, System.EventArgs.Empty);
		}

		protected virtual UndoUnit OnApplied (UndoUnit pRedoUnit, System.EventArgs pEventArgs)
		{
			OnApplied (pEventArgs);
			if ((pRedoUnit != null) && (pRedoUnit != this) && (this.Applied != null))
			{
				pRedoUnit.Applied = (AppliedEventHandler)this.Applied.Clone ();
			}
			return pRedoUnit;
		}

		///////////////////////////////////////////////////////////////////////////////

		public override String ToString ()
		{
			return String.Format ("{0} {1} {2}", this.ActionDescription.Trim (), this.TargetDescription.Trim (), this.ChangeDescription.Trim ()).Trim ();
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

		protected Stack<UndoUnit> mUndoStack;
		protected Stack<UndoUnit> mRedoStack;

		public Boolean CanUndo
		{
			get
			{
				return (this.mUndoStack.Count > 0);
			}
		}

		public Boolean CanRedo
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
				String lTitle = "Undo";

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
				String lTitle = "Redo";

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

			private UndoUnit mUndoUnit;

			public UndoUnit UndoUnit
			{
				get
				{
					return this.mUndoUnit;
				}
			}
		}

		public delegate void UndoneEventHandler (object sender, EventArgs e);
		public event UndoneEventHandler Undone;

		public delegate void RedoneEventHandler (object sender, EventArgs e);
		public event RedoneEventHandler Redone;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public void Clear ()
		{
			mUndoStack = new Stack<UndoUnit> ();
			mRedoStack = new Stack<UndoUnit> ();
		}

		public Boolean PutUndoUnit (UndoUnit pUndoUnit)
		{
			if (pUndoUnit != null)
			{
				pUndoUnit.Applied += new UndoUnit.AppliedEventHandler (UndoUnitApplied);
				mUndoStack.Push (pUndoUnit);
				mRedoStack.Clear ();
				return true;
			}
			return false;
		}

		public Boolean Undo ()
		{
			if (mUndoStack.Count > 0)
			{
				UndoUnit lUndoUnit = mUndoStack.Pop ();
				UndoUnit lRedoUnit = null;

				try
				{
					lRedoUnit = lUndoUnit.Apply ();
				}
#if DEBUG
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
#else
				catch {}
#endif

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

		public Boolean Redo ()
		{
			if (mRedoStack.Count > 0)
			{
				UndoUnit lRedoUnit = mRedoStack.Pop ();
				UndoUnit lUndoUnit = null;

				try
				{
					lUndoUnit = lRedoUnit.Apply ();
				}
#if DEBUG
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
#else
				catch {}
#endif

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
