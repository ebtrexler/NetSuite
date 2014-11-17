object GJCurrentForm: TGJCurrentForm
  Left = 0
  Top = 0
  Caption = 'GJCurrentForm'
  ClientHeight = 31
  ClientWidth = 294
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 201
    Height = 13
    Caption = 'Instantaneous Gap Junction Conductance'
  end
  object Label2: TLabel
    Left = 277
    Top = 8
    Width = 12
    Height = 13
    Caption = #181'S'
  end
  object Edit1: TEdit
    Left = 212
    Top = 5
    Width = 59
    Height = 21
    Alignment = taRightJustify
    TabOrder = 0
    Text = '1.00'
    OnKeyPress = Edit1KeyPress
  end
end
