object VoltageClampFitForm: TVoltageClampFitForm
  Left = 0
  Top = 0
  Caption = 'NetFit - Determine kinetic parameters from voltage clamp data'
  ClientHeight = 647
  ClientWidth = 950
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 950
    Height = 21
    Align = alTop
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 606
    Width = 950
    Height = 41
    Align = alBottom
    TabOrder = 1
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 21
    Width = 950
    Height = 585
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 2
    OnChange = PageControl1Change
    OnChanging = PageControl1Changing
    object TabSheet1: TTabSheet
      Caption = 'Open a Voltage Clamp File'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ScrollBox1: TScrollBox
        Left = 0
        Top = 0
        Width = 942
        Height = 557
        Align = alClient
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Parameters for Current'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ScrollBox2: TScrollBox
        Left = 0
        Top = 0
        Width = 942
        Height = 557
        Align = alClient
        TabOrder = 0
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Generate Trial Fits and Optimize'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label1: TLabel
        Left = 154
        Top = 14
        Width = 117
        Height = 13
        Caption = 'Choose Voltage Channel'
      end
      object Label2: TLabel
        Left = 314
        Top = 14
        Width = 118
        Height = 13
        Caption = 'Choose Current Channel'
      end
      object Label3: TLabel
        Left = 8
        Top = 14
        Width = 46
        Height = 13
        Caption = 'Filename:'
      end
      object FilenameLabel: TLabel
        Left = 12
        Top = 36
        Width = 133
        Height = 13
        AutoSize = False
        Caption = '      '
        EllipsisPosition = epPathEllipsis
      end
      object Label5: TLabel
        Left = 723
        Top = 42
        Width = 71
        Height = 13
        Caption = 'Calls to fitGen:'
      end
      object fitGenLabel: TLabel
        Left = 800
        Top = 42
        Width = 81
        Height = 13
        Caption = '                           '
      end
      object Label6: TLabel
        Left = 723
        Top = 25
        Width = 58
        Height = 13
        Caption = 'ChiSq (gof):'
      end
      object ChiSqLabel: TLabel
        Left = 787
        Top = 25
        Width = 60
        Height = 13
        Caption = '                    '
      end
      object Label7: TLabel
        Left = 616
        Top = 25
        Width = 92
        Height = 13
        Caption = 'Number of Threads'
      end
      object VChanComboBox: TComboBox
        Left = 154
        Top = 33
        Width = 145
        Height = 21
        AutoComplete = False
        AutoCloseUp = True
        Style = csDropDownList
        TabOrder = 0
      end
      object GenerateButton: TButton
        Left = 494
        Top = 6
        Width = 75
        Height = 25
        Caption = 'Generate'
        TabOrder = 1
        OnClick = GenerateButtonClick
      end
      object CChanComboBox: TComboBox
        Left = 314
        Top = 33
        Width = 145
        Height = 21
        AutoComplete = False
        AutoCloseUp = True
        Style = csDropDownList
        TabOrder = 2
      end
      object SaveButton: TButton
        Left = 863
        Top = 0
        Width = 75
        Height = 25
        Caption = 'Save to ABF'
        TabOrder = 3
        OnClick = SaveButtonClick
      end
      object Panel3: TPanel
        AlignWithMargins = True
        Left = 3
        Top = 60
        Width = 936
        Height = 494
        Margins.Top = 60
        Align = alClient
        Caption = 'Panel3'
        TabOrder = 4
        DesignSize = (
          936
          494)
        object ParamsPanel: TPanel
          Left = 716
          Top = 1
          Width = 219
          Height = 492
          Align = alRight
          BevelInner = bvSpace
          BevelKind = bkSoft
          BevelOuter = bvLowered
          Caption = 'ParamsPanel'
          TabOrder = 0
          object Label4: TLabel
            Left = 2
            Top = 2
            Width = 136
            Height = 13
            Align = alTop
            Alignment = taCenter
            Caption = 'Set Vary=0 to hold constant'
          end
          object ValueListEditor1: TValueListEditor
            Left = 2
            Top = 15
            Width = 211
            Height = 471
            Align = alClient
            TabOrder = 0
            TitleCaptions.Strings = (
              'Parameter'
              'Vary?'
              '')
            ColWidths = (
              168
              37)
          end
        end
        object PlotPanel1: TPLOTPanel
          AlignWithMargins = True
          Left = 4
          Top = 4
          Width = 709
          Height = 486
          FullScaleMinY = -100.000000000000000000
          VisibleMinY = -100.000000000000000000
          FullScaleMaxY = 100.000000000000000000
          VisibleMaxY = 100.000000000000000000
          FullScaleMaxX = 60.000000000000000000
          VisibleMaxX = 1.000000000000000000
          MouseMode = XYZoom
          AxesColor = 15790320
          GridLinesColor = 32512
          AxesFont.Charset = DEFAULT_CHARSET
          AxesFont.Color = clBlack
          AxesFont.Height = 16
          AxesFont.Name = 'Arial'
          AxesFont.Style = []
          YUnits = 'nA'
          XUnits = 'ms'
          Align = alClient
          DesignSize = (
            709
            486)
        end
        object ConcatEpisCheckBox: TCheckBox
          Left = 535
          Top = 471
          Width = 126
          Height = 17
          Anchors = [akRight, akBottom]
          Caption = 'Concatenate Episodes'
          TabOrder = 2
          OnClick = ConcatEpisCheckBoxClick
        end
      end
      object OptimizeButton: TButton
        Left = 470
        Top = 33
        Width = 59
        Height = 25
        Caption = 'Optimize'
        TabOrder = 5
        OnClick = OptimizeButtonClick
      end
      object MultiThreadCheckBox: TCheckBox
        Left = 620
        Top = 3
        Width = 97
        Height = 17
        Caption = 'Multi Threaded'
        TabOrder = 6
      end
      object NumThreadsEdit: TEdit
        Left = 616
        Top = 39
        Width = 92
        Height = 21
        Alignment = taRightJustify
        TabOrder = 7
        Text = '4'
      end
      object StopButton: TButton
        Left = 535
        Top = 33
        Width = 59
        Height = 25
        Caption = 'Stop'
        TabOrder = 8
        OnClick = StopButtonClick
      end
      object SaveParamsButton: TButton
        Left = 863
        Top = 30
        Width = 75
        Height = 25
        Caption = 'Save Params'
        TabOrder = 9
        OnClick = SaveParamsButtonClick
      end
    end
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'abf'
    Filter = 'ABF Files|*.abf'
    Title = 'Save Original and Generated Data'
    Left = 781
    Top = 41
  end
  object SaveDialog2: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Tab delimited text | *.txt'
    Title = 'Save kinetic parameters to tab delimited text file'
    Left = 822
    Top = 46
  end
end
