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
using System.IO;
using System.Xml;
using System.Xml.Serialization;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class MainForm : Form
	{
		private CharacterFile	mCharacterFile = null;
		private RecentFileList	mRecentFiles = new RecentFileList ();
		private UndoManager		mUndoManager = new UndoManager ();
		private String			mInitialFile = null;
		private const String	mNodeCharacter = "NodeCharacter";
		private const String	mNodeWordBalloon = "NodeWordBalloon";
		private const String	mNodeTTSMode = "NodeTTSMode";
		private const String	mNodeAnimations = "NodeAnimations";
		private const String	mNodeStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public MainForm ()
		{
			InitializeComponent ();
		}

		public MainForm (String[] args)
		{
			Singleton = this;

			if ((args != null) && (args.Length > 0))
			{
				mInitialFile = args[0];
			}
			InitializeComponent ();

			if (!this.DesignMode)
			{
				MainMenuStrip = MenuStripMain;
				Application.Idle += new EventHandler (Application_Idle);
				mRecentFiles.RecentItemClick += new RecentFileList.RecentItemClickEventHandler (RecentFiles_RecentItemClick);
				mUndoManager.Undone += new UndoManager.UndoneEventHandler (UndoManager_Undone);
				mUndoManager.Redone += new UndoManager.RedoneEventHandler (UndoManager_Redone);

				InitializationComplete (this, EventArgs.Empty);
			}
		}

		~MainForm ()
		{
			Application.Idle -= new EventHandler (Application_Idle);
			Singleton = null;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void LoadAllConfig ()
		{
			LoadConfig (this, EventArgs.Empty);
			LoadRecentFiles ();
			LoadTreeExpansion ();
			LoadWindowState ();
		}

		private void LoadWindowState ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;

			if ((lSettings.MainFormSize.X > 0) && (lSettings.MainFormSize.Y > 0))
			{
				this.Location = lSettings.MainFormLocation;
				this.Size = (System.Drawing.Size)lSettings.MainFormSize;
			}
			this.WindowState = lSettings.MainFormState;

			if (lSettings.MainFormSplit > 0)
			{
				SplitContainerMain.SplitterDistance = lSettings.MainFormSplit;
			}
		}

		private void LoadTreeExpansion ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
			TreeNode			lNode;

			lNode = RootNode (mNodeCharacter);
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
			lNode = RootNode (mNodeAnimations);
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
			lNode = RootNode (mNodeStates);
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

		private void LoadRecentFiles ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;

			mRecentFiles.Clear ();
			if (lSettings.RecentFiles != null)
			{
				foreach (String lPath in lSettings.RecentFiles)
				{
					mRecentFiles.Add (lPath);
#if DEBUG_NOT
					System.Diagnostics.Debug.Print ("Load Recent [{0}]", lPath);
#endif
				}
			}
			mRecentFiles.ShowPaths (MenuItemFile.DropDown, MenuItemFilePrintPreview);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void SaveAllConfig ()
		{
			SaveConfig (this, EventArgs.Empty);
			SaveTreeExpansion ();
			SaveWindowState ();
			SaveRecentFiles ();

			Properties.Settings.Default.Save ();
		}

		private void SaveWindowState ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;

			if (this.WindowState == FormWindowState.Normal)
			{
				lSettings.MainFormLocation = this.Location;
				lSettings.MainFormSize = (System.Drawing.Point)this.Size;
			}
			else
			{
				lSettings.MainFormLocation = (System.Drawing.Point)this.RestoreBounds.Location;
				lSettings.MainFormSize = (System.Drawing.Point)this.RestoreBounds.Size;
			}
			lSettings.MainFormState = this.WindowState;

			lSettings.MainFormSplit = SplitContainerMain.SplitterDistance;
		}

		private void SaveTreeExpansion ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
			TreeNode			lNode;

			lNode = RootNode (mNodeCharacter);
			if (lNode != null)
			{
				lSettings.CharacterNodeExpanded = lNode.IsExpanded;
			}
			lNode = RootNode (mNodeAnimations);
			if (lNode != null)
			{
				lSettings.AnimationsNodeExpanded = lNode.IsExpanded;
			}
			lNode = RootNode (mNodeStates);
			if (lNode != null)
			{
				lSettings.StatesNodeExpanded = lNode.IsExpanded;
			}
		}

		private void SaveRecentFiles ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;

			lSettings.RecentFiles = new System.Collections.Specialized.StringCollection ();
			foreach (String lPath in mRecentFiles)
			{
				lSettings.RecentFiles.Add (lPath);
#if DEBUG_NOT
				System.Diagnostics.Debug.Print ("Save Recent [{0}]", lPath);
#endif
			}
		}

		private void UpdateRecentFiles ()
		{
			if ((mCharacterFile != null) && (!String.IsNullOrEmpty (mCharacterFile.Path)))
			{
				mRecentFiles.MostRecent = mCharacterFile.Path;
				mRecentFiles.ShowPaths (MenuItemFile.DropDown, MenuItemFilePrintPreview);
				SaveRecentFiles ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		static public MainForm Singleton
		{
			get;
			private set;
		}

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
		}

		public Boolean FileIsReadOnly
		{
			get
			{
				return ((mCharacterFile == null) || (mCharacterFile.IsReadOnly));
			}
		}

		public Boolean FileIsDirty
		{
			get
			{
				if (CharacterFile != null)
				{
					return CharacterFile.IsDirty;
				}
				return false;
			}
			set
			{
				if (CharacterFile != null)
				{
					CharacterFile.IsDirty = value;
					ShowFileState ();
					ShowEditState ();
				}
			}
		}

		public UndoManager UndoManager
		{
			get
			{
				return mUndoManager;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event EventHandler InitializationComplete;
		public event EventHandler LoadConfig;
		public event EventHandler SaveConfig;

		public event EditEventHandler CanEdit;
		public event EditEventHandler EditCopy;
		public event EditEventHandler EditCut;
		public event EditEventHandler EditDelete;
		public event EditEventHandler EditPaste;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private Boolean OpenCharacterFile (String pFilePath)
		{
			Boolean			lRet = false;
			CursorState		lCursorState = new CursorState (this);
			CharacterFile	lCharacterFile = null;
			Boolean			lOpened = false;

			try
			{
				if ((mCharacterFile != null) && (String.Compare (System.IO.Path.GetFullPath (pFilePath), System.IO.Path.GetFullPath (mCharacterFile.Path), true) == 0))
				{
					return true;
				}
			}
			catch
			{
			}

			lCursorState.ShowWait ();
			if (mCharacterFile != null)
			{
				SaveAllConfig ();
			}

			try
			{
				lCharacterFile = CharacterFile.CreateInstance (pFilePath);
			}
			catch
			{
			}

			if (lCharacterFile != null)
			{
				try
				{
#if DEBUG_NOT
					lOpened = lCharacterFile.Open (pFilePath, 1);
#else
					lOpened = lCharacterFile.Open (pFilePath);
#endif
				}
				catch (Exception pException)
				{
					MessageBox.Show (pException.Message);
					lCursorState.RestoreCursor ();
					return false;
				}
			}

			try
			{
				if (lOpened)
				{
					mUndoManager.Clear ();
					mCharacterFile = lCharacterFile;

					ShowFileState ();
					ShowEditState ();
					ShowAnimationNames ();
					LoadTreeExpansion ();

					PanelCharacter.CharacterFile = mCharacterFile;
					PanelBalloon.CharacterFile = mCharacterFile;
					PanelTts.CharacterFile = mCharacterFile;
					PanelAnimations.CharacterFile = mCharacterFile;
					PanelAnimation.CharacterFile = mCharacterFile;
					PanelFrame.CharacterFile = mCharacterFile;
					PanelBranching.CharacterFile = mCharacterFile;
					PanelOverlays.CharacterFile = mCharacterFile;
					PanelState.CharacterFile = mCharacterFile;

					ShowSelectedTreeNode ();
					UpdateRecentFiles ();
					lRet = true;
				}
				else
				{
					MessageBox.Show (String.Format (Properties.Resources.MsgInvalidFile, pFilePath), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				}
			}
			catch
			{
			}

			lCursorState.RestoreCursor ();
			return lRet;
		}

		private void ShowAnimationNames ()
		{
			TreeNode	lAnimationsNode = RootNode (mNodeAnimations);

			if (lAnimationsNode != null)
			{
				Boolean	lExpanded = lAnimationsNode.IsExpanded;

				TreeViewMain.BeginUpdate ();
				lAnimationsNode.Nodes.Clear ();

				if (mCharacterFile != null)
				{
					String[]		lAnimations = mCharacterFile.GetAnimationNames ();
					TreeNode		lAnimationNode;
					FileGestures	lGestures = mCharacterFile.Gestures;

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
				TreeViewMain.EndUpdate ();
			}
		}

		private void ShowAnimationFrames (FileAnimation pAnimation, TreeNode pAnimationNode)
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
					lFrameName = Program.TitleFrame (lFrameNdx);
					lFrameNode = (lFrameNdx < pAnimationNode.Nodes.Count) ? pAnimationNode.Nodes[lFrameNdx] : pAnimationNode.Nodes.Add (lFrameName);
					lFrameNode.Text = lFrameName;
					lFrameNode.Tag = lFrame;

					lFrameSubNode = (lFrameNode.Nodes.Count >= 1) ? lFrameNode.Nodes[0] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Branching";
					lFrameSubNode.Tag = PanelBranching;
					lFrameSubNode = (lFrameNode.Nodes.Count >= 2) ? lFrameNode.Nodes[1] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Overlays";
					lFrameSubNode.Tag = PanelOverlays;
				}

				while (pAnimationNode.Nodes.Count > pAnimation.Frames.Count)
				{
					pAnimationNode.Nodes.RemoveAt (pAnimationNode.Nodes.Count - 1);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Tree Selection

		private TreeNode RootNode (String pNodeName)
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

		///////////////////////////////////////////////////////////////////////////////

		private class TreeSelection
		{
			public CharacterFile		mCharacterFile = null;
			public FileAnimation		mAnimation = null;
			public FileAnimationFrame	mFrame = null;
			public String				mStateName = null;
			public UserControl			mPanel = null;

			public TreeSelection (TreeNode pTreeNode)
			{
				mCharacterFile = MainForm.Singleton.CharacterFile;

				if (pTreeNode.Name == mNodeCharacter)
				{
					mPanel = MainForm.Singleton.PanelCharacter;
				}
				else if (pTreeNode.Name == mNodeWordBalloon)
				{
					mPanel = MainForm.Singleton.PanelBalloon;
				}
				else if (pTreeNode.Name == mNodeTTSMode)
				{
					mPanel = MainForm.Singleton.PanelTts;
				}
				else if (pTreeNode.Name == mNodeAnimations)
				{
					mPanel = MainForm.Singleton.PanelAnimations;
				}
				else if (pTreeNode.Parent != null)
				{
					if (pTreeNode.Parent.Parent != null)
					{
						if (pTreeNode.Parent.Parent.Parent != null)
						{
							if (pTreeNode.Parent.Parent.Parent.Name == mNodeAnimations)
							{
								mAnimation = pTreeNode.Parent.Parent.Tag as FileAnimation;
								mFrame = pTreeNode.Parent.Tag as FileAnimationFrame;
								mPanel = pTreeNode.Tag as UserControl;
							}
						}
						else if (pTreeNode.Parent.Parent.Name == mNodeAnimations)
						{
							mAnimation = pTreeNode.Parent.Tag as FileAnimation;
							mFrame = pTreeNode.Tag as FileAnimationFrame;
							mPanel = MainForm.Singleton.PanelFrame;
						}
					}
					else if (pTreeNode.Parent.Name == mNodeAnimations)
					{
						mAnimation = pTreeNode.Tag as FileAnimation;
						mPanel = MainForm.Singleton.PanelAnimation;
					}
					else if (pTreeNode.Parent.Name == mNodeStates)
					{
						mStateName = pTreeNode.Text;
						mPanel = MainForm.Singleton.PanelState;
					}
				}
			}

			public TreeNode SelectedNode ()
			{
				TreeNode	lSelectedNode = null;

				if (mPanel == MainForm.Singleton.PanelCharacter)
				{
					lSelectedNode = MainForm.Singleton.RootNode (MainForm.mNodeCharacter);
				}
				else if (mPanel == MainForm.Singleton.PanelAnimations)
				{
					lSelectedNode = MainForm.Singleton.RootNode (MainForm.mNodeAnimations);
				}
				else if (mPanel == MainForm.Singleton.PanelState)
				{
					foreach (TreeNode lStateNode in MainForm.Singleton.RootNode (MainForm.mNodeStates).Nodes)
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
					foreach (TreeNode lAnimationNode in MainForm.Singleton.RootNode (MainForm.mNodeAnimations).Nodes)
					{
						if (mAnimation == lAnimationNode.Tag as FileAnimation)
						{
							if (mPanel == MainForm.Singleton.PanelAnimation)
							{
								lSelectedNode = lAnimationNode;
							}
							else
							{
								foreach (TreeNode lFrameNode in lAnimationNode.Nodes)
								{
									if (mFrame == lFrameNode.Tag as FileAnimationFrame)
									{
										if (mPanel == MainForm.Singleton.PanelFrame)
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

			public TreeSelection (FileAnimation pAnimation)
			{
				mCharacterFile = MainForm.Singleton.CharacterFile;
				mAnimation = pAnimation;
				mPanel = MainForm.Singleton.PanelAnimation;
			}

			public TreeSelection (FileAnimation pAnimation, FileAnimationFrame pFrame)
			{
				mCharacterFile = MainForm.Singleton.CharacterFile;
				mAnimation = pAnimation;
				mFrame = pFrame;
				mPanel = MainForm.Singleton.PanelFrame;
			}

			public TreeSelection (String pStateName)
			{
				mCharacterFile = MainForm.Singleton.CharacterFile;
				mStateName = pStateName;
				mPanel = MainForm.Singleton.PanelState;
			}

			///////////////////////////////////////////////////////////////////////////////

			public void CanEdit (EditEventArgs e)
			{
				if (this.HasNonEmptyBalloon)
				{
					e.CopyObjectTitle = Properties.Resources.TitleBalloon;
				}
				else if (this.HasNonEmptyTts)
				{
					e.CopyObjectTitle = Properties.Resources.TitleTts;
				}
				else if (mPanel == MainForm.Singleton.PanelFrame)
				{
					e.CopyObjectTitle = Program.QuotedTitle (Program.TitleFrameAnimation (mFrame));
					if (!MainForm.Singleton.FileIsReadOnly)
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				else if (mPanel == MainForm.Singleton.PanelAnimation)
				{
					e.CopyObjectTitle = Program.TitleAnimation (mAnimation);
					if (!MainForm.Singleton.FileIsReadOnly)
					{
						e.CutObjectTitle = e.CopyObjectTitle;
						e.DeleteObjectTitle = e.CopyObjectTitle;
					}
				}
				else if (!String.IsNullOrEmpty (mStateName))
				{
					if (this.HasNonEmptyState)
					{
						e.CopyObjectTitle = Program.TitleState (mStateName);
					}
					if (!MainForm.Singleton.FileIsReadOnly && (e.PasteObject is KeyValuePair<String, String[]>))
					{
						try
						{
							KeyValuePair<String, String[]>	lState = (KeyValuePair<String, String[]>)e.PasteObject;
							e.PasteObjectTitle = Program.TitleState (lState.Key);
						}
						catch
						{
						}
					}
				}

				if (!MainForm.Singleton.FileIsReadOnly && (e.PasteObject != null))
				{
					if (e.PasteObject is FileBalloon)
					{
						if (mPanel == MainForm.Singleton.PanelBalloon)
						{
							e.PasteObjectTitle = Properties.Resources.TitleBalloon;
						}
					}
					else if (e.PasteObject is FileTts)
					{
						if (mPanel == MainForm.Singleton.PanelTts)
						{
							e.PasteObjectTitle = Properties.Resources.TitleTts;
						}
					}
					else if (e.PasteObject is FileAnimationFrame)
					{
						if (mPanel == MainForm.Singleton.PanelFrame)
						{
							e.PasteObjectTitle = e.PasteReplaceTitle + Program.TitleFrame (e.PasteObject as FileAnimationFrame);
						}
						else if (mPanel == MainForm.Singleton.PanelAnimation)
						{
							e.PasteObjectTitle = e.PasteAddTitle + Program.TitleFrame (e.PasteObject as FileAnimationFrame);
						}
					}
					else if (e.PasteObject is FileAnimation)
					{
						if (mAnimation != null)
						{
							e.PasteObjectTitle = e.PasteReplaceTitle + Program.TitleAnimation (e.PasteObject as FileAnimation);
						}
						else if (mPanel == MainForm.Singleton.PanelAnimations)
						{
							e.PasteObjectTitle = e.PasteAddTitle + Program.TitleAnimation (e.PasteObject as FileAnimation);
						}
					}
				}
			}

			public void EditCopy (EditEventArgs e)
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
				else if (mPanel == MainForm.Singleton.PanelFrame)
				{
					lCopyObject = mFrame;
				}
				else if (mPanel == MainForm.Singleton.PanelAnimation)
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

			public void EditCut (EditEventArgs e)
			{
				if (!MainForm.Singleton.FileIsReadOnly)
				{
					if (mPanel == MainForm.Singleton.PanelFrame)
					{
						e.IsUsed = true;
						try
						{
							Clipboard.SetData (DataFormats.Serializable, mFrame);
							MainForm.Singleton.PanelAnimation.DeleteSelectedFrame (mFrame, true);
						}
						catch
						{
						}
					}
					else if (mPanel == MainForm.Singleton.PanelAnimation)
					{
						e.IsUsed = true;
						try
						{
							Clipboard.SetData (DataFormats.Serializable, mAnimation);
							MainForm.Singleton.PanelAnimations.DeleteSelectedAnimation (mAnimation, true);
						}
						catch
						{
						}
					}
				}
			}

			public void EditDelete (EditEventArgs e)
			{
				if (!MainForm.Singleton.FileIsReadOnly)
				{
					if (mPanel == MainForm.Singleton.PanelFrame)
					{
						e.IsUsed = true;
						MainForm.Singleton.PanelAnimation.DeleteSelectedFrame (mFrame, false);
					}
					else if (mPanel == MainForm.Singleton.PanelAnimation)
					{
						e.IsUsed = true;
						MainForm.Singleton.PanelAnimations.DeleteSelectedAnimation (mAnimation, false);
					}
				}
			}

			public void EditPaste (EditEventArgs e)
			{
				if (!MainForm.Singleton.FileIsReadOnly)
				{
					if ((mPanel == MainForm.Singleton.PanelBalloon) && (e.PasteObject is FileBalloon))
					{
					}
					else if ((mPanel == MainForm.Singleton.PanelTts) && (e.PasteObject is FileTts))
					{
					}
					else if ((mPanel == MainForm.Singleton.PanelFrame) && (e.PasteObject is FileAnimationFrame))
					{
					}
					else if ((mPanel == MainForm.Singleton.PanelAnimation) && (e.PasteObject is FileAnimationFrame))
					{
					}
					else if ((mPanel == MainForm.Singleton.PanelAnimation) && (e.PasteObject is FileAnimation))
					{
					}
					else if ((mPanel == MainForm.Singleton.PanelAnimations) && (e.PasteObject is FileAnimation))
					{
					}
					else if (!String.IsNullOrEmpty (mStateName) && (e.PasteObject is KeyValuePair<String, String[]>))
					{
						try
						{
							KeyValuePair<String, String[]>	lState = (KeyValuePair<String, String[]>)e.PasteObject;
							e.PasteObjectTitle = Program.TitleState (lState.Key);
							MainForm.Singleton.PanelState.PasteStateAnimations (mStateName, lState.Value);
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
					return (mPanel == MainForm.Singleton.PanelBalloon) && ((mCharacterFile.Header.Style & CharacterStyle.CharStyleBalloon) != 0);
				}
			}

			protected Boolean HasNonEmptyTts
			{
				get
				{
					return (mPanel == MainForm.Singleton.PanelTts) && ((mCharacterFile.Header.Style & CharacterStyle.CharStyleTts) != 0);
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

		///////////////////////////////////////////////////////////////////////////////

		private void ShowSelectedTreeNode ()
		{
			if (TreeViewMain.SelectedNode == null)
			{
				TreeViewMain.SelectedNode = RootNode (mNodeCharacter);
			}
			if (TreeViewMain.SelectedNode != null)
			{
				try
				{
					ShowSelectedPart (new TreeSelection (TreeViewMain.SelectedNode));
				}
				finally
				{
					ResumeLayout (true);
				}
			}
		}

		private void ShowSelectedPart (TreeSelection pSelectedPart)
		{
			if (pSelectedPart.mPanel == PanelAnimation)
			{
				PanelAnimation.Animation = pSelectedPart.mAnimation;
			}
			else if (pSelectedPart.mPanel == PanelFrame)
			{
				PanelFrame.Frame = pSelectedPart.mFrame;
			}
			else if (pSelectedPart.mPanel == PanelBranching)
			{
				PanelBranching.Frame = pSelectedPart.mFrame;
			}
			else if (pSelectedPart.mPanel == PanelOverlays)
			{
				PanelOverlays.Frame = pSelectedPart.mFrame;
			}
			else if (pSelectedPart.mPanel == PanelState)
			{
				PanelState.StateName = pSelectedPart.mStateName;
			}

			PanelCharacter.Visible = (pSelectedPart.mPanel == PanelCharacter);
			PanelBalloon.Visible = (pSelectedPart.mPanel == PanelBalloon);
			PanelTts.Visible = (pSelectedPart.mPanel == PanelTts);
			PanelAnimations.Visible = (pSelectedPart.mPanel == PanelAnimations);
			PanelAnimation.Visible = (pSelectedPart.mPanel == PanelAnimation);
			PanelFrame.Visible = (pSelectedPart.mPanel == PanelFrame);
			PanelBranching.Visible = (pSelectedPart.mPanel == PanelBranching);
			PanelOverlays.Visible = (pSelectedPart.mPanel == PanelOverlays);
			PanelState.Visible = (pSelectedPart.mPanel == PanelState);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Editing State

		private void ShowFileState ()
		{
			if (mCharacterFile == null)
			{
				this.Text = Program.AssemblyTitle;
				MenuItemFileSave.Enabled = false;
				MenuItemFileSaveAs.Enabled = false;
				ToolButtonFileSave.Enabled = false;
			}
			else
			{
				this.Text = Program.AssemblyTitle + " [" + mCharacterFile.FileName + ((mCharacterFile.IsDirty) ? " *" : "") + "]";
				MenuItemFileSave.Enabled = mCharacterFile.IsDirty;
				MenuItemFileSaveAs.Enabled = true;
				ToolButtonFileSave.Enabled = mCharacterFile.IsDirty;
			}
		}

		private void ShowEditState ()
		{
			TextBox			lTextBox = null;
			Control			lActive = GetActiveControl (ref lTextBox);
			TreeSelection	lTreeSelection = null;

			if (lActive == TreeViewMain)
			{
				lTreeSelection = new TreeSelection (TreeViewMain.SelectedNode);
			}

			if (lTextBox != null)
			{
				MenuItemEditCopy.Enabled = (lTextBox.SelectionLength > 0);
				MenuItemEditCut.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
				MenuItemEditDelete.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
				MenuItemEditPaste.Enabled = !lTextBox.ReadOnly && (Clipboard.ContainsText (TextDataFormat.Text) || Clipboard.ContainsText (TextDataFormat.UnicodeText));

				MenuItemEditCopy.Text = MenuItemEditCopy.Enabled ? Properties.Resources.EditCopyText : Properties.Resources.EditCopy;
				MenuItemEditCut.Text = MenuItemEditCut.Enabled ? Properties.Resources.EditCutText : Properties.Resources.EditCut;
				MenuItemEditDelete.Text = MenuItemEditDelete.Enabled ? Properties.Resources.EditDeleteText : Properties.Resources.EditDelete;
				MenuItemEditPaste.Text = MenuItemEditPaste.Enabled ? Properties.Resources.EditPasteText : Properties.Resources.EditPaste;
			}
			else
			{
				EditEventArgs	lEventArgs = new EditEventArgs (DataFormats.Serializable);

				if (lTreeSelection != null)
				{
					lTreeSelection.CanEdit (lEventArgs);
				}
				else if (CanEdit != null)
				{
					CanEdit (this, lEventArgs);
				}

				if (!String.IsNullOrEmpty (lEventArgs.CopyObjectTitle))
				{
					MenuItemEditCopy.Enabled = true;
					MenuItemEditCopy.Text = String.Format (Properties.Resources.EditCopyThis, lEventArgs.CopyObjectTitle);
				}
				else
				{
					MenuItemEditCopy.Enabled = false;
					MenuItemEditCopy.Text = Properties.Resources.EditCopy;
				}
				if (!MainForm.Singleton.FileIsReadOnly && !String.IsNullOrEmpty (lEventArgs.CutObjectTitle))
				{
					MenuItemEditCut.Enabled = true;
					MenuItemEditCut.Text = String.Format (Properties.Resources.EditCutThis, lEventArgs.CutObjectTitle);
				}
				else
				{
					MenuItemEditCut.Enabled = false;
					MenuItemEditCut.Text = Properties.Resources.EditCut;
				}
				if (!MainForm.Singleton.FileIsReadOnly && !String.IsNullOrEmpty (lEventArgs.DeleteObjectTitle))
				{
					MenuItemEditDelete.Enabled = true;
					MenuItemEditDelete.Text = String.Format (Properties.Resources.EditDeleteThis, lEventArgs.DeleteObjectTitle);
				}
				else
				{
					MenuItemEditDelete.Enabled = false;
					MenuItemEditDelete.Text = Properties.Resources.EditDelete;
				}
				if (!MainForm.Singleton.FileIsReadOnly && !String.IsNullOrEmpty (lEventArgs.PasteObjectTitle))
				{
					MenuItemEditPaste.Enabled = true;
					MenuItemEditPaste.Text = String.Format (Properties.Resources.EditPasteThis, lEventArgs.PasteObjectTitle);
				}
				else
				{
					MenuItemEditPaste.Enabled = false;
					MenuItemEditPaste.Text = Properties.Resources.EditPaste;
				}
			}

			ToolButtonEditCopy.Enabled = MenuItemEditCopy.Enabled;
			ToolButtonEditCut.Enabled = MenuItemEditCut.Enabled;
			ToolButtonEditPaste.Enabled = MenuItemEditPaste.Enabled;
			ToolButtonEditDelete.Enabled = MenuItemEditDelete.Enabled;
			ToolButtonEditCopy.Text = MenuItemEditCopy.Text.Replace ("&", "");
			ToolButtonEditCut.Text = MenuItemEditCut.Text.Replace ("&", "");
			ToolButtonEditPaste.Text = MenuItemEditPaste.Text.Replace ("&", "");
			ToolButtonEditDelete.Text = MenuItemEditDelete.Text.Replace ("&", "");

			if ((lTextBox != null) && lTextBox.Modified)
			{
				MenuItemEditUndo.Enabled = lTextBox.CanUndo;
				MenuItemEditRedo.Enabled = false;
				MenuItemEditUndo.Text = MenuItemEditUndo.Enabled ? Properties.Resources.EditUndoText : Properties.Resources.EditUndo;
				MenuItemEditRedo.Text = Properties.Resources.EditRedo;
			}
			else
			{
				MenuItemEditUndo.Enabled = UndoManager.CanUndo;
				MenuItemEditRedo.Enabled = UndoManager.CanRedo;
				MenuItemEditUndo.Text = UndoManager.CanUndo ? String.Format (Properties.Resources.EditUndoThis, UndoManager.UndoName) : Properties.Resources.EditUndo;
				MenuItemEditRedo.Text = UndoManager.CanRedo ? String.Format (Properties.Resources.EditRedoThis, UndoManager.RedoName) : Properties.Resources.EditRedo;
			}

			ToolButtonEditUndo.Enabled = MenuItemEditUndo.Enabled;
			ToolButtonEditRedo.Enabled = MenuItemEditRedo.Enabled;
			ToolButtonEditUndo.Text = MenuItemEditUndo.Text.Replace ("&", "");
			ToolButtonEditRedo.Text = MenuItemEditRedo.Text.Replace ("&", "");
		}

		///////////////////////////////////////////////////////////////////////////////

		internal Control GetActiveControl ()
		{
			try
			{
				Control	lActive = this.ActiveControl;

				if ((lActive != null) && (lActive.Enabled))
				{
					while (!lActive.Focused && lActive.HasChildren && lActive.ContainsFocus)
					{
						ContainerControl	lContainer = null;
						Control				lContained = null;

						lContainer = lActive as ContainerControl;
						if (lContainer != null)
						{
							lContained = lContainer.ActiveControl;
						}
						if ((lContained != null) && (lContained.Enabled))
						{
							lActive = lContained;
						}
						else
						{
							break;
						}
					}

					if ((lActive != null) && lActive.Enabled && lActive.Focused)
					{
						return lActive;
					}
				}
			}
			catch
			{
			}

			return null;
		}

		internal Control GetActiveControl (ref TextBox pTextBox)
		{
			NumericUpDownEx	lNumeric = null;
			Control			lActive = GetActiveControl (ref pTextBox, ref lNumeric);

			if (lNumeric != null)
			{
				pTextBox = lNumeric.TextBox;
			}
			return lActive;
		}

		internal Control GetActiveControl (ref TextBox pTextBox, ref NumericUpDownEx pNumeric)
		{
			Control	lActive = GetActiveControl ();

			pTextBox = null;
			pNumeric = null;

			if (lActive != null)
			{
				pTextBox = lActive as TextBox;
				pNumeric = lActive as NumericUpDownEx;
			}
			return lActive;
		}

		///////////////////////////////////////////////////////////////////////////////

		public new Boolean ValidateChildren ()
		{
			return ValidateChildren (ValidationConstraints.Visible | ValidationConstraints.Enabled | ValidationConstraints.Selectable);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Form Event Handlers

		private void MainForm_Load (object sender, EventArgs e)
		{
			LoadAllConfig ();
		}

		private void MainForm_FormClosing (object sender, FormClosingEventArgs e)
		{
			SaveAllConfig ();

#if	!DEBUG
			if ((mCharacterFile != null) && (mCharacterFile.IsDirty))
			{
				switch (MessageBox.Show (String.Format (Properties.Resources.MsgSaveFile, mCharacterFile.FileName), Program.AssemblyTitle, MessageBoxButtons.YesNoCancel))
				{
					case DialogResult.Yes:
						{
							mCharacterFile.Save ();
						}
						break;
					case DialogResult.Cancel:
						{
							e.Cancel = true;
						}
						break;
				}
			}
#endif
		}

		private void MainForm_Shown (object sender, EventArgs e)
		{
			if (!String.IsNullOrEmpty (mInitialFile))
			{
				Update ();
				OpenCharacterFile (mInitialFile);
			}
			if (TreeViewMain.SelectedNode != null)
			{
				TreeViewMain.SelectedNode.EnsureVisible ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		void Application_Idle (object sender, EventArgs e)
		{
			try
			{
				ShowEditState ();
			}
			catch
			{
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Tree and ToolStrip Event Handlers

		private void TreeViewMain_AfterSelect (object sender, TreeViewEventArgs e)
		{
			ShowSelectedTreeNode ();
			ShowEditState ();
		}

		private void ToolButtonFileNew_Click (object sender, EventArgs e)
		{
			MenuItemFileNew_Click (sender, e);
		}

		private void ToolButtonFileOpen_Click (object sender, EventArgs e)
		{
			MenuItemFileOpen_Click (sender, e);
		}

		private void ToolButtonFileSave_Click (object sender, EventArgs e)
		{
			MenuItemFileSave_Click (sender, e);
		}

		private void ToolButtonEditUndo_Click (object sender, EventArgs e)
		{
			MenuItemEditUndo_Click (sender, e);
		}

		private void ToolButtonEditRedo_Click (object sender, EventArgs e)
		{
			MenuItemEditRedo_Click (sender, e);
		}

		private void ToolButtonEditCut_Click (object sender, EventArgs e)
		{
			MenuItemEditCut_Click (sender, e);
		}

		private void ToolButtonEditCopy_Click (object sender, EventArgs e)
		{
			MenuItemEditCopy_Click (sender, e);
		}

		private void ToolButtonEditPaste_Click (object sender, EventArgs e)
		{
			MenuItemEditPaste_Click (sender, e);
		}

		private void ToolButtonEditDelete_Click (object sender, EventArgs e)
		{
			MenuItemEditDelete_Click (sender, e);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Menu Event Handlers

		private void MenuItemExit_Click (object sender, EventArgs e)
		{
			Close ();
		}

		private void MenuItemHelpAbout_Click (object sender, EventArgs e)
		{
			(new AgentCharacterEditor.AboutBox ()).ShowDialog ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemFileNew_Click (object sender, EventArgs e)
		{
			MessageBox.Show ("Not implemented", Program.AssemblyTitle, MessageBoxButtons.OK);
		}

		private void MenuItemFileOpen_Click (object sender, EventArgs e)
		{
			String	lFilePath = String.Empty;

			if (OpenFileDialogEx.OpenCharacterFile (ref lFilePath))
			{
				OpenCharacterFile (lFilePath);
			}
		}

		private void MenuItemFileSave_Click (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && (!mCharacterFile.IsReadOnly) && (mCharacterFile.IsDirty))
			{
				CursorState	lCursorState = new CursorState (this);

				lCursorState.ShowWait ();

				try
				{
#if DEBUG_NOT
					mCharacterFile.Save (3);
#else
					mCharacterFile.Save ();
#endif
					ShowFileState ();
					ShowEditState ();
				}
				catch
				{
				}

				lCursorState.RestoreCursor ();
			}
		}

		private void MenuItemFileSaveAs_Click (object sender, EventArgs e)
		{
			if (mCharacterFile != null)
			{
				String	lFilePath = String.Empty;

				if (!String.IsNullOrEmpty (mCharacterFile.Path))
				{
					lFilePath = "*" + System.IO.Path.GetExtension (mCharacterFile.Path);
				}
				if (OpenFileDialogEx.SaveCharacterFile (ref lFilePath))
				{
					CursorState		lCursorState = new CursorState (this);
					CharacterFile	lCharacterFile = null;
					Boolean			lSaved = false;

					if (String.Compare (mCharacterFile.Path, lFilePath, true) == 0)
					{
						MessageBox.Show (Properties.Resources.MsgInvalidSaveAs, Program.AssemblyTitle, MessageBoxButtons.OK);
					}
					else
					{
						lCursorState.ShowWait ();

						try
						{
							lCharacterFile = CharacterFile.CreateInstance (lFilePath);
						}
						catch
						{
						}
						if (lCharacterFile != null)
						{
							try
							{
#if DEBUG_NOT
								lSaved = lCharacterFile.Save (lFilePath, mCharacterFile, 3);
#else
								lSaved = lCharacterFile.Save (lFilePath, mCharacterFile);
#endif
							}
							catch (Exception pException)
							{
								MessageBox.Show (pException.Message);
							}

							if (lSaved)
							{
								lFilePath = lCharacterFile.Path;
								lCharacterFile.Close ();
								lCharacterFile = null;
								OpenCharacterFile (lFilePath);
							}
							else
							{
								MessageBox.Show (String.Format (Properties.Resources.MsgFailedSaveAs, lFilePath), Program.AssemblyTitle, MessageBoxButtons.OK);
							}
						}
						else
						{
							MessageBox.Show ("Not implemented", Program.AssemblyTitle, MessageBoxButtons.OK);
						}

						lCursorState.RestoreCursor ();
					}
				}
			}
		}

		private void MenuItemFilePrint_Click (object sender, EventArgs e)
		{
			MessageBox.Show ("Not implemented", Program.AssemblyTitle, MessageBoxButtons.OK);
		}

		private void MenuItemFilePrintPreview_Click (object sender, EventArgs e)
		{
			MessageBox.Show ("Not implemented", Program.AssemblyTitle, MessageBoxButtons.OK);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemEditUndo_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if ((lTextBox != null) && lTextBox.Modified)
			{
				if (lTextBox.CanUndo)
				{
					lTextBox.Undo ();
					ToolStripTop.TipTextChanged ();
					ValidateChildren ();
				}
			}
			else if (UndoManager.Undo ())
			{
				ShowFileState ();
				ShowEditState ();
			}
		}

		private void MenuItemEditRedo_Click (object sender, EventArgs e)
		{
			if (UndoManager.Redo ())
			{
				ShowFileState ();
				ShowEditState ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemEditCopy_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				lTextBox.Copy ();
				ValidateChildren ();
			}
			else
			{
				EditEventArgs	lEventArgs = new EditEventArgs ();

				if (lActive == TreeViewMain)
				{
					(new TreeSelection (TreeViewMain.SelectedNode)).EditCopy (lEventArgs);
				}
				else if (EditCopy != null)
				{
					EditCopy (this, lEventArgs);
				}
				if (!lEventArgs.IsUsed)
				{
					ShowEditState ();
				}
			}
		}

		private void MenuItemEditCut_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				lTextBox.Cut ();
				ValidateChildren ();
			}
			else
			{
				EditEventArgs	lEventArgs = new EditEventArgs ();

				if (lActive == TreeViewMain)
				{
					(new TreeSelection (TreeViewMain.SelectedNode)).EditCut (lEventArgs);
				}
				else if (EditCut != null)
				{
					EditCut (this, lEventArgs);
				}
				if (!lEventArgs.IsUsed)
				{
					ShowEditState ();
				}
			}
		}

		private void MenuItemEditDelete_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				lTextBox.Paste (String.Empty);
				ValidateChildren ();
			}
			else
			{
				EditEventArgs	lEventArgs = new EditEventArgs ();

				if (lActive == TreeViewMain)
				{
					(new TreeSelection (TreeViewMain.SelectedNode)).EditDelete (lEventArgs);
				}
				else if (EditDelete != null)
				{
					EditDelete (this, lEventArgs);
				}
				if (!lEventArgs.IsUsed)
				{
					ShowEditState ();
				}
			}
		}

		private void MenuItemEditPaste_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				lTextBox.Paste ();
				ValidateChildren ();
			}
			else
			{
				EditEventArgs	lEventArgs = new EditEventArgs (DataFormats.Serializable);

				if (lActive == TreeViewMain)
				{
					(new TreeSelection (TreeViewMain.SelectedNode)).EditPaste (lEventArgs);
				}
				else if (EditPaste != null)
				{
					EditPaste (this, lEventArgs);
				}
				if (lEventArgs.IsUsed)
				{
					ToolStripTop.TipTextChanged ();
				}
				ShowEditState ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		private void RecentFiles_RecentItemClick (object sender, String e)
		{
			if (!OpenCharacterFile (e))
			{
				if (MessageBox.Show (String.Format ("Remove {0} from the recent files list?", e), Program.AssemblyTitle, MessageBoxButtons.YesNo) == DialogResult.Yes)
				{
					if (mRecentFiles.RemovePath (e))
					{
						UpdateRecentFiles ();
					}
				}
			}
		}

		private void UndoManager_Undone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty = true;
			ToolStripTop.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("[{0}] Undone", e.UndoUnit.ToString ());
#endif
		}

		private void UndoManager_Redone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty = true;
			ToolStripTop.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("[{0}] Redone", e.UndoUnit.ToString ());
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PanelAnimations_AnimationAdded (object sender, AnimationEventArgs e)
		{
			ShowAnimationNames ();
		}

		private void PanelAnimations_AnimationRemoved (object sender, AnimationEventArgs e)
		{
			ShowAnimationNames ();
			ShowSelectedTreeNode ();
		}

		private void PanelAnimation_AnimationNameChanged (object sender, AnimationEventArgs e)
		{
			TreeNode	lAnimationNode = (new TreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				lAnimationNode.Text = e.Animation.Name;
			}
		}

		private void PanelAnimation_AnimationFrameAdded (object sender, AnimationFrameEventArgs e)
		{
			TreeNode	lAnimationNode = (new TreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (e.Animation, lAnimationNode);
			}
		}

		private void PanelAnimation_AnimationFrameRemoved (object sender, AnimationFrameEventArgs e)
		{
			TreeNode	lAnimationNode = (new TreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (e.Animation, lAnimationNode);
				ShowSelectedTreeNode ();
			}
		}

		private void PanelAnimation_AnimationFrameReordered (object sender, AnimationFrameEventArgs e)
		{
			TreeNode	lAnimationNode = (new TreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (e.Animation, lAnimationNode);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PanelAnimations_GoToAnimation (object sender, AnimationEventArgs e)
		{
			TreeNode	lAnimationNode = (new TreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				TreeViewMain.SelectedNode = lAnimationNode;
				ShowSelectedTreeNode ();
			}
		}

		private void PanelAnimation_GoToFrame (object sender, AnimationFrameEventArgs e)
		{
			TreeNode	lFrameNode = (new TreeSelection (e.Animation, e.Frame)).SelectedNode ();

			if (lFrameNode != null)
			{
				TreeViewMain.SelectedNode = lFrameNode;
				ShowSelectedTreeNode ();
			}
		}

		private void PanelAnimation_GoToState (object sender, StateEventArgs e)
		{
			TreeNode	lStateNode = (new TreeSelection (e.StateName)).SelectedNode ();

			if (lStateNode != null)
			{
				TreeViewMain.SelectedNode = lStateNode;
				ShowSelectedTreeNode ();
			}
		}

		private void PanelState_GoToAnimation (object sender, AnimationEventArgs e)
		{
			TreeNode	lAnimationNode = (new TreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				TreeViewMain.SelectedNode = lAnimationNode;
				ShowSelectedTreeNode ();
			}
		}

		#endregion
	}
}
