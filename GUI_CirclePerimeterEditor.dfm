object GUI_CirclePerimeterForm: TGUI_CirclePerimeterForm
  Left = 0
  Top = 0
  Caption = 'GUI_CirclePerimeterForm'
  ClientHeight = 337
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object NetworkPaintBox32: TPaintBox32
    Left = 0
    Top = 0
    Width = 527
    Height = 337
    TabOrder = 0
    OnMouseDown = NetworkPaintBox32MouseDown
    OnMouseMove = NetworkPaintBox32MouseMove
    OnResize = NetworkPaintBox32Resize
  end
end
