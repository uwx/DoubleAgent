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

			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.InitializationComplete += new EventHandler (MainForm_InitializationComplete);
			}
		}

		void MainForm_InitializationComplete (object sender, EventArgs e)
		{
			MainForm.Singleton.PanelAnimations.AnimationAdded += new AnimationAddedEventHandler (PanelAnimations_AnimationAdded);
			MainForm.Singleton.PanelAnimations.AnimationRemoved += new AnimationRemovedEventHandler (PanelAnimations_AnimationRemoved);
			MainForm.Singleton.PanelAnimation.AnimationNameChanged += new AnimationNameChangedEventHandler (PanelAnimation_AnimationNameChanged);
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

		public event GoToAnimationEventHandler GoToAnimation;

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
				ListViewAnimations.Enabled = true;//!MainForm.Singleton.FileIsReadOnly;

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

		internal class AddDeleteStateAnimation : UndoableUpdate<CharacterFile>
		{
			public AddDeleteStateAnimation (CharacterFile pCharacterFile, String pStateName, String pAnimationName, Boolean pIsDelete)
				: base (pCharacterFile, pCharacterFile)
			{
				this.StateName = pStateName;
				this.AnimationName = pAnimationName;
				this.IsDelete = pIsDelete;
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
			public Boolean IsDelete
			{
				get;
				private set;
			}

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (this.StateName);
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return String.Format (" uses {0}", Program.QuotedTitle (this.AnimationName));
				}
			}

			public override UndoUnit Apply ()
			{
				if (this.IsDelete)
				{
					if (CharacterFile.States.RemoveStateAnimation (this.StateName, this.AnimationName))
					{
						return OnApplied (new AddDeleteStateAnimation (CharacterFile, this.StateName, this.AnimationName, false));
					}
				}
				else
				{
					if (CharacterFile.States.AddStateAnimation (this.StateName, this.AnimationName))
					{
						return OnApplied (new AddDeleteStateAnimation (CharacterFile, this.StateName, this.AnimationName, true));
					}
				}
				return null;
			}
		}

		internal class UpdateAllStateAnimations : UndoableUpdate<CharacterFile>
		{
			public UpdateAllStateAnimations (CharacterFile pCharacterFile, String pStateName, String[] pAnimationNames)
				: base (pCharacterFile, pCharacterFile)
			{
				this.StateName = pStateName;
				this.AnimationNames = pAnimationNames;
			}

			public String StateName
			{
				get;
				private set;
			}
			public String[] AnimationNames
			{
				get;
				private set;
			}
			public override String TargetDescription
			{
				get
				{
					return Program.TitleState (this.StateName);
				}
			}

			public override UndoUnit Apply ()
			{
				UpdateAllStateAnimations	lApplied = null;
				String[]					lAnimationNames = null;

				if (this.CharacterFile.States.ContainsKey (this.StateName))
				{
					lAnimationNames = this.CharacterFile.States[this.StateName].Clone () as String[];
				}
				if (lAnimationNames != null)
				{
					foreach (String lAnimationName in lAnimationNames)
					{
						if ((this.AnimationNames == null) || !Array.Exists (this.AnimationNames, lAnimationName.Equals))
						{
							if (this.CharacterFile.States.RemoveStateAnimation (this.StateName, lAnimationName))
							{
								lApplied = this;
							}
						}
					}
				}
				if (this.AnimationNames != null)
				{
					foreach (String lAnimationName in this.AnimationNames)
					{
						if ((lAnimationNames == null) || !Array.Exists (lAnimationNames, lAnimationName.Equals))
						{
							if (this.CharacterFile.States.AddStateAnimation (this.StateName, lAnimationName))
							{
								lApplied = this;
							}
						}
					}
				}
				if (lApplied != null)
				{
					lApplied.AnimationNames = lAnimationNames;
					return OnApplied (lApplied);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		
		internal Boolean PasteStateAnimations (String pStateName, String[] pAnimationNames)
		{
			Boolean	lRet = false;
			
			if ((mCharacterFile != null) && !String.IsNullOrEmpty (pStateName) && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateAllStateAnimations	lUpdate = new UpdateAllStateAnimations (mCharacterFile, pStateName, pAnimationNames);

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableAction_Applied);
				lRet = UpdateAllStateAnimations.PutUndo (lUpdate.Apply () as UpdateAllStateAnimations);
			}
			return lRet;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewAnimations_ItemCheck (object sender, ItemCheckEventArgs e)
		{
			if (IsEmpty || MainForm.Singleton.FileIsReadOnly)
			{
				e.NewValue = e.CurrentValue;
			}
		}

		private void ListViewAnimations_ItemChecked (object sender, ItemCheckedEventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				AddDeleteStateAnimation	lUpdate;

				if (e.Item.Checked)
				{
					lUpdate = new AddDeleteStateAnimation (mCharacterFile, mStateName, e.Item.Text, false);
				}
				else
				{
					lUpdate = new AddDeleteStateAnimation (mCharacterFile, mStateName, e.Item.Text, true);
				}

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (UndoableAction_Applied);
				AddDeleteStateAnimation.PutUndo (lUpdate.Apply () as AddDeleteStateAnimation);
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
						GoToAnimation (this, new AnimationEventArgs (mCharacterFile.Gestures[lItem.Text]));
					}
					catch
					{
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void UndoableAction_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				if ((sender is AddDeleteStateAnimation) && ((sender as AddDeleteStateAnimation).StateName == mStateName))
				{
					ShowStateAnimations ();
				}
				else if ((sender is UpdateAllStateAnimations) && ((sender as UpdateAllStateAnimations).StateName == mStateName))
				{
					ShowStateAnimations ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void PanelAnimations_AnimationAdded (object sender, AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		internal void PanelAnimations_AnimationRemoved (object sender, AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		internal void PanelAnimation_AnimationNameChanged (object sender, AnimationEventArgs e)
		{
			if (!IsEmpty)
			{
				ShowStateAnimations ();
			}
		}

		#endregion
	}
}
