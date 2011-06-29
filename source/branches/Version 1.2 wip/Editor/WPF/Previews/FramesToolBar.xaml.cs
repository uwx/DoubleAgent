#if DEBUG
//#define	DEBUG_ANIMATIONS
#endif
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
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Windows.Media;
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
				return String.Format ("{0}", Titles.Frame (Frame));
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
		public String FrameTargetTitleNext2
		{
			get
			{
				return "Branch to\nnext frame...";
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

		///////////////////////////////////////////////////////////////////////////////

		public Visibility ShowAddShiftsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (AddShiftsBranchingTarget && CanMoveFrameNext) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowAddKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (AddKeepsBranchingTarget && CanMoveFrameNext) ? Visibility.Visible : Visibility.Hidden;
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

		//=============================================================================

		public String SayAddShiftsBranchingTarget
		{
			get
			{
				return "Change branching destinations to the new frame positions";
			}
		}
		public String SayAddKeepsBranchingTarget
		{
			get
			{
				return "Maintain the current branching destinations regardless of the new frame positions";
			}
		}

		public String SayDeleteShiftsBranchingTarget
		{
			get
			{
				return String.Empty;
			}
		}
		public String SayDeleteKeepsBranchingTarget
		{
			get
			{
				return String.Empty;
			}
		}
		public String SayDeleteMovesBranchingPrev
		{
			get
			{
				return String.Empty;
			}
		}
		public String SayDeleteMovesBranchingNext
		{
			get
			{
				return String.Empty;
			}
		}
		public String SayDeleteDeletesBranching
		{
			get
			{
				return String.Empty;
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

		//=============================================================================

		public String SayMovePrevMovesBranchingSource
		{
			get
			{
				return (Frame == null) ? "Keep the frame's branching at its original position" : FrameIsBranchingSource (Frame) ? String.Format ("Swap {0}'s branching with frame {1}", Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMovePrevKeepsBranchingSource
		{
			get
			{
				return (Frame == null) ? "Move the frame's branching with it" : FrameIsBranchingSource (Frame) ? String.Format ("Keep {0}'s branching when it's moved", Titles.Frame (Frame)) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMovePrevMovesBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Change branching destinations to the frame's new position" : FrameIsBranchingTarget (Frame) ? String.Format ("Swap branching destinations for frames {0} and {1}", FramesPreview.Frames.SelectedIndex + 1, FramesPreview.Frames.SelectedIndex) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}
		public String SayMovePrevKeepsBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Keep branching destinations at the frame's original position" : FrameIsBranchingTarget (Frame) ? String.Format ("Don't change any branching destinations when {0} is moved", Titles.Frame (Frame)) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}

		//=============================================================================

		public String SayMoveNextMovesBranchingSource
		{
			get
			{
				return (Frame == null) ? "Keep the frame's branching at its original position" : FrameIsBranchingSource (Frame) ? String.Format ("Swap {0}'s branching with frame {1}", Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex + 2) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMoveNextKeepsBranchingSource
		{
			get
			{
				return (Frame == null) ? "Move the frame's branching with it" : FrameIsBranchingSource (Frame) ? String.Format ("Keep {0}'s branching when it's moved", Titles.Frame (Frame)) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMoveNextMovesBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Change branching destinations to the frame's new position" : FrameIsBranchingTarget (Frame) ? String.Format ("Swap branching destinations for frames {0} and {1}", FramesPreview.Frames.SelectedIndex + 1, FramesPreview.Frames.SelectedIndex + 2) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}
		public String SayMoveNextKeepsBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Keep branching destinations at the frame's original position" : FrameIsBranchingTarget (Frame) ? String.Format ("Don't change any branching destinations when {0} is moved", Titles.Frame (Frame)) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean EnableAddBranchingDefaultOptions
		{
			get
			{
				return !Program.FileIsReadOnly && !AddBranchingDefaultOptions;
			}
		}
		public Boolean EnableDeleteBranchingDefaultOptions
		{
			get
			{
				return !Program.FileIsReadOnly && !DeleteBranchingDefaultOptions;
			}
		}
		public Boolean EnableMovePrevBranchingDefaultOptions
		{
			get
			{
				return !Program.FileIsReadOnly && !MovePrevBranchingDefaultOptions;
			}
		}
		public Boolean EnableMoveNextBranchingDefaultOptions
		{
			get
			{
				return !Program.FileIsReadOnly && !MoveNextBranchingDefaultOptions;
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
			if (PropertyChanged != null)
			{
				UpdateAddAnimation (-1);

				NotifyPropertyChanged ("AddShiftsBranchingTarget");
				NotifyPropertyChanged ("AddKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowAddShiftsBranchingTarget");
				NotifyPropertyChanged ("ShowAddKeepsBranchingTarget");
				NotifyPropertyChanged ("SayAddShiftsBranchingTarget");
				NotifyPropertyChanged ("SayAddKeepsBranchingTarget");

				NotifyPropertyChanged ("AddBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableAddBranchingDefaultOptions");

				UpdateAddAnimation (100);
			}
		}

		private void DeleteBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateDeleteAnimation (-1);

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

				UpdateDeleteAnimation (100);
			}
		}

		private void DeleteBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateDeleteAnimation (-1);

				NotifyPropertyChanged ("DeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("DeleteKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowDeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("ShowDeleteKeepsBranchingTarget");
				NotifyPropertyChanged ("SayDeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("SayDeleteKeepsBranchingTarget");

				NotifyPropertyChanged ("DeleteBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableDeleteBranchingDefaultOptions");

				UpdateDeleteAnimation (100);
			}
		}

		//=============================================================================

		private void MovePrevBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMovePrevAnimation (-1);

				NotifyPropertyChanged ("MovePrevMovesBranchingSource");
				NotifyPropertyChanged ("MovePrevKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMovePrevMovesBranchingSource");
				NotifyPropertyChanged ("ShowMovePrevKeepsBranchingSource");
				NotifyPropertyChanged ("SayMovePrevMovesBranchingSource");
				NotifyPropertyChanged ("SayMovePrevKeepsBranchingSource");

				NotifyPropertyChanged ("MovePrevBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMovePrevBranchingDefaultOptions");

				UpdateMovePrevAnimation (100);
			}
		}
		private void MovePrevBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMovePrevAnimation (-1);

				NotifyPropertyChanged ("MovePrevMovesBranchingTarget");
				NotifyPropertyChanged ("MovePrevKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMovePrevMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMovePrevKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMovePrevMovesBranchingTarget");
				NotifyPropertyChanged ("SayMovePrevKeepsBranchingTarget");

				NotifyPropertyChanged ("MovePrevBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMovePrevBranchingDefaultOptions");

				UpdateMovePrevAnimation (100);
			}
		}
		private void MoveNextBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMoveNextAnimation (-1);

				NotifyPropertyChanged ("MoveNextMovesBranchingSource");
				NotifyPropertyChanged ("MoveNextKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMoveNextMovesBranchingSource");
				NotifyPropertyChanged ("ShowMoveNextKeepsBranchingSource");
				NotifyPropertyChanged ("SayMoveNextMovesBranchingSource");
				NotifyPropertyChanged ("SayMoveNextKeepsBranchingSource");

				NotifyPropertyChanged ("MoveNextBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveNextBranchingDefaultOptions");

				UpdateMoveNextAnimation (100);
			}
		}
		private void MoveNextBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMoveNextAnimation (-1);

				NotifyPropertyChanged ("MoveNextMovesBranchingTarget");
				NotifyPropertyChanged ("MoveNextKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMoveNextMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMoveNextKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMoveNextMovesBranchingTarget");
				NotifyPropertyChanged ("SayMoveNextKeepsBranchingTarget");

				NotifyPropertyChanged ("MoveNextBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveNextBranchingDefaultOptions");

				UpdateMoveNextAnimation (100);
			}
		}

		//=============================================================================

		private void OnFrameStateChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("CanMoveFramePrev");
				NotifyPropertyChanged ("CanMoveFrameNext");
				NotifyPropertyChanged ("FrameTitle");
				NotifyPropertyChanged ("FrameNoneTitle");
				NotifyPropertyChanged ("FrameSourceTitle");
				NotifyPropertyChanged ("FrameSourceTitleNext");
				NotifyPropertyChanged ("FrameSourceTitlePrev");
				NotifyPropertyChanged ("FrameTargetTitle");
				NotifyPropertyChanged ("FrameTargetTitleNext");
				NotifyPropertyChanged ("FrameTargetTitlePrev");
				AddShiftsBranchingChanged ();
				DeleteBranchingSourceChanged ();
				DeleteBranchingTargetChanged ();
				MovePrevBranchingSourceChanged ();
				MovePrevBranchingTargetChanged ();
				MoveNextBranchingSourceChanged ();
				MoveNextBranchingTargetChanged ();
			}
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
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg1Err, true, DefaultStartRest * 2));
						SetPartAnimation (6, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (MakeFadeAnimation (Owner.EDelTrg2Err, true, DefaultStartRest * 2));
						SetPartAnimation (7, lPartAnimation);

						MakeAnimationVisual (Owner.EDelSrc0);
						MakeAnimationVisual (Owner.EDelTrg0);
						MakeAnimationVisual (Owner.EDelTrg1Err);
						MakeAnimationVisual (Owner.EDelTrg2Err);

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
					SetPartEnabled (6, Owner.DeleteKeepsBranchingTarget && !Owner.CanMoveFrameNext);
					SetPartEnabled (7, Owner.DeleteKeepsBranchingTarget && Owner.CanMoveFrameNext);
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

					DeleteDefaults.Visibility = Visibility.Visible;
					EDelNone.Visibility = Visibility.Collapsed;
				}
				else
				{
					DeleteDefaults.Visibility = Visibility.Collapsed;
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

	internal class AnimationOverlay
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationOverlay (FrameworkElement pContentElement, Canvas pOverlayElement)
		{
			ContentElement = pContentElement;
			OverlayElement = pOverlayElement;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public FrameworkElement ContentElement
		{
			get;
			private set;
		}
		public Canvas OverlayElement
		{
			get;
			private set;
		}
		public Rectangle BackgroundElement
		{
			get;
			private set;
		}

		public Storyboard Animation
		{
			get;
			private set;
		}
		public Clock AnimationClock
		{
			get;
			private set;
		}

		///////////////////////////////////////////////////////////////////////////////

		private class PartAnimation
		{
			public Storyboard Animation = null;
			public Boolean Enabled = false;
		}
		private List<PartAnimation> mPartAnimations = new List<PartAnimation> ();

		//=============================================================================

		public int SetPartAnimation (int pIndex, Storyboard pAnimation)
		{
			PartAnimation lPartAnimation = new PartAnimation ();

			InitOverlayAnimation (pAnimation);
			lPartAnimation.Animation = pAnimation;

			if (pIndex < mPartAnimations.Count)
			{
				mPartAnimations.Insert (pIndex, lPartAnimation);
				return pIndex;
			}
			else
			{
				mPartAnimations.Add (lPartAnimation);
				return mPartAnimations.Count - 1;
			}
		}
		public Storyboard GetPartAnimation (int pIndex)
		{
			if ((pIndex >= 0) && (pIndex < mPartAnimations.Count))
			{
				return mPartAnimations[pIndex].Animation;
			}
			return null;
		}
		IEnumerable<Storyboard> PartAnimations ()
		{
			foreach (PartAnimation lPartAnimation in mPartAnimations)
			{
				yield return lPartAnimation.Animation;
			}
		}
		IEnumerable<Storyboard> EnabledPartAnimations ()
		{
			foreach (PartAnimation lPartAnimation in mPartAnimations)
			{
				if (lPartAnimation.Enabled)
				{
					yield return lPartAnimation.Animation;
				}
			}
		}

		public Boolean SetPartEnabled (int pIndex, Boolean pEnabled)
		{
			if ((pIndex >= 0) && (pIndex < mPartAnimations.Count))
			{
				mPartAnimations[pIndex].Enabled = pEnabled;
				return true;
			}
			return false;
		}
		public Boolean GetPartEnabled (int pIndex)
		{
			if ((pIndex >= 0) && (pIndex < mPartAnimations.Count))
			{
				return mPartAnimations[pIndex].Enabled;
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////////

		public static Duration DefaultDuration
		{
			get
			{
				return new Duration (TimeSpan.FromSeconds (5));
			}
		}
		public static Double DefaultStartRest
		{
			get
			{
				return 0.2;
			}
		}
		public static Double DefaultEndRest
		{
			get
			{
				return 0.4;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean PrepareOverlay (Rectangle pBackgroundElement)
		{
			if ((OverlayElement.Children.Count <= 0) && (ContentElement.ActualWidth > 0) && (ContentElement.ActualHeight > 0))
			{
				BackgroundElement = MakeAnimationBackground (pBackgroundElement);
				return true;
			}
			return false;
		}

		public Boolean CompleteOverlay (Storyboard pAnimation)
		{
			if ((pAnimation != null) && (Animation == null))
			{
				InitOverlayAnimation (pAnimation);
				pAnimation.RepeatBehavior = RepeatBehavior.Forever;
				pAnimation.Name = String.Format ("{0}_A", ContentElement.Name);
				Animation = pAnimation;
				return true;
			}
			return false;
		}

		private void InitOverlayAnimation (Storyboard pAnimation)
		{
			pAnimation.FillBehavior = FillBehavior.Stop;
			pAnimation.AutoReverse = false;
#if false
			pAnimation.AccelerationRatio = 0.25;
			pAnimation.DecelerationRatio = 0.25;
#endif
		}

		//=============================================================================

		public virtual Boolean StartOverlayAnimation ()
		{
			StopOverlayAnimation ();

			if (Animation != null)
			{
				try
				{
					Clock lClock;
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Start Animation {0}", Animation.Name);
#endif
					foreach (PartAnimation lPartAnimation in mPartAnimations)
					{
						if (lPartAnimation.Enabled)
						{
							if (!Animation.Children.Contains (lPartAnimation.Animation))
							{
								Animation.Children.Add (lPartAnimation.Animation);
							}
						}
						else
						{
							if (Animation.Children.Contains (lPartAnimation.Animation))
							{
								Animation.Children.Remove (lPartAnimation.Animation);
							}
						}
					}

					lClock = Animation.CreateClock (true);
					Animation.Begin (OverlayElement, HandoffBehavior.SnapshotAndReplace, true);
					AnimationClock = lClock;
					return true;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			return false;
		}

		//=============================================================================

		public void StopOverlayAnimation ()
		{
			try
			{
				if (Animation != null)
				{
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Stop Animation {0}", Animation.Name);
#endif
					Animation.Stop (OverlayElement);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}

			try
			{
				if ((AnimationClock != null) && (AnimationClock.Controller != null))
				{
					AnimationClock.Controller.Remove ();
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			AnimationClock = null;
		}

		public void PauseOverlayAnimation ()
		{
			try
			{
				if (Animation != null)
				{
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Pause Animation {0}", Animation.Name);
#endif
					Animation.Pause (OverlayElement);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		public void RemoveOverlayAnimation ()
		{
			try
			{
				if ((AnimationClock != null) && (AnimationClock.Controller != null))
				{
#if DEBUG_ANIMATIONS
					System.Diagnostics.Debug.Print ("Remove Animation {0}", Animation.Name);
#endif
					AnimationClock.Controller.Remove ();
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			AnimationClock = null;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Implementation

		public Rectangle MakeAnimationBackground (Rectangle pBackgroundElement)
		{
			Rectangle lBackgroundElement = MakeAnimationElement (pBackgroundElement);

			if (lBackgroundElement != null)
			{
				lBackgroundElement.Style = pBackgroundElement.Style;
				lBackgroundElement.Fill = pBackgroundElement.Fill;
				lBackgroundElement.Stroke = pBackgroundElement.Stroke;
				lBackgroundElement.StrokeThickness = pBackgroundElement.StrokeThickness;
			}
			return lBackgroundElement;
		}

		public Rectangle MakeAnimationVisual (FrameworkElement pSourceElement)
		{
			Rectangle lAnimationVisual = MakeAnimationElement (pSourceElement);

			if (lAnimationVisual != null)
			{
				lAnimationVisual.Fill = new VisualBrush (pSourceElement);
			}
			return lAnimationVisual;
		}

		public Rectangle MakeAnimationElement (FrameworkElement pSourceElement)
		{
			Rectangle lAnimationElement = null;

			if (pSourceElement != null)
			{
				lAnimationElement = new Rectangle ();

				lAnimationElement.Name = String.Format ("{0}_A", pSourceElement.Name);
				lAnimationElement.SnapsToDevicePixels = false;
				lAnimationElement.UseLayoutRounding = pSourceElement.UseLayoutRounding;

				LayoutAnimationElement (pSourceElement, lAnimationElement);
				pSourceElement.LayoutUpdated += (object sender, EventArgs e) =>
				{
					LayoutAnimationElement (pSourceElement, lAnimationElement);
				};
				OverlayElement.Children.Add (lAnimationElement);
			}
			return lAnimationElement;
		}

		private void LayoutAnimationElement (FrameworkElement pSourceElement, Rectangle pAnimationElement)
		{
			Point lOrigin = pSourceElement.TranslatePoint (new Point (0, 0), OverlayElement);

			pAnimationElement.Width = pSourceElement.ActualWidth;
			pAnimationElement.Height = pSourceElement.ActualHeight;
			Canvas.SetLeft (pAnimationElement, lOrigin.X);
			Canvas.SetTop (pAnimationElement, lOrigin.Y);
		}

		//=============================================================================

		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement)
		{
			return MakeSwapAnimation (pAnimationVisual, pTargetElement, DefaultStartRest);
		}
		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement, Double? pStartRest)
		{
			return MakeSwapAnimation (pAnimationVisual, pTargetElement, pStartRest, DefaultEndRest);
		}
		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement, Double? pStartRest, Double? pEndRest)
		{
			try
			{
				Double lOrigin = Canvas.GetLeft (pAnimationVisual);
				Point lDestination = pTargetElement.TranslatePoint (new Point (0, 0), OverlayElement);
				DoubleAnimationOverride lAnimation = new DoubleAnimationOverride (lOrigin, lDestination.X);

				lAnimation.Duration = DefaultDuration;
				lAnimation.InitializeRewind (pStartRest, pEndRest);

#if DEBUG_NOT
				lAnimation.CurrentTimeInvalidated += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						Clock lParent;
						for (lParent = lClock; lParent.Parent != null; lParent = lParent.Parent)
						{
						}
						System.Diagnostics.Debug.Print ("{0} TimeInvalidated [{1} ({2})] [{3:F4} {4}] [{5:F4} {6}]", pAnimationVisual.Name, Canvas.GetLeft (pAnimationVisual), lAnimation.GetCurrentValue (lOrigin, lDestination.X, lClock as AnimationClock), lClock.CurrentProgress, lClock.CurrentIteration, lParent.CurrentProgress, lParent.CurrentIteration);
					}
					catch
					{
					}
				};
#endif
#if DEBUG_NOT
				lAnimation.CurrentStateInvalidated += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						System.Diagnostics.Debug.Print ("{0} StateInvalidated [{1}] [{2} ({3})] in [{4} to [{5}]", pAnimationVisual.Name, lClock.CurrentState, Canvas.GetLeft (pAnimationVisual), lAnimation.GetCurrentValue (lOrigin, lDestination.X, lClock as AnimationClock), lOrigin, lDestination.X);
					}
					catch
					{
					}
				};
#endif
#if DEBUG_NOT
				lAnimation.Completed += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						System.Diagnostics.Debug.Print ("{0} Completed [{1}] ({2})]", pAnimationVisual.Name, Canvas.GetLeft (pAnimationVisual), lAnimation.GetCurrentValue (lOrigin, lDestination.X, lClock as AnimationClock));
					}
					catch
					{
					}
				};
#endif

				Storyboard.SetTarget (lAnimation, pAnimationVisual);
				Storyboard.SetTargetProperty (lAnimation, new PropertyPath (Canvas.LeftProperty));
				return lAnimation;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}

#if false
		public AnimationTimeline MakeDoubleAnimation (Rectangle pAnimationVisual, DependencyProperty pProperty, Double pMidPoint)
		{
			try
			{
				DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();
				Double lValue = (Double)pAnimationVisual.GetValue (pProperty);

				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (lValue, KeyTime.FromPercent (0)));
				lAnimation.KeyFrames.Add (new SplineDoubleKeyFrame (lValue + pMidPoint, KeyTime.FromPercent (0.5), new KeySpline (0.0, 0.8, 0.2, 1.0)));
				lAnimation.KeyFrames.Add (new SplineDoubleKeyFrame (lValue, KeyTime.FromPercent (1.0), new KeySpline (0.8, 0.0, 1.0, 0.2)));
				lAnimation.Duration = DefaultDuration;

				Storyboard.SetTarget (lAnimation, pAnimationVisual);
				Storyboard.SetTargetProperty (lAnimation, new PropertyPath (pProperty));
				return lAnimation;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}
#endif

		//=============================================================================

		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn)
		{
			return MakeFadeAnimation (pText, pFadeIn, DefaultStartRest);
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn, Double? pStartRest)
		{
			return MakeFadeAnimation (pText, pFadeIn, pStartRest, DefaultEndRest);
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn, Double? pStartRest, Double? pEndRest)
		{
			if (pFadeIn)
			{
				return MakeFadeAnimation (pText, 0.0, 1.0, pStartRest, pEndRest);
			}
			else
			{
				return MakeFadeAnimation (pText, 1.0, 0.0, pStartRest, pEndRest);
			}
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Double pFrom, Double pTo, Double? pStartRest, Double? pEndRest)
		{
			try
			{
				if ((pStartRest.HasValue && pStartRest.Value > 0) || (pEndRest.HasValue && pEndRest.Value > 0))
				{
					DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();

					lAnimation.Duration = DefaultDuration;
					lAnimation.FillBehavior = FillBehavior.Stop;
					lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (pFrom, KeyTime.FromPercent (0)));
					if (pStartRest.HasValue && pStartRest.Value > 0)
					{
						lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (pFrom, KeyTime.FromPercent (pStartRest.Value)));
					}
					if (pEndRest.HasValue && pEndRest.Value > 0)
					{
						lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (pTo, KeyTime.FromPercent (1.0 - pEndRest.Value)));
					}
					lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (pTo, KeyTime.FromPercent (1.0)));

					Storyboard.SetTarget (lAnimation, pText);
					Storyboard.SetTargetProperty (lAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
					return lAnimation;
				}
				else
				{
					DoubleAnimation lAnimation = new DoubleAnimation ();

					lAnimation.Duration = DefaultDuration;
					lAnimation.FillBehavior = FillBehavior.Stop;
					lAnimation.From = pFrom;
					lAnimation.To = pTo;

					Storyboard.SetTarget (lAnimation, pText);
					Storyboard.SetTargetProperty (lAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
					return lAnimation;
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}

		//=============================================================================

		public AnimationTimeline MakeFlashAnimation (FrameworkElement pAnimationVisual)
		{
			try
			{
				DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();
				TimeSpan lDuration = DefaultDuration.TimeSpan;
				TimeSpan lRestTime = TimeSpan.FromSeconds ((lDuration.TotalSeconds * DefaultStartRest) - 0.3);

				lAnimation.Duration = DefaultDuration;
				lAnimation.FillBehavior = FillBehavior.Stop;
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (0));
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (0, KeyTime.FromTimeSpan (lRestTime)));
				lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (1, KeyTime.FromTimeSpan (TimeSpan.FromSeconds (lRestTime.TotalSeconds + 0.1))));
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (1, KeyTime.FromTimeSpan (TimeSpan.FromSeconds (lRestTime.TotalSeconds + 0.3))));
				lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (0, KeyTime.FromTimeSpan (TimeSpan.FromSeconds (lRestTime.TotalSeconds + 0.8))));
				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (0, KeyTime.FromTimeSpan (DefaultDuration.TimeSpan)));
#if DEBUG_NOT
				lAnimation.DebugPrint ();
				lAnimation.CurrentTimeInvalidated += (object sender, EventArgs e) =>
				{
					try
					{
						Clock lClock = sender as Clock;
						System.Diagnostics.Debug.Print ("{0} TimeInvalidated [{1:0.000} ({2:0.000})] [{3} {4:0.000} {5}]", pAnimationVisual.Name, pAnimationVisual.Opacity, lAnimation.GetCurrentValue (0, 1, lClock as AnimationClock), lClock.CurrentTime, lClock.CurrentProgress, lClock.CurrentIteration);
					}
					catch
					{
					}
				};
#endif

				Storyboard.SetTarget (lAnimation, pAnimationVisual);
				Storyboard.SetTargetProperty (lAnimation, new PropertyPath (FrameworkElement.OpacityProperty));
				return lAnimation;
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
			return null;
		}

		//=============================================================================

#if DEBUG
		public void DumpAnimations ()
		{
			try
			{
				if (Animation != null)
				{
					Animation.DebugPrint (String.Format ("{0} Animation", OverlayElement.Name), String.Empty);
				}
				foreach (PartAnimation lPartAnimation in mPartAnimations)
				{
					if (lPartAnimation.Animation != null)
					{
						lPartAnimation.Animation.DebugPrint (String.Format ("{0}   Part", OverlayElement.Name), String.Empty);
					}
				}
			}
			catch
			{
			}
		}

		public void DumpClocks ()
		{
			try
			{
				if (AnimationClock != null)
				{
					AnimationClock.DebugPrint (String.Format ("{0} Clock", OverlayElement.Name), String.Empty);
				}
			}
			catch
			{
			}
		}
#endif
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

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Wrapper for DoubleAnimationUsingKeyFrames.  This should just be a subclass
	//	but the buggers have sealed all the useful methods.
	//
	//	The purpose of this class is to allow the first repeat iteration to
	//	pick up where any previous animation left off (i.e. animate from the 
	//	current position) but force any repeats to animate from the default
	//	origin.
	//
	/////////////////////////////////////////////////////////////////////////////

	[ContentProperty ("KeyFrames")]
	internal class DoubleAnimationOverride : DoubleAnimationBase, IKeyFrameAnimation, IAddChild
	{
		/////////////////////////////////////////////////////////////////////////////
		#region Initialization

		DoubleAnimationUsingKeyFrames mAnimation = new DoubleAnimationUsingKeyFrames ();
		Double mOrigin = 0;
		Double mDestination = 0;
		Boolean mRepeatRewind = false;

		public DoubleAnimationOverride (Double pOrigin, Double pDestination)
		{
			mOrigin = pOrigin;
			mDestination = pDestination;
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("DoubleAnimationOverride [{0} {1}]", mOrigin, mDestination);
#endif
		}
		protected DoubleAnimationOverride ()
		{
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Properties

		public DoubleAnimationUsingKeyFrames InnerAnimation
		{
			get
			{
				return mAnimation;
			}
		}

		public DoubleKeyFrameCollection KeyFrames
		{
			get
			{
				return mAnimation.KeyFrames;
			}
			set
			{
				mAnimation.KeyFrames = value;
			}
		}

		System.Collections.IList IKeyFrameAnimation.KeyFrames
		{
			get
			{
				return mAnimation.KeyFrames;
			}
			set
			{
				mAnimation.KeyFrames = value as DoubleKeyFrameCollection;
			}
		}

		public override bool IsDestinationDefault
		{
			get
			{
				return mAnimation.IsDestinationDefault;
			}
		}

		public static Double RewindRatio
		{
			get
			{
				return 0.025;
			}
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Infrastructure

		public virtual void AddChild (object child)
		{
		}
		public virtual void AddText (string childText)
		{
		}
		public bool ShouldSerializeKeyFrames ()
		{
			return mAnimation.ShouldSerializeKeyFrames ();
		}

		//=============================================================================

		public new DoubleAnimationOverride Clone ()
		{
			DoubleAnimationOverride lClone = CreateInstanceCore () as DoubleAnimationOverride;
			lClone.CloneCore (this);
			return lClone;
		}
		protected override void CloneCore (Freezable sourceFreezable)
		{
			base.CloneCore (sourceFreezable);
			if (sourceFreezable is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (sourceFreezable as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.Clone ();
			}
		}
		public new DoubleAnimationOverride CloneCurrentValue ()
		{
			DoubleAnimationOverride lClone = CreateInstanceCore () as DoubleAnimationOverride;
			lClone.CloneCurrentValueCore (this);
			return lClone;
		}
		protected override void CloneCurrentValueCore (Freezable sourceFreezable)
		{
			base.CloneCurrentValueCore (sourceFreezable);
			if (sourceFreezable is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (sourceFreezable as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.CloneCurrentValue ();
			}
		}
		protected override Freezable CreateInstanceCore ()
		{
			return new DoubleAnimationOverride ();
		}
		protected override bool FreezeCore (bool isChecking)
		{
			if (!isChecking && !mAnimation.IsFrozen)
			{
				mAnimation.Duration = Duration;
			}
			return base.FreezeCore (isChecking);
		}
		protected override void GetAsFrozenCore (Freezable source)
		{
			base.GetAsFrozenCore (source);
			if (source is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (source as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.GetAsFrozen () as DoubleAnimationUsingKeyFrames;
			}
		}
		protected override void GetCurrentValueAsFrozenCore (Freezable source)
		{
			base.GetCurrentValueAsFrozenCore (source);
			if (source is DoubleAnimationOverride)
			{
				DoubleAnimationOverride lSource = (source as DoubleAnimationOverride);
				this.Duration = lSource.Duration;
				this.mOrigin = lSource.mOrigin;
				this.mDestination = lSource.mDestination;
				this.mRepeatRewind = lSource.mRepeatRewind;
				this.mAnimation = lSource.mAnimation.GetCurrentValueAsFrozen () as DoubleAnimationUsingKeyFrames;
			}
		}

		//=============================================================================

		protected override void OnPropertyChanged (DependencyPropertyChangedEventArgs e)
		{
			base.OnPropertyChanged (e);

			if (e.Property == DurationProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.Duration = Duration;
				}
			}
			else if (e.Property == FillBehaviorProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.FillBehavior = FillBehavior;
				}
			}
			else if (e.Property == RepeatBehaviorProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.RepeatBehavior = RepeatBehavior;
				}
			}
			else if (e.Property == AutoReverseProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					mAnimation.AutoReverse = AutoReverse;
				}
			}
			else if (e.Property == Storyboard.TargetProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					Storyboard.SetTarget (mAnimation, Storyboard.GetTarget (this));
				}
			}
			else if (e.Property == Storyboard.TargetNameProperty)
			{
				if (!mAnimation.IsFrozen)
				{
					Storyboard.SetTargetName (mAnimation, Storyboard.GetTargetName (this));
				}
			}
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Implementation

		public void InitializeRewind ()
		{
			InitializeRewind (null, null);
		}
		public void InitializeRewind (Double? pStartRest)
		{
			InitializeRewind (pStartRest, null);
		}
		public void InitializeRewind (Double? pStartRest, Double? pEndRest)
		{
			InitializeRewind (pStartRest, pEndRest, false);
		}
		public void InitializeRewind (Double? pStartRest, Double? pEndRest, Boolean pRepeatRewind)
		{
			mRepeatRewind = pRepeatRewind;
			FillBehavior = FillBehavior.Stop;
			KeyFrames.Add (new LinearDoubleKeyFrame (mOrigin, KeyTime.FromPercent (RewindRatio)));

			if ((pStartRest.HasValue) && (pStartRest.Value > RewindRatio))
			{
				KeyFrames.Add (new DiscreteDoubleKeyFrame (mOrigin, KeyTime.FromPercent (pStartRest.Value)));
			}
			if ((pEndRest.HasValue) && (pEndRest.Value > 0))
			{
				KeyFrames.Add (new LinearDoubleKeyFrame (mDestination, KeyTime.FromPercent (1.0 - pEndRest.Value)));
			}
			KeyFrames.Add (new LinearDoubleKeyFrame (mDestination, KeyTime.FromPercent (1.0)));
		}

		protected override Duration GetNaturalDurationCore (Clock clock)
		{
			return Duration;
		}

		protected override double GetCurrentValueCore (double defaultOriginValue, double defaultDestinationValue, AnimationClock animationClock)
		{
			double lRet;
			Clock lMasterClock;

			for (lMasterClock = animationClock; lMasterClock.Parent != null; lMasterClock = lMasterClock.Parent)
			{
			}
			if (lMasterClock.CurrentIteration > 1)
			{
				if (mRepeatRewind)
				{
					defaultOriginValue = mDestination;
					defaultDestinationValue = mOrigin;
				}
				else
				{
					defaultOriginValue = mOrigin;
					defaultDestinationValue = mDestination;
				}
			}
			lRet = mAnimation.GetCurrentValue (defaultOriginValue, defaultDestinationValue, animationClock);
#if DEBUG_NOT
			try
			{
				System.Diagnostics.Debug.Print ("GCV [{0:000.0000}] [{1:000.0000} {2:000.0000}] [{3} {4::000.0000} {5}] [{6} {7::000.0000} {8}]", lRet, defaultOriginValue, defaultDestinationValue, animationClock.CurrentTime, animationClock.CurrentProgress, animationClock.CurrentIteration, lMasterClock.CurrentTime, lMasterClock.CurrentProgress, lMasterClock.CurrentIteration);
			}
			catch
			{
			}
#endif
			return lRet;
		}

		#endregion
	}
}
