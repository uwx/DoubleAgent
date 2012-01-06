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
using System.ComponentModel;
using System.Threading;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Properties;
using DoubleAgent;
using DoubleAgent.Character;
using AppResources = AgentCharacterEditor.Resources;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesToolBar
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FramesPreview FramesPreview
		{
			get;
			set;
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public FileAnimationFrame Frame
		{
			get
			{
				return mFrame;
			}
			set
			{
				if (mFrame != value)
				{
					mFrame = value;

					if (!IsEmpty && (SelectedFrameState == null))
					{
						SelectedFrameState = new FrameStateWorker ();
						SelectedFrameState.ProgressChanged += new ProgressChangedEventHandler (FrameStateWorker_ProgressChanged);
					}
					if (SelectedFrameState != null)
					{
						SelectedFrameState.Frame = Frame;
					}

					RefreshState ();
				}
			}
		}

		private FileAnimationFrame mFrame = null;

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsValid
		{
			get
			{
				return (FramesPreview != null);
			}
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean IsEmpty
		{
			get
			{
				return (FramesPreview == null) || (Frame == null);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean CanAddFrame
		{
			get
			{
				return IsValid && !Program.FileIsReadOnly;
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean CanDeleteFrame
		{
			get
			{
				return IsValid && !Program.FileIsReadOnly && !IsEmpty;
			}
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public String DeleteFrameTitle
		{
			get
			{
				return String.Format (AppResources.Resources.EditDeleteThis.NoMenuPrefix (), Titles.Frame (CanDeleteFrame ? Frame : null));
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean CanMoveFramePrev
		{
			get
			{
				if (IsValid)
				{
					int lSelectedIndex = FramesPreview.Frames.SelectedIndex;
					return !Program.FileIsReadOnly && (lSelectedIndex > 0) && (lSelectedIndex < FramesPreview.Frames.Items.Count);
				}
				return false;
			}
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public String MoveFramePrevTitle
		{
			get
			{
				return String.Format (AppResources.Resources.EditMoveFramePrev.NoMenuPrefix (), Titles.Frame (CanMoveFramePrev ? Frame : null));
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean CanMoveFrameNext
		{
			get
			{
				if (IsValid)
				{
					int lSelectedIndex = FramesPreview.Frames.SelectedIndex;
					return !Program.FileIsReadOnly && (lSelectedIndex >= 0) && (lSelectedIndex < FramesPreview.Frames.Items.Count - 1);
				}
				return false;
			}
		}
		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public String MoveFrameNextTitle
		{
			get
			{
				return String.Format (AppResources.Resources.EditMoveFrameNext.NoMenuPrefix (), Titles.Frame (CanMoveFrameNext ? Frame : null));
			}
		}

		[System.ComponentModel.Browsable (false)]
		[System.ComponentModel.EditorBrowsable (System.ComponentModel.EditorBrowsableState.Never)]
		[System.ComponentModel.DesignerSerializationVisibility (System.ComponentModel.DesignerSerializationVisibility.Hidden)]
		public Boolean CanMoveNextFrameNext
		{
			get
			{
				if (IsValid)
				{
					int lSelectedIndex = FramesPreview.Frames.SelectedIndex;
					return !Program.FileIsReadOnly && (lSelectedIndex >= 0) && (lSelectedIndex < FramesPreview.Frames.Items.Count - 2);
				}
				return false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean mAddShiftsBranchingTarget = true;
		private Boolean mDeleteShiftsBranchingTarget = true;
		private Boolean mDeleteMovesBranchingPrev = false;
		private Boolean mDeleteMovesBranchingNext = false;

		//=============================================================================

		public Boolean AddShiftsBranchingTarget
		{
			get
			{
				return mAddShiftsBranchingTarget;
			}
			set
			{
				if (mAddShiftsBranchingTarget != value)
				{
					mAddShiftsBranchingTarget = value;
					AddShiftsBranchingChanged ();
				}
			}
		}
		public Boolean AddKeepsBranchingTarget
		{
			get
			{
				return !AddShiftsBranchingTarget;
			}
			set
			{
				AddShiftsBranchingTarget = !value;
			}
		}

		public Boolean DeleteShiftsBranchingTarget
		{
			get
			{
				return mDeleteShiftsBranchingTarget;
			}
			set
			{
				if (mDeleteShiftsBranchingTarget != value)
				{
					mDeleteShiftsBranchingTarget = value;
					DeleteBranchingTargetChanged ();
				}
			}
		}
		public Boolean DeleteKeepsBranchingTarget
		{
			get
			{
				return !DeleteShiftsBranchingTarget;
			}
			set
			{
				DeleteShiftsBranchingTarget = !value;
			}
		}

		public Boolean DeleteMovesBranchingPrev
		{
			get
			{
				if (IsValid && CanMoveFramePrev && !CanMoveFrameNext && mDeleteMovesBranchingNext)
				{
					return mDeleteMovesBranchingNext;
				}
				else
				{
					return mDeleteMovesBranchingPrev;
				}
			}
			set
			{
				if (IsValid && !CanMoveFramePrev)
				{
					System.Diagnostics.Debug.Print ("Ignore DeleteMovesBranchingPrev {0}", value);
				}
				else if (mDeleteMovesBranchingPrev != value)
				{
					mDeleteMovesBranchingPrev = value;
					if (mDeleteMovesBranchingPrev)
					{
						mDeleteMovesBranchingNext = false;
					}
					DeleteBranchingSourceChanged ();
				}
			}
		}
		public Boolean DeleteMovesBranchingNext
		{
			get
			{
				if (IsValid && CanMoveFrameNext && !CanMoveFramePrev && mDeleteMovesBranchingPrev)
				{
					return mDeleteMovesBranchingPrev;
				}
				else
				{
					return mDeleteMovesBranchingNext;
				}
			}
			set
			{
				if (IsValid && !CanMoveFrameNext)
				{
					System.Diagnostics.Debug.Print ("Ignore DeleteMovesBranchingNext {0}", value);
				}
				else if (mDeleteMovesBranchingNext != value)
				{
					mDeleteMovesBranchingNext = value;
					if (mDeleteMovesBranchingNext)
					{
						mDeleteMovesBranchingPrev = false;
					}
					DeleteBranchingSourceChanged ();
				}
			}
		}
		public Boolean DeleteDeletesBranching
		{
			get
			{
				return (!DeleteMovesBranchingPrev && !DeleteMovesBranchingNext) || (!CanMoveFramePrev && !CanMoveFrameNext);
			}
			set
			{
				if (value && (mDeleteMovesBranchingPrev || mDeleteMovesBranchingNext))
				{
					mDeleteMovesBranchingPrev = false;
					mDeleteMovesBranchingNext = false;
					DeleteBranchingSourceChanged ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private Boolean mMovePrevMovesBranchingSource = false;
		private Boolean mMovePrevMovesBranchingTarget = true;
		private Boolean mMoveNextMovesBranchingSource = false;
		private Boolean mMoveNextMovesBranchingTarget = true;

		//=============================================================================

		public Boolean MovePrevMovesBranchingSource
		{
			get
			{
				return mMovePrevMovesBranchingSource;
			}
			set
			{
				if (mMovePrevMovesBranchingSource != value)
				{
					mMovePrevMovesBranchingSource = value;
					MovePrevBranchingSourceChanged ();
				}
			}
		}
		public Boolean MovePrevKeepsBranchingSource
		{
			get
			{
				return !MovePrevMovesBranchingSource;
			}
			set
			{
				MovePrevMovesBranchingSource = !value;
			}
		}
		public Boolean MovePrevMovesBranchingTarget
		{
			get
			{
				return mMovePrevMovesBranchingTarget;
			}
			set
			{
				if (mMovePrevMovesBranchingTarget != value)
				{
					mMovePrevMovesBranchingTarget = value;
					MovePrevBranchingTargetChanged ();
				}
			}
		}
		public Boolean MovePrevKeepsBranchingTarget
		{
			get
			{
				return !MovePrevMovesBranchingTarget;
			}
			set
			{
				MovePrevMovesBranchingTarget = !value;
			}
		}

		//=============================================================================

		public Boolean MoveNextMovesBranchingSource
		{
			get
			{
				return mMoveNextMovesBranchingSource;
			}
			set
			{
				if (mMoveNextMovesBranchingSource != value)
				{
					mMoveNextMovesBranchingSource = value;
					MoveNextBranchingSourceChanged ();
				}
			}
		}
		public Boolean MoveNextKeepsBranchingSource
		{
			get
			{
				return !MoveNextMovesBranchingSource;
			}
			set
			{
				MoveNextMovesBranchingSource = !value;
			}
		}
		public Boolean MoveNextMovesBranchingTarget
		{
			get
			{
				return mMoveNextMovesBranchingTarget;
			}
			set
			{
				if (mMoveNextMovesBranchingTarget != value)
				{
					mMoveNextMovesBranchingTarget = value;
					MoveNextBranchingTargetChanged ();
				}
			}
		}
		public Boolean MoveNextKeepsBranchingTarget
		{
			get
			{
				return !MoveNextMovesBranchingTarget;
			}
			set
			{
				MoveNextMovesBranchingTarget = !value;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		public Boolean AddBranchingDefaultOptions
		{
			get
			{
				return AddShiftsBranchingTarget;
			}
			set
			{
				if (value)
				{
					AddShiftsBranchingTarget = true;
				}
			}
		}
		public Boolean DeleteBranchingDefaultOptions
		{
			get
			{
				return DeleteShiftsBranchingTarget && DeleteDeletesBranching;
			}
			set
			{
				if (value)
				{
					DeleteShiftsBranchingTarget = true;
					DeleteDeletesBranching = true;
				}
			}
		}
		public Boolean MovePrevBranchingDefaultOptions
		{
			get
			{
				return !MovePrevMovesBranchingSource && MovePrevMovesBranchingTarget;
			}
			set
			{
				if (value)
				{
					MovePrevMovesBranchingSource = false;
					MovePrevMovesBranchingTarget = true;
				}
			}
		}
		public Boolean MoveNextBranchingDefaultOptions
		{
			get
			{
				return !MoveNextMovesBranchingSource && MoveNextMovesBranchingTarget;
			}
			set
			{
				if (value)
				{
					MoveNextMovesBranchingSource = false;
					MoveNextMovesBranchingTarget = true;
				}
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

		///////////////////////////////////////////////////////////////////////////////

		public String SayAddShiftsBranchingTarget
		{
			get
			{
				return AppResources.Resources.FrameAddShiftsBranchingTarget;
			}
		}
		public String SayAddKeepsBranchingTarget
		{
			get
			{
				return AppResources.Resources.FrameAddKeepsBranchingTarget;
			}
		}

		//=============================================================================

		public String SayDeleteShiftsBranchingTarget
		{
			get
			{
				return AppResources.Resources.FrameDeleteShiftsBranchingTarget;
			}
		}
		public String SayDeleteKeepsBranchingTarget
		{
			get
			{
				return AppResources.Resources.FrameDeleteKeepsBranchingTarget;
			}
		}

		public String SayDeleteMovesBranchingPrev
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameDeleteMovesBranchingPrev0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameDeleteMovesBranchingPrev, Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}
		public String SayDeleteMovesBranchingNext
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameDeleteMovesBranchingNext0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameDeleteMovesBranchingNext, Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex + 2) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}
		public String SayDeleteDeletesBranching
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameDeleteDeletesBranching0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameDeleteDeletesBranching, Titles.Frame (Frame)) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}

		//=============================================================================

		public String SayMovePrevMovesBranchingSource
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveMovesBranchingSource0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameMoveMovesBranchingSource, Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}
		public String SayMovePrevKeepsBranchingSource
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveKeepsBranchingSource0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameMoveKeepsBranchingSource, Titles.Frame (Frame)) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}
		public String SayMovePrevMovesBranchingTarget
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveMovesBranchingTarget0 : FrameIsBranchingTarget (Frame) ? String.Format (AppResources.Resources.FrameMoveMovesBranchingTarget, FramesPreview.Frames.SelectedIndex + 1, FramesPreview.Frames.SelectedIndex) : String.Format (AppResources.Resources.FrameMoveBranchingTargetNone, Titles.Frame (Frame));
			}
		}
		public String SayMovePrevKeepsBranchingTarget
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveKeepsBranchingTarget0 : FrameIsBranchingTarget (Frame) ? String.Format (AppResources.Resources.FrameMoveKeepsBranchingTarget, Titles.Frame (Frame)) : String.Format (AppResources.Resources.FrameMoveBranchingTargetNone, Titles.Frame (Frame));
			}
		}

		//=============================================================================

		public String SayMoveNextMovesBranchingSource
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveMovesBranchingSource0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameMoveMovesBranchingSource, Titles.Frame (Frame), FramesPreview.Frames.SelectedIndex + 2) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}
		public String SayMoveNextKeepsBranchingSource
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveKeepsBranchingSource0 : FrameIsBranchingSource (Frame) ? String.Format (AppResources.Resources.FrameMoveKeepsBranchingSource, Titles.Frame (Frame)) : String.Format (AppResources.Resources.FrameMoveBranchingSourceNone, Titles.Frame (Frame));
			}
		}
		public String SayMoveNextMovesBranchingTarget
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveMovesBranchingTarget0 : FrameIsBranchingTarget (Frame) ? String.Format (AppResources.Resources.FrameMoveMovesBranchingTarget, FramesPreview.Frames.SelectedIndex + 1, FramesPreview.Frames.SelectedIndex + 2) : String.Format (AppResources.Resources.FrameMoveBranchingTargetNone, Titles.Frame (Frame));
			}
		}
		public String SayMoveNextKeepsBranchingTarget
		{
			get
			{
				return (Frame == null) ? AppResources.Resources.FrameMoveKeepsBranchingTarget0 : FrameIsBranchingTarget (Frame) ? String.Format (AppResources.Resources.FrameMoveKeepsBranchingTarget, Titles.Frame (Frame)) : String.Format (AppResources.Resources.FrameMoveBranchingTargetNone, Titles.Frame (Frame));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Methods

		private Boolean FrameIsBranchingSource (FileAnimationFrame pFrame)
		{
			if (pFrame == null)
			{
				return false;
			}
			else if ((SelectedFrameState != null) && (SelectedFrameState.Frame == pFrame) && (!SelectedFrameState.IsWorking))
			{
				return SelectedFrameState.FrameIsBranchingSource;
			}
			else
			{
				SelectedFrameState.Frame = pFrame;
				return true;
			}
		}

		private Boolean FrameIsBranchingTarget (FileAnimationFrame pFrame)
		{
			if (pFrame == null)
			{
				return false;
			}
			else if ((SelectedFrameState != null) && (SelectedFrameState.Frame == pFrame) && (!SelectedFrameState.IsWorking))
			{
				return SelectedFrameState.FrameIsBranchingTarget;
			}
			else
			{
				SelectedFrameState.Frame = pFrame;
				return true;
			}
		}

		//=============================================================================

		public void RefreshState ()
		{
			ButtonAdd.IsEnabled = CanAddFrame;
			ButtonDelete.IsEnabled = CanDeleteFrame;
			ButtonMovePrev.IsEnabled = CanMoveFramePrev;
			ButtonMoveNext.IsEnabled = CanMoveFrameNext;

			ButtonDelete.SetTipText (DeleteFrameTitle);
			ButtonMovePrev.SetTipText (MoveFramePrevTitle);
			ButtonMoveNext.SetTipText (MoveFrameNextTitle);

			OnFrameStateChanged ();
			if (SelectedFrameState != null)
			{
				SelectedFrameState.Refresh (Frame);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Background FrameState

		private FrameStateWorker SelectedFrameState
		{
			get;
			set;
		}

		void FrameStateWorker_ProgressChanged (object sender, ProgressChangedEventArgs e)
		{
#if DEBUG_NOT
			System.Diagnostics.Debug.Print ("FrameStateWorker_ProgressChanged Frame [{0}] FrameIsBranchingSource [{1}] FrameIsBranchingTarget [{2}]", SelectedFrameState.Frame, SelectedFrameState.FrameIsBranchingSource, SelectedFrameState.FrameIsBranchingTarget);
#endif
			OnFrameStateChanged ();
		}

		#endregion
	}

	///////////////////////////////////////////////////////////////////////////////

	internal class FrameStateWorker : BackgroundWorkerEx
	{
		private FileAnimationFrame mFrame = null;
		private Boolean mFrameIsBranchingSource = false;
		private Boolean mFrameIsBranchingTarget = false;
		private Object mLock = new Object ();

		public FrameStateWorker ()
		{
		}

		public FileAnimationFrame Frame
		{
			get
			{
				FileAnimationFrame lFrame = null;
				lock (mLock)
				{
					lFrame = mFrame;
				}
				return lFrame;
			}
			set
			{
				Boolean lFrameChanged = false;
				lock (mLock)
				{
					if (mFrame != value)
					{
						mFrame = value;
						lFrameChanged = true;
					}
				}
				if (lFrameChanged)
				{
					Refresh ();
				}
			}
		}

		public Boolean FrameIsBranchingSource
		{
			get
			{
				Boolean lRet = false;
				lock (mLock)
				{
					lRet = mFrameIsBranchingSource;
				}
				return lRet;
			}
			private set
			{
				lock (mLock)
				{
					mFrameIsBranchingSource = value;
				}
			}
		}

		public Boolean FrameIsBranchingTarget
		{
			get
			{
				Boolean lRet = false;
				lock (mLock)
				{
					lRet = mFrameIsBranchingTarget;
				}
				return lRet;
			}
			private set
			{
				lock (mLock)
				{
					mFrameIsBranchingTarget = value;
				}
			}
		}

		//=============================================================================

		public void Refresh (FileAnimationFrame pFrame)
		{
			if (Frame == pFrame)
			{
				Refresh ();
			}
			else
			{
				Frame = pFrame;
			}
		}

		public void Refresh ()
		{
			if (Frame == null)
			{
				StopWork ();
			}
			else
			{
				StartWork ();
			}
		}

		protected override Boolean StartWork ()
		{
			FrameIsBranchingSource = false;
			FrameIsBranchingTarget = false;
			IsWorking = true;
			return base.StartWork ();
		}

		protected override Boolean StopWork ()
		{
			Boolean lRet = base.StopWork ();
			IsWorking = false;
			FrameIsBranchingSource = false;
			FrameIsBranchingTarget = false;
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void OnDoWork (DoWorkEventArgs e)
		{
			UInt32 lWorkCycle;

			IsWorking = true;

			do
			{
				lWorkCycle = StartWorkCycle ();

				if ((lWorkCycle > 0) && IsWorking && !CancelWorkCycle (e))
				{
					try
					{
						FileAnimation lAnimation = null;
						FileAnimationFrame lTargetFrame = Frame;
						int lTargetNdx = -1;
						int lBranchingNdx;

						FrameIsBranchingSource = false;
						FrameIsBranchingTarget = false;

						if (CancelWorkCycle (e))
						{
							break;
						}
						else if (HasNewWork (lWorkCycle))
						{
							continue;
						}
						else if (lTargetFrame != null)
						{
							if (lTargetFrame.ExitFrame >= 0)
							{
								FrameIsBranchingSource = true;
							}
							else if (lTargetFrame.Branching != null)
							{
								for (lBranchingNdx = 0; lBranchingNdx < lTargetFrame.Branching.Length; lBranchingNdx++)
								{
									if (lTargetFrame.Branching[lBranchingNdx].mProbability > 0)
									{
										FrameIsBranchingSource = true;
										break;
									}
								}
							}
						}

						if (CancelWorkCycle (e))
						{
							break;
						}
						else if (HasNewWork (lWorkCycle))
						{
							continue;
						}
						else if ((lTargetFrame != null) && ((lAnimation = lTargetFrame.Animation) != null) && (lAnimation.Frames.Contains (lTargetFrame)))
						{
							lTargetNdx = lAnimation.Frames.IndexOf (lTargetFrame);
						}

						if (CancelWorkCycle (e))
						{
							break;
						}
						else if (HasNewWork (lWorkCycle))
						{
							continue;
						}
						else if (lTargetNdx >= 0)
						{
							foreach (FileAnimationFrame lSourceFrame in lAnimation.Frames)
							{
								if (CancelWorkCycle (e))
								{
									break;
								}
								else if (HasNewWork (lWorkCycle))
								{
									break;
								}
								else if (lSourceFrame != lTargetFrame)
								{
									if (lSourceFrame.ExitFrame == lTargetNdx)
									{
										FrameIsBranchingTarget = true;
										break;
									}
									else if (lSourceFrame.Branching != null)
									{
										for (lBranchingNdx = 0; lBranchingNdx < lSourceFrame.Branching.Length; lBranchingNdx++)
										{
											if ((lSourceFrame.Branching[lBranchingNdx].mProbability > 0) && (lSourceFrame.Branching[lBranchingNdx].mFrameNdx == lTargetNdx))
											{
												FrameIsBranchingTarget = true;
												break;
											}
										}
									}
									if (FrameIsBranchingTarget)
									{
										break;
									}
								}
							}
						}
					}
					catch (Exception pException)
					{
						System.Diagnostics.Debug.Print (pException.Message);
					}

					IsWorking = false;

					if (CancelWorkCycle (e))
					{
						break;
					}
					else if (HasNewWork (lWorkCycle))
					{
						continue;
					}
					else
					{
						try
						{
							ReportProgress (100);
						}
						catch (Exception pException)
						{
							System.Diagnostics.Debug.Print (pException.Message);
						}
					}
				}
			}
			while (WaitForWork (lWorkCycle, e));
		}
	}
}
