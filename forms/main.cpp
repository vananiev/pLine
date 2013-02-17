//---------------------------------------------------------------------------

#include <vcl.h>
#include <string>
#include <fstream>
#pragma hdrstop

#include "main.h"
#include "ObectsLibrary.h"
#include "time.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
bool ScopeAutoRefreshing;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
   : TForm(Owner), LinkLines(this)
{
   // инициализируем объекта
   Grid = NULL;
   LinkLines.Clear();   
   Objects.clear();
	  // компонент отображения связей
   LinkLines.Parent = this;
   LinkLines.OnDelete = LinkDelete;
   LinkLines.OnAdd = LinkAdd;
   LinkLines.OnChange = LinkChange;
   // Принудительное обновление осилографа при каждом шаге
   ScopeAutoRefreshing = true;
   mnuScopeAutoRefresh->Checked = ScopeAutoRefreshing;
   // если главная форма и если передали параметр - открываем файл
   FileName = "";
   if(Name == "frmMain") if( ParamCount()>0 )  OpenFile( ParamStr(1) );
   NeedBuild = true;
   LineTesting = false;
   LineTestOutputPath = "D:\\";

   mnuObjLibClick(this);
}
//---------------------------------------------------------------------------
void err(unsigned Level, std::string Description){
   if(Level==_ERROR_) frmMain->Timer->Enabled = false; 
   ShowMessage(Description.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LinkDelete(PLinkComponent *Sender){
   // остановим рассчет и запретим продолжение
   Timer->Enabled = false;
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LinkAdd(PLinkComponent *Sender){	// вызывается при изменении связей
   // остановим рассчет и запретим продолжение
   Timer->Enabled = false;
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
   Accept = false;
   // если в процессе соединения выводов
   if(PBase *pc = dynamic_cast<PBase*>(Source)){
   		int from_X = pc->Left + 4;
      	int from_Y = pc->Top  + pc->Height + 4;
      	TColor color = Canvas->Pen->Color;
      	Canvas->Pen->Color = clBlack;
      	Repaint();                       // очищаем все временные линии
      	Canvas->MoveTo(from_X, from_Y);
      	Canvas->LineTo(X, Y);
      	Canvas->Pen->Color = color;
         Accept = true;
      }
   // перетащили новый объект из библиоьеки компонентов
   else   if(Source->ClassNameIs("TTreeView")){
		Accept = true;
   	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormPaint(TObject *Sender)
{
	// рисуем линии PaintLines()
   LinkLines.Paint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
   // очищаем все временные линии созданные при соединении выводов
   if(Source->ClassNameIs("PBase"))
      Repaint();
   // перетащили новый объект из библиоьеки компонентов
   else if(Source->ClassNameIs("TTreeView")){
   	TTreeView *obj = (TTreeView*)Source;
   	int n = obj->Selected->AbsoluteIndex;
   	if ( n != -1){
      	TTreeNode *Node = obj->Items->Item[n];
         if(Node != NULL){// && !Node->HasChildren){
            // создаем элемент
      		// сохраняем его в списке и применяем параметры по умолчанию
            TTreeNode *Prev = obj->Selected;
            String Adress = Prev->Text;
   			while ( Prev ){
     				if ( Prev->HasChildren ) Adress = Prev -> Text + "\\" + Adress;
     				Prev = Prev->Parent;
   				}
            AddObject( Adress, X, Y);
            }
        	}
      }
}
//---------------------------------------------------------------------------
void TfrmMain::AddObject(String TypeName, int X, int Y){    // список TypeName - передается из frmObjLib->tvList
   String cName = "";
   String ClassName = "PBase";
   if(TypeName == "Источники\\Система")         cName = "big_system";
   else if(TypeName == "Источники\\Станция")    cName = "source";
   else if(TypeName == "Потребители\\Линейный") cName = "consumer";
   else if(TypeName == "Потребители\\Нелинейный") cName = "rectifier";
   // сохраняем его в списке и применяем параметры по умолчанию
   if(cName!=""){
      // добавить компонент-модель
   	CreateControl(ClassName, cName);
      PBase *control = Objects.back();
      control->Left = X - control->Width/2;
      control->Top  = Y - control->Height/2;
   	}
   else{
      err(_WARNING_, "не определено действие для создания указанного компонента из Библиотеки объектов");
   	}
}
//---------------------------------------------------------------------------
void TfrmMain::CreateControl(String Class, String ModelClass){
      // добавить компонент-модель
      PBase *control;
      TComponent *owner;
      //if(ComponentOwner) owner = ComponentOwner;
      //else
             owner = this;
      control = new PBase( owner );
      AddControl( control );              // добавить компонент в список отображаемых и установить параметры
      control->ObjType( ModelClass );     // ассоциируем компонент с моделью типа
}
//---------------------------------------------------------------------------
// добавить компонент в список отображаемых и установить параметры
void TfrmMain::AddControl(PBase *control){
   control->Parent = this;
	control->OnChange = UnitChange;
   control->OnPinMouseDown = PinMouseDown;
   control->OnPinDragDrop = PinDragDrop;
   control->OnDelete = UnitDelete;
   Objects.push_back( control );    		// сохраняем в список компонентов
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::PinMouseDown(TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
   if(Shift.Contains(ssLeft)&&Shift.Contains(ssShift))   // был нажат шифр
      // отобразим осциллограф
      if( Grid && mnuScope->Enabled ){
         element *nd = ((PBase*)((TShape*)Sender)->Owner)->Obj();
      	mnuScopeClick(NULL);
         TfrmScope *f = frmScope[frmScope.size()-1];
         f->showNodeItems( (node*)nd );
         f->cmbPot1->ItemIndex = 0;
         f->rbVoltage->Checked = true;
      }else
         err(_WARNING_,"PowerSystem: перед вызовом осциллографа выполните сборку сети");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::PinDragDrop (System::TObject* Sender, System::TObject* Source, int X, int Y){
	// сохраняем информацию графической о связи
   PBase *src = dynamic_cast<PBase*>(Source);
   PBase *snd = dynamic_cast<PBase*>(((TComponent*)Sender)->Owner);
   // если перетащили на самого себя, то не добавляем
   if( src == snd ) return;
   if(src &&  snd ){
      Pair_Conrtol Pair;
      Pair.First = src;
      Pair.Second = snd;
      LinkLines.Add( Pair );
      // отобразим связь
      LinkLines.Paint();
      }
}
//---------------------------------------------------------------------------
/* Событие происходит при изменении элемента */
void __fastcall TfrmMain::UnitChange(TObject *Sender){
   // при изменении свойств объекта, чтобы запустить расчет надо пересобрать сеть
   // после пересборки кнопка старта активируется
   Timer->Enabled = false;
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LinkChange(link *Link){
   // при изменении свойств объекта, чтобы запустить расчет надо пересобрать сеть
   // после пересборки кнопка старта активируется
   Timer->Enabled = false;
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::UnitDelete(TObject *Sender){
   // закрываем все открытые осциллографы
   //CloseScope();
   // остановим рассчет и запретим продолжение
   Timer->Enabled = false;
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
   // удаляем компонент из списка
   vector<PBase*>::iterator iter = Objects.begin();
    while( iter != Objects.end() )
    {
      if( *iter == (PBase*)Sender){
        Objects.erase( iter );
        break;}
      else
        ++iter;
    }
   // удаляем все связи с этим объектом
   PBase* obj = (PBase*)Sender;
   LinkLines.Delete( obj );			// удалим все связи, связанные с выводом */
	// перерисуем связи
   LinkLines.Paint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuObjLibClick(TObject *Sender)
{
      // вызов библиотеки объектов
      TfrmObjLib *frmOL = new TfrmObjLib(this);
      frmOL->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   // щелкнули правую кнопку
	if(Button == mbRight){
      // может мы указываем на линию связи?
      LinkLines.ShowMenuFor( X, Y );
   	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStartClick(TObject *Sender)
{
   // при старте собираем сеть если нужно
   if(!Timer->Enabled && NeedBuild){
      mnuBuildGridClick(Sender);
      if(NeedBuild) return;
      }
   // в режиме тестирования линии открываем файлы
   if(LineTesting){
      for(int phase=0; phase<3; phase++){
         String ph;
         switch(phase){
         case 0: ph="A";break;
         case 1: ph="B";break;
         case 2: ph="C";break;
         }
         LineTestFile[0+phase*5].open( (LineTestOutputPath+"\\U1_"+ph+".plg").c_str(), ios::out | ios::trunc );
         LineTestFile[1+phase*5].open( (LineTestOutputPath+"\\I1_"+ph+".plg").c_str(), ios::out | ios::trunc );
         LineTestFile[2+phase*5].open( (LineTestOutputPath+"\\U2_"+ph+".plg").c_str(), ios::out | ios::trunc );
         LineTestFile[3+phase*5].open( (LineTestOutputPath+"\\I2_"+ph+".plg").c_str(), ios::out | ios::trunc );
         LineTestFile[4+phase*5].open( (LineTestOutputPath+"\\T_"+ph+".plg").c_str(), ios::out | ios::trunc );
         }
      }
   Timer->Enabled = !Timer->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnPauseClick(TObject *Sender)
{
   Timer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnStopClick(TObject *Sender)
{
   Timer->Enabled=false;
   TIME = 0;
   lblTime->Caption=FloatToStr(TIME);
   // в режиме тестирования линии открываем файлы
   if(LineTesting){
      for(int i=0; i<15; i++)
         if(LineTestFile[i].is_open())
            LineTestFile[i].close();
      }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TimerTimer(TObject *Sender)
{
   static calc_speed = 100;
   static clock_t t0;
   t0 = clock();
   for(int i=0;i<calc_speed && Timer->Enabled;i++){
      Grid->calc();
      TIME += smp;
      if(ScopeAutoRefreshing)
         for(int i=0, n=frmScope.size(); i<n; i++)
            if(frmScope[i]->Timer->Enabled)  frmScope[i]->TimerTimer(this);
      if(LineTesting) writeToFile();
      }
   lblTime->Caption=FloatToStr(TIME);
   double time_ms = (clock()-t0)/CLOCKS_PER_SEC*1e3;
   if(time_ms < 0.8*Timer->Interval) calc_speed = 1.1*calc_speed + 1;
   else if (time_ms > 1.2*Timer->Interval) calc_speed = 0.9*calc_speed - 1;
   if(calc_speed<1)calc_speed=1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::writeToFile(){
   if(LineTesting){
         node *nd0 = (node*)Objects[0]->Obj();
         node *nd1 = (node*)Objects[1]->Obj();
         link *ln = LinkLines.Links[0];
         for(int phase=0; phase<3; phase++){
            LineTestFile[0+phase*5] << TIME << "\t" << get_complex_voltage( nd0, phase ) << endl;
            LineTestFile[1+phase*5] << TIME << "\t" << get_complex_current( ln,nd0,phase ) << endl;
            LineTestFile[2+phase*5] << TIME << "\t" << get_complex_voltage( nd1,phase ) << endl;
            LineTestFile[3+phase*5] << TIME << "\t" << get_complex_current( ln,nd1,phase ) << endl;
            LineTestFile[4+phase*5] << TIME << "\t" << get_temperature( ln,phase ) << endl;
            }
         }
}
//---------------------------------------------------------------------------
Complex TfrmMain::get_complex_current(link* Link, node* Node, int ph){
      Dynamic_Array< phase<Complex*> > *I;
      if(Link->From()==Node) I = &Link->I1;
      else          I = &Link->I2;
      Complex ret = Complex(0,0);
      for(int i_freq=0; i_freq < num_f; i_freq++){
         ret += *(*I)[i_freq][ph];
         }
      return ret;
}
//---------------------------------------------------------------------------
Complex TfrmMain::get_complex_voltage(node* Node, int ph){
      Dynamic_Array< phase<Complex*> > *U = &Node->U;
      Complex ret = Complex(0,0);
      for(int i_freq=0; i_freq < num_f; i_freq++){
         ret += *(*U)[i_freq][ph];
         }
      return ret;
}
//---------------------------------------------------------------------------
Float TfrmMain::get_temperature(link* Link, int ph){
      return Link->T_wire[ph];
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuBuildGridClick(TObject *Sender)
{
   if(!NeedBuild) return;
	Timer->Enabled = false;  // останавливаем рассчет
	if(Objects.size()==0){
   	err(_NOTICE_,"PowerSystem: в сети нет ни одного объекта для сборки");
   	return;
      }
   // закрываем все открытые осциллографы
   CloseScope();

   TIME = 0;
   lblTime->Caption = 0;
   lblTime->Caption=FloatToStr(TIME);

   static bool cntBuild = 0;   // номер сборки сети
   //if(cntBuild!=0)
   //   err(_NOTICE_,"PowerSystem: пересборка сети. Значения потенциалов и токов остаются из предшествующего рассчета.");
   // удаляем старую сеть
   delete Grid;  Grid = NULL;
	// вызовем метод accept() ля всех объектов
   // это необходимо делать до связывания элементов,
   // т.к. она может изменить информацию о внутренних связях
   for(int i=0, n=Objects.size(); i<n; i++)
   	Objects[i]->Obj()->accept();

   if( Objects.size() != 0 && LinkLines.size()!=0 ){
   	Grid = new grid( LinkLines.Links );
      }
   else{
   	err(_ERROR_, "PowerSystem: не возможно собрать сеть, т.к. не указано ни одной связи между элементами");
      return;
      }
   if(  Grid->Make_Equation() ){
      // уравнения подготовлены без ошибок
      NeedBuild = false; //btnStart->Enabled = true;
      btnStart->SetFocus();
      mnuScope->Enabled = true;
      // увеличиваем номер сборки сети (просто информация)
      cntBuild++;
      // схема изменена
      if(Caption[Caption.Length()] != '*')
         Caption = Caption + " *";
      }
   else{
      // были ошибки при сборке сети
      delete Grid; Grid = NULL;
      NeedBuild = true; //btnStart->Enabled = false;
      mnuScope->Enabled = false;
      }
}
//---------------------------------------------------------------------------
// Закрытие осциллогрфов
void TfrmMain::CloseScope(){
   for(int i=0, n=frmScope.size(); i<n; i++){
      frmScope[i]->Timer->Enabled = false;
      delete frmScope[i]; 	frmScope[i] = NULL;
   	}
   frmScope.clear();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuScopeClick(TObject *Sender)
{
	// если рассчет разрешен
   if(Grid && !NeedBuild){ //btnStart->Enabled){
      frmScope.push_back(new TfrmScope(this));
      frmScope[frmScope.size()-1]->obj = Grid;
      // метод вызова get_signal() из этой формы или внутри осциллографа по таймеру 
      frmScope[frmScope.size()-1]->ScopeAutoRefresh = ScopeAutoRefreshing;
      frmScope[frmScope.size()-1]->Show();
   }else
      err(1,"PowerSystem: перед вызовом осциллографа выполните сборку сети");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuSaveClick(TObject *Sender)
{
	// останавливаем рассчет
   if(FileName == ""){
      // выводим диалог и сохраняем
      mnuSaveAsClick(Sender);
      return;
      }
  else{
      ofstream out(FileName.c_str());
      if(out){
      	// число расчитываемых гармник
         out << num_f << " ";
      	// сохраняем объекты
         int n = Objects.size();
         out << n << " ";
         for(int i=0; i<n; i++) {
            out << Objects[i]->ClassName() << " ";
            Objects[i]->Save( out );
            }
         //  сохраним информацию о связях
         LinkLines.Save( out );
      	}
      }
    // Убираем знак несохраненности
    Caption = FileName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuOpenClick(TObject *Sender)
{
   // закрываем все открытые осциллографы
   CloseScope();
   // останавливаем рассчет
   Timer->Enabled = false;
   // выводим диалог и открываем
   openDlg->Filter = ".plf|*.plf|любой файл|*.*";
   if( openDlg->Execute() ){
      OpenFile(openDlg->FileName);
    }
}
//---------------------------------------------------------------------------
bool TfrmMain::OpenFile(String name){
      ifstream in(name.c_str());
      if(in){
         // очищаем существующую сеть
         mnuNewClick(NULL);
         // читаем число гармоник, которые нужно расчитывать
         in >> num_f;
      	// читаем объекты
         int n;
         in >> n;
         for(int i=0; i<n; i++)  {
            // читаем тип
            String cName;
            in >> cName;
            // добавляем компонент, указывающий на нулевую модель
            CreateControl( cName, "" );
            // загружаем параметры компонента
            Objects.back()->Read( in );
            }
         // загружаем информацию о связях
         LinkLines.Read( in );
      	// Изменяем заголовок окна
         if( Name.Length() < 32 )
            Caption = name;
         else
            Caption = name.SubString(Name.Length()-32, 32);
         FileName = name;
         NeedBuild = true; //btnStart->Enabled = false;
         mnuScope->Enabled = false;
         return true;
         }
      else{
         ShowMessage((String)"Файл " + name + "не найден");
         return false;
         }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuNewClick(TObject *Sender)
{
   CloseScope();
	//обнуляем время
   Timer->Enabled = false;
   // в режиме тестирования линии открываем файлы
   if(LineTesting){
      for(int i=0; i<15; i++)
         if(LineTestFile[i].is_open())
            LineTestFile[i].close();
      }
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
   TIME = 0;
   lblTime->Caption = 0;
   // удаляем сеть
   delete Grid;  Grid = NULL;
   // очищаем существующую сеть
   for(int i=0, n=Objects.size(); i<n; i++)
      delete Objects[i];
   Objects.clear();
   LinkLines.Clear();
   FileName = "";
   Caption = "New Model";
   LineTesting = false; // не в режиме тестирования линии
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuNumFreqClick(TObject *Sender)
{
   int old_val = num_f;
	num_f = fromStr<int>(InputBox("Число гармоник", "Новое значение", num_f).c_str());
   if(old_val == num_f) return;
   CloseScope();
	//обнуляем время
   Timer->Enabled = false;
   NeedBuild = true; //btnStart->Enabled = false;
   mnuScope->Enabled = false;
   // удаляем сеть
   delete Grid; Grid = NULL;
   for(int i=0, n=Objects.size(); i<n; i++)
   	Objects[i]->Obj()->accept();
   for(int i=0, n=LinkLines.Links.size(); i<n; i++)
   	LinkLines.Links[i]->accept();

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuScopeAutoRefreshClick(TObject *Sender)
{
   mnuScopeAutoRefresh->Checked = !mnuScopeAutoRefresh->Checked;
   ScopeAutoRefreshing = mnuScopeAutoRefresh->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuSmpClick(TObject *Sender)
{
	smp = fromStr<Float>(InputBox("Шаг времени", "Новое значение", toStr<Float>(smp).c_str()).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuStartStopClick(TObject *Sender)
{
	btnStartClick(this);	
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuClearClick(TObject *Sender)
{
	btnStopClick(this);	
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuSaveAsClick(TObject *Sender)
{
	// останавливаем рассчет
   Timer->Enabled = false;
   // выводим диалог и сохраняем
   saveDlg->Filter = "Файл Power Lines|*.plf|любой файл|*.*";
   if( saveDlg->Execute() ){
      FileName = saveDlg->FileName;
      mnuSaveClick(Sender);
      }
}
//---------------------------------------------------------------------------
// Быстро смоделировать линию
void __fastcall TfrmMain::mnuLineClick(TObject *Sender)
{
   // Открываем модель тестирования линии
   String d = ParamStr(0);
   const char *exeName = ParamStr(0).c_str();
   int e, i;
   for(e=ParamStr(0).Length(); exeName[e]!='\\' && e>=0; e--);
   char pathName[1024];
   for(i=0; i<e; i++) pathName[i] = exeName[i];
   pathName[i] = 0;
   if( !OpenFile( (String)pathName + "\\LineTest.plf" ) ) return;

   // Проверяем все ли компоненты на месте
   if( Objects.size()>2 ) {ShowMessage("Не должно быть более двух объектов"); return;}
   vector<PBase*>::iterator iter = Objects.begin();
   bool bSystem = false, bConsumer = false;
   while(iter != Objects.end()){
      if( dynamic_cast<big_system*>( (*iter)->Obj() )  )
         bSystem = true;
      else if( dynamic_cast<consumer*>( (*iter)->Obj() ) )
         bConsumer = true;
      iter++;
   }
   if( ! (bSystem && bConsumer) ) {ShowMessage("Не найдена система и потребитель"); return;}
   if( LinkLines.Links.size()>1 ) {ShowMessage("Не должно быть более одной линии"); return;}
   
   LineTestOutputPath = InputBox("Режим тестирования линии", "Папка сохранения результатов", LineTestOutputPath);

   // Мы в режиме тестирования линии
   LineTesting = true;
   smp = 1;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormResize(TObject *Sender)
{
   btnStop->Left = ClientWidth - 25;
   btnPause->Left = ClientWidth - 49;
   btnStart->Left = ClientWidth - 73;
   lblSec->Left = ClientWidth - 97;
   lblTime->Left = ClientWidth - 137;
}
//---------------------------------------------------------------------------

