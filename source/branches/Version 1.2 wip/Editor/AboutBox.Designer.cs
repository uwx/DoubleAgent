﻿/////////////////////////////////////////////////////////////////////////////
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (AboutBox));
			this.LogoPictureBox = new System.Windows.Forms.PictureBox ();
			this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel ();
			this.LabelProductName = new System.Windows.Forms.Label ();
			this.LabelVersion = new System.Windows.Forms.Label ();
			this.LabelCopyright = new System.Windows.Forms.Label ();
			this.LabelCompanyName = new System.Windows.Forms.Label ();
			this.TextBoxDescription = new System.Windows.Forms.TextBox ();
			this.okButton = new System.Windows.Forms.Button ();
			((System.ComponentModel.ISupportInitialize)(this.LogoPictureBox)).BeginInit ();
			this.tableLayoutPanel.SuspendLayout ();
			this.SuspendLayout ();
			// 
			// LogoPictureBox
			// 
			this.LogoPictureBox.ErrorImage = null;
			this.LogoPictureBox.Image = ((System.Drawing.Image)(resources.GetObject ("LogoPictureBox.Image")));
			this.LogoPictureBox.InitialImage = null;
			this.LogoPictureBox.Location = new System.Drawing.Point (4, 4);
			this.LogoPictureBox.Margin = new System.Windows.Forms.Padding (4);
			this.LogoPictureBox.Name = "LogoPictureBox";
			this.tableLayoutPanel.SetRowSpan (this.LogoPictureBox, 4);
			this.LogoPictureBox.Size = new System.Drawing.Size (64, 64);
			this.LogoPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.LogoPictureBox.TabIndex = 12;
			this.LogoPictureBox.TabStop = false;
			// 
			// tableLayoutPanel
			// 
			this.tableLayoutPanel.ColumnCount = 2;
			this.tableLayoutPanel.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle ());
			this.tableLayoutPanel.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel.Controls.Add (this.LogoPictureBox, 0, 0);
			this.tableLayoutPanel.Controls.Add (this.LabelProductName, 1, 0);
			this.tableLayoutPanel.Controls.Add (this.LabelVersion, 1, 1);
			this.tableLayoutPanel.Controls.Add (this.LabelCopyright, 1, 2);
			this.tableLayoutPanel.Controls.Add (this.LabelCompanyName, 1, 3);
			this.tableLayoutPanel.Controls.Add (this.TextBoxDescription, 0, 4);
			this.tableLayoutPanel.Controls.Add (this.okButton, 1, 5);
			this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel.Location = new System.Drawing.Point (12, 11);
			this.tableLayoutPanel.Margin = new System.Windows.Forms.Padding (4);
			this.tableLayoutPanel.Name = "tableLayoutPanel";
			this.tableLayoutPanel.RowCount = 6;
			this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.tableLayoutPanel.Size = new System.Drawing.Size (450, 325);
			this.tableLayoutPanel.TabIndex = 0;
			// 
			// LabelProductName
			// 
			this.LabelProductName.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelProductName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.LabelProductName.Location = new System.Drawing.Point (80, 0);
			this.LabelProductName.Margin = new System.Windows.Forms.Padding (8, 0, 4, 0);
			this.LabelProductName.Name = "LabelProductName";
			this.LabelProductName.Size = new System.Drawing.Size (366, 32);
			this.LabelProductName.TabIndex = 0;
			this.LabelProductName.Text = "Double Agent Character Editor";
			// 
			// LabelVersion
			// 
			this.LabelVersion.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelVersion.Location = new System.Drawing.Point (80, 32);
			this.LabelVersion.Margin = new System.Windows.Forms.Padding (8, 0, 4, 0);
			this.LabelVersion.MaximumSize = new System.Drawing.Size (0, 21);
			this.LabelVersion.Name = "LabelVersion";
			this.LabelVersion.Size = new System.Drawing.Size (366, 21);
			this.LabelVersion.TabIndex = 1;
			this.LabelVersion.Text = "Version";
			this.LabelVersion.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// LabelCopyright
			// 
			this.LabelCopyright.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelCopyright.Location = new System.Drawing.Point (80, 53);
			this.LabelCopyright.Margin = new System.Windows.Forms.Padding (8, 0, 4, 0);
			this.LabelCopyright.MaximumSize = new System.Drawing.Size (0, 21);
			this.LabelCopyright.Name = "LabelCopyright";
			this.LabelCopyright.Size = new System.Drawing.Size (366, 21);
			this.LabelCopyright.TabIndex = 2;
			this.LabelCopyright.Text = "Copyright";
			this.LabelCopyright.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// LabelCompanyName
			// 
			this.LabelCompanyName.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LabelCompanyName.Location = new System.Drawing.Point (80, 74);
			this.LabelCompanyName.Margin = new System.Windows.Forms.Padding (8, 0, 4, 0);
			this.LabelCompanyName.MaximumSize = new System.Drawing.Size (0, 21);
			this.LabelCompanyName.Name = "LabelCompanyName";
			this.LabelCompanyName.Size = new System.Drawing.Size (366, 21);
			this.LabelCompanyName.TabIndex = 3;
			this.LabelCompanyName.Text = "Company Name";
			this.LabelCompanyName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// TextBoxDescription
			// 
			this.tableLayoutPanel.SetColumnSpan (this.TextBoxDescription, 2);
			this.TextBoxDescription.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TextBoxDescription.Location = new System.Drawing.Point (8, 99);
			this.TextBoxDescription.Margin = new System.Windows.Forms.Padding (8, 4, 4, 4);
			this.TextBoxDescription.Multiline = true;
			this.TextBoxDescription.Name = "TextBoxDescription";
			this.TextBoxDescription.ReadOnly = true;
			this.TextBoxDescription.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.TextBoxDescription.Size = new System.Drawing.Size (438, 187);
			this.TextBoxDescription.TabIndex = 4;
			this.TextBoxDescription.TabStop = false;
			this.TextBoxDescription.Text = "Description";
			// 
			// okButton
			// 
			this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.okButton.Location = new System.Drawing.Point (346, 294);
			this.okButton.Margin = new System.Windows.Forms.Padding (4);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size (100, 27);
			this.okButton.TabIndex = 5;
			this.okButton.Text = "&OK";
			// 
			// AboutBox
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size (474, 347);
			this.Controls.Add (this.tableLayoutPanel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding (4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AboutBox";
			this.Padding = new System.Windows.Forms.Padding (12, 11, 12, 11);
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "About";
			((System.ComponentModel.ISupportInitialize)(this.LogoPictureBox)).EndInit ();
			this.tableLayoutPanel.ResumeLayout (false);
			this.tableLayoutPanel.PerformLayout ();
			this.ResumeLayout (false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
		private System.Windows.Forms.Label LabelProductName;
		private System.Windows.Forms.Label LabelVersion;
		private System.Windows.Forms.Label LabelCopyright;
		private System.Windows.Forms.Label LabelCompanyName;
		private System.Windows.Forms.TextBox TextBoxDescription;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.PictureBox LogoPictureBox;
	}
}
