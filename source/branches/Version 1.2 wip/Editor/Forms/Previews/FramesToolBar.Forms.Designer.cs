namespace AgentCharacterEditor.Previews
{
	partial class FramesToolBar
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
			this.ButtonAdd = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonDelete = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMoveUp = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMoveDown = new DoubleAgent.ToolStripButtonCompat();
			toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.SuspendLayout();
			// 
			// toolStripSeparator1
			// 
			toolStripSeparator1.Name = "toolStripSeparator1";
			toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// ButtonAdd
			// 
			this.ButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonAdd.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.ButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonAdd.Name = "ButtonAdd";
			this.ButtonAdd.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonAdd.Size = new System.Drawing.Size(24, 24);
			this.ButtonAdd.Text = "Add a new Frame";
			// 
			// ButtonDelete
			// 
			this.ButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.ButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonDelete.Name = "ButtonDelete";
			this.ButtonDelete.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonDelete.Size = new System.Drawing.Size(24, 24);
			this.ButtonDelete.Text = "Delete frame";
			// 
			// ButtonMoveUp
			// 
			this.ButtonMoveUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveUp.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveLeft;
			this.ButtonMoveUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveUp.Name = "ButtonMoveUp";
			this.ButtonMoveUp.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMoveUp.Size = new System.Drawing.Size(24, 24);
			this.ButtonMoveUp.Text = "Move frame up";
			// 
			// ButtonMoveDown
			// 
			this.ButtonMoveDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveDown.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveRight;
			this.ButtonMoveDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveDown.Name = "ButtonMoveDown";
			this.ButtonMoveDown.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMoveDown.Size = new System.Drawing.Size(24, 24);
			this.ButtonMoveDown.Text = "Move frame down";
			// 
			// FramesToolBar
			// 
			this.BackColor = System.Drawing.Color.Transparent;
			this.Dock = System.Windows.Forms.DockStyle.None;
			this.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonDelete,
            toolStripSeparator1,
            this.ButtonMoveUp,
            this.ButtonMoveDown});
			this.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.Name = "ToolBarFrames";
			this.Size = new System.Drawing.Size(237, 27);
			this.ResumeLayout(false);

		}

		#endregion
		public DoubleAgent.ToolStripButtonCompat ButtonAdd;
		public DoubleAgent.ToolStripButtonCompat ButtonDelete;
		public DoubleAgent.ToolStripButtonCompat ButtonMoveUp;
		public DoubleAgent.ToolStripButtonCompat ButtonMoveDown;
	}
}
