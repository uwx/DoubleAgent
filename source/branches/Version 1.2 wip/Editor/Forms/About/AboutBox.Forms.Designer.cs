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
namespace AgentCharacterEditor.About
{
	partial class AboutBox
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose (bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose ();
			}
			base.Dispose (disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent ()
		{
			System.Windows.Forms.Panel BorderPanel;
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutBox));
			this.TextBoxLicenseIntro = new System.Windows.Forms.RichTextBox();
			this.LogoPictureBox = new System.Windows.Forms.PictureBox();
			this.LayoutPanel = new System.Windows.Forms.TableLayoutPanel();
			this.LabelProductName = new System.Windows.Forms.Label();
			this.LabelVersion = new System.Windows.Forms.Label();
			this.LabelCopyright = new System.Windows.Forms.Label();
			this.OkButton = new System.Windows.Forms.Button();
			this.LicenseButton = new System.Windows.Forms.Button();
			BorderPanel = new System.Windows.Forms.Panel();
			BorderPanel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.LogoPictureBox)).BeginInit();
			this.LayoutPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// BorderPanel
			// 
			BorderPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LayoutPanel.SetColumnSpan(BorderPanel, 3);
			BorderPanel.Controls.Add(this.TextBoxLicenseIntro);
			BorderPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			BorderPanel.Location = new System.Drawing.Point(4, 90);
			BorderPanel.Margin = new System.Windows.Forms.Padding(4, 16, 4, 16);
			BorderPanel.Name = "BorderPanel";
			BorderPanel.Padding = new System.Windows.Forms.Padding(4);
			BorderPanel.Size = new System.Drawing.Size(397, 196);
			BorderPanel.TabIndex = 3;
			// 
			// TextBoxLicenseIntro
			// 
			this.TextBoxLicenseIntro.BackColor = System.Drawing.SystemColors.Control;
			this.TextBoxLicenseIntro.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.TextBoxLicenseIntro.CausesValidation = false;
			this.TextBoxLicenseIntro.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TextBoxLicenseIntro.Location = new System.Drawing.Point(4, 4);
			this.TextBoxLicenseIntro.Margin = new System.Windows.Forms.Padding(0);
			this.TextBoxLicenseIntro.Name = "TextBoxLicenseIntro";
			this.TextBoxLicenseIntro.ReadOnly = true;
			this.TextBoxLicenseIntro.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
			this.TextBoxLicenseIntro.Size = new System.Drawing.Size(387, 186);
			this.TextBoxLicenseIntro.TabIndex = 0;
			this.TextBoxLicenseIntro.TabStop = false;
			this.TextBoxLicenseIntro.Text = "";
			this.TextBoxLicenseIntro.LinkClicked += new System.Windows.Forms.LinkClickedEventHandler(this.TextBoxLicenseIntro_LinkClicked);
			// 
			// LogoPictureBox
			// 
			this.LogoPictureBox.ErrorImage = null;
			this.LogoPictureBox.Image = ((System.Drawing.Image)(resources.GetObject("LogoPictureBox.Image")));
			this.LogoPictureBox.InitialImage = null;
			this.LogoPictureBox.Location = new System.Drawing.Point(4, 4);
			this.LogoPictureBox.Margin = new System.Windows.Forms.Padding(4);
			this.LogoPictureBox.Name = "LogoPictureBox";
			this.LayoutPanel.SetRowSpan(this.LogoPictureBox, 4);
			this.LogoPictureBox.Size = new System.Drawing.Size(64, 64);
			this.LogoPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.LogoPictureBox.TabIndex = 12;
			this.LogoPictureBox.TabStop = false;
			// 
			// LayoutPanel
			// 
			this.LayoutPanel.ColumnCount = 3;
			this.LayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.LayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.LayoutPanel.Controls.Add(this.LogoPictureBox, 0, 0);
			this.LayoutPanel.Controls.Add(this.LabelProductName, 1, 0);
			this.LayoutPanel.Controls.Add(this.LabelVersion, 1, 1);
			this.LayoutPanel.Controls.Add(this.LabelCopyright, 1, 2);
			this.LayoutPanel.Controls.Add(this.OkButton, 2, 5);
			this.LayoutPanel.Controls.Add(this.LicenseButton, 0, 5);
			this.LayoutPanel.Controls.Add(BorderPanel, 0, 4);
			this.LayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanel.Location = new System.Drawing.Point(12, 11);
			this.LayoutPanel.Margin = new System.Windows.Forms.Padding(4);
			this.LayoutPanel.Name = "LayoutPanel";
			this.LayoutPanel.RowCount = 6;
			this.LayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanel.Size = new System.Drawing.Size(405, 337);
			this.LayoutPanel.TabIndex = 0;
			// 
			// LabelProductName
			// 
			this.LayoutPanel.SetColumnSpan(this.LabelProductName, 2);
			this.LabelProductName.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelProductName.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.LabelProductName.Location = new System.Drawing.Point(80, 0);
			this.LabelProductName.Margin = new System.Windows.Forms.Padding(8, 0, 4, 0);
			this.LabelProductName.Name = "LabelProductName";
			this.LabelProductName.Size = new System.Drawing.Size(321, 32);
			this.LabelProductName.TabIndex = 0;
			this.LabelProductName.Text = "Double Agent Character Editor";
			// 
			// LabelVersion
			// 
			this.LayoutPanel.SetColumnSpan(this.LabelVersion, 2);
			this.LabelVersion.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelVersion.Location = new System.Drawing.Point(80, 32);
			this.LabelVersion.Margin = new System.Windows.Forms.Padding(8, 0, 4, 0);
			this.LabelVersion.MaximumSize = new System.Drawing.Size(0, 21);
			this.LabelVersion.Name = "LabelVersion";
			this.LabelVersion.Size = new System.Drawing.Size(321, 21);
			this.LabelVersion.TabIndex = 1;
			this.LabelVersion.Text = "Version";
			this.LabelVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// LabelCopyright
			// 
			this.LayoutPanel.SetColumnSpan(this.LabelCopyright, 2);
			this.LabelCopyright.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelCopyright.Location = new System.Drawing.Point(80, 53);
			this.LabelCopyright.Margin = new System.Windows.Forms.Padding(8, 0, 4, 0);
			this.LabelCopyright.MaximumSize = new System.Drawing.Size(0, 21);
			this.LabelCopyright.Name = "LabelCopyright";
			this.LabelCopyright.Size = new System.Drawing.Size(321, 21);
			this.LabelCopyright.TabIndex = 2;
			this.LabelCopyright.Text = "Copyright";
			this.LabelCopyright.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// OkButton
			// 
			this.OkButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.OkButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.OkButton.Location = new System.Drawing.Point(301, 306);
			this.OkButton.Margin = new System.Windows.Forms.Padding(4);
			this.OkButton.Name = "OkButton";
			this.OkButton.Size = new System.Drawing.Size(100, 27);
			this.OkButton.TabIndex = 0;
			this.OkButton.Text = "&OK";
			// 
			// LicenseButton
			// 
			this.LicenseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.LayoutPanel.SetColumnSpan(this.LicenseButton, 2);
			this.LicenseButton.Location = new System.Drawing.Point(4, 306);
			this.LicenseButton.Margin = new System.Windows.Forms.Padding(4);
			this.LicenseButton.Name = "LicenseButton";
			this.LicenseButton.Size = new System.Drawing.Size(200, 27);
			this.LicenseButton.TabIndex = 4;
			this.LicenseButton.Text = "GNU Public License";
			this.LicenseButton.UseVisualStyleBackColor = true;
			this.LicenseButton.Click += new System.EventHandler(this.LicenseButton_Click);
			// 
			// AboutBox
			// 
			this.AcceptButton = this.OkButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(429, 359);
			this.Controls.Add(this.LayoutPanel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AboutBox";
			this.Padding = new System.Windows.Forms.Padding(12, 11, 12, 11);
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "About";
			BorderPanel.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.LogoPictureBox)).EndInit();
			this.LayoutPanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel LayoutPanel;
		private System.Windows.Forms.Label LabelProductName;
		private System.Windows.Forms.Label LabelVersion;
		private System.Windows.Forms.Label LabelCopyright;
		private System.Windows.Forms.Button OkButton;
		private System.Windows.Forms.PictureBox LogoPictureBox;
		private System.Windows.Forms.RichTextBox TextBoxLicenseIntro;
		private System.Windows.Forms.Button LicenseButton;
	}
}
