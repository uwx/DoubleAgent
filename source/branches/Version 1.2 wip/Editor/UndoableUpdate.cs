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
	internal abstract class UndoableUpdate<T> : UndoUnit<T> where T : class
	{
		protected UndoableUpdate (CharacterFile pCharacterFile, T pTarget)
			: base (pTarget)
		{
			this.CharacterFile = pCharacterFile;
		}

		public CharacterFile CharacterFile
		{
			get;
			private set;
		}

		public virtual bool PutUndo ()
		{
			Program.MainForm.FileIsDirty = CharacterFile.IsDirty;
			return Program.MainForm.UndoManager.PutUndoUnit (this);
		}

		static public bool PutUndo (UndoableUpdate<T> pUndoableAction)
		{
			if (pUndoableAction != null)
			{
				return pUndoableAction.PutUndo ();
			}
			return false;
		}

		public virtual String DebugString ()
		{
			return ToString ();
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal abstract class UndoableUpdate : UndoableUpdate<CharacterFile>
	{
		public UndoableUpdate (CharacterFile pCharacterFile)
			: base (pCharacterFile, pCharacterFile)
		{
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	internal abstract class UndoableAddRemove<T> : UndoableUpdate<T> where T : class
	{
		public UndoableAddRemove (CharacterFile pCharacterFile)
			: base (pCharacterFile, default (T))
		{
			this.IsRemove = false;
			this.IsRedo = false;
		}
		public UndoableAddRemove (CharacterFile pCharacterFile, T pTarget)
			: base (pCharacterFile, pTarget)
		{
			this.IsRemove = true;
			this.IsRedo = false;
		}

		public Boolean IsRemove
		{
			get;
			protected set;
		}
		protected Boolean IsRedo
		{
			get;
			set;
		}

		protected String AddRemoveTitle
		{
			get
			{
				return (IsRemove == !IsRedo) ? "remove" : "add";
			}
		}
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
					System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(First) [{1}]", pRepeatNum.ToString (), pUpdate.DebugString ());
#endif
					this.UpdateStart = pUpdate.Apply () as T;
				}
				else
				{
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(Repeat) [{1}]", pRepeatNum.ToString (), pUpdate.DebugString ());
#endif
					pUpdate.Apply ();
				}
			}
			else if (this.UpdateStarted)
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(Last) [{1}]", pRepeatNum.ToString (), pUpdate.DebugString ());
#endif
				pUpdate.Apply ();
				EndUpdate ();
			}
			else
			{
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("PutRepeat {0} Apply(Single) [{1}] PutUndo [{1}]", pRepeatNum.ToString (), pUpdate.DebugString ());
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
				System.Diagnostics.Debug.Print ("EndRepeat PutUndo [{0}]", lUpdate.DebugString ());
#endif
				lUpdate.PutUndo ();
			}
			this.UpdateStart = null;
		}
	}
}