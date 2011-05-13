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
			System.Windows.Forms.ColumnHeader columnHeader1;
			this.Frames = new AgentCharacterEditor.Previews.FramesListView();
			columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.SuspendLayout();
			// 
			// Frames
			// 
			this.Frames.Alignment = System.Windows.Forms.ListViewAlignment.Left;
			this.Frames.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.Frames.CharacterFile = null;
			this.Frames.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            columnHeader1});
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
			this.MinimumSize = new System.Drawing.Size (64, 64);
			this.Name = "FramesPreview";
			this.Size = new System.Drawing.Size(398, 198);
			this.VisibleChanged += new System.EventHandler(this.FramesPreview_VisibleChanged);
			this.ResumeLayout(false);

		}

		#endregion

		public FramesListView Frames;
	}
}
