object CopyCurrentsForm: TCopyCurrentsForm
  Left = 0
  Top = 0
  Caption = 'Copy Currents'
  ClientHeight = 229
  ClientWidth = 375
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
    Left = 12
    Top = 12
    Width = 99
    Height = 13
    Caption = 'Copy Currents from:'
  end
  object Label2: TLabel
    Left = 176
    Top = 9
    Width = 133
    Height = 13
    Caption = 'Copy Currents To Target(s)'
  end
  object CopyFromComboBox: TComboBox
    Left = 8
    Top = 28
    Width = 145
    Height = 21
    TabOrder = 0
    Text = 'CopyFromComboBox'
  end
  object CopyToListBox: TListBox
    Left = 172
    Top = 28
    Width = 189
    Height = 167
    ItemHeight = 13
    MultiSelect = True
    TabOrder = 1
  end
  object Button1: TButton
    Left = 36
    Top = 156
    Width = 75
    Height = 25
    Caption = 'Copy'
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 36
    Top = 187
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object ToNamesListBox: TListBox
    Left = 20
    Top = 53
    Width = 121
    Height = 97
    ItemHeight = 13
    TabOrder = 4
    Visible = False
  end
  object ClearThenCopyCheckBox: TCheckBox
    Left = 160
    Top = 204
    Width = 201
    Height = 17
    Caption = 'Clear target currents before copying'
    TabOrder = 5
  end
end
