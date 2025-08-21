object ReprogramForm: TReprogramForm
  Left = 0
  Top = 0
  Caption = 'VTrex Biomedical Firmware Updater'
  ClientHeight = 341
  ClientWidth = 554
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 10
    Top = 5
    Width = 85
    Height = 13
    Caption = 'Hex File Contents'
  end
  object Memo1: TMemo
    Left = 8
    Top = 23
    Width = 358
    Height = 301
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'Open Hex file to send...')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Close: TButton
    Left = 429
    Top = 299
    Width = 75
    Height = 25
    Caption = 'Close'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 375
    Top = 8
    Width = 175
    Height = 279
    Caption = 'GroupBox1'
    TabOrder = 2
    object Filename: TLabel
      Left = 21
      Top = 81
      Width = 42
      Height = 13
      Caption = 'Filename'
    end
    object Label1: TLabel
      Left = 21
      Top = 103
      Width = 3
      Height = 13
    end
    object Label2: TLabel
      Left = 21
      Top = 132
      Width = 44
      Height = 13
      Caption = 'Com Port'
    end
    object Label4: TLabel
      Left = 10
      Top = 227
      Width = 42
      Height = 13
      Caption = 'Progress'
    end
    object Open: TButton
      Left = 47
      Top = 33
      Width = 118
      Height = 25
      Caption = 'Open Hex File'
      TabOrder = 0
      OnClick = OpenClick
    end
    object Convert: TButton
      Left = 47
      Top = 196
      Width = 118
      Height = 25
      Caption = 'Update Firmware'
      TabOrder = 1
      OnClick = ConvertClick
    end
    object ComPortChooser: TComboBox
      Left = 20
      Top = 147
      Width = 145
      Height = 21
      ItemIndex = 0
      TabOrder = 2
      Text = '1'
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9'
        '10'
        '11'
        '12')
    end
    object ProgressBar1: TProgressBar
      Left = 10
      Top = 243
      Width = 150
      Height = 17
      Max = 10
      TabOrder = 3
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'hex'
    Filter = 'hex files|*.hex'
    Title = 'Open Hex file'
    Left = 503
    Top = 3
  end
end
