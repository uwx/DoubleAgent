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
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class PartsTreePanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public const String NodeNameCharacter = "NodeCharacter";
		public const String NodeNameWordBalloon = "NodeWordBalloon";
		public const String NodeNameTTSMode = "NodeTTSMode";
		public const String NodeNameAnimations = "NodeAnimations";
		public const String NodeNameStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////

		public PartsTreePanel ()
		{
			InitializeComponent ();
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			LoadExpansion ();
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			SaveExpansion ();
		}

		///////////////////////////////////////////////////////////////////////////////

		public void LoadExpansion ()
		{
			Properties.Settings lSettings = Properties.Settings.Default;
			TreeNode lNode;

			TreeViewMain.BeginUpdate ();

			lNode = GetRootNode (NodeNameCharacter);
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
			lNode = GetRootNode (NodeNameAnimations);
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
			lNode = GetRootNode (NodeNameStates);
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

		public void SaveExpansion ()
		{
			Properties.Settings lSettings = Properties.Settings.Default;
			TreeNode lNode;

			lNode = GetRootNode (NodeNameCharacter);
			if (lNode != null)
			{
				lSettings.CharacterNodeExpanded = lNode.IsExpanded;
			}
			lNode = GetRootNode (NodeNameAnimations);
			if (lNode != null)
			{
				lSettings.AnimationsNodeExpanded = lNode.IsExpanded;
			}
			lNode = GetRootNode (NodeNameStates);
			if (lNode != null)
			{
				lSettings.StatesNodeExpanded = lNode.IsExpanded;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void InitNodeTags ()
		{
			TreeNode lNode;

			lNode = GetRootNode (NodeNameCharacter);
			if (lNode != null)
			{
				lNode.Tag = new ResolveCharacter ();

				foreach (TreeNode lSubNode in lNode.Nodes)
				{
					if (lSubNode.Name == NodeNameWordBalloon)
					{
						lSubNode.Tag = new ResolveBalloon ();
					}
					else if (lSubNode.Name == NodeNameTTSMode)
					{
						lSubNode.Tag = new ResolveTts ();
					}
				}
			}
			lNode = GetRootNode (NodeNameAnimations);
			if (lNode != null)
			{
				lNode.Tag = new ResolveCharacter ();
				(lNode.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeAnimations;
			}
			lNode = GetRootNode (NodeNameStates);
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

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;

				InitNodeTags ();
				ShowAnimationNames ();
				if (TreeViewMain.SelectedNode != null)
				{
					TreeViewMain.SelectedNode.EnsureVisible ();
				}
			}
		}

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
		#region Events

		public event Global.NavigationEventHandler Navigate;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void ShowAnimationNames ()
		{
			TreeNode lAnimationsNode = GetRootNode (NodeNameAnimations);
			TreeNode lSelectedNode = TreeViewMain.SelectedNode;

			if (lAnimationsNode != null)
			{
				List<TreeNode> lAnimationNodes = new List<TreeNode> ();

				TreeViewMain.BeginUpdate ();

				if (!IsEmpty)
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
				TreeViewMain.SelectedNode = GetRootNode (NodeNameCharacter);
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
					lFrameName = Global.TitleFrame (lFrameNdx);
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
					CausesValidation = false;
					TreeViewMain.SelectedNode = lPartNode;
					CausesValidation = Visible;
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
					pEventArgs.CopyObjectTitle = Properties.Resources.TitleBalloon;
				}
				else if (lSelectedObject is FileTts)
				{
					pEventArgs.CopyObjectTitle = Properties.Resources.TitleTts;
				}
				else if (lSelectedObject is FileAnimation)
				{
					pEventArgs.CopyObjectTitle = Global.TitleAnimation (lSelectedObject as FileAnimation);
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
						pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
					}
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					pEventArgs.CopyObjectTitle = Global.TitleFrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ();
					if (!Program.FileIsReadOnly)
					{
						pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
						pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
					}
				}
				else if ((lSelectedPart is ResolveState) && (lSelectedObject != null))
				{
					pEventArgs.CopyObjectTitle = Global.TitleState (lSelectedObject as String);
				}

				if (!Program.FileIsReadOnly && (pEventArgs.PasteObject != null))
				{
					if ((pEventArgs.PasteObject is FileBalloon) && (lSelectedPart is ResolveBalloon))
					{
						pEventArgs.PasteObjectTitle = Properties.Resources.TitleBalloon;
					}
					else if ((pEventArgs.PasteObject is FileTts) && (lSelectedPart is ResolveTts))
					{
						pEventArgs.PasteObjectTitle = Properties.Resources.TitleTts;
					}
					else if (pEventArgs.PasteObject is FileAnimation)
					{
						if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
						{
							pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lSelectedObject as FileAnimation, Global.TitleAnimation (lSelectedObject as FileAnimation), Global.TitleAnimation (pEventArgs.PasteObject as FileAnimation));
						}
						else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
						{
							if (Program.MainForm.PanelAnimations.HasNewAnimationName () && !CharacterFile.Gestures.Contains (Program.MainForm.PanelAnimations.GetNewAnimationName ()))
							{
								pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (null, Program.MainForm.PanelAnimations.GetNewAnimationName ().Quoted (), Global.TitleAnimation (pEventArgs.PasteObject as FileAnimation));
							}
							else
							{
								pEventArgs.PasteTitle = null;
								pEventArgs.PasteObjectTitle = Properties.Resources.EditPasteAnimation0;
							}
						}
					}
					else if (pEventArgs.PasteObject is FileAnimationFrame)
					{
						if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
						{
							pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (lSelectedObject as FileAnimationFrame, Global.TitleFrameAnimation (lSelectedObject as FileAnimationFrame), Global.TitleFrame (pEventArgs.PasteObject as FileAnimationFrame));

						}
						else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
						{
							pEventArgs.PasteObjectTitle = pEventArgs.PasteTypeTitle (null, Global.TitleFrame (pEventArgs.PasteObject as FileAnimationFrame));
						}
					}
					else if ((pEventArgs.PasteObject is FileState) && (lSelectedPart is ResolveState))
					{
						pEventArgs.PasteObjectTitle = Global.TitleState (pEventArgs.PasteObject as FileState);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool EditCopy (Global.EditEventArgs pEventArgs)
		{
			if (TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject != null)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
					}
					catch
					{
					}
					return true;
				}
			}
			return false;
		}

		protected override bool EditCut (Global.EditEventArgs pEventArgs)
		{
			if (TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
						Program.MainForm.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					}
					catch
					{
						System.Media.SystemSounds.Asterisk.Play ();
					}
					return true;
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
						Program.MainForm.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					}
					catch
					{
						System.Media.SystemSounds.Asterisk.Play ();
					}
					return true;
				}
			}
			return false;
		}

		protected override bool EditDelete (Global.EditEventArgs pEventArgs)
		{
			if (TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					Program.MainForm.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					return true;
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					Program.MainForm.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool EditPaste (Global.EditEventArgs pEventArgs)
		{
			if (TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if ((pEventArgs.PasteObject is FileBalloon) && (lSelectedPart is ResolveBalloon))
				{
					Program.MainForm.PanelBalloon.PasteBalloon (pEventArgs.PasteObject as FileBalloon);
					return true;
				}
				else if ((pEventArgs.PasteObject is FileTts) && (lSelectedPart is ResolveTts))
				{
					Program.MainForm.PanelTts.PasteTts (pEventArgs.PasteObject as FileTts);
					return true;
				}
				else if (pEventArgs.PasteObject is FileAnimation)
				{
					if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						Program.MainForm.PanelAnimations.PasteSelectedAnimation (lSelectedObject as FileAnimation, pEventArgs.PasteObject as FileAnimation);
						return true;
					}
					else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
					{
						Program.MainForm.PanelAnimations.PasteSelectedAnimation (null, pEventArgs.PasteObject as FileAnimation);
						return true;
					}
				}
				else if (pEventArgs.PasteObject is FileAnimationFrame)
				{
					if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
					{
						Program.MainForm.PanelAnimation.PasteSelectedFrame ((lSelectedObject as FileAnimationFrame).Animation, lSelectedObject as FileAnimationFrame, pEventArgs.PasteObject as FileAnimationFrame);
						return true;
					}
					else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						Program.MainForm.PanelAnimation.PasteSelectedFrame (lSelectedObject as FileAnimation, null, pEventArgs.PasteObject as FileAnimationFrame);
						return true;
					}
				}
				else if ((pEventArgs.PasteObject is FileState) && (lSelectedPart is ResolveState))
				{
					Program.MainForm.PanelState.PasteStateAnimations ((lSelectedPart as ResolveState).StateName, (pEventArgs.PasteObject as FileState).AnimationNames);
					return true;
				}
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			AddDeleteAnimation lAddDeleteAnimation = pUpdate as AddDeleteAnimation;
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			AddDeleteAnimationFrame lAddDeleteFrame = pUpdate as AddDeleteAnimationFrame;
			ReorderAnimationFrame lReorderFrame = pUpdate as ReorderAnimationFrame;

			if (lAddDeleteAnimation != null)
			{
				ShowAnimationNames ();
			}
			else if ((lUpdateAnimation != null) && (lUpdateAnimation.NameChanged || lUpdateAnimation.ForClipboard))
			{
				TreeNode lAnimationNode = GetPartNode (new ResolveAnimation (lUpdateAnimation.Target));

				if (lAnimationNode != null)
				{
					if (lUpdateAnimation.NameChanged)
					{
						lAnimationNode.Text = lUpdateAnimation.Target.Name;
					}
					if (lUpdateAnimation.ForClipboard)
					{
						ShowAnimationFrames (lUpdateAnimation.Target, lAnimationNode);
					}
				}
			}
			else if (lAddDeleteFrame != null)
			{
				TreeNode lAnimationNode = GetPartNode (new ResolveAnimation (lAddDeleteFrame.Animation));

				if (lAnimationNode != null)
				{
					ShowAnimationFrames (lAddDeleteFrame.Animation, lAnimationNode);
				}
			}
			else if (lReorderFrame != null)
			{
				TreeNode lAnimationNode = GetPartNode (new ResolveAnimation (lReorderFrame.Animation));

				if (lAnimationNode != null)
				{
					ShowAnimationFrames (lReorderFrame.Animation, lAnimationNode);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TreeViewMain_AfterSelect (object sender, TreeViewEventArgs e)
		{
			if (CausesValidation && (Navigate != null))
			{
				try
				{
					Navigate (this, new Global.NavigationEventArgs (SelectedPart));
				}
				catch
				{
				}
			}
		}

		#endregion
	}
}
