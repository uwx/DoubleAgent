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
		private String				mFrameName = null;
		private String				mAnimationName = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BranchingForm ()
		{
			InitializeComponent ();
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Properties

		public CharacterFile CharacterFile
		{
			get
			{
				return mCharacterFile;
			}
			set
			{
				mCharacterFile = value;
				FileFrame = null;
				FrameName = null;
				AnimationName = null;
			}
		}

		public FileAnimationFrame FileFrame
		{
			get
			{
				return mFrame;
			}
			set
			{
				mFrame = value;
				FrameName = null;
				mAnimationName = null;
			}
		}

		public String FrameName
		{
			get
			{
				return mFrameName;
			}
			set
			{
				mFrameName = value;
			}
		}

		public String AnimationName
		{
			get
			{
				return mAnimationName;
			}
			set
			{
				mAnimationName = value;
				ShowFrameName ();
				ShowFrameBranching ();
				ShowExitFrame ();
			}
		}

		private bool IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFrame == null) || String.IsNullOrEmpty (mFrameName) || String.IsNullOrEmpty (mAnimationName));
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
				TextBoxFrameName.Text = mAnimationName + " - " + mFrameName;
				TextBoxFrameName.Enabled = true;
			}
		}

		private void ShowFrameBranching ()
		{
			if (IsEmpty)
			{
				NumericBranching1.ResetText ();
				NumericBranching2.ResetText ();
				NumericBranching3.ResetText ();
				NumericBranchingNot.ResetText ();
				NumericTarget1.ResetText ();
				NumericTarget2.ResetText ();
				NumericTarget3.ResetText ();

				NumericBranching1.Enabled = false;
				NumericBranching2.Enabled = false;
				NumericBranching3.Enabled = false;
				NumericBranchingNot.Enabled = false;
				NumericTarget1.Enabled = false;
				NumericTarget2.Enabled = false;
				NumericTarget3.Enabled = false;
			}
			else
			{
				Int16	lRemainder = 100;

				if ((mFrame.Branching != null) && (mFrame.Branching.Length >= 1))
				{
					NumericBranching1.Value = mFrame.Branching[0].mProbability;
					NumericTarget1.Value = mFrame.Branching[0].mFrameNdx;
					lRemainder -= (Int16)mFrame.Branching[0].mProbability;
				}
				else
				{
					NumericBranching1.Value = 0;
					NumericTarget1.ResetText ();
				}
				if ((mFrame.Branching != null) && (mFrame.Branching.Length >= 2))
				{
					NumericBranching2.Value = mFrame.Branching[1].mProbability;
					NumericTarget2.Value = mFrame.Branching[1].mFrameNdx;
					lRemainder -= (Int16)mFrame.Branching[1].mProbability;
				}
				else
				{
					NumericBranching2.Value = 0;
					NumericTarget2.ResetText ();
				}
				if ((mFrame.Branching != null) && (mFrame.Branching.Length >= 3))
				{
					NumericBranching3.Value = mFrame.Branching[2].mProbability;
					NumericTarget3.Value = mFrame.Branching[2].mFrameNdx;
					lRemainder -= (Int16)mFrame.Branching[2].mProbability;
				}
				else
				{
					NumericBranching3.Value = 0;
					NumericTarget3.ResetText ();
				}

				NumericBranchingNot.Value = lRemainder;

				NumericBranching1.Enabled = !Program.MainForm.FileIsReadOnly;
				NumericBranching2.Enabled = !Program.MainForm.FileIsReadOnly;
				NumericBranching3.Enabled = !Program.MainForm.FileIsReadOnly;
				NumericBranchingNot.Enabled = false;
				NumericTarget1.Enabled = (NumericBranching1.Value > 0) && !Program.MainForm.FileIsReadOnly;
				NumericTarget2.Enabled = (NumericBranching2.Value > 0) && !Program.MainForm.FileIsReadOnly;
				NumericTarget3.Enabled = (NumericBranching3.Value > 0) && !Program.MainForm.FileIsReadOnly;
			}
		}

		private void ShowExitFrame ()
		{
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
					NumericTargetExit.Value = mFrame.ExitFrame;
					NumericTargetExit.Enabled = !Program.MainForm.FileIsReadOnly;
				}
				else
				{
					CheckBoxExit.Checked = false;
					NumericTargetExit.ResetText ();
					NumericTargetExit.Enabled = false;
				}
				CheckBoxExit.Enabled = !Program.MainForm.FileIsReadOnly;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		private bool UpdateFrameBranching ()
		{
			bool	lRet = false;

			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				Int16[]				lProbability = new Int16[3];
				Int16[]				lTarget = new Int16[3];
				FileFrameBranch[]	lBranching = null;
				int					lBranchingNdx;
				int					lBranchingCount = 0;

				lProbability[0] = (Int16)NumericBranching1.Value;
				lProbability[1] = (Int16)NumericBranching2.Value;
				lProbability[2] = (Int16)NumericBranching3.Value;
				lTarget[0] = (Int16)NumericTarget1.Value;
				lTarget[1] = (Int16)NumericTarget2.Value;
				lTarget[2] = (Int16)NumericTarget3.Value;

				if (lProbability[0] + lProbability[1] + lProbability[2] > 100)
				{
					lProbability[0] = (Int16)Math.Min (lProbability[0], (Int16)100);
					lProbability[1] = (Int16)Math.Min (lProbability[1], Math.Max (100 - lProbability[0], 0));
					lProbability[2] = (Int16)Math.Min (lProbability[2], Math.Max (100 - lProbability[1] - lProbability[0], 0));
					lRet = true;
				}

				for (lBranchingNdx = 0; lBranchingNdx < 3; lBranchingNdx++)
				{
					if (lProbability[lBranchingNdx] > 0)
					{
						lBranchingCount++;
					}
					else if (lTarget[lBranchingNdx] > 0)
					{
						lRet = true;
					}
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
					mFrame.Branching = lBranching;
					lRet = true;
				}
			}
			return lRet;
		}

		private static bool BranchingEqual (FileFrameBranch[] pSource, FileFrameBranch[] pTarget)
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

		private void NumericBranching1_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (UpdateFrameBranching ())
				{
					ShowFrameBranching ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void NumericTarget1_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (UpdateFrameBranching ())
				{
					ShowFrameBranching ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void NumericBranching2_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (UpdateFrameBranching ())
				{
					ShowFrameBranching ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void NumericTarget2_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (UpdateFrameBranching ())
				{
					ShowFrameBranching ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void NumericBranching3_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (UpdateFrameBranching ())
				{
					ShowFrameBranching ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void NumericTarget3_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (UpdateFrameBranching ())
				{
					ShowFrameBranching ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void CheckBoxExit_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
			}
		}

		private void NumericTargetExit_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
			}
		}
		#endregion
	}
}
