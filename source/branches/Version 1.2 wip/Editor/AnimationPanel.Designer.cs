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
			System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
			System.Windows.Forms.Panel PanelTopLeft;
			System.Windows.Forms.Panel PanelTopRight;
			System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("GesturingDown");
			System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem("GesturingLeft");
			System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem("GesturingRight");
			System.Windows.Forms.ListViewItem listViewItem4 = new System.Windows.Forms.ListViewItem("GesturingUp");
			System.Windows.Forms.ListViewItem listViewItem5 = new System.Windows.Forms.ListViewItem("Listening");
			System.Windows.Forms.ListViewItem listViewItem6 = new System.Windows.Forms.ListViewItem("Hearing");
			System.Windows.Forms.ListViewItem listViewItem7 = new System.Windows.Forms.ListViewItem("Hiding");
			System.Windows.Forms.ListViewItem listViewItem8 = new System.Windows.Forms.ListViewItem("IdlingLevel1");
			System.Windows.Forms.ListViewItem listViewItem9 = new System.Windows.Forms.ListViewItem("IdlingLevel2");
			System.Windows.Forms.ListViewItem listViewItem10 = new System.Windows.Forms.ListViewItem("IdlingLevel3");
			System.Windows.Forms.ListViewItem listViewItem11 = new System.Windows.Forms.ListViewItem("MovingDown");
			System.Windows.Forms.ListViewItem listViewItem12 = new System.Windows.Forms.ListViewItem("MovingLeft");
			System.Windows.Forms.ListViewItem listViewItem13 = new System.Windows.Forms.ListViewItem("MovingRight");
			System.Windows.Forms.ListViewItem listViewItem14 = new System.Windows.Forms.ListViewItem("MovingUp");
			System.Windows.Forms.ListViewItem listViewItem15 = new System.Windows.Forms.ListViewItem("Showing");
			System.Windows.Forms.ListViewItem listViewItem16 = new System.Windows.Forms.ListViewItem("Speaking");
			System.Windows.Forms.ColumnHeader Column1;
			this.LabelName = new System.Windows.Forms.Label();
			this.TextBoxName = new DoubleAgent.TextBoxEx();
			this.LabelReturn = new System.Windows.Forms.Label();
			this.ComboBoxReturn = new System.Windows.Forms.ComboBox();
			this.ListViewStates = new DoubleAgent.ListViewEx();
			this.LabelStates = new System.Windows.Forms.Label();
			this.GroupBoxFrames = new System.Windows.Forms.GroupBox();
			this.LayoutPanelFrames = new System.Windows.Forms.TableLayoutPanel();
			this.FramesView = new AgentCharacterEditor.FramesPreview();
			this.PanelFramesTop = new System.Windows.Forms.Panel();
			this.LabelDurationDesc = new System.Windows.Forms.Label();
			this.NumericFrameDuration = new DoubleAgent.NumericUpDownEx();
			this.LabelDuration = new System.Windows.Forms.Label();
			this.ToolStripFrames = new DoubleAgent.ToolStripEx();
			this.ButtonAdd = new System.Windows.Forms.ToolStripButton();
			this.ButtonDelete = new System.Windows.Forms.ToolStripButton();
			this.ButtonMoveUp = new System.Windows.Forms.ToolStripButton();
			this.ButtonMoveDown = new System.Windows.Forms.ToolStripButton();
			this.ButtonShowBranching = new System.Windows.Forms.ToolStripButton();
			this.ButtonShowExitBranching = new System.Windows.Forms.ToolStripButton();
			this.ButtonViewSmall = new System.Windows.Forms.ToolStripButton();
			this.ButtonViewMedium = new System.Windows.Forms.ToolStripButton();
			this.ButtonViewLarge = new System.Windows.Forms.ToolStripButton();
			this.LayoutPanelTop = new System.Windows.Forms.TableLayoutPanel();
			this.LayoutPanelMain = new System.Windows.Forms.TableLayoutPanel();
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			PanelTopLeft = new System.Windows.Forms.Panel();
			PanelTopRight = new System.Windows.Forms.Panel();
			Column1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			PanelTopLeft.SuspendLayout();
			PanelTopRight.SuspendLayout();
			this.GroupBoxFrames.SuspendLayout();
			this.LayoutPanelFrames.SuspendLayout();
			this.PanelFramesTop.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameDuration)).BeginInit();
			this.ToolStripFrames.SuspendLayout();
			this.LayoutPanelTop.SuspendLayout();
			this.LayoutPanelMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size(6, 26);
			// 
			// toolStripSeparator2
			// 
			toolStripSeparator2.Name = "toolStripSeparator2";
			toolStripSeparator2.Size = new System.Drawing.Size(6, 26);
			// 
			// toolStripSeparator3
			// 
			toolStripSeparator3.Name = "toolStripSeparator3";
			toolStripSeparator3.Size = new System.Drawing.Size(6, 26);
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
			this.LabelReturn.TabIndex = 2;
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
			listViewItem1.StateImageIndex = 0;
			listViewItem2.StateImageIndex = 0;
			listViewItem3.StateImageIndex = 0;
			listViewItem4.StateImageIndex = 0;
			listViewItem5.StateImageIndex = 0;
			listViewItem6.StateImageIndex = 0;
			listViewItem7.StateImageIndex = 0;
			listViewItem8.StateImageIndex = 0;
			listViewItem9.StateImageIndex = 0;
			listViewItem10.StateImageIndex = 0;
			listViewItem11.StateImageIndex = 0;
			listViewItem12.StateImageIndex = 0;
			listViewItem13.StateImageIndex = 0;
			listViewItem14.StateImageIndex = 0;
			listViewItem15.StateImageIndex = 0;
			listViewItem16.StateImageIndex = 0;
			this.ListViewStates.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
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
            listViewItem16});
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
			// GroupBoxFrames
			// 
			this.GroupBoxFrames.Controls.Add(this.LayoutPanelFrames);
			this.GroupBoxFrames.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GroupBoxFrames.Location = new System.Drawing.Point(0, 86);
			this.GroupBoxFrames.Margin = new System.Windows.Forms.Padding(0, 9, 0, 0);
			this.GroupBoxFrames.Name = "GroupBoxFrames";
			this.GroupBoxFrames.Padding = new System.Windows.Forms.Padding(6, 12, 6, 6);
			this.GroupBoxFrames.Size = new System.Drawing.Size(632, 514);
			this.GroupBoxFrames.TabIndex = 0;
			this.GroupBoxFrames.TabStop = false;
			this.GroupBoxFrames.Text = "Frames";
			// 
			// LayoutPanelFrames
			// 
			this.LayoutPanelFrames.AutoSize = true;
			this.LayoutPanelFrames.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.LayoutPanelFrames.ColumnCount = 1;
			this.LayoutPanelFrames.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelFrames.Controls.Add(this.FramesView, 0, 1);
			this.LayoutPanelFrames.Controls.Add(this.PanelFramesTop, 0, 0);
			this.LayoutPanelFrames.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanelFrames.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.LayoutPanelFrames.Location = new System.Drawing.Point(6, 27);
			this.LayoutPanelFrames.Margin = new System.Windows.Forms.Padding(0);
			this.LayoutPanelFrames.Name = "LayoutPanelFrames";
			this.LayoutPanelFrames.RowCount = 2;
			this.LayoutPanelFrames.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanelFrames.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelFrames.Size = new System.Drawing.Size(620, 481);
			this.LayoutPanelFrames.TabIndex = 0;
			// 
			// FramesView
			// 
			this.FramesView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.FramesView.AutoScroll = true;
			this.FramesView.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.FramesView.BackColor = System.Drawing.SystemColors.Window;
			this.FramesView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.FramesView.Location = new System.Drawing.Point(0, 29);
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
			this.PanelFramesTop.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.PanelFramesTop.AutoSize = true;
			this.PanelFramesTop.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelFramesTop.Controls.Add(this.LabelDurationDesc);
			this.PanelFramesTop.Controls.Add(this.NumericFrameDuration);
			this.PanelFramesTop.Controls.Add(this.LabelDuration);
			this.PanelFramesTop.Controls.Add(this.ToolStripFrames);
			this.PanelFramesTop.Location = new System.Drawing.Point(0, 0);
			this.PanelFramesTop.Margin = new System.Windows.Forms.Padding(0);
			this.PanelFramesTop.Name = "PanelFramesTop";
			this.PanelFramesTop.Size = new System.Drawing.Size(620, 26);
			this.PanelFramesTop.TabIndex = 1;
			// 
			// LabelDurationDesc
			// 
			this.LabelDurationDesc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelDurationDesc.AutoSize = true;
			this.LabelDurationDesc.Location = new System.Drawing.Point(537, 2);
			this.LabelDurationDesc.Name = "LabelDurationDesc";
			this.LabelDurationDesc.Size = new System.Drawing.Size(80, 17);
			this.LabelDurationDesc.TabIndex = 3;
			this.LabelDurationDesc.Text = "(1/100 sec)";
			// 
			// NumericFrameDuration
			// 
			this.NumericFrameDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.NumericFrameDuration.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericFrameDuration.HighlightBackColor = System.Drawing.Color.Pink;
			this.NumericFrameDuration.Location = new System.Drawing.Point(473, 0);
			this.NumericFrameDuration.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
			this.NumericFrameDuration.Name = "NumericFrameDuration";
			this.NumericFrameDuration.Size = new System.Drawing.Size(58, 22);
			this.NumericFrameDuration.TabIndex = 2;
			this.NumericFrameDuration.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
			this.NumericFrameDuration.Validated += new System.EventHandler(this.NumericFrameDuration_Validated);
			// 
			// LabelDuration
			// 
			this.LabelDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelDuration.AutoSize = true;
			this.LabelDuration.Location = new System.Drawing.Point(336, 2);
			this.LabelDuration.Name = "LabelDuration";
			this.LabelDuration.Size = new System.Drawing.Size(131, 17);
			this.LabelDuration.TabIndex = 1;
			this.LabelDuration.Text = "New frame duration";
			// 
			// ToolStripFrames
			// 
			this.ToolStripFrames.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripFrames.CanOverflow = false;
			this.ToolStripFrames.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripFrames.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripFrames.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonDelete,
            toolStripSeparator1,
            this.ButtonMoveUp,
            this.ButtonMoveDown,
            toolStripSeparator2,
            this.ButtonShowBranching,
            this.ButtonShowExitBranching,
            toolStripSeparator3,
            this.ButtonViewSmall,
            this.ButtonViewMedium,
            this.ButtonViewLarge});
			this.ToolStripFrames.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripFrames.Location = new System.Drawing.Point(0, 0);
			this.ToolStripFrames.Name = "ToolStripFrames";
			this.ToolStripFrames.Size = new System.Drawing.Size(259, 26);
			this.ToolStripFrames.TabIndex = 0;
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonAdd.Size = new System.Drawing.Size(23, 23);
			this.ButtonAdd.Text = "Add a new Frame";
			this.ButtonAdd.Click += new System.EventHandler(this.ButtonAdd_Click);
			// 
			// ButtonDelete
			// 
			this.ButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.ButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonDelete.Name = "ButtonDelete";
			this.ButtonDelete.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonDelete.Size = new System.Drawing.Size(23, 23);
			this.ButtonDelete.Text = "Delete frame";
			this.ButtonDelete.Click += new System.EventHandler(this.ButtonDelete_Click);
			// 
			// ButtonMoveUp
			// 
			this.ButtonMoveUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveUp.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveLeft;
			this.ButtonMoveUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveUp.Name = "ButtonMoveUp";
			this.ButtonMoveUp.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonMoveUp.Size = new System.Drawing.Size(23, 23);
			this.ButtonMoveUp.Text = "Move frame up";
			this.ButtonMoveUp.Click += new System.EventHandler(this.ButtonMoveUp_Click);
			// 
			// ButtonMoveDown
			// 
			this.ButtonMoveDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveDown.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveRight;
			this.ButtonMoveDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveDown.Name = "ButtonMoveDown";
			this.ButtonMoveDown.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonMoveDown.Size = new System.Drawing.Size(23, 23);
			this.ButtonMoveDown.Text = "Move frame down";
			this.ButtonMoveDown.Click += new System.EventHandler(this.ButtonMoveDown_Click);
			// 
			// ButtonShowBranching
			// 
			this.ButtonShowBranching.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShowBranching.Image = global::AgentCharacterEditor.Properties.Resources.ImgShowBranching;
			this.ButtonShowBranching.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShowBranching.Name = "ButtonShowBranching";
			this.ButtonShowBranching.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonShowBranching.Size = new System.Drawing.Size(23, 23);
			this.ButtonShowBranching.Text = "Show branching";
			this.ButtonShowBranching.Click += new System.EventHandler(this.ButtonShowBranching_Click);
			// 
			// ButtonShowExitBranching
			// 
			this.ButtonShowExitBranching.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonShowExitBranching.Image = global::AgentCharacterEditor.Properties.Resources.ImgShowExitBranching;
			this.ButtonShowExitBranching.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonShowExitBranching.Name = "ButtonShowExitBranching";
			this.ButtonShowExitBranching.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonShowExitBranching.Size = new System.Drawing.Size(23, 23);
			this.ButtonShowExitBranching.Text = "Show exit branching";
			this.ButtonShowExitBranching.Click += new System.EventHandler(this.ButtonShowExitBranching_Click);
			// 
			// ButtonViewSmall
			// 
			this.ButtonViewSmall.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonViewSmall.Image = global::AgentCharacterEditor.Properties.Resources.ImgViewSmall;
			this.ButtonViewSmall.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonViewSmall.Name = "ButtonViewSmall";
			this.ButtonViewSmall.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonViewSmall.Size = new System.Drawing.Size(23, 23);
			this.ButtonViewSmall.Text = "Small";
			this.ButtonViewSmall.Click += new System.EventHandler(this.ButtonViewSmall_Click);
			// 
			// ButtonViewMedium
			// 
			this.ButtonViewMedium.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonViewMedium.Image = global::AgentCharacterEditor.Properties.Resources.ImgViewMedium;
			this.ButtonViewMedium.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonViewMedium.Name = "ButtonViewMedium";
			this.ButtonViewMedium.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonViewMedium.Size = new System.Drawing.Size(23, 23);
			this.ButtonViewMedium.Text = "Medium";
			this.ButtonViewMedium.Click += new System.EventHandler(this.ButtonViewMedium_Click);
			// 
			// ButtonViewLarge
			// 
			this.ButtonViewLarge.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonViewLarge.Image = global::AgentCharacterEditor.Properties.Resources.ImgViewLarge;
			this.ButtonViewLarge.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonViewLarge.Name = "ButtonViewLarge";
			this.ButtonViewLarge.Padding = new System.Windows.Forms.Padding(0, 1, 0, 2);
			this.ButtonViewLarge.Size = new System.Drawing.Size(23, 23);
			this.ButtonViewLarge.Text = "Large";
			this.ButtonViewLarge.Click += new System.EventHandler(this.ButtonViewLarge_Click);
			// 
			// LayoutPanelTop
			// 
			this.LayoutPanelTop.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.LayoutPanelTop.ColumnCount = 2;
			this.LayoutPanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 60F));
			this.LayoutPanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
			this.LayoutPanelTop.Controls.Add(PanelTopLeft, 0, 0);
			this.LayoutPanelTop.Controls.Add(PanelTopRight, 1, 0);
			this.LayoutPanelTop.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.LayoutPanelTop.Location = new System.Drawing.Point(0, 0);
			this.LayoutPanelTop.Margin = new System.Windows.Forms.Padding(0);
			this.LayoutPanelTop.Name = "LayoutPanelTop";
			this.LayoutPanelTop.RowCount = 1;
			this.LayoutPanelTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelTop.Size = new System.Drawing.Size(632, 77);
			this.LayoutPanelTop.TabIndex = 5;
			// 
			// LayoutPanelMain
			// 
			this.LayoutPanelMain.ColumnCount = 1;
			this.LayoutPanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Controls.Add(this.LayoutPanelTop, 0, 0);
			this.LayoutPanelMain.Controls.Add(this.GroupBoxFrames, 0, 1);
			this.LayoutPanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LayoutPanelMain.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
			this.LayoutPanelMain.Location = new System.Drawing.Point(9, 0);
			this.LayoutPanelMain.Margin = new System.Windows.Forms.Padding(0);
			this.LayoutPanelMain.Name = "LayoutPanelMain";
			this.LayoutPanelMain.RowCount = 2;
			this.LayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.LayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.LayoutPanelMain.Size = new System.Drawing.Size(632, 600);
			this.LayoutPanelMain.TabIndex = 6;
			// 
			// AnimationPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.LayoutPanelMain);
			this.Name = "AnimationPanel";
			PanelTopLeft.ResumeLayout(false);
			PanelTopLeft.PerformLayout();
			PanelTopRight.ResumeLayout(false);
			this.GroupBoxFrames.ResumeLayout(false);
			this.GroupBoxFrames.PerformLayout();
			this.LayoutPanelFrames.ResumeLayout(false);
			this.LayoutPanelFrames.PerformLayout();
			this.PanelFramesTop.ResumeLayout(false);
			this.PanelFramesTop.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameDuration)).EndInit();
			this.ToolStripFrames.ResumeLayout(false);
			this.ToolStripFrames.PerformLayout();
			this.LayoutPanelTop.ResumeLayout(false);
			this.LayoutPanelMain.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label LabelName;
		private DoubleAgent.TextBoxEx TextBoxName;
		private System.Windows.Forms.Label LabelReturn;
		private System.Windows.Forms.ComboBox ComboBoxReturn;
		private System.Windows.Forms.GroupBox GroupBoxFrames;
		private FramesPreview FramesView;
		private DoubleAgent.ListViewEx ListViewStates;
		private System.Windows.Forms.TableLayoutPanel LayoutPanelFrames;
		private DoubleAgent.ToolStripEx ToolStripFrames;
		private System.Windows.Forms.ToolStripButton ButtonAdd;
		private System.Windows.Forms.ToolStripButton ButtonDelete;
		private System.Windows.Forms.ToolStripButton ButtonMoveUp;
		private System.Windows.Forms.ToolStripButton ButtonMoveDown;
		private System.Windows.Forms.Panel PanelFramesTop;
		private System.Windows.Forms.Label LabelDurationDesc;
		private DoubleAgent.NumericUpDownEx NumericFrameDuration;
		private System.Windows.Forms.Label LabelDuration;
		private System.Windows.Forms.ToolStripButton ButtonViewSmall;
		private System.Windows.Forms.ToolStripButton ButtonViewMedium;
		private System.Windows.Forms.ToolStripButton ButtonViewLarge;
		private System.Windows.Forms.ToolStripButton ButtonShowBranching;
		private System.Windows.Forms.ToolStripButton ButtonShowExitBranching;
		private System.Windows.Forms.TableLayoutPanel LayoutPanelTop;
		private System.Windows.Forms.Label LabelStates;
		private System.Windows.Forms.TableLayoutPanel LayoutPanelMain;
	}
}
