object BiologicalCellForm: TBiologicalCellForm
  Left = 0
  Top = 0
  Caption = 'Biological Cell Form'
  ClientHeight = 437
  ClientWidth = 299
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label6: TLabel
    Left = 16
    Top = 24
    Width = 109
    Height = 13
    Caption = 'Analog Input Channels'
  end
  object Label7: TLabel
    Left = 164
    Top = 24
    Width = 117
    Height = 13
    Caption = 'Analog Output Channels'
  end
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 81
    Height = 13
    Caption = 'Voltage Sampling'
  end
  object Label2: TLabel
    Left = 164
    Top = 8
    Width = 82
    Height = 13
    Caption = 'Current Injection'
  end
  object Label3: TLabel
    Left = 8
    Top = 315
    Width = 110
    Height = 13
    Caption = 'Gain ( cell mV / ADC V )'
  end
  object Label4: TLabel
    Left = 157
    Top = 315
    Width = 109
    Height = 13
    Caption = 'Gain ( DAC V / cell nA )'
  end
  object Label5: TLabel
    Left = 8
    Top = 215
    Width = 139
    Height = 13
    Caption = 'Amp Gain ( cell mV / Amp mV)'
  end
  object Label8: TLabel
    Left = 157
    Top = 215
    Width = 134
    Height = 13
    Caption = 'Ext. Command Sens. (nA/V)'
  end
  object Label9: TLabel
    Left = 59
    Top = 280
    Width = 181
    Height = 13
    Caption = 'PRESS ENTER TO CONVERT GAINS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Panel1: TPanel
    Left = 4
    Top = 208
    Width = 293
    Height = 157
    BevelInner = bvRaised
    TabOrder = 6
  end
  object lbxAIChannels: TListBox
    Left = 16
    Top = 40
    Width = 121
    Height = 157
    ItemHeight = 13
    TabOrder = 0
    OnClick = lbxAIAOChannelsClick
  end
  object lbxAOChannels: TListBox
    Left = 164
    Top = 40
    Width = 121
    Height = 157
    Color = clBtnHighlight
    ItemHeight = 13
    TabOrder = 1
    OnClick = lbxAIAOChannelsClick
  end
  object AIGainEdit: TEdit
    Left = 16
    Top = 334
    Width = 121
    Height = 21
    TabOrder = 2
    Text = 'AIGainEdit'
    OnKeyPress = NetworkGainEditKeyPress
  end
  object AOGainEdit: TEdit
    Left = 164
    Top = 334
    Width = 121
    Height = 21
    TabOrder = 3
    Text = 'AOGainEdit'
    OnKeyPress = NetworkGainEditKeyPress
  end
  object ADCGainEdit: TEdit
    Left = 16
    Top = 234
    Width = 121
    Height = 21
    TabOrder = 4
    Text = 'ADCGainEdit'
    OnKeyPress = AmpGainEditKeyPress
  end
  object DACGainEdit: TEdit
    Left = 164
    Top = 234
    Width = 121
    Height = 21
    TabOrder = 5
    Text = 'DACGainEdit'
    OnKeyPress = AmpGainEditKeyPress
  end
  object Panel2: TPanel
    Left = 4
    Top = 372
    Width = 293
    Height = 65
    BevelInner = bvRaised
    TabOrder = 7
    object Label10: TLabel
      Left = 4
      Top = 24
      Width = 115
      Height = 13
      Caption = 'Current Injection Limits:'
    end
    object Label11: TLabel
      Left = 153
      Top = 12
      Width = 17
      Height = 13
      Caption = 'pos'
    end
    object Label12: TLabel
      Left = 152
      Top = 39
      Width = 18
      Height = 13
      Caption = 'neg'
    end
    object Label13: TLabel
      Left = 264
      Top = 12
      Width = 13
      Height = 13
      Caption = 'nA'
    end
    object Label14: TLabel
      Left = 264
      Top = 40
      Width = 13
      Height = 13
      Caption = 'nA'
    end
    object PosLimitEdit: TEdit
      Left = 176
      Top = 8
      Width = 86
      Height = 21
      TabOrder = 0
      Text = 'PosLimitEdit'
      OnKeyPress = PosNegLimitEditKeyPress
    end
    object NegLimitEdit: TEdit
      Left = 176
      Top = 35
      Width = 86
      Height = 21
      TabOrder = 1
      Text = 'NegLimitEdit'
      OnKeyPress = PosNegLimitEditKeyPress
    end
  end
end
