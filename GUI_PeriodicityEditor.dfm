object PeriodicityForm: TPeriodicityForm
  Left = 0
  Top = 0
  Caption = 'Setup for Current Periodicity'
  ClientHeight = 303
  ClientWidth = 288
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
    Left = 16
    Top = 65
    Width = 179
    Height = 26
    Caption = 
      'Enter the Values for Each Parameter.  Duty Cycle applies to Puls' +
      'es only.'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 12
    Top = 20
    Width = 77
    Height = 13
    Caption = 'Waveform Type'
  end
  object ValueListEditor1: TValueListEditor
    Left = 8
    Top = 100
    Width = 273
    Height = 142
    Strings.Strings = (
      '')
    TabOrder = 0
    TitleCaptions.Strings = (
      'Parameter'
      'Value'
      '')
    ColWidths = (
      150
      117)
  end
  object OkButton: TButton
    Left = 44
    Top = 259
    Width = 75
    Height = 25
    Caption = 'Save Changes'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object CancelButton: TButton
    Left = 168
    Top = 259
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object WaveformTypeComboBox: TComboBox
    Left = 8
    Top = 32
    Width = 273
    Height = 21
    TabOrder = 3
  end
end
