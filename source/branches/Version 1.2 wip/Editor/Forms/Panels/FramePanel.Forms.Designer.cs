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
	partial class FramePanel
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
			System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
			this.LabelFrameName = new System.Windows.Forms.Label();
			this.PanelOverlayOffset = new System.Windows.Forms.Panel();
			this.LabelOffsetX = new System.Windows.Forms.Label();
			this.NumericOffsetX = new DoubleAgent.NumericUpDownEx();
			this.LabelOffsetY = new System.Windows.Forms.Label();
			this.NumericOffsetY = new DoubleAgent.NumericUpDownEx();
			this.TextBoxFrameName = new DoubleAgent.TextBoxEx();
			this.GroupBoxImages = new DoubleAgent.GroupBoxCompat();
			this.PanelImages = new System.Windows.Forms.TableLayoutPanel();
			this.FlowLayoutRight = new System.Windows.Forms.FlowLayoutPanel();
			this.PanelSample = new System.Windows.Forms.TableLayoutPanel();
			this.PanelImageClip = new System.Windows.Forms.Panel();
			this.PictureBoxImageSample = new AgentCharacterEditor.FrameSample();
			this.ToolStripShiftUp = new DoubleAgent.ToolStripEx();
			this.ButtonShiftUp = new DoubleAgent.ToolStripButtonEx();
			this.ToolStripShiftRight = new DoubleAgent.ToolStripEx();
			this.ButtonShiftRight = new DoubleAgent.ToolStripButtonEx();
			this.ToolStripShiftDown = new DoubleAgent.ToolStripEx();
			this.ButtonShiftDown = new DoubleAgent.ToolStripButtonEx();
			this.ToolStripShiftLeft = new DoubleAgent.ToolStripEx();
			this.ButtonShiftLeft = new DoubleAgent.ToolStripButtonEx();
			this.CheckBoxTransparent = new DoubleAgent.CheckBoxCompat();
			this.PanelImagesLeft = new System.Windows.Forms.Panel();
			this.ListViewImages = new DoubleAgent.ListViewEx();
			this.ColumnHeaderImage = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeaderPosition = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeaderSize = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeaderPath = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ToolStripImages = new DoubleAgent.ToolStripEx();
			this.ButtonAdd = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonDelete = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonChooseFile = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMoveUp = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMoveDown = new DoubleAgent.ToolStripButtonCompat();
			this.LabelDurationUnits = new System.Windows.Forms.Label();
			this.LabelDuration = new System.Windows.Forms.Label();
			this.GroupBoxSound = new DoubleAgent.GroupBoxCompat();
			this.ToolStripSound = new DoubleAgent.ToolStripEx();
			this.ButtonSoundPreview = new DoubleAgent.ToolStripButtonCompat();
			this.ToolStripSoundFile = new DoubleAgent.ToolStripEx();
			this.ButtonSoundImport = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonSoundExport = new DoubleAgent.ToolStripButtonCompat();
			this.TextBoxSoundFile = new DoubleAgent.TextBoxEx();
			this.PanelMain = new System.Windows.Forms.TableLayoutPanel();
			this.PanelTop = new System.Windows.Forms.Panel();
			this.NumericDuration = new DoubleAgent.NumericUpDownEx();
			this.PictureBoxFrameSample = new AgentCharacterEditor.FrameSample();
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.PanelOverlayOffset.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetY)).BeginInit();
			this.GroupBoxImages.SuspendLayout();
			this.PanelImages.SuspendLayout();
			this.FlowLayoutRight.SuspendLayout();
			this.PanelSample.SuspendLayout();
			this.PanelImageClip.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxImageSample)).BeginInit();
			this.ToolStripShiftUp.SuspendLayout();
			this.ToolStripShiftRight.SuspendLayout();
			this.ToolStripShiftDown.SuspendLayout();
			this.ToolStripShiftLeft.SuspendLayout();
			this.PanelImagesLeft.SuspendLayout();
			this.ToolStripImages.SuspendLayout();
			this.GroupBoxSound.SuspendLayout();
			this.ToolStripSound.SuspendLayout();
			this.ToolStripSoundFile.SuspendLayout();
			this.PanelMain.SuspendLayout();
			this.PanelTop.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericDuration)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxFrameSample)).BeginInit();
			this.SuspendLayout();
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// LabelFrameName
			// 
			this.LabelFrameName.AutoSize = true;
			this.LabelFrameName.Location = new System.Drawing.Point(0, 6);
			this.LabelFrameName.Name = "LabelFrameName";
			this.LabelFrameName.Size = new System.Drawing.Size(48, 17);
			this.LabelFrameName.TabIndex = 1;
			this.LabelFrameName.Text = "Frame";
			// 
			// PanelOverlayOffset
			// 
			this.PanelOverlayOffset.AutoSize = true;
			this.PanelOverlayOffset.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelOverlayOffset.Controls.Add(this.LabelOffsetX);
			this.PanelOverlayOffset.Controls.Add(this.NumericOffsetX);
			this.PanelOverlayOffset.Controls.Add(this.LabelOffsetY);
			this.PanelOverlayOffset.Controls.Add(this.NumericOffsetY);
			this.PanelOverlayOffset.Location = new System.Drawing.Point(12, 29);
			this.PanelOverlayOffset.Margin = new System.Windows.Forms.Padding(0, 29, 0, 12);
			this.PanelOverlayOffset.Name = "PanelOverlayOffset";
			this.PanelOverlayOffset.Size = new System.Drawing.Size(236, 28);
			this.PanelOverlayOffset.TabIndex = 0;
			// 
			// LabelOffsetX
			// 
			this.LabelOffsetX.AutoSize = true;
			this.LabelOffsetX.Location = new System.Drawing.Point(3, 5);
			this.LabelOffsetX.Name = "LabelOffsetX";
			this.LabelOffsetX.Size = new System.Drawing.Size(59, 17);
			this.LabelOffsetX.TabIndex = 0;
			this.LabelOffsetX.Text = "X Offset";
			// 
			// NumericOffsetX
			// 
			this.NumericOffsetX.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericOffsetX.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericOffsetX.Location = new System.Drawing.Point(68, 3);
			this.NumericOffsetX.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
			this.NumericOffsetX.Minimum = new decimal(new int[] {
            10000,
            0,
            0,
            -2147483648});
			this.NumericOffsetX.Name = "NumericOffsetX";
			this.NumericOffsetX.Size = new System.Drawing.Size(47, 22);
			this.NumericOffsetX.TabIndex = 1;
			this.NumericOffsetX.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
			this.NumericOffsetX.Validated += new System.EventHandler(this.NumericOffsetX_Validated);
			// 
			// LabelOffsetY
			// 
			this.LabelOffsetY.AutoSize = true;
			this.LabelOffsetY.Location = new System.Drawing.Point(121, 5);
			this.LabelOffsetY.Name = "LabelOffsetY";
			this.LabelOffsetY.Size = new System.Drawing.Size(59, 17);
			this.LabelOffsetY.TabIndex = 2;
			this.LabelOffsetY.Text = "Y Offset";
			// 
			// NumericOffsetY
			// 
			this.NumericOffsetY.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericOffsetY.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericOffsetY.Location = new System.Drawing.Point(186, 3);
			this.NumericOffsetY.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
			this.NumericOffsetY.Minimum = new decimal(new int[] {
            10000,
            0,
            0,
            -2147483648});
			this.NumericOffsetY.Name = "NumericOffsetY";
			this.NumericOffsetY.Size = new System.Drawing.Size(47, 22);
			this.NumericOffsetY.TabIndex = 3;
			this.NumericOffsetY.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
			this.NumericOffsetY.Validated += new System.EventHandler(this.NumericOffsetY_Validated);
			// 
			// TextBoxFrameName
			// 
			this.TextBoxFrameName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxFrameName.Location = new System.Drawing.Point(132, 3);
			this.TextBoxFrameName.MaximumSize = new System.Drawing.Size(600, 50);
			this.TextBoxFrameName.Name = "TextBoxFrameName";
			this.TextBoxFrameName.ReadOnly = true;
			this.TextBoxFrameName.Size = new System.Drawing.Size(550, 22);
			this.TextBoxFrameName.TabIndex = 2;
			// 
			// GroupBoxImages
			// 
			this.GroupBoxImages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxImages.Controls.Add(this.PanelImages);
			this.GroupBoxImages.Location = new System.Drawing.Point(0, 197);
			this.GroupBoxImages.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxImages.Name = "GroupBoxImages";
			this.GroupBoxImages.Padding = new System.Windows.Forms.Padding(6, 9, 6, 6);
			this.GroupBoxImages.Size = new System.Drawing.Size(682, 382);
			this.GroupBoxImages.TabIndex = 0;
			this.GroupBoxImages.TabStop = false;
			this.GroupBoxImages.Text = "Images";
			// 
			// PanelImages
			// 
			this.PanelImages.ColumnCount = 2;
			this.PanelImages.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 60F));
			this.PanelImages.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
			this.PanelImages.Controls.Add(this.FlowLayoutRight, 1, 0);
			this.PanelImages.Controls.Add(this.PanelImagesLeft, 0, 0);
			this.PanelImages.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelImages.Location = new System.Drawing.Point(6, 24);
			this.PanelImages.Margin = new System.Windows.Forms.Padding(0);
			this.PanelImages.Name = "PanelImages";
			this.PanelImages.RowCount = 1;
			this.PanelImages.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelImages.Size = new System.Drawing.Size(670, 352);
			this.PanelImages.TabIndex = 0;
			// 
			// FlowLayoutRight
			// 
			this.FlowLayoutRight.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.FlowLayoutRight.Controls.Add(this.PanelOverlayOffset);
			this.FlowLayoutRight.Controls.Add(this.PanelSample);
			this.FlowLayoutRight.Controls.Add(this.CheckBoxTransparent);
			this.FlowLayoutRight.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.FlowLayoutRight.Location = new System.Drawing.Point(402, 0);
			this.FlowLayoutRight.Margin = new System.Windows.Forms.Padding(0);
			this.FlowLayoutRight.Name = "FlowLayoutRight";
			this.FlowLayoutRight.Padding = new System.Windows.Forms.Padding(12, 0, 6, 6);
			this.FlowLayoutRight.Size = new System.Drawing.Size(268, 352);
			this.FlowLayoutRight.TabIndex = 1;
			this.FlowLayoutRight.WrapContents = false;
			// 
			// PanelSample
			// 
			this.PanelSample.AutoSize = true;
			this.PanelSample.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelSample.ColumnCount = 3;
			this.PanelSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.Controls.Add(this.PanelImageClip, 1, 1);
			this.PanelSample.Controls.Add(this.ToolStripShiftUp, 1, 0);
			this.PanelSample.Controls.Add(this.ToolStripShiftRight, 2, 1);
			this.PanelSample.Controls.Add(this.ToolStripShiftDown, 1, 2);
			this.PanelSample.Controls.Add(this.ToolStripShiftLeft, 0, 1);
			this.PanelSample.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelSample.Location = new System.Drawing.Point(12, 69);
			this.PanelSample.Margin = new System.Windows.Forms.Padding(0);
			this.PanelSample.Name = "PanelSample";
			this.PanelSample.RowCount = 3;
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.Size = new System.Drawing.Size(186, 186);
			this.PanelSample.TabIndex = 1;
			// 
			// PanelImageClip
			// 
			this.PanelImageClip.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.PanelImageClip.BackColor = System.Drawing.SystemColors.ControlDark;
			this.PanelImageClip.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PanelImageClip.Controls.Add(this.PictureBoxImageSample);
			this.PanelImageClip.Location = new System.Drawing.Point(28, 28);
			this.PanelImageClip.Margin = new System.Windows.Forms.Padding(0);
			this.PanelImageClip.Name = "PanelImageClip";
			this.PanelImageClip.Size = new System.Drawing.Size(130, 130);
			this.PanelImageClip.TabIndex = 4;
			// 
			// PictureBoxImageSample
			// 
			this.PictureBoxImageSample.BackColor = System.Drawing.SystemColors.Window;
			this.PictureBoxImageSample.Location = new System.Drawing.Point(0, 0);
			this.PictureBoxImageSample.Margin = new System.Windows.Forms.Padding(0);
			this.PictureBoxImageSample.Name = "PictureBoxImageSample";
			this.PictureBoxImageSample.Size = new System.Drawing.Size(128, 128);
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
			this.ToolStripShiftUp.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftUp});
			this.ToolStripShiftUp.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftUp.Location = new System.Drawing.Point(28, 0);
			this.ToolStripShiftUp.MaximumSize = new System.Drawing.Size(10000, 23);
			this.ToolStripShiftUp.MinimumSize = new System.Drawing.Size(0, 23);
			this.ToolStripShiftUp.Name = "ToolStripShiftUp";
			this.ToolStripShiftUp.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftUp.Size = new System.Drawing.Size(130, 23);
			this.ToolStripShiftUp.TabIndex = 0;
			// 
			// ButtonShiftUp
			// 
			this.ButtonShiftUp.AutoSize = false;
			this.ButtonShiftUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftUp.Image = global::AgentCharacterEditor.Properties.Resources.ImgShiftUp;
			this.ButtonShiftUp.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftUp.Margin = new System.Windows.Forms.Padding(0);
			this.ButtonShiftUp.Name = "ButtonShiftUp";
			this.ButtonShiftUp.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftUp.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonShiftUp.RepeatEnabled = true;
			this.ButtonShiftUp.Size = new System.Drawing.Size(24, 24);
			this.ButtonShiftUp.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler(this.ButtonShiftUp_RepeatEnd);
			this.ButtonShiftUp.Click += new System.EventHandler(this.ButtonShiftUp_Click);
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
			this.ToolStripShiftRight.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftRight});
			this.ToolStripShiftRight.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftRight.Location = new System.Drawing.Point(163, 28);
			this.ToolStripShiftRight.MaximumSize = new System.Drawing.Size(23, 10000);
			this.ToolStripShiftRight.MinimumSize = new System.Drawing.Size(23, 0);
			this.ToolStripShiftRight.Name = "ToolStripShiftRight";
			this.ToolStripShiftRight.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftRight.Size = new System.Drawing.Size(23, 130);
			this.ToolStripShiftRight.TabIndex = 2;
			// 
			// ButtonShiftRight
			// 
			this.ButtonShiftRight.AutoSize = false;
			this.ButtonShiftRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftRight.Image = global::AgentCharacterEditor.Properties.Resources.ImgShiftRight;
			this.ButtonShiftRight.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftRight.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftRight.Margin = new System.Windows.Forms.Padding(0);
			this.ButtonShiftRight.Name = "ButtonShiftRight";
			this.ButtonShiftRight.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftRight.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonShiftRight.RepeatEnabled = true;
			this.ButtonShiftRight.Size = new System.Drawing.Size(24, 24);
			this.ButtonShiftRight.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler(this.ButtonShiftRight_RepeatEnd);
			this.ButtonShiftRight.Click += new System.EventHandler(this.ButtonShiftRight_Click);
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
			this.ToolStripShiftDown.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftDown});
			this.ToolStripShiftDown.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftDown.Location = new System.Drawing.Point(28, 163);
			this.ToolStripShiftDown.MaximumSize = new System.Drawing.Size(10000, 23);
			this.ToolStripShiftDown.MinimumSize = new System.Drawing.Size(0, 23);
			this.ToolStripShiftDown.Name = "ToolStripShiftDown";
			this.ToolStripShiftDown.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftDown.Size = new System.Drawing.Size(130, 23);
			this.ToolStripShiftDown.TabIndex = 3;
			// 
			// ButtonShiftDown
			// 
			this.ButtonShiftDown.AutoSize = false;
			this.ButtonShiftDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftDown.Image = global::AgentCharacterEditor.Properties.Resources.ImgShiftDown;
			this.ButtonShiftDown.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftDown.Margin = new System.Windows.Forms.Padding(0);
			this.ButtonShiftDown.Name = "ButtonShiftDown";
			this.ButtonShiftDown.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftDown.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonShiftDown.RepeatEnabled = true;
			this.ButtonShiftDown.Size = new System.Drawing.Size(24, 24);
			this.ButtonShiftDown.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler(this.ButtonShiftDown_RepeatEnd);
			this.ButtonShiftDown.Click += new System.EventHandler(this.ButtonShiftDown_Click);
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
			this.ToolStripShiftLeft.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonShiftLeft});
			this.ToolStripShiftLeft.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Table;
			this.ToolStripShiftLeft.Location = new System.Drawing.Point(0, 28);
			this.ToolStripShiftLeft.MaximumSize = new System.Drawing.Size(23, 10000);
			this.ToolStripShiftLeft.MinimumSize = new System.Drawing.Size(23, 0);
			this.ToolStripShiftLeft.Name = "ToolStripShiftLeft";
			this.ToolStripShiftLeft.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftLeft.Size = new System.Drawing.Size(23, 130);
			this.ToolStripShiftLeft.TabIndex = 1;
			// 
			// ButtonShiftLeft
			// 
			this.ButtonShiftLeft.AutoSize = false;
			this.ButtonShiftLeft.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShiftLeft.Image = global::AgentCharacterEditor.Properties.Resources.ImgShiftLeft;
			this.ButtonShiftLeft.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
			this.ButtonShiftLeft.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShiftLeft.Margin = new System.Windows.Forms.Padding(0);
			this.ButtonShiftLeft.Name = "ButtonShiftLeft";
			this.ButtonShiftLeft.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
			this.ButtonShiftLeft.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonShiftLeft.RepeatEnabled = true;
			this.ButtonShiftLeft.Size = new System.Drawing.Size(24, 24);
			this.ButtonShiftLeft.RepeatEnd += new DoubleAgent.ToolStripButtonEx.RepeatEndEventHandler(this.ButtonShiftLeft_RepeatEnd);
			this.ButtonShiftLeft.Click += new System.EventHandler(this.ButtonShiftLeft_Click);
			// 
			// CheckBoxTransparent
			// 
			this.CheckBoxTransparent.AutoSize = true;
			this.CheckBoxTransparent.Location = new System.Drawing.Point(15, 267);
			this.CheckBoxTransparent.Margin = new System.Windows.Forms.Padding(3, 12, 3, 3);
			this.CheckBoxTransparent.Name = "CheckBoxTransparent";
			this.CheckBoxTransparent.Size = new System.Drawing.Size(151, 21);
			this.CheckBoxTransparent.TabIndex = 2;
			this.CheckBoxTransparent.Text = "Show transparency";
			this.CheckBoxTransparent.UseVisualStyleBackColor = true;
			this.CheckBoxTransparent.CheckedChanged += new System.EventHandler(this.CheckBoxTransparent_CheckedChanged);
			// 
			// PanelImagesLeft
			// 
			this.PanelImagesLeft.Controls.Add(this.ListViewImages);
			this.PanelImagesLeft.Controls.Add(this.ToolStripImages);
			this.PanelImagesLeft.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelImagesLeft.Location = new System.Drawing.Point(0, 0);
			this.PanelImagesLeft.Margin = new System.Windows.Forms.Padding(0);
			this.PanelImagesLeft.Name = "PanelImagesLeft";
			this.PanelImagesLeft.Size = new System.Drawing.Size(402, 352);
			this.PanelImagesLeft.TabIndex = 2;
			// 
			// ListViewImages
			// 
			this.ListViewImages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewImages.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ColumnHeaderImage,
            this.ColumnHeaderPosition,
            this.ColumnHeaderSize,
            this.ColumnHeaderPath});
			this.ListViewImages.FullRowSelect = true;
			this.ListViewImages.GridLines = true;
			this.ListViewImages.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.ListViewImages.HideSelection = false;
			this.ListViewImages.Location = new System.Drawing.Point(0, 29);
			this.ListViewImages.MultiSelect = false;
			this.ListViewImages.Name = "ListViewImages";
			this.ListViewImages.ShowGroups = false;
			this.ListViewImages.ShowItemToolTips = true;
			this.ListViewImages.Size = new System.Drawing.Size(402, 323);
			this.ListViewImages.TabIndex = 1;
			this.ListViewImages.UseCompatibleStateImageBehavior = false;
			this.ListViewImages.View = System.Windows.Forms.View.Details;
			this.ListViewImages.ItemActivate += new System.EventHandler(this.ListViewImages_ItemActivate);
			this.ListViewImages.SelectedIndexChanged += new System.EventHandler(this.ListViewImages_SelectedIndexChanged);
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
			this.ToolStripImages.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonDelete,
            this.ButtonChooseFile,
            toolStripSeparator1,
            this.ButtonMoveUp,
            this.ButtonMoveDown});
			this.ToolStripImages.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripImages.Location = new System.Drawing.Point(0, 0);
			this.ToolStripImages.Name = "ToolStripImages";
			this.ToolStripImages.Size = new System.Drawing.Size(160, 27);
			this.ToolStripImages.TabIndex = 0;
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonAdd.Size = new System.Drawing.Size(24, 24);
			this.ButtonAdd.Text = "Add a new image";
			this.ButtonAdd.Click += new System.EventHandler(this.ButtonAdd_Click);
			// 
			// ButtonDelete
			// 
			this.ButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.ButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonDelete.Name = "ButtonDelete";
			this.ButtonDelete.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonDelete.Size = new System.Drawing.Size(24, 24);
			this.ButtonDelete.Text = "Remove";
			this.ButtonDelete.Click += new System.EventHandler(this.ButtonDelete_Click);
			// 
			// ButtonChooseFile
			// 
			this.ButtonChooseFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonChooseFile.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileOpen;
			this.ButtonChooseFile.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonChooseFile.Name = "ButtonChooseFile";
			this.ButtonChooseFile.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonChooseFile.Size = new System.Drawing.Size(24, 24);
			this.ButtonChooseFile.Text = "Open image file";
			this.ButtonChooseFile.Click += new System.EventHandler(this.ButtonOpen_Click);
			// 
			// ButtonMoveUp
			// 
			this.ButtonMoveUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveUp.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveUp;
			this.ButtonMoveUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveUp.Name = "ButtonMoveUp";
			this.ButtonMoveUp.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMoveUp.Size = new System.Drawing.Size(24, 24);
			this.ButtonMoveUp.Text = "Move image up";
			this.ButtonMoveUp.Click += new System.EventHandler(this.ButtonMoveUp_Click);
			// 
			// ButtonMoveDown
			// 
			this.ButtonMoveDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveDown.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveDown;
			this.ButtonMoveDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveDown.Name = "ButtonMoveDown";
			this.ButtonMoveDown.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMoveDown.Size = new System.Drawing.Size(24, 24);
			this.ButtonMoveDown.Text = "Move image down";
			this.ButtonMoveDown.Click += new System.EventHandler(this.ButtonMoveDown_Click);
			// 
			// LabelDurationUnits
			// 
			this.LabelDurationUnits.AutoSize = true;
			this.LabelDurationUnits.Location = new System.Drawing.Point(202, 33);
			this.LabelDurationUnits.Name = "LabelDurationUnits";
			this.LabelDurationUnits.Size = new System.Drawing.Size(111, 17);
			this.LabelDurationUnits.TabIndex = 4;
			this.LabelDurationUnits.Text = "(1/100 seconds)";
			// 
			// LabelDuration
			// 
			this.LabelDuration.AutoSize = true;
			this.LabelDuration.Location = new System.Drawing.Point(0, 33);
			this.LabelDuration.Name = "LabelDuration";
			this.LabelDuration.Size = new System.Drawing.Size(62, 17);
			this.LabelDuration.TabIndex = 2;
			this.LabelDuration.Text = "Duration";
			// 
			// GroupBoxSound
			// 
			this.GroupBoxSound.Controls.Add(this.ToolStripSound);
			this.GroupBoxSound.Controls.Add(this.ToolStripSoundFile);
			this.GroupBoxSound.Controls.Add(this.TextBoxSoundFile);
			this.GroupBoxSound.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxSound.Location = new System.Drawing.Point(0, 588);
			this.GroupBoxSound.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxSound.MaximumSize = new System.Drawing.Size(730, 10000);
			this.GroupBoxSound.Name = "GroupBoxSound";
			this.GroupBoxSound.Padding = new System.Windows.Forms.Padding(6, 9, 6, 6);
			this.GroupBoxSound.Size = new System.Drawing.Size(682, 62);
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
			this.ToolStripSound.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonSoundPreview});
			this.ToolStripSound.Location = new System.Drawing.Point(6, 25);
			this.ToolStripSound.Name = "ToolStripSound";
			this.ToolStripSound.Size = new System.Drawing.Size(27, 27);
			this.ToolStripSound.TabIndex = 0;
			// 
			// ButtonSoundPreview
			// 
			this.ButtonSoundPreview.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonSoundPreview.Enabled = false;
			this.ButtonSoundPreview.Image = global::AgentCharacterEditor.Properties.Resources.ImgRun;
			this.ButtonSoundPreview.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonSoundPreview.Name = "ButtonSoundPreview";
			this.ButtonSoundPreview.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonSoundPreview.Size = new System.Drawing.Size(24, 24);
			this.ButtonSoundPreview.Text = "Preview";
			this.ButtonSoundPreview.Click += new System.EventHandler(this.ButtonSoundPreview_Click);
			// 
			// ToolStripSoundFile
			// 
			this.ToolStripSoundFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripSoundFile.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripSoundFile.CanOverflow = false;
			this.ToolStripSoundFile.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripSoundFile.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripSoundFile.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonSoundImport,
            this.ButtonSoundExport});
			this.ToolStripSoundFile.Location = new System.Drawing.Point(619, 25);
			this.ToolStripSoundFile.Name = "ToolStripSoundFile";
			this.ToolStripSoundFile.Size = new System.Drawing.Size(51, 27);
			this.ToolStripSoundFile.TabIndex = 2;
			// 
			// ButtonSoundImport
			// 
			this.ButtonSoundImport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonSoundImport.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileOpen;
			this.ButtonSoundImport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonSoundImport.Name = "ButtonSoundImport";
			this.ButtonSoundImport.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonSoundImport.Size = new System.Drawing.Size(24, 24);
			this.ButtonSoundImport.Text = "Open sound file";
			this.ButtonSoundImport.Click += new System.EventHandler(this.ButtonSoundImport_Click);
			// 
			// ButtonSoundExport
			// 
			this.ButtonSoundExport.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonSoundExport.Enabled = false;
			this.ButtonSoundExport.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileSave;
			this.ButtonSoundExport.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonSoundExport.Name = "ButtonSoundExport";
			this.ButtonSoundExport.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonSoundExport.Size = new System.Drawing.Size(24, 24);
			this.ButtonSoundExport.Text = "Save sound";
			// 
			// TextBoxSoundFile
			// 
			this.TextBoxSoundFile.AcceptsReturn = true;
			this.TextBoxSoundFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxSoundFile.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
			this.TextBoxSoundFile.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
			this.TextBoxSoundFile.Location = new System.Drawing.Point(35, 27);
			this.TextBoxSoundFile.Name = "TextBoxSoundFile";
			this.TextBoxSoundFile.Size = new System.Drawing.Size(583, 22);
			this.TextBoxSoundFile.TabIndex = 1;
			this.TextBoxSoundFile.Validated += new System.EventHandler(this.TextBoxSoundFile_Validated);
			// 
			// PanelMain
			// 
			this.PanelMain.ColumnCount = 1;
			this.PanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Controls.Add(this.PanelTop, 0, 0);
			this.PanelMain.Controls.Add(this.GroupBoxImages, 0, 2);
			this.PanelMain.Controls.Add(this.GroupBoxSound, 0, 3);
			this.PanelMain.Controls.Add(this.PictureBoxFrameSample, 0, 1);
			this.PanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelMain.Location = new System.Drawing.Point(9, 0);
			this.PanelMain.Margin = new System.Windows.Forms.Padding(0);
			this.PanelMain.Name = "PanelMain";
			this.PanelMain.RowCount = 4;
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelMain.Size = new System.Drawing.Size(682, 650);
			this.PanelMain.TabIndex = 0;
			// 
			// PanelTop
			// 
			this.PanelTop.Controls.Add(this.LabelFrameName);
			this.PanelTop.Controls.Add(this.LabelDuration);
			this.PanelTop.Controls.Add(this.TextBoxFrameName);
			this.PanelTop.Controls.Add(this.LabelDurationUnits);
			this.PanelTop.Controls.Add(this.NumericDuration);
			this.PanelTop.Location = new System.Drawing.Point(0, 0);
			this.PanelTop.Margin = new System.Windows.Forms.Padding(0);
			this.PanelTop.Name = "PanelTop";
			this.PanelTop.Size = new System.Drawing.Size(682, 60);
			this.PanelTop.TabIndex = 2;
			// 
			// NumericDuration
			// 
			this.NumericDuration.AutoSize = true;
			this.NumericDuration.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericDuration.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericDuration.Location = new System.Drawing.Point(132, 31);
			this.NumericDuration.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
			this.NumericDuration.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.NumericDuration.Name = "NumericDuration";
			this.NumericDuration.Size = new System.Drawing.Size(64, 22);
			this.NumericDuration.TabIndex = 3;
			this.NumericDuration.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.NumericDuration.Validated += new System.EventHandler(this.NumericDuration_Validated);
			// 
			// PictureBoxFrameSample
			// 
			this.PictureBoxFrameSample.BackColor = System.Drawing.SystemColors.Window;
			this.PictureBoxFrameSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PictureBoxFrameSample.Location = new System.Drawing.Point(132, 60);
			this.PictureBoxFrameSample.Margin = new System.Windows.Forms.Padding(132, 0, 0, 0);
			this.PictureBoxFrameSample.MaximumSize = new System.Drawing.Size(258, 258);
			this.PictureBoxFrameSample.Name = "PictureBoxFrameSample";
			this.PictureBoxFrameSample.Size = new System.Drawing.Size(128, 128);
			this.PictureBoxFrameSample.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.PictureBoxFrameSample.TabIndex = 2;
			this.PictureBoxFrameSample.TabStop = false;
			// 
			// FramePanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.PanelMain);
			this.Name = "FramePanel";
			this.Size = new System.Drawing.Size(700, 650);
			this.PanelOverlayOffset.ResumeLayout(false);
			this.PanelOverlayOffset.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetY)).EndInit();
			this.GroupBoxImages.ResumeLayout(false);
			this.PanelImages.ResumeLayout(false);
			this.FlowLayoutRight.ResumeLayout(false);
			this.FlowLayoutRight.PerformLayout();
			this.PanelSample.ResumeLayout(false);
			this.PanelImageClip.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxImageSample)).EndInit();
			this.ToolStripShiftUp.ResumeLayout(false);
			this.ToolStripShiftUp.PerformLayout();
			this.ToolStripShiftRight.ResumeLayout(false);
			this.ToolStripShiftRight.PerformLayout();
			this.ToolStripShiftDown.ResumeLayout(false);
			this.ToolStripShiftDown.PerformLayout();
			this.ToolStripShiftLeft.ResumeLayout(false);
			this.ToolStripShiftLeft.PerformLayout();
			this.PanelImagesLeft.ResumeLayout(false);
			this.PanelImagesLeft.PerformLayout();
			this.ToolStripImages.ResumeLayout(false);
			this.ToolStripImages.PerformLayout();
			this.GroupBoxSound.ResumeLayout(false);
			this.GroupBoxSound.PerformLayout();
			this.ToolStripSound.ResumeLayout(false);
			this.ToolStripSound.PerformLayout();
			this.ToolStripSoundFile.ResumeLayout(false);
			this.ToolStripSoundFile.PerformLayout();
			this.PanelMain.ResumeLayout(false);
			this.PanelTop.ResumeLayout(false);
			this.PanelTop.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericDuration)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxFrameSample)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label LabelFrameName;
		private DoubleAgent.TextBoxEx TextBoxFrameName;
		private DoubleAgent.GroupBoxCompat GroupBoxImages;
		private DoubleAgent.GroupBoxCompat GroupBoxSound;
		private DoubleAgent.ToolStripEx ToolStripImages;
		private DoubleAgent.ListViewEx ListViewImages;
		private System.Windows.Forms.ColumnHeader ColumnHeaderImage;
		private System.Windows.Forms.ColumnHeader ColumnHeaderPosition;
		private System.Windows.Forms.ColumnHeader ColumnHeaderSize;
		private System.Windows.Forms.ColumnHeader ColumnHeaderPath;
		private DoubleAgent.ToolStripButtonCompat ButtonAdd;
		private DoubleAgent.ToolStripButtonCompat ButtonDelete;
		private DoubleAgent.ToolStripButtonCompat ButtonMoveUp;
		private DoubleAgent.ToolStripButtonCompat ButtonMoveDown;
		private DoubleAgent.TextBoxEx TextBoxSoundFile;
		private DoubleAgent.ToolStripEx ToolStripSoundFile;
		private DoubleAgent.ToolStripButtonCompat ButtonSoundImport;
		private DoubleAgent.ToolStripButtonCompat ButtonSoundExport;
		private DoubleAgent.ToolStripEx ToolStripSound;
		private DoubleAgent.ToolStripButtonCompat ButtonSoundPreview;
		private FrameSample PictureBoxImageSample;
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
		private FrameSample PictureBoxFrameSample;
		private System.Windows.Forms.TableLayoutPanel PanelMain;
		private System.Windows.Forms.Panel PanelImageClip;
		private System.Windows.Forms.FlowLayoutPanel FlowLayoutRight;
		private System.Windows.Forms.Panel PanelImagesLeft;
		private System.Windows.Forms.TableLayoutPanel PanelImages;
		private DoubleAgent.ToolStripButtonCompat ButtonChooseFile;
		private System.Windows.Forms.TableLayoutPanel PanelSample;
		private System.Windows.Forms.Panel PanelOverlayOffset;
		private DoubleAgent.CheckBoxCompat CheckBoxTransparent;
		private System.Windows.Forms.Panel PanelTop;
	}
}
