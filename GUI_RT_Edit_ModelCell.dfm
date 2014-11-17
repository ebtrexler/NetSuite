object ModelCellForm: TModelCellForm
  Left = 0
  Top = 0
  Caption = 'ModelCellForm'
  ClientHeight = 57
  ClientWidth = 189
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
    Top = 7
    Width = 59
    Height = 13
    Caption = 'Capacitance'
  end
  object Label2: TLabel
    Left = 164
    Top = 7
    Width = 12
    Height = 13
    Caption = 'nF'
  end
  object Label3: TLabel
    Left = 8
    Top = 32
    Width = 83
    Height = 13
    Caption = 'Starting Potential'
  end
  object Label4: TLabel
    Left = 164
    Top = 32
    Width = 14
    Height = 13
    Caption = 'mV'
  end
  object CapacitanceEdit: TEdit
    Left = 97
    Top = 5
    Width = 53
    Height = 21
    TabOrder = 0
    Text = '5'
    OnKeyPress = EditKeyPress
  end
  object InitialVmEdit: TEdit
    Left = 97
    Top = 29
    Width = 53
    Height = 21
    TabOrder = 1
    Text = '-60'
    OnKeyPress = EditKeyPress
  end
end
