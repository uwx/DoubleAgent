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
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;

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
		public Visibility ShowDeleteKeepsBranchingTarget
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (!DeleteShiftsBranchingTarget && CanMoveFrameDown) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteMovesBranchingUp
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteMovesBranchingUp && CanMoveFrameUp && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteMovesBranchingDown
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : (DeleteMovesBranchingDown && CanMoveFrameDown && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
			}
		}
		public Visibility ShowDeleteDeletesBranching
		{
			get
			{
				return Program.FileIsReadOnly ? Visibility.Collapsed : ((!DeleteMovesBranchingUp || !CanMoveFrameUp) && (!DeleteMovesBranchingDown || !CanMoveFrameDown) && FrameIsBranchingTarget (Frame)) ? Visibility.Visible : Visibility.Hidden;
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
				NotifyPropertyChanged ("AddShiftsBranchingTarget");
				NotifyPropertyChanged ("AddKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowAddShiftsBranchingTarget");
				NotifyPropertyChanged ("ShowAddKeepsBranchingTarget");
				NotifyPropertyChanged ("SayAddShiftsBranchingTarget");
				NotifyPropertyChanged ("SayAddKeepsBranchingTarget");

				NotifyPropertyChanged ("AddBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableAddBranchingDefaultOptions");
			}
		}

		private void DeleteShiftsBranchingChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("DeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("DeleteMovesBranchingUp");
				NotifyPropertyChanged ("DeleteMovesBranchingDown");
				NotifyPropertyChanged ("ShowDeleteShiftsBranchingTarget");
				NotifyPropertyChanged ("ShowDeleteKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowDeleteMovesBranchingUp");
				NotifyPropertyChanged ("ShowDeleteMovesBranchingDown");
				NotifyPropertyChanged ("ShowDeleteDeletesBranching");

				NotifyPropertyChanged ("DeleteBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableDeleteBranchingDefaultOptions");
			}
		}

		//=============================================================================

		private void MoveUpBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MoveUpMovesBranchingSource");
				NotifyPropertyChanged ("MoveUpKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMoveUpMovesBranchingSource");
				NotifyPropertyChanged ("ShowMoveUpKeepsBranchingSource");
				NotifyPropertyChanged ("SayMoveUpMovesBranchingSource");
				NotifyPropertyChanged ("SayMoveUpKeepsBranchingSource");

				NotifyPropertyChanged ("MoveUpBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveUpBranchingDefaultOptions");
			}
		}
		private void MoveUpBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MoveUpMovesBranchingTarget");
				NotifyPropertyChanged ("MoveUpKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMoveUpMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMoveUpKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMoveUpMovesBranchingTarget");
				NotifyPropertyChanged ("SayMoveUpKeepsBranchingTarget");

				NotifyPropertyChanged ("MoveUpBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveUpBranchingDefaultOptions");
			}
		}
		private void MoveDownBranchingSourceChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MoveDownMovesBranchingSource");
				NotifyPropertyChanged ("MoveDownKeepsBranchingSource");
				NotifyPropertyChanged ("ShowMoveDownMovesBranchingSource");
				NotifyPropertyChanged ("ShowMoveDownKeepsBranchingSource");
				NotifyPropertyChanged ("SayMoveDownMovesBranchingSource");
				NotifyPropertyChanged ("SayMoveDownKeepsBranchingSource");

				NotifyPropertyChanged ("MoveDownBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveDownBranchingDefaultOptions");

				UpdateMoveDownAnimation ();
			}
		}
		private void MoveDownBranchingTargetChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("MoveDownMovesBranchingTarget");
				NotifyPropertyChanged ("MoveDownKeepsBranchingTarget");
				NotifyPropertyChanged ("ShowMoveDownMovesBranchingTarget");
				NotifyPropertyChanged ("ShowMoveDownKeepsBranchingTarget");
				NotifyPropertyChanged ("SayMoveDownMovesBranchingTarget");
				NotifyPropertyChanged ("SayMoveDownKeepsBranchingTarget");

				NotifyPropertyChanged ("MoveDownBranchingDefaultOptions");
				NotifyPropertyChanged ("EnableMoveDownBranchingDefaultOptions");
			}
		}

		//=============================================================================

		private void OnFrameStateChanged ()
		{
			if (PropertyChanged != null)
			{
				NotifyPropertyChanged ("FrameSourceTitle");
				NotifyPropertyChanged ("FrameSourceTitleNext");
				NotifyPropertyChanged ("FrameSourceTitlePrev");
				NotifyPropertyChanged ("FrameTargetTitle");
				NotifyPropertyChanged ("FrameTargetTitleNext");
				NotifyPropertyChanged ("FrameTargetTitlePrev");
				AddShiftsBranchingChanged ();
				DeleteShiftsBranchingChanged ();
				MoveUpBranchingSourceChanged ();
				MoveUpBranchingTargetChanged ();
				MoveDownBranchingSourceChanged ();
				MoveDownBranchingTargetChanged ();
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Animations

		private ExpanderContentOverlay mMoveUpAnimation = null;

		private void MuGrid_LayoutUpdated (object sender, System.EventArgs e)
		{
			if (ExpanderMoveUp.IsExpanded)
			{
				try
				{
					if (mMoveUpAnimation == null)
					{
						mMoveUpAnimation = new ExpanderContentOverlay (ExpanderMoveUp, MuGrid, MuCanvas);
					}
					if (mMoveUpAnimation.PrepareOverlay (MuFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						ParallelTimeline lAnimationGroup;

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBls1), MuBls2));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBlt1), MuBlt2));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBls2), MuBls1));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBlt2), MuBlt1));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBs1), MuBs2));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBs2), MuBs1));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBs1T1, false));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBs1T2, true));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBs2T1, false));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBs2T2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBt1), MuBt2));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeSwapAnimation (mMoveUpAnimation.MakeAnimationVisual (MuBt2), MuBt1));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBt1T1, false));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBt1T2, true));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBt2T1, false));
						lAnimationGroup.Children.Add (mMoveUpAnimation.MakeTextFadeAnimation (MuBt2T2, true));
						lStoryboard.Children.Add (lAnimationGroup);
#if DEBUG_NOT
						MdGrid.UpdateLayout ();
						MuGrid.DebugPrint ();
#endif
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

		private ExpanderContentOverlay mMoveDownAnimation = null;

		private void MdGrid_LayoutUpdated (object sender, EventArgs e)
		{
			if (ExpanderMoveDown.IsExpanded)
			{
				try
				{
					if (mMoveDownAnimation == null)
					{
						mMoveDownAnimation = new ExpanderContentOverlay (ExpanderMoveDown, MdGrid, MdCanvas);
					}
					if (mMoveDownAnimation.PrepareOverlay (MdFill))
					{
						Storyboard lStoryboard = new Storyboard ();
						ParallelTimeline lAnimationGroup;
						Rectangle lAnimationVisual;

						mMoveDownAnimation.Animation1 = new Storyboard ();
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBls1), MdBls2));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.TopProperty, 20));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Rectangle.HeightProperty, -20));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBls2), MdBls1));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.TopProperty, -20));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Rectangle.HeightProperty, 20));

						mMoveDownAnimation.Animation2 = new Storyboard ();
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBlt1), MdBlt2));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.BottomProperty, -20));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Rectangle.HeightProperty, 20));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBlt2), MdBlt1));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.BottomProperty, 20));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Rectangle.HeightProperty, -20));

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBs1), MdBs2));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.TopProperty, 20));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBs2), MdBs1));
						mMoveDownAnimation.Animation1.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.TopProperty, -20));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBs1T1, false));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBs1T2, true));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBs2T1, false));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBs2T2, true));
						lStoryboard.Children.Add (lAnimationGroup);

						lAnimationGroup = new ParallelTimeline ();
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBt1), MdBt2));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.TopProperty, 20));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeSwapAnimation (lAnimationVisual = mMoveDownAnimation.MakeAnimationVisual (MdBt2), MdBt1));
						mMoveDownAnimation.Animation2.Children.Add (mMoveDownAnimation.MakeDoubleAnimation (lAnimationVisual, Canvas.TopProperty, -20));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBt1T1, false));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBt1T2, true));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBt2T1, false));
						lAnimationGroup.Children.Add (mMoveDownAnimation.MakeTextFadeAnimation (MdBt2T2, true));
						lStoryboard.Children.Add (lAnimationGroup);
#if DEBUG_NOT
						MdGrid.UpdateLayout ();
						MdGrid.DebugPrint ();
#endif
						UpdateMoveDownAnimation ();
						mMoveDownAnimation.StartOverlayAnimation (lStoryboard);
					}
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
		}

		private void UpdateMoveDownAnimation ()
		{
			if (mMoveDownAnimation != null)
			{
				mMoveDownAnimation.Animation1Enabled = MoveDownMovesBranchingSource;
				mMoveDownAnimation.Animation2Enabled = MoveDownMovesBranchingTarget;

				if (mMoveDownAnimation.Animation != null)
				{
					try
					{
						if (mMoveDownAnimation.Animation.GetCurrentState (mMoveDownAnimation.OverlayElement) == ClockState.Active)
						{
							if (mMoveDownAnimation.Animation1 != null)
							{
								Boolean lActive = false;

								try
								{
									lActive = (mMoveDownAnimation.Animation1.GetCurrentState (mMoveDownAnimation.OverlayElement) == ClockState.Active);
								}
								catch
								{
								}

								if (mMoveDownAnimation.Animation1Enabled)
								{
									if (!lActive)
									{
										System.Diagnostics.Debug.Print ("Animation1 Start");
										//mMoveDownAnimation.Animation1.RepeatBehavior = RepeatBehavior.Forever;
										mMoveDownAnimation.Animation1.Begin (mMoveDownAnimation.OverlayElement, HandoffBehavior.Compose, true);
									}
								}
								else
								{
									if (lActive)
									{
										System.Diagnostics.Debug.Print ("Animation1 Stop");
										//mMoveDownAnimation.Animation1.SkipToFill (mMoveDownAnimation.OverlayElement);
										//mMoveDownAnimation.Animation1.Stop (mMoveDownAnimation.OverlayElement);
									}
								}
							}
							//mMoveDownAnimation.StartOverlayAnimation (mMoveDownAnimation.Animation);
						}
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}
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
		public Duration AnimationDuration
		{
			get
			{
				return new Duration (TimeSpan.FromSeconds (5));
			}
		}

		public Storyboard Animation1
		{
			get;
			set;
		}
		public Storyboard Animation2
		{
			get;
			set;
		}
		public Boolean Animation1Enabled
		{
			get;
			set;
		}
		public Boolean Animation2Enabled
		{
			get;
			set;
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

		public Boolean StartOverlayAnimation (Storyboard pAnimation)
		{
			if (pAnimation != null)
			{
				try
				{
					Storyboard lPrevAnimation = Animation;

					pAnimation.AccelerationRatio = 0.25;
					pAnimation.DecelerationRatio = 0.25;
					pAnimation.RepeatBehavior = RepeatBehavior.Forever;
					pAnimation.Name = String.Format ("{0}_A", BaseElement.Name);
#if DEBUG
					System.Diagnostics.Debug.Print ("Start Animation {0}", pAnimation.Name);
#endif
					pAnimation.Begin (OverlayElement, HandoffBehavior.SnapshotAndReplace, true);
					Animation = pAnimation;

					if (Animation1 != null)
					{
						Animation1.AccelerationRatio = 0.25;
						Animation1.DecelerationRatio = 0.25;
						Animation1.RepeatBehavior = RepeatBehavior.Forever;
						if (Animation1Enabled)
						{
#if DEBUG
							System.Diagnostics.Debug.Print ("Start Animation1 {0}", pAnimation.Name);
#endif
							Animation1.Begin (OverlayElement, HandoffBehavior.Compose, true);
						}
					}
					if (Animation2 != null)
					{
						Animation2.AccelerationRatio = 0.25;
						Animation2.DecelerationRatio = 0.25;
						Animation2.RepeatBehavior = RepeatBehavior.Forever;
						if (Animation2Enabled)
						{
#if DEBUG
							System.Diagnostics.Debug.Print ("Start Animation2 {0}", pAnimation.Name);
#endif
							Animation2.Begin (OverlayElement, HandoffBehavior.Compose, true);
						}
					}
					if ((lPrevAnimation != null) && (lPrevAnimation != Animation))
					{
						try
						{
							lPrevAnimation.Stop (OverlayElement);
						}
						catch (Exception pException)
						{
							System.Diagnostics.Debug.Print (pException.Message);
						}
					}

					return true;
				}
				catch (Exception pException)
				{
					System.Diagnostics.Debug.Print (pException.Message);
				}
			}
			return false;
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
				lBackgroundElement.LayoutUpdated += new System.EventHandler (lBackgroundElement_LayoutUpdated);
			}
			return lBackgroundElement;
		}

		void lBackgroundElement_LayoutUpdated (object sender, System.EventArgs e)
		{
			//System.Diagnostics.Debug.Print ("lBackgroundElement_LayoutUpdated [{0}]", sender);
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

				//lBackgroundElement.LayoutUpdated += delegate (object sender, EventArgs e)
				//{
				//};
				PutAnimationElement (pSourceElement, lAnimationElement);

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
			try
			{
				DoubleAnimation lAnimation = new DoubleAnimation ();
				Point lOrigin = pTargetElement.TranslatePoint (new Point (0, 0), OverlayElement);

				lAnimation.From = Canvas.GetLeft (pAnimationVisual);
				lAnimation.To = lOrigin.X;
				lAnimation.By = 0.1;
				lAnimation.Duration = AnimationDuration;

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

		public AnimationTimeline MakeDoubleAnimation (Rectangle pAnimationVisual, DependencyProperty pProperty, Double pMidPoint)
		{
			try
			{
				DoubleAnimationUsingKeyFrames lAnimation = new DoubleAnimationUsingKeyFrames ();
				Double lValue = (Double)pAnimationVisual.GetValue (pProperty);

				lAnimation.KeyFrames.Add (new DiscreteDoubleKeyFrame (lValue, KeyTime.FromPercent (0)));
				lAnimation.KeyFrames.Add (new SplineDoubleKeyFrame (lValue + pMidPoint, KeyTime.FromPercent (0.5), new KeySpline (0.0, 0.8, 0.2, 1.0)));
				lAnimation.KeyFrames.Add (new SplineDoubleKeyFrame (lValue, KeyTime.FromPercent (1.0), new KeySpline (0.8, 0.0, 1.0, 0.2)));
				lAnimation.Duration = AnimationDuration;

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

		public AnimationTimeline MakeTextFadeAnimation (TextBlock pText, Boolean pFadeIn)
		{
			if (pFadeIn)
			{
				return MakeTextFadeAnimation (pText, 0.0, 1.0);
			}
			else
			{
				return MakeTextFadeAnimation (pText, 1.0, 0.0);
			}
		}
		public AnimationTimeline MakeTextFadeAnimation (TextBlock pText, Double pFrom, Double pTo)
		{
			try
			{
				DoubleAnimation lAnimation = new DoubleAnimation ();

				lAnimation.From = pFrom;
				lAnimation.To = pTo;
				lAnimation.By = 1;
				lAnimation.Duration = AnimationDuration;

				Storyboard.SetTarget (lAnimation, pText);
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

		private Dictionary<FrameworkElement, Rectangle> mAnimationElements = new Dictionary<FrameworkElement, Rectangle> ();

		private void PutAnimationElement (FrameworkElement pSourceElement, Rectangle pAnimationElement)
		{
			if ((pSourceElement != null) && (pAnimationElement != null))
			{
				if (!OverlayElement.Children.Contains (pAnimationElement))
				{
					OverlayElement.Children.Add (pAnimationElement);
				}
				if (!mAnimationElements.ContainsKey (pSourceElement))
				{
					mAnimationElements[pSourceElement] = pAnimationElement;
				}
			}
		}

		private Rectangle GetAnimationElement (FrameworkElement pSourceElement)
		{
			Rectangle lAnimationElement = null;
			mAnimationElements.TryGetValue (pSourceElement, out lAnimationElement);
			return lAnimationElement;
		}

		#endregion
	}

	/////////////////////////////////////////////////////////////////////////////

	internal class ExpanderContentOverlay : AnimationOverlay
	{
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

		private void Expander_Expanded (object sender, RoutedEventArgs e)
		{
#if DEBUG
			System.Diagnostics.Debug.Print ("{0} Expanded", Expander.Name);
#endif
			try
			{
				if (Animation != null)
				{
					StartOverlayAnimation (Animation);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		private void Expander_Collapsed (object sender, RoutedEventArgs e)
		{
#if DEBUG
			System.Diagnostics.Debug.Print ("{0} Collapsed", Expander.Name);
#endif
			try
			{
				if (Animation != null)
				{
					Animation.Pause (OverlayElement);
				}
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}
	}
}
