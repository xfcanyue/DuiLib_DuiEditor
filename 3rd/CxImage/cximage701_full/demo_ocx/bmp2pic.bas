Attribute VB_Name = "bmp2picbox"
Option Explicit

Private Type PictDesc
    cbSizeofStruct As Long
    picType As Long
    hImage As Long
    xExt As Long
    yExt As Long
End Type
Private Type Guid
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(0 To 7) As Byte
End Type
Private Declare Function OleCreatePictureIndirect Lib "olepro32.dll" (lpPictDesc As PictDesc, riid As Guid, ByVal fPictureOwnsHandle As Long, ipic As IPicture) As Long

'Karim El Bureiasi <k-l-b(at)web(dot)de>
'Dim hBmp As Long
'hBmp = AxImage1.MakeBitmap(hDC)
'Set picPreview.Picture = BitmapToPicture(hBmp)
'picPreview.Refresh

Public Function BitmapToPicture(ByVal hBmp As Long) As IPicture

    If (hBmp = 0) Then Exit Function
    
    Dim oNewPic As Picture, tPicConv As PictDesc, IGuid As Guid
    
    ' Fill PictDesc structure with necessary parts:
    With tPicConv
    .cbSizeofStruct = Len(tPicConv)
    .picType = vbPicTypeBitmap
    .hImage = hBmp
    End With
    
    ' Fill in IDispatch Interface ID
    With IGuid
    .Data1 = &H20400
    .Data4(0) = &HC0
    .Data4(7) = &H46
    End With
    
    ' Create a picture object:
    OleCreatePictureIndirect tPicConv, IGuid, True, oNewPic
    
    ' Return it:
    Set BitmapToPicture = oNewPic
    

End Function

