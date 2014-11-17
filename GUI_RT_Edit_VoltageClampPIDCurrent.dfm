object VoltageClamp_PID_CurrentForm: TVoltageClamp_PID_CurrentForm
  Left = 0
  Top = 0
  Caption = 'VoltageClamp_PID_CurrentForm'
  ClientHeight = 196
  ClientWidth = 333
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
    Left = 24
    Top = 45
    Width = 58
    Height = 13
    Caption = 'Proportional'
  end
  object Label2: TLabel
    Left = 24
    Top = 72
    Width = 38
    Height = 13
    Caption = 'Integral'
  end
  object Label3: TLabel
    Left = 24
    Top = 100
    Width = 53
    Height = 13
    Caption = 'Differential'
  end
  object Label4: TLabel
    Left = 148
    Top = 16
    Width = 26
    Height = 13
    Caption = 'Gains'
  end
  object Label7: TLabel
    Left = 182
    Top = 72
    Width = 20
    Height = 13
    Caption = 'max'
  end
  object Label8: TLabel
    Left = 256
    Top = 72
    Width = 16
    Height = 13
    Caption = 'min'
  end
  object PGainEdit: TEdit
    Left = 104
    Top = 42
    Width = 69
    Height = 21
    TabOrder = 0
    Text = 'PGainEdit'
  end
  object IGainEdit: TEdit
    Left = 104
    Top = 69
    Width = 69
    Height = 21
    TabOrder = 1
    Text = 'IGainEdit'
  end
  object DGainEdit: TEdit
    Left = 104
    Top = 97
    Width = 69
    Height = 21
    TabOrder = 2
    Text = 'DGainEdit'
  end
  object Panel1: TPanel
    Left = 24
    Top = 135
    Width = 265
    Height = 53
    TabOrder = 3
    object Label5: TLabel
      Left = 168
      Top = 8
      Width = 42
      Height = 13
      Caption = 'Tau (ms)'
    end
    object Label6: TLabel
      Left = 12
      Top = 8
      Width = 111
      Height = 13
      Caption = 'Command Voltage (mV)'
    end
    object VCommandEdit: TEdit
      Left = 12
      Top = 27
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'VCommandEdit'
    end
  end
  object TauEdit: TEdit
    Left = 192
    Top = 162
    Width = 85
    Height = 21
    TabOrder = 4
    Text = 'TauEdit'
  end
  object ImaxEdit: TEdit
    Left = 208
    Top = 69
    Width = 41
    Height = 21
    TabOrder = 5
    Text = 'ImaxEdit'
  end
  object IminEdit: TEdit
    Left = 278
    Top = 69
    Width = 41
    Height = 21
    TabOrder = 6
    Text = 'Edit1'
  end
end
