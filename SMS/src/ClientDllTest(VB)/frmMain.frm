VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Test"
   ClientHeight    =   4395
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6345
   LinkTopic       =   "Form1"
   ScaleHeight     =   4395
   ScaleWidth      =   6345
   StartUpPosition =   2  '��Ļ����
   Begin VB.CommandButton cmdSend 
      Caption         =   "����"
      Height          =   375
      Left            =   4320
      TabIndex        =   14
      Top             =   3600
      Width           =   1215
   End
   Begin VB.CommandButton cmdDisconnect 
      Caption         =   "�Ͽ�"
      Height          =   375
      Left            =   4320
      TabIndex        =   13
      Top             =   1680
      Width           =   1215
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "����"
      Height          =   375
      Left            =   4320
      TabIndex        =   12
      Top             =   1200
      Width           =   1215
   End
   Begin VB.TextBox txtContent 
      Height          =   975
      Left            =   1440
      MultiLine       =   -1  'True
      TabIndex        =   11
      Text            =   "frmMain.frx":0000
      Top             =   3000
      Width           =   2295
   End
   Begin VB.TextBox txtTel 
      Height          =   375
      Left            =   1440
      TabIndex        =   10
      Top             =   2520
      Width           =   2295
   End
   Begin VB.TextBox txtPassword 
      Height          =   375
      IMEMode         =   3  'DISABLE
      Left            =   1440
      PasswordChar    =   "*"
      TabIndex        =   7
      Top             =   1680
      Width           =   2295
   End
   Begin VB.TextBox txtName 
      Height          =   375
      Left            =   1440
      TabIndex        =   6
      Top             =   1200
      Width           =   2295
   End
   Begin VB.TextBox txtPort 
      Height          =   375
      Left            =   1440
      TabIndex        =   3
      Top             =   720
      Width           =   2295
   End
   Begin VB.TextBox txtServer 
      Height          =   375
      Left            =   1440
      TabIndex        =   1
      Top             =   240
      Width           =   2295
   End
   Begin VB.Label Label6 
      Caption         =   "���ݣ�"
      Height          =   255
      Left            =   720
      TabIndex        =   9
      Top             =   3000
      Width           =   615
   End
   Begin VB.Label Label5 
      Caption         =   "�ֻ����룺"
      Height          =   255
      Left            =   360
      TabIndex        =   8
      Top             =   2640
      Width           =   975
   End
   Begin VB.Label Label4 
      Caption         =   "���룺"
      Height          =   255
      Left            =   720
      TabIndex        =   5
      Top             =   1800
      Width           =   615
   End
   Begin VB.Label Label3 
      Caption         =   "��½�ʺţ�"
      Height          =   255
      Left            =   360
      TabIndex        =   4
      Top             =   1320
      Width           =   975
   End
   Begin VB.Label Label2 
      Caption         =   "ͨѶ�˿ڣ�"
      Height          =   255
      Left            =   360
      TabIndex        =   2
      Top             =   840
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "��������ַ��"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   360
      Width           =   1095
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdConnect_Click()
    Dim strServer As String, strName As String, strPassword As String, nPort As Long
    
    strServer = txtServer.Text
    strName = txtName.Text
    strPassword = txtPassword.Text
    nPort = CInt(txtPort.Text)
    
    connect_to_server strServer, nPort, strName, strPassword
    
End Sub

Private Sub cmdDisconnect_Click()
    disconnect
End Sub

Private Sub cmdSend_Click()
    Dim strTel As String, strContent As String
    Dim nRet As Long
    Dim strInfo As String
        
    strTel = txtTel.Text
    strContent = txtContent.Text
    
    nRet = send_message(strTel, strContent, "vbtag")
    
    If (nRet <> 0) Then
        strInfo = "���͵��������ɹ�"
    Else
        strInfo = "���͵�������ʧ�ܣ������룺" & CStr(get_last_error_code())
    End If
    
    MsgBox strInfo, vbInformation + vbOKOnly, "��ʾ��Ϣ"
    
End Sub

Private Sub Form_Load()
    txtServer.Text = "127.0.0.1"
    txtPort.Text = "1234"
    txtName.Text = "admin"
    txtPassword.Text = "8888"
    txtTel.Text = "10086"
    txtContent.Text = "11"
    
    set_callback AddressOf SmsCallbackProc
    
End Sub

