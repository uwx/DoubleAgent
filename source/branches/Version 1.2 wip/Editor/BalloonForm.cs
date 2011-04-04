using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
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

			if (IsEmpty)
			{
				CheckBoxWordBalloon.Checked = false;
			}
			else
			{
				CheckBoxWordBalloon.Checked = ((mCharacterFile.Header.Style & CharacterStyle.CharStyleBalloon) != 0);
			}
			ShowBalloonStyle ();
			ShowBalloonColors ();
			ShowBalloonFont ();
		}

		private void ShowBalloonStyle ()
		{
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
		#region Event Handlers

		private void CheckBoxWordBalloon_CheckedChanged (object sender, EventArgs e)
		{
			if ((mCharacterFile != null) && !Program.MainForm.FileIsReadOnly)
			{
				if (CheckBoxWordBalloon.Checked)
				{
					mCharacterFile.Header.Style |= CharacterStyle.CharStyleBalloon;
				}
				else
				{
					mCharacterFile.Header.Style &= ~CharacterStyle.CharStyleBalloon;
				}
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				ShowBalloonProperties ();
			}
		}

		private void NumericCharsPerLine_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mFileBalloon.PerLine = (UInt16)NumericCharsPerLine.Value;
				ShowBalloonStyle ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void NumericNumLines_Validated (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				mFileBalloon.Lines = (UInt16)NumericNumLines.Value;
				ShowBalloonStyle ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void RadioButtonNumLines_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (RadioButtonNumLines.Checked)
				{
					mCharacterFile.Header.Style &= ~CharacterStyle.CharStyleSizeToText;
					ShowBalloonStyle ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void RadioButtonSizeToText_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (RadioButtonSizeToText.Checked)
				{
					mCharacterFile.Header.Style |= CharacterStyle.CharStyleSizeToText;
					ShowBalloonStyle ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void CheckBoxAutoHide_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (CheckBoxAutoHide.Checked)
				{
					mCharacterFile.Header.Style &= ~CharacterStyle.CharStyleNoAutoHide;
				}
				else
				{
					mCharacterFile.Header.Style |= CharacterStyle.CharStyleNoAutoHide;
				}
				ShowBalloonStyle ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
		}

		private void CheckBoxAutoPace_CheckedChanged (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				if (this.CheckBoxAutoHide.Checked)
				{
					mCharacterFile.Header.Style &= ~CharacterStyle.CharStyleNoAutoPace;
				}
				else
				{
					mCharacterFile.Header.Style |= CharacterStyle.CharStyleNoAutoPace;
				}
				ShowBalloonStyle ();
				Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
			}
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
					mFileBalloon.Font = lFontDialog.Font;
					ShowBalloonFont ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		private void ButtonBalloonForeground_Click (object sender, EventArgs e)
		{
			if (!IsEmpty && !Program.MainForm.FileIsReadOnly)
			{
				ColorDialog	lColorDialog = new ColorDialog ();

				lColorDialog.Color = mFileBalloon.FgColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == DialogResult.OK)
				{
					mFileBalloon.FgColor = lColorDialog.Color;
					ShowBalloonColors ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
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
					mFileBalloon.BkColor = lColorDialog.Color;
					ShowBalloonColors ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
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
					mFileBalloon.BrColor = lColorDialog.Color;
					ShowBalloonColors ();
					Program.MainForm.FileIsDirty = mCharacterFile.IsDirty;
				}
			}
		}

		#endregion
		///////////////////////////////////////////////////////////////////////////////
	}
}
