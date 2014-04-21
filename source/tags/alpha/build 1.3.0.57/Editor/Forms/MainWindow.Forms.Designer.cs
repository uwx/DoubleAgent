﻿/////////////////////////////////////////////////////////////////////////////
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
namespace AgentCharacterEditor
{
	partial class MainWindow
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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.ToolStripSeparator MenuSepFile1;
			System.Windows.Forms.ToolStripSeparator MenuSepFile2;
			System.Windows.Forms.ToolStripSeparator MenuSepFile3;
			System.Windows.Forms.ToolStripSeparator MenuSepEdit1;
			System.Windows.Forms.ToolStripSeparator ToolStripSeparator1;
			System.Windows.Forms.ToolStripSeparator ToolStripSeparator2;
			this.ToolStripContainerMain = new System.Windows.Forms.ToolStripContainer();
			this.SplitContainerMain = new System.Windows.Forms.SplitContainer();
			this.PanelPartsTree = new AgentCharacterEditor.Panels.PartsTreePanel();
			this.PanelBalloon = new AgentCharacterEditor.Panels.BalloonPanel();
			this.PanelCharacter = new AgentCharacterEditor.Panels.CharacterPanel();
			this.PanelTts = new AgentCharacterEditor.Panels.TtsPanel();
			this.PanelAnimations = new AgentCharacterEditor.Panels.AnimationsPanel();
			this.PanelAnimation = new AgentCharacterEditor.Panels.AnimationPanel();
			this.PanelState = new AgentCharacterEditor.Panels.StatePanel();
			this.PanelFrame = new AgentCharacterEditor.Panels.FramePanel();
			this.PanelBranching = new AgentCharacterEditor.Panels.BranchingPanel();
			this.PanelOverlays = new AgentCharacterEditor.Panels.OverlayPanel();
			this.ToolStripNavigation = new DoubleAgent.ToolStripEx();
			this.ToolButtonNavigateBack = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonNavigateForward = new DoubleAgent.ToolStripButtonCompat();
			this.MenuStripMain = new System.Windows.Forms.MenuStrip();
			this.MenuItemFile = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemFileNew = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemFileOpen = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemFileSave = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemFileSaveAs = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemFilePrint = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemFilePrintPreview = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemExit = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEdit = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEditUndo = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEditRedo = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEditCut = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEditCopy = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEditPaste = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemEditDelete = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemView = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemNavigateBack = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemNavigateForward = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemHelp = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuItemHelpAbout = new System.Windows.Forms.ToolStripMenuItem();
			this.ContextMenuEdit = new Global.ContextMenuEdit(this.components);
			this.ToolStripMain = new DoubleAgent.ToolStripEx();
			this.ToolButtonFileNew = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonFileOpen = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonFileSave = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonEditUndo = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonEditRedo = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonEditCut = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonEditCopy = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonEditPaste = new DoubleAgent.ToolStripButtonCompat();
			this.ToolButtonEditDelete = new DoubleAgent.ToolStripButtonCompat();
			MenuSepFile1 = new System.Windows.Forms.ToolStripSeparator();
			MenuSepFile2 = new System.Windows.Forms.ToolStripSeparator();
			MenuSepFile3 = new System.Windows.Forms.ToolStripSeparator();
			MenuSepEdit1 = new System.Windows.Forms.ToolStripSeparator();
			ToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			ToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.ToolStripContainerMain.ContentPanel.SuspendLayout();
			this.ToolStripContainerMain.TopToolStripPanel.SuspendLayout();
			this.ToolStripContainerMain.SuspendLayout();
			this.SplitContainerMain.Panel1.SuspendLayout();
			this.SplitContainerMain.Panel2.SuspendLayout();
			this.SplitContainerMain.SuspendLayout();
			this.ToolStripNavigation.SuspendLayout();
			this.MenuStripMain.SuspendLayout();
			this.ToolStripMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// MenuSepFile1
			// 
			MenuSepFile1.Name = "MenuSepFile1";
			MenuSepFile1.Size = new System.Drawing.Size(193, 6);
			// 
			// MenuSepFile2
			// 
			MenuSepFile2.Name = "MenuSepFile2";
			MenuSepFile2.Size = new System.Drawing.Size(193, 6);
			// 
			// MenuSepFile3
			// 
			MenuSepFile3.Name = "MenuSepFile3";
			MenuSepFile3.Size = new System.Drawing.Size(193, 6);
			// 
			// MenuSepEdit1
			// 
			MenuSepEdit1.Name = "MenuSepEdit1";
			MenuSepEdit1.Size = new System.Drawing.Size(178, 6);
			// 
			// ToolStripSeparator1
			// 
			ToolStripSeparator1.Name = "ToolStripSeparator1";
			ToolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// ToolStripSeparator2
			// 
			ToolStripSeparator2.Name = "ToolStripSeparator2";
			ToolStripSeparator2.Size = new System.Drawing.Size(6, 27);
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
			this.ToolStripContainerMain.ContentPanel.Controls.Add(this.SplitContainerMain);
			this.ToolStripContainerMain.ContentPanel.Margin = new System.Windows.Forms.Padding(0);
			this.ToolStripContainerMain.ContentPanel.Padding = new System.Windows.Forms.Padding(8);
			this.ToolStripContainerMain.ContentPanel.Size = new System.Drawing.Size(982, 496);
			this.ToolStripContainerMain.LeftToolStripPanelVisible = false;
			this.ToolStripContainerMain.Location = new System.Drawing.Point(0, 28);
			this.ToolStripContainerMain.Margin = new System.Windows.Forms.Padding(0);
			this.ToolStripContainerMain.Name = "ToolStripContainerMain";
			this.ToolStripContainerMain.RightToolStripPanelVisible = false;
			this.ToolStripContainerMain.Size = new System.Drawing.Size(982, 527);
			this.ToolStripContainerMain.TabIndex = 0;
			// 
			// ToolStripContainerMain.TopToolStripPanel
			// 
			this.ToolStripContainerMain.TopToolStripPanel.BackColor = System.Drawing.SystemColors.Control;
			this.ToolStripContainerMain.TopToolStripPanel.Controls.Add(this.ToolStripNavigation);
			// 
			// SplitContainerMain
			// 
			this.SplitContainerMain.CausesValidation = false;
			this.SplitContainerMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitContainerMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.SplitContainerMain.Location = new System.Drawing.Point(8, 8);
			this.SplitContainerMain.Margin = new System.Windows.Forms.Padding(0);
			this.SplitContainerMain.Name = "SplitContainerMain";
			// 
			// SplitContainerMain.Panel1
			// 
			this.SplitContainerMain.Panel1.Controls.Add(this.PanelPartsTree);
			this.SplitContainerMain.Panel1MinSize = 100;
			// 
			// SplitContainerMain.Panel2
			// 
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelBalloon);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelCharacter);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelTts);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelAnimations);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelAnimation);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelState);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelFrame);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelBranching);
			this.SplitContainerMain.Panel2.Controls.Add(this.PanelOverlays);
			this.SplitContainerMain.Panel2MinSize = 100;
			this.SplitContainerMain.Size = new System.Drawing.Size(966, 480);
			this.SplitContainerMain.SplitterDistance = 200;
			this.SplitContainerMain.SplitterWidth = 8;
			this.SplitContainerMain.TabIndex = 0;
			this.SplitContainerMain.TabStop = false;
			// 
			// PanelPartsTree
			// 
			this.PanelPartsTree.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.PanelPartsTree.Location = new System.Drawing.Point(0, 0);
			this.PanelPartsTree.Margin = new System.Windows.Forms.Padding(0);
			this.PanelPartsTree.Name = "PanelPartsTree";
			this.PanelPartsTree.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelPartsTree.Size = new System.Drawing.Size(200, 479);
			this.PanelPartsTree.TabIndex = 0;
			// 
			// PanelBalloon
			// 
			this.PanelBalloon.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelBalloon.Location = new System.Drawing.Point(0, 0);
			this.PanelBalloon.Margin = new System.Windows.Forms.Padding(0);
			this.PanelBalloon.Name = "PanelBalloon";
			this.PanelBalloon.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelBalloon.Size = new System.Drawing.Size(758, 480);
			this.PanelBalloon.TabIndex = 0;
			this.PanelBalloon.Visible = false;
			// 
			// PanelCharacter
			// 
			this.PanelCharacter.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelCharacter.Location = new System.Drawing.Point(0, 0);
			this.PanelCharacter.Margin = new System.Windows.Forms.Padding(0);
			this.PanelCharacter.Name = "PanelCharacter";
			this.PanelCharacter.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelCharacter.Size = new System.Drawing.Size(758, 480);
			this.PanelCharacter.TabIndex = 0;
			// 
			// PanelTts
			// 
			this.PanelTts.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelTts.Location = new System.Drawing.Point(0, 0);
			this.PanelTts.Margin = new System.Windows.Forms.Padding(0);
			this.PanelTts.Name = "PanelTts";
			this.PanelTts.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelTts.Size = new System.Drawing.Size(758, 480);
			this.PanelTts.TabIndex = 0;
			this.PanelTts.Visible = false;
			// 
			// PanelAnimations
			// 
			this.PanelAnimations.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelAnimations.Location = new System.Drawing.Point(0, 0);
			this.PanelAnimations.Margin = new System.Windows.Forms.Padding(0);
			this.PanelAnimations.Name = "PanelAnimations";
			this.PanelAnimations.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelAnimations.Size = new System.Drawing.Size(758, 480);
			this.PanelAnimations.TabIndex = 0;
			this.PanelAnimations.Visible = false;
			// 
			// PanelAnimation
			// 
			this.PanelAnimation.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelAnimation.Location = new System.Drawing.Point(0, 0);
			this.PanelAnimation.Margin = new System.Windows.Forms.Padding(0);
			this.PanelAnimation.Name = "PanelAnimation";
			this.PanelAnimation.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelAnimation.Size = new System.Drawing.Size(758, 480);
			this.PanelAnimation.TabIndex = 0;
			this.PanelAnimation.Visible = false;
			this.PanelAnimation.Navigate += new AgentCharacterEditor.Navigation.NavigationEventHandler(this.OnNavigate);
			// 
			// PanelState
			// 
			this.PanelState.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelState.Location = new System.Drawing.Point(0, 0);
			this.PanelState.Margin = new System.Windows.Forms.Padding(0);
			this.PanelState.Name = "PanelState";
			this.PanelState.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelState.Size = new System.Drawing.Size(758, 480);
			this.PanelState.TabIndex = 0;
			this.PanelState.Visible = false;
			// 
			// PanelFrame
			// 
			this.PanelFrame.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelFrame.Location = new System.Drawing.Point(0, 0);
			this.PanelFrame.Margin = new System.Windows.Forms.Padding(0);
			this.PanelFrame.Name = "PanelFrame";
			this.PanelFrame.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelFrame.Size = new System.Drawing.Size(758, 480);
			this.PanelFrame.TabIndex = 0;
			this.PanelFrame.Visible = false;
			// 
			// PanelBranching
			// 
			this.PanelBranching.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelBranching.Location = new System.Drawing.Point(0, 0);
			this.PanelBranching.Margin = new System.Windows.Forms.Padding(0);
			this.PanelBranching.Name = "PanelBranching";
			this.PanelBranching.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelBranching.Size = new System.Drawing.Size(758, 480);
			this.PanelBranching.TabIndex = 0;
			this.PanelBranching.Visible = false;
			// 
			// PanelOverlays
			// 
			this.PanelOverlays.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PanelOverlays.Location = new System.Drawing.Point(0, 0);
			this.PanelOverlays.Margin = new System.Windows.Forms.Padding(0);
			this.PanelOverlays.Name = "PanelOverlays";
			this.PanelOverlays.Padding = new System.Windows.Forms.Padding(9, 0, 9, 0);
			this.PanelOverlays.Size = new System.Drawing.Size(758, 480);
			this.PanelOverlays.TabIndex = 0;
			this.PanelOverlays.Visible = false;
			// 
			// ToolStripNavigation
			// 
			this.ToolStripNavigation.CanOverflow = false;
			this.ToolStripNavigation.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripNavigation.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripNavigation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolButtonNavigateBack,
            this.ToolButtonNavigateForward});
			this.ToolStripNavigation.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripNavigation.Location = new System.Drawing.Point(3, 0);
			this.ToolStripNavigation.Name = "ToolStripNavigation";
			this.ToolStripNavigation.Padding = new System.Windows.Forms.Padding(8, 2, 8, 2);
			this.ToolStripNavigation.Size = new System.Drawing.Size(66, 31);
			this.ToolStripNavigation.TabIndex = 0;
			// 
			// ToolButtonNavigateBack
			// 
			this.ToolButtonNavigateBack.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonNavigateBack.Image = global::AgentCharacterEditor.Properties.Resources.ImgNavigateBack;
			this.ToolButtonNavigateBack.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonNavigateBack.Name = "ToolButtonNavigateBack";
			this.ToolButtonNavigateBack.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonNavigateBack.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonNavigateBack.Text = "Back";
			this.ToolButtonNavigateBack.Click += new System.EventHandler(this.ToolButtonNavigateBack_Click);
			// 
			// ToolButtonNavigateForward
			// 
			this.ToolButtonNavigateForward.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonNavigateForward.Image = global::AgentCharacterEditor.Properties.Resources.ImgNavigateForward;
			this.ToolButtonNavigateForward.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonNavigateForward.Name = "ToolButtonNavigateForward";
			this.ToolButtonNavigateForward.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonNavigateForward.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonNavigateForward.Text = "Forward";
			this.ToolButtonNavigateForward.Click += new System.EventHandler(this.ToolButtonNavigateForward_Click);
			// 
			// MenuStripMain
			// 
			this.MenuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemFile,
            this.MenuItemEdit,
            this.MenuItemView,
            this.MenuItemHelp});
			this.MenuStripMain.Location = new System.Drawing.Point(0, 0);
			this.MenuStripMain.Name = "MenuStripMain";
			this.MenuStripMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
			this.MenuStripMain.Size = new System.Drawing.Size(982, 31);
			this.MenuStripMain.TabIndex = 1;
			this.MenuStripMain.Text = "MenuStripMain";
			// 
			// MenuItemFile
			// 
			this.MenuItemFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemFileNew,
            this.MenuItemFileOpen,
            MenuSepFile1,
            this.MenuItemFileSave,
            this.MenuItemFileSaveAs,
            MenuSepFile2,
            this.MenuItemFilePrint,
            this.MenuItemFilePrintPreview,
            MenuSepFile3,
            this.MenuItemExit});
			this.MenuItemFile.Name = "MenuItemFile";
			this.MenuItemFile.Size = new System.Drawing.Size(47, 27);
			this.MenuItemFile.Text = "&File";
			// 
			// MenuItemFileNew
			// 
			this.MenuItemFileNew.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.MenuItemFileNew.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFileNew.Name = "MenuItemFileNew";
			this.MenuItemFileNew.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
			this.MenuItemFileNew.Size = new System.Drawing.Size(196, 28);
			this.MenuItemFileNew.Text = "&New";
			this.MenuItemFileNew.Click += new System.EventHandler(this.MenuItemFileNew_Click);
			// 
			// MenuItemFileOpen
			// 
			this.MenuItemFileOpen.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileOpen;
			this.MenuItemFileOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFileOpen.Name = "MenuItemFileOpen";
			this.MenuItemFileOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
			this.MenuItemFileOpen.Size = new System.Drawing.Size(196, 28);
			this.MenuItemFileOpen.Text = "&Open...";
			this.MenuItemFileOpen.Click += new System.EventHandler(this.MenuItemFileOpen_Click);
			// 
			// MenuItemFileSave
			// 
			this.MenuItemFileSave.Enabled = false;
			this.MenuItemFileSave.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileSave;
			this.MenuItemFileSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFileSave.Name = "MenuItemFileSave";
			this.MenuItemFileSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.MenuItemFileSave.Size = new System.Drawing.Size(196, 28);
			this.MenuItemFileSave.Text = "&Save";
			this.MenuItemFileSave.Click += new System.EventHandler(this.MenuItemFileSave_Click);
			// 
			// MenuItemFileSaveAs
			// 
			this.MenuItemFileSaveAs.Enabled = false;
			this.MenuItemFileSaveAs.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileSave;
			this.MenuItemFileSaveAs.Name = "MenuItemFileSaveAs";
			this.MenuItemFileSaveAs.Size = new System.Drawing.Size(196, 28);
			this.MenuItemFileSaveAs.Text = "Save &As...";
			this.MenuItemFileSaveAs.Click += new System.EventHandler(this.MenuItemFileSaveAs_Click);
			// 
			// MenuItemFilePrint
			// 
			this.MenuItemFilePrint.Enabled = false;
			this.MenuItemFilePrint.Image = global::AgentCharacterEditor.Properties.Resources.ImgFilePrint;
			this.MenuItemFilePrint.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFilePrint.Name = "MenuItemFilePrint";
			this.MenuItemFilePrint.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
			this.MenuItemFilePrint.Size = new System.Drawing.Size(196, 28);
			this.MenuItemFilePrint.Text = "&Print";
			this.MenuItemFilePrint.Click += new System.EventHandler(this.MenuItemFilePrint_Click);
			// 
			// MenuItemFilePrintPreview
			// 
			this.MenuItemFilePrintPreview.Enabled = false;
			this.MenuItemFilePrintPreview.Image = global::AgentCharacterEditor.Properties.Resources.ImgFilePrintPreview;
			this.MenuItemFilePrintPreview.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemFilePrintPreview.Name = "MenuItemFilePrintPreview";
			this.MenuItemFilePrintPreview.Size = new System.Drawing.Size(196, 28);
			this.MenuItemFilePrintPreview.Text = "Print Pre&view";
			this.MenuItemFilePrintPreview.Click += new System.EventHandler(this.MenuItemFilePrintPreview_Click);
			// 
			// MenuItemExit
			// 
			this.MenuItemExit.Name = "MenuItemExit";
			this.MenuItemExit.Size = new System.Drawing.Size(196, 28);
			this.MenuItemExit.Text = "E&xit";
			this.MenuItemExit.Click += new System.EventHandler(this.MenuItemExit_Click);
			// 
			// MenuItemEdit
			// 
			this.MenuItemEdit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemEditUndo,
            this.MenuItemEditRedo,
            MenuSepEdit1,
            this.MenuItemEditCut,
            this.MenuItemEditCopy,
            this.MenuItemEditPaste,
            this.MenuItemEditDelete});
			this.MenuItemEdit.Name = "MenuItemEdit";
			this.MenuItemEdit.Size = new System.Drawing.Size(51, 27);
			this.MenuItemEdit.Text = "&Edit";
			// 
			// MenuItemEditUndo
			// 
			this.MenuItemEditUndo.Enabled = false;
			this.MenuItemEditUndo.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditUndo;
			this.MenuItemEditUndo.Name = "MenuItemEditUndo";
			this.MenuItemEditUndo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
			this.MenuItemEditUndo.Size = new System.Drawing.Size(181, 28);
			this.MenuItemEditUndo.Text = "&Undo";
			this.MenuItemEditUndo.Click += new System.EventHandler(this.MenuItemEditUndo_Click);
			// 
			// MenuItemEditRedo
			// 
			this.MenuItemEditRedo.Enabled = false;
			this.MenuItemEditRedo.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditRedo;
			this.MenuItemEditRedo.Name = "MenuItemEditRedo";
			this.MenuItemEditRedo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
			this.MenuItemEditRedo.Size = new System.Drawing.Size(181, 28);
			this.MenuItemEditRedo.Text = "&Redo";
			this.MenuItemEditRedo.Click += new System.EventHandler(this.MenuItemEditRedo_Click);
			// 
			// MenuItemEditCut
			// 
			this.MenuItemEditCut.Enabled = false;
			this.MenuItemEditCut.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditCut;
			this.MenuItemEditCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemEditCut.Name = "MenuItemEditCut";
			this.MenuItemEditCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
			this.MenuItemEditCut.Size = new System.Drawing.Size(181, 28);
			this.MenuItemEditCut.Text = "Cu&t";
			this.MenuItemEditCut.Click += new System.EventHandler(this.MenuItemEditCut_Click);
			// 
			// MenuItemEditCopy
			// 
			this.MenuItemEditCopy.Enabled = false;
			this.MenuItemEditCopy.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditCopy;
			this.MenuItemEditCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemEditCopy.Name = "MenuItemEditCopy";
			this.MenuItemEditCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
			this.MenuItemEditCopy.Size = new System.Drawing.Size(181, 28);
			this.MenuItemEditCopy.Text = "&Copy";
			this.MenuItemEditCopy.Click += new System.EventHandler(this.MenuItemEditCopy_Click);
			// 
			// MenuItemEditPaste
			// 
			this.MenuItemEditPaste.Enabled = false;
			this.MenuItemEditPaste.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditPaste;
			this.MenuItemEditPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.MenuItemEditPaste.Name = "MenuItemEditPaste";
			this.MenuItemEditPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
			this.MenuItemEditPaste.Size = new System.Drawing.Size(181, 28);
			this.MenuItemEditPaste.Text = "&Paste";
			this.MenuItemEditPaste.Click += new System.EventHandler(this.MenuItemEditPaste_Click);
			// 
			// MenuItemEditDelete
			// 
			this.MenuItemEditDelete.Enabled = false;
			this.MenuItemEditDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.MenuItemEditDelete.Name = "MenuItemEditDelete";
			this.MenuItemEditDelete.ShortcutKeyDisplayString = "";
			this.MenuItemEditDelete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
			this.MenuItemEditDelete.Size = new System.Drawing.Size(181, 28);
			this.MenuItemEditDelete.Text = "&Delete";
			this.MenuItemEditDelete.Click += new System.EventHandler(this.MenuItemEditDelete_Click);
			// 
			// MenuItemView
			// 
			this.MenuItemView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemNavigateBack,
            this.MenuItemNavigateForward});
			this.MenuItemView.Name = "MenuItemView";
			this.MenuItemView.Size = new System.Drawing.Size(58, 27);
			this.MenuItemView.Text = "&View";
			// 
			// MenuItemNavigateBack
			// 
			this.MenuItemNavigateBack.Image = global::AgentCharacterEditor.Properties.Resources.ImgNavigateBack;
			this.MenuItemNavigateBack.Name = "MenuItemNavigateBack";
			this.MenuItemNavigateBack.Size = new System.Drawing.Size(165, 28);
			this.MenuItemNavigateBack.Text = "Go &back";
			this.MenuItemNavigateBack.Click += new System.EventHandler(this.MenuItemNavigateBack_Click);
			// 
			// MenuItemNavigateForward
			// 
			this.MenuItemNavigateForward.Image = global::AgentCharacterEditor.Properties.Resources.ImgNavigateForward;
			this.MenuItemNavigateForward.Name = "MenuItemNavigateForward";
			this.MenuItemNavigateForward.Size = new System.Drawing.Size(165, 28);
			this.MenuItemNavigateForward.Text = "Go &forward";
			this.MenuItemNavigateForward.Click += new System.EventHandler(this.MenuItemNavigateForward_Click);
			// 
			// MenuItemHelp
			// 
			this.MenuItemHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuItemHelpAbout});
			this.MenuItemHelp.Name = "MenuItemHelp";
			this.MenuItemHelp.Size = new System.Drawing.Size(57, 27);
			this.MenuItemHelp.Text = "&Help";
			// 
			// MenuItemHelpAbout
			// 
			this.MenuItemHelpAbout.Image = global::AgentCharacterEditor.Properties.Resources.ImgHelpAbout;
			this.MenuItemHelpAbout.Name = "MenuItemHelpAbout";
			this.MenuItemHelpAbout.Size = new System.Drawing.Size(139, 28);
			this.MenuItemHelpAbout.Text = "&About...";
			this.MenuItemHelpAbout.Click += new System.EventHandler(this.MenuItemHelpAbout_Click);
			// 
			// ContextMenuEdit
			// 
			this.ContextMenuEdit.Name = "ContextMenuEdit";
			this.ContextMenuEdit.Size = new System.Drawing.Size(61, 4);
			this.ContextMenuEdit.Opening += new System.ComponentModel.CancelEventHandler(this.ContextMenuStub_Opening);
			// 
			// ToolStripMain
			// 
			this.ToolStripMain.AllowMerge = false;
			this.ToolStripMain.CanOverflow = false;
			this.ToolStripMain.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolStripMain.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolButtonFileNew,
            this.ToolButtonFileOpen,
            this.ToolButtonFileSave,
            ToolStripSeparator1,
            this.ToolButtonEditUndo,
            this.ToolButtonEditRedo,
            ToolStripSeparator2,
            this.ToolButtonEditCut,
            this.ToolButtonEditCopy,
            this.ToolButtonEditPaste,
            this.ToolButtonEditDelete});
			this.ToolStripMain.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
			this.ToolStripMain.Location = new System.Drawing.Point(70, 28);
			this.ToolStripMain.Name = "ToolStripMain";
			this.ToolStripMain.Padding = new System.Windows.Forms.Padding(8, 2, 8, 2);
			this.ToolStripMain.Size = new System.Drawing.Size(246, 31);
			this.ToolStripMain.TabIndex = 1;
			// 
			// ToolButtonFileNew
			// 
			this.ToolButtonFileNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonFileNew.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileNew;
			this.ToolButtonFileNew.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonFileNew.Name = "ToolButtonFileNew";
			this.ToolButtonFileNew.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonFileNew.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonFileNew.Text = "New";
			this.ToolButtonFileNew.Click += new System.EventHandler(this.ToolButtonFileNew_Click);
			// 
			// ToolButtonFileOpen
			// 
			this.ToolButtonFileOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonFileOpen.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileOpen;
			this.ToolButtonFileOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonFileOpen.Name = "ToolButtonFileOpen";
			this.ToolButtonFileOpen.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonFileOpen.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonFileOpen.Text = "Open";
			this.ToolButtonFileOpen.Click += new System.EventHandler(this.ToolButtonFileOpen_Click);
			// 
			// ToolButtonFileSave
			// 
			this.ToolButtonFileSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonFileSave.Enabled = false;
			this.ToolButtonFileSave.Image = global::AgentCharacterEditor.Properties.Resources.ImgFileSave;
			this.ToolButtonFileSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonFileSave.Name = "ToolButtonFileSave";
			this.ToolButtonFileSave.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonFileSave.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonFileSave.Text = "Save";
			this.ToolButtonFileSave.Click += new System.EventHandler(this.ToolButtonFileSave_Click);
			// 
			// ToolButtonEditUndo
			// 
			this.ToolButtonEditUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditUndo.Enabled = false;
			this.ToolButtonEditUndo.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditUndo;
			this.ToolButtonEditUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditUndo.Name = "ToolButtonEditUndo";
			this.ToolButtonEditUndo.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonEditUndo.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonEditUndo.Text = "Undo";
			this.ToolButtonEditUndo.Click += new System.EventHandler(this.ToolButtonEditUndo_Click);
			// 
			// ToolButtonEditRedo
			// 
			this.ToolButtonEditRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditRedo.Enabled = false;
			this.ToolButtonEditRedo.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditRedo;
			this.ToolButtonEditRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditRedo.Name = "ToolButtonEditRedo";
			this.ToolButtonEditRedo.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonEditRedo.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonEditRedo.Text = "Redo";
			this.ToolButtonEditRedo.Click += new System.EventHandler(this.ToolButtonEditRedo_Click);
			// 
			// ToolButtonEditCut
			// 
			this.ToolButtonEditCut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditCut.Enabled = false;
			this.ToolButtonEditCut.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditCut;
			this.ToolButtonEditCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditCut.Name = "ToolButtonEditCut";
			this.ToolButtonEditCut.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonEditCut.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonEditCut.Text = "Cut";
			this.ToolButtonEditCut.Click += new System.EventHandler(this.ToolButtonEditCut_Click);
			// 
			// ToolButtonEditCopy
			// 
			this.ToolButtonEditCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditCopy.Enabled = false;
			this.ToolButtonEditCopy.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditCopy;
			this.ToolButtonEditCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditCopy.Name = "ToolButtonEditCopy";
			this.ToolButtonEditCopy.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonEditCopy.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonEditCopy.Text = "Copy";
			this.ToolButtonEditCopy.Click += new System.EventHandler(this.ToolButtonEditCopy_Click);
			// 
			// ToolButtonEditPaste
			// 
			this.ToolButtonEditPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditPaste.Enabled = false;
			this.ToolButtonEditPaste.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditPaste;
			this.ToolButtonEditPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditPaste.Name = "ToolButtonEditPaste";
			this.ToolButtonEditPaste.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonEditPaste.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonEditPaste.Text = "Paste";
			this.ToolButtonEditPaste.Click += new System.EventHandler(this.ToolButtonEditPaste_Click);
			// 
			// ToolButtonEditDelete
			// 
			this.ToolButtonEditDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ToolButtonEditDelete.Enabled = false;
			this.ToolButtonEditDelete.Image = global::AgentCharacterEditor.Properties.Resources.ImgEditDelete;
			this.ToolButtonEditDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ToolButtonEditDelete.Name = "ToolButtonEditDelete";
			this.ToolButtonEditDelete.Padding = new System.Windows.Forms.Padding(2);
			this.ToolButtonEditDelete.Size = new System.Drawing.Size(24, 24);
			this.ToolButtonEditDelete.Text = "Delete";
			this.ToolButtonEditDelete.Click += new System.EventHandler(this.ToolButtonEditDelete_Click);
			// 
			// MainWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
			this.ClientSize = new System.Drawing.Size(982, 555);
			this.ContextMenuStrip = this.ContextMenuEdit;
			this.Controls.Add(this.MenuStripMain);
			this.Controls.Add(this.ToolStripMain);
			this.Controls.Add(this.ToolStripContainerMain);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.MinimumSize = new System.Drawing.Size(600, 400);
			this.Name = "MainWindow";
			this.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultBounds;
			this.Text = "Double Agent Character Editor";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.Shown += new System.EventHandler(this.MainForm_Shown);
			this.ToolStripContainerMain.ContentPanel.ResumeLayout(false);
			this.ToolStripContainerMain.TopToolStripPanel.ResumeLayout(false);
			this.ToolStripContainerMain.TopToolStripPanel.PerformLayout();
			this.ToolStripContainerMain.ResumeLayout(false);
			this.ToolStripContainerMain.PerformLayout();
			this.SplitContainerMain.Panel1.ResumeLayout(false);
			this.SplitContainerMain.Panel2.ResumeLayout(false);
			this.SplitContainerMain.ResumeLayout(false);
			this.ToolStripNavigation.ResumeLayout(false);
			this.ToolStripNavigation.PerformLayout();
			this.MenuStripMain.ResumeLayout(false);
			this.MenuStripMain.PerformLayout();
			this.ToolStripMain.ResumeLayout(false);
			this.ToolStripMain.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStripContainer ToolStripContainerMain;
		private DoubleAgent.ToolStripEx ToolStripMain;
		private DoubleAgent.ToolStripButtonCompat ToolButtonFileNew;
		private DoubleAgent.ToolStripButtonCompat ToolButtonFileOpen;
		private DoubleAgent.ToolStripButtonCompat ToolButtonFileSave;
		private DoubleAgent.ToolStripButtonCompat ToolButtonEditCut;
		private DoubleAgent.ToolStripButtonCompat ToolButtonEditCopy;
		private DoubleAgent.ToolStripButtonCompat ToolButtonEditPaste;
		private DoubleAgent.ToolStripButtonCompat ToolButtonEditDelete;
		private DoubleAgent.ToolStripButtonCompat ToolButtonEditUndo;
		private DoubleAgent.ToolStripButtonCompat ToolButtonEditRedo;
		private System.Windows.Forms.SplitContainer SplitContainerMain;
		private System.Windows.Forms.MenuStrip MenuStripMain;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFile;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileNew;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileOpen;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileSave;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFileSaveAs;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFilePrint;
		private System.Windows.Forms.ToolStripMenuItem MenuItemFilePrintPreview;
		private System.Windows.Forms.ToolStripMenuItem MenuItemExit;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEdit;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditUndo;
		private System.Windows.Forms.ToolStripMenuItem MenuItemEditRedo;
		public System.Windows.Forms.ToolStripMenuItem MenuItemEditCut;
		public System.Windows.Forms.ToolStripMenuItem MenuItemEditCopy;
		public System.Windows.Forms.ToolStripMenuItem MenuItemEditPaste;
		public System.Windows.Forms.ToolStripMenuItem MenuItemEditDelete;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelp;
		private System.Windows.Forms.ToolStripMenuItem MenuItemHelpAbout;
		public AgentCharacterEditor.Panels.CharacterPanel PanelCharacter;
		public AgentCharacterEditor.Panels.BalloonPanel PanelBalloon;
		public AgentCharacterEditor.Panels.TtsPanel PanelTts;
		public AgentCharacterEditor.Panels.AnimationsPanel PanelAnimations;
		public AgentCharacterEditor.Panels.AnimationPanel PanelAnimation;
		public AgentCharacterEditor.Panels.StatePanel PanelState;
		public AgentCharacterEditor.Panels.FramePanel PanelFrame;
		public AgentCharacterEditor.Panels.BranchingPanel PanelBranching;
		public AgentCharacterEditor.Panels.OverlayPanel PanelOverlays;
		public Global.ContextMenuEdit ContextMenuEdit;
		private AgentCharacterEditor.Panels.PartsTreePanel PanelPartsTree;
		private DoubleAgent.ToolStripEx ToolStripNavigation;
		private DoubleAgent.ToolStripButtonCompat ToolButtonNavigateBack;
		private DoubleAgent.ToolStripButtonCompat ToolButtonNavigateForward;
		private System.Windows.Forms.ToolStripMenuItem MenuItemView;
		private System.Windows.Forms.ToolStripMenuItem MenuItemNavigateBack;
		private System.Windows.Forms.ToolStripMenuItem MenuItemNavigateForward;
	}
}

