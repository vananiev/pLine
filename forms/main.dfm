object frmMain: TfrmMain
  Left = 465
  Top = 395
  Width = 610
  Height = 323
  Caption = 'Power Lines'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDragDrop = FormDragDrop
  OnDragOver = FormDragOver
  OnMouseDown = FormMouseDown
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object lblSec: TLabel
    Left = 504
    Top = 8
    Width = 21
    Height = 13
    Caption = #1089#1077#1082'.'
  end
  object lblTime: TStaticText
    Left = 464
    Top = 8
    Width = 40
    Height = 20
    Hint = #1058#1077#1082#1091#1097#1077#1077' '#1074#1088#1077#1084#1103
    AutoSize = False
    Caption = '0'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
  end
  object btnStart: TButton
    Left = 528
    Top = 0
    Width = 25
    Height = 25
    Hint = #1057#1090#1072#1088#1090
    Caption = '>'
    Default = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = btnStartClick
  end
  object btnPause: TButton
    Left = 552
    Top = 0
    Width = 25
    Height = 25
    Hint = #1055#1072#1091#1079#1072
    Caption = '| |'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    OnClick = btnPauseClick
  end
  object btnStop: TButton
    Left = 576
    Top = 0
    Width = 25
    Height = 25
    Hint = #1055#1088#1080#1085#1103#1090#1100' '#1101#1090#1086#1090' '#1084#1086#1084#1077#1085#1090' '#1074#1088#1077#1084#1077#1085#1080' '#1079#1072' 0'
    Caption = '< |'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    OnClick = btnStopClick
  end
  object MainMenu1: TMainMenu
    object mnuFile: TMenuItem
      Caption = #1060#1072#1081#1083
      object mnuNew: TMenuItem
        Caption = #1057#1086#1079#1076#1072#1090#1100
        ShortCut = 16462
        OnClick = mnuNewClick
      end
      object mnuOpen: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
        ShortCut = 16463
        OnClick = mnuOpenClick
      end
      object mnuSave: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        ShortCut = 16467
        OnClick = mnuSaveClick
      end
      object mnuSaveAs: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1072#1082
        OnClick = mnuSaveAsClick
      end
    end
    object mnuTools: TMenuItem
      Caption = #1048#1085#1089#1090#1088#1091#1084#1077#1085#1090#1099
      object mnuBuildGrid: TMenuItem
        Caption = #1057#1073#1086#1088#1082#1072' '#1089#1077#1090#1080
        ShortCut = 16504
        OnClick = mnuBuildGridClick
      end
      object mnuObjLib: TMenuItem
        Caption = #1041#1080#1073#1083#1080#1086#1090#1077#1082#1072' '#1086#1073#1098#1077#1082#1090#1086#1074
        OnClick = mnuObjLibClick
      end
      object mnuScope: TMenuItem
        Caption = #1054#1089#1094#1080#1083#1086#1075#1088#1072#1092
        Enabled = False
        ShortCut = 122
        OnClick = mnuScopeClick
      end
      object mnuNumFreq: TMenuItem
        Caption = #1063#1080#1089#1083#1086' '#1075#1072#1088#1084#1086#1085#1080#1082
        OnClick = mnuNumFreqClick
      end
      object mnuSmp: TMenuItem
        Caption = #1064#1072#1075' '#1074#1088#1077#1084#1077#1085#1080
        OnClick = mnuSmpClick
      end
      object mnuScopeAutoRefresh: TMenuItem
        Caption = #1040#1074#1090#1086#1086#1073#1085#1086#1074#1083#1077#1085#1080#1077' '#1086#1089#1094#1080#1083#1083#1086#1075#1088#1072#1092#1072
        OnClick = mnuScopeAutoRefreshClick
      end
    end
    object mnuModeling: TMenuItem
      Caption = #1052#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1077
      object mnuStartStop: TMenuItem
        Caption = #1057#1090#1072#1088#1090'/'#1055#1072#1091#1079#1072
        ShortCut = 120
        OnClick = mnuStartStopClick
      end
      object mnuClear: TMenuItem
        Caption = #1057#1073#1088#1086#1089
        ShortCut = 121
        OnClick = mnuClearClick
      end
      object mnuLine: TMenuItem
        Caption = #1058#1077#1089#1090#1080#1088#1086#1074#1072#1090#1100' '#1083#1080#1085#1080#1102
        OnClick = mnuLineClick
      end
    end
  end
  object Timer: TTimer
    Enabled = False
    Interval = 45
    OnTimer = TimerTimer
    Left = 32
  end
  object saveDlg: TSaveDialog
    DefaultExt = '.png'
    Left = 64
  end
  object openDlg: TOpenDialog
    Left = 96
  end
end
