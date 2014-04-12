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
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;
using AgentCharacterEditor.Global;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class BranchingPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public override ResolvePart FilePart
		{
			get
			{
				return base.FilePart;
			}
			protected set
			{
				base.FilePart = value;
				if (FilePart is ResolveAnimationFrame)
				{
					(FilePart as ResolveAnimationFrame).Scope = ResolveAnimationFrame.ScopeType.ScopeBranching;
				}
				Frame = (base.FilePart is ResolveAnimationFrame) ? (base.FilePart as ResolveAnimationFrame).Target : null;
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
				return Titles.Frame (Frame);
			}
		}

		public override Boolean IsPanelEmpty
		{
			get
			{
				return base.IsPanelEmpty || (Frame == null);
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		public override void ShowFilePart (ResolvePart pFilePart)
		{
			try
			{
				FilePart = pFilePart;

				ShowFrameName ();
				ShowFrameBranching ();
				ShowExitFrame ();
			}
			catch (Exception pException)
			{
				System.Diagnostics.Debug.Print (pException.Message);
			}
		}

		private void ShowFrameName ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					TextBoxFrameName.Clear ();
					TextBoxFrameName.IsEnabled = false;
				}
				else
				{
					TextBoxFrameName.Text = Titles.FrameAnimation (Frame);
					TextBoxFrameName.IsEnabled = true;
				}
			}
		}

		private void ShowFrameBranching ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					NumericBranching0.Clear ();
					NumericBranching1.Clear ();
					NumericBranching2.Clear ();
					NumericBranchingNot.Clear ();
					NumericTarget0.Clear ();
					NumericTarget1.Clear ();
					NumericTarget2.Clear ();

					NumericBranching0.IsEnabled = false;
					NumericBranching1.IsEnabled = false;
					NumericBranching2.IsEnabled = false;
					NumericBranchingNot.IsEnabled = false;
					NumericTarget0.IsEnabled = false;
					NumericTarget1.IsEnabled = false;
					NumericTarget2.IsEnabled = false;
				}
				else
				{
					Int16 lRemainder = 100;

					if (Frame.GetBranchingCount () >= 1)
					{
						NumericTarget0.Maximum = this.Animation.Frames.Count;
						NumericTarget0.Value = Frame.Branching[0].mFrameNdx + 1;
						NumericBranching0.Value = Frame.Branching[0].mProbability;
						lRemainder -= (Int16)Frame.Branching[0].mProbability;
					}
					else
					{
						NumericTarget0.Clear ();
						NumericBranching0.Value = 0;
					}
					if (Frame.GetBranchingCount () >= 2)
					{
						NumericTarget1.Maximum = this.Animation.Frames.Count;
						NumericTarget1.Value = Frame.Branching[1].mFrameNdx + 1;
						NumericBranching1.Value = Frame.Branching[1].mProbability;
						lRemainder -= (Int16)Frame.Branching[1].mProbability;
					}
					else
					{
						NumericTarget1.Clear ();
						NumericBranching1.Value = 0;
					}
					if (Frame.GetBranchingCount () >= 3)
					{
						NumericTarget2.Maximum = this.Animation.Frames.Count;
						NumericTarget2.Value = Frame.Branching[2].mFrameNdx + 1;
						NumericBranching2.Value = Frame.Branching[2].mProbability;
						lRemainder -= (Int16)Frame.Branching[2].mProbability;
					}
					else
					{
						NumericTarget2.Clear ();
						NumericBranching2.Value = 0;
					}

					NumericBranchingNot.Value = lRemainder;

					NumericBranching0.IsEnabled = !Program.FileIsReadOnly;
					NumericBranching1.IsEnabled = !Program.FileIsReadOnly;
					NumericBranching2.IsEnabled = !Program.FileIsReadOnly;
					NumericBranchingNot.IsEnabled = false;
					NumericTarget0.IsEnabled = (NumericBranching0.Value > 0) && !Program.FileIsReadOnly;
					NumericTarget1.IsEnabled = (NumericBranching1.Value > 0) && !Program.FileIsReadOnly;
					NumericTarget2.IsEnabled = (NumericBranching2.Value > 0) && !Program.FileIsReadOnly;
				}

				NumericBranching0.IsModified = false;
				NumericBranching1.IsModified = false;
				NumericBranching2.IsModified = false;
				NumericBranchingNot.IsModified = false;
				NumericTarget0.IsModified = false;
				NumericTarget1.IsModified = false;
				NumericTarget2.IsModified = false;
			}
		}

		private void ShowExitFrame ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					CheckBoxExit.IsChecked = false;
					CheckBoxExit.IsEnabled = false;
					NumericTargetExit.Clear ();
					NumericTargetExit.IsEnabled = false;
				}
				else
				{
					if (Frame.ExitFrame >= 0)
					{
						CheckBoxExit.IsChecked = true;
						NumericTargetExit.Maximum = this.Animation.Frames.Count;
						NumericTargetExit.Value = Frame.ExitFrame + 1;
						NumericTargetExit.IsEnabled = !Program.FileIsReadOnly;

						if (Frame.ExitFrame == (Int16)this.Animation.Frames.IndexOf (Frame))
						{
							NumericTargetExit.IsHighlighted = true;
						}
					}
					else
					{
						CheckBoxExit.IsChecked = false;
						NumericTargetExit.Clear ();
						NumericTargetExit.IsEnabled = false;
					}
					CheckBoxExit.IsEnabled = !Program.FileIsReadOnly;
				}
				NumericTargetExit.IsModified = false;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private Boolean ApplyBranchingUpdates ()
		{
			Boolean lRet = false;

			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
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
				if (!Frame.IsBranchingEqual (lBranching))
				{
					UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

					lUpdate.Branching = lBranching;
					lRet = UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this);
				}
			}

			NumericBranching0.IsModified = false;
			NumericBranching1.IsModified = false;
			NumericBranching2.IsModified = false;
			NumericTarget0.IsModified = false;
			NumericTarget1.IsModified = false;
			NumericTarget2.IsModified = false;

			return lRet;
		}

		//=============================================================================

		private void HandleExitTypeChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

				if (CheckBoxExit.IsChecked.Value)
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

				if (!UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this))
				{
					ShowExitFrame ();
				}
			}

		}

		private void HandleExitFrameChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateAnimationFrame lUpdate = new UpdateAnimationFrame (Frame, false);

				lUpdate.ExitFrame = (Int16)(NumericTargetExit.Value - 1);
				if (!UpdateAnimationFrame.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateAnimationFrame, this))
				{
					ShowExitFrame ();
				}
			}
			NumericTargetExit.IsModified = false;
		}

		//=============================================================================

		protected override void UpdateApplied (object pUpdate)
		{
			UpdateAnimation lUpdateAnimation = pUpdate as UpdateAnimation;
			UpdateAnimationFrame lUpdateFrame = pUpdate as UpdateAnimationFrame;
			AddDeleteAnimationFrame lAddDeleteFrame = pUpdate as AddDeleteAnimationFrame;

			if ((lUpdateAnimation != null) && (lUpdateAnimation.Target == Animation) && lUpdateAnimation.NameChanged)
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
			else if ((lAddDeleteFrame != null) && (lAddDeleteFrame.Animation == Animation))
			{
				ShowFrameBranching ();
				ShowExitFrame ();
			}
		}

		#endregion
	}
}
