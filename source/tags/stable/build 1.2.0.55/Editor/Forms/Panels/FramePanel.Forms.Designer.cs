/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.Panel PanelFrame;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
			System.Windows.Forms.TableLayoutPanel PanelMiddle;
			System.Windows.Forms.TableLayoutPanel PanelImage;
			System.Windows.Forms.TableLayoutPanel PanelSound;
			this.LabelFrameName = new DoubleAgent.LabelCompat();
			this.TextBoxFrameName = new DoubleAgent.TextBoxEx();
			this.LabelDuration = new DoubleAgent.LabelCompat();
			this.NumericDuration = new DoubleAgent.NumericUpDownEx();
			this.LabelDurationUnits = new DoubleAgent.LabelCompat();
			this.GroupBoxFrame = new System.Windows.Forms.GroupBox();
			this.PanelFrameSample = new System.Windows.Forms.TableLayoutPanel();
			this.CheckBoxFrameTransparent = new DoubleAgent.CheckBoxCompat();
			this.PictureBoxFrameSample = new AgentCharacterEditor.FrameSample();
			this.GroupBoxImage = new System.Windows.Forms.GroupBox();
			this.PanelImageDetails = new System.Windows.Forms.Panel();
			this.LabelOffsetX = new DoubleAgent.LabelCompat();
			this.NumericOffsetX = new DoubleAgent.NumericUpDownEx();
			this.LabelOffsetY = new DoubleAgent.LabelCompat();
			this.NumericOffsetY = new DoubleAgent.NumericUpDownEx();
			this.PanelSample = new System.Windows.Forms.TableLayoutPanel();
			this.CheckBoxImageTransparent = new DoubleAgent.CheckBoxCompat();
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
			this.ToolStripSound = new DoubleAgent.ToolStripEx();
			this.ButtonSoundPreview = new DoubleAgent.ToolStripButtonCompat();
			this.TextBoxSoundFile = new DoubleAgent.TextBoxEx();
			this.ToolStripSoundFile = new DoubleAgent.ToolStripEx();
			this.ButtonSoundImport = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonSoundExport = new DoubleAgent.ToolStripButtonCompat();
			this.PanelMain = new System.Windows.Forms.TableLayoutPanel();
			this.GroupBoxImages = new DoubleAgent.GroupBoxCompat();
			this.PanelImages = new System.Windows.Forms.TableLayoutPanel();
			this.ListViewImages = new DoubleAgent.ListViewEx();
			this.ColumnHeaderImage = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeaderPosition = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeaderSize = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeaderPath = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ContextMenuImages = new AgentCharacterEditor.Global.ContextMenuEdit(this.components);
			this.MenuItemAdd = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemChooseFile = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemMovePrev = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemMoveNext = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolStripImages = new DoubleAgent.ToolStripEx();
			this.ButtonAdd = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonDelete = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonChooseFile = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMovePrev = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMoveNext = new DoubleAgent.ToolStripButtonCompat();
			this.GroupBoxSound = new DoubleAgent.GroupBoxCompat();
			PanelFrame = new System.Windows.Forms.Panel();
			toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			PanelMiddle = new System.Windows.Forms.TableLayoutPanel();
			PanelImage = new System.Windows.Forms.TableLayoutPanel();
			PanelSound = new System.Windows.Forms.TableLayoutPanel();
			PanelFrame.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericDuration)).BeginInit();
			PanelMiddle.SuspendLayout();
			this.GroupBoxFrame.SuspendLayout();
			this.PanelFrameSample.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxFrameSample)).BeginInit();
			this.GroupBoxImage.SuspendLayout();
			PanelImage.SuspendLayout();
			this.PanelImageDetails.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetY)).BeginInit();
			this.PanelSample.SuspendLayout();
			this.PanelImageClip.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxImageSample)).BeginInit();
			this.ToolStripShiftUp.SuspendLayout();
			this.ToolStripShiftRight.SuspendLayout();
			this.ToolStripShiftDown.SuspendLayout();
			this.ToolStripShiftLeft.SuspendLayout();
			PanelSound.SuspendLayout();
			this.ToolStripSound.SuspendLayout();
			this.ToolStripSoundFile.SuspendLayout();
			this.PanelMain.SuspendLayout();
			this.GroupBoxImages.SuspendLayout();
			this.PanelImages.SuspendLayout();
			this.ContextMenuImages.SuspendLayout();
			this.ToolStripImages.SuspendLayout();
			this.GroupBoxSound.SuspendLayout();
			this.SuspendLayout();
			// 
			// PanelFrame
			// 
			PanelFrame.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			PanelFrame.AutoSize = true;
			PanelFrame.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			PanelFrame.Controls.Add(this.LabelFrameName);
			PanelFrame.Controls.Add(this.TextBoxFrameName);
			PanelFrame.Controls.Add(this.LabelDuration);
			PanelFrame.Controls.Add(this.NumericDuration);
			PanelFrame.Controls.Add(this.LabelDurationUnits);
			PanelFrame.Location = new System.Drawing.Point(0, 0);
			PanelFrame.Margin = new System.Windows.Forms.Padding(0);
			PanelFrame.Name = "PanelFrame";
			PanelFrame.Size = new System.Drawing.Size(682, 28);
			PanelFrame.TabIndex = 0;
			// 
			// LabelFrameName
			// 
			this.LabelFrameName.AutoSize = true;
			this.LabelFrameName.Location = new System.Drawing.Point(0, 6);
			this.LabelFrameName.Name = "LabelFrameName";
			this.LabelFrameName.Size = new System.Drawing.Size(48, 17);
			this.LabelFrameName.TabIndex = 0;
			this.LabelFrameName.Text = "Frame";
			// 
			// TextBoxFrameName
			// 
			this.TextBoxFrameName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxFrameName.Location = new System.Drawing.Point(131, 3);
			this.TextBoxFrameName.MaximumSize = new System.Drawing.Size(600, 50);
			this.TextBoxFrameName.Name = "TextBoxFrameName";
			this.TextBoxFrameName.ReadOnly = true;
			this.TextBoxFrameName.Size = new System.Drawing.Size(296, 22);
			this.TextBoxFrameName.TabIndex = 1;
			// 
			// LabelDuration
			// 
			this.LabelDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelDuration.AutoSize = true;
			this.LabelDuration.Location = new System.Drawing.Point(433, 6);
			this.LabelDuration.Name = "LabelDuration";
			this.LabelDuration.Size = new System.Drawing.Size(62, 17);
			this.LabelDuration.TabIndex = 2;
			this.LabelDuration.Text = "Duration";
			// 
			// NumericDuration
			// 
			this.NumericDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.NumericDuration.AutoSize = true;
			this.NumericDuration.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericDuration.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericDuration.Location = new System.Drawing.Point(501, 3);
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
			// LabelDurationUnits
			// 
			this.LabelDurationUnits.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelDurationUnits.AutoSize = true;
			this.LabelDurationUnits.Location = new System.Drawing.Point(571, 6);
			this.LabelDurationUnits.Name = "LabelDurationUnits";
			this.LabelDurationUnits.Size = new System.Drawing.Size(111, 17);
			this.LabelDurationUnits.TabIndex = 4;
			this.LabelDurationUnits.Text = "(1/100 seconds)";
			// 
			// toolStripSeparator2
			// 
			toolStripSeparator2.Name = "toolStripSeparator2";
			toolStripSeparator2.Size = new System.Drawing.Size(218, 6);
			// 
			// toolStripSeparator3
			// 
			toolStripSeparator3.Name = "toolStripSeparator3";
			toolStripSeparator3.Size = new System.Drawing.Size(218, 6);
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// PanelMiddle
			// 
			PanelMiddle.AutoSize = true;
			PanelMiddle.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			PanelMiddle.ColumnCount = 2;
			PanelMiddle.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			PanelMiddle.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			PanelMiddle.Controls.Add(this.GroupBoxFrame, 1, 0);
			PanelMiddle.Controls.Add(this.GroupBoxImage, 0, 0);
			PanelMiddle.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelMiddle.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			PanelMiddle.Location = new System.Drawing.Point(0, 260);
			PanelMiddle.Margin = new System.Windows.Forms.Padding(0);
			PanelMiddle.Name = "PanelMiddle";
			PanelMiddle.RowCount = 1;
			PanelMiddle.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			PanelMiddle.Size = new System.Drawing.Size(682, 319);
			PanelMiddle.TabIndex = 3;
			// 
			// GroupBoxFrame
			// 
			this.GroupBoxFrame.AutoSize = true;
			this.GroupBoxFrame.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.GroupBoxFrame.Controls.Add(this.PanelFrameSample);
			this.GroupBoxFrame.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxFrame.Location = new System.Drawing.Point(338, 9);
			this.GroupBoxFrame.Margin = new System.Windows.Forms.Padding(9, 9, 0, 0);
			this.GroupBoxFrame.Name = "GroupBoxFrame";
			this.GroupBoxFrame.Padding = new System.Windows.Forms.Padding(6, 9, 6, 6);
			this.GroupBoxFrame.Size = new System.Drawing.Size(344, 310);
			this.GroupBoxFrame.TabIndex = 4;
			this.GroupBoxFrame.TabStop = false;
			this.GroupBoxFrame.Text = "Frame";
			// 
			// PanelFrameSample
			// 
			this.PanelFrameSample.ColumnCount = 1;
			this.PanelFrameSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelFrameSample.Controls.Add(this.CheckBoxFrameTransparent, 0, 1);
			this.PanelFrameSample.Controls.Add(this.PictureBoxFrameSample, 0, 0);
			this.PanelFrameSample.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelFrameSample.Location = new System.Drawing.Point(6, 24);
			this.PanelFrameSample.Margin = new System.Windows.Forms.Padding(0);
			this.PanelFrameSample.Name = "PanelFrameSample";
			this.PanelFrameSample.RowCount = 2;
			this.PanelFrameSample.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelFrameSample.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelFrameSample.Size = new System.Drawing.Size(332, 280);
			this.PanelFrameSample.TabIndex = 3;
			// 
			// CheckBoxFrameTransparent
			// 
			this.CheckBoxFrameTransparent.AutoSize = true;
			this.PanelFrameSample.SetColumnSpan(this.CheckBoxFrameTransparent, 3);
			this.CheckBoxFrameTransparent.Location = new System.Drawing.Point(3, 140);
			this.CheckBoxFrameTransparent.Margin = new System.Windows.Forms.Padding(3, 12, 3, 3);
			this.CheckBoxFrameTransparent.Name = "CheckBoxFrameTransparent";
			this.CheckBoxFrameTransparent.Size = new System.Drawing.Size(151, 21);
			this.CheckBoxFrameTransparent.TabIndex = 3;
			this.CheckBoxFrameTransparent.Text = "Show transparency";
			this.CheckBoxFrameTransparent.UseVisualStyleBackColor = true;
			this.CheckBoxFrameTransparent.CheckedChanged += new System.EventHandler(this.CheckBoxFrameTransparent_CheckedChanged);
			// 
			// PictureBoxFrameSample
			// 
			this.PictureBoxFrameSample.BackColor = System.Drawing.SystemColors.Window;
			this.PictureBoxFrameSample.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PictureBoxFrameSample.Location = new System.Drawing.Point(0, 0);
			this.PictureBoxFrameSample.Margin = new System.Windows.Forms.Padding(0);
			this.PictureBoxFrameSample.MaximumSize = new System.Drawing.Size(258, 258);
			this.PictureBoxFrameSample.Name = "PictureBoxFrameSample";
			this.PictureBoxFrameSample.Size = new System.Drawing.Size(128, 128);
			this.PictureBoxFrameSample.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.PictureBoxFrameSample.TabIndex = 2;
			this.PictureBoxFrameSample.TabStop = false;
			// 
			// GroupBoxImage
			// 
			this.GroupBoxImage.AutoSize = true;
			this.GroupBoxImage.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.GroupBoxImage.Controls.Add(PanelImage);
			this.GroupBoxImage.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxImage.Location = new System.Drawing.Point(0, 9);
			this.GroupBoxImage.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxImage.Name = "GroupBoxImage";
			this.GroupBoxImage.Padding = new System.Windows.Forms.Padding(6, 9, 6, 6);
			this.GroupBoxImage.Size = new System.Drawing.Size(329, 310);
			this.GroupBoxImage.TabIndex = 4;
			this.GroupBoxImage.TabStop = false;
			this.GroupBoxImage.Text = "Image %1";
			// 
			// PanelImage
			// 
			PanelImage.AutoSize = true;
			PanelImage.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			PanelImage.ColumnCount = 2;
			PanelImage.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			PanelImage.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			PanelImage.Controls.Add(this.PanelImageDetails, 1, 0);
			PanelImage.Controls.Add(this.PanelSample, 0, 0);
			PanelImage.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelImage.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			PanelImage.Location = new System.Drawing.Point(6, 24);
			PanelImage.Margin = new System.Windows.Forms.Padding(0);
			PanelImage.Name = "PanelImage";
			PanelImage.RowCount = 1;
			PanelImage.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			PanelImage.Size = new System.Drawing.Size(317, 280);
			PanelImage.TabIndex = 2;
			// 
			// PanelImageDetails
			// 
			this.PanelImageDetails.AutoSize = true;
			this.PanelImageDetails.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelImageDetails.Controls.Add(this.LabelOffsetX);
			this.PanelImageDetails.Controls.Add(this.NumericOffsetX);
			this.PanelImageDetails.Controls.Add(this.LabelOffsetY);
			this.PanelImageDetails.Controls.Add(this.NumericOffsetY);
			this.PanelImageDetails.Location = new System.Drawing.Point(198, 27);
			this.PanelImageDetails.Margin = new System.Windows.Forms.Padding(12, 27, 0, 0);
			this.PanelImageDetails.Name = "PanelImageDetails";
			this.PanelImageDetails.Size = new System.Drawing.Size(119, 53);
			this.PanelImageDetails.TabIndex = 3;
			// 
			// LabelOffsetX
			// 
			this.LabelOffsetX.AutoSize = true;
			this.LabelOffsetX.Location = new System.Drawing.Point(0, 2);
			this.LabelOffsetX.Name = "LabelOffsetX";
			this.LabelOffsetX.Size = new System.Drawing.Size(59, 17);
			this.LabelOffsetX.TabIndex = 0;
			this.LabelOffsetX.Text = "X Offset";
			// 
			// NumericOffsetX
			// 
			this.NumericOffsetX.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericOffsetX.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericOffsetX.Location = new System.Drawing.Point(69, 0);
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
			this.LabelOffsetY.Location = new System.Drawing.Point(0, 30);
			this.LabelOffsetY.Name = "LabelOffsetY";
			this.LabelOffsetY.Size = new System.Drawing.Size(59, 17);
			this.LabelOffsetY.TabIndex = 2;
			this.LabelOffsetY.Text = "Y Offset";
			// 
			// NumericOffsetY
			// 
			this.NumericOffsetY.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericOffsetY.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericOffsetY.Location = new System.Drawing.Point(69, 28);
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
			// PanelSample
			// 
			this.PanelSample.AutoSize = true;
			this.PanelSample.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelSample.ColumnCount = 3;
			this.PanelSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelSample.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.Controls.Add(this.CheckBoxImageTransparent, 0, 3);
			this.PanelSample.Controls.Add(this.PanelImageClip, 1, 1);
			this.PanelSample.Controls.Add(this.ToolStripShiftUp, 1, 0);
			this.PanelSample.Controls.Add(this.ToolStripShiftRight, 2, 1);
			this.PanelSample.Controls.Add(this.ToolStripShiftDown, 1, 2);
			this.PanelSample.Controls.Add(this.ToolStripShiftLeft, 0, 1);
			this.PanelSample.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelSample.Location = new System.Drawing.Point(0, 0);
			this.PanelSample.Margin = new System.Windows.Forms.Padding(0);
			this.PanelSample.Name = "PanelSample";
			this.PanelSample.RowCount = 4;
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
			this.PanelSample.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelSample.Size = new System.Drawing.Size(186, 222);
			this.PanelSample.TabIndex = 1;
			// 
			// CheckBoxImageTransparent
			// 
			this.CheckBoxImageTransparent.AutoSize = true;
			this.PanelSample.SetColumnSpan(this.CheckBoxImageTransparent, 3);
			this.CheckBoxImageTransparent.Location = new System.Drawing.Point(3, 198);
			this.CheckBoxImageTransparent.Margin = new System.Windows.Forms.Padding(3, 12, 3, 3);
			this.CheckBoxImageTransparent.Name = "CheckBoxImageTransparent";
			this.CheckBoxImageTransparent.Size = new System.Drawing.Size(151, 21);
			this.CheckBoxImageTransparent.TabIndex = 2;
			this.CheckBoxImageTransparent.Text = "Show transparency";
			this.CheckBoxImageTransparent.UseVisualStyleBackColor = true;
			this.CheckBoxImageTransparent.CheckedChanged += new System.EventHandler(this.CheckBoxImageTransparent_CheckedChanged);
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
			this.ToolStripShiftUp.MaximumSize = new System.Drawing.Size(10000, 24);
			this.ToolStripShiftUp.MinimumSize = new System.Drawing.Size(0, 24);
			this.ToolStripShiftUp.Name = "ToolStripShiftUp";
			this.ToolStripShiftUp.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftUp.Size = new System.Drawing.Size(130, 24);
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
			this.ToolStripShiftRight.Location = new System.Drawing.Point(162, 28);
			this.ToolStripShiftRight.MaximumSize = new System.Drawing.Size(24, 10000);
			this.ToolStripShiftRight.MinimumSize = new System.Drawing.Size(24, 0);
			this.ToolStripShiftRight.Name = "ToolStripShiftRight";
			this.ToolStripShiftRight.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftRight.Size = new System.Drawing.Size(24, 130);
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
			this.ToolStripShiftDown.Location = new System.Drawing.Point(28, 162);
			this.ToolStripShiftDown.MaximumSize = new System.Drawing.Size(10000, 24);
			this.ToolStripShiftDown.MinimumSize = new System.Drawing.Size(0, 24);
			this.ToolStripShiftDown.Name = "ToolStripShiftDown";
			this.ToolStripShiftDown.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftDown.Size = new System.Drawing.Size(130, 24);
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
			this.ToolStripShiftLeft.MaximumSize = new System.Drawing.Size(24, 10000);
			this.ToolStripShiftLeft.MinimumSize = new System.Drawing.Size(24, 0);
			this.ToolStripShiftLeft.Name = "ToolStripShiftLeft";
			this.ToolStripShiftLeft.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStripShiftLeft.Size = new System.Drawing.Size(24, 130);
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
			// PanelSound
			// 
			PanelSound.AutoSize = true;
			PanelSound.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			PanelSound.ColumnCount = 3;
			PanelSound.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			PanelSound.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			PanelSound.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			PanelSound.Controls.Add(this.ToolStripSound, 0, 0);
			PanelSound.Controls.Add(this.TextBoxSoundFile, 1, 0);
			PanelSound.Controls.Add(this.ToolStripSoundFile, 2, 0);
			PanelSound.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelSound.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			PanelSound.Location = new System.Drawing.Point(6, 24);
			PanelSound.Margin = new System.Windows.Forms.Padding(0);
			PanelSound.Name = "PanelSound";
			PanelSound.RowCount = 1;
			PanelSound.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			PanelSound.Size = new System.Drawing.Size(670, 32);
			PanelSound.TabIndex = 4;
			// 
			// ToolStripSound
			// 
			this.ToolStripSound.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripSound.CanOverflow = false;
			this.ToolStripSound.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripSound.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripSound.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonSoundPreview});
			this.ToolStripSound.Location = new System.Drawing.Point(0, 0);
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
			// TextBoxSoundFile
			// 
			this.TextBoxSoundFile.AcceptsReturn = true;
			this.TextBoxSoundFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxSoundFile.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
			this.TextBoxSoundFile.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
			this.TextBoxSoundFile.Location = new System.Drawing.Point(30, 3);
			this.TextBoxSoundFile.Name = "TextBoxSoundFile";
			this.TextBoxSoundFile.Size = new System.Drawing.Size(586, 22);
			this.TextBoxSoundFile.TabIndex = 1;
			this.TextBoxSoundFile.Validated += new System.EventHandler(this.TextBoxSoundFile_Validated);
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
			this.ToolStripSoundFile.Location = new System.Drawing.Point(619, 0);
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
			// PanelMain
			// 
			this.PanelMain.ColumnCount = 1;
			this.PanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Controls.Add(PanelFrame, 0, 0);
			this.PanelMain.Controls.Add(this.GroupBoxImages, 0, 1);
			this.PanelMain.Controls.Add(PanelMiddle, 0, 2);
			this.PanelMain.Controls.Add(this.GroupBoxSound, 0, 3);
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
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
			this.PanelMain.Size = new System.Drawing.Size(682, 650);
			this.PanelMain.TabIndex = 0;
			// 
			// GroupBoxImages
			// 
			this.GroupBoxImages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxImages.AutoSize = true;
			this.GroupBoxImages.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.GroupBoxImages.Controls.Add(this.PanelImages);
			this.GroupBoxImages.Location = new System.Drawing.Point(0, 37);
			this.GroupBoxImages.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxImages.Name = "GroupBoxImages";
			this.GroupBoxImages.Padding = new System.Windows.Forms.Padding(6, 9, 6, 6);
			this.GroupBoxImages.Size = new System.Drawing.Size(682, 223);
			this.GroupBoxImages.TabIndex = 0;
			this.GroupBoxImages.TabStop = false;
			this.GroupBoxImages.Text = "Images";
			// 
			// PanelImages
			// 
			this.PanelImages.AutoSize = true;
			this.PanelImages.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelImages.ColumnCount = 1;
			this.PanelImages.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelImages.Controls.Add(this.ListViewImages, 0, 1);
			this.PanelImages.Controls.Add(this.ToolStripImages, 0, 0);
			this.PanelImages.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelImages.Location = new System.Drawing.Point(6, 24);
			this.PanelImages.Margin = new System.Windows.Forms.Padding(0);
			this.PanelImages.Name = "PanelImages";
			this.PanelImages.RowCount = 2;
			this.PanelImages.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelImages.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelImages.Size = new System.Drawing.Size(670, 193);
			this.PanelImages.TabIndex = 0;
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
			this.ListViewImages.ContextMenuStrip = this.ContextMenuImages;
			this.ListViewImages.FullRowSelect = true;
			this.ListViewImages.GridLines = true;
			this.ListViewImages.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.ListViewImages.HideSelection = false;
			this.ListViewImages.Location = new System.Drawing.Point(0, 30);
			this.ListViewImages.Margin = new System.Windows.Forms.Padding(0, 3, 0, 0);
			this.ListViewImages.MaximumSize = new System.Drawing.Size(10000, 400);
			this.ListViewImages.MinimumSize = new System.Drawing.Size(200, 100);
			this.ListViewImages.MultiSelect = false;
			this.ListViewImages.Name = "ListViewImages";
			this.ListViewImages.ShowGroups = false;
			this.ListViewImages.ShowItemToolTips = true;
			this.ListViewImages.Size = new System.Drawing.Size(670, 163);
			this.ListViewImages.TabIndex = 1;
			this.ListViewImages.UseCompatibleStateImageBehavior = false;
			this.ListViewImages.View = System.Windows.Forms.View.Details;
			this.ListViewImages.ItemActivate += new System.EventHandler(this.ListViewImages_ItemActivate);
			this.ListViewImages.SelectedIndexChanged += new System.EventHandler(this.ListViewImages_SelectedIndexChanged);
			this.ListViewImages.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ListViewImages_MouseUp);
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
			// ContextMenuImages
			// 
			this.ContextMenuImages.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemAdd,
            this.MenuItemChooseFile,
            toolStripSeparator2,
            this.MenuItemMovePrev,
            this.MenuItemMoveNext,
            toolStripSeparator3});
			this.ContextMenuImages.Name = "ContextMenuImages";
			this.ContextMenuImages.Size = new System.Drawing.Size(222, 128);
			this.ContextMenuImages.Opening += new System.ComponentModel.CancelEventHandler(this.ContextMenuImages_Opening);
			// 
			// MenuItemAdd
			// 
			this.MenuItemAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.MenuItemAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemAdd.Name = "MenuItemAdd";
			this.MenuItemAdd.Size = new System.Drawing.Size(221, 28);
			this.MenuItemAdd.Text = "Add a new image";
			this.MenuItemAdd.Click += new System.EventHandler(this.ButtonAdd_Click);
			// 
			// MenuItemChooseFile
			// 
			this.MenuItemChooseFile.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileOpen;
			this.MenuItemChooseFile.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemChooseFile.Name = "MenuItemChooseFile";
			this.MenuItemChooseFile.Size = new System.Drawing.Size(221, 28);
			this.MenuItemChooseFile.Text = "Open image file";
			this.MenuItemChooseFile.Click += new System.EventHandler(this.ButtonOpen_Click);
			// 
			// MenuItemMovePrev
			// 
			this.MenuItemMovePrev.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveUp;
			this.MenuItemMovePrev.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemMovePrev.Name = "MenuItemMovePrev";
			this.MenuItemMovePrev.Size = new System.Drawing.Size(221, 28);
			this.MenuItemMovePrev.Text = "Move image up";
			this.MenuItemMovePrev.Click += new System.EventHandler(this.ButtonMovePrev_Click);
			// 
			// MenuItemMoveNext
			// 
			this.MenuItemMoveNext.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveDown;
			this.MenuItemMoveNext.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemMoveNext.Name = "MenuItemMoveNext";
			this.MenuItemMoveNext.Size = new System.Drawing.Size(221, 28);
			this.MenuItemMoveNext.Text = "Move image down";
			this.MenuItemMoveNext.Click += new System.EventHandler(this.ButtonMoveNext_Click);
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
            this.ButtonMovePrev,
            this.ButtonMoveNext});
			this.ToolStripImages.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripImages.Location = new System.Drawing.Point(0, 0);
			this.ToolStripImages.Name = "ToolStripImages";
			this.ToolStripImages.Size = new System.Drawing.Size(129, 27);
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
			// ButtonMovePrev
			// 
			this.ButtonMovePrev.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMovePrev.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveUp;
			this.ButtonMovePrev.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMovePrev.Name = "ButtonMovePrev";
			this.ButtonMovePrev.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMovePrev.Size = new System.Drawing.Size(24, 24);
			this.ButtonMovePrev.Text = "Move image up";
			this.ButtonMovePrev.Click += new System.EventHandler(this.ButtonMovePrev_Click);
			// 
			// ButtonMoveNext
			// 
			this.ButtonMoveNext.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveNext.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveDown;
			this.ButtonMoveNext.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveNext.Name = "ButtonMoveNext";
			this.ButtonMoveNext.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMoveNext.Size = new System.Drawing.Size(24, 24);
			this.ButtonMoveNext.Text = "Move image down";
			this.ButtonMoveNext.Click += new System.EventHandler(this.ButtonMoveNext_Click);
			// 
			// GroupBoxSound
			// 
			this.GroupBoxSound.Controls.Add(PanelSound);
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
			// FramePanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.PanelMain);
			this.Name = "FramePanel";
			this.Size = new System.Drawing.Size(700, 650);
			PanelFrame.ResumeLayout(false);
			PanelFrame.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericDuration)).EndInit();
			PanelMiddle.ResumeLayout(false);
			PanelMiddle.PerformLayout();
			this.GroupBoxFrame.ResumeLayout(false);
			this.PanelFrameSample.ResumeLayout(false);
			this.PanelFrameSample.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.PictureBoxFrameSample)).EndInit();
			this.GroupBoxImage.ResumeLayout(false);
			this.GroupBoxImage.PerformLayout();
			PanelImage.ResumeLayout(false);
			PanelImage.PerformLayout();
			this.PanelImageDetails.ResumeLayout(false);
			this.PanelImageDetails.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.NumericOffsetY)).EndInit();
			this.PanelSample.ResumeLayout(false);
			this.PanelSample.PerformLayout();
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
			PanelSound.ResumeLayout(false);
			PanelSound.PerformLayout();
			this.ToolStripSound.ResumeLayout(false);
			this.ToolStripSound.PerformLayout();
			this.ToolStripSoundFile.ResumeLayout(false);
			this.ToolStripSoundFile.PerformLayout();
			this.PanelMain.ResumeLayout(false);
			this.PanelMain.PerformLayout();
			this.GroupBoxImages.ResumeLayout(false);
			this.GroupBoxImages.PerformLayout();
			this.PanelImages.ResumeLayout(false);
			this.PanelImages.PerformLayout();
			this.ContextMenuImages.ResumeLayout(false);
			this.ToolStripImages.ResumeLayout(false);
			this.ToolStripImages.PerformLayout();
			this.GroupBoxSound.ResumeLayout(false);
			this.GroupBoxSound.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private DoubleAgent.LabelCompat LabelFrameName;
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
		private DoubleAgent.ToolStripButtonCompat ButtonMovePrev;
		private DoubleAgent.ToolStripButtonCompat ButtonMoveNext;
		private DoubleAgent.TextBoxEx TextBoxSoundFile;
		private DoubleAgent.ToolStripEx ToolStripSoundFile;
		private DoubleAgent.ToolStripButtonCompat ButtonSoundImport;
		private DoubleAgent.ToolStripButtonCompat ButtonSoundExport;
		private DoubleAgent.ToolStripEx ToolStripSound;
		private DoubleAgent.ToolStripButtonCompat ButtonSoundPreview;
		private FrameSample PictureBoxImageSample;
		private DoubleAgent.LabelCompat LabelOffsetY;
		private DoubleAgent.LabelCompat LabelOffsetX;
		private DoubleAgent.NumericUpDownEx NumericOffsetY;
		private DoubleAgent.NumericUpDownEx NumericOffsetX;
		private DoubleAgent.NumericUpDownEx NumericDuration;
		private DoubleAgent.LabelCompat LabelDurationUnits;
		private DoubleAgent.LabelCompat LabelDuration;
		private DoubleAgent.ToolStripEx ToolStripShiftUp;
		private DoubleAgent.ToolStripEx ToolStripShiftDown;
		private DoubleAgent.ToolStripEx ToolStripShiftLeft;
		private DoubleAgent.ToolStripEx ToolStripShiftRight;
		private DoubleAgent.ToolStripButtonEx ButtonShiftUp;
		private DoubleAgent.ToolStripButtonEx ButtonShiftDown;
		private DoubleAgent.ToolStripButtonEx ButtonShiftLeft;
		private DoubleAgent.ToolStripButtonEx ButtonShiftRight;
		private FrameSample PictureBoxFrameSample;
		private System.Windows.Forms.Panel PanelImageClip;
		private System.Windows.Forms.TableLayoutPanel PanelImages;
		private DoubleAgent.ToolStripButtonCompat ButtonChooseFile;
		private System.Windows.Forms.TableLayoutPanel PanelSample;
		private DoubleAgent.CheckBoxCompat CheckBoxImageTransparent;
		private Global.ContextMenuEdit ContextMenuImages;
		private System.Windows.Forms.ToolStripMenuItem MenuItemAdd;
		private System.Windows.Forms.ToolStripMenuItem MenuItemChooseFile;
		private System.Windows.Forms.ToolStripMenuItem MenuItemMovePrev;
		private System.Windows.Forms.ToolStripMenuItem MenuItemMoveNext;
		private System.Windows.Forms.GroupBox GroupBoxImage;
		private System.Windows.Forms.TableLayoutPanel PanelMain;
		private System.Windows.Forms.Panel PanelImageDetails;
		private DoubleAgent.TextBoxEx TextBoxFrameName;
		private System.Windows.Forms.GroupBox GroupBoxFrame;
		private System.Windows.Forms.TableLayoutPanel PanelFrameSample;
		private DoubleAgent.CheckBoxCompat CheckBoxFrameTransparent;
	}
}
