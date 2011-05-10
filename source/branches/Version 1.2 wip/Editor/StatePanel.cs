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
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public partial class StatePanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public StatePanel ()
		{
			InitializeComponent ();
		}

		#endregion
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
				return (State != null) ? State.StateName : (FilePart is ResolveState) ? (FilePart as ResolveState).StateName : null;
			}
		}


		public override Boolean IsEmpty
		{
			get
			{
				return base.IsEmpty || String.IsNullOrEmpty (StateName);
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

		public event Global.NavigationEventHandler Navigate;

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

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (StatePanel pPanel)
				: base (pPanel)
			{
				SelectedAnimation = pPanel.ListViewAnimations.SelectedIndex;
			}

			public void RestoreContext (StatePanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewAnimations.SelectedIndex = SelectedAnimation;
			}

			public int SelectedAnimation
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowStateName ()
		{
			if (IsEmpty)
			{
				TextBoxName.ResetText ();
			}
			else
			{
				TextBoxName.Text = StateName;
			}
		}

		private void ShowStateAnimations ()
		{
			// Refresh it just in case the state was newly added
			State = (FilePart is ResolveState) ? (FilePart as ResolveState).Target : null;

			if (IsEmpty)
			{
				ListViewAnimations.Items.Clear ();
				ListViewAnimations.Enabled = false;
			}
			else
			{
				ListViewAnimations.Enabled = true;//!Program.FileIsReadOnly;
				if (State == null)
				{
					ShowFileAnimations (null);
				}
				else
				{
					ShowFileAnimations (State.AnimationNames);
				}
			}
		}

		private void ShowFileAnimations (String[] pStateAnimations)
		{
			String[] lAnimations = CharacterFile.GetAnimationNames ();
			int lListNdx = 0;

			CausesValidation = false;
			ListViewAnimations.BeginUpdate ();
			ListViewAnimations.UpdateItemCount (lAnimations.Length);

			foreach (String lAnimation in lAnimations)
			{
				ListViewItem lListItem;

				lListItem = (lListNdx < ListViewAnimations.Items.Count) ? ListViewAnimations.Items[lListNdx] : ListViewAnimations.Items.Add (lAnimation);
				lListItem.Text = lAnimation;

				if (
						(pStateAnimations != null)
					&& (
							(Array.IndexOf (pStateAnimations, lAnimation) >= 0)
						|| (Array.IndexOf (pStateAnimations, lAnimation.ToUpper ()) >= 0)
						)
					)
				{
					lListItem.Checked = true;
				}
				else
				{
					lListItem.Checked = false;
				}
				lListNdx++;
			}

			ListViewAnimations.EndUpdate ();
			ListViewAnimations.ArrangeIcons ();
			CausesValidation = Visible;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteStateAnimations (String pStateName, String[] pAnimationNames)
		{
			UpdateAllStateAnimations lUpdate = null;

			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				lUpdate = new UpdateAllStateAnimations (pStateName, pAnimationNames);
				if (!UpdateAllStateAnimations.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAllStateAnimations, this))
				{
					lUpdate = null;
				}
			}
			return lUpdate;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			AddDeleteStateAnimation lAddDeleteStateAnimation = pUpdate as AddDeleteStateAnimation;
			UpdateAllStateAnimations lUpdateAllStateAnimations = pUpdate as UpdateAllStateAnimations;

			if ((lAddDeleteStateAnimation != null) && (lAddDeleteStateAnimation.StateName == StateName))
			{
				ShowStateAnimations ();
			}
			else if ((lUpdateAllStateAnimations != null) && (lUpdateAllStateAnimations.StateName == StateName))
			{
				ShowStateAnimations ();
			}
			else if ((pUpdate is AddDeleteAnimation) || (pUpdate is UpdateAnimation))
			{
				ShowStateAnimations ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewAnimations_ItemCheck (object sender, ItemCheckEventArgs e)
		{
			if (CausesValidation && (IsEmpty || Program.FileIsReadOnly))
			{
				e.NewValue = e.CurrentValue;
			}
		}

		private void ListViewAnimations_ItemChecked (object sender, ItemCheckedEventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				AddDeleteStateAnimation lUpdate;

				if (e.Item.Checked)
				{
					lUpdate = new AddDeleteStateAnimation (StateName, e.Item.Text, false);
				}
				else
				{
					lUpdate = new AddDeleteStateAnimation (StateName, e.Item.Text, true);
				}
				AddDeleteStateAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteStateAnimation, this);
			}
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (Navigate != null))
			{
				ListViewItem lItem = ListViewAnimations.SelectedItem;

				if ((lItem != null) && CharacterFile.Gestures.Contains (lItem.Text))
				{
					try
					{
						Navigate (this, new Global.NavigationEventArgs (new ResolveAnimation (lItem.Text)));
					}
					catch
					{
					}
				}
			}
		}

		#endregion
	}
}
