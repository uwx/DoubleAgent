using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
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
				bool	lChanged = false;

				if (e.Item.Checked)
				{
					lChanged = mCharacterFile.States.AddStateAnimation (mStateName, e.Item.Text);
				}
				else
				{
					lChanged = mCharacterFile.States.RemoveStateAnimation (mStateName, e.Item.Text);
				}
				if (lChanged)
				{
#if DEBUG
					ShowStateAnimations ();
#endif
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		#endregion
	}
}
