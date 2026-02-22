object HHCurrentForm: THHCurrentForm
  Left = 0
  Top = 0
  Caption = 'HHCurrentForm'
  ClientHeight = 692
  ClientWidth = 530
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 309
    Top = 35
    Width = 26
    Height = 13
    Caption = #181'S '#177
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 213
    Top = 35
    Width = 33
    Height = 13
    Caption = 'Gmax'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 309
    Top = 65
    Width = 18
    Height = 13
    Caption = 'mV'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 217
    Top = 65
    Width = 25
    Height = 13
    Caption = 'Erev'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 295
    Top = 101
    Width = 175
    Height = 13
    Caption = 'PRESS ENTER TO UPDATE PLOTS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 389
    Top = 35
    Width = 13
    Height = 13
    Caption = '%'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label8: TLabel
    Left = 12
    Top = 32
    Width = 182
    Height = 13
    Caption = 'Activiation/Inactivation Params'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object GroupBox1: TGroupBox
    Left = 9
    Top = 51
    Width = 185
    Height = 210
    Caption = 'm'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentColor = False
    ParentFont = False
    TabOrder = 0
    object Panel1: TPanel
      Left = 8
      Top = 16
      Width = 170
      Height = 190
      BevelInner = bvRaised
      BevelKind = bkTile
      Caption = 'Panel1'
      TabOrder = 0
      object ValueListEditor_m: TValueListEditor
        Left = 8
        Top = 10
        Width = 150
        Height = 170
        GradientEndColor = clTeal
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goEditing, goThumbTracking]
        Strings.Strings = (
          '')
        TabOrder = 0
        TitleCaptions.Strings = (
          'Parameter'
          'Value'
          '')
        OnKeyPress = ParamsEditors_KeyPress
        ColWidths = (
          95
          49)
      end
    end
  end
  object GroupBox2: TGroupBox
    Left = 9
    Top = 267
    Width = 185
    Height = 210
    Caption = 'h'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentColor = False
    ParentFont = False
    TabOrder = 1
    object Panel2: TPanel
      Left = 8
      Top = 16
      Width = 170
      Height = 190
      BevelInner = bvRaised
      BevelKind = bkTile
      Caption = 'Panel1'
      TabOrder = 0
      object ValueListEditor_h: TValueListEditor
        Left = 8
        Top = 8
        Width = 150
        Height = 170
        Strings.Strings = (
          '')
        TabOrder = 0
        TitleCaptions.Strings = (
          'Parameter'
          'Value'
          '')
        OnKeyPress = ParamsEditors_KeyPress
        ColWidths = (
          97
          47)
      end
    end
  end
  object GmaxEdit: TEdit
    Left = 254
    Top = 32
    Width = 49
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    Text = '0'
    OnKeyPress = ParamsEditors_KeyPress
  end
  object ErevEdit: TEdit
    Left = 254
    Top = 62
    Width = 49
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
    Text = '0'
    OnKeyPress = ParamsEditors_KeyPress
  end
  object LoadButton: TButton
    Left = 442
    Top = 35
    Width = 75
    Height = 25
    Caption = 'Load Params'
    TabOrder = 4
    OnClick = LoadButtonClick
  end
  object SaveButton: TButton
    Left = 442
    Top = 60
    Width = 75
    Height = 25
    Caption = 'Save Params'
    TabOrder = 5
    OnClick = SaveButtonClick
  end
  object MultiPLOTPanel1: TMultiPLOTPanel
    Left = 217
    Top = 116
    Width = 307
    Height = 563
    Cursor = crVSplit
    PLOTPanel0.Left = 0
    PLOTPanel0.Top = 0
    PLOTPanel0.Width = 307
    PLOTPanel0.Height = 183
    PLOTPanel0.Hint = 'This is the steady state IV curve'
    PLOTPanel0.FullScaleMinY = -100.000000000000000000
    PLOTPanel0.VisibleMinY = -100.000000000000000000
    PLOTPanel0.FullScaleMaxY = 100.000000000000000000
    PLOTPanel0.VisibleMaxY = 100.000000000000000000
    PLOTPanel0.FullScaleMaxX = 600.000000000000000000
    PLOTPanel0.VisibleMaxX = 1.000000000000000000
    PLOTPanel0.MouseMode = XZoom
    PLOTPanel0.BackgroundColor = clSilver
    PLOTPanel0.AxesColor = 15790320
    PLOTPanel0.GridLinesColor = clMedGray
    PLOTPanel0.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel0.AxesFont.Color = clBlack
    PLOTPanel0.AxesFont.Height = 14
    PLOTPanel0.AxesFont.Name = 'Arial'
    PLOTPanel0.AxesFont.Style = [fsBold]
    PLOTPanel0.YUnits = 'I-V  (nA)'
    PLOTPanel0.XUnits = 'mV'
    PLOTPanel0.TimeLocked = True
    PLOTPanel0.DrawEraseExtra = 1
    PLOTPanel0.ConcatenatePlots = True
    PLOTPanel0.DesignSize = (
      307
      183)
    PLOTPanel1.Tag = 1
    PLOTPanel1.Left = 0
    PLOTPanel1.Top = 188
    PLOTPanel1.Width = 307
    PLOTPanel1.Height = 182
    PLOTPanel1.Hint = 'These are color coded steady state values'
    PLOTPanel1.FullScaleMinY = -100.000000000000000000
    PLOTPanel1.VisibleMinY = -100.000000000000000000
    PLOTPanel1.FullScaleMaxY = 100.000000000000000000
    PLOTPanel1.VisibleMaxY = 100.000000000000000000
    PLOTPanel1.FullScaleMaxX = 600.000000000000000000
    PLOTPanel1.VisibleMaxX = 1.000000000000000000
    PLOTPanel1.MouseMode = XZoom
    PLOTPanel1.BackgroundColor = clSilver
    PLOTPanel1.AxesColor = 15790320
    PLOTPanel1.GridLinesColor = clMedGray
    PLOTPanel1.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel1.AxesFont.Color = clBlack
    PLOTPanel1.AxesFont.Height = 14
    PLOTPanel1.AxesFont.Name = 'Arial'
    PLOTPanel1.AxesFont.Style = [fsBold]
    PLOTPanel1.YUnits = 'ss (m,h,n)'
    PLOTPanel1.XUnits = 'mV'
    PLOTPanel1.TimeLocked = True
    PLOTPanel1.DrawEraseExtra = 1
    PLOTPanel1.ConcatenatePlots = True
    PLOTPanel1.DesignSize = (
      307
      182)
    PLOTPanel2.Tag = 2
    PLOTPanel2.Left = 0
    PLOTPanel2.Top = 375
    PLOTPanel2.Width = 307
    PLOTPanel2.Height = 188
    PLOTPanel2.Hint = 'These are color coded relaxation time constants'
    PLOTPanel2.FullScaleMinY = -100.000000000000000000
    PLOTPanel2.VisibleMinY = -100.000000000000000000
    PLOTPanel2.FullScaleMaxY = 100.000000000000000000
    PLOTPanel2.VisibleMaxY = 100.000000000000000000
    PLOTPanel2.FullScaleMaxX = 600.000000000000000000
    PLOTPanel2.VisibleMaxX = 1.000000000000000000
    PLOTPanel2.MouseMode = XZoom
    PLOTPanel2.BackgroundColor = clSilver
    PLOTPanel2.AxesColor = 15790320
    PLOTPanel2.GridLinesColor = clMedGray
    PLOTPanel2.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel2.AxesFont.Color = clBlack
    PLOTPanel2.AxesFont.Height = 14
    PLOTPanel2.AxesFont.Name = 'Arial'
    PLOTPanel2.AxesFont.Style = [fsBold]
    PLOTPanel2.YUnits = 'tau (m,h,n)'
    PLOTPanel2.XUnits = 'mV'
    PLOTPanel2.TimeLocked = True
    PLOTPanel2.DrawEraseExtra = 1
    PLOTPanel2.ConcatenatePlots = True
    PLOTPanel2.DesignSize = (
      307
      188)
    PLOTPanel3.Tag = 3
    PLOTPanel3.Left = 0
    PLOTPanel3.Top = 0
    PLOTPanel3.Width = 0
    PLOTPanel3.Height = 0
    PLOTPanel3.FullScaleMinY = -100.000000000000000000
    PLOTPanel3.VisibleMinY = -100.000000000000000000
    PLOTPanel3.FullScaleMaxY = 100.000000000000000000
    PLOTPanel3.VisibleMaxY = 100.000000000000000000
    PLOTPanel3.FullScaleMaxX = 60.000000000000000000
    PLOTPanel3.VisibleMaxX = 1.000000000000000000
    PLOTPanel3.MouseMode = XZoom
    PLOTPanel3.BackgroundColor = clSilver
    PLOTPanel3.AxesColor = 15790320
    PLOTPanel3.GridLinesColor = clMedGray
    PLOTPanel3.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel3.AxesFont.Color = clBlack
    PLOTPanel3.AxesFont.Height = 14
    PLOTPanel3.AxesFont.Name = 'Arial'
    PLOTPanel3.AxesFont.Style = [fsBold]
    PLOTPanel3.DrawEraseExtra = 1
    PLOTPanel3.DesignSize = (
      0
      0)
    PLOTPanel4.Tag = 4
    PLOTPanel4.Left = 0
    PLOTPanel4.Top = 0
    PLOTPanel4.Width = 0
    PLOTPanel4.Height = 0
    PLOTPanel4.FullScaleMinY = -100.000000000000000000
    PLOTPanel4.VisibleMinY = -100.000000000000000000
    PLOTPanel4.FullScaleMaxY = 100.000000000000000000
    PLOTPanel4.VisibleMaxY = 100.000000000000000000
    PLOTPanel4.FullScaleMaxX = 60.000000000000000000
    PLOTPanel4.VisibleMaxX = 1.000000000000000000
    PLOTPanel4.MouseMode = XZoom
    PLOTPanel4.BackgroundColor = clSilver
    PLOTPanel4.AxesColor = 15790320
    PLOTPanel4.GridLinesColor = clMedGray
    PLOTPanel4.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel4.AxesFont.Color = clBlack
    PLOTPanel4.AxesFont.Height = 14
    PLOTPanel4.AxesFont.Name = 'Arial'
    PLOTPanel4.AxesFont.Style = [fsBold]
    PLOTPanel4.DrawEraseExtra = 1
    PLOTPanel4.DesignSize = (
      0
      0)
    PLOTPanel5.Tag = 5
    PLOTPanel5.Left = 0
    PLOTPanel5.Top = 0
    PLOTPanel5.Width = 0
    PLOTPanel5.Height = 0
    PLOTPanel5.FullScaleMinY = -100.000000000000000000
    PLOTPanel5.VisibleMinY = -100.000000000000000000
    PLOTPanel5.FullScaleMaxY = 100.000000000000000000
    PLOTPanel5.VisibleMaxY = 100.000000000000000000
    PLOTPanel5.FullScaleMaxX = 60.000000000000000000
    PLOTPanel5.VisibleMaxX = 1.000000000000000000
    PLOTPanel5.MouseMode = XZoom
    PLOTPanel5.BackgroundColor = clSilver
    PLOTPanel5.AxesColor = 15790320
    PLOTPanel5.GridLinesColor = clMedGray
    PLOTPanel5.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel5.AxesFont.Color = clBlack
    PLOTPanel5.AxesFont.Height = 14
    PLOTPanel5.AxesFont.Name = 'Arial'
    PLOTPanel5.AxesFont.Style = [fsBold]
    PLOTPanel5.DrawEraseExtra = 1
    PLOTPanel5.DesignSize = (
      0
      0)
    PLOTPanel6.Tag = 6
    PLOTPanel6.Left = 0
    PLOTPanel6.Top = 0
    PLOTPanel6.Width = 0
    PLOTPanel6.Height = 0
    PLOTPanel6.FullScaleMinY = -100.000000000000000000
    PLOTPanel6.VisibleMinY = -100.000000000000000000
    PLOTPanel6.FullScaleMaxY = 100.000000000000000000
    PLOTPanel6.VisibleMaxY = 100.000000000000000000
    PLOTPanel6.FullScaleMaxX = 60.000000000000000000
    PLOTPanel6.VisibleMaxX = 1.000000000000000000
    PLOTPanel6.MouseMode = XZoom
    PLOTPanel6.BackgroundColor = clSilver
    PLOTPanel6.AxesColor = 15790320
    PLOTPanel6.GridLinesColor = clMedGray
    PLOTPanel6.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel6.AxesFont.Color = clBlack
    PLOTPanel6.AxesFont.Height = 14
    PLOTPanel6.AxesFont.Name = 'Arial'
    PLOTPanel6.AxesFont.Style = [fsBold]
    PLOTPanel6.DrawEraseExtra = 1
    PLOTPanel6.DesignSize = (
      0
      0)
    PLOTPanel7.Tag = 7
    PLOTPanel7.Left = 0
    PLOTPanel7.Top = 0
    PLOTPanel7.Width = 0
    PLOTPanel7.Height = 0
    PLOTPanel7.FullScaleMinY = -100.000000000000000000
    PLOTPanel7.VisibleMinY = -100.000000000000000000
    PLOTPanel7.FullScaleMaxY = 100.000000000000000000
    PLOTPanel7.VisibleMaxY = 100.000000000000000000
    PLOTPanel7.FullScaleMaxX = 60.000000000000000000
    PLOTPanel7.VisibleMaxX = 1.000000000000000000
    PLOTPanel7.MouseMode = XZoom
    PLOTPanel7.BackgroundColor = clSilver
    PLOTPanel7.AxesColor = 15790320
    PLOTPanel7.GridLinesColor = clMedGray
    PLOTPanel7.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel7.AxesFont.Color = clBlack
    PLOTPanel7.AxesFont.Height = 14
    PLOTPanel7.AxesFont.Name = 'Arial'
    PLOTPanel7.AxesFont.Style = [fsBold]
    PLOTPanel7.DrawEraseExtra = 1
    PLOTPanel7.DesignSize = (
      0
      0)
    SplitterPositions = <
      item
      end
      item
        Value = 0.333333343267440800
      end
      item
        Value = 0.666666686534881600
      end
      item
        Value = 1.000000000000000000
      end
      item
        Value = 1.000000000000000000
      end
      item
        Value = 1.000000000000000000
      end
      item
        Value = 1.000000000000000000
      end
      item
        Value = 1.000000000000000000
      end>
    SplitterColor = clBlack
    NumVisiblePlots = 3
    GlobalChanges = True
    MouseMode = XZoom
    DrawEraseExtra = 0
    Color = clBlack
    object Label7: TLabel
      Left = 132
      Top = 83
      Width = 175
      Height = 13
      Caption = 'PRESS ENTER TO UPDATE PLOTS'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object GroupBox3: TGroupBox
    Left = 9
    Top = 483
    Width = 185
    Height = 210
    Caption = 'n'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentColor = False
    ParentFont = False
    TabOrder = 7
    object Panel3: TPanel
      Left = 8
      Top = 16
      Width = 170
      Height = 190
      BevelInner = bvRaised
      BevelKind = bkTile
      Caption = 'Panel1'
      TabOrder = 0
      object ValueListEditor_n: TValueListEditor
        Left = 8
        Top = 8
        Width = 150
        Height = 170
        Strings.Strings = (
          '')
        TabOrder = 0
        TitleCaptions.Strings = (
          'Parameter'
          'Value'
          '')
        OnKeyPress = ParamsEditors_KeyPress
        ColWidths = (
          101
          43)
      end
    end
  end
  object GnoiseEdit: TEdit
    Left = 341
    Top = 32
    Width = 48
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    Text = '0'
    OnKeyPress = ParamsEditors_KeyPress
  end
  object Panel4: TPanel
    Left = 201
    Top = 32
    Width = 10
    Height = 654
    BevelInner = bvRaised
    TabOrder = 9
  end
  object Panel5: TPanel
    Left = 217
    Top = 101
    Width = 307
    Height = 13
    BevelInner = bvRaised
    TabOrder = 10
  end
  object PeriodicCheckBox: TCheckBox
    Left = 341
    Top = 76
    Width = 54
    Height = 17
    Caption = 'Periodic'
    TabOrder = 11
  end
  object PeriodEditButton: TButton
    Left = 401
    Top = 75
    Width = 22
    Height = 20
    Caption = '...'
    TabOrder = 12
    OnClick = PeriodEditButtonClick
  end
  object ParamLoggingCheckBox: TCheckBox
    Left = 128
    Top = 4
    Width = 234
    Height = 17
    Caption = 'Enable Parameter Logging During Run'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 13
  end
  object Panel6: TPanel
    Left = 8
    Top = 24
    Width = 517
    Height = 5
    BevelInner = bvRaised
    TabOrder = 14
  end
  object UseVdrvComboBox: TCheckBox
    Left = 217
    Top = 84
    Width = 118
    Height = 17
    Caption = 'Use Vdrv?'
    Checked = True
    State = cbChecked
    TabOrder = 15
    Visible = False
    OnClick = UseVdrvComboBoxClick
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'hhp'
    Filter = 'Hodgkin-Huxley Parameter Files|*.hhp'
    Title = 'Open Hodgkin-Huxley Type Current Parameter File'
    Left = 428
    Top = 488
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'hhp'
    Filter = 'Hodgkin-Huxley Parameter Files|*.hhp'
    Title = 'Save Hodgkin-Huxley Type Current Parameter File'
    Left = 428
    Top = 536
  end
end
