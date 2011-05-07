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
using System.Text;
using System.Windows.Forms;
using DoubleAgent;
using DoubleAgent.Character;
using AgentCharacterEditor.Updates;

namespace AgentCharacterEditor
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
				ShowBalloonProperties ();
			}
		}

		protected FileBalloon FileBalloon
		{
			get
			{
				return (CharacterFile == null) ? null : CharacterFile.Balloon;
			}
		}

		protected override Boolean IsEmpty
		{
			get
			{
				return ((FileBalloon == null) || ((CharacterFile.Header.Style & CharacterStyle.Balloon) == CharacterStyle.None));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowBalloonProperties ()
		{
			CheckBoxWordBalloon.Enabled = (CharacterFile != null) && (!Program.FileIsReadOnly);
			GroupBoxBalloonDisplay.Enabled = !IsEmpty;

			CausesValidation = false;
			if (IsEmpty)
			{
				CheckBoxWordBalloon.Checked = false;
			}
			else
			{
				CheckBoxWordBalloon.Checked = ((CharacterFile.Header.Style & CharacterStyle.Balloon) != CharacterStyle.None);
			}
			CausesValidation = Visible;

			ShowBalloonStyle ();
			ShowBalloonColors ();
			ShowBalloonFont ();
			ShowBalloonPreview ();
		}

		private void ShowBalloonStyle ()
		{
			CausesValidation = false;

			if (IsEmpty)
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

			LabelCharsPerLine.Enabled = !IsEmpty;
			NumericCharsPerLine.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			RadioButtonSizeToText.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			RadioButtonNumLines.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			CheckBoxAutoHide.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			CheckBoxAutoPace.Enabled = !IsEmpty && !Program.FileIsReadOnly;

			CausesValidation = Visible;
		}

		private void ShowBalloonFont ()
		{
			if (IsEmpty)
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
					System.Drawing.Font lFont = FileBalloon.Font;
					String lFontName = String.IsNullOrEmpty (lFont.OriginalFontName) ? lFont.Name : lFont.OriginalFontName;

					LabelBalloonFontSample.Text = lFontName + " " + ((int)(lFont.SizeInPoints + 0.4999)).ToString ();
					if (lFont.Bold)
					{
						LabelBalloonFontSample.Text += " Bold";
					}
					if (lFont.Italic)
					{
						LabelBalloonFontSample.Text += " Italic";
					}
					LabelBalloonFontSample.Font = lFont;

					if ((lFont.Name != lFont.OriginalFontName)
						|| (lFont.SizeInPoints != (float)(int)lFont.SizeInPoints)
						)
					{
						LabelBalloonFontSample.Text += " (" + lFont.Name + " " + lFont.SizeInPoints.ToString () + ")";
					}
				}
				ButtonBalloonFont.Enabled = !Program.FileIsReadOnly;
			}
		}

		private void ShowBalloonColors ()
		{
			if (IsEmpty)
			{
				LabelBalloonForegroundSample.BackColor = BackColor;
				LabelBalloonBackgroundSample.BackColor = BackColor;
				LabelBalloonBorderSample.BackColor = BackColor;
			}
			else
			{
				LabelBalloonForegroundSample.BackColor = FileBalloon.FgColor;
				LabelBalloonBackgroundSample.BackColor = FileBalloon.BkColor;
				LabelBalloonBorderSample.BackColor = FileBalloon.BrColor;
			}

			ButtonBalloonForeground.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			ButtonBalloonBackground.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			ButtonBalloonBorder.Enabled = !IsEmpty && !Program.FileIsReadOnly;
			LabelBalloonForegroundSample.Enabled = !IsEmpty;
			LabelBalloonBackgroundSample.Enabled = !IsEmpty;
			LabelBalloonBorderSample.Enabled = !IsEmpty;
		}

		private void ShowBalloonPreview ()
		{
			if (IsEmpty)
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
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteBalloon (FileBalloon pPasteBalloon)
		{
			if ((CharacterFile != null) && (pPasteBalloon != null) && !Program.FileIsReadOnly)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			return null;
		}

		protected override void UpdateApplied (Object pUpdate)
		{
			UpdateCharacterBalloon lUpdateCharacterBalloon = pUpdate as UpdateCharacterBalloon;
			UpdateCharacterTts lUpdateCharacterTts = pUpdate as UpdateCharacterTts;

			if ((lUpdateCharacterBalloon != null) && (lUpdateCharacterBalloon.CharacterFile == CharacterFile))
			{
				if (lUpdateCharacterBalloon.EnabledChanged)
				{
					ShowBalloonProperties ();
				}
				else if (!IsEmpty)
				{
					if (lUpdateCharacterBalloon.StyleChanged || lUpdateCharacterBalloon.SizeChanged)
					{
						ShowBalloonStyle ();
					}
					if (lUpdateCharacterBalloon.ColorChanged)
					{
						ShowBalloonColors ();
					}
					if (lUpdateCharacterBalloon.FontChanged)
					{
						ShowBalloonFont ();
					}
					ShowBalloonPreview ();
				}
			}

			if ((lUpdateCharacterTts != null) && (lUpdateCharacterTts.CharacterFile == CharacterFile))
			{
				if (!IsEmpty && lUpdateCharacterTts.EnabledChanged)
				{
					ShowBalloonPreview ();
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckedChanged (object sender, EventArgs e)
		{
			if (CausesValidation && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				if (CheckBoxWordBalloon.Checked)
				{
					lUpdate.CharacterStyle |= CharacterStyle.Balloon;
				}
				else
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.Balloon;
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericCharsPerLine_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				lUpdate.PerLine = (UInt16)NumericCharsPerLine.Value;
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		private void NumericNumLines_Validated (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				lUpdate.Lines = (UInt16)NumericNumLines.Value;
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		private void RadioButtonNumLines_CheckedChanged (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (RadioButtonNumLines.Checked)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.CharacterStyle &= ~CharacterStyle.SizeToText;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		private void RadioButtonSizeToText_CheckedChanged (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				if (RadioButtonSizeToText.Checked)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.CharacterStyle |= CharacterStyle.SizeToText;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		private void CheckBoxAutoHide_CheckedChanged (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				if (CheckBoxAutoHide.Checked)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.NoAutoHide;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.NoAutoHide;
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		private void CheckBoxAutoPace_CheckedChanged (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				if (CheckBoxAutoPace.Checked)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.NoAutoPace;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.NoAutoPace;
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				FontDialog lFontDialog = new FontDialog ();

				lFontDialog.ShowEffects = false;
				lFontDialog.Font = FileBalloon.Font;
				if (lFontDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.Font = lFontDialog.Font;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonForeground_Click (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				ColorDialog lColorDialog = new ColorDialog ();

				lColorDialog.Color = FileBalloon.FgColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.FgColor = lColorDialog.Color;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		private void ButtonBalloonBackground_Click (object sender, EventArgs e)
		{
			if (CausesValidation && (!IsEmpty) && (!Program.FileIsReadOnly))
			{
				ColorDialog lColorDialog = new ColorDialog ();

				lColorDialog.Color = FileBalloon.BkColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.BkColor = lColorDialog.Color;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		private void ButtonBalloonBorder_Click (object sender, EventArgs e)
		{
			if (CausesValidation && !IsEmpty && !Program.FileIsReadOnly)
			{
				ColorDialog lColorDialog = new ColorDialog ();

				lColorDialog.Color = FileBalloon.BrColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.BrColor = lColorDialog.Color;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainForm.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		#endregion
	}
}
