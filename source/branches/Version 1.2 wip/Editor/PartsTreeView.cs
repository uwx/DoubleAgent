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
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public class PartsTreeView : System.Windows.Forms.TreeView
	{
		public const String	NodeNameCharacter = "NodeCharacter";
		public const String	NodeNameWordBalloon = "NodeWordBalloon";
		public const String	NodeNameTTSMode = "NodeTTSMode";
		public const String	NodeNameAnimations = "NodeAnimations";
		public const String	NodeNameStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////

		public void ShowAnimationNames (CharacterFile pCharacterFile)
		{
			TreeNode	lAnimationsNode = GetRootNode (NodeNameAnimations);

			if (lAnimationsNode != null)
			{
				Boolean	lExpanded = lAnimationsNode.IsExpanded;

				BeginUpdate ();
				lAnimationsNode.Nodes.Clear ();

				if (pCharacterFile != null)
				{
					String[]		lAnimations = pCharacterFile.GetAnimationNames ();
					TreeNode		lAnimationNode;
					FileGestures	lGestures = pCharacterFile.Gestures;

					foreach (String lAnimationName in lAnimations)
					{
						FileAnimation	lAnimation = null;

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

						lAnimationNode = lAnimationsNode.Nodes.Add (lAnimationName);
						lAnimationNode.Tag = lAnimation;

						ShowAnimationFrames (lAnimation, lAnimationNode);
					}
				}

				if (lExpanded)
				{
					lAnimationsNode.Expand ();
				}
				EndUpdate ();
			}
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeNode pAnimationNode)
		{
			if ((pAnimation != null) && (pAnimationNode != null))
			{
				int			lFrameNdx;
				String		lFrameName;
				TreeNode	lFrameNode;
				TreeNode	lFrameSubNode;

				foreach (FileAnimationFrame lFrame in pAnimation.Frames)
				{
					lFrameNdx = pAnimation.Frames.IndexOf (lFrame);
					lFrameName = Global.TitleFrame (lFrameNdx);
					lFrameNode = (lFrameNdx < pAnimationNode.Nodes.Count) ? pAnimationNode.Nodes[lFrameNdx] : pAnimationNode.Nodes.Add (lFrameName);
					lFrameNode.Text = lFrameName;
					lFrameNode.Tag = lFrame;

					lFrameSubNode = (lFrameNode.Nodes.Count >= 1) ? lFrameNode.Nodes[0] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Branching";
					lFrameSubNode.Tag = Program.MainForm.PanelBranching;
					lFrameSubNode = (lFrameNode.Nodes.Count >= 2) ? lFrameNode.Nodes[1] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Overlays";
					lFrameSubNode.Tag = Program.MainForm.PanelOverlays;
				}

				while (pAnimationNode.Nodes.Count > pAnimation.Frames.Count)
				{
					pAnimationNode.Nodes.RemoveAt (pAnimationNode.Nodes.Count - 1);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public TreeNode GetRootNode (String pNodeName)
		{
			foreach (TreeNode lNode in Nodes)
			{
				if (lNode.Name == pNodeName)
				{
					return lNode;
				}
			}
			return null;
		}

		public PartsTreeSelection GetSelectedPart ()
		{
			return new PartsTreeSelection (SelectedNode);
		}

		///////////////////////////////////////////////////////////////////////////////

		public void LoadExpansion ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
			TreeNode			lNode;

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
				SelectedNode = lNode;
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
		}

		public void SaveExpansion ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
			TreeNode			lNode;

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
	}

	///////////////////////////////////////////////////////////////////////////////

	public class PartsTreeSelection
	{
		public CharacterFile		mCharacterFile = null;
		public FileAnimation		mAnimation = null;
		public FileAnimationFrame	mFrame = null;
		public String				mStateName = null;
		public UserControl			mPanel = null;

		public PartsTreeSelection (TreeNode pTreeNode)
		{
			mCharacterFile = Program.MainForm.CharacterFile;

			if (pTreeNode.Name == PartsTreeView.NodeNameCharacter)
			{
				mPanel = Program.MainForm.PanelCharacter;
			}
			else if (pTreeNode.Name == PartsTreeView.NodeNameWordBalloon)
			{
				mPanel = Program.MainForm.PanelBalloon;
			}
			else if (pTreeNode.Name == PartsTreeView.NodeNameTTSMode)
			{
				mPanel = Program.MainForm.PanelTts;
			}
			else if (pTreeNode.Name == PartsTreeView.NodeNameAnimations)
			{
				mPanel = Program.MainForm.PanelAnimations;
			}
			else if (pTreeNode.Parent != null)
			{
				if (pTreeNode.Parent.Parent != null)
				{
					if (pTreeNode.Parent.Parent.Parent != null)
					{
						if (pTreeNode.Parent.Parent.Parent.Name == PartsTreeView.NodeNameAnimations)
						{
							mAnimation = pTreeNode.Parent.Parent.Tag as FileAnimation;
							mFrame = pTreeNode.Parent.Tag as FileAnimationFrame;
							mPanel = pTreeNode.Tag as UserControl;
						}
					}
					else if (pTreeNode.Parent.Parent.Name == PartsTreeView.NodeNameAnimations)
					{
						mAnimation = pTreeNode.Parent.Tag as FileAnimation;
						mFrame = pTreeNode.Tag as FileAnimationFrame;
						mPanel = Program.MainForm.PanelFrame;
					}
				}
				else if (pTreeNode.Parent.Name == PartsTreeView.NodeNameAnimations)
				{
					mAnimation = pTreeNode.Tag as FileAnimation;
					mPanel = Program.MainForm.PanelAnimation;
				}
				else if (pTreeNode.Parent.Name == PartsTreeView.NodeNameStates)
				{
					mStateName = pTreeNode.Text;
					mPanel = Program.MainForm.PanelState;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public PartsTreeSelection (FileAnimation pAnimation)
		{
			mCharacterFile = Program.MainForm.CharacterFile;
			mAnimation = pAnimation;
			mPanel = Program.MainForm.PanelAnimation;
		}

		public PartsTreeSelection (FileAnimation pAnimation, FileAnimationFrame pFrame)
		{
			mCharacterFile = Program.MainForm.CharacterFile;
			mAnimation = pAnimation;
			mFrame = pFrame;
			mPanel = Program.MainForm.PanelFrame;
		}

		public PartsTreeSelection (String pStateName)
		{
			mCharacterFile = Program.MainForm.CharacterFile;
			mStateName = pStateName;
			mPanel = Program.MainForm.PanelState;
		}

		///////////////////////////////////////////////////////////////////////////////

		public TreeNode SelectedNode ()
		{
			TreeNode	lSelectedNode = null;

			if (mPanel == Program.MainForm.PanelCharacter)
			{
				lSelectedNode = Program.MainForm.TreeViewMain.GetRootNode (PartsTreeView.NodeNameCharacter);
			}
			else if (mPanel == Program.MainForm.PanelAnimations)
			{
				lSelectedNode = Program.MainForm.TreeViewMain.GetRootNode (PartsTreeView.NodeNameAnimations);
			}
			else if (mPanel == Program.MainForm.PanelState)
			{
				foreach (TreeNode lStateNode in Program.MainForm.TreeViewMain.GetRootNode (PartsTreeView.NodeNameStates).Nodes)
				{
					if (mStateName == lStateNode.Text)
					{
						lSelectedNode = lStateNode;
						break;
					}
				}
			}
			else if (mAnimation != null)
			{
				foreach (TreeNode lAnimationNode in Program.MainForm.TreeViewMain.GetRootNode (PartsTreeView.NodeNameAnimations).Nodes)
				{
					if (mAnimation == lAnimationNode.Tag as FileAnimation)
					{
						if (mPanel == Program.MainForm.PanelAnimation)
						{
							lSelectedNode = lAnimationNode;
						}
						else
						{
							foreach (TreeNode lFrameNode in lAnimationNode.Nodes)
							{
								if (mFrame == lFrameNode.Tag as FileAnimationFrame)
								{
									if (mPanel == Program.MainForm.PanelFrame)
									{
										lSelectedNode = lFrameNode;
									}
									else
									{
										foreach (TreeNode lSubNode in lAnimationNode.Nodes)
										{
											if (mPanel == lSubNode.Tag as UserControl)
											{
												lSelectedNode = lSubNode;
												break;
											}
										}
									}
									break;
								}
							}
						}
						break;
					}
				}
			}

			return lSelectedNode;
		}

		///////////////////////////////////////////////////////////////////////////////

		public void CanEdit (Global.EditEventArgs e)
		{
			if (this.HasNonEmptyBalloon)
			{
				e.CopyObjectTitle = Properties.Resources.TitleBalloon;
			}
			else if (this.HasNonEmptyTts)
			{
				e.CopyObjectTitle = Properties.Resources.TitleTts;
			}
			else if (mPanel == Program.MainForm.PanelFrame)
			{
				e.CopyObjectTitle = Global.QuotedTitle (Global.TitleFrameAnimation (mFrame));
				if (!Program.FileIsReadOnly)
				{
					e.CutObjectTitle = e.CopyObjectTitle;
					e.DeleteObjectTitle = e.CopyObjectTitle;
				}
			}
			else if (mPanel == Program.MainForm.PanelAnimation)
			{
				e.CopyObjectTitle = Global.TitleAnimation (mAnimation);
				if (!Program.FileIsReadOnly)
				{
					e.CutObjectTitle = e.CopyObjectTitle;
					e.DeleteObjectTitle = e.CopyObjectTitle;
				}
			}
			else if (!String.IsNullOrEmpty (mStateName))
			{
				if (this.HasNonEmptyState)
				{
					e.CopyObjectTitle = Global.TitleState (mStateName);
				}
				if (!Program.FileIsReadOnly && (e.PasteObject is KeyValuePair<String, String[]>))
				{
					try
					{
						KeyValuePair<String, String[]>	lState = (KeyValuePair<String, String[]>)e.PasteObject;
						e.PasteObjectTitle = Global.TitleState (lState.Key);
					}
					catch
					{
					}
				}
			}

			if (!Program.FileIsReadOnly && (e.PasteObject != null))
			{
				if (e.PasteObject is FileBalloon)
				{
					if (mPanel == Program.MainForm.PanelBalloon)
					{
						e.PasteObjectTitle = Properties.Resources.TitleBalloon;
					}
				}
				else if (e.PasteObject is FileTts)
				{
					if (mPanel == Program.MainForm.PanelTts)
					{
						e.PasteObjectTitle = Properties.Resources.TitleTts;
					}
				}
				else if (e.PasteObject is FileAnimationFrame)
				{
					if (mFrame != null)
					{
						e.PasteObjectTitle = e.PasteTypeTitle (mFrame, Global.TitleFrameAnimation (mFrame), Global.TitleFrame (e.PasteObject as FileAnimationFrame));
					}
					else if (mPanel == Program.MainForm.PanelAnimation)
					{
						e.PasteObjectTitle = e.PasteTypeTitle (null, Global.TitleFrame (e.PasteObject as FileAnimationFrame));
					}
				}
				else if (e.PasteObject is FileAnimation)
				{
					if (mAnimation != null)
					{
						e.PasteObjectTitle = e.PasteTypeTitle (mAnimation, Global.TitleAnimation (mAnimation), Global.TitleAnimation (e.PasteObject as FileAnimation));
					}
					else if (mPanel == Program.MainForm.PanelAnimations)
					{
						if (Program.MainForm.PanelAnimations.HasNewAnimationName () && !mCharacterFile.Gestures.Contains (Program.MainForm.PanelAnimations.GetNewAnimationName ()))
						{
							e.PasteObjectTitle = e.PasteTypeTitle (null, Global.QuotedTitle (Program.MainForm.PanelAnimations.GetNewAnimationName ()), Global.TitleAnimation (e.PasteObject as FileAnimation));
						}
						else
						{
							e.PasteTitle = null;
							e.PasteObjectTitle = Properties.Resources.EditPasteAnimation0;
						}
					}
				}
			}
		}

		public void EditCopy (Global.EditEventArgs e)
		{
			Object	lCopyObject = null;

			if (this.HasNonEmptyBalloon)
			{
				lCopyObject = mCharacterFile.Balloon;
			}
			else if (this.HasNonEmptyTts)
			{
				lCopyObject = mCharacterFile.Tts;
			}
			else if (mPanel == Program.MainForm.PanelFrame)
			{
				lCopyObject = mFrame;
			}
			else if (mPanel == Program.MainForm.PanelAnimation)
			{
				lCopyObject = mAnimation;
			}
			else if (this.HasNonEmptyState)
			{
				KeyValuePair <String, String[]>	lState = new KeyValuePair<String, String[]> (mStateName, mCharacterFile.States[mStateName]);
				lCopyObject = lState;
			}
			if (lCopyObject != null)
			{
				e.IsUsed = true;
				try
				{
					Clipboard.SetData (DataFormats.Serializable, lCopyObject);
				}
				catch
				{
				}
			}
		}

		public void EditCut (Global.EditEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				if (mPanel == Program.MainForm.PanelFrame)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, mFrame);
						Program.MainForm.PanelAnimation.DeleteSelectedFrame (mFrame, true);
					}
					catch
					{
					}
				}
				else if (mPanel == Program.MainForm.PanelAnimation)
				{
					e.IsUsed = true;
					try
					{
						Clipboard.SetData (DataFormats.Serializable, mAnimation);
						Program.MainForm.PanelAnimations.DeleteSelectedAnimation (mAnimation, true);
					}
					catch
					{
					}
				}
			}
		}

		public void EditDelete (Global.EditEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				if (mPanel == Program.MainForm.PanelFrame)
				{
					e.IsUsed = true;
					Program.MainForm.PanelAnimation.DeleteSelectedFrame (mFrame, false);
				}
				else if (mPanel == Program.MainForm.PanelAnimation)
				{
					e.IsUsed = true;
					Program.MainForm.PanelAnimations.DeleteSelectedAnimation (mAnimation, false);
				}
			}
		}

		public void EditPaste (Global.EditEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				if ((mPanel == Program.MainForm.PanelBalloon) && (e.PasteObject is FileBalloon))
				{
					Program.MainForm.PanelBalloon.PasteBalloon (e.PasteObject as FileBalloon);
				}
				else if ((mPanel == Program.MainForm.PanelTts) && (e.PasteObject is FileTts))
				{
					Program.MainForm.PanelTts.PasteTts (e.PasteObject as FileTts);
				}
				else if ((mPanel == Program.MainForm.PanelFrame) && (e.PasteObject is FileAnimationFrame))
				{
					Program.MainForm.PanelAnimation.PasteSelectedFrame (mFrame.Animation, mFrame, e.PasteObject as FileAnimationFrame);
				}
				else if ((mPanel == Program.MainForm.PanelAnimation) && (e.PasteObject is FileAnimationFrame))
				{
					Program.MainForm.PanelAnimation.PasteSelectedFrame (mAnimation, null, e.PasteObject as FileAnimationFrame);
				}
				else if ((mPanel == Program.MainForm.PanelAnimation) && (e.PasteObject is FileAnimation))
				{
					Program.MainForm.PanelAnimations.PasteSelectedAnimation (mAnimation, e.PasteObject as FileAnimation);
				}
				else if ((mPanel == Program.MainForm.PanelAnimations) && (e.PasteObject is FileAnimation))
				{
					Program.MainForm.PanelAnimations.PasteSelectedAnimation (null, e.PasteObject as FileAnimation);
				}
				else if (!String.IsNullOrEmpty (mStateName) && (e.PasteObject is KeyValuePair<String, String[]>))
				{
					try
					{
						KeyValuePair<String, String[]>	lState = (KeyValuePair<String, String[]>)e.PasteObject;
						e.PasteObjectTitle = Global.TitleState (lState.Key);
						Program.MainForm.PanelState.PasteStateAnimations (mStateName, lState.Value);
					}
					catch
					{
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected Boolean HasNonEmptyBalloon
		{
			get
			{
				return (mPanel == Program.MainForm.PanelBalloon) && ((mCharacterFile.Header.Style & CharacterStyle.CharStyleBalloon) != 0);
			}
		}

		protected Boolean HasNonEmptyTts
		{
			get
			{
				return (mPanel == Program.MainForm.PanelTts) && ((mCharacterFile.Header.Style & CharacterStyle.CharStyleTts) != 0);
			}
		}

		protected Boolean HasNonEmptyState
		{
			get
			{
				return (!String.IsNullOrEmpty (mStateName) && mCharacterFile.States.ContainsKey (mStateName) && (mCharacterFile.States[mStateName].Length > 0));
			}
		}
	}
}
