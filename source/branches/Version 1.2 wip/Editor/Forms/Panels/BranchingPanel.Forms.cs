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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using AgentCharacterEditor.Properties;

namespace AgentCharacterEditor.Panels
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
		#region Event Handlers

		private void NumericBranching0_Validated (object sender, EventArgs e)
		{
			if (!ApplyBranchingUpdates ())
			{
				ShowFrameBranching ();
			}
			NumericBranching0.IsModified = false;
		}

		private void NumericTarget0_Validated (object sender, EventArgs e)
		{
			if (!ApplyBranchingUpdates ())
			{
				ShowFrameBranching ();
			}
			NumericTarget0.IsModified = false;
		}

		private void NumericBranching1_Validated (object sender, EventArgs e)
		{
			if (!ApplyBranchingUpdates ())
			{
				ShowFrameBranching ();
			}
			NumericBranching1.IsModified = false;
		}

		private void NumericTarget1_Validated (object sender, EventArgs e)
		{
			if (!ApplyBranchingUpdates ())
			{
				ShowFrameBranching ();
			}
			NumericTarget1.IsModified = false;
		}

		private void NumericBranching2_Validated (object sender, EventArgs e)
		{
			if (!ApplyBranchingUpdates ())
			{
				ShowFrameBranching ();
			}
			NumericBranching2.IsModified = false;
		}

		private void NumericTarget2_Validated (object sender, EventArgs e)
		{
			if (!ApplyBranchingUpdates ())
			{
				ShowFrameBranching ();
			}
			NumericTarget2.IsModified = false;
		}

		//=============================================================================

		private void CheckBoxExit_Click (object sender, EventArgs e)
		{
			HandleExitTypeChanged ();
		}

		private void NumericTargetExit_Validated (object sender, EventArgs e)
		{
			HandleExitFrameChanged ();
			NumericTargetExit.IsModified = false;
		}

		#endregion
	}
}
