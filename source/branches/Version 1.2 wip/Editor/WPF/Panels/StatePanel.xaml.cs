using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Global;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class StatePanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public StatePanel ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowStateAnimations ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				// Refresh it just in case the state was newly added
				State = (FilePart is ResolveState) ? (FilePart as ResolveState).Target : null;

				if (IsPanelEmpty)
				{
					ListViewAnimations.Items.Clear ();
					ListViewAnimations.IsEnabled = false;
				}
				else
				{
					ListViewAnimations.IsEnabled = true;//!Program.FileIsReadOnly;
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
		}

		private void ShowFileAnimations (String[] pStateAnimations)
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				String[] lAnimations = CharacterFile.GetAnimationNames ();
				int lListNdx = 0;

				ListViewAnimations.SetVerticalScrollBarVisibility (ScrollBarVisibility.Disabled);
				ListViewAnimations.UpdateItemCount (lAnimations.Length);

				foreach (String lAnimation in lAnimations)
				{
					ListViewItemCommon lListItem;
					CheckBox lListItemContent;

					lListItem = ((lListNdx < ListViewAnimations.Items.Count) ? ListViewAnimations.Items[lListNdx] : ListViewAnimations.Items.Add (lAnimation)) as ListViewItemCommon;
					lListItemContent = (lListItem.Content is CheckBox) ? lListItem.Content as CheckBox : new CheckBox ();

					lListItem.IsTabStop = false;
					lListItem.Focusable = false;
					lListItem.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
					lListItem.Content = lListItemContent;

					lListItemContent.Content = lAnimation;
					lListItemContent.IsEnabled = !Program.FileIsReadOnly;
					lListItemContent.Checked += new RoutedEventHandler (ListItemContent_CheckedChanged);
					lListItemContent.Unchecked += new RoutedEventHandler (ListItemContent_CheckedChanged);

					if (
							(pStateAnimations != null)
						&& (
								(Array.IndexOf (pStateAnimations, lAnimation) >= 0)
							|| (Array.IndexOf (pStateAnimations, lAnimation.ToUpper ()) >= 0)
							)
						)
					{
						lListItemContent.IsChecked = true;
					}
					else
					{
						lListItemContent.IsChecked = false;
					}
					lListNdx++;
				}
			}
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
		#region EventHandlers

		void ListItemContent_CheckedChanged (object sender, RoutedEventArgs e)
		{
			CheckBox lItemContent = e.Source as CheckBox;
			if ((lItemContent != null) && !IsPanelFilling)
			{
				HandleItemChecked (lItemContent.Content as String, lItemContent.IsChecked.Value);
			}
		}

		#endregion
	}
}
