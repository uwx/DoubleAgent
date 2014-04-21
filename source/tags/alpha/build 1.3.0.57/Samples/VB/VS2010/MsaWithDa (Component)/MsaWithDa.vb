Option Strict Off
Option Explicit On
Friend Class MsaWithDa
    Inherits System.Windows.Forms.Form

    Private Const mMsCharacterName As String = "Genie"
    Private Const mMsCharacterFile As String = "C:\Windows\MsAgent\Chars\Genie.acs"

    Private Const mDaCharacterName As String = "Merlin"
    Private Const mDaCharacterFile As String = "C:\Windows\MsAgent\Chars\Merlin.acs"

    Dim mMsControlChar As AgentObjects.IAgentCtlCharacterEx
    Dim WithEvents mDaAgentControl As DoubleAgent.Control.Control
    Dim mDaControlChar As DoubleAgent.Control.Character

    Dim mMsServer As AgentServerObjects.AgentServer
    Dim mMsServerChar As AgentServerObjects.IAgentCharacterEx
    Dim mMsServerCharId As Integer

    Dim mDaServer As DoubleAgent.Server.Server
    Dim mDaServerChar As DoubleAgent.Server.Character
    Dim mDaServerCharId As Integer

    Private Sub MsaWithDa_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        mDaAgentControl = New DoubleAgent.Control.Control
        SetDaControlButtons()
        SetMsControlButtons()
        SetMsServerButtons()
        SetDaControlButtons()
        SetDaServerButtons()
    End Sub

    Private Sub MsaWithDa_FormClosed(sender As System.Object, e As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
        mDaAgentControl.Connected = False
        mMsAgentControl.Connected = False
    End Sub

    '***********************************************************************
    Private Sub SetMsControlButtons()
        If (mMsControlChar Is Nothing) Then
            LoadMsControlChar.Enabled = True
            UnloadMsControlChar.Enabled = False
            ShowMsControlChar.Enabled = False
            HideMsControlChar.Enabled = False
        Else
            LoadMsControlChar.Enabled = False
            UnloadMsControlChar.Enabled = True
            ShowMsControlChar.Enabled = Not mMsControlChar.Visible
            HideMsControlChar.Enabled = mMsControlChar.Visible
        End If
    End Sub

    Private Sub mMsAgentControl_HideEvent(sender As System.Object, e As AxAgentObjects._AgentEvents_HideEvent) Handles mMsAgentControl.HideEvent
        SetMsControlButtons()
    End Sub

    Private Sub mMsAgentControl_ShowEvent(sender As System.Object, e As AxAgentObjects._AgentEvents_ShowEvent) Handles mMsAgentControl.ShowEvent
        SetMsControlButtons()
    End Sub

    Private Sub LoadMsControlChar_Click(sender As System.Object, e As System.EventArgs) Handles LoadMsControlChar.Click
        If (mMsControlChar Is Nothing) Then
            mMsAgentControl.RaiseRequestErrors = False
            mMsAgentControl.Characters.Load(mMsCharacterName, mMsCharacterFile)
            mMsAgentControl.RaiseRequestErrors = True
            mMsControlChar = mMsAgentControl.Characters.Character(mMsCharacterName)
            If (Not mMsControlChar Is Nothing) Then
                mMsControlChar.MoveTo(700, 100)
            End If
        End If
        SetMsControlButtons()
    End Sub

    Private Sub UnloadMsControlChar_Click(sender As System.Object, e As System.EventArgs) Handles UnloadMsControlChar.Click
        If Not (mMsControlChar Is Nothing) Then
            mMsControlChar = Nothing
            mMsAgentControl.Characters.Unload(mMsCharacterName)
        End If
        SetMsControlButtons()
    End Sub

    Private Sub ShowMsControlChar_Click(sender As System.Object, e As System.EventArgs) Handles ShowMsControlChar.Click
        If (Not mMsControlChar Is Nothing) Then
            mMsControlChar.Show(False)
            mMsControlChar.Speak(mMsControlChar.Name + ", Microsoft Agent", "")
        End If
    End Sub

    Private Sub HideMsControlChar_Click(sender As System.Object, e As System.EventArgs) Handles HideMsControlChar.Click
        If (Not mMsControlChar Is Nothing) Then
            mMsControlChar.Hide(False)
        End If
    End Sub

    Private Sub ShowMsControlOptions_Click(sender As System.Object, e As System.EventArgs) Handles ShowMsControlOptions.Click
        mMsAgentControl.PropertySheet.Visible = True
    End Sub

    Private Sub ShowMsControlChars_Click(sender As System.Object, e As System.EventArgs) Handles ShowMsControlChars.Click
        mMsAgentControl.ShowDefaultCharacterProperties()
    End Sub
    '***********************************************************************
    Private Sub SetDaControlButtons()
        If (mDaControlChar Is Nothing) Then
            LoadDaControlChar.Enabled = True
            UnloadDaControlChar.Enabled = False
            ShowDaControlChar.Enabled = False
            HideDaControlChar.Enabled = False
        Else
            LoadDaControlChar.Enabled = False
            UnloadDaControlChar.Enabled = True
            ShowDaControlChar.Enabled = Not mDaControlChar.Visible
            HideDaControlChar.Enabled = mDaControlChar.Visible
        End If
    End Sub

    Private Sub mDaAgentControl_Hide(CharacterID As String, Cause As DoubleAgent.Control.VisibilityCauseType) Handles mDaAgentControl.Hide
        SetDaControlButtons()
    End Sub

    Private Sub mDaAgentControl_Show(CharacterID As String, Cause As DoubleAgent.Control.VisibilityCauseType) Handles mDaAgentControl.Show
        SetDaControlButtons()
    End Sub

    Private Sub LoadDaControlChar_Click(sender As System.Object, e As System.EventArgs) Handles LoadDaControlChar.Click
        If (mDaControlChar Is Nothing) Then
            mDaAgentControl.RaiseRequestErrors = False
            mDaAgentControl.Characters.Load(mDaCharacterName, mDaCharacterFile)
            mDaAgentControl.RaiseRequestErrors = True
            mDaControlChar = mDaAgentControl.Characters.Character(mDaCharacterName)
            If (Not mDaControlChar Is Nothing) Then
                mDaControlChar.MoveTo(700, 300)
            End If
        End If
        SetDaControlButtons()
    End Sub

    Private Sub UnloadDaControlChar_Click1(sender As Object, e As System.EventArgs) Handles UnloadDaControlChar.Click
        If Not (mDaControlChar Is Nothing) Then
            mDaControlChar = Nothing
            mDaAgentControl.Characters.Unload(mDaCharacterName)
        End If
        SetDaControlButtons()
    End Sub

    Private Sub ShowDaControlChar_Click(sender As System.Object, e As System.EventArgs) Handles ShowDaControlChar.Click
        If (Not mDaControlChar Is Nothing) Then
            mDaControlChar.Show(False)
            mDaControlChar.Speak(mDaControlChar.Name + ", Double Agent", "")
        End If
    End Sub

    Private Sub HideDaControlChar_Click(sender As System.Object, e As System.EventArgs) Handles HideDaControlChar.Click
        If (Not mDaControlChar Is Nothing) Then
            mDaControlChar.Hide(False)
        End If
    End Sub

    Private Sub ShowDaControlOptions_Click(sender As System.Object, e As System.EventArgs) Handles ShowDaControlOptions.Click
        mDaAgentControl.PropertySheet.Visible = True
    End Sub

    Private Sub ShowDaControlChars_Click(sender As System.Object, e As System.EventArgs) Handles ShowDaControlChars.Click
        mDaAgentControl.ShowDefaultCharacterProperties(0, 0)
    End Sub
    '***********************************************************************
    Private Sub SetMsServerButtons()
        If (mMsServerChar Is Nothing) Then
            LoadMsServerChar.Enabled = True
            UnloadMsServerChar.Enabled = False
            ShowMsServerChar.Enabled = False
            HideMsServerChar.Enabled = False
        Else
            LoadMsServerChar.Enabled = False
            UnloadMsServerChar.Enabled = True
            ShowMsServerChar.Enabled = True
            HideMsServerChar.Enabled = True
        End If
    End Sub

    Private Sub LoadMsServerChar_Click(sender As System.Object, e As System.EventArgs) Handles LoadMsServerChar.Click
        Dim lReqId As Integer

        If (mMsServer Is Nothing) Then
            mMsServer = New AgentServerObjects.AgentServer
        End If

        If (mMsServerChar Is Nothing) Then
            mMsServer.Load(mMsCharacterFile, mMsServerCharId, lReqId)
            mMsServer.GetCharacter(mMsServerCharId, mMsServerChar)
            If (Not mMsServerChar Is Nothing) Then
                mMsServerChar.MoveTo(700, 100, 0, lReqId)
            End If
        End If
        SetMsServerButtons()
    End Sub

    Private Sub UnloadMsServerChar_Click(sender As System.Object, e As System.EventArgs) Handles UnloadMsServerChar.Click
        If Not (mMsServer Is Nothing) And Not (mMsServerChar Is Nothing) Then
            mMsServerChar = Nothing
            mMsServer.Unload(mMsServerCharId)
            mMsServerCharId = 0
        End If
        SetMsServerButtons()
    End Sub

    Private Sub ShowMsServerChar_Click(sender As System.Object, e As System.EventArgs) Handles ShowMsServerChar.Click
        Dim lReqId As Integer

        If (Not mMsServerChar Is Nothing) Then
            mMsServerChar.Show(False, lReqId)
            mMsServerChar.Speak(mMsCharacterName + ", Microsoft Agent", "", lReqId)
        End If
        SetMsServerButtons()
    End Sub

    Private Sub HideMsServerChar_Click(sender As System.Object, e As System.EventArgs) Handles HideMsServerChar.Click
        Dim lReqId As Integer

        If (Not mMsServerChar Is Nothing) Then
            mMsServerChar.Hide(False, lReqId)
        End If
        SetMsServerButtons()
    End Sub

    Private Sub ShowMsServerChars_Click(sender As System.Object, e As System.EventArgs) Handles ShowMsServerChars.Click
        Dim lServerEx As AgentServerObjects.IAgentEx

        If (mMsServer Is Nothing) Then
            mMsServer = New AgentServerObjects.AgentServer
        End If

        lServerEx = mMsServer
        lServerEx.ShowDefaultCharacterProperties(0, 0, True)
    End Sub

    Private Sub ShowMsServerOptions_Click(sender As System.Object, e As System.EventArgs) Handles ShowMsServerOptions.Click
        Dim lServerProps As AgentServerObjects.IAgentPropertySheet

        If (mMsServer Is Nothing) Then
            mMsServer = New AgentServerObjects.AgentServer
        End If

        lServerProps = mMsServer
        lServerProps.SetVisible(True)
    End Sub
    '***********************************************************************
    Private Sub SetDaServerButtons()
        If (mDaServerChar Is Nothing) Then
            LoadDaServerChar.Enabled = True
            UnloadDaServerChar.Enabled = False
            ShowDaServerChar.Enabled = False
            HideDaServerChar.Enabled = False
        Else
            LoadDaServerChar.Enabled = False
            UnloadDaServerChar.Enabled = True
            ShowDaServerChar.Enabled = True
            HideDaServerChar.Enabled = True
        End If
    End Sub

    Private Sub LoadDaServerChar_Click(sender As System.Object, e As System.EventArgs) Handles LoadDaServerChar.Click
        Dim lReqId As Integer

        If (mDaServer Is Nothing) Then
            mDaServer = New DoubleAgent.Server.Server
        End If

        If (mDaServerChar Is Nothing) Then
            mDaServer.Load(mDaCharacterFile, mDaServerCharId, lReqId)
            mDaServerChar = mDaServer.Character(mDaServerCharId)
            If (Not mDaServerChar Is Nothing) Then
                mDaServerChar.MoveTo(700, 300, 0, lReqId)
            End If
        End If
        SetDaServerButtons()
    End Sub

    Private Sub UnloadDaServerChar_Click(sender As System.Object, e As System.EventArgs) Handles UnloadDaServerChar.Click
        If Not (mDaServer Is Nothing) And Not (mDaServerChar Is Nothing) Then
            mDaServerChar = Nothing
            mDaServer.Unload(mDaServerCharId)
            mDaServerCharId = 0
        End If
        SetDaServerButtons()
    End Sub

    Private Sub ShowDaServerChar_Click(sender As System.Object, e As System.EventArgs) Handles ShowDaServerChar.Click
        Dim lReqId As Integer

        If (Not mDaServerChar Is Nothing) Then
            mDaServerChar.Show(False, lReqId)
            mDaServerChar.Speak(mDaCharacterName + ", Double Agent", "", lReqId)
        End If
        SetDaServerButtons()
    End Sub

    Private Sub HideDaServerChar_Click(sender As System.Object, e As System.EventArgs) Handles HideDaServerChar.Click
        Dim lReqId As Integer

        If (Not mDaServerChar Is Nothing) Then
            mDaServerChar.Hide(False, lReqId)
        End If
        SetDaServerButtons()
    End Sub

    Private Sub ShowDaServerChars_Click(sender As System.Object, e As System.EventArgs) Handles ShowDaServerChars.Click
        Dim lServerProps As DoubleAgent.Server.PropertySheet

        If (mDaServer Is Nothing) Then
            mDaServer = New DoubleAgent.Server.Server
        End If

        lServerProps = mDaServer.PropertySheet
        lServerProps.Page = "Character"
        lServerProps.SetVisible(True)
    End Sub

    Private Sub ShowDaServerOptions_Click(sender As System.Object, e As System.EventArgs) Handles ShowDaServerOptions.Click
        Dim lServerProps As DoubleAgent.Server.PropertySheet

        If (mDaServer Is Nothing) Then
            mDaServer = New DoubleAgent.Server.Server
        End If

        lServerProps = mDaServer.PropertySheet
        lServerProps.SetVisible(True)
    End Sub

    Private Sub AboutToolStripMenuItem_Click(sender As System.Object, e As System.EventArgs) Handles AboutToolStripMenuItem.Click
        Dim lAbout As New AboutBox
        lAbout.mDaControl = mDaAgentControl
        lAbout.Show(Me)
    End Sub
End Class