object DeviceDesc: TDeviceDesc
  Left = 0
  Top = 0
  Caption = 'Device Description'
  ClientHeight = 303
  ClientWidth = 460
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.SheetOfGlass = True
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 81
    Height = 13
    Caption = 'Installed Devices'
  end
  object Label2: TLabel
    Left = 176
    Top = 16
    Width = 109
    Height = 13
    Caption = 'Analog Input Channels'
  end
  object Label3: TLabel
    Left = 324
    Top = 13
    Width = 117
    Height = 13
    Caption = 'Analog Output Channels'
  end
  object Label4: TLabel
    Left = 24
    Top = 144
    Width = 104
    Height = 26
    Caption = 'Maximum Multiplexed Input Sample Rate:'
    WordWrap = True
  end
  object lblMaxMultAISampRate: TLabel
    Left = 24
    Top = 176
    Width = 110
    Height = 13
    Caption = 'lblMaxMultAISampRate'
  end
  object Label5: TLabel
    Left = 24
    Top = 208
    Width = 104
    Height = 26
    Caption = 'Maximum Multiplexed Output Sample Rate:'
    WordWrap = True
  end
  object lblMaxMultAOSampRate: TLabel
    Left = 24
    Top = 240
    Width = 114
    Height = 13
    Caption = 'lblMaxMultAOSampRate'
  end
  object Label6: TLabel
    Left = 176
    Top = 205
    Width = 121
    Height = 13
    Caption = 'Available Voltage Ranges'
  end
  object Label7: TLabel
    Left = 324
    Top = 144
    Width = 121
    Height = 13
    Caption = 'Available Voltage Ranges'
  end
  object lbxInstalledDevices: TListBox
    Left = 24
    Top = 32
    Width = 121
    Height = 97
    ItemHeight = 13
    TabOrder = 0
    OnClick = lbxInstalledDevicesClick
  end
  object lbxAIChannels: TListBox
    Left = 176
    Top = 32
    Width = 121
    Height = 157
    ItemHeight = 13
    TabOrder = 1
  end
  object lbxAOChannels: TListBox
    Left = 324
    Top = 32
    Width = 121
    Height = 97
    ItemHeight = 13
    ParentColor = True
    TabOrder = 2
  end
  object lbxAIVoltRngs: TListBox
    Left = 176
    Top = 224
    Width = 121
    Height = 71
    ItemHeight = 13
    TabOrder = 3
  end
  object lbxAOVoltRngs: TListBox
    Left = 324
    Top = 163
    Width = 121
    Height = 71
    ItemHeight = 13
    TabOrder = 4
  end
end
