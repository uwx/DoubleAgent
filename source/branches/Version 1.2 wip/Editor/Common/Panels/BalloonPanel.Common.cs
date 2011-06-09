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
using AgentCharacterEditor.Navigation;
using AgentCharacterEditor.Updates;
using DoubleAgent;
using DoubleAgent.Character;

namespace AgentCharacterEditor.Panels
{
	public partial class BalloonPanel
	{
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

		public override Boolean IsPanelEmpty
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
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				CheckBoxWordBalloon.IsEnabled = (CharacterFile != null) && (!Program.FileIsReadOnly);
				GroupBoxBalloonDisplay.IsEnabled = !IsPanelEmpty;

				if (IsPanelEmpty)
				{
					CheckBoxWordBalloon.IsChecked = false;
				}
				else
				{
					CheckBoxWordBalloon.IsChecked = ((CharacterFile.Header.Style & CharacterStyle.Balloon) != CharacterStyle.None);
				}

				ShowBalloonStyle ();
				ShowBalloonColors ();
				ShowBalloonFont ();
				ShowBalloonPreview ();
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		private void ShowBalloonStyle ()
		{
			using (PanelFillingState lFillingState = new PanelFillingState (this))
			{
				if (IsPanelEmpty)
				{
					NumericCharsPerLine.Clear ();
					NumericNumLines.Clear ();
					RadioButtonSizeToText.IsChecked = false;
					RadioButtonNumLines.IsChecked = false;
					CheckBoxAutoHide.IsChecked = false;
					CheckBoxAutoPace.IsChecked = false;
				}
				else
				{
					NumericCharsPerLine.Value = FileBalloon.PerLine;
					NumericNumLines.Value = FileBalloon.Lines;

					if ((CharacterFile.Header.Style & CharacterStyle.SizeToText) != CharacterStyle.None)
					{
						RadioButtonSizeToText.IsChecked = true;
						RadioButtonNumLines.IsChecked = false;
						NumericNumLines.IsEnabled = false;
					}
					else
					{
						RadioButtonSizeToText.IsChecked = false;
						RadioButtonNumLines.IsChecked = true;
						NumericNumLines.IsEnabled = !Program.FileIsReadOnly;
					}

					CheckBoxAutoHide.IsChecked = ((CharacterFile.Header.Style & CharacterStyle.NoAutoHide) == CharacterStyle.None);
					CheckBoxAutoPace.IsChecked = ((CharacterFile.Header.Style & CharacterStyle.NoAutoPace) == CharacterStyle.None);
				}

				LabelCharsPerLine.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				NumericCharsPerLine.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				RadioButtonSizeToText.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				RadioButtonNumLines.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				CheckBoxAutoHide.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;
				CheckBoxAutoPace.IsEnabled = !IsPanelEmpty && !Program.FileIsReadOnly;

				NumericCharsPerLine.IsModified = false;
				NumericNumLines.IsModified = false;
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected String GetFontFamilyName (System.Drawing.Font pFont)
		{
			return String.IsNullOrEmpty (pFont.OriginalFontName) ? pFont.Name : pFont.OriginalFontName;
		}

		protected String FormatFontName (System.Drawing.Font pFont)
		{
			String lText;
			String lFontName = GetFontFamilyName (pFont);
			int lFontSize = (int)(pFont.SizeInPoints + 0.4999);

			lText = lFontName + " " + lFontSize.ToString ();
			if (pFont.Bold)
			{
				lText += " Bold";
			}
			if (pFont.Italic)
			{
				lText += " Italic";
			}

			if (((pFont.OriginalFontName != null) && (pFont.Name != pFont.OriginalFontName)) || (pFont.SizeInPoints != (float)lFontSize))
			{
				lText += " (" + pFont.Name + " " + pFont.SizeInPoints.ToString () + ")";
			}
			return lText;
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

		///////////////////////////////////////////////////////////////////////////////

		protected void HandleEnabledChanged ()
		{
			if (!IsPanelFilling && (CharacterFile != null) && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				if (CheckBoxWordBalloon.IsChecked.Value)
				{
					lUpdate.CharacterStyle |= CharacterStyle.Balloon;
				}
				else
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.Balloon;
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		protected void HandleCharsPerLineChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				lUpdate.PerLine = (UInt16)NumericCharsPerLine.Value;
				if (NumericCharsPerLine.Value != (Decimal)lUpdate.PerLine)
				{
					using (PanelFillingState lFillingState = new PanelFillingState (this))
					{
						NumericCharsPerLine.Value = (Decimal)lUpdate.PerLine;
						NumericCharsPerLine.IsModified = false;
					}
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
			NumericCharsPerLine.IsModified = false;
		}

		protected void HandleNumLinesChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				lUpdate.Lines = (UInt16)NumericNumLines.Value;
				if (NumericNumLines.Value != (Decimal)lUpdate.Lines)
				{
					using (PanelFillingState lFillingState = new PanelFillingState (this))
					{
						NumericNumLines.Value = (Decimal)lUpdate.Lines;
						NumericNumLines.IsModified = false;
					}
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
			NumericNumLines.IsModified = false;
		}

		protected void HandleSizeToTextChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				if (RadioButtonSizeToText.IsChecked.Value)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.CharacterStyle |= CharacterStyle.SizeToText;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
				else if (RadioButtonNumLines.IsChecked.Value)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.CharacterStyle &= ~CharacterStyle.SizeToText;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		protected void HandleAutoHideChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				if (CheckBoxAutoHide.IsChecked.Value)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.NoAutoHide;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.NoAutoHide;
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		protected void HandleAutoPaceChanged ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

				if (CheckBoxAutoPace.IsChecked.Value)
				{
					lUpdate.CharacterStyle &= ~CharacterStyle.NoAutoPace;
				}
				else
				{
					lUpdate.CharacterStyle |= CharacterStyle.NoAutoPace;
				}
				UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
			}
		}

		///////////////////////////////////////////////////////////////////////////////

		protected void HandleUpdateFont ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				System.Windows.Forms.FontDialog lFontDialog = new System.Windows.Forms.FontDialog ();

				lFontDialog.ShowEffects = true;
				lFontDialog.Font = FileBalloon.Font;
				if (lFontDialog.ShowDialog () == System.Windows.Forms.DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.Font = lFontDialog.Font;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		protected void HandleUpdateForeground ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				System.Windows.Forms.ColorDialog lColorDialog = new System.Windows.Forms.ColorDialog ();

				lColorDialog.Color = FileBalloon.FgColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == System.Windows.Forms.DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.FgColor = lColorDialog.Color;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		protected void HandleUpdateBackground ()
		{
			if (!IsPanelFilling && (!IsPanelEmpty) && (!Program.FileIsReadOnly))
			{
				System.Windows.Forms.ColorDialog lColorDialog = new System.Windows.Forms.ColorDialog ();

				lColorDialog.Color = FileBalloon.BkColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == System.Windows.Forms.DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.BkColor = lColorDialog.Color;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		protected void HandleUpdateBorder ()
		{
			if (!IsPanelFilling && !IsPanelEmpty && !Program.FileIsReadOnly)
			{
				System.Windows.Forms.ColorDialog lColorDialog = new System.Windows.Forms.ColorDialog ();

				lColorDialog.Color = FileBalloon.BrColor;
				lColorDialog.FullOpen = true;
				if (lColorDialog.ShowDialog () == System.Windows.Forms.DialogResult.OK)
				{
					UpdateCharacterBalloon lUpdate = new UpdateCharacterBalloon ();

					lUpdate.BrColor = lColorDialog.Color;
					UpdateCharacterBalloon.PutUndo (lUpdate.Apply (Program.MainWindow.OnUpdateApplied) as UpdateCharacterBalloon, this);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////

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
				else if (!IsPanelEmpty)
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
				if (!IsPanelEmpty && lUpdateCharacterTts.EnabledChanged)
				{
					ShowBalloonPreview ();
				}
			}
		}

		#endregion
	}
}
