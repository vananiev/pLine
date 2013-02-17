object frmProperty: TfrmProperty
  Left = 1082
  Top = 35
  Width = 206
  Height = 300
  BorderStyle = bsSizeToolWin
  Caption = #1057#1074#1086#1081#1089#1090#1074#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Fields: TValueListEditor
    Left = 0
    Top = 0
    Width = 169
    Height = 257
    TabOrder = 0
    OnEditButtonClick = FieldsEditButtonClick
    ColWidths = (
      74
      89)
  end
  object openDlg: TOpenDialog
    Top = 24
  end
end
