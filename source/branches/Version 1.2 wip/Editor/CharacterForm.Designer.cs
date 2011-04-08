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
	partial class CharacterForm
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
			System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem ("Arabic");
			System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem ("Basque");
			System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem ("Chinese (Simplified)");
			System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem ("Chinese (Traditional)");
			System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem ("Croatian");
			System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem ("Czech");
			System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem ("Danish");
			System.Windows.Forms.ListViewItem listViewItem8 = new System.Windows.Forms.ListViewItem ("Dutch");
			System.Windows.Forms.ListViewItem listViewItem9 = new System.Windows.Forms.ListViewItem (new string[] {
            "English"}, -1, System.Drawing.Color.Empty, System.Drawing.Color.Empty, new System.Drawing.Font ("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0))));
			System.Windows.Forms.ListViewItem listViewItem10 = new System.Windows.Forms.ListViewItem ("Finnish");
			System.Windows.Forms.ListViewItem listViewItem11 = new System.Windows.Forms.ListViewItem ("French");
			System.Windows.Forms.ListViewItem listViewItem12 = new System.Windows.Forms.ListViewItem ("German");
			System.Windows.Forms.ListViewItem listViewItem13 = new System.Windows.Forms.ListViewItem ("Greek");
			System.Windows.Forms.ListViewItem listViewItem14 = new System.Windows.Forms.ListViewItem ("Hebrew");
			System.Windows.Forms.ListViewItem listViewItem15 = new System.Windows.Forms.ListViewItem ("Hungarian");
			System.Windows.Forms.ListViewItem listViewItem16 = new System.Windows.Forms.ListViewItem ("Italian");
			System.Windows.Forms.ListViewItem listViewItem17 = new System.Windows.Forms.ListViewItem ("Japanese");
			System.Windows.Forms.ListViewItem listViewItem18 = new System.Windows.Forms.ListViewItem ("Korean");
			System.Windows.Forms.ListViewItem listViewItem19 = new System.Windows.Forms.ListViewItem ("Norwegian");
			System.Windows.Forms.ListViewItem listViewItem20 = new System.Windows.Forms.ListViewItem ("Polish");
			System.Windows.Forms.ListViewItem listViewItem21 = new System.Windows.Forms.ListViewItem ("Portuguese");
			System.Windows.Forms.ListViewItem listViewItem22 = new System.Windows.Forms.ListViewItem ("Portuguese (Brazil)");
			System.Windows.Forms.ListViewItem listViewItem23 = new System.Windows.Forms.ListViewItem ("Romanian");
			System.Windows.Forms.ListViewItem listViewItem24 = new System.Windows.Forms.ListViewItem ("Russian");
			System.Windows.Forms.ListViewItem listViewItem25 = new System.Windows.Forms.ListViewItem ("Slovakian");
			System.Windows.Forms.ListViewItem listViewItem26 = new System.Windows.Forms.ListViewItem ("Slovenian");
			System.Windows.Forms.ListViewItem listViewItem27 = new System.Windows.Forms.ListViewItem ("Spanish");
			System.Windows.Forms.ListViewItem listViewItem28 = new System.Windows.Forms.ListViewItem ("Swedish");
			System.Windows.Forms.ListViewItem listViewItem29 = new System.Windows.Forms.ListViewItem ("Thai");
			System.Windows.Forms.ListViewItem listViewItem30 = new System.Windows.Forms.ListViewItem ("Turkish");
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (CharacterForm));
			this.GroupBoxGeneral = new System.Windows.Forms.GroupBox ();
			this.LayoutPanelGeneral = new System.Windows.Forms.TableLayoutPanel ();
			this.LabelName = new System.Windows.Forms.Label ();
			this.LabelDescription = new System.Windows.Forms.Label ();
			this.LabelExtra = new System.Windows.Forms.Label ();
			this.TextBoxName = new DoubleAgent.TextBoxEx ();
			this.TextBoxDescription = new System.Windows.Forms.TextBox ();
			this.TextBoxExtra = new System.Windows.Forms.TextBox ();
			this.ListViewLanguage = new DoubleAgent.ListViewEx ();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader ();
			this.GroupBoxIcon = new System.Windows.Forms.GroupBox ();
			this.ToolStripIconFile = new DoubleAgent.ToolStripEx ();
			this.ButtonIconImport = new System.Windows.Forms.ToolStripButton ();
			this.ButtonIconExport = new System.Windows.Forms.ToolStripButton ();
			this.TextBoxIconFile = new DoubleAgent.TextBoxEx ();
			this.PictureBoxIconLarge = new System.Windows.Forms.PictureBox ();
			this.PictureBoxIconSmall = new System.Windows.Forms.PictureBox ();
			this.GroupBoxGUID = new System.Windows.Forms.GroupBox ();
			this.TextBoxGUID = new System.Windows.Forms.TextBox ();
			this.ButtonNewGUID = new System.Windows.Forms.Button ();
			this.LayoutPanelMain = new System.Windows.Forms.TableLayoutPanel ();
			this.GroupBoxGeneral.SuspendLayout ();
			this.LayoutPanelGeneral.SuspendLayout ();
			this.GroupBoxIcon.SuspendLayout ();
			this.ToolStripIconFile.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxIconLarge)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxIconSmall)).BeginInit ();
			this.GroupBoxGUID.SuspendLayout ();
			this.LayoutPanelMain.SuspendLayout ();
			this.SuspendLayout ();
			// 
			// GroupBoxGeneral
			// 
			this.GroupBoxGeneral.Controls.Add (this.LayoutPanelGeneral);
			this.GroupBoxGeneral.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxGeneral.Location = new System.Drawing.Point (0, 0);
			this.GroupBoxGeneral.Margin = new System.Windows.Forms.Padding (0);
			this.GroupBoxGeneral.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxGeneral.Name = "GroupBoxGeneral";
			this.GroupBoxGeneral.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxGeneral.Size = new System.Drawing.Size (582, 452);
			this.GroupBoxGeneral.TabIndex = 0;
			this.GroupBoxGeneral.TabStop = false;
			this.GroupBoxGeneral.Text = "General";
			// 
			// LayoutPanelGeneral
			// 
			this.LayoutPanelGeneral.ColumnCount = 3;
			this.LayoutPanelGeneral.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle ());
			this.LayoutPanelGeneral.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle ());
			this.LayoutPanelGeneral.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelGeneral.Controls.Add (this.LabelName, 1, 0);
			this.LayoutPanelGeneral.Controls.Add (this.LabelDescription, 1, 1);
			this.LayoutPanelGeneral.Controls.Add (this.LabelExtra, 1, 2);
			this.LayoutPanelGeneral.Controls.Add (this.TextBoxName, 2, 0);
			this.LayoutPanelGeneral.Controls.Add (this.TextBoxDescription, 2, 1);
			this.LayoutPanelGeneral.Controls.Add (this.TextBoxExtra, 2, 2);
			this.LayoutPanelGeneral.Controls.Add (this.ListViewLanguage, 0, 0);
			this.LayoutPanelGeneral.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanelGeneral.Location = new System.Drawing.Point (6, 24);
			this.LayoutPanelGeneral.Margin = new System.Windows.Forms.Padding (0);
			this.LayoutPanelGeneral.Name = "LayoutPanelGeneral";
			this.LayoutPanelGeneral.RowCount = 3;
			this.LayoutPanelGeneral.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.LayoutPanelGeneral.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 50F));
			this.LayoutPanelGeneral.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 50F));
			this.LayoutPanelGeneral.Size = new System.Drawing.Size (570, 422);
			this.LayoutPanelGeneral.TabIndex = 0;
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.Location = new System.Drawing.Point (194, 5);
			this.LabelName.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size (45, 17);
			this.LabelName.TabIndex = 0;
			this.LabelName.Text = "Name";
			// 
			// LabelDescription
			// 
			this.LabelDescription.AutoSize = true;
			this.LabelDescription.Location = new System.Drawing.Point (194, 33);
			this.LabelDescription.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelDescription.Name = "LabelDescription";
			this.LabelDescription.Size = new System.Drawing.Size (79, 17);
			this.LabelDescription.TabIndex = 1;
			this.LabelDescription.Text = "Description";
			// 
			// LabelExtra
			// 
			this.LabelExtra.AutoSize = true;
			this.LabelExtra.Location = new System.Drawing.Point (194, 230);
			this.LabelExtra.Margin = new System.Windows.Forms.Padding (3, 5, 3, 0);
			this.LabelExtra.Name = "LabelExtra";
			this.LabelExtra.Size = new System.Drawing.Size (40, 17);
			this.LabelExtra.TabIndex = 2;
			this.LabelExtra.Text = "Extra";
			// 
			// TextBoxName
			// 
			this.TextBoxName.AcceptsReturn = true;
			this.TextBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxName.Location = new System.Drawing.Point (279, 3);
			this.TextBoxName.Name = "TextBoxName";
			this.TextBoxName.Size = new System.Drawing.Size (288, 22);
			this.TextBoxName.TabIndex = 1;
			this.TextBoxName.Validated += new System.EventHandler (this.TextBoxName_Validated);
			// 
			// TextBoxDescription
			// 
			this.TextBoxDescription.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TextBoxDescription.Location = new System.Drawing.Point (279, 31);
			this.TextBoxDescription.Multiline = true;
			this.TextBoxDescription.Name = "TextBoxDescription";
			this.TextBoxDescription.Size = new System.Drawing.Size (288, 191);
			this.TextBoxDescription.TabIndex = 2;
			this.TextBoxDescription.Validated += new System.EventHandler (this.TextBoxDescription_Validated);
			// 
			// TextBoxExtra
			// 
			this.TextBoxExtra.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TextBoxExtra.Location = new System.Drawing.Point (279, 228);
			this.TextBoxExtra.Multiline = true;
			this.TextBoxExtra.Name = "TextBoxExtra";
			this.TextBoxExtra.Size = new System.Drawing.Size (288, 191);
			this.TextBoxExtra.TabIndex = 3;
			this.TextBoxExtra.Validated += new System.EventHandler (this.TextBoxExtra_Validated);
			// 
			// ListViewLanguage
			// 
			this.ListViewLanguage.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
			this.ListViewLanguage.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ListViewLanguage.FullRowSelect = true;
			this.ListViewLanguage.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ListViewLanguage.HideSelection = false;
			listViewItem1.Tag = "ar-SA-0401";
			listViewItem2.Tag = "eu-ES-042D";
			listViewItem3.Tag = "zh-CN-0804";
			listViewItem4.Tag = "zh-TW-0404";
			listViewItem5.Tag = "hr-HR-041A";
			listViewItem6.Tag = "cs-CZ-0405";
			listViewItem7.Tag = "da-DK-0406";
			listViewItem8.Tag = "nl-NL-0413";
			listViewItem9.Tag = "en-US-0409";
			listViewItem10.Tag = "fi-FI-040B";
			listViewItem11.Tag = "fr-FR-040C";
			listViewItem12.Tag = "de-DE-0407";
			listViewItem13.Tag = "el-GR-0408";
			listViewItem14.Tag = "he-IL-040D";
			listViewItem15.Tag = "hu-HU-040E";
			listViewItem16.Tag = "it-IT-0410";
			listViewItem17.Tag = "ja-JP-0411";
			listViewItem18.Tag = "ko-KR-0412";
			listViewItem19.Tag = "nb-NO-0414";
			listViewItem20.Tag = "pl-PL-0415";
			listViewItem21.Tag = "pt-PT-0816";
			listViewItem22.Tag = "pt-BR-0416";
			listViewItem23.Tag = "ro-RO-0418";
			listViewItem24.Tag = "ru-RU-0419";
			listViewItem25.Tag = "sk-SK-041B";
			listViewItem26.Tag = "sl-SI-0424";
			listViewItem27.Tag = "es-ES-0C0A";
			listViewItem28.Tag = "sv-SE-041D";
			listViewItem29.Tag = "th-TH-041E";
			listViewItem30.Tag = "tr-TR-041F";
			this.ListViewLanguage.Items.AddRange (new System.Windows.Forms.ListViewItem[] {
            listViewItem1,
            listViewItem2,
            listViewItem3,
            listViewItem4,
            listViewItem5,
            listViewItem6,
            listViewItem7,
            listViewItem8,
            listViewItem9,
            listViewItem10,
            listViewItem11,
            listViewItem12,
            listViewItem13,
            listViewItem14,
            listViewItem15,
            listViewItem16,
            listViewItem17,
            listViewItem18,
            listViewItem19,
            listViewItem20,
            listViewItem21,
            listViewItem22,
            listViewItem23,
            listViewItem24,
            listViewItem25,
            listViewItem26,
            listViewItem27,
            listViewItem28,
            listViewItem29,
            listViewItem30});
			this.ListViewLanguage.Location = new System.Drawing.Point (3, 3);
			this.ListViewLanguage.MinimumSize = new System.Drawing.Size (185, 4);
			this.ListViewLanguage.MultiSelect = false;
			this.ListViewLanguage.Name = "ListViewLanguage";
			this.LayoutPanelGeneral.SetRowSpan (this.ListViewLanguage, 3);
			this.ListViewLanguage.ShowGroups = false;
			this.ListViewLanguage.ShowItemToolTips = true;
			this.ListViewLanguage.Size = new System.Drawing.Size (185, 416);
			this.ListViewLanguage.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.ListViewLanguage.TabIndex = 0;
			this.ListViewLanguage.UseCompatibleStateImageBehavior = false;
			this.ListViewLanguage.View = System.Windows.Forms.View.Details;
			this.ListViewLanguage.SelectedIndexChanged += new System.EventHandler (this.ListViewLanguage_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "";
			this.columnHeader1.Width = 160;
			// 
			// GroupBoxIcon
			// 
			this.GroupBoxIcon.Controls.Add (this.ToolStripIconFile);
			this.GroupBoxIcon.Controls.Add (this.TextBoxIconFile);
			this.GroupBoxIcon.Controls.Add (this.PictureBoxIconLarge);
			this.GroupBoxIcon.Controls.Add (this.PictureBoxIconSmall);
			this.GroupBoxIcon.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxIcon.Location = new System.Drawing.Point (0, 461);
			this.GroupBoxIcon.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxIcon.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxIcon.Name = "GroupBoxIcon";
			this.GroupBoxIcon.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxIcon.Size = new System.Drawing.Size (582, 70);
			this.GroupBoxIcon.TabIndex = 1;
			this.GroupBoxIcon.TabStop = false;
			this.GroupBoxIcon.Text = "Icon";
			// 
			// ToolStripIconFile
			// 
			this.ToolStripIconFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripIconFile.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripIconFile.CanOverflow = false;
			this.ToolStripIconFile.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripIconFile.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripIconFile.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonIconImport,
            this.ButtonIconExport});
			this.ToolStripIconFile.Location = new System.Drawing.Point (496, 24);
			this.ToolStripIconFile.Name = "ToolStripIconFile";
			this.ToolStripIconFile.Size = new System.Drawing.Size (80, 26);
			this.ToolStripIconFile.TabIndex = 2;
			// 
			// ButtonIconImport
			// 
			this.ButtonIconImport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonIconImport.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonIconImport.Image")));
			this.ButtonIconImport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonIconImport.Name = "ButtonIconImport";
			this.ButtonIconImport.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonIconImport.Size = new System.Drawing.Size (23, 23);
			this.ButtonIconImport.ToolTipText = "Open icon file";
			this.ButtonIconImport.Click += new System.EventHandler (this.ButtonIconImport_Click);
			// 
			// ButtonIconExport
			// 
			this.ButtonIconExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonIconExport.Enabled = false;
			this.ButtonIconExport.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonIconExport.Image")));
			this.ButtonIconExport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonIconExport.Name = "ButtonIconExport";
			this.ButtonIconExport.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonIconExport.Size = new System.Drawing.Size (23, 23);
			this.ButtonIconExport.ToolTipText = "Save icon";
			// 
			// TextBoxIconFile
			// 
			this.TextBoxIconFile.AcceptsReturn = true;
			this.TextBoxIconFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxIconFile.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
			this.TextBoxIconFile.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
			this.TextBoxIconFile.Location = new System.Drawing.Point (89, 26);
			this.TextBoxIconFile.Name = "TextBoxIconFile";
			this.TextBoxIconFile.Size = new System.Drawing.Size (435, 22);
			this.TextBoxIconFile.TabIndex = 0;
			this.TextBoxIconFile.Validated += new System.EventHandler (this.TextBoxIconFile_Validated);
			// 
			// PictureBoxIconLarge
			// 
			this.PictureBoxIconLarge.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PictureBoxIconLarge.ErrorImage = null;
			this.PictureBoxIconLarge.Image = ((System.Drawing.Image)(resources.GetObject ("PictureBoxIconLarge.Image")));
			this.PictureBoxIconLarge.InitialImage = ((System.Drawing.Image)(resources.GetObject ("PictureBoxIconLarge.InitialImage")));
			this.PictureBoxIconLarge.Location = new System.Drawing.Point (37, 29);
			this.PictureBoxIconLarge.Name = "PictureBoxIconLarge";
			this.PictureBoxIconLarge.Padding = new System.Windows.Forms.Padding (2);
			this.PictureBoxIconLarge.Size = new System.Drawing.Size (32, 32);
			this.PictureBoxIconLarge.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.PictureBoxIconLarge.TabIndex = 0;
			this.PictureBoxIconLarge.TabStop = false;
			// 
			// PictureBoxIconSmall
			// 
			this.PictureBoxIconSmall.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PictureBoxIconSmall.ErrorImage = null;
			this.PictureBoxIconSmall.Image = ((System.Drawing.Image)(resources.GetObject ("PictureBoxIconSmall.Image")));
			this.PictureBoxIconSmall.InitialImage = ((System.Drawing.Image)(resources.GetObject ("PictureBoxIconSmall.InitialImage")));
			this.PictureBoxIconSmall.Location = new System.Drawing.Point (9, 29);
			this.PictureBoxIconSmall.Name = "PictureBoxIconSmall";
			this.PictureBoxIconSmall.Padding = new System.Windows.Forms.Padding (2);
			this.PictureBoxIconSmall.Size = new System.Drawing.Size (22, 22);
			this.PictureBoxIconSmall.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.PictureBoxIconSmall.TabIndex = 0;
			this.PictureBoxIconSmall.TabStop = false;
			// 
			// GroupBoxGUID
			// 
			this.GroupBoxGUID.Controls.Add (this.TextBoxGUID);
			this.GroupBoxGUID.Controls.Add (this.ButtonNewGUID);
			this.GroupBoxGUID.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxGUID.Location = new System.Drawing.Point (0, 540);
			this.GroupBoxGUID.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxGUID.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxGUID.Name = "GroupBoxGUID";
			this.GroupBoxGUID.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxGUID.Size = new System.Drawing.Size (582, 60);
			this.GroupBoxGUID.TabIndex = 2;
			this.GroupBoxGUID.TabStop = false;
			this.GroupBoxGUID.Text = "Unique Identifier";
			// 
			// TextBoxGUID
			// 
			this.TextBoxGUID.Location = new System.Drawing.Point (6, 30);
			this.TextBoxGUID.MaximumSize = new System.Drawing.Size (300, 22);
			this.TextBoxGUID.MinimumSize = new System.Drawing.Size (300, 22);
			this.TextBoxGUID.Name = "TextBoxGUID";
			this.TextBoxGUID.ReadOnly = true;
			this.TextBoxGUID.Size = new System.Drawing.Size (300, 22);
			this.TextBoxGUID.TabIndex = 0;
			this.TextBoxGUID.TabStop = false;
			// 
			// ButtonNewGUID
			// 
			this.ButtonNewGUID.Location = new System.Drawing.Point (312, 28);
			this.ButtonNewGUID.Name = "ButtonNewGUID";
			this.ButtonNewGUID.Size = new System.Drawing.Size (102, 26);
			this.ButtonNewGUID.TabIndex = 1;
			this.ButtonNewGUID.Text = "New GUID";
			this.ButtonNewGUID.UseVisualStyleBackColor = true;
			this.ButtonNewGUID.Click += new System.EventHandler (this.ButtonNewGUID_Click);
			// 
			// LayoutPanelMain
			// 
			this.LayoutPanelMain.AutoSize = true;
			this.LayoutPanelMain.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.LayoutPanelMain.ColumnCount = 1;
			this.LayoutPanelMain.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Controls.Add (this.GroupBoxGUID, 0, 2);
			this.LayoutPanelMain.Controls.Add (this.GroupBoxGeneral, 0, 0);
			this.LayoutPanelMain.Controls.Add (this.GroupBoxIcon, 0, 1);
			this.LayoutPanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanelMain.Location = new System.Drawing.Point (9, 0);
			this.LayoutPanelMain.Margin = new System.Windows.Forms.Padding (0);
			this.LayoutPanelMain.Name = "LayoutPanelMain";
			this.LayoutPanelMain.RowCount = 3;
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.LayoutPanelMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.LayoutPanelMain.Size = new System.Drawing.Size (582, 600);
			this.LayoutPanelMain.TabIndex = 3;
			// 
			// CharacterForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.LayoutPanelMain);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "CharacterForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (600, 600);
			this.GroupBoxGeneral.ResumeLayout (false);
			this.LayoutPanelGeneral.ResumeLayout (false);
			this.LayoutPanelGeneral.PerformLayout ();
			this.GroupBoxIcon.ResumeLayout (false);
			this.GroupBoxIcon.PerformLayout ();
			this.ToolStripIconFile.ResumeLayout (false);
			this.ToolStripIconFile.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxIconLarge)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxIconSmall)).EndInit ();
			this.GroupBoxGUID.ResumeLayout (false);
			this.GroupBoxGUID.PerformLayout ();
			this.LayoutPanelMain.ResumeLayout (false);
			this.ResumeLayout (false);
			this.PerformLayout ();

		}

		#endregion

		private System.Windows.Forms.GroupBox GroupBoxGeneral;
		private System.Windows.Forms.TableLayoutPanel LayoutPanelGeneral;
		private System.Windows.Forms.Label LabelName;
		private System.Windows.Forms.Label LabelDescription;
		private System.Windows.Forms.Label LabelExtra;
		private DoubleAgent.TextBoxEx TextBoxName;
		private System.Windows.Forms.TextBox TextBoxDescription;
		private System.Windows.Forms.TextBox TextBoxExtra;
		private System.Windows.Forms.GroupBox GroupBoxIcon;
		private System.Windows.Forms.GroupBox GroupBoxGUID;
		private System.Windows.Forms.TableLayoutPanel LayoutPanelMain;
		private System.Windows.Forms.Button ButtonNewGUID;
		private System.Windows.Forms.PictureBox PictureBoxIconSmall;
		private DoubleAgent.TextBoxEx TextBoxIconFile;
		private DoubleAgent.ToolStripEx ToolStripIconFile;
		private System.Windows.Forms.ToolStripButton ButtonIconImport;
		private System.Windows.Forms.TextBox TextBoxGUID;
		private DoubleAgent.ListViewEx ListViewLanguage;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ToolStripButton ButtonIconExport;
		private System.Windows.Forms.PictureBox PictureBoxIconLarge;
	}
}
