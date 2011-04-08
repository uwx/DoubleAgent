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

namespace AgentCharacterEditor
{
	public partial class StateForm : UserControl
	{
		private CharacterFile	mCharacterFile = null;
		private String			mStateName = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public StateForm ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				ShowStateAnimations ();
			}
		}

		public String StateName
		{
			get
			{
				return mStateName;
			}
			set
			{
				mStateName = value;
				ShowStateName ();
				ShowStateAnimations ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || String.IsNullOrEmpty (mStateName));
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
				TextBoxName.Text = mStateName;
			}
		}

		private void ShowStateAnimations ()
		{
			FileStates	lFileStates;
			String[]							lStateAnimations = null;

			if (IsEmpty)
			{
				ListViewAnimations.Items.Clear ();
				ListViewAnimations.Enabled = false;
			}
			else
			{
				ListViewAnimations.Enabled = true;//!Program.MainForm.FileIsReadOnly;

				lFileStates = mCharacterFile.States;
				if (lFileStates != null)
				{
					if (lFileStates.ContainsKey (mStateName))
					{
						lStateAnimations = lFileStates[mStateName];
					}
					else if (lFileStates.ContainsKey (mStateName.ToUpper ()))
					{
						lStateAnimations = lFileStates[mStateName.ToUpper ()];
					}
				}
				ShowFileAnimations (lStateAnimations);
			}
		}

		private void ShowFileAnimations (String[] pStateAnimations)
		{
			String[]	lAnimations = mCharacterFile.GetAnimationNames ();
			int			lListNdx = 0;

			ListViewAnimations.ItemCheck -= new System.Windows.Forms.ItemCheckEventHandler (this.ListViewAnimations_ItemCheck);
			ListViewAnimations.ItemChecked -= new System.Windows.Forms.ItemCheckedEventHandler (ListViewAnimations_ItemChecked);
			ListViewAnimations.BeginUpdate ();
			ListViewAnimations.UpdateItemCount (lAnimations.Length);

			foreach (String lAnimation in lAnimations)
			{
				ListViewItem	lListItem;

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
			ListViewAnimations.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler (this.ListViewAnimations_ItemCheck);
			ListViewAnimations.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler (ListViewAnimations_ItemChecked);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class AddStateAnimation : UndoableUpdate
		{
			public AddStateAnimation (CharacterFile pCharacterFile, String pStateName, String pAnimationName)
				: base (pCharacterFile)
			{
				this.StateName = pStateName;
				this.AnimationName = pAnimationName;
			}

			public String StateName
			{
				get;
				private set;
			}
			public String AnimationName
			{
				get;
				private set;
			}

			public override UndoUnit Apply ()
			{
				if (CharacterFile.States.AddStateAnimation (this.StateName, this.AnimationName))
				{
					return OnApplied (new RemoveStateAnimation (CharacterFile, this.StateName, this.AnimationName));
				}
				return null;
			}

			public override string ToString ()
			{
				return String.Format ("\"{0}\" uses \"{1}\"", this.StateName, this.AnimationName);
			}
		}

		internal class RemoveStateAnimation : UndoableUpdate
		{
			public RemoveStateAnimation (CharacterFile pCharacterFile, String pStateName, String pAnimationName)
				: base (pCharacterFile)
			{
				this.StateName = pStateName;
				this.AnimationName = pAnimationName;
			}

			public String StateName
			{
				get;
				private set;
			}
			public String AnimationName
			{
				get;
				private set;
			}

			public override UndoUnit Apply ()
			{
				if (CharacterFile.States.RemoveStateAnimation (this.StateName, this.AnimationName))
				{
					return OnApplied (new AddStateAnimation (CharacterFile, this.StateName, this.AnimationName));
				}
				return null;
			}

			public override string ToString ()
			{
				return String.Format ("\"{0}\" uses \"{1}\"", this.StateName, this.AnimationName);
			}
		}
		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewAnimations_ItemCheck (object sender, ItemCheckEventArgs e)
		{
			if (IsEmpty || Program.MainForm.FileIsReadOnly)
			{
				e.NewValue = e.CurrentValue;
			}
		}

		private void ListViewAnimations_ItemChecked (object sender, ItemCheckedEventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UndoableUpdate	lUpdate;

				if (e.Item.Checked)
				{
					lUpdate = new AddStateAnimation (mCharacterFile, mStateName, e.Item.Text);
				}
				else
				{
					lUpdate = new RemoveStateAnimation (mCharacterFile, mStateName, e.Item.Text);
				}
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableAction_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UndoableUpdate);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void UndoableAction_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		public void PanelAnimations_AnimationAdded (object sender, AnimationsForm.AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		public void PanelAnimations_AnimationRemoved (object sender, AnimationsForm.AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		public void PanelAnimation_AnimationNameChanged (object sender, AnimationForm.AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		#endregion
	}
}
