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
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Panels;
using AppResources = AgentCharacterEditor.Resources;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class MainWindow
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		private CharacterFile mCharacterFile = null;
		private RecentFileList mRecentFiles = new RecentFileList ();
		private Stack<KeyValuePair<FilePartPanel, Object>> mNavigateBackStack = new Stack<KeyValuePair<FilePartPanel, Object>> ();
		private Stack<KeyValuePair<FilePartPanel, Object>> mNavigateForwardStack = new Stack<KeyValuePair<FilePartPanel, Object>> ();

		protected void InitializeCommon ()
		{
			PanelPartsTree.Navigate += new NavigationEventHandler (OnNavigate);

			mRecentFiles.RecentItemClick += new RecentFileList.RecentItemClickEventHandler (RecentFiles_RecentItemClick);

			Program.UndoManager.Undone += new UndoManager.UndoneEventHandler (UndoManager_Undone);
			Program.UndoManager.Redone += new UndoManager.RedoneEventHandler (UndoManager_Redone);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void LoadAllConfig ()
		{
			try
			{
				LoadConfig (this, EventArgs.Empty);
				LoadRecentFiles ();
				LoadWindowState ();
			}
			catch
			{
			}
		}

		private void SaveAllConfig ()
		{
			try
			{
				SaveConfig (this, EventArgs.Empty);
				SaveWindowState ();
				SaveRecentFiles ();
			}
			catch
			{
			}
			SaveSettings ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
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

		///////////////////////////////////////////////////////////////////////////////

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
						//lCharacterFile.Header.ImageSize = PictureBoxSample.DefaultImageSize;
						lCharacterFile.Header.Guid = System.Guid.NewGuid ();
						lCharacterFile.Names.Add (CharacterPanel.mLangDefault, AppResources.Resources.TitleNewFile);
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
					ShowFileException (pException);
					lCursorState.RestoreCursor ();
					return false;
				}
			}

			try
			{
				if (lOpened)
				{
					IsWindowShowing = true;

					Program.UndoManager.Clear ();
					mNavigateBackStack.Clear ();
					mNavigateForwardStack.Clear ();
					mCharacterFile = lCharacterFile;

					ShowFileState ();
					ShowEditState ();
					ShowNavigateState ();

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

					IsWindowShowing = false;
					lRet = true;
				}
				else
				{
					ShowFileInvalid (pFilePath);
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
						ShowFileException (pException);
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
						ShowFileSaveError (pSaveAsPath);
					}
				}
				else
				{
					ShowFileSaveInvalid (pSaveAsPath);
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
					ShowFileException (pException);
				}
			}

			if (lCursorState != null)
			{
				lCursorState.RestoreCursor ();
			}
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
		#region Navigation

		private void ShowSelectedPart ()
		{
			ShowSelectedPart (PanelPartsTree.SelectedPart);
		}

		private void ShowSelectedPart (ResolvePart pSelectedPart)
		{
			FilePartPanel lSelectedPanel = null;

			if (pSelectedPart is ResolveCharacter)
			{
				ResolveCharacter lResolveCharacter = pSelectedPart as ResolveCharacter;
				if (lResolveCharacter.Scope == ResolveCharacter.ScopeType.ScopeCharacter)
				{
					lSelectedPanel = PanelCharacter;
				}
				else if (lResolveCharacter.Scope == ResolveCharacter.ScopeType.ScopeAnimations)
				{
					lSelectedPanel = PanelAnimations;
				}
			}
			else if (pSelectedPart is ResolveBalloon)
			{
				lSelectedPanel = PanelBalloon;
			}
			else if (pSelectedPart is ResolveTts)
			{
				lSelectedPanel = PanelTts;
			}
			else if (pSelectedPart is ResolveAnimation)
			{
				PanelAnimation.FilePart = pSelectedPart;
				lSelectedPanel = PanelAnimation;
			}
			else if (pSelectedPart is ResolveAnimationFrame)
			{
				ResolveAnimationFrame lResolveAnimationFrame = pSelectedPart as ResolveAnimationFrame;
				switch (lResolveAnimationFrame.Scope)
				{
					case ResolveAnimationFrame.ScopeType.ScopeFrame:
						{
							PanelFrame.FilePart = pSelectedPart;
							lSelectedPanel = PanelFrame;
						}
						break;
					case ResolveAnimationFrame.ScopeType.ScopeBranching:
						{
							PanelBranching.FilePart = pSelectedPart;
							lSelectedPanel = PanelBranching;
						}
						break;
					case ResolveAnimationFrame.ScopeType.ScopeOverlays:
						{
							PanelOverlays.FilePart = pSelectedPart;
							lSelectedPanel = PanelOverlays;
						}
						break;
				}
			}
			else if (pSelectedPart is ResolveState)
			{
				PanelState.FilePart = pSelectedPart;
				lSelectedPanel = PanelState;
			}

			ShowSelectedPanel (lSelectedPanel);
		}

		private void ShowSelectedPanel (FilePartPanel pSelectedPanel)
		{
			PanelCharacter.IsPanelVisible = Object.ReferenceEquals (PanelCharacter, pSelectedPanel);
			PanelBalloon.IsPanelVisible = Object.ReferenceEquals (PanelBalloon, pSelectedPanel);
			PanelTts.IsPanelVisible = Object.ReferenceEquals (PanelTts, pSelectedPanel);
			PanelAnimations.IsPanelVisible = Object.ReferenceEquals (PanelAnimations, pSelectedPanel);
			PanelAnimation.IsPanelVisible = Object.ReferenceEquals (PanelAnimation, pSelectedPanel);
			PanelFrame.IsPanelVisible = Object.ReferenceEquals (PanelFrame, pSelectedPanel);
			PanelBranching.IsPanelVisible = Object.ReferenceEquals (PanelBranching, pSelectedPanel);
			PanelOverlays.IsPanelVisible = Object.ReferenceEquals (PanelOverlays, pSelectedPanel);
			PanelState.IsPanelVisible = Object.ReferenceEquals (PanelState, pSelectedPanel);
		}

		///////////////////////////////////////////////////////////////////////////////

		private KeyValuePair<FilePartPanel, Object> GetNavigationContext ()
		{
			if (PanelCharacter.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelCharacter, PanelCharacter.NavigationContext);
			}
			else if (PanelBalloon.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelBalloon, PanelBalloon.NavigationContext);
			}
			else if (PanelTts.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelTts, PanelTts.NavigationContext);
			}
			else if (PanelAnimations.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelAnimations, PanelAnimations.NavigationContext);
			}
			else if (PanelAnimation.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelAnimation, PanelAnimation.NavigationContext);
			}
			else if (PanelFrame.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelFrame, PanelFrame.NavigationContext);
			}
			else if (PanelBranching.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelBranching, PanelBranching.NavigationContext);
			}
			else if (PanelOverlays.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelOverlays, PanelOverlays.NavigationContext);
			}
			else if (PanelState.IsPanelVisible)
			{
				return new KeyValuePair<FilePartPanel, Object> (PanelState, PanelState.NavigationContext);
			}

			return new KeyValuePair<FilePartPanel, Object> (null, null);
		}

		private Boolean ShowNavigationContext (KeyValuePair<FilePartPanel, Object> pContext)
		{
			if (pContext.Key != null)
			{
				ShowSelectedPanel (pContext.Key);
				pContext.Key.NavigationContext = pContext.Value;

				if (!pContext.Key.IsPanelEmpty)
				{
					PanelPartsTree.SelectedPart = pContext.Key.FilePart;
					return true;
				}
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handler Implementation

		private void HandleFileNew ()
		{
			if (PromptSaveCharacterFile ())
			{
				OpenCharacterFile (String.Empty);
			}
		}

		private void HandleFileOpen ()
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

		private Boolean CanHandleFileSave
		{
			get
			{
				return (mCharacterFile != null) && (mCharacterFile.IsDirty || String.IsNullOrEmpty (mCharacterFile.Path));
			}
		}
		private void HandleFileSave ()
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

		private Boolean CanHandleFileSaveAs
		{
			get
			{
				return (mCharacterFile != null);
			}
		}
		private void HandleFileSaveAs ()
		{
			String lFilePath = String.Empty;

			if ((mCharacterFile != null) && GetSaveAsPath (ref lFilePath))
			{
				if (String.Compare (mCharacterFile.Path, lFilePath, true) == 0)
				{
					ShowFileSaveAsInvalid (lFilePath);
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

		private void HandleEditUndo ()
		{
			if (Program.UndoManager.CanUndo)
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

		private void HandleEditRedo ()
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

		private Boolean CanHandleNavigateBack
		{
			get{return mNavigateBackStack.Count > 0;}
		}
		private void HandleNavigateBack ()
		{
			KeyValuePair<FilePartPanel, Object> lContext = GetNavigationContext ();
			while (mNavigateBackStack.Count > 0)
			{
				if (ShowNavigationContext (mNavigateBackStack.Pop ()))
				{
					mNavigateForwardStack.Push (lContext);
					break;
				}
			}
			ShowNavigateState ();
		}

		private Boolean CanHandleNavigateForward
		{
			get{return mNavigateForwardStack.Count > 0;}
		}
		private void HandleNavigateForward ()
		{
			KeyValuePair<FilePartPanel, Object> lContext = GetNavigationContext ();
			while (mNavigateForwardStack.Count > 0)
			{
				if (ShowNavigationContext (mNavigateForwardStack.Pop ()))
				{
					mNavigateBackStack.Push (lContext);
					break;
				}
			}
			ShowNavigateState ();
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
					if (PromptRemoveRecent (e))
					{
						if (mRecentFiles.RemovePath (e))
						{
							UpdateRecentFiles ();
						}
					}
				}
			}
		}

		private void OnNavigate (object sender, NavigationEventArgs e)
		{
			if (!IsWindowShowing)
			{
				mNavigateBackStack.Push (GetNavigationContext ());
				mNavigateForwardStack.Clear ();
			}
			ShowSelectedPart (e.Part);
			if (!Object.ReferenceEquals (sender, PanelPartsTree))
			{
				PanelPartsTree.SelectedPart = e.Part;
			}
			ShowEditState ();
			ShowNavigateState ();
		}

		#endregion
	}
}
