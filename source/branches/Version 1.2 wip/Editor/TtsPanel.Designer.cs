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
	partial class TtsPanel
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
			this.LayoutPanelMain = new System.Windows.Forms.TableLayoutPanel();
			this.CheckBoxUseTTS = new System.Windows.Forms.CheckBox();
			this.GroupBoxTTS = new System.Windows.Forms.GroupBox();
			this.TextBoxGender = new System.Windows.Forms.TextBox();
			this.LabelGender = new System.Windows.Forms.Label();
			this.TextBoxLanguage = new System.Windows.Forms.TextBox();
			this.LabelLanguage = new System.Windows.Forms.Label();
			this.TextBoxVendor = new System.Windows.Forms.TextBox();
			this.LabelVendor = new System.Windows.Forms.Label();
			this.TextBoxTTSModeID = new System.Windows.Forms.TextBox();
			this.LabelModeID = new System.Windows.Forms.Label();
			this.ComboBoxName = new System.Windows.Forms.ComboBox();
			this.LabelName = new System.Windows.Forms.Label();
			this.LayoutPanelMain.SuspendLayout();
			this.GroupBoxTTS.SuspendLayout();
			this.SuspendLayout();
			// 
			// LayoutPanelMain
			// 
			this.LayoutPanelMain.ColumnCount = 1;
			this.LayoutPanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Controls.Add(this.CheckBoxUseTTS, 0, 0);
			this.LayoutPanelMain.Controls.Add(this.GroupBoxTTS, 0, 1);
			this.LayoutPanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.LayoutPanelMain.Location = new System.Drawing.Point(9, 0);
			this.LayoutPanelMain.Margin = new System.Windows.Forms.Padding(0);
			this.LayoutPanelMain.Name = "LayoutPanelMain";
			this.LayoutPanelMain.RowCount = 2;
			this.LayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Size = new System.Drawing.Size(582, 600);
			this.LayoutPanelMain.TabIndex = 0;
			// 
			// CheckBoxUseTTS
			// 
			this.CheckBoxUseTTS.AutoSize = true;
			this.CheckBoxUseTTS.Location = new System.Drawing.Point(9, 9);
			this.CheckBoxUseTTS.Margin = new System.Windows.Forms.Padding(9);
			this.CheckBoxUseTTS.Name = "CheckBoxUseTTS";
			this.CheckBoxUseTTS.Size = new System.Drawing.Size(258, 21);
			this.CheckBoxUseTTS.TabIndex = 0;
			this.CheckBoxUseTTS.Text = "Use Text-to-Speech for voice output";
			this.CheckBoxUseTTS.UseVisualStyleBackColor = true;
			this.CheckBoxUseTTS.CheckedChanged += new System.EventHandler(this.CheckBoxUseTTS_CheckedChanged);
			// 
			// GroupBoxTTS
			// 
			this.GroupBoxTTS.Controls.Add(this.TextBoxGender);
			this.GroupBoxTTS.Controls.Add(this.LabelGender);
			this.GroupBoxTTS.Controls.Add(this.TextBoxLanguage);
			this.GroupBoxTTS.Controls.Add(this.LabelLanguage);
			this.GroupBoxTTS.Controls.Add(this.TextBoxVendor);
			this.GroupBoxTTS.Controls.Add(this.LabelVendor);
			this.GroupBoxTTS.Controls.Add(this.TextBoxTTSModeID);
			this.GroupBoxTTS.Controls.Add(this.LabelModeID);
			this.GroupBoxTTS.Controls.Add(this.ComboBoxName);
			this.GroupBoxTTS.Controls.Add(this.LabelName);
			this.GroupBoxTTS.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxTTS.Location = new System.Drawing.Point(0, 48);
			this.GroupBoxTTS.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxTTS.MaximumSize = new System.Drawing.Size(730, 10000);
			this.GroupBoxTTS.Name = "GroupBoxTTS";
			this.GroupBoxTTS.Padding = new System.Windows.Forms.Padding(6, 9, 6, 6);
			this.GroupBoxTTS.Size = new System.Drawing.Size(582, 552);
			this.GroupBoxTTS.TabIndex = 1;
			this.GroupBoxTTS.TabStop = false;
			this.GroupBoxTTS.Text = "Voice Engine";
			// 
			// TextBoxGender
			// 
			this.TextBoxGender.BackColor = System.Drawing.SystemColors.Window;
			this.TextBoxGender.Location = new System.Drawing.Point(98, 55);
			this.TextBoxGender.Name = "TextBoxGender";
			this.TextBoxGender.ReadOnly = true;
			this.TextBoxGender.Size = new System.Drawing.Size(99, 22);
			this.TextBoxGender.TabIndex = 5;
			// 
			// LabelGender
			// 
			this.LabelGender.AutoSize = true;
			this.LabelGender.Location = new System.Drawing.Point(9, 58);
			this.LabelGender.Name = "LabelGender";
			this.LabelGender.Size = new System.Drawing.Size(56, 17);
			this.LabelGender.TabIndex = 4;
			this.LabelGender.Text = "Gender";
			// 
			// TextBoxLanguage
			// 
			this.TextBoxLanguage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxLanguage.BackColor = System.Drawing.SystemColors.Window;
			this.TextBoxLanguage.Location = new System.Drawing.Point(281, 55);
			this.TextBoxLanguage.Name = "TextBoxLanguage";
			this.TextBoxLanguage.ReadOnly = true;
			this.TextBoxLanguage.Size = new System.Drawing.Size(292, 22);
			this.TextBoxLanguage.TabIndex = 7;
			// 
			// LabelLanguage
			// 
			this.LabelLanguage.AutoSize = true;
			this.LabelLanguage.Location = new System.Drawing.Point(203, 58);
			this.LabelLanguage.Name = "LabelLanguage";
			this.LabelLanguage.Size = new System.Drawing.Size(72, 17);
			this.LabelLanguage.TabIndex = 6;
			this.LabelLanguage.Text = "Language";
			// 
			// TextBoxVendor
			// 
			this.TextBoxVendor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxVendor.BackColor = System.Drawing.SystemColors.Window;
			this.TextBoxVendor.Location = new System.Drawing.Point(98, 83);
			this.TextBoxVendor.Name = "TextBoxVendor";
			this.TextBoxVendor.ReadOnly = true;
			this.TextBoxVendor.Size = new System.Drawing.Size(475, 22);
			this.TextBoxVendor.TabIndex = 9;
			// 
			// LabelVendor
			// 
			this.LabelVendor.AutoSize = true;
			this.LabelVendor.Location = new System.Drawing.Point(9, 86);
			this.LabelVendor.Name = "LabelVendor";
			this.LabelVendor.Size = new System.Drawing.Size(54, 17);
			this.LabelVendor.TabIndex = 8;
			this.LabelVendor.Text = "Vendor";
			// 
			// TextBoxTTSModeID
			// 
			this.TextBoxTTSModeID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxTTSModeID.BackColor = System.Drawing.SystemColors.Window;
			this.TextBoxTTSModeID.Location = new System.Drawing.Point(98, 27);
			this.TextBoxTTSModeID.Name = "TextBoxTTSModeID";
			this.TextBoxTTSModeID.ReadOnly = true;
			this.TextBoxTTSModeID.Size = new System.Drawing.Size(475, 22);
			this.TextBoxTTSModeID.TabIndex = 3;
			// 
			// LabelModeID
			// 
			this.LabelModeID.AutoSize = true;
			this.LabelModeID.Location = new System.Drawing.Point(9, 30);
			this.LabelModeID.Name = "LabelModeID";
			this.LabelModeID.Size = new System.Drawing.Size(83, 17);
			this.LabelModeID.TabIndex = 2;
			this.LabelModeID.Text = "TTSModeID";
			// 
			// ComboBoxName
			// 
			this.ComboBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.ComboBoxName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ComboBoxName.FormattingEnabled = true;
			this.ComboBoxName.Location = new System.Drawing.Point(98, 119);
			this.ComboBoxName.Name = "ComboBoxName";
			this.ComboBoxName.Size = new System.Drawing.Size(475, 24);
			this.ComboBoxName.Sorted = true;
			this.ComboBoxName.TabIndex = 1;
			this.ComboBoxName.SelectionChangeCommitted += new System.EventHandler(this.ComboBoxName_SelectionChangeCommitted);
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.Location = new System.Drawing.Point(9, 122);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size(45, 17);
			this.LabelName.TabIndex = 0;
			this.LabelName.Text = "Name";
			// 
			// TtsPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.LayoutPanelMain);
			this.Name = "TtsPanel";
			this.Size = new System.Drawing.Size(600, 600);
			this.LayoutPanelMain.ResumeLayout(false);
			this.LayoutPanelMain.PerformLayout();
			this.GroupBoxTTS.ResumeLayout(false);
			this.GroupBoxTTS.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel LayoutPanelMain;
		private System.Windows.Forms.CheckBox CheckBoxUseTTS;
		private System.Windows.Forms.GroupBox GroupBoxTTS;
		private System.Windows.Forms.TextBox TextBoxTTSModeID;
		private System.Windows.Forms.Label LabelModeID;
		private System.Windows.Forms.ComboBox ComboBoxName;
		private System.Windows.Forms.Label LabelName;
		private System.Windows.Forms.TextBox TextBoxVendor;
		private System.Windows.Forms.Label LabelVendor;
		private System.Windows.Forms.TextBox TextBoxGender;
		private System.Windows.Forms.Label LabelGender;
		private System.Windows.Forms.TextBox TextBoxLanguage;
		private System.Windows.Forms.Label LabelLanguage;
	}
}
