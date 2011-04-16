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
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor
{
	public partial class BranchingForm : UserControl
	{
		private CharacterFile		mCharacterFile = null;
		private FileAnimationFrame	mFrame = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BranchingForm ()
		{
			InitializeComponent ();

			if (MainForm.Singleton != null)
			{
				MainForm.Singleton.PanelAnimation.AnimationNameChanged += new AnimationNameChangedEventHandler (PanelAnimation_AnimationNameChanged);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		[System.ComponentModel.Browsable (false)]
		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				this.Frame = null;
			}
		}

		[System.ComponentModel.Browsable (false)]
		public FileAnimation Animation
		{
			get
			{
				return (mFrame == null) ? null : mFrame.Animation;
			}
		}
		[System.ComponentModel.Browsable (false)]
		public String FrameTitle
		{
			get
			{
				return Program.TitleFrame (mFrame);
			}
		}
		[System.ComponentModel.Browsable (false)]
		public FileAnimationFrame Frame
		{
			get
			{
				return mFrame;
			}
			set
			{
				mFrame = value;

				ShowFrameName ();
				ShowFrameBranching ();
				ShowExitFrame ();
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFrame == null));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowFrameName ()
		{
			if (IsEmpty)
			{
				TextBoxFrameName.ResetText ();
				TextBoxFrameName.Enabled = false;
			}
			else
			{
				TextBoxFrameName.Text = Program.TitleFrameAnimation (mFrame);
				TextBoxFrameName.Enabled = true;
			}
		}

		private void ShowFrameBranching ()
		{
			NumericBranching0.Validated -= new System.EventHandler (NumericBranching0_Validated);
			NumericBranching1.Validated -= new System.EventHandler (NumericBranching1_Validated);
			NumericBranching2.Validated -= new System.EventHandler (NumericBranching2_Validated);
			NumericTarget0.Validated -= new System.EventHandler (NumericTarget0_Validated);
			NumericTarget1.Validated -= new System.EventHandler (NumericTarget1_Validated);
			NumericTarget2.Validated -= new System.EventHandler (NumericTarget2_Validated);

			if (IsEmpty)
			{
				NumericBranching0.ResetText ();
				NumericBranching1.ResetText ();
				NumericBranching2.ResetText ();
				NumericBranchingNot.ResetText ();
				NumericTarget0.ResetText ();
				NumericTarget1.ResetText ();
				NumericTarget2.ResetText ();

				NumericBranching0.Enabled = false;
				NumericBranching1.Enabled = false;
				NumericBranching2.Enabled = false;
				NumericBranchingNot.Enabled = false;
				NumericTarget0.Enabled = false;
				NumericTarget1.Enabled = false;
				NumericTarget2.Enabled = false;
			}
			else
			{
				Int16	lRemainder = 100;

				if ((mFrame.Branching != null) && (mFrame.Branching.Length >= 1))
				{
					NumericTarget0.Maximum = this.Animation.Frames.Count;
					NumericTarget0.Value = mFrame.Branching[0].mFrameNdx + 1;
					NumericBranching0.Value = mFrame.Branching[0].mProbability;
					lRemainder -= (Int16)mFrame.Branching[0].mProbability;
				}
				else
				{
					NumericTarget0.ResetText ();
					NumericBranching0.Value = 0;
				}
				if ((mFrame.Branching != null) && (mFrame.Branching.Length >= 2))
				{
					NumericTarget1.Maximum = this.Animation.Frames.Count;
					NumericTarget1.Value = mFrame.Branching[1].mFrameNdx + 1;
					NumericBranching1.Value = mFrame.Branching[1].mProbability;
					lRemainder -= (Int16)mFrame.Branching[1].mProbability;
				}
				else
				{
					NumericTarget1.ResetText ();
					NumericBranching1.Value = 0;
				}
				if ((mFrame.Branching != null) && (mFrame.Branching.Length >= 3))
				{
					NumericTarget2.Maximum = this.Animation.Frames.Count;
					NumericTarget2.Value = mFrame.Branching[2].mFrameNdx + 1;
					NumericBranching2.Value = mFrame.Branching[2].mProbability;
					lRemainder -= (Int16)mFrame.Branching[2].mProbability;
				}
				else
				{
					NumericTarget2.ResetText ();
					NumericBranching2.Value = 0;
				}

				NumericBranchingNot.Value = lRemainder;

				NumericBranching0.Enabled = !MainForm.Singleton.FileIsReadOnly;
				NumericBranching1.Enabled = !MainForm.Singleton.FileIsReadOnly;
				NumericBranching2.Enabled = !MainForm.Singleton.FileIsReadOnly;
				NumericBranchingNot.Enabled = false;
				NumericTarget0.Enabled = (NumericBranching0.Value > 0) && !MainForm.Singleton.FileIsReadOnly;
				NumericTarget1.Enabled = (NumericBranching1.Value > 0) && !MainForm.Singleton.FileIsReadOnly;
				NumericTarget2.Enabled = (NumericBranching2.Value > 0) && !MainForm.Singleton.FileIsReadOnly;
			}

			NumericBranching0.Validated += new System.EventHandler (NumericBranching0_Validated);
			NumericBranching1.Validated += new System.EventHandler (NumericBranching1_Validated);
			NumericBranching2.Validated += new System.EventHandler (NumericBranching2_Validated);
			NumericTarget0.Validated += new System.EventHandler (NumericTarget0_Validated);
			NumericTarget1.Validated += new System.EventHandler (NumericTarget1_Validated);
			NumericTarget2.Validated += new System.EventHandler (NumericTarget2_Validated);
		}

		private void ShowExitFrame ()
		{
			CheckBoxExit.Click -= new System.EventHandler (CheckBoxExit_Click);
			NumericTargetExit.Validated -= new System.EventHandler (NumericTargetExit_Validated);

			if (IsEmpty)
			{
				CheckBoxExit.Checked = false;
				CheckBoxExit.Enabled = false;
				NumericTargetExit.ResetText ();
				NumericTargetExit.Enabled = false;
			}
			else
			{
				if (mFrame.ExitFrame >= 0)
				{
					CheckBoxExit.Checked = true;
					NumericTargetExit.Maximum = this.Animation.Frames.Count;
					NumericTargetExit.Value = mFrame.ExitFrame + 1;
					NumericTargetExit.Enabled = !MainForm.Singleton.FileIsReadOnly;

					if (mFrame.ExitFrame == (Int16)this.Animation.Frames.IndexOf (mFrame))
					{
						NumericTargetExit.Highlighted = true;
					}
				}
				else
				{
					CheckBoxExit.Checked = false;
					NumericTargetExit.ResetText ();
					NumericTargetExit.Enabled = false;
				}
				CheckBoxExit.Enabled = !MainForm.Singleton.FileIsReadOnly;
			}

			CheckBoxExit.Click += new System.EventHandler (CheckBoxExit_Click);
			NumericTargetExit.Validated += new System.EventHandler (NumericTargetExit_Validated);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class UpdateFrameExit : UndoableUpdate<FileAnimationFrame>
		{
			public UpdateFrameExit (CharacterFile pCharacterFile, FileAnimationFrame pFrame, Int16 pExitFrame)
				: base (pCharacterFile, pFrame)
			{
				this.Frame = pFrame;
				this.ExitFrame = pExitFrame;
			}

			public FileAnimationFrame Frame
			{
				get;
				set;
			}
			public Int16 ExitFrame
			{
				get;
				set;
			}

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleFrameAnimation (this.Frame));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return " exit branching";
				}
			}

			public override UndoUnit Apply ()
			{
				if (Target.ExitFrame != this.ExitFrame)
				{
					Int16	lSwap = Target.ExitFrame;
					Target.ExitFrame = this.ExitFrame;
					this.ExitFrame = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal class UpdateFrameBranching : UndoableUpdate<FileAnimationFrame>
		{
			public UpdateFrameBranching (CharacterFile pCharacterFile, FileAnimationFrame pFrame, FileFrameBranch[] pBranching)
				: base (pCharacterFile, pFrame)
			{
				this.Frame = pFrame;
				this.Branching = pBranching;
			}

			public FileAnimationFrame Frame
			{
				get;
				set;
			}
			public FileFrameBranch[] Branching
			{
				get;
				set;
			}

			public override String TargetDescription
			{
				get
				{
					return Program.QuotedTitle (Program.TitleFrameAnimation (this.Frame));
				}
			}
			public override String ChangeDescription
			{
				get
				{
					return " branching";
				}
			}

			public override UndoUnit Apply ()
			{
				if (!BranchingEqual (Target.Branching, this.Branching))
				{
					FileFrameBranch[]	lSwap = Target.Branching;
					Target.Branching = this.Branching;
					this.Branching = lSwap;

					return OnApplied (this);
				}
				return null;
			}
		}

		private Boolean ApplyBranchingUpdates ()
		{
			Boolean	lRet = false;

			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				Int16[]				lProbability = new Int16[3];
				Int16[]				lTarget = new Int16[3];
				FileFrameBranch[]	lBranching = null;
				int					lBranchingNdx;
				int					lBranchingCount = 0;

				lProbability[0] = (Int16)NumericBranching0.Value;
				lProbability[1] = (Int16)NumericBranching1.Value;
				lProbability[2] = (Int16)NumericBranching2.Value;
				lTarget[0] = (Int16)(NumericTarget0.Value - 1);
				lTarget[1] = (Int16)(NumericTarget1.Value - 1);
				lTarget[2] = (Int16)(NumericTarget2.Value - 1);

				if (lProbability[0] + lProbability[1] + lProbability[2] > 100)
				{
					lProbability[0] = (Int16)Math.Min (lProbability[0], (Int16)100);
					lProbability[1] = (Int16)Math.Min (lProbability[1], Math.Max (100 - lProbability[0], 0));
					lProbability[2] = (Int16)Math.Min (lProbability[2], Math.Max (100 - lProbability[1] - lProbability[0], 0));
				}
				if (lProbability[0] > 0)
				{
					lBranchingCount++;
				}
				if (lProbability[1] > 0)
				{
					lBranchingCount++;
				}
				if (lProbability[2] > 0)
				{
					lBranchingCount++;
				}

				if (lBranchingCount > 0)
				{
					lBranching = new FileFrameBranch[lBranchingCount];
					lBranchingCount = 0;
					for (lBranchingNdx = 0; lBranchingNdx < 3; lBranchingNdx++)
					{
						if (lProbability[lBranchingNdx] > 0)
						{
							lBranching[lBranchingCount].mProbability = (UInt16)lProbability[lBranchingNdx];
							lBranching[lBranchingCount].mFrameNdx = (Int16)lTarget[lBranchingNdx];
							lBranchingCount++;
						}
					}
				}
				if (!BranchingEqual (mFrame.Branching, lBranching))
				{
					UpdateFrameBranching	lUpdate = new UpdateFrameBranching (mCharacterFile, mFrame, lBranching);

					lUpdate.Applied += new UndoUnit.AppliedEventHandler (UpdateBranching_Applied);
					lRet = UpdateFrameBranching.PutUndo (lUpdate.Apply () as UpdateFrameBranching);
				}
			}
			return lRet;
		}

		private static Boolean BranchingEqual (FileFrameBranch[] pSource, FileFrameBranch[] pTarget)
		{
			if ((pSource == null) != (pTarget == null))
			{
				return false;
			}
			if ((pSource != null) && (pTarget != null))
			{
				if (pSource.Length != pTarget.Length)
				{
					return false;
				}
				for (int lNdx = 0; lNdx < pSource.Length; lNdx++)
				{
					if ((pSource[lNdx].mFrameNdx != pTarget[lNdx].mFrameNdx) || (pSource[lNdx].mProbability != pTarget[lNdx].mProbability))
					{
						return false;
					}
				}
			}
			return true;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void NumericBranching0_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericTarget0_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericBranching1_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericTarget1_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericBranching2_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericTarget2_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		void UpdateBranching_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateFrameBranching	lUpdate = sender as UpdateFrameBranching;

				if (lUpdate.Target == mFrame)
				{
					ShowFrameBranching ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxExit_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateFrameExit	lUpdate;

				if (CheckBoxExit.Checked)
				{
					Int16	lExitFrame = (Int16)Math.Min (Math.Max ((int)mFrame.ExitFrame, 0), this.Animation.Frames.Count - 1);

					if (lExitFrame == (Int16)this.Animation.Frames.IndexOf (mFrame))
					{
						lExitFrame++;
					}
					lUpdate = new UpdateFrameExit (mCharacterFile, mFrame, lExitFrame);
				}
				else
				{
					lUpdate = new UpdateFrameExit (mCharacterFile, mFrame, -1);
				}

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (ExitFrame_Applied);
				if (!UpdateFrameExit.PutUndo (lUpdate.Apply () as UpdateFrameExit))
				{
					ShowExitFrame ();
				}
			}
		}

		private void NumericTargetExit_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !MainForm.Singleton.FileIsReadOnly)
			{
				UpdateFrameExit	lUpdate = new UpdateFrameExit (mCharacterFile, mFrame, (Int16)(NumericTargetExit.Value - 1));

				lUpdate.Applied += new UndoUnit.AppliedEventHandler (ExitFrame_Applied);
				if (!UpdateFrameExit.PutUndo (lUpdate.Apply () as UpdateFrameExit))
				{
					ShowExitFrame ();
				}
			}
		}

		void ExitFrame_Applied (object sender, EventArgs e)
		{
			if (!IsEmpty)
			{
				UpdateFrameExit	lUpdate = sender as UpdateFrameExit;

				if ((lUpdate != null) && (lUpdate.Target == mFrame))
				{
					ShowExitFrame ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		internal void PanelAnimation_AnimationNameChanged (object sender, AnimationEventArgs e)
		{
			if (!IsEmpty && e.Animation == this.Animation)
			{
				ShowFrameName ();
			}
		}

		#endregion
	}
}
