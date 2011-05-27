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
using System.Windows.Forms;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Global;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
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
		#region Navigation

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (StatePanel pPanel)
				: base (pPanel)
			{
				SelectedAnimation = pPanel.ListViewAnimations.SelectedIndex;
				FocusedAnimation = pPanel.ListViewAnimations.FocusedIndex;
			}

			public void RestoreContext (StatePanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewAnimations.SelectedIndex = SelectedAnimation;
				pPanel.ListViewAnimations.FocusedIndex = FocusedAnimation;
			}

			public int SelectedAnimation
			{
				get;
				protected set;
			}
			public int FocusedAnimation
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowStateAnimations ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			// Refresh it just in case the state was newly added
			State = (FilePart is ResolveState) ? (FilePart as ResolveState).Target : null;

			if (IsPanelEmpty)
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

			PopIsPanelFilling (lWasFilling);
		}

		private void ShowFileAnimations (String[] pStateAnimations)
		{
			Boolean lWasFilling = PushIsPanelFilling (true);
			String[] lAnimations = CharacterFile.GetAnimationNames ();
			int lListNdx = 0;

			ListViewAnimations.BeginUpdate ();
			ListViewAnimations.UpdateItemCount (lAnimations.Length);

			foreach (String lAnimation in lAnimations)
			{
				ListViewItemCommon lListItem;

				lListItem = ((lListNdx < ListViewAnimations.Items.Count) ? ListViewAnimations.Items[lListNdx] : ListViewAnimations.Items.Add (lAnimation)) as ListViewItemCommon;
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

			PopIsPanelFilling (lWasFilling);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void ListViewAnimations_ItemCheck (object sender, ItemCheckEventArgs e)
		{
			if (!IsPanelFilling && (IsPanelEmpty || Program.FileIsReadOnly))
			{
				e.NewValue = e.CurrentValue;
			}
		}

		private void ListViewAnimations_ItemChecked (object sender, ItemCheckedEventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleItemChecked (e.Item.Text, e.Item.Checked);
			}
		}

		private void ListViewAnimations_ItemActivate (object sender, EventArgs e)
		{
			if (!IsPanelFilling)
			{
				ListViewItemCommon lItem = ListViewAnimations.SelectedItem as ListViewItemCommon;

				if (lItem != null)
				{
					NavigateToItem (lItem.Text);
				}
			}
		}

		#endregion
	}
}
