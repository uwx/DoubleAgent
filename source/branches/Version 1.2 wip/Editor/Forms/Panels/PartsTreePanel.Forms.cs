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
using System.Windows.Forms;
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
				TreeNode lNode;

				TreeViewMain.BeginUpdate ();

				lNode = GetRootNode (ItemNameCharacter);
				if (lNode != null)
				{
					if (lSettings.CharacterNodeExpanded)
					{
						lNode.Expand ();
					}
					else
					{
						lNode.Collapse ();
					}
					TreeViewMain.SelectedNode = lNode;
				}
				lNode = GetRootNode (ItemNameAnimations);
				if (lNode != null)
				{
					if (lSettings.AnimationsNodeExpanded)
					{
						lNode.Expand ();
					}
					else
					{
						lNode.Collapse ();
					}
				}
				lNode = GetRootNode (ItemNameStates);
				if (lNode != null)
				{
					if (lSettings.StatesNodeExpanded)
					{
						lNode.Expand ();
					}
					else
					{
						lNode.Collapse ();
					}
				}

				TreeViewMain.EndUpdate ();
			}
		}

		public void SaveExpansion ()
		{
			Settings lSettings = Settings.Default;
			TreeNode lNode;

			lNode = GetRootNode (ItemNameCharacter);
			if (lNode != null)
			{
				lSettings.CharacterNodeExpanded = lNode.IsExpanded;
			}
			lNode = GetRootNode (ItemNameAnimations);
			if (lNode != null)
			{
				lSettings.AnimationsNodeExpanded = lNode.IsExpanded;
			}
			lNode = GetRootNode (ItemNameStates);
			if (lNode != null)
			{
				lSettings.StatesNodeExpanded = lNode.IsExpanded;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void InitNodeTags ()
		{
			TreeNode lNode;

			lNode = GetRootNode (ItemNameCharacter);
			if (lNode != null)
			{
				lNode.Tag = new ResolveCharacter ();

				foreach (TreeNode lSubNode in lNode.Nodes)
				{
					if (lSubNode.Name == ItemNameWordBalloon)
					{
						lSubNode.Tag = new ResolveBalloon ();
					}
					else if (lSubNode.Name == ItemNameTTSMode)
					{
						lSubNode.Tag = new ResolveTts ();
					}
				}
			}
			lNode = GetRootNode (ItemNameAnimations);
			if (lNode != null)
			{
				lNode.Tag = new ResolveCharacter ();
				(lNode.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeAnimations;
			}
			lNode = GetRootNode (ItemNameStates);
			if (lNode != null)
			{
				lNode.Tag = new ResolveCharacter ();
				(lNode.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeStates;

				foreach (TreeNode lStateNode in lNode.Nodes)
				{
					lStateNode.Tag = new ResolveState (lStateNode.Text);
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
				return GetNodePart (TreeViewMain.SelectedNode);
			}
			set
			{
				SelectPartNode (value);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void ShowFilePart ()
		{
			InitNodeTags ();
			ShowAnimationNames ();
			if (TreeViewMain.SelectedNode != null)
			{
				TreeViewMain.SelectedNode.EnsureVisible ();
			}
		}

		public void ShowAnimationNames ()
		{
			TreeNode lAnimationsNode = GetRootNode (ItemNameAnimations);
			TreeNode lSelectedNode = TreeViewMain.SelectedNode;

			if (lAnimationsNode != null)
			{
				List<TreeNode> lAnimationNodes = new List<TreeNode> ();

				TreeViewMain.BeginUpdate ();

				if (!IsPanelEmpty)
				{
					String[] lAnimations = CharacterFile.GetAnimationNames ();
					FileGestures lGestures = CharacterFile.Gestures;
					TreeNode lAnimationNode;

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

						if (lAnimationsNode.Nodes.ContainsKey (lAnimationName))
						{
							lAnimationNode = lAnimationsNode.Nodes[lAnimationName];
							lAnimationNode.Text = lAnimationName;
						}
						else
						{
							lAnimationNode = lAnimationsNode.Nodes.Add (lAnimationName, lAnimationName);
						}
						lAnimationNode.Tag = new ResolveAnimation (lAnimation);
						lAnimationNodes.Add (lAnimationNode);

						ShowAnimationFrames (lAnimation, lAnimationNode, true);
					}
				}

				lAnimationsNode.Nodes.Clear ();
				lAnimationsNode.Nodes.AddRange (lAnimationNodes.ToArray ());

				TreeViewMain.EndUpdate ();
			}

			try
			{
				TreeViewMain.SelectedNode = lSelectedNode;
			}
			catch
			{
			}
			if (TreeViewMain.SelectedNode == null)
			{
				TreeViewMain.SelectedNode = GetRootNode (ItemNameCharacter);
			}
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeNode pAnimationNode)
		{
			ShowAnimationFrames (pAnimation, pAnimationNode, false);
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeNode pAnimationNode, Boolean pInUpdate)
		{
			if ((pAnimation != null) && (pAnimationNode != null))
			{
				int lFrameNdx;
				String lFrameName;
				TreeNode lFrameNode;
				TreeNode lFrameSubNode;

				if (!pInUpdate)
				{
					TreeViewMain.BeginUpdate ();
				}

				foreach (FileAnimationFrame lFrame in pAnimation.Frames)
				{
					lFrameNdx = pAnimation.Frames.IndexOf (lFrame);
					lFrameName = Titles.Frame (lFrameNdx);
					lFrameNode = (lFrameNdx < pAnimationNode.Nodes.Count) ? pAnimationNode.Nodes[lFrameNdx] : pAnimationNode.Nodes.Add (lFrameName);
					lFrameNode.Text = lFrameName;
					lFrameNode.Tag = new ResolveAnimationFrame (lFrame);

					lFrameSubNode = (lFrameNode.Nodes.Count >= 1) ? lFrameNode.Nodes[0] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Branching";
					lFrameSubNode.Tag = new ResolveAnimationFrame (lFrame);
					(lFrameSubNode.Tag as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeBranching;

					lFrameSubNode = (lFrameNode.Nodes.Count >= 2) ? lFrameNode.Nodes[1] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Overlays";
					lFrameSubNode.Tag = new ResolveAnimationFrame (lFrame);
					(lFrameSubNode.Tag as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeOverlays;
				}

				while (pAnimationNode.Nodes.Count > pAnimation.Frames.Count)
				{
					pAnimationNode.Nodes.RemoveAt (pAnimationNode.Nodes.Count - 1);
				}

				if (!pInUpdate)
				{
					TreeViewMain.EndUpdate ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal TreeNode GetRootNode (String pNodeName)
		{
			foreach (TreeNode lNode in TreeViewMain.Nodes)
			{
				if (lNode.Name == pNodeName)
				{
					return lNode;
				}
			}
			return null;
		}

		public ResolvePart GetNodePart (TreeNode pNode)
		{
			if (pNode != null)
			{
				return pNode.Tag as ResolvePart;
			}
			return null;
		}

		public TreeNode GetPartNode (ResolvePart pPart)
		{
			TreeNode lPartNode = null;

			if (pPart != null)
			{
				foreach (TreeNode lNode in TreeViewMain.Nodes)
				{
					if ((lPartNode = GetPartNode (lNode, pPart)) != null)
					{
						break;
					}
				}
			}
			return lPartNode;
		}

		public TreeNode GetPartNode (TreeNode pNode, ResolvePart pPart)
		{
			TreeNode lObjectNode = null;

			if ((pPart != null) && (pNode != null))
			{
				ResolvePart lPart = pNode.Tag as ResolvePart;

				if ((lPart != null) && Object.ReferenceEquals (lPart.Part, pPart.Part))
				{
					if ((pPart is ResolveCharacter) && (lPart is ResolveCharacter))
					{
						if ((pPart as ResolveCharacter).Scope == (lPart as ResolveCharacter).Scope)
						{
							lObjectNode = pNode;
						}
					}
					else if ((pPart is ResolveAnimationFrame) && (lPart is ResolveAnimationFrame))
					{
						if ((pPart as ResolveAnimationFrame).Scope == (lPart as ResolveAnimationFrame).Scope)
						{
							lObjectNode = pNode;
						}
					}
					else
					{
						lObjectNode = pNode;
					}
				}
				if (lObjectNode == null)
				{
					foreach (TreeNode lNode in pNode.Nodes)
					{
						if ((lObjectNode = GetPartNode (lNode, pPart)) != null)
						{
							break;
						}
					}
				}
			}
			return lObjectNode;
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean SelectPartNode (ResolvePart pPart)
		{
			if (pPart != null)
			{
				TreeNode lPartNode = GetPartNode (pPart);

				if ((lPartNode != null) && (TreeViewMain.SelectedNode != lPartNode))
				{
					Boolean lWasShowing = PushIsPanelShowing (true);
					TreeViewMain.SelectedNode = lPartNode;
					PopIsPanelShowing (lWasShowing);
					return true;
				}
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		{
			if (TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileBalloon)
				{
					pEventArgs.PutCopyTitle (AppResources.Resources.TitleBalloon);
				}
				else if (lSelectedObject is FileTts)
				{
					pEventArgs.PutCopyTitle (AppResources.Resources.TitleTts);
				}
				else if (lSelectedObject is FileAnimation)
				{
					String lObjectTitle = Titles.Animation (lSelectedObject as FileAnimation);

					pEventArgs.PutCopyTitle (lObjectTitle);
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.PutCutTitle (lObjectTitle);
						pEventArgs.PutDeleteTitle (lObjectTitle);
					}
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					String lObjectTitle = Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ();

					pEventArgs.PutCopyTitle (lObjectTitle);
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.PutCutTitle (lObjectTitle);
						pEventArgs.PutDeleteTitle (lObjectTitle);
					}
				}
				else if ((lSelectedPart is ResolveState) && (lSelectedObject != null))
				{
					pEventArgs.PutCopyTitle (Titles.State ((lSelectedPart as ResolveState).StateName));
				}

				if (!Program.FileIsReadOnly && (pEventArgs.GetPasteObject () != null))
				{
					if ((pEventArgs.GetPasteObject () is FileBalloon) && (lSelectedPart is ResolveBalloon))
					{
						pEventArgs.PasteObjectTitle = AppResources.Resources.TitleBalloon;
					}
					else if ((pEventArgs.GetPasteObject () is FileTts) && (lSelectedPart is ResolveTts))
					{
						pEventArgs.PasteObjectTitle = AppResources.Resources.TitleTts;
					}
					else if (pEventArgs.GetPasteObject () is FileAnimation)
					{
						if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
						{
							pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lSelectedObject as FileAnimation, Titles.Animation (lSelectedObject as FileAnimation), Titles.Animation (pEventArgs.GetPasteObject () as FileAnimation));
						}
						else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
						{
							if (Program.MainWindow.PanelAnimations.HasNewAnimationName () && !CharacterFile.Gestures.Contains (Program.MainWindow.PanelAnimations.GetNewAnimationName ()))
							{
								pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (null, Program.MainWindow.PanelAnimations.GetNewAnimationName ().Quoted (), Titles.Animation (pEventArgs.GetPasteObject () as FileAnimation));
							}
							else
							{
								pEventArgs.PasteTypeTitle = null;
								pEventArgs.PasteObjectTitle = AppResources.Resources.EditPasteAnimation0;
							}
						}
					}
					else if (pEventArgs.GetPasteObject () is FileAnimationFrame)
					{
						if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
						{
							pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (lSelectedObject as FileAnimationFrame, Titles.FrameAnimation (lSelectedObject as FileAnimationFrame), Titles.Frame (pEventArgs.GetPasteObject () as FileAnimationFrame));

						}
						else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
						{
							pEventArgs.PasteObjectTitle = Titles.PasteTypeTitle (null, Titles.Frame (pEventArgs.GetPasteObject () as FileAnimationFrame));
						}
					}
					else if ((pEventArgs.GetPasteObject () is FileState) && (lSelectedPart is ResolveState))
					{
						pEventArgs.PasteObjectTitle = Titles.State (pEventArgs.GetPasteObject () as FileState);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					if (pEventArgs.PutCopyObject (lSelectedObject))
					{
						Program.MainWindow.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					}
					return true;
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					if (pEventArgs.PutCopyObject (lSelectedObject))
					{
						Program.MainWindow.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					}
					return true;
				}
			}
			return false;
		}

		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					Program.MainWindow.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					return true;
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					Program.MainWindow.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if ((pEventArgs.GetPasteObject () is FileBalloon) && (lSelectedPart is ResolveBalloon))
				{
					Program.MainWindow.PanelBalloon.PasteBalloon (pEventArgs.GetPasteObject () as FileBalloon);
					return true;
				}
				else if ((pEventArgs.GetPasteObject () is FileTts) && (lSelectedPart is ResolveTts))
				{
					Program.MainWindow.PanelTts.PasteTts (pEventArgs.GetPasteObject () as FileTts);
					return true;
				}
				else if (pEventArgs.GetPasteObject () is FileAnimation)
				{
					if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						Program.MainWindow.PanelAnimations.PasteSelectedAnimation (lSelectedObject as FileAnimation, pEventArgs.GetPasteObject () as FileAnimation);
						return true;
					}
					else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
					{
						Program.MainWindow.PanelAnimations.PasteSelectedAnimation (null, pEventArgs.GetPasteObject () as FileAnimation);
						return true;
					}
				}
				else if (pEventArgs.GetPasteObject () is FileAnimationFrame)
				{
					if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
					{
						Program.MainWindow.PanelAnimation.PasteSelectedFrame ((lSelectedObject as FileAnimationFrame).Animation, lSelectedObject as FileAnimationFrame, pEventArgs.GetPasteObject () as FileAnimationFrame);
						return true;
					}
					else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						Program.MainWindow.PanelAnimation.PasteSelectedFrame (lSelectedObject as FileAnimation, null, pEventArgs.GetPasteObject () as FileAnimationFrame);
						return true;
					}
				}
				else if ((pEventArgs.GetPasteObject () is FileState) && (lSelectedPart is ResolveState))
				{
					Program.MainWindow.PanelState.PasteStateAnimations ((lSelectedPart as ResolveState).StateName, (pEventArgs.GetPasteObject () as FileState).AnimationNames);
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void RefreshAnimationName (FileAnimation pAnimation)
		{
			TreeNode lAnimationNode = GetPartNode (new ResolveAnimation (pAnimation));

			if (lAnimationNode != null)
			{
				lAnimationNode.Text = pAnimation.Name;
			}
		}

		protected void RefreshAnimationFrames (FileAnimation pAnimation)
		{
			TreeNode lAnimationNode = GetPartNode (new ResolveAnimation (pAnimation));

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (pAnimation, lAnimationNode);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TreeViewMain_AfterSelect (object sender, TreeViewEventArgs e)
		{
			if (!IsPanelShowing && (Navigate != null))
			{
				try
				{
					Navigate (this, new NavigationEventArgs (SelectedPart));
				}
				catch
				{
				}
			}
		}

		#endregion
	}
}
