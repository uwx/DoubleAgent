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

		private void ShowBalloonProperties ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			CheckBoxWordBalloon.Enabled = (CharacterFile != null) && (!Program.FileIsReadOnly);
			GroupBoxBalloonDisplay.Enabled = !IsPanelEmpty;

			if (IsPanelEmpty)
			{
				CheckBoxWordBalloon.Checked = false;
			}
			else
			{
				CheckBoxWordBalloon.Checked = ((CharacterFile.Header.Style & CharacterStyle.Balloon) != CharacterStyle.None);
			}

			ShowBalloonStyle ();
			ShowBalloonColors ();
			ShowBalloonFont ();
			ShowBalloonPreview ();

			PopIsPanelShowing (lWasShowing);
		}

		private void ShowBalloonStyle ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

			if (IsPanelEmpty)
			{
				NumericCharsPerLine.ResetText ();
				NumericNumLines.ResetText ();
				RadioButtonSizeToText.Checked = false;
				RadioButtonNumLines.Checked = false;
				CheckBoxAutoHide.Checked = false;
				CheckBoxAutoPace.Checked = false;
			}
			else
			{
				NumericCharsPerLine.Value = FileBalloon.PerLine;
				NumericNumLines.Value = FileBalloon.Lines;

				if ((CharacterFile.Header.Style & CharacterStyle.SizeToText) != CharacterStyle.None)
				{
					RadioButtonSizeToText.Checked = true;
					RadioButtonNumLines.Checked = false;
					NumericNumLines.Enabled = false;
				}
				else
				{
					RadioButtonSizeToText.Checked = false;
					RadioButtonNumLines.Checked = true;
					NumericNumLines.Enabled = !Program.FileIsReadOnly;
				}

				CheckBoxAutoHide.Checked = ((CharacterFile.Header.Style & CharacterStyle.NoAutoHide) == CharacterStyle.None);
				CheckBoxAutoPace.Checked = ((CharacterFile.Header.Style & CharacterStyle.NoAutoPace) == CharacterStyle.None);
			}

			LabelCharsPerLine.Enabled = !IsPanelEmpty;
			NumericCharsPerLine.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			RadioButtonSizeToText.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			RadioButtonNumLines.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			CheckBoxAutoHide.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;
			CheckBoxAutoPace.Enabled = !IsPanelEmpty && !Program.FileIsReadOnly;

			PopIsPanelShowing (lWasShowing);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonFont ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

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

			PopIsPanelShowing (lWasShowing);
		}

		private void ShowBalloonColors ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

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

			PopIsPanelShowing (lWasShowing);
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonPreview ()
		{
			Boolean lWasShowing = PushIsPanelShowing (true);

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

			PopIsPanelShowing (lWasShowing);
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelShowing && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				HandleEnabledChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericCharsPerLine_Validated (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				lUpdate.PerLine = (UInt16)NumericCharsPerLine.Value;
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		private void NumericNumLines_Validated (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				lUpdate.Lines = (UInt16)NumericNumLines.Value;
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		private void RadioButtonNumLines_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void RadioButtonSizeToText_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleSizeToTextChanged ();
			}
		}

		private void CheckBoxAutoHide_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoHideChanged ();
			}
		}

		private void CheckBoxAutoPace_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				HandleAutoPaceChanged ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateFont ();
			}
		}

		private void ButtonBalloonForeground_Click (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateForeground ();
			}
		}

		private void ButtonBalloonBackground_Click (object sender, EventArgs e)
		{
			if (!IsPanelShowing && (!IsPanelEmpty) && (!Program.FileIsReadOnly))
			{
				UpdateBackground ();
			}
		}

		private void ButtonBalloonBorder_Click (object sender, EventArgs e)
		{
			if (!IsPanelShowing && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateBorder ();
			}
		}

		#endregion
	}
}
