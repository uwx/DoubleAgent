﻿/////////////////////////////////////////////////////////////////////////////
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
using System.Windows.Forms;
using AgentCharacterEditor.Global;
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class BalloonPanel : FilePartPanel
	{
		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BalloonPanel ()
		{
			InitializeComponent ();

			NumericNumLines.Minimum = (decimal)FileBalloon.MinLines;
			NumericNumLines.Maximum = (decimal)FileBalloon.MaxLines;
			NumericCharsPerLine.Minimum = (decimal)FileBalloon.MinPerLine;
			NumericCharsPerLine.Maximum = (decimal)FileBalloon.MaxPerLine;
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowBalloonFont ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
			{
				LabelBalloonFontSample.ResetText ();
				LabelBalloonFontSample.Font = Font;
				ButtonBalloonFont.Enabled = false;
			}
			else
			{
				if (FileBalloon.Font == null)
				{
					LabelBalloonFontSample.ResetText ();
					LabelBalloonFontSample.Font = Font;
				}
				else
				{
					LabelBalloonFontSample.Text = FormatFontName (FileBalloon.Font);
					LabelBalloonFontSample.Font = FileBalloon.Font;
				}
				ButtonBalloonFont.Enabled = !Program.FileIsReadOnly;
			}

			PopIsPanelFilling (lWasFilling);
		}

		private void ShowBalloonColors ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
			{
				LabelBalloonForegroundSample.BackColor = BackColor;
				LabelBalloonBackgroundSample.BackColor = BackColor;
				LabelBalloonBorderSample.BackColor = BackColor;
				LabelBalloonFontSample.ForeColor = ForeColor;
				LabelBalloonFontSample.BackColor = BackColor;
			}
			else
			{
				LabelBalloonForegroundSample.BackColor = FileBalloon.FgColor;
				LabelBalloonBackgroundSample.BackColor = FileBalloon.BkColor;
				LabelBalloonBorderSample.BackColor = FileBalloon.BrColor;
				LabelBalloonFontSample.ForeColor = LabelBalloonForegroundSample.BackColor;
				LabelBalloonFontSample.BackColor = LabelBalloonBackgroundSample.BackColor;
			}

			ButtonBalloonForeground.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			ButtonBalloonBackground.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			ButtonBalloonBorder.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			LabelBalloonForegroundSample.Enabled = !IsPanelEmpty;
			LabelBalloonBackgroundSample.Enabled = !IsPanelEmpty;
			LabelBalloonBorderSample.Enabled = !IsPanelEmpty;

			PopIsPanelFilling (lWasFilling);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonPreview ()
		{
			Boolean lWasFilling = PushIsPanelFilling (true);

			if (IsPanelEmpty)
			{
				GroupBoxBalloonPreview.Enabled = false;
				WordBalloonPreview.CharacterFile = null;
			}
			else
			{
				GroupBoxBalloonPreview.Enabled = true;
				WordBalloonPreview.CharacterFile = CharacterFile;
			}
			WordBalloonPreview.Refresh ();

			PopIsPanelFilling (lWasFilling);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckChangedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				HandleEnabledChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericCharsPerLine_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleCharsPerLineChanged ();
			}
			NumericCharsPerLine.IsModified = false;
		}

		private void NumericNumLines_Validated (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleNumLinesChanged ();
			}
			NumericNumLines.IsModified = false;
		}

		private void RadioButtonNumLines_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void RadioButtonSizeToText_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void CheckBoxAutoHide_CheckChangedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoHideChanged ();
			}
		}

		private void CheckBoxAutoPace_CheckChangedChanged (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoPaceChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateFont ();
			}
		}

		private void ButtonBalloonForeground_Click (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateForeground ();
			}
		}

		private void ButtonBalloonBackground_Click (object sender, EventArgs e)
		{
			if (!IsPanelFilling && (!IsPanelEmpty) && (!Program.FileIsReadOnly))
			{
				UpdateBackground ();
			}
		}

		private void ButtonBalloonBorder_Click (object sender, EventArgs e)
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateBorder ();
			}
		}

		#endregion
	}
}
