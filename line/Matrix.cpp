//---------------------------------------------------------------------------

#pragma hdrstop

#include "matrix.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/*/ перемножение с созврат результата в матрицу N x 1
void Multiply_Matrix(Matrix<Float>  *op1, Float op2[], Float *result[]){
	for(unsigned int i=0; i<op1->rows; i++){
         *result[i] = 0;
      	for(unsigned int k=0; k<op1->cols; k++)
      		(*result[i]) += op1->A[i][k] * op2[k];
         }
} */
//---------------------------------------------------------------------------
/*/ копирование одной матрицы в другую
bool Copy_Matrix(Matrix<char> *from,Matrix<Float> *to){
   if(from->Rows()!=to->Rows() || from->Cols()!=to->Cols()) {err( 0, "ѕрисваивание матрицы: матрицы имеют разный размер" ); return false;}
	for(unsigned int i=0; i<from->Rows(); i++)
      for(unsigned int j=0; j<from->Cols(); j++)
      	(*to)[i][j] = (Float)(*from)[i][j];
   return true;
} */
//---------------------------------------------------------------------------
/*Float fabs(complex<Float> X){
   return sqrt(norm(X));
}*/














