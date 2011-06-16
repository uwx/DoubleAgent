namespace AgentCharacterEditor.Previews
{
	partial class AnimationPreviewHost
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
			this.WPFHost = new System.Windows.Forms.Integration.ElementHost();
			this.WPFTarget = new AgentCharacterEditor.Previews.AnimationPreview();
			this.SuspendLayout();
			// 
			// WPFHost
			// 
			this.WPFHost.CausesValidation = false;
			this.WPFHost.Location = new System.Drawing.Point(0, 0);
			this.WPFHost.Margin = new System.Windows.Forms.Padding(0);
			this.WPFHost.MaximumSize = new System.Drawing.Size(256, 256);
			this.WPFHost.MinimumSize = new System.Drawing.Size(16, 16);
			this.WPFHost.Name = "WPFHost";
			this.WPFHost.Size = new System.Drawing.Size(128, 128);
			this.WPFHost.BackColor = System.Drawing.SystemColors.Window;
			this.WPFHost.TabIndex = 0;
			this.WPFHost.TabStop = false;
			this.WPFHost.Child = this.WPFTarget;
			// 
			// WPFTarget
			// 
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.SystemColors.Window;
			this.CausesValidation = false;
			this.Controls.Add(this.WPFHost);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "AnimationPreviewHost";
			this.Size = new System.Drawing.Size(128, 128);
			this.ResumeLayout(false);

		}

		public System.Windows.Forms.Integration.ElementHost WPFHost;
		public AgentCharacterEditor.Previews.AnimationPreview WPFTarget;

		#endregion
	}
}
