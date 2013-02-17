//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "line_parameters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLine_parameters *frmLine_parameters;
vector<TfrmLine_parameters*> LineParametersForms;
//---------------------------------------------------------------------------
__fastcall TfrmLine_parameters::TfrmLine_parameters(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
String RoundTo(const Float &value, int digits) {
   int exp=0;
   Float t = value;
   if(fabs(t)<1e-100) return 0;
   Float abs_value = fabs(value);
   if(abs_value<1000 && abs_value>0.01){
   	Float tmp;
   	if( abs_value>=100) { tmp = (int)(0.5+value); }
   	else if(abs_value>=10){ tmp = 0.1*(int)(0.5+10*value); }
      else if(abs_value>=1){ tmp = 0.01*(int)(0.5+100*value); }
      else if(abs_value>=0.1){ tmp = 0.001*(int)(0.5+1000*value); }
      else if(abs_value>=0.01){ tmp = 0.001*(int)(0.5+1000*value); }
      return FloatToStr(tmp);
      }
   while(fabs(t) < 1){
	   t *= 10;
	   exp--;
      }
   while(fabs(t) > 10){
	   t = t/10;
	   exp++;
      }
   Float temp = pow10(digits-1);
   if(exp != 0)
      return FloatToStr((floor(t*temp+0.5))/temp) + "e" + IntToStr(exp);
   else
      return FloatToStr((floor(t*temp+0.5))/temp);
}
//---------------------------------------------------------------------------

int TfrmLine_parameters::printMatrix(Matrix<Complex>* A, char* Name, int x, int y){
   int y_step = 10 + Canvas->Font->Size;
   int x_step = 104;
   int rows = Line->num_wire;
   int cols = Line->num_wire;
   // Имя массива
   Canvas->TextOutA(x, y, Name);
   int c=0; for(;Name[c] && c<100; c++);
   x += c*8 + 10;
   // Скобки
   Canvas->MoveTo(x-4, y-4);
   Canvas->LineTo(x-8, y-4);
   Canvas->LineTo(x-8, y + 4 + 2*y_step + Canvas->Font->Size);
   Canvas->LineTo(x-4, y + 4 + 2*y_step + Canvas->Font->Size);
   Canvas->MoveTo(x + cols*x_step - 4, y-4);
   Canvas->LineTo(x + cols*x_step - 0, y-4);
   Canvas->LineTo(x + cols*x_step - 0, y + 4 + (rows-1)*y_step + Canvas->Font->Size);
   Canvas->LineTo(x + cols*x_step - 4, y + 4 +(rows-1)*y_step + Canvas->Font->Size);
   // Значения
	for(int i=0; i<cols; i++){
   	for(int j=0; j<rows; j++){
      		Float im = imag((*A)[i][j]);
      		String R = RoundTo(real((*A)[i][j]),3);
            String X = RoundTo(fabs(im),3);
            String Z = R + (im>0?" + j":" - j") + X;
         	Canvas->TextOutA(x + j*x_step, y + i*y_step, Z.c_str());
      	}
   	}
   // установка ширины
   if(ClientWidth < x + cols*x_step + 10) ClientWidth = x + cols*x_step + 10;
   return y + 4 +(rows-1)*y_step + Canvas->Font->Size;
}
//---------------------------------------------------------------------------
int TfrmLine_parameters::printParametr(Matrix<Complex>* A, char* Name, int x, int y){
   Float im = imag((*A)[0][0]) - imag((*A)[0][1]);
   String R = RoundTo(real((*A)[0][0]) - real((*A)[0][1]),3);
   String X = RoundTo(fabs(im),3);
   String Z = R + (im>0?" + j":" - j") + X;
   Canvas->TextOutA(x, y, ((String)Name + " " + Z).c_str());
   return y+22;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLine_parameters::TimerTimer(TObject *Sender)
{
   int y=0;
	if( Line->Z0.size()>1 ){
      Canvas->Rectangle(-10,-10,ClientWidth+10,ClientHeight+10);
   	Canvas->TextOutA( 10, 10, ((String)"Параметры линии на частоте " + (toStr<int>(freq)).c_str() + "Гц").c_str());

      Canvas->TextOutA( 10, 36, "Удельные параметры:");
		y = printMatrix(Line->Z0[1], "Zo =", 10, 62);
   	y = printMatrix(Line->Y0[1], "Yo =", 10, y + 20);

      Canvas->TextOutA( 10, y+16, "Удельные параметры ПОН последовательностей:");
      static Complex h = polar(1,Float(2)/3*M_PI );
      static Complex h2 = polar(1,-Float(2)/3*M_PI );;
      static Complex _H[9]={ 1,1,1, h2,h,1, h,h2,1 };
      static Matrix<Complex> H(3,3,_H);
      static Matrix<Complex> H_1 = H.inverse();
      Matrix<Complex> Z0pnz = H_1*(*Line->Z0[1])*H;
      Matrix<Complex> Y0pnz = H_1*(*Line->Y0[1])*H;
      y = printMatrix(&Z0pnz, "Zo[ПОН] =", 10, y + 40);
   	y = printMatrix(&Y0pnz, "Yo[ПОН] =", 10, y + 20);

      Canvas->TextOutA( 10, y+16, "Волновые параметры:");
      Matrix<Complex> Y = sqrt( (*Line->Z0[1]) * (*Line->Y0[1]) ) ;
      Matrix<Complex> Zv = (*Line->Z0[1]) * Y.inverse() ;
		y = printMatrix(&Zv, "Zv =", 10, y + 40);
   	y = printMatrix(&Y, "y =", 10, y + 20);

      // параметры однофазной схемы замещения
      Canvas->TextOutA( 10, y+26, "Парам.однолин.схемы замещ.при симметричной нагрузке:");
      y=y+52;
      if(!Line->is_Transpositioned){
         Canvas->TextOutA( 10, y, "Линия не симметрична (используйте транспозицию)");
      }else{
         y = printParametr(Line->Z0[1], "Zo =", 10, y);
         y = printParametr(Line->Y0[1], "Yo =", 10, y);
         y = printParametr(&Zv, "Zv =", 10, y);
         printParametr(&Y, "y =", 10, y);
         }
      }
   ClientHeight = y + 16;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLine_parameters::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   // остановим вывод графика
   Timer->Enabled = false;
   // ищем свой указатель в векторе и удаляем его
   unsigned n = LineParametersForms.size();
   for (unsigned i=0;i<n;i++)
      if(LineParametersForms[i] == this){
         LineParametersForms.erase(LineParametersForms.begin()+i);
         break;
         }
   /*
   caNone   The form is not allowed to close, so nothing happens.
   caHide   The form is not closed, but just hidden. Your application can still access a hidden form.
   caFree   The form is closed and all allocated memory for the form is freed.
   caMinimize   The form is minimized, rather than closed. This is the default action for MDI child forms.
   */
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLine_parameters::FormShow(TObject *Sender)
{
    if( Line )
    	Caption = (String)"Линия: " + Line->Name.c_str();	
}
//---------------------------------------------------------------------------


