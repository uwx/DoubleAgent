namespace AgentCharacterEditor
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
			System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode ("Word Balloon");
			System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode ("Voice");
			System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode ("Character", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2});
			System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode ("Animations");
			System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode ("GesturingDown");
			System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode ("GesturingLeft");
			System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode ("GesturingRight");
			System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode ("GesturingUp");
			System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode ("Hearing");
			System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode ("Hiding");
			System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode ("IdlingLevel1");
			System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode ("IdlingLevel2");
			System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode ("IdlingLevel3");
			System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode ("Listening");
			System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode ("MovingDown");
			System.Windows.Forms.TreeNode treeNode16 = new System.Windows.Forms.TreeNode ("MovingLeft");
			System.Windows.Forms.TreeNode treeNode17 = new System.Windows.Forms.TreeNode ("MovingRight");
			System.Windows.Forms.TreeNode treeNode18 = new System.Windows.Forms.TreeNode ("MovingUp");
			System.Windows.Forms.TreeNode treeNode19 = new System.Windows.Forms.TreeNode ("Showing");
			System.Windows.Forms.TreeNode treeNode20 = new System.Windows.Forms.TreeNode ("Speaking");
			System.Windows.Forms.TreeNode treeNode21 = new System.Windows.Forms.TreeNode ("States", new System.Windows.Forms.TreeNode[] {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (MainForm));
			this.ToolStripContainerMain = new System.Windows.Forms.ToolStripContainer ();
			this.SplitContainerMain = new System.Windows.Forms.SplitContainer ();
			this.TreeViewMain = new System.Windows.Forms.TreeView ();
			this.PanelBalloon = new AgentCharacterEditor.BalloonForm ();
			this.PanelCharacter = new AgentCharacterEditor.CharacterForm ();
			this.PanelTts = new AgentCharacterEditor.TtsForm ();
			this.PanelAnimations = new AgentCharacterEditor.AnimationsForm ();
			this.PanelAnimation = new AgentCharacterEditor.AnimationForm ();
			this.PanelState = new AgentCharacterEditor.StateForm ();
			this.PanelFrame = new AgentCharacterEditor.FrameForm ();
			this.PanelBranching = new AgentCharacterEditor.BranchingForm ();
			this.PanelOverlays = new AgentCharacterEditor.OverlayForm ();
			this.ToolStripTop = new System.Windows.Forms.ToolStrip ();
			this.ToolButtonFileNew = new System.Windows.Forms.ToolStripButton ();
			this.ToolButtonFileOpen = new System.Windows.Forms.ToolStripButton ();
			this.ToolButtonFileSave = new System.Windows.Forms.ToolStripButton ();
			this.ToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator ();
			this.ToolButtonEditUndo = new System.Windows.Forms.ToolStripButton ();
			this.ToolButtonEditRedo = new System.Windows.Forms.ToolStripButton ();
			this.ToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator ();
			this.ToolButtonEditCut = new System.Windows.Forms.ToolStripButton ();
			this.ToolButtonEditCopy = new System.Windows.Forms.ToolStripButton ();
			this.ToolButtonEditPaste = new System.Windows.Forms.ToolStripButton ();
			this.ToolButtonEditDelete = new System.Windows.Forms.ToolStripButton ();
			this.MenuStripMain = new System.Windows.Forms.MenuStrip ();
			this.MenuItemFile = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemFileNew = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemFileOpen = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuSepFile1 = new System.Windows.Forms.ToolStripSeparator ();
			this.MenuItemFileSave = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemFileSaveAs = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuSepFile2 = new System.Windows.Forms.ToolStripSeparator ();
			this.MenuItemFilePrint = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemFilePrintPreview = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuSepFile3 = new System.Windows.Forms.ToolStripSeparator ();
			this.MenuItemExit = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemEdit = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemEditUndo = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemEditRedo = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuSepEdit1 = new System.Windows.Forms.ToolStripSeparator ();
			this.MenuItemEditCut = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemEditCopy = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemEditPaste = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemEditDelete = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemHelp = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemHelpContents = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemHelpIndex = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuItemHelpSearch = new System.Windows.Forms.ToolStripMenuItem ();
			this.MenuSepHelp1 = new System.Windows.Forms.ToolStripSeparator ();
			this.MenuItemHelpAbout = new System.Windows.Forms.ToolStripMenuItem ();
			this.ToolStripContainerMain.ContentPanel.SuspendLayout ();
			this.ToolStripContainerMain.TopToolStripPanel.SuspendLayout ();
			this.ToolStripContainerMain.SuspendLayout ();
			this.SplitContainerMain.Panel1.SuspendLayout ();
			this.SplitContainerMain.Panel2.SuspendLayout ();
			this.SplitContainerMain.SuspendLayout ();
			this.ToolStripTop.SuspendLayout ();
			this.MenuStripMain.SuspendLayout ();
			this.SuspendLayout ();
			// 
			// ToolStripContainerMain
			// 
			this.ToolStripContainerMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ToolStripContainerMain.BottomToolStripPanelVisible = false;
			// 
			// ToolStripContainerMain.ContentPanel
			// 
			this.ToolStripContainerMain.ContentPanel.Controls.Add (this.SplitContainerMain);
			this.ToolStripContainerMain.ContentPanel.Margin = new System.Windows.Forms.Padding (0);
			this.ToolStripContainerMain.ContentPanel.Padding = new System.Windows.Forms.Padding (8);
			this.ToolStripContainerMain.ContentPanel.Size = new System.Drawing.Size (982, 497);
			this.ToolStripContainerMain.LeftToolStripPanelVisible = false;
			this.ToolStripContainerMain.Location = new System.Drawing.Point (0, 28);
			this.ToolStripContainerMain.Margin = new System.Windows.Forms.Padding (0);
			this.ToolStripContainerMain.Name = "ToolStripContainerMain";
			this.ToolStripContainerMain.RightToolStripPanelVisible = false;
			this.ToolStripContainerMain.Size = new System.Drawing.Size (982, 527);
			this.ToolStripContainerMain.TabIndex = 0;
			// 
			// ToolStripContainerMain.TopToolStripPanel
			// 
			this.ToolStripContainerMain.TopToolStripPanel.BackColor = System.Drawing.SystemColors.Control;
			this.ToolStripContainerMain.TopToolStripPanel.Controls.Add (this.ToolStripTop);
			// 
			// SplitContainerMain
			// 
			this.SplitContainerMain.CausesValidation = false;
			this.SplitContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitContainerMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.SplitContainerMain.Location = new System.Drawing.Point (8, 8);
			this.SplitContainerMain.Margin = new System.Windows.Forms.Padding (0);
			this.SplitContainerMain.Name = "SplitContainerMain";
			// 
			// SplitContainerMain.Panel1
			// 
			this.SplitContainerMain.Panel1.Controls.Add (this.TreeViewMain);
			this.SplitContainerMain.Panel1MinSize = 100;
			// 
			// SplitContainerMain.Panel2
			// 
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelBalloon);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelCharacter);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelTts);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelAnimations);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelAnimation);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelState);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelFrame);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelBranching);
			this.SplitContainerMain.Panel2.Controls.Add (this.PanelOverlays);
			this.SplitContainerMain.Panel2MinSize = 100;
			this.SplitContainerMain.Size = new System.Drawing.Size (966, 481);
			this.SplitContainerMain.SplitterDistance = 306;
			this.SplitContainerMain.SplitterWidth = 8;
			this.SplitContainerMain.TabIndex = 0;
			this.SplitContainerMain.TabStop = false;
			// 
			// TreeViewMain
			// 
			this.TreeViewMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TreeViewMain.FullRowSelect = true;
			this.TreeViewMain.HideSelection = false;
			this.TreeViewMain.Location = new System.Drawing.Point (0, 0);
			this.TreeViewMain.Margin = new System.Windows.Forms.Padding (0);
			this.TreeViewMain.Name = "TreeViewMain";
			treeNode1.Name = "NodeWordBalloon";
			treeNode1.Text = "Word Balloon";
			treeNode2.Name = "NodeTTSMode";
			treeNode2.Text = "Voice";
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
			this.TreeViewMain.Nodes.AddRange (new System.Windows.Forms.TreeNode[] {
            treeNode3,
            treeNode4,
            treeNode21});
			this.TreeViewMain.Size = new System.Drawing.Size (306, 480);
			this.TreeViewMain.TabIndex = 0;
			this.TreeViewMain.AfterSelect += new System.Windows.Forms.TreeViewEventHandler (this.TreeViewMain_AfterSelect);
			// 
			// PanelBalloon
			// 
			this.PanelBalloon.CharacterFile = null;
			this.PanelBalloon.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelBalloon.Location = new System.Drawing.Point (0, 0);
			this.PanelBalloon.Margin = new System.Windows.Forms.Padding (0);
			this.PanelBalloon.Name = "PanelBalloon";
			this.PanelBalloon.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.PanelBalloon.Size = new System.Drawing.Size (652, 481);
			this.PanelBalloon.TabIndex = 0;
			this.PanelBalloon.Visible = false;
			// 
			// PanelCharacter
			// 
			this.PanelCharacter.CharacterFile = null;
			this.PanelCharacter.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelCharacter.Location = new System.Drawing.Point (0, 0);
			this.PanelCharacter.Margin = new System.Windows.Forms.Padding (0);
			this.PanelCharacter.Name = "PanelCharacter";
			this.PanelCharacter.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.PanelCharacter.Size = new System.Drawing.Size (652, 481);
			this.PanelCharacter.TabIndex = 0;
			// 
			// PanelTts
			// 
			this.PanelTts.CharacterFile = null;
			this.PanelTts.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelTts.Location = new System.Drawing.Point (0, 0);
			this.PanelTts.Margin = new System.Windows.Forms.Padding (0);
			this.PanelTts.Name = "PanelTts";
			this.PanelTts.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.PanelTts.Size = new System.Drawing.Size (652, 481);
			this.PanelTts.TabIndex = 0;
			this.PanelTts.Visible = false;
			// 
			// PanelAnimations
			// 
			this.PanelAnimations.CharacterFile = null;
			this.PanelAnimations.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelAnimations.Location = new System.Drawing.Point (0, 0);
			this.PanelAnimations.Margin = new System.Windows.Forms.Padding (0);
			this.PanelAnimations.Name = "PanelAnimations";
			this.PanelAnimations.Padding = new System.Windows.Forms.Padding (9, 0, 9, 0);
			this.PanelAnimations.Size = new System.Drawing.Size (652, 481);
			this.PanelAnimations.TabIndex = 0;
			this.PanelAnimations.Visible = false;
			this.PanelAnimations.AnimationAdded += new AgentCharacterEditor.AnimationsForm.AnimationAddedEvent (this.PanelAnimations_AnimationAdded);
			this.PanelAnimations.AnimationRemoved += new AgentCharacterEditor.AnimationsForm.AnimationRemovedEvent (this.PanelAnimations_AnimationRemoved);
			// 
			// PanelAnimation
			// 
			this.PanelAnimation.AnimationName = null;
			this.PanelAnimation.CharacterFile = null;
			this.PanelAnimation.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelAnimation.FileAnimation = null;
			this.PanelAnimation.Location = new System.Drawing.Point (0, 0);
			this.PanelAnimation.Margin = new System.Windows.Forms.Padding (0);
			this.PanelAnimation.Name = "PanelAnimation";
			this.PanelAnimation.Padding = new System.Windows.Forms.Padding (9);
			this.PanelAnimation.Size = new System.Drawing.Size (652, 481);
			this.PanelAnimation.TabIndex = 0;
			this.PanelAnimation.Visible = false;
			this.PanelAnimation.AnimationNameChanged += new AgentCharacterEditor.AnimationForm.AnimationNameChangedEvent (this.PanelAnimation_AnimationNameChanged);
			this.PanelAnimation.AnimationFrameRemoved += new AgentCharacterEditor.AnimationForm.AnimationFrameRemovedEvent (this.PanelAnimation_AnimationFrameRemoved);
			this.PanelAnimation.AnimationFrameAdded += new AgentCharacterEditor.AnimationForm.AnimationFrameAddedEvent (this.PanelAnimation_AnimationFrameAdded);
			this.PanelAnimation.AnimationFrameReordered += new AgentCharacterEditor.AnimationForm.AnimationFrameReorderedEvent (this.PanelAnimation_AnimationFrameReordered);
			// 
			// PanelState
			// 
			this.PanelState.CharacterFile = null;
			this.PanelState.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelState.Location = new System.Drawing.Point (0, 0);
			this.PanelState.Margin = new System.Windows.Forms.Padding (0);
			this.PanelState.Name = "PanelState";
			this.PanelState.Padding = new System.Windows.Forms.Padding (9);
			this.PanelState.Size = new System.Drawing.Size (652, 481);
			this.PanelState.StateName = null;
			this.PanelState.TabIndex = 0;
			this.PanelState.Visible = false;
			// 
			// PanelFrame
			// 
			this.PanelFrame.AnimationName = null;
			this.PanelFrame.CharacterFile = null;
			this.PanelFrame.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelFrame.FileFrame = null;
			this.PanelFrame.FrameName = null;
			this.PanelFrame.Location = new System.Drawing.Point (0, 0);
			this.PanelFrame.Margin = new System.Windows.Forms.Padding (0);
			this.PanelFrame.Name = "PanelFrame";
			this.PanelFrame.Padding = new System.Windows.Forms.Padding (9);
			this.PanelFrame.Size = new System.Drawing.Size (652, 481);
			this.PanelFrame.TabIndex = 0;
			this.PanelFrame.Visible = false;
			// 
			// PanelBranching
			// 
			this.PanelBranching.AnimationName = null;
			this.PanelBranching.CharacterFile = null;
			this.PanelBranching.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelBranching.FileFrame = null;
			this.PanelBranching.FrameName = null;
			this.PanelBranching.Location = new System.Drawing.Point (0, 0);
			this.PanelBranching.Margin = new System.Windows.Forms.Padding (0);
			this.PanelBranching.Name = "PanelBranching";
			this.PanelBranching.Padding = new System.Windows.Forms.Padding (9);
			this.PanelBranching.Size = new System.Drawing.Size (652, 481);
			this.PanelBranching.TabIndex = 0;
			this.PanelBranching.Visible = false;
			// 
			// PanelOverlays
			// 
			this.PanelOverlays.AnimationName = null;
			this.PanelOverlays.CharacterFile = null;
			this.PanelOverlays.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelOverlays.FileFrame = null;
			this.PanelOverlays.FrameName = null;
			this.PanelOverlays.Location = new System.Drawing.Point (0, 0);
			this.PanelOverlays.Margin = new System.Windows.Forms.Padding (0);
			this.PanelOverlays.Name = "PanelOverlays";
			this.PanelOverlays.Padding = new System.Windows.Forms.Padding (9);
			this.PanelOverlays.Size = new System.Drawing.Size (652, 481);
			this.PanelOverlays.TabIndex = 0;
			this.PanelOverlays.Visible = false;
			// 
			// ToolStripTop
			// 
			this.ToolStripTop.AllowMerge = false;
			this.ToolStripTop.CanOverflow = false;
			this.ToolStripTop.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripTop.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripTop.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.ToolButtonFileNew,
            this.ToolButtonFileOpen,
            this.ToolButtonFileSave,
            this.ToolStripSeparator1,
            this.ToolButtonEditUndo,
            this.ToolButtonEditRedo,
            this.ToolStripSeparator2,
            this.ToolButtonEditCut,
            this.ToolButtonEditCopy,
            this.ToolButtonEditPaste,
            this.ToolButtonEditDelete});
			this.ToolStripTop.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
			this.ToolStripTop.Location = new System.Drawing.Point (0, 0);
			this.ToolStripTop.Name = "ToolStripTop";
			this.ToolStripTop.Padding = new System.Windows.Forms.Padding (8, 2, 8, 2);
			this.ToolStripTop.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.ToolStripTop.Size = new System.Drawing.Size (982, 30);
			this.ToolStripTop.Stretch = true;
			this.ToolStripTop.TabIndex = 0;
			// 
			// ToolButtonFileNew
			// 
			this.ToolButtonFileNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonFileNew.Enabled = false;
			this.ToolButtonFileNew.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonFileNew.Image")));
			this.ToolButtonFileNew.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonFileNew.Name = "ToolButtonFileNew";
			this.ToolButtonFileNew.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonFileNew.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonFileNew.Text = "New";
			this.ToolButtonFileNew.Click += new System.EventHandler (this.ToolButtonFileNew_Click);
			// 
			// ToolButtonFileOpen
			// 
			this.ToolButtonFileOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonFileOpen.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonFileOpen.Image")));
			this.ToolButtonFileOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonFileOpen.Name = "ToolButtonFileOpen";
			this.ToolButtonFileOpen.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonFileOpen.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonFileOpen.Text = "Open";
			this.ToolButtonFileOpen.Click += new System.EventHandler (this.ToolButtonFileOpen_Click);
			// 
			// ToolButtonFileSave
			// 
			this.ToolButtonFileSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonFileSave.Enabled = false;
			this.ToolButtonFileSave.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonFileSave.Image")));
			this.ToolButtonFileSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonFileSave.Name = "ToolButtonFileSave";
			this.ToolButtonFileSave.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonFileSave.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonFileSave.Text = "Save";
			this.ToolButtonFileSave.Click += new System.EventHandler (this.ToolButtonFileSave_Click);
			// 
			// ToolStripSeparator1
			// 
			this.ToolStripSeparator1.Name = "ToolStripSeparator1";
			this.ToolStripSeparator1.Size = new System.Drawing.Size (6, 23);
			// 
			// ToolButtonEditUndo
			// 
			this.ToolButtonEditUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditUndo.Enabled = false;
			this.ToolButtonEditUndo.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonEditUndo.Image")));
			this.ToolButtonEditUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditUndo.Name = "ToolButtonEditUndo";
			this.ToolButtonEditUndo.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonEditUndo.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonEditUndo.Text = "Undo";
			this.ToolButtonEditUndo.Click += new System.EventHandler (this.ToolButtonEditUndo_Click);
			// 
			// ToolButtonEditRedo
			// 
			this.ToolButtonEditRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditRedo.Enabled = false;
			this.ToolButtonEditRedo.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonEditRedo.Image")));
			this.ToolButtonEditRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditRedo.Name = "ToolButtonEditRedo";
			this.ToolButtonEditRedo.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonEditRedo.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonEditRedo.Text = "Redo";
			this.ToolButtonEditRedo.Click += new System.EventHandler (this.ToolButtonEditRedo_Click);
			// 
			// ToolStripSeparator2
			// 
			this.ToolStripSeparator2.Name = "ToolStripSeparator2";
			this.ToolStripSeparator2.Size = new System.Drawing.Size (6, 23);
			// 
			// ToolButtonEditCut
			// 
			this.ToolButtonEditCut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditCut.Enabled = false;
			this.ToolButtonEditCut.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonEditCut.Image")));
			this.ToolButtonEditCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditCut.Name = "ToolButtonEditCut";
			this.ToolButtonEditCut.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonEditCut.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonEditCut.Text = "Cut";
			this.ToolButtonEditCut.Click += new System.EventHandler (this.ToolButtonEditCut_Click);
			// 
			// ToolButtonEditCopy
			// 
			this.ToolButtonEditCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditCopy.Enabled = false;
			this.ToolButtonEditCopy.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonEditCopy.Image")));
			this.ToolButtonEditCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditCopy.Name = "ToolButtonEditCopy";
			this.ToolButtonEditCopy.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonEditCopy.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonEditCopy.Text = "Copy";
			this.ToolButtonEditCopy.Click += new System.EventHandler (this.ToolButtonEditCopy_Click);
			// 
			// ToolButtonEditPaste
			// 
			this.ToolButtonEditPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditPaste.Enabled = false;
			this.ToolButtonEditPaste.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonEditPaste.Image")));
			this.ToolButtonEditPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditPaste.Name = "ToolButtonEditPaste";
			this.ToolButtonEditPaste.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonEditPaste.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonEditPaste.Text = "Paste";
			this.ToolButtonEditPaste.Click += new System.EventHandler (this.ToolButtonEditPaste_Click);
			// 
			// ToolButtonEditDelete
			// 
			this.ToolButtonEditDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditDelete.Enabled = false;
			this.ToolButtonEditDelete.Image = ((System.Drawing.Image)(resources.GetObject ("ToolButtonEditDelete.Image")));
			this.ToolButtonEditDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditDelete.Name = "ToolButtonEditDelete";
			this.ToolButtonEditDelete.Padding = new System.Windows.Forms.Padding (0, 1, 0, 2);
			this.ToolButtonEditDelete.Size = new System.Drawing.Size (23, 23);
			this.ToolButtonEditDelete.Text = "Delete";
			this.ToolButtonEditDelete.Click += new System.EventHandler (this.ToolButtonEditDelete_Click);
			// 
			// MenuStripMain
			// 
			this.MenuStripMain.Items.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemFile,
            this.MenuItemEdit,
            this.MenuItemHelp});
			this.MenuStripMain.Location = new System.Drawing.Point (0, 0);
			this.MenuStripMain.Name = "MenuStripMain";
			this.MenuStripMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
			this.MenuStripMain.Size = new System.Drawing.Size (982, 28);
			this.MenuStripMain.TabIndex = 1;
			this.MenuStripMain.Text = "MenuStripMain";
			// 
			// MenuItemFile
			// 
			this.MenuItemFile.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemFileNew,
            this.MenuItemFileOpen,
            this.MenuSepFile1,
            this.MenuItemFileSave,
            this.MenuItemFileSaveAs,
            this.MenuSepFile2,
            this.MenuItemFilePrint,
            this.MenuItemFilePrintPreview,
            this.MenuSepFile3,
            this.MenuItemExit});
			this.MenuItemFile.Name = "MenuItemFile";
			this.MenuItemFile.Size = new System.Drawing.Size (44, 24);
			this.MenuItemFile.Text = "&File";
			// 
			// MenuItemFileNew
			// 
			this.MenuItemFileNew.Enabled = false;
			this.MenuItemFileNew.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemFileNew.Image")));
			this.MenuItemFileNew.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFileNew.Name = "MenuItemFileNew";
			this.MenuItemFileNew.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
			this.MenuItemFileNew.Size = new System.Drawing.Size (176, 24);
			this.MenuItemFileNew.Text = "&New";
			this.MenuItemFileNew.Click += new System.EventHandler (this.MenuItemFileNew_Click);
			// 
			// MenuItemFileOpen
			// 
			this.MenuItemFileOpen.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemFileOpen.Image")));
			this.MenuItemFileOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFileOpen.Name = "MenuItemFileOpen";
			this.MenuItemFileOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
			this.MenuItemFileOpen.Size = new System.Drawing.Size (176, 24);
			this.MenuItemFileOpen.Text = "&Open...";
			this.MenuItemFileOpen.Click += new System.EventHandler (this.MenuItemFileOpen_Click);
			// 
			// MenuSepFile1
			// 
			this.MenuSepFile1.Name = "MenuSepFile1";
			this.MenuSepFile1.Size = new System.Drawing.Size (173, 6);
			// 
			// MenuItemFileSave
			// 
			this.MenuItemFileSave.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemFileSave.Image")));
			this.MenuItemFileSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFileSave.Name = "MenuItemFileSave";
			this.MenuItemFileSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.MenuItemFileSave.Size = new System.Drawing.Size (176, 24);
			this.MenuItemFileSave.Text = "&Save";
			this.MenuItemFileSave.Click += new System.EventHandler (this.MenuItemFileSave_Click);
			// 
			// MenuItemFileSaveAs
			// 
			this.MenuItemFileSaveAs.Enabled = false;
			this.MenuItemFileSaveAs.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemFileSaveAs.Image")));
			this.MenuItemFileSaveAs.Name = "MenuItemFileSaveAs";
			this.MenuItemFileSaveAs.Size = new System.Drawing.Size (176, 24);
			this.MenuItemFileSaveAs.Text = "Save &As...";
			this.MenuItemFileSaveAs.Click += new System.EventHandler (this.MenuItemFileSaveAs_Click);
			// 
			// MenuSepFile2
			// 
			this.MenuSepFile2.Name = "MenuSepFile2";
			this.MenuSepFile2.Size = new System.Drawing.Size (173, 6);
			// 
			// MenuItemFilePrint
			// 
			this.MenuItemFilePrint.Enabled = false;
			this.MenuItemFilePrint.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemFilePrint.Image")));
			this.MenuItemFilePrint.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFilePrint.Name = "MenuItemFilePrint";
			this.MenuItemFilePrint.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
			this.MenuItemFilePrint.Size = new System.Drawing.Size (176, 24);
			this.MenuItemFilePrint.Text = "&Print";
			this.MenuItemFilePrint.Click += new System.EventHandler (this.MenuItemFilePrint_Click);
			// 
			// MenuItemFilePrintPreview
			// 
			this.MenuItemFilePrintPreview.Enabled = false;
			this.MenuItemFilePrintPreview.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemFilePrintPreview.Image")));
			this.MenuItemFilePrintPreview.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFilePrintPreview.Name = "MenuItemFilePrintPreview";
			this.MenuItemFilePrintPreview.Size = new System.Drawing.Size (176, 24);
			this.MenuItemFilePrintPreview.Text = "Print Pre&view";
			this.MenuItemFilePrintPreview.Click += new System.EventHandler (this.MenuItemFilePrintPreview_Click);
			// 
			// MenuSepFile3
			// 
			this.MenuSepFile3.Name = "MenuSepFile3";
			this.MenuSepFile3.Size = new System.Drawing.Size (173, 6);
			// 
			// MenuItemExit
			// 
			this.MenuItemExit.Name = "MenuItemExit";
			this.MenuItemExit.Size = new System.Drawing.Size (176, 24);
			this.MenuItemExit.Text = "E&xit";
			this.MenuItemExit.Click += new System.EventHandler (this.MenuItemExit_Click);
			// 
			// MenuItemEdit
			// 
			this.MenuItemEdit.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemEditUndo,
            this.MenuItemEditRedo,
            this.MenuSepEdit1,
            this.MenuItemEditCut,
            this.MenuItemEditCopy,
            this.MenuItemEditPaste,
            this.MenuItemEditDelete});
			this.MenuItemEdit.Name = "MenuItemEdit";
			this.MenuItemEdit.Size = new System.Drawing.Size (47, 24);
			this.MenuItemEdit.Text = "&Edit";
			// 
			// MenuItemEditUndo
			// 
			this.MenuItemEditUndo.Enabled = false;
			this.MenuItemEditUndo.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemEditUndo.Image")));
			this.MenuItemEditUndo.Name = "MenuItemEditUndo";
			this.MenuItemEditUndo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
			this.MenuItemEditUndo.Size = new System.Drawing.Size (165, 24);
			this.MenuItemEditUndo.Text = "&Undo";
			this.MenuItemEditUndo.Click += new System.EventHandler (this.MenuItemEditUndo_Click);
			// 
			// MenuItemEditRedo
			// 
			this.MenuItemEditRedo.Enabled = false;
			this.MenuItemEditRedo.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemEditRedo.Image")));
			this.MenuItemEditRedo.Name = "MenuItemEditRedo";
			this.MenuItemEditRedo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
			this.MenuItemEditRedo.Size = new System.Drawing.Size (165, 24);
			this.MenuItemEditRedo.Text = "&Redo";
			this.MenuItemEditRedo.Click += new System.EventHandler (this.MenuItemEditRedo_Click);
			// 
			// MenuSepEdit1
			// 
			this.MenuSepEdit1.Name = "MenuSepEdit1";
			this.MenuSepEdit1.Size = new System.Drawing.Size (162, 6);
			// 
			// MenuItemEditCut
			// 
			this.MenuItemEditCut.Enabled = false;
			this.MenuItemEditCut.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemEditCut.Image")));
			this.MenuItemEditCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemEditCut.Name = "MenuItemEditCut";
			this.MenuItemEditCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
			this.MenuItemEditCut.Size = new System.Drawing.Size (165, 24);
			this.MenuItemEditCut.Text = "Cu&t";
			this.MenuItemEditCut.Click += new System.EventHandler (this.MenuItemEditCut_Click);
			// 
			// MenuItemEditCopy
			// 
			this.MenuItemEditCopy.Enabled = false;
			this.MenuItemEditCopy.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemEditCopy.Image")));
			this.MenuItemEditCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemEditCopy.Name = "MenuItemEditCopy";
			this.MenuItemEditCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
			this.MenuItemEditCopy.Size = new System.Drawing.Size (165, 24);
			this.MenuItemEditCopy.Text = "&Copy";
			this.MenuItemEditCopy.Click += new System.EventHandler (this.MenuItemEditCopy_Click);
			// 
			// MenuItemEditPaste
			// 
			this.MenuItemEditPaste.Enabled = false;
			this.MenuItemEditPaste.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemEditPaste.Image")));
			this.MenuItemEditPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemEditPaste.Name = "MenuItemEditPaste";
			this.MenuItemEditPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
			this.MenuItemEditPaste.Size = new System.Drawing.Size (165, 24);
			this.MenuItemEditPaste.Text = "&Paste";
			this.MenuItemEditPaste.Click += new System.EventHandler (this.MenuItemEditPaste_Click);
			// 
			// MenuItemEditDelete
			// 
			this.MenuItemEditDelete.Enabled = false;
			this.MenuItemEditDelete.Image = ((System.Drawing.Image)(resources.GetObject ("MenuItemEditDelete.Image")));
			this.MenuItemEditDelete.Name = "MenuItemEditDelete";
			this.MenuItemEditDelete.ShortcutKeyDisplayString = "";
			this.MenuItemEditDelete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
			this.MenuItemEditDelete.Size = new System.Drawing.Size (165, 24);
			this.MenuItemEditDelete.Text = "&Delete";
			this.MenuItemEditDelete.Click += new System.EventHandler (this.MenuItemEditDelete_Click);
			// 
			// MenuItemHelp
			// 
			this.MenuItemHelp.DropDownItems.AddRange (new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemHelpContents,
            this.MenuItemHelpIndex,
            this.MenuItemHelpSearch,
            this.MenuSepHelp1,
            this.MenuItemHelpAbout});
			this.MenuItemHelp.Name = "MenuItemHelp";
			this.MenuItemHelp.Size = new System.Drawing.Size (53, 24);
			this.MenuItemHelp.Text = "&Help";
			// 
			// MenuItemHelpContents
			// 
			this.MenuItemHelpContents.Enabled = false;
			this.MenuItemHelpContents.Name = "MenuItemHelpContents";
			this.MenuItemHelpContents.Size = new System.Drawing.Size (136, 24);
			this.MenuItemHelpContents.Text = "&Contents";
			// 
			// MenuItemHelpIndex
			// 
			this.MenuItemHelpIndex.Enabled = false;
			this.MenuItemHelpIndex.Name = "MenuItemHelpIndex";
			this.MenuItemHelpIndex.Size = new System.Drawing.Size (136, 24);
			this.MenuItemHelpIndex.Text = "&Index";
			// 
			// MenuItemHelpSearch
			// 
			this.MenuItemHelpSearch.Enabled = false;
			this.MenuItemHelpSearch.Name = "MenuItemHelpSearch";
			this.MenuItemHelpSearch.Size = new System.Drawing.Size (136, 24);
			this.MenuItemHelpSearch.Text = "&Search";
			// 
			// MenuSepHelp1
			// 
			this.MenuSepHelp1.Name = "MenuSepHelp1";
			this.MenuSepHelp1.Size = new System.Drawing.Size (133, 6);
			// 
			// MenuItemHelpAbout
			// 
			this.MenuItemHelpAbout.Name = "MenuItemHelpAbout";
			this.MenuItemHelpAbout.Size = new System.Drawing.Size (136, 24);
			this.MenuItemHelpAbout.Text = "&About...";
			this.MenuItemHelpAbout.Click += new System.EventHandler (this.MenuItemHelpAbout_Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF (8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
			this.ClientSize = new System.Drawing.Size (982, 555);
			this.Controls.Add (this.MenuStripMain);
			this.Controls.Add (this.ToolStripContainerMain);
			this.Font = new System.Drawing.Font ("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Icon = ((System.Drawing.Icon)(resources.GetObject ("$this.Icon")));
			this.MinimumSize = new System.Drawing.Size (600, 400);
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultBounds;
			this.Text = "Agent Character Editor";
			this.Load += new System.EventHandler (this.MainForm_Load);
			this.Shown += new System.EventHandler (this.MainForm_Shown);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.MainForm_FormClosing);
			this.ToolStripContainerMain.ContentPanel.ResumeLayout (false);
			this.ToolStripContainerMain.TopToolStripPanel.ResumeLayout (false);
			this.ToolStripContainerMain.TopToolStripPanel.PerformLayout ();
			this.ToolStripContainerMain.ResumeLayout (false);
			this.ToolStripContainerMain.PerformLayout ();
			this.SplitContainerMain.Panel1.ResumeLayout (false);
			this.SplitContainerMain.Panel2.ResumeLayout (false);
			this.SplitContainerMain.ResumeLayout (false);
			this.ToolStripTop.ResumeLayout (false);
			this.ToolStripTop.PerformLayout ();
			this.MenuStripMain.ResumeLayout (false);
			this.MenuStripMain.PerformLayout ();
			this.ResumeLayout (false);
			this.PerformLayout ();

		}

		#endregion

		private System.Windows.Forms.ToolStripContainer ToolStripContainerMain;
		private System.Windows.Forms.ToolStrip ToolStripTop;
		private System.Windows.Forms.ToolStripButton ToolButtonFileNew;
		private System.Windows.Forms.ToolStripButton ToolButtonFileOpen;
		private System.Windows.Forms.ToolStripButton ToolButtonFileSave;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator1;
		private System.Windows.Forms.ToolStripButton ToolButtonEditCut;
		private System.Windows.Forms.ToolStripButton ToolButtonEditCopy;
		private System.Windows.Forms.ToolStripButton ToolButtonEditPaste;
		private System.Windows.Forms.ToolStripButton ToolButtonEditDelete;
		private System.Windows.Forms.ToolStripButton ToolButtonEditUndo;
		private System.Windows.Forms.ToolStripButton ToolButtonEditRedo;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator2;
		private System.Windows.Forms.SplitContainer SplitContainerMain;
		private System.Windows.Forms.MenuStrip MenuStripMain;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFile;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileNew;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileOpen;
		private System.Windows.Forms.ToolStripSeparator MenuSepFile1;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileSave;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileSaveAs;
		private System.Windows.Forms.ToolStripSeparator MenuSepFile2;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFilePrint;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFilePrintPreview;
		private System.Windows.Forms.ToolStripSeparator MenuSepFile3;
		private System.Windows.Forms.ToolStripMenuItem MenuItemExit;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEdit;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditUndo;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditRedo;
		private System.Windows.Forms.ToolStripSeparator MenuSepEdit1;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditCut;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditCopy;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditPaste;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditDelete;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelp;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelpContents;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelpIndex;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelpSearch;
		private System.Windows.Forms.ToolStripSeparator MenuSepHelp1;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelpAbout;
		private System.Windows.Forms.TreeView TreeViewMain;
		private AgentCharacterEditor.CharacterForm PanelCharacter;
		private AgentCharacterEditor.BalloonForm PanelBalloon;
		private AgentCharacterEditor.TtsForm PanelTts;
		private AgentCharacterEditor.AnimationsForm PanelAnimations;
		private AgentCharacterEditor.AnimationForm PanelAnimation;
		private AgentCharacterEditor.StateForm PanelState;
		private AgentCharacterEditor.FrameForm PanelFrame;
		private AgentCharacterEditor.BranchingForm PanelBranching;
		private AgentCharacterEditor.OverlayForm PanelOverlays;
	}
}

