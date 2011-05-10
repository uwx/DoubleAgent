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
	partial class StatePanel
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
			this.LabelName = new System.Windows.Forms.Label();
			this.TextBoxName = new System.Windows.Forms.TextBox();
			this.GroupBoxAnimations = new System.Windows.Forms.GroupBox();
			this.ListViewAnimations = new DoubleAgent.ListViewEx();
			this.Column1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.PanelTop = new System.Windows.Forms.Panel();
			this.LayoutPanelMain = new System.Windows.Forms.TableLayoutPanel();
			this.GroupBoxAnimations.SuspendLayout();
			this.PanelTop.SuspendLayout();
			this.LayoutPanelMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.Location = new System.Drawing.Point(0, 6);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size(80, 17);
			this.LabelName.TabIndex = 0;
			this.LabelName.Text = "State name";
			// 
			// TextBoxName
			// 
			this.TextBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxName.Location = new System.Drawing.Point(131, 3);
			this.TextBoxName.MaximumSize = new System.Drawing.Size(600, 50);
			this.TextBoxName.Name = "TextBoxName";
			this.TextBoxName.ReadOnly = true;
			this.TextBoxName.Size = new System.Drawing.Size(501, 22);
			this.TextBoxName.TabIndex = 1;
			// 
			// GroupBoxAnimations
			// 
			this.GroupBoxAnimations.Controls.Add(this.ListViewAnimations);
			this.GroupBoxAnimations.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxAnimations.Location = new System.Drawing.Point(0, 37);
			this.GroupBoxAnimations.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxAnimations.MaximumSize = new System.Drawing.Size(730, 10000);
			this.GroupBoxAnimations.Name = "GroupBoxAnimations";
			this.GroupBoxAnimations.Padding = new System.Windows.Forms.Padding(6, 12, 6, 6);
			this.GroupBoxAnimations.Size = new System.Drawing.Size(632, 563);
			this.GroupBoxAnimations.TabIndex = 0;
			this.GroupBoxAnimations.TabStop = false;
			this.GroupBoxAnimations.Text = "Animations";
			// 
			// ListViewAnimations
			// 
			this.ListViewAnimations.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewAnimations.CheckBoxes = true;
			this.ListViewAnimations.CheckOnActivate = false;
			this.ListViewAnimations.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Column1});
			this.ListViewAnimations.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ListViewAnimations.Location = new System.Drawing.Point(9, 30);
			this.ListViewAnimations.MultiSelect = false;
			this.ListViewAnimations.Name = "ListViewAnimations";
			this.ListViewAnimations.ShowItemToolTips = true;
			this.ListViewAnimations.Size = new System.Drawing.Size(614, 524);
			this.ListViewAnimations.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.ListViewAnimations.TabIndex = 0;
			this.ListViewAnimations.UseCompatibleStateImageBehavior = false;
			this.ListViewAnimations.View = System.Windows.Forms.View.List;
			this.ListViewAnimations.ItemActivate += new System.EventHandler(this.ListViewAnimations_ItemActivate);
			this.ListViewAnimations.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.ListViewAnimations_ItemCheck);
			this.ListViewAnimations.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.ListViewAnimations_ItemChecked);
			// 
			// Column1
			// 
			this.Column1.Text = "";
			this.Column1.Width = 160;
			// 
			// PanelTop
			// 
			this.PanelTop.Controls.Add(this.LabelName);
			this.PanelTop.Controls.Add(this.TextBoxName);
			this.PanelTop.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelTop.Location = new System.Drawing.Point(0, 0);
			this.PanelTop.Margin = new System.Windows.Forms.Padding(0);
			this.PanelTop.Name = "PanelTop";
			this.PanelTop.Size = new System.Drawing.Size(632, 28);
			this.PanelTop.TabIndex = 1;
			// 
			// LayoutPanelMain
			// 
			this.LayoutPanelMain.ColumnCount = 1;
			this.LayoutPanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Controls.Add(this.PanelTop, 0, 0);
			this.LayoutPanelMain.Controls.Add(this.GroupBoxAnimations, 0, 1);
			this.LayoutPanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.LayoutPanelMain.Location = new System.Drawing.Point(9, 0);
			this.LayoutPanelMain.Margin = new System.Windows.Forms.Padding(0);
			this.LayoutPanelMain.Name = "LayoutPanelMain";
			this.LayoutPanelMain.RowCount = 2;
			this.LayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Size = new System.Drawing.Size(632, 600);
			this.LayoutPanelMain.TabIndex = 0;
			// 
			// StatePanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.LayoutPanelMain);
			this.Name = "StatePanel";
			this.GroupBoxAnimations.ResumeLayout(false);
			this.PanelTop.ResumeLayout(false);
			this.PanelTop.PerformLayout();
			this.LayoutPanelMain.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label LabelName;
		private System.Windows.Forms.TextBox TextBoxName;
		private DoubleAgent.ListViewEx ListViewAnimations;
		private System.Windows.Forms.ColumnHeader Column1;
		private System.Windows.Forms.GroupBox GroupBoxAnimations;
		private System.Windows.Forms.Panel PanelTop;
		private System.Windows.Forms.TableLayoutPanel LayoutPanelMain;
	}
}
