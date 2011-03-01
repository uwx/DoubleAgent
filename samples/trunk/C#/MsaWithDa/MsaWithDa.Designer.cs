namespace MsaWithDa
{
    partial class MsaWithDa
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (MsaWithDa));
            this.MsServerBox = new System.Windows.Forms.GroupBox ();
            this.mUnloadMsServerChar = new System.Windows.Forms.Button ();
            this.mHideMsServerChar = new System.Windows.Forms.Button ();
            this.mShowMsServerChars = new System.Windows.Forms.Button ();
            this.mShowMsServerOptions = new System.Windows.Forms.Button ();
            this.mShowMsServerChar = new System.Windows.Forms.Button ();
            this.mLoadMsServerChar = new System.Windows.Forms.Button ();
            this.MsControlBox = new System.Windows.Forms.GroupBox ();
            this.mUnloadMsControlChar = new System.Windows.Forms.Button ();
            this.mHideMsControlChar = new System.Windows.Forms.Button ();
            this.mShowMsControlChars = new System.Windows.Forms.Button ();
            this.mShowMsControlOptions = new System.Windows.Forms.Button ();
            this.mShowMsControlChar = new System.Windows.Forms.Button ();
            this.mLoadMsControlChar = new System.Windows.Forms.Button ();
            this.DaServerBox = new System.Windows.Forms.GroupBox ();
            this.mUnloadDaServerChar = new System.Windows.Forms.Button ();
            this.mHideDaServerChar = new System.Windows.Forms.Button ();
            this.mShowDaServerChars = new System.Windows.Forms.Button ();
            this.mShowDaServerOptions = new System.Windows.Forms.Button ();
            this.mShowDaServerChar = new System.Windows.Forms.Button ();
            this.mLoadDaServerChar = new System.Windows.Forms.Button ();
            this.DaControlBox = new System.Windows.Forms.GroupBox ();
            this.mUnloadDaControlChar = new System.Windows.Forms.Button ();
            this.mHideDaControlChar = new System.Windows.Forms.Button ();
            this.mShowDaControlChars = new System.Windows.Forms.Button ();
            this.mShowDaControlOptions = new System.Windows.Forms.Button ();
            this.mShowDaControlChar = new System.Windows.Forms.Button ();
            this.mLoadDaControlChar = new System.Windows.Forms.Button ();
            this.mMsControl = new AxAgentObjects.AxAgent ();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip ();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem ();
            this.MsServerBox.SuspendLayout ();
            this.MsControlBox.SuspendLayout ();
            this.DaServerBox.SuspendLayout ();
            this.DaControlBox.SuspendLayout ();
            ((System.ComponentModel.ISupportInitialize)(this.mMsControl)).BeginInit ();
            this.menuStrip1.SuspendLayout ();
            this.SuspendLayout ();
            // 
            // MsServerBox
            // 
            this.MsServerBox.Controls.Add (this.mUnloadMsServerChar);
            this.MsServerBox.Controls.Add (this.mHideMsServerChar);
            this.MsServerBox.Controls.Add (this.mShowMsServerChars);
            this.MsServerBox.Controls.Add (this.mShowMsServerOptions);
            this.MsServerBox.Controls.Add (this.mShowMsServerChar);
            this.MsServerBox.Controls.Add (this.mLoadMsServerChar);
            this.MsServerBox.Location = new System.Drawing.Point (16, 39);
            this.MsServerBox.Margin = new System.Windows.Forms.Padding (4);
            this.MsServerBox.Name = "MsServerBox";
            this.MsServerBox.Padding = new System.Windows.Forms.Padding (4);
            this.MsServerBox.Size = new System.Drawing.Size (357, 180);
            this.MsServerBox.TabIndex = 0;
            this.MsServerBox.TabStop = false;
            this.MsServerBox.Text = "MS Agent Server";
            // 
            // mUnloadMsServerChar
            // 
            this.mUnloadMsServerChar.Location = new System.Drawing.Point (8, 101);
            this.mUnloadMsServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mUnloadMsServerChar.Name = "mUnloadMsServerChar";
            this.mUnloadMsServerChar.Size = new System.Drawing.Size (101, 70);
            this.mUnloadMsServerChar.TabIndex = 2;
            this.mUnloadMsServerChar.Text = "Unload character";
            this.mUnloadMsServerChar.UseVisualStyleBackColor = true;
            this.mUnloadMsServerChar.Click += new System.EventHandler (this.UnloadMsServerChar_Click);
            // 
            // mHideMsServerChar
            // 
            this.mHideMsServerChar.Location = new System.Drawing.Point (117, 101);
            this.mHideMsServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mHideMsServerChar.Name = "mHideMsServerChar";
            this.mHideMsServerChar.Size = new System.Drawing.Size (101, 70);
            this.mHideMsServerChar.TabIndex = 3;
            this.mHideMsServerChar.Text = "Hide character";
            this.mHideMsServerChar.UseVisualStyleBackColor = true;
            this.mHideMsServerChar.Click += new System.EventHandler (this.HideMsServerChar_Click);
            // 
            // mShowMsServerChars
            // 
            this.mShowMsServerChars.Location = new System.Drawing.Point (249, 101);
            this.mShowMsServerChars.Margin = new System.Windows.Forms.Padding (4);
            this.mShowMsServerChars.Name = "mShowMsServerChars";
            this.mShowMsServerChars.Size = new System.Drawing.Size (101, 70);
            this.mShowMsServerChars.TabIndex = 5;
            this.mShowMsServerChars.Text = "Show characters";
            this.mShowMsServerChars.UseVisualStyleBackColor = true;
            this.mShowMsServerChars.Click += new System.EventHandler (this.ShowMsServerChars_Click);
            // 
            // mShowMsServerOptions
            // 
            this.mShowMsServerOptions.Location = new System.Drawing.Point (249, 23);
            this.mShowMsServerOptions.Margin = new System.Windows.Forms.Padding (4);
            this.mShowMsServerOptions.Name = "mShowMsServerOptions";
            this.mShowMsServerOptions.Size = new System.Drawing.Size (101, 70);
            this.mShowMsServerOptions.TabIndex = 4;
            this.mShowMsServerOptions.Text = "Show options";
            this.mShowMsServerOptions.UseVisualStyleBackColor = true;
            this.mShowMsServerOptions.Click += new System.EventHandler (this.ShowMsServerOptions_Click);
            // 
            // mShowMsServerChar
            // 
            this.mShowMsServerChar.Location = new System.Drawing.Point (117, 23);
            this.mShowMsServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mShowMsServerChar.Name = "mShowMsServerChar";
            this.mShowMsServerChar.Size = new System.Drawing.Size (101, 70);
            this.mShowMsServerChar.TabIndex = 1;
            this.mShowMsServerChar.Text = "Show character";
            this.mShowMsServerChar.UseVisualStyleBackColor = true;
            this.mShowMsServerChar.Click += new System.EventHandler (this.ShowMsServerChar_Click);
            // 
            // mLoadMsServerChar
            // 
            this.mLoadMsServerChar.Location = new System.Drawing.Point (8, 23);
            this.mLoadMsServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mLoadMsServerChar.Name = "mLoadMsServerChar";
            this.mLoadMsServerChar.Size = new System.Drawing.Size (101, 70);
            this.mLoadMsServerChar.TabIndex = 0;
            this.mLoadMsServerChar.Text = "Load character";
            this.mLoadMsServerChar.UseVisualStyleBackColor = true;
            this.mLoadMsServerChar.Click += new System.EventHandler (this.LoadMsServerChar_Click);
            // 
            // MsControlBox
            // 
            this.MsControlBox.Controls.Add (this.mUnloadMsControlChar);
            this.MsControlBox.Controls.Add (this.mHideMsControlChar);
            this.MsControlBox.Controls.Add (this.mShowMsControlChars);
            this.MsControlBox.Controls.Add (this.mShowMsControlOptions);
            this.MsControlBox.Controls.Add (this.mShowMsControlChar);
            this.MsControlBox.Controls.Add (this.mLoadMsControlChar);
            this.MsControlBox.Location = new System.Drawing.Point (433, 39);
            this.MsControlBox.Margin = new System.Windows.Forms.Padding (4);
            this.MsControlBox.Name = "MsControlBox";
            this.MsControlBox.Padding = new System.Windows.Forms.Padding (4);
            this.MsControlBox.Size = new System.Drawing.Size (357, 180);
            this.MsControlBox.TabIndex = 1;
            this.MsControlBox.TabStop = false;
            this.MsControlBox.Text = "MS Agent Control";
            // 
            // mUnloadMsControlChar
            // 
            this.mUnloadMsControlChar.Location = new System.Drawing.Point (8, 101);
            this.mUnloadMsControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mUnloadMsControlChar.Name = "mUnloadMsControlChar";
            this.mUnloadMsControlChar.Size = new System.Drawing.Size (101, 70);
            this.mUnloadMsControlChar.TabIndex = 2;
            this.mUnloadMsControlChar.Text = "Unload character";
            this.mUnloadMsControlChar.UseVisualStyleBackColor = true;
            this.mUnloadMsControlChar.Click += new System.EventHandler (this.UnloadMsControlChar_Click);
            // 
            // mHideMsControlChar
            // 
            this.mHideMsControlChar.Location = new System.Drawing.Point (117, 101);
            this.mHideMsControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mHideMsControlChar.Name = "mHideMsControlChar";
            this.mHideMsControlChar.Size = new System.Drawing.Size (101, 70);
            this.mHideMsControlChar.TabIndex = 3;
            this.mHideMsControlChar.Text = "Hide character";
            this.mHideMsControlChar.UseVisualStyleBackColor = true;
            this.mHideMsControlChar.Click += new System.EventHandler (this.HideMsControlChar_Click);
            // 
            // mShowMsControlChars
            // 
            this.mShowMsControlChars.Location = new System.Drawing.Point (249, 101);
            this.mShowMsControlChars.Margin = new System.Windows.Forms.Padding (4);
            this.mShowMsControlChars.Name = "mShowMsControlChars";
            this.mShowMsControlChars.Size = new System.Drawing.Size (101, 70);
            this.mShowMsControlChars.TabIndex = 5;
            this.mShowMsControlChars.Text = "Show characters";
            this.mShowMsControlChars.UseVisualStyleBackColor = true;
            this.mShowMsControlChars.Click += new System.EventHandler (this.ShowMsControlChars_Click);
            // 
            // mShowMsControlOptions
            // 
            this.mShowMsControlOptions.Location = new System.Drawing.Point (249, 23);
            this.mShowMsControlOptions.Margin = new System.Windows.Forms.Padding (4);
            this.mShowMsControlOptions.Name = "mShowMsControlOptions";
            this.mShowMsControlOptions.Size = new System.Drawing.Size (101, 70);
            this.mShowMsControlOptions.TabIndex = 4;
            this.mShowMsControlOptions.Text = "Show options";
            this.mShowMsControlOptions.UseVisualStyleBackColor = true;
            this.mShowMsControlOptions.Click += new System.EventHandler (this.ShowMsControlOptions_Click);
            // 
            // mShowMsControlChar
            // 
            this.mShowMsControlChar.Location = new System.Drawing.Point (117, 23);
            this.mShowMsControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mShowMsControlChar.Name = "mShowMsControlChar";
            this.mShowMsControlChar.Size = new System.Drawing.Size (101, 70);
            this.mShowMsControlChar.TabIndex = 1;
            this.mShowMsControlChar.Text = "Show character";
            this.mShowMsControlChar.UseVisualStyleBackColor = true;
            this.mShowMsControlChar.Click += new System.EventHandler (this.ShowMsControlChar_Click);
            // 
            // mLoadMsControlChar
            // 
            this.mLoadMsControlChar.Location = new System.Drawing.Point (8, 23);
            this.mLoadMsControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mLoadMsControlChar.Name = "mLoadMsControlChar";
            this.mLoadMsControlChar.Size = new System.Drawing.Size (101, 70);
            this.mLoadMsControlChar.TabIndex = 0;
            this.mLoadMsControlChar.Text = "Load character";
            this.mLoadMsControlChar.UseVisualStyleBackColor = true;
            this.mLoadMsControlChar.Click += new System.EventHandler (this.LoadMsControlChar_Click);
            // 
            // DaServerBox
            // 
            this.DaServerBox.Controls.Add (this.mUnloadDaServerChar);
            this.DaServerBox.Controls.Add (this.mHideDaServerChar);
            this.DaServerBox.Controls.Add (this.mShowDaServerChars);
            this.DaServerBox.Controls.Add (this.mShowDaServerOptions);
            this.DaServerBox.Controls.Add (this.mShowDaServerChar);
            this.DaServerBox.Controls.Add (this.mLoadDaServerChar);
            this.DaServerBox.Location = new System.Drawing.Point (16, 247);
            this.DaServerBox.Margin = new System.Windows.Forms.Padding (4);
            this.DaServerBox.Name = "DaServerBox";
            this.DaServerBox.Padding = new System.Windows.Forms.Padding (4);
            this.DaServerBox.Size = new System.Drawing.Size (357, 180);
            this.DaServerBox.TabIndex = 2;
            this.DaServerBox.TabStop = false;
            this.DaServerBox.Text = "Double Agent Server";
            // 
            // mUnloadDaServerChar
            // 
            this.mUnloadDaServerChar.Location = new System.Drawing.Point (8, 101);
            this.mUnloadDaServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mUnloadDaServerChar.Name = "mUnloadDaServerChar";
            this.mUnloadDaServerChar.Size = new System.Drawing.Size (101, 70);
            this.mUnloadDaServerChar.TabIndex = 2;
            this.mUnloadDaServerChar.Text = "Unload character";
            this.mUnloadDaServerChar.UseVisualStyleBackColor = true;
            this.mUnloadDaServerChar.Click += new System.EventHandler (this.UnloadDaServerChar_Click);
            // 
            // mHideDaServerChar
            // 
            this.mHideDaServerChar.Location = new System.Drawing.Point (117, 101);
            this.mHideDaServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mHideDaServerChar.Name = "mHideDaServerChar";
            this.mHideDaServerChar.Size = new System.Drawing.Size (101, 70);
            this.mHideDaServerChar.TabIndex = 3;
            this.mHideDaServerChar.Text = "Hide character";
            this.mHideDaServerChar.UseVisualStyleBackColor = true;
            this.mHideDaServerChar.Click += new System.EventHandler (this.HideDaServerChar_Click);
            // 
            // mShowDaServerChars
            // 
            this.mShowDaServerChars.Location = new System.Drawing.Point (249, 101);
            this.mShowDaServerChars.Margin = new System.Windows.Forms.Padding (4);
            this.mShowDaServerChars.Name = "mShowDaServerChars";
            this.mShowDaServerChars.Size = new System.Drawing.Size (101, 70);
            this.mShowDaServerChars.TabIndex = 5;
            this.mShowDaServerChars.Text = "Show characters";
            this.mShowDaServerChars.UseVisualStyleBackColor = true;
            this.mShowDaServerChars.Click += new System.EventHandler (this.ShowDaServerChars_Click);
            // 
            // mShowDaServerOptions
            // 
            this.mShowDaServerOptions.Location = new System.Drawing.Point (249, 23);
            this.mShowDaServerOptions.Margin = new System.Windows.Forms.Padding (4);
            this.mShowDaServerOptions.Name = "mShowDaServerOptions";
            this.mShowDaServerOptions.Size = new System.Drawing.Size (101, 70);
            this.mShowDaServerOptions.TabIndex = 4;
            this.mShowDaServerOptions.Text = "Show options";
            this.mShowDaServerOptions.UseVisualStyleBackColor = true;
            this.mShowDaServerOptions.Click += new System.EventHandler (this.ShowDaServerOptions_Click);
            // 
            // mShowDaServerChar
            // 
            this.mShowDaServerChar.Location = new System.Drawing.Point (117, 23);
            this.mShowDaServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mShowDaServerChar.Name = "mShowDaServerChar";
            this.mShowDaServerChar.Size = new System.Drawing.Size (101, 70);
            this.mShowDaServerChar.TabIndex = 1;
            this.mShowDaServerChar.Text = "Show character";
            this.mShowDaServerChar.UseVisualStyleBackColor = true;
            this.mShowDaServerChar.Click += new System.EventHandler (this.ShowDaServerChar_Click);
            // 
            // mLoadDaServerChar
            // 
            this.mLoadDaServerChar.Location = new System.Drawing.Point (8, 23);
            this.mLoadDaServerChar.Margin = new System.Windows.Forms.Padding (4);
            this.mLoadDaServerChar.Name = "mLoadDaServerChar";
            this.mLoadDaServerChar.Size = new System.Drawing.Size (101, 70);
            this.mLoadDaServerChar.TabIndex = 0;
            this.mLoadDaServerChar.Text = "Load character";
            this.mLoadDaServerChar.UseVisualStyleBackColor = true;
            this.mLoadDaServerChar.Click += new System.EventHandler (this.LoadDaServerChar_Click);
            // 
            // DaControlBox
            // 
            this.DaControlBox.Controls.Add (this.mUnloadDaControlChar);
            this.DaControlBox.Controls.Add (this.mHideDaControlChar);
            this.DaControlBox.Controls.Add (this.mShowDaControlChars);
            this.DaControlBox.Controls.Add (this.mShowDaControlOptions);
            this.DaControlBox.Controls.Add (this.mShowDaControlChar);
            this.DaControlBox.Controls.Add (this.mLoadDaControlChar);
            this.DaControlBox.Location = new System.Drawing.Point (433, 247);
            this.DaControlBox.Margin = new System.Windows.Forms.Padding (4);
            this.DaControlBox.Name = "DaControlBox";
            this.DaControlBox.Padding = new System.Windows.Forms.Padding (4);
            this.DaControlBox.Size = new System.Drawing.Size (357, 180);
            this.DaControlBox.TabIndex = 3;
            this.DaControlBox.TabStop = false;
            this.DaControlBox.Text = "Double Agent Control";
            // 
            // mUnloadDaControlChar
            // 
            this.mUnloadDaControlChar.Location = new System.Drawing.Point (8, 101);
            this.mUnloadDaControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mUnloadDaControlChar.Name = "mUnloadDaControlChar";
            this.mUnloadDaControlChar.Size = new System.Drawing.Size (101, 70);
            this.mUnloadDaControlChar.TabIndex = 2;
            this.mUnloadDaControlChar.Text = "Unload character";
            this.mUnloadDaControlChar.UseVisualStyleBackColor = true;
            this.mUnloadDaControlChar.Click += new System.EventHandler (this.UnloadDaControlChar_Click);
            // 
            // mHideDaControlChar
            // 
            this.mHideDaControlChar.Location = new System.Drawing.Point (117, 101);
            this.mHideDaControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mHideDaControlChar.Name = "mHideDaControlChar";
            this.mHideDaControlChar.Size = new System.Drawing.Size (101, 70);
            this.mHideDaControlChar.TabIndex = 3;
            this.mHideDaControlChar.Text = "Hide character";
            this.mHideDaControlChar.UseVisualStyleBackColor = true;
            this.mHideDaControlChar.Click += new System.EventHandler (this.HideDaControlChar_Click);
            // 
            // mShowDaControlChars
            // 
            this.mShowDaControlChars.Location = new System.Drawing.Point (249, 101);
            this.mShowDaControlChars.Margin = new System.Windows.Forms.Padding (4);
            this.mShowDaControlChars.Name = "mShowDaControlChars";
            this.mShowDaControlChars.Size = new System.Drawing.Size (101, 70);
            this.mShowDaControlChars.TabIndex = 5;
            this.mShowDaControlChars.Text = "Show characters";
            this.mShowDaControlChars.UseVisualStyleBackColor = true;
            this.mShowDaControlChars.Click += new System.EventHandler (this.ShowDaControlChars_Click);
            // 
            // mShowDaControlOptions
            // 
            this.mShowDaControlOptions.Location = new System.Drawing.Point (249, 23);
            this.mShowDaControlOptions.Margin = new System.Windows.Forms.Padding (4);
            this.mShowDaControlOptions.Name = "mShowDaControlOptions";
            this.mShowDaControlOptions.Size = new System.Drawing.Size (101, 70);
            this.mShowDaControlOptions.TabIndex = 4;
            this.mShowDaControlOptions.Text = "Show options";
            this.mShowDaControlOptions.UseVisualStyleBackColor = true;
            this.mShowDaControlOptions.Click += new System.EventHandler (this.ShowDaControlOptions_Click);
            // 
            // mShowDaControlChar
            // 
            this.mShowDaControlChar.Location = new System.Drawing.Point (117, 23);
            this.mShowDaControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mShowDaControlChar.Name = "mShowDaControlChar";
            this.mShowDaControlChar.Size = new System.Drawing.Size (101, 70);
            this.mShowDaControlChar.TabIndex = 1;
            this.mShowDaControlChar.Text = "Show character";
            this.mShowDaControlChar.UseVisualStyleBackColor = true;
            this.mShowDaControlChar.Click += new System.EventHandler (this.ShowDaControlChar_Click);
            // 
            // mLoadDaControlChar
            // 
            this.mLoadDaControlChar.Location = new System.Drawing.Point (8, 23);
            this.mLoadDaControlChar.Margin = new System.Windows.Forms.Padding (4);
            this.mLoadDaControlChar.Name = "mLoadDaControlChar";
            this.mLoadDaControlChar.Size = new System.Drawing.Size (101, 70);
            this.mLoadDaControlChar.TabIndex = 0;
            this.mLoadDaControlChar.Text = "Load character";
            this.mLoadDaControlChar.UseVisualStyleBackColor = true;
            this.mLoadDaControlChar.Click += new System.EventHandler (this.LoadDaControlChar_Click);
            // 
            // mMsControl
            // 
            this.mMsControl.Enabled = true;
            this.mMsControl.Location = new System.Drawing.Point (393, 39);
            this.mMsControl.Margin = new System.Windows.Forms.Padding (4);
            this.mMsControl.Name = "mMsControl";
            this.mMsControl.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject ("mMsControl.OcxState")));
            this.mMsControl.Size = new System.Drawing.Size (40, 40);
            this.mMsControl.TabIndex = 4;
            this.mMsControl.HideEvent += new AxAgentObjects._AgentEvents_HideEventHandler (this.MsControl_HideEvent);
            this.mMsControl.ShowEvent += new AxAgentObjects._AgentEvents_ShowEventHandler (this.MsControl_ShowEvent);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point (0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size (807, 28);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size (53, 24);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size (119, 24);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler (this.aboutToolStripMenuItem_Click);
            // 
            // MsaWithDa
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size (807, 436);
            this.Controls.Add (this.mMsControl);
            this.Controls.Add (this.DaControlBox);
            this.Controls.Add (this.MsControlBox);
            this.Controls.Add (this.DaServerBox);
            this.Controls.Add (this.MsServerBox);
            this.Controls.Add (this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding (4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MsaWithDa";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "MS Agent with Double Agent";
            this.MsServerBox.ResumeLayout (false);
            this.MsControlBox.ResumeLayout (false);
            this.DaServerBox.ResumeLayout (false);
            this.DaControlBox.ResumeLayout (false);
            ((System.ComponentModel.ISupportInitialize)(this.mMsControl)).EndInit ();
            this.menuStrip1.ResumeLayout (false);
            this.menuStrip1.PerformLayout ();
            this.ResumeLayout (false);
            this.PerformLayout ();

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
		private AxAgentObjects.AxAgent mMsControl;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
    }
}

