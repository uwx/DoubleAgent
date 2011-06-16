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
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Panels;
using DoubleAgent;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor
{
	public partial class MainWindow : Form
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private String mInitialFile = null;

		public MainWindow ()
		{
			InitializeComponent ();
		}

		public MainWindow (String[] args)
		{
			if ((args != null) && (args.Length > 0))
			{
				mInitialFile = args[0];
			}
		}

		~MainWindow ()
		{
			Application.Idle -= new EventHandler (Application_Idle);
		}

		public void Initialize ()
		{
			InitializeComponent ();
			InitializeCommon ();
			InitializeContextMenu (ContextMenuEdit);

			IsWindowShowing = false;
			Icon = Properties.Resources.IconDoubleAgent;
			ToolStripMain.Left = ToolStripNavigation.Right;

			MainMenuStrip = MenuStripMain;
			Application.Idle += new EventHandler (Application_Idle);
		}

		public void InitializeContextMenu (ContextMenuEdit pContextMenu)
		{
			pContextMenu.InitializeComponent ();
			pContextMenu.MenuItemCut.Click += new EventHandler (MenuItemEditCut_Click);
			pContextMenu.MenuItemCopy.Click += new EventHandler (MenuItemEditCopy_Click);
			pContextMenu.MenuItemDelete.Click += new EventHandler (MenuItemEditDelete_Click);
			pContextMenu.MenuItemPaste.Click += new EventHandler (MenuItemEditPaste_Click);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void LoadWindowState ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
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
		}

		private void LoadRecentFiles ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
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
			mRecentFiles.ShowPaths (MenuItemFile.DropDown, MenuItemFilePrintPreview);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void SaveSettings ()
		{
			Settings.Default.Save ();
		}

		private void SaveWindowState ()
		{
			Settings lSettings = Settings.Default;

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
			Settings lSettings = Settings.Default;

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

		protected Boolean IsWindowShowing
		{
			get
			{
				return !CausesValidation;
			}
			set
			{
				CausesValidation = !value && Visible;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event Global.CanEditEventHandler CanEdit;
		public event Global.EditEventHandler EditCopy;
		public event Global.EditEventHandler EditCut;
		public event Global.EditEventHandler EditDelete;
		public event Global.EditEventHandler EditPaste;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region File

		private Boolean PromptSaveCharacterFile ()
		{
			Boolean lRet = true;
#if	!DEBUG
			if (mCharacterFile.IsDirty && !mCharacterFile.IsReadOnly)
			{
				String lMessage;

				if (String.IsNullOrEmpty (mCharacterFile.Path))
				{
					String lNewName = GetSaveNewName ();

					if (String.IsNullOrEmpty (lNewName))
					{
						lNewName = AppResources.Resources.TitleNewFile;
					}
					lMessage = String.Format (AppResources.Resources.MsgSaveNew, lNewName);
				}
				else
				{
					lMessage = String.Format (AppResources.Resources.MsgSaveFile, mCharacterFile.FileName);
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

		private Boolean PromptRemoveRecent (String pFilePath)
		{
			if (MessageBox.Show (String.Format ("Remove {0} from the recent files list?", pFilePath), Program.AssemblyTitle, MessageBoxButtons.YesNo) == DialogResult.Yes)
			{
				return true;
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		private void FadeShowSelectedPanel (FilePartPanel pSelectedPanel)
		{
			ShowSelectedPanel (pSelectedPanel);
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
				this.Text = String.Format (mCharacterFile.IsDirty ? AppResources.Resources.TitleDirtyFile : AppResources.Resources.TitleOpenFile, Program.AssemblyTitle, String.IsNullOrEmpty (mCharacterFile.Path) ? AppResources.Resources.TitleNewFile : mCharacterFile.Path);
				MenuItemFileSave.Enabled = CanHandleFileSave;
				MenuItemFileSaveAs.Enabled = CanHandleFileSaveAs;
				ToolButtonFileSave.Enabled = MenuItemFileSave.Enabled;
			}
		}

		public void ShowNavigateState ()
		{
			MenuItemNavigateBack.Enabled = CanHandleNavigateBack;
			MenuItemNavigateForward.Enabled = CanHandleNavigateForward;
			ToolButtonNavigateBack.Enabled = MenuItemNavigateBack.Enabled;
			ToolButtonNavigateForward.Enabled = MenuItemNavigateForward.Enabled;
		}

		public void ShowEditState ()
		{
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);
			Boolean lMenuInitialized = false;

			if ((lTextBox == null) || (CanEdit != null))
			{
				Global.CanEditEventArgs lEventArgs = new Global.CanEditEventArgs ();

				if (CanEdit != null)
				{
					try
					{
						CanEdit (this, lEventArgs);
					}
					catch
					{
					}
				}

				lMenuInitialized = lEventArgs.IsUsed;

				if ((lTextBox == null) || lMenuInitialized)
				{
					lEventArgs.UpdateMenuItems (MenuItemEditCopy, MenuItemEditCut, MenuItemEditDelete, MenuItemEditPaste);
					lEventArgs.UpdateToolStripButtons (ToolButtonEditCopy, ToolButtonEditCut, ToolButtonEditDelete, ToolButtonEditPaste);
				}
			}

			if ((lTextBox != null) && !lMenuInitialized)
			{
				MenuItemEditCopy.Enabled = (lTextBox.SelectionLength > 0);
				MenuItemEditCut.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
				MenuItemEditDelete.Enabled = !lTextBox.ReadOnly && (lTextBox.SelectionLength > 0);
				MenuItemEditPaste.Enabled = !lTextBox.ReadOnly && (Clipboard.ContainsText (TextDataFormat.Text) || Clipboard.ContainsText (TextDataFormat.UnicodeText));

				MenuItemEditCopy.Text = MenuItemEditCopy.Enabled ? AppResources.Resources.EditCopyText : AppResources.Resources.EditCopy;
				MenuItemEditCut.Text = MenuItemEditCut.Enabled ? AppResources.Resources.EditCutText : AppResources.Resources.EditCut;
				MenuItemEditDelete.Text = MenuItemEditDelete.Enabled ? AppResources.Resources.EditDeleteText : AppResources.Resources.EditDelete;
				MenuItemEditPaste.Text = MenuItemEditPaste.Enabled ? AppResources.Resources.EditPasteText : AppResources.Resources.EditPaste;

				ToolButtonEditCopy.Enabled = MenuItemEditCopy.Enabled;
				ToolButtonEditCut.Enabled = MenuItemEditCut.Enabled;
				ToolButtonEditPaste.Enabled = MenuItemEditPaste.Enabled;
				ToolButtonEditDelete.Enabled = MenuItemEditDelete.Enabled;
				ToolButtonEditCopy.Text = MenuItemEditCopy.Text.NoMenuPrefix ();
				ToolButtonEditCut.Text = MenuItemEditCut.Text.NoMenuPrefix ();
				ToolButtonEditPaste.Text = MenuItemEditPaste.Text.NoMenuPrefix ();
				ToolButtonEditDelete.Text = MenuItemEditDelete.Text.NoMenuPrefix ();

				MenuItemEditCopy.Text = MenuItemEditCopy.Text.FixMenuPrefix ();
				MenuItemEditDelete.Text = MenuItemEditDelete.Text.FixMenuPrefix ();
				MenuItemEditCut.Text = MenuItemEditCut.Text.FixMenuPrefix ();
				MenuItemEditPaste.Text = MenuItemEditPaste.Text.FixMenuPrefix ();
			}

			if ((lTextBox != null) && lTextBox.Modified)
			{
				MenuItemEditUndo.Enabled = lTextBox.CanUndo;
				MenuItemEditRedo.Enabled = false;
				MenuItemEditUndo.Text = MenuItemEditUndo.Enabled ? AppResources.Resources.EditUndoText.FixMenuPrefix () : AppResources.Resources.EditUndo.FixMenuPrefix ();
				MenuItemEditRedo.Text = AppResources.Resources.EditRedo.FixMenuPrefix ();

				ToolButtonEditUndo.Enabled = MenuItemEditUndo.Enabled;
				ToolButtonEditRedo.Enabled = MenuItemEditRedo.Enabled;
				ToolButtonEditUndo.Text = ToolButtonEditUndo.Enabled ? AppResources.Resources.EditUndoText.NoMenuPrefix () : AppResources.Resources.EditUndo.NoMenuPrefix ();
				ToolButtonEditRedo.Text = AppResources.Resources.EditRedo.NoMenuPrefix ();
			}
			else
			{
				MenuItemEditUndo.Enabled = Program.UndoManager.CanUndo;
				MenuItemEditRedo.Enabled = Program.UndoManager.CanRedo;
				MenuItemEditUndo.Text = Program.UndoManager.CanUndo ? String.Format (AppResources.Resources.EditUndoThis.FixMenuPrefix (), Program.UndoManager.UndoName) : AppResources.Resources.EditUndo.FixMenuPrefix ();
				MenuItemEditRedo.Text = Program.UndoManager.CanRedo ? String.Format (AppResources.Resources.EditRedoThis.FixMenuPrefix (), Program.UndoManager.RedoName) : AppResources.Resources.EditRedo.FixMenuPrefix ();

				ToolButtonEditUndo.Enabled = MenuItemEditUndo.Enabled;
				ToolButtonEditRedo.Enabled = MenuItemEditRedo.Enabled;
				ToolButtonEditUndo.Text = Program.UndoManager.CanUndo ? String.Format (AppResources.Resources.EditUndoThis.NoMenuPrefix (), Program.UndoManager.UndoName) : AppResources.Resources.EditUndo.NoMenuPrefix ();
				ToolButtonEditRedo.Text = Program.UndoManager.CanRedo ? String.Format (AppResources.Resources.EditRedoThis.NoMenuPrefix (), Program.UndoManager.RedoName) : AppResources.Resources.EditRedo.NoMenuPrefix ();
			}
		}

		public void ShowEditState (ContextMenuEdit pContextMenu)
		{
			if (CanEdit != null)
			{
				Global.CanEditEventArgs lEventArgs = new Global.CanEditEventArgs (pContextMenu);

				try
				{
					CanEdit (this, lEventArgs);
				}
				catch
				{
				}

				lEventArgs.UpdateMenuItems (pContextMenu.MenuItemCopy, pContextMenu.MenuItemCut, pContextMenu.MenuItemDelete, pContextMenu.MenuItemPaste);
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
			IsWindowShowing = false;
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

		private void ToolButtonNavigateBack_Click (object sender, EventArgs e)
		{
			HandleNavigateBack ();
		}

		private void ToolButtonNavigateForward_Click (object sender, EventArgs e)
		{
			HandleNavigateForward ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ToolButtonFileNew_Click (object sender, EventArgs e)
		{
			HandleFileNew ();
		}

		private void ToolButtonFileOpen_Click (object sender, EventArgs e)
		{
			HandleFileOpen ();
		}

		private void ToolButtonFileSave_Click (object sender, EventArgs e)
		{
			HandleFileSave ();
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
			(new AgentCharacterEditor.About.AboutBox ()).ShowDialog (this);
		}

		private void MenuItemNavigateBack_Click (object sender, EventArgs e)
		{
			HandleNavigateBack ();
		}

		private void MenuItemNavigateForward_Click (object sender, EventArgs e)
		{
			HandleNavigateForward ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void MenuItemFileNew_Click (object sender, EventArgs e)
		{
			HandleFileNew ();
		}

		private void MenuItemFileOpen_Click (object sender, EventArgs e)
		{
			HandleFileOpen ();
		}

		private void MenuItemFileSave_Click (object sender, EventArgs e)
		{
			HandleFileSave ();
		}

		private void MenuItemFileSaveAs_Click (object sender, EventArgs e)
		{
			HandleFileSaveAs ();
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
					ToolStripMain.TipTextChanged ();
					ValidateChildren ();
				}
			}
			else
			{
				HandleEditUndo ();
			}
		}

		private void MenuItemEditRedo_Click (object sender, EventArgs e)
		{
			HandleEditRedo ();
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
				EditPaste (this, lEventArgs = new Global.EditEventArgs ());
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

		private void ContextMenuStub_Opening (object sender, System.ComponentModel.CancelEventArgs e)
		{
			TextBox lTextBox = null;
			Control lActive = GetActiveControl (ref lTextBox);

			if (lTextBox != null)
			{
				e.Cancel = true;
			}
			else if ((lActive != null) && lActive.RectangleToScreen (lActive.ClientRectangle).Contains (Control.MousePosition))
			{
				ShowEditState (ContextMenuEdit);
			}
			else
			{
				e.Cancel = true;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Internal Event Handlers

		private void UndoManager_Undone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
			ToolStripMain.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] Undone", e.UndoUnit.ToString ());
#endif
		}

		private void UndoManager_Redone (object sender, UndoManager.EventArgs e)
		{
			FileIsDirty ();
			ToolStripMain.TipTextChanged ();
#if DEBUG
			System.Diagnostics.Debug.Print ("UndoUnit [{0}] Redone", e.UndoUnit.ToString ());
#endif
		}

		#endregion
	}
}
