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
	public partial class BranchingPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BranchingPanel ()
		{
			InitializeComponent ();
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
				if (FilePart is ResolveAnimationFrame)
				{
					(FilePart as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeBranching;
				}
				Frame = (base.FilePart is ResolveAnimationFrame) ? (base.FilePart as ResolveAnimationFrame).Target : null;

				ShowFrameName ();
				ShowFrameBranching ();
				ShowExitFrame ();
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
				return Global.TitleFrame (Frame);
			}
		}

		public override Boolean IsEmpty
		{
			get
			{
				return base.IsEmpty || (Frame == null);
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
				TextBoxFrameName.Text = Global.TitleFrameAnimation (Frame);
				TextBoxFrameName.Enabled = true;
			}
		}

		private void ShowFrameBranching ()
		{
			CausesValidation = false;

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
				Int16 lRemainder = 100;

				if ((Frame.Branching != null) && (Frame.Branching.Length >= 1))
				{
					NumericTarget0.Maximum = this.Animation.Frames.Count;
					NumericTarget0.Value = Frame.Branching[0].mFrameNdx + 1;
					NumericBranching0.Value = Frame.Branching[0].mProbability;
					lRemainder -= (Int16)Frame.Branching[0].mProbability;
				}
				else
				{
					NumericTarget0.ResetText ();
					NumericBranching0.Value = 0;
				}
				if ((Frame.Branching != null) && (Frame.Branching.Length >= 2))
				{
					NumericTarget1.Maximum = this.Animation.Frames.Count;
					NumericTarget1.Value = Frame.Branching[1].mFrameNdx + 1;
					NumericBranching1.Value = Frame.Branching[1].mProbability;
					lRemainder -= (Int16)Frame.Branching[1].mProbability;
				}
				else
				{
					NumericTarget1.ResetText ();
					NumericBranching1.Value = 0;
				}
				if ((Frame.Branching != null) && (Frame.Branching.Length >= 3))
				{
					NumericTarget2.Maximum = this.Animation.Frames.Count;
					NumericTarget2.Value = Frame.Branching[2].mFrameNdx + 1;
					NumericBranching2.Value = Frame.Branching[2].mProbability;
					lRemainder -= (Int16)Frame.Branching[2].mProbability;
				}
				else
				{
					NumericTarget2.ResetText ();
					NumericBranching2.Value = 0;
				}

				NumericBranchingNot.Value = lRemainder;

				NumericBranching0.Enabled = !Program.FileIsReadOnly;
				NumericBranching1.Enabled = !Program.FileIsReadOnly;
				NumericBranching2.Enabled = !Program.FileIsReadOnly;
				NumericBranchingNot.Enabled = false;
				NumericTarget0.Enabled = (NumericBranching0.Value > 0) && !Program.FileIsReadOnly;
				NumericTarget1.Enabled = (NumericBranching1.Value > 0) && !Program.FileIsReadOnly;
				NumericTarget2.Enabled = (NumericBranching2.Value > 0) && !Program.FileIsReadOnly;
			}

			CausesValidation = Visible;
		}

		private void ShowExitFrame ()
		{
			CausesValidation = false;

			if (IsEmpty)
			{
				CheckBoxExit.Checked = false;
				CheckBoxExit.Enabled = false;
				NumericTargetExit.ResetText ();
				NumericTargetExit.Enabled = false;
			}
			else
			{
				if (Frame.ExitFrame >= 0)
				{
					CheckBoxExit.Checked = true;
					NumericTargetExit.Maximum = this.Animation.Frames.Count;
					NumericTargetExit.Value = Frame.ExitFrame + 1;
					NumericTargetExit.Enabled = !Program.FileIsReadOnly;

					if (Frame.ExitFrame == (Int16)this.Animation.Frames.IndexOf (Frame))
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
				CheckBoxExit.Enabled = !Program.FileIsReadOnly;
			}

			CausesValidation = Visible;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private Boolean ApplyBranchingUpdates ()
		{
			Boolean lRet = false;

			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				Int16[] lProbability = new Int16[3];
				Int16[] lTarget = new Int16[3];
				FileFrameBranch[] lBranching = null;
				int lBranchingNdx;
				int lBranchingCount = 0;

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
				if (!UpdateAnimationFrame.BranchingEqual (Frame.Branching, lBranching))
				{
					UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

					lUpdate.Branching = lBranching;
					lRet = UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this);
				}
			}
			return lRet;
		}

		///////////////////////////////////////////////////////////////////////////////

		protected override void UpdateApplied (object pUpdate)
		{
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			UpdateAnimationFrame lUpdateFrame = pUpdate as UpdateAnimationFrame;

			if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == this.Animation) && lUpdateAnimation.NameChanged)
			{
				ShowFrameName ();
			}
			else if ((lUpdateFrame != null) && (lUpdateFrame.Target == Frame))
			{
				if (lUpdateFrame.BranchingChanged)
				{
					ShowFrameBranching ();
				}
				if (lUpdateFrame.ExitFrameChanged)
				{
					ShowExitFrame ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void NumericBranching0_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericTarget0_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericBranching1_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericTarget1_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericBranching2_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		private void NumericTarget2_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxExit_Click (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

				if (CheckBoxExit.Checked)
				{
					Int16 lExitFrame = (Int16)Math.Min (Math.Max ((int)Frame.ExitFrame, 0), this.Animation.Frames.Count - 1);

					if (lExitFrame == (Int16)this.Animation.Frames.IndexOf (Frame))
					{
						lExitFrame++;
					}
					lUpdate.ExitFrame = lExitFrame;
				}
				else
				{
					lUpdate.ExitFrame = -1;
				}

				if (!UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this))
				{
					ShowExitFrame ();
				}
			}
		}

		private void NumericTargetExit_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

				lUpdate.ExitFrame = (Int16)(NumericTargetExit.Value - 1);
				if (!UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateAnimationFrame, this))
				{
					ShowExitFrame ();
				}
			}
		}

		#endregion
	}
}
