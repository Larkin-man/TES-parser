object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'TES file parseer'
  ClientHeight = 525
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 19
  object Splitter1: TSplitter
    Left = 544
    Top = 27
    Height = 498
    ExplicitLeft = 288
    ExplicitTop = 144
    ExplicitHeight = 100
  end
  object Splitter2: TSplitter
    Left = 281
    Top = 27
    Height = 498
    ExplicitLeft = 276
    ExplicitTop = -3
  end
  object PanelUp: TPanel
    Left = 0
    Top = 0
    Width = 792
    Height = 27
    Align = alTop
    Caption = 'PanelUp'
    ShowCaption = False
    TabOrder = 0
    object LDele: TLabel
      Left = 324
      Top = 1
      Width = 107
      Height = 25
      Align = alLeft
      Caption = 'Deleted Size=0'
      Visible = False
      ExplicitLeft = 321
      ExplicitHeight = 19
    end
    object Bevel1: TBevel
      Left = 130
      Top = 1
      Width = 3
      Height = 25
      Align = alLeft
      Shape = bsSpacer
    end
    object OpenBtn: TButton
      Left = 1
      Top = 1
      Width = 64
      Height = 25
      Align = alLeft
      Caption = 'Open'
      TabOrder = 0
      OnClick = OpenBtnClick
    end
    object Save: TButton
      Left = 65
      Top = 1
      Width = 65
      Height = 25
      Align = alLeft
      Caption = 'Save'
      Enabled = False
      TabOrder = 1
      OnClick = SaveClick
    end
    object EFinds: TEdit
      Left = 133
      Top = 1
      Width = 134
      Height = 25
      Hint = 'Search in data'
      Align = alLeft
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      ExplicitLeft = 130
      ExplicitHeight = 27
    end
    object FindStr: TButton
      Left = 267
      Top = 1
      Width = 57
      Height = 25
      Hint = #1048#1097#1077#1090' '#1087#1077#1088#1074#1091#1102' '#1089#1090#1088#1086#1082#1091' '#1084#1077#1084#1086' '#1074' '#1076#1072#1090#1077
      Align = alLeft
      Caption = 'Find'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = FindStrClick
      ExplicitLeft = 264
    end
  end
  object Out: TMemo
    Left = 547
    Top = 27
    Width = 30
    Height = 498
    Align = alClient
    Lines.Strings = (
      'O'
      'u'
      't')
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object PanelTags: TPanel
    Left = 577
    Top = 27
    Width = 215
    Height = 498
    Align = alRight
    ShowCaption = False
    TabOrder = 2
    object Label1: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 23
      Width = 207
      Height = 19
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alTop
      Caption = #1055#1088#1086#1095#1080#1090#1072#1090#1100' '#1089#1077#1082#1094#1080#1102': '
      ExplicitWidth = 148
    end
    object Label2: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 268
      Width = 207
      Height = 19
      Margins.Top = 0
      Margins.Bottom = 0
      Align = alTop
      Caption = #1055#1088#1086#1095#1080#1090#1072#1090#1100' '#1090#1077#1075':'
      ExplicitWidth = 113
    end
    object ButtonGroup1: TButtonGroup
      AlignWithMargins = True
      Left = 4
      Top = 101
      Width = 207
      Height = 164
      Align = alTop
      BevelKind = bkTile
      BorderStyle = bsNone
      ButtonHeight = 26
      ButtonOptions = [gboFullSize, gboShowCaptions]
      Items = <
        item
          Caption = #1062#1077#1083#1086#1095#1080#1089#1083#1077#1085#1085#1099#1077' 4'#1073#1072#1081#1090#1072
        end
        item
          Caption = #1058#1077#1082#1089#1090#1086#1074#1086#1077' '#1087#1086#1083#1077
        end
        item
          Caption = #1042#1077#1097#1077#1089#1090#1074#1077#1085#1085#1099#1077' 4'#1073#1072#1081#1090#1072
        end
        item
          Caption = #1062#1077#1083#1086#1095#1080#1089#1083#1077#1085#1085#1099#1077' 2'#1073#1072#1081#1090#1072' '#1073#1077#1079#1079#1085#1072#1082#1086#1074#1099#1077' '
        end
        item
          Caption = #1062#1077#1083#1086#1095#1080#1089#1083#1077#1085#1085#1099#1077' 1'#1073#1072#1081#1090
        end
        item
          Caption = 'Length[12'#1073#1072#1081#1090'] '#1073#1077#1079' '#1088#1072#1079#1073#1086#1088#1072' '#1089#1090#1088#1091#1082#1090#1091#1088#1099
        end>
      TabOrder = 0
      OnButtonClicked = ButtonGroup1ButtonClicked
    end
    object FlowPanel1: TFlowPanel
      AlignWithMargins = True
      Left = 4
      Top = 290
      Width = 207
      Height = 204
      Align = alClient
      BevelKind = bkSoft
      BevelOuter = bvNone
      Caption = 'FlowPanel1'
      ShowCaption = False
      TabOrder = 1
      object HEDRRead: TButton
        Left = 0
        Top = 0
        Width = 50
        Height = 25
        Caption = 'TES3'
        Enabled = False
        TabOrder = 0
        OnClick = HEDRReadClick
      end
      object MASTread: TButton
        Left = 50
        Top = 0
        Width = 50
        Height = 25
        Caption = 'MAST'
        TabOrder = 1
        OnClick = MASTreadClick
      end
      object Read3Bytes: TButton
        Left = 100
        Top = 0
        Width = 50
        Height = 25
        Caption = '3byte'
        TabOrder = 2
        OnClick = Read3BytesClick
      end
      object CellRead: TButton
        Left = 150
        Top = 0
        Width = 50
        Height = 25
        Caption = 'CELL'
        TabOrder = 3
        OnClick = CellReadClick
      end
      object Nam5Read: TButton
        Left = 0
        Top = 25
        Width = 50
        Height = 25
        Caption = 'NAM5'
        TabOrder = 4
        OnClick = Nam5ReadClick
      end
      object Read3INT: TButton
        Left = 50
        Top = 25
        Width = 50
        Height = 25
        Caption = '3int'
        TabOrder = 5
        OnClick = Read3INTClick
      end
      object Read1STR: TButton
        Left = 100
        Top = 25
        Width = 50
        Height = 25
        Caption = '1str'
        TabOrder = 6
        OnClick = Read1STRClick
      end
      object Nam0Read: TButton
        Left = 150
        Top = 25
        Width = 50
        Height = 25
        Caption = 'NAM0'
        TabOrder = 7
        OnClick = Nam0ReadClick
      end
      object FRMRRead: TButton
        Left = 0
        Top = 50
        Width = 50
        Height = 25
        Caption = 'FRMR'
        TabOrder = 8
        OnClick = FRMRReadClick
      end
      object ReadInts: TButton
        Left = 50
        Top = 50
        Width = 50
        Height = 25
        Caption = 'Ints'
        TabOrder = 9
        OnClick = ReadIntsClick
      end
      object LTEXread: TButton
        Left = 100
        Top = 50
        Width = 50
        Height = 25
        Caption = 'LTEX'
        TabOrder = 10
        OnClick = LTEXreadClick
      end
      object INTVread: TButton
        Left = 150
        Top = 50
        Width = 50
        Height = 25
        Caption = 'INTV'
        TabOrder = 11
        OnClick = INTVreadClick
      end
      object WriteMVRFB: TCheckBox
        Left = 0
        Top = 75
        Width = 97
        Height = 17
        Caption = 'WriteMVRF'
        TabOrder = 12
      end
      object DelDatas: TButton
        Left = 97
        Top = 75
        Width = 75
        Height = 25
        Caption = 'DelDatas'
        TabOrder = 13
        OnClick = DelDatasClick
      end
      object TestP: TButton
        Left = 0
        Top = 100
        Width = 49
        Height = 25
        Caption = 'TestP'
        TabOrder = 14
        OnClick = TestPClick
      end
      object SPELread: TButton
        Left = 49
        Top = 100
        Width = 50
        Height = 25
        Caption = 'SPEL'
        TabOrder = 15
        OnClick = SPELreadClick
      end
      object UpDown1: TUpDown
        Left = 99
        Top = 100
        Width = 19
        Height = 28
        TabOrder = 16
        Visible = False
        OnChangingEx = UpDown1ChangingEx
      end
      object ExportSPEL: TButton
        Left = 0
        Top = 128
        Width = 89
        Height = 25
        Caption = 'ExportSPEL'
        TabOrder = 17
        OnClick = ExportSPELClick
      end
      object Sost: TCheckBox
        Left = 89
        Top = 128
        Width = 97
        Height = 17
        Hint = #1057#1086#1089#1090#1072#1074#1085#1099#1077' '#1087#1088#1080' '#1086#1090#1082#1088#1099#1090#1080#1080
        Caption = #1057#1086#1089#1090#1072#1074#1085#1099#1077
        ParentShowHint = False
        ShowHint = True
        TabOrder = 18
      end
      object NextTag: TButton
        Left = 0
        Top = 153
        Width = 37
        Height = 25
        Caption = '>>'
        TabOrder = 19
        OnClick = NextTagClick
      end
      object FindDELE: TButton
        Left = 37
        Top = 153
        Width = 75
        Height = 25
        Caption = 'FindDELE'
        TabOrder = 20
        OnClick = FindDELEClick
      end
      object DelDials: TButton
        Left = 112
        Top = 153
        Width = 75
        Height = 25
        Caption = 'DelDials'
        TabOrder = 21
        OnClick = DelDialsClick
      end
      object ExportBtn: TButton
        Left = 0
        Top = 178
        Width = 65
        Height = 25
        Hint = #1069#1082#1089#1087#1086#1088#1090' '#1074#1099#1076#1077#1083#1077#1085#1085#1099#1093' '#1089#1090#1088#1086#1082
        Caption = 'Export'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 22
        OnClick = ExportBtnClick
      end
    end
    object NextS: TStaticText
      Left = 1
      Top = 1
      Width = 213
      Height = 22
      Align = alTop
      BevelKind = bkFlat
      Caption = 'Next=TES3[0]'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = NextSClick
      OnContextPopup = NextSContextPopup
    end
    object LenSize: TRadioGroup
      Left = 1
      Top = 42
      Width = 213
      Height = 39
      Align = alTop
      Caption = #1044#1083#1080#1085#1072' '#1087#1086#1083#1103' Length'
      Columns = 2
      ItemIndex = 0
      Items.Strings = (
        '4 '#1073#1072#1081#1090#1072
        '12 '#1073#1072#1081#1090)
      TabOrder = 3
    end
    object Reinter: TCheckBox
      Left = 1
      Top = 81
      Width = 213
      Height = 17
      Align = alTop
      Caption = #1056#1077#1080#1085#1090#1088#1077#1088#1087#1088#1077#1090#1080#1088#1086#1074#1072#1090#1100' '#1087#1088#1077#1076#1099#1076#1091#1097#1077#1077
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
  end
  object PanelList: TPanel
    Left = 0
    Top = 27
    Width = 281
    Height = 498
    Align = alLeft
    Caption = 'PanelList'
    ShowCaption = False
    TabOrder = 3
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 279
      Height = 41
      Align = alTop
      BevelKind = bkSoft
      BevelOuter = bvNone
      Caption = 'Panel4'
      ShowCaption = False
      TabOrder = 0
      object ToE: TLabeledEdit
        Left = 24
        Top = 5
        Width = 72
        Height = 27
        EditLabel.Width = 19
        EditLabel.Height = 19
        EditLabel.Caption = 'To'
        EditLabel.OnClick = ToESubLabelClick
        LabelPosition = lpLeft
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '0'
      end
      object Go: TButton
        Left = 99
        Top = 5
        Width = 32
        Height = 25
        Caption = 'Go'
        TabOrder = 1
        OnClick = GoClick
      end
      object Delete: TButton
        Left = 197
        Top = 5
        Width = 62
        Height = 25
        Hint = #1059#1076#1072#1083#1080#1090#1100' '#1090#1077#1075' (Delete)'
        Caption = 'Delete'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = DeleteClick
      end
      object Refresh: TButton
        Left = 132
        Top = 5
        Width = 62
        Height = 25
        Caption = 'Refresh'
        TabOrder = 3
        OnClick = RefreshClick
      end
    end
    object HeaderControl1: THeaderControl
      AlignWithMargins = True
      Left = 4
      Top = 45
      Width = 273
      Height = 17
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      Sections = <
        item
          ImageIndex = -1
          Text = 'Header'
          Width = 36
        end
        item
          ImageIndex = -1
          Text = 'Start'
          Width = 46
        end
        item
          ImageIndex = -1
          Text = 'Size'
          Width = 42
        end
        item
          AutoSize = True
          ImageIndex = -1
          Text = 'Data'
          Width = 149
        end>
      OnSectionClick = HeaderControl1SectionClick
      OnSectionResize = HeaderControl1SectionResize
      ParentFont = False
      OnResize = HeaderControl1Resize
    end
    object List: TStringGrid
      Left = 1
      Top = 65
      Width = 279
      Height = 432
      Align = alClient
      ColCount = 4
      DefaultColWidth = 60
      DefaultRowHeight = 16
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goRangeSelect, goRowSelect, goThumbTracking]
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 2
      OnKeyUp = ListKeyUp
      OnSelectCell = ListSelectCell
    end
  end
  object Panel1: TPanel
    Left = 284
    Top = 27
    Width = 260
    Height = 498
    Align = alLeft
    Caption = 'PanelList'
    ShowCaption = False
    TabOrder = 4
    object HeaderControl2: THeaderControl
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 252
      Height = 17
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      Sections = <
        item
          ImageIndex = -1
          Text = 'Header'
          Width = 36
        end
        item
          ImageIndex = -1
          Text = 'Start'
          Width = 46
        end
        item
          ImageIndex = -1
          Text = 'Size'
          Width = 42
        end
        item
          AutoSize = True
          ImageIndex = -1
          Text = 'Data'
          Width = 128
        end>
      OnSectionResize = HeaderControl1SectionResize
      ParentFont = False
      OnResize = HeaderControl1Resize
    end
    object List2: TStringGrid
      Left = 1
      Top = 24
      Width = 258
      Height = 473
      Align = alClient
      ColCount = 4
      DefaultColWidth = 60
      DefaultRowHeight = 16
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goRangeSelect, goRowSelect, goThumbTracking]
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 1
      OnSelectCell = List2SelectCell
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 312
    Top = 104
  end
end
