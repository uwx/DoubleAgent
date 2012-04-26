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
using System.Collections.Generic;
using System.Windows;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Updates;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class PartsTreePanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public const String ItemNameCharacter = "NodeCharacter";
		public const String ItemNameWordBalloon = "NodeWordBalloon";
		public const String ItemNameTTSMode = "NodeTTSMode";
		public const String ItemNameAnimations = "NodeAnimations";
		public const String ItemNameStates = "NodeStates";

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			LoadExpansion ();
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			SaveExpansion ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event NavigationEventHandler Navigate;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		protected override Boolean HandleCanEditCopy (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileBalloon)
				{
					pEventArgs.PutCopyTitle (AppResources.Resources.TitleBalloon);
				}
				else if (lSelectedObject is FileTts)
				{
					pEventArgs.PutCopyTitle (AppResources.Resources.TitleTts);
				}
				else if (lSelectedObject is FileAnimation)
				{
					pEventArgs.PutCopyTitle (Titles.Animation (lSelectedObject as FileAnimation));
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					pEventArgs.PutCopyTitle (Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ());
				}
				else if ((lSelectedPart is ResolveState) && (lSelectedObject != null))
				{
					pEventArgs.PutCopyTitle (Titles.State ((lSelectedPart as ResolveState).StateName));
				}
				return true;
			}
			return false;
		}

		protected override Boolean HandleEditCopy (EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject != null)
				{
					pEventArgs.PutCopyObject (lSelectedObject);
				}
				return true;
			}
			return false;
		}

		//=============================================================================

		protected override Boolean HandleCanEditCut (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					pEventArgs.PutCutTitle (Titles.Animation (lSelectedObject as FileAnimation));
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					pEventArgs.PutCutTitle (Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ());
				}
				return true;
			}
			return false;
		}

		protected override bool HandleEditCut (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					if (pEventArgs.PutCopyObject (lSelectedObject))
					{
						Program.MainWindow.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					}
					return true;
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					if (pEventArgs.PutCopyObject (lSelectedObject))
					{
						Program.MainWindow.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					}
					return true;
				}
			}
			return false;
		}

		//=============================================================================

		protected override Boolean HandleCanEditDelete (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					pEventArgs.PutDeleteTitle (Titles.Animation (lSelectedObject as FileAnimation));
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					pEventArgs.PutDeleteTitle (Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ());
				}
				return true;
			}
			return false;
		}

		protected override bool HandleEditDelete (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					Program.MainWindow.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
					return true;
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					Program.MainWindow.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
					return true;
				}
			}
			return false;
		}

		//=============================================================================

		protected override Boolean HandleCanEditPaste (CanEditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs) && (pEventArgs.GetPasteObject () != null))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if ((pEventArgs.GetPasteObject () is FileBalloon) && (lSelectedPart is ResolveBalloon))
				{
					pEventArgs.PutPasteTitle (AppResources.Resources.TitleBalloon);
				}
				else if ((pEventArgs.GetPasteObject () is FileTts) && (lSelectedPart is ResolveTts))
				{
					pEventArgs.PutPasteTitle (AppResources.Resources.TitleTts);
				}
				else if (pEventArgs.GetPasteObject () is FileAnimation)
				{
					if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lSelectedObject as FileAnimation, Titles.Animation (lSelectedObject as FileAnimation), Titles.Animation (pEventArgs.GetPasteObject () as FileAnimation)));
					}
					else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
					{
						if (Program.MainWindow.PanelAnimations.HasNewAnimationName && !CharacterFile.Gestures.Contains (Program.MainWindow.PanelAnimations.NewAnimationName))
						{
							pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (null, Program.MainWindow.PanelAnimations.NewAnimationName.Quoted (), Titles.Animation (pEventArgs.GetPasteObject () as FileAnimation)));
						}
						else
						{
							pEventArgs.PutPasteTitle (null, AppResources.Resources.EditPasteAnimation0);
						}
					}
				}
				else if (pEventArgs.GetPasteObject () is FileAnimationFrame)
				{
					if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
					{
						pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (lSelectedObject as FileAnimationFrame, Titles.FrameAnimation (lSelectedObject as FileAnimationFrame), Titles.Frame (pEventArgs.GetPasteObject () as FileAnimationFrame)));

					}
					else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						pEventArgs.PutPasteTitle (Titles.PasteTypeTitle (null, Titles.Frame (pEventArgs.GetPasteObject () as FileAnimationFrame)));
					}
				}
				else if ((pEventArgs.GetPasteObject () is FileState) && (lSelectedPart is ResolveState))
				{
					pEventArgs.PutPasteTitle (Titles.State (pEventArgs.GetPasteObject () as FileState));
				}
				return true;
			}
			return false;
		}

		protected override bool HandleEditPaste (Global.EditEventArgs pEventArgs)
		{
			if (IsControlEditTarget (TreeViewMain, pEventArgs))
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if ((pEventArgs.GetPasteObject () is FileBalloon) && (lSelectedPart is ResolveBalloon))
				{
					Program.MainWindow.PanelBalloon.PasteBalloon (pEventArgs.GetPasteObject () as FileBalloon);
					return true;
				}
				else if ((pEventArgs.GetPasteObject () is FileTts) && (lSelectedPart is ResolveTts))
				{
					Program.MainWindow.PanelTts.PasteTts (pEventArgs.GetPasteObject () as FileTts);
					return true;
				}
				else if (pEventArgs.GetPasteObject () is FileAnimation)
				{
					if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						Program.MainWindow.PanelAnimations.PasteSelectedAnimation (lSelectedObject as FileAnimation, pEventArgs.GetPasteObject () as FileAnimation);
						return true;
					}
					else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
					{
						Program.MainWindow.PanelAnimations.PasteSelectedAnimation (null, pEventArgs.GetPasteObject () as FileAnimation);
						return true;
					}
				}
				else if (pEventArgs.GetPasteObject () is FileAnimationFrame)
				{
					if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
					{
						Program.MainWindow.PanelAnimation.PasteSelectedFrame ((lSelectedObject as FileAnimationFrame).Animation, lSelectedObject as FileAnimationFrame, pEventArgs.GetPasteObject () as FileAnimationFrame);
						return true;
					}
					else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
					{
						Program.MainWindow.PanelAnimation.PasteSelectedFrame (lSelectedObject as FileAnimation, null, pEventArgs.GetPasteObject () as FileAnimationFrame);
						return true;
					}
				}
				else if ((pEventArgs.GetPasteObject () is FileState) && (lSelectedPart is ResolveState))
				{
					Program.MainWindow.PanelState.PasteStateAnimations ((lSelectedPart as ResolveState).StateName, (pEventArgs.GetPasteObject () as FileState).AnimationNames);
					return true;
				}
			}
			return false;
		}

		//=============================================================================

		protected override void UpdateApplied (object pUpdate)
		{
			AddDeleteAnimation lAddDeleteAnimation = pUpdate as AddDeleteAnimation;
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			AddDeleteAnimationFrame lAddDeleteFrame = pUpdate as AddDeleteAnimationFrame;
			ReorderAnimationFrame lReorderFrame = pUpdate as ReorderAnimationFrame;

			if (lAddDeleteAnimation != null)
			{
				ShowAnimationNames ();
			}
			else if ((lUpdateAnimation != null) && (lUpdateAnimation.NameChanged || lUpdateAnimation.IsForClipboard))
			{
				if (lUpdateAnimation.NameChanged)
				{
					RefreshAnimationName (lUpdateAnimation.Target);
				}
				if (lUpdateAnimation.IsForClipboard)
				{
					RefreshAnimationFrames (lUpdateAnimation.Target);
				}
			}
			else if (lAddDeleteFrame != null)
			{
				RefreshAnimationFrames (lAddDeleteFrame.Animation);
			}
			else if (lReorderFrame != null)
			{
				RefreshAnimationFrames (lReorderFrame.Animation);
			}
		}

		#endregion
	}
}