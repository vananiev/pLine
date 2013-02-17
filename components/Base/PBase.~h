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
class PACKAGE PBase : public TGraphicControl  // TGraphicControl позволяет сделать компонент прозрачным
{
private:
   typedef TGraphicControl inherited;
	DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
   int MouseDownPos_X, MouseDownPos_Y; // где была нажата кнопка мыши
   bool _Drag; // флаг нереноса
   void __fastcall PinMouseDown(TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
   void __fastcall PinDragDrop (System::TObject* Sender, System::TObject* Source, int X, int Y);
   void __fastcall PinDragOver(System::TObject* Sender, System::TObject* Source, int X, int Y, TDragState State, bool &Accept);
   int __fastcall XtoParent(int x);
   int __fastcall YtoParent(int y);
protected:
   TPopupMenu *Menu; // контектное меню
   virtual void __fastcall MenuItemClick(TObject *Sender);         // вызывается при клике на элемент меню
   element *_Obj;
   //virtual void __fastcall SetParent(TWinControl* AParent);
   TShape* Shape;            // объект-круг
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
   virtual void __fastcall Paint(void);   // вызывается родителем при просисовке компонента
   __property DragKind  = {default=0};
	__property DragMode  = {default=0};
   __property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
   __property OnEndDrag = {default=0};
   __property OnStartDrag = {default=0};
   TNotifyEvent OnChange;
   TMouseEvent OnPinMouseDown;            // событие возникает при клике по выводу
   TDragDropEvent OnPinDragDrop;          // возникает в конце перетаскивания на объект
   TDragOverEvent OnPinDragOver;          // возникает в конце перетаскивания на объект
   void Obj(element *obj);               // ассоцииируем силовой объект с компонентом
   element * Obj();           		      // ассоцииируем силовой объект с компонентом
   element* ObjType(String ClassName);   // создаем объект и ассоциируем его с компонентом
   TNotifyEvent OnDelete;						// Вызывается при удалении
   virtual void Save(ostream &stream);		// сохранить контрол и объект (saveModel=false сохраняется только компонент без модели )
   virtual void Read(istream &stream);		// загрузить контрол и объект
__published:
};




//---------------------------------------------------------------------------
class Pair_Conrtol;
class PLinkComponent{
      vector<Pair_Conrtol> Controls;
      void __fastcall MenuItemClick(TObject *Sender);
      TPopupMenu* Menu;							// меню данной связи
      int menuFor;								// для какого элемента обображено меню
      //void Change();								// вызывается при изменении компонентов связи
   public:
      vector<link*> Links;
      void Add   (Pair_Conrtol pc);  // добавить новую пару
      void Delete(PBase* pc);			// удалить связи, где присутствует указанный вывод
 		//void Delete2(node* nd);					// удалить связи, где присутствует указанный вывод
      link* operator[](int i);
      int size();
      int FindAtPos(int X, int Y);	      // поиск линии по координатам
      PLinkComponent(TComponent *owner);
      //PLinkComponent operator=(const PLinkComponent &Obj);// присваивание
      ~PLinkComponent();
      void ShowMenuFor(int LinkNumber);	// показать меню для заданной связи
      void ShowMenuFor(int X, int Y);		// показать меню для связи с заданными координатами
      void __fastcall Paint();            // прорисовывает связи
      TForm* Parent;							   // компонент на котором рисуем
      TComponent* Owner;							   // компонент-владелец
      void Save(ostream &stream);			// сохранить контрол и связи
      void Read(istream &stream);			// загрузить контрол и связи
      void __fastcall (__closure *OnDelete)(PLinkComponent* Sender);  // Вызывается при удалении связи
      void __fastcall (__closure *OnAdd)(PLinkComponent* Sender);  // вызывается при добавлении связей
      void __fastcall (__closure *OnChange)(link* Link);  // вызывается при изменении связей
      void Clear();
};

//---------------------------------------------------------------------------

class Pair_Conrtol{
	public:
      PBase* First;
      PBase* Second;
};
#endif
