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
   // �������������� �������
   Grid = NULL;
   LinkLines.Clear();   
   Objects.clear();
	  // ��������� ����������� ������
   LinkLines.Parent = this;
   LinkLines.OnDelete = LinkDelete;
   LinkLines.OnAdd = LinkAdd;
   LinkLines.OnChange = LinkChange;
   // �������������� ���������� ���������� ��� ������ ����
   ScopeAutoRefreshing = true;
   mnuScopeAutoRefresh->Checked = ScopeAutoRefreshing;
   // ���� ������� ����� � ���� �������� �������� - ��������� ����
   if(Name == "frmMain") if( ParamCount()>0 )  OpenFile( ParamStr(1) );

   mnuObjLibClick(this);
}
//---------------------------------------------------------------------------
void err(unsigned Level, std::string Description){
   if(Level==_ERROR_) frmMain->Timer->Enabled = false; 
   ShowMessage(Description.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LinkDelete(PLinkComponent *Sender){
   // ��������� ������� � �������� �����������
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LinkAdd(PLinkComponent *Sender){	// ���������� ��� ��������� ������
   // ��������� ������� � �������� �����������
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
   Accept = false;
   // ���� � �������� ���������� �������
   if(PBase *pc = dynamic_cast<PBase*>(Source)){
   		int from_X = pc->Left + 4;
      	int from_Y = pc->Top  + pc->Height + 4;
      	TColor color = Canvas->Pen->Color;
      	Canvas->Pen->Color = clBlack;
      	Repaint();                       // ������� ��� ��������� �����
      	Canvas->MoveTo(from_X, from_Y);
      	Canvas->LineTo(X, Y);
      	Canvas->Pen->Color = color;
         Accept = true;
      }
   // ���������� ����� ������ �� ���������� �����������
   else   if(Source->ClassNameIs("TTreeView")){
		Accept = true;
   	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormPaint(TObject *Sender)
{
	// ������ ����� PaintLines()
   LinkLines.Paint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormDragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
   // ������� ��� ��������� ����� ��������� ��� ���������� �������
   if(Source->ClassNameIs("PBase"))
      Repaint();
   // ���������� ����� ������ �� ���������� �����������
   else if(Source->ClassNameIs("TTreeView")){
   	TTreeView *obj = (TTreeView*)Source;
   	int n = obj->Selected->AbsoluteIndex;
   	if ( n != -1){
      	TTreeNode *Node = obj->Items->Item[n];
         if(Node != NULL){// && !Node->HasChildren){
            // ������� �������
      		// ��������� ��� � ������ � ��������� ��������� �� ���������
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
void TfrmMain::AddObject(String TypeName, int X, int Y){    // ������ TypeName - ���������� �� frmObjLib->tvList
   String cName = "";
   String ClassName = "PBase";
   if(TypeName == "���������\\�������")         cName = "big_system";
   else if(TypeName == "���������\\�������")    cName = "source";
   else if(TypeName == "�����������\\��������") cName = "consumer";
   else if(TypeName == "�����������\\����������") cName = "rectifier";
   // ��������� ��� � ������ � ��������� ��������� �� ���������
   if(cName!=""){
      // �������� ���������-������
   	CreateControl(ClassName, cName);
      PBase *control = Objects.back();
      control->Left = X - control->Width/2;
      control->Top  = Y - control->Height/2;
   	}
   else{
      err(_WARNING_, "�� ���������� �������� ��� �������� ���������� ���������� �� ���������� ��������");
   	}
}
//---------------------------------------------------------------------------
void TfrmMain::CreateControl(String Class, String ModelClass){
      // �������� ���������-������
      PBase *control;
      TComponent *owner;
      //if(ComponentOwner) owner = ComponentOwner;
      //else
             owner = this;
      control = new PBase( owner );
      AddControl( control );              // �������� ��������� � ������ ������������ � ���������� ���������
      control->ObjType( ModelClass );     // ����������� ��������� � ������� ����
}
//---------------------------------------------------------------------------
// �������� ��������� � ������ ������������ � ���������� ���������
void TfrmMain::AddControl(PBase *control){
   control->Parent = this;
	control->OnChange = UnitChange;
   control->OnPinMouseDown = PinMouseDown;
   control->OnPinDragDrop = PinDragDrop;
   control->OnDelete = UnitDelete;
   Objects.push_back( control );    		// ��������� � ������ �����������
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::PinMouseDown(TObject* Sender, TMouseButton Button, Classes::TShiftState Shift, int X, int Y){
   if(Shift.Contains(ssLeft)&&Shift.Contains(ssShift))   // ��� ����� ����
      // ��������� �����������
      if( Grid && mnuScope->Enabled ){
         element *nd = ((PBase*)((TShape*)Sender)->Owner)->Obj();
      	mnuScopeClick(NULL);
         TfrmScope *f = frmScope[frmScope.size()-1];
         f->showNodeItems( (node*)nd );
         f->cmbPot1->ItemIndex = 0;
         f->rbVoltage->Checked = true;
      }else
         err(_WARNING_,"PowerSystem: ����� ������� ������������ ��������� ������ ����");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::PinDragDrop (System::TObject* Sender, System::TObject* Source, int X, int Y){
	// ��������� ���������� ����������� � �����
   PBase *src = dynamic_cast<PBase*>(Source);
   PBase *snd = dynamic_cast<PBase*>(((TComponent*)Sender)->Owner);
   // ���� ���������� �� ������ ����, �� �� ���������
   if( src == snd ) return;
   if(src &&  snd ){
      Pair_Conrtol Pair;
      Pair.First = src;
      Pair.Second = snd;
      LinkLines.Add( Pair );
      // ��������� �����
      LinkLines.Paint();
      }
}
//---------------------------------------------------------------------------
/* ������� ���������� ��� ��������� �������� */
void __fastcall TfrmMain::UnitChange(TObject *Sender){
   // ��� ��������� ������� �������, ����� ��������� ������ ���� ����������� ����
   // ����� ���������� ������ ������ ������������
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LinkChange(link *Link){
   // ��� ��������� ������� �������, ����� ��������� ������ ���� ����������� ����
   // ����� ���������� ������ ������ ������������
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::UnitDelete(TObject *Sender){
   // ��������� ��� �������� ������������
   //CloseScope();
   // ��������� ������� � �������� �����������
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
   // ������� ��������� �� ������
   vector<PBase*>::iterator iter = Objects.begin();
    while( iter != Objects.end() )
    {
      if( *iter == (PBase*)Sender){
        Objects.erase( iter );
        break;}
      else
        ++iter;
    }
   // ������� ��� ����� � ���� ��������
   PBase* obj = (PBase*)Sender;
   LinkLines.Delete( obj );			// ������ ��� �����, ��������� � ������� */
	// ���������� �����
   LinkLines.Paint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuObjLibClick(TObject *Sender)
{
      // ����� ���������� ��������
      TfrmObjLib *frmOL = new TfrmObjLib(this);
      frmOL->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   // �������� ������ ������
	if(Button == mbRight){
      // ����� �� ��������� �� ����� �����?
      LinkLines.ShowMenuFor( X, Y );
   	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStartClick(TObject *Sender)
{
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
      }
   lblTime->Caption=FloatToStr(TIME);
   double time_ms = (clock()-t0)/CLOCKS_PER_SEC*1e3;
   if(time_ms < 0.8*Timer->Interval) calc_speed = 1.1*calc_speed + 1;
   else if (time_ms > 1.2*Timer->Interval) calc_speed = 0.9*calc_speed - 1;
   if(calc_speed<1)calc_speed=1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuBuildGridClick(TObject *Sender)
{
	Timer->Enabled = false;  // ������������� �������
	if(Objects.size()==0){
   	err(_NOTICE_,"PowerSystem: � ���� ��� �� ������ ������� ��� ������");
   	return;
      }
   // ��������� ��� �������� ������������
   CloseScope();

   TIME = 0;
   lblTime->Caption = 0;
   lblTime->Caption=FloatToStr(TIME);

   static bool cntBuild = 0;   // ����� ������ ����
   //if(cntBuild!=0)
   //   err(_NOTICE_,"PowerSystem: ���������� ����. �������� ����������� � ����� �������� �� ��������������� ��������.");
   // ������� ������ ����
   delete Grid;  Grid = NULL;
	// ������� ����� accept() �� ���� ��������
   // ��� ���������� ������ �� ���������� ���������,
   // �.�. ��� ����� �������� ���������� � ���������� ������
   for(int i=0, n=Objects.size(); i<n; i++)
   	Objects[i]->Obj()->accept();

   if( Objects.size() != 0 && LinkLines.size()!=0 ){
   	Grid = new grid( LinkLines.Links );
      }
   else{
   	err(_ERROR_, "PowerSystem: �� �������� ������� ����, �.�. �� ������� �� ����� ����� ����� ����������");
      return;
      }
   if(  Grid->Make_Equation() ){
      // ��������� ������������ ��� ������
      btnStart->Enabled = true;
      btnStart->SetFocus();
      mnuScope->Enabled = true;
      // ����������� ����� ������ ���� (������ ����������)
      cntBuild++;
      }
   else{
      // ���� ������ ��� ������ ����
      delete Grid; Grid = NULL;
      btnStart->Enabled = false;
      mnuScope->Enabled = false;
      }
}
//---------------------------------------------------------------------------
// �������� ������������
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
	// ���� ������� ��������
   if(Grid && btnStart->Enabled){
      frmScope.push_back(new TfrmScope(this));
      frmScope[frmScope.size()-1]->obj = Grid;
      // ����� ������ get_signal() �� ���� ����� ��� ������ ������������ �� ������� 
      frmScope[frmScope.size()-1]->ScopeAutoRefresh = ScopeAutoRefreshing;
      frmScope[frmScope.size()-1]->Show();
   }else
      err(1,"PowerSystem: ����� ������� ������������ ��������� ������ ����");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuSaveClick(TObject *Sender)
{
	// ������������� �������
   Timer->Enabled = false;
   // ������� ������ � ���������
   saveDlg->Filter = "���� Power Lines|*.plf|����� ����|*.*";
   if( saveDlg->Execute() ){
     	AnsiString Name = saveDlg->FileName;
      ofstream out(Name.c_str());
      if(out){
      	// ����� ������������� �������
         out << num_f << " ";
      	// ��������� �������
         int n = Objects.size();
         out << n << " ";
         for(int i=0; i<n; i++) {
            out << Objects[i]->ClassName() << " ";
            Objects[i]->Save( out );
            }
         //  �������� ���������� � ������
         LinkLines.Save( out );
      	}
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuOpenClick(TObject *Sender)
{
   // ��������� ��� �������� ������������
   CloseScope();
   // ������������� �������
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
   // ������� ������ � ���������
   openDlg->Filter = ".plf|*.plf|����� ����|*.*";
   if( openDlg->Execute() ){
      OpenFile(openDlg->FileName);
    }
}
//---------------------------------------------------------------------------
void TfrmMain::OpenFile(String name){
      ifstream in(name.c_str());
      if(in){
         // ������� ������������ ����
         mnuNewClick(NULL);
         // ������ ����� ��������, ������� ����� �����������
         in >> num_f;
      	// ������ �������
         int n;
         in >> n;
         for(int i=0; i<n; i++)  {
            // ������ ���
            String cName;
            in >> cName;
            // ��������� ���������, ����������� �� ������� ������
            CreateControl( cName, "" );
            // ��������� ��������� ����������
            Objects.back()->Read( in );
            }
         // ��������� ���������� � ������
         LinkLines.Read( in );
      	// �������� ��������� ����
         if( Name.Length() < 32 )
            Caption = name;
         else
            Caption = name.SubString(Name.Length()-32, 32);
         }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuNewClick(TObject *Sender)
{
   CloseScope();
	//�������� �����
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
   TIME = 0;
   lblTime->Caption = 0;
   // ������� ����
   delete Grid;  Grid = NULL;
   // ������� ������������ ����
   for(int i=0, n=Objects.size(); i<n; i++)
      delete Objects[i];
   Objects.clear();
   LinkLines.Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuNumFreqClick(TObject *Sender)
{
   int old_val = num_f;
	num_f = fromStr<int>(InputBox("����� ��������", "����� ��������", num_f).c_str());
   if(old_val == num_f) return;
   CloseScope();
	//�������� �����
   Timer->Enabled = false;
   btnStart->Enabled = false;
   mnuScope->Enabled = false;
   // ������� ����
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
	smp = fromStr<Float>(InputBox("��� �������", "����� ��������", toStr<Float>(smp).c_str()).c_str());
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

