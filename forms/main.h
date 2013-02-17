//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "model.h"
#include "PBase.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>               // хидер базового графического-компонента для модели
#include "kernel.h"
#include "Scope.h"

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TMainMenu *MainMenu1;
   TMenuItem *mnuFile;
   TMenuItem *mnuTools;
   TMenuItem *mnuBuildGrid;
   TMenuItem *mnuObjLib;
   TLabel *lblSec;
   TStaticText *lblTime;
   TButton *btnStart;
   TButton *btnPause;
   TButton *btnStop;
   TTimer *Timer;
   TSaveDialog *saveDlg;
   TOpenDialog *openDlg;
   TMenuItem *mnuScope;
	TMenuItem *mnuNew;
	TMenuItem *mnuOpen;
	TMenuItem *mnuSave;
	TMenuItem *mnuNumFreq;
   TMenuItem *mnuScopeAutoRefresh;
   TMenuItem *mnuSmp;
	TMenuItem *mnuModeling;
	TMenuItem *mnuStartStop;
	TMenuItem *mnuClear;
   TMenuItem *mnuSaveAs;
   TMenuItem *mnuLine;
   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormDragDrop(TObject *Sender, TObject *Source, int X, int Y);
   void __fastcall FormDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
   void __fastcall mnuObjLibClick(TObject *Sender);
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall btnStartClick(TObject *Sender);
   void __fastcall btnPauseClick(TObject *Sender);
   void __fastcall btnStopClick(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall mnuBuildGridClick(TObject *Sender);
   void __fastcall mnuScopeClick(TObject *Sender);
	void __fastcall mnuSaveClick(TObject *Sender);
	void __fastcall mnuOpenClick(TObject *Sender);
	void __fastcall mnuNewClick(TObject *Sender);
	void __fastcall mnuNumFreqClick(TObject *Sender);
   void __fastcall mnuScopeAutoRefreshClick(TObject *Sender);
   void __fastcall mnuSmpClick(TObject *Sender);
	void __fastcall mnuStartStopClick(TObject *Sender);
	void __fastcall mnuClearClick(TObject *Sender);
   void __fastcall mnuSaveAsClick(TObject *Sender);
   void __fastcall mnuLineClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
private:	// User declarations
   grid *Grid;								   // это сеть, которая будет собираться
   void __fastcall PinMouseDown(TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
   void __fastcall PinDragDrop (System::TObject* Sender, System::TObject* Source, int X, int Y);
   void __fastcall UnitChange(TObject *Sender);
   void __fastcall LinkChange(link *Link);
   void __fastcall UnitDelete(TObject *Sender);
   void AddObject(String TypeName, int X, int Y);
   void CreateControl(String Class, String ModelClass);
   void AddControl(PBase *control);
	void __fastcall LinkDelete(PLinkComponent *Sender);   // вызывается при удалении связи
   void __fastcall LinkAdd(PLinkComponent *Sender);		// вызывается при изменении связей
   void CloseScope();
   bool OpenFile(String name);
   String FileName;
   bool NeedBuild;      // требуется пересобрать схему
   bool LineTesting;    // режим тестирования линии
   String LineTestOutputPath;
   ofstream LineTestFile[15];
   Complex get_complex_current(link* Link, node* Node, int phase);
   Complex get_complex_voltage(node* Node, int phase);
   Float get_temperature(link* Link, int phase);
   void __fastcall writeToFile();
public:		// User declarations
   __fastcall TfrmMain(TComponent* Owner);
   vector<PBase*>       Objects;			// список всех объектов на форме
   PLinkComponent LinkLines;           // связи между выводами
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
