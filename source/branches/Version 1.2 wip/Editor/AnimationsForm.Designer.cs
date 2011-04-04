﻿namespace AgentCharacterEditor
{
	partial class AnimationsForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (AnimationsForm));
			this.LayoutPanelMain = new System.Windows.Forms.TableLayoutPanel ();
			this.GroupBoxPalette = new System.Windows.Forms.GroupBox ();
			this.PictureBoxPalette = new System.Windows.Forms.PictureBox ();
			this.ToolStripPaletteFile = new DoubleAgent.ToolStripEx ();
			this.ButtonPaletteImport = new System.Windows.Forms.ToolStripButton ();
			this.ButtonPaletteExport = new System.Windows.Forms.ToolStripButton ();
			this.LabelTransparencyClick = new System.Windows.Forms.Label ();
			this.LabelTransparency = new System.Windows.Forms.Label ();
			this.LabelColorSample = new System.Windows.Forms.Label ();
			this.LabelTransparencySample = new System.Windows.Forms.Label ();
			this.TextBoxPaletteFile = new DoubleAgent.TextBoxEx ();
			this.LabelPaletteFile = new System.Windows.Forms.Label ();
			this.PanelTop = new System.Windows.Forms.Panel ();
			this.ToolStripAnimations = new DoubleAgent.ToolStripEx ();
			this.ButtonAdd = new System.Windows.Forms.ToolStripButton ();
			this.ButtonRemove = new System.Windows.Forms.ToolStripButton ();
			this.ListViewAnimations = new DoubleAgent.ListViewEx ();
			this.TextBoxNewName = new DoubleAgent.TextBoxEx ();
			this.LabelWidth = new System.Windows.Forms.Label ();
			this.LabelNewName = new System.Windows.Forms.Label ();
			this.NumericFrameHeight = new DoubleAgent.NumericUpDownEx ();
			this.NumericFrameWidth = new DoubleAgent.NumericUpDownEx ();
			this.LabelHeight = new System.Windows.Forms.Label ();
			this.LayoutPanelMain.SuspendLayout ();
			this.GroupBoxPalette.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxPalette)).BeginInit ();
			this.ToolStripPaletteFile.SuspendLayout ();
			this.PanelTop.SuspendLayout ();
			this.ToolStripAnimations.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameHeight)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameWidth)).BeginInit ();
			this.SuspendLayout ();
			// 
			// LayoutPanelMain
			// 
			this.LayoutPanelMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.LayoutPanelMain.ColumnCount = 1;
			this.LayoutPanelMain.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Controls.Add (this.GroupBoxPalette, 0, 1);
			this.LayoutPanelMain.Controls.Add (this.PanelTop, 0, 0);
			this.LayoutPanelMain.Location = new System.Drawing.Point (12, 3);
			this.LayoutPanelMain.Margin = new System.Windows.Forms.Padding (0);
			this.LayoutPanelMain.Name = "LayoutPanelMain";
			this.LayoutPanelMain.RowCount = 2;
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.LayoutPanelMain.Size = new System.Drawing.Size (626, 594);
			this.LayoutPanelMain.TabIndex = 0;
			// 
			// GroupBoxPalette
			// 
			this.GroupBoxPalette.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxPalette.Controls.Add (this.PictureBoxPalette);
			this.GroupBoxPalette.Controls.Add (this.ToolStripPaletteFile);
			this.GroupBoxPalette.Controls.Add (this.LabelTransparencyClick);
			this.GroupBoxPalette.Controls.Add (this.LabelTransparency);
			this.GroupBoxPalette.Controls.Add (this.LabelColorSample);
			this.GroupBoxPalette.Controls.Add (this.LabelTransparencySample);
			this.GroupBoxPalette.Controls.Add (this.TextBoxPaletteFile);
			this.GroupBoxPalette.Controls.Add (this.LabelPaletteFile);
			this.GroupBoxPalette.Location = new System.Drawing.Point (0, 254);
			this.GroupBoxPalette.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxPalette.MaximumSize = new System.Drawing.Size (628, 10000);
			this.GroupBoxPalette.Name = "GroupBoxPalette";
			this.GroupBoxPalette.Padding = new System.Windows.Forms.Padding (6, 12, 6, 6);
			this.GroupBoxPalette.Size = new System.Drawing.Size (626, 340);
			this.GroupBoxPalette.TabIndex = 2;
			this.GroupBoxPalette.TabStop = false;
			this.GroupBoxPalette.Text = "Palette";
			// 
			// PictureBoxPalette
			// 
			this.PictureBoxPalette.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PictureBoxPalette.ErrorImage = null;
			this.PictureBoxPalette.InitialImage = null;
			this.PictureBoxPalette.Location = new System.Drawing.Point (12, 70);
			this.PictureBoxPalette.Name = "PictureBoxPalette";
			this.PictureBoxPalette.Size = new System.Drawing.Size (256, 256);
			this.PictureBoxPalette.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.PictureBoxPalette.TabIndex = 5;
			this.PictureBoxPalette.TabStop = false;
			this.PictureBoxPalette.MouseLeave += new System.EventHandler (this.PictureBoxPalette_MouseLeave);
			this.PictureBoxPalette.MouseMove += new System.Windows.Forms.MouseEventHandler (this.PictureBoxPalette_MouseMove);
			this.PictureBoxPalette.MouseClick += new System.Windows.Forms.MouseEventHandler (this.PictureBoxPalette_MouseClick);
			// 
			// ToolStripPaletteFile
			// 
			this.ToolStripPaletteFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripPaletteFile.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripPaletteFile.CanOverflow = false;
			this.ToolStripPaletteFile.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripPaletteFile.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripPaletteFile.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonPaletteImport,
            this.ButtonPaletteExport});
			this.ToolStripPaletteFile.Location = new System.Drawing.Point (571, 28);
			this.ToolStripPaletteFile.Name = "ToolStripPaletteFile";
			this.ToolStripPaletteFile.RenderInline = true;
			this.ToolStripPaletteFile.Size = new System.Drawing.Size (49, 26);
			this.ToolStripPaletteFile.TabIndex = 2;
			// 
			// ButtonPaletteImport
			// 
			this.ButtonPaletteImport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonPaletteImport.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonPaletteImport.Image")));
			this.ButtonPaletteImport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonPaletteImport.Name = "ButtonPaletteImport";
			this.ButtonPaletteImport.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonPaletteImport.Size = new System.Drawing.Size (23, 23);
			this.ButtonPaletteImport.ToolTipText = "Import palette";
			this.ButtonPaletteImport.Click += new System.EventHandler (this.ButtonPaletteImport_Click);
			// 
			// ButtonPaletteExport
			// 
			this.ButtonPaletteExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonPaletteExport.Enabled = false;
			this.ButtonPaletteExport.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonPaletteExport.Image")));
			this.ButtonPaletteExport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonPaletteExport.Name = "ButtonPaletteExport";
			this.ButtonPaletteExport.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonPaletteExport.Size = new System.Drawing.Size (23, 23);
			this.ButtonPaletteExport.ToolTipText = "Save palette";
			// 
			// LabelTransparencyClick
			// 
			this.LabelTransparencyClick.Font = new System.Drawing.Font ("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.LabelTransparencyClick.Location = new System.Drawing.Point (283, 258);
			this.LabelTransparencyClick.Name = "LabelTransparencyClick";
			this.LabelTransparencyClick.Size = new System.Drawing.Size (156, 56);
			this.LabelTransparencyClick.TabIndex = 6;
			this.LabelTransparencyClick.Text = "Click to set the Transparency";
			this.LabelTransparencyClick.Visible = false;
			// 
			// LabelTransparency
			// 
			this.LabelTransparency.Location = new System.Drawing.Point (286, 70);
			this.LabelTransparency.Name = "LabelTransparency";
			this.LabelTransparency.Size = new System.Drawing.Size (100, 21);
			this.LabelTransparency.TabIndex = 3;
			this.LabelTransparency.Text = "Transparency";
			this.LabelTransparency.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// LabelColorSample
			// 
			this.LabelColorSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelColorSample.Font = new System.Drawing.Font ("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.LabelColorSample.Location = new System.Drawing.Point (286, 148);
			this.LabelColorSample.Name = "LabelColorSample";
			this.LabelColorSample.Size = new System.Drawing.Size (100, 100);
			this.LabelColorSample.TabIndex = 5;
			this.LabelColorSample.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.LabelColorSample.Visible = false;
			// 
			// LabelTransparencySample
			// 
			this.LabelTransparencySample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelTransparencySample.Font = new System.Drawing.Font ("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.LabelTransparencySample.Location = new System.Drawing.Point (286, 91);
			this.LabelTransparencySample.Name = "LabelTransparencySample";
			this.LabelTransparencySample.Size = new System.Drawing.Size (100, 26);
			this.LabelTransparencySample.TabIndex = 4;
			this.LabelTransparencySample.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// TextBoxPaletteFile
			// 
			this.TextBoxPaletteFile.AcceptsReturn = true;
			this.TextBoxPaletteFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxPaletteFile.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
			this.TextBoxPaletteFile.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
			this.TextBoxPaletteFile.Location = new System.Drawing.Point (147, 30);
			this.TextBoxPaletteFile.Name = "TextBoxPaletteFile";
			this.TextBoxPaletteFile.Size = new System.Drawing.Size (421, 22);
			this.TextBoxPaletteFile.TabIndex = 1;
			this.TextBoxPaletteFile.Validated += new System.EventHandler (this.TextBoxPaletteFile_Validated);
			// 
			// LabelPaletteFile
			// 
			this.LabelPaletteFile.AutoSize = true;
			this.LabelPaletteFile.Location = new System.Drawing.Point (9, 33);
			this.LabelPaletteFile.Name = "LabelPaletteFile";
			this.LabelPaletteFile.Size = new System.Drawing.Size (107, 17);
			this.LabelPaletteFile.TabIndex = 0;
			this.LabelPaletteFile.Text = "Image file name";
			// 
			// PanelTop
			// 
			this.PanelTop.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PanelTop.Controls.Add (this.ToolStripAnimations);
			this.PanelTop.Controls.Add (this.ListViewAnimations);
			this.PanelTop.Controls.Add (this.TextBoxNewName);
			this.PanelTop.Controls.Add (this.LabelWidth);
			this.PanelTop.Controls.Add (this.LabelNewName);
			this.PanelTop.Controls.Add (this.NumericFrameHeight);
			this.PanelTop.Controls.Add (this.NumericFrameWidth);
			this.PanelTop.Controls.Add (this.LabelHeight);
			this.PanelTop.Location = new System.Drawing.Point (0, 0);
			this.PanelTop.Margin = new System.Windows.Forms.Padding (0);
			this.PanelTop.Name = "PanelTop";
			this.PanelTop.Size = new System.Drawing.Size (626, 245);
			this.PanelTop.TabIndex = 3;
			// 
			// ToolStripAnimations
			// 
			this.ToolStripAnimations.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripAnimations.CanOverflow = false;
			this.ToolStripAnimations.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripAnimations.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripAnimations.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonRemove});
			this.ToolStripAnimations.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripAnimations.Location = new System.Drawing.Point (0, 0);
			this.ToolStripAnimations.Name = "ToolStripAnimations";
			this.ToolStripAnimations.RenderInline = true;
			this.ToolStripAnimations.Size = new System.Drawing.Size (49, 26);
			this.ToolStripAnimations.TabIndex = 5;
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonAdd.Image")));
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonAdd.Size = new System.Drawing.Size (23, 23);
			this.ButtonAdd.Text = "Add new animation";
			this.ButtonAdd.Click += new System.EventHandler (this.ButtonAdd_Click);
			// 
			// ButtonRemove
			// 
			this.ButtonRemove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonRemove.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonRemove.Image")));
			this.ButtonRemove.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonRemove.Name = "ButtonRemove";
			this.ButtonRemove.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonRemove.Size = new System.Drawing.Size (23, 23);
			this.ButtonRemove.Text = "Delete animation";
			this.ButtonRemove.Click += new System.EventHandler (this.ButtonRemove_Click);
			// 
			// ListViewAnimations
			// 
			this.ListViewAnimations.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewAnimations.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ListViewAnimations.HideSelection = false;
			this.ListViewAnimations.Location = new System.Drawing.Point (0, 29);
			this.ListViewAnimations.MultiSelect = false;
			this.ListViewAnimations.Name = "ListViewAnimations";
			this.ListViewAnimations.SelectedIndex = -1;
			this.ListViewAnimations.SelectedItem = null;
			this.ListViewAnimations.ShowItemToolTips = true;
			this.ListViewAnimations.Size = new System.Drawing.Size (626, 166);
			this.ListViewAnimations.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.ListViewAnimations.TabIndex = 4;
			this.ListViewAnimations.UseCompatibleStateImageBehavior = false;
			this.ListViewAnimations.View = System.Windows.Forms.View.List;
			this.ListViewAnimations.SelectedIndexChanged += new System.EventHandler (this.ListViewAnimations_SelectedIndexChanged);
			// 
			// TextBoxNewName
			// 
			this.TextBoxNewName.AcceptsReturn = true;
			this.TextBoxNewName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxNewName.Location = new System.Drawing.Point (336, 0);
			this.TextBoxNewName.MaxLength = 255;
			this.TextBoxNewName.Name = "TextBoxNewName";
			this.TextBoxNewName.Size = new System.Drawing.Size (290, 22);
			this.TextBoxNewName.TabIndex = 1;
			this.TextBoxNewName.TextChanged += new System.EventHandler (this.TextBoxNewName_TextChanged);
			// 
			// LabelWidth
			// 
			this.LabelWidth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.LabelWidth.AutoSize = true;
			this.LabelWidth.Location = new System.Drawing.Point (0, 215);
			this.LabelWidth.Name = "LabelWidth";
			this.LabelWidth.Size = new System.Drawing.Size (44, 17);
			this.LabelWidth.TabIndex = 0;
			this.LabelWidth.Text = "Width";
			// 
			// LabelNewName
			// 
			this.LabelNewName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelNewName.AutoSize = true;
			this.LabelNewName.Location = new System.Drawing.Point (191, 3);
			this.LabelNewName.Name = "LabelNewName";
			this.LabelNewName.Size = new System.Drawing.Size (139, 17);
			this.LabelNewName.TabIndex = 0;
			this.LabelNewName.Text = "New animation name";
			// 
			// NumericFrameHeight
			// 
			this.NumericFrameHeight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.NumericFrameHeight.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericFrameHeight.Location = new System.Drawing.Point (191, 213);
			this.NumericFrameHeight.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericFrameHeight.Name = "NumericFrameHeight";
			this.NumericFrameHeight.Size = new System.Drawing.Size (80, 22);
			this.NumericFrameHeight.TabIndex = 3;
			this.NumericFrameHeight.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericFrameHeight.Validated += new System.EventHandler (this.NumericFrameHeight_Validated);
			// 
			// NumericFrameWidth
			// 
			this.NumericFrameWidth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.NumericFrameWidth.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericFrameWidth.Location = new System.Drawing.Point (50, 213);
			this.NumericFrameWidth.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericFrameWidth.Name = "NumericFrameWidth";
			this.NumericFrameWidth.Size = new System.Drawing.Size (80, 22);
			this.NumericFrameWidth.TabIndex = 1;
			this.NumericFrameWidth.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericFrameWidth.Validated += new System.EventHandler (this.NumericFrameWidth_Validated);
			// 
			// LabelHeight
			// 
			this.LabelHeight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.LabelHeight.AutoSize = true;
			this.LabelHeight.Location = new System.Drawing.Point (136, 215);
			this.LabelHeight.Name = "LabelHeight";
			this.LabelHeight.Size = new System.Drawing.Size (49, 17);
			this.LabelHeight.TabIndex = 2;
			this.LabelHeight.Text = "Height";
			// 
			// AnimationsForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.LayoutPanelMain);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "AnimationsForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (650, 600);
			this.LayoutPanelMain.ResumeLayout (false);
			this.GroupBoxPalette.ResumeLayout (false);
			this.GroupBoxPalette.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxPalette)).EndInit ();
			this.ToolStripPaletteFile.ResumeLayout (false);
			this.ToolStripPaletteFile.PerformLayout ();
			this.PanelTop.ResumeLayout (false);
			this.PanelTop.PerformLayout ();
			this.ToolStripAnimations.ResumeLayout (false);
			this.ToolStripAnimations.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameHeight)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameWidth)).EndInit ();
			this.ResumeLayout (false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel LayoutPanelMain;
		private System.Windows.Forms.GroupBox GroupBoxPalette;
		private System.Windows.Forms.Label LabelTransparencySample;
		private DoubleAgent.TextBoxEx TextBoxPaletteFile;
		private System.Windows.Forms.Label LabelPaletteFile;
		private DoubleAgent.ToolStripEx ToolStripPaletteFile;
		private System.Windows.Forms.ToolStripButton ButtonPaletteImport;
		private System.Windows.Forms.ToolStripButton ButtonPaletteExport;
		private System.Windows.Forms.PictureBox PictureBoxPalette;
		private System.Windows.Forms.Label LabelColorSample;
		private System.Windows.Forms.Label LabelTransparency;
		private System.Windows.Forms.Label LabelTransparencyClick;
		private DoubleAgent.NumericUpDownEx NumericFrameHeight;
		private System.Windows.Forms.Label LabelHeight;
		private DoubleAgent.NumericUpDownEx NumericFrameWidth;
		private System.Windows.Forms.Label LabelWidth;
		private System.Windows.Forms.Label LabelNewName;
		private DoubleAgent.TextBoxEx TextBoxNewName;
		private System.Windows.Forms.Panel PanelTop;
		private DoubleAgent.ListViewEx ListViewAnimations;
		private DoubleAgent.ToolStripEx ToolStripAnimations;
		private System.Windows.Forms.ToolStripButton ButtonAdd;
		private System.Windows.Forms.ToolStripButton ButtonRemove;
	}
}
