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
namespace AgentCharacterEditor
{
	partial class BranchingForm
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
			this.LabelFrameName = new System.Windows.Forms.Label ();
			this.TextBoxFrameName = new System.Windows.Forms.TextBox ();
			this.GroupBoxBranching = new System.Windows.Forms.GroupBox ();
			this.FlowLayoutNot = new System.Windows.Forms.FlowLayoutPanel ();
			this.NumericBranchingNot = new DoubleAgent.NumericUpDownEx ();
			this.LabelBranchingNot = new System.Windows.Forms.Label ();
			this.FlowLayout3 = new System.Windows.Forms.FlowLayoutPanel ();
			this.NumericBranching2 = new DoubleAgent.NumericUpDownEx ();
			this.LabelBranching3 = new System.Windows.Forms.Label ();
			this.NumericTarget2 = new DoubleAgent.NumericUpDownEx ();
			this.FlowLayout2 = new System.Windows.Forms.FlowLayoutPanel ();
			this.NumericBranching1 = new DoubleAgent.NumericUpDownEx ();
			this.LabelBranching2 = new System.Windows.Forms.Label ();
			this.NumericTarget1 = new DoubleAgent.NumericUpDownEx ();
			this.FlowLayout1 = new System.Windows.Forms.FlowLayoutPanel ();
			this.NumericBranching0 = new DoubleAgent.NumericUpDownEx ();
			this.LabelBranching1 = new System.Windows.Forms.Label ();
			this.NumericTarget0 = new DoubleAgent.NumericUpDownEx ();
			this.GroupBoxExit = new System.Windows.Forms.GroupBox ();
			this.FlowLayoutExit = new System.Windows.Forms.FlowLayoutPanel ();
			this.CheckBoxExit = new System.Windows.Forms.CheckBox ();
			this.NumericTargetExit = new DoubleAgent.NumericUpDownEx ();
			this.GroupBoxBranching.SuspendLayout ();
			this.FlowLayoutNot.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranchingNot)).BeginInit ();
			this.FlowLayout3.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranching2)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTarget2)).BeginInit ();
			this.FlowLayout2.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranching1)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTarget1)).BeginInit ();
			this.FlowLayout1.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranching0)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTarget0)).BeginInit ();
			this.GroupBoxExit.SuspendLayout ();
			this.FlowLayoutExit.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTargetExit)).BeginInit ();
			this.SuspendLayout ();
			// 
			// LabelFrameName
			// 
			this.LabelFrameName.AutoSize = true;
			this.LabelFrameName.Location = new System.Drawing.Point (9, 6);
			this.LabelFrameName.Name = "LabelFrameName";
			this.LabelFrameName.Size = new System.Drawing.Size (48, 17);
			this.LabelFrameName.TabIndex = 0;
			this.LabelFrameName.Text = "Frame";
			// 
			// TextBoxFrameName
			// 
			this.TextBoxFrameName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxFrameName.Location = new System.Drawing.Point (140, 3);
			this.TextBoxFrameName.MaximumSize = new System.Drawing.Size (600, 50);
			this.TextBoxFrameName.Name = "TextBoxFrameName";
			this.TextBoxFrameName.ReadOnly = true;
			this.TextBoxFrameName.Size = new System.Drawing.Size (448, 22);
			this.TextBoxFrameName.TabIndex = 1;
			// 
			// GroupBoxBranching
			// 
			this.GroupBoxBranching.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxBranching.Controls.Add (this.FlowLayoutNot);
			this.GroupBoxBranching.Controls.Add (this.FlowLayout3);
			this.GroupBoxBranching.Controls.Add (this.FlowLayout2);
			this.GroupBoxBranching.Controls.Add (this.FlowLayout1);
			this.GroupBoxBranching.Location = new System.Drawing.Point (9, 37);
			this.GroupBoxBranching.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxBranching.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxBranching.Name = "GroupBoxBranching";
			this.GroupBoxBranching.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxBranching.Size = new System.Drawing.Size (582, 160);
			this.GroupBoxBranching.TabIndex = 2;
			this.GroupBoxBranching.TabStop = false;
			this.GroupBoxBranching.Text = "Normal Branching";
			// 
			// FlowLayoutNot
			// 
			this.FlowLayoutNot.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayoutNot.Controls.Add (this.NumericBranchingNot);
			this.FlowLayoutNot.Controls.Add (this.LabelBranchingNot);
			this.FlowLayoutNot.Location = new System.Drawing.Point (9, 123);
			this.FlowLayoutNot.Margin = new System.Windows.Forms.Padding (0);
			this.FlowLayoutNot.Name = "FlowLayoutNot";
			this.FlowLayoutNot.Size = new System.Drawing.Size (567, 33);
			this.FlowLayoutNot.TabIndex = 3;
			this.FlowLayoutNot.WrapContents = false;
			// 
			// NumericBranchingNot
			// 
			this.NumericBranchingNot.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericBranchingNot.Enabled = false;
			this.NumericBranchingNot.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericBranchingNot.Highlighted = false;
			this.NumericBranchingNot.Location = new System.Drawing.Point (3, 3);
			this.NumericBranchingNot.Name = "NumericBranchingNot";
			this.NumericBranchingNot.Size = new System.Drawing.Size (60, 22);
			this.NumericBranchingNot.TabIndex = 0;
			this.NumericBranchingNot.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			// 
			// LabelBranchingNot
			// 
			this.LabelBranchingNot.AutoSize = true;
			this.LabelBranchingNot.Location = new System.Drawing.Point (69, 5);
			this.LabelBranchingNot.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelBranchingNot.Name = "LabelBranchingNot";
			this.LabelBranchingNot.Size = new System.Drawing.Size (177, 17);
			this.LabelBranchingNot.TabIndex = 1;
			this.LabelBranchingNot.Text = "% of the time, don\'t branch";
			// 
			// FlowLayout3
			// 
			this.FlowLayout3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayout3.Controls.Add (this.NumericBranching2);
			this.FlowLayout3.Controls.Add (this.LabelBranching3);
			this.FlowLayout3.Controls.Add (this.NumericTarget2);
			this.FlowLayout3.Location = new System.Drawing.Point (9, 90);
			this.FlowLayout3.Margin = new System.Windows.Forms.Padding (0);
			this.FlowLayout3.Name = "FlowLayout3";
			this.FlowLayout3.Size = new System.Drawing.Size (567, 33);
			this.FlowLayout3.TabIndex = 2;
			this.FlowLayout3.WrapContents = false;
			// 
			// NumericBranching2
			// 
			this.NumericBranching2.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericBranching2.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericBranching2.Highlighted = false;
			this.NumericBranching2.Location = new System.Drawing.Point (3, 3);
			this.NumericBranching2.Name = "NumericBranching2";
			this.NumericBranching2.Size = new System.Drawing.Size (60, 22);
			this.NumericBranching2.TabIndex = 0;
			this.NumericBranching2.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericBranching2.Validated += new System.EventHandler (this.NumericBranching2_Validated);
			// 
			// LabelBranching3
			// 
			this.LabelBranching3.AutoSize = true;
			this.LabelBranching3.Location = new System.Drawing.Point (69, 5);
			this.LabelBranching3.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelBranching3.Name = "LabelBranching3";
			this.LabelBranching3.Size = new System.Drawing.Size (198, 17);
			this.LabelBranching3.TabIndex = 1;
			this.LabelBranching3.Text = "% of the time, branch to frame";
			// 
			// NumericTarget2
			// 
			this.NumericTarget2.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericTarget2.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericTarget2.Highlighted = false;
			this.NumericTarget2.Location = new System.Drawing.Point (273, 3);
			this.NumericTarget2.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericTarget2.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTarget2.Name = "NumericTarget2";
			this.NumericTarget2.Size = new System.Drawing.Size (60, 22);
			this.NumericTarget2.TabIndex = 2;
			this.NumericTarget2.Value = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTarget2.Validated += new System.EventHandler (this.NumericTarget2_Validated);
			// 
			// FlowLayout2
			// 
			this.FlowLayout2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayout2.Controls.Add (this.NumericBranching1);
			this.FlowLayout2.Controls.Add (this.LabelBranching2);
			this.FlowLayout2.Controls.Add (this.NumericTarget1);
			this.FlowLayout2.Location = new System.Drawing.Point (9, 57);
			this.FlowLayout2.Margin = new System.Windows.Forms.Padding (0);
			this.FlowLayout2.Name = "FlowLayout2";
			this.FlowLayout2.Size = new System.Drawing.Size (567, 33);
			this.FlowLayout2.TabIndex = 1;
			this.FlowLayout2.WrapContents = false;
			// 
			// NumericBranching1
			// 
			this.NumericBranching1.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericBranching1.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericBranching1.Highlighted = false;
			this.NumericBranching1.Location = new System.Drawing.Point (3, 3);
			this.NumericBranching1.Name = "NumericBranching1";
			this.NumericBranching1.Size = new System.Drawing.Size (60, 22);
			this.NumericBranching1.TabIndex = 0;
			this.NumericBranching1.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericBranching1.Validated += new System.EventHandler (this.NumericBranching1_Validated);
			// 
			// LabelBranching2
			// 
			this.LabelBranching2.AutoSize = true;
			this.LabelBranching2.Location = new System.Drawing.Point (69, 5);
			this.LabelBranching2.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelBranching2.Name = "LabelBranching2";
			this.LabelBranching2.Size = new System.Drawing.Size (198, 17);
			this.LabelBranching2.TabIndex = 1;
			this.LabelBranching2.Text = "% of the time, branch to frame";
			// 
			// NumericTarget1
			// 
			this.NumericTarget1.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericTarget1.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericTarget1.Highlighted = false;
			this.NumericTarget1.Location = new System.Drawing.Point (273, 3);
			this.NumericTarget1.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericTarget1.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTarget1.Name = "NumericTarget1";
			this.NumericTarget1.Size = new System.Drawing.Size (60, 22);
			this.NumericTarget1.TabIndex = 2;
			this.NumericTarget1.Value = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTarget1.Validated += new System.EventHandler (this.NumericTarget1_Validated);
			// 
			// FlowLayout1
			// 
			this.FlowLayout1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayout1.Controls.Add (this.NumericBranching0);
			this.FlowLayout1.Controls.Add (this.LabelBranching1);
			this.FlowLayout1.Controls.Add (this.NumericTarget0);
			this.FlowLayout1.Location = new System.Drawing.Point (9, 24);
			this.FlowLayout1.Margin = new System.Windows.Forms.Padding (0);
			this.FlowLayout1.Name = "FlowLayout1";
			this.FlowLayout1.Size = new System.Drawing.Size (567, 33);
			this.FlowLayout1.TabIndex = 0;
			this.FlowLayout1.WrapContents = false;
			// 
			// NumericBranching0
			// 
			this.NumericBranching0.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericBranching0.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericBranching0.Highlighted = false;
			this.NumericBranching0.Location = new System.Drawing.Point (3, 3);
			this.NumericBranching0.Name = "NumericBranching0";
			this.NumericBranching0.Size = new System.Drawing.Size (60, 22);
			this.NumericBranching0.TabIndex = 0;
			this.NumericBranching0.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericBranching0.Validated += new System.EventHandler (this.NumericBranching0_Validated);
			// 
			// LabelBranching1
			// 
			this.LabelBranching1.AutoSize = true;
			this.LabelBranching1.Location = new System.Drawing.Point (69, 5);
			this.LabelBranching1.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelBranching1.Name = "LabelBranching1";
			this.LabelBranching1.Size = new System.Drawing.Size (198, 17);
			this.LabelBranching1.TabIndex = 1;
			this.LabelBranching1.Text = "% of the time, branch to frame";
			// 
			// NumericTarget0
			// 
			this.NumericTarget0.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericTarget0.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericTarget0.Highlighted = false;
			this.NumericTarget0.Location = new System.Drawing.Point (273, 3);
			this.NumericTarget0.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericTarget0.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTarget0.Name = "NumericTarget0";
			this.NumericTarget0.Size = new System.Drawing.Size (60, 22);
			this.NumericTarget0.TabIndex = 2;
			this.NumericTarget0.Value = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTarget0.Validated += new System.EventHandler (this.NumericTarget0_Validated);
			// 
			// GroupBoxExit
			// 
			this.GroupBoxExit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxExit.Controls.Add (this.FlowLayoutExit);
			this.GroupBoxExit.Location = new System.Drawing.Point (9, 206);
			this.GroupBoxExit.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxExit.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxExit.Name = "GroupBoxExit";
			this.GroupBoxExit.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxExit.Size = new System.Drawing.Size (582, 59);
			this.GroupBoxExit.TabIndex = 3;
			this.GroupBoxExit.TabStop = false;
			this.GroupBoxExit.Text = "Exit Branching";
			// 
			// FlowLayoutExit
			// 
			this.FlowLayoutExit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayoutExit.Controls.Add (this.CheckBoxExit);
			this.FlowLayoutExit.Controls.Add (this.NumericTargetExit);
			this.FlowLayoutExit.Location = new System.Drawing.Point (9, 24);
			this.FlowLayoutExit.Margin = new System.Windows.Forms.Padding (0);
			this.FlowLayoutExit.Name = "FlowLayoutExit";
			this.FlowLayoutExit.Size = new System.Drawing.Size (567, 30);
			this.FlowLayoutExit.TabIndex = 0;
			this.FlowLayoutExit.WrapContents = false;
			// 
			// CheckBoxExit
			// 
			this.CheckBoxExit.AutoSize = true;
			this.CheckBoxExit.Location = new System.Drawing.Point (3, 5);
			this.CheckBoxExit.Margin = new System.Windows.Forms.Padding (3, 5, 3, 3);
			this.CheckBoxExit.Name = "CheckBoxExit";
			this.CheckBoxExit.Size = new System.Drawing.Size (174, 21);
			this.CheckBoxExit.TabIndex = 0;
			this.CheckBoxExit.Text = "Exit after Stop to frame";
			this.CheckBoxExit.UseVisualStyleBackColor = true;
			this.CheckBoxExit.Click += new System.EventHandler (this.CheckBoxExit_Click);
			// 
			// NumericTargetExit
			// 
			this.NumericTargetExit.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericTargetExit.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericTargetExit.Highlighted = false;
			this.NumericTargetExit.Location = new System.Drawing.Point (183, 3);
			this.NumericTargetExit.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericTargetExit.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTargetExit.Name = "NumericTargetExit";
			this.NumericTargetExit.Size = new System.Drawing.Size (60, 22);
			this.NumericTargetExit.TabIndex = 1;
			this.NumericTargetExit.Value = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericTargetExit.Validated += new System.EventHandler (this.NumericTargetExit_Validated);
			// 
			// BranchingForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.GroupBoxExit);
			this.Controls.Add (this.GroupBoxBranching);
			this.Controls.Add (this.LabelFrameName);
			this.Controls.Add (this.TextBoxFrameName);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "BranchingForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (600, 600);
			this.GroupBoxBranching.ResumeLayout (false);
			this.FlowLayoutNot.ResumeLayout (false);
			this.FlowLayoutNot.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranchingNot)).EndInit ();
			this.FlowLayout3.ResumeLayout (false);
			this.FlowLayout3.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranching2)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTarget2)).EndInit ();
			this.FlowLayout2.ResumeLayout (false);
			this.FlowLayout2.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranching1)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTarget1)).EndInit ();
			this.FlowLayout1.ResumeLayout (false);
			this.FlowLayout1.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericBranching0)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTarget0)).EndInit ();
			this.GroupBoxExit.ResumeLayout (false);
			this.FlowLayoutExit.ResumeLayout (false);
			this.FlowLayoutExit.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericTargetExit)).EndInit ();
			this.ResumeLayout (false);
			this.PerformLayout ();

		}

		#endregion

		private System.Windows.Forms.Label LabelFrameName;
		private System.Windows.Forms.TextBox TextBoxFrameName;
		private System.Windows.Forms.GroupBox GroupBoxBranching;
		private System.Windows.Forms.GroupBox GroupBoxExit;
		private System.Windows.Forms.Label LabelBranchingNot;
		private System.Windows.Forms.Label LabelBranching3;
		private System.Windows.Forms.Label LabelBranching2;
		private System.Windows.Forms.Label LabelBranching1;
		private DoubleAgent.NumericUpDownEx NumericBranchingNot;
		private DoubleAgent.NumericUpDownEx NumericBranching2;
		private DoubleAgent.NumericUpDownEx NumericBranching1;
		private DoubleAgent.NumericUpDownEx NumericBranching0;
		private System.Windows.Forms.FlowLayoutPanel FlowLayout1;
		private DoubleAgent.NumericUpDownEx NumericTarget0;
		private DoubleAgent.NumericUpDownEx NumericTarget2;
		private DoubleAgent.NumericUpDownEx NumericTarget1;
		private System.Windows.Forms.FlowLayoutPanel FlowLayoutNot;
		private System.Windows.Forms.FlowLayoutPanel FlowLayout3;
		private System.Windows.Forms.FlowLayoutPanel FlowLayout2;
		private System.Windows.Forms.FlowLayoutPanel FlowLayoutExit;
		private System.Windows.Forms.CheckBox CheckBoxExit;
		private DoubleAgent.NumericUpDownEx NumericTargetExit;

	}
}
