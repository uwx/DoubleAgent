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
	partial class AnimationPanel
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
			System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
			System.Windows.Forms.Panel PanelTopLeft;
			System.Windows.Forms.Panel PanelTopRight;
			System.Windows.Forms.ColumnHeader Column1;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
			this.LabelName = new DoubleAgent.LabelCompat();
			this.TextBoxName = new DoubleAgent.TextBoxEx();
			this.LabelReturn = new DoubleAgent.LabelCompat();
			this.ComboBoxReturn = new DoubleAgent.ComboBoxCompat();
			this.ListViewStates = new DoubleAgent.ListViewEx();
			this.LabelStates = new DoubleAgent.LabelCompat();
			this.PanelPreviewRate = new System.Windows.Forms.TableLayoutPanel();
			this.LabelSlow = new DoubleAgent.LabelCompat();
			this.SliderRate = new System.Windows.Forms.TrackBar();
			this.LabelFast = new DoubleAgent.LabelCompat();
			this.GroupBoxFrames = new DoubleAgent.GroupBoxCompat();
			this.PanelFrames = new System.Windows.Forms.TableLayoutPanel();
			this.FramesView = new AgentCharacterEditor.Previews.FramesPreview();
			this.PanelFramesTop = new System.Windows.Forms.TableLayoutPanel();
			this.LabelDurationDesc = new DoubleAgent.LabelCompat();
			this.NumericFrameDuration = new DoubleAgent.NumericUpDownEx();
			this.LabelDuration = new DoubleAgent.LabelCompat();
			this.ToolStripFramesView = new DoubleAgent.ToolStripEx();
			this.ButtonShowBranching = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonShowExitBranching = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonViewSmall = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonViewMedium = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonViewLarge = new DoubleAgent.ToolStripButtonCompat();
			this.ToolBarFrames = new AgentCharacterEditor.Previews.FramesToolBar();
			this.PanelTop = new System.Windows.Forms.TableLayoutPanel();
			this.PanelMain = new System.Windows.Forms.TableLayoutPanel();
			this.GroupBoxPreview = new DoubleAgent.GroupBoxCompat();
			this.PanelPreview = new System.Windows.Forms.TableLayoutPanel();
			this.PanelPreviewControls = new System.Windows.Forms.TableLayoutPanel();
			this.ToolStripPreview = new DoubleAgent.ToolStripEx();
			this.PreviewButtonPlay = new DoubleAgent.ToolStripButtonCompat();
			this.PreviewButtonStop = new DoubleAgent.ToolStripButtonCompat();
			this.PreviewButtonPause = new DoubleAgent.ToolStripButtonCompat();
			this.PreviewButtonSkipBack = new DoubleAgent.ToolStripButtonCompat();
			this.PreviewButtonSkipForward = new DoubleAgent.ToolStripButtonCompat();
			this.PreviewButtonRepeat = new DoubleAgent.ToolStripButtonCompat();
			this.PreviewButtonMute = new DoubleAgent.ToolStripButtonCompat();
			this.AnimationPreview = new AgentCharacterEditor.Previews.AnimationPreviewHost();
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			PanelTopLeft = new System.Windows.Forms.Panel();
			PanelTopRight = new System.Windows.Forms.Panel();
			Column1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
			toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
			PanelTopLeft.SuspendLayout();
			PanelTopRight.SuspendLayout();
			this.PanelPreviewRate.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.SliderRate)).BeginInit();
			this.GroupBoxFrames.SuspendLayout();
			this.PanelFrames.SuspendLayout();
			this.PanelFramesTop.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameDuration)).BeginInit();
			this.ToolStripFramesView.SuspendLayout();
			this.PanelTop.SuspendLayout();
			this.PanelMain.SuspendLayout();
			this.GroupBoxPreview.SuspendLayout();
			this.PanelPreview.SuspendLayout();
			this.PanelPreviewControls.SuspendLayout();
			this.ToolStripPreview.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// toolStripSeparator3
			// 
			toolStripSeparator3.Name = "toolStripSeparator3";
			toolStripSeparator3.Size = new System.Drawing.Size(6, 27);
			// 
			// PanelTopLeft
			// 
			PanelTopLeft.Controls.Add(this.LabelName);
			PanelTopLeft.Controls.Add(this.TextBoxName);
			PanelTopLeft.Controls.Add(this.LabelReturn);
			PanelTopLeft.Controls.Add(this.ComboBoxReturn);
			PanelTopLeft.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelTopLeft.Location = new System.Drawing.Point(0, 0);
			PanelTopLeft.Margin = new System.Windows.Forms.Padding(0);
			PanelTopLeft.Name = "PanelTopLeft";
			PanelTopLeft.Size = new System.Drawing.Size(379, 77);
			PanelTopLeft.TabIndex = 0;
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.Location = new System.Drawing.Point(0, 6);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size(109, 17);
			this.LabelName.TabIndex = 0;
			this.LabelName.Text = "Animation name";
			// 
			// TextBoxName
			// 
			this.TextBoxName.AcceptsReturn = true;
			this.TextBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxName.Location = new System.Drawing.Point(131, 3);
			this.TextBoxName.MaximumSize = new System.Drawing.Size(600, 50);
			this.TextBoxName.Name = "TextBoxName";
			this.TextBoxName.Size = new System.Drawing.Size(245, 22);
			this.TextBoxName.TabIndex = 1;
			this.TextBoxName.Validated += new System.EventHandler(this.TextBoxName_Validated);
			// 
			// LabelReturn
			// 
			this.LabelReturn.AutoSize = true;
			this.LabelReturn.Location = new System.Drawing.Point(0, 34);
			this.LabelReturn.Name = "LabelReturn";
			this.LabelReturn.Size = new System.Drawing.Size(116, 17);
			this.LabelReturn.TabIndex = 1;
			this.LabelReturn.Text = "Return animation";
			// 
			// ComboBoxReturn
			// 
			this.ComboBoxReturn.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.ComboBoxReturn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ComboBoxReturn.Items.AddRange(new object[] {
            "None"});
			this.ComboBoxReturn.Location = new System.Drawing.Point(131, 31);
			this.ComboBoxReturn.MaximumSize = new System.Drawing.Size(600, 0);
			this.ComboBoxReturn.Name = "ComboBoxReturn";
			this.ComboBoxReturn.Size = new System.Drawing.Size(245, 24);
			this.ComboBoxReturn.TabIndex = 3;
			this.ComboBoxReturn.SelectionChangeCommitted += new System.EventHandler(this.ComboBoxReturn_SelectionChangeCommitted);
			// 
			// PanelTopRight
			// 
			PanelTopRight.Controls.Add(this.ListViewStates);
			PanelTopRight.Controls.Add(this.LabelStates);
			PanelTopRight.Dock = System.Windows.Forms.DockStyle.Fill;
			PanelTopRight.Location = new System.Drawing.Point(379, 0);
			PanelTopRight.Margin = new System.Windows.Forms.Padding(0);
			PanelTopRight.Name = "PanelTopRight";
			PanelTopRight.Size = new System.Drawing.Size(253, 77);
			PanelTopRight.TabIndex = 1;
			// 
			// ListViewStates
			// 
			this.ListViewStates.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewStates.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            Column1});
			this.ListViewStates.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ListViewStates.Location = new System.Drawing.Point(80, 3);
			this.ListViewStates.Margin = new System.Windows.Forms.Padding(0);
			this.ListViewStates.MultiSelect = false;
			this.ListViewStates.Name = "ListViewStates";
			this.ListViewStates.ShowGroups = false;
			this.ListViewStates.Size = new System.Drawing.Size(170, 71);
			this.ListViewStates.TabIndex = 0;
			this.ListViewStates.UseCompatibleStateImageBehavior = false;
			this.ListViewStates.View = System.Windows.Forms.View.SmallIcon;
			this.ListViewStates.ItemActivate += new System.EventHandler(this.ListViewStates_ItemActivate);
			// 
			// Column1
			// 
			Column1.Text = "";
			Column1.Width = 160;
			// 
			// LabelStates
			// 
			this.LabelStates.Location = new System.Drawing.Point(6, 6);
			this.LabelStates.Name = "LabelStates";
			this.LabelStates.Size = new System.Drawing.Size(65, 59);
			this.LabelStates.TabIndex = 0;
			this.LabelStates.Text = "Used by state(s)";
			// 
			// toolStripSeparator4
			// 
			toolStripSeparator4.Name = "toolStripSeparator4";
			toolStripSeparator4.Size = new System.Drawing.Size(6, 27);
			// 
			// toolStripSeparator5
			// 
			toolStripSeparator5.Name = "toolStripSeparator5";
			toolStripSeparator5.Size = new System.Drawing.Size(6, 27);
			// 
			// PanelPreviewRate
			// 
			this.PanelPreviewRate.AutoSize = true;
			this.PanelPreviewRate.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelPreviewRate.ColumnCount = 2;
			this.PanelPreviewRate.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelPreviewRate.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelPreviewRate.Controls.Add(this.LabelSlow, 0, 0);
			this.PanelPreviewRate.Controls.Add(this.SliderRate, 0, 1);
			this.PanelPreviewRate.Controls.Add(this.LabelFast, 1, 0);
			this.PanelPreviewRate.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelPreviewRate.Location = new System.Drawing.Point(0, 45);
			this.PanelPreviewRate.Margin = new System.Windows.Forms.Padding(0, 18, 0, 0);
			this.PanelPreviewRate.Name = "PanelPreviewRate";
			this.PanelPreviewRate.RowCount = 2;
			this.PanelPreviewRate.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelPreviewRate.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelPreviewRate.Size = new System.Drawing.Size(183, 73);
			this.PanelPreviewRate.TabIndex = 4;
			// 
			// LabelSlow
			// 
			this.LabelSlow.AutoSize = true;
			this.LabelSlow.Location = new System.Drawing.Point(3, 0);
			this.LabelSlow.Name = "LabelSlow";
			this.LabelSlow.Size = new System.Drawing.Size(35, 17);
			this.LabelSlow.TabIndex = 2;
			this.LabelSlow.Text = "slow";
			this.LabelSlow.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// SliderRate
			// 
			this.PanelPreviewRate.SetColumnSpan(this.SliderRate, 2);
			this.SliderRate.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SliderRate.LargeChange = 1;
			this.SliderRate.Location = new System.Drawing.Point(0, 17);
			this.SliderRate.Margin = new System.Windows.Forms.Padding(0);
			this.SliderRate.Maximum = 1;
			this.SliderRate.Minimum = -2;
			this.SliderRate.Name = "SliderRate";
			this.SliderRate.Size = new System.Drawing.Size(183, 56);
			this.SliderRate.TabIndex = 1;
			this.SliderRate.TickStyle = System.Windows.Forms.TickStyle.Both;
			this.SliderRate.Scroll += new System.EventHandler(this.TrackBarRate_Scroll);
			// 
			// LabelFast
			// 
			this.LabelFast.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelFast.AutoSize = true;
			this.LabelFast.Location = new System.Drawing.Point(149, 0);
			this.LabelFast.Name = "LabelFast";
			this.LabelFast.Size = new System.Drawing.Size(31, 17);
			this.LabelFast.TabIndex = 3;
			this.LabelFast.Text = "fast";
			this.LabelFast.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// GroupBoxFrames
			// 
			this.GroupBoxFrames.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxFrames.AutoSize = true;
			this.GroupBoxFrames.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.GroupBoxFrames.Controls.Add(this.PanelFrames);
			this.GroupBoxFrames.Location = new System.Drawing.Point(0, 86);
			this.GroupBoxFrames.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxFrames.Name = "GroupBoxFrames";
			this.GroupBoxFrames.Padding = new System.Windows.Forms.Padding(6, 12, 6, 6);
			this.GroupBoxFrames.Size = new System.Drawing.Size(632, 270);
			this.GroupBoxFrames.TabIndex = 1;
			this.GroupBoxFrames.TabStop = false;
			this.GroupBoxFrames.Text = "Frames";
			// 
			// PanelFrames
			// 
			this.PanelFrames.AutoSize = true;
			this.PanelFrames.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelFrames.ColumnCount = 1;
			this.PanelFrames.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelFrames.Controls.Add(this.FramesView, 0, 1);
			this.PanelFrames.Controls.Add(this.PanelFramesTop, 0, 0);
			this.PanelFrames.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelFrames.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelFrames.Location = new System.Drawing.Point(6, 27);
			this.PanelFrames.Margin = new System.Windows.Forms.Padding(0);
			this.PanelFrames.Name = "PanelFrames";
			this.PanelFrames.RowCount = 2;
			this.PanelFrames.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelFrames.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelFrames.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
			this.PanelFrames.Size = new System.Drawing.Size(620, 237);
			this.PanelFrames.TabIndex = 0;
			// 
			// FramesView
			// 
			this.FramesView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.FramesView.AutoScroll = true;
			this.FramesView.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.FramesView.BackColor = System.Drawing.SystemColors.Window;
			this.FramesView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.FramesView.Location = new System.Drawing.Point(0, 33);
			this.FramesView.Margin = new System.Windows.Forms.Padding(0, 3, 0, 0);
			this.FramesView.MinimumSize = new System.Drawing.Size(64, 64);
			this.FramesView.Name = "FramesView";
			this.FramesView.ShowBranching = true;
			this.FramesView.ShowExitBranching = true;
			this.FramesView.Size = new System.Drawing.Size(620, 204);
			this.FramesView.TabIndex = 0;
			// 
			// PanelFramesTop
			// 
			this.PanelFramesTop.ColumnCount = 5;
			this.PanelFramesTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelFramesTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelFramesTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelFramesTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelFramesTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelFramesTop.Controls.Add(this.LabelDurationDesc, 4, 0);
			this.PanelFramesTop.Controls.Add(this.NumericFrameDuration, 3, 0);
			this.PanelFramesTop.Controls.Add(this.LabelDuration, 2, 0);
			this.PanelFramesTop.Controls.Add(this.ToolStripFramesView, 1, 0);
			this.PanelFramesTop.Controls.Add(this.ToolBarFrames, 0, 0);
			this.PanelFramesTop.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelFramesTop.Location = new System.Drawing.Point(0, 0);
			this.PanelFramesTop.Margin = new System.Windows.Forms.Padding(0);
			this.PanelFramesTop.Name = "PanelFramesTop";
			this.PanelFramesTop.RowCount = 1;
			this.PanelFramesTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
			this.PanelFramesTop.Size = new System.Drawing.Size(620, 30);
			this.PanelFramesTop.TabIndex = 1;
			// 
			// LabelDurationDesc
			// 
			this.LabelDurationDesc.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.LabelDurationDesc.AutoSize = true;
			this.LabelDurationDesc.Location = new System.Drawing.Point(537, 6);
			this.LabelDurationDesc.Name = "LabelDurationDesc";
			this.LabelDurationDesc.Size = new System.Drawing.Size(80, 17);
			this.LabelDurationDesc.TabIndex = 4;
			this.LabelDurationDesc.Text = "(1/100 sec)";
			// 
			// NumericFrameDuration
			// 
			this.NumericFrameDuration.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.NumericFrameDuration.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericFrameDuration.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericFrameDuration.Location = new System.Drawing.Point(473, 4);
			this.NumericFrameDuration.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
			this.NumericFrameDuration.Name = "NumericFrameDuration";
			this.NumericFrameDuration.Size = new System.Drawing.Size(58, 22);
			this.NumericFrameDuration.TabIndex = 3;
			this.NumericFrameDuration.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
			this.NumericFrameDuration.Validated += new System.EventHandler(this.NumericFrameDuration_Validated);
			// 
			// LabelDuration
			// 
			this.LabelDuration.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.LabelDuration.AutoSize = true;
			this.LabelDuration.Location = new System.Drawing.Point(336, 6);
			this.LabelDuration.Name = "LabelDuration";
			this.LabelDuration.Size = new System.Drawing.Size(131, 17);
			this.LabelDuration.TabIndex = 2;
			this.LabelDuration.Text = "New frame duration";
			// 
			// ToolStripFramesView
			// 
			this.ToolStripFramesView.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripFramesView.CanOverflow = false;
			this.ToolStripFramesView.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripFramesView.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripFramesView.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            toolStripSeparator1,
            this.ButtonShowBranching,
            this.ButtonShowExitBranching,
            toolStripSeparator3,
            this.ButtonViewSmall,
            this.ButtonViewMedium,
            this.ButtonViewLarge});
			this.ToolStripFramesView.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripFramesView.Location = new System.Drawing.Point(136, 0);
			this.ToolStripFramesView.Name = "ToolStripFramesView";
			this.ToolStripFramesView.Size = new System.Drawing.Size(129, 27);
			this.ToolStripFramesView.TabIndex = 1;
			// 
			// ButtonShowBranching
			// 
			this.ButtonShowBranching.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShowBranching.Image = global::AgentCharacterEditor.Properties.Resources.ImgShowBranching;
			this.ButtonShowBranching.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShowBranching.Name = "ButtonShowBranching";
			this.ButtonShowBranching.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonShowBranching.Size = new System.Drawing.Size(24, 24);
			this.ButtonShowBranching.Text = "Show branching";
			this.ButtonShowBranching.Click += new System.EventHandler(this.ButtonShowBranching_Click);
			// 
			// ButtonShowExitBranching
			// 
			this.ButtonShowExitBranching.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShowExitBranching.Image = global::AgentCharacterEditor.Properties.Resources.ImgShowExitBranching;
			this.ButtonShowExitBranching.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShowExitBranching.Name = "ButtonShowExitBranching";
			this.ButtonShowExitBranching.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonShowExitBranching.Size = new System.Drawing.Size(24, 24);
			this.ButtonShowExitBranching.Text = "Show exit branching";
			this.ButtonShowExitBranching.Click += new System.EventHandler(this.ButtonShowExitBranching_Click);
			// 
			// ButtonViewSmall
			// 
			this.ButtonViewSmall.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonViewSmall.Image = global::AgentCharacterEditor.Properties.Resources.ImgViewSmall;
			this.ButtonViewSmall.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonViewSmall.Name = "ButtonViewSmall";
			this.ButtonViewSmall.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonViewSmall.Size = new System.Drawing.Size(24, 24);
			this.ButtonViewSmall.Text = "Small";
			this.ButtonViewSmall.Click += new System.EventHandler(this.ButtonViewSmall_Click);
			// 
			// ButtonViewMedium
			// 
			this.ButtonViewMedium.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonViewMedium.Image = global::AgentCharacterEditor.Properties.Resources.ImgViewMedium;
			this.ButtonViewMedium.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonViewMedium.Name = "ButtonViewMedium";
			this.ButtonViewMedium.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonViewMedium.Size = new System.Drawing.Size(24, 24);
			this.ButtonViewMedium.Text = "Medium";
			this.ButtonViewMedium.Click += new System.EventHandler(this.ButtonViewMedium_Click);
			// 
			// ButtonViewLarge
			// 
			this.ButtonViewLarge.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonViewLarge.Image = global::AgentCharacterEditor.Properties.Resources.ImgViewLarge;
			this.ButtonViewLarge.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonViewLarge.Name = "ButtonViewLarge";
			this.ButtonViewLarge.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonViewLarge.Size = new System.Drawing.Size(24, 24);
			this.ButtonViewLarge.Text = "Large";
			this.ButtonViewLarge.Click += new System.EventHandler(this.ButtonViewLarge_Click);
			// 
			// ToolBarFrames
			// 
			this.ToolBarFrames.BackColor = System.Drawing.Color.Transparent;
			this.ToolBarFrames.CanOverflow = false;
			this.ToolBarFrames.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolBarFrames.Frame = null;
			this.ToolBarFrames.FramesPreview = null;
			this.ToolBarFrames.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolBarFrames.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolBarFrames.Location = new System.Drawing.Point(0, 0);
			this.ToolBarFrames.Name = "ToolBarFrames";
			this.ToolBarFrames.Size = new System.Drawing.Size(136, 27);
			this.ToolBarFrames.TabIndex = 0;
			// 
			// PanelTop
			// 
			this.PanelTop.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.PanelTop.ColumnCount = 2;
			this.PanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 60F));
			this.PanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
			this.PanelTop.Controls.Add(PanelTopLeft, 0, 0);
			this.PanelTop.Controls.Add(PanelTopRight, 1, 0);
			this.PanelTop.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelTop.Location = new System.Drawing.Point(0, 0);
			this.PanelTop.Margin = new System.Windows.Forms.Padding(0);
			this.PanelTop.Name = "PanelTop";
			this.PanelTop.RowCount = 1;
			this.PanelTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelTop.Size = new System.Drawing.Size(632, 77);
			this.PanelTop.TabIndex = 0;
			// 
			// PanelMain
			// 
			this.PanelMain.ColumnCount = 1;
			this.PanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Controls.Add(this.PanelTop, 0, 0);
			this.PanelMain.Controls.Add(this.GroupBoxFrames, 0, 1);
			this.PanelMain.Controls.Add(this.GroupBoxPreview, 0, 2);
			this.PanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.PanelMain.Location = new System.Drawing.Point(9, 0);
			this.PanelMain.Margin = new System.Windows.Forms.Padding(0);
			this.PanelMain.Name = "PanelMain";
			this.PanelMain.RowCount = 3;
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelMain.Size = new System.Drawing.Size(632, 600);
			this.PanelMain.TabIndex = 6;
			// 
			// GroupBoxPreview
			// 
			this.GroupBoxPreview.Controls.Add(this.PanelPreview);
			this.GroupBoxPreview.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxPreview.Location = new System.Drawing.Point(0, 365);
			this.GroupBoxPreview.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxPreview.Name = "GroupBoxPreview";
			this.GroupBoxPreview.Padding = new System.Windows.Forms.Padding(6, 12, 6, 6);
			this.GroupBoxPreview.Size = new System.Drawing.Size(632, 235);
			this.GroupBoxPreview.TabIndex = 2;
			this.GroupBoxPreview.TabStop = false;
			this.GroupBoxPreview.Text = "Preview";
			// 
			// PanelPreview
			// 
			this.PanelPreview.ColumnCount = 3;
			this.PanelPreview.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelPreview.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelPreview.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelPreview.Controls.Add(this.PanelPreviewControls, 1, 0);
			this.PanelPreview.Controls.Add(this.AnimationPreview, 0, 0);
			this.PanelPreview.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelPreview.Location = new System.Drawing.Point(6, 27);
			this.PanelPreview.Margin = new System.Windows.Forms.Padding(0);
			this.PanelPreview.Name = "PanelPreview";
			this.PanelPreview.RowCount = 1;
			this.PanelPreview.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelPreview.Size = new System.Drawing.Size(620, 202);
			this.PanelPreview.TabIndex = 0;
			// 
			// PanelPreviewControls
			// 
			this.PanelPreviewControls.AutoSize = true;
			this.PanelPreviewControls.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelPreviewControls.ColumnCount = 1;
			this.PanelPreviewControls.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.PanelPreviewControls.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelPreviewControls.Controls.Add(this.PanelPreviewRate, 0, 1);
			this.PanelPreviewControls.Controls.Add(this.ToolStripPreview, 0, 0);
			this.PanelPreviewControls.Location = new System.Drawing.Point(142, 0);
			this.PanelPreviewControls.Margin = new System.Windows.Forms.Padding(0);
			this.PanelPreviewControls.Name = "PanelPreviewControls";
			this.PanelPreviewControls.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.PanelPreviewControls.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.PanelPreviewControls.Size = new System.Drawing.Size(183, 118);
			this.PanelPreviewControls.TabIndex = 2;
			// 
			// ToolStripPreview
			// 
			this.ToolStripPreview.CanOverflow = false;
			this.ToolStripPreview.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripPreview.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripPreview.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.PreviewButtonPlay,
            this.PreviewButtonStop,
            this.PreviewButtonPause,
            toolStripSeparator4,
            this.PreviewButtonSkipBack,
            this.PreviewButtonSkipForward,
            toolStripSeparator5,
            this.PreviewButtonRepeat,
            this.PreviewButtonMute});
			this.ToolStripPreview.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripPreview.Location = new System.Drawing.Point(0, 0);
			this.ToolStripPreview.Name = "ToolStripPreview";
			this.ToolStripPreview.Size = new System.Drawing.Size(183, 27);
			this.ToolStripPreview.TabIndex = 0;
			this.ToolStripPreview.Text = "toolStripEx1";
			// 
			// PreviewButtonPlay
			// 
			this.PreviewButtonPlay.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonPlay.Image = global::AgentCharacterEditor.Properties.Resources.ImgPlay;
			this.PreviewButtonPlay.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonPlay.Name = "PreviewButtonPlay";
			this.PreviewButtonPlay.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonPlay.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonPlay.Text = "Play";
			this.PreviewButtonPlay.Click += new System.EventHandler(this.PreviewButtonPlay_Click);
			// 
			// PreviewButtonStop
			// 
			this.PreviewButtonStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonStop.Image = global::AgentCharacterEditor.Properties.Resources.ImgStop;
			this.PreviewButtonStop.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonStop.Name = "PreviewButtonStop";
			this.PreviewButtonStop.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonStop.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonStop.Text = "Stop";
			this.PreviewButtonStop.Click += new System.EventHandler(this.PreviewButtonStop_Click);
			// 
			// PreviewButtonPause
			// 
			this.PreviewButtonPause.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonPause.Image = global::AgentCharacterEditor.Properties.Resources.ImgPause;
			this.PreviewButtonPause.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonPause.Name = "PreviewButtonPause";
			this.PreviewButtonPause.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonPause.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonPause.Text = "Pause";
			this.PreviewButtonPause.Click += new System.EventHandler(this.PreviewButtonPause_Click);
			// 
			// PreviewButtonSkipBack
			// 
			this.PreviewButtonSkipBack.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonSkipBack.Image = global::AgentCharacterEditor.Properties.Resources.ImgSkipBack;
			this.PreviewButtonSkipBack.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonSkipBack.Name = "PreviewButtonSkipBack";
			this.PreviewButtonSkipBack.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonSkipBack.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonSkipBack.Text = "Back";
			this.PreviewButtonSkipBack.Click += new System.EventHandler(this.PreviewButtonSkipBack_Click);
			// 
			// PreviewButtonSkipForward
			// 
			this.PreviewButtonSkipForward.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonSkipForward.Image = global::AgentCharacterEditor.Properties.Resources.ImgSkipForward;
			this.PreviewButtonSkipForward.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonSkipForward.Name = "PreviewButtonSkipForward";
			this.PreviewButtonSkipForward.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonSkipForward.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonSkipForward.Text = "Forward";
			this.PreviewButtonSkipForward.Click += new System.EventHandler(this.PreviewButtonSkipForward_Click);
			// 
			// PreviewButtonRepeat
			// 
			this.PreviewButtonRepeat.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonRepeat.Image = global::AgentCharacterEditor.Properties.Resources.ImgRepeat;
			this.PreviewButtonRepeat.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonRepeat.Name = "PreviewButtonRepeat";
			this.PreviewButtonRepeat.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonRepeat.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonRepeat.Text = "Repeat";
			this.PreviewButtonRepeat.Click += new System.EventHandler(this.PreviewButtonRepeat_Click);
			// 
			// PreviewButtonMute
			// 
			this.PreviewButtonMute.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PreviewButtonMute.Image = global::AgentCharacterEditor.Properties.Resources.ImgMute;
			this.PreviewButtonMute.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PreviewButtonMute.Name = "PreviewButtonMute";
			this.PreviewButtonMute.Padding = new System.Windows.Forms.Padding(2);
			this.PreviewButtonMute.Size = new System.Drawing.Size(24, 24);
			this.PreviewButtonMute.Text = "Mute";
			this.PreviewButtonMute.Click += new System.EventHandler(this.PreviewButtonMute_Click);
			// 
			// AnimationPreview
			// 
			this.AnimationPreview.AutoSize = true;
			this.AnimationPreview.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.AnimationPreview.BackColor = System.Drawing.SystemColors.Window;
			this.AnimationPreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.AnimationPreview.CausesValidation = false;
			this.AnimationPreview.Location = new System.Drawing.Point(0, 0);
			this.AnimationPreview.Margin = new System.Windows.Forms.Padding(0, 0, 12, 0);
			this.AnimationPreview.MaximumSize = new System.Drawing.Size(258, 258);
			this.AnimationPreview.MinimumSize = new System.Drawing.Size(18, 18);
			this.AnimationPreview.Name = "AnimationPreview";
			this.AnimationPreview.Size = new System.Drawing.Size(130, 130);
			this.AnimationPreview.TabIndex = 0;
			this.AnimationPreview.TabStop = false;
			// 
			// AnimationPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.PanelMain);
			this.Name = "AnimationPanel";
			PanelTopLeft.ResumeLayout(false);
			PanelTopLeft.PerformLayout();
			PanelTopRight.ResumeLayout(false);
			this.PanelPreviewRate.ResumeLayout(false);
			this.PanelPreviewRate.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.SliderRate)).EndInit();
			this.GroupBoxFrames.ResumeLayout(false);
			this.GroupBoxFrames.PerformLayout();
			this.PanelFrames.ResumeLayout(false);
			this.PanelFramesTop.ResumeLayout(false);
			this.PanelFramesTop.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameDuration)).EndInit();
			this.ToolStripFramesView.ResumeLayout(false);
			this.ToolStripFramesView.PerformLayout();
			this.PanelTop.ResumeLayout(false);
			this.PanelMain.ResumeLayout(false);
			this.PanelMain.PerformLayout();
			this.GroupBoxPreview.ResumeLayout(false);
			this.PanelPreview.ResumeLayout(false);
			this.PanelPreview.PerformLayout();
			this.PanelPreviewControls.ResumeLayout(false);
			this.PanelPreviewControls.PerformLayout();
			this.ToolStripPreview.ResumeLayout(false);
			this.ToolStripPreview.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private DoubleAgent.LabelCompat LabelName;
		private DoubleAgent.TextBoxEx TextBoxName;
		private DoubleAgent.LabelCompat LabelReturn;
		private DoubleAgent.ComboBoxCompat ComboBoxReturn;
		private DoubleAgent.GroupBoxCompat GroupBoxFrames;
		private AgentCharacterEditor.Previews.FramesPreview FramesView;
		private DoubleAgent.ListViewEx ListViewStates;
		private System.Windows.Forms.TableLayoutPanel PanelFrames;
		private DoubleAgent.ToolStripEx ToolStripFramesView;
		private System.Windows.Forms.TableLayoutPanel PanelFramesTop;
		private DoubleAgent.LabelCompat LabelDurationDesc;
		private DoubleAgent.NumericUpDownEx NumericFrameDuration;
		private DoubleAgent.LabelCompat LabelDuration;
		private DoubleAgent.ToolStripButtonCompat ButtonViewSmall;
		private DoubleAgent.ToolStripButtonCompat ButtonViewMedium;
		private DoubleAgent.ToolStripButtonCompat ButtonViewLarge;
		private DoubleAgent.ToolStripButtonCompat ButtonShowBranching;
		private DoubleAgent.ToolStripButtonCompat ButtonShowExitBranching;
		private System.Windows.Forms.TableLayoutPanel PanelTop;
		private DoubleAgent.LabelCompat LabelStates;
		private System.Windows.Forms.TableLayoutPanel PanelMain;
		private DoubleAgent.GroupBoxCompat GroupBoxPreview;
		private System.Windows.Forms.TableLayoutPanel PanelPreview;
		private AgentCharacterEditor.Previews.AnimationPreviewHost AnimationPreview;
		private System.Windows.Forms.TableLayoutPanel PanelPreviewControls;
		private System.Windows.Forms.TrackBar SliderRate;
		private DoubleAgent.ToolStripEx ToolStripPreview;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonPlay;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonPause;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonStop;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonSkipBack;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonSkipForward;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonRepeat;
		private DoubleAgent.ToolStripButtonCompat PreviewButtonMute;
		private DoubleAgent.LabelCompat LabelSlow;
		private DoubleAgent.LabelCompat LabelFast;
		private System.Windows.Forms.TableLayoutPanel PanelPreviewRate;
		private Previews.FramesToolBar ToolBarFrames;
	}
}
