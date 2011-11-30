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
	partial class LicenseBox
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
			System.Windows.Forms.TableLayoutPanel PanelMain;
			System.Windows.Forms.Panel PanelBorder;
			this.OkButton = new System.Windows.Forms.Button();
			this.TextBoxLicenseDetails = new System.Windows.Forms.RichTextBox();
			PanelMain = new System.Windows.Forms.TableLayoutPanel();
			PanelBorder = new System.Windows.Forms.Panel();
			PanelMain.SuspendLayout();
			PanelBorder.SuspendLayout();
			this.SuspendLayout();
			// 
			// PanelMain
			// 
			PanelMain.ColumnCount = 1;
			PanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			PanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
			PanelMain.Controls.Add(this.OkButton, 0, 1);
			PanelMain.Controls.Add(PanelBorder, 0, 0);
			PanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelMain.Location = new System.Drawing.Point(12, 11);
			PanelMain.Margin = new System.Windows.Forms.Padding(4);
			PanelMain.Name = "PanelMain";
			PanelMain.RowCount = 2;
			PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			PanelMain.Size = new System.Drawing.Size(670, 495);
			PanelMain.TabIndex = 0;
			// 
			// OkButton
			// 
			this.OkButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.OkButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.OkButton.Location = new System.Drawing.Point(285, 464);
			this.OkButton.Margin = new System.Windows.Forms.Padding(4);
			this.OkButton.Name = "OkButton";
			this.OkButton.Size = new System.Drawing.Size(100, 27);
			this.OkButton.TabIndex = 0;
			this.OkButton.Text = "&OK";
			// 
			// BorderPanel
			// 
			PanelBorder.BackColor = System.Drawing.SystemColors.Window;
			PanelBorder.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			PanelBorder.CausesValidation = false;
			PanelBorder.Controls.Add(this.TextBoxLicenseDetails);
			PanelBorder.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelBorder.Location = new System.Drawing.Point(0, 0);
			PanelBorder.Margin = new System.Windows.Forms.Padding(0, 0, 0, 4);
			PanelBorder.Name = "BorderPanel";
			PanelBorder.Padding = new System.Windows.Forms.Padding(4, 4, 0, 4);
			PanelBorder.Size = new System.Drawing.Size(670, 456);
			PanelBorder.TabIndex = 1;
			// 
			// TextBoxLicenseDetails
			// 
			this.TextBoxLicenseDetails.BackColor = System.Drawing.SystemColors.Window;
			this.TextBoxLicenseDetails.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.TextBoxLicenseDetails.CausesValidation = false;
			this.TextBoxLicenseDetails.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TextBoxLicenseDetails.Location = new System.Drawing.Point(4, 4);
			this.TextBoxLicenseDetails.Margin = new System.Windows.Forms.Padding(0, 0, 0, 4);
			this.TextBoxLicenseDetails.Name = "TextBoxLicenseDetails";
			this.TextBoxLicenseDetails.ReadOnly = true;
			this.TextBoxLicenseDetails.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
			this.TextBoxLicenseDetails.Size = new System.Drawing.Size(664, 446);
			this.TextBoxLicenseDetails.TabIndex = 0;
			this.TextBoxLicenseDetails.TabStop = false;
			this.TextBoxLicenseDetails.Text = "";
			this.TextBoxLicenseDetails.LinkClicked += new System.Windows.Forms.LinkClickedEventHandler(this.TextBoxLicenseDetails_LinkClicked);
			// 
			// LicenseBox
			// 
			this.AcceptButton = this.OkButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(694, 517);
			this.Controls.Add(PanelMain);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "LicenseBox";
			this.Padding = new System.Windows.Forms.Padding(12, 11, 12, 11);
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "GNU Public License";
			PanelMain.ResumeLayout(false);
			PanelBorder.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button OkButton;
		private System.Windows.Forms.RichTextBox TextBoxLicenseDetails;
	}
}
