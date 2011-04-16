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
	partial class FrameForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (FrameForm));
			this.LabelFrameName = new System.Windows.Forms.Label ();
			this.PanelOverlayOffset = new System.Windows.Forms.Panel ();
			this.LabelOffsetX = new System.Windows.Forms.Label ();
			this.NumericOffsetX = new DoubleAgent.NumericUpDownEx ();
			this.LabelOffsetY = new System.Windows.Forms.Label ();
			this.NumericOffsetY = new DoubleAgent.NumericUpDownEx ();
			this.TextBoxFrameName = new System.Windows.Forms.TextBox ();
			this.GroupBoxImages = new System.Windows.Forms.GroupBox ();
			this.TableLayoutImages = new System.Windows.Forms.TableLayoutPanel ();
			this.FlowLayoutRight = new System.Windows.Forms.FlowLayoutPanel ();
			this.TableLayoutSample = new System.Windows.Forms.TableLayoutPanel ();
			this.PanelImageClip = new System.Windows.Forms.Panel ();
			this.PictureBoxImageSample = new AgentCharacterEditor.PictureBoxSample ();
			this.ToolStripShiftUp = new DoubleAgent.ToolStripEx ();
			this.ButtonShiftUp = new DoubleAgent.ToolStripButtonEx ();
			this.ToolStripShiftRight = new DoubleAgent.ToolStripEx ();
			this.ButtonShiftRight = new DoubleAgent.ToolStripButtonEx ();
			this.ToolStripShiftDown = new DoubleAgent.ToolStripEx ();
			this.ButtonShiftDown = new DoubleAgent.ToolStripButtonEx ();
			this.ToolStripShiftLeft = new DoubleAgent.ToolStripEx ();
			this.ButtonShiftLeft = new DoubleAgent.ToolStripButtonEx ();
			this.CheckBoxTransparent = new System.Windows.Forms.CheckBox ();
			this.PanelImagesLeft = new System.Windows.Forms.Panel ();
			this.ListViewImages = new DoubleAgent.ListViewEx ();
			this.ColumnHeaderImage = new System.Windows.Forms.ColumnHeader ();
			this.ColumnHeaderPosition = new System.Windows.Forms.ColumnHeader ();
			this.ColumnHeaderSize = new System.Windows.Forms.ColumnHeader ();
			this.ColumnHeaderPath = new System.Windows.Forms.ColumnHeader ();
			this.ToolStripImages = new DoubleAgent.ToolStripEx ();
			this.ButtonAdd = new System.Windows.Forms.ToolStripButton ();
			this.ButtonDelete = new System.Windows.Forms.ToolStripButton ();
			this.ButtonOpen = new System.Windows.Forms.ToolStripButton ();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator ();
			this.ButtonMoveUp = new System.Windows.Forms.ToolStripButton ();
			this.ButtonMoveDown = new System.Windows.Forms.ToolStripButton ();
			this.LabelDurationUnits = new System.Windows.Forms.Label ();
			this.LabelDuration = new System.Windows.Forms.Label ();
			this.GroupBoxSound = new System.Windows.Forms.GroupBox ();
			this.ToolStripSound = new DoubleAgent.ToolStripEx ();
			this.ButtonSoundPreview = new System.Windows.Forms.ToolStripButton ();
			this.ToolStripSoundFile = new DoubleAgent.ToolStripEx ();
			this.ButtonSoundImport = new System.Windows.Forms.ToolStripButton ();
			this.ButtonSoundExport = new System.Windows.Forms.ToolStripButton ();
			this.TextBoxSoundFile = new DoubleAgent.TextBoxEx ();
			this.TableLayoutMain = new System.Windows.Forms.TableLayoutPanel ();
			this.PictureBoxFrameSample = new AgentCharacterEditor.PictureBoxSample ();
			this.NumericDuration = new DoubleAgent.NumericUpDownEx ();
			this.PanelOverlayOffset.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetX)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetY)).BeginInit ();
			this.GroupBoxImages.SuspendLayout ();
			this.TableLayoutImages.SuspendLayout ();
			this.FlowLayoutRight.SuspendLayout ();
			this.TableLayoutSample.SuspendLayout ();
			this.PanelImageClip.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxImageSample)).BeginInit ();
			this.ToolStripShiftUp.SuspendLayout ();
			this.ToolStripShiftRight.SuspendLayout ();
			this.ToolStripShiftDown.SuspendLayout ();
			this.ToolStripShiftLeft.SuspendLayout ();
			this.PanelImagesLeft.SuspendLayout ();
			this.ToolStripImages.SuspendLayout ();
			this.GroupBoxSound.SuspendLayout ();
			this.ToolStripSound.SuspendLayout ();
			this.ToolStripSoundFile.SuspendLayout ();
			this.TableLayoutMain.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxFrameSample)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericDuration)).BeginInit ();
			this.SuspendLayout ();
			// 
			// LabelFrameName
			// 
			this.LabelFrameName.AutoSize = true;
			this.LabelFrameName.Location = new System.Drawing.Point (9, 6);
			this.LabelFrameName.Name = "LabelFrameName";
			this.LabelFrameName.Size = new System.Drawing.Size (48, 17);
			this.LabelFrameName.TabIndex = 1;
			this.LabelFrameName.Text = "Frame";
			// 
			// PanelOverlayOffset
			// 
			this.PanelOverlayOffset.AutoSize = true;
			this.PanelOverlayOffset.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelOverlayOffset.Controls.Add (this.LabelOffsetX);
			this.PanelOverlayOffset.Controls.Add (this.NumericOffsetX);
			this.PanelOverlayOffset.Controls.Add (this.LabelOffsetY);
			this.PanelOverlayOffset.Controls.Add (this.NumericOffsetY);
			this.PanelOverlayOffset.Location = new System.Drawing.Point (12, 29);
			this.PanelOverlayOffset.Margin = new System.Windows.Forms.Padding (0, 29, 0, 12);
			this.PanelOverlayOffset.Name = "PanelOverlayOffset";
			this.PanelOverlayOffset.Size = new System.Drawing.Size (236, 28);
			this.PanelOverlayOffset.TabIndex = 0;
			// 
			// LabelOffsetX
			// 
			this.LabelOffsetX.AutoSize = true;
			this.LabelOffsetX.Location = new System.Drawing.Point (3, 5);
			this.LabelOffsetX.Name = "LabelOffsetX";
			this.LabelOffsetX.Size = new System.Drawing.Size (59, 17);
			this.LabelOffsetX.TabIndex = 0;
			this.LabelOffsetX.Text = "X Offset";
			// 
			// NumericOffsetX
			// 
			this.NumericOffsetX.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericOffsetX.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericOffsetX.Highlighted = false;
			this.NumericOffsetX.Location = new System.Drawing.Point (68, 3);
			this.NumericOffsetX.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericOffsetX.Minimum = new decimal (new int[] {
            10000,
            0,
            0,
            -2147483648});
			this.NumericOffsetX.Name = "NumericOffsetX";
			this.NumericOffsetX.Size = new System.Drawing.Size (47, 22);
			this.NumericOffsetX.TabIndex = 1;
			this.NumericOffsetX.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericOffsetX.Validated += new System.EventHandler (this.NumericOffsetX_Validated);
			// 
			// LabelOffsetY
			// 
			this.LabelOffsetY.AutoSize = true;
			this.LabelOffsetY.Location = new System.Drawing.Point (121, 5);
			this.LabelOffsetY.Name = "LabelOffsetY";
			this.LabelOffsetY.Size = new System.Drawing.Size (59, 17);
			this.LabelOffsetY.TabIndex = 2;
			this.LabelOffsetY.Text = "Y Offset";
			// 
			// NumericOffsetY
			// 
			this.NumericOffsetY.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericOffsetY.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericOffsetY.Highlighted = false;
			this.NumericOffsetY.Location = new System.Drawing.Point (186, 3);
			this.NumericOffsetY.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericOffsetY.Minimum = new decimal (new int[] {
            10000,
            0,
            0,
            -2147483648});
			this.NumericOffsetY.Name = "NumericOffsetY";
			this.NumericOffsetY.Size = new System.Drawing.Size (47, 22);
			this.NumericOffsetY.TabIndex = 3;
			this.NumericOffsetY.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericOffsetY.Validated += new System.EventHandler (this.NumericOffsetY_Validated);
			// 
			// TextBoxFrameName
			// 
			this.TextBoxFrameName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxFrameName.Location = new System.Drawing.Point (140, 3);
			this.TextBoxFrameName.MaximumSize = new System.Drawing.Size (600, 50);
			this.TextBoxFrameName.Name = "TextBoxFrameName";
			this.TextBoxFrameName.ReadOnly = true;
			this.TextBoxFrameName.Size = new System.Drawing.Size (551, 22);
			this.TextBoxFrameName.TabIndex = 2;
			// 
			// GroupBoxImages
			// 
			this.GroupBoxImages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxImages.Controls.Add (this.TableLayoutImages);
			this.GroupBoxImages.Location = new System.Drawing.Point (0, 137);
			this.GroupBoxImages.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxImages.Name = "GroupBoxImages";
			this.GroupBoxImages.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxImages.Size = new System.Drawing.Size (682, 377);
			this.GroupBoxImages.TabIndex = 0;
			this.GroupBoxImages.TabStop = false;
			this.GroupBoxImages.Text = "Images";
			// 
			// TableLayoutImages
			// 
			this.TableLayoutImages.ColumnCount = 2;
			this.TableLayoutImages.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 60F));
			this.TableLayoutImages.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 40F));
			this.TableLayoutImages.Controls.Add (this.FlowLayoutRight, 1, 0);
			this.TableLayoutImages.Controls.Add (this.PanelImagesLeft, 0, 0);
			this.TableLayoutImages.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TableLayoutImages.Location = new System.Drawing.Point (6, 24);
			this.TableLayoutImages.Margin = new System.Windows.Forms.Padding (0);
			this.TableLayoutImages.Name = "TableLayoutImages";
			this.TableLayoutImages.RowCount = 1;
			this.TableLayoutImages.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutImages.Size = new System.Drawing.Size (670, 347);
			this.TableLayoutImages.TabIndex = 0;
			// 
			// FlowLayoutRight
			// 
			this.FlowLayoutRight.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayoutRight.Controls.Add (this.PanelOverlayOffset);
			this.FlowLayoutRight.Controls.Add (this.TableLayoutSample);
			this.FlowLayoutRight.Controls.Add (this.CheckBoxTransparent);
			this.FlowLayoutRight.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.FlowLayoutRight.Location = new System.Drawing.Point (402, 0);
			this.FlowLayoutRight.Margin = new System.Windows.Forms.Padding (0);
			this.FlowLayoutRight.Name = "FlowLayoutRight";
			this.FlowLayoutRight.Padding = new System.Windows.Forms.Padding (12, 0, 6, 6);
			this.FlowLayoutRight.Size = new System.Drawing.Size (268, 347);
			this.FlowLayoutRight.TabIndex = 1;
			this.FlowLayoutRight.WrapContents = false;
			// 
			// TableLayoutSample
			// 
			this.TableLayoutSample.AutoSize = true;
			this.TableLayoutSample.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.TableLayoutSample.ColumnCount = 3;
			this.TableLayoutSample.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Absolute, 28F));
			this.TableLayoutSample.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle ());
			this.TableLayoutSample.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Absolute, 28F));
			this.TableLayoutSample.Controls.Add (this.PanelImageClip, 1, 1);
			this.TableLayoutSample.Controls.Add (this.ToolStripShiftUp, 1, 0);
			this.TableLayoutSample.Controls.Add (this.ToolStripShiftRight, 2, 1);
			this.TableLayoutSample.Controls.Add (this.ToolStripShiftDown, 1, 2);
			this.TableLayoutSample.Controls.Add (this.ToolStripShiftLeft, 0, 1);
			this.TableLayoutSample.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.TableLayoutSample.Location = new System.Drawing.Point (12, 69);
			this.TableLayoutSample.Margin = new System.Windows.Forms.Padding (0);
			this.TableLayoutSample.Name = "TableLayoutSample";
			this.TableLayoutSample.RowCount = 3;
			this.TableLayoutSample.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 28F));
			this.TableLayoutSample.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.TableLayoutSample.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 28F));
			this.TableLayoutSample.Size = new System.Drawing.Size (186, 186);
			this.TableLayoutSample.TabIndex = 1;
			// 
			// PanelImageClip
			// 
			this.PanelImageClip.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.PanelImageClip.BackColor = System.Drawing.SystemColors.ControlDark;
			this.PanelImageClip.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PanelImageClip.Controls.Add (this.PictureBoxImageSample);
			this.PanelImageClip.Location = new System.Drawing.Point (28, 28);
			this.PanelImageClip.Margin = new System.Windows.Forms.Padding (0);
			this.PanelImageClip.Name = "PanelImageClip";
			this.PanelImageClip.Size = new System.Drawing.Size (130, 130);
			this.PanelImageClip.TabIndex = 4;
			// 
			// PictureBoxImageSample
			// 
			this.PictureBoxImageSample.BackColor = System.Drawing.SystemColors.Window;
			this.PictureBoxImageSample.Location = new System.Drawing.Point (0, 0);
			this.PictureBoxImageSample.Margin = new System.Windows.Forms.Padding (0);
			this.PictureBoxImageSample.Name = "PictureBoxImageSample";
			this.PictureBoxImageSample.Size = new System.Drawing.Size (128, 128);
			this.PictureBoxImageSample.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.PictureBoxImageSample.TabIndex = 2;
			this.PictureBoxImageSample.TabStop = false;
			// 
			// ToolStripShiftUp
			// 
			this.ToolStripShiftUp.AllowMerge = false;
			this.ToolStripShiftUp.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripShiftUp.AutoSize = false;
			this.ToolStripShiftUp.BackColor = System.Drawing.SystemColors.Control;
			this.ToolStripShiftUp.CanOverflow = false;
			this.ToolStripShiftUp.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripShiftUp.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripShiftUp.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftUp});
			this.ToolStripShiftUp.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftUp.Location = new System.Drawing.Point (28, 0);
			this.ToolStripShiftUp.MaximumSize = new System.Drawing.Size (10000, 23);
			this.ToolStripShiftUp.MinimumSize = new System.Drawing.Size (0, 23);
			this.ToolStripShiftUp.Name = "ToolStripShiftUp";
			this.ToolStripShiftUp.Padding = new System.Windows.Forms.Padding (0);
			this.ToolStripShiftUp.Size = new System.Drawing.Size (130, 23);
			this.ToolStripShiftUp.TabIndex = 0;
			// 
			// ButtonShiftUp
			// 
			this.ButtonShiftUp.AutoSize = false;
			this.ButtonShiftUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftUp.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonShiftUp.Image")));
			this.ButtonShiftUp.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftUp.Margin = new System.Windows.Forms.Padding (0);
			this.ButtonShiftUp.Name = "ButtonShiftUp";
			this.ButtonShiftUp.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftUp.RepeatEnabled = true;
			this.ButtonShiftUp.Size = new System.Drawing.Size (128, 23);
			this.ButtonShiftUp.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler (this.ButtonShiftUp_RepeatEnd);
			this.ButtonShiftUp.Click += new System.EventHandler (this.ButtonShiftUp_Click);
			// 
			// ToolStripShiftRight
			// 
			this.ToolStripShiftRight.AllowMerge = false;
			this.ToolStripShiftRight.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripShiftRight.AutoSize = false;
			this.ToolStripShiftRight.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripShiftRight.CanOverflow = false;
			this.ToolStripShiftRight.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripShiftRight.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripShiftRight.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftRight});
			this.ToolStripShiftRight.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftRight.Location = new System.Drawing.Point (163, 28);
			this.ToolStripShiftRight.MaximumSize = new System.Drawing.Size (23, 10000);
			this.ToolStripShiftRight.MinimumSize = new System.Drawing.Size (23, 0);
			this.ToolStripShiftRight.Name = "ToolStripShiftRight";
			this.ToolStripShiftRight.Padding = new System.Windows.Forms.Padding (0);
			this.ToolStripShiftRight.Size = new System.Drawing.Size (23, 130);
			this.ToolStripShiftRight.TabIndex = 2;
			// 
			// ButtonShiftRight
			// 
			this.ButtonShiftRight.AutoSize = false;
			this.ButtonShiftRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftRight.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonShiftRight.Image")));
			this.ButtonShiftRight.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftRight.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftRight.Margin = new System.Windows.Forms.Padding (0);
			this.ButtonShiftRight.Name = "ButtonShiftRight";
			this.ButtonShiftRight.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftRight.RepeatEnabled = true;
			this.ButtonShiftRight.Size = new System.Drawing.Size (23, 128);
			this.ButtonShiftRight.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler (this.ButtonShiftRight_RepeatEnd);
			this.ButtonShiftRight.Click += new System.EventHandler (this.ButtonShiftRight_Click);
			// 
			// ToolStripShiftDown
			// 
			this.ToolStripShiftDown.AllowMerge = false;
			this.ToolStripShiftDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripShiftDown.AutoSize = false;
			this.ToolStripShiftDown.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripShiftDown.CanOverflow = false;
			this.ToolStripShiftDown.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripShiftDown.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripShiftDown.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftDown});
			this.ToolStripShiftDown.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftDown.Location = new System.Drawing.Point (28, 163);
			this.ToolStripShiftDown.MaximumSize = new System.Drawing.Size (10000, 23);
			this.ToolStripShiftDown.MinimumSize = new System.Drawing.Size (0, 23);
			this.ToolStripShiftDown.Name = "ToolStripShiftDown";
			this.ToolStripShiftDown.Padding = new System.Windows.Forms.Padding (0);
			this.ToolStripShiftDown.Size = new System.Drawing.Size (130, 23);
			this.ToolStripShiftDown.TabIndex = 3;
			// 
			// ButtonShiftDown
			// 
			this.ButtonShiftDown.AutoSize = false;
			this.ButtonShiftDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftDown.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonShiftDown.Image")));
			this.ButtonShiftDown.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftDown.Margin = new System.Windows.Forms.Padding (0);
			this.ButtonShiftDown.Name = "ButtonShiftDown";
			this.ButtonShiftDown.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftDown.RepeatEnabled = true;
			this.ButtonShiftDown.Size = new System.Drawing.Size (128, 23);
			this.ButtonShiftDown.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler (this.ButtonShiftDown_RepeatEnd);
			this.ButtonShiftDown.Click += new System.EventHandler (this.ButtonShiftDown_Click);
			// 
			// ToolStripShiftLeft
			// 
			this.ToolStripShiftLeft.AllowMerge = false;
			this.ToolStripShiftLeft.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.ToolStripShiftLeft.AutoSize = false;
			this.ToolStripShiftLeft.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripShiftLeft.CanOverflow = false;
			this.ToolStripShiftLeft.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripShiftLeft.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripShiftLeft.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftLeft});
			this.ToolStripShiftLeft.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftLeft.Location = new System.Drawing.Point (0, 28);
			this.ToolStripShiftLeft.MaximumSize = new System.Drawing.Size (23, 10000);
			this.ToolStripShiftLeft.MinimumSize = new System.Drawing.Size (23, 0);
			this.ToolStripShiftLeft.Name = "ToolStripShiftLeft";
			this.ToolStripShiftLeft.Padding = new System.Windows.Forms.Padding (0);
			this.ToolStripShiftLeft.Size = new System.Drawing.Size (23, 130);
			this.ToolStripShiftLeft.TabIndex = 1;
			// 
			// ButtonShiftLeft
			// 
			this.ButtonShiftLeft.AutoSize = false;
			this.ButtonShiftLeft.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftLeft.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonShiftLeft.Image")));
			this.ButtonShiftLeft.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftLeft.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftLeft.Margin = new System.Windows.Forms.Padding (0);
			this.ButtonShiftLeft.Name = "ButtonShiftLeft";
			this.ButtonShiftLeft.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftLeft.RepeatEnabled = true;
			this.ButtonShiftLeft.Size = new System.Drawing.Size (23, 128);
			this.ButtonShiftLeft.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler (this.ButtonShiftLeft_RepeatEnd);
			this.ButtonShiftLeft.Click += new System.EventHandler (this.ButtonShiftLeft_Click);
			// 
			// CheckBoxTransparent
			// 
			this.CheckBoxTransparent.AutoSize = true;
			this.CheckBoxTransparent.Location = new System.Drawing.Point (15, 267);
			this.CheckBoxTransparent.Margin = new System.Windows.Forms.Padding (3, 12, 3, 3);
			this.CheckBoxTransparent.Name = "CheckBoxTransparent";
			this.CheckBoxTransparent.Size = new System.Drawing.Size (151, 21);
			this.CheckBoxTransparent.TabIndex = 2;
			this.CheckBoxTransparent.Text = "Show transparency";
			this.CheckBoxTransparent.UseVisualStyleBackColor = true;
			this.CheckBoxTransparent.CheckedChanged += new System.EventHandler (this.CheckBoxTransparent_CheckedChanged);
			// 
			// PanelImagesLeft
			// 
			this.PanelImagesLeft.Controls.Add (this.ListViewImages);
			this.PanelImagesLeft.Controls.Add (this.ToolStripImages);
			this.PanelImagesLeft.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelImagesLeft.Location = new System.Drawing.Point (0, 0);
			this.PanelImagesLeft.Margin = new System.Windows.Forms.Padding (0);
			this.PanelImagesLeft.Name = "PanelImagesLeft";
			this.PanelImagesLeft.Size = new System.Drawing.Size (402, 347);
			this.PanelImagesLeft.TabIndex = 2;
			// 
			// ListViewImages
			// 
			this.ListViewImages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewImages.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.ColumnHeaderImage,
            this.ColumnHeaderPosition,
            this.ColumnHeaderSize,
            this.ColumnHeaderPath});
			this.ListViewImages.FullRowSelect = true;
			this.ListViewImages.GridLines = true;
			this.ListViewImages.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.ListViewImages.HideSelection = false;
			this.ListViewImages.Location = new System.Drawing.Point (0, 29);
			this.ListViewImages.MultiSelect = false;
			this.ListViewImages.Name = "ListViewImages";
			this.ListViewImages.SelectedIndex = -1;
			this.ListViewImages.SelectedItem = null;
			this.ListViewImages.ShowGroups = false;
			this.ListViewImages.ShowItemToolTips = true;
			this.ListViewImages.Size = new System.Drawing.Size (402, 318);
			this.ListViewImages.TabIndex = 1;
			this.ListViewImages.UseCompatibleStateImageBehavior = false;
			this.ListViewImages.View = System.Windows.Forms.View.Details;
			this.ListViewImages.ItemActivate += new System.EventHandler (this.ListViewImages_ItemActivate);
			this.ListViewImages.SelectedIndexChanged += new System.EventHandler (this.ListViewImages_SelectedIndexChanged);
			// 
			// ColumnHeaderImage
			// 
			this.ColumnHeaderImage.Text = "Image";
			this.ColumnHeaderImage.Width = 100;
			// 
			// ColumnHeaderPosition
			// 
			this.ColumnHeaderPosition.Text = "Offset";
			this.ColumnHeaderPosition.Width = 90;
			// 
			// ColumnHeaderSize
			// 
			this.ColumnHeaderSize.Text = "Dimensions";
			this.ColumnHeaderSize.Width = 90;
			// 
			// ColumnHeaderPath
			// 
			this.ColumnHeaderPath.Text = "Path";
			this.ColumnHeaderPath.Width = 100;
			// 
			// ToolStripImages
			// 
			this.ToolStripImages.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripImages.CanOverflow = false;
			this.ToolStripImages.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripImages.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripImages.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonDelete,
            this.ButtonOpen,
            this.toolStripSeparator1,
            this.ButtonMoveUp,
            this.ButtonMoveDown});
			this.ToolStripImages.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripImages.Location = new System.Drawing.Point (0, 0);
			this.ToolStripImages.Name = "ToolStripImages";
			this.ToolStripImages.Size = new System.Drawing.Size (124, 26);
			this.ToolStripImages.TabIndex = 0;
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonAdd.Image")));
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonAdd.Size = new System.Drawing.Size (23, 23);
			this.ButtonAdd.Text = "Add";
			this.ButtonAdd.Click += new System.EventHandler (this.ButtonAdd_Click);
			// 
			// ButtonDelete
			// 
			this.ButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonDelete.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonDelete.Image")));
			this.ButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonDelete.Name = "ButtonDelete";
			this.ButtonDelete.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonDelete.Size = new System.Drawing.Size (23, 23);
			this.ButtonDelete.Text = "Remove";
			this.ButtonDelete.Click += new System.EventHandler (this.ButtonDelete_Click);
			// 
			// ButtonOpen
			// 
			this.ButtonOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonOpen.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonOpen.Image")));
			this.ButtonOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonOpen.Name = "ButtonOpen";
			this.ButtonOpen.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonOpen.Size = new System.Drawing.Size (23, 23);
			this.ButtonOpen.Text = "Open image file";
			this.ButtonOpen.Click += new System.EventHandler (this.ButtonOpen_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size (6, 26);
			// 
			// ButtonMoveUp
			// 
			this.ButtonMoveUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveUp.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonMoveUp.Image")));
			this.ButtonMoveUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveUp.Name = "ButtonMoveUp";
			this.ButtonMoveUp.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonMoveUp.Size = new System.Drawing.Size (23, 23);
			this.ButtonMoveUp.Text = "Move up";
			this.ButtonMoveUp.Click += new System.EventHandler (this.ButtonMoveUp_Click);
			// 
			// ButtonMoveDown
			// 
			this.ButtonMoveDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveDown.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonMoveDown.Image")));
			this.ButtonMoveDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveDown.Name = "ButtonMoveDown";
			this.ButtonMoveDown.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonMoveDown.Size = new System.Drawing.Size (23, 23);
			this.ButtonMoveDown.Text = "Move down";
			this.ButtonMoveDown.Click += new System.EventHandler (this.ButtonMoveDown_Click);
			// 
			// LabelDurationUnits
			// 
			this.LabelDurationUnits.AutoSize = true;
			this.LabelDurationUnits.Location = new System.Drawing.Point (206, 36);
			this.LabelDurationUnits.Name = "LabelDurationUnits";
			this.LabelDurationUnits.Size = new System.Drawing.Size (111, 17);
			this.LabelDurationUnits.TabIndex = 4;
			this.LabelDurationUnits.Text = "(1/100 seconds)";
			// 
			// LabelDuration
			// 
			this.LabelDuration.AutoSize = true;
			this.LabelDuration.Location = new System.Drawing.Point (9, 36);
			this.LabelDuration.Name = "LabelDuration";
			this.LabelDuration.Size = new System.Drawing.Size (62, 17);
			this.LabelDuration.TabIndex = 2;
			this.LabelDuration.Text = "Duration";
			// 
			// GroupBoxSound
			// 
			this.GroupBoxSound.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxSound.Controls.Add (this.ToolStripSound);
			this.GroupBoxSound.Controls.Add (this.ToolStripSoundFile);
			this.GroupBoxSound.Controls.Add (this.TextBoxSoundFile);
			this.GroupBoxSound.Location = new System.Drawing.Point (9, 585);
			this.GroupBoxSound.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxSound.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxSound.Name = "GroupBoxSound";
			this.GroupBoxSound.Padding = new System.Windows.Forms.Padding (6, 9, 6, 6);
			this.GroupBoxSound.Size = new System.Drawing.Size (682, 62);
			this.GroupBoxSound.TabIndex = 1;
			this.GroupBoxSound.TabStop = false;
			this.GroupBoxSound.Text = "Sound Effect";
			// 
			// ToolStripSound
			// 
			this.ToolStripSound.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripSound.CanOverflow = false;
			this.ToolStripSound.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripSound.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripSound.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonSoundPreview});
			this.ToolStripSound.Location = new System.Drawing.Point (6, 25);
			this.ToolStripSound.Name = "ToolStripSound";
			this.ToolStripSound.Size = new System.Drawing.Size (26, 26);
			this.ToolStripSound.TabIndex = 0;
			// 
			// ButtonSoundPreview
			// 
			this.ButtonSoundPreview.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonSoundPreview.Enabled = false;
			this.ButtonSoundPreview.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonSoundPreview.Image")));
			this.ButtonSoundPreview.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonSoundPreview.Name = "ButtonSoundPreview";
			this.ButtonSoundPreview.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonSoundPreview.Size = new System.Drawing.Size (23, 23);
			this.ButtonSoundPreview.Text = "Preview";
			this.ButtonSoundPreview.Click += new System.EventHandler (this.ButtonSoundPreview_Click);
			// 
			// ToolStripSoundFile
			// 
			this.ToolStripSoundFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripSoundFile.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripSoundFile.CanOverflow = false;
			this.ToolStripSoundFile.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripSoundFile.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripSoundFile.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonSoundImport,
            this.ButtonSoundExport});
			this.ToolStripSoundFile.Location = new System.Drawing.Point (627, 25);
			this.ToolStripSoundFile.Name = "ToolStripSoundFile";
			this.ToolStripSoundFile.Size = new System.Drawing.Size (49, 26);
			this.ToolStripSoundFile.TabIndex = 2;
			// 
			// ButtonSoundImport
			// 
			this.ButtonSoundImport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonSoundImport.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonSoundImport.Image")));
			this.ButtonSoundImport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonSoundImport.Name = "ButtonSoundImport";
			this.ButtonSoundImport.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonSoundImport.Size = new System.Drawing.Size (23, 23);
			this.ButtonSoundImport.Text = "Open sound file";
			this.ButtonSoundImport.Click += new System.EventHandler (this.ButtonSoundImport_Click);
			// 
			// ButtonSoundExport
			// 
			this.ButtonSoundExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonSoundExport.Enabled = false;
			this.ButtonSoundExport.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonSoundExport.Image")));
			this.ButtonSoundExport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonSoundExport.Name = "ButtonSoundExport";
			this.ButtonSoundExport.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonSoundExport.Size = new System.Drawing.Size (23, 23);
			this.ButtonSoundExport.Text = "Save sound";
			// 
			// TextBoxSoundFile
			// 
			this.TextBoxSoundFile.AcceptsReturn = true;
			this.TextBoxSoundFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxSoundFile.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
			this.TextBoxSoundFile.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
			this.TextBoxSoundFile.Location = new System.Drawing.Point (35, 27);
			this.TextBoxSoundFile.Name = "TextBoxSoundFile";
			this.TextBoxSoundFile.Size = new System.Drawing.Size (589, 22);
			this.TextBoxSoundFile.TabIndex = 1;
			this.TextBoxSoundFile.Validated += new System.EventHandler (this.TextBoxSoundFile_Validated);
			// 
			// TableLayoutMain
			// 
			this.TableLayoutMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TableLayoutMain.ColumnCount = 1;
			this.TableLayoutMain.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutMain.Controls.Add (this.GroupBoxImages, 0, 1);
			this.TableLayoutMain.Controls.Add (this.PictureBoxFrameSample, 0, 0);
			this.TableLayoutMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.TableLayoutMain.Location = new System.Drawing.Point (9, 68);
			this.TableLayoutMain.Margin = new System.Windows.Forms.Padding (0);
			this.TableLayoutMain.Name = "TableLayoutMain";
			this.TableLayoutMain.RowCount = 2;
			this.TableLayoutMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.TableLayoutMain.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutMain.Size = new System.Drawing.Size (682, 514);
			this.TableLayoutMain.TabIndex = 0;
			// 
			// PictureBoxFrameSample
			// 
			this.PictureBoxFrameSample.BackColor = System.Drawing.SystemColors.Window;
			this.PictureBoxFrameSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PictureBoxFrameSample.Location = new System.Drawing.Point (132, 0);
			this.PictureBoxFrameSample.Margin = new System.Windows.Forms.Padding (132, 0, 0, 0);
			this.PictureBoxFrameSample.MaximumSize = new System.Drawing.Size (258, 258);
			this.PictureBoxFrameSample.Name = "PictureBoxFrameSample";
			this.PictureBoxFrameSample.Size = new System.Drawing.Size (128, 128);
			this.PictureBoxFrameSample.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.PictureBoxFrameSample.TabIndex = 2;
			this.PictureBoxFrameSample.TabStop = false;
			// 
			// NumericDuration
			// 
			this.NumericDuration.AutoSize = true;
			this.NumericDuration.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericDuration.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericDuration.Highlighted = false;
			this.NumericDuration.Location = new System.Drawing.Point (140, 34);
			this.NumericDuration.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericDuration.Minimum = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericDuration.Name = "NumericDuration";
			this.NumericDuration.Size = new System.Drawing.Size (64, 22);
			this.NumericDuration.TabIndex = 3;
			this.NumericDuration.Value = new decimal (new int[] {
            1,
            0,
            0,
            0});
			this.NumericDuration.Validated += new System.EventHandler (this.NumericDuration_Validated);
			// 
			// FrameForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.LabelFrameName);
			this.Controls.Add (this.GroupBoxSound);
			this.Controls.Add (this.TextBoxFrameName);
			this.Controls.Add (this.LabelDuration);
			this.Controls.Add (this.LabelDurationUnits);
			this.Controls.Add (this.NumericDuration);
			this.Controls.Add (this.TableLayoutMain);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "FrameForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (700, 650);
			this.VisibleChanged += new System.EventHandler (this.FrameForm_VisibleChanged);
			this.PanelOverlayOffset.ResumeLayout (false);
			this.PanelOverlayOffset.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetX)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetY)).EndInit ();
			this.GroupBoxImages.ResumeLayout (false);
			this.TableLayoutImages.ResumeLayout (false);
			this.FlowLayoutRight.ResumeLayout (false);
			this.FlowLayoutRight.PerformLayout ();
			this.TableLayoutSample.ResumeLayout (false);
			this.PanelImageClip.ResumeLayout (false);
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxImageSample)).EndInit ();
			this.ToolStripShiftUp.ResumeLayout (false);
			this.ToolStripShiftUp.PerformLayout ();
			this.ToolStripShiftRight.ResumeLayout (false);
			this.ToolStripShiftRight.PerformLayout ();
			this.ToolStripShiftDown.ResumeLayout (false);
			this.ToolStripShiftDown.PerformLayout ();
			this.ToolStripShiftLeft.ResumeLayout (false);
			this.ToolStripShiftLeft.PerformLayout ();
			this.PanelImagesLeft.ResumeLayout (false);
			this.PanelImagesLeft.PerformLayout ();
			this.ToolStripImages.ResumeLayout (false);
			this.ToolStripImages.PerformLayout ();
			this.GroupBoxSound.ResumeLayout (false);
			this.GroupBoxSound.PerformLayout ();
			this.ToolStripSound.ResumeLayout (false);
			this.ToolStripSound.PerformLayout ();
			this.ToolStripSoundFile.ResumeLayout (false);
			this.ToolStripSoundFile.PerformLayout ();
			this.TableLayoutMain.ResumeLayout (false);
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxFrameSample)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.NumericDuration)).EndInit ();
			this.ResumeLayout (false);
			this.PerformLayout ();

		}

		#endregion

		private System.Windows.Forms.Label LabelFrameName;
		private System.Windows.Forms.TextBox TextBoxFrameName;
		private System.Windows.Forms.GroupBox GroupBoxImages;
		private System.Windows.Forms.GroupBox GroupBoxSound;
		private DoubleAgent.ToolStripEx ToolStripImages;
		private DoubleAgent.ListViewEx ListViewImages;
		private System.Windows.Forms.ColumnHeader ColumnHeaderImage;
		private System.Windows.Forms.ColumnHeader ColumnHeaderPosition;
		private System.Windows.Forms.ColumnHeader ColumnHeaderSize;
		private System.Windows.Forms.ColumnHeader ColumnHeaderPath;
		private System.Windows.Forms.ToolStripButton ButtonAdd;
		private System.Windows.Forms.ToolStripButton ButtonDelete;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton ButtonMoveUp;
		private System.Windows.Forms.ToolStripButton ButtonMoveDown;
		private DoubleAgent.TextBoxEx TextBoxSoundFile;
		private DoubleAgent.ToolStripEx ToolStripSoundFile;
		private System.Windows.Forms.ToolStripButton ButtonSoundImport;
		private System.Windows.Forms.ToolStripButton ButtonSoundExport;
		private DoubleAgent.ToolStripEx ToolStripSound;
		private System.Windows.Forms.ToolStripButton ButtonSoundPreview;
		private PictureBoxSample PictureBoxImageSample;
		private System.Windows.Forms.Label LabelOffsetY;
		private System.Windows.Forms.Label LabelOffsetX;
		private DoubleAgent.NumericUpDownEx NumericOffsetY;
		private DoubleAgent.NumericUpDownEx NumericOffsetX;
		private DoubleAgent.NumericUpDownEx NumericDuration;
		private System.Windows.Forms.Label LabelDurationUnits;
		private System.Windows.Forms.Label LabelDuration;
		private DoubleAgent.ToolStripEx ToolStripShiftUp;
		private DoubleAgent.ToolStripEx ToolStripShiftDown;
		private DoubleAgent.ToolStripEx ToolStripShiftLeft;
		private DoubleAgent.ToolStripEx ToolStripShiftRight;
		private DoubleAgent.ToolStripButtonEx ButtonShiftUp;
		private DoubleAgent.ToolStripButtonEx ButtonShiftDown;
		private DoubleAgent.ToolStripButtonEx ButtonShiftLeft;
		private DoubleAgent.ToolStripButtonEx ButtonShiftRight;
		private PictureBoxSample PictureBoxFrameSample;
		private System.Windows.Forms.TableLayoutPanel TableLayoutMain;
		private System.Windows.Forms.Panel PanelImageClip;
		private System.Windows.Forms.FlowLayoutPanel FlowLayoutRight;
		private System.Windows.Forms.Panel PanelImagesLeft;
		private System.Windows.Forms.TableLayoutPanel TableLayoutImages;
		private System.Windows.Forms.ToolStripButton ButtonOpen;
		private System.Windows.Forms.TableLayoutPanel TableLayoutSample;
		private System.Windows.Forms.Panel PanelOverlayOffset;
		private System.Windows.Forms.CheckBox CheckBoxTransparent;
	}
}
