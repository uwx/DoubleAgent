/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
namespace AgentCharacterEditor.Panels
{
	partial class PartsTreePanel
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
			System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode(global::AgentCharacterEditor.Resources.Resources.TitleBalloon);
			System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode(global::AgentCharacterEditor.Resources.Resources.TitleTts);
			System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Character", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2});
			System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Animations");
			System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("GesturingDown");
			System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("GesturingLeft");
			System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("GesturingRight");
			System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("GesturingUp");
			System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("Hearing");
			System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("Hiding");
			System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode("IdlingLevel1");
			System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode("IdlingLevel2");
			System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode("IdlingLevel3");
			System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode("Listening");
			System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode("MovingDown");
			System.Windows.Forms.TreeNode treeNode16 = new System.Windows.Forms.TreeNode("MovingLeft");
			System.Windows.Forms.TreeNode treeNode17 = new System.Windows.Forms.TreeNode("MovingRight");
			System.Windows.Forms.TreeNode treeNode18 = new System.Windows.Forms.TreeNode("MovingUp");
			System.Windows.Forms.TreeNode treeNode19 = new System.Windows.Forms.TreeNode("Showing");
			System.Windows.Forms.TreeNode treeNode20 = new System.Windows.Forms.TreeNode("Speaking");
			System.Windows.Forms.TreeNode treeNode21 = new System.Windows.Forms.TreeNode("States", new System.Windows.Forms.TreeNode[] {
            treeNode5,
            treeNode6,
            treeNode7,
            treeNode8,
            treeNode9,
            treeNode10,
            treeNode11,
            treeNode12,
            treeNode13,
            treeNode14,
            treeNode15,
            treeNode16,
            treeNode17,
            treeNode18,
            treeNode19,
            treeNode20});
			this.TreeViewMain = new DoubleAgent.TreeViewEx ();
			this.SuspendLayout();
			// 
			// TreeViewMain
			// 
			this.TreeViewMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.TreeViewMain.FullRowSelect = true;
			this.TreeViewMain.HideSelection = false;
			this.TreeViewMain.Location = new System.Drawing.Point(0, 0);
			this.TreeViewMain.Margin = new System.Windows.Forms.Padding(0);
			this.TreeViewMain.Name = "TreeViewMain";
			treeNode1.Name = "NodeWordBalloon";
			treeNode1.Text = global::AgentCharacterEditor.Resources.Resources.TitleBalloon;
			treeNode2.Name = "NodeTTSMode";
			treeNode2.Text = global::AgentCharacterEditor.Resources.Resources.TitleTts;
			treeNode3.ForeColor = System.Drawing.SystemColors.HotTrack;
			treeNode3.Name = "NodeCharacter";
			treeNode3.Text = "Character";
			treeNode4.ForeColor = System.Drawing.SystemColors.HotTrack;
			treeNode4.Name = "NodeAnimations";
			treeNode4.Text = "Animations";
			treeNode5.Name = "NodeStateGesturingDown";
			treeNode5.Tag = "GesturingDown";
			treeNode5.Text = "GesturingDown";
			treeNode6.Name = "NodeStateGesturingLeft";
			treeNode6.Tag = "GesturingLeft";
			treeNode6.Text = "GesturingLeft";
			treeNode7.Name = "NodeStateGesturingRight";
			treeNode7.Tag = "GesturingRight";
			treeNode7.Text = "GesturingRight";
			treeNode8.Name = "NodeStateGesturingUp";
			treeNode8.Tag = "GesturingUp";
			treeNode8.Text = "GesturingUp";
			treeNode9.Name = "NodeStateHearing";
			treeNode9.Tag = "Hearing";
			treeNode9.Text = "Hearing";
			treeNode10.Name = "NodeStateHiding";
			treeNode10.Tag = "Hiding";
			treeNode10.Text = "Hiding";
			treeNode11.Name = "NodeStateIdlingLevel1";
			treeNode11.Tag = "IdlingLevel1";
			treeNode11.Text = "IdlingLevel1";
			treeNode12.Name = "NodeStateIdlingLevel2";
			treeNode12.Tag = "IdlingLevel2";
			treeNode12.Text = "IdlingLevel2";
			treeNode13.Name = "NodeStateIdlingLevel3";
			treeNode13.Tag = "IdlingLevel3";
			treeNode13.Text = "IdlingLevel3";
			treeNode14.Name = "NodeStateListening";
			treeNode14.Tag = "Listening";
			treeNode14.Text = "Listening";
			treeNode15.Name = "NodeStateMovingDown";
			treeNode15.Tag = "MovingDown";
			treeNode15.Text = "MovingDown";
			treeNode16.Name = "NodeStateMovingLeft";
			treeNode16.Tag = "MovingLeft";
			treeNode16.Text = "MovingLeft";
			treeNode17.Name = "NodeStateMovingRight";
			treeNode17.Tag = "MovingRight";
			treeNode17.Text = "MovingRight";
			treeNode18.Name = "NodeStateMovingUp";
			treeNode18.Tag = "MovingUp";
			treeNode18.Text = "MovingUp";
			treeNode19.Name = "NodeStateShowing";
			treeNode19.Tag = "Showing";
			treeNode19.Text = "Showing";
			treeNode20.Name = "NodeStateSpeaking";
			treeNode20.Tag = "Speaking";
			treeNode20.Text = "Speaking";
			treeNode21.ForeColor = System.Drawing.SystemColors.HotTrack;
			treeNode21.Name = "NodeStates";
			treeNode21.Text = "States";
			this.TreeViewMain.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode3,
            treeNode4,
            treeNode21});
			this.TreeViewMain.Size = new System.Drawing.Size(200, 450);
			this.TreeViewMain.TabIndex = 1;
			this.TreeViewMain.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.TreeViewMain_AfterSelect);
			// 
			// PartsTreeForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.TreeViewMain);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "PartsTreeForm";
			this.Size = new System.Drawing.Size(200, 450);
			this.ResumeLayout(false);

		}

		#endregion

		internal DoubleAgent.TreeViewEx TreeViewMain;

	}
}
