//---------------------------------------------------------------------------

#ifndef line_parametersH
#define line_parametersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include "model.h"
//---------------------------------------------------------------------------
class TfrmLine_parameters : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer;
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	int printMatrix(Matrix<Complex>* A, char* Name, int x=0, int y=0);
public:		// User declarations
	__fastcall TfrmLine_parameters(TComponent* Owner);
   link *Line;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLine_parameters *frmLine_parameters;
//---------------------------------------------------------------------------
#endif
