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
			this.ButtonMovePrev = new DoubleAgent.ToolStripButtonCompat();
			this.ButtonMoveNext = new DoubleAgent.ToolStripButtonCompat();
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
			// ButtonMovePrev
			// 
			this.ButtonMovePrev.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMovePrev.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveLeft;
			this.ButtonMovePrev.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMovePrev.Name = "ButtonMovePrev";
			this.ButtonMovePrev.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMovePrev.Size = new System.Drawing.Size(24, 24);
			this.ButtonMovePrev.Text = "Move frame up";
			// 
			// ButtonMoveNext
			// 
			this.ButtonMoveNext.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ButtonMoveNext.Image = global::AgentCharacterEditor.Properties.Resources.ImgMoveRight;
			this.ButtonMoveNext.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ButtonMoveNext.Name = "ButtonMoveNext";
			this.ButtonMoveNext.Padding = new System.Windows.Forms.Padding(2);
			this.ButtonMoveNext.Size = new System.Drawing.Size(24, 24);
			this.ButtonMoveNext.Text = "Move frame down";
			// 
			// FramesToolBar
			// 
			this.BackColor = System.Drawing.Color.Transparent;
			this.Dock = System.Windows.Forms.DockStyle.None;
			this.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonAdd,
            this.ButtonDelete,
            toolStripSeparator1,
            this.ButtonMovePrev,
            this.ButtonMoveNext});
			this.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.Name = "ToolBarFrames";
			this.Size = new System.Drawing.Size(237, 27);
			this.ResumeLayout(false);

		}

		#endregion
		public DoubleAgent.ToolStripButtonCompat ButtonAdd;
		public DoubleAgent.ToolStripButtonCompat ButtonDelete;
		public DoubleAgent.ToolStripButtonCompat ButtonMovePrev;
		public DoubleAgent.ToolStripButtonCompat ButtonMoveNext;
	}
}
