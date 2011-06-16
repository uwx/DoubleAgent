namespace AgentCharacterEditor.Previews
{
	partial class FramesPreview
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
			System.Windows.Forms.ColumnHeader columnHeader1;
			System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
			this.Frames = new AgentCharacterEditor.Previews.FramesListView();
			this.ContextMenuFrames = new AgentCharacterEditor.Global.ContextMenuEdit(this.components);
			this.MenuItemAdd = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemMoveUp = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemMoveDown = new System.Windows.Forms.ToolStripMenuItem();
			columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.ContextMenuFrames.SuspendLayout();
			this.SuspendLayout();
			// 
			// Frames
			// 
			this.Frames.Alignment = System.Windows.Forms.ListViewAlignment.Left;
			this.Frames.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Frames.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            columnHeader1});
			this.Frames.ContextMenuStrip = this.ContextMenuFrames;
			this.Frames.HideSelection = false;
			this.Frames.LabelWrap = false;
			this.Frames.Location = new System.Drawing.Point(0, 0);
			this.Frames.Margin = new System.Windows.Forms.Padding(0);
			this.Frames.MultiSelect = false;
			this.Frames.Name = "Frames";
			this.Frames.OwnerDraw = true;
			this.Frames.Scrollable = false;
			this.Frames.ShowGroups = false;
			this.Frames.Size = new System.Drawing.Size(398, 198);
			this.Frames.TabIndex = 0;
			this.Frames.UseCompatibleStateImageBehavior = false;
			// 
			// ContextMenuFrames
			// 
			this.ContextMenuFrames.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemAdd,
            this.MenuItemMoveUp,
            this.MenuItemMoveDown,
            toolStripSeparator1});
			this.ContextMenuFrames.Name = "ContextMenuFrames";
			this.ContextMenuFrames.Size = new System.Drawing.Size(219, 116);
			// 
			// MenuItemAdd
			// 
			this.MenuItemAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.MenuItemAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemAdd.Name = "MenuItemAdd";
			this.MenuItemAdd.Size = new System.Drawing.Size(218, 28);
			this.MenuItemAdd.Text = "Add a new Frame";
			// 
			// MenuItemMoveUp
			// 
			this.MenuItemMoveUp.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveLeft;
			this.MenuItemMoveUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemMoveUp.Name = "MenuItemMoveUp";
			this.MenuItemMoveUp.Size = new System.Drawing.Size(218, 28);
			this.MenuItemMoveUp.Text = "Move frame up";
			// 
			// MenuItemMoveDown
			// 
			this.MenuItemMoveDown.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveRight;
			this.MenuItemMoveDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemMoveDown.Name = "MenuItemMoveDown";
			this.MenuItemMoveDown.Size = new System.Drawing.Size(218, 28);
			this.MenuItemMoveDown.Text = "Move frame down";
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size(215, 6);
			// 
			// FramesPreview
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoScroll = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.SystemColors.Window;
			this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.Controls.Add(this.Frames);
			this.DoubleBuffered = true;
			this.Margin = new System.Windows.Forms.Padding(0);
			this.MinimumSize = new System.Drawing.Size(64, 64);
			this.Name = "FramesPreview";
			this.Size = new System.Drawing.Size(398, 198);
			this.VisibleChanged += new System.EventHandler(this.FramesPreview_VisibleChanged);
			this.ContextMenuFrames.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		public FramesListView Frames;
		public Global.ContextMenuEdit ContextMenuFrames;
		public System.Windows.Forms.ToolStripMenuItem MenuItemAdd;
		public System.Windows.Forms.ToolStripMenuItem MenuItemMoveUp;
		public System.Windows.Forms.ToolStripMenuItem MenuItemMoveDown;
	}
}
