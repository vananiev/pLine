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
   vector<PBase*> *Nodes;   	// ������ ���� �������� �� �����
   PLinkComponent *Links;     // ����� ����� ��������
   Float get_signal();        // �������� �������� ����������� ���������
   int Phase;						// � ����� ���� �������� ������
   ofstream fout;             // ����, � ������� ��������� ��������
   String File;					// ��� ����� � ������� �����
   Float last_Time;				// ����� ����������� ���������� �������
   Float FileSaveStep;        // ��������� ��� ������ � ����
   Float LastFileSaveTime;
public:		// User declarations
   __fastcall TfrmScope(TComponent* Owner);
   grid *obj;
   Graphics::TBitmap *out;
   Types::TRect Rect;   // ������ ������� ������������� canvas
   TRect curve_Rect;    // ������������� ��� ������ ������
   // ���������� �������� �������
   Float Xmax;
   Float Xmin;
   Float Ymax;
   Float Ymin;
   vector<Float> X;
   vector<Float> Y;

   Float *Signal;       // ��������� ������
   link* Link;          // � ����� ����� ������� ���������
   bool  FromStart;     // � ������ ��� ����� ����� ������� ���������
   node* Node;          // � ������ ���� ������� ���������

   double Grid_Step;        	// ����� �������. ���
   unsigned border_X;         // ���� �� ��� X
   unsigned border_Y;         // ���� �� ��� Y
   bool Scale_To;             // �� X ��� �� Y ����� �������������
   int Mouse_Down_X;          // ���������� ��� ������� �� �����
   int Mouse_Down_Y;          // ���������� ��� ������� �� �����
   Float Mouse_Down_Xmin;    // ���������� ��� ������� �� �����
   Float Mouse_Down_Xmax;    // ���������� ��� ������� �� �����
   Float Mouse_Down_Ymin;    // ���������� ��� ������� �� �����
   Float Mouse_Down_Ymax; // ���������� ��� ������� �� �����
   Float Main_X; Float Main_Y; // ���������� ������� (���������� ������) ����
   bool Move_Graph;     // ������������� ������
   bool Mouse_Scale;    // ������������ ������ ������
   bool ScopeAutoRefresh;		// ����� �� get_signal() ���������� ��-��� ������������?
   void showNodeItems(node *N);// ���������� �� ����� ���� � ���������� ������ ��� ��������� ����
};
//---------------------------------------------------------------------------
extern PACKAGE vector<TfrmScope*> frmScope;
//---------------------------------------------------------------------------
#endif
