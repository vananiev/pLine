//---------------------------------------------------------------------------
#ifndef PBaseH
#define PBaseH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <vector>
#include "model.h"

using namespace std;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class PACKAGE PBase : public TGraphicControl  // TGraphicControl ��������� ������� ��������� ����������
{
private:
   typedef TGraphicControl inherited;
	DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
   int MouseDownPos_X, MouseDownPos_Y; // ��� ���� ������ ������ ����
   bool _Drag; // ���� ��������
   void __fastcall PinMouseDown(TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
   void __fastcall PinDragDrop (System::TObject* Sender, System::TObject* Source, int X, int Y);
   void __fastcall PinDragOver(System::TObject* Sender, System::TObject* Source, int X, int Y, TDragState State, bool &Accept);
   int __fastcall XtoParent(int x);
   int __fastcall YtoParent(int y);
protected:
   TPopupMenu *Menu; // ���������� ����
   virtual void __fastcall MenuItemClick(TObject *Sender);         // ���������� ��� ����� �� ������� ����
   element *_Obj;
   //virtual void __fastcall SetParent(TWinControl* AParent);
   TShape* Shape;            // ������-����
   TColor savedColor;
   void __fastcall MouseEnter(TMessage& Msg);
   void __fastcall MouseLeave(TMessage& Msg);
   #pragma warn -8027
   BEGIN_MESSAGE_MAP
      VCL_MESSAGE_HANDLER(CM_MOUSEENTER,TMessage,MouseEnter);
      VCL_MESSAGE_HANDLER(CM_MOUSELEAVE,TMessage,MouseLeave);
   END_MESSAGE_MAP(TGraphicControl);
   #pragma warn -8027
public:
   __fastcall PBase(TComponent* Owner);
   __fastcall ~PBase();
   virtual void __fastcall Paint(void);   // ���������� ��������� ��� ���������� ����������
   __property DragKind  = {default=0};
	__property DragMode  = {default=0};
   __property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
   __property OnEndDrag = {default=0};
   __property OnStartDrag = {default=0};
   TNotifyEvent OnChange;
   TMouseEvent OnPinMouseDown;            // ������� ��������� ��� ����� �� ������
   TDragDropEvent OnPinDragDrop;          // ��������� � ����� �������������� �� ������
   TDragOverEvent OnPinDragOver;          // ��������� � ����� �������������� �� ������
   void Obj(element *obj);               // ������������ ������� ������ � �����������
   element * Obj();           		      // ������������ ������� ������ � �����������
   element* ObjType(String ClassName);   // ������� ������ � ����������� ��� � �����������
   TNotifyEvent OnDelete;						// ���������� ��� ��������
   virtual void Save(ostream &stream);		// ��������� ������� � ������ (saveModel=false ����������� ������ ��������� ��� ������ )
   virtual void Read(istream &stream);		// ��������� ������� � ������
__published:
};




//---------------------------------------------------------------------------
class Pair_Conrtol;
class PLinkComponent{
      vector<Pair_Conrtol> Controls;
      void __fastcall MenuItemClick(TObject *Sender);
      TPopupMenu* Menu;							// ���� ������ �����
      int menuFor;								// ��� ������ �������� ���������� ����
      //void Change();								// ���������� ��� ��������� ����������� �����
   public:
      vector<link*> Links;
      void Add   (Pair_Conrtol pc);  // �������� ����� ����
      void Delete(PBase* pc);			// ������� �����, ��� ������������ ��������� �����
 		//void Delete2(node* nd);					// ������� �����, ��� ������������ ��������� �����
      link* operator[](int i);
      int size();
      int FindAtPos(int X, int Y);	      // ����� ����� �� �����������
      PLinkComponent(TComponent *owner);
      //PLinkComponent operator=(const PLinkComponent &Obj);// ������������
      ~PLinkComponent();
      void ShowMenuFor(int LinkNumber);	// �������� ���� ��� �������� �����
      void ShowMenuFor(int X, int Y);		// �������� ���� ��� ����� � ��������� ������������
      void __fastcall Paint();            // ������������� �����
      TForm* Parent;							   // ��������� �� ������� ������
      TComponent* Owner;							   // ���������-��������
      void Save(ostream &stream);			// ��������� ������� � �����
      void Read(istream &stream);			// ��������� ������� � �����
      void __fastcall (__closure *OnDelete)(PLinkComponent* Sender);  // ���������� ��� �������� �����
      void __fastcall (__closure *OnAdd)(PLinkComponent* Sender);  // ���������� ��� ���������� ������
      void __fastcall (__closure *OnChange)(link* Link);  // ���������� ��� ��������� ������
      void Clear();
};

//---------------------------------------------------------------------------

class Pair_Conrtol{
	public:
      PBase* First;
      PBase* Second;
};
#endif
