VERSION 5.00
Object = "{1147E550-A208-11DE-ABF2-002421116FB2}#1.1#0"; "DaControl.dll"
Object = "{F5BE8BC2-7DE6-11D0-91FE-00C04FD701A5}#2.0#0"; "AgentCtl.dll"
Begin VB.Form MsaWithDa 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "MS Agent with Double Agent"
   ClientHeight    =   4596
   ClientLeft      =   120
   ClientTop       =   744
   ClientWidth     =   8208
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4596
   ScaleWidth      =   8208
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame MsServerFrame 
      Caption         =   "MS Agent Server"
      Height          =   2052
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3732
      Begin VB.CommandButton ShowMsServerChars 
         Caption         =   "Show characters"
         Height          =   732
         Left            =   2640
         TabIndex        =   6
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowMsServerOptions 
         Caption         =   "Show options"
         Height          =   732
         Left            =   2640
         TabIndex        =   5
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton HideMsServerChar 
         Caption         =   "Hide character"
         Height          =   732
         Left            =   1320
         TabIndex        =   4
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowMsServerChar 
         Caption         =   "Show character"
         Height          =   732
         Left            =   1320
         TabIndex        =   3
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton UnloadMsServerChar 
         Caption         =   "Unload character"
         Height          =   732
         Left            =   120
         TabIndex        =   2
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton LoadMsServerChar 
         Caption         =   "Load character"
         Height          =   732
         Left            =   120
         TabIndex        =   1
         Top             =   360
         Width           =   972
      End
   End
   Begin VB.Frame MsControlFrame 
      Caption         =   "MS Agent Control"
      Height          =   2052
      Left            =   4320
      TabIndex        =   14
      Top             =   120
      Width           =   3732
      Begin VB.CommandButton ShowMsControlChars 
         Caption         =   "Show characters"
         Height          =   732
         Left            =   2640
         TabIndex        =   20
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowMsControlOptions 
         Caption         =   "Show options"
         Height          =   732
         Left            =   2640
         TabIndex        =   19
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton HideMsControlChar 
         Caption         =   "Hide character"
         Height          =   732
         Left            =   1320
         TabIndex        =   18
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowMsControlChar 
         Caption         =   "Show character"
         Height          =   732
         Left            =   1320
         TabIndex        =   17
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton UnloadMsControlChar 
         Caption         =   "Unload character"
         Height          =   732
         Left            =   120
         TabIndex        =   16
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton LoadMsControlChar 
         Caption         =   "Load character"
         Height          =   732
         Left            =   120
         TabIndex        =   15
         Top             =   360
         Width           =   972
      End
   End
   Begin VB.Frame DaServerFrame 
      Caption         =   "Double Agent Server"
      Height          =   2052
      Left            =   120
      TabIndex        =   7
      Top             =   2400
      Width           =   3732
      Begin VB.CommandButton ShowDaServerChars 
         Caption         =   "Show characters"
         Height          =   732
         Left            =   2640
         TabIndex        =   13
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowDaServerOptions 
         Caption         =   "Show options"
         Height          =   732
         Left            =   2640
         TabIndex        =   12
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton HideDaServerChar 
         Caption         =   "Hide character"
         Height          =   732
         Left            =   1320
         TabIndex        =   11
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowDaServerChar 
         Caption         =   "Show character"
         Height          =   732
         Left            =   1320
         TabIndex        =   10
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton UnloadDaServerChar 
         Caption         =   "Unload character"
         Height          =   732
         Left            =   120
         TabIndex        =   9
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton LoadDaServerChar 
         Caption         =   "Load character"
         Height          =   732
         Left            =   120
         TabIndex        =   8
         Top             =   360
         Width           =   972
      End
   End
   Begin VB.Frame DaControlFrame 
      Caption         =   "Double Agent Control"
      Height          =   2052
      Left            =   4320
      TabIndex        =   21
      Top             =   2400
      Width           =   3732
      Begin VB.CommandButton ShowDaControlChars 
         Caption         =   "Show characters"
         Height          =   732
         Left            =   2640
         TabIndex        =   27
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowDaControlOptions 
         Caption         =   "Show options"
         Height          =   732
         Left            =   2640
         TabIndex        =   26
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton HideDaControlChar 
         Caption         =   "Hide character"
         Height          =   732
         Left            =   1320
         TabIndex        =   25
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton ShowDaControlChar 
         Caption         =   "Show character"
         Height          =   732
         Left            =   1320
         TabIndex        =   24
         Top             =   360
         Width           =   972
      End
      Begin VB.CommandButton UnloadDaControlChar 
         Caption         =   "Unload character"
         Height          =   732
         Left            =   120
         TabIndex        =   23
         Top             =   1200
         Width           =   972
      End
      Begin VB.CommandButton LoadDaControlChar 
         Caption         =   "Load character"
         Height          =   732
         Left            =   120
         TabIndex        =   22
         Top             =   360
         Width           =   972
      End
   End
   Begin DoubleAgentCtl.DaControl mDaAgentControl 
      Height          =   420
      Left            =   3840
      Top             =   2520
      Visible         =   0   'False
      Width           =   480
      _Version        =   257
      _ExtentX        =   847
      _ExtentY        =   741
      RaiseRequestErrors=   -1  'True
      AutoConnect     =   -1  'True
      AutoSize        =   0   'False
      BackColor       =   -2147483643
      BorderColor     =   -2147483640
      BorderStyle     =   0
      BorderVisible   =   0   'False
      BorderWidth     =   1
   End
   Begin AgentObjectsCtl.Agent mMsAgentControl 
      Left            =   3840
      Top             =   240
      _Version        =   257
      _ExtentX        =   847
      _ExtentY        =   847
      RaiseRequestErrors=   -1  'True
      AutoConnect     =   -1  'True
      AutoSize        =   0   'False
      BackColor       =   -2147483643
      BorderColor     =   -2147483640
      BorderStyle     =   1
      BorderVisible   =   -1  'True
      BorderWidth     =   1
   End
   Begin VB.Menu Help 
      Caption         =   "Help"
      Index           =   0
      Begin VB.Menu About 
         Caption         =   "About"
         Index           =   0
      End
   End
End
Attribute VB_Name = "MsaWithDa"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Const mMsCharacterName As String = "Genie"
Private Const mMsCharacterFile As String = "C:\Windows\MsAgent\Chars\Genie.acs"

Private Const mDaCharacterName As String = "Merlin"
Private Const mDaCharacterFile As String = "C:\Windows\MsAgent\Chars\Merlin.acs"

Dim mMsControlChar As AgentObjectsCtl.IAgentCtlCharacterEx
Dim mDaControlChar As DoubleAgentCtl.DaCtlCharacter

Dim mMsServer As AgentServerObjects.AgentServer
Dim mMsServerChar As AgentServerObjects.IAgentCharacterEx
Dim mMsServerCharId As Long

Dim mDaServer As DoubleAgentSvr.DaServer
Dim mDaServerChar As DoubleAgentSvr.DaSvrCharacter
Dim mDaServerCharId As Long

Option Explicit

Private Sub About_Click(Index As Integer)
    Dim lAbout As New frmAbout
    Set lAbout.mDaControl = mDaAgentControl
    Call lAbout.Show(vbModal, Me)
End Sub

Private Sub Form_Load()
    SetMsControlButtons
    SetMsServerButtons
    SetDaControlButtons
    SetDaServerButtons
End Sub

Private Sub Form_Unload(Cancel As Integer)
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

Private Sub mMsAgentControl_Hide(ByVal CharacterID As String, ByVal Cause As Integer)
    SetMsControlButtons
End Sub

Private Sub mMsAgentControl_Show(ByVal CharacterID As String, ByVal Cause As Integer)
    SetMsControlButtons
End Sub

Private Sub LoadMsControlChar_Click()
    If (mMsControlChar Is Nothing) Then
        mMsAgentControl.RaiseRequestErrors = False
        Call mMsAgentControl.Characters.Load(mMsCharacterName, mMsCharacterFile)
        mMsAgentControl.RaiseRequestErrors = True
        Set mMsControlChar = mMsAgentControl.Characters.Character(mMsCharacterName)
        If (Not mMsControlChar Is Nothing) Then
            Call mMsControlChar.MoveTo(700, 100)
        End If
    End If
    SetMsControlButtons
End Sub

Private Sub UnloadMsControlChar_Click()
    If Not (mMsControlChar Is Nothing) Then
        Set mMsControlChar = Nothing
        Call mMsAgentControl.Characters.Unload(mMsCharacterName)
    End If
    SetMsControlButtons
End Sub

Private Sub ShowMsControlChar_Click()
    If (Not mMsControlChar Is Nothing) Then
        Call mMsControlChar.Show(False)
        Call mMsControlChar.Speak(mMsControlChar.Name + ", Microsoft Agent", "")
    End If
End Sub

Private Sub HideMsControlChar_Click()
    If (Not mMsControlChar Is Nothing) Then
        Call mMsControlChar.Hide(False)
    End If
End Sub

Private Sub ShowMsControlOptions_Click()
    mMsAgentControl.PropertySheet.Visible = True
End Sub

Private Sub ShowMsControlChars_Click()
    mMsAgentControl.ShowDefaultCharacterProperties
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

Private Sub mDaAgentControl_Hide(ByVal CharacterID As String, ByVal Cause As DoubleAgentCtl.VisibilityCauseType)
    SetDaControlButtons
End Sub

Private Sub mDaAgentControl_Show(ByVal CharacterID As String, ByVal Cause As DoubleAgentCtl.VisibilityCauseType)
    SetDaControlButtons
End Sub

Private Sub LoadDaControlChar_Click()
    Dim lCharacters As DoubleAgentCtl.DaCtlCharacters

    If (mDaControlChar Is Nothing) Then
        Set lCharacters = mDaAgentControl.Characters
        mDaAgentControl.RaiseRequestErrors = False
        Call lCharacters.Load(mDaCharacterName, mDaCharacterFile)
        mDaAgentControl.RaiseRequestErrors = True
        Set mDaControlChar = lCharacters.Character(mDaCharacterName)
        If (Not mDaControlChar Is Nothing) Then
            Call mDaControlChar.MoveTo(700, 300)
        End If
    End If
    SetDaControlButtons
End Sub

Private Sub UnloadDaControlChar_Click()
    Dim lCharacters As DoubleAgentCtl.DaCtlCharacters
    
    If Not (mDaControlChar Is Nothing) Then
        Set lCharacters = mDaAgentControl.Characters
        Set mDaControlChar = Nothing
        Call lCharacters.Unload(mDaCharacterName)
    End If
    SetDaControlButtons
End Sub

Private Sub ShowDaControlChar_Click()
    If (Not mDaControlChar Is Nothing) Then
        Call mDaControlChar.Show(False)
        Call mDaControlChar.Speak(mDaControlChar.Name + ", Double Agent", "")
    End If
End Sub

Private Sub HideDaControlChar_Click()
    If (Not mDaControlChar Is Nothing) Then
        Call mDaControlChar.Hide(False)
    End If
End Sub

Private Sub ShowDaControlOptions_Click()
    mDaAgentControl.PropertySheet.Visible = True
End Sub

Private Sub ShowDaControlChars_Click()
    mDaAgentControl.ShowDefaultCharacterProperties
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

Private Sub LoadMsServerChar_Click()
    Dim lReqId As Long
    
    If (mMsServer Is Nothing) Then
        Set mMsServer = New AgentServerObjects.AgentServer
    End If
    
    If (mMsServerChar Is Nothing) Then
        Call mMsServer.Load(mMsCharacterFile, mMsServerCharId, lReqId)
        Call mMsServer.GetCharacter(mMsServerCharId, mMsServerChar)
        If (Not mMsServerChar Is Nothing) Then
            Call mMsServerChar.MoveTo(700, 100, 0, lReqId)
        End If
    End If
    SetMsServerButtons
End Sub

Private Sub UnloadMsServerChar_Click()
    If Not (mMsServer Is Nothing) And Not (mMsServerChar Is Nothing) Then
        Set mMsServerChar = Nothing
        Call mMsServer.Unload(mMsServerCharId)
        mMsServerCharId = 0
    End If
    SetMsServerButtons
End Sub

Private Sub ShowMsServerChar_Click()
    Dim lReqId As Long
    
    If (Not mMsServerChar Is Nothing) Then
        Call mMsServerChar.Show(False, lReqId)
        Call mMsServerChar.Speak(mMsCharacterName + ", Microsoft Agent", "", lReqId)
    End If
    SetMsServerButtons
End Sub

Private Sub HideMsServerChar_Click()
    Dim lReqId As Long
    
    If (Not mMsServerChar Is Nothing) Then
        Call mMsServerChar.Hide(False, lReqId)
    End If
    SetMsServerButtons
End Sub

Private Sub ShowMsServerChars_Click()
    Dim lServerEx As AgentServerObjects.IAgentEx

    If (mMsServer Is Nothing) Then
        Set mMsServer = New AgentServerObjects.AgentServer
    End If
    
    Set lServerEx = mMsServer
    Call lServerEx.ShowDefaultCharacterProperties(0, 0, True)
End Sub

Private Sub ShowMsServerOptions_Click()
    Dim lServerProps As AgentServerObjects.IAgentPropertySheet

    If (mMsServer Is Nothing) Then
        Set mMsServer = New AgentServerObjects.AgentServer
    End If
    
    Set lServerProps = mMsServer
    Call lServerProps.SetVisible(True)
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

Private Sub LoadDaServerChar_Click()
    Dim lReqId As Long

    If (mDaServer Is Nothing) Then
        Set mDaServer = New DoubleAgentSvr.DaServer
    End If

    If (mDaServerChar Is Nothing) Then
        Call mDaServer.Load(mDaCharacterFile, mDaServerCharId, lReqId)
        Call mDaServer.GetCharacter(mDaServerCharId, mDaServerChar)
        If (Not mDaServerChar Is Nothing) Then
            Call mDaServerChar.MoveTo(700, 300, 0, lReqId)
        End If
    End If
    SetDaServerButtons
End Sub

Private Sub UnloadDaServerChar_Click()
    If Not (mDaServer Is Nothing) And Not (mDaServerChar Is Nothing) Then
        Set mDaServerChar = Nothing
        Call mDaServer.Unload(mDaServerCharId)
        mDaServerCharId = 0
    End If
    SetDaServerButtons
End Sub

Private Sub ShowDaServerChar_Click()
    Dim lReqId As Long

    If (Not mDaServerChar Is Nothing) Then
        Call mDaServerChar.Show(False, lReqId)
        Call mDaServerChar.Speak(mDaCharacterName + ", Double Agent", "", lReqId)
    End If
    SetDaServerButtons
End Sub

Private Sub HideDaServerChar_Click()
    Dim lReqId As Long

    If (Not mDaServerChar Is Nothing) Then
        Call mDaServerChar.Hide(False, lReqId)
    End If
    SetDaServerButtons
End Sub

Private Sub ShowDaServerChars_Click()
    If (mDaServer Is Nothing) Then
        Set mDaServer = New DoubleAgentSvr.DaServer
    End If

    Call mDaServer.ShowDefaultCharacterProperties(0, 0, True)
End Sub

Private Sub ShowDaServerOptions_Click()
    Dim lServerProps As DoubleAgentSvr.DaSvrPropertySheet

    If (mDaServer Is Nothing) Then
        Set mDaServer = New DoubleAgentSvr.DaServer
    End If
    
    Set lServerProps = mDaServer
    Call lServerProps.SetVisible(True)
End Sub

