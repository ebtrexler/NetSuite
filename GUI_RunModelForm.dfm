object RunDynamicClampForm: TRunDynamicClampForm
  Left = 0
  Top = 0
  Cursor = crHourGlass
  Caption = 'Run the model......'
  ClientHeight = 719
  ClientWidth = 864
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.Enabled = True
  OldCreateOrder = False
  ShowHint = True
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter3: TSplitter
    Left = 0
    Top = 709
    Width = 864
    Height = 7
    Cursor = crVSplit
    Hint = 'Move up to reveal dynamic clamp timing statistics'
    Align = alBottom
    Color = clHighlight
    ParentColor = False
    ExplicitTop = 504
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 175
    Height = 709
    Align = alLeft
    BevelInner = bvRaised
    BevelKind = bkSoft
    BorderWidth = 1
    BorderStyle = bsSingle
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    object Label1: TLabel
      Left = 3
      Top = 2
      Width = 86
      Height = 26
      Alignment = taCenter
      Caption = 'Sample Rate (per channel) (Hz)'
      WordWrap = True
    end
    object Label2: TLabel
      Left = 4
      Top = 160
      Width = 133
      Height = 13
      Caption = 'Time before dynamic clamp:'
    end
    object Label3: TLabel
      Left = 4
      Top = 199
      Width = 130
      Height = 13
      Caption = 'Duration of dynamic clamp:'
    end
    object Label4: TLabel
      Left = 5
      Top = 238
      Width = 125
      Height = 13
      Caption = 'Time after dynamic clamp:'
    end
    object Label5: TLabel
      Left = 3
      Top = 35
      Width = 79
      Height = 26
      Alignment = taCenter
      Caption = 'Coerced sample rate (Hz)'
      WordWrap = True
    end
    object CoercedSampleRateLabel: TLabel
      Left = 35
      Top = 47
      Width = 122
      Height = 13
      Alignment = taRightJustify
      Caption = 'CoercedSampleRateLabel'
    end
    object Label8: TLabel
      Left = 131
      Top = 178
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object Label9: TLabel
      Left = 131
      Top = 217
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object Label10: TLabel
      Left = 131
      Top = 257
      Width = 16
      Height = 13
      Caption = 'sec'
    end
    object Label25: TLabel
      Left = 4
      Top = 104
      Width = 130
      Height = 13
      Caption = 'Maximum rk4 timestep (ms)'
    end
    object SampleRateEdit: TEdit
      Left = 97
      Top = 8
      Width = 60
      Height = 21
      Hint = 'Requested sample rate per channel'
      TabOrder = 0
      Text = 'SampleRateEdit'
      OnKeyPress = EditKeyPress
    end
    object TimeBeforeEdit: TEdit
      Left = 4
      Top = 175
      Width = 121
      Height = 21
      Hint = 
        'Calculations but no current injection (This field ignored in sim' +
        'ulation only mode)'
      TabOrder = 2
      Text = 'TimeBeforeEdit'
    end
    object DurationEdit: TEdit
      Left = 4
      Top = 214
      Width = 121
      Height = 21
      Hint = 'Calculations AND current injection'
      TabOrder = 3
      Text = 'DurationEdit'
      OnKeyPress = EditKeyPress
    end
    object TimeAfterEdit: TEdit
      Left = 4
      Top = 254
      Width = 121
      Height = 21
      Hint = 
        'Calculations but no current injection (This field ignored in sim' +
        'ulation only mode)'
      TabOrder = 4
      Text = 'TimeAfterEdit'
    end
    object ActionToolBar1: TActionToolBar
      Left = 3
      Top = 612
      Width = 161
      Height = 86
      ActionManager = ActionManager1
      Align = alBottom
      Caption = 'ActionToolBar1'
      Color = clMenuBar
      ColorMap.DisabledFontColor = 7171437
      ColorMap.HighlightColor = clWhite
      ColorMap.BtnSelectedFont = clBlack
      ColorMap.UnusedColor = clWhite
      EdgeInner = esLowered
      EdgeOuter = esRaised
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Spacing = 0
    end
    object Panel7: TPanel
      Left = 0
      Top = 279
      Width = 164
      Height = 56
      BevelInner = bvRaised
      BevelKind = bkSoft
      TabOrder = 6
      object Label15: TLabel
        Left = 5
        Top = 27
        Width = 90
        Height = 13
        Caption = 'Number of repeats'
      end
      object ResetAfterRunCheckBox: TCheckBox
        Left = 5
        Top = 4
        Width = 152
        Height = 17
        Hint = 'If checked, initializes (resets) network between repeats'
        Caption = 'Reset After Runs/Repeats'
        TabOrder = 0
      end
      object RepeatEdit: TEdit
        Left = 101
        Top = 23
        Width = 54
        Height = 21
        Hint = 'Number of times to repeat.  Zero means until user presses STOP.'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = 'RepeatEdit'
      end
    end
    object Panel8: TPanel
      Left = 1
      Top = 335
      Width = 164
      Height = 46
      BevelInner = bvRaised
      BevelKind = bkSoft
      TabOrder = 7
      object SaveDataCheckBox: TCheckBox
        Left = 5
        Top = 4
        Width = 102
        Height = 34
        Hint = 
          'If checked, each run is saved to a new text file -- Must choose ' +
          'base file name.  Time info is added'
        Caption = 'Electrode + Cell Data Saved'
        TabOrder = 0
        WordWrap = True
        OnClick = SaveDataCheckBoxClick
      end
      object FileParamsButton: TButton
        Left = 103
        Top = 4
        Width = 51
        Height = 33
        Hint = 
          'Click to change file location -- Must choose base file name.  Ti' +
          'me info is added'
        Caption = 'Choose Location'
        TabOrder = 1
        WordWrap = True
        OnClick = FileParamsButtonClick
      end
    end
    object Panel9: TPanel
      Left = 3
      Top = 528
      Width = 164
      Height = 85
      BevelInner = bvRaised
      BevelKind = bkSoft
      TabOrder = 8
      object Label20: TLabel
        Left = 3
        Top = 4
        Width = 110
        Height = 13
        Caption = 'Output Data to Figure:'
      end
      object Label21: TLabel
        Left = 5
        Top = 56
        Width = 49
        Height = 13
        Caption = 'Plot every'
      end
      object Label24: TLabel
        Left = 107
        Top = 56
        Width = 43
        Height = 13
        Caption = 'nth point'
      end
      object CGM_Button: TButton
        Left = 80
        Top = 23
        Width = 75
        Height = 25
        Hint = 
          'All available plots are written to a text based CGM file, compat' +
          'ible with CorelDRAW'
        Caption = 'CGM'
        TabOrder = 1
        OnClick = CGM_ButtonClick
      end
      object CGMIncrementEdit: TEdit
        Left = 60
        Top = 52
        Width = 45
        Height = 21
        Hint = 'Choose every nth point'
        TabOrder = 0
        Text = '10'
      end
      object PS_Button: TButton
        Left = 4
        Top = 23
        Width = 70
        Height = 25
        Hint = 
          'If RALCGM is installed correctly, all available plots are writte' +
          'n to a cgm then translated to a postscript file'
        Caption = 'PS'
        TabOrder = 2
        OnClick = CGM_ButtonClick
      end
    end
    object Panel10: TPanel
      Left = 0
      Top = 94
      Width = 167
      Height = 4
      TabOrder = 9
    end
    object Panel11: TPanel
      Left = 0
      Top = 149
      Width = 167
      Height = 4
      TabOrder = 10
    end
    object MaxRK4TimestepEdit: TEdit
      Left = 4
      Top = 122
      Width = 121
      Height = 21
      TabOrder = 1
      Text = 'MaxRK4TimestepEdit'
      OnKeyPress = EditKeyPress
    end
    object Panel12: TPanel
      Left = 1
      Top = 428
      Width = 164
      Height = 99
      BevelInner = bvRaised
      BevelKind = bkSoft
      TabOrder = 11
      object Panel13: TPanel
        Left = 2
        Top = 2
        Width = 156
        Height = 91
        Align = alBottom
        Caption = 'Panel13'
        TabOrder = 0
        object Label26: TLabel
          Left = 1
          Top = 1
          Width = 154
          Height = 13
          Align = alTop
          Alignment = taCenter
          Caption = 'Channels for Data Logging'
          ExplicitWidth = 127
        end
        object DataLoggingListBox: TListBox
          Left = 1
          Top = 16
          Width = 154
          Height = 74
          Hint = 'Chooose AD Channels +/- 10 V full scale'
          Align = alBottom
          Anchors = [akLeft, akTop, akRight, akBottom]
          Columns = 2
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
          OnClick = DataLoggingListBoxClick
        end
      end
    end
    object InterpolateCheckBox: TCheckBox
      Left = 6
      Top = 67
      Width = 76
      Height = 26
      Hint = 'Interpolate acquired data?'
      Caption = 'Interpolate Data (Hz)'
      TabOrder = 12
      WordWrap = True
      OnClick = InterpolateCheckBoxClick
    end
    object InterpolateRateEdit: TEdit
      Left = 97
      Top = 72
      Width = 60
      Height = 21
      Hint = 
        'In dynamic clamp, acquired samples can be unevenly spaced.  Choo' +
        'se an appropriate interpolated sample rate.'
      TabOrder = 13
      Text = 'InterpolateRateEdit'
      OnKeyPress = InterpolateRateEditKeyPress
    end
    object Panel16: TPanel
      Left = 1
      Top = 381
      Width = 164
      Height = 45
      BevelInner = bvRaised
      BevelKind = bkSoft
      TabOrder = 14
      object LogKineticParamsCheckBox: TCheckBox
        Left = 3
        Top = 5
        Width = 77
        Height = 32
        Hint = 
          'If checked, each run is saved to a new text file -- Must choose ' +
          'base file name.  Time info is added'
        Caption = 'Log Kinetic Parameters'
        TabOrder = 0
        WordWrap = True
        OnClick = LogKineticParamsCheckBoxClick
      end
      object LogKineticParamsButton: TButton
        Left = 102
        Top = 4
        Width = 52
        Height = 33
        Hint = 
          'Click to change file location -- Must choose base file name.  Ti' +
          'me info is added'
        Caption = 'Choose Location'
        TabOrder = 1
        WordWrap = True
        OnClick = LogKineticParamsButtonClick
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 716
    Width = 864
    Height = 3
    Align = alBottom
    BevelInner = bvRaised
    BevelKind = bkSoft
    BorderWidth = 1
    BorderStyle = bsSingle
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    object Label6: TLabel
      Left = 8
      Top = 37
      Width = 42
      Height = 13
      Caption = 'Progress'
    end
    object Label7: TLabel
      Left = 8
      Top = 13
      Width = 93
      Height = 13
      Caption = 'Repeats remaining:'
    end
    object RunNumberLabel: TLabel
      Left = 107
      Top = 13
      Width = 81
      Height = 13
      Caption = 'RunNumberLabel'
    end
    object ProgressBar1: TProgressBar
      Left = 8
      Top = 56
      Width = 42
      Height = 153
      Orientation = pbVertical
      MarqueeInterval = 0
      TabOrder = 0
    end
    object Chart1: TChart
      Left = 383
      Top = 8
      Width = 386
      Height = 201
      BackImage.Inside = True
      Title.Text.Strings = (
        'Sample Time Histogram')
      Title.VertMargin = 0
      BottomAxis.Logarithmic = True
      BottomAxis.Title.Caption = 'less than or equal us per sample'
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.Maximum = 0.500000000000000000
      DepthAxis.Minimum = -0.500000000000000000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.Maximum = 0.500000000000000000
      DepthTopAxis.Minimum = -0.500000000000000000
      LeftAxis.Logarithmic = True
      LeftAxis.Title.Caption = 'Counts'
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      PrintMargins = (
        15
        11
        15
        11)
      ColorPaletteIndex = 13
      object Series1: TBarSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.ShapeStyle = fosRoundRectangle
        Marks.Visible = False
        Title = 'Sample Times'
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
      end
    end
    object GroupBox1: TGroupBox
      Left = 77
      Top = 28
      Width = 300
      Height = 157
      Caption = 'Run Performance'
      TabOrder = 2
      object Label11: TLabel
        Left = 19
        Top = 16
        Width = 179
        Height = 13
        Caption = 'Average Number of Samples Per Call:'
      end
      object AvgSampsLabel: TLabel
        Left = 20
        Top = 35
        Width = 3
        Height = 13
      end
      object Label13: TLabel
        Left = 92
        Top = 54
        Width = 106
        Height = 13
        Caption = 'Average us per Read:'
      end
      object Label14: TLabel
        Left = 78
        Top = 73
        Width = 120
        Height = 13
        Caption = 'Maximum us single Read:'
      end
      object AvgusDuringLabel: TLabel
        Left = 204
        Top = 54
        Width = 3
        Height = 13
      end
      object MaxusDuringLabel: TLabel
        Left = 204
        Top = 73
        Width = 3
        Height = 13
      end
      object Label17: TLabel
        Left = 20
        Top = 100
        Width = 178
        Height = 13
        Caption = 'Total Number of Samples Requested:'
      end
      object Label18: TLabel
        Left = 47
        Top = 119
        Width = 151
        Height = 13
        Caption = 'Total Number of Samples Read:'
      end
      object TotalSampsReqLabel: TLabel
        Left = 204
        Top = 100
        Width = 3
        Height = 13
      end
      object Label12: TLabel
        Left = 31
        Top = 138
        Width = 167
        Height = 13
        Caption = 'Total Number of Successful Reads:'
      end
      object TotalSampsReadLabel: TLabel
        Left = 204
        Top = 119
        Width = 3
        Height = 13
      end
      object TotalReadsLabel: TLabel
        Left = 204
        Top = 138
        Width = 3
        Height = 13
      end
    end
  end
  object MultiPLOTPanel1: TMultiPLOTPanel
    Left = 297
    Top = 0
    Width = 567
    Height = 709
    PLOTPanel0.Left = 0
    PLOTPanel0.Top = 0
    PLOTPanel0.Width = 0
    PLOTPanel0.Height = 478
    PLOTPanel0.FullScaleMinY = -100.000000000000000000
    PLOTPanel0.VisibleMinY = -100.000000000000000000
    PLOTPanel0.FullScaleMaxY = 100.000000000000000000
    PLOTPanel0.VisibleMaxY = 100.000000000000000000
    PLOTPanel0.FullScaleMaxX = 60.000000000000000000
    PLOTPanel0.VisibleMaxX = 1.000000000000000000
    PLOTPanel0.MouseMode = XZoom
    PLOTPanel0.BackgroundColor = clSilver
    PLOTPanel0.AxesColor = 15790320
    PLOTPanel0.GridLinesColor = clMedGray
    PLOTPanel0.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel0.AxesFont.Color = clBlack
    PLOTPanel0.AxesFont.Height = 16
    PLOTPanel0.AxesFont.Name = 'Arial'
    PLOTPanel0.AxesFont.Style = []
    PLOTPanel0.TimeLocked = True
    PLOTPanel0.DrawEraseExtra = 1
    PLOTPanel0.DesignSize = (
      0
      478)
    PLOTPanel1.Tag = 1
    PLOTPanel1.Left = 0
    PLOTPanel1.Top = 61
    PLOTPanel1.Width = 0
    PLOTPanel1.Height = 55
    PLOTPanel1.FullScaleMinY = -100.000000000000000000
    PLOTPanel1.VisibleMinY = -100.000000000000000000
    PLOTPanel1.FullScaleMaxY = 100.000000000000000000
    PLOTPanel1.VisibleMaxY = 100.000000000000000000
    PLOTPanel1.FullScaleMaxX = 60.000000000000000000
    PLOTPanel1.VisibleMaxX = 1.000000000000000000
    PLOTPanel1.MouseMode = XZoom
    PLOTPanel1.BackgroundColor = clSilver
    PLOTPanel1.AxesColor = 15790320
    PLOTPanel1.GridLinesColor = clMedGray
    PLOTPanel1.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel1.AxesFont.Color = clBlack
    PLOTPanel1.AxesFont.Height = 16
    PLOTPanel1.AxesFont.Name = 'Arial'
    PLOTPanel1.AxesFont.Style = []
    PLOTPanel1.TimeLocked = True
    PLOTPanel1.DrawEraseExtra = 1
    PLOTPanel1.DesignSize = (
      0
      55)
    PLOTPanel2.Tag = 2
    PLOTPanel2.Left = 0
    PLOTPanel2.Top = 121
    PLOTPanel2.Width = 0
    PLOTPanel2.Height = 56
    PLOTPanel2.FullScaleMinY = -100.000000000000000000
    PLOTPanel2.VisibleMinY = -100.000000000000000000
    PLOTPanel2.FullScaleMaxY = 100.000000000000000000
    PLOTPanel2.VisibleMaxY = 100.000000000000000000
    PLOTPanel2.FullScaleMaxX = 60.000000000000000000
    PLOTPanel2.VisibleMaxX = 1.000000000000000000
    PLOTPanel2.MouseMode = XZoom
    PLOTPanel2.BackgroundColor = clSilver
    PLOTPanel2.AxesColor = 15790320
    PLOTPanel2.GridLinesColor = clMedGray
    PLOTPanel2.AxesFont.Charset = DEFAULT_CHARSET
    PLOTPanel2.AxesFont.Color = clBlack
    PLOTPanel2.AxesFont.Height = 16
    PLOTPanel2.AxesFont.Name = 'Arial'
    PLOTPanel2.AxesFont.Style = []
    PLOTPanel2.TimeLocked = True
    PLOTPanel2.DrawEraseExtra = 1
    PLOTPanel2.DesignSize = (
      0
      56)
    PLOTPanel3.Tag = 3
    PLOTPanel3.Left = 0
    PLOTPanel3.Top = 182
    PLOTPanel3.Width = 0
    PLOTPanel3.Height = 56
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
    PLOTPanel3.AxesFont.Height = 16
    PLOTPanel3.AxesFont.Name = 'Arial'
    PLOTPanel3.AxesFont.Style = []
    PLOTPanel3.TimeLocked = True
    PLOTPanel3.DrawEraseExtra = 1
    PLOTPanel3.DesignSize = (
      0
      56)
    PLOTPanel4.Tag = 4
    PLOTPanel4.Left = 0
    PLOTPanel4.Top = 243
    PLOTPanel4.Width = 0
    PLOTPanel4.Height = 55
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
    PLOTPanel4.AxesFont.Height = 16
    PLOTPanel4.AxesFont.Name = 'Arial'
    PLOTPanel4.AxesFont.Style = []
    PLOTPanel4.TimeLocked = True
    PLOTPanel4.DrawEraseExtra = 1
    PLOTPanel4.DesignSize = (
      0
      55)
    PLOTPanel5.Tag = 5
    PLOTPanel5.Left = 0
    PLOTPanel5.Top = 303
    PLOTPanel5.Width = 0
    PLOTPanel5.Height = 56
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
    PLOTPanel5.AxesFont.Height = 16
    PLOTPanel5.AxesFont.Name = 'Arial'
    PLOTPanel5.AxesFont.Style = []
    PLOTPanel5.TimeLocked = True
    PLOTPanel5.DrawEraseExtra = 1
    PLOTPanel5.DesignSize = (
      0
      56)
    PLOTPanel6.Tag = 6
    PLOTPanel6.Left = 0
    PLOTPanel6.Top = 364
    PLOTPanel6.Width = 0
    PLOTPanel6.Height = 55
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
    PLOTPanel6.AxesFont.Height = 16
    PLOTPanel6.AxesFont.Name = 'Arial'
    PLOTPanel6.AxesFont.Style = []
    PLOTPanel6.TimeLocked = True
    PLOTPanel6.DrawEraseExtra = 1
    PLOTPanel6.DesignSize = (
      0
      55)
    PLOTPanel7.Tag = 7
    PLOTPanel7.Left = 0
    PLOTPanel7.Top = 424
    PLOTPanel7.Width = 0
    PLOTPanel7.Height = 61
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
    PLOTPanel7.AxesFont.Height = 16
    PLOTPanel7.AxesFont.Name = 'Arial'
    PLOTPanel7.AxesFont.Style = []
    PLOTPanel7.TimeLocked = True
    PLOTPanel7.DrawEraseExtra = 1
    PLOTPanel7.DesignSize = (
      0
      61)
    SplitterPositions = <
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
    GlobalChanges = True
    MouseMode = XZoom
    DrawEraseExtra = 0
    Align = alClient
    Color = clBlack
    ExplicitTop = 1
  end
  object Panel3: TPanel
    Left = 175
    Top = 0
    Width = 122
    Height = 709
    Align = alLeft
    TabOrder = 3
    object Label16: TLabel
      Left = 1
      Top = 1
      Width = 120
      Height = 13
      Align = alTop
      Alignment = taCenter
      Caption = 'Choose Up to 8 Plots'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold, fsUnderline]
      ParentFont = False
      ExplicitWidth = 115
    end
    object Panel4: TPanel
      Left = 1
      Top = 510
      Width = 120
      Height = 198
      Align = alBottom
      TabOrder = 0
      object Panel6: TPanel
        Left = 1
        Top = 1
        Width = 118
        Height = 56
        Align = alTop
        TabOrder = 0
        object Label19: TLabel
          Left = 1
          Top = 42
          Width = 116
          Height = 13
          Align = alBottom
          Alignment = taCenter
          Caption = 'Plots to Display'
          ExplicitWidth = 73
        end
        object AddToDisplayButton: TSpeedButton
          Left = 4
          Top = 5
          Width = 33
          Height = 33
          Hint = 'Chooses selected cell and electrode plots to be displayed'
          Glyph.Data = {
            76020000424D7602000000000000760000002800000020000000200000000100
            04000000000000020000C30E0000C30E00001000000000000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
            888888888888888888887788888888888807788888888888887778FFFFFFFFFF
            F0700FFFFFFFFFFFFF8778FFFFFFFFFF777707FFFFFFFFFFFF8778FFFFFFFFF7
            7777777FFFFFFFFFFF8778FFFFFFFF8477777778FFFFFFFFFF8778FFFFFFFF07
            77777777FFFFFFFFFF8778FFFFFFF777777777851FFFFFFFFF8778FFFFFF7577
            7777778807FFFFFFFF8778FFFFF8077777777788777FFFFFFF8778FFFFF07777
            777777888778FFFFFF8778FFFF757777777777888877FFFFFF8778FFF7477777
            7777778888871FFFFF8778FF8057777777777788888877FFFF8778FF04577777
            777777888888877FFF8778F745577777777777888888F878FF87787444444445
            57777000055777777F8778770000044407777777777777778F8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7000077FFFFFFFFFFF87778888888888
            8777777888888888887787777777777777777777777777777778}
          OnClick = AddToDisplayButtonClick
        end
        object RemoveFromDisplayButton: TSpeedButton
          Left = 43
          Top = 5
          Width = 33
          Height = 33
          Hint = 'Hide plots that are selected below'
          Glyph.Data = {
            76020000424D7602000000000000760000002800000020000000200000000100
            04000000000000020000C30E0000C30E00001000000000000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00877777777777
            777777777777777777787788888888888777777888888888887778FFFFFFFFFF
            7000077FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF8778FFFFFFFFFF
            7777777FFFFFFFFFFF8778FFFFFFFFFF7777777FFFFFFFFFFF87787700000444
            07777777777777778F8778744444444557777000055777777F8778F745577777
            777777888888F878FF8778FF04577777777777888888877FFF8778FF80577777
            77777788888877FFFF8778FFF74777777777778888871FFFFF8778FFFF757777
            777777888877FFFFFF8778FFFFF07777777777888778FFFFFF8778FFFFF80777
            77777788777FFFFFFF8778FFFFFF75777777778807FFFFFFFF8778FFFFFFF777
            777777850FFFFFFFFF8778FFFFFFFF0777777777FFFFFFFFFF8778FFFFFFFF84
            77777778FFFFFFFFFF8778FFFFFFFFF77777777FFFFFFFFFFF8778FFFFFFFFFF
            777707FFFFFFFFFFFF8778FFFFFFFFFFF0700FFFFFFFFFFFFF87778888888888
            8807788888888888887788888888888888888888888888888888}
          OnClick = RemoveFromDisplayButtonClick
        end
        object ClearDisplayListButton: TSpeedButton
          Left = 81
          Top = 5
          Width = 33
          Height = 33
          Hint = 'Hide all displayed plots'
          Glyph.Data = {
            76020000424D7602000000000000760000002800000020000000200000000100
            04000000000000020000C30E0000C30E00001000000000000000000000000000
            8000008000000080800080000000800080008080000080808000C0C0C0000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FF0000000000
            00000000000000000000FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0F00FFFFFFFFFFFFFFF00000FFFFF0FF0F000FFFFF
            FFFFFF0000000000FFF0FF09000000FFFFF000000000000000F0FF0990000000
            00000099999999000000FF099990000000099999999999990000FF0999999999
            99999999999999999000FF0FF9999999999999FFFFFFFF9999F0FF0FFFF99999
            999FFFFFFFFFFFF999F0FF0FFFFFFFFFFFFFFFFFFFFFFFFFF9F0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FFFFFFFFF
            FFFFFFFFFFFFFFFFFFF0FF000000000000000000000000000000}
          OnClick = ClearDisplayListButtonClick
        end
      end
      object PlotsDisplayedListBox: TListBox
        Left = 1
        Top = 57
        Width = 118
        Height = 140
        Align = alClient
        ItemHeight = 13
        MultiSelect = True
        TabOrder = 1
      end
    end
    object Panel5: TPanel
      Left = 1
      Top = 14
      Width = 120
      Height = 496
      Align = alClient
      TabOrder = 1
      object Label22: TLabel
        Left = 1
        Top = 1
        Width = 118
        Height = 13
        Align = alTop
        Alignment = taCenter
        Caption = 'Cells (voltages)'
        ExplicitWidth = 74
      end
      object Splitter1: TSplitter
        Left = 1
        Top = 289
        Width = 118
        Height = 3
        Cursor = crVSplit
        Align = alTop
        ExplicitLeft = -3
        ExplicitTop = 350
      end
      object Splitter2: TSplitter
        Left = 1
        Top = 412
        Width = 118
        Height = 3
        Cursor = crVSplit
        Align = alBottom
        ExplicitLeft = -3
        ExplicitTop = 379
      end
      object CellsPlotsListBox: TListBox
        Left = 1
        Top = 14
        Width = 118
        Height = 275
        Hint = 
          'Select one or more cells to plot after run.  Use downward arrow ' +
          'to move to display list. '
        Align = alTop
        ItemHeight = 13
        MultiSelect = True
        TabOrder = 0
      end
      object Panel14: TPanel
        Left = 1
        Top = 415
        Width = 118
        Height = 80
        Align = alBottom
        TabOrder = 1
        object Label27: TLabel
          Left = 1
          Top = 1
          Width = 116
          Height = 13
          Align = alTop
          Alignment = taCenter
          Caption = 'Data Logs (Volts)'
          ExplicitWidth = 82
        end
        object DataLogsPlotsListBox: TListBox
          Left = 1
          Top = 14
          Width = 116
          Height = 65
          Hint = 
            'Select one or more electrodes to plot after run.  Use downward a' +
            'rrow to move to display list.'
          Align = alClient
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
        end
      end
      object Panel15: TPanel
        Left = 1
        Top = 292
        Width = 118
        Height = 120
        Align = alClient
        TabOrder = 2
        object Label23: TLabel
          Left = 1
          Top = 1
          Width = 116
          Height = 13
          Align = alTop
          Alignment = taCenter
          Caption = 'Electrodes (currents)'
          ExplicitWidth = 101
        end
        object ElectrodesPlotsListBox: TListBox
          Left = 1
          Top = 14
          Width = 116
          Height = 105
          Hint = 
            'Select one or more electrodes to plot after run.  Use downward a' +
            'rrow to move to display list.'
          Align = alClient
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
        end
      end
    end
  end
  object ActionManager1: TActionManager
    ActionBars = <
      item
        Items = <
          item
            Action = StartAction
            Caption = '&Start'
            ImageIndex = 0
            ShortCut = 16467
          end
          item
            Action = StopAction
            Caption = 'S&top'
            ImageIndex = 1
            ShortCut = 16465
          end
          item
            Action = Action1
          end
          item
            Action = CloseAction
            Caption = '&Close'
            ImageIndex = 2
            ShortCut = 16474
          end>
        ActionBar = ActionToolBar1
      end>
    Images = ImageList1
    Left = 428
    Top = 228
    StyleName = 'Platform Default'
    object StartAction: TAction
      Category = 'Control'
      Caption = 'Start'
      Hint = 'Starts simulation or experiment with chosen settings'
      ImageIndex = 0
      OnExecute = StartActionExecute
      OnUpdate = StartActionUpdate
    end
    object StopAction: TAction
      Category = 'Control'
      Caption = 'Stop'
      Hint = 'Terminates a simulation or experiment early'
      ImageIndex = 1
      OnExecute = StopActionExecute
      OnUpdate = StopActionUpdate
    end
    object CloseAction: TAction
      Category = 'Control'
      Caption = 'Close'
      Hint = 'Close this form and return to network settings'
      ImageIndex = 2
      OnExecute = CloseActionExecute
    end
    object Action1: TAction
      Category = 'Control'
      Caption = '          '
    end
  end
  object ImageList1: TImageList
    Height = 32
    Width = 32
    Left = 436
    Top = 284
    Bitmap = {
      494C0101070094005C0120002000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000800000004000000001002000000000000080
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF00000000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000FF000000FF00000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF0000008000000080000000000000000000000000FF000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF0000008000000080000000000000000000000000FF000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000080000000FF000000FF00000080000000800000808080000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000080000000FF000000FF00000080000000800000808080000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      00000000000000FF000000800000008000000080000000800000000000000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF008080
      8000808080000000000000000000000000000080000000800000000000000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      000000000000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF00000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF008080
      8000808080000000000000000000000000000080000000800000000000000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0080808000808080000000000000000000000000000000
      FF000000FF000000FF00000000000000000000000000FF00000000000000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF008080
      8000808080000000000000000000000000000080000000800000000000000000
      FF000000FF000000FF00000000000000000000000000FF000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF008080
      8000808080000000000000000000000000000080000000800000000000000000
      FF000000FF000000FF00000000000000000000000000FF000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF00000000000000000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FF000000800000008000000080000000800000000000000000
      FF000000FF000000FF00000000000000000000000000FF000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF00000000000000000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FF000000800000008000000080000000800000000000000000
      FF000000FF000000FF00000000000000000000000000FF000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000080000000FF000000FF00000080000000800000808080000000
      FF000000FF000000FF00000000000000000000000000FF000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF0000008000000080000000000000000000000000FF000000
      FF000000FF0000000000000000000000000000000000FF000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF0000008000000080000000000000000000000000FF000000
      FF000000FF0000000000000000000000000000000000FF00000000000000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF00000000000000000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      000000000000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF00000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF00000000000000000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF00000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF00000000000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C040A000C040A000C040A00000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800080808000808080008080800080808000808080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000800000008000000080000000800000008000000080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF00008080800080808000808080008080
      8000808080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000800000008000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000008000000080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF00008080800080808000808080008080
      8000808080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000080000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF00008080
      8000808080008080800080808000808080008080800080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008000000080000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000800000008000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000080808000808080008080800080808000808080008080
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      000000000000000000000000000000000000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000080000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000080000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000080808000808080008080800080808000808080008080
      8000808080000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A00000000000000000000000000000000000000000000000
      000000000000000080000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000080000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF0000808080008080
      8000808080008080800080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      00000000000000000000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      8000000080000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      8000000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF0000808080008080
      8000808080008080800080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      00000000000000000000C040A000C040A000C040A000C040A000C040A000C040
      A00000000000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      80000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00008080800080808000808080008080800080808000808080000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      000000000000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A00000000000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      80000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF00008080800080808000808080000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      0000C040A000C040A000C040A000C040A000C040A000C040A000C040A0000000
      00000000000000000000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF00008080800080808000808080000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000808080008080800000000000000000000000000000000000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00008080
      8000808080000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A00000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF00000080808000808080000000000000000000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A0000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000808080008080800000000000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000808080008080800000000000C040A000C040A000C040A000C040
      A000C040A000C040A00000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000808080008080800000000000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF00000080808000808080000000000000000000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A0000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A00000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000808080008080800000000000000000000000000000000000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      0000C040A000C040A000C040A000C040A000C040A000C040A000C040A0000000
      00000000000000000000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      80000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      000000000000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A00000000000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      80000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000FF000000FF000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      00000000000000000000C040A000C040A000C040A000C040A000C040A000C040
      A00000000000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      8000000080000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      8000000080000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000000000000000000000000000000000000000
      000000000000000080000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000080000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000C040A000C040
      A000C040A000C040A00000000000000000000000000000000000000000000000
      000000000000000080000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000080000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000808080008080800000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      000000000000000000000000000000000000C040A000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008000000080000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000800000008000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FF000000FF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000008080800080808000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C040A000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000080000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FF000000FF00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C040A000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000800000008000000080000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000800000008000000080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C040
      A000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000800000008000000080000000800000008000000080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C040A000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000C040A000C040A000C040A000C040A000C040A000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000080000000400000000100010000000000000400000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFFBFFFFFFDFFFFFFFF00000000
      FFC003FFBFFFFFFD0000000000000000FF0000FFBFFFFFFDFFF87FFF00000000
      FC00003FBFFFFFFDFFF03FFF00000000FC00003FBFFFFFFDFFE01FFF00000000
      F800001FBFFFFFFDFFC00FFF00000000E0000007BFFFFFFDFF8007FF00000000
      E0000007BFFFFFFDFF8007FF00000000C0000003BFFFFFFDFF8007FF00000000
      C0000003BFFFFFFDFFF87FFF0000000080000003BFFFFFFDFFF87FFF00000000
      80000003B8FFFF1DFFF87FFF0000000080000003B0FFFF0DFFF87FFF00000000
      80000001A0FFFF05FFF87FFF000000008000000180FFFF01FFF87FFF00000000
      8000000180000001FFF87FFF000000008000000180000001FFF87FFF00000000
      8000000180000001FFF87FFF000000008000000180000001FFF87FFF00000000
      8000000380FFFF01FFF87FFF0000000080000003A0FFFF05FFF87FFF00000000
      C0000003B0FFFF0DFFF87FFF00000000C0000003B8FFFF1DFFF87FFF00000000
      C0000003BFFFFFFDFF8007FF00000000E0000007BFFFFFFDFF8007FF00000000
      E0000007BFFFFFFDFF8007FF00000000F800001FBFFFFFFDFFC00FFF00000000
      FC00003FBFFFFFFDFFE01FFF00000000FC00003FBFFFFFFDFFF03FFF00000000
      FF0000FFBFFFFFFDFFF87FFF00000000FFC003FFBFFFFFFD0000000000000000
      FFFFFFFFBFFFFFFDFFFFFFFF00000000E0FFFFFFFFFFFFFFFFFC7FFFFFFFFFFF
      E0FFFFFFF0000001FFF83FFFFFE007FFE01FFFFFF0000001FFF03FFFFF0000FF
      E003FFFFC0000001FFE03FFFFC00003FE003FFFFC0000001FFC03FFFFC00003F
      E0003FFFC0000001FF803FFFF800001FE00007FFC0000001FF003FFFE0000007
      E00007FFC0000001FE000003E0000007E00000FFC0000001FC000001C0000003
      E00000FFC0000001FC080001C0000003E000001FC0000001F8080001C0000003
      E000001FC0000001F01C000180000001E000001FC0000001E03FFFC180000001
      E0000007C0000001C07FFFC180000001E0000007C000000180FFFFC180000001
      E0000007C000000101FFFFC180000001E0000007C000000103FFFFC180000001
      E0000007C000000101FFFFC180000001E000001FC000000180FFFFC180000001
      E00000FFC0000001C07FFFC180000001E00000FFC0000001E03FFFC180000001
      E00007FFC0000001F01C0001C0000003E0003FFFC0000001F8080001C0000003
      E0003FFFC0000001FC080001C0000003E003FFFFC0000001FE000001E0000007
      E003FFFFC0000001FE000003E0000007E01FFFFFC0000001FF003FFFF800001F
      FCFFFFFFC0000001FF803FFFFC00003FFCFFFFFFC0000007FFC03FFFFC00003F
      FFFFFFFFC0000007FFE03FFFFF0000FFFFFFFFFFFFFFFFFFFFF03FFFFFE007FF
      FFFFFFFFFFFFFFFFFFF83FFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object ImageList2: TImageList
    Height = 32
    Width = 32
    Left = 572
    Top = 276
    Bitmap = {
      494C010103006C00480120002000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000800000002000000001002000000000000040
      0000000000000000000000000000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008080800080808000808080008080800080808000808080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080808000808080008080800080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000808080008080800000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000000000000000000000000000000000008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000808080000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00808080008080800080808000808080000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008080
      800080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000008000
      0000808080008080800080808000808080008080800080808000808080000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0080808000808080008080
      8000808080008080800080808000808080008080800080808000000000008000
      800000008000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008080800080008000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      00000000000080808000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      0000808080008080800080808000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000808080008080800000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0080808000800080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000000000008080800080808000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF008080800080000000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      00000000000000000000000000008080800000008000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00000000000000000080008000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      0000000000000000000000000000000000008080800080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000808080008080
      8000000000000000000000000000000000000000000080000000800000008000
      0000000000008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      800000000000FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00000000008000000080008000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000000000000000000000000000000000008080800080808000FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000808080008000
      0000800000008000000080000000800000008000000080000000800000008000
      8000800080008080800080808000808080008080800000000000000000000000
      0000000000008000800080008000808080008080800080808000808080008080
      800080808000FFFFFF0000000000808080008080800000000000FFFFFF008080
      8000800000008000800080008000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000000000000000000000000000FFFFFF0000000000808080000000
      0000FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF008080
      8000800000008000800080008000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000000000000000000000000000FFFFFF0000000000808080000000
      0000FFFFFF00FFFFFF0000000000808080008080800000000000808080008000
      0000800000008000000080000000800000008000000080000000800000008000
      8000800080008080800080808000808080008080800000000000000000000000
      0000000000008000800080008000808080008080800080808000808080008080
      800080808000FFFFFF0000000000808080000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00000000008000000080008000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000000000000000000000000000000000008080800080808000FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000808080008080
      8000000000000000000000000000000000000000000080000000800000008000
      0000000000008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      800000000000FFFFFF0000000000808080000000000000000000000000000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      00000000000000000000000000000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00000000000000000080008000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      0000000000000000000000000000000000008080800080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF008080800080000000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      00000000000000000000000000008080800000008000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0080808000800080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000000000008080800080808000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      00000000000000000000000000000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      000000000000808080008080800000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      0000808080008080800080808000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008080800080008000808080008080
      8000808080008080800080808000808080008080800080808000000000000000
      00000000000080808000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0080808000808080008080
      8000808080008080800080808000808080008080800080808000000000008000
      800000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000008000
      0000808080008080800080808000808080008080800080808000808080000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008080
      800080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00808080008080800080808000808080000000000080808000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000808080008080800080808000808080008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000808080000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080008080800000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0080808000000000000000000000000000000000008080800080808000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000808080008080800000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080808000808080008080800080808000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008080800080808000808080008080800080808000808080000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000080000000200000000100010000000000000200000000000000000000
      000000000000000000000000FFFFFF0080000001FFFFFFFFC000000000000000
      3FF81FFC3FFC7FFCDFFFFFFE000000004000000240000002DFFFFFFE00000000
      4000000240000002DFFFFFFE000000004000000240000002DFFFFFFE00000000
      4000000240201002DFFFFFFE000000004000000240000002DFFFFFFE00000000
      4000000240002002DFFFFFFE000000004000000240003002DFFFFFFE00000000
      4000000241003002DFFFFFFE000000004000000240003902DFFFFFFE00000000
      4000000240003C02DFFFFFFE000000004000000240003E02DFFFFFFE00000000
      4000000248003F02D3FFF83E000000004000000A40003F82D1FFC00E00000000
      4000000240003F52C03E00020000000040003F5240000002C000000000000000
      40003F824000000AC00000000000000048003F0240000002C000000000000000
      40003E0240000002D8003FC20000000040003C0240000002DE01FFE200000000
      4000390240000002DFFFFFFA000000004100300240000002DFFFFFFE00000000
      4000300240000002DFFFFFFE000000004000200240000002DFFFFFFE00000000
      4000000240000002DFFFFFFE000000004020100240000002DFFFFFFE00000000
      4000000240000002DFFFFFFE000000004000000240000002DFFFFFFE00000000
      4000000240000002DFFFFFFE000000003FFC7FFC3FF81FFCDFFFFFFE00000000
      FFFFFFFF80000001C00000000000000000000000000000000000000000000000
      000000000000}
  end
  object CGMComponent1: TCGMComponent
    AutoScale = True
    Left = 452
    Top = 404
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text Files|*.txt|ABF Files (Axon Binary Format)|*.abf'
    FilterIndex = 2
    Title = 'Choose the file location for saving cell voltages'
    Left = 348
    Top = 220
  end
  object SaveDialog2: TSaveDialog
    DefaultExt = 'cgm'
    Filter = 'Computer Graphics Metafiles|*.cgm|Postscript files|*.ps'
    Title = 'Choose the file location for saving figure files'
    Left = 348
    Top = 276
  end
  object SaveDialog3: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text Files|*.txt'
    Title = 'Choose the file location for logging kinetic parameters'
    Left = 344
    Top = 376
  end
end
