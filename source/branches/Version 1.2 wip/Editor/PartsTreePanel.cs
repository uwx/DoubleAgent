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
	public partial class PartsTreePanel : UserControl
	{
		private CharacterFile mCharacterFile = null;
		public const String NodeNameCharacter = "NodeCharacter";
		public const String NodeNameWordBalloon = "NodeWordBalloon";
		public const String NodeNameTTSMode = "NodeTTSMode";
		public const String NodeNameAnimations = "NodeAnimations";
		public const String NodeNameStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public PartsTreePanel ()
		{
			InitializeComponent ();

			if (Program.MainForm != null)
			{
				Program.MainForm.LoadConfig += new EventHandler (MainForm_LoadConfig);
				Program.MainForm.SaveConfig += new EventHandler (MainForm_SaveConfig);
				Program.MainForm.UpdateApplied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				Program.MainForm.CanEdit += new Global.CanEditEventHandler (MainForm_CanEdit);
				Program.MainForm.EditCopy += new Global.EditEventHandler (MainForm_EditCopy);
				Program.MainForm.EditCut += new Global.EditEventHandler (MainForm_EditCut);
				Program.MainForm.EditDelete += new Global.EditEventHandler (MainForm_EditDelete);
				Program.MainForm.EditPaste += new Global.EditEventHandler (MainForm_EditPaste);
			}
		}

		void MainForm_LoadConfig (object sender, EventArgs e)
		{
			LoadExpansion ();
		}

		void MainForm_SaveConfig (object sender, EventArgs e)
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
				InitNodeTags ();
				ShowAnimationNames ();
				if (TreeViewMain.SelectedNode != null)
				{
					TreeViewMain.SelectedNode.EnsureVisible ();
				}
			}
		}

		[System.ComponentModel.Browsable (false)]
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

		private Boolean IsEmpty
		{
			get
			{
				return (mCharacterFile == null);
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
					String[] lAnimations = mCharacterFile.GetAnimationNames ();
					FileGestures lGestures = mCharacterFile.Gestures;
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
			if (pPart != null)
			{
				return GetObjectNode (pPart.Part);
			}
			return null;
		}

		public TreeNode GetObjectNode (Object pObject)
		{
			TreeNode lObjectNode = null;

			if (pObject != null)
			{
				foreach (TreeNode lNode in TreeViewMain.Nodes)
				{
					if ((lObjectNode = GetObjectNode (lNode, pObject)) != null)
					{
						break;
					}
				}
			}
			return lObjectNode;
		}

		public TreeNode GetObjectNode (TreeNode pParentNode, Object pObject)
		{
			TreeNode lObjectNode = null;

			if ((pObject != null) && (pParentNode != null))
			{
				foreach (TreeNode lNode in pParentNode.Nodes)
				{
					ResolvePart lPart = lNode.Tag as ResolvePart;
					if ((lPart != null) && Object.ReferenceEquals (lPart.Part, pObject))
					{
						lObjectNode = lNode;
						break;
					}
					else if ((lObjectNode = GetObjectNode (lNode, pObject)) != null)
					{
						break;
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
				return SelectObjectNode (pPart.Part);
			}
			return false;
		}

		public Boolean SelectObjectNode (Object pObject)
		{
			TreeNode lObjectNode = GetObjectNode (pObject);
			if (lObjectNode != null)
			{
				TreeViewMain.SelectedNode = lObjectNode;
				return true;
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				AddDeleteAnimation lAddDeleteAnimation = sender as AddDeleteAnimation;
				UpdateAnimation lUpdateAnimation = sender as UpdateAnimation;
				AddDeleteAnimationFrame lAddDeleteFrame = sender as AddDeleteAnimationFrame;
				ReorderAnimationFrame lReorderFrame = sender as ReorderAnimationFrame;

				if (lAddDeleteAnimation != null)
				{
					ShowAnimationNames ();
				}
				else if ((lUpdateAnimation != null) && (lUpdateAnimation.NameChanged || lUpdateAnimation.ForClipboard))
				{
					TreeNode lAnimationNode = GetObjectNode (lUpdateAnimation.Target);

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
					TreeNode lAnimationNode = GetObjectNode (lAddDeleteFrame.Animation);

					if (lAnimationNode != null)
					{
						ShowAnimationFrames (lAddDeleteFrame.Animation, lAnimationNode);
					}
				}
				else if (lReorderFrame != null)
				{
					TreeNode lAnimationNode = GetObjectNode (lReorderFrame.Animation);

					if (lAnimationNode != null)
					{
						ShowAnimationFrames (lReorderFrame.Animation, lAnimationNode);
					}
				}

			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TreeViewMain_AfterSelect (object sender, TreeViewEventArgs e)
		{
			if (Navigate != null)
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
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		internal void MainForm_CanEdit (object sender, Global.CanEditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileBalloon)
				{
					e.CopyObjectTitle = Properties.Resources.TitleBalloon;
				}
				else if (lSelectedObject is FileTts)
				{
					e.CopyObjectTitle = Properties.Resources.TitleTts;
				}
				else if (lSelectedObject is FileAnimation)
				{
					e.CopyObjectTitle = Global.TitleAnimation (lSelectedObject as FileAnimation);
					if (!Program.FileIsReadOnly)
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					e.CopyObjectTitle = Global.TitleFrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ();
					if (!Program.FileIsReadOnly)
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				else if ((lSelectedPart is ResolveState) && (lSelectedObject != null))
				{
					e.CopyObjectTitle = Global.TitleState (lSelectedObject as String);
				}

				if (!Program.FileIsReadOnly && (e.PasteObject != null))
				{
					if ((e.PasteObject is FileBalloon) && (lSelectedPart is ResolveBalloon))
					{
						e.PasteObjectTitle = Properties.Resources.TitleBalloon;
					}
					else if ((e.PasteObject is FileTts) && (lSelectedPart is ResolveTts))
					{
						e.PasteObjectTitle = Properties.Resources.TitleTts;
					}
					else if (e.PasteObject is FileAnimation)
					{
						if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
						{
							e.PasteObjectTitle = e.PasteTypeTitle (lSelectedObject as FileAnimation, Global.TitleAnimation (lSelectedObject as FileAnimation), Global.TitleAnimation (e.PasteObject as FileAnimation));
						}
						else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
						{
							if (Program.MainForm.PanelAnimations.HasNewAnimationName () && !mCharacterFile.Gestures.Contains (Program.MainForm.PanelAnimations.GetNewAnimationName ()))
							{
								e.PasteObjectTitle = e.PasteTypeTitle (null, Program.MainForm.PanelAnimations.GetNewAnimationName ().Quoted (), Global.TitleAnimation (e.PasteObject as FileAnimation));
							}
							else
							{
								e.PasteTitle = null;
								e.PasteObjectTitle = Properties.Resources.EditPasteAnimation0;
							}
						}
					}
					else if (e.PasteObject is FileAnimationFrame)
					{
						if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
						{
							e.PasteObjectTitle = e.PasteTypeTitle (lSelectedObject as FileAnimationFrame, Global.TitleFrameAnimation (lSelectedObject as FileAnimationFrame), Global.TitleFrame (e.PasteObject as FileAnimationFrame));

						}
						else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
						{
							e.PasteObjectTitle = e.PasteTypeTitle (null, Global.TitleFrame (e.PasteObject as FileAnimationFrame));
						}
					}
					else if ((e.PasteObject is KeyValuePair<String, String[]>) && (lSelectedPart is ResolveState) && (lSelectedObject is String))
					{
						try
						{
							KeyValuePair<String, String[]> lState = (KeyValuePair<String, String[]>)e.PasteObject;
							e.PasteObjectTitle = Global.TitleState (lState.Key);
						}
						catch
						{
						}
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal void MainForm_EditCopy (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if ((lSelectedObject != null) && (lSelectedPart is ResolveState))
				{
					String lStateName = lSelectedObject as String;
					KeyValuePair<String, String[]> lState = new KeyValuePair<String, String[]> (lStateName, mCharacterFile.States[lStateName]);
					lSelectedObject = lState;
				}
				if (lSelectedObject != null)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
					}
					catch
					{
					}
				}
			}
		}

		internal void MainForm_EditCut (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
						Program.MainForm.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					}
					catch
					{
						System.Media.SystemSounds.Asterisk.Play ();
					}
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
						Program.MainForm.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					}
					catch
					{
						System.Media.SystemSounds.Asterisk.Play ();
					}
				}
			}
		}

		internal void MainForm_EditDelete (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					e.IsUsed = true;
					Program.MainForm.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					e.IsUsed = true;
					Program.MainForm.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal void MainForm_EditPaste (object sender, Global.EditEventArgs e)
		{
			if (!e.IsUsed && !IsEmpty && !Program.FileIsReadOnly && TreeViewMain.ContainsFocus)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if ((e.PasteObject is FileBalloon) && (lSelectedPart is ResolveBalloon))
				{
					e.IsUsed = true;
					Program.MainForm.PanelBalloon.PasteBalloon (e.PasteObject as FileBalloon);
				}
				else if ((e.PasteObject is FileTts) && (lSelectedPart is ResolveTts))
				{
					e.IsUsed = true;
					Program.MainForm.PanelTts.PasteTts (e.PasteObject as FileTts);
				}
				else if (e.PasteObject is FileAnimation)
				{
					if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						e.IsUsed = true;
						Program.MainForm.PanelAnimations.PasteSelectedAnimation (lSelectedObject as FileAnimation, e.PasteObject as FileAnimation);
					}
					else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
					{
						e.IsUsed = true;
						Program.MainForm.PanelAnimations.PasteSelectedAnimation (null, e.PasteObject as FileAnimation);
					}
				}
				else if (e.PasteObject is FileAnimationFrame)
				{
					if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
					{
						e.IsUsed = true;
						Program.MainForm.PanelAnimation.PasteSelectedFrame ((lSelectedObject as FileAnimationFrame).Animation, lSelectedObject as FileAnimationFrame, e.PasteObject as FileAnimationFrame);
					}
					else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						e.IsUsed = true;
						Program.MainForm.PanelAnimation.PasteSelectedFrame (lSelectedObject as FileAnimation, null, e.PasteObject as FileAnimationFrame);
					}
				}
				else if ((e.PasteObject is KeyValuePair<String, String[]>) && (lSelectedPart is ResolveState) && (lSelectedObject is String))
				{
					try
					{
						e.IsUsed = true;
						KeyValuePair<String, String[]> lState = (KeyValuePair<String, String[]>)e.PasteObject;
						Program.MainForm.PanelState.PasteStateAnimations (lSelectedObject as String, lState.Value);
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
