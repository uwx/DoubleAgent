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
namespace AgentCharacterEditor.Panels
{
	partial class AnimationsPanel
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
			this.components = new System.ComponentModel.Container ();
			System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
			this.PanelMain = new System.Windows.Forms.TableLayoutPanel ();
			this.GroupBoxPalette = new DoubleAgent.GroupBoxCompat ();
			this.PictureBoxPalette = new System.Windows.Forms.PictureBox ();
			this.ToolStripPaletteFile = new DoubleAgent.ToolStripEx ();
			this.ButtonPaletteImport = new DoubleAgent.ToolStripButtonCompat ();
			this.ButtonPaletteExport = new DoubleAgent.ToolStripButtonCompat ();
			this.LabelTransparencyClick = new DoubleAgent.LabelCompat ();
			this.LabelTransparency = new DoubleAgent.LabelCompat ();
			this.LabelColorSample = new DoubleAgent.LabelCompat ();
			this.LabelTransparencySample = new DoubleAgent.LabelCompat ();
			this.TextBoxPaletteFile = new DoubleAgent.TextBoxEx ();
			this.LabelPaletteFile = new DoubleAgent.LabelCompat ();
			this.PanelTop = new System.Windows.Forms.Panel ();
			this.ToolStripAnimations = new DoubleAgent.ToolStripEx ();
			this.ButtonAdd = new DoubleAgent.ToolStripButtonCompat ();
			this.ButtonDelete = new DoubleAgent.ToolStripButtonCompat ();
			this.ListViewAnimations = new DoubleAgent.ListViewEx ();
			this.ContextMenuAnimations = new AgentCharacterEditor.Global.ContextMenuEdit (this.components);
			this.MenuItemAdd = new System.Windows.Forms.ToolStripMenuItem ();
			this.TextBoxNewName = new DoubleAgent.TextBoxEx ();
			this.LabelWidth = new DoubleAgent.LabelCompat ();
			this.LabelNewName = new DoubleAgent.LabelCompat ();
			this.NumericFrameHeight = new DoubleAgent.NumericUpDownEx ();
			this.NumericFrameWidth = new DoubleAgent.NumericUpDownEx ();
			this.LabelHeight = new DoubleAgent.LabelCompat ();
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator ();
			this.PanelMain.SuspendLayout ();
			this.GroupBoxPalette.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxPalette)).BeginInit ();
			this.ToolStripPaletteFile.SuspendLayout ();
			this.PanelTop.SuspendLayout ();
			this.ToolStripAnimations.SuspendLayout ();
			this.ContextMenuAnimations.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameHeight)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameWidth)).BeginInit ();
			this.SuspendLayout ();
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size (226, 6);
			// 
			// PanelMain
			// 
			this.PanelMain.ColumnCount = 1;
			this.PanelMain.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Controls.Add (this.GroupBoxPalette, 0, 1);
			this.PanelMain.Controls.Add (this.PanelTop, 0, 0);
			this.PanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelMain.Location = new System.Drawing.Point (9, 0);
			this.PanelMain.Margin = new System.Windows.Forms.Padding (0);
			this.PanelMain.Name = "PanelMain";
			this.PanelMain.RowCount = 2;
			this.PanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.PanelMain.Size = new System.Drawing.Size (632, 600);
			this.PanelMain.TabIndex = 0;
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
			this.GroupBoxPalette.Location = new System.Drawing.Point (0, 260);
			this.GroupBoxPalette.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxPalette.MaximumSize = new System.Drawing.Size (628, 10000);
			this.GroupBoxPalette.Name = "GroupBoxPalette";
			this.GroupBoxPalette.Padding = new System.Windows.Forms.Padding (6, 12, 6, 6);
			this.GroupBoxPalette.Size = new System.Drawing.Size (628, 340);
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
			this.PictureBoxPalette.MouseClick += new System.Windows.Forms.MouseEventHandler (this.PictureBoxPalette_MouseClick);
			this.PictureBoxPalette.MouseLeave += new System.EventHandler (this.PictureBoxPalette_MouseLeave);
			this.PictureBoxPalette.MouseMove += new System.Windows.Forms.MouseEventHandler (this.PictureBoxPalette_MouseMove);
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
			this.ToolStripPaletteFile.Size = new System.Drawing.Size (51, 27);
			this.ToolStripPaletteFile.TabIndex = 2;
			// 
			// ButtonPaletteImport
			// 
			this.ButtonPaletteImport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonPaletteImport.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileOpen;
			this.ButtonPaletteImport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonPaletteImport.Name = "ButtonPaletteImport";
			this.ButtonPaletteImport.Padding = new System.Windows.Forms.Padding (2);
			this.ButtonPaletteImport.Size = new System.Drawing.Size (24, 24);
			this.ButtonPaletteImport.Text = "Import palette";
			this.ButtonPaletteImport.Click += new System.EventHandler (this.ButtonPaletteImport_Click);
			// 
			// ButtonPaletteExport
			// 
			this.ButtonPaletteExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonPaletteExport.Enabled = false;
			this.ButtonPaletteExport.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileSave;
			this.ButtonPaletteExport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonPaletteExport.Name = "ButtonPaletteExport";
			this.ButtonPaletteExport.Padding = new System.Windows.Forms.Padding (2);
			this.ButtonPaletteExport.Size = new System.Drawing.Size (24, 24);
			this.ButtonPaletteExport.Text = "Save palette";
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
			this.TextBoxPaletteFile.Size = new System.Drawing.Size (423, 22);
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
			this.PanelTop.Size = new System.Drawing.Size (632, 251);
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
            this.ButtonDelete});
			this.ToolStripAnimations.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripAnimations.Location = new System.Drawing.Point (0, 3);
			this.ToolStripAnimations.Name = "ToolStripAnimations";
			this.ToolStripAnimations.Size = new System.Drawing.Size (51, 27);
			this.ToolStripAnimations.TabIndex = 5;
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding (2);
			this.ButtonAdd.Size = new System.Drawing.Size (24, 24);
			this.ButtonAdd.Text = global::AgentCharacterEditor.Resources.Resources.EditAddAnimation;
			this.ButtonAdd.Click += new System.EventHandler (this.ButtonAdd_Click);
			// 
			// ButtonDelete
			// 
			this.ButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.ButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonDelete.Name = "ButtonDelete";
			this.ButtonDelete.Padding = new System.Windows.Forms.Padding (2);
			this.ButtonDelete.Size = new System.Drawing.Size (24, 24);
			this.ButtonDelete.Text = "Delete animation";
			this.ButtonDelete.Click += new System.EventHandler (this.ButtonDelete_Click);
			// 
			// ListViewAnimations
			// 
			this.ListViewAnimations.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
			| System.Windows.Forms.AnchorStyles.Left)
			| System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewAnimations.ContextMenuStrip = this.ContextMenuAnimations;
			this.ListViewAnimations.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ListViewAnimations.HideSelection = false;
			this.ListViewAnimations.Location = new System.Drawing.Point (0, 32);
			this.ListViewAnimations.MultiSelect = false;
			this.ListViewAnimations.Name = "ListViewAnimations";
			this.ListViewAnimations.ShowItemToolTips = true;
			this.ListViewAnimations.Size = new System.Drawing.Size (632, 172);
			this.ListViewAnimations.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.ListViewAnimations.TabIndex = 4;
			this.ListViewAnimations.UseCompatibleStateImageBehavior = false;
			this.ListViewAnimations.View = System.Windows.Forms.View.List;
			this.ListViewAnimations.ItemActivate += new System.EventHandler (this.ListViewAnimations_ItemActivate);
			this.ListViewAnimations.SelectedIndexChanged += new System.EventHandler (this.ListViewAnimations_SelectedIndexChanged);
			// 
			// ContextMenuAnimations
			// 
			this.ContextMenuAnimations.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemAdd,
            toolStripSeparator1});
			this.ContextMenuAnimations.Name = "ContextMenuAnimations";
			this.ContextMenuAnimations.Size = new System.Drawing.Size (230, 38);
			this.ContextMenuAnimations.Opening += new System.ComponentModel.CancelEventHandler (this.ContextMenuAnimations_Opening);
			// 
			// MenuItemAdd
			// 
			this.MenuItemAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.MenuItemAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemAdd.Name = "MenuItemAdd";
			this.MenuItemAdd.Size = new System.Drawing.Size (229, 28);
			this.MenuItemAdd.Text = "Add new animation";
			this.MenuItemAdd.Click += new System.EventHandler (this.ButtonAdd_Click);
			// 
			// TextBoxNewName
			// 
			this.TextBoxNewName.AcceptsReturn = true;
			this.TextBoxNewName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxNewName.Location = new System.Drawing.Point (342, 3);
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
			this.LabelWidth.Location = new System.Drawing.Point (0, 221);
			this.LabelWidth.Name = "LabelWidth";
			this.LabelWidth.Size = new System.Drawing.Size (44, 17);
			this.LabelWidth.TabIndex = 0;
			this.LabelWidth.Text = "Width";
			// 
			// LabelNewName
			// 
			this.LabelNewName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelNewName.AutoSize = true;
			this.LabelNewName.Location = new System.Drawing.Point (197, 6);
			this.LabelNewName.Name = "LabelNewName";
			this.LabelNewName.Size = new System.Drawing.Size (139, 17);
			this.LabelNewName.TabIndex = 0;
			this.LabelNewName.Text = "New animation name";
			// 
			// NumericFrameHeight
			// 
			this.NumericFrameHeight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.NumericFrameHeight.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericFrameHeight.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericFrameHeight.Location = new System.Drawing.Point (191, 219);
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
			this.NumericFrameWidth.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericFrameWidth.Location = new System.Drawing.Point (50, 219);
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
			this.LabelHeight.Location = new System.Drawing.Point (136, 221);
			this.LabelHeight.Name = "LabelHeight";
			this.LabelHeight.Size = new System.Drawing.Size (49, 17);
			this.LabelHeight.TabIndex = 2;
			this.LabelHeight.Text = "Height";
			// 
			// AnimationsPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.PanelMain);
			this.Name = "AnimationsPanel";
			this.PanelMain.ResumeLayout (false);
			this.GroupBoxPalette.ResumeLayout (false);
			this.GroupBoxPalette.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxPalette)).EndInit ();
			this.ToolStripPaletteFile.ResumeLayout (false);
			this.ToolStripPaletteFile.PerformLayout ();
			this.PanelTop.ResumeLayout (false);
			this.PanelTop.PerformLayout ();
			this.ToolStripAnimations.ResumeLayout (false);
			this.ToolStripAnimations.PerformLayout ();
			this.ContextMenuAnimations.ResumeLayout (false);
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameHeight)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameWidth)).EndInit ();
			this.ResumeLayout (false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel PanelMain;
		private DoubleAgent.GroupBoxCompat GroupBoxPalette;
		private DoubleAgent.LabelCompat LabelTransparencySample;
		private DoubleAgent.TextBoxEx TextBoxPaletteFile;
		private DoubleAgent.LabelCompat LabelPaletteFile;
		private DoubleAgent.ToolStripEx ToolStripPaletteFile;
		private DoubleAgent.ToolStripButtonCompat ButtonPaletteImport;
		private DoubleAgent.ToolStripButtonCompat ButtonPaletteExport;
		private System.Windows.Forms.PictureBox PictureBoxPalette;
		private DoubleAgent.LabelCompat LabelColorSample;
		private DoubleAgent.LabelCompat LabelTransparency;
		private DoubleAgent.LabelCompat LabelTransparencyClick;
		private DoubleAgent.NumericUpDownEx NumericFrameHeight;
		private DoubleAgent.LabelCompat LabelHeight;
		private DoubleAgent.NumericUpDownEx NumericFrameWidth;
		private DoubleAgent.LabelCompat LabelWidth;
		private DoubleAgent.LabelCompat LabelNewName;
		private DoubleAgent.TextBoxEx TextBoxNewName;
		private System.Windows.Forms.Panel PanelTop;
		private DoubleAgent.ListViewEx ListViewAnimations;
		private DoubleAgent.ToolStripEx ToolStripAnimations;
		private DoubleAgent.ToolStripButtonCompat ButtonAdd;
		private DoubleAgent.ToolStripButtonCompat ButtonDelete;
		private Global.ContextMenuEdit ContextMenuAnimations;
		private System.Windows.Forms.ToolStripMenuItem MenuItemAdd;
	}
}
