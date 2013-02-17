//---------------------------------------------------------------------------

#ifndef ScopeH
#define ScopeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include <vector>
#include "model.h"
#include "pngimage.hpp"
#include "main.h"
#include "kernel.h"
#include <fstream>
//---------------------------------------------------------------------------
class TfrmScope : public TForm
{
__published:	// IDE-managed Components
   TTimer *Timer;
   TPanel *pnlControl;
   TButton *btnStart;
   TButton *btnPause;
   TButton *btnStop;
   TRadioButton *rbCurrent;
   TRadioButton *rbVoltage;
   TComboBox *cmbBranches;
   TComboBox *cmbPot1;
   TPaintBox *Graph;
	TScrollBar *Scroll;
   TPopupMenu *PopupMenu;
   TMenuItem *mnu_Scale;
   TMenuItem *Fit_Graph;
   TMenuItem *Save;
   TSaveDialog *saveDlg;
   TComboBox *cmb_Timer_Value;
   TLabel *Label1;
   TRadioButton *rbTemperature;
   TComboBox *cmbTemperature;
	TMenuItem *mnuToFile;
	TComboBox *cmbPhase;
	TMenuItem *mnuFileSaveStep;
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall btnStartClick(TObject *Sender);
   void __fastcall btnPauseClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall btnStopClick(TObject *Sender);
   void __fastcall FormPaint(TObject *Sender);
	void __fastcall ScrollChange(TObject *Sender);
   void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
   void __fastcall mnu_ScaleClick(TObject *Sender);
   void __fastcall GraphMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall GraphMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall GraphMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall Fit_GraphClick(TObject *Sender);
   void __fastcall SaveClick(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall cmb_Timer_ValueChange(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall cmbBranchesClick(TObject *Sender);
   void __fastcall cmbPot1Change(TObject *Sender);
   void __fastcall cmbTemperatureChange(TObject *Sender);
	void __fastcall mnuToFileClick(TObject *Sender);
   void __fastcall cmbPhaseChange(TObject *Sender);
	void __fastcall mnuFileSaveStepClick(TObject *Sender);
private:	// User declarations
   vector<PBase*> *Nodes;   	// список всех объектов на форме
   PLinkComponent *Links;     // связи между выводами
   Float get_signal();        // получить значение измеряемого параметра
   int Phase;						// с какой фазы получаем сигнал
   ofstream fout;             // файл, в который сохраняем значения
   String File;					// имя файла в который пишем
   Float last_Time;				// время предыдущего обновления графика
   Float FileSaveStep;        // временной шаг записи в файл
   Float LastFileSaveTime;
public:		// User declarations
   __fastcall TfrmScope(TComponent* Owner);
   grid *obj;
   Graphics::TBitmap *out;
   Types::TRect Rect;   // какого размера прямоугольник canvas
   TRect curve_Rect;    // прямоугольник для вывода кривой
   // предельные значения графика
   Float Xmax;
   Float Xmin;
   Float Ymax;
   Float Ymin;
   vector<Float> X;
   vector<Float> Y;

   Float *Signal;       // выводимый сигнал
   link* Link;          // с какой линии снимать показания
   bool  FromStart;     // с начала или конца линии снимать показания
   node* Node;          // с какого узла снимать показания

   double Grid_Step;        	// сетка графика. шаг
   unsigned border_X;         // поле по оси X
   unsigned border_Y;         // поле по оси Y
   bool Scale_To;             // по X или по Y будем мастабировать
   int Mouse_Down_X;          // координаты при нажании по мышке
   int Mouse_Down_Y;          // координаты при нажании по мышке
   Float Mouse_Down_Xmin;    // координаты при нажании по мышке
   Float Mouse_Down_Xmax;    // координаты при нажании по мышке
   Float Mouse_Down_Ymin;    // координаты при нажании по мышке
   Float Mouse_Down_Ymax; // координаты при нажании по мышке
   Float Main_X; Float Main_Y; // координаты главных (выделенных жирным) осей
   bool Move_Graph;     // перетаскиваем график
   bool Mouse_Scale;    // масштабируем график мышкой
   bool ScopeAutoRefresh;		// будет ли get_signal() вызываться из-вне осциллографа?
   void showNodeItems(node *N);// предлагать на выбор токи и напряжения только для заданного узла
};
//---------------------------------------------------------------------------
extern PACKAGE vector<TfrmScope*> frmScope;
//---------------------------------------------------------------------------
#endif
