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
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
{
	public partial class MainForm : Form
	{
		private CharacterFile	mCharacterFile = null;
		private RecentFileList	mRecentFiles = new RecentFileList ();
		private String			mInitialFile = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public MainForm ()
		{
			InitializeComponent ();
		}

		public MainForm (String[] args)
		{
			if ((args != null) && (args.Length > 0))
			{
				mInitialFile = args[0];
			}
		}

		~MainForm ()
		{
			Application.Idle -= new EventHandler (Application_Idle);
		}

		public void Initialize ()
		{
			InitializeComponent ();

			MainMenuStrip = MenuStripMain;
			Application.Idle += new EventHandler (Application_Idle);
			mRecentFiles.RecentItemClick += new RecentFileList.RecentItemClickEventHandler (RecentFiles_RecentItemClick);
			Program.UndoManager.Undone += new UndoManager.UndoneEventHandler (UndoManager_Undone);
			Program.UndoManager.Redone += new UndoManager.RedoneEventHandler (UndoManager_Redone);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void LoadAllConfig ()
		{
			LoadConfig (this, EventArgs.Empty);
			LoadRecentFiles ();
			TreeViewMain.LoadExpansion ();
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
			TreeViewMain.SaveExpansion ();
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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event EventHandler LoadConfig;
		public event EventHandler SaveConfig;

		public event Global.EditEventHandler CanEdit;
		public event Global.EditEventHandler EditCopy;
		public event Global.EditEventHandler EditCut;
		public event Global.EditEventHandler EditDelete;
		public event Global.EditEventHandler EditPaste;

		public event UndoUnit.AppliedEventHandler UpdateApplied;

		///////////////////////////////////////////////////////////////////////////////

		public void FileIsDirty ()
		{
			if (CharacterFile != null)
			{
				CharacterFile.IsDirty = true;
				ShowFileState ();
				ShowEditState ();
			}
		}

		public void OnUpdateApplied (object sender, EventArgs e)
		{
			AddDeleteAnimation		lAddDeleteAnimation = sender as AddDeleteAnimation;
			UpdateAnimation			lUpdateAnimation = sender as UpdateAnimation;
			AddDeleteAnimationFrame	lAddDeleteFrame = sender as AddDeleteAnimationFrame;
			ReorderAnimationFrame	lReorderFrame = sender as ReorderAnimationFrame;

			if (lAddDeleteAnimation != null)
			{
				TreeViewMain.ShowAnimationNames (CharacterFile);
				if (lAddDeleteAnimation.IsDelete)
				{
					ShowSelectedTreeNode ();
				}
			}
			else if ((lUpdateAnimation != null) && lUpdateAnimation.NameChanged)
			{
				TreeNode	lAnimationNode = (new PartsTreeSelection (lUpdateAnimation.Target)).SelectedNode ();

				if (lAnimationNode != null)
				{
					lAnimationNode.Text = lUpdateAnimation.Target.Name;
				}
			}
			else if (lAddDeleteFrame != null)
			{
				TreeNode	lAnimationNode = (new PartsTreeSelection (lAddDeleteFrame.Animation)).SelectedNode ();

				if (lAnimationNode != null)
				{
					TreeViewMain.ShowAnimationFrames (lAddDeleteFrame.Animation, lAnimationNode);
				}
				if (lAddDeleteFrame.IsDelete)
				{
					ShowSelectedTreeNode ();
				}
			}
			else if (lReorderFrame != null)
			{
				TreeNode	lAnimationNode = (new PartsTreeSelection (lReorderFrame.Animation)).SelectedNode ();

				if (lAnimationNode != null)
				{
					TreeViewMain.ShowAnimationFrames (lReorderFrame.Animation, lAnimationNode);
				}
			}

			if (UpdateApplied != null)
			{
				UpdateApplied (sender, e);
			}
		}

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
					Program.UndoManager.Clear ();
					mCharacterFile = lCharacterFile;

					ShowFileState ();
					ShowEditState ();
					TreeViewMain.ShowAnimationNames (CharacterFile);
					TreeViewMain.LoadExpansion ();

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

		///////////////////////////////////////////////////////////////////////////////

		private void ShowSelectedTreeNode ()
		{
			if (TreeViewMain.SelectedNode == null)
			{
				TreeViewMain.SelectedNode = TreeViewMain.GetRootNode (PartsTreeView.NodeNameCharacter);
			}
			if (TreeViewMain.SelectedNode != null)
			{
				try
				{
					ShowSelectedPart (TreeViewMain.GetSelectedPart ());
				}
				finally
				{
					ResumeLayout (true);
				}
			}
		}

		private void ShowSelectedPart (PartsTreeSelection pSelectedPart)
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
			TextBox					lTextBox = null;
			Control					lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs	lEventArgs = null;

			if (lActive == TreeViewMain)
			{
				TreeViewMain.GetSelectedPart ().CanEdit (lEventArgs = new Global.EditEventArgs (DataFormats.Serializable));
			}
			else if (CanEdit != null)
			{
				CanEdit (this, lEventArgs = new Global.EditEventArgs (DataFormats.Serializable));
			}

			if ((lEventArgs != null) && lEventArgs.IsUsed)
			{
				if (!String.IsNullOrEmpty (lEventArgs.CopyObjectTitle))
				{
					MenuItemEditCopy.Enabled = true;
					MenuItemEditCopy.Text = String.Format (lEventArgs.CopyTitle, lEventArgs.CopyObjectTitle);
				}
				else
				{
					MenuItemEditCopy.Enabled = false;
					MenuItemEditCopy.Text = Properties.Resources.EditCopy;
				}
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (lEventArgs.CutObjectTitle))
				{
					MenuItemEditCut.Enabled = true;
					MenuItemEditCut.Text = String.Format (lEventArgs.CutTitle, lEventArgs.CutObjectTitle);
				}
				else
				{
					MenuItemEditCut.Enabled = false;
					MenuItemEditCut.Text = Properties.Resources.EditCut;
				}
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (lEventArgs.DeleteObjectTitle))
				{
					MenuItemEditDelete.Enabled = true;
					MenuItemEditDelete.Text = String.Format (lEventArgs.DeleteTitle, lEventArgs.DeleteObjectTitle);
				}
				else
				{
					MenuItemEditDelete.Enabled = false;
					MenuItemEditDelete.Text = Properties.Resources.EditDelete;
				}
				if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (lEventArgs.PasteObjectTitle))
				{
					if (!String.IsNullOrEmpty (lEventArgs.PasteTitle))
					{
						MenuItemEditPaste.Enabled = true;
						MenuItemEditPaste.Text = String.Format (lEventArgs.PasteTitle, lEventArgs.PasteObjectTitle);
					}
					else
					{
						MenuItemEditPaste.Enabled = false;
						MenuItemEditPaste.Text = lEventArgs.PasteObjectTitle;
					}
				}
				else
				{
					MenuItemEditPaste.Enabled = false;
					MenuItemEditPaste.Text = Properties.Resources.EditPaste;
				}
			}
			else if (lTextBox != null)
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
				MenuItemEditCopy.Enabled = false;
				MenuItemEditDelete.Enabled = false;
				MenuItemEditCut.Enabled = false;
				MenuItemEditPaste.Enabled = false;
				MenuItemEditCopy.Text = Properties.Resources.EditCopy;
				MenuItemEditDelete.Text = Properties.Resources.EditDelete;
				MenuItemEditCut.Text = Properties.Resources.EditCut;
				MenuItemEditPaste.Text = Properties.Resources.EditPaste;
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
				MenuItemEditUndo.Enabled = Program.UndoManager.CanUndo;
				MenuItemEditRedo.Enabled = Program.UndoManager.CanRedo;
				MenuItemEditUndo.Text = Program.UndoManager.CanUndo ? String.Format (Properties.Resources.EditUndoThis, Program.UndoManager.UndoName) : Properties.Resources.EditUndo;
				MenuItemEditRedo.Text = Program.UndoManager.CanRedo ? String.Format (Properties.Resources.EditRedoThis, Program.UndoManager.RedoName) : Properties.Resources.EditRedo;
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
			else if (Program.UndoManager.Undo ())
			{
				ShowFileState ();
				ShowEditState ();
			}
		}

		private void MenuItemEditRedo_Click (object sender, EventArgs e)
		{
			if (Program.UndoManager.Redo ())
			{
				ShowFileState ();
				ShowEditState ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemEditCopy_Click (object sender, EventArgs e)
		{
			TextBox					lTextBox = null;
			Control					lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs	lEventArgs = null;

			if (lActive == TreeViewMain)
			{
				TreeViewMain.GetSelectedPart ().EditCopy (lEventArgs = new Global.EditEventArgs ());
			}
			else if (EditCopy != null)
			{
				EditCopy (this, lEventArgs = new Global.EditEventArgs ());
			}

			if (lTextBox != null && ((lEventArgs == null) || !lEventArgs.IsUsed))
			{
				lTextBox.Copy ();
				ValidateChildren ();
			}
			if ((lEventArgs != null) && !lEventArgs.IsUsed)
			{
				ShowEditState ();
			}
		}

		private void MenuItemEditCut_Click (object sender, EventArgs e)
		{
			TextBox					lTextBox = null;
			Control					lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs	lEventArgs = null;

			if (lActive == TreeViewMain)
			{
				TreeViewMain.GetSelectedPart ().EditCut (lEventArgs = new Global.EditEventArgs ());
			}
			else if (EditCut != null)
			{
				EditCut (this, lEventArgs = new Global.EditEventArgs ());
			}

			if (lTextBox != null && ((lEventArgs == null) || !lEventArgs.IsUsed))
			{
				lTextBox.Cut ();
				ValidateChildren ();
			}
			if ((lEventArgs != null) && !lEventArgs.IsUsed)
			{
				ShowEditState ();
			}
		}

		private void MenuItemEditDelete_Click (object sender, EventArgs e)
		{
			TextBox					lTextBox = null;
			Control					lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs	lEventArgs = null;

			if (lActive == TreeViewMain)
			{
				TreeViewMain.GetSelectedPart ().EditDelete (lEventArgs = new Global.EditEventArgs ());
			}
			else if (EditDelete != null)
			{
				EditDelete (this, lEventArgs = new Global.EditEventArgs ());
			}

			if (lTextBox != null && ((lEventArgs == null) || !lEventArgs.IsUsed))
			{
				lTextBox.Paste (String.Empty);
				ValidateChildren ();
			}
			if ((lEventArgs != null) && !lEventArgs.IsUsed)
			{
				ShowEditState ();
			}
		}

		private void MenuItemEditPaste_Click (object sender, EventArgs e)
		{
			TextBox					lTextBox = null;
			Control					lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs	lEventArgs = null;

			if (lActive == TreeViewMain)
			{
				TreeViewMain.GetSelectedPart ().EditPaste (lEventArgs = new Global.EditEventArgs (DataFormats.Serializable));
			}
			else if (EditPaste != null)
			{
				EditPaste (this, lEventArgs = new Global.EditEventArgs (DataFormats.Serializable));
			}

			if (lTextBox != null && ((lEventArgs == null) || !lEventArgs.IsUsed))
			{
				lTextBox.Paste ();
				ValidateChildren ();
			}
			if ((lEventArgs != null) && !lEventArgs.IsUsed)
			{
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
			FileIsDirty ();
			ToolStripTop.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("[{0}] Undone", e.UndoUnit.ToString ());
#endif
		}

		private void UndoManager_Redone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
			ToolStripTop.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("[{0}] Redone", e.UndoUnit.ToString ());
#endif
		}

		///////////////////////////////////////////////////////////////////////////////

		private void PanelAnimations_GoToAnimation (object sender, Global.AnimationEventArgs e)
		{
			TreeNode	lAnimationNode = (new PartsTreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				TreeViewMain.SelectedNode = lAnimationNode;
				ShowSelectedTreeNode ();
			}
		}

		private void PanelAnimation_GoToFrame (object sender, Global.AnimationFrameEventArgs e)
		{
			TreeNode	lFrameNode = (new PartsTreeSelection (e.Animation, e.Frame)).SelectedNode ();

			if (lFrameNode != null)
			{
				TreeViewMain.SelectedNode = lFrameNode;
				ShowSelectedTreeNode ();
			}
		}

		private void PanelAnimation_GoToState (object sender, Global.StateEventArgs e)
		{
			TreeNode	lStateNode = (new PartsTreeSelection (e.StateName)).SelectedNode ();

			if (lStateNode != null)
			{
				TreeViewMain.SelectedNode = lStateNode;
				ShowSelectedTreeNode ();
			}
		}

		private void PanelState_GoToAnimation (object sender, Global.AnimationEventArgs e)
		{
			TreeNode	lAnimationNode = (new PartsTreeSelection (e.Animation)).SelectedNode ();

			if (lAnimationNode != null)
			{
				TreeViewMain.SelectedNode = lAnimationNode;
				ShowSelectedTreeNode ();
			}
		}

		#endregion
	}
}
