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
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AgentCharacterEditor.Panels
{
	public partial class BranchingPanel : AgentCharacterEditor.Panels.FilePartPanel
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

		private void NumericBranching0_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericBranching0.IsModified)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
			NumericBranching0.IsModified = false;
		}

		private void NumericTarget0_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericTarget0.IsModified)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
			NumericTarget0.IsModified = false;
		}

		private void NumericBranching1_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericBranching1.IsModified)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
			NumericBranching1.IsModified = false;
		}

		private void NumericTarget1_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericTarget1.IsModified)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
			NumericTarget1.IsModified = false;
		}

		private void NumericBranching2_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericBranching2.IsModified)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
			NumericBranching2.IsModified = false;
		}

		private void NumericTarget2_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericTarget2.IsModified)
			{
				if (!ApplyBranchingUpdates ())
				{
					ShowFrameBranching ();
				}
			}
			NumericTarget2.IsModified = false;
		}

		//=============================================================================

		private void CheckBoxExit_CheckChanged (object sender, RoutedEventArgs e)
		{
			HandleExitTypeChanged ();
		}

		private void NumericTargetExit_IsModifiedChanged (object sender, RoutedEventArgs e)
		{
			if (NumericTargetExit.IsModified)
			{
				HandleExitFrameChanged ();
			}
			NumericTargetExit.IsModified = false;
		}
		#endregion
	}
}
