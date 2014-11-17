object TypeAndNameDlg: TTypeAndNameDlg
  Left = 227
  Top = 108
  ActiveControl = TypesComboBox
  BorderStyle = bsDialog
  Caption = 'Input Cell Type and Name'
  ClientHeight = 145
  ClientWidth = 384
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 281
    Height = 129
    Shape = bsFrame
  end
  object TypeLabel: TLabel
    Left = 28
    Top = 21
    Width = 44
    Height = 13
    Caption = 'Cell Type'
  end
  object NameLabel: TLabel
    Left = 28
    Top = 77
    Width = 47
    Height = 13
    Caption = 'Cell Name'
  end
  object OKBtn: TButton
    Left = 300
    Top = 8
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 300
    Top = 38
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object TypesComboBox: TComboBox
    Left = 44
    Top = 40
    Width = 229
    Height = 21
    Style = csDropDownList
    TabOrder = 2
  end
  object Edit1: TEdit
    Left = 44
    Top = 96
    Width = 229
    Height = 21
    TabOrder = 3
    Text = 'NewCell'
  end
end
