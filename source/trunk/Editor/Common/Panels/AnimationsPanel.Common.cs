/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class AnimationsPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			protected set
			{
				base.FilePart = value;
				if (FilePart is ResolveCharacter)
				{
					(FilePart as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeAnimations;
				}
			}
		}

		protected override Boolean TrackUpdatesWhenHidden
		{
			get
			{
				return true;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event NavigationEventHandler Navigate;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Navigation

		public override object NavigationContext
		{
			get
			{
				return new PanelContext (this);
			}
			set
			{
				if (value is PanelContext)
				{
					(value as PanelContext).RestoreContext (this);
				}
				else
				{
					base.NavigationContext = value;
				}
			}
		}

		public new class PanelContext : FilePartPanel.PanelContext
		{
			public PanelContext (AnimationsPanel pPanel)
				: base (pPanel)
			{
				SelectedAnimation = pPanel.ListViewAnimations.SelectedIndex;
			}

			public void RestoreContext (AnimationsPanel pPanel)
			{
				base.RestoreContext (pPanel);
				pPanel.ListViewAnimations.SelectedIndex = SelectedAnimation;
			}

			public int SelectedAnimation
			{
				get;
				protected set;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public override void ShowFilePart (ResolvePart pFilePart)
		{
			try
			{
				FilePart = pFilePart;

				ShowAnimationNames ();
				ShowFrameProperties ();
				ShowPalette ();
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		private void ShowAnimationNames ()
		{
			PanelContext lContext = IsPanelFilling ? null : new PanelContext (this);

			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					ShowAnimationNames (null);
					ListViewAnimations.IsEnabled = false;
					TextBoxNewName.Clear ();
					TextBoxNewName.IsEnabled = false;
					LabelNewName.IsEnabled = false;
				}
				else
				{
					ShowAnimationNames (CharacterFile.GetAnimationNames ());
					ListViewAnimations.IsEnabled = true;
					TextBoxNewName.IsEnabled = !Program.FileIsReadOnly;
					LabelNewName.IsEnabled = !Program.FileIsReadOnly;
				}
				if (lContext != null)
				{
					lContext.RestoreContext (this);
				}
				ShowAddState ();
				ShowDeleteState ();
			}
		}

		//=============================================================================

		private void ShowAddState ()
		{
			ButtonAdd.IsEnabled = CanAddAnimation;
			ButtonAdd.SetTitle (AddAnimationTitle);
		}

		private void ShowDeleteState ()
		{
			ButtonDelete.IsEnabled = CanDeleteAnimation;
			ButtonDelete.SetTitle (DeleteAnimationTitle);
		}

		//=============================================================================

		private Boolean CanAddAnimation
		{
			get
			{
				if (IsPanelEmpty || Program.FileIsReadOnly)
				{
					return false;
				}
				else if (HasNewAnimationName && !CharacterFile.Gestures.Contains (NewAnimationName))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		private String AddAnimationTitle
		{
			get
			{
				if (IsPanelEmpty || Program.FileIsReadOnly)
				{
					return AppResources.Resources.EditAddAnimation;
				}
				else if (HasNewAnimationName && !CharacterFile.Gestures.Contains (NewAnimationName))
				{
					return String.Format (AppResources.Resources.EditAddAnimation1, NewAnimationName);
				}
				else
				{
					return AppResources.Resources.EditAddAnimation0;
				}
			}
		}

		private Boolean CanDeleteAnimation
		{
			get
			{
				if (IsPanelEmpty || Program.FileIsReadOnly)
				{
					return false;
				}
				else
				{
					return (GetSelectedAnimation () != null);
				}
			}
		}
		private String DeleteAnimationTitle
		{
			get
			{
				if (IsPanelEmpty || Program.FileIsReadOnly)
				{
					return String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Animation ((FileAnimation)null));
				}
				else
				{
					return String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Animation (GetSelectedAnimation ()));
				}
			}
		}

		//=============================================================================

		internal String NewAnimationName
		{
			get
			{
				return TextBoxNewName.Text.Trim ();
			}
		}
		internal Boolean HasNewAnimationName
		{
			get
			{
				return !String.IsNullOrEmpty (NewAnimationName);
			}
		}

		//=============================================================================

		private void ShowFrameProperties ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					NumericFrameWidth.Clear ();
					NumericFrameHeight.Clear ();

					NumericFrameWidth.IsEnabled = false;
					NumericFrameHeight.IsEnabled = false;
					LabelWidth.IsEnabled = false;
					LabelHeight.IsEnabled = false;
				}
				else
				{
					NumericFrameWidth.Value = CharacterFile.Header.ImageSize.Width;
					NumericFrameHeight.Value = CharacterFile.Header.ImageSize.Height;

					NumericFrameWidth.IsEnabled = !Program.FileIsReadOnly;
					NumericFrameHeight.IsEnabled = !Program.FileIsReadOnly;
					LabelWidth.IsEnabled = !Program.FileIsReadOnly;
					LabelHeight.IsEnabled = !Program.FileIsReadOnly;
				}
				NumericFrameWidth.IsModified = false;
				NumericFrameHeight.IsModified = false;
			}
		}

		//=============================================================================

		private void ShowPalette ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				System.Drawing.Imaging.ColorPalette lPalette = null;

				if (CharacterFile == null)
				{
					TextBoxPaletteFile.Clear ();
					TextBoxPaletteFile.IsEnabled = false;
					LabelPaletteFile.IsEnabled = false;
					ButtonPaletteImport.IsEnabled = false;
					ButtonPaletteExport.IsEnabled = false;
				}
				else
				{
					TextBoxPaletteFile.Text = CharacterFile.PaletteFilePath;
					TextBoxPaletteFile.IsEnabled = !Program.FileIsReadOnly;
					LabelPaletteFile.IsEnabled = !Program.FileIsReadOnly;
					ButtonPaletteImport.IsEnabled = !Program.FileIsReadOnly;
					ButtonPaletteExport.IsEnabled = false;

					lPalette = CharacterFile.Header.Palette;
				}
				TextBoxPaletteFile.IsModified = false;

				if ((lPalette != null) && (lPalette.Entries.Length == 256))
				{
					ShowPaletteImage (MakePaletteBitmap (lPalette));
					ShowPaletteTransparency (CharacterFile.Header.Transparency, GetPaletteColor (lPalette, CharacterFile.Header.Transparency));
				}
				else
				{
					ShowPaletteImage (null);
					ShowPaletteTransparency (-1, System.Drawing.Color.Empty);
				}
				ShowSelectedTransparency (-1, System.Drawing.Color.Empty);
			}
		}

		private void ShowSelectedTransparency (int pTransparencyNdx)
		{
			System.Drawing.Imaging.ColorPalette lPalette = null;

			if (CharacterFile != null)
			{
				lPalette = CharacterFile.Header.Palette;
			}
			if (lPalette != null)
			{
				ShowSelectedTransparency (pTransparencyNdx, GetPaletteColor (lPalette, pTransparencyNdx));
			}
			else
			{
				ShowSelectedTransparency (-1, System.Drawing.Color.Empty);
			}
		}

		//=============================================================================

		private static System.Drawing.Bitmap MakePaletteBitmap (System.Drawing.Imaging.ColorPalette pPalette)
		{
			System.Drawing.Bitmap lBitmap = new System.Drawing.Bitmap (256, 256, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
			System.Drawing.Color[] lColors = pPalette.Entries;
			int lColorNdx = 0;
			System.Drawing.Imaging.BitmapData lData;
			Byte[] lBytes;

			try
			{
				lBitmap.Palette = pPalette;
				lData = lBitmap.LockBits (new System.Drawing.Rectangle (0, 0, lBitmap.Width, lBitmap.Height), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
				lBytes = new byte[lData.Stride * lData.Height];
				System.Runtime.InteropServices.Marshal.Copy (lData.Scan0, lBytes, 0, lData.Stride * lData.Height);

				for (lColorNdx = 0; lColorNdx < lData.Stride * lData.Height; lColorNdx++)
				{
					lBytes[lColorNdx] = (Byte)((lColorNdx / (lData.Stride * 16) * 16) + ((lColorNdx % lData.Stride) / 16));
				}

				System.Runtime.InteropServices.Marshal.Copy (lBytes, 0, lData.Scan0, lData.Stride * lData.Height);
				lBitmap.UnlockBits (lData);

				return lBitmap;
			}
			catch
			{
			}

			return null;
		}

		private static System.Drawing.Color GetPaletteColor (System.Drawing.Imaging.ColorPalette pPalette, int pColorNdx)
		{
			System.Drawing.Color[] lColors = pPalette.Entries;

			if ((pColorNdx >= 0) && (pColorNdx < lColors.Length))
			{
				return lColors[pColorNdx];
			}
			return System.Drawing.Color.Empty;
		}

		private static System.Drawing.Color GetContrastingColor (System.Drawing.Color pColor)
		{
			return (pColor.GetBrightness () > 0.5) ? System.Drawing.Color.Black : System.Drawing.Color.White;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate DeleteSelectedAnimation (FileAnimation pAnimation, Boolean pForClipboard)
		{
			if ((pAnimation != null) && !Program.FileIsReadOnly)
			{
				using (PanelFillingState lFillingState = new PanelFillingState (this))
				{
					AddDeleteAnimation lUpdate = new AddDeleteAnimation (pAnimation, pForClipboard);

					if (AddDeleteAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimation, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedAnimation (FileAnimation pAnimation, FileAnimation pPasteAnimation)
		{
			if ((CharacterFile != null) && (pPasteAnimation != null) && !Program.FileIsReadOnly)
			{
				String lAnimationName = NewAnimationName;

				if (pAnimation == null)
				{
					if (String.IsNullOrEmpty (lAnimationName))
					{
						System.Media.SystemSounds.Beep.Play ();
					}
					else if (CharacterFile.Gestures.Contains (lAnimationName))
					{
						Program.ShowWarningMessage (String.Format (AppResources.Resources.MsgDuplicateAnimation, lAnimationName));
					}
					else
					{
						AddDeleteAnimation lUpdate = new AddDeleteAnimation (lAnimationName, true);

						lUpdate = lUpdate.Apply () as AddDeleteAnimation;
						if (lUpdate != null)
						{
							pPasteAnimation.CopyTo (lUpdate.Target, true);
							lUpdate = lUpdate.Apply () as AddDeleteAnimation;
						}
						if (lUpdate != null)
						{
							using (PanelFillingState lFillingState = new PanelFillingState (this))
							{
								if (AddDeleteAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimation, this))
								{
									SelectAnimation (lAnimationName);
									return lUpdate;
								}
							}
						}
					}
				}
				else
				{
					UpdateAnimation lUpdate = new UpdateAnimation (pAnimation, true);

					pPasteAnimation.CopyTo (pAnimation, true);
					lUpdate = lUpdate.Apply () as UpdateAnimation;
					if ((lUpdate != null) && !UpdateAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimation, this))
					{
						lUpdate = null;
					}
				}
			}
			return null;
		}

		//=============================================================================

		protected override bool HandleCanEditCopy (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewAnimations, pEventArgs))
			{
				FileAnimation lAnimation = GetSelectedAnimation ();

				if (lAnimation != null)
				{
					pEventArgs.PutCopyTitle (Titles.Animation (lAnimation));
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCopy (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewAnimations, pEventArgs))
			{
				FileAnimation lAnimation = GetSelectedAnimation ();

				if (lAnimation != null)
				{
					pEventArgs.PutCopyObject (lAnimation);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditCut (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewAnimations, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileAnimation lAnimation = GetSelectedAnimation ();

				if (lAnimation != null)
				{
					pEventArgs.PutCutTitle (Titles.Animation (lAnimation));
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewAnimations, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileAnimation lAnimation = GetSelectedAnimation ();

				if (lAnimation != null)
				{
					if (pEventArgs.PutCopyObject (lAnimation))
					{
						DeleteSelectedAnimation (lAnimation, true);
					}
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditDelete (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewAnimations, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileAnimation lAnimation = GetSelectedAnimation ();

				if (lAnimation != null)
				{
					pEventArgs.PutDeleteTitle (Titles.Animation (lAnimation));
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewAnimations, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileAnimation lAnimation = GetSelectedAnimation ();

				if (lAnimation != null)
				{
					DeleteSelectedAnimation (lAnimation, false);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditPaste (CanEditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileAnimation))
			{
				FileAnimation lAnimation = null;

				if (IsControlEditTarget (ListViewAnimations, pEventArgs))
				{
					lAnimation = GetSelectedAnimation ();
				}
				if ((lAnimation != null) && IsControlEditTarget (ListViewAnimations, pEventArgs))
				{
					pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lAnimation, Titles.Animation (lAnimation), Titles.Animation (pEventArgs.GetPasteObject () as FileAnimation)));
					return true;
				}
				else if (IsControlEditTarget (ListViewAnimations, pEventArgs) || IsControlEditTarget (TextBoxNewName, pEventArgs))
				{
					if (HasNewAnimationName && !CharacterFile.Gestures.Contains (NewAnimationName))
					{
						pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (null, NewAnimationName.Quoted (), Titles.Animation (pEventArgs.GetPasteObject () as FileAnimation)));
					}
					else
					{
						pEventArgs.PutPasteTitle (AppResources.Resources.EditPasteAnimation0, null);
					}
					return true;
				}
			}
			return false;
		}
		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (!Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileAnimation))
			{
				if (IsControlEditTarget (ListViewAnimations, pEventArgs))
				{
					FileAnimation lAnimation = GetSelectedAnimation ();

					PasteSelectedAnimation (lAnimation, pEventArgs.GetPasteObject () as FileAnimation);
					return true;
				}
				else if (HasNewAnimationName)
				{
					PasteSelectedAnimation (null, pEventArgs.GetPasteObject () as FileAnimation);
					return true;
				}
			}
			return false;
		}

		//=============================================================================

		private void HandleAddAnimation ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				String lAnimationName = NewAnimationName;
				AddDeleteAnimation lUpdate;

				if (String.IsNullOrEmpty (lAnimationName))
				{
					System.Media.SystemSounds.Beep.Play ();
				}
				else if (CharacterFile.Gestures.Contains (lAnimationName))
				{
					Program.ShowWarningMessage (String.Format (AppResources.Resources.MsgDuplicateAnimation, lAnimationName));
				}
				else
				{
					using (PanelFillingState lFillingState = new PanelFillingState (this))
					{
						lUpdate = new AddDeleteAnimation (lAnimationName, false);
						if (AddDeleteAnimation.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteAnimation, this))
						{
							SelectAnimation (lAnimationName);
						}
						else
						{
							System.Media.SystemSounds.Beep.Play ();
						}
					}
				}
			}
		}

		private void HandleDeleteAnimation ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				int lSelNdx = ListViewAnimations.SelectedIndex;
				FileAnimation lAnimation = GetSelectedAnimation (ListViewAnimations.SelectedItem as ListViewItemCommon);

				if (DeleteSelectedAnimation (lAnimation, false) != null)
				{
					ListViewAnimations.SelectedIndex = Math.Min (lSelNdx, ListViewAnimations.Items.Count - 1);
				}
				else
				{
					System.Media.SystemSounds.Beep.Play ();
				}
			}
		}

		//=============================================================================

		private void HandleUpdateFrameWidth ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.ImageSize = new System.Drawing.Size ((int)NumericFrameWidth.Value, CharacterFile.Header.ImageSize.Height);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
			NumericFrameWidth.IsModified = false;
		}

		private void HandleUpdateFrameHeight ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.ImageSize = new System.Drawing.Size (CharacterFile.Header.ImageSize.Width, (int)NumericFrameHeight.Value);
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
			NumericFrameHeight.IsModified = false;
		}

		private void HandleUpdatePaletteFile ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate = new UpdateCharacterHeader ();

				lUpdate.PaletteFilePath = TextBoxPaletteFile.Text;
				UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
			}
			TextBoxPaletteFile.IsModified = false;
		}

		private void HandleImportPalette ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				String lFilePath = CharacterFile.PaletteFilePath;
				UpdateCharacterHeader lUpdate;

				if (OpenFileDialogEx.OpenPaletteFile (ref lFilePath))
				{
					lUpdate = new UpdateCharacterHeader ();
					lUpdate.PaletteFilePath = lFilePath;
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
				}
			}
		}

		private void HandleUpdatePaletteTransparency (int pTransparencyNdx)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterHeader lUpdate;

				if ((pTransparencyNdx >= 0) && (pTransparencyNdx <= 255))
				{
					lUpdate = new UpdateCharacterHeader ();
					lUpdate.PaletteTransparency = (Byte)pTransparencyNdx;
					UpdateCharacterHeader.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterHeader, this);
				}
			}
		}

		//=============================================================================

		protected override void UpdateApplied (Object pUpdate)
		{
			UpdateCharacterHeader lUpdateCharacter = pUpdate as UpdateCharacterHeader;
			AddDeleteAnimation lAddDeleteAnimation = pUpdate as AddDeleteAnimation;
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;

			if ((lUpdateCharacter != null) && (lUpdateCharacter.CharacterFile == CharacterFile))
			{
				if (lUpdateCharacter.ImageSizeChanged)
				{
					ShowFrameProperties ();
				}
				if (lUpdateCharacter.PaletteChanged || lUpdateCharacter.PaletteTransparencyChanged)
				{
					ShowPalette ();
				}
			}
			else if ((lAddDeleteAnimation != null) && (lAddDeleteAnimation.CharacterFile == CharacterFile))
			{
				ShowAnimationNames ();
				if (IsPanelFilling && !lAddDeleteAnimation.IsDelete)
				{
					SelectAnimation (lAddDeleteAnimation.Target.Name);
				}
			}
			else if ((lUpdateAnimation != null) && lUpdateAnimation.NameChanged)
			{
				ShowAnimationNames ();
			}
		}

		#endregion
	}
}
