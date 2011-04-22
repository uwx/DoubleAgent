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
	public partial class BalloonForm : UserControl
	{
		private CharacterFile	mCharacterFile = null;
		private FileBalloon		mFileBalloon = null;

		///////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BalloonForm ()
		{
			InitializeComponent ();

			NumericNumLines.Minimum = (decimal)FileBalloon.MinLines;
			NumericNumLines.Maximum = (decimal)FileBalloon.MaxLines;
			NumericCharsPerLine.Minimum = (decimal)FileBalloon.MinPerLine;
			NumericCharsPerLine.Maximum = (decimal)FileBalloon.MaxPerLine;

			if (Program.MainForm != null)
			{
				Program.MainForm.LoadConfig += new EventHandler (MainForm_LoadConfig);
				Program.MainForm.SaveConfig += new EventHandler (MainForm_SaveConfig);
			}
		}

		void MainForm_LoadConfig (object sender, EventArgs e)
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		void MainForm_SaveConfig (object sender, EventArgs e)
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
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
				if (mCharacterFile != null)
				{
					mFileBalloon = mCharacterFile.Balloon;
				}
				else
				{
					mFileBalloon = null;
				}
				ShowBalloonProperties ();
			}
		}

		private Boolean IsEmpty
		{
			get
			{
				return ((mCharacterFile == null) || (mFileBalloon == null) || ((mCharacterFile.Header.Style & CharacterStyle.CharStyleBalloon) == 0));
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Display

		private void ShowBalloonProperties ()
		{
			CheckBoxWordBalloon.Enabled = (mCharacterFile != null) && (!Program.FileIsReadOnly);

			CheckBoxWordBalloon.CheckedChanged -= new System.EventHandler (CheckBoxWordBalloon_CheckedChanged);
			if (IsEmpty)
			{
				CheckBoxWordBalloon.Checked = false;
			}
			else
			{
				CheckBoxWordBalloon.Checked = ((mCharacterFile.Header.Style & CharacterStyle.CharStyleBalloon) != 0);
			}
			CheckBoxWordBalloon.CheckedChanged += new System.EventHandler (CheckBoxWordBalloon_CheckedChanged);

			ShowBalloonStyle ();
			ShowBalloonColors ();
			ShowBalloonFont ();
		}

		private void ShowBalloonStyle ()
		{
			RadioButtonNumLines.CheckedChanged -= new System.EventHandler (RadioButtonNumLines_CheckedChanged);
			RadioButtonSizeToText.CheckedChanged -= new System.EventHandler (RadioButtonSizeToText_CheckedChanged);
			CheckBoxAutoHide.CheckedChanged -= new System.EventHandler (CheckBoxAutoHide_CheckedChanged);
			CheckBoxAutoPace.CheckedChanged -= new System.EventHandler (CheckBoxAutoPace_CheckedChanged);

			if (IsEmpty)
			{
				NumericCharsPerLine.ResetText ();
				NumericNumLines.ResetText ();
				RadioButtonSizeToText.Checked = false;
				RadioButtonNumLines.Checked = false;
				CheckBoxAutoHide.Checked = false;
				CheckBoxAutoPace.Checked = false;

				NumericCharsPerLine.Enabled = false;
				NumericNumLines.Enabled = false;
				RadioButtonSizeToText.Enabled = false;
				RadioButtonNumLines.Enabled = false;
				CheckBoxAutoHide.Enabled = false;
				CheckBoxAutoPace.Enabled = false;
			}
			else
			{
				NumericCharsPerLine.Value = mFileBalloon.PerLine;
				NumericNumLines.Value = mFileBalloon.Lines;

				if ((mCharacterFile.Header.Style & CharacterStyle.CharStyleSizeToText) != 0)
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

				CheckBoxAutoHide.Checked = ((mCharacterFile.Header.Style & CharacterStyle.CharStyleNoAutoHide) == 0);
				CheckBoxAutoPace.Checked = ((mCharacterFile.Header.Style & CharacterStyle.CharStyleNoAutoPace) == 0);

				NumericCharsPerLine.Enabled = !Program.FileIsReadOnly;
				RadioButtonSizeToText.Enabled = !Program.FileIsReadOnly;
				RadioButtonNumLines.Enabled = !Program.FileIsReadOnly;
				CheckBoxAutoHide.Enabled = !Program.FileIsReadOnly;
				CheckBoxAutoPace.Enabled = !Program.FileIsReadOnly;
			}

			RadioButtonNumLines.CheckedChanged += new System.EventHandler (RadioButtonNumLines_CheckedChanged);
			RadioButtonSizeToText.CheckedChanged += new System.EventHandler (RadioButtonSizeToText_CheckedChanged);
			CheckBoxAutoHide.CheckedChanged += new System.EventHandler (CheckBoxAutoHide_CheckedChanged);
			CheckBoxAutoPace.CheckedChanged += new System.EventHandler (CheckBoxAutoPace_CheckedChanged);
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
				if (mFileBalloon.Font == null)
				{
					LabelBalloonFontSample.ResetText ();
					LabelBalloonFontSample.Font = Font;
				}
				else
				{
					System.Drawing.Font	lFont = mFileBalloon.Font;
					String				lFontName = String.IsNullOrEmpty (lFont.OriginalFontName) ? lFont.Name : lFont.OriginalFontName;

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
				ButtonBalloonForeground.Enabled = false;
				LabelBalloonForegroundSample.BackColor = BackColor;
				ButtonBalloonBackground.Enabled = false;
				LabelBalloonBackgroundSample.BackColor = BackColor;
				ButtonBalloonBorder.Enabled = false;
				LabelBalloonBorderSample.BackColor = BackColor;
			}
			else
			{
				ButtonBalloonForeground.Enabled = !Program.FileIsReadOnly;
				LabelBalloonForegroundSample.BackColor = mFileBalloon.FgColor;
				ButtonBalloonBackground.Enabled = !Program.FileIsReadOnly;
				LabelBalloonBackgroundSample.BackColor = mFileBalloon.BkColor;
				ButtonBalloonBorder.Enabled = !Program.FileIsReadOnly;
				LabelBalloonBorderSample.BackColor = mFileBalloon.BrColor;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal UndoableUpdate PasteBalloon (FileBalloon pPasteBalloon)
		{
			if ((mCharacterFile != null) && (pPasteBalloon != null) && !Program.FileIsReadOnly)
			{
				System.Media.SystemSounds.Beep.Play ();
			}
			return null;
		}

		private void OnUpdateApplied (object sender, EventArgs e)
		{
			UpdateCharacterBalloon	lUpdate = sender as UpdateCharacterBalloon;

			if ((lUpdate != null) && (lUpdate.CharacterFile == mCharacterFile))
			{
				if ((lUpdate.CharacterStyle & CharacterStyle.CharStyleBalloon) != (mCharacterFile.Header.Style & CharacterStyle.CharStyleBalloon))
				{
					ShowBalloonProperties ();
				}
				else if (!IsEmpty)
				{
					if (lUpdate.StyleChanged || lUpdate.SizeChanged)
					{
						ShowBalloonStyle ();
					}
					if (lUpdate.ColorChanged)
					{
						ShowBalloonColors ();
					}
					if (lUpdate.FontChanged)
					{
						ShowBalloonFont ();
					}
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckedChanged (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

				if (CheckBoxWordBalloon.Checked)
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleBalloon;
				}
				else
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleBalloon;
				}
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericCharsPerLine_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

				lUpdate.PerLine = (UInt16)NumericCharsPerLine.Value;
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
			}
		}

		private void NumericNumLines_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

				lUpdate.Lines = (UInt16)NumericNumLines.Value;
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
			}
		}

		private void RadioButtonNumLines_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				if (RadioButtonNumLines.Checked)
				{
					UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleSizeToText;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
				}
			}
		}

		private void RadioButtonSizeToText_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				if (RadioButtonSizeToText.Checked)
				{
					UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

					lUpdate.CharacterStyle |= CharacterStyle.CharStyleSizeToText;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
				}
			}
		}

		private void CheckBoxAutoHide_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

				if (CheckBoxAutoHide.Checked)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleNoAutoHide;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleNoAutoHide;
				}
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
			}
		}

		private void CheckBoxAutoPace_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

				if (CheckBoxAutoPace.Checked)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleNoAutoPace;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleNoAutoPace;
				}
				lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				FontDialog	lFontDialog = new FontDialog ();

				lFontDialog.ShowEffects = false;
				lFontDialog.Font = mFileBalloon.Font;
				if (lFontDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

					lUpdate.Font = lFontDialog.Font;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonForeground_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.FgColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

					lUpdate.FgColor = lColorDialog.Color;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
				}
			}
		}

		private void ButtonBalloonBackground_Click (object sender, EventArgs e)
		{
			if ((!IsEmpty) && (!Program.FileIsReadOnly))
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.BkColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

					lUpdate.BkColor = lColorDialog.Color;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
				}
			}
		}

		private void ButtonBalloonBorder_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.FileIsReadOnly)
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.BrColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateCharacterBalloon	lUpdate = new UpdateCharacterBalloon (mCharacterFile);

					lUpdate.BrColor = lColorDialog.Color;
					lUpdate.Applied += new UndoUnit.AppliedEventHandler (OnUpdateApplied);
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply () as UpdateCharacterBalloon, this);
				}
			}
		}

		#endregion
	}
}
