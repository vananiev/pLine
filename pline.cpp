//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("forms\main.cpp", frmMain);
USEFORM("forms\frmProp.cpp", frmProperty);
USEFORM("forms\ObectsLibrary.cpp", frmObjLib);
USEFORM("forms\Scope.cpp", frmScope);
USEFORM("forms\line_parameters.cpp", frmLine_parameters);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   try
   {
       Application->Initialize();
       Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->Run();
   }
   catch (Exception &exception)
   {
       Application->ShowException(&exception);
   }
   catch (...)
   {
       try
       {
          throw Exception("");
       }
       catch (Exception &exception)
       {
          Application->ShowException(&exception);
       }
   }
   return 0;
}
//---------------------------------------------------------------------------
