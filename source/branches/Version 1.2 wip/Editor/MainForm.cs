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
		private const String	mNodeCharacter = "NodeCharacter";
		private const String	mNodeWordBalloon = "NodeWordBalloon";
		private const String	mNodeTTSMode = "NodeTTSMode";
		private const String	mNodeAnimations = "NodeAnimations";
		private const String	mNodeStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public MainForm ()
		{
			mRecentFiles = new RecentFileList ();
			mRecentFiles.RecentItemClick += new RecentFileList.RecentItemClickEvent (RecentFiles_RecentItemClick);
			InitializeComponent ();
			Application.Idle += new EventHandler (Application_Idle);
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
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private bool OpenCharacterFile (String pFilePath)
		{
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

			CharacterFile	lCharacterFile = CharacterFile.CreateInstance (pFilePath);

			try
			{
				lCharacterFile.Open (pFilePath, 1);
			}
			catch (Exception e)
			{
				MessageBox.Show (e.Message);
				return false;
			}

			if (mCharacterFile != null)
			{
				SaveConfig ();
			}
			if (lCharacterFile.IsOpen)
			{
				mCharacterFile = lCharacterFile;

				ShowFileState ();
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
				mRecentFiles.MostRecent = mCharacterFile.Path;
				mRecentFiles.ShowPaths (MenuItemFile.DropDown, MenuItemFilePrintPreview);
				return true;
			}
			else
			{
				MessageBox.Show (String.Format (Properties.Resources.MsgInvalidFile, pFilePath), Program.AssemblyTitle, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return false;
			}
		}

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
										lFileFrame = (FileAnimationFrame)TreeViewMain.SelectedNode.Parent.Tag;
										lFrameForm = (UserControl)TreeViewMain.SelectedNode.Tag;
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
									lFileFrame = (FileAnimationFrame)TreeViewMain.SelectedNode.Tag;
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
								lFileAnimation = (FileAnimation)TreeViewMain.SelectedNode.Tag;
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
#if	DEBUG
			//OpenAgentFile ("C:\\Windows\\MsAgent\\Chars\\Merlin.acs");
			//OpenAgentFile ("C:\\DoubleAgent\\_Test\\Chars\\TestChar1.acs");
			OpenCharacterFile ("C:\\DoubleAgent\\_Test\\Chars\\TestChar1.acd");
#endif
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
				Control			lActive = GetActiveControl ();
				TextBox			lTextBox = null;
				NumericUpDownEx	lNumeric = null;


				if (lActive != null)
				{
					try
					{
						lTextBox = (TextBox)lActive;
					}
					catch
					{
					}
					try
					{
						lNumeric = (NumericUpDownEx)lActive;
					}
					catch
					{
					}
				}

				if (lTextBox != null)
				{
					ToolButtonEditCopy.Enabled = (lTextBox.SelectionLength > 0);
					ToolButtonEditCut.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
					ToolButtonEditDelete.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
					ToolButtonEditPaste.Enabled = !lTextBox.ReadOnly && (Clipboard.ContainsText (TextDataFormat.Text) || Clipboard.ContainsText (TextDataFormat.UnicodeText));
				}
				else if (lNumeric != null)
				{
					ToolButtonEditCopy.Enabled = (lNumeric.TextBox.SelectionLength > 0);
					ToolButtonEditCut.Enabled = !lNumeric.ReadOnly && (lNumeric.TextBox.SelectionLength > 0);
					ToolButtonEditDelete.Enabled = !lNumeric.ReadOnly && (lNumeric.TextBox.SelectionLength > 0);
					ToolButtonEditPaste.Enabled = !lNumeric.ReadOnly && (Clipboard.ContainsText (TextDataFormat.Text) || Clipboard.ContainsText (TextDataFormat.UnicodeText));
				}
				else
				{
					ToolButtonEditCopy.Enabled = false;
					ToolButtonEditCut.Enabled = false;
					ToolButtonEditDelete.Enabled = false;
					ToolButtonEditPaste.Enabled = false;
				}
			}
			catch
			{
			}
		}

		internal Control GetActiveControl ()
		{
			try
			{
				Control	lActive = this.ActiveControl;

				if ((lActive != null) && (lActive.Enabled))
				{
					while (!lActive.Focused && lActive.HasChildren)
					{
						try
						{
							ContainerControl	lContainer = (ContainerControl)lActive;
							Control				lContained = lContainer.ActiveControl;

							if ((lContained != null) && (lContained.Enabled))
							{
								lActive = lContained;
							}
							else
							{
								break;
							}
						}
						catch
						{
						}
					}

					if ((lActive != null) && (lActive.Enabled) && (lActive.Focused))
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

		private void MenuItemFileNew_Click (object sender, EventArgs e)
		{

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
				mCharacterFile.Save ();
				ShowFileState ();
			}
		}

		private void MenuItemFileSaveAs_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemFilePrint_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemFilePrintPreview_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemEditUndo_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemEditRedo_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemEditCut_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemEditCopy_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemEditPaste_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemEditDelete_Click (object sender, EventArgs e)
		{

		}

		private void MenuItemHelpAbout_Click (object sender, EventArgs e)
		{
			(new AgentCharacterEditor.AboutBox ()).ShowDialog ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		void RecentFiles_RecentItemClick (object sender, string e)
		{
			OpenCharacterFile (e);
		}

		private void PanelAnimations_AnimationAdded (object sender, AnimationsForm.AnimationEventArgs e)
		{
			ShowAnimationNames ();
		}

		private void PanelAnimations_AnimationRemoved (object sender, AnimationsForm.AnimationEventArgs e)
		{
			ShowAnimationNames ();
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
