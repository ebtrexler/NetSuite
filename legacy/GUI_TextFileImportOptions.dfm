object TextFileImportOptionsForm: TTextFileImportOptionsForm
  Left = 0
  Top = 0
  Caption = 'Text File Import Options'
  ClientHeight = 69
  ClientWidth = 354
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 4
    Top = 6
    Width = 252
    Height = 55
    Caption = 'File Description'
    TabOrder = 0
    object Label1: TLabel
      Left = 139
      Top = 9
      Width = 84
      Height = 13
      Caption = 'Sample Rate (ms)'
    end
    object FirstColumnTimeCheckBox: TCheckBox
      Left = 10
      Top = 16
      Width = 105
      Height = 17
      Caption = 'First Column Time'
      TabOrder = 0
      OnClick = FirstColumnTimeCheckBoxClick
    end
    object FirstRowHeaderCheckBox: TCheckBox
      Left = 10
      Top = 35
      Width = 104
      Height = 17
      Caption = 'First Row Header'
      TabOrder = 1
    end
    object SampleRateEdit: TEdit
      Left = 142
      Top = 28
      Width = 77
      Height = 21
      Alignment = taRightJustify
      TabOrder = 2
      Text = '1'
    end
  end
  object Button1: TButton
    Left = 268
    Top = 6
    Width = 75
    Height = 25
    Caption = 'Load File'
    ModalResult = 1
    TabOrder = 1
  end
  object Button2: TButton
    Left = 268
    Top = 37
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
