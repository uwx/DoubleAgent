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
	partial class StateForm
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
			this.LabelName = new System.Windows.Forms.Label ();
			this.TextBoxName = new System.Windows.Forms.TextBox ();
			this.ListViewAnimations = new DoubleAgent.ListViewEx ();
			this.Column1 = new System.Windows.Forms.ColumnHeader ();
			this.GroupBoxAnimations = new System.Windows.Forms.GroupBox ();
			this.GroupBoxAnimations.SuspendLayout ();
			this.SuspendLayout ();
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.Location = new System.Drawing.Point (9, 6);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size (80, 17);
			this.LabelName.TabIndex = 1;
			this.LabelName.Text = "State name";
			// 
			// TextBoxName
			// 
			this.TextBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxName.Location = new System.Drawing.Point (140, 3);
			this.TextBoxName.MaximumSize = new System.Drawing.Size (600, 50);
			this.TextBoxName.Name = "TextBoxName";
			this.TextBoxName.ReadOnly = true;
			this.TextBoxName.Size = new System.Drawing.Size (500, 22);
			this.TextBoxName.TabIndex = 2;
			// 
			// ListViewAnimations
			// 
			this.ListViewAnimations.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewAnimations.CheckBoxes = true;
			this.ListViewAnimations.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.Column1});
			this.ListViewAnimations.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ListViewAnimations.Location = new System.Drawing.Point (6, 21);
			this.ListViewAnimations.MultiSelect = false;
			this.ListViewAnimations.Name = "ListViewAnimations";
			this.ListViewAnimations.SelectedIndex = -1;
			this.ListViewAnimations.SelectedItem = null;
			this.ListViewAnimations.ShowItemToolTips = true;
			this.ListViewAnimations.Size = new System.Drawing.Size (620, 378);
			this.ListViewAnimations.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.ListViewAnimations.TabIndex = 6;
			this.ListViewAnimations.UseCompatibleStateImageBehavior = false;
			this.ListViewAnimations.View = System.Windows.Forms.View.List;
			this.ListViewAnimations.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler (this.ListViewAnimations_ItemChecked);
			this.ListViewAnimations.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler (this.ListViewAnimations_ItemCheck);
			// 
			// Column1
			// 
			this.Column1.Text = "";
			this.Column1.Width = 160;
			// 
			// GroupBoxAnimations
			// 
			this.GroupBoxAnimations.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxAnimations.Controls.Add (this.ListViewAnimations);
			this.GroupBoxAnimations.Location = new System.Drawing.Point (9, 42);
			this.GroupBoxAnimations.Margin = new System.Windows.Forms.Padding (0);
			this.GroupBoxAnimations.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxAnimations.Name = "GroupBoxAnimations";
			this.GroupBoxAnimations.Size = new System.Drawing.Size (632, 405);
			this.GroupBoxAnimations.TabIndex = 7;
			this.GroupBoxAnimations.TabStop = false;
			this.GroupBoxAnimations.Text = "Animations";
			// 
			// StateForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.GroupBoxAnimations);
			this.Controls.Add (this.TextBoxName);
			this.Controls.Add (this.LabelName);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "StateForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (650, 450);
			this.GroupBoxAnimations.ResumeLayout (false);
			this.ResumeLayout (false);
			this.PerformLayout ();

		}

		#endregion

		private System.Windows.Forms.Label LabelName;
		private System.Windows.Forms.TextBox TextBoxName;
		private DoubleAgent.ListViewEx ListViewAnimations;
		private System.Windows.Forms.ColumnHeader Column1;
		private System.Windows.Forms.GroupBox GroupBoxAnimations;
	}
}
