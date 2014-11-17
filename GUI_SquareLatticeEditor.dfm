object GUI_SquareLatticeForm: TGUI_SquareLatticeForm
  Left = 0
  Top = 0
  Caption = 'GUI_SquareLatticeForm'
  ClientHeight = 381
  ClientWidth = 382
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ScrollBox1: TScrollBox
    Left = 0
    Top = 0
    Width = 382
    Height = 381
    Align = alClient
    Padding.Left = 5
    Padding.Top = 5
    Padding.Right = 5
    Padding.Bottom = 5
    TabOrder = 0
    object NetworkPaintBox32: TPaintBox32
      Left = 8
      Top = 8
      Width = 361
      Height = 361
      Options = [pboWantArrowKeys, pboAutoFocus]
      RepaintMode = rmDirect
      TabOrder = 0
      TabStop = True
      OnMouseDown = NetworkPaintBox32MouseDown
      OnMouseMove = NetworkPaintBox32MouseMove
    end
  end
end
