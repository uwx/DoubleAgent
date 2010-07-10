namespace CtlAndSvr
{
    partial class MainForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent ()
        {
			this.components = new System.ComponentModel.Container ();
			System.Windows.Forms.Label label1;
			System.Windows.Forms.Label label2;
			System.Windows.Forms.Label label3;
			System.Windows.Forms.GroupBox groupBox1;
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (MainForm));
			this.SpeechText = new System.Windows.Forms.TextBox ();
			this.ListenCheck = new System.Windows.Forms.CheckBox ();
			this.HideButton = new System.Windows.Forms.Button ();
			this.SpeakButton = new System.Windows.Forms.Button ();
			this.ShowButton = new System.Windows.Forms.Button ();
			this.TestDaControl = new DoubleAgent.AxControl.AxControl ();
			this.CharacterFiles = new System.Windows.Forms.ComboBox ();
			this.TtsEngines = new System.Windows.Forms.ComboBox ();
			this.AxControlBinding = new System.Windows.Forms.BindingSource (this.components);
			this.ConnectedCheck = new System.Windows.Forms.CheckBox ();
			this.SrEngines = new System.Windows.Forms.ComboBox ();
			this.ContainedCheck = new System.Windows.Forms.CheckBox ();
			label1 = new System.Windows.Forms.Label ();
			label2 = new System.Windows.Forms.Label ();
			label3 = new System.Windows.Forms.Label ();
			groupBox1 = new System.Windows.Forms.GroupBox ();
			groupBox1.SuspendLayout ();
			((System.ComponentModel.ISupportInitialize)(this.TestDaControl)).BeginInit ();
			((System.ComponentModel.ISupportInitialize)(this.AxControlBinding)).BeginInit ();
			this.SuspendLayout ();
			// 
			// label1
			// 
			label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point (237, 12);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size (96, 17);
			label1.TabIndex = 1;
			label1.Text = "Character File";
			// 
			// label2
			// 
			label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			label2.AutoSize = true;
			label2.Location = new System.Drawing.Point (237, 70);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size (152, 17);
			label2.TabIndex = 3;
			label2.Text = "Test-to-speech Engine";
			// 
			// label3
			// 
			label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			label3.AutoSize = true;
			label3.Location = new System.Drawing.Point (237, 128);
			label3.Name = "label3";
			label3.Size = new System.Drawing.Size (183, 17);
			label3.TabIndex = 5;
			label3.Text = "Speech Recognition Engine";
			// 
			// groupBox1
			// 
			groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			groupBox1.Controls.Add (this.SpeechText);
			groupBox1.Controls.Add (this.ListenCheck);
			groupBox1.Controls.Add (this.HideButton);
			groupBox1.Controls.Add (this.SpeakButton);
			groupBox1.Controls.Add (this.ShowButton);
			groupBox1.Location = new System.Drawing.Point (12, 232);
			groupBox1.Name = "groupBox1";
			groupBox1.Size = new System.Drawing.Size (652, 127);
			groupBox1.TabIndex = 10;
			groupBox1.TabStop = false;
			groupBox1.Text = "Actions";
			// 
			// SpeechText
			// 
			this.SpeechText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.SpeechText.DataBindings.Add (new System.Windows.Forms.Binding ("Text", global::CtlAndSvr.Properties.Settings.Default, "SpeechText", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.SpeechText.Location = new System.Drawing.Point (288, 27);
			this.SpeechText.Margin = new System.Windows.Forms.Padding (9);
			this.SpeechText.Multiline = true;
			this.SpeechText.Name = "SpeechText";
			this.SpeechText.Size = new System.Drawing.Size (352, 83);
			this.SpeechText.TabIndex = 4;
			this.SpeechText.Text = global::CtlAndSvr.Properties.Settings.Default.SpeechText;
			// 
			// ListenCheck
			// 
			this.ListenCheck.Appearance = System.Windows.Forms.Appearance.Button;
			this.ListenCheck.Location = new System.Drawing.Point (150, 78);
			this.ListenCheck.Margin = new System.Windows.Forms.Padding (9);
			this.ListenCheck.Name = "ListenCheck";
			this.ListenCheck.Size = new System.Drawing.Size (120, 32);
			this.ListenCheck.TabIndex = 3;
			this.ListenCheck.Text = "Listen";
			this.ListenCheck.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.ListenCheck.UseVisualStyleBackColor = true;
			this.ListenCheck.CheckedChanged += new System.EventHandler (this.ListenCheck_CheckedChanged);
			// 
			// HideButton
			// 
			this.HideButton.Location = new System.Drawing.Point (12, 78);
			this.HideButton.Margin = new System.Windows.Forms.Padding (9);
			this.HideButton.Name = "HideButton";
			this.HideButton.Size = new System.Drawing.Size (120, 32);
			this.HideButton.TabIndex = 1;
			this.HideButton.Text = "Hide";
			this.HideButton.UseVisualStyleBackColor = true;
			this.HideButton.Click += new System.EventHandler (this.HideButton_Click);
			// 
			// SpeakButton
			// 
			this.SpeakButton.Location = new System.Drawing.Point (150, 27);
			this.SpeakButton.Margin = new System.Windows.Forms.Padding (9);
			this.SpeakButton.Name = "SpeakButton";
			this.SpeakButton.Size = new System.Drawing.Size (120, 32);
			this.SpeakButton.TabIndex = 2;
			this.SpeakButton.Text = "Speak";
			this.SpeakButton.UseVisualStyleBackColor = true;
			this.SpeakButton.Click += new System.EventHandler (this.SpeakButton_Click);
			// 
			// ShowButton
			// 
			this.ShowButton.Location = new System.Drawing.Point (12, 27);
			this.ShowButton.Margin = new System.Windows.Forms.Padding (9);
			this.ShowButton.Name = "ShowButton";
			this.ShowButton.Size = new System.Drawing.Size (120, 33);
			this.ShowButton.TabIndex = 0;
			this.ShowButton.Text = "Show";
			this.ShowButton.UseVisualStyleBackColor = true;
			this.ShowButton.Click += new System.EventHandler (this.ShowButton_Click);
			// 
			// TestDaControl
			// 
			this.TestDaControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TestDaControl.Enabled = true;
			this.TestDaControl.Location = new System.Drawing.Point (12, 12);
			this.TestDaControl.Name = "TestDaControl";
			this.TestDaControl.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject ("TestDaControl.OcxState")));
			this.TestDaControl.Size = new System.Drawing.Size (200, 200);
			this.TestDaControl.TabIndex = 0;
			this.TestDaControl.DaShow += new DoubleAgent.AxControl.DaShowEventHandler (this.TestDaControl_DaShow);
			this.TestDaControl.DaHide += new DoubleAgent.AxControl.DaHideEventHandler (this.TestDaControl_DaHide);
			this.TestDaControl.DaListenStart += new DoubleAgent.AxControl.DaListenStartEventHandler (this.TestDaControl_DaListenStart);
			this.TestDaControl.DaListenComplete += new DoubleAgent.AxControl.DaListenCompleteEventHandler (this.TestDaControl_DaListenComplete);
			// 
			// CharacterFiles
			// 
			this.CharacterFiles.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.CharacterFiles.CausesValidation = false;
			this.CharacterFiles.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.CharacterFiles.FormattingEnabled = true;
			this.CharacterFiles.Location = new System.Drawing.Point (240, 32);
			this.CharacterFiles.Name = "CharacterFiles";
			this.CharacterFiles.Size = new System.Drawing.Size (426, 24);
			this.CharacterFiles.TabIndex = 2;
			this.CharacterFiles.SelectionChangeCommitted += new System.EventHandler (this.CharacterFiles_SelectionChangeCommitted);
			// 
			// TtsEngines
			// 
			this.TtsEngines.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TtsEngines.DisplayMember = "DisplayName";
			this.TtsEngines.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.TtsEngines.Location = new System.Drawing.Point (240, 90);
			this.TtsEngines.Name = "TtsEngines";
			this.TtsEngines.Size = new System.Drawing.Size (426, 24);
			this.TtsEngines.TabIndex = 4;
			this.TtsEngines.ValueMember = "TTSModeID";
			this.TtsEngines.SelectionChangeCommitted += new System.EventHandler (this.TtsEngines_SelectionChangeCommitted);
			// 
			// AxControlBinding
			// 
			this.AxControlBinding.AllowNew = false;
			this.AxControlBinding.DataSource = this.TestDaControl;
			this.AxControlBinding.Position = 0;
			// 
			// ConnectedCheck
			// 
			this.ConnectedCheck.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ConnectedCheck.AutoSize = true;
			this.ConnectedCheck.DataBindings.Add (new System.Windows.Forms.Binding ("Checked", this.AxControlBinding, "Connected", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.ConnectedCheck.Location = new System.Drawing.Point (240, 191);
			this.ConnectedCheck.Name = "ConnectedCheck";
			this.ConnectedCheck.Size = new System.Drawing.Size (98, 21);
			this.ConnectedCheck.TabIndex = 7;
			this.ConnectedCheck.Text = "Connected";
			this.ConnectedCheck.UseVisualStyleBackColor = true;
			this.ConnectedCheck.CheckedChanged += new System.EventHandler (this.ConnectedCheck_CheckedChanged);
			// 
			// SrEngines
			// 
			this.SrEngines.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.SrEngines.DisplayMember = "DisplayName";
			this.SrEngines.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.SrEngines.Location = new System.Drawing.Point (240, 148);
			this.SrEngines.Name = "SrEngines";
			this.SrEngines.Size = new System.Drawing.Size (426, 24);
			this.SrEngines.TabIndex = 6;
			this.SrEngines.ValueMember = "SRModeID";
			this.SrEngines.SelectionChangeCommitted += new System.EventHandler (this.SrEngines_SelectionChangeCommitted);
			// 
			// ContainedCheck
			// 
			this.ContainedCheck.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ContainedCheck.AutoSize = true;
			this.ContainedCheck.Location = new System.Drawing.Point (344, 191);
			this.ContainedCheck.Name = "ContainedCheck";
			this.ContainedCheck.Size = new System.Drawing.Size (94, 21);
			this.ContainedCheck.TabIndex = 8;
			this.ContainedCheck.Text = "Contained";
			this.ContainedCheck.UseVisualStyleBackColor = true;
			this.ContainedCheck.CheckedChanged += new System.EventHandler (this.ContainedCheck_CheckedChanged);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size (682, 371);
			this.Controls.Add (groupBox1);
			this.Controls.Add (this.ContainedCheck);
			this.Controls.Add (this.ConnectedCheck);
			this.Controls.Add (label3);
			this.Controls.Add (label2);
			this.Controls.Add (label1);
			this.Controls.Add (this.SrEngines);
			this.Controls.Add (this.TtsEngines);
			this.Controls.Add (this.CharacterFiles);
			this.Controls.Add (this.TestDaControl);
			this.DataBindings.Add (new System.Windows.Forms.Binding ("Location", global::CtlAndSvr.Properties.Settings.Default, "Location", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
			this.Location = global::CtlAndSvr.Properties.Settings.Default.Location;
			this.Name = "MainForm";
			this.Text = "Double Agent Control";
			groupBox1.ResumeLayout (false);
			groupBox1.PerformLayout ();
			((System.ComponentModel.ISupportInitialize)(this.TestDaControl)).EndInit ();
			((System.ComponentModel.ISupportInitialize)(this.AxControlBinding)).EndInit ();
			this.ResumeLayout (false);
			this.PerformLayout ();

        }

        #endregion

        private DoubleAgent.AxControl.AxControl TestDaControl;
		private System.Windows.Forms.ComboBox CharacterFiles;
		private System.Windows.Forms.ComboBox TtsEngines;
		private System.Windows.Forms.CheckBox ConnectedCheck;
		private System.Windows.Forms.BindingSource AxControlBinding;
		private System.Windows.Forms.ComboBox SrEngines;
		private System.Windows.Forms.CheckBox ContainedCheck;
		private System.Windows.Forms.Button HideButton;
		private System.Windows.Forms.Button SpeakButton;
		private System.Windows.Forms.Button ShowButton;
		private System.Windows.Forms.CheckBox ListenCheck;
		private System.Windows.Forms.TextBox SpeechText;
    }
}

