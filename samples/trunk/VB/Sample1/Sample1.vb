Option Strict Off
Option Explicit On
Friend Class Sample1
	Inherits System.Windows.Forms.Form

    Private Const mMsCharacterName As String = "Genie"
	Private Const mMsCharacterFile As String = "C:\Windows\MsAgent\Chars\Genie.acs"
	
	Private Const mDaCharacterName As String = "Merlin"
	Private Const mDaCharacterFile As String = "C:\Windows\MsAgent\Chars\Merlin.acs"
	
	Dim mMsControlChar As AgentObjects.IAgentCtlCharacterEx
	Dim mDaControlChar As DoubleAgentCtl.DaCtlCharacter
	
	Dim mMsServer As AgentServerObjects.AgentServer
	Dim mMsServerChar As AgentServerObjects.IAgentCharacterEx
	Dim mMsServerCharId As Integer
	
	Dim mDaServer As DoubleAgentSvr.DaServer
	Dim mDaServerChar As DoubleAgentSvr.DaCharacter
	Dim mDaServerCharId As Integer
	
    Private Sub Sample1_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
        SetMsControlButtons()
        SetMsServerButtons()
        SetDaControlButtons()
        SetDaServerButtons()
    End Sub
	
	Private Sub Sample1_FormClosed(ByVal eventSender As System.Object, ByVal eventArgs As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
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
	
	Private Sub mMsAgentControl_HideEvent(ByVal eventSender As System.Object, ByVal eventArgs As AxAgentObjects._AgentEvents_HideEvent) Handles mMsAgentControl.HideEvent
		SetMsControlButtons()
	End Sub
	
	Private Sub mMsAgentControl_ShowEvent(ByVal eventSender As System.Object, ByVal eventArgs As AxAgentObjects._AgentEvents_ShowEvent) Handles mMsAgentControl.ShowEvent
		SetMsControlButtons()
	End Sub
	
	Private Sub LoadMsControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles LoadMsControlChar.Click
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
	
	Private Sub UnloadMsControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles UnloadMsControlChar.Click
        If Not (mMsControlChar Is Nothing) Then
            mMsControlChar = Nothing
            mMsAgentControl.Characters.Unload(mMsCharacterName)
        End If
		SetMsControlButtons()
	End Sub
	
	Private Sub ShowMsControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowMsControlChar.Click
		If (Not mMsControlChar Is Nothing) Then
            mMsControlChar.Show(False)
            mMsControlChar.Speak(mMsControlChar.Name + ", Microsoft Agent", "")
		End If
	End Sub
	
	Private Sub HideMsControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles HideMsControlChar.Click
		If (Not mMsControlChar Is Nothing) Then
            mMsControlChar.Hide(False)
		End If
	End Sub
	
	Private Sub ShowMsControlOptions_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowMsControlOptions.Click
		mMsAgentControl.PropertySheet.Visible = True
	End Sub
	
	Private Sub ShowMsControlChars_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowMsControlChars.Click
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
	
	Private Sub mDaAgentControl_HideEvent(ByVal eventSender As System.Object, ByVal eventArgs As AxDoubleAgentCtl._DaCtlEvents_HideEvent) Handles mDaAgentControl.HideEvent
		SetDaControlButtons()
	End Sub
	
	Private Sub mDaAgentControl_ShowEvent(ByVal eventSender As System.Object, ByVal eventArgs As AxDoubleAgentCtl._DaCtlEvents_ShowEvent) Handles mDaAgentControl.ShowEvent
		SetDaControlButtons()
	End Sub
	
	Private Sub LoadDaControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles LoadDaControlChar.Click
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
	
    Private Sub UnloadDaControlChar_Click1(ByVal sender As Object, ByVal e As System.EventArgs) Handles UnloadDaControlChar.Click
        If Not (mDaControlChar Is Nothing) Then
            mDaControlChar = Nothing
            mDaAgentControl.Characters.Unload(mDaCharacterName)
        End If
        SetDaControlButtons()
    End Sub
	
	Private Sub ShowDaControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowDaControlChar.Click
		If (Not mDaControlChar Is Nothing) Then
            mDaControlChar.Show(False)
            mDaControlChar.Speak(mDaControlChar.Name + ", Double Agent", "")
		End If
	End Sub
	
	Private Sub HideDaControlChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles HideDaControlChar.Click
		If (Not mDaControlChar Is Nothing) Then
            mDaControlChar.Hide(False)
		End If
	End Sub
	
	Private Sub ShowDaControlOptions_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowDaControlOptions.Click
		mDaAgentControl.PropertySheet.Visible = True
	End Sub
	
	Private Sub ShowDaControlChars_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowDaControlChars.Click
		mDaAgentControl.ShowDefaultCharacterProperties()
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
	
	Private Sub LoadMsServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles LoadMsServerChar.Click
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
	
	Private Sub UnloadMsServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles UnloadMsServerChar.Click
		If Not (mMsServer Is Nothing) And Not (mMsServerChar Is Nothing) Then
            mMsServerChar = Nothing
            mMsServer.Unload(mMsServerCharId)
			mMsServerCharId = 0
		End If
		SetMsServerButtons()
	End Sub
	
	Private Sub ShowMsServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowMsServerChar.Click
		Dim lReqId As Integer
		
		If (Not mMsServerChar Is Nothing) Then
            mMsServerChar.Show(False, lReqId)
            mMsServerChar.Speak(mMsCharacterName + ", Microsoft Agent", "", lReqId)
		End If
		SetMsServerButtons()
	End Sub
	
	Private Sub HideMsServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles HideMsServerChar.Click
		Dim lReqId As Integer
		
		If (Not mMsServerChar Is Nothing) Then
            mMsServerChar.Hide(False, lReqId)
		End If
		SetMsServerButtons()
	End Sub
	
	Private Sub ShowMsServerChars_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowMsServerChars.Click
		Dim lServerEx As AgentServerObjects.IAgentEx
		
		If (mMsServer Is Nothing) Then
			mMsServer = New AgentServerObjects.AgentServer
		End If
		
		lServerEx = mMsServer
        lServerEx.ShowDefaultCharacterProperties(0, 0, True)
	End Sub
	
	Private Sub ShowMsServerOptions_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowMsServerOptions.Click
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
	
	Private Sub LoadDaServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles LoadDaServerChar.Click
		Dim lReqId As Integer
		
		If (mDaServer Is Nothing) Then
			mDaServer = New DoubleAgentSvr.DaServer
		End If
		
		If (mDaServerChar Is Nothing) Then
            mDaServer.Load(mDaCharacterFile, mDaServerCharId, lReqId)
            mDaServer.GetCharacter(mDaServerCharId, mDaServerChar)
            If (Not mDaServerChar Is Nothing) Then
                mDaServerChar.MoveTo(700, 300, 0, lReqId)
            End If
        End If
		SetDaServerButtons()
	End Sub
	
	Private Sub UnloadDaServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles UnloadDaServerChar.Click
		If Not (mDaServer Is Nothing) And Not (mDaServerChar Is Nothing) Then
            mDaServerChar = Nothing
            mDaServer.Unload(mDaServerCharId)
			mDaServerCharId = 0
		End If
		SetDaServerButtons()
	End Sub
	
	Private Sub ShowDaServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowDaServerChar.Click
		Dim lReqId As Integer
		
		If (Not mDaServerChar Is Nothing) Then
            mDaServerChar.Show(False, lReqId)
            mDaServerChar.Speak(mDaCharacterName + ", Double Agent", "", lReqId)
		End If
		SetDaServerButtons()
	End Sub
	
	Private Sub HideDaServerChar_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles HideDaServerChar.Click
		Dim lReqId As Integer
		
		If (Not mDaServerChar Is Nothing) Then
            mDaServerChar.Hide(False, lReqId)
		End If
		SetDaServerButtons()
	End Sub
	
	Private Sub ShowDaServerChars_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowDaServerChars.Click
		If (mDaServer Is Nothing) Then
			mDaServer = New DoubleAgentSvr.DaServer
		End If
		
        mDaServer.ShowDefaultCharacterProperties(0, 0, True)
	End Sub
	
	Private Sub ShowDaServerOptions_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles ShowDaServerOptions.Click
		Dim lServerProps As DoubleAgentSvr.DaPropertySheet
		
		If (mDaServer Is Nothing) Then
			mDaServer = New DoubleAgentSvr.DaServer
		End If
		
		lServerProps = mDaServer
        lServerProps.SetVisible(True)
	End Sub
End Class