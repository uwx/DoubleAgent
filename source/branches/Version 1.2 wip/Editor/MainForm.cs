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

namespace AgentCharacterEditor
{
	public partial class MainForm : Form
	{
		private CharacterFile mCharacterFile = null;
		private RecentFileList mRecentFiles = new RecentFileList ();
		private String mInitialFile = null;

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
			LoadWindowState ();
		}

		private void LoadWindowState ()
		{
			Properties.Settings lSettings = Properties.Settings.Default;

			if ((lSettings.MainFormSize.X > 0) && (lSettings.MainFormSize.Y > 0))
			{
				System.Drawing.Point lLocation = lSettings.MainFormLocation;
				System.Drawing.Size lSize = (System.Drawing.Size)lSettings.MainFormSize;

				lSize.Width = Math.Max (lSize.Width, MinimumSize.Width);
				lSize.Height = Math.Max (lSize.Height, MinimumSize.Height);
				Location = lLocation;
				Size = lSize;
			}
			WindowState = (lSettings.MainFormState == FormWindowState.Minimized) ? FormWindowState.Normal : lSettings.MainFormState;

			if (lSettings.MainFormSplit > 0)
			{
				SplitContainerMain.SplitterDistance = lSettings.MainFormSplit;
			}
		}

		private void LoadRecentFiles ()
		{
			Properties.Settings lSettings = Properties.Settings.Default;

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
			SaveWindowState ();
			SaveRecentFiles ();

			Properties.Settings.Default.Save ();
		}

		private void SaveWindowState ()
		{
			Properties.Settings lSettings = Properties.Settings.Default;

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
			lSettings.MainFormState = (WindowState == FormWindowState.Minimized) ? FormWindowState.Normal : WindowState;

			lSettings.MainFormSplit = SplitContainerMain.SplitterDistance;
		}

		private void SaveRecentFiles ()
		{
			Properties.Settings lSettings = Properties.Settings.Default;

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

		public event Global.CanEditEventHandler CanEdit;
		public event Global.EditEventHandler EditCopy;
		public event Global.EditEventHandler EditCut;
		public event Global.EditEventHandler EditDelete;
		public event Global.EditEventHandler EditPaste;
		public event Global.ContextMenuEventHandler EditMenu;

		public event UndoUnit.AppliedEventHandler UpdateApplied;

		///////////////////////////////////////////////////////////////////////////////

		public void OnUpdateApplied (object sender, EventArgs e)
		{
			if (UpdateApplied != null)
			{
				UpdateApplied (sender, e);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region File

		public void FileIsDirty ()
		{
			if (CharacterFile != null)
			{
				CharacterFile.IsDirty = true;
				ShowFileState ();
				ShowEditState ();
			}
		}

		private Boolean OpenCharacterFile (String pFilePath)
		{
			Boolean lRet = false;
			CursorState lCursorState = new CursorState (this);
			CharacterFile lCharacterFile = null;
			Boolean lOpened = false;

			try
			{
				if ((mCharacterFile != null) && !String.IsNullOrEmpty (pFilePath) && (String.Compare (System.IO.Path.GetFullPath (pFilePath), System.IO.Path.GetFullPath (mCharacterFile.Path), true) == 0))
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
				if (String.IsNullOrEmpty (pFilePath))
				{
					lCharacterFile = CharacterFile.CreateInstance (CharacterFile.AcdFileExt);
				}
				else
				{
					lCharacterFile = CharacterFile.CreateInstance (pFilePath);
				}
			}
			catch
			{
			}

			if (lCharacterFile != null)
			{
				try
				{
					if (String.IsNullOrEmpty (pFilePath))
					{
						lOpened = true;
						lCharacterFile.Header.ImageSize = PictureBoxSample.DefaultImageSize;
						lCharacterFile.Header.Guid = System.Guid.NewGuid ();
						lCharacterFile.Names.Add (CharacterPanel.mLangDefault, Properties.Resources.TitleNewFile);
						lCharacterFile.IsDirty = false;
					}
					else
					{
#if DEBUG_NOT
						lOpened = lCharacterFile.Open (pFilePath, 1);
#else
						lOpened = lCharacterFile.Open (pFilePath);
#endif
					}
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

					PanelPartsTree.FilePart = new ResolveCharacter ();
					PanelCharacter.FilePart = new ResolveCharacter ();
					PanelBalloon.FilePart = new ResolveBalloon ();
					PanelTts.FilePart = new ResolveTts ();
					PanelAnimations.FilePart = new ResolveCharacter ();
					PanelAnimation.FilePart = new ResolveCharacter ();
					PanelFrame.FilePart = new ResolveCharacter ();
					PanelBranching.FilePart = new ResolveCharacter ();
					PanelOverlays.FilePart = new ResolveCharacter ();
					PanelState.FilePart = new ResolveCharacter ();

					PanelPartsTree.LoadExpansion ();
					ShowSelectedPart ();
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

		private Boolean SaveCharacterFile ()
		{
			return SaveCharacterFile (String.Empty);
		}

		private Boolean SaveCharacterFile (String pSaveAsPath)
		{
			Boolean lRet = false;
			CursorState lCursorState = null;

			if (mCharacterFile == null)
			{
				lRet = true;
			}
			else if (!String.IsNullOrEmpty (pSaveAsPath))
			{
				CharacterFile lCharacterFile = null;

				lCursorState = new CursorState (this);
				lCursorState.ShowWait ();
				SaveAllConfig ();

				try
				{
					lCharacterFile = CharacterFile.CreateInstance (pSaveAsPath);
				}
				catch
				{
				}
				if (lCharacterFile != null)
				{
					try
					{
#if DEBUG_NOT
						lRet = lCharacterFile.Save (pSaveAsPath, mCharacterFile, 3);
#else
						lRet = lCharacterFile.Save (pSaveAsPath, mCharacterFile);
#endif
					}
					catch (Exception pException)
					{
						MessageBox.Show (pException.Message);
					}

					try
					{
						lCharacterFile.Close ();
						lCharacterFile = null;
					}
					catch
					{
					}

					if (!lRet)
					{
						MessageBox.Show (String.Format (Properties.Resources.MsgFailedSaveAs, pSaveAsPath), Program.AssemblyTitle, MessageBoxButtons.OK);
					}
				}
				else
				{
					MessageBox.Show ("Not implemented", Program.AssemblyTitle, MessageBoxButtons.OK);
				}
			}
			else if (String.IsNullOrEmpty (mCharacterFile.Path) && !mCharacterFile.IsReadOnly)
			{
				String lFilePath = String.Empty;

				if (GetSaveNewPath (ref lFilePath))
				{
					lRet = SaveCharacterFile (lFilePath);
				}
			}
			else if (mCharacterFile.IsDirty && !mCharacterFile.IsReadOnly)
			{
				lCursorState = new CursorState (this);
				lCursorState.ShowWait ();
				SaveAllConfig ();

				try
				{
#if DEBUG_NOT
					lRet = mCharacterFile.Save (3);
#else
					lRet = mCharacterFile.Save ();
#endif
				}
				catch (Exception pException)
				{
					MessageBox.Show (pException.Message);
				}
			}

			if (lCursorState != null)
			{
				lCursorState.RestoreCursor ();
			}
			return lRet;
		}

		private Boolean PromptSaveCharacterFile ()
		{
			Boolean lRet = true;
#if	!DEBUG
			if (mCharacterFile.IsDirty && !mCharacterFile.IsReadOnly)
			{
				String	lMessage;

				if (String.IsNullOrEmpty (mCharacterFile.Path))
				{
					String	lNewName = GetSaveNewName ();

					if (String.IsNullOrEmpty (lNewName))
					{
						lNewName = Properties.Resources.TitleNewFile;
					}
					lMessage = String.Format (Properties.Resources.MsgSaveNew, lNewName);
				}
				else
				{
					lMessage = String.Format (Properties.Resources.MsgSaveFile, mCharacterFile.FileName);
				}

				switch (MessageBox.Show (lMessage, Program.AssemblyTitle, MessageBoxButtons.YesNoCancel))
				{
					case DialogResult.Yes:
						{
							lRet = SaveCharacterFile ();
						}
						break;
					case DialogResult.Cancel:
						{
							lRet = false;
						}
						break;
				}
			}
#endif
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean GetOpenPath (ref String pFilePath)
		{
			pFilePath = String.Empty;

			if ((mCharacterFile != null) && !String.IsNullOrEmpty (mCharacterFile.Path))
			{
				try
				{
					pFilePath = System.IO.Path.Combine (System.IO.Path.GetDirectoryName (mCharacterFile.Path), "*" + System.IO.Path.GetExtension (mCharacterFile.Path));
				}
				catch
				{
				}
			}
			return OpenFileDialogEx.OpenCharacterFile (ref pFilePath);
		}

		private Boolean GetSaveAsPath (ref String pFilePath)
		{
			pFilePath = String.Empty;

			if ((mCharacterFile != null) && !String.IsNullOrEmpty (mCharacterFile.Path))
			{
				try
				{
					pFilePath = System.IO.Path.Combine (System.IO.Path.GetDirectoryName (mCharacterFile.Path), "*" + System.IO.Path.GetExtension (mCharacterFile.Path));
				}
				catch
				{
				}
				return OpenFileDialogEx.SaveCharacterFile (ref pFilePath);
			}
			else
			{
				return GetSaveNewPath (ref pFilePath);
			}
		}

		private Boolean GetSaveNewPath (ref String pFilePath)
		{
			pFilePath = GetSaveNewName ();
			if (String.IsNullOrEmpty (pFilePath))
			{
				pFilePath = "*";
			}
			pFilePath += CharacterFile.AcdFileExt;

			return OpenFileDialogEx.SaveCharacterFile (ref pFilePath);
		}

		private String GetSaveNewName ()
		{
			if (mCharacterFile != null)
			{
				FileCharacterName lName = mCharacterFile.FindName (0);

				if ((lName != null) && !String.IsNullOrEmpty (lName.Name))
				{
					return lName.Name;
				}
			}
			return String.Empty;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowSelectedPart ()
		{
			ShowSelectedPart (PanelPartsTree.SelectedPart);
		}

		private void ShowSelectedPart (ResolvePart pSelectedPart)
		{
			ResolveCharacter lResolveCharacter = pSelectedPart as ResolveCharacter;
			ResolveAnimationFrame lResolveAnimationFrame = pSelectedPart as ResolveAnimationFrame;

			if (pSelectedPart is ResolveAnimation)
			{
				PanelAnimation.FilePart = pSelectedPart;
			}
			else if (pSelectedPart is ResolveState)
			{
				PanelState.FilePart = pSelectedPart;
			}
			else if (lResolveAnimationFrame != null)
			{
				switch (lResolveAnimationFrame.Scope)
				{
					case ResolveAnimationFrame.ScopeType.ScopeFrame:
						{
							PanelFrame.FilePart = pSelectedPart;
						}
						break;
					case ResolveAnimationFrame.ScopeType.ScopeBranching:
						{
							PanelBranching.FilePart = pSelectedPart;
						}
						break;
					case ResolveAnimationFrame.ScopeType.ScopeOverlays:
						{
							PanelOverlays.FilePart = pSelectedPart;
						}
						break;
				}
			}

			PanelCharacter.Visible = (lResolveCharacter != null) && (lResolveCharacter.Scope == ResolveCharacter.ScopeType.ScopeCharacter);
			PanelBalloon.Visible = (pSelectedPart is ResolveBalloon);
			PanelTts.Visible = (pSelectedPart is ResolveTts);
			PanelAnimations.Visible = (lResolveCharacter != null) && (lResolveCharacter.Scope == ResolveCharacter.ScopeType.ScopeAnimations);
			PanelAnimation.Visible = (pSelectedPart is ResolveAnimation);
			PanelFrame.Visible = (lResolveAnimationFrame != null) && (lResolveAnimationFrame.Scope == ResolveAnimationFrame.ScopeType.ScopeFrame);
			PanelBranching.Visible = (lResolveAnimationFrame != null) && (lResolveAnimationFrame.Scope == ResolveAnimationFrame.ScopeType.ScopeBranching);
			PanelOverlays.Visible = (lResolveAnimationFrame != null) && (lResolveAnimationFrame.Scope == ResolveAnimationFrame.ScopeType.ScopeOverlays);
			PanelState.Visible = (pSelectedPart is ResolveState);
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
				this.Text = String.Format (mCharacterFile.IsDirty ? Properties.Resources.TitleDirtyFile : Properties.Resources.TitleOpenFile, Program.AssemblyTitle, String.IsNullOrEmpty (mCharacterFile.Path) ? Properties.Resources.TitleNewFile : mCharacterFile.Path);
				MenuItemFileSave.Enabled = (mCharacterFile.IsDirty || String.IsNullOrEmpty (mCharacterFile.Path));
				MenuItemFileSaveAs.Enabled = true;
				ToolButtonFileSave.Enabled = MenuItemFileSave.Enabled;
			}
		}

		public void ShowEditState ()
		{
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);
			Global.CanEditEventArgs lEventArgs = null;

			if (CanEdit != null)
			{
				lEventArgs = new Global.CanEditEventArgs (DataFormats.Serializable);
				CanEdit (this, lEventArgs);
			}

			if ((lEventArgs != null) && lEventArgs.IsUsed)
			{
				ShowEditState (lEventArgs, MenuItemEditCopy, MenuItemEditCut, MenuItemEditPaste, MenuItemEditDelete);
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
			ToolButtonEditCopy.Text = MenuItemEditCopy.Text.NoMenuPrefix ();
			ToolButtonEditCut.Text = MenuItemEditCut.Text.NoMenuPrefix ();
			ToolButtonEditPaste.Text = MenuItemEditPaste.Text.NoMenuPrefix ();
			ToolButtonEditDelete.Text = MenuItemEditDelete.Text.NoMenuPrefix ();

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
			ToolButtonEditUndo.Text = MenuItemEditUndo.Text.NoMenuPrefix ();
			ToolButtonEditRedo.Text = MenuItemEditRedo.Text.NoMenuPrefix ();
		}

		public void ShowEditState (Global.CanEditEventArgs pEditArgs, ToolStripItem pCopyItem, ToolStripItem pCutItem, ToolStripItem pPasteItem, ToolStripItem pDeleteItem)
		{
			if (!String.IsNullOrEmpty (pEditArgs.CopyObjectTitle))
			{
				pCopyItem.Enabled = true;
				pCopyItem.Text = String.Format (pEditArgs.CopyTitle, pEditArgs.CopyObjectTitle);
			}
			else
			{
				pCopyItem.Enabled = false;
				pCopyItem.Text = Properties.Resources.EditCopy;
			}
			if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (pEditArgs.CutObjectTitle))
			{
				pCutItem.Enabled = true;
				pCutItem.Text = String.Format (pEditArgs.CutTitle, pEditArgs.CutObjectTitle);
			}
			else
			{
				pCutItem.Enabled = false;
				pCutItem.Text = Properties.Resources.EditCut;
			}
			if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (pEditArgs.DeleteObjectTitle))
			{
				pDeleteItem.Enabled = true;
				pDeleteItem.Text = String.Format (pEditArgs.DeleteTitle, pEditArgs.DeleteObjectTitle);
			}
			else
			{
				pDeleteItem.Enabled = false;
				pDeleteItem.Text = Properties.Resources.EditDelete;
			}
			if (!Program.FileIsReadOnly && !String.IsNullOrEmpty (pEditArgs.PasteObjectTitle))
			{
				if (!String.IsNullOrEmpty (pEditArgs.PasteTitle))
				{
					pPasteItem.Enabled = true;
					pPasteItem.Text = String.Format (pEditArgs.PasteTitle, pEditArgs.PasteObjectTitle);
				}
				else
				{
					pPasteItem.Enabled = false;
					pPasteItem.Text = pEditArgs.PasteObjectTitle;
				}
			}
			else
			{
				pPasteItem.Enabled = false;
				pPasteItem.Text = Properties.Resources.EditPaste;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal Control GetActiveControl ()
		{
			try
			{
				Control lActive = this.ActiveControl;

				if ((lActive != null) && (lActive.Enabled))
				{
					while (!lActive.Focused && lActive.HasChildren && lActive.ContainsFocus)
					{
						ContainerControl lContainer = null;
						Control lContained = null;

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
			NumericUpDownEx lNumeric = null;
			Control lActive = GetActiveControl (ref pTextBox, ref lNumeric);

			if (lNumeric != null)
			{
				pTextBox = lNumeric.TextBox;
			}
			return lActive;
		}

		internal Control GetActiveControl (ref TextBox pTextBox, ref NumericUpDownEx pNumeric)
		{
			Control lActive = GetActiveControl ();

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
			if (!PromptSaveCharacterFile ())
			{
				e.Cancel = true;
			}
		}

		private void MainForm_Shown (object sender, EventArgs e)
		{
			if (!String.IsNullOrEmpty (mInitialFile))
			{
				Update ();
				OpenCharacterFile (mInitialFile);
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
		#region ToolStrip Event Handlers

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
			(new AgentCharacterEditor.AboutBox ()).ShowDialog (this);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemFileNew_Click (object sender, EventArgs e)
		{
			if (PromptSaveCharacterFile ())
			{
				OpenCharacterFile (String.Empty);
			}
		}

		private void MenuItemFileOpen_Click (object sender, EventArgs e)
		{
			if (PromptSaveCharacterFile ())
			{
				String lFilePath = String.Empty;

				if (GetOpenPath (ref lFilePath))
				{
					OpenCharacterFile (lFilePath);
				}
			}
		}

		private void MenuItemFileSave_Click (object sender, EventArgs e)
		{
			if (mCharacterFile != null)
			{
				if (String.IsNullOrEmpty (mCharacterFile.Path))
				{
					String lFilePath = String.Empty;

					if ((GetSaveNewPath (ref lFilePath)) && SaveCharacterFile (lFilePath))
					{
						OpenCharacterFile (lFilePath);
					}
				}
				else
				{
					SaveCharacterFile ();
				}
				ShowFileState ();
				ShowEditState ();
			}
		}

		private void MenuItemFileSaveAs_Click (object sender, EventArgs e)
		{
			String lFilePath = String.Empty;

			if ((mCharacterFile != null) && GetSaveAsPath (ref lFilePath))
			{
				if (String.Compare (mCharacterFile.Path, lFilePath, true) == 0)
				{
					MessageBox.Show (Properties.Resources.MsgInvalidSaveAs, Program.AssemblyTitle, MessageBoxButtons.OK);
				}
				else if (SaveCharacterFile (lFilePath))
				{
					OpenCharacterFile (lFilePath);
				}
				ShowFileState ();
				ShowEditState ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);

			if ((lTextBox != null) && lTextBox.Modified)
			{
				if (lTextBox.CanUndo)
				{
					lTextBox.Undo ();
					ToolStripTop.TipTextChanged ();
					ValidateChildren ();
				}
			}
			else if (Program.UndoManager.CanUndo)
			{
				if (Program.UndoManager.Undo ())
				{
					ShowFileState ();
					ShowEditState ();
				}
				else
				{
					System.Media.SystemSounds.Asterisk.Play ();
				}
			}
		}

		private void MenuItemEditRedo_Click (object sender, EventArgs e)
		{
			if (Program.UndoManager.CanRedo)
			{
				if (Program.UndoManager.Redo ())
				{
					ShowFileState ();
					ShowEditState ();
				}
				else
				{
					System.Media.SystemSounds.Asterisk.Play ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemEditCopy_Click (object sender, EventArgs e)
		{
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs lEventArgs = null;

			if (EditCopy != null)
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
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs lEventArgs = null;

			if (EditCut != null)
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
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs lEventArgs = null;

			if (EditDelete != null)
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
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);
			Global.EditEventArgs lEventArgs = null;

			if (EditPaste != null)
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
		#region Context Menu Event Handlers

		private void ContextMenuEdit_Opening (object sender, System.ComponentModel.CancelEventArgs e)
		{
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				e.Cancel = true;
			}
			else if ((lActive != null) && lActive.RectangleToScreen (lActive.ClientRectangle).Contains (Control.MousePosition))
			{
				Global.ContextMenuEventArgs lEventArgs = new Global.ContextMenuEventArgs (ContextMenuEdit, lActive, DataFormats.Serializable);

				if (EditMenu != null)
				{
					List<ToolStripItem> lRemove = new List<ToolStripItem> ();

					foreach (ToolStripItem lMenuItem in ContextMenuEdit.Items)
					{
						if (!Object.ReferenceEquals (lMenuItem, ContextItemEditCopy) && !Object.ReferenceEquals (lMenuItem, ContextItemEditCut) && !Object.ReferenceEquals (lMenuItem, ContextItemEditPaste) && !Object.ReferenceEquals (lMenuItem, ContextItemEditDelete))
						{
							lRemove.Add (lMenuItem);
						}
					}
					foreach (ToolStripItem lMenuItem in lRemove)
					{
						ContextMenuEdit.Items.Remove (lMenuItem);
					}

					try
					{
						EditMenu (this, lEventArgs);
					}
					catch
					{
					}
				}

				if (lEventArgs.IsUsed)
				{
					ShowEditState (lEventArgs, ContextItemEditCopy, ContextItemEditCut, ContextItemEditPaste, ContextItemEditDelete);
				}
				else
				{
					ShowEditState ();

					ContextItemEditCopy.Enabled = MenuItemEditCopy.Enabled;
					ContextItemEditCopy.Text = MenuItemEditCopy.Text;
					ContextItemEditCut.Enabled = MenuItemEditCut.Enabled;
					ContextItemEditCut.Text = MenuItemEditCut.Text;
					ContextItemEditPaste.Enabled = MenuItemEditPaste.Enabled;
					ContextItemEditPaste.Text = MenuItemEditPaste.Text;
					ContextItemEditDelete.Enabled = MenuItemEditDelete.Enabled;
					ContextItemEditDelete.Text = MenuItemEditDelete.Text;
				}
			}
			else
			{
				e.Cancel = true;
			}
		}

		private void ContextItemEditCopy_Click (object sender, EventArgs e)
		{
			MenuItemEditCopy_Click (sender, e);
		}

		private void ContextItemEditCut_Click (object sender, EventArgs e)
		{
			MenuItemEditCut_Click (sender, e);
		}

		private void ContextItemEditPaste_Click (object sender, EventArgs e)
		{
			MenuItemEditPaste_Click (sender, e);
		}

		private void ContextItemEditDelete_Click (object sender, EventArgs e)
		{
			MenuItemEditDelete_Click (sender, e);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		private void RecentFiles_RecentItemClick (object sender, String e)
		{
			if (PromptSaveCharacterFile ())
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
		}

		private void OnNavigate (object sender, Global.NavigationEventArgs e)
		{
			ShowSelectedPart (e.Part);
			ShowEditState ();
			if (!Object.ReferenceEquals (sender, PanelPartsTree))
			{
				PanelPartsTree.SelectedPart = e.Part;
			}
		}

		private void UndoManager_Undone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
			ToolStripTop.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] Undone", e.UndoUnit.ToString ());
#endif
		}

		private void UndoManager_Redone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
			ToolStripTop.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] Redone", e.UndoUnit.ToString ());
#endif
		}

		#endregion
	}
}
