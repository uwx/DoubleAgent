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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class PartsTreePanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public PartsTreePanel ()
		{
			InitializeComponent ();
		}

		///////////////////////////////////////////////////////////////////////////////

		public void LoadExpansion ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				TreeViewItem lTreeItem;

				lTreeItem = GetRootItem (ItemNameCharacter);
				if (lTreeItem != null)
				{
					lTreeItem.IsExpanded = lSettings.CharacterNodeExpanded;
					lTreeItem.IsSelected = true;
				}
				lTreeItem = GetRootItem (ItemNameAnimations);
				if (lTreeItem != null)
				{
					lTreeItem.IsExpanded = lSettings.AnimationsNodeExpanded;
				}
				lTreeItem = GetRootItem (ItemNameStates);
				if (lTreeItem != null)
				{
					lTreeItem.IsExpanded = lSettings.StatesNodeExpanded;
				}

				TreeViewMain.InvalidateVisual ();
			}
		}

		public void SaveExpansion ()
		{
			Settings lSettings = Settings.Default;
			TreeViewItem lTreeItem;

			lTreeItem = GetRootItem (ItemNameCharacter);
			if (lTreeItem != null)
			{
				lSettings.CharacterNodeExpanded = lTreeItem.IsExpanded;
			}
			lTreeItem = GetRootItem (ItemNameAnimations);
			if (lTreeItem != null)
			{
				lSettings.AnimationsNodeExpanded = lTreeItem.IsExpanded;
			}
			lTreeItem = GetRootItem (ItemNameStates);
			if (lTreeItem != null)
			{
				lSettings.StatesNodeExpanded = lTreeItem.IsExpanded;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void InitItemTags ()
		{
			TreeViewItem lTreeItem;

			lTreeItem = GetRootItem (ItemNameCharacter);
			if (lTreeItem != null)
			{
				lTreeItem.Tag = new ResolveCharacter ();

				foreach (TreeViewItem lSubItem in lTreeItem.Items)
				{
					if (lSubItem.Name == ItemNameWordBalloon)
					{
						lSubItem.Tag = new ResolveBalloon ();
					}
					else if (lSubItem.Name == ItemNameTTSMode)
					{
						lSubItem.Tag = new ResolveTts ();
					}
				}
			}
			lTreeItem = GetRootItem (ItemNameAnimations);
			if (lTreeItem != null)
			{
				lTreeItem.Tag = new ResolveCharacter ();
				(lTreeItem.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeAnimations;
			}
			lTreeItem = GetRootItem (ItemNameStates);
			if (lTreeItem != null)
			{
				lTreeItem.Tag = new ResolveCharacter ();
				(lTreeItem.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeStates;

				foreach (TreeViewItem lStateItem in lTreeItem.Items)
				{
					lStateItem.Tag = new ResolveState (lStateItem.Header.ToString ());
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public ResolvePart SelectedPart
		{
			get
			{
				return GetItemPart (TreeViewMain.SelectedItem as TreeViewItem);
			}
			set
			{
				SelectPartItem (value);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void ShowFilePart ()
		{
						InitItemTags ();
				ShowAnimationNames ();
				if (TreeViewMain.SelectedItem != null)
				{
					LogicalTreeHelper.BringIntoView (TreeViewMain.SelectedItem as TreeViewItem);
				}
}

		public void ShowAnimationNames ()
		{
			TreeViewItem lAnimationsItem = GetRootItem (ItemNameAnimations);
			TreeViewItem lSelectedItem = TreeViewMain.SelectedItem as TreeViewItem;

			if (lAnimationsItem != null)
			{
				List<TreeViewItem> lAnimationItems = new List<TreeViewItem> ();

				if (!IsPanelEmpty)
				{
					String[] lAnimations = CharacterFile.GetAnimationNames ();
					FileGestures lGestures = CharacterFile.Gestures;
					TreeViewItem lAnimationItem;

					foreach (String lAnimationName in lAnimations)
					{
						FileAnimation lAnimation = null;

						try
						{
							lAnimation = lGestures[lAnimationName];
						}
						catch
						{
						}
						if (lAnimation == null)
						{
							try
							{
								lAnimation = lGestures[lAnimationName.ToUpper ()];
							}
							catch
							{
							}
						}

						lAnimationItem = GetChildItem (lAnimationsItem, lAnimationName);
						if (lAnimationItem == null)
						{
							lAnimationsItem.Items.Add (lAnimationItem = new TreeViewItem ());
						}
						lAnimationItem.Header = lAnimationName;
						lAnimationItem.Tag = new ResolveAnimation (lAnimation);
						lAnimationItems.Add (lAnimationItem);

						ShowAnimationFrames (lAnimation, lAnimationItem, true);
					}
				}

				lAnimationsItem.Items.Clear ();
				foreach (TreeViewItem lTreeItem in lAnimationItems)
				{
					lAnimationsItem.Items.Add (lTreeItem);
				}
				TreeViewMain.InvalidateVisual ();
			}

			try
			{
				if (lSelectedItem != null)
				{
					lSelectedItem.IsSelected = true;
				}
				lSelectedItem = TreeViewMain.SelectedItem as TreeViewItem;
				if (lSelectedItem == null)
				{
					lSelectedItem = GetRootItem (ItemNameCharacter);
					if (lSelectedItem != null)
					{
						lSelectedItem.IsSelected = true;
					}
				}
			}
			catch
			{
			}
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeViewItem pAnimationItem)
		{
			ShowAnimationFrames (pAnimation, pAnimationItem, false);
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeViewItem pAnimationItem, Boolean pInUpdate)
		{
			if ((pAnimation != null) && (pAnimationItem != null))
			{
				int lFrameNdx;
				String lFrameName;
				TreeViewItem lFrameItem;
				TreeViewItem lFrameSubItem;

				if (!pInUpdate)
				{
					//TreeViewMain.BeginUpdate ();
				}

				foreach (FileAnimationFrame lFrame in pAnimation.Frames)
				{
					lFrameNdx = pAnimation.Frames.IndexOf (lFrame);
					lFrameName = Titles.Frame (lFrameNdx);
					lFrameItem = ((lFrameNdx < pAnimationItem.Items.Count) ? pAnimationItem.Items[lFrameNdx] : pAnimationItem.Items[pAnimationItem.Items.Add (new TreeViewItem ())]) as TreeViewItem;
					lFrameItem.Header = lFrameName;
					lFrameItem.Tag = new ResolveAnimationFrame (lFrame);

					lFrameSubItem = ((lFrameItem.Items.Count >= 1) ? lFrameItem.Items[0] : lFrameItem.Items[lFrameItem.Items.Add (new TreeViewItem ())]) as TreeViewItem;
					lFrameSubItem.Header = "Branching";
					lFrameSubItem.Tag = new ResolveAnimationFrame (lFrame);
					(lFrameSubItem.Tag as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeBranching;

					lFrameSubItem = ((lFrameItem.Items.Count >= 2) ? lFrameItem.Items[1] : lFrameItem.Items[lFrameItem.Items.Add (new TreeViewItem ())]) as TreeViewItem;
					lFrameSubItem.Header = "Overlays";
					lFrameSubItem.Tag = new ResolveAnimationFrame (lFrame);
					(lFrameSubItem.Tag as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeOverlays;
				}

				while (pAnimationItem.Items.Count > pAnimation.Frames.Count)
				{
					pAnimationItem.Items.RemoveAt (pAnimationItem.Items.Count - 1);
				}

				if (!pInUpdate)
				{
					//TreeViewMain.EndUpdate ();
					TreeViewMain.InvalidateVisual ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal TreeViewItem GetRootItem (String pItemName)
		{
			foreach (TreeViewItem lTreeItem in TreeViewMain.Items)
			{
				if (lTreeItem.Name == pItemName)
				{
					return lTreeItem;
				}
			}
			return null;
		}

		internal TreeViewItem GetChildItem (TreeViewItem pParentItem, String pItemHeader)
		{
			foreach (TreeViewItem lTreeItem in pParentItem.Items)
			{
				if (lTreeItem.Header.ToString () == pItemHeader)
				{
					return lTreeItem;
				}
			}
			return null;
		}

		public ResolvePart GetItemPart (TreeViewItem pItem)
		{
			if (pItem != null)
			{
				return pItem.Tag as ResolvePart;
			}
			return null;
		}

		public TreeViewItem GetPartItem (ResolvePart pPart)
		{
			TreeViewItem lPartItem = null;

			if (pPart != null)
			{
				foreach (TreeViewItem lTreeItem in TreeViewMain.Items)
				{
					if ((lPartItem = GetPartItem (lTreeItem, pPart)) != null)
					{
						break;
					}
				}
			}
			return lPartItem;
		}

		public TreeViewItem GetPartItem (TreeViewItem pItem, ResolvePart pPart)
		{
			TreeViewItem lObjectItem = null;

			if ((pPart != null) && (pItem != null))
			{
				ResolvePart lPart = pItem.Tag as ResolvePart;

				if ((lPart != null) && Object.ReferenceEquals (lPart.Part, pPart.Part))
				{
					if ((pPart is ResolveCharacter) && (lPart is ResolveCharacter))
					{
						if ((pPart as ResolveCharacter).Scope == (lPart as ResolveCharacter).Scope)
						{
							lObjectItem = pItem;
						}
					}
					else if ((pPart is ResolveAnimationFrame) && (lPart is ResolveAnimationFrame))
					{
						if ((pPart as ResolveAnimationFrame).Scope == (lPart as ResolveAnimationFrame).Scope)
						{
							lObjectItem = pItem;
						}
					}
					else
					{
						lObjectItem = pItem;
					}
				}
				if (lObjectItem == null)
				{
					foreach (TreeViewItem lTreeItem in pItem.Items)
					{
						if ((lObjectItem = GetPartItem (lTreeItem, pPart)) != null)
						{
							break;
						}
					}
				}
			}
			return lObjectItem;
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean SelectPartItem (ResolvePart pPart)
		{
			if (pPart != null)
			{
				TreeViewItem lPartItem = GetPartItem (pPart);

				if ((lPartItem != null) && (TreeViewMain.SelectedItem != lPartItem))
				{
					Boolean lWasShowing = PushIsPanelShowing (true);
					lPartItem.IsSelected = true;
					PopIsPanelShowing (lWasShowing);
					return true;
				}
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		protected override Boolean HandleEditDelete (EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
			}
			return false;
		}

		protected override Boolean HandleEditPaste (EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void RefreshAnimationName (FileAnimation pAnimation)
		{
			TreeViewItem lAnimationItem = GetPartItem (new ResolveAnimation (pAnimation));

			if (lAnimationItem != null)
			{
				lAnimationItem.Header = pAnimation.Name;
			}
		}

		protected void RefreshAnimationFrames (FileAnimation pAnimation)
		{
			TreeViewItem lAnimationItem = GetPartItem (new ResolveAnimation (pAnimation));

			if (lAnimationItem != null)
			{
				ShowAnimationFrames (pAnimation, lAnimationItem);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TreeViewMain_SelectedItemChanged (object sender, RoutedPropertyChangedEventArgs<object> e)
		{
			if (!IsPanelShowing && (Navigate != null))
			{
				try
				{
					Navigate (this, new AgentCharacterEditor.Navigation.NavigationEventArgs (SelectedPart));
				}
				catch
				{
				}
			}
		}

		#endregion
	}
}
