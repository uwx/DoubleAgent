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
			mCharacterFile = null;
			mFileBalloon = null;

			InitializeComponent ();

			NumericNumLines.Minimum = (decimal)FileBalloon.MinLines;
			NumericNumLines.Maximum = (decimal)FileBalloon.MaxLines;
			NumericCharsPerLine.Minimum = (decimal)FileBalloon.MinPerLine;
			NumericCharsPerLine.Maximum = (decimal)FileBalloon.MaxPerLine;
		}

		public void LoadConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
		}

		public void SaveConfig ()
		{
			Properties.Settings	lSettings = Properties.Settings.Default;
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

		private bool IsEmpty
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
			CheckBoxWordBalloon.Enabled = (mCharacterFile != null) && (!Program.MainForm.FileIsReadOnly);

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
					NumericNumLines.Enabled = !Program.MainForm.FileIsReadOnly;
				}

				CheckBoxAutoHide.Checked = ((mCharacterFile.Header.Style & CharacterStyle.CharStyleNoAutoHide) == 0);
				CheckBoxAutoPace.Checked = ((mCharacterFile.Header.Style & CharacterStyle.CharStyleNoAutoPace) == 0);

				NumericCharsPerLine.Enabled = !Program.MainForm.FileIsReadOnly;
				RadioButtonSizeToText.Enabled = !Program.MainForm.FileIsReadOnly;
				RadioButtonNumLines.Enabled = !Program.MainForm.FileIsReadOnly;
				CheckBoxAutoHide.Enabled = !Program.MainForm.FileIsReadOnly;
				CheckBoxAutoPace.Enabled = !Program.MainForm.FileIsReadOnly;
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
				ButtonBalloonFont.Enabled = !Program.MainForm.FileIsReadOnly;
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
				ButtonBalloonForeground.Enabled = !Program.MainForm.FileIsReadOnly;
				LabelBalloonForegroundSample.BackColor = mFileBalloon.FgColor;
				ButtonBalloonBackground.Enabled = !Program.MainForm.FileIsReadOnly;
				LabelBalloonBackgroundSample.BackColor = mFileBalloon.BkColor;
				ButtonBalloonBorder.Enabled = !Program.MainForm.FileIsReadOnly;
				LabelBalloonBorderSample.BackColor = mFileBalloon.BrColor;
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Update

		internal class UpdateBalloon : UndoableUpdate
		{
			public UpdateBalloon (CharacterFile pCharacterFile)
				: base (pCharacterFile)
			{
				this.CharacterStyle = pCharacterFile.Header.Style & mStyleMask;
				if (pCharacterFile.Balloon != null)
				{
					this.Lines = pCharacterFile.Balloon.Lines;
					this.PerLine = pCharacterFile.Balloon.PerLine;
					this.BkColor = pCharacterFile.Balloon.BkColor;
					this.FgColor = pCharacterFile.Balloon.FgColor;
					this.BrColor = pCharacterFile.Balloon.BrColor;
					this.Font = pCharacterFile.Balloon.Font;
				}
			}

			public CharacterStyle CharacterStyle
			{
				get;
				set;
			}
			public UInt16 Lines
			{
				get;
				set;
			}
			public UInt16 PerLine
			{
				get;
				set;
			}
			public Color BkColor
			{
				get;
				set;
			}
			public Color FgColor
			{
				get;
				set;
			}
			public Color BrColor
			{
				get;
				set;
			}
			public Font Font
			{
				get;
				set;
			}

			public override UndoUnit Apply ()
			{
				UndoUnit	lApplied = null;

				if (this.StyleChanged)
				{
					CharacterStyle	lSwap = CharacterFile.Header.Style & mStyleMask;
					CharacterFile.Header.Style = (CharacterFile.Header.Style & ~mStyleMask) | (this.CharacterStyle & mStyleMask);
					this.CharacterStyle = lSwap;
					lApplied = this;
				}
				if (CharacterFile.Balloon != null)
				{
					if (this.SizeChanged)
					{
						UInt16	lSwap;

						lSwap = CharacterFile.Balloon.Lines;
						CharacterFile.Balloon.Lines = this.Lines;
						this.Lines = lSwap;

						lSwap = CharacterFile.Balloon.PerLine;
						CharacterFile.Balloon.PerLine = this.PerLine;
						this.PerLine = lSwap;

						lApplied = this;
					}
					if (this.ColorChanged)
					{
						Color	lSwap;

						lSwap = CharacterFile.Balloon.BkColor;
						CharacterFile.Balloon.BkColor = this.BkColor;
						this.BkColor = lSwap;

						lSwap = CharacterFile.Balloon.FgColor;
						CharacterFile.Balloon.FgColor = this.FgColor;
						this.FgColor = lSwap;

						lSwap = CharacterFile.Balloon.BrColor;
						CharacterFile.Balloon.BrColor = this.BrColor;
						this.BrColor = lSwap;

						lApplied = this;
					}
					if (this.FontChanged)
					{
						Font	lSwap;

						lSwap = CharacterFile.Balloon.Font;
						CharacterFile.Balloon.Font = this.Font;
						this.Font = lSwap;

						lApplied = this;
					}
				}
				if (lApplied != null)
				{
					return OnApplied (lApplied);
				}
				return null;
			}

			public override string ToString ()
			{
				String	lChange = StyleChanged ? "style" : SizeChanged ? "size" : ColorChanged ? "color" : FontChanged ? "font" : "";
				return String.Format ("balloon {0}", lChange).TrimEnd ();
			}

			private const CharacterStyle	mStyleMask = (CharacterStyle.CharStyleBalloon | CharacterStyle.CharStyleSizeToText | CharacterStyle.CharStyleNoAutoHide | CharacterStyle.CharStyleNoAutoPace);

			private bool StyleChanged
			{
				get
				{
					return (CharacterFile.Header.Style & mStyleMask) != (this.CharacterStyle & mStyleMask);
				}
			}

			private bool SizeChanged
			{
				get
				{
					return (CharacterFile.Balloon != null) && ((CharacterFile.Balloon.Lines != this.Lines) || (CharacterFile.Balloon.PerLine != this.PerLine));
				}
			}

			private bool ColorChanged
			{
				get
				{
					return (CharacterFile.Balloon != null) && ((CharacterFile.Balloon.BkColor != this.BkColor) || (CharacterFile.Balloon.FgColor != this.FgColor) || (CharacterFile.Balloon.BrColor != this.BrColor));
				}
			}

			private bool FontChanged
			{
				get
				{
					try
					{
						return (CharacterFile.Balloon != null) && !CharacterFile.Balloon.Font.Equals (this.Font);
					}
					catch
					{
					}
					return false;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckedChanged (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.MainForm.FileIsReadOnly)
			{
				UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

				if (CheckBoxWordBalloon.Checked)
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleBalloon;
				}
				else
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleBalloon;
				}
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableBalloonUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
			}
		}

		private void UndoableBalloonUpdate_Applied (object sender, EventArgs e)
		{
			ShowBalloonProperties ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void NumericCharsPerLine_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

				lUpdate.PerLine = (UInt16)NumericCharsPerLine.Value;
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableStyleUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
			}
		}

		private void NumericNumLines_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

				lUpdate.Lines = (UInt16)NumericNumLines.Value;
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableStyleUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
			}
		}

		private void RadioButtonNumLines_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (RadioButtonNumLines.Checked)
				{
					UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleSizeToText;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableStyleUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
				}
			}
		}

		private void RadioButtonSizeToText_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (RadioButtonSizeToText.Checked)
				{
					UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

					lUpdate.CharacterStyle |= CharacterStyle.CharStyleSizeToText;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableStyleUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
				}
			}
		}

		private void CheckBoxAutoHide_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

				if (CheckBoxAutoHide.Checked)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleNoAutoHide;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleNoAutoHide;
				}
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableStyleUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
			}
		}

		private void CheckBoxAutoPace_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

				if (CheckBoxAutoPace.Checked)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.CharStyleNoAutoPace;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.CharStyleNoAutoPace;
				}
				lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableStyleUpdate_Applied);
				UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
			}
		}

		private void UndoableStyleUpdate_Applied (object sender, EventArgs e)
		{
			ShowBalloonStyle ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonFont_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				FontDialog	lFontDialog = new FontDialog ();

				lFontDialog.ShowEffects = false;
				lFontDialog.Font = mFileBalloon.Font;
				if (lFontDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

					lUpdate.Font = lFontDialog.Font;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableFontUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
				}
			}
		}

		private void UndoableFontUpdate_Applied (object sender, EventArgs e)
		{
			ShowBalloonFont ();
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ButtonBalloonForeground_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.FgColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

					lUpdate.FgColor = lColorDialog.Color;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableColorUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
				}
			}
		}

		private void ButtonBalloonBackground_Click (object sender, EventArgs e)
		{
			if ((!IsEmpty) && (!Program.MainForm.FileIsReadOnly))
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.BkColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

					lUpdate.BkColor = lColorDialog.Color;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableColorUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
				}
			}
		}

		private void ButtonBalloonBorder_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.BrColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					UpdateBalloon	lUpdate = new UpdateBalloon (mCharacterFile);

					lUpdate.BrColor = lColorDialog.Color;
					lUpdate.Applied += new UndoUnit.AppliedEvent (UndoableColorUpdate_Applied);
					UndoableUpdate.PutUndo (lUpdate.Apply () as UpdateBalloon);
				}
			}
		}

		private void UndoableColorUpdate_Applied (object sender, EventArgs e)
		{
			ShowBalloonColors ();
		}

		#endregion
	}
}
