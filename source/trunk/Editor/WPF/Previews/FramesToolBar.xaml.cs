#if DEBUG
//#define	DEBUG_ANIMATIONS
#endif
/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;
using DoubleAgent;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesToolBar : ToolBar, INotifyPropertyChanged
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramesToolBar ()
		{
			InitializeComponent ();

			mAddAnimation = new ExpanderAddOverlay (this);
			mDeleteAnimation = new ExpanderDeleteOverlay (this);
			mMovePrevAnimation = new ExpanderMovePrevOverlay (this);
			mMoveNextAnimation = new ExpanderMoveNextOverlay (this);
		}

		public void LoadConfig ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				AddShiftsBranchingTarget = lSettings.AddFrameShiftsBranchingTarget;
				DeleteShiftsBranchingTarget = lSettings.DeleteFrameShiftsBranchingTarget;
				DeleteMovesBranchingPrev = lSettings.DeleteFrameMovesBranchingPrev;
				DeleteMovesBranchingNext = lSettings.DeleteFrameMovesBranchingNext;
				MovePrevMovesBranchingSource = lSettings.MoveFramePrevMovesBranchingSource;
				MovePrevMovesBranchingTarget = lSettings.MoveFramePrevMovesBranchingTarget;
				MoveNextMovesBranchingSource = lSettings.MoveFrameNextMovesBranchingSource;
				MoveNextMovesBranchingTarget = lSettings.MoveFrameNextMovesBranchingTarget;
			}
		}

		public void SaveConfig ()
		{
			Settings lSettings = Settings.Default;

			lSettings.AddFrameShiftsBranchingTarget = AddShiftsBranchingTarget;
			lSettings.DeleteFrameShiftsBranchingTarget = DeleteShiftsBranchingTarget;
			lSettings.DeleteFrameMovesBranchingPrev = DeleteMovesBranchingPrev;
			lSettings.DeleteFrameMovesBranchingNext = MovePrevMovesBranchingSource;
			lSettings.MoveFramePrevMovesBranchingSource = MovePrevMovesBranchingSource;
			lSettings.MoveFramePrevMovesBranchingTarget = MovePrevMovesBranchingTarget;
			lSettings.MoveFrameNextMovesBranchingSource = MoveNextMovesBranchingSource;
			lSettings.MoveFrameNextMovesBranchingTarget = MoveNextMovesBranchingTarget;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public String FrameTitle
		{
			get
			{
				return Titles.Frame (Frame);
			}
		}
		public String FrameNoneTitle
		{
			get
			{
				return (Frame == null) ? "There are no options for the selected frame" : String.Format ("There are no options for {0}", Titles.Frame (Frame));
			}
		}

		public String FrameSourceTitle
		{
			get
			{
				return (Frame == null) ? "Branch from\nframe" : String.Format ("Branch from\n{0}", Titles.Frame (Frame));
			}
		}
		public String FrameTargetTitle
		{
			get
			{
				return (Frame == null) ? "Branch to\nframe" : String.Format ("Branch to\n{0}", Titles.Frame (Frame));
			}
		}

		public String FrameSourceTitleNext
		{
			get
			{
				return (Frame == null) ? "Branch from\nnext frame" : String.Format ("Branch from\n{0}", Titles.Frame (Frame.GetFrameIndex () + 1));
			}
		}
		public String FrameTargetTitleNext
		{
			get
			{
				return (Frame == null) ? "Branch to\nnext frame" : String.Format ("Branch to\n{0}", Titles.Frame (Frame.GetFrameIndex () + 1));
			}
		}

		public String FrameSourceTitlePrev
		{
			get
			{
				return (Frame == null) ? "Branch from prior frame" : String.Format ("Branch from\n{0}", Titles.Frame (Frame.GetFrameIndex () - 1));
			}
		}
		public String FrameTargetTitlePrev
		{
			get
			{
				return (Frame == null) ? "Branch to prior frame" : String.Format ("Branch to\n{0}", Titles.Frame (Frame.GetFrameIndex () - 1));
			}
		}

		public String FrameTargetTitleNextFrame
		{
			get
			{
				return "Branch to\nnext frame...";
			}
		}
		public String FrameTargetTitleLastFrame
		{
			get
			{
				return "Branch to\nlast frame...";
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public Visibility ShowAddShiftsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (AddShiftsBranchingTarget && (Frame != null)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowAddKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (AddKeepsBranchingTarget && (Frame != null)) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		public Visibility ShowDeleteShiftsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteShiftsBranchingTarget && (CanMoveFrameNext || CanMoveFramePrev)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteKeepsBranchingTarget && (CanMoveFrameNext || CanMoveFramePrev)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteMovesBranchingPrev
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteMovesBranchingPrev && CanMoveFramePrev && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteMovesBranchingNext
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteMovesBranchingNext && CanMoveFrameNext && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteDeletesBranching
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : ((!DeleteMovesBranchingPrev || !CanMoveFramePrev) && (!DeleteMovesBranchingNext || !CanMoveFrameNext) && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public Visibility ShowMovePrevMovesBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MovePrevMovesBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMovePrevKeepsBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MovePrevKeepsBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMovePrevMovesBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MovePrevMovesBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMovePrevKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MovePrevKeepsBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		//=============================================================================

		public Visibility ShowMoveNextMovesBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveNextMovesBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveNextKeepsBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveNextKeepsBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveNextMovesBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveNextMovesBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveNextKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveNextKeepsBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Property Change Notifications

		public event PropertyChangedEventHandler PropertyChanged;

		private void NotifyPropertyChanged (String pPropertyName)
		{
			if (PropertyChanged != null)
			{
				PropertyChanged (this, new PropertyChangedEventArgs (pPropertyName));
			}
		}

		//=============================================================================

		private void AddShiftsBranchingChanged ()
		{
			UpdateAddAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("AddShiftsBranchingTarget");
				NotifyPropertyChanged ("AddKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowAddShiftsBranchingTarget");
				NotifyPropertyChanged ("ShowAddKeepsBranchingTarget");
				NotifyPropertyChanged ("SayAddShiftsBranchingTarget");
				NotifyPropertyChanged ("SayAddKeepsBranchingTarget");

				NotifyPropertyChanged ("AddBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableAddBranchingDefaultOptions");
			}

			UpdateAddAnimation (100);
		}

		private void DeleteBranchingSourceChanged ()
		{
			UpdateDeleteAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("DeleteMovesBranchingPrev");
				NotifyPropertyChanged ("DeleteMovesBranchingNext");
				NotifyPropertyChanged ("DeleteDeletesBranching");
				NotifyPropertyChanged ("ShowDeleteMovesBranchingPrev");
				NotifyPropertyChanged ("ShowDeleteMovesBranchingNext");
				NotifyPropertyChanged ("ShowDeleteDeletesBranching");
				NotifyPropertyChanged ("SayDeleteMovesBranchingPrev");
				NotifyPropertyChanged ("SayDeleteMovesBranchingNext");
				NotifyPropertyChanged ("SayDeleteDeletesBranching");

				NotifyPropertyChanged ("DeleteBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableDeleteBranchingDefaultOptions");
			}

			UpdateDeleteAnimation (100);
		}

		private void DeleteBranchingTargetChanged ()
		{
			UpdateDeleteAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("DeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("DeleteKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowDeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("ShowDeleteKeepsBranchingTarget");
				NotifyPropertyChanged ("SayDeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("SayDeleteKeepsBranchingTarget");

				NotifyPropertyChanged ("DeleteBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableDeleteBranchingDefaultOptions");
			}

			UpdateDeleteAnimation (100);
		}

		//=============================================================================

		private void MovePrevBranchingSourceChanged ()
		{
			UpdateMovePrevAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MovePrevMovesBranchingSource");
				NotifyPropertyChanged ("MovePrevKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMovePrevMovesBranchingSource");
				NotifyPropertyChanged ("ShowMovePrevKeepsBranchingSource");
				NotifyPropertyChanged ("SayMovePrevMovesBranchingSource");
				NotifyPropertyChanged ("SayMovePrevKeepsBranchingSource");

				NotifyPropertyChanged ("MovePrevBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMovePrevBranchingDefaultOptions");
			}

			UpdateMovePrevAnimation (100);
		}
		private void MovePrevBranchingTargetChanged ()
		{
			UpdateMovePrevAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MovePrevMovesBranchingTarget");
				NotifyPropertyChanged ("MovePrevKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMovePrevMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMovePrevKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMovePrevMovesBranchingTarget");
				NotifyPropertyChanged ("SayMovePrevKeepsBranchingTarget");

				NotifyPropertyChanged ("MovePrevBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMovePrevBranchingDefaultOptions");
			}

			UpdateMovePrevAnimation (100);
		}
		private void MoveNextBranchingSourceChanged ()
		{
			UpdateMoveNextAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MoveNextMovesBranchingSource");
				NotifyPropertyChanged ("MoveNextKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMoveNextMovesBranchingSource");
				NotifyPropertyChanged ("ShowMoveNextKeepsBranchingSource");
				NotifyPropertyChanged ("SayMoveNextMovesBranchingSource");
				NotifyPropertyChanged ("SayMoveNextKeepsBranchingSource");

				NotifyPropertyChanged ("MoveNextBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveNextBranchingDefaultOptions");
			}

			UpdateMoveNextAnimation (100);
		}
		private void MoveNextBranchingTargetChanged ()
		{
			UpdateMoveNextAnimation (-1);

			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MoveNextMovesBranchingTarget");
				NotifyPropertyChanged ("MoveNextKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMoveNextMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMoveNextKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMoveNextMovesBranchingTarget");
				NotifyPropertyChanged ("SayMoveNextKeepsBranchingTarget");

				NotifyPropertyChanged ("MoveNextBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveNextBranchingDefaultOptions");
			}

			UpdateMoveNextAnimation (100);
		}

		//=============================================================================

		private void OnFrameStateChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("CanMoveFramePrev");
				NotifyPropertyChanged ("CanMoveFrameNext");
				NotifyPropertyChanged ("CanMoveNextFrameNext");
				NotifyPropertyChanged ("FrameTitle");
				NotifyPropertyChanged ("FrameNoneTitle");
				NotifyPropertyChanged ("FrameSourceTitle");
				NotifyPropertyChanged ("FrameSourceTitleNext");
				NotifyPropertyChanged ("FrameSourceTitlePrev");
				NotifyPropertyChanged ("FrameTargetTitle");
				NotifyPropertyChanged ("FrameTargetTitleNext");
				NotifyPropertyChanged ("FrameTargetTitlePrev");
			}

			AddShiftsBranchingChanged ();
			DeleteBranchingSourceChanged ();
			DeleteBranchingTargetChanged ();
			MovePrevBranchingSourceChanged ();
			MovePrevBranchingTargetChanged ();
			MoveNextBranchingSourceChanged ();
			MoveNextBranchingTargetChanged ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Animations

		private ExpanderAddOverlay mAddAnimation = null;

		private class ExpanderAddOverlay : ExpanderContentOverlay
		{
			public ExpanderAddOverlay (FramesToolBar pOwner)
				: base (pOwner.ExpanderAdd, pOwner.EAddContent, pOwner.EAddCanvas)
			{
				Owner = pOwner;
			}
			public FramesToolBar Owner
			{
				get;
				set;
			}

			public override Boolean DefineOverlayAnimation ()
			{
				try
				{
					if (PrepareOverlay (Owner.EAddFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;

						lStoryboard.Children.Add (MakeFlashAnimation (Owner.EAddGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EAddTrgLine1), Owner.EAddTrgLine2));
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EAddTrgLine2), Owner.EAddTrgLine3));
						SetPartAnimation (0, lPartAnimation);

						Owner.EAddTrg0.Visibility = Visibility.Visible;
						lStoryboard.Children.Add (MakeFadeAnimation (MakeAnimationVisual (Owner.EAddTrg0), true));

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EAddTrg1), Owner.EAddTrg2));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EAddTrg1Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EAddTrg1Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EAddTrg2), Owner.EAddTrg3));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EAddTrg2Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EAddTrg2Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						CompleteOverlay (lStoryboard);
						return true;
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
				return false;
			}

			public override void UpdateOverlayAnimation ()
			{
				if (Owner.Frame != null)
				{
					SetPartEnabled (0, Owner.AddShiftsBranchingTarget);
					OverlayElement.Visibility = Visibility.Visible;
					StartOverlayAnimation ();
				}
				else
				{
					StopOverlayAnimation ();
					OverlayElement.Visibility = Visibility.Hidden;
				}
			}
		}

		private void UpdateAddExpander ()
		{
			if (IsValid)
			{
				if (Frame != null)
				{
					EAddTrg2.Visibility = CanMoveFrameNext ? Visibility.Visible : Visibility.Hidden;
					EAddTrgLine2.Visibility = CanMoveFrameNext ? Visibility.Visible : Visibility.Hidden;
					EAddTrg0Txt1.Visibility = AddShiftsBranchingTarget ? Visibility.Hidden : Visibility.Visible;
					EAddTrg0Txt2.Visibility = AddShiftsBranchingTarget ? Visibility.Visible : Visibility.Hidden;

					AddKeepBranchingTarget.Visibility = Visibility.Visible;
					AddShiftBranchingTarget.Visibility = Visibility.Visible;
					AddDefaults.Visibility = Visibility.Visible;
					AddNone.Visibility = Visibility.Collapsed;
				}
				else
				{
					AddKeepBranchingTarget.Visibility = Visibility.Hidden;
					AddShiftBranchingTarget.Visibility = Visibility.Hidden;
					AddDefaults.Visibility = Visibility.Collapsed;
					AddNone.Visibility = Visibility.Visible;
				}
			}
		}

		private void UpdateAddAnimation ()
		{
			if (IsValid)
			{
				UpdateAddExpander ();
				if (mAddAnimation != null)
				{
					mAddAnimation.UpdateOverlayAnimation ();
				}
			}
		}
		private void UpdateAddAnimation (int pDelay)
		{
			if (IsValid)
			{
				UpdateAddExpander ();
				if (mAddAnimation != null)
				{
					mAddAnimation.UpdateOverlayAnimation (pDelay);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private ExpanderDeleteOverlay mDeleteAnimation = null;

		private class ExpanderDeleteOverlay : ExpanderContentOverlay
		{
			public ExpanderDeleteOverlay (FramesToolBar pOwner)
				: base (pOwner.ExpanderDelete, pOwner.EDelContent, pOwner.EDelCanvas)
			{
				Owner = pOwner;
			}
			public FramesToolBar Owner
			{
				get;
				set;
			}

			public override Boolean DefineOverlayAnimation ()
			{
				try
				{
					if (PrepareOverlay (Owner.EDelFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;
						Rectangle lAnimationVisual;

						lStoryboard.Children.Add (MakeFlashAnimation (Owner.EDelGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeFadeAnimation (lAnimationVisual = MakeAnimationVisual (Owner.EDelSrcLine1), false));
						SetPartAnimation (0, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (lAnimationVisual, Owner.EDelSrcLine0));
						SetPartAnimation (1, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EDelSrcLine2), Owner.EDelSrcLine1));
						SetPartAnimation (2, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EDelTrgLine2), Owner.EDelTrgLine1));
						SetPartAnimation (3, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EDelTrgLine1), Owner.EDelTrgLine0));
						SetPartAnimation (4, lPartAnimation);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeFadeAnimation (MakeAnimationVisual (Owner.EDelSrc1), false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (MakeAnimationVisual (Owner.EDelTrg1), false));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EDelSrc2), Owner.EDelSrc1));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EDelSrc2Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EDelSrc2Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EDelTrg2), Owner.EDelTrg1));
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg2Txt1, false));
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg2Txt2, true));
						SetPartAnimation (5, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EDelTrg3), Owner.EDelTrg2));
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg3Err, true, DefaultStartRest * 2));
						SetPartAnimation (6, lPartAnimation);
 
						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg2Err, true, DefaultStartRest * 2));
						SetPartAnimation (7, lPartAnimation);
 
						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg1Err, true, DefaultStartRest * 2));
						SetPartAnimation (8, lPartAnimation);

						MakeAnimationVisual (Owner.EDelSrc0);
						MakeAnimationVisual (Owner.EDelTrg0);
						MakeAnimationVisual (Owner.EDelSrcLine0);
						MakeAnimationVisual (Owner.EDelTrgLine3);
						MakeAnimationVisual (Owner.EDelTrg1Err);
						MakeAnimationVisual (Owner.EDelTrg2Err);
						MakeAnimationVisual (Owner.EDelTrg3Err);

						CompleteOverlay (lStoryboard);
						return true;
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
				return false;
			}

			public override void UpdateOverlayAnimation ()
			{
				if (Owner.CanMoveFrameNext || Owner.CanMoveFramePrev)
				{
					SetPartEnabled (0, Owner.DeleteDeletesBranching);
					SetPartEnabled (1, Owner.DeleteMovesBranchingPrev && Owner.CanMoveFramePrev);
					SetPartEnabled (2, Owner.CanMoveFrameNext);
					SetPartEnabled (3, Owner.DeleteShiftsBranchingTarget && Owner.CanMoveFrameNext);
					SetPartEnabled (4, Owner.DeleteShiftsBranchingTarget && !Owner.CanMoveFrameNext);
					SetPartEnabled (5, Owner.CanMoveFrameNext);
					SetPartEnabled (6, Owner.DeleteKeepsBranchingTarget && Owner.CanMoveNextFrameNext);
					SetPartEnabled (7, Owner.DeleteKeepsBranchingTarget && Owner.CanMoveFrameNext && !Owner.CanMoveNextFrameNext);
					SetPartEnabled (8, Owner.DeleteKeepsBranchingTarget && !Owner.CanMoveFrameNext);
					OverlayElement.Visibility = Visibility.Visible;
					StartOverlayAnimation ();
				}
				else
				{
					StopOverlayAnimation ();
					OverlayElement.Visibility = Visibility.Hidden;
				}
			}
		}

		private void UpdateDeleteExpander ()
		{
			if (IsValid)
			{
				if (CanMoveFrameNext || CanMoveFramePrev)
				{
					EDelSrc0.Visibility = CanMoveFramePrev ? Visibility.Visible : Visibility.Hidden;
					EDelSrcLine0.Visibility = CanMoveFramePrev ? Visibility.Visible : Visibility.Hidden;
					EDelSrc2.Visibility = CanMoveFrameNext ? Visibility.Visible : Visibility.Hidden;
					EDelSrcLine2.Visibility = CanMoveFrameNext ? Visibility.Visible : Visibility.Hidden;
					EDelTrg0.Visibility = CanMoveFrameNext ? Visibility.Hidden : Visibility.Visible;
					EDelTrg2.Visibility = CanMoveFrameNext ? Visibility.Visible : Visibility.Hidden;
					EDelTrgLine2.Visibility = CanMoveFrameNext ? Visibility.Visible : Visibility.Hidden;
					EDelTrg3.Visibility = CanMoveNextFrameNext ? Visibility.Visible : Visibility.Hidden;
					EDelTrgLine3.Visibility = CanMoveNextFrameNext ? Visibility.Visible : Visibility.Hidden;

					if (CanMoveFrameNext)
					{
						Grid.SetColumn (DeleteKeepBranchingTarget, Grid.GetColumn (DeleteMoveBranchingNext));
						Grid.SetColumn (DeleteShiftBranchingTarget, Grid.GetColumn (DeleteMoveBranchingNext));
					}
					else
					{
						Grid.SetColumn (DeleteKeepBranchingTarget, Grid.GetColumn (DeleteMoveBranchingPrev));
						Grid.SetColumn (DeleteShiftBranchingTarget, Grid.GetColumn (DeleteMoveBranchingPrev));
					}

					DeleteMoveBranchingNext.Visibility = Visibility.Visible;
					DeleteMoveBranchingPrev.Visibility = Visibility.Visible;
					DeleteDeleteBranching.Visibility = Visibility.Visible;
					DeleteKeepBranchingTarget.Visibility = Visibility.Visible;
					DeleteShiftBranchingTarget.Visibility = Visibility.Visible;
					DeleteDefaults.Visibility = Visibility.Visible;
					EDelSplitTop.Visibility = Visibility.Visible;
					EDelSplitBottom.Visibility = Visibility.Visible;
					EDelNone.Visibility = Visibility.Collapsed;
				}
				else
				{
					DeleteMoveBranchingNext.Visibility = Visibility.Hidden;
					DeleteMoveBranchingPrev.Visibility = Visibility.Hidden;
					DeleteDeleteBranching.Visibility = Visibility.Hidden;
					DeleteKeepBranchingTarget.Visibility = Visibility.Hidden;
					DeleteShiftBranchingTarget.Visibility = Visibility.Hidden;
					DeleteDefaults.Visibility = Visibility.Collapsed;
					EDelSplitTop.Visibility = Visibility.Collapsed;
					EDelSplitBottom.Visibility = Visibility.Collapsed;
					EDelNone.Visibility = Visibility.Visible;
				}
			}
		}

		private void UpdateDeleteAnimation ()
		{
			if (IsValid)
			{
				UpdateDeleteExpander ();
				if (mDeleteAnimation != null)
				{
					mDeleteAnimation.UpdateOverlayAnimation ();
				}
			}
		}
		private void UpdateDeleteAnimation (int pDelay)
		{
			if (IsValid)
			{
				UpdateDeleteExpander ();
				if (mDeleteAnimation != null)
				{
					mDeleteAnimation.UpdateOverlayAnimation (pDelay);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private ExpanderMovePrevOverlay mMovePrevAnimation = null;

		private class ExpanderMovePrevOverlay : ExpanderContentOverlay
		{
			public ExpanderMovePrevOverlay (FramesToolBar pOwner)
				: base (pOwner.ExpanderMovePrev, pOwner.EMovPrvContent, pOwner.EMovPrvCanvas)
			{
				Owner = pOwner;
			}
			public FramesToolBar Owner
			{
				get;
				set;
			}

			public override Boolean DefineOverlayAnimation ()
			{
				try
				{
					if (PrepareOverlay (Owner.EMovPrvFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;

						lStoryboard.Children.Add (MakeFlashAnimation (Owner.EMovePrvGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvSrcLine1), Owner.EMovePrvSrcLine2));
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvSrcLine2), Owner.EMovePrvSrcLine1));
						SetPartAnimation (0, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvTrgLine1), Owner.EMovePrvTrgLine2));
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvTrgLine2), Owner.EMovePrvTrgLine1));
						SetPartAnimation (1, lPartAnimation);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvSrc1), Owner.EMovePrvSrc2));
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvSrc2), Owner.EMovePrvSrc1));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvSrc1Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvSrc1Txt2, true));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvSrc2Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvSrc2Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvTrg1), Owner.EMovePrvTrg2));
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovePrvTrg2), Owner.EMovePrvTrg1));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvTrg1Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvTrg1Txt2, true));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvTrg2Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovePrvTrg2Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);
#if DEBUG_ANIMATIONS_NOT
						EMovNxtContent.UpdateLayout ();
						EMovPrvContent.DebugPrint ();
#endif
						CompleteOverlay (lStoryboard);
						return true;
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
				return false;
			}

			public override void UpdateOverlayAnimation ()
			{
				SetPartEnabled (0, !Owner.MovePrevMovesBranchingSource);
				SetPartEnabled (1, Owner.MovePrevMovesBranchingTarget);
				StartOverlayAnimation ();
			}
		}

		private void UpdateMovePrevAnimation ()
		{
			if (IsValid)
			{
				if (mMovePrevAnimation != null)
				{
					mMovePrevAnimation.UpdateOverlayAnimation ();
				}
			}
		}
		private void UpdateMovePrevAnimation (int pDelay)
		{
			if (IsValid)
			{
				if (mMovePrevAnimation != null)
				{
					mMovePrevAnimation.UpdateOverlayAnimation (pDelay);
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		private ExpanderMoveNextOverlay mMoveNextAnimation = null;

		private class ExpanderMoveNextOverlay : ExpanderContentOverlay
		{
			public ExpanderMoveNextOverlay (FramesToolBar pOwner)
				: base (pOwner.ExpanderMoveNext, pOwner.EMovNxtContent, pOwner.EMovNxtCanvas)
			{
				Owner = pOwner;
			}
			public FramesToolBar Owner
			{
				get;
				set;
			}

			public override Boolean DefineOverlayAnimation ()
			{
				try
				{
					if (PrepareOverlay (Owner.EMovNxtFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;

						lStoryboard.Children.Add (MakeFlashAnimation (Owner.EMovNxtGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtSrcLine1), Owner.EMovNxtSrcLine2));
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtSrcLine2), Owner.EMovNxtSrcLine1));
						SetPartAnimation (0, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtTrgLine1), Owner.EMovNxtTrgLine2));
						lPartAnimation.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtTrgLine2), Owner.EMovNxtTrgLine1));
						SetPartAnimation (1, lPartAnimation);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtSrc1), Owner.EMovNxtSrc2));
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtSrc2), Owner.EMovNxtSrc1));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtSrc1Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtSrc1Txt2, true));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtSrc2Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtSrc2Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtTrg1), Owner.EMovNxtTrg2));
						lAnimationGroup.Children.Add (MakeSwapAnimation (MakeAnimationVisual (Owner.EMovNxtTrg2), Owner.EMovNxtTrg1));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtTrg1Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtTrg1Txt2, true));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtTrg2Txt1, false));
						lAnimationGroup.Children.Add (MakeFadeAnimation (Owner.EMovNxtTrg2Txt2, true));
						lStoryboard.Children.Add (lAnimationGroup);
#if DEBUG_ANIMATIONS_NOT
						EMovNxtContent.UpdateLayout ();
						EMovNxtContent.DebugPrint ();
#endif
						CompleteOverlay (lStoryboard);
						return true;
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
				return false;
			}

			public override void UpdateOverlayAnimation ()
			{
				SetPartEnabled (0, !Owner.MoveNextMovesBranchingSource);
				SetPartEnabled (1, Owner.MoveNextMovesBranchingTarget);
				StartOverlayAnimation ();
			}
		}

		private void UpdateMoveNextAnimation ()
		{
			if (IsValid)
			{
				if (mMoveNextAnimation != null)
				{
					mMoveNextAnimation.UpdateOverlayAnimation ();
				}
			}
		}
		private void UpdateMoveNextAnimation (int pDelay)
		{
			if (IsValid)
			{
				if (mMoveNextAnimation != null)
				{
					mMoveNextAnimation.UpdateOverlayAnimation (pDelay);
				}
			}
		}

		#endregion
	}

	/////////////////////////////////////////////////////////////////////////////

	internal class ExpanderContentOverlay : AnimationOverlay
	{
		/////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public ExpanderContentOverlay (Expander pExpander, FrameworkElement pContentElement, Canvas pOverlayElement)
			: base (pContentElement, pOverlayElement)
		{
			Expander = pExpander;
			Expander.Expanded += new RoutedEventHandler (Expander_Expanded);
			Expander.Collapsed += new RoutedEventHandler (Expander_Collapsed);
			ContentElement.LayoutUpdated += new EventHandler (Content_LayoutUpdated);
		}

		public Expander Expander
		{
			get;
			protected set;
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Methods

		public virtual Boolean DefineOverlayAnimation ()
		{
			return false;
		}

		public virtual void UpdateOverlayAnimation ()
		{
			StartOverlayAnimation ();
		}

		public override Boolean StartOverlayAnimation ()
		{
			if (Expander.IsExpanded)
			{
				return base.StartOverlayAnimation ();
			}
			else
			{
				StopOverlayAnimation ();
				return false;
			}
		}

		//=============================================================================

		public void UpdateOverlayAnimation (int pDelay)
		{
			if (pDelay == 0)
			{
				UpdateOverlayAnimation ();
			}
			else if (pDelay < 0)
			{
				mAnimationDelay = null;
			}
			else
			{
				if (mAnimationDelay == null)
				{
					mAnimationDelay = new AsyncTimer ();
					mAnimationDelay.TimerPulse += new AsyncTimer.TimerPulseHandler (AnimationDelay_TimerPulse);
					mAnimationDelay.Start (pDelay);
				}
			}
		}

		private void AnimationDelay_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			if (mAnimationDelay != null)
			{
				mAnimationDelay = null;
				UpdateOverlayAnimation ();
			}
		}

		private AsyncTimer mAnimationDelay = null;

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void Content_LayoutUpdated (object sender, EventArgs e)
		{
			if (Expander.IsExpanded)
			{
				DefineOverlayAnimation ();
			}
		}

		private void Expander_Expanded (object sender, RoutedEventArgs e)
		{
#if DEBUG_ANIMATIONS
			System.Diagnostics.Debug.Print ("{0} Expanded", Expander.Name);
#endif
			UpdateOverlayAnimation (200);
		}

		private void Expander_Collapsed (object sender, RoutedEventArgs e)
		{
#if DEBUG_ANIMATIONS
			System.Diagnostics.Debug.Print ("{0} Collapsed", Expander.Name);
#endif
			RemoveOverlayAnimation ();
			PauseOverlayAnimation ();
		}

		#endregion
	}
}
