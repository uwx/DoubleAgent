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
	public partial class StateForm : UserControl
	{
		private CharacterFile	mCharacterFile = null;
		private String			mStateName = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public StateForm ()
		{
			InitializeComponent ();
			CausesValidation = Visible;

			if (Program.MainForm != null)
			{
				Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
			}
		}

		private void StateForm_VisibleChanged (object sender, EventArgs e)
		{
			CausesValidation = Visible;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
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

		[System.ComponentModel.Browsable (false)]
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

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || String.IsNullOrEmpty (mStateName));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event Global.GoToAnimationEventHandler GoToAnimation;

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
			String[]	lStateAnimations = null;

			if (IsEmpty)
			{
				ListViewAnimations.Items.Clear ();
				ListViewAnimations.Enabled = false;
			}
			else
			{
				ListViewAnimations.Enabled = true;//!Program.FileIsReadOnly;

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

			CausesValidation = false;
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
			CausesValidation = Visible;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteStateAnimations (String pStateName, String[] pAnimationNames)
		{
			UpdateAllStateAnimations	lUpdate = null;

			if ((mCharacterFile != null) && !String.IsNullOrEmpty (pStateName) && !Program.FileIsReadOnly)
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

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				AddDeleteStateAnimation		lAddDeleteStateAnimation = sender as AddDeleteStateAnimation;
				UpdateAllStateAnimations	lUpdateAllStateAnimations = sender as UpdateAllStateAnimations;

				if ((lAddDeleteStateAnimation != null) && (lAddDeleteStateAnimation.Target == mStateName))
				{
					ShowStateAnimations ();
				}
				else if ((lUpdateAllStateAnimations != null) && (lUpdateAllStateAnimations.Target == mStateName))
				{
					ShowStateAnimations ();
				}
				else if ((sender is AddDeleteAnimation) || (sender is UpdateAnimation))
				{
					ShowStateAnimations ();
				}
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
				AddDeleteStateAnimation	lUpdate;

				if (e.Item.Checked)
				{
					lUpdate = new AddDeleteStateAnimation (mStateName, e.Item.Text, false);
				}
				else
				{
					lUpdate = new AddDeleteStateAnimation (mStateName, e.Item.Text, true);
				}
				AddDeleteStateAnimation.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as AddDeleteStateAnimation, this);
			}
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsEmpty && (GoToAnimation != null))
			{
				ListViewItem	lItem = ListViewAnimations.SelectedItem;

				if ((lItem != null) && mCharacterFile.Gestures.Contains (lItem.Text))
				{
					try
					{
						GoToAnimation (this, new Global.AnimationEventArgs (mCharacterFile.Gestures[lItem.Text]));
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
