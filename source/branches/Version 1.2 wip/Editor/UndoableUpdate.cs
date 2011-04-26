/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	namespace Updates
	{
		internal abstract class UndoableUpdate : UndoUnit
		{
			protected UndoableUpdate (Boolean pForClipboard)
			{
				this.ForClipboard = pForClipboard;
			}

			public CharacterFile CharacterFile
			{
				get
				{
					return Program.MainForm.CharacterFile;
				}
			}
			public Object Source
			{
				get;
				protected set;
			}
			public Boolean ForClipboard
			{
				get;
				protected set;
			}
			public override String ActionDescription
			{
				get
				{
					if (this.ForClipboard)
					{
						return Properties.Resources.UndoActionPaste;
					}
					return base.ActionDescription;
				}
			}

			///////////////////////////////////////////////////////////////////////////////

			protected override UndoUnit OnApplied (UndoUnit pRedoUnit, System.EventArgs pEventArgs)
			{
				if (!Object.ReferenceEquals (pRedoUnit, this))
				{
					UndoableUpdate lRedoUnit = pRedoUnit as UndoableUpdate;
					if (lRedoUnit != null)
					{
						lRedoUnit.Source = Source;
					}
				}
				return base.OnApplied (pRedoUnit, pEventArgs);
			}

			public virtual Boolean PutUndo ()
			{
				return PutUndo (null);
			}

			public virtual Boolean PutUndo (Object pSource)
			{
				if (pSource != null)
				{
					Source = pSource;
				}
				if (CharacterFile.IsDirty)
				{
					Program.MainForm.FileIsDirty ();
				}
				return Program.UndoManager.PutUndoUnit (this);
			}

			static public Boolean PutUndo (UndoableUpdate pUndoableAction, Object pSource)
			{
				if (pUndoableAction != null)
				{
					return pUndoableAction.PutUndo (pSource);
				}
				return false;
			}

			///////////////////////////////////////////////////////////////////////////////

			public override String ToString ()
			{
				return String.Format (Properties.Resources.UndoDescription, this.ActionDescription.Trim (), this.TargetDescription.Trim (), this.ChangeDescription.Trim ()).Trim ();
			}
#if DEBUG
			public virtual String DebugString
			{
				get
				{
					return ToString ();
				}
			}
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		internal abstract class UndoableUpdate<T> : UndoableUpdate where T : class
		{
			protected UndoableUpdate ()
				: base (false)
			{
			}
			protected UndoableUpdate (Object pTarget)
				: base (false)
			{
				RawTarget = pTarget;
#if DEBUG
				if (RawTarget != null)
				{
					System.Diagnostics.Debug.Assert ((RawTarget is ResolvePart<T>) || (RawTarget is T));
				}
#endif
			}
			protected UndoableUpdate (Object pTarget, Boolean pForClipboard)
				: base (pForClipboard)
			{
				RawTarget = pTarget;
#if DEBUG
				if (RawTarget != null)
				{
					System.Diagnostics.Debug.Assert ((RawTarget is ResolvePart<T>) || (RawTarget is T));
				}
#endif
			}

			public virtual T Target
			{
				get
				{
					if (RawTarget is ResolvePart<T>)
					{
						return (RawTarget as ResolvePart<T>).Target;
					}
					else
					{
						return RawTarget as T;
					}
				}
				protected set
				{
					RawTarget = value;
				}
			}
			protected Object RawTarget
			{
				get;
				private set;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal abstract class UndoableAddDelete<T> : UndoableUpdate<T> where T : class
		{
			public UndoableAddDelete ()
				: base (default (T))
			{
				this.IsDelete = false;
				this.IsRedo = false;
				this.ForClipboard = false;
			}
			public UndoableAddDelete (Object pTarget)
				: base (pTarget)
			{
				this.IsDelete = (pTarget is T);
				this.IsRedo = false;
				this.ForClipboard = false;
			}
			public UndoableAddDelete (Boolean pForClipboard)
				: base (default (T))
			{
				this.IsDelete = false;
				this.IsRedo = false;
				this.ForClipboard = pForClipboard;
			}
			public UndoableAddDelete (Object pTarget, Boolean pForClipboard)
				: base (pTarget)
			{
				this.IsDelete = (pTarget is T);
				this.IsRedo = false;
				this.ForClipboard = pForClipboard;
			}

			public Boolean IsDelete
			{
				get;
				protected set;
			}
			protected Boolean IsRedo
			{
				get;
				set;
			}
			public override String ActionDescription
			{
				get
				{
					return (IsDelete == !IsRedo) ? this.ForClipboard ? Properties.Resources.UndoActionCut : Properties.Resources.UndoActionDelete : this.ForClipboard ? Properties.Resources.UndoActionPaste : Properties.Resources.UndoActionAdd;
				}
			}
#if DEBUG
			protected String RedoString
			{
				get
				{
					return IsRedo ? "redo" : "undo";
				}
			}
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class RepeatingUpdate<T, TT>
			where T : UndoableUpdate<TT>
			where TT : class
		{
			public T UpdateStart
			{
				get;
				private set;
			}
			public Boolean UpdateStarted
			{
				get
				{
					return (UpdateStart != null);
				}
			}

			public RepeatingUpdate ()
			{
				this.UpdateStart = null;
			}

			///////////////////////////////////////////////////////////////////////////////

			public void PutUpdate (T pUpdate, int pRepeatNum)
			{
				if (pRepeatNum > 0)
				{
					if (!this.UpdateStarted)
					{
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(First) [{1}]", pRepeatNum.ToString (), pUpdate.DebugString);
#endif
						this.UpdateStart = pUpdate.Apply () as T;
					}
					else
					{
#if DEBUG_NOT
						System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(Repeat) [{1}]", pRepeatNum.ToString (), pUpdate.DebugString);
#endif
						pUpdate.Apply ();
					}
				}
				else if (this.UpdateStarted)
				{
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(Last) [{1}]", pRepeatNum.ToString (), pUpdate.DebugString);
#endif
					pUpdate.Apply ();
					EndUpdate ();
				}
				else
				{
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(Single) [{1}] PutUndo [{1}]", pRepeatNum.ToString (), pUpdate.DebugString);
#endif
					T lUpdate = pUpdate.Apply () as T;
					if (lUpdate != null)
					{
						lUpdate.PutUndo ();
					}
				}
			}

			public void EndUpdate ()
			{
				if (this.UpdateStarted)
				{
					T lUpdate = this.UpdateStart;
					this.UpdateStart = null;
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("EndRepeat PutUndo [{0}]", lUpdate.DebugString);
#endif
					lUpdate.PutUndo ();
				}
				this.UpdateStart = null;
			}
		}
	}
}

