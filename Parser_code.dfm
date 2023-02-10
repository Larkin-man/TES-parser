object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = 'TES parseer'
  ClientHeight = 757
  ClientWidth = 1097
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnHelp = FormHelp
  PixelsPerInch = 96
  TextHeight = 19
  object Splitter1: TSplitter
    Left = 601
    Top = 27
    Height = 730
    ExplicitLeft = 546
    ExplicitTop = 32
    ExplicitHeight = 547
  end
  object Splitter2: TSplitter
    Left = 305
    Top = 27
    Height = 730
    ExplicitLeft = 276
    ExplicitTop = -3
    ExplicitHeight = 498
  end
  object PanelUp: TPanel
    Left = 0
    Top = 0
    Width = 1097
    Height = 27
    Align = alTop
    Caption = 'PanelUp'
    PopupMenu = PopupMenu1
    ShowCaption = False
    TabOrder = 0
    ExplicitWidth = 1072
    object LDele: TLabel
      AlignWithMargins = True
      Left = 517
      Top = 4
      Width = 107
      Height = 19
      Align = alLeft
      Caption = 'Deleted Size=0'
      Visible = False
      ExplicitLeft = 452
    end
    object Bevel1: TBevel
      Left = 193
      Top = 1
      Width = 3
      Height = 25
      Align = alLeft
      Shape = bsSpacer
      ExplicitLeft = 130
    end
    object OpenBtn: TButton
      Left = 1
      Top = 1
      Width = 64
      Height = 25
      Hint = 'Open es*'
      Align = alLeft
      Caption = 'Open'
      ImageIndex = 0
      Images = ImageList1
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = OpenBtnClick
    end
    object Save: TButton
      Left = 65
      Top = 1
      Width = 64
      Height = 25
      Align = alLeft
      Caption = 'Save'
      Enabled = False
      ImageIndex = 1
      Images = ImageList1
      TabOrder = 1
      OnClick = SaveClick
    end
    object EFinds: TEdit
      Left = 196
      Top = 1
      Width = 261
      Height = 25
      Hint = 'Search in data'
      ParentCustomHint = False
      Align = alLeft
      BiDiMode = bdLeftToRight
      Ctl3D = True
      DoubleBuffered = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentBiDiMode = False
      ParentCtl3D = False
      ParentDoubleBuffered = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      ExplicitHeight = 27
    end
    object FindStr: TButton
      Left = 457
      Top = 1
      Width = 57
      Height = 25
      Hint = 'Find string in Data'
      Align = alLeft
      Caption = 'Find'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Pitch = fpVariable
      Font.Style = []
      ImageIndex = 2
      Images = ImageList1
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = FindStrClick
    end
    object ProModeCK: TCheckBox
      AlignWithMargins = True
      Left = 630
      Top = 4
      Width = 97
      Height = 19
      Align = alLeft
      Caption = 'Pro mode'
      Checked = True
      State = cbChecked
      TabOrder = 4
      OnClick = ProModeCKClick
    end
    object Save2: TButton
      Left = 129
      Top = 1
      Width = 64
      Height = 25
      Align = alLeft
      Caption = 'Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 53
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ImageIndex = 3
      Images = ImageList1
      ParentFont = False
      TabOrder = 5
      Visible = False
      OnClick = Save2Click
    end
  end
  object Out: TMemo
    Left = 604
    Top = 27
    Width = 278
    Height = 730
    Align = alClient
    Lines.Strings = (
      'List'
      'List2'
      'Out')
    ScrollBars = ssVertical
    TabOrder = 3
    ExplicitLeft = 588
    ExplicitWidth = 269
  end
  object PanelPRO: TPanel
    Left = 882
    Top = 27
    Width = 215
    Height = 730
    Align = alRight
    ShowCaption = False
    TabOrder = 4
    ExplicitLeft = 857
    object FlowPanel1: TFlowPanel
      AlignWithMargins = True
      Left = 3
      Top = 237
      Width = 209
      Height = 490
      Margins.Left = 2
      Margins.Top = 1
      Margins.Right = 2
      Margins.Bottom = 2
      Align = alClient
      BevelKind = bkTile
      BevelOuter = bvNone
      Caption = 'FlowPanel1'
      ShowCaption = False
      TabOrder = 0
      object TES3Read: TButton
        Left = 0
        Top = 0
        Width = 50
        Height = 25
        Hint = 'read TES3 header'
        Caption = 'TES3'
        Enabled = False
        TabOrder = 0
        OnClick = TES3ReadClick
      end
      object CellRead: TButton
        Left = 50
        Top = 0
        Width = 50
        Height = 25
        Caption = 'CELL'
        TabOrder = 1
        OnClick = CellReadClick
      end
      object GMDTRead: TButton
        Left = 100
        Top = 0
        Width = 50
        Height = 25
        Caption = 'sGMDT'
        TabOrder = 2
        Visible = False
        OnClick = GMDTReadClick
      end
      object TestP: TButton
        Left = 150
        Top = 0
        Width = 49
        Height = 25
        Caption = 'TestP'
        TabOrder = 4
        Visible = False
        OnClick = TestPClick
      end
      object SPELread: TButton
        Left = 0
        Top = 25
        Width = 50
        Height = 25
        Caption = 'SPEL'
        TabOrder = 5
        Visible = False
        OnClick = SPELreadClick
      end
      object DelTrash: TButton
        Left = 50
        Top = 25
        Width = 97
        Height = 25
        Hint = 'Auto delete trash CELL'
        Caption = 'Delete Trash'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 13
        OnClick = DelTrashClick
      end
      object SPLMread: TButton
        Left = 147
        Top = 25
        Width = 49
        Height = 25
        Caption = 'SPLM'
        TabOrder = 10
        Visible = False
        OnClick = SPLMreadClick
      end
      object ExportBtn: TButton
        Left = 0
        Top = 50
        Width = 58
        Height = 25
        Hint = 'Export selected records'
        Caption = 'Export'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 8
        OnClick = ExportBtnClick
      end
      object ExportSPEL: TButton
        Left = 58
        Top = 50
        Width = 96
        Height = 25
        Hint = 'Export selected SPEL'
        Caption = 'Export SPEL'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        OnClick = ExportSPELClick
      end
      object FindNext: TButton
        Left = 0
        Top = 75
        Width = 79
        Height = 25
        Hint = 'Find text from string field. Right click is delete.'
        Caption = 'Find Next'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        OnClick = FindNextClick
        OnContextPopup = NextTagContextPopup
      end
      object FindinList2: TButton
        Left = 79
        Top = 75
        Width = 113
        Height = 25
        Hint = 'Find in List 2'
        Caption = 'Find in subList'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 15
        OnClick = FindinList2Click
      end
      object DelDials: TButton
        Left = 0
        Top = 100
        Width = 145
        Height = 25
        Hint = 'Delete DIAL'#39's without INFO'
        Caption = 'Delete trash DIAL'#39's'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 9
        OnClick = DelDialsClick
      end
      object DelDatas: TButton
        Left = 0
        Top = 125
        Width = 77
        Height = 25
        Hint = 'Find and delete Headers with Data'#39's from Memo strings.'
        Caption = 'Del Datas'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = DelDatasClick
      end
      object PushCoord: TButton
        Left = 77
        Top = 125
        Width = 121
        Height = 25
        Hint = 
          'For CELL and PGRD subtracts number from coordinates of all objec' +
          'ts'
        Caption = 'Swap coordinates'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 11
        OnClick = PushCoordClick
      end
      object Wordwap: TCheckBox
        AlignWithMargins = True
        Left = 2
        Top = 153
        Width = 95
        Height = 17
        Margins.Left = 2
        Margins.Right = 1
        Caption = 'Word wrap'
        Checked = True
        State = cbChecked
        TabOrder = 12
        OnClick = WordwapClick
      end
      object Sel: TButton
        Left = 98
        Top = 150
        Width = 56
        Height = 25
        Hint = 'Start clicking on the selected rows in List1'
        Caption = 'Select'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 14
        OnClick = SelClick
      end
      object NextCell: TButton
        Left = 0
        Top = 175
        Width = 71
        Height = 25
        Hint = 'Next CELL LAND PGRD'
        Caption = 'Next cell'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 20
        OnClick = NextCellClick
      end
      object DeleteExtraData: TButton
        Left = 71
        Top = 175
        Width = 129
        Height = 25
        Hint = 
          'Deletes from a sheet 2 all up to the following heading FRMR NAM0' +
          ' DATA'
        Caption = 'Delete ExtraData'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 16
        OnClick = DeleteExtraDataClick
      end
      object FindinSublists: TButton
        Left = 0
        Top = 200
        Width = 120
        Height = 25
        Hint = 'Find in all list and sublists'
        Caption = 'Find in sublistS'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 21
        OnClick = FindinSublistsClick
      end
      object setlocaleBtn: TButton
        Left = 120
        Top = 200
        Width = 75
        Height = 25
        Hint = 'setlocale for Localized. '#39'Find'#39' field is locale page'
        Caption = 'setlocale'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 19
        OnClick = setlocaleBtnClick
      end
      object CheckCoord: TButton
        Left = 0
        Top = 225
        Width = 129
        Height = 25
        Hint = 'max min of Z of selected CELL'
        Caption = 'Check coordinate'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 17
        OnClick = CheckCoordClick
        OnContextPopup = CheckCoordContextPopup
      end
      object AddMainFields: TButton
        Left = 0
        Top = 250
        Width = 121
        Height = 25
        Hint = 'To add bytes after the size'
        Caption = 'Add main fields'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 18
        OnClick = AddMainFieldsClick
      end
      object Replace: TButton
        Left = 0
        Top = 275
        Width = 94
        Height = 25
        Hint = 'Replace @# to {} from clipboard'
        Caption = 'Replace @#'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 22
        OnClick = ReplaceClick
      end
      object CheckConflicts: TButton
        Left = 0
        Top = 300
        Width = 113
        Height = 25
        Hint = 'Check plugin same identifiers'
        Caption = 'Check conflicts'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 23
        OnClick = CheckConflictsClick
      end
      object PrepareE: TButton
        Left = 0
        Top = 325
        Width = 145
        Height = 25
        Hint = 'akella\1C. INFO replace @# to {}'
        Caption = 'Prepare for editing'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 24
        OnClick = PrepareEClick
      end
      object PrepareGame: TButton
        Left = 0
        Top = 350
        Width = 136
        Height = 25
        Hint = 'akella\1C. INFO replace @# to {}'
        Caption = 'Prepare for game'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 25
        OnClick = PrepareGameClick
      end
    end
    object PanelSubRead: TPanel
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 209
      Height = 232
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 1
      Align = alTop
      BevelKind = bkTile
      BevelOuter = bvNone
      Caption = 'PanelSubRead'
      ShowCaption = False
      TabOrder = 1
      object Label1: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 25
        Width = 198
        Height = 19
        Margins.Left = 4
        Margins.Top = 0
        Margins.Bottom = 0
        Align = alTop
        Caption = 'Read subheader: '
        ExplicitWidth = 124
      end
      object ButtonGroup1: TButtonGroup
        AlignWithMargins = True
        Left = 3
        Top = 47
        Width = 199
        Height = 160
        Margins.Bottom = 1
        Align = alTop
        BorderStyle = bsNone
        ButtonHeight = 26
        ButtonOptions = [gboFullSize, gboShowCaptions]
        Items = <
          item
            Caption = 'Integer 4 bytes'
          end
          item
            Caption = 'String'
          end
          item
            Caption = 'Localized symbol'
          end
          item
            Caption = 'Float 4 bytes'
          end
          item
            Caption = 'Integer 2 bytes'
          end
          item
            Caption = 'Integer 1 bytes'
          end>
        TabOrder = 0
        OnButtonClicked = ButtonGroup1ButtonClicked
      end
      object Reinter: TCheckBox
        AlignWithMargins = True
        Left = 6
        Top = 208
        Width = 196
        Height = 17
        Margins.Left = 6
        Margins.Top = 0
        Margins.Bottom = 2
        Align = alTop
        Caption = 'Reinterpet previous'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object NextS: TStaticText
        AlignWithMargins = True
        Left = 3
        Top = 2
        Width = 199
        Height = 22
        Margins.Top = 2
        Margins.Bottom = 1
        Align = alTop
        BevelKind = bkTile
        BevelOuter = bvNone
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
    end
  end
  object PanelList: TPanel
    Left = 0
    Top = 27
    Width = 305
    Height = 730
    Align = alLeft
    Caption = 'PanelList'
    ShowCaption = False
    TabOrder = 1
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 303
      Height = 40
      Align = alTop
      BevelKind = bkTile
      BevelOuter = bvNone
      Caption = 'Panel4'
      ShowCaption = False
      TabOrder = 0
      object Secr: TLabel
        Left = 243
        Top = 7
        Width = 30
        Height = 19
        Caption = 'Secr'
        Visible = False
      end
      object ToE: TLabeledEdit
        Left = 24
        Top = 4
        Width = 107
        Height = 27
        EditLabel.Width = 19
        EditLabel.Height = 19
        EditLabel.Caption = 'To'
        EditLabel.OnClick = ToESubLabelClick
        LabelPosition = lpLeft
        NumbersOnly = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = '0'
      end
      object Go: TButton
        Left = 137
        Top = 5
        Width = 32
        Height = 25
        Hint = 'Go to Offset'
        Caption = 'Go'
        TabOrder = 1
        OnClick = GoClick
      end
      object Delete: TButton
        Left = 177
        Top = 5
        Width = 62
        Height = 25
        Hint = 'Delete selected header'
        Caption = 'Delete'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = DeleteClick
      end
    end
    object HeaderControl1: THeaderControl
      AlignWithMargins = True
      Left = 4
      Top = 44
      Width = 297
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
          Text = 'Offset'
          Width = 49
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
          Width = 170
        end>
      OnSectionClick = HeaderControl1SectionClick
      OnSectionResize = HeaderControl1SectionResize
      ParentFont = False
      OnResize = HeaderControl1Resize
    end
    object List: TStringGrid
      Left = 1
      Top = 64
      Width = 303
      Height = 665
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
  object PanelList2: TPanel
    Left = 308
    Top = 27
    Width = 293
    Height = 730
    Align = alLeft
    Caption = 'PanelList'
    ShowCaption = False
    TabOrder = 2
    object HeaderControl2: THeaderControl
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 285
      Height = 17
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      Sections = <
        item
          AllowClick = False
          ImageIndex = -1
          Text = 'Header'
          Width = 36
        end
        item
          AllowClick = False
          ImageIndex = -1
          Text = 'Offset'
          Width = 49
        end
        item
          AllowClick = False
          ImageIndex = -1
          Text = 'Size'
          Width = 42
        end
        item
          AllowClick = False
          AutoSize = True
          ImageIndex = -1
          Text = 'Data'
          Width = 128
        end
        item
          AllowClick = False
          ImageIndex = -1
          Text = 'Type'
          Width = 30
        end>
      Style = hsFlat
      OnSectionResize = HeaderControl2SectionResize
      ParentFont = False
      OnResize = HeaderControl2Resize
      ExplicitWidth = 279
    end
    object List2: TStringGrid
      Left = 1
      Top = 24
      Width = 291
      Height = 679
      Align = alClient
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
      PopupMenu = PopupMenu1
      ScrollBars = ssVertical
      TabOrder = 1
      OnKeyUp = List2KeyUp
      OnSelectCell = List2SelectCell
      ExplicitWidth = 275
    end
    object SubDescript: TEdit
      Left = 1
      Top = 703
      Width = 291
      Height = 26
      Align = alBottom
      Color = cl3DLight
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = 'SubDescript'
      Visible = False
      ExplicitWidth = 275
    end
  end
  object OpenDialog1: TOpenDialog
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing, ofDontAddToRecent]
    Left = 184
    Top = 128
  end
  object PopupMenu1: TPopupMenu
    Left = 416
    Top = 128
    object NFindSub: TMenuItem
      Caption = 'Find in Sublist'
      OnClick = FindinList2Click
    end
    object FindHeader1: TMenuItem
      Caption = 'Find Header'
      OnClick = FindHeader1Click
    end
    object TestMenu: TMenuItem
      Caption = 'test'
      Visible = False
      OnClick = TestMenuClick
    end
    object NList2: TMenuItem
      AutoCheck = True
      Caption = 'Enable Sublist'
      Checked = True
      OnClick = NList2Click
    end
    object ClearOut: TMenuItem
      AutoCheck = True
      Caption = 'Clearing out'
      OnClick = ClearOutClick
    end
    object EnableLsit2Delete1: TMenuItem
      AutoCheck = True
      Caption = 'Enable Subheader Delete'
      Enabled = False
      OnClick = EnableLsit2Delete1Click
    end
    object Rewrites: TMenuItem
      AutoCheck = True
      Caption = 'Rewrites'
      OnClick = RewritesClick
    end
    object ShowData1: TMenuItem
      AutoCheck = True
      Caption = 'Show Data'
      Checked = True
    end
    object BreakIf0Len: TMenuItem
      AutoCheck = True
      Caption = 'BreakIf0Len'
      Checked = True
    end
  end
  object ImageList1: TImageList
    Left = 96
    Top = 128
    Bitmap = {
      494C010104000A00800010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E6DED500BFAA9300B39A80000000000000000000000000000000
      00000000000000000000000000000000000000000000E8E8E800E5E5E500E5E5
      E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E5E5E500E5E5E500E8E8E800000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000E8E8E800E5E5E500E5E5
      E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E5E5E500E5E5E500E8E8E80000000000000000000000000000000000D6C8
      B800B2997B00AB8C6900BC9C77009B7A57000000000000000000000000000000
      000000000000000000000000000000000000E1E1E100D1D1D100B0A29C008750
      3900777777007777770077777700777777007777770077777700875039008750
      39008750390087503900906E6100E1E1E1000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C4C4C400000000000000000000000000E1E1E100D1D1D100A7A0A9006648
      6D0077777700777777007777770077777700777777007777770066486D006648
      6D0066486D0066486D007B6A8000E1E1E1000000000000000000A28F7A00AA88
      6200C6A48100D4B39100D7B694007A5020008D7051008D7051009D7D5A009D7D
      5A009D7D5A00A28F7A000000000000000000FFFFFF00D3C5BE00955D4600A16A
      5400B9B9AB00B17C6500B17C6500B5B5A700B5B5A700B9B9AB00B17C6500C38E
      7700C08B7400D6A18A00955D4600FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008080
      8000000000002F2F2F000000000000000000FFFFFF00CAC2CC0071557A007F63
      8700B3ACB0008F7397008F739700AFA8AC00AFA8AC00B3ACB0008F739700A185
      A9009E82A600B498BC0071557A00FFFFFF000000000000000000A5886500D8B8
      9600CFAF8D00CFAF8D00D8B8960090643500BFA28300BFA28300D5B49200D5B4
      9200DAB99700A5886500CCC4BA0000000000FFFFFF00A36C5400D49F8800AD78
      6100BABAB100AD786100AD786100B1B1A800B1B1A800BABAB100AD786100C38E
      7700B8836C00D8A38C00A36C5400FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000A4A4A4000202
      020035353500515151000000000000000000FFFFFF0080638800B296BA008B6F
      9300B6B2B4008B6F93008B6F9300ADA9AB00ADA9AB00B6B2B4008B6F9300A185
      A900967A9E00B69ABE0080638800FFFFFF000000000000000000AE947100D9BB
      9900D0B29000D0B29000D9BB99008E643400B99C7E00B99C7E00CEAE8C00CEAE
      8C00CEAE8C00D7B79500AE94710000000000FFFFFF00A86F5800D29D8600A974
      5D00C0C0BB009560490095604900B7B7B200B7B7B200C0C0BB00A9745D00C38E
      7700B07B6400DAA58E00A86F5800FFFFFF000000000000000000000000000000
      00000000000000000000000000000000000000000000A4A4A400000000004646
      460053535300000000000000000000000000FFFFFF0084678D00B094B800876B
      8F00BEBBBD0073577B0073577B00B5B2B400B5B2B400BEBBBD00876B8F00A185
      A9008E729600B89CC00084678D00FFFFFF000000000000000000B1997500DBBE
      9C00D2B59300D2B59300DBBE9C0091673700BA9E8000BA9E8000CFB08E00CFB0
      8E00CFB08E00D8B99700B199750000000000FFFFFF00AC735C00D5A08900A570
      5900C8C8C700C3C3C200C3C3C200C3C3C200C3C3C200C8C8C700A5705900C38E
      7700A8735C00DDA89100AC735C00FFFFFF00000000000000000000000000D4D5
      D600D8D1CC00D9C8BA00D9C7B900DAD1CA00D1D2D300424242003D3C3C006363
      630000000000000000000000000000000000FFFFFF00886B9100B397BB008367
      8B00C8C7C700C3C2C200C3C2C200C3C2C200C3C2C200C8C7C70083678B00A185
      A900866A8E00BB9FC300886B9100FFFFFF000000000000000000B49C7900DCC0
      9E00D3B79500D3B79500DCC09E00936A3A00BCA18200BCA18200D1B39100D1B3
      9100D1B39100DABC9A00B49C790000000000FFFFFF00AF776000D9A48D00A26D
      5600A16C5500A16C5500A16C5500A16C5500A16C5500A16C5500A26D5600A26D
      5600A26D5600E1AC9500AF776000FFFFFF000000000000000000D4D6D600DFCE
      C000DDC0A800EAD6C300EDDDCD00E1C9B400E3CEBD00B1B2B200A2A2A3000000
      000000000000000000000000000000000000FFFFFF008B6F9400B79BBF008064
      88007F6387007F6387007F6387007F6387007F6387007F638700806488008064
      880080648800BFA3C7008B6F9400FFFFFF000000000000000000B6A07C00DDC3
      A100D4BA9800D4BA9800DDC3A100966E3D00BDA48600BDA48600D3B79500D3B7
      9500D3B79500DCC09E00B6A07C0000000000FFFFFF00B27A6300DCA79000D39E
      8700D39E8700D39E8700D39E8700D39E8700D39E8700D39E8700D39E8700D39E
      8700D39E8700DCA79000B27A6300FFFFFF000000000000000000DCCFC500E1C4
      AB00F6E6D600F8EADD00FAF1E800FDF9F500E7D1C000E3D1C300E2E3E4000000
      000000000000000000000000000000000000FFFFFF008E729700BA9EC200B195
      B900B195B900B195B900B195B900B195B900B195B900B195B900B195B900B195
      B900B195B900BA9EC2008E729700FFFFFF000000000000000000B8A27F00DFC7
      A500D6BE9C00D6BE9C00DFC7A50098724000BEA78900BEA78900D4BA9800D4BA
      9800D4BA9800DDC3A100B8A27F0000000000FFFFFF00B57D6600E0AB9400F0EE
      DD00E4F4E400E4F4E400E4F4E400E4F4E400E4F4E400E4F4E400E4F4E400E4F4
      E400F0EEDD00E0AB9400B57D6600FFFFFF0000000000CDCDCD00E0C9B600F5E8
      DE00F6E9DD00F5E5D500F6E8DA00F8EDE300F8EEE400DEC4AF00CBCAC9000000
      000000000000000000000000000000000000FFFFFF0091759A00BEA2C600E8DF
      E400F1EDE900F1EDE900F1EDE900F1EDE900F1EDE900F1EDE900F1EDE900F1ED
      E900E8DFE400BEA2C60091759A00FFFFFF000000000000000000BAA58200E0CA
      A800D7C19F00D7C19F00E0CAA8009A764300C0AB8C00C0AB8C00D6BE9C00D6BE
      9C00DFC7A500BAA58200E6DFD20000000000FFFFFF00B8806900E3AE9700EAF6
      E900E0ECDF00E0ECDF00E0ECDF00E0ECDF00E0ECDF00E0ECDF00E0ECDF00E0EC
      DF00EAF6E900E3AE9700B8806900FFFFFF0000000000C9C8C700E1C9B600F9F2
      EC00F7EDE400F7EBE000F5E6D700F5E5D700F7E9DD00E0C5AE00CAC7C4000000
      000000000000000000000000000000000000FFFFFF0094789D00C1A5C900F3F0
      ED00E9E6E300E9E6E300E9E6E300E9E6E300E9E6E300E9E6E300E9E6E300E9E6
      E300F3F0ED00C1A5C90094789D00FFFFFF000000000000000000BCA88500E2CC
      AA00D9C3A100D9C3A100E2CCAA009D7A4600C2AD8F00C2AD8F00D8C19F00D8C1
      9F00E1CAA800BCA885000000000000000000FFFFFF00BB846D00E6B19A00EFF8
      EF00E7F1E700E7F1E700E7F1E700E7F1E700E7F1E700E7F1E700E7F1E700E7F1
      E700EFF8EF00E6B19A00BB846D00FFFFFF0000000000CECECE00E1CCBC00F8F1
      EB00F9F2EB00F7EDE300F5E8DD00F5E4D400F2E1D100DEC3AE00CECECD000000
      000000000000000000000000000000000000FFFFFF009A7CA100C4A8CC00F6F4
      F200EFEDEA00EFEDEA00EFEDEA00EFEDEA00EFEDEA00EFEDEA00EFEDEA00EFED
      EA00F6F4F200C4A8CC009A7CA100FFFFFF000000000000000000BDAA8700E3CF
      AD00DAC6A400DAC6A400E3CFAD00A3824E00C4B19200C4B19200DAC5A300DAC5
      A300E3CEAC00BDAA87000000000000000000FFFFFF00BD867000EAB59E00F6FB
      F500F1F6F000F1F6F000F1F6F000F1F6F000F1F6F000F1F6F000F1F6F000F1F6
      F000F6FBF500EAB59E00BD867000FFFFFF000000000000000000DDD2C900E9D5
      C500FCF9F700FBF6F000FAF3EB00F7E9DB00E2C5AC00DFCFC400DCDDDE000000
      000000000000000000000000000000000000FFFFFF009B7FA300C8ACD000F9F7
      F700F4F2F200F4F2F200F4F2F200F4F2F200F4F2F200F4F2F200F4F2F200F4F2
      F200F9F7F700C8ACD0009B7FA300FFFFFF000000000000000000BFAC8A00E4D2
      B000DBC9A700DBC9A700E5D2B000A6895500D1BF9E00D9C7A500DBC8A600DBC8
      A600E4D1AF00BFAC8A000000000000000000FFFFFF00BF887200ECB7A000FBFE
      FB00F8FBF800F8FBF800F8FBF800F8FBF800F8FBF800F8FBF800F8FBF800F8FB
      F800FBFEFB00ECB7A000BF887200FFFFFF000000000000000000D2D3D400DDD0
      C500E4CEBC00EEDED200ECDCCD00E0C6B000DCCABC00D2D3D300FAFAFA000000
      000000000000000000000000000000000000FFFFFF009D81A500CAAED200FEFD
      FC00FBFAF900FBFAF900FBFAF900FBFAF900FBFAF900FBFAF900FBFAF900FBFA
      F900FEFDFC00CAAED2009D81A500FFFFFF000000000000000000C0AE8C00E6D4
      B200E2D1AF00DAC59F00C5AC7E00AA8F5B00DCCAA800DCCAA800DCCAA800DCCA
      A800E5D3B100C0AE8C000000000000000000FFFFFF00C18B7400F2BDA600FFFF
      FF00FEFFFE00FEFFFE00FEFFFE00FEFFFE00FEFFFE00FEFFFE00FEFFFE00FEFF
      FE00FFFFFF00F2BDA600C18B7400FFFFFF000000000000000000FEFEFE00D8D9
      DA00D6D1CD00D6C7BC00D6C7BC00D7D1CD00D5D7D800FBFBFB00000000000000
      000000000000000000000000000000000000FFFFFF009F83A700D0B4D800FFFF
      FF00FFFFFE00FFFFFE00FFFFFE00FFFFFE00FFFFFE00FFFFFE00FFFFFE00FFFF
      FE00FFFFFF00D0B4D8009F83A700FFFFFF000000000000000000C2B08E00D7C0
      9600BFA57200BEA47200D2BD9200E2D0AD00E6D4B200E6D4B200E6D4B200E6D4
      B200EAD9B700C2B08E000000000000000000FFFFFF00D1A89800C28D7600BCCC
      BB00BCCCBB00BCCCBB00BCCCBB00BCCCBB00BCCCBB00BCCCBB00BCCCBB00BCCC
      BB00BCCCBB00C28D7600D1A89800FFFFFF000000000000000000000000000000
      000000000000E1E2E300E0E1E200000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00B8A3BE00A084A800C8C4
      C000C8C4C000C8C4C000C8C4C000C8C4C000C8C4C000C8C4C000C8C4C000C8C4
      C000C8C4C000A084A800B8A3BE00FFFFFF000000000000000000D1C4AA00C2B1
      8F00C2B18F00C2B18F00C2B18F00C2B18F00C2B18F00C2B18F00C2B18F00C2B1
      8F00C2B18F00D1C4AA000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000F8FF8001FFFF8001E0FF0000FFF70000
      C0030000FFE30000C0010000FFC30000C0010000FF870000C0010000E00F0000
      C0010000C01F0000C0010000C01F0000C0010000801F0000C0010000801F0000
      C0030000801F0000C0030000C01F0000C0030000C01F0000C0030000C03F0000
      C0030000F9FF0000C0030000FFFF000000000000000000000000000000000000
      000000000000}
  end
end
