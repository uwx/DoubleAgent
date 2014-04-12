/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
namespace AgentCharacterEditor.Panels
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
			this.PanelMain = new System.Windows.Forms.TableLayoutPanel();
			this.CheckBoxUseTTS = new DoubleAgent.CheckBoxCompat();
			this.GroupBoxTTS = new DoubleAgent.GroupBoxCompat();
			this.TextBoxGender = new DoubleAgent.TextBoxEx();
			this.LabelGender = new DoubleAgent.LabelCompat();
			this.TextBoxLanguage = new DoubleAgent.TextBoxEx();
			this.LabelLanguage = new DoubleAgent.LabelCompat();
			this.TextBoxVendor = new DoubleAgent.TextBoxEx();
			this.LabelVendor = new DoubleAgent.LabelCompat();
			this.TextBoxTTSModeID = new DoubleAgent.TextBoxEx();
			this.LabelModeID = new DoubleAgent.LabelCompat();
			this.ComboBoxName = new DoubleAgent.ComboBoxCompat();
			this.LabelName = new DoubleAgent.LabelCompat();
			this.PanelMain.SuspendLayout();
			this.GroupBoxTTS.SuspendLayout();
			this.SuspendLayout();
			// 
			// PanelMain
			// 
			this.PanelMain.ColumnCount = 1;
			this.PanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Controls.Add(this.CheckBoxUseTTS, 0, 0);
			this.PanelMain.Controls.Add(this.GroupBoxTTS, 0, 1);
			this.PanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelMain.Location = new System.Drawing.Point(9, 0);
			this.PanelMain.Margin = new System.Windows.Forms.Padding(0);
			this.PanelMain.Name = "PanelMain";
			this.PanelMain.RowCount = 2;
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Size = new System.Drawing.Size(582, 600);
			this.PanelMain.TabIndex = 0;
			// 
			// CheckBoxUseTTS
			// 
			this.CheckBoxUseTTS.AutoSize = true;
			this.CheckBoxUseTTS.IsChecked = false;
			this.CheckBoxUseTTS.IsEnabled = true;
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
			this.GroupBoxTTS.IsEnabled = true;
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
			this.TextBoxGender.IsEnabled = true;
			this.TextBoxGender.IsModified = false;
			this.TextBoxGender.IsReadOnly = true;
			this.TextBoxGender.Location = new System.Drawing.Point(98, 55);
			this.TextBoxGender.Name = "TextBoxGender";
			this.TextBoxGender.ReadOnly = true;
			this.TextBoxGender.Size = new System.Drawing.Size(100, 22);
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
			this.TextBoxLanguage.IsEnabled = true;
			this.TextBoxLanguage.IsModified = false;
			this.TextBoxLanguage.IsReadOnly = true;
			this.TextBoxLanguage.Location = new System.Drawing.Point(282, 55);
			this.TextBoxLanguage.Name = "TextBoxLanguage";
			this.TextBoxLanguage.ReadOnly = true;
			this.TextBoxLanguage.Size = new System.Drawing.Size(291, 22);
			this.TextBoxLanguage.TabIndex = 7;
			// 
			// LabelLanguage
			// 
			this.LabelLanguage.AutoSize = true;
			this.LabelLanguage.Location = new System.Drawing.Point(204, 58);
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
			this.TextBoxVendor.IsEnabled = true;
			this.TextBoxVendor.IsModified = false;
			this.TextBoxVendor.IsReadOnly = true;
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
			this.TextBoxTTSModeID.IsEnabled = true;
			this.TextBoxTTSModeID.IsModified = false;
			this.TextBoxTTSModeID.IsReadOnly = true;
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
			this.Controls.Add(this.PanelMain);
			this.Name = "TtsPanel";
			this.Size = new System.Drawing.Size(600, 600);
			this.PanelMain.ResumeLayout(false);
			this.PanelMain.PerformLayout();
			this.GroupBoxTTS.ResumeLayout(false);
			this.GroupBoxTTS.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel PanelMain;
		private DoubleAgent.CheckBoxCompat CheckBoxUseTTS;
		private DoubleAgent.GroupBoxCompat GroupBoxTTS;
		private DoubleAgent.TextBoxEx TextBoxTTSModeID;
		private DoubleAgent.LabelCompat LabelModeID;
		private DoubleAgent.ComboBoxCompat ComboBoxName;
		private DoubleAgent.LabelCompat LabelName;
		private DoubleAgent.TextBoxEx TextBoxVendor;
		private DoubleAgent.LabelCompat LabelVendor;
		private DoubleAgent.TextBoxEx TextBoxGender;
		private DoubleAgent.LabelCompat LabelGender;
		private DoubleAgent.TextBoxEx TextBoxLanguage;
		private DoubleAgent.LabelCompat LabelLanguage;
	}
}
