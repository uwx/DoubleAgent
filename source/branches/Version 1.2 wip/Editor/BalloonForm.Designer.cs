namespace AgentCharacterEditor
{
	partial class BalloonForm
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose (bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose ();
			}
			base.Dispose (disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent ()
		{
			this.LayoutPanelMain = new System.Windows.Forms.TableLayoutPanel ();
			this.CheckBoxWordBalloon = new System.Windows.Forms.CheckBox ();
			this.GroupBoxBalloonDisplay = new System.Windows.Forms.GroupBox ();
			this.LabelBalloonBorderSample = new System.Windows.Forms.Label ();
			this.LabelBalloonBackgroundSample = new System.Windows.Forms.Label ();
			this.LabelBalloonFontSample = new System.Windows.Forms.Label ();
			this.LabelBalloonForegroundSample = new System.Windows.Forms.Label ();
			this.ButtonBalloonBorder = new System.Windows.Forms.Button ();
			this.CheckBoxAutoPace = new System.Windows.Forms.CheckBox ();
			this.ButtonBalloonBackground = new System.Windows.Forms.Button ();
			this.CheckBoxAutoHide = new System.Windows.Forms.CheckBox ();
			this.ButtonBalloonForeground = new System.Windows.Forms.Button ();
			this.NumericNumLines = new DoubleAgent.NumericUpDownEx ();
			this.ButtonBalloonFont = new System.Windows.Forms.Button ();
			this.RadioButtonSizeToText = new System.Windows.Forms.RadioButton ();
			this.RadioButtonNumLines = new System.Windows.Forms.RadioButton ();
			this.NumericCharsPerLine = new DoubleAgent.NumericUpDownEx ();
			this.LabelCharsPerLine = new System.Windows.Forms.Label ();
			this.GroupBoxBalloonSample = new System.Windows.Forms.GroupBox ();
			this.LayoutPanelMain.SuspendLayout ();
			this.GroupBoxBalloonDisplay.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericNumLines)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericCharsPerLine)).BeginInit ();
			this.SuspendLayout ();
			// 
			// LayoutPanelMain
			// 
			this.LayoutPanelMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.LayoutPanelMain.ColumnCount = 1;
			this.LayoutPanelMain.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Controls.Add (this.CheckBoxWordBalloon, 0, 0);
			this.LayoutPanelMain.Controls.Add (this.GroupBoxBalloonDisplay, 0, 1);
			this.LayoutPanelMain.Controls.Add (this.GroupBoxBalloonSample, 0, 2);
			this.LayoutPanelMain.Location = new System.Drawing.Point (9, 0);
			this.LayoutPanelMain.Margin = new System.Windows.Forms.Padding (0);
			this.LayoutPanelMain.Name = "LayoutPanelMain";
			this.LayoutPanelMain.RowCount = 3;
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Size = new System.Drawing.Size (582, 600);
			this.LayoutPanelMain.TabIndex = 0;
			// 
			// CheckBoxWordBalloon
			// 
			this.CheckBoxWordBalloon.AutoSize = true;
			this.CheckBoxWordBalloon.Location = new System.Drawing.Point (9, 9);
			this.CheckBoxWordBalloon.Margin = new System.Windows.Forms.Padding (9);
			this.CheckBoxWordBalloon.Name = "CheckBoxWordBalloon";
			this.CheckBoxWordBalloon.Size = new System.Drawing.Size (168, 21);
			this.CheckBoxWordBalloon.TabIndex = 0;
			this.CheckBoxWordBalloon.Text = "Use the Word Balloon";
			this.CheckBoxWordBalloon.UseVisualStyleBackColor = true;
			this.CheckBoxWordBalloon.CheckedChanged += new System.EventHandler (this.CheckBoxWordBalloon_CheckedChanged);
			// 
			// GroupBoxBalloonDisplay
			// 
			this.GroupBoxBalloonDisplay.Controls.Add (this.LabelBalloonBorderSample);
			this.GroupBoxBalloonDisplay.Controls.Add (this.LabelBalloonBackgroundSample);
			this.GroupBoxBalloonDisplay.Controls.Add (this.LabelBalloonFontSample);
			this.GroupBoxBalloonDisplay.Controls.Add (this.LabelBalloonForegroundSample);
			this.GroupBoxBalloonDisplay.Controls.Add (this.ButtonBalloonBorder);
			this.GroupBoxBalloonDisplay.Controls.Add (this.CheckBoxAutoPace);
			this.GroupBoxBalloonDisplay.Controls.Add (this.ButtonBalloonBackground);
			this.GroupBoxBalloonDisplay.Controls.Add (this.CheckBoxAutoHide);
			this.GroupBoxBalloonDisplay.Controls.Add (this.ButtonBalloonForeground);
			this.GroupBoxBalloonDisplay.Controls.Add (this.NumericNumLines);
			this.GroupBoxBalloonDisplay.Controls.Add (this.ButtonBalloonFont);
			this.GroupBoxBalloonDisplay.Controls.Add (this.RadioButtonSizeToText);
			this.GroupBoxBalloonDisplay.Controls.Add (this.RadioButtonNumLines);
			this.GroupBoxBalloonDisplay.Controls.Add (this.NumericCharsPerLine);
			this.GroupBoxBalloonDisplay.Controls.Add (this.LabelCharsPerLine);
			this.GroupBoxBalloonDisplay.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxBalloonDisplay.Location = new System.Drawing.Point (0, 48);
			this.GroupBoxBalloonDisplay.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxBalloonDisplay.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxBalloonDisplay.Name = "GroupBoxBalloonDisplay";
			this.GroupBoxBalloonDisplay.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxBalloonDisplay.Size = new System.Drawing.Size (582, 180);
			this.GroupBoxBalloonDisplay.TabIndex = 1;
			this.GroupBoxBalloonDisplay.TabStop = false;
			this.GroupBoxBalloonDisplay.Text = "Default DisplaySettings";
			// 
			// LabelBalloonBorderSample
			// 
			this.LabelBalloonBorderSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelBalloonBorderSample.Location = new System.Drawing.Point (429, 139);
			this.LabelBalloonBorderSample.Name = "LabelBalloonBorderSample";
			this.LabelBalloonBorderSample.Size = new System.Drawing.Size (74, 30);
			this.LabelBalloonBorderSample.TabIndex = 14;
			// 
			// LabelBalloonBackgroundSample
			// 
			this.LabelBalloonBackgroundSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelBalloonBackgroundSample.Location = new System.Drawing.Point (429, 101);
			this.LabelBalloonBackgroundSample.Name = "LabelBalloonBackgroundSample";
			this.LabelBalloonBackgroundSample.Size = new System.Drawing.Size (74, 30);
			this.LabelBalloonBackgroundSample.TabIndex = 12;
			// 
			// LabelBalloonFontSample
			// 
			this.LabelBalloonFontSample.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.LabelBalloonFontSample.AutoEllipsis = true;
			this.LabelBalloonFontSample.Location = new System.Drawing.Point (429, 22);
			this.LabelBalloonFontSample.Name = "LabelBalloonFontSample";
			this.LabelBalloonFontSample.Size = new System.Drawing.Size (144, 30);
			this.LabelBalloonFontSample.TabIndex = 8;
			this.LabelBalloonFontSample.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// LabelBalloonForegroundSample
			// 
			this.LabelBalloonForegroundSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelBalloonForegroundSample.Location = new System.Drawing.Point (429, 63);
			this.LabelBalloonForegroundSample.Name = "LabelBalloonForegroundSample";
			this.LabelBalloonForegroundSample.Size = new System.Drawing.Size (74, 30);
			this.LabelBalloonForegroundSample.TabIndex = 10;
			// 
			// ButtonBalloonBorder
			// 
			this.ButtonBalloonBorder.Location = new System.Drawing.Point (235, 139);
			this.ButtonBalloonBorder.Name = "ButtonBalloonBorder";
			this.ButtonBalloonBorder.Size = new System.Drawing.Size (188, 30);
			this.ButtonBalloonBorder.TabIndex = 13;
			this.ButtonBalloonBorder.Text = "Border color...";
			this.ButtonBalloonBorder.UseVisualStyleBackColor = true;
			this.ButtonBalloonBorder.Click += new System.EventHandler (this.ButtonBalloonBorder_Click);
			// 
			// CheckBoxAutoPace
			// 
			this.CheckBoxAutoPace.AutoSize = true;
			this.CheckBoxAutoPace.Location = new System.Drawing.Point (12, 144);
			this.CheckBoxAutoPace.Name = "CheckBoxAutoPace";
			this.CheckBoxAutoPace.Size = new System.Drawing.Size (95, 21);
			this.CheckBoxAutoPace.TabIndex = 6;
			this.CheckBoxAutoPace.Text = "Auto-pace";
			this.CheckBoxAutoPace.UseVisualStyleBackColor = true;
			this.CheckBoxAutoPace.CheckedChanged += new System.EventHandler (this.CheckBoxAutoPace_CheckedChanged);
			// 
			// ButtonBalloonBackground
			// 
			this.ButtonBalloonBackground.Location = new System.Drawing.Point (235, 101);
			this.ButtonBalloonBackground.Name = "ButtonBalloonBackground";
			this.ButtonBalloonBackground.Size = new System.Drawing.Size (188, 30);
			this.ButtonBalloonBackground.TabIndex = 11;
			this.ButtonBalloonBackground.Text = "Background color...";
			this.ButtonBalloonBackground.UseVisualStyleBackColor = true;
			this.ButtonBalloonBackground.Click += new System.EventHandler (this.ButtonBalloonBackground_Click);
			// 
			// CheckBoxAutoHide
			// 
			this.CheckBoxAutoHide.AutoSize = true;
			this.CheckBoxAutoHide.Location = new System.Drawing.Point (12, 117);
			this.CheckBoxAutoHide.Name = "CheckBoxAutoHide";
			this.CheckBoxAutoHide.Size = new System.Drawing.Size (91, 21);
			this.CheckBoxAutoHide.TabIndex = 5;
			this.CheckBoxAutoHide.Text = "Auto-hide";
			this.CheckBoxAutoHide.UseVisualStyleBackColor = true;
			this.CheckBoxAutoHide.CheckedChanged += new System.EventHandler (this.CheckBoxAutoHide_CheckedChanged);
			// 
			// ButtonBalloonForeground
			// 
			this.ButtonBalloonForeground.Location = new System.Drawing.Point (235, 63);
			this.ButtonBalloonForeground.Name = "ButtonBalloonForeground";
			this.ButtonBalloonForeground.Size = new System.Drawing.Size (188, 30);
			this.ButtonBalloonForeground.TabIndex = 9;
			this.ButtonBalloonForeground.Text = "Foreground color...";
			this.ButtonBalloonForeground.UseVisualStyleBackColor = true;
			this.ButtonBalloonForeground.Click += new System.EventHandler (this.ButtonBalloonForeground_Click);
			// 
			// NumericNumLines
			// 
			this.NumericNumLines.Location = new System.Drawing.Point (140, 58);
			this.NumericNumLines.Maximum = new decimal (new int[] {
            128,
            0,
            0,
            0});
			this.NumericNumLines.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericNumLines.Name = "NumericNumLines";
			this.NumericNumLines.Size = new System.Drawing.Size (57, 22);
			this.NumericNumLines.TabIndex = 3;
			this.NumericNumLines.Value = new decimal (new int[] {
            2,
            0,
            0,
            0});
			this.NumericNumLines.Validated += new System.EventHandler (this.NumericNumLines_Validated);
			// 
			// ButtonBalloonFont
			// 
			this.ButtonBalloonFont.Location = new System.Drawing.Point (235, 25);
			this.ButtonBalloonFont.Name = "ButtonBalloonFont";
			this.ButtonBalloonFont.Size = new System.Drawing.Size (188, 30);
			this.ButtonBalloonFont.TabIndex = 7;
			this.ButtonBalloonFont.Text = "Font...";
			this.ButtonBalloonFont.UseVisualStyleBackColor = true;
			this.ButtonBalloonFont.Click += new System.EventHandler (this.ButtonBalloonFont_Click);
			// 
			// RadioButtonSizeToText
			// 
			this.RadioButtonSizeToText.AutoSize = true;
			this.RadioButtonSizeToText.Location = new System.Drawing.Point (12, 85);
			this.RadioButtonSizeToText.Name = "RadioButtonSizeToText";
			this.RadioButtonSizeToText.Size = new System.Drawing.Size (98, 21);
			this.RadioButtonSizeToText.TabIndex = 4;
			this.RadioButtonSizeToText.TabStop = true;
			this.RadioButtonSizeToText.Text = "Size to text";
			this.RadioButtonSizeToText.UseVisualStyleBackColor = true;
			this.RadioButtonSizeToText.CheckedChanged += new System.EventHandler (this.RadioButtonSizeToText_CheckedChanged);
			// 
			// RadioButtonNumLines
			// 
			this.RadioButtonNumLines.AutoSize = true;
			this.RadioButtonNumLines.Location = new System.Drawing.Point (12, 58);
			this.RadioButtonNumLines.Name = "RadioButtonNumLines";
			this.RadioButtonNumLines.Size = new System.Drawing.Size (128, 21);
			this.RadioButtonNumLines.TabIndex = 2;
			this.RadioButtonNumLines.TabStop = true;
			this.RadioButtonNumLines.Text = "Number of lines";
			this.RadioButtonNumLines.UseVisualStyleBackColor = true;
			this.RadioButtonNumLines.CheckedChanged += new System.EventHandler (this.RadioButtonNumLines_CheckedChanged);
			// 
			// NumericCharsPerLine
			// 
			this.NumericCharsPerLine.Location = new System.Drawing.Point (140, 26);
			this.NumericCharsPerLine.Maximum = new decimal (new int[] {
            255,
            0,
            0,
            0});
			this.NumericCharsPerLine.Minimum = new decimal (new int[] {
            8,
            0,
            0,
            0});
			this.NumericCharsPerLine.Name = "NumericCharsPerLine";
			this.NumericCharsPerLine.Size = new System.Drawing.Size (57, 22);
			this.NumericCharsPerLine.TabIndex = 1;
			this.NumericCharsPerLine.Value = new decimal (new int[] {
            8,
            0,
            0,
            0});
			this.NumericCharsPerLine.Validated += new System.EventHandler (this.NumericCharsPerLine_Validated);
			// 
			// LabelCharsPerLine
			// 
			this.LabelCharsPerLine.AutoSize = true;
			this.LabelCharsPerLine.Location = new System.Drawing.Point (9, 29);
			this.LabelCharsPerLine.Name = "LabelCharsPerLine";
			this.LabelCharsPerLine.Size = new System.Drawing.Size (128, 17);
			this.LabelCharsPerLine.TabIndex = 0;
			this.LabelCharsPerLine.Text = "Characters per line";
			// 
			// GroupBoxBalloonSample
			// 
			this.GroupBoxBalloonSample.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxBalloonSample.Enabled = false;
			this.GroupBoxBalloonSample.Location = new System.Drawing.Point (0, 237);
			this.GroupBoxBalloonSample.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxBalloonSample.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxBalloonSample.Name = "GroupBoxBalloonSample";
			this.GroupBoxBalloonSample.Padding = new System.Windows.Forms.Padding (6, 0, 6, 6);
			this.GroupBoxBalloonSample.Size = new System.Drawing.Size (582, 363);
			this.GroupBoxBalloonSample.TabIndex = 2;
			this.GroupBoxBalloonSample.TabStop = false;
			this.GroupBoxBalloonSample.Text = "Sample";
			// 
			// BalloonForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.LayoutPanelMain);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "BalloonForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (600, 600);
			this.LayoutPanelMain.ResumeLayout (false);
			this.LayoutPanelMain.PerformLayout ();
			this.GroupBoxBalloonDisplay.ResumeLayout (false);
			this.GroupBoxBalloonDisplay.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericNumLines)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericCharsPerLine)).EndInit ();
			this.ResumeLayout (false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel LayoutPanelMain;
		private System.Windows.Forms.CheckBox CheckBoxWordBalloon;
		private System.Windows.Forms.GroupBox GroupBoxBalloonDisplay;
		private System.Windows.Forms.RadioButton RadioButtonSizeToText;
		private System.Windows.Forms.RadioButton RadioButtonNumLines;
		private DoubleAgent.NumericUpDownEx NumericCharsPerLine;
		private System.Windows.Forms.Label LabelCharsPerLine;
		private System.Windows.Forms.GroupBox GroupBoxBalloonSample;
		private DoubleAgent.NumericUpDownEx NumericNumLines;
		private System.Windows.Forms.CheckBox CheckBoxAutoPace;
		private System.Windows.Forms.CheckBox CheckBoxAutoHide;
		private System.Windows.Forms.Button ButtonBalloonBorder;
		private System.Windows.Forms.Button ButtonBalloonBackground;
		private System.Windows.Forms.Button ButtonBalloonForeground;
		private System.Windows.Forms.Button ButtonBalloonFont;
		private System.Windows.Forms.Label LabelBalloonBorderSample;
		private System.Windows.Forms.Label LabelBalloonBackgroundSample;
		private System.Windows.Forms.Label LabelBalloonForegroundSample;
		private System.Windows.Forms.Label LabelBalloonFontSample;
	}
}
