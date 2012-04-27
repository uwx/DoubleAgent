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
using System.Windows.Forms;
using DoubleAgent;
using AgentCharacterEditor.Properties;

namespace AgentCharacterEditor.Previews
{
	public partial class FramesToolBar : DoubleAgent.ToolStripEx
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public FramesToolBar ()
		{
			InitializeComponent ();

			ButtonAdd.DropDown.ShowItemToolTips = true;
			ButtonAdd.DropDown.RenderMode = ToolStripRenderMode.Professional;
			if (ButtonAdd.DropDown is ToolStripDropDownMenu)
			{
				(ButtonAdd.DropDown as ToolStripDropDownMenu).ShowImageMargin = true;
				(ButtonAdd.DropDown as ToolStripDropDownMenu).ShowCheckMargin = true;
			}

			ButtonDelete.DropDown.ShowItemToolTips = true;
			ButtonDelete.DropDown.RenderMode = ToolStripRenderMode.Professional;
			if (ButtonDelete.DropDown is ToolStripDropDownMenu)
			{
				(ButtonDelete.DropDown as ToolStripDropDownMenu).ShowImageMargin = true;
				(ButtonDelete.DropDown as ToolStripDropDownMenu).ShowCheckMargin = true;
			}

			ButtonMoveNext.DropDown.ShowItemToolTips = true;
			ButtonMoveNext.DropDown.RenderMode = ToolStripRenderMode.Professional;
			if (ButtonMoveNext.DropDown is ToolStripDropDownMenu)
			{
				(ButtonMoveNext.DropDown as ToolStripDropDownMenu).ShowImageMargin = true;
				(ButtonMoveNext.DropDown as ToolStripDropDownMenu).ShowCheckMargin = true;
			}

			ButtonMovePrev.DropDown.ShowItemToolTips = true;
			ButtonMovePrev.DropDown.RenderMode = ToolStripRenderMode.Professional;
			if (ButtonMovePrev.DropDown is ToolStripDropDownMenu)
			{
				(ButtonMovePrev.DropDown as ToolStripDropDownMenu).ShowImageMargin = true;
				(ButtonMovePrev.DropDown as ToolStripDropDownMenu).ShowCheckMargin = true;
			}
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
		#region Implementation

		private void AddShiftsBranchingChanged ()
		{
		}
		private void DeleteBranchingSourceChanged ()
		{
		}
		private void DeleteBranchingTargetChanged ()
		{
		}
		private void MovePrevBranchingSourceChanged ()
		{
		}
		private void MovePrevBranchingTargetChanged ()
		{
		}
		private void MoveNextBranchingSourceChanged ()
		{
		}
		private void MoveNextBranchingTargetChanged ()
		{
		}

		private void OnFrameStateChanged ()
		{
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		void ButtonAdd_DropDownOpening (object sender, System.EventArgs e)
		{
			AddShiftBranchingTarget.Enabled = !Program.FileIsReadOnly && (Frame != null);
			AddKeepBranchingTarget.Enabled = !Program.FileIsReadOnly && (Frame != null);
			AddShiftBranchingTarget.Checked = AddShiftsBranchingTarget;
			AddKeepBranchingTarget.Checked = AddKeepsBranchingTarget;
			AddShiftBranchingTarget.ToolTipText = SayAddShiftsBranchingTarget;
			AddKeepBranchingTarget.ToolTipText = SayAddKeepsBranchingTarget;

			AddUseDefaults.Enabled = EnableAddBranchingDefaultOptions;
			AddUseDefaults.Checked = AddBranchingDefaultOptions;
		}

		void AddShiftBranchingTarget_Click (object sender, System.EventArgs e)
		{
			AddShiftsBranchingTarget = true;
		}
		void AddKeepBranchingTarget_Click (object sender, System.EventArgs e)
		{
			AddKeepsBranchingTarget = true;
		}
		void AddUseDefaults_Click (object sender, System.EventArgs e)
		{
			AddBranchingDefaultOptions = true;
		}

		//=============================================================================

		void ButtonDelete_DropDownOpening (object sender, System.EventArgs e)
		{
			DeleteMoveBranchingPrev.Enabled = !Program.FileIsReadOnly && CanMoveFramePrev;
			DeleteMoveBranchingNext.Enabled = !Program.FileIsReadOnly && CanMoveFrameNext;
			DeleteDeleteBranching.Enabled = !Program.FileIsReadOnly && (CanMoveFrameNext || CanMoveFramePrev);
			DeleteShiftBranchingTarget.Enabled = !Program.FileIsReadOnly && (CanMoveFrameNext || CanMoveFramePrev);
			DeleteKeepBranchingTarget.Enabled = !Program.FileIsReadOnly && (CanMoveFrameNext || CanMoveFramePrev);
			DeleteMoveBranchingPrev.Checked = DeleteMovesBranchingPrev && !DeleteDeletesBranching && (CanMoveFramePrev || !CanMoveFrameNext);
			DeleteMoveBranchingNext.Checked = DeleteMovesBranchingNext && !DeleteDeletesBranching && (CanMoveFrameNext || !CanMoveFramePrev);
			DeleteDeleteBranching.Checked = DeleteDeletesBranching;
			DeleteShiftBranchingTarget.Checked = DeleteShiftsBranchingTarget;
			DeleteKeepBranchingTarget.Checked = DeleteKeepsBranchingTarget;
			DeleteMoveBranchingPrev.ToolTipText = SayDeleteMovesBranchingPrev;
			DeleteMoveBranchingNext.ToolTipText = SayDeleteMovesBranchingNext;
			DeleteDeleteBranching.ToolTipText = SayDeleteDeletesBranching;
			DeleteShiftBranchingTarget.ToolTipText = SayDeleteShiftsBranchingTarget;
			DeleteKeepBranchingTarget.ToolTipText = SayDeleteKeepsBranchingTarget;

			DeleteUseDefaults.Enabled = EnableDeleteBranchingDefaultOptions;
			DeleteUseDefaults.Checked = DeleteBranchingDefaultOptions;
		}

		void DeleteMoveBranchingPrev_Click (object sender, System.EventArgs e)
		{
			DeleteMovesBranchingPrev = true;
		}
		void DeleteMoveBranchingNext_Click (object sender, System.EventArgs e)
		{
			DeleteMovesBranchingNext = true;
		}
		void DeleteDeleteBranching_Click (object sender, System.EventArgs e)
		{
			DeleteDeletesBranching = true;
		}

		void DeleteShiftBranchingTarget_Click (object sender, System.EventArgs e)
		{
			DeleteShiftsBranchingTarget = true;
		}
		void DeleteKeepBranchingTarget_Click (object sender, System.EventArgs e)
		{
			DeleteKeepsBranchingTarget = true;
		}

		void DeleteUseDefaults_Click (object sender, System.EventArgs e)
		{
			DeleteBranchingDefaultOptions = true;
		}

		//=============================================================================

		void ButtonMoveNext_DropDownOpening (object sender, System.EventArgs e)
		{
			MoveNextMoveBranchingSource.Enabled = !Program.FileIsReadOnly && CanMoveFrameNext;
			MoveNextKeepBranchingSource.Enabled = !Program.FileIsReadOnly && CanMoveFrameNext;
			MoveNextMoveBranchingTarget.Enabled = !Program.FileIsReadOnly && CanMoveFrameNext;
			MoveNextKeepBranchingTarget.Enabled = !Program.FileIsReadOnly && CanMoveFrameNext;
			MoveNextMoveBranchingSource.Checked = MoveNextMovesBranchingSource;
			MoveNextKeepBranchingSource.Checked = MoveNextKeepsBranchingSource;
			MoveNextMoveBranchingTarget.Checked = MoveNextMovesBranchingTarget;
			MoveNextKeepBranchingTarget.Checked = MoveNextKeepsBranchingTarget;
			MoveNextMoveBranchingSource.ToolTipText = SayMoveNextMovesBranchingSource;
			MoveNextKeepBranchingSource.ToolTipText = SayMoveNextKeepsBranchingSource;
			MoveNextMoveBranchingTarget.ToolTipText = SayMoveNextMovesBranchingTarget;
			MoveNextKeepBranchingTarget.ToolTipText = SayMoveNextKeepsBranchingTarget;

			MoveNextUseDefaults.Enabled = EnableMoveNextBranchingDefaultOptions;
			MoveNextUseDefaults.Checked = MoveNextBranchingDefaultOptions;
		}

		void MoveNextMoveBranchingSource_Click (object sender, System.EventArgs e)
		{
			MoveNextMovesBranchingSource = true;
		}
		void MoveNextKeepBranchingSource_Click (object sender, System.EventArgs e)
		{
			MoveNextKeepsBranchingSource = true;
		}
		void MoveNextMoveBranchingTarget_Click (object sender, System.EventArgs e)
		{
			MoveNextMovesBranchingTarget = true;
		}
		void MoveNextKeepBranchingTarget_Click (object sender, System.EventArgs e)
		{
			MoveNextKeepsBranchingTarget = true;
		}
		void MoveNextUseDefaults_Click (object sender, System.EventArgs e)
		{
			MoveNextBranchingDefaultOptions = true;
		}

		//=============================================================================

		void ButtonMovePrev_DropDownOpening (object sender, System.EventArgs e)
		{
			MovePrevMoveBranchingSource.Enabled = !Program.FileIsReadOnly && CanMoveFramePrev;
			MovePrevKeepBranchingSource.Enabled = !Program.FileIsReadOnly && CanMoveFramePrev;
			MovePrevMoveBranchingTarget.Enabled = !Program.FileIsReadOnly && CanMoveFramePrev;
			MovePrevKeepBranchingTarget.Enabled = !Program.FileIsReadOnly && CanMoveFramePrev;
			MovePrevMoveBranchingSource.Checked = MovePrevMovesBranchingSource;
			MovePrevKeepBranchingSource.Checked = MovePrevKeepsBranchingSource;
			MovePrevMoveBranchingTarget.Checked = MovePrevMovesBranchingTarget;
			MovePrevKeepBranchingTarget.Checked = MovePrevKeepsBranchingTarget;
			MovePrevMoveBranchingSource.ToolTipText = SayMovePrevMovesBranchingSource;
			MovePrevKeepBranchingSource.ToolTipText = SayMovePrevKeepsBranchingSource;
			MovePrevMoveBranchingTarget.ToolTipText = SayMovePrevMovesBranchingTarget;
			MovePrevKeepBranchingTarget.ToolTipText = SayMovePrevKeepsBranchingTarget;

			MovePrevUseDefaults.Enabled = EnableMovePrevBranchingDefaultOptions;
			MovePrevUseDefaults.Checked = MovePrevBranchingDefaultOptions;
		}

		void MovePrevMoveBranchingSource_Click (object sender, System.EventArgs e)
		{
			MovePrevMovesBranchingSource = true;
		}
		void MovePrevKeepBranchingSource_Click (object sender, System.EventArgs e)
		{
			MovePrevKeepsBranchingSource = true;
		}
		void MovePrevMoveBranchingTarget_Click (object sender, System.EventArgs e)
		{
			MovePrevMovesBranchingTarget = true;
		}
		void MovePrevKeepBranchingTarget_Click (object sender, System.EventArgs e)
		{
			MovePrevKeepsBranchingTarget = true;
		}
		void MovePrevUseDefaults_Click (object sender, System.EventArgs e)
		{
			MovePrevBranchingDefaultOptions = true;
		}

		#endregion
	}
}
