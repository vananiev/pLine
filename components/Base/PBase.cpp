//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include <vector>
#include "PBase.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// настройки компонента
#include "frmProp.h"
#include "line_parameters.h"
typedef  TfrmProperty  HelpForm;       // класс формы которая будет вызвана в качестве формы свойств
typedef  TfrmLine_parameters  LineParametersForm;       // класс формы которая будет вызвана в качестве формы параметров линии
extern vector<LineParametersForm*> LineParametersForms;
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(PBase *)
{
   new PBase(NULL);
}
//---------------------------------------------------------------------------
__fastcall PBase::PBase(TComponent* Owner)
   : TGraphicControl(Owner)
{
	//ControlAtPos 	Определить элемент управления, содержащийся в указанной позиции. 	TWinControl
   Parent = dynamic_cast<TWinControl*>(Owner);
   Width=70;
   Height=12;
   // параметры холста
   Canvas->Brush->Color = clBtnFace;   // Brush - заливка
   Canvas->Pen->Color = clGray;      // Pen - рисование линий
   savedColor = Canvas->Pen->Color;
   Canvas->Font->Size = 8;
   Canvas->Font->Color = clGray;
   Canvas->Font->Name="Times New Roman";
   // задаем уникальное имя компоненту
   int PBaseNameId = 0;
   String name = (String)ClassName() + IntToStr( ++PBaseNameId );
   while(  Owner->FindComponent( name )  )
      name = (String)ClassName() + IntToStr( ++PBaseNameId );
   Name = name;
   // рисуем круг
   Shape = new TShape(this);
   // форма
   Shape->Width = 8;
   Shape->Height = 8;
   Shape->Parent = dynamic_cast<TWinControl*>(Owner);
   Shape->Shape = stEllipse;
   // события круга
   Shape->OnMouseDown = PinMouseDown;
   Shape->OnDragOver = PinDragOver;
   Shape->OnDragDrop = PinDragDrop;
   Shape->Pen->Color = clGray;
   // прорисовка компонента
   //Paint();
   // способ перетаскивания
   DragKind = dkDrag;
   DragMode = dmManual;
   // в момент перетаскивания устанавливается в true
   _Drag = false;
   // контексттное меню
   Menu = new TPopupMenu(this);
   // удалить
   TMenuItem *miDel = new TMenuItem(Menu);
   miDel->Name = "Delete";
   miDel->Caption = "Удалить";
   miDel->OnClick = MenuItemClick;
   Menu->Items->Add(miDel);
   // свойства
   TMenuItem *miProp = new TMenuItem(Menu);
   miProp->Name = "Properties";
   miProp->Caption = "Свойства";
   miProp->OnClick = MenuItemClick;
   Menu->Items->Add(miProp);
   // силовой объект ассоциированный с компонентом
   _Obj = NULL;
   // событие об изменении элемеента
   OnChange = NULL;
   // событие возникает при клике по выводу
   OnPinMouseDown = NULL;
   // возникает в конце перетаскивания на объект
   OnPinDragDrop = NULL;
   OnPinDragOver = NULL;
   // Вызывается при удалении
   OnDelete = NULL;
    // прозрачная кисть - замкнутые участки делаются прозрачными
   //Canvas->Brush->Style = bsClear;
}
//---------------------------------------------------------------------------
__fastcall PBase::~PBase(){
   // удалим саму модель
   delete _Obj;  _Obj = NULL;
}
//---------------------------------------------------------------------------
void __fastcall PBase::Paint(void){
	  if(Parent == NULL) return;
   // имя
   String name;
   if(_Obj)
      name = _Obj->Name.c_str();
   else
      name = Name;
   Width = 7*name.Length();
   Canvas->TextOutA(0, 0, name);
   Shape->Left = Left;
   Shape->Top =  Top + Height;
}
//---------------------------------------------------------------------------
int __fastcall PBase::XtoParent(int x){return x+Left;}
int __fastcall PBase::YtoParent(int y){return y+Top;}
//---------------------------------------------------------------------------
void __fastcall PBase::MouseMove(Classes::TShiftState Shift, int X, int Y){
   // осуществляем перетаскивание
   if(_Drag){
      Left += X-MouseDownPos_X;
      Top += Y-MouseDownPos_Y;
      }
   if(OnMouseMove) OnMouseMove(this, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall PBase::MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
   if(Button==mbLeft){
      // перетаскиваем элемент
      _Drag = true;
      MouseDownPos_X = X;
      MouseDownPos_Y = Y;
      }
   else if (Button==mbRight){
      // определим координаты относительно окна
      TPoint p, sp;
      p.x = X; p.y = Y;
      sp = ClientToScreen(p);
      /*/ если это сеть
      if(dynamic_cast<PSubGrid*>(_Obj))
         Menu->Items->Find("Редактировать сеть")->Visible = true;
      else
         Menu->Items->Find("Редактировать сеть")->Visible = false;*/
      // выводим меню
      Menu->Popup(sp.x,sp.y);
      }
   // вызываем дополнительный внешний обработчик
   if(OnMouseDown)  OnMouseDown(this, Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall PBase::MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
   _Drag = false;
   if(OnMouseUp) OnMouseUp(this, Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall PBase::MenuItemClick(TObject *Sender){
   String itemName =  ((TMenuItem*)Sender)->Name;
   if(itemName == "Properties"){
      // меню свойства
      HelpForm *frmP = new HelpForm(this);
      frmP->Obj = _Obj;
      frmP->ShowModal();
      delete frmP;
      Repaint();  // если произошли изменения внешнего вида, то отобразим
      // событие-уведомление об изменении элемента
      if(OnChange) OnChange(this);
      }
   else if(itemName == "Delete"){
      // удаление
      if(OnDelete) OnDelete(this);
      // удалим объект и модель
      delete this;
      }
}
//---------------------------------------------------------------------------
void PBase::Obj(element *obj){
   if(_Obj!=NULL){
      err(0, "PBase: компонент уже асоциирован с моделю " + _Obj->ClassName() + ". Новая ассоциация не выполнена.");
      return;
      }
   if(obj) _Obj = obj;
}
//---------------------------------------------------------------------------
element * PBase::Obj(){
	return _Obj;
}
//---------------------------------------------------------------------------
 // создаем объект и ассоциируем его с компонентом
element* PBase::ObjType(String ClassName){
   Visible = false;
   vector<void*> Params;
   Obj( CreateObj(ClassName, Params) );
   Visible = true;
   return _Obj;
}
//---------------------------------------------------------------------------
void __fastcall PBase::PinMouseDown(TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
   BeginDrag(true, -1); // начинаем перетаскивание объекта
   // вызываем внешний обработчик события
   if(OnPinMouseDown)OnPinMouseDown(Sender, Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall PBase::PinDragDrop (System::TObject* Sender, System::TObject* Source, int X, int Y){
   // вызываем внешний обработчик события
   if(OnPinDragDrop) OnPinDragDrop(Sender, Source, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall PBase::PinDragOver(System::TObject* Sender, System::TObject* Source, int X, int Y, TDragState State, bool &Accept){
   if(Sender != Source)  Accept = true; // разрешаем принять объект
   // вызываем внешний обработчик события
   if(OnPinDragOver) OnPinDragOver(Sender, Source, X, Y, State, Accept);
   if(State==dsDragEnter){
   	Shape->Pen->Color = clRed;
      Shape->Height++;
   	Shape->Width++;
   }else if(State==dsDragLeave){
   	Shape->Pen->Color = savedColor;
   	Shape->Height--;
   	Shape->Width--;
      }
}
//---------------------------------------------------------------------------
void __fastcall PBase::MouseEnter(TMessage& Msg){
   Shape->Height++;
   Shape->Width++;
   // делаем вывод больше по размеру и изменяем положение
   Shape->Pen->Color = clRed;
   Canvas->Font->Style = Canvas->Font->Style << fsBold;
}
//---------------------------------------------------------------------------
void __fastcall PBase::MouseLeave(TMessage& Msg){
   Shape->Height--;
   Shape->Width--;
   Shape->Pen->Color = savedColor;          // цвет карандаша
   Canvas->Font->Style = Canvas->Font->Style >> fsBold;
}
//---------------------------------------------------------------------------
void PBase::Save(ostream &stream){		// сохранение параметров объекта в поток
   if(Name==""){
      err(0, "PBase: компонент имеет нулевое имя. Возможно файл будет неоткрываем.");
   	}
   stream << Name << " ";
   stream << Left << " ";
   stream << Top << " ";
   stream << Width << " ";
   stream << Height << " ";
   stream << true << " ";
   // сохранить сам объект
   SaveObj( _Obj, stream );
}
//---------------------------------------------------------------------------
void PBase::Read(istream &stream){    // загрузка параметров объекта из потока
	String name;
   stream >> name; Name = name;
   int left, top, width, height;
   stream >> left; 	Left = left;
   stream >> top;    Top = top;
   stream >> width;  Width = width;
   stream >> height; Height = height;
   // была ли сохранена модель
   bool saveModel = true;
   stream >> saveModel;
   // создадим объект
   if(saveModel) Obj( CreateObj(stream) );
}





//---------------------------------------------------------------------------
PLinkComponent::PLinkComponent(TComponent *owner){
   Owner = owner;
   Menu = new TPopupMenu(NULL);
	// удалить
   TMenuItem *miDel = new TMenuItem(Menu);
   miDel->Name = "Delete";
   miDel->Caption = "Удалить связь";
   miDel->OnClick = MenuItemClick;
   Menu->Items->Add(miDel);
   // параметры линии
   TMenuItem *miPar = new TMenuItem(Menu);
   miPar->Name = "Line_Parameters";
   miPar->Caption = "Параметры";
   miPar->OnClick = MenuItemClick;
   Menu->Items->Add(miPar);
   // свойства
   TMenuItem *miProp = new TMenuItem(Menu);
   miProp->Name = "Properties";
   miProp->Caption = "Свойства";
   miProp->OnClick = MenuItemClick;
   Menu->Items->Add(miProp);
   // установим при отображении меню
   menuFor = -1;
   // на ком рисуем
   Parent =  (TForm*)owner;
}
//---------------------------------------------------------------------------
void PLinkComponent::Delete(PBase* pc){			// удалить связи, где присутствует указанный объект
   if(Controls.size()==0)return;
   vector<Pair_Conrtol>::iterator iter = Controls.begin();
    while( iter != Controls.end() )
    {
      if(iter->First == pc  ||  iter->Second == pc)
        iter = Controls.erase( iter );
      else
        ++iter;
    }
    element* nd = ((PBase*)pc)->Obj();
    vector<link*>::iterator it = Links.begin();
    while( it != Links.end() )
    {
      if((*it)->From() == nd  ||  (*it)->To() == nd)
        it = Links.erase( it );
      else
        ++it;
    }
}
//---------------------------------------------------------------------------
PLinkComponent::~PLinkComponent(){
   /*
   vector<link*>::iterator iter = Links.begin();
    while( iter != Links.end() )
    {
      delete *iter;
      ++iter;
    } */
   Clear();
}
//---------------------------------------------------------------------------
void __fastcall PLinkComponent::MenuItemClick(TObject *Sender){
   // menuFor - содержит номер связи для которой происходил вывод меню
   if( menuFor<0 || menuFor>=(int)Links.size() ) return;
   String itemName =  ((TMenuItem*)Sender)->Name;
	if(itemName == "Delete"){
      // удаление
      Controls.erase(Controls.begin() + menuFor);
      delete Links[menuFor]; Links[menuFor] = NULL;
      Links.erase(Links.begin() + menuFor);
      if(OnDelete) OnDelete(this);
      Paint();
      }
    else if(itemName == "Properties"){
      // меню свойства
      HelpForm *frmP = new HelpForm(NULL);
      frmP->Obj = Links[menuFor];
      frmP->ShowModal();
      delete frmP; //frmP = NULL;
      // событие-уведомление об изменении элемента
      if(OnChange) OnChange(Links[menuFor]);
      }
   else if(itemName == "Line_Parameters"){
      // меню свойства
      LineParametersForm *frmP = new LineParametersForm(NULL);
      frmP->Line = Links[menuFor];
      frmP->Show();
      LineParametersForms.push_back(frmP);
      }
}
//---------------------------------------------------------------------------
void PLinkComponent::ShowMenuFor(int LinkNumber){	// показать меню для заданной связи
   if( LinkNumber<0 || LinkNumber>=(int)Links.size() ){
      err(3, "PLinkComponent: отображение меню. Компонента с таким номером не существует.");
      return;
   	}
	// определим координаты относительно окна
   TPoint sp;
   GetCursorPos(&sp);
   // выводим меню
   menuFor = LinkNumber;
   Menu->Popup(sp.x,sp.y);
}
//---------------------------------------------------------------------------
void PLinkComponent::ShowMenuFor(int X, int Y){	// показать меню для связи c заданными координатами
	int n = FindAtPos(X,Y);
   if( n != -1 )
   	ShowMenuFor( n );
}
//---------------------------------------------------------------------------
// линию проходящую через заданную точку
int PLinkComponent::FindAtPos(int X, int Y){
	for(int i=0, n = Controls.size(); i<n;i++){
   	int X1 = Controls[i].First->Left +  4;
      int Y1 = Controls[i].First->Top + Controls[i].First->Height + 4;
      int X2 =  Controls[i].Second->Left +  4;
      int Y2 =  Controls[i].Second->Top + Controls[i].Second->Height + 4;
      // находимся внутри отрезка ?
      if(  ((X>(X1-4) && X<(X2+4))||(X>(X2-4) && X<(X1+4)))  &&
           ((Y>(Y1-4) && Y<(Y2+4))||(Y>(Y2-4) && Y<(Y1+4)))     ){
         // tсли вертикальная или горизонтальная линия, то достаточно условия вхождения в прямоуг
         if( X1==X2 || Y1==Y2 )
            return i;
      	// удовлетворяем ли уравнению линии
         float k = 1.0*(Y1-Y2)/(X1-X2);
         float c = Y1 - k*X1;
         if(fabs(Y- k*X-c)<=6) // промахнулись на 4 пиксел
            return  i;	// возвращаем связь
         }
   	}
   return -1;
}
//---------------------------------------------------------------------------
void PLinkComponent::Add   (Pair_Conrtol pc){  // добавить новую пару
   Controls.push_back(pc);
   link *w = new link((node*)pc.First->Obj(), (node*)pc.Second->Obj());
   Links.push_back(w);
   w->Name = "w" + toStr<int>(Links.size());
   // сообщаем об изменении
   if(OnAdd) OnAdd(this);
}
//---------------------------------------------------------------------------
link* PLinkComponent::operator[](int i){
	return Links.at(i);
}
//---------------------------------------------------------------------------
int PLinkComponent::size(){
	return Links.size();
}
//---------------------------------------------------------------------------
void __fastcall PLinkComponent::Paint(){
   if(Parent==NULL) return;
	static bool Painting = false;
   if(Painting) return;
   Painting = true;
   Parent->Repaint();                      // очищаем все временные линии
   // рисуем линии связи
   TColor color = Parent->Canvas->Pen->Color;
   TBrushStyle bStyle= Parent->Canvas->Brush->Style;
   Parent->Canvas->Pen->Color = clGray;
   Parent->Canvas->Brush->Style = bsClear;
   for(int i=0, n=Links.size(); i<n; i++){
   	int from_X = Controls[i].First->Left + 4;
      int from_Y = Controls[i].First->Top + Controls[i].First->Height + 4;
      int to_X =  Controls[i].Second->Left + 4;
      int to_Y =  Controls[i].Second->Top + Controls[i].Second->Height + 4;
      Parent->Canvas->MoveTo(from_X, from_Y);
      Parent->Canvas->LineTo(to_X, to_Y);
      Parent->Canvas->TextOutA((from_X+to_X)/2, (from_Y+to_Y)/2, Links[i]->Name.c_str());
      }
   Parent->Canvas->Pen->Color = color;
   Parent->Canvas->Brush->Style = bStyle;
   // закончили рисовать
   Painting = false;
}
//----------------------------------------------------------------------------
void PLinkComponent::Save(ostream &stream){			// сохранить контрол
   int n = Links.size();
   stream << n << " ";
   for(int i=0; i<n; i++){
      stream << Controls[i].First ->Name << " " << Controls[i].Second->Name << " ";
      Links[i]->Save(stream);
      }
}
//---------------------------------------------------------------------------
void PLinkComponent::Read(istream &stream){			// загрузить контрол
   int n;   // число связей
   stream >> n;
   for(int i=0; i<n; i++){
      // это пара объединяемых компонент
      Pair_Conrtol pc;
      pc.First=NULL; pc.Second=NULL;
      // прочитаем имя компонен
      String X, Y;
      stream >> X >> Y;
      // найдем эти компоненты (родителем компонентов д.б. PBase) и добавим их в список связанных
      for(int i = 0;i < Owner->ComponentCount; i++){
         PBase *pb = dynamic_cast<PBase*>( Owner->Components[i] );
         if( pb ){
               if     ( pb->Name == X ) pc.First = pb;
               else if( pb->Name == Y ) pc.Second= pb;
            }
         }
      if( !(pc.First && pc.Second ) )
         err(_ERROR_, "PLinkComponent: не найден объект-узел форме");
      else{
         Add( pc );
         Links[ Controls.size()-1 ]->Read(stream);
         }
      }
}
//---------------------------------------------------------------------------
void PLinkComponent::Clear(){
   // закрываем формы
   for(int i=0, n=LineParametersForms.size(); i<n; i++){
      LineParametersForms[i]->Timer->Enabled = false;
      delete LineParametersForms[i]; 	LineParametersForms[i] = NULL;
   	}
   LineParametersForms.clear();
   // очищаем объекты
   vector<link*>::iterator iter = Links.begin();
    while( iter != Links.end() )
    {
      delete *iter;
      ++iter;
    }
   Links.clear();
   Controls.clear();
}

