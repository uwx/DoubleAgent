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
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class StatePanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				State = (FilePart is ResolveState) ? (FilePart as ResolveState).Target : null;

				ShowStateName ();
				ShowStateAnimations ();
			}
		}

		protected FileState State
		{
			get;
			set;
		}
		protected String StateName
		{
			get
			{
				return (FilePart is ResolveState) ? (FilePart as ResolveState).StateName : (State != null) ? State.StateName : null;
			}
		}


		public override Boolean IsPanelEmpty
		{
			get
			{
				return base.IsPanelEmpty || String.IsNullOrEmpty (StateName);
			}
		}
		protected override bool TrackUpdatesWhenHidden
		{
			get
			{
				return true;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event NavigationEventHandler Navigate;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public override object NavigationContext
		{
			get
			{
				return new PanelContext (this);
			}
			set
			{
				if (value is PanelContext)
				{
					(value as PanelContext).RestoreContext (this);
				}
				else
				{
					base.NavigationContext = value;
				}
			}
		}

		private void NavigateToItem (String pAnimationName)
		{
			if (!IsPanelEmpty && (Navigate != null) && CharacterFile.Gestures.Contains (pAnimationName))
			{
				try
				{
					Navigate (this, new NavigationEventArgs (new ResolveAnimation (pAnimationName)));
				}
				catch
				{
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowStateName ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					TextBoxName.Clear ();
				}
				else
				{
					TextBoxName.Text = StateName;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteStateAnimations (String pStateName, String[] pAnimationNames)
		{
			UpdateAllStateAnimations lUpdate = null;

			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				lUpdate = new UpdateAllStateAnimations (pStateName, pAnimationNames);
				if (!UpdateAllStateAnimations.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAllStateAnimations, this))
				{
					lUpdate = null;
				}
			}
			return lUpdate;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleItemChecked (String pAnimationName, Boolean pItemChecked)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				AddDeleteStateAnimation lUpdate;

				if (pItemChecked)
				{
					lUpdate = new AddDeleteStateAnimation (StateName, pAnimationName, false);
				}
				else
				{
					lUpdate = new AddDeleteStateAnimation (StateName, pAnimationName, true);
				}
				AddDeleteStateAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteStateAnimation, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			AddDeleteStateAnimation lAddDeleteStateAnimation = pUpdate as AddDeleteStateAnimation;
			UpdateAllStateAnimations lUpdateAllStateAnimations = pUpdate as UpdateAllStateAnimations;
			AddDeleteAnimation lAddDeleteAnimation = pUpdate as AddDeleteAnimation;
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;

			if ((lAddDeleteStateAnimation != null) && (lAddDeleteStateAnimation.CharacterFile == CharacterFile) && (lAddDeleteStateAnimation.StateName == StateName))
			{
				ShowStateAnimations ();
			}
			else if ((lUpdateAllStateAnimations != null) && (lAddDeleteStateAnimation.CharacterFile == CharacterFile) && (lUpdateAllStateAnimations.StateName == StateName))
			{
				ShowStateAnimations ();
			}
			else if ((lAddDeleteAnimation != null) && (lAddDeleteAnimation.CharacterFile == CharacterFile))
			{
				ShowStateAnimations ();
			}
			else if ((lUpdateAnimation != null) && (lUpdateAnimation.CharacterFile == CharacterFile) && lUpdateAnimation.NameChanged)
			{
				ShowStateAnimations ();
			}
		}

		#endregion
	}
}
