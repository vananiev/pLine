//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmProp.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProperty *frmProperty;
//---------------------------------------------------------------------------
__fastcall TfrmProperty::TfrmProperty(TComponent* Owner)
   : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmProperty::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   element* model = Obj;
   // сохраняем изменения перед закрытием
   if(model){
      for(int i=0, n=model->Fields.size(); i<n;i++)
         model->Fields[i].Value(   Fields->Values[ model->Fields[i].Name.c_str() ].c_str()   );
      model->accept(); // применяем изменения
      }
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperty::FormShow(TObject *Sender)
{
   element* model = Obj;
   // перебираем поля которые надо вывести
   if(Obj){
      for(int i=0, n=model->Fields.size(); i<n;i++){
         Fields->Values[ model->Fields[i].Name.c_str() ] =  model->Fields[i].Value().c_str();
         Fields->ItemProps[model->Fields[i].Name.c_str()]->ReadOnly = (model->Fields[i].Mode == ro)?true:false;
         Fields->ItemProps[model->Fields[i].Name.c_str()]->EditStyle = (model->Fields[i].Mode == rwf)?esEllipsis:esSimple;
         }
   	// подгоняем размер
   	Fields->Height = (Fields->DefaultRowHeight+2)*Fields->RowCount;
   	if( Fields->Height > Screen->Height-50) Fields->Height = Screen->Height-50;
      }
}

//---------------------------------------------------------------------------

void __fastcall TfrmProperty::FormResize(TObject *Sender)
{
   Fields->Width  = Width - 16;
   Height = Fields->Height + 40;
}
//---------------------------------------------------------------------------

void __fastcall TfrmProperty::FieldsEditButtonClick(TObject *Sender)
{
   // выводим диалог и открываем
   openDlg->Filter = "Файл параметра|*.plv|любой файл|*.*";
   TValueListEditor *obj = dynamic_cast<TValueListEditor*>(Sender);
   if( obj && openDlg->Execute() ){
      Fields->Values[ Obj->Fields[obj->Row-1].Name.c_str() ] = openDlg->FileName;
      }
}
//---------------------------------------------------------------------------

