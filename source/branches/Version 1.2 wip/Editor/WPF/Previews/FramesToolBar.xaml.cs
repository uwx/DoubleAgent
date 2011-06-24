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
		}

		public void LoadConfig ()
		{
			Settings lSettings = Settings.Default;

			if (lSettings.IsValid)
			{
				AddShiftsBranchingTarget = lSettings.AddFrameShiftsBranchingTarget;
				DeleteShiftsBranchingTarget = lSettings.DeleteFrameShiftsBranchingTarget;
				DeleteMovesBranchingUp = lSettings.DeleteFrameShiftsBranchingUp;
				DeleteMovesBranchingDown = lSettings.DeleteFrameShiftsBranchingDown;
				MoveUpMovesBranchingSource = lSettings.MoveFrameUpMovesBranchingSource;
				MoveUpMovesBranchingTarget = lSettings.MoveFrameUpMovesBranchingTarget;
				MoveDownMovesBranchingSource = lSettings.MoveFrameDownMovesBranchingSource;
				MoveDownMovesBranchingTarget = lSettings.MoveFrameDownMovesBranchingTarget;
			}
		}

		public void SaveConfig ()
		{
			Settings lSettings = Settings.Default;

			lSettings.AddFrameShiftsBranchingTarget = AddShiftsBranchingTarget;
			lSettings.DeleteFrameShiftsBranchingTarget = DeleteShiftsBranchingTarget;
			lSettings.DeleteFrameShiftsBranchingUp = DeleteMovesBranchingUp;
			lSettings.DeleteFrameShiftsBranchingDown = MoveUpMovesBranchingSource;
			lSettings.MoveFrameUpMovesBranchingSource = MoveUpMovesBranchingSource;
			lSettings.MoveFrameUpMovesBranchingTarget = MoveUpMovesBranchingTarget;
			lSettings.MoveFrameDownMovesBranchingSource = MoveDownMovesBranchingSource;
			lSettings.MoveFrameDownMovesBranchingTarget = MoveDownMovesBranchingTarget;
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
				return Program.FileIsReadOnly ? Visibility.Collapsed : (AddShiftsBranchingTarget && CanMoveFrameDown) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowAddKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (AddKeepsBranchingTarget && CanMoveFrameDown) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		public Visibility ShowDeleteShiftsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteShiftsBranchingTarget && CanMoveFrameDown) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteShiftsBranchingTargetUp
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteShiftsBranchingTarget && !CanMoveFrameDown && CanMoveFrameUp) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteKeepsBranchingTarget && CanMoveFrameDown) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteMovesBranchingUp
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteMovesBranchingUp && CanMoveFrameUp && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteMovesBranchingDown
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteMovesBranchingDown && CanMoveFrameDown && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteDeletesBranching
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : ((!DeleteMovesBranchingUp || !CanMoveFrameUp) && (!DeleteMovesBranchingDown || !CanMoveFrameDown) && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
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
		public String SayDeleteMovesBranchingUp
		{
			get
			{
				return String.Empty;
			}
		}
		public String SayDeleteMovesBranchingDown
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

		public Visibility ShowMoveUpMovesBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveUpMovesBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveUpKeepsBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveUpKeepsBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveUpMovesBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveUpMovesBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveUpKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveUpKeepsBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		//=============================================================================

		public Visibility ShowMoveDownMovesBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveDownMovesBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveDownKeepsBranchingSource
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveDownKeepsBranchingSource && FrameIsBranchingSource (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveDownMovesBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveDownMovesBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowMoveDownKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (MoveDownKeepsBranchingTarget && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}

		//=============================================================================

		public String SayMoveUpMovesBranchingSource
		{
			get
			{
				return (Frame == null) ? "Keep the frame's branching at its original position" : FrameIsBranchingSource (Frame) ? String.Format ("Swap {0}'s branching with frame {1}", Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMoveUpKeepsBranchingSource
		{
			get
			{
				return (Frame == null) ? "Move the frame's branching with it" : FrameIsBranchingSource (Frame) ? String.Format ("Keep {0}'s branching when it's moved", Titles.Frame (Frame)) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMoveUpMovesBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Change branching destinations to the frame's new position" : FrameIsBranchingTarget (Frame) ? String.Format ("Swap branching destinations for frames {0} and {1}", FramesPreview.Frames.SelectedIndex + 1, FramesPreview.Frames.SelectedIndex) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}
		public String SayMoveUpKeepsBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Keep branching destinations at the frame's original position" : FrameIsBranchingTarget (Frame) ? String.Format ("Don't change any branching destinations when {0} is moved", Titles.Frame (Frame)) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}

		//=============================================================================

		public String SayMoveDownMovesBranchingSource
		{
			get
			{
				return (Frame == null) ? "Keep the frame's branching at its original position" : FrameIsBranchingSource (Frame) ? String.Format ("Swap {0}'s branching with frame {1}", Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex + 2) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMoveDownKeepsBranchingSource
		{
			get
			{
				return (Frame == null) ? "Move the frame's branching with it" : FrameIsBranchingSource (Frame) ? String.Format ("Keep {0}'s branching when it's moved", Titles.Frame (Frame)) : String.Format ("{0} has no branching", Titles.Frame (Frame));
			}
		}
		public String SayMoveDownMovesBranchingTarget
		{
			get
			{
				return (Frame == null) ? "Change branching destinations to the frame's new position" : FrameIsBranchingTarget (Frame) ? String.Format ("Swap branching destinations for frames {0} and {1}", FramesPreview.Frames.SelectedIndex + 1, FramesPreview.Frames.SelectedIndex + 2) : String.Format ("{0} is not a branching destination", Titles.Frame (Frame));
			}
		}
		public String SayMoveDownKeepsBranchingTarget
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
		public Boolean EnableMoveUpBranchingDefaultOptions
		{
			get
			{
				return !Program.FileIsReadOnly && !MoveUpBranchingDefaultOptions;
			}
		}
		public Boolean EnableMoveDownBranchingDefaultOptions
		{
			get
			{
				return !Program.FileIsReadOnly && !MoveDownBranchingDefaultOptions;
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

				NotifyPropertyChanged ("DeleteMovesBranchingUp");
				NotifyPropertyChanged ("DeleteMovesBranchingDown");
				NotifyPropertyChanged ("DeleteDeletesBranching");
				NotifyPropertyChanged ("ShowDeleteMovesBranchingUp");
				NotifyPropertyChanged ("ShowDeleteMovesBranchingDown");
				NotifyPropertyChanged ("ShowDeleteDeletesBranching");
				NotifyPropertyChanged ("SayDeleteMovesBranchingUp");
				NotifyPropertyChanged ("SayDeleteMovesBranchingDown");
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
				NotifyPropertyChanged ("ShowDeleteShiftsBranchingTargetUp");
				NotifyPropertyChanged ("ShowDeleteKeepsBranchingTarget");
				NotifyPropertyChanged ("SayDeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("SayDeleteKeepsBranchingTarget");

				NotifyPropertyChanged ("DeleteBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableDeleteBranchingDefaultOptions");

				UpdateDeleteAnimation (100);
			}
		}

		//=============================================================================

		private void MoveUpBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMoveUpAnimation (-1);

				NotifyPropertyChanged ("MoveUpMovesBranchingSource");
				NotifyPropertyChanged ("MoveUpKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMoveUpMovesBranchingSource");
				NotifyPropertyChanged ("ShowMoveUpKeepsBranchingSource");
				NotifyPropertyChanged ("SayMoveUpMovesBranchingSource");
				NotifyPropertyChanged ("SayMoveUpKeepsBranchingSource");

				NotifyPropertyChanged ("MoveUpBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveUpBranchingDefaultOptions");

				UpdateMoveUpAnimation (100);
			}
		}
		private void MoveUpBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMoveUpAnimation (-1);

				NotifyPropertyChanged ("MoveUpMovesBranchingTarget");
				NotifyPropertyChanged ("MoveUpKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMoveUpMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMoveUpKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMoveUpMovesBranchingTarget");
				NotifyPropertyChanged ("SayMoveUpKeepsBranchingTarget");

				NotifyPropertyChanged ("MoveUpBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveUpBranchingDefaultOptions");

				UpdateMoveUpAnimation (100);
			}
		}
		private void MoveDownBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMoveDownAnimation (-1);

				NotifyPropertyChanged ("MoveDownMovesBranchingSource");
				NotifyPropertyChanged ("MoveDownKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMoveDownMovesBranchingSource");
				NotifyPropertyChanged ("ShowMoveDownKeepsBranchingSource");
				NotifyPropertyChanged ("SayMoveDownMovesBranchingSource");
				NotifyPropertyChanged ("SayMoveDownKeepsBranchingSource");

				NotifyPropertyChanged ("MoveDownBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveDownBranchingDefaultOptions");

				UpdateMoveDownAnimation (100);
			}
		}
		private void MoveDownBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				UpdateMoveDownAnimation (-1);

				NotifyPropertyChanged ("MoveDownMovesBranchingTarget");
				NotifyPropertyChanged ("MoveDownKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMoveDownMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMoveDownKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMoveDownMovesBranchingTarget");
				NotifyPropertyChanged ("SayMoveDownKeepsBranchingTarget");

				NotifyPropertyChanged ("MoveDownBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveDownBranchingDefaultOptions");

				UpdateMoveDownAnimation (100);
			}
		}

		//=============================================================================

		private void OnFrameStateChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("CanMoveFrameUp");
				NotifyPropertyChanged ("CanMoveFrameDown");
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
				MoveUpBranchingSourceChanged ();
				MoveUpBranchingTargetChanged ();
				MoveDownBranchingSourceChanged ();
				MoveDownBranchingTargetChanged ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Animations

		private ExpanderContentOverlay mAddAnimation = null;
		private AsyncTimer mAddAnimationDelay = null;

		private void AddGrid_LayoutUpdated (object sender, EventArgs e)
		{
			if (ExpanderAdd.IsExpanded)
			{
				try
				{
					if (mAddAnimation == null)
					{
						mAddAnimation = new ExpanderContentOverlay (ExpanderAdd, AddContent, AddCanvas);
					}
					if (mAddAnimation.PrepareOverlay (AddFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;

						lStoryboard.Children.Add (mAddAnimation.MakeFlashAnimation (AddGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mAddAnimation.MakeSwapAnimation (mAddAnimation.MakeAnimationVisual (AddBlt1), AddBlt2, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mAddAnimation.MakeSwapAnimation (mAddAnimation.MakeAnimationVisual (AddBlt2), AddBlt3, AnimationOverlay.DefaultRestRatio));
						mAddAnimation.SetPartAnimation (0, lPartAnimation);

						lStoryboard.Children.Add (mAddAnimation.MakeFadeAnimation (mAddAnimation.MakeAnimationVisual (AddBt0), true, AnimationOverlay.DefaultRestRatio));

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mAddAnimation.MakeSwapAnimation (mAddAnimation.MakeAnimationVisual (AddBt1), AddBt2, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mAddAnimation.MakeFadeAnimation (AddBt1T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mAddAnimation.MakeFadeAnimation (AddBt1T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mAddAnimation.MakeSwapAnimation (mAddAnimation.MakeAnimationVisual (AddBt2), AddBt3, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mAddAnimation.MakeFadeAnimation (AddBt2T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mAddAnimation.MakeFadeAnimation (AddBt2T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);

						UpdateAddAnimation ();
						mAddAnimation.StartOverlayAnimation (lStoryboard);
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		//=============================================================================

		private void UpdateAddAnimation ()
		{
			if (Frame != null)
			{
				if (mAddAnimation != null)
				{
					AddBt2.Visibility = CanMoveFrameDown ? Visibility.Visible : Visibility.Hidden;
					AddBlt2.Visibility = CanMoveFrameDown ? Visibility.Visible : Visibility.Hidden;
					AddBt0T1.Visibility = AddShiftsBranchingTarget ? Visibility.Hidden : Visibility.Visible;
					AddBt0T2.Visibility = AddShiftsBranchingTarget ? Visibility.Visible : Visibility.Hidden;

					mAddAnimation.SetPartEnabled (0, AddShiftsBranchingTarget);
					mAddAnimation.OverlayElement.Visibility = Visibility.Visible;
					mAddAnimation.StartOverlayAnimation (mAddAnimation.Animation);
				}
				AddKeepBranchingTarget.Visibility = Visibility.Visible;
				AddShiftBranchingTarget.Visibility = Visibility.Visible;
				AddDefaults.Visibility = Visibility.Visible;
				AddNone.Visibility = Visibility.Collapsed;
			}
			else
			{
				if (mAddAnimation != null)
				{
					mAddAnimation.StopOverlayAnimation ();
					mAddAnimation.OverlayElement.Visibility = Visibility.Hidden;
				}
				AddKeepBranchingTarget.Visibility = Visibility.Hidden;
				AddShiftBranchingTarget.Visibility = Visibility.Hidden;
				AddDefaults.Visibility = Visibility.Collapsed;
				AddNone.Visibility = Visibility.Visible;
			}
		}

		//=============================================================================

		private void UpdateAddAnimation (int pDelay)
		{
			if (pDelay == 0)
			{
				UpdateAddAnimation ();
			}
			else if (pDelay < 0)
			{
				mAddAnimationDelay = null;
			}
			else
			{
				if (mAddAnimationDelay == null)
				{
					mAddAnimationDelay = new AsyncTimer ();
					mAddAnimationDelay.TimerPulse += new AsyncTimer.TimerPulseHandler (AddAnimationDelay_TimerPulse);
					mAddAnimationDelay.Start (pDelay);
				}
			}
		}

		void AddAnimationDelay_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			if (mAddAnimationDelay != null)
			{
				mAddAnimationDelay = null;
				UpdateAddAnimation ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private ExpanderContentOverlay mDeleteAnimation = null;
		private AsyncTimer mDeleteAnimationDelay = null;

		private void DelGrid_LayoutUpdated (object sender, EventArgs e)
		{
			if (ExpanderDelete.IsExpanded)
			{
				try
				{
					if (mDeleteAnimation == null)
					{
						mDeleteAnimation = new ExpanderContentOverlay (ExpanderDelete, DelContent, DelCanvas);
					}
					if (mDeleteAnimation.PrepareOverlay (DelFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;
						Rectangle lAnimationVisual;

						lStoryboard.Children.Add (mDeleteAnimation.MakeFlashAnimation (DelGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mDeleteAnimation.MakeFadeAnimation (lAnimationVisual = mDeleteAnimation.MakeAnimationVisual (DelBls1), false, AnimationOverlay.DefaultRestRatio));
						mDeleteAnimation.SetPartAnimation (0, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mDeleteAnimation.MakeSwapAnimation (lAnimationVisual, DelBls0, AnimationOverlay.DefaultRestRatio));
						mDeleteAnimation.SetPartAnimation (1, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mDeleteAnimation.MakeSwapAnimation (mDeleteAnimation.MakeAnimationVisual (DelBls2), DelBls1, AnimationOverlay.DefaultRestRatio));
						mDeleteAnimation.SetPartAnimation (2, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mDeleteAnimation.MakeSwapAnimation (mDeleteAnimation.MakeAnimationVisual (DelBlt2), DelBlt1, AnimationOverlay.DefaultRestRatio));
						mDeleteAnimation.SetPartAnimation (3, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mDeleteAnimation.MakeSwapAnimation (mDeleteAnimation.MakeAnimationVisual (DelBlt1), DelBlt0, AnimationOverlay.DefaultRestRatio));
						mDeleteAnimation.SetPartAnimation (4, lPartAnimation);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mDeleteAnimation.MakeFadeAnimation (mDeleteAnimation.MakeAnimationVisual (DelBs1), false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mDeleteAnimation.MakeFadeAnimation (mDeleteAnimation.MakeAnimationVisual (DelBt1), false, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mDeleteAnimation.MakeSwapAnimation (mDeleteAnimation.MakeAnimationVisual (DelBs2), DelBs1, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mDeleteAnimation.MakeFadeAnimation (DelBs2T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mDeleteAnimation.MakeFadeAnimation (DelBs2T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mDeleteAnimation.MakeSwapAnimation (mDeleteAnimation.MakeAnimationVisual (DelBt2), DelBt1, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mDeleteAnimation.MakeFadeAnimation (DelBt2T1, false, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mDeleteAnimation.MakeFadeAnimation (DelBt2T2, true, AnimationOverlay.DefaultRestRatio));
						mDeleteAnimation.SetPartAnimation (5, lPartAnimation);

						mDeleteAnimation.MakeAnimationVisual (DelBs0);
						mDeleteAnimation.MakeAnimationVisual (DelBt0);

						UpdateDeleteAnimation ();
						mDeleteAnimation.StartOverlayAnimation (lStoryboard);
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		//=============================================================================

		private void UpdateDeleteAnimation ()
		{
			if (CanMoveFrameDown || CanMoveFrameUp)
			{
				DelBs0.Visibility = CanMoveFrameUp ? Visibility.Visible : Visibility.Hidden;
				DelBls0.Visibility = CanMoveFrameUp ? Visibility.Visible : Visibility.Hidden;
				DelBs2.Visibility = CanMoveFrameDown ? Visibility.Visible : Visibility.Hidden;
				DelBls2.Visibility = CanMoveFrameDown ? Visibility.Visible : Visibility.Hidden;
				DelBt0.Visibility = CanMoveFrameDown ? Visibility.Hidden : Visibility.Visible;
				DelBt2.Visibility = CanMoveFrameDown ? Visibility.Visible : Visibility.Hidden;
				DelBlt2.Visibility = CanMoveFrameDown ? Visibility.Visible : Visibility.Hidden;

				if (CanMoveFrameDown)
				{
					Grid.SetColumn (DeleteKeepBranchingTarget, Grid.GetColumn (DeleteMoveBranchingDown));
					Grid.SetColumn (DeleteShiftBranchingTarget, Grid.GetColumn (DeleteMoveBranchingDown));
				}
				else
				{
					Grid.SetColumn (DeleteKeepBranchingTarget, Grid.GetColumn (DeleteMoveBranchingUp));
					Grid.SetColumn (DeleteShiftBranchingTarget, Grid.GetColumn (DeleteMoveBranchingUp));
				}

				if (mDeleteAnimation != null)
				{
					mDeleteAnimation.SetPartEnabled (0, DeleteDeletesBranching);
					mDeleteAnimation.SetPartEnabled (1, DeleteMovesBranchingUp && CanMoveFrameUp);
					mDeleteAnimation.SetPartEnabled (2, CanMoveFrameDown);
					mDeleteAnimation.SetPartEnabled (3, DeleteShiftsBranchingTarget && CanMoveFrameDown);
					mDeleteAnimation.SetPartEnabled (4, DeleteShiftsBranchingTarget && !CanMoveFrameDown);
					mDeleteAnimation.SetPartEnabled (5, CanMoveFrameDown);
					mDeleteAnimation.OverlayElement.Visibility = Visibility.Visible;
					mDeleteAnimation.StartOverlayAnimation (mDeleteAnimation.Animation);
				}

				DeleteDefaults.Visibility = Visibility.Visible;
				DelNone.Visibility = Visibility.Collapsed;
			}
			else
			{
				if (mDeleteAnimation != null)
				{
					mDeleteAnimation.StopOverlayAnimation ();
					mDeleteAnimation.OverlayElement.Visibility = Visibility.Hidden;
				}

				DeleteDefaults.Visibility = Visibility.Collapsed;
				DelNone.Visibility = Visibility.Visible;
			}
		}

		//=============================================================================

		private void UpdateDeleteAnimation (int pDelay)
		{
			if (pDelay == 0)
			{
				UpdateDeleteAnimation ();
			}
			else if (pDelay < 0)
			{
				mDeleteAnimationDelay = null;
			}
			else
			{
				if (mDeleteAnimationDelay == null)
				{
					mDeleteAnimationDelay = new AsyncTimer ();
					mDeleteAnimationDelay.TimerPulse += new AsyncTimer.TimerPulseHandler (DeleteAnimationDelay_TimerPulse);
					mDeleteAnimationDelay.Start (pDelay);
				}
			}
		}

		void DeleteAnimationDelay_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			if (mDeleteAnimationDelay != null)
			{
				mDeleteAnimationDelay = null;
				UpdateDeleteAnimation ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private ExpanderContentOverlay mMoveUpAnimation = null;
		private AsyncTimer mMoveUpAnimationDelay = null;

		private void MuGrid_LayoutUpdated (object sender, System.EventArgs e)
		{
			if (ExpanderMoveUp.IsExpanded)
			{
				try
				{
					if (mMoveUpAnimation == null)
					{
						mMoveUpAnimation = new ExpanderContentOverlay (ExpanderMoveUp, MuContent, MuCanvas);
					}
					if (mMoveUpAnimation.PrepareOverlay (MuFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;

						lStoryboard.Children.Add (mMoveUpAnimation.MakeFlashAnimation (MuGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBls1), MuBls2, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBls2), MuBls1, AnimationOverlay.DefaultRestRatio));
						mMoveUpAnimation.SetPartAnimation (0, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBlt2), MuBlt1, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBlt1), MuBlt2, AnimationOverlay.DefaultRestRatio));
						mMoveUpAnimation.SetPartAnimation (1, lPartAnimation);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBs1), MuBs2, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBs2), MuBs1, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBs1T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBs1T2, true, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBs2T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBs2T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBt1), MuBt2, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBt2), MuBt1, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBt1T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBt1T2, true, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBt2T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeFadeAnimation (MuBt2T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);
#if DEBUG_NOT
						MdContent.UpdateLayout ();
						MuContent.DebugPrint ();
#endif
						UpdateMoveUpAnimation ();
						mMoveUpAnimation.StartOverlayAnimation (lStoryboard);
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		//=============================================================================

		private void UpdateMoveUpAnimation ()
		{
			if (mMoveUpAnimation != null)
			{
				mMoveUpAnimation.SetPartEnabled (0, !MoveUpMovesBranchingSource);
				mMoveUpAnimation.SetPartEnabled (1, MoveUpMovesBranchingTarget);
				mMoveUpAnimation.StartOverlayAnimation (mMoveUpAnimation.Animation);
			}
		}

		//=============================================================================

		private void UpdateMoveUpAnimation (int pDelay)
		{
			if (pDelay == 0)
			{
				UpdateMoveUpAnimation ();
			}
			else if (pDelay < 0)
			{
				mMoveUpAnimationDelay = null;
			}
			else
			{
				if (mMoveUpAnimationDelay == null)
				{
					mMoveUpAnimationDelay = new AsyncTimer ();
					mMoveUpAnimationDelay.TimerPulse += new AsyncTimer.TimerPulseHandler (MoveUpAnimationDelay_TimerPulse);
					mMoveUpAnimationDelay.Start (pDelay);
				}
			}
		}

		void MoveUpAnimationDelay_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			if (mMoveUpAnimationDelay != null)
			{
				mMoveUpAnimationDelay = null;
				UpdateMoveUpAnimation ();
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		private ExpanderContentOverlay mMoveDownAnimation = null;
		private AsyncTimer mMoveDownAnimationDelay = null;

		private void MdGrid_LayoutUpdated (object sender, EventArgs e)
		{
			if (ExpanderMoveDown.IsExpanded)
			{
				try
				{
					if (mMoveDownAnimation == null)
					{
						mMoveDownAnimation = new ExpanderContentOverlay (ExpanderMoveDown, MdContent, MdCanvas);
					}
					if (mMoveDownAnimation.PrepareOverlay (MdFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						Storyboard lPartAnimation;
						ParallelTimeline lAnimationGroup;

						lStoryboard.Children.Add (mMoveDownAnimation.MakeFlashAnimation (MdGlow));

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBls1), MdBls2, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBls2), MdBls1, AnimationOverlay.DefaultRestRatio));
						mMoveDownAnimation.SetPartAnimation (0, lPartAnimation);

						lPartAnimation = new Storyboard ();
						lPartAnimation.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBlt1), MdBlt2, AnimationOverlay.DefaultRestRatio));
						lPartAnimation.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBlt2), MdBlt1, AnimationOverlay.DefaultRestRatio));
						mMoveDownAnimation.SetPartAnimation (1, lPartAnimation);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBs1), MdBs2, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBs2), MdBs1, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBs1T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBs1T2, true, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBs2T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBs2T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBt1), MdBt2, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (mMoveDownAnimation.MakeAnimationVisual (MdBt2), MdBt1, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBt1T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBt1T2, true, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBt2T1, false, AnimationOverlay.DefaultRestRatio));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeFadeAnimation (MdBt2T2, true, AnimationOverlay.DefaultRestRatio));
						lStoryboard.Children.Add (lAnimationGroup);
#if DEBUG_NOT
						MdContent.UpdateLayout ();
						MdContent.DebugPrint ();
#endif
						UpdateMoveDownAnimation ();
						mMoveDownAnimation.StartOverlayAnimation (lStoryboard);
#if DEBUG_NOT
						mMoveDownAnimation.DumpAnimations ();
						mMoveDownAnimation.DumpClocks ();
#endif
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		//=============================================================================

		private void UpdateMoveDownAnimation ()
		{
			if (mMoveDownAnimation != null)
			{
				mMoveDownAnimation.SetPartEnabled (0, !MoveDownMovesBranchingSource);
				mMoveDownAnimation.SetPartEnabled (1, MoveDownMovesBranchingTarget);
				mMoveDownAnimation.StartOverlayAnimation (mMoveDownAnimation.Animation);
			}
		}

		//=============================================================================

		private void UpdateMoveDownAnimation (int pDelay)
		{
			if (pDelay == 0)
			{
				UpdateMoveDownAnimation ();
			}
			else if (pDelay < 0)
			{
				mMoveDownAnimationDelay = null;
			}
			else
			{
				if (mMoveDownAnimationDelay == null)
				{
					mMoveDownAnimationDelay = new AsyncTimer ();
					mMoveDownAnimationDelay.TimerPulse += new AsyncTimer.TimerPulseHandler (MoveDownAnimationDelay_TimerPulse);
					mMoveDownAnimationDelay.Start (pDelay);
				}
			}
		}

		void MoveDownAnimationDelay_TimerPulse (object sender, AsyncTimer.TimerEventArgs e)
		{
			if (mMoveDownAnimationDelay != null)
			{
				mMoveDownAnimationDelay = null;
				UpdateMoveDownAnimation ();
			}
		}

		#endregion
	}

	/////////////////////////////////////////////////////////////////////////////

	internal class AnimationOverlay
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public AnimationOverlay (FrameworkElement pBaseElement, Canvas pOverlayElement)
		{
			BaseElement = pBaseElement;
			OverlayElement = pOverlayElement;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public FrameworkElement BaseElement
		{
			get;
			protected set;
		}
		public Canvas OverlayElement
		{
			get;
			protected set;
		}
		public Rectangle BackgroundElement
		{
			get;
			protected set;
		}

		public Storyboard Animation
		{
			get;
			protected set;
		}
		public Clock AnimationClock
		{
			get;
			protected set;
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
		public static Double DefaultRestRatio
		{
			get
			{
				return 0.2;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		public Boolean PrepareOverlay (Rectangle pBackgroundElement)
		{
			if ((OverlayElement.Children.Count <= 0) && (BaseElement.ActualWidth > 0) && (BaseElement.ActualHeight > 0))
			{
				BackgroundElement = MakeAnimationBackground (pBackgroundElement);
				return true;
			}
			return false;
		}

		//=============================================================================

		public Boolean StartOverlayAnimation (Storyboard pAnimation)
		{
			StopOverlayAnimation ();

			if ((pAnimation != null) && (pAnimation != Animation))
			{
				InitOverlayAnimation (pAnimation);
				pAnimation.RepeatBehavior = RepeatBehavior.Forever;
				pAnimation.Name = String.Format ("{0}_A", BaseElement.Name);
			}

			if (pAnimation != null)
			{
				try
				{
					Clock lClock;
#if DEBUG
					System.Diagnostics.Debug.Print ("Start Animation {0}", pAnimation.Name);
#endif
					foreach (PartAnimation lPartAnimation in mPartAnimations)
					{
						if (lPartAnimation.Enabled)
						{
							if (!pAnimation.Children.Contains (lPartAnimation.Animation))
							{
								pAnimation.Children.Add (lPartAnimation.Animation);
							}
						}
						else
						{
							if (pAnimation.Children.Contains (lPartAnimation.Animation))
							{
								pAnimation.Children.Remove (lPartAnimation.Animation);
							}
						}
					}

					lClock = pAnimation.CreateClock (true);
					pAnimation.Begin (OverlayElement, HandoffBehavior.SnapshotAndReplace, true);
					Animation = pAnimation;
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

		public void StopOverlayAnimation ()
		{
			try
			{
				if (Animation != null)
				{
#if DEBUG
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
#if DEBUG
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
#if DEBUG
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
				lAnimationElement.UseLayoutRounding = false;

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
			return MakeSwapAnimation (pAnimationVisual, pTargetElement, null);
		}

		public AnimationTimeline MakeSwapAnimation (Rectangle pAnimationVisual, FrameworkElement pTargetElement, Double? pRestRatio)
		{
			try
			{
				Double lOrigin = Canvas.GetLeft (pAnimationVisual);
				Point lDestination = pTargetElement.TranslatePoint (new Point (0, 0), OverlayElement);
				DoubleAnimationOverride lAnimation = new DoubleAnimationOverride (lOrigin, lDestination.X);

				lAnimation.Duration = DefaultDuration;
				lAnimation.InitializeRewind (pRestRatio);

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
			if (pFadeIn)
			{
				return MakeFadeAnimation (pText, 0.0, 1.0, (Double?)null);
			}
			else
			{
				return MakeFadeAnimation (pText, 1.0, 0.0, (Double?)null);
			}
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Boolean pFadeIn, Double? pRestRatio)
		{
			if (pFadeIn)
			{
				return MakeFadeAnimation (pText, 0.0, 1.0, pRestRatio);
			}
			else
			{
				return MakeFadeAnimation (pText, 1.0, 0.0, pRestRatio);
			}
		}
		public AnimationTimeline MakeFadeAnimation (FrameworkElement pText, Double pFrom, Double pTo, Double? pRestRatio)
		{
			try
			{
				if (pRestRatio.HasValue)
				{
					DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();

					lAnimation.Duration = DefaultDuration;
					lAnimation.FillBehavior = FillBehavior.Stop;
					lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (pFrom, KeyTime.FromPercent (0)));
					lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (pFrom, KeyTime.FromPercent (pRestRatio.Value)));
					lAnimation.KeyFrames.Add (new LinearDoubleKeyFrame (pTo, KeyTime.FromPercent (1.0 - pRestRatio.Value)));

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
				TimeSpan lRestTime = TimeSpan.FromSeconds ((lDuration.TotalSeconds * DefaultRestRatio) - 0.3);

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

		public ExpanderContentOverlay (Expander pExpander, FrameworkElement pBaseElement, Canvas pOverlayElement)
			: base (pBaseElement, pOverlayElement)
		{
			Expander = pExpander;
			Expander.Expanded += new RoutedEventHandler (Expander_Expanded);
			Expander.Collapsed += new RoutedEventHandler (Expander_Collapsed);
		}

		public Expander Expander
		{
			get;
			protected set;
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void Expander_Expanded (object sender, RoutedEventArgs e)
		{
#if DEBUG
			System.Diagnostics.Debug.Print ("{0} Expanded", Expander.Name);
#endif
			StartOverlayAnimation (Animation);
		}

		private void Expander_Collapsed (object sender, RoutedEventArgs e)
		{
#if DEBUG
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

		public void InitializeRewind (Double? pRestRatio)
		{
			InitializeRewind (pRestRatio, false);
		}

		public void InitializeRewind (Double? pRestRatio, Boolean pRepeatRewind)
		{
			mRepeatRewind = pRepeatRewind;
			FillBehavior = FillBehavior.Stop;
			KeyFrames.Add (new LinearDoubleKeyFrame (mOrigin, KeyTime.FromPercent (RewindRatio)));

			if (pRestRatio.HasValue)
			{
				if (pRestRatio.Value > RewindRatio)
				{
					KeyFrames.Add (new DiscreteDoubleKeyFrame (mOrigin, KeyTime.FromPercent (pRestRatio.Value - RewindRatio)));
				}
				KeyFrames.Add (new LinearDoubleKeyFrame (mDestination, KeyTime.FromPercent (1.0 - Math.Max (pRestRatio.Value, RewindRatio))));
			}
			else
			{
				KeyFrames.Add (new LinearDoubleKeyFrame (mDestination, KeyTime.FromPercent (1.0 - RewindRatio)));
			}
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
