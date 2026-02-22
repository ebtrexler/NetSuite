object InjectionElectrodeForm: TInjectionElectrodeForm
  Left = 0
  Top = 0
  Caption = 'InjectionElectrodeForm'
  ClientHeight = 349
  ClientWidth = 300
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
    Top = 12
    Width = 261
    Height = 13
    Caption = 'This electrode injects a simple square pulse of current.'
  end
  object ValueListEditor1: TValueListEditor
    Left = 16
    Top = 31
    Width = 273
    Height = 300
    Strings.Strings = (
      '')
    TabOrder = 0
    TitleCaptions.Strings = (
      'Parameter'
      'Value'
      '')
    OnKeyPress = ValueListEditor1KeyPress
    ColWidths = (
      150
      117)
  end
end
