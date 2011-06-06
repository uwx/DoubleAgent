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
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class OverlayPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			set
			{
				base.FilePart = value;
				if (FilePart is ResolveAnimationFrame)
				{
					(FilePart as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeOverlays;
				}
				Frame = (FilePart is ResolveAnimationFrame) ? (FilePart as ResolveAnimationFrame).Target : null;

				ShowFrameName ();
				ShowFrameOverlays ();
				if (!IsPanelEmpty)
				{
					SelectFirstOverlay ();
					ShowSelectedOverlay ();
				}
			}
		}

		protected FileAnimationFrame Frame
		{
			get;
			set;
		}

		protected FileAnimation Animation
		{
			get
			{
				return (Frame == null) ? null : Frame.Animation;
			}
		}

		protected String FrameTitle
		{
			get
			{
				return Titles.Frame (Frame);
			}
		}

		public override Boolean IsPanelEmpty
		{
			get
			{
				return base.IsPanelEmpty || (Frame == null);
			}
		}

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

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameName ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
			{
				TextBoxFrameName.Clear ();
				TextBoxFrameName.IsEnabled = false;
			}
			else
			{
				TextBoxFrameName.Text = Titles.FrameAnimation (Frame);
				TextBoxFrameName.IsEnabled = true;
			}

			PopIsPanelFilling (lWasFilling);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowFrameOverlay (FileFrameOverlay pFrameOverlay)
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (pFrameOverlay == null)
			{
				NumericOffsetX.Clear ();
				NumericOffsetY.Clear ();
				CheckBoxReplace.IsChecked = false;

				NumericOffsetX.IsEnabled = false;
				NumericOffsetY.IsEnabled = false;
				CheckBoxReplace.IsEnabled = false;
			}
			else
			{
				NumericOffsetX.Value = pFrameOverlay.Offset.X;
				NumericOffsetY.Value = pFrameOverlay.Offset.Y;
				CheckBoxReplace.IsChecked = pFrameOverlay.ReplaceFlag;

				NumericOffsetX.IsEnabled = !Program.FileIsReadOnly;
				NumericOffsetY.IsEnabled = !Program.FileIsReadOnly;
				CheckBoxReplace.IsEnabled = !Program.FileIsReadOnly;
			}
			NumericOffsetX.IsModified = false;
			NumericOffsetY.IsModified = false;

			ShowFrameSample (pFrameOverlay);
		}

		private void ShowSelectedOverlay ()
		{
			ShowFrameOverlay (GetSelectedOverlay (true));
			ShowSelectionState (GetSelectedOverlay (false), ListViewOverlays.SelectedIndex);
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean SelectFirstOverlay ()
		{
			for (int lNdx = 0; lNdx < ListViewOverlays.Items.Count; lNdx++)
			{
				if (GetSelectedOverlay (lNdx) != null)
				{
					ListViewOverlays.SelectedIndex = lNdx;
					return true;
				}
			}
			return false;
		}

		private FileFrameOverlay GetSelectedOverlay (Boolean pIncludeFocus)
		{
			return GetSelectedOverlay (ListViewOverlays.GetSelectedIndex (pIncludeFocus));
		}

		private FileFrameOverlay GetSelectedOverlay (int pOverlayNdx)
		{
			FileFrameOverlay lFrameOverlay = null;

			if ((Frame != null) && (Frame.Overlays != null))
			{
				if (Frame.Overlays.Contains ((MouthOverlay)pOverlayNdx))
				{
					lFrameOverlay = Frame.Overlays[(MouthOverlay)pOverlayNdx];
				}
			}
			return lFrameOverlay;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private RepeatingUpdate<UpdateFrameOverlay, FileFrameOverlay> mRepeatUpdateFrameOverlay = new RepeatingUpdate<UpdateFrameOverlay, FileFrameOverlay> ();

		///////////////////////////////////////////////////////////////////////////////

		private UndoableUpdate UpdateSelectedOverlay (FileFrameOverlay pFrameOverlay)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly)
			{
				int lOverlayNdx = ListViewOverlays.SelectedIndex;
				String lFilePath = String.Empty;

				if (pFrameOverlay != null)
				{
					lFilePath = pFrameOverlay.ImageFilePath;
				}
				if (OpenFileDialogEx.OpenImageFile (ref lFilePath))
				{
					if (pFrameOverlay != null)
					{
						UpdateFrameOverlay lUpdate = new UpdateFrameOverlay (pFrameOverlay, false);

						lUpdate.OverlayImagePath = lFilePath;
						if (UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameOverlay, this))
						{
							return lUpdate;
						}
					}
					else if ((lOverlayNdx >= 0) && !String.IsNullOrEmpty (lFilePath))
					{
						AddDeleteFrameOverlay lUpdate = new AddDeleteFrameOverlay (Frame, (MouthOverlay)lOverlayNdx, lFilePath, false);

						if (AddDeleteFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteFrameOverlay, this))
						{
							return lUpdate;
						}
					}
				}
			}
			return null;
		}

		internal UndoableUpdate DeleteSelectedOverlay (FileFrameOverlay pFrameOverlay, Boolean pForClipboard)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly && (pFrameOverlay != null))
			{
				AddDeleteFrameOverlay lUpdate = new AddDeleteFrameOverlay (pFrameOverlay, pForClipboard);

				if (AddDeleteFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteFrameOverlay, this))
				{
					return lUpdate;
				}
			}
			return null;
		}

		internal UndoableUpdate PasteSelectedOverlay (FileFrameOverlay pFrameOverlay, FileFrameOverlay pPasteOverlay)
		{
			if (!IsPanelEmpty && !Program.FileIsReadOnly && (pPasteOverlay != null))
			{
				if (pFrameOverlay == null)
				{
					int lOverlayNdx = ListViewOverlays.SelectedIndex;

					if (lOverlayNdx >= 0)
					{
						AddDeleteFrameOverlay lUpdate = new AddDeleteFrameOverlay (Frame, (MouthOverlay)lOverlayNdx, String.Empty, true);

						lUpdate = lUpdate.Apply () as AddDeleteFrameOverlay;
						if (lUpdate != null)
						{
							pPasteOverlay.CopyTo (lUpdate.Target);
							lUpdate = lUpdate.Apply () as AddDeleteFrameOverlay;
						}
						if ((lUpdate != null) && AddDeleteFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as AddDeleteFrameOverlay, this))
						{
							return lUpdate;
						}
					}
				}
				else
				{
					UpdateFrameOverlay lUpdate = new UpdateFrameOverlay (pFrameOverlay, true);

					pPasteOverlay.CopyTo (pFrameOverlay);
					lUpdate = lUpdate.Apply () as UpdateFrameOverlay;
					if ((lUpdate != null) && UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameOverlay, this))
					{
						return lUpdate;
					}
				}
			}
			return null;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override bool HandleCanEditCopy (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs))
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					pEventArgs.PutCopyTitle (Titles.OverlayFrameAnimation (lFrameOverlay).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCopy (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs))
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					pEventArgs.PutCopyObject (lFrameOverlay);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditCut (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					pEventArgs.PutCutTitle (Titles.OverlayFrameAnimation (lFrameOverlay).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					if (pEventArgs.PutCopyObject (lFrameOverlay))
					{
						DeleteSelectedOverlay (lFrameOverlay, true);
					}
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditDelete (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					pEventArgs.PutDeleteTitle (Titles.OverlayFrameAnimation (lFrameOverlay).Quoted ());
				}
				return true;
			}
			return false;
		}
		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs) && !Program.FileIsReadOnly)
			{
				FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);

				if (lFrameOverlay != null)
				{
					DeleteSelectedOverlay (lFrameOverlay, false);
					return true;
				}
			}
			return false;
		}

		protected override bool HandleCanEditPaste (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs) && !Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileFrameOverlay))
			{
				pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (pEventArgs.GetPasteObject () as FileFrameOverlay, Titles.Overlay ((pEventArgs.GetPasteObject () as FileFrameOverlay).OverlayType), Titles.Overlay ((MouthOverlay)ListViewOverlays.SelectedIndex)));
				return true;
			}
			return false;
		}
		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (ListViewOverlays, pEventArgs) && !Program.FileIsReadOnly && (pEventArgs.GetPasteObject () is FileFrameOverlay))
			{
				PasteSelectedOverlay (GetSelectedOverlay (false), pEventArgs.GetPasteObject () as FileFrameOverlay);
				return true;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleNumericOffsetXChanged ()
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new System.Drawing.Point ((int)NumericOffsetX.Value, lFrameOverlay.Offset.Y);
				UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameOverlay, this);
			}
		}

		private void HandleNumericOffsetYChanged ()
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.Offset = new System.Drawing.Point (lFrameOverlay.Offset.X, (int)NumericOffsetY.Value);
				UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameOverlay, this);
			}
		}

		private void HandleReplaceChanged ()
		{
			FileFrameOverlay lFrameOverlay = GetSelectedOverlay (false);
			UpdateFrameOverlay lUpdate;

			if (!IsPanelEmpty && !Program.FileIsReadOnly && (lFrameOverlay != null))
			{
				lUpdate = new UpdateFrameOverlay (lFrameOverlay, false);
				lUpdate.ReplaceFlag = CheckBoxReplace.IsChecked.Value;
				UpdateFrameOverlay.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateFrameOverlay, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void HandleShiftUp ()
		{
		}

		private void HandleShiftDown ()
		{
		}

		private void HandleShiftLeft ()
		{
		}

		private void HandleShiftRight ()
		{
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			AddDeleteFrameOverlay lAddDeleteOverlay = pUpdate as AddDeleteFrameOverlay;
			UpdateFrameOverlay lUpdateOverlay = pUpdate as UpdateFrameOverlay;
			UpdateAnimationFrame lUpdateFrame = pUpdate as UpdateAnimationFrame;

			if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == this.Animation) && lUpdateAnimation.NameChanged)
			{
				ShowFrameName ();
			}
			else if ((lAddDeleteOverlay != null) && (lAddDeleteOverlay.Frame == Frame))
			{
				PanelContext lContext = new PanelContext (this);
				ShowFrameOverlays ();
				lContext.RestoreContext (this);
				ShowSelectedOverlay ();
			}
			else if ((lUpdateOverlay != null) && (lUpdateOverlay.Frame == Frame))
			{
				RefreshOverlayItem (lUpdateOverlay.Target);
				ShowSelectedOverlay ();
			}
			else if ((lUpdateFrame != null) && (lUpdateFrame.Target == Frame) && lUpdateFrame.ForClipboard)
			{
				PanelContext lContext = new PanelContext (this);
				ShowFrameOverlays ();
				lContext.RestoreContext (this);
				ShowSelectedOverlay ();
			}
		}

		#endregion
	}
}
