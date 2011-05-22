using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Panels
{
	public partial class PartsTreePanel : AgentCharacterEditor.Panels.FilePartPanel
	{
		#region Initialization

		public const String ItemNameCharacter = "NodeCharacter";
		public const String ItemNameWordBalloon = "NodeWordBalloon";
		public const String ItemNameTTSMode = "NodeTTSMode";
		public const String ItemNameAnimations = "NodeAnimations";
		public const String ItemNameStates = "NodeStates";

		///////////////////////////////////////////////////////////////////////////////
		public PartsTreePanel ()
		{
			InitializeComponent ();
		}

		protected override void OnLoadConfig (object sender, EventArgs e)
		{
			LoadExpansion ();
		}

		protected override void OnSaveConfig (object sender, EventArgs e)
		{
			SaveExpansion ();
		}

		///////////////////////////////////////////////////////////////////////////////

		public void LoadExpansion ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				TreeViewItem lTreeItem;

				//TreeViewMain.BeginUpdate ();

				lTreeItem = GetRootItem (ItemNameCharacter);
				if (lTreeItem != null)
				{
					lTreeItem.IsExpanded = lSettings.CharacterNodeExpanded;
					lTreeItem.IsSelected = true;
				}
				lTreeItem = GetRootItem (ItemNameAnimations);
				if (lTreeItem != null)
				{
					lTreeItem.IsExpanded = lSettings.AnimationsNodeExpanded;
				}
				lTreeItem = GetRootItem (ItemNameStates);
				if (lTreeItem != null)
				{
					lTreeItem.IsExpanded = lSettings.StatesNodeExpanded;
				}

				//TreeViewMain.EndUpdate ();
				TreeViewMain.InvalidateVisual ();
			}
		}

		public void SaveExpansion ()
		{
			Settings lSettings = Settings.Default;
			TreeViewItem lTreeItem;

			lTreeItem = GetRootItem (ItemNameCharacter);
			if (lTreeItem != null)
			{
				lSettings.CharacterNodeExpanded = lTreeItem.IsExpanded;
			}
			lTreeItem = GetRootItem (ItemNameAnimations);
			if (lTreeItem != null)
			{
				lSettings.AnimationsNodeExpanded = lTreeItem.IsExpanded;
			}
			lTreeItem = GetRootItem (ItemNameStates);
			if (lTreeItem != null)
			{
				lSettings.StatesNodeExpanded = lTreeItem.IsExpanded;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void InitItemTags ()
		{
			TreeViewItem lTreeItem;

			lTreeItem = GetRootItem (ItemNameCharacter);
			if (lTreeItem != null)
			{
				lTreeItem.Tag = new ResolveCharacter ();

				foreach (TreeViewItem lSubItem in lTreeItem.Items)
				{
					if (lSubItem.Name == ItemNameWordBalloon)
					{
						lSubItem.Tag = new ResolveBalloon ();
					}
					else if (lSubItem.Name == ItemNameTTSMode)
					{
						lSubItem.Tag = new ResolveTts ();
					}
				}
			}
			lTreeItem = GetRootItem (ItemNameAnimations);
			if (lTreeItem != null)
			{
				lTreeItem.Tag = new ResolveCharacter ();
				(lTreeItem.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeAnimations;
			}
			lTreeItem = GetRootItem (ItemNameStates);
			if (lTreeItem != null)
			{
				lTreeItem.Tag = new ResolveCharacter ();
				(lTreeItem.Tag as ResolveCharacter).Scope = ResolveCharacter.ScopeType.ScopeStates;

				foreach (TreeViewItem lStateItem in lTreeItem.Items)
				{
					lStateItem.Tag = new ResolveState (lStateItem.Header.ToString ());
				}
			}
		}

		#endregion
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

				InitItemTags ();
				ShowAnimationNames ();
				if (TreeViewMain.SelectedItem != null)
				{
					//TreeViewMain.SelectedItem.EnsureVisible ();
				}
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public ResolvePart SelectedPart
		{
			get
			{
				return GetItemPart (TreeViewMain.SelectedItem as TreeViewItem);
			}
			set
			{
				SelectPartItem (value);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Events

		public event NavigationEventHandler Navigate;

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public void ShowAnimationNames ()
		{
			TreeViewItem lAnimationsItem = GetRootItem (ItemNameAnimations);
			TreeViewItem lSelectedItem = TreeViewMain.SelectedItem as TreeViewItem;

			if (lAnimationsItem != null)
			{
				List<TreeViewItem> lAnimationItems = new List<TreeViewItem> ();

				//TreeViewMain.BeginUpdate ();

				if (!IsPanelEmpty)
				{
					String[] lAnimations = CharacterFile.GetAnimationNames ();
					FileGestures lGestures = CharacterFile.Gestures;
					TreeViewItem lAnimationItem;

					foreach (String lAnimationName in lAnimations)
					{
						FileAnimation lAnimation = null;

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

						lAnimationItem = GetChildItem (lAnimationsItem, lAnimationName);
						if (lAnimationItem == null)
						{
							lAnimationsItem.Items.Add (lAnimationItem = new TreeViewItem ());
						}
						lAnimationItem.Header = lAnimationName;
						lAnimationItem.Tag = new ResolveAnimation (lAnimation);
						lAnimationItems.Add (lAnimationItem);

						ShowAnimationFrames (lAnimation, lAnimationItem, true);
					}
				}

				lAnimationsItem.Items.Clear ();
				foreach (TreeViewItem lTreeItem in lAnimationItems)
				{
					lAnimationsItem.Items.Add (lTreeItem);
				}
				//TreeViewMain.EndUpdate ();
				TreeViewMain.InvalidateVisual ();
			}

			try
			{
				if (lSelectedItem != null)
				{
					lSelectedItem.IsSelected = true;
				}
				lSelectedItem = TreeViewMain.SelectedItem as TreeViewItem;
				if (lSelectedItem == null)
				{
					lSelectedItem = GetRootItem (ItemNameCharacter);
					if (lSelectedItem != null)
					{
						lSelectedItem.IsSelected = true;
					}
				}
			}
			catch
			{
			}
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeViewItem pAnimationItem)
		{
			ShowAnimationFrames (pAnimation, pAnimationItem, false);
		}

		public void ShowAnimationFrames (FileAnimation pAnimation, TreeViewItem pAnimationItem, Boolean pInUpdate)
		{
			if ((pAnimation != null) && (pAnimationItem != null))
			{
				int lFrameNdx;
				String lFrameName;
				TreeViewItem lFrameItem;
				TreeViewItem lFrameSubItem;

				if (!pInUpdate)
				{
					//TreeViewMain.BeginUpdate ();
				}

				foreach (FileAnimationFrame lFrame in pAnimation.Frames)
				{
					lFrameNdx = pAnimation.Frames.IndexOf (lFrame);
					lFrameName = Titles.Frame (lFrameNdx);
					lFrameItem = ((lFrameNdx < pAnimationItem.Items.Count) ? pAnimationItem.Items[lFrameNdx] : pAnimationItem.Items[pAnimationItem.Items.Add (new TreeViewItem ())]) as TreeViewItem;
					lFrameItem.Header = lFrameName;
					lFrameItem.Tag = new ResolveAnimationFrame (lFrame);

					lFrameSubItem = ((lFrameItem.Items.Count >= 1) ? lFrameItem.Items[0] : lFrameItem.Items[lFrameItem.Items.Add (new TreeViewItem ())]) as TreeViewItem;
					lFrameSubItem.Header = "Branching";
					lFrameSubItem.Tag = new ResolveAnimationFrame (lFrame);
					(lFrameSubItem.Tag as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeBranching;

					lFrameSubItem = ((lFrameItem.Items.Count >= 2) ? lFrameItem.Items[1] : lFrameItem.Items[lFrameItem.Items.Add (new TreeViewItem ())]) as TreeViewItem;
					lFrameSubItem.Header = "Overlays";
					lFrameSubItem.Tag = new ResolveAnimationFrame (lFrame);
					(lFrameSubItem.Tag as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeOverlays;
				}

				while (pAnimationItem.Items.Count > pAnimation.Frames.Count)
				{
					pAnimationItem.Items.RemoveAt (pAnimationItem.Items.Count - 1);
				}

				if (!pInUpdate)
				{
					//TreeViewMain.EndUpdate ();
					TreeViewMain.InvalidateVisual ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal TreeViewItem GetRootItem (String pItemName)
		{
			foreach (TreeViewItem lTreeItem in TreeViewMain.Items)
			{
				if (lTreeItem.Name == pItemName)
				{
					return lTreeItem;
				}
			}
			return null;
		}

		internal TreeViewItem GetChildItem (TreeViewItem pParentItem, String pItemHeader)
		{
			foreach (TreeViewItem lTreeItem in pParentItem.Items)
			{
				if (lTreeItem.Header.ToString () == pItemHeader)
				{
					return lTreeItem;
				}
			}
			return null;
		}

		public ResolvePart GetItemPart (TreeViewItem pItem)
		{
			if (pItem != null)
			{
				return pItem.Tag as ResolvePart;
			}
			return null;
		}

		public TreeViewItem GetPartItem (ResolvePart pPart)
		{
			TreeViewItem lPartItem = null;

			if (pPart != null)
			{
				foreach (TreeViewItem lTreeItem in TreeViewMain.Items)
				{
					if ((lPartItem = GetPartItem (lTreeItem, pPart)) != null)
					{
						break;
					}
				}
			}
			return lPartItem;
		}

		public TreeViewItem GetPartItem (TreeViewItem pItem, ResolvePart pPart)
		{
			TreeViewItem lObjectItem = null;

			if ((pPart != null) && (pItem != null))
			{
				ResolvePart lPart = pItem.Tag as ResolvePart;

				if ((lPart != null) && Object.ReferenceEquals (lPart.Part, pPart.Part))
				{
					if ((pPart is ResolveCharacter) && (lPart is ResolveCharacter))
					{
						if ((pPart as ResolveCharacter).Scope == (lPart as ResolveCharacter).Scope)
						{
							lObjectItem = pItem;
						}
					}
					else if ((pPart is ResolveAnimationFrame) && (lPart is ResolveAnimationFrame))
					{
						if ((pPart as ResolveAnimationFrame).Scope == (lPart as ResolveAnimationFrame).Scope)
						{
							lObjectItem = pItem;
						}
					}
					else
					{
						lObjectItem = pItem;
					}
				}
				if (lObjectItem == null)
				{
					foreach (TreeViewItem lTreeItem in pItem.Items)
					{
						if ((lObjectItem = GetPartItem (lTreeItem, pPart)) != null)
						{
							break;
						}
					}
				}
			}
			return lObjectItem;
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean SelectPartItem (ResolvePart pPart)
		{
			if (pPart != null)
			{
				TreeViewItem lPartItem = GetPartItem (pPart);

				if ((lPartItem != null) && (TreeViewMain.SelectedItem != lPartItem))
				{
					//CausesValidation = false;
					//TreeViewMain.SelectedItem = lPartItem;
					//CausesValidation = Visible;
					lPartItem.IsSelected = true;
					return true;
				}
			}
			return false;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private void TreeViewMain_CanEditCopy (object sender, CanExecuteRoutedEventArgs e)
		{
			ResolvePart lSelectedPart = SelectedPart;
			Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

			if (lSelectedObject is FileBalloon)
			{
				e.PutCopyTitle (AppResources.Resources.TitleBalloon);
			}
			else if (lSelectedObject is FileTts)
			{
				e.PutCopyTitle (AppResources.Resources.TitleTts);
			}
			else if (lSelectedObject is FileAnimation)
			{
				e.PutCopyTitle (Titles.Animation (lSelectedObject as FileAnimation));
			}
			else if (lSelectedObject is FileAnimationFrame)
			{
				e.PutCopyTitle (Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ());
			}
			else if ((lSelectedPart is ResolveState) && (lSelectedObject != null))
			{
				e.PutCopyTitle (Titles.State ((lSelectedPart as ResolveState).StateName));
			}
			e.Handled = true;
		}
		private void TreeViewMain_OnEditCopy (object sender, ExecutedRoutedEventArgs e)
		{
			ResolvePart lSelectedPart = SelectedPart;
			Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

			if (lSelectedObject != null)
			{
				try
				{
					Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
				}
				catch
				{
				}
			}
			e.Handled = true;
		}

		private void TreeViewMain_CanEditCut (object sender, CanExecuteRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					e.PutCutTitle (Titles.Animation (lSelectedObject as FileAnimation));
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					e.PutCutTitle (Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ());
				}
			}
		}
		private void TreeViewMain_OnEditCut (object sender, ExecutedRoutedEventArgs e)
		{

		}

		private void TreeViewMain_CanEditDelete (object sender, CanExecuteRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

				if (lSelectedObject is FileAnimation)
				{
					e.PutDeleteTitle (Titles.Animation (lSelectedObject as FileAnimation));
				}
				else if (lSelectedObject is FileAnimationFrame)
				{
					e.PutDeleteTitle (Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ());
				}
			}
		}
		private void TreeViewMain_OnEditDelete (object sender, ExecutedRoutedEventArgs e)
		{

		}

		private void TreeViewMain_CanEditPaste (object sender, CanExecuteRoutedEventArgs e)
		{
			if (!Program.FileIsReadOnly)
			{
				ResolvePart lSelectedPart = SelectedPart;
				Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;
				FileCharacterName lPasteObject = Clipboard.GetData (DataFormats.Serializable) as FileCharacterName;
			}
		}
		private void TreeViewMain_OnEditPaste (object sender, ExecutedRoutedEventArgs e)
		{

		}


		//protected override void ShowEditState (Global.CanEditEventArgs pEventArgs)
		//{
		//    if (TreeViewMain.ContainsFocus)
		//    {
		//        ResolvePart lSelectedPart = SelectedPart;
		//        Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

		//        if (lSelectedObject is FileBalloon)
		//        {
		//            pEventArgs.CopyObjectTitle = Resources.TitleBalloon;
		//        }
		//        else if (lSelectedObject is FileTts)
		//        {
		//            pEventArgs.CopyObjectTitle = Resources.TitleTts;
		//        }
		//        else if (lSelectedObject is FileAnimation)
		//        {
		//            pEventArgs.CopyObjectTitle = Properties.Titles.Animation (lSelectedObject as FileAnimation);
		//            if (!Program.FileIsReadOnly)
		//            {
		//                pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
		//                pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
		//            }
		//        }
		//        else if (lSelectedObject is FileAnimationFrame)
		//        {
		//            pEventArgs.CopyObjectTitle = Properties.Titles.FrameAnimation (lSelectedObject as FileAnimationFrame).Quoted ();
		//            if (!Program.FileIsReadOnly)
		//            {
		//                pEventArgs.CutObjectTitle = pEventArgs.CopyObjectTitle;
		//                pEventArgs.DeleteObjectTitle = pEventArgs.CopyObjectTitle;
		//            }
		//        }
		//        else if ((lSelectedPart is ResolveState) && (lSelectedObject != null))
		//        {
		//            pEventArgs.CopyObjectTitle = Properties.Titles.State (lSelectedObject as String);
		//        }

		//        if (!Program.FileIsReadOnly && (pEventArgs.PasteObject != null))
		//        {
		//            if ((pEventArgs.PasteObject is FileBalloon) && (lSelectedPart is ResolveBalloon))
		//            {
		//                pEventArgs.PasteObjectTitle = Resources.TitleBalloon;
		//            }
		//            else if ((pEventArgs.PasteObject is FileTts) && (lSelectedPart is ResolveTts))
		//            {
		//                pEventArgs.PasteObjectTitle = Resources.TitleTts;
		//            }
		//            else if (pEventArgs.PasteObject is FileAnimation)
		//            {
		//                if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
		//                {
		//                    pEventArgs.PasteObjectTitle = AppTitles.PasteTypeTitle (lSelectedObject as FileAnimation, Properties.Titles.Animation (lSelectedObject as FileAnimation), Properties.Titles.Animation (pEventArgs.PasteObject as FileAnimation));
		//                }
		//                else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
		//                {
		//                    if (Program.MainForm.PanelAnimations.HasNewAnimationName () && !CharacterFile.Gestures.Contains (Program.MainForm.PanelAnimations.GetNewAnimationName ()))
		//                    {
		//                        pEventArgs.PasteObjectTitle = AppTitles.PasteTypeTitle (null, Program.MainForm.PanelAnimations.GetNewAnimationName ().Quoted (), Properties.Titles.Animation (pEventArgs.PasteObject as FileAnimation));
		//                    }
		//                    else
		//                    {
		//                        pEventArgs.PasteTitle = null;
		//                        pEventArgs.PasteObjectTitle = Resources.EditPasteAnimation0;
		//                    }
		//                }
		//            }
		//            else if (pEventArgs.PasteObject is FileAnimationFrame)
		//            {
		//                if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
		//                {
		//                    pEventArgs.PasteObjectTitle = AppTitles.PasteTypeTitle (lSelectedObject as FileAnimationFrame, Properties.Titles.FrameAnimation (lSelectedObject as FileAnimationFrame), Properties.Titles.Frame (pEventArgs.PasteObject as FileAnimationFrame));

		//                }
		//                else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
		//                {
		//                    pEventArgs.PasteObjectTitle = AppTitles.PasteTypeTitle (null, Properties.Titles.Frame (pEventArgs.PasteObject as FileAnimationFrame));
		//                }
		//            }
		//            else if ((pEventArgs.PasteObject is FileState) && (lSelectedPart is ResolveState))
		//            {
		//                pEventArgs.PasteObjectTitle = Properties.Titles.State (pEventArgs.PasteObject as FileState);
		//            }
		//        }
		//    }
		//}

		///////////////////////////////////////////////////////////////////////////////

		//protected override bool EditCopy (Global.EditEventArgs pEventArgs)
		//{
		//    if (TreeViewMain.ContainsFocus)
		//    {
		//        ResolvePart lSelectedPart = SelectedPart;
		//        Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

		//        if (lSelectedObject != null)
		//        {
		//            try
		//            {
		//                Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
		//            }
		//            catch
		//            {
		//            }
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		//protected override bool EditCut (Global.EditEventArgs pEventArgs)
		//{
		//    if (TreeViewMain.ContainsFocus)
		//    {
		//        ResolvePart lSelectedPart = SelectedPart;
		//        Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

		//        if (lSelectedObject is FileAnimation)
		//        {
		//            try
		//            {
		//                Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
		//                Program.MainForm.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
		//            }
		//            catch
		//            {
		//                System.Media.SystemSounds.Asterisk.Play ();
		//            }
		//            return true;
		//        }
		//        else if (lSelectedObject is FileAnimationFrame)
		//        {
		//            try
		//            {
		//                Clipboard.SetData (DataFormats.Serializable, lSelectedObject);
		//                Program.MainForm.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
		//            }
		//            catch
		//            {
		//                System.Media.SystemSounds.Asterisk.Play ();
		//            }
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		//protected override bool EditDelete (Global.EditEventArgs pEventArgs)
		//{
		//    if (TreeViewMain.ContainsFocus)
		//    {
		//        ResolvePart lSelectedPart = SelectedPart;
		//        Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

		//        if (lSelectedObject is FileAnimation)
		//        {
		//            Program.MainForm.PanelAnimations.DeleteSelectedAnimation (lSelectedObject as FileAnimation, true);
		//            return true;
		//        }
		//        else if (lSelectedObject is FileAnimationFrame)
		//        {
		//            Program.MainForm.PanelAnimation.DeleteSelectedFrame (lSelectedObject as FileAnimationFrame, true);
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		///////////////////////////////////////////////////////////////////////////////

		//protected override bool EditPaste (Global.EditEventArgs pEventArgs)
		//{
		//    if (TreeViewMain.ContainsFocus)
		//    {
		//        ResolvePart lSelectedPart = SelectedPart;
		//        Object lSelectedObject = (lSelectedPart == null) ? null : lSelectedPart.Part;

		//        if ((pEventArgs.PasteObject is FileBalloon) && (lSelectedPart is ResolveBalloon))
		//        {
		//            Program.MainForm.PanelBalloon.PasteBalloon (pEventArgs.PasteObject as FileBalloon);
		//            return true;
		//        }
		//        else if ((pEventArgs.PasteObject is FileTts) && (lSelectedPart is ResolveTts))
		//        {
		//            Program.MainForm.PanelTts.PasteTts (pEventArgs.PasteObject as FileTts);
		//            return true;
		//        }
		//        else if (pEventArgs.PasteObject is FileAnimation)
		//        {
		//            if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
		//            {
		//                Program.MainForm.PanelAnimations.PasteSelectedAnimation (lSelectedObject as FileAnimation, pEventArgs.PasteObject as FileAnimation);
		//                return true;
		//            }
		//            else if ((lSelectedPart is ResolveCharacter) && ((lSelectedPart as ResolveCharacter).Scope == ResolveCharacter.ScopeType.ScopeAnimations))
		//            {
		//                Program.MainForm.PanelAnimations.PasteSelectedAnimation (null, pEventArgs.PasteObject as FileAnimation);
		//                return true;
		//            }
		//        }
		//        else if (pEventArgs.PasteObject is FileAnimationFrame)
		//        {
		//            if ((lSelectedPart is ResolveAnimationFrame) && (lSelectedObject is FileAnimationFrame) && ((lSelectedPart as ResolveAnimationFrame).Scope == ResolveAnimationFrame.ScopeType.ScopeFrame))
		//            {
		//                Program.MainForm.PanelAnimation.PasteSelectedFrame ((lSelectedObject as FileAnimationFrame).Animation, lSelectedObject as FileAnimationFrame, pEventArgs.PasteObject as FileAnimationFrame);
		//                return true;
		//            }
		//            else if ((lSelectedPart is ResolveAnimation) && (lSelectedObject is FileAnimation))
		//            {
		//                Program.MainForm.PanelAnimation.PasteSelectedFrame (lSelectedObject as FileAnimation, null, pEventArgs.PasteObject as FileAnimationFrame);
		//                return true;
		//            }
		//        }
		//        else if ((pEventArgs.PasteObject is FileState) && (lSelectedPart is ResolveState))
		//        {
		//            Program.MainForm.PanelState.PasteStateAnimations ((lSelectedPart as ResolveState).StateName, (pEventArgs.PasteObject as FileState).AnimationNames);
		//            return true;
		//        }
		//    }
		//    return false;
		//}

		///////////////////////////////////////////////////////////////////////////////

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
			else if ((lUpdateAnimation != null) && (lUpdateAnimation.NameChanged || lUpdateAnimation.ForClipboard))
			{
				TreeViewItem lAnimationItem = GetPartItem (new ResolveAnimation (lUpdateAnimation.Target));

				if (lAnimationItem != null)
				{
					if (lUpdateAnimation.NameChanged)
					{
						lAnimationItem.Header = lUpdateAnimation.Target.Name;
					}
					if (lUpdateAnimation.ForClipboard)
					{
						ShowAnimationFrames (lUpdateAnimation.Target, lAnimationItem);
					}
				}
			}
			else if (lAddDeleteFrame != null)
			{
				TreeViewItem lAnimationItem = GetPartItem (new ResolveAnimation (lAddDeleteFrame.Animation));

				if (lAnimationItem != null)
				{
					ShowAnimationFrames (lAddDeleteFrame.Animation, lAnimationItem);
				}
			}
			else if (lReorderFrame != null)
			{
				TreeViewItem lAnimationItem = GetPartItem (new ResolveAnimation (lReorderFrame.Animation));

				if (lAnimationItem != null)
				{
					ShowAnimationFrames (lReorderFrame.Animation, lAnimationItem);
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void TreeViewMain_SelectedItemChanged (object sender, RoutedPropertyChangedEventArgs<object> e)
		{
			if (Navigate != null)
			{
				try
				{
					Navigate (this, new AgentCharacterEditor.Navigation.NavigationEventArgs (SelectedPart));
				}
				catch
				{
				}
			}
		}

		//private void TreeViewMain_AfterSelect (object sender, TreeViewEventArgs e)
		//{
		//    if (CausesValidation && (Navigate != null))
		//    {
		//        try
		//        {
		//            Navigate (this, new NavigationEventArgs (SelectedPart));
		//        }
		//        catch
		//        {
		//        }
		//    }
		//}

		#endregion
	}
}
