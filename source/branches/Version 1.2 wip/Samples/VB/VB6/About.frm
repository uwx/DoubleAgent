VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About..."
   ClientHeight    =   2328
   ClientLeft      =   2340
   ClientTop       =   1932
   ClientWidth     =   5736
   ClipControls    =   0   'False
   ControlBox      =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1602.769
   ScaleMode       =   0  'User
   ScaleWidth      =   5380.766
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdMore 
      Cancel          =   -1  'True
      Caption         =   "More..."
      Default         =   -1  'True
      Height          =   345
      Left            =   3792
      TabIndex        =   4
      Top             =   896
      Width           =   1260
   End
   Begin VB.PictureBox picIcon 
      AutoSize        =   -1  'True
      ClipControls    =   0   'False
      Height          =   432
      Left            =   240
      Picture         =   "About.frx":0000
      ScaleHeight     =   263.118
      ScaleMode       =   0  'User
      ScaleWidth      =   263.118
      TabIndex        =   1
      Top             =   240
      Width           =   432
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Height          =   345
      Left            =   2238
      TabIndex        =   0
      Top             =   1680
      Width           =   1260
   End
   Begin VB.Label lblTitle 
      Caption         =   "Sample program..."
      ForeColor       =   &H00000000&
      Height          =   480
      Left            =   960
      TabIndex        =   3
      Top             =   240
      Width           =   4092
   End
   Begin VB.Label lblAckwnoledgement 
      Caption         =   "This program used Double Agent under the GNU Public License."
      ForeColor       =   &H00000000&
      Height          =   456
      Left            =   960
      TabIndex        =   2
      Top             =   840
      Width           =   2652
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Public mDaControl As DoubleAgentCtl.DaControl

Private Sub cmdMore_Click()
    mDaControl.PropertySheet.Visible = True
    mDaControl.PropertySheet.Page = "Copyright"
End Sub

Private Sub cmdOK_Click()
    Unload Me
End Sub
