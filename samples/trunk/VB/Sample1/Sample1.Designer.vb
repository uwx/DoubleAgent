<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class Sample1
#Region "Windows Form Designer generated code "
	<System.Diagnostics.DebuggerNonUserCode()> Public Sub New()
		MyBase.New()
		'This call is required by the Windows Form Designer.
		InitializeComponent()
	End Sub
	'Form overrides dispose to clean up the component list.
	<System.Diagnostics.DebuggerNonUserCode()> Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
		If Disposing Then
            If Not components Is Nothing Then
            	components.Dispose()
            End If
		End If
		MyBase.Dispose(Disposing)
	End Sub
	'Required by the Windows Form Designer
	Private components As System.ComponentModel.IContainer
	Public ToolTip1 As System.Windows.Forms.ToolTip
	Public WithEvents ShowMsServerChars As System.Windows.Forms.Button
	Public WithEvents ShowMsServerOptions As System.Windows.Forms.Button
	Public WithEvents HideMsServerChar As System.Windows.Forms.Button
	Public WithEvents ShowMsServerChar As System.Windows.Forms.Button
	Public WithEvents UnloadMsServerChar As System.Windows.Forms.Button
	Public WithEvents LoadMsServerChar As System.Windows.Forms.Button
	Public WithEvents MsServerFrame As System.Windows.Forms.GroupBox
	Public WithEvents ShowMsControlChars As System.Windows.Forms.Button
	Public WithEvents ShowMsControlOptions As System.Windows.Forms.Button
	Public WithEvents HideMsControlChar As System.Windows.Forms.Button
	Public WithEvents ShowMsControlChar As System.Windows.Forms.Button
	Public WithEvents UnloadMsControlChar As System.Windows.Forms.Button
	Public WithEvents LoadMsControlChar As System.Windows.Forms.Button
	Public WithEvents MsControlFrame As System.Windows.Forms.GroupBox
	Public WithEvents ShowDaServerChars As System.Windows.Forms.Button
	Public WithEvents ShowDaServerOptions As System.Windows.Forms.Button
	Public WithEvents HideDaServerChar As System.Windows.Forms.Button
	Public WithEvents ShowDaServerChar As System.Windows.Forms.Button
	Public WithEvents UnloadDaServerChar As System.Windows.Forms.Button
	Public WithEvents LoadDaServerChar As System.Windows.Forms.Button
	Public WithEvents DaServerFrame As System.Windows.Forms.GroupBox
	Public WithEvents ShowDaControlChars As System.Windows.Forms.Button
	Public WithEvents ShowDaControlOptions As System.Windows.Forms.Button
	Public WithEvents HideDaControlChar As System.Windows.Forms.Button
	Public WithEvents ShowDaControlChar As System.Windows.Forms.Button
	Public WithEvents UnloadDaControlChar As System.Windows.Forms.Button
	Public WithEvents LoadDaControlChar As System.Windows.Forms.Button
	Public WithEvents DaControlFrame As System.Windows.Forms.GroupBox
	Public WithEvents mDaAgentControl As AxDoubleAgentCtl.AxDaControl
	Public WithEvents mMsAgentControl As AxAgentObjects.AxAgent
	'NOTE: The following procedure is required by the Windows Form Designer
	'It can be modified using the Windows Form Designer.
	'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Sample1))
        Me.ToolTip1 = New System.Windows.Forms.ToolTip(Me.components)
        Me.MsServerFrame = New System.Windows.Forms.GroupBox
        Me.ShowMsServerChars = New System.Windows.Forms.Button
        Me.ShowMsServerOptions = New System.Windows.Forms.Button
        Me.HideMsServerChar = New System.Windows.Forms.Button
        Me.ShowMsServerChar = New System.Windows.Forms.Button
        Me.UnloadMsServerChar = New System.Windows.Forms.Button
        Me.LoadMsServerChar = New System.Windows.Forms.Button
        Me.MsControlFrame = New System.Windows.Forms.GroupBox
        Me.ShowMsControlChars = New System.Windows.Forms.Button
        Me.ShowMsControlOptions = New System.Windows.Forms.Button
        Me.HideMsControlChar = New System.Windows.Forms.Button
        Me.ShowMsControlChar = New System.Windows.Forms.Button
        Me.UnloadMsControlChar = New System.Windows.Forms.Button
        Me.LoadMsControlChar = New System.Windows.Forms.Button
        Me.DaServerFrame = New System.Windows.Forms.GroupBox
        Me.ShowDaServerChars = New System.Windows.Forms.Button
        Me.ShowDaServerOptions = New System.Windows.Forms.Button
        Me.HideDaServerChar = New System.Windows.Forms.Button
        Me.ShowDaServerChar = New System.Windows.Forms.Button
        Me.UnloadDaServerChar = New System.Windows.Forms.Button
        Me.LoadDaServerChar = New System.Windows.Forms.Button
        Me.DaControlFrame = New System.Windows.Forms.GroupBox
        Me.ShowDaControlChars = New System.Windows.Forms.Button
        Me.ShowDaControlOptions = New System.Windows.Forms.Button
        Me.HideDaControlChar = New System.Windows.Forms.Button
        Me.ShowDaControlChar = New System.Windows.Forms.Button
        Me.UnloadDaControlChar = New System.Windows.Forms.Button
        Me.LoadDaControlChar = New System.Windows.Forms.Button
        Me.mDaAgentControl = New AxDoubleAgentCtl.AxDaControl
        Me.mMsAgentControl = New AxAgentObjects.AxAgent
        Me.MsServerFrame.SuspendLayout()
        Me.MsControlFrame.SuspendLayout()
        Me.DaServerFrame.SuspendLayout()
        Me.DaControlFrame.SuspendLayout()
        CType(Me.mDaAgentControl, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.mMsAgentControl, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'MsServerFrame
        '
        Me.MsServerFrame.BackColor = System.Drawing.SystemColors.Control
        Me.MsServerFrame.Controls.Add(Me.ShowMsServerChars)
        Me.MsServerFrame.Controls.Add(Me.ShowMsServerOptions)
        Me.MsServerFrame.Controls.Add(Me.HideMsServerChar)
        Me.MsServerFrame.Controls.Add(Me.ShowMsServerChar)
        Me.MsServerFrame.Controls.Add(Me.UnloadMsServerChar)
        Me.MsServerFrame.Controls.Add(Me.LoadMsServerChar)
        Me.MsServerFrame.ForeColor = System.Drawing.SystemColors.ControlText
        Me.MsServerFrame.Location = New System.Drawing.Point(8, 8)
        Me.MsServerFrame.Margin = New System.Windows.Forms.Padding(2)
        Me.MsServerFrame.Name = "MsServerFrame"
        Me.MsServerFrame.Padding = New System.Windows.Forms.Padding(0)
        Me.MsServerFrame.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.MsServerFrame.Size = New System.Drawing.Size(233, 139)
        Me.MsServerFrame.TabIndex = 0
        Me.MsServerFrame.TabStop = False
        Me.MsServerFrame.Text = "MS Agent Server"
        '
        'ShowMsServerChars
        '
        Me.ShowMsServerChars.BackColor = System.Drawing.SystemColors.Control
        Me.ShowMsServerChars.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowMsServerChars.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowMsServerChars.Location = New System.Drawing.Point(158, 81)
        Me.ShowMsServerChars.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowMsServerChars.Name = "ShowMsServerChars"
        Me.ShowMsServerChars.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowMsServerChars.Size = New System.Drawing.Size(68, 50)
        Me.ShowMsServerChars.TabIndex = 6
        Me.ShowMsServerChars.Text = "Show characters"
        Me.ShowMsServerChars.UseVisualStyleBackColor = False
        '
        'ShowMsServerOptions
        '
        Me.ShowMsServerOptions.BackColor = System.Drawing.SystemColors.Control
        Me.ShowMsServerOptions.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowMsServerOptions.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowMsServerOptions.Location = New System.Drawing.Point(158, 24)
        Me.ShowMsServerOptions.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowMsServerOptions.Name = "ShowMsServerOptions"
        Me.ShowMsServerOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowMsServerOptions.Size = New System.Drawing.Size(68, 50)
        Me.ShowMsServerOptions.TabIndex = 5
        Me.ShowMsServerOptions.Text = "Show options"
        Me.ShowMsServerOptions.UseVisualStyleBackColor = False
        '
        'HideMsServerChar
        '
        Me.HideMsServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.HideMsServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.HideMsServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.HideMsServerChar.Location = New System.Drawing.Point(82, 81)
        Me.HideMsServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.HideMsServerChar.Name = "HideMsServerChar"
        Me.HideMsServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.HideMsServerChar.Size = New System.Drawing.Size(61, 50)
        Me.HideMsServerChar.TabIndex = 4
        Me.HideMsServerChar.Text = "Hide character"
        Me.HideMsServerChar.UseVisualStyleBackColor = False
        '
        'ShowMsServerChar
        '
        Me.ShowMsServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.ShowMsServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowMsServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowMsServerChar.Location = New System.Drawing.Point(82, 24)
        Me.ShowMsServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowMsServerChar.Name = "ShowMsServerChar"
        Me.ShowMsServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowMsServerChar.Size = New System.Drawing.Size(61, 50)
        Me.ShowMsServerChar.TabIndex = 3
        Me.ShowMsServerChar.Text = "Show character"
        Me.ShowMsServerChar.UseVisualStyleBackColor = False
        '
        'UnloadMsServerChar
        '
        Me.UnloadMsServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.UnloadMsServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.UnloadMsServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.UnloadMsServerChar.Location = New System.Drawing.Point(8, 81)
        Me.UnloadMsServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.UnloadMsServerChar.Name = "UnloadMsServerChar"
        Me.UnloadMsServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.UnloadMsServerChar.Size = New System.Drawing.Size(61, 50)
        Me.UnloadMsServerChar.TabIndex = 2
        Me.UnloadMsServerChar.Text = "Unload character"
        Me.UnloadMsServerChar.UseVisualStyleBackColor = False
        '
        'LoadMsServerChar
        '
        Me.LoadMsServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.LoadMsServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.LoadMsServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.LoadMsServerChar.Location = New System.Drawing.Point(8, 24)
        Me.LoadMsServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.LoadMsServerChar.Name = "LoadMsServerChar"
        Me.LoadMsServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.LoadMsServerChar.Size = New System.Drawing.Size(61, 50)
        Me.LoadMsServerChar.TabIndex = 1
        Me.LoadMsServerChar.Text = "Load character"
        Me.LoadMsServerChar.UseVisualStyleBackColor = False
        '
        'MsControlFrame
        '
        Me.MsControlFrame.BackColor = System.Drawing.SystemColors.Control
        Me.MsControlFrame.Controls.Add(Me.ShowMsControlChars)
        Me.MsControlFrame.Controls.Add(Me.ShowMsControlOptions)
        Me.MsControlFrame.Controls.Add(Me.HideMsControlChar)
        Me.MsControlFrame.Controls.Add(Me.ShowMsControlChar)
        Me.MsControlFrame.Controls.Add(Me.UnloadMsControlChar)
        Me.MsControlFrame.Controls.Add(Me.LoadMsControlChar)
        Me.MsControlFrame.ForeColor = System.Drawing.SystemColors.ControlText
        Me.MsControlFrame.Location = New System.Drawing.Point(270, 8)
        Me.MsControlFrame.Margin = New System.Windows.Forms.Padding(2)
        Me.MsControlFrame.Name = "MsControlFrame"
        Me.MsControlFrame.Padding = New System.Windows.Forms.Padding(0)
        Me.MsControlFrame.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.MsControlFrame.Size = New System.Drawing.Size(233, 139)
        Me.MsControlFrame.TabIndex = 14
        Me.MsControlFrame.TabStop = False
        Me.MsControlFrame.Text = "MS Agent Control"
        '
        'ShowMsControlChars
        '
        Me.ShowMsControlChars.BackColor = System.Drawing.SystemColors.Control
        Me.ShowMsControlChars.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowMsControlChars.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowMsControlChars.Location = New System.Drawing.Point(158, 81)
        Me.ShowMsControlChars.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowMsControlChars.Name = "ShowMsControlChars"
        Me.ShowMsControlChars.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowMsControlChars.Size = New System.Drawing.Size(68, 50)
        Me.ShowMsControlChars.TabIndex = 20
        Me.ShowMsControlChars.Text = "Show characters"
        Me.ShowMsControlChars.UseVisualStyleBackColor = False
        '
        'ShowMsControlOptions
        '
        Me.ShowMsControlOptions.BackColor = System.Drawing.SystemColors.Control
        Me.ShowMsControlOptions.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowMsControlOptions.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowMsControlOptions.Location = New System.Drawing.Point(158, 24)
        Me.ShowMsControlOptions.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowMsControlOptions.Name = "ShowMsControlOptions"
        Me.ShowMsControlOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowMsControlOptions.Size = New System.Drawing.Size(68, 50)
        Me.ShowMsControlOptions.TabIndex = 19
        Me.ShowMsControlOptions.Text = "Show options"
        Me.ShowMsControlOptions.UseVisualStyleBackColor = False
        '
        'HideMsControlChar
        '
        Me.HideMsControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.HideMsControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.HideMsControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.HideMsControlChar.Location = New System.Drawing.Point(82, 81)
        Me.HideMsControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.HideMsControlChar.Name = "HideMsControlChar"
        Me.HideMsControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.HideMsControlChar.Size = New System.Drawing.Size(61, 50)
        Me.HideMsControlChar.TabIndex = 18
        Me.HideMsControlChar.Text = "Hide character"
        Me.HideMsControlChar.UseVisualStyleBackColor = False
        '
        'ShowMsControlChar
        '
        Me.ShowMsControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.ShowMsControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowMsControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowMsControlChar.Location = New System.Drawing.Point(82, 24)
        Me.ShowMsControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowMsControlChar.Name = "ShowMsControlChar"
        Me.ShowMsControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowMsControlChar.Size = New System.Drawing.Size(61, 50)
        Me.ShowMsControlChar.TabIndex = 17
        Me.ShowMsControlChar.Text = "Show character"
        Me.ShowMsControlChar.UseVisualStyleBackColor = False
        '
        'UnloadMsControlChar
        '
        Me.UnloadMsControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.UnloadMsControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.UnloadMsControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.UnloadMsControlChar.Location = New System.Drawing.Point(8, 81)
        Me.UnloadMsControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.UnloadMsControlChar.Name = "UnloadMsControlChar"
        Me.UnloadMsControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.UnloadMsControlChar.Size = New System.Drawing.Size(61, 50)
        Me.UnloadMsControlChar.TabIndex = 16
        Me.UnloadMsControlChar.Text = "Unload character"
        Me.UnloadMsControlChar.UseVisualStyleBackColor = False
        '
        'LoadMsControlChar
        '
        Me.LoadMsControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.LoadMsControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.LoadMsControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.LoadMsControlChar.Location = New System.Drawing.Point(8, 24)
        Me.LoadMsControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.LoadMsControlChar.Name = "LoadMsControlChar"
        Me.LoadMsControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.LoadMsControlChar.Size = New System.Drawing.Size(61, 50)
        Me.LoadMsControlChar.TabIndex = 15
        Me.LoadMsControlChar.Text = "Load character"
        Me.LoadMsControlChar.UseVisualStyleBackColor = False
        '
        'DaServerFrame
        '
        Me.DaServerFrame.BackColor = System.Drawing.SystemColors.Control
        Me.DaServerFrame.Controls.Add(Me.ShowDaServerChars)
        Me.DaServerFrame.Controls.Add(Me.ShowDaServerOptions)
        Me.DaServerFrame.Controls.Add(Me.HideDaServerChar)
        Me.DaServerFrame.Controls.Add(Me.ShowDaServerChar)
        Me.DaServerFrame.Controls.Add(Me.UnloadDaServerChar)
        Me.DaServerFrame.Controls.Add(Me.LoadDaServerChar)
        Me.DaServerFrame.ForeColor = System.Drawing.SystemColors.ControlText
        Me.DaServerFrame.Location = New System.Drawing.Point(8, 162)
        Me.DaServerFrame.Margin = New System.Windows.Forms.Padding(2)
        Me.DaServerFrame.Name = "DaServerFrame"
        Me.DaServerFrame.Padding = New System.Windows.Forms.Padding(0)
        Me.DaServerFrame.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.DaServerFrame.Size = New System.Drawing.Size(233, 139)
        Me.DaServerFrame.TabIndex = 7
        Me.DaServerFrame.TabStop = False
        Me.DaServerFrame.Text = "Double Agent Server"
        '
        'ShowDaServerChars
        '
        Me.ShowDaServerChars.BackColor = System.Drawing.SystemColors.Control
        Me.ShowDaServerChars.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowDaServerChars.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowDaServerChars.Location = New System.Drawing.Point(158, 81)
        Me.ShowDaServerChars.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowDaServerChars.Name = "ShowDaServerChars"
        Me.ShowDaServerChars.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowDaServerChars.Size = New System.Drawing.Size(68, 50)
        Me.ShowDaServerChars.TabIndex = 13
        Me.ShowDaServerChars.Text = "Show characters"
        Me.ShowDaServerChars.UseVisualStyleBackColor = False
        '
        'ShowDaServerOptions
        '
        Me.ShowDaServerOptions.BackColor = System.Drawing.SystemColors.Control
        Me.ShowDaServerOptions.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowDaServerOptions.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowDaServerOptions.Location = New System.Drawing.Point(158, 24)
        Me.ShowDaServerOptions.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowDaServerOptions.Name = "ShowDaServerOptions"
        Me.ShowDaServerOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowDaServerOptions.Size = New System.Drawing.Size(68, 50)
        Me.ShowDaServerOptions.TabIndex = 12
        Me.ShowDaServerOptions.Text = "Show options"
        Me.ShowDaServerOptions.UseVisualStyleBackColor = False
        '
        'HideDaServerChar
        '
        Me.HideDaServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.HideDaServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.HideDaServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.HideDaServerChar.Location = New System.Drawing.Point(82, 81)
        Me.HideDaServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.HideDaServerChar.Name = "HideDaServerChar"
        Me.HideDaServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.HideDaServerChar.Size = New System.Drawing.Size(61, 50)
        Me.HideDaServerChar.TabIndex = 11
        Me.HideDaServerChar.Text = "Hide character"
        Me.HideDaServerChar.UseVisualStyleBackColor = False
        '
        'ShowDaServerChar
        '
        Me.ShowDaServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.ShowDaServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowDaServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowDaServerChar.Location = New System.Drawing.Point(82, 24)
        Me.ShowDaServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowDaServerChar.Name = "ShowDaServerChar"
        Me.ShowDaServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowDaServerChar.Size = New System.Drawing.Size(61, 50)
        Me.ShowDaServerChar.TabIndex = 10
        Me.ShowDaServerChar.Text = "Show character"
        Me.ShowDaServerChar.UseVisualStyleBackColor = False
        '
        'UnloadDaServerChar
        '
        Me.UnloadDaServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.UnloadDaServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.UnloadDaServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.UnloadDaServerChar.Location = New System.Drawing.Point(8, 81)
        Me.UnloadDaServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.UnloadDaServerChar.Name = "UnloadDaServerChar"
        Me.UnloadDaServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.UnloadDaServerChar.Size = New System.Drawing.Size(61, 50)
        Me.UnloadDaServerChar.TabIndex = 9
        Me.UnloadDaServerChar.Text = "Unload character"
        Me.UnloadDaServerChar.UseVisualStyleBackColor = False
        '
        'LoadDaServerChar
        '
        Me.LoadDaServerChar.BackColor = System.Drawing.SystemColors.Control
        Me.LoadDaServerChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.LoadDaServerChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.LoadDaServerChar.Location = New System.Drawing.Point(8, 24)
        Me.LoadDaServerChar.Margin = New System.Windows.Forms.Padding(2)
        Me.LoadDaServerChar.Name = "LoadDaServerChar"
        Me.LoadDaServerChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.LoadDaServerChar.Size = New System.Drawing.Size(61, 50)
        Me.LoadDaServerChar.TabIndex = 8
        Me.LoadDaServerChar.Text = "Load character"
        Me.LoadDaServerChar.UseVisualStyleBackColor = False
        '
        'DaControlFrame
        '
        Me.DaControlFrame.BackColor = System.Drawing.SystemColors.Control
        Me.DaControlFrame.Controls.Add(Me.ShowDaControlChars)
        Me.DaControlFrame.Controls.Add(Me.ShowDaControlOptions)
        Me.DaControlFrame.Controls.Add(Me.HideDaControlChar)
        Me.DaControlFrame.Controls.Add(Me.ShowDaControlChar)
        Me.DaControlFrame.Controls.Add(Me.UnloadDaControlChar)
        Me.DaControlFrame.Controls.Add(Me.LoadDaControlChar)
        Me.DaControlFrame.ForeColor = System.Drawing.SystemColors.ControlText
        Me.DaControlFrame.Location = New System.Drawing.Point(270, 162)
        Me.DaControlFrame.Margin = New System.Windows.Forms.Padding(2)
        Me.DaControlFrame.Name = "DaControlFrame"
        Me.DaControlFrame.Padding = New System.Windows.Forms.Padding(0)
        Me.DaControlFrame.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.DaControlFrame.Size = New System.Drawing.Size(233, 139)
        Me.DaControlFrame.TabIndex = 21
        Me.DaControlFrame.TabStop = False
        Me.DaControlFrame.Text = "Double Agent Control"
        '
        'ShowDaControlChars
        '
        Me.ShowDaControlChars.BackColor = System.Drawing.SystemColors.Control
        Me.ShowDaControlChars.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowDaControlChars.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowDaControlChars.Location = New System.Drawing.Point(158, 81)
        Me.ShowDaControlChars.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowDaControlChars.Name = "ShowDaControlChars"
        Me.ShowDaControlChars.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowDaControlChars.Size = New System.Drawing.Size(68, 50)
        Me.ShowDaControlChars.TabIndex = 27
        Me.ShowDaControlChars.Text = "Show characters"
        Me.ShowDaControlChars.UseVisualStyleBackColor = False
        '
        'ShowDaControlOptions
        '
        Me.ShowDaControlOptions.BackColor = System.Drawing.SystemColors.Control
        Me.ShowDaControlOptions.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowDaControlOptions.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowDaControlOptions.Location = New System.Drawing.Point(158, 24)
        Me.ShowDaControlOptions.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowDaControlOptions.Name = "ShowDaControlOptions"
        Me.ShowDaControlOptions.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowDaControlOptions.Size = New System.Drawing.Size(68, 50)
        Me.ShowDaControlOptions.TabIndex = 26
        Me.ShowDaControlOptions.Text = "Show options"
        Me.ShowDaControlOptions.UseVisualStyleBackColor = False
        '
        'HideDaControlChar
        '
        Me.HideDaControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.HideDaControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.HideDaControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.HideDaControlChar.Location = New System.Drawing.Point(82, 81)
        Me.HideDaControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.HideDaControlChar.Name = "HideDaControlChar"
        Me.HideDaControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.HideDaControlChar.Size = New System.Drawing.Size(61, 50)
        Me.HideDaControlChar.TabIndex = 25
        Me.HideDaControlChar.Text = "Hide character"
        Me.HideDaControlChar.UseVisualStyleBackColor = False
        '
        'ShowDaControlChar
        '
        Me.ShowDaControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.ShowDaControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.ShowDaControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.ShowDaControlChar.Location = New System.Drawing.Point(82, 24)
        Me.ShowDaControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.ShowDaControlChar.Name = "ShowDaControlChar"
        Me.ShowDaControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.ShowDaControlChar.Size = New System.Drawing.Size(61, 50)
        Me.ShowDaControlChar.TabIndex = 24
        Me.ShowDaControlChar.Text = "Show character"
        Me.ShowDaControlChar.UseVisualStyleBackColor = False
        '
        'UnloadDaControlChar
        '
        Me.UnloadDaControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.UnloadDaControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.UnloadDaControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.UnloadDaControlChar.Location = New System.Drawing.Point(8, 81)
        Me.UnloadDaControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.UnloadDaControlChar.Name = "UnloadDaControlChar"
        Me.UnloadDaControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.UnloadDaControlChar.Size = New System.Drawing.Size(61, 50)
        Me.UnloadDaControlChar.TabIndex = 23
        Me.UnloadDaControlChar.Text = "Unload character"
        Me.UnloadDaControlChar.UseVisualStyleBackColor = False
        '
        'LoadDaControlChar
        '
        Me.LoadDaControlChar.BackColor = System.Drawing.SystemColors.Control
        Me.LoadDaControlChar.Cursor = System.Windows.Forms.Cursors.Default
        Me.LoadDaControlChar.ForeColor = System.Drawing.SystemColors.ControlText
        Me.LoadDaControlChar.Location = New System.Drawing.Point(8, 24)
        Me.LoadDaControlChar.Margin = New System.Windows.Forms.Padding(2)
        Me.LoadDaControlChar.Name = "LoadDaControlChar"
        Me.LoadDaControlChar.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.LoadDaControlChar.Size = New System.Drawing.Size(61, 50)
        Me.LoadDaControlChar.TabIndex = 22
        Me.LoadDaControlChar.Text = "Load character"
        Me.LoadDaControlChar.UseVisualStyleBackColor = False
        '
        'mDaAgentControl
        '
        Me.mDaAgentControl.Enabled = True
        Me.mDaAgentControl.Location = New System.Drawing.Point(237, 165)
        Me.mDaAgentControl.Margin = New System.Windows.Forms.Padding(2)
        Me.mDaAgentControl.Name = "mDaAgentControl"
        Me.mDaAgentControl.OcxState = CType(resources.GetObject("mDaAgentControl.OcxState"), System.Windows.Forms.AxHost.State)
        Me.mDaAgentControl.Size = New System.Drawing.Size(41, 31)
        Me.mDaAgentControl.TabIndex = 22
        '
        'mMsAgentControl
        '
        Me.mMsAgentControl.Enabled = True
        Me.mMsAgentControl.Location = New System.Drawing.Point(241, 12)
        Me.mMsAgentControl.Margin = New System.Windows.Forms.Padding(2)
        Me.mMsAgentControl.Name = "mMsAgentControl"
        Me.mMsAgentControl.OcxState = CType(resources.GetObject("mMsAgentControl.OcxState"), System.Windows.Forms.AxHost.State)
        Me.mMsAgentControl.Size = New System.Drawing.Size(32, 32)
        Me.mMsAgentControl.TabIndex = 23
        '
        'Sample1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.BackColor = System.Drawing.SystemColors.Control
        Me.ClientSize = New System.Drawing.Size(512, 311)
        Me.Controls.Add(Me.MsServerFrame)
        Me.Controls.Add(Me.MsControlFrame)
        Me.Controls.Add(Me.DaServerFrame)
        Me.Controls.Add(Me.DaControlFrame)
        Me.Controls.Add(Me.mDaAgentControl)
        Me.Controls.Add(Me.mMsAgentControl)
        Me.Cursor = System.Windows.Forms.Cursors.Default
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Location = New System.Drawing.Point(-2, 30)
        Me.Margin = New System.Windows.Forms.Padding(2)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "Sample1"
        Me.RightToLeft = System.Windows.Forms.RightToLeft.No
        Me.Text = "Sample 1"
        Me.MsServerFrame.ResumeLayout(False)
        Me.MsControlFrame.ResumeLayout(False)
        Me.DaServerFrame.ResumeLayout(False)
        Me.DaControlFrame.ResumeLayout(False)
        CType(Me.mDaAgentControl, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.mMsAgentControl, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
#End Region 
End Class