namespace AgentCharacterEditor
{
	partial class AnimationForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (AnimationForm));
			System.Windows.Forms.ListViewItem listViewItem17 = new System.Windows.Forms.ListViewItem ("GesturingDown");
			System.Windows.Forms.ListViewItem listViewItem18 = new System.Windows.Forms.ListViewItem ("GesturingLeft");
			System.Windows.Forms.ListViewItem listViewItem19 = new System.Windows.Forms.ListViewItem ("GesturingRight");
			System.Windows.Forms.ListViewItem listViewItem20 = new System.Windows.Forms.ListViewItem ("GesturingUp");
			System.Windows.Forms.ListViewItem listViewItem21 = new System.Windows.Forms.ListViewItem ("Listening");
			System.Windows.Forms.ListViewItem listViewItem22 = new System.Windows.Forms.ListViewItem ("Hearing");
			System.Windows.Forms.ListViewItem listViewItem23 = new System.Windows.Forms.ListViewItem ("Hiding");
			System.Windows.Forms.ListViewItem listViewItem24 = new System.Windows.Forms.ListViewItem ("IdlingLevel1");
			System.Windows.Forms.ListViewItem listViewItem25 = new System.Windows.Forms.ListViewItem ("IdlingLevel2");
			System.Windows.Forms.ListViewItem listViewItem26 = new System.Windows.Forms.ListViewItem ("IdlingLevel3");
			System.Windows.Forms.ListViewItem listViewItem27 = new System.Windows.Forms.ListViewItem ("MovingDown");
			System.Windows.Forms.ListViewItem listViewItem28 = new System.Windows.Forms.ListViewItem ("MovingLeft");
			System.Windows.Forms.ListViewItem listViewItem29 = new System.Windows.Forms.ListViewItem ("MovingRight");
			System.Windows.Forms.ListViewItem listViewItem30 = new System.Windows.Forms.ListViewItem ("MovingUp");
			System.Windows.Forms.ListViewItem listViewItem31 = new System.Windows.Forms.ListViewItem ("Showing");
			System.Windows.Forms.ListViewItem listViewItem32 = new System.Windows.Forms.ListViewItem ("Speaking");
			this.LabelName = new System.Windows.Forms.Label ();
			this.LabelReturn = new System.Windows.Forms.Label ();
			this.ComboBoxReturn = new System.Windows.Forms.ComboBox ();
			this.GroupBoxFrames = new System.Windows.Forms.GroupBox ();
			this.TableLayoutFrames = new System.Windows.Forms.TableLayoutPanel ();
			this.ToolStripFrames = new DoubleAgent.ToolStripEx ();
			this.ButtonAdd = new System.Windows.Forms.ToolStripButton ();
			this.ButtonRemove = new System.Windows.Forms.ToolStripButton ();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator ();
			this.ButtonMoveUp = new System.Windows.Forms.ToolStripButton ();
			this.ButtonMoveDown = new System.Windows.Forms.ToolStripButton ();
			this.ListViewPreview = new DoubleAgent.ListViewEx ();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader ();
			this.TableLayoutMain = new System.Windows.Forms.TableLayoutPanel ();
			this.GroupBoxStates = new System.Windows.Forms.GroupBox ();
			this.ListViewStates = new DoubleAgent.ListViewEx ();
			this.Column1 = new System.Windows.Forms.ColumnHeader ();
			this.TextBoxName = new DoubleAgent.TextBoxEx ();
			this.PanelFramesTop = new System.Windows.Forms.Panel ();
			this.LabelDurationDesc = new System.Windows.Forms.Label ();
			this.NumericFrameDuration = new DoubleAgent.NumericUpDownEx ();
			this.LabelDuration = new System.Windows.Forms.Label ();
			this.GroupBoxFrames.SuspendLayout ();
			this.TableLayoutFrames.SuspendLayout ();
			this.ToolStripFrames.SuspendLayout ();
			this.TableLayoutMain.SuspendLayout ();
			this.GroupBoxStates.SuspendLayout ();
			this.PanelFramesTop.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameDuration)).BeginInit ();
			this.SuspendLayout ();
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.Location = new System.Drawing.Point (9, 6);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size (109, 17);
			this.LabelName.TabIndex = 0;
			this.LabelName.Text = "Animation name";
			// 
			// LabelReturn
			// 
			this.LabelReturn.AutoSize = true;
			this.LabelReturn.Location = new System.Drawing.Point (9, 34);
			this.LabelReturn.Name = "LabelReturn";
			this.LabelReturn.Size = new System.Drawing.Size (116, 17);
			this.LabelReturn.TabIndex = 2;
			this.LabelReturn.Text = "Return animation";
			// 
			// ComboBoxReturn
			// 
			this.ComboBoxReturn.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ComboBoxReturn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ComboBoxReturn.Items.AddRange (new object[] {
            "None"});
			this.ComboBoxReturn.Location = new System.Drawing.Point (140, 31);
			this.ComboBoxReturn.MaximumSize = new System.Drawing.Size (600, 0);
			this.ComboBoxReturn.Name = "ComboBoxReturn";
			this.ComboBoxReturn.Size = new System.Drawing.Size (500, 24);
			this.ComboBoxReturn.TabIndex = 3;
			this.ComboBoxReturn.SelectionChangeCommitted += new System.EventHandler (this.ComboBoxReturn_SelectionChangeCommitted);
			// 
			// GroupBoxFrames
			// 
			this.GroupBoxFrames.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxFrames.AutoSize = true;
			this.GroupBoxFrames.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.GroupBoxFrames.Controls.Add (this.TableLayoutFrames);
			this.GroupBoxFrames.Location = new System.Drawing.Point (0, 9);
			this.GroupBoxFrames.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxFrames.Name = "GroupBoxFrames";
			this.GroupBoxFrames.Padding = new System.Windows.Forms.Padding (6, 12, 6, 6);
			this.GroupBoxFrames.Size = new System.Drawing.Size (632, 190);
			this.GroupBoxFrames.TabIndex = 0;
			this.GroupBoxFrames.TabStop = false;
			this.GroupBoxFrames.Text = "Frames";
			// 
			// TableLayoutFrames
			// 
			this.TableLayoutFrames.AutoSize = true;
			this.TableLayoutFrames.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.TableLayoutFrames.ColumnCount = 1;
			this.TableLayoutFrames.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutFrames.Controls.Add (this.ListViewPreview, 0, 1);
			this.TableLayoutFrames.Controls.Add (this.PanelFramesTop, 0, 0);
			this.TableLayoutFrames.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TableLayoutFrames.Location = new System.Drawing.Point (6, 27);
			this.TableLayoutFrames.Margin = new System.Windows.Forms.Padding (0);
			this.TableLayoutFrames.Name = "TableLayoutFrames";
			this.TableLayoutFrames.RowCount = 2;
			this.TableLayoutFrames.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.TableLayoutFrames.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.TableLayoutFrames.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 20F));
			this.TableLayoutFrames.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 20F));
			this.TableLayoutFrames.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Absolute, 20F));
			this.TableLayoutFrames.Size = new System.Drawing.Size (620, 157);
			this.TableLayoutFrames.TabIndex = 0;
			// 
			// ToolStripFrames
			// 
			this.ToolStripFrames.BackColor = System.Drawing.Color.Transparent;
			this.ToolStripFrames.CanOverflow = false;
			this.ToolStripFrames.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripFrames.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripFrames.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonRemove,
            this.toolStripSeparator1,
            this.ButtonMoveUp,
            this.ButtonMoveDown});
			this.ToolStripFrames.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripFrames.Location = new System.Drawing.Point (0, 0);
			this.ToolStripFrames.Name = "ToolStripFrames";
			this.ToolStripFrames.RenderInline = true;
			this.ToolStripFrames.Size = new System.Drawing.Size (101, 26);
			this.ToolStripFrames.TabIndex = 0;
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = ((System.Drawing.Image)(resources.GetObject ("ButtonAdd.Image")));
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ButtonAdd.Size = new System.Drawing.Size (23, 23);
			this.ButtonAdd.Text = "Add a new frame";
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
			this.ButtonRemove.Text = "Remove this frame";
			this.ButtonRemove.Click += new System.EventHandler (this.ButtonRemove_Click);
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
			// ListViewPreview
			// 
			this.ListViewPreview.Alignment = System.Windows.Forms.ListViewAlignment.Left;
			this.ListViewPreview.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewPreview.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
			this.ListViewPreview.GridLines = true;
			this.ListViewPreview.HideSelection = false;
			this.ListViewPreview.LabelWrap = false;
			this.ListViewPreview.Location = new System.Drawing.Point (0, 29);
			this.ListViewPreview.Margin = new System.Windows.Forms.Padding (0, 3, 0, 0);
			this.ListViewPreview.MinimumSize = new System.Drawing.Size (128, 128);
			this.ListViewPreview.MultiSelect = false;
			this.ListViewPreview.Name = "ListViewPreview";
			this.ListViewPreview.SelectedIndex = -1;
			this.ListViewPreview.SelectedItem = null;
			this.ListViewPreview.ShowGroups = false;
			this.ListViewPreview.Size = new System.Drawing.Size (620, 128);
			this.ListViewPreview.TabIndex = 0;
			this.ListViewPreview.UseCompatibleStateImageBehavior = false;
			this.ListViewPreview.SelectedIndexChanged += new System.EventHandler (this.ListViewPreview_SelectedIndexChanged);
			// 
			// TableLayoutMain
			// 
			this.TableLayoutMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TableLayoutMain.ColumnCount = 1;
			this.TableLayoutMain.ColumnStyles.Add (new System.Windows.Forms.ColumnStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutMain.Controls.Add (this.GroupBoxFrames, 0, 0);
			this.TableLayoutMain.Controls.Add (this.GroupBoxStates, 0, 1);
			this.TableLayoutMain.Location = new System.Drawing.Point (9, 58);
			this.TableLayoutMain.Margin = new System.Windows.Forms.Padding (0);
			this.TableLayoutMain.Name = "TableLayoutMain";
			this.TableLayoutMain.RowCount = 2;
			this.TableLayoutMain.RowStyles.Add (new System.Windows.Forms.RowStyle ());
			this.TableLayoutMain.RowStyles.Add (new System.Windows.Forms.RowStyle (System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutMain.Size = new System.Drawing.Size (632, 542);
			this.TableLayoutMain.TabIndex = 4;
			// 
			// GroupBoxStates
			// 
			this.GroupBoxStates.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.GroupBoxStates.Controls.Add (this.ListViewStates);
			this.GroupBoxStates.Location = new System.Drawing.Point (0, 208);
			this.GroupBoxStates.Margin = new System.Windows.Forms.Padding (0, 9, 0, 0);
			this.GroupBoxStates.MaximumSize = new System.Drawing.Size (730, 10000);
			this.GroupBoxStates.Name = "GroupBoxStates";
			this.GroupBoxStates.Padding = new System.Windows.Forms.Padding (6, 12, 6, 6);
			this.GroupBoxStates.Size = new System.Drawing.Size (632, 334);
			this.GroupBoxStates.TabIndex = 1;
			this.GroupBoxStates.TabStop = false;
			this.GroupBoxStates.Text = "Used by state(s)";
			// 
			// ListViewStates
			// 
			this.ListViewStates.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListViewStates.Columns.AddRange (new System.Windows.Forms.ColumnHeader[] {
            this.Column1});
			this.ListViewStates.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			listViewItem17.StateImageIndex = 0;
			listViewItem18.StateImageIndex = 0;
			listViewItem19.StateImageIndex = 0;
			listViewItem20.StateImageIndex = 0;
			listViewItem21.StateImageIndex = 0;
			listViewItem22.StateImageIndex = 0;
			listViewItem23.StateImageIndex = 0;
			listViewItem24.StateImageIndex = 0;
			listViewItem25.StateImageIndex = 0;
			listViewItem26.StateImageIndex = 0;
			listViewItem27.StateImageIndex = 0;
			listViewItem28.StateImageIndex = 0;
			listViewItem29.StateImageIndex = 0;
			listViewItem30.StateImageIndex = 0;
			listViewItem31.StateImageIndex = 0;
			listViewItem32.StateImageIndex = 0;
			this.ListViewStates.Items.AddRange (new System.Windows.Forms.ListViewItem[] {
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
            listViewItem30,
            listViewItem31,
            listViewItem32});
			this.ListViewStates.Location = new System.Drawing.Point (6, 27);
			this.ListViewStates.Margin = new System.Windows.Forms.Padding (0);
			this.ListViewStates.MultiSelect = false;
			this.ListViewStates.Name = "ListViewStates";
			this.ListViewStates.SelectedIndex = -1;
			this.ListViewStates.SelectedItem = null;
			this.ListViewStates.ShowGroups = false;
			this.ListViewStates.Size = new System.Drawing.Size (620, 301);
			this.ListViewStates.TabIndex = 0;
			this.ListViewStates.UseCompatibleStateImageBehavior = false;
			this.ListViewStates.View = System.Windows.Forms.View.Details;
			// 
			// Column1
			// 
			this.Column1.Text = "";
			this.Column1.Width = 160;
			// 
			// TextBoxName
			// 
			this.TextBoxName.AcceptsReturn = true;
			this.TextBoxName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TextBoxName.Location = new System.Drawing.Point (140, 3);
			this.TextBoxName.MaximumSize = new System.Drawing.Size (600, 50);
			this.TextBoxName.Name = "TextBoxName";
			this.TextBoxName.Size = new System.Drawing.Size (500, 22);
			this.TextBoxName.TabIndex = 1;
			this.TextBoxName.Validated += new System.EventHandler (this.TextBoxName_Validated);
			// 
			// PanelFramesTop
			// 
			this.PanelFramesTop.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PanelFramesTop.AutoSize = true;
			this.PanelFramesTop.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.PanelFramesTop.Controls.Add (this.LabelDurationDesc);
			this.PanelFramesTop.Controls.Add (this.NumericFrameDuration);
			this.PanelFramesTop.Controls.Add (this.LabelDuration);
			this.PanelFramesTop.Controls.Add (this.ToolStripFrames);
			this.PanelFramesTop.Location = new System.Drawing.Point (0, 0);
			this.PanelFramesTop.Margin = new System.Windows.Forms.Padding (0, 0, 0, 0);
			this.PanelFramesTop.Name = "PanelFramesTop";
			this.PanelFramesTop.Size = new System.Drawing.Size (620, 26);
			this.PanelFramesTop.TabIndex = 1;
			// 
			// LabelDurationDesc
			// 
			this.LabelDurationDesc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelDurationDesc.AutoSize = true;
			this.LabelDurationDesc.Location = new System.Drawing.Point (537, 2);
			this.LabelDurationDesc.Name = "LabelDurationDesc";
			this.LabelDurationDesc.Size = new System.Drawing.Size (80, 17);
			this.LabelDurationDesc.TabIndex = 3;
			this.LabelDurationDesc.Text = "(1/100 sec)";
			// 
			// NumericFrameDuration
			// 
			this.NumericFrameDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.NumericFrameDuration.DefaultBackColor = System.Drawing.SystemColors.Window;
			this.NumericFrameDuration.Location = new System.Drawing.Point (473, 0);
			this.NumericFrameDuration.Maximum = new decimal (new int[] {
            10000,
            0,
            0,
            0});
			this.NumericFrameDuration.Name = "NumericFrameDuration";
			this.NumericFrameDuration.Size = new System.Drawing.Size (58, 22);
			this.NumericFrameDuration.TabIndex = 2;
			this.NumericFrameDuration.Value = new decimal (new int[] {
            0,
            0,
            0,
            0});
			this.NumericFrameDuration.Validated += new System.EventHandler (this.NumericFrameDuration_Validated);
			// 
			// LabelDuration
			// 
			this.LabelDuration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.LabelDuration.AutoSize = true;
			this.LabelDuration.Location = new System.Drawing.Point (336, 2);
			this.LabelDuration.Name = "LabelDuration";
			this.LabelDuration.Size = new System.Drawing.Size (131, 17);
			this.LabelDuration.TabIndex = 1;
			this.LabelDuration.Text = "New frame duration";
			// 
			// AnimationForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add (this.TableLayoutMain);
			this.Controls.Add (this.ComboBoxReturn);
			this.Controls.Add (this.LabelReturn);
			this.Controls.Add (this.TextBoxName);
			this.Controls.Add (this.LabelName);
			this.Margin = new System.Windows.Forms.Padding (0);
			this.Name = "AnimationForm";
			this.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.Size = new System.Drawing.Size (650, 600);
			this.GroupBoxFrames.ResumeLayout (false);
			this.GroupBoxFrames.PerformLayout ();
			this.TableLayoutFrames.ResumeLayout (false);
			this.TableLayoutFrames.PerformLayout ();
			this.ToolStripFrames.ResumeLayout (false);
			this.ToolStripFrames.PerformLayout ();
			this.TableLayoutMain.ResumeLayout (false);
			this.TableLayoutMain.PerformLayout ();
			this.GroupBoxStates.ResumeLayout (false);
			this.PanelFramesTop.ResumeLayout (false);
			this.PanelFramesTop.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.NumericFrameDuration)).EndInit ();
			this.ResumeLayout (false);
			this.PerformLayout ();

		}

		#endregion

		private System.Windows.Forms.Label LabelName;
		private DoubleAgent.TextBoxEx TextBoxName;
		private System.Windows.Forms.Label LabelReturn;
		private System.Windows.Forms.ComboBox ComboBoxReturn;
		private System.Windows.Forms.GroupBox GroupBoxFrames;
		private DoubleAgent.ListViewEx ListViewPreview;
		private System.Windows.Forms.TableLayoutPanel TableLayoutMain;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.GroupBox GroupBoxStates;
		private DoubleAgent.ListViewEx ListViewStates;
		private System.Windows.Forms.ColumnHeader Column1;
		private System.Windows.Forms.TableLayoutPanel TableLayoutFrames;
		private DoubleAgent.ToolStripEx ToolStripFrames;
		private System.Windows.Forms.ToolStripButton ButtonAdd;
		private System.Windows.Forms.ToolStripButton ButtonRemove;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton ButtonMoveUp;
		private System.Windows.Forms.ToolStripButton ButtonMoveDown;
		private System.Windows.Forms.Panel PanelFramesTop;
		private System.Windows.Forms.Label LabelDurationDesc;
		private DoubleAgent.NumericUpDownEx NumericFrameDuration;
		private System.Windows.Forms.Label LabelDuration;
	}
}
