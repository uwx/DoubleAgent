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
	public partial class MainForm : Form
	{
		private CharacterFile	mCharacterFile = null;
		private RecentFileList	mRecentFiles;
		private UndoManager		mUndoManager;
		private String			mInitialFile = null;
		private const String	mNodeCharacter = "NodeCharacter";
		private const String	mNodeWordBalloon = "NodeWordBalloon";
		private const String	mNodeTTSMode = "NodeTTSMode";
		private const String	mNodeAnimations = "NodeAnimations";
		private const String	mNodeStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public MainForm (string[] args)
		{
			if ((args != null) && (args.Length > 0))
			{
				mInitialFile = args[0];
			}
			mRecentFiles = new RecentFileList ();
			mUndoManager = new UndoManager ();

			InitializeComponent ();

			this.MainMenuStrip = this.MenuStripMain;
			Application.Idle += new EventHandler (Application_Idle);
			mRecentFiles.RecentItemClick += new RecentFileList.RecentItemClickEvent (RecentFiles_RecentItemClick);
			mUndoManager.Undone += new UndoManager.UndoneEvent (UndoManager_Undone);
			mUndoManager.Redone += new UndoManager.RedoneEvent (UndoManager_Redone);

			PanelAnimations.AnimationAdded += new AnimationsForm.AnimationAddedEvent (PanelState.PanelAnimations_AnimationAdded);
			PanelAnimations.AnimationRemoved += new AnimationsForm.AnimationRemovedEvent (PanelState.PanelAnimations_AnimationRemoved);
			PanelAnimation.AnimationNameChanged += new AnimationForm.AnimationNameChangedEvent (PanelState.PanelAnimation_AnimationNameChanged);
			PanelAnimation.AnimationNameChanged += new AnimationForm.AnimationNameChangedEvent (PanelAnimations.PanelAnimation_AnimationNameChanged);
		}

		~MainForm ()
		{
			Application.Idle -= new EventHandler (Application_Idle);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void LoadConfig ()
		{
			PanelCharacter.LoadConfig ();
			PanelBalloon.LoadConfig ();
			PanelTts.LoadConfig ();
			PanelFrame.LoadConfig ();

			LoadRecentFiles ();
			mRecentFiles.ShowPaths (MenuItemFile.DropDown, MenuItemFilePrintPreview);
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

			lNode = FindTreeRootNode (mNodeCharacter);
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
			lNode = FindTreeRootNode (mNodeAnimations);
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
			lNode = FindTreeRootNode (mNodeStates);
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
		}

		///////////////////////////////////////////////////////////////////////////////

		private void SaveConfig ()
		{
			PanelCharacter.SaveConfig ();
			PanelBalloon.SaveConfig ();
			PanelTts.SaveConfig ();
			PanelFrame.SaveConfig ();

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

			lNode = FindTreeRootNode (mNodeCharacter);
			if (lNode != null)
			{
				lSettings.CharacterNodeExpanded = lNode.IsExpanded;
			}
			lNode = FindTreeRootNode (mNodeAnimations);
			if (lNode != null)
			{
				lSettings.AnimationsNodeExpanded = lNode.IsExpanded;
			}
			lNode = FindTreeRootNode (mNodeStates);
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

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
		}

		public bool FileIsReadOnly
		{
			get
			{
				return ((mCharacterFile == null) || (mCharacterFile.IsReadOnly));
			}
		}

		public bool FileIsDirty
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
		#region Display

		private bool OpenCharacterFile (String pFilePath)
		{
			bool			lRet = false;
			CursorState		lCursorState = new CursorState (this);
			CharacterFile	lCharacterFile = null;
			bool			lOpened = false;

			try
			{
				if ((mCharacterFile != null) && (System.IO.Path.GetFullPath (pFilePath).Equals (System.IO.Path.GetFullPath (mCharacterFile.Path), StringComparison.InvariantCultureIgnoreCase)))
				{
					return false;
				}
			}
			catch
			{
			}

			lCursorState.ShowWait ();
			if (mCharacterFile != null)
			{
				SaveConfig ();
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
#if DEBUG
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
			TreeNode	lAnimationsNode = FindTreeRootNode (mNodeAnimations);

			if (lAnimationsNode != null)
			{
				bool	lExpanded = lAnimationsNode.IsExpanded;

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
				int			lFrameNum = 0;
				String		lFrameName;
				TreeNode	lFrameNode;
				TreeNode	lFrameSubNode;

				foreach (FileAnimationFrame lFrame in pAnimation.Frames)
				{
					lFrameName = "Frame " + (++lFrameNum).ToString ();
					lFrameNode = (lFrameNum <= pAnimationNode.Nodes.Count) ? pAnimationNode.Nodes[lFrameNum - 1] : pAnimationNode.Nodes.Add (lFrameName);
					lFrameNode.Text = lFrameName;
					lFrameNode.Tag = lFrame;

					lFrameSubNode = (lFrameNode.Nodes.Count >= 1) ? lFrameNode.Nodes[0] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Branching";
					lFrameSubNode.Tag = PanelBranching;
					lFrameSubNode = (lFrameNode.Nodes.Count >= 2) ? lFrameNode.Nodes[1] : lFrameNode.Nodes.Add ("");
					lFrameSubNode.Text = "Overlays";
					lFrameSubNode.Tag = PanelOverlays;
				}

				while (pAnimationNode.Nodes.Count > lFrameNum)
				{
					pAnimationNode.Nodes.RemoveAt (pAnimationNode.Nodes.Count - 1);
				}
			}
		}

		private void ShowSelectedTreeNode ()
		{
			if (TreeViewMain.SelectedNode == null)
			{
				TreeViewMain.SelectedNode = FindTreeRootNode (mNodeCharacter);
			}
			if (TreeViewMain.SelectedNode != null)
			{
				SuspendLayout ();

				try
				{
					if (TreeViewMain.SelectedNode.Name == mNodeCharacter)
					{
						PanelCharacter.Visible = true;
					}
					else
					{
						PanelCharacter.Visible = false;
					}

					if (TreeViewMain.SelectedNode.Name == mNodeWordBalloon)
					{
						PanelBalloon.Visible = true;
					}
					else
					{
						PanelBalloon.Visible = false;
					}

					if (TreeViewMain.SelectedNode.Name == mNodeTTSMode)
					{
						PanelTts.Visible = true;
					}
					else
					{
						PanelTts.Visible = false;
					}

					if (TreeViewMain.SelectedNode.Name == mNodeAnimations)
					{
						PanelAnimations.Visible = true;
					}
					else
					{
						PanelAnimations.Visible = false;
					}

					if (TreeViewMain.SelectedNode.Parent != null)
					{
						FileAnimation		lFileAnimation = null;
						FileAnimationFrame	lFileFrame = null;
						String				lAnimationName = null;
						String				lFrameName = null;
						UserControl			lFrameForm = null;

						if (TreeViewMain.SelectedNode.Parent.Parent != null)
						{
							if (TreeViewMain.SelectedNode.Parent.Parent.Parent != null)
							{
								if (TreeViewMain.SelectedNode.Parent.Parent.Parent.Name == mNodeAnimations)
								{
									try
									{
										lAnimationName = TreeViewMain.SelectedNode.Parent.Parent.Text;
										lFrameName = TreeViewMain.SelectedNode.Parent.Text;
										lFileFrame = TreeViewMain.SelectedNode.Parent.Tag as FileAnimationFrame;
										lFrameForm = TreeViewMain.SelectedNode.Tag as UserControl;
									}
									catch
									{
									}
								}
							}
							else if (TreeViewMain.SelectedNode.Parent.Parent.Name == mNodeAnimations)
							{
								try
								{
									lAnimationName = TreeViewMain.SelectedNode.Parent.Text;
									lFrameName = TreeViewMain.SelectedNode.Text;
									lFileFrame = TreeViewMain.SelectedNode.Tag as FileAnimationFrame;
									lFrameForm = PanelFrame;
								}
								catch
								{
								}
							}
						}
						else if (TreeViewMain.SelectedNode.Parent.Name == mNodeAnimations)
						{
							try
							{
								lAnimationName = TreeViewMain.SelectedNode.Text;
								lFileAnimation = TreeViewMain.SelectedNode.Tag as FileAnimation;
							}
							catch
							{
							}
						}

						if (lFileAnimation != null)
						{
							PanelAnimation.Visible = true;
							PanelAnimation.FileAnimation = lFileAnimation;
							PanelAnimation.AnimationName = lAnimationName;
						}
						else
						{
							PanelAnimation.Visible = false;
						}

						if (lFileFrame != null)
						{
							if (lFrameForm == PanelFrame)
							{
								PanelFrame.Visible = true;
								PanelFrame.FileFrame = lFileFrame;
								PanelFrame.FrameName = lFrameName;
								PanelFrame.AnimationName = lAnimationName;
							}
							else
							{
								PanelFrame.Visible = false;
							}

							if (lFrameForm == PanelBranching)
							{
								PanelBranching.Visible = true;
								PanelBranching.FileFrame = lFileFrame;
								PanelBranching.FrameName = lFrameName;
								PanelBranching.AnimationName = lAnimationName;
							}
							else
							{
								PanelBranching.Visible = false;
							}

							if (lFrameForm == PanelOverlays)
							{
								PanelOverlays.Visible = true;
								PanelOverlays.FileFrame = lFileFrame;
								PanelOverlays.FrameName = lFrameName;
								PanelOverlays.AnimationName = lAnimationName;
							}
							else
							{
								PanelOverlays.Visible = false;
							}
						}
						else
						{
							PanelFrame.Visible = false;
							PanelBranching.Visible = false;
							PanelOverlays.Visible = false;
						}

						if (TreeViewMain.SelectedNode.Parent.Name == mNodeStates)
						{
							PanelState.Visible = true;
							PanelState.StateName = TreeViewMain.SelectedNode.Text;
						}
						else
						{
							PanelState.Visible = false;
						}
					}
					else
					{
						PanelAnimation.Visible = false;
						PanelFrame.Visible = false;
						PanelBranching.Visible = false;
						PanelOverlays.Visible = false;
						PanelState.Visible = false;
					}
				}
				finally
				{
					ResumeLayout (true);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				ToolButtonEditCopy.Enabled = (lTextBox.SelectionLength > 0);
				ToolButtonEditCut.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
				ToolButtonEditDelete.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
				ToolButtonEditPaste.Enabled = !lTextBox.ReadOnly && (Clipboard.ContainsText (TextDataFormat.Text) || Clipboard.ContainsText (TextDataFormat.UnicodeText));
			}
			else
			{
				ToolButtonEditCopy.Enabled = false;
				ToolButtonEditCut.Enabled = false;
				ToolButtonEditDelete.Enabled = false;
				ToolButtonEditPaste.Enabled = false;
			}

			if ((lTextBox != null) && lTextBox.Modified)
			{
				ToolButtonEditUndo.Enabled = lTextBox.CanUndo;
				ToolButtonEditRedo.Enabled = false;
				ToolButtonEditUndo.ToolTipText = ToolButtonEditUndo.Enabled ? "Undo typing" : "Undo";
				ToolButtonEditRedo.ToolTipText = "Redo";
			}
			else
			{
				ToolButtonEditUndo.Enabled = UndoManager.CanUndo;
				ToolButtonEditRedo.Enabled = UndoManager.CanRedo;
				ToolButtonEditUndo.ToolTipText = UndoManager.UndoTip;
				ToolButtonEditRedo.ToolTipText = UndoManager.RedoTip;
			}

			MenuItemEditUndo.Text = "&" + ToolButtonEditUndo.ToolTipText;
			MenuItemEditRedo.Text = "&" + ToolButtonEditRedo.ToolTipText;
			MenuItemEditUndo.Enabled = ToolButtonEditUndo.Enabled;
			MenuItemEditRedo.Enabled = ToolButtonEditRedo.Enabled;
			MenuItemEditCopy.Enabled = ToolButtonEditCopy.Enabled;
			MenuItemEditCut.Enabled = ToolButtonEditCut.Enabled;
			MenuItemEditDelete.Enabled = ToolButtonEditDelete.Enabled;
			MenuItemEditPaste.Enabled = ToolButtonEditPaste.Enabled;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Methods

		private TreeNode FindTreeRootNode (String pNodeName)
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

		private TreeNode FindTreeAnimationNode (FileAnimation pAnimation)
		{
			TreeNode	lAnimationsNode = FindTreeRootNode (mNodeAnimations);

			if (lAnimationsNode != null)
			{
				foreach (TreeNode lNode in lAnimationsNode.Nodes)
				{
					try
					{
						if ((FileAnimation)lNode.Tag == pAnimation)
						{
							return lNode;
						}
					}
					catch
					{
					}
				}
			}
			return null;
		}

		private TreeNode FindTreeAnimationNode (String pAnimationName)
		{
			TreeNode	lAnimationsNode = FindTreeRootNode (mNodeAnimations);

			if (lAnimationsNode != null)
			{
				foreach (TreeNode lNode in lAnimationsNode.Nodes)
				{
					try
					{
						FileAnimation	lAnimation = (FileAnimation)lNode.Tag;

						if ((lAnimation != null) && (String.Compare (lAnimation.Name, pAnimationName, true) == 0))
						{
							return lNode;
						}
					}
					catch
					{
					}
				}
			}
			return null;
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

		public new bool ValidateChildren ()
		{
			return ValidateChildren (ValidationConstraints.Visible | ValidationConstraints.Enabled | ValidationConstraints.Selectable);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Form Event Handlers

		private void MainForm_Load (object sender, EventArgs e)
		{
			LoadConfig ();
		}

		private void MainForm_FormClosing (object sender, FormClosingEventArgs e)
		{
			SaveConfig ();

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
		#region Form UI Event Handlers

		private void TreeViewMain_AfterSelect (object sender, TreeViewEventArgs e)
		{
			ShowSelectedTreeNode ();
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
#if DEBUG
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
					bool			lSaved = false;

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
#if DEBUG
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
							MessageBox.Show ("<save failed>", Program.AssemblyTitle, MessageBoxButtons.OK);
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

		private void MenuItemEditCut_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				lTextBox.Cut ();
				ValidateChildren ();
			}
		}

		private void MenuItemEditCopy_Click (object sender, EventArgs e)
		{
			TextBox	lTextBox = null;
			Control	lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				lTextBox.Copy ();
				ValidateChildren ();
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
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		private void RecentFiles_RecentItemClick (object sender, string e)
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

		private void PanelAnimations_AnimationAdded (object sender, AnimationsForm.AnimationEventArgs e)
		{
			ShowAnimationNames ();
		}

		private void PanelAnimations_AnimationRemoved (object sender, AnimationsForm.AnimationEventArgs e)
		{
			ShowAnimationNames ();
			ShowSelectedTreeNode ();
		}

		private void PanelAnimation_AnimationNameChanged (object sender, AnimationForm.AnimationEventArgs e)
		{
			TreeNode	lAnimationNode = FindTreeAnimationNode (e.Animation);

			if (lAnimationNode != null)
			{
				lAnimationNode.Text = e.Animation.Name;
			}
		}

		private void PanelAnimation_AnimationFrameAdded (object sender, AnimationForm.AnimationFrameEventArgs e)
		{
			TreeNode	lAnimationNode = FindTreeAnimationNode (e.Animation);

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (e.Animation, lAnimationNode);
			}
		}

		private void PanelAnimation_AnimationFrameRemoved (object sender, AnimationForm.AnimationFrameEventArgs e)
		{
			TreeNode	lAnimationNode = FindTreeAnimationNode (e.Animation);

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (e.Animation, lAnimationNode);
				ShowSelectedTreeNode ();
			}
		}

		private void PanelAnimation_AnimationFrameReordered (object sender, AnimationForm.AnimationFrameEventArgs e)
		{
			TreeNode	lAnimationNode = FindTreeAnimationNode (e.Animation);

			if (lAnimationNode != null)
			{
				ShowAnimationFrames (e.Animation, lAnimationNode);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
	}
}
