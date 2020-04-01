VERSION 5.00
Object = "{4FED1AC3-6CF3-11D6-BB83-9FDD9223647D}#1.0#0"; "AXIMAGE.OCX"
Begin VB.Form frmDocument 
   Caption         =   "frmDocument"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   Begin AXIMAGELib.AxImage AxImage1 
      Height          =   2175
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2775
      _Version        =   65536
      _ExtentX        =   4895
      _ExtentY        =   3836
      _StockProps     =   1
      BackColor       =   -2147483643
   End
End
Attribute VB_Name = "frmDocument"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub rtfText_SelChange()
End Sub

Private Sub Form_Load()
    AxImage1.Border = True
    Form_Resize
End Sub


Private Sub Form_Resize()
    On Error Resume Next
    AxImage1.Move 50, 50, Me.ScaleWidth - 100, Me.ScaleHeight - 100
    AxImage1.RightMargin = AxImage1.Width - 400
End Sub

