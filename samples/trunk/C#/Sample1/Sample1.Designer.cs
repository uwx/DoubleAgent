namespace Sample1
{
    partial class Sample1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Sample1));
			this.MsServerBox = new System.Windows.Forms.GroupBox();
			this.mUnloadMsServerChar = new System.Windows.Forms.Button();
			this.mHideMsServerChar = new System.Windows.Forms.Button();
			this.mShowMsServerChars = new System.Windows.Forms.Button();
			this.mShowMsServerOptions = new System.Windows.Forms.Button();
			this.mShowMsServerChar = new System.Windows.Forms.Button();
			this.mLoadMsServerChar = new System.Windows.Forms.Button();
			this.MsControlBox = new System.Windows.Forms.GroupBox();
			this.mUnloadMsControlChar = new System.Windows.Forms.Button();
			this.mHideMsControlChar = new System.Windows.Forms.Button();
			this.mShowMsControlChars = new System.Windows.Forms.Button();
			this.mShowMsControlOptions = new System.Windows.Forms.Button();
			this.mShowMsControlChar = new System.Windows.Forms.Button();
			this.mLoadMsControlChar = new System.Windows.Forms.Button();
			this.DaServerBox = new System.Windows.Forms.GroupBox();
			this.mUnloadDaServerChar = new System.Windows.Forms.Button();
			this.mHideDaServerChar = new System.Windows.Forms.Button();
			this.mShowDaServerChars = new System.Windows.Forms.Button();
			this.mShowDaServerOptions = new System.Windows.Forms.Button();
			this.mShowDaServerChar = new System.Windows.Forms.Button();
			this.mLoadDaServerChar = new System.Windows.Forms.Button();
			this.DaControlBox = new System.Windows.Forms.GroupBox();
			this.mUnloadDaControlChar = new System.Windows.Forms.Button();
			this.mHideDaControlChar = new System.Windows.Forms.Button();
			this.mShowDaControlChars = new System.Windows.Forms.Button();
			this.mShowDaControlOptions = new System.Windows.Forms.Button();
			this.mShowDaControlChar = new System.Windows.Forms.Button();
			this.mLoadDaControlChar = new System.Windows.Forms.Button();
			this.mDaControl = new AxDoubleAgentCtl.AxDaControl();
			this.mMsControl = new AxAgentObjects.AxAgent();
			this.MsServerBox.SuspendLayout();
			this.MsControlBox.SuspendLayout();
			this.DaServerBox.SuspendLayout();
			this.DaControlBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.mDaControl)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.mMsControl)).BeginInit();
			this.SuspendLayout();
			// 
			// MsServerBox
			// 
			this.MsServerBox.Controls.Add(this.mUnloadMsServerChar);
			this.MsServerBox.Controls.Add(this.mHideMsServerChar);
			this.MsServerBox.Controls.Add(this.mShowMsServerChars);
			this.MsServerBox.Controls.Add(this.mShowMsServerOptions);
			this.MsServerBox.Controls.Add(this.mShowMsServerChar);
			this.MsServerBox.Controls.Add(this.mLoadMsServerChar);
			this.MsServerBox.Location = new System.Drawing.Point(12, 12);
			this.MsServerBox.Name = "MsServerBox";
			this.MsServerBox.Size = new System.Drawing.Size(268, 146);
			this.MsServerBox.TabIndex = 0;
			this.MsServerBox.TabStop = false;
			this.MsServerBox.Text = "MS Agent Server";
			// 
			// mUnloadMsServerChar
			// 
			this.mUnloadMsServerChar.Location = new System.Drawing.Point(6, 82);
			this.mUnloadMsServerChar.Name = "mUnloadMsServerChar";
			this.mUnloadMsServerChar.Size = new System.Drawing.Size(76, 57);
			this.mUnloadMsServerChar.TabIndex = 0;
			this.mUnloadMsServerChar.Text = "Unload character";
			this.mUnloadMsServerChar.UseVisualStyleBackColor = true;
			this.mUnloadMsServerChar.Click += new System.EventHandler(this.UnloadMsServerChar_Click);
			// 
			// mHideMsServerChar
			// 
			this.mHideMsServerChar.Location = new System.Drawing.Point(88, 82);
			this.mHideMsServerChar.Name = "mHideMsServerChar";
			this.mHideMsServerChar.Size = new System.Drawing.Size(76, 57);
			this.mHideMsServerChar.TabIndex = 0;
			this.mHideMsServerChar.Text = "Hide character";
			this.mHideMsServerChar.UseVisualStyleBackColor = true;
			this.mHideMsServerChar.Click += new System.EventHandler(this.HideMsServerChar_Click);
			// 
			// mShowMsServerChars
			// 
			this.mShowMsServerChars.Location = new System.Drawing.Point(187, 82);
			this.mShowMsServerChars.Name = "mShowMsServerChars";
			this.mShowMsServerChars.Size = new System.Drawing.Size(76, 57);
			this.mShowMsServerChars.TabIndex = 0;
			this.mShowMsServerChars.Text = "Show characters";
			this.mShowMsServerChars.UseVisualStyleBackColor = true;
			this.mShowMsServerChars.Click += new System.EventHandler(this.ShowMsServerChars_Click);
			// 
			// mShowMsServerOptions
			// 
			this.mShowMsServerOptions.Location = new System.Drawing.Point(187, 19);
			this.mShowMsServerOptions.Name = "mShowMsServerOptions";
			this.mShowMsServerOptions.Size = new System.Drawing.Size(76, 57);
			this.mShowMsServerOptions.TabIndex = 0;
			this.mShowMsServerOptions.Text = "Show options";
			this.mShowMsServerOptions.UseVisualStyleBackColor = true;
			this.mShowMsServerOptions.Click += new System.EventHandler(this.ShowMsServerOptions_Click);
			// 
			// mShowMsServerChar
			// 
			this.mShowMsServerChar.Location = new System.Drawing.Point(88, 19);
			this.mShowMsServerChar.Name = "mShowMsServerChar";
			this.mShowMsServerChar.Size = new System.Drawing.Size(76, 57);
			this.mShowMsServerChar.TabIndex = 0;
			this.mShowMsServerChar.Text = "Show character";
			this.mShowMsServerChar.UseVisualStyleBackColor = true;
			this.mShowMsServerChar.Click += new System.EventHandler(this.ShowMsServerChar_Click);
			// 
			// mLoadMsServerChar
			// 
			this.mLoadMsServerChar.Location = new System.Drawing.Point(6, 19);
			this.mLoadMsServerChar.Name = "mLoadMsServerChar";
			this.mLoadMsServerChar.Size = new System.Drawing.Size(76, 57);
			this.mLoadMsServerChar.TabIndex = 0;
			this.mLoadMsServerChar.Text = "Load character";
			this.mLoadMsServerChar.UseVisualStyleBackColor = true;
			this.mLoadMsServerChar.Click += new System.EventHandler(this.LoadMsServerChar_Click);
			// 
			// MsControlBox
			// 
			this.MsControlBox.Controls.Add(this.mUnloadMsControlChar);
			this.MsControlBox.Controls.Add(this.mHideMsControlChar);
			this.MsControlBox.Controls.Add(this.mShowMsControlChars);
			this.MsControlBox.Controls.Add(this.mShowMsControlOptions);
			this.MsControlBox.Controls.Add(this.mShowMsControlChar);
			this.MsControlBox.Controls.Add(this.mLoadMsControlChar);
			this.MsControlBox.Location = new System.Drawing.Point(325, 12);
			this.MsControlBox.Name = "MsControlBox";
			this.MsControlBox.Size = new System.Drawing.Size(268, 146);
			this.MsControlBox.TabIndex = 0;
			this.MsControlBox.TabStop = false;
			this.MsControlBox.Text = "MS Agent Control";
			// 
			// mUnloadMsControlChar
			// 
			this.mUnloadMsControlChar.Location = new System.Drawing.Point(6, 82);
			this.mUnloadMsControlChar.Name = "mUnloadMsControlChar";
			this.mUnloadMsControlChar.Size = new System.Drawing.Size(76, 57);
			this.mUnloadMsControlChar.TabIndex = 0;
			this.mUnloadMsControlChar.Text = "Unload character";
			this.mUnloadMsControlChar.UseVisualStyleBackColor = true;
			this.mUnloadMsControlChar.Click += new System.EventHandler(this.UnloadMsControlChar_Click);
			// 
			// mHideMsControlChar
			// 
			this.mHideMsControlChar.Location = new System.Drawing.Point(88, 82);
			this.mHideMsControlChar.Name = "mHideMsControlChar";
			this.mHideMsControlChar.Size = new System.Drawing.Size(76, 57);
			this.mHideMsControlChar.TabIndex = 0;
			this.mHideMsControlChar.Text = "Hide character";
			this.mHideMsControlChar.UseVisualStyleBackColor = true;
			this.mHideMsControlChar.Click += new System.EventHandler(this.HideMsControlChar_Click);
			// 
			// mShowMsControlChars
			// 
			this.mShowMsControlChars.Location = new System.Drawing.Point(187, 82);
			this.mShowMsControlChars.Name = "mShowMsControlChars";
			this.mShowMsControlChars.Size = new System.Drawing.Size(76, 57);
			this.mShowMsControlChars.TabIndex = 0;
			this.mShowMsControlChars.Text = "Show characters";
			this.mShowMsControlChars.UseVisualStyleBackColor = true;
			this.mShowMsControlChars.Click += new System.EventHandler(this.ShowMsControlChars_Click);
			// 
			// mShowMsControlOptions
			// 
			this.mShowMsControlOptions.Location = new System.Drawing.Point(187, 19);
			this.mShowMsControlOptions.Name = "mShowMsControlOptions";
			this.mShowMsControlOptions.Size = new System.Drawing.Size(76, 57);
			this.mShowMsControlOptions.TabIndex = 0;
			this.mShowMsControlOptions.Text = "Show options";
			this.mShowMsControlOptions.UseVisualStyleBackColor = true;
			this.mShowMsControlOptions.Click += new System.EventHandler(this.ShowMsControlOptions_Click);
			// 
			// mShowMsControlChar
			// 
			this.mShowMsControlChar.Location = new System.Drawing.Point(88, 19);
			this.mShowMsControlChar.Name = "mShowMsControlChar";
			this.mShowMsControlChar.Size = new System.Drawing.Size(76, 57);
			this.mShowMsControlChar.TabIndex = 0;
			this.mShowMsControlChar.Text = "Show character";
			this.mShowMsControlChar.UseVisualStyleBackColor = true;
			this.mShowMsControlChar.Click += new System.EventHandler(this.ShowMsControlChar_Click);
			// 
			// mLoadMsControlChar
			// 
			this.mLoadMsControlChar.Location = new System.Drawing.Point(6, 19);
			this.mLoadMsControlChar.Name = "mLoadMsControlChar";
			this.mLoadMsControlChar.Size = new System.Drawing.Size(76, 57);
			this.mLoadMsControlChar.TabIndex = 0;
			this.mLoadMsControlChar.Text = "Load character";
			this.mLoadMsControlChar.UseVisualStyleBackColor = true;
			this.mLoadMsControlChar.Click += new System.EventHandler(this.LoadMsControlChar_Click);
			// 
			// DaServerBox
			// 
			this.DaServerBox.Controls.Add(this.mUnloadDaServerChar);
			this.DaServerBox.Controls.Add(this.mHideDaServerChar);
			this.DaServerBox.Controls.Add(this.mShowDaServerChars);
			this.DaServerBox.Controls.Add(this.mShowDaServerOptions);
			this.DaServerBox.Controls.Add(this.mShowDaServerChar);
			this.DaServerBox.Controls.Add(this.mLoadDaServerChar);
			this.DaServerBox.Location = new System.Drawing.Point(12, 181);
			this.DaServerBox.Name = "DaServerBox";
			this.DaServerBox.Size = new System.Drawing.Size(268, 146);
			this.DaServerBox.TabIndex = 0;
			this.DaServerBox.TabStop = false;
			this.DaServerBox.Text = "Double Agent Server";
			// 
			// mUnloadDaServerChar
			// 
			this.mUnloadDaServerChar.Location = new System.Drawing.Point(6, 82);
			this.mUnloadDaServerChar.Name = "mUnloadDaServerChar";
			this.mUnloadDaServerChar.Size = new System.Drawing.Size(76, 57);
			this.mUnloadDaServerChar.TabIndex = 0;
			this.mUnloadDaServerChar.Text = "Unload character";
			this.mUnloadDaServerChar.UseVisualStyleBackColor = true;
			this.mUnloadDaServerChar.Click += new System.EventHandler(this.UnloadDaServerChar_Click);
			// 
			// mHideDaServerChar
			// 
			this.mHideDaServerChar.Location = new System.Drawing.Point(88, 82);
			this.mHideDaServerChar.Name = "mHideDaServerChar";
			this.mHideDaServerChar.Size = new System.Drawing.Size(76, 57);
			this.mHideDaServerChar.TabIndex = 0;
			this.mHideDaServerChar.Text = "Hide character";
			this.mHideDaServerChar.UseVisualStyleBackColor = true;
			this.mHideDaServerChar.Click += new System.EventHandler(this.HideDaServerChar_Click);
			// 
			// mShowDaServerChars
			// 
			this.mShowDaServerChars.Location = new System.Drawing.Point(187, 82);
			this.mShowDaServerChars.Name = "mShowDaServerChars";
			this.mShowDaServerChars.Size = new System.Drawing.Size(76, 57);
			this.mShowDaServerChars.TabIndex = 0;
			this.mShowDaServerChars.Text = "Show characters";
			this.mShowDaServerChars.UseVisualStyleBackColor = true;
			this.mShowDaServerChars.Click += new System.EventHandler(this.ShowDaServerChars_Click);
			// 
			// mShowDaServerOptions
			// 
			this.mShowDaServerOptions.Location = new System.Drawing.Point(187, 19);
			this.mShowDaServerOptions.Name = "mShowDaServerOptions";
			this.mShowDaServerOptions.Size = new System.Drawing.Size(76, 57);
			this.mShowDaServerOptions.TabIndex = 0;
			this.mShowDaServerOptions.Text = "Show options";
			this.mShowDaServerOptions.UseVisualStyleBackColor = true;
			this.mShowDaServerOptions.Click += new System.EventHandler(this.ShowDaServerOptions_Click);
			// 
			// mShowDaServerChar
			// 
			this.mShowDaServerChar.Location = new System.Drawing.Point(88, 19);
			this.mShowDaServerChar.Name = "mShowDaServerChar";
			this.mShowDaServerChar.Size = new System.Drawing.Size(76, 57);
			this.mShowDaServerChar.TabIndex = 0;
			this.mShowDaServerChar.Text = "Show character";
			this.mShowDaServerChar.UseVisualStyleBackColor = true;
			this.mShowDaServerChar.Click += new System.EventHandler(this.ShowDaServerChar_Click);
			// 
			// mLoadDaServerChar
			// 
			this.mLoadDaServerChar.Location = new System.Drawing.Point(6, 19);
			this.mLoadDaServerChar.Name = "mLoadDaServerChar";
			this.mLoadDaServerChar.Size = new System.Drawing.Size(76, 57);
			this.mLoadDaServerChar.TabIndex = 0;
			this.mLoadDaServerChar.Text = "Load character";
			this.mLoadDaServerChar.UseVisualStyleBackColor = true;
			this.mLoadDaServerChar.Click += new System.EventHandler(this.LoadDaServerChar_Click);
			// 
			// DaControlBox
			// 
			this.DaControlBox.Controls.Add(this.mUnloadDaControlChar);
			this.DaControlBox.Controls.Add(this.mHideDaControlChar);
			this.DaControlBox.Controls.Add(this.mShowDaControlChars);
			this.DaControlBox.Controls.Add(this.mShowDaControlOptions);
			this.DaControlBox.Controls.Add(this.mShowDaControlChar);
			this.DaControlBox.Controls.Add(this.mLoadDaControlChar);
			this.DaControlBox.Location = new System.Drawing.Point(325, 181);
			this.DaControlBox.Name = "DaControlBox";
			this.DaControlBox.Size = new System.Drawing.Size(268, 146);
			this.DaControlBox.TabIndex = 0;
			this.DaControlBox.TabStop = false;
			this.DaControlBox.Text = "Double Agent Control";
			// 
			// mUnloadDaControlChar
			// 
			this.mUnloadDaControlChar.Location = new System.Drawing.Point(6, 82);
			this.mUnloadDaControlChar.Name = "mUnloadDaControlChar";
			this.mUnloadDaControlChar.Size = new System.Drawing.Size(76, 57);
			this.mUnloadDaControlChar.TabIndex = 0;
			this.mUnloadDaControlChar.Text = "Unload character";
			this.mUnloadDaControlChar.UseVisualStyleBackColor = true;
			this.mUnloadDaControlChar.Click += new System.EventHandler(this.UnloadDaControlChar_Click);
			// 
			// mHideDaControlChar
			// 
			this.mHideDaControlChar.Location = new System.Drawing.Point(88, 82);
			this.mHideDaControlChar.Name = "mHideDaControlChar";
			this.mHideDaControlChar.Size = new System.Drawing.Size(76, 57);
			this.mHideDaControlChar.TabIndex = 0;
			this.mHideDaControlChar.Text = "Hide character";
			this.mHideDaControlChar.UseVisualStyleBackColor = true;
			this.mHideDaControlChar.Click += new System.EventHandler(this.HideDaControlChar_Click);
			// 
			// mShowDaControlChars
			// 
			this.mShowDaControlChars.Location = new System.Drawing.Point(187, 82);
			this.mShowDaControlChars.Name = "mShowDaControlChars";
			this.mShowDaControlChars.Size = new System.Drawing.Size(76, 57);
			this.mShowDaControlChars.TabIndex = 0;
			this.mShowDaControlChars.Text = "Show characters";
			this.mShowDaControlChars.UseVisualStyleBackColor = true;
			this.mShowDaControlChars.Click += new System.EventHandler(this.ShowDaControlChars_Click);
			// 
			// mShowDaControlOptions
			// 
			this.mShowDaControlOptions.Location = new System.Drawing.Point(187, 19);
			this.mShowDaControlOptions.Name = "mShowDaControlOptions";
			this.mShowDaControlOptions.Size = new System.Drawing.Size(76, 57);
			this.mShowDaControlOptions.TabIndex = 0;
			this.mShowDaControlOptions.Text = "Show options";
			this.mShowDaControlOptions.UseVisualStyleBackColor = true;
			this.mShowDaControlOptions.Click += new System.EventHandler(this.ShowDaControlOptions_Click);
			// 
			// mShowDaControlChar
			// 
			this.mShowDaControlChar.Location = new System.Drawing.Point(88, 19);
			this.mShowDaControlChar.Name = "mShowDaControlChar";
			this.mShowDaControlChar.Size = new System.Drawing.Size(76, 57);
			this.mShowDaControlChar.TabIndex = 0;
			this.mShowDaControlChar.Text = "Show character";
			this.mShowDaControlChar.UseVisualStyleBackColor = true;
			this.mShowDaControlChar.Click += new System.EventHandler(this.ShowDaControlChar_Click);
			// 
			// mLoadDaControlChar
			// 
			this.mLoadDaControlChar.Location = new System.Drawing.Point(6, 19);
			this.mLoadDaControlChar.Name = "mLoadDaControlChar";
			this.mLoadDaControlChar.Size = new System.Drawing.Size(76, 57);
			this.mLoadDaControlChar.TabIndex = 0;
			this.mLoadDaControlChar.Text = "Load character";
			this.mLoadDaControlChar.UseVisualStyleBackColor = true;
			this.mLoadDaControlChar.Click += new System.EventHandler(this.LoadDaControlChar_Click);
			// 
			// mDaControl
			// 
			this.mDaControl.Enabled = true;
			this.mDaControl.Location = new System.Drawing.Point(287, 31);
			this.mDaControl.Name = "mDaControl";
			this.mDaControl.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("mDaControl.OcxState")));
			this.mDaControl.Size = new System.Drawing.Size(32, 32);
			this.mDaControl.TabIndex = 1;
			this.mDaControl.HideEvent += new AxDoubleAgentCtl._DaCtlEvents_HideEventHandler(this.DaControl_HideEvent);
			this.mDaControl.ShowEvent += new AxDoubleAgentCtl._DaCtlEvents_ShowEventHandler(this.DaControl_ShowEvent);
			// 
			// mMsControl
			// 
			this.mMsControl.Enabled = true;
			this.mMsControl.Location = new System.Drawing.Point(288, 205);
			this.mMsControl.Name = "mMsControl";
			this.mMsControl.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("mMsControl.OcxState")));
			this.mMsControl.Size = new System.Drawing.Size(32, 32);
			this.mMsControl.TabIndex = 2;
			this.mMsControl.HideEvent += new AxAgentObjects._AgentEvents_HideEventHandler(this.MsControl_HideEvent);
			this.mMsControl.ShowEvent += new AxAgentObjects._AgentEvents_ShowEventHandler(this.MsControl_ShowEvent);
			// 
			// Sample1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(605, 337);
			this.Controls.Add(this.mMsControl);
			this.Controls.Add(this.mDaControl);
			this.Controls.Add(this.DaControlBox);
			this.Controls.Add(this.MsControlBox);
			this.Controls.Add(this.DaServerBox);
			this.Controls.Add(this.MsServerBox);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Sample1";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Sample1";
			this.MsServerBox.ResumeLayout(false);
			this.MsControlBox.ResumeLayout(false);
			this.DaServerBox.ResumeLayout(false);
			this.DaControlBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.mDaControl)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.mMsControl)).EndInit();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox MsServerBox;
        private System.Windows.Forms.Button mUnloadMsServerChar;
        private System.Windows.Forms.Button mLoadMsServerChar;
        private System.Windows.Forms.Button mHideMsServerChar;
        private System.Windows.Forms.Button mShowMsServerChar;
        private System.Windows.Forms.Button mShowMsServerChars;
        private System.Windows.Forms.Button mShowMsServerOptions;
        private System.Windows.Forms.GroupBox MsControlBox;
        private System.Windows.Forms.Button mUnloadMsControlChar;
        private System.Windows.Forms.Button mHideMsControlChar;
        private System.Windows.Forms.Button mShowMsControlChars;
        private System.Windows.Forms.Button mShowMsControlOptions;
        private System.Windows.Forms.Button mShowMsControlChar;
        private System.Windows.Forms.Button mLoadMsControlChar;
        private System.Windows.Forms.GroupBox DaServerBox;
        private System.Windows.Forms.Button mUnloadDaServerChar;
        private System.Windows.Forms.Button mHideDaServerChar;
        private System.Windows.Forms.Button mShowDaServerChars;
        private System.Windows.Forms.Button mShowDaServerOptions;
        private System.Windows.Forms.Button mShowDaServerChar;
        private System.Windows.Forms.Button mLoadDaServerChar;
        private System.Windows.Forms.GroupBox DaControlBox;
        private System.Windows.Forms.Button mUnloadDaControlChar;
        private System.Windows.Forms.Button mHideDaControlChar;
        private System.Windows.Forms.Button mShowDaControlChars;
        private System.Windows.Forms.Button mShowDaControlOptions;
        private System.Windows.Forms.Button mShowDaControlChar;
        private System.Windows.Forms.Button mLoadDaControlChar;
		private AxDoubleAgentCtl.AxDaControl mDaControl;
		private AxAgentObjects.AxAgent mMsControl;
    }
}

