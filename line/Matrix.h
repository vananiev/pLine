//---------------------------------------------------------------------------
#ifndef matrixH
#define matrixH
#include <vector>
#include <math.h>
#include <system.hpp>
#include <string>
#include <complex>
#include <vector>
using namespace std;
extern void err(unsigned, string);
typedef long double Float;
typedef complex<Float>  Complex;
//---------------------------------------------------------------------------
template<class T> class Matrix;
//bool Copy_Matrix(Matrix<char> *from, Matrix<Float> *to);
template<class T> Matrix<T> operator*(T op1, Matrix<T> op2);
template<class T> Matrix<T> operator*(Float op1, Matrix<T> op2);
//---------------------------------------------------------------------------
template<class T> class Matrix{
   private:
		unsigned __fastcall GetRows(){return rows;};
      unsigned __fastcall GetCols(){return cols;};
      T **A;	// �������� �������
      unsigned int rows; // ����� �����
      unsigned int cols; // ����� ��������
      void Connected_Nodes(unsigned Node, bool used_node[], Matrix &Tree, unsigned &NumWritedBranch); // ����� �������� ����� ��� ���������� ������
      void reset_and_resize(unsigned int num_rows, unsigned int num_columns);
   public:
      Matrix(unsigned int num_rows, unsigned int num_columns); 		// ������� �������
      Matrix(unsigned int num_rows, unsigned int num_columns, T *B);// �������������� �������
      Matrix(Matrix &M);	// ����� ������� - ����� ������
      Matrix(const Matrix<T> &ob);//����������� �����
   	~Matrix();
      T *operator[](unsigned int i);
      operator Matrix<Complex>();
      //Matrix<T> operator=(Matrix<T> op2);
      Matrix<T> &operator=(const Matrix<T> &op2);
      Matrix<T> operator=(T *op2);
	   Matrix operator+(Matrix op2);
      Matrix<T> operator-(Matrix<T> op2);
      Matrix<T> operator-();
      Matrix<T> operator*(Matrix<T> op2);
      Matrix<T> operator*(vector<T> op2);
      void link_to_massiv(T** to);
      friend Matrix operator*<>(T op1, Matrix op2);
      friend Matrix operator*<>(Float op1, Matrix op2);
      Matrix transpose();
      unsigned triangular(Matrix &temp);
      Matrix<T> triangular();
      T det();
      //Matrix< complex<Float> > inverse_complex();
      Matrix inverse();
      void Delete_Row(unsigned row);
      Matrix tree(bool deleted_branch[]);	// �� ������� ���������� ����� �������� ���.���. ������, ���������� �������� ����� (deleted_branch[i]==true, ���� �������)
      Matrix tree_with_null_branch(); // �� ������� ���������� ����� �������� ���.���. ������, �������� �� ������ �����
		//__property unsigned Rows  = { read=GetRows };
      //__property unsigned Cols  = { read=GetCols };
		unsigned __fastcall Rows(){return rows;};
      unsigned __fastcall Cols(){return cols;};
      void Distance_From_Node(unsigned Start_Node, unsigned Distance[]);	// ����� ����������� ���������� �� ���� �� ����
		Matrix<char> Way(unsigned Start_Node, unsigned End_Node); // ���������� ���� �� ���� �� ����
		//friend void Multiply_Matrix(Matrix<Float>  *op1, Float op2[], Float *result[]);
      void set_to_zero();
      void insert_to(Matrix *To, unsigned to_row, unsigned to_col);
};
//-------------------------------------------------------------------------
template < class T > Matrix<T>::Matrix(unsigned int num_rows, unsigned int num_columns){
   rows = num_rows;
   cols = num_columns;
	A = new T*[rows];
   for(unsigned k=0; k<rows;k++)
   	A[k] = new T[cols];
   for(unsigned int i=0; i<rows; i++)
   	for(unsigned int j=0; j<cols; j++)
      	A[i][j]=T();
}
//-------------------------------------------------------------------------
template < class T > Matrix<T>::Matrix(unsigned int num_rows, unsigned int num_columns, T *B){
   rows = num_rows;
   cols = num_columns;
	A = new T*[rows];
   for(unsigned k=0; k<rows;k++)
   	A[k] = new T[cols];
   for(unsigned int i=0; i<rows; i++){
   	T *p = (B+i*cols);
   	for(unsigned int j=0; j<cols; j++)
      	A[i][j]=p[j];
      }
}
//---------------------------------------------------------------------------
template < class T > Matrix<T>::Matrix(Matrix &M){
   rows = M.rows;
   cols = M.cols;
	A = new T*[rows];
   for(unsigned k=0; k<rows;k++)
   	A[k] = new T[cols];
   for(unsigned int i=0; i<rows; i++)
   	for(unsigned int j=0; j<cols; j++)
      	A[i][j]=M.A[i][j];
}
//---------------------------------------------------------------------------
// ��� ����������� �����
template < class T > Matrix<T>::Matrix(const Matrix<T> &ob){
   rows = ob.rows;
   cols = ob.cols;
   A = new T*[rows];
   for(unsigned k=0; k<rows;k++){
   	try{ A[k] = new T[cols];}
      catch(std::bad_alloc xa) {err(0, "�������� ������");}
      }
   for(unsigned int i=0; i<rows; i++)
   	for(unsigned int j=0; j<cols; j++)
      	A[i][j]=ob.A[i][j];
}
//---------------------------------------------------------------------------
template <class T>  Matrix<T>::~Matrix(){
   for(unsigned k=0; k<rows;k++)
   	{delete [] A[k];  A[k] = NULL;}
   delete [] A; A = NULL;
}
//-------------------------------------------------------------------------
template < class T > void Matrix<T>::reset_and_resize(unsigned int num_rows, unsigned int num_columns){
   for(unsigned k=0; k<rows;k++)
   	{delete [] A[k];  A[k] = NULL;}
   delete [] A; A = NULL;
   rows = num_rows;
   cols = num_columns;
	A = new T*[rows];
   for(unsigned k=0; k<rows;k++)
   	A[k] = new T[cols];
}
//---------------------------------------------------------------------------
template <class T> Matrix<T>  Matrix<T>::operator+(Matrix<T> op2){
	Matrix<T> temp(rows, cols);
   if(rows!=op2.rows || cols!=op2.cols) {err( 0, "�������� ������: ������� ����� ������ ������ __FILE__:__LINE__" ); return temp;}
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	temp.A[i][j] = A[i][j] + op2.A[i][j];
   return temp;
}
//---------------------------------------------------------------------------
template <class T> T* Matrix<T>::operator[](unsigned int i){
	return A[i];
}
//---------------------------------------------------------------------------
template<class T> Matrix<T>::operator Matrix<Complex>(){
   Matrix<Complex> R(rows,cols);
   for(unsigned int i=0; i<rows; i++)
   	for(unsigned int j=0; j<cols; j++)
      	R[i][j]=(Float)A[i][j];
   return R;
}
//---------------------------------------------------------------------------
/*template<class T> Matrix<T> Matrix<T>::operator=(Matrix<T> op2){
   if(rows!=op2.rows || cols!=op2.cols) {err( 0, "������������ �������: ������� ����� ������ ������" ); return *this;}
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	A[i][j] = op2.A[i][j];
   return *this;
} */
//---------------------------------------------------------------------------
template<class T> Matrix<T> & Matrix<T>::operator=(const Matrix<T> &op2){
   if(rows!=op2.rows || cols!=op2.cols) {
      //err( 0, "������������ �������: ������� ����� ������ ������. ������ ����������" );
      //return *this;
      reset_and_resize(op2.rows, op2.cols);
      }
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	A[i][j] = op2.A[i][j];
   return *this;
}
//---------------------------------------------------------------------------
template<class T> Matrix<T> Matrix<T>::operator=(T *op2){
   for(unsigned int i=0; i<rows; i++)
   	for(unsigned int j=0; j<cols; j++)
      	A[i][j]=*(op2+i*cols+j);
   return *this;
}
//---------------------------------------------------------------------------
template<class T> Matrix<T>  Matrix<T>::operator-(Matrix<T> op2){
	Matrix<T> temp(rows, cols);
   if(rows!=op2.rows || cols!=op2.cols) {err( 0, "��������� ������: ������� ����� ������ ������"); return temp;}
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	temp.A[i][j] = A[i][j] - op2.A[i][j];
   return temp;
}
//---------------------------------------------------------------------------
template<class T> Matrix<T> Matrix<T>::operator-(){
	Matrix<T> temp(rows, cols);
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	temp.A[i][j] = - A[i][j];
   return temp;
}
//---------------------------------------------------------------------------
template<class T> Matrix<T>  Matrix<T>::operator*(vector<T> op2){
	Matrix<T> temp(rows, 1);
   if(cols!=op2.size()) {err( 0, "��������� ������: ����� ����� � �������� �� ���������"); return temp;}
	for(unsigned int i=0; i<rows; i++){
      temp.A[i][0] = 0;
      for(unsigned int k=0; k<cols; k++)
         temp.A[i][0] += A[i][k] * op2[k];
      }
   return temp;
}
//---------------------------------------------------------------------------
template<class T> Matrix<T>  Matrix<T>::operator*(Matrix<T> op2){
	Matrix<T> temp(rows, op2.cols);
   if(cols!=op2.rows) {err( 0, "��������� ������: ����� ����� � �������� �� ���������"); return temp;} 
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<op2.cols; j++){
         temp.A[i][j] = 0;
      	for(unsigned int k=0; k<cols; k++)
      		temp.A[i][j] += A[i][k] * op2.A[k][j];
         }
   return temp;
}
//---------------------------------------------------------------------------
// ������ ������ ��������� �� �������-�������
template<class T> void Matrix<T>::link_to_massiv(T** to){
   if(cols!=1) {err( 0, "������������� ������� ������� � ������: ����� �������� � ������� �� ����� 1"); return;}
   for(unsigned int j=0; j<rows; j++)
   	to[j] = &A[j][0];
}
//---------------------------------------------------------------------------
// ������������� �������
template<class T > Matrix< T > Matrix< T >::transpose(){
	Matrix< T > temp(cols, rows);
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	temp.A[j][i] = A[i][j];
   return temp;
}
//---------------------------------------------------------------------------
/* ���������� ������� � ������������ ���� (���������� ����� ������������)*/
template<class T> unsigned Matrix<T>::triangular(Matrix &temp){
   if(temp.cols != temp.rows) {err( 0, "���������� ������� � ������������ ����: ����� ����� � �������� �� ���������"); return 0;}
   // �� ������ ������ �� ��� ����?
   unsigned h;
   for(unsigned h=0; h< rows; h++) if(fabs(temp.A[h][0])>1e-300) break;
   if(h==rows){err( 0, "��������� �������� �������: ��� �������� � ������ ������� ������� �������"); return 0;}
   unsigned u=0;	// ����� ������������ �����
   for(unsigned i=0; i<rows-1; i++){
      // ���� ������������ ������� ���� ������ ������ �������
      if(fabs(temp.A[i][i]) < 1e-300){
         //����� ������ � ��������� ������������ ���������
         unsigned NotNull; for(NotNull=i+1; NotNull<rows; NotNull++) if(fabs(temp.A[NotNull][i])>1e-300) break;
         if(NotNull==rows) {err( 0, "���������� ������� � ������������ ����: ������ � ������ ���������� ������������� ��������"); return 0;}
         T *tmp = temp.A[i];
         temp.A[i] = temp.A[NotNull];
      	temp.A[NotNull] = tmp;
         u++;
         }
      for(unsigned k=i+1; k<rows; k++){
      	if(temp.A[k][i] != 0){
         	// ��������� ���������
         	T d = -1.0*temp.A[k][i]/temp.A[i][i];
         	// ���������� ���������� ������
         	temp.A[k][i] = 0;
         	for(unsigned j=i+1; j<cols; j++)
            	temp.A[k][j] = d*temp.A[i][j] + temp.A[k][j];
            }
      	}
   	}
   return u;
}
//---------------------------------------------------------------------------
/* ���������� ������� � ������������ ���� */
template<class T> Matrix<T> Matrix<T>::triangular(){
	Matrix<T> temp(*this);
   triangular(temp);
   return temp;
}
//---------------------------------------------------------------------------
/* ������� ������������ ������� */
template<class T> T Matrix<T>::det(){
	T d;
   if(cols != rows) {err( 0, "����� ������������: ������� �� ����������"); return 0;}
   Matrix temp(*this);
   unsigned u = triangular(temp);
   if( u%2==0 ) d=1; else d=-1;
   for(unsigned i=0; i<rows; i++)
   	d *= temp.A[i][i];
   return d;
}
//---------------------------------------------------------------------------
/* ������� �������� ������� ������� ������ � ������� */
template<class T> Matrix<T> Matrix<T>::inverse(){
   Matrix temp(*this);
   Matrix E(rows, cols);
   if(temp.cols != temp.rows) {err( 0, "��������� �������� �������: �������� ������� �� ����������"); return E;}
   // �� ������ ������ �� ��� ����?
   unsigned h;
   for(h=0; h< rows; h++) if(fabs(A[h][0])>1e-300) break;
   if(h==rows){err( 0, "��������� �������� �������: ��� �������� � ������ ������� ������� �������"); return E;}
   // ��������� ��������� �������
   for(unsigned i=0; i<rows; i++) E.A[i][i] = 1;
   unsigned u=0;	// ����� ������������ �����
   // � ������ �����������
   for(unsigned i=0; i<rows-1; i++){
      // ������� ������������ ������� � �������
      unsigned max_i = i;
      Float max = fabs(temp.A[i][i]);
      for(unsigned r=i+1; r<rows-1; r++)
         if(fabs(temp.A[r][i]) > max){
            max = fabs(temp.A[r][i]);
            max_i = r;
            }
      if(max_i != i){ // ������ ������ �������
         T *tmp = temp.A[i];
         temp.A[i] = temp.A[max_i];
      	temp.A[max_i] = tmp;
         u++;
         // � ��� ���������
         tmp = E.A[i];
      	E.A[i] = E.A[max_i];
      	E.A[max_i] = tmp;
         }
      // ���� ������������ ������� ���� ������ ������ �������
      if(fabs(temp.A[i][i]) < 1e-300){
         //����� ������ � ��������� ������������ ���������
         unsigned NotNull; for(NotNull=i+1; NotNull<rows; NotNull++) if(fabs(temp.A[NotNull][i])>1e-300) break;
         if(NotNull==rows) {err( 0, "���������� ������� � ������������ ����: ������ � ������ �� �������� ������������� ��������"); return E;}
         T *tmp = temp.A[i];
         temp.A[i] = temp.A[NotNull];
      	temp.A[NotNull] = tmp;
         u++;
         // � ��� ���������
         tmp = E.A[i];
      	E.A[i] = E.A[NotNull];
      	E.A[NotNull] = tmp;
         }
      // �������� �� ������������ �������� "1"
      Float d = temp.A[i][i];
      for(unsigned j=i; j<cols; j++)
      	temp.A[i][j] = temp.A[i][j]/d;
      for(unsigned j=0; j<cols; j++)
      	E.A[i][j] = E.A[i][j]/d;
      for(unsigned k=i+1; k<rows; k++){
         	// ��������� ���������
         	Float d = -temp.A[k][i];
         	// ���������� ���������� ������
         	temp.A[k][i] = 0;
         	for(unsigned j=i+1; j<cols; j++)
            	temp.A[k][j] = d*temp.A[i][j] + temp.A[k][j];
            for(unsigned j=0; j<cols; j++)
            	E.A[k][j] = d*E.A[i][j] + E.A[k][j];
      	}
   	}
   // �������� �� ������� ������������ �������� "1"
	Float d = 1.0*temp.A[rows-1][rows-1];
   if(d==0){
      err(0,"Matrix::inverse(): ������� �� ����");
      d=1e-100;}
   temp.A[rows-1][rows-1] = 1;
   for(unsigned j=0; j<cols; j++)
      E.A[rows-1][j] = E.A[rows-1][j]/d;

   //� �������� �����������
   for(unsigned i=rows-1; i>0; i--){
      for(unsigned k=0; k<i; k++){
      	if(fabs(temp.A[k][i])>1e-300){
         	// ��������� ���������
         	Float d = -temp.A[k][i];
         	// ���������� ���������� ������
         	temp.A[k][i] = 0;
            for(unsigned j=0; j<cols; j++)
            	E.A[k][j] = d*E.A[i][j] + E.A[k][j];
            }
      	}
   	}
   return E;
}
/*/---------------------------------------------------------------------------
Matrix< complex<Float> > Matrix< complex<Float> >::inverse_complex(){
	return inverse< Matrix< complex<Float> > >();
} */
//---------------------------------------------------------------------------
/*template <>
Matrix< Matrix<complex<Float> > > Matrix< Matrix<complex<Float> > >::inverse(){
	// ���������� ����� �������� � �����
   unsigned int Row = 0, Col = 0;
   for(unsigned i=0; i<rows; i++) Row+= A[0][i].Rows();
   for(unsigned i=0; i<cols; i++) Col+= A[i][0].Cols();

   // ��������� ������� �� ������ ����� ����-��������
   Matrix< complex<Float> > S(Row, Col);
   unsigned int i_rows = 0, j_cols = 0;
   for(unsigned i=0; i<rows; i++)
   	for(unsigned j=0; j<cols; j++){
      	unsigned n=A[i][j].Rows();
         unsigned m=A[i][j].Cols();
         for(unsigned h=0; h<n; h++)
            for(unsigned g=0; g<m; g++)
            	S[i_rows + h][j_cols + g] = (A[i][j])[h][g];
         i_rows += n;
         j_cols += m;
      	}

   Matrix< complex<Float> > I = S.inverse_complex();

   // ����������� ������� � ���������� == �������
   Matrix< Matrix<complex<Float> > > Res(rows, cols);
   i_rows = 0; j_cols = 0;
   for(unsigned i=0; i<rows; i++)
   	for(unsigned j=0; j<cols; j++){

      	unsigned n=A[i][j].Rows();
         unsigned m=A[i][j].Cols();
         Matrix< complex<Float> > tmp(n,m);

         for(unsigned h=0; h<n; h++)
            for(unsigned g=0; g<m; g++)
            	tmp[h][g] = I[i_rows + h][j_cols + g];
         i_rows += n;
         j_cols += m;

         Res[i][j] = tmp;
      	}
   return Res;
}  */
//---------------------------------------------------------------------------
template <>
Matrix< complex<Float> > Matrix< complex<Float> >::inverse()
{
   Matrix temp(*this);
   Matrix E(rows, cols);
   if(temp.cols != temp.rows) {err( 0, "��������� �������� �������: �������� ������� �� ����������"); return E;}
   // �� ������ ������ �� ��� ����?
   unsigned h;
   for(h=0; h< rows; h++) if(norm(A[h][0])>1e-300) break;
   if(h==rows){err( 0, "��������� �������� �������: ��� �������� � ������ ������� ������� �������"); return E;}
   // ��������� ��������� �������
   for(unsigned i=0; i<rows; i++) E.A[i][i] = 1;
   unsigned u=0;	// ����� ������������ �����
   // � ������ �����������
   for(unsigned i=0; i<rows-1; i++){
      // ������� ������������ ������� � �������
      unsigned max_i = i;
      Float max = norm(temp.A[i][i]);
      for(unsigned r=i+1; r<rows-1; r++)
         if(norm(temp.A[r][i]) > max){
            max = norm(temp.A[r][i]);
            max_i = r;
            }
      if(max_i != i){ // ������ ������ �������
         complex<Float> *tmp = temp.A[i];
         temp.A[i] = temp.A[max_i];
      	temp.A[max_i] = tmp;
         u++;
         // � ��� ���������
         tmp = E.A[i];
      	E.A[i] = E.A[max_i];
      	E.A[max_i] = tmp;
         }
      // ���� ������������ ������� ���� ������ ������ �������
      if(norm(temp.A[i][i]) < 1e-300){
         //����� ������ � ��������� ������������ ���������
         unsigned NotNull; for(NotNull=i+1; NotNull<rows; NotNull++) if(norm(temp.A[NotNull][i])>1e-300) break;
         if(NotNull==rows) {err( 0, "���������� ������� � ������������ ����: ������ � ������ �� �������� ������������� ��������"); return E;}
         complex<Float> *tmp = temp.A[i];
         temp.A[i] = temp.A[NotNull];
      	temp.A[NotNull] = tmp;
         u++;
         // � ��� ���������
         tmp = E.A[i];
      	E.A[i] = E.A[NotNull];
      	E.A[NotNull] = tmp;
         }
      // �������� �� ������������ �������� "1"
      complex<Float> d = temp.A[i][i];
      for(unsigned j=i; j<cols; j++)
      	temp.A[i][j] = temp.A[i][j]/d;
      for(unsigned j=0; j<cols; j++)
      	E.A[i][j] = E.A[i][j]/d;
      for(unsigned k=i+1; k<rows; k++){
         	// ��������� ���������
         	complex<Float> d = -temp.A[k][i];
         	// ���������� ���������� ������
         	temp.A[k][i] = 0;
         	for(unsigned j=i+1; j<cols; j++)
            	temp.A[k][j] = d*temp.A[i][j] + temp.A[k][j];
            for(unsigned j=0; j<cols; j++)
            	E.A[k][j] = d*E.A[i][j] + E.A[k][j];
      	}
   	}
   // �������� �� ������� ������������ �������� "1"
	complex<Float> d = temp.A[rows-1][rows-1];
   if(norm(d)==0){
      err(0,"Matrix::inverse(): ������� �� ����");
      d=1e-100;}
   temp.A[rows-1][rows-1] = 1;
   for(unsigned j=0; j<cols; j++)
      E.A[rows-1][j] = E.A[rows-1][j]/d;

   //� �������� �����������
   for(unsigned i=rows-1; i>0; i--){
      for(unsigned k=0; k<i; k++){
      	if(norm(temp.A[k][i])>1e-300){
         	// ��������� ���������
         	complex<Float> d = -temp.A[k][i];
         	// ���������� ���������� ������
         	temp.A[k][i] = 0;
            for(unsigned j=0; j<cols; j++)
            	E.A[k][j] = d*E.A[i][j] + E.A[k][j];
            }
      	}
   	}
   return E;
}
//---------------------------------------------------------------------------
template<class T> Matrix<char> Matrix<T>::Way(unsigned Start_Node, unsigned End_Node){
   Matrix<char> way(1,cols);
   if(Start_Node==End_Node) return way;
	unsigned *Distance = new unsigned[cols];
   for(unsigned i=0;i<cols;i++) Distance[i] = -1;
   // ����� ���������� �� ����
   Distance[Start_Node] = 0;
   Distance_From_Node(Start_Node, Distance);
   //unsigned g[200];
   //for(unsigned i=0; i<cols;i++)g[i]=Distance[i];
   // ���������� ����
   unsigned nB = cols;
   unsigned nN = rows;
   // ���� �� �������� �������
   unsigned s = End_Node;
   while( s != Start_Node){
   		unsigned Dis = Distance[s]-1; // ���� �������� ���� �� ����� ���������� �� ����������
   		// ���� ����� �� �������� �������
         unsigned i;
   		for(i=0;i<nB;i++){
   			if(A[s][i]!=0){
               // � ����� ���������������� ������� ��������
               unsigned j;
               for(j=0;j<nN;j++)
               	if(A[j][i]!=0 && Dis == Distance[j]){
                     way[0][i] = A[s][i];	// 1 ��� -1 -� ����������� �� ����������� ��� ������ �����
               		s = j;
               		break;
                     }
               if(j<nN) break; // ������� � ������ ����, ����� ������ ����������
            	}
            }
         // ��� ����
         if(i >= nB){
            for(unsigned x=0;x<nB;x++) way[0][x]=0;
            break;
         	}
      	}
   delete [] Distance;
   return way;
}
//---------------------------------------------------------------------------
// ����������� ����� ���������� �� ����
template<class T> void Matrix<T>::Distance_From_Node(unsigned Start_Node, unsigned Distance[]){ //, End_Node
   vector<unsigned> Cocedi;
   Float Find_Dist = Distance[Start_Node]+1;
   //���� �������� �� �������������� ����
   for(unsigned j=0; j<cols; j++)
   	if(fabs(A[Start_Node][j])>1e-300) // ���� ���� �����, ������ �� ������ ��� ����
      	for(unsigned i=0; i<rows; i++)
         	if(fabs(A[i][j])>1e-300 && fabs(Find_Dist-Distance[i])<1e-300){	// ���� �� �� ���� ���������� ������
               Distance[i] = Find_Dist;			// �������� ����
               Cocedi.push_back(i);
            	}
   // ���� ����������
   for(unsigned i=0, n=Cocedi.size(); i<n; i++)
   	Distance_From_Node(Cocedi[i], Distance);//, End_Node ����� �������� �����
}
//---------------------------------------------------------------------------
// ����������� ����� ���������� �� ����
template<> void Matrix<char>::Distance_From_Node(unsigned Start_Node, unsigned Distance[]){
   std::vector <unsigned> Cocedi;  // std::vector
   unsigned Find_Dist = Distance[Start_Node]+1;
   //���� �������� �� �������������� ����
   for(unsigned j=0; j<cols; j++)
   	if(A[Start_Node][j]!=0) // ���� ���� �����, ������ �� ������ ��� ����
      	for(unsigned i=0; i<rows; i++)
         	if(A[i][j]!=0 && Find_Dist<Distance[i]){	// ���� �� �� ���� ���������� ������
               Distance[i] = Find_Dist;			// �������� ����
               Cocedi.push_back(i);
            	}
   // ���� ����������
   for(unsigned i=0, n=Cocedi.size(); i<n; i++)
   	Distance_From_Node(Cocedi[i], Distance);// ����� �������� �����
}
//---------------------------------------------------------------------------
// ����������� ����� �������� �����
template<class T> void Matrix<T>::Connected_Nodes(unsigned Node, bool used_node[], Matrix &Tree, unsigned &NumWritedBranch){
   //���� �������� �� �������������� ����
   for(unsigned j=0; j<cols; j++)
   	if(fabs(A[Node][j])>1e-300) // ���� ���� �����, ������ �� ������ ��� ����
      	for(unsigned i=0; i<rows; i++)
         	if(fabs(A[i][j])>1e-300 && !used_node[i]){	// ���� �� �� ��������������� ����
               used_node[i] = true;			// �������� ����
               for(unsigned s=0; s<rows; s++) Tree.A[s][j] = A[s][j]; // �������� ����������
               NumWritedBranch++;
               if(NumWritedBranch < (Tree.rows-1))	// ����� �� ����������
               	Connected_Nodes(i, used_node, Tree, NumWritedBranch);// ����� �������� �����
            	break;
            	}
}
//---------------------------------------------------------------------------
template<class T> Matrix<T> Matrix<T>::tree(bool deleted_branch[]){
// ����� ����� � ������ ������ ���� �� 1 ��� ������
	Matrix Res(rows, rows-1);
   Matrix Tree(rows, cols);
   // ���� ������
   Tree = tree_with_null_branch();
   // ������� ������� �������
   for(unsigned i=0; i<cols;i++) deleted_branch[i]=true;	// ���������� �������, ��� ��� ����� �������
   unsigned write_col=0;
   for(unsigned h=0;h<Tree.cols;h++){
   	unsigned i;
   	for(i=0;i<Tree.rows;i++)if(fabs(Tree.A[i][h])>1e-300) break;
      if(i!=Tree.rows){    // � ������� �� ��� ����
         deleted_branch[h] = false;	// ����� �� �������
         for(unsigned r=0;r<Tree.rows;r++) Res.A[r][write_col]=Tree.A[r][h];
         write_col++;
         }
      }
   if(write_col != Res.Cols){err(0, "Matrix::tree: ��� ���������� ������ �� ����������� �����_������ == (�����_�����-1)");}
   return Res;
   /*
   // ��������� ������ �����
   unsigned k=0;  // ����� ����������� ����� � ������
   unsigned j;    // ������� �����
   for(j=0; j<cols; j++){
   	unsigned h=0;
   	// �� ��� �� �������� ������� == 0 ?
   	for(h=0; h<rows; h++) if(fabs(A[h][j])>1e-300) break;
   	if(h!=rows){
      	for(unsigned i=0; i<rows; i++){
            if(fabs(A[i][j])>1e-300) used_node[i]=true;
         	temp.A[i][k] = A[i][j];
            }
         deleted_branch[j] = false; // ��������� j-�����
         k++;
      	break;
         }
      }
   // ��������� ����� ����������� �� ����� ����
   for(j++; j<cols; j++){
     if(k<temp.cols) // ���� ����� ��� �����
   	for(unsigned i=0; i<rows; i++)
      	if(fabs(A[i][j])>1e-300 && !used_node[i]){
         	// ���������
            for(unsigned s=0; s<rows; s++) temp.A[s][k] = A[s][j];
            deleted_branch[j] = false; // ��������� j-�����
            used_node[i] = true;			// ������� ��� ������������ �������
            k++; // � ����� ������ ����� �� ���� ����� ������
            break;
         	}
   	} */
}
//---------------------------------------------------------------------------
template<class T> Matrix<T> Matrix<T>::tree_with_null_branch(){
   // ����� ����� � ������ ������ ���� �� 1 ��� ������
   Matrix Tree(rows, cols);
   bool *used_node = new bool[Tree.rows];
   for(unsigned i=0; i<Tree.rows;i++) used_node[i]=false;
   // ��������� ������� ����
   static unsigned Uz=0; // ������� ����
   used_node[Uz]=true;
   unsigned k=0;	 // ����� ������� ������ � ������
   //���� �������� �� �������������� ����
   Connected_Nodes(Uz, used_node, Tree, k);
   delete [] used_node;
   return Tree;
	/*/ ����� ����� � ������ ������ ���� �� 1 ��� ������
	Matrix temp(rows, cols);
   bool *used_node = new bool[temp.rows];
   for(unsigned i=0; i<temp.rows;i++) used_node[i]=false;
   // ��������� ����� ����������� �� ����� ����
   for(unsigned j=0; j<cols; j++){
   	unsigned i;
   	for(i=0; i<rows; i++)
      	if(fabs(A[i][j])>1e-300 && !used_node[i]){ // ��������� �� ����� ����� ����?
         	// ��
            for(unsigned s=0; s<rows; s++) temp.A[s][j] = A[s][j];
            break;
         	}
      if(i==rows) // ���
          for(unsigned s=0; s<rows; s++) temp.A[s][j] = 0;
   	}
   return temp;*/
}
//---------------------------------------------------------------------------
template<class T> void Matrix<T>::Delete_Row(unsigned row){
	for(unsigned i=row; i<(rows-1);i++) A[i] = A[i+1];
   delete A[rows-1];
   rows--;
}
//---------------------------------------------------------------------------
template<class T> Matrix<Float> Multiply_Matrix(Matrix<T>  *op1, Float ***op2, unsigned op2_rows, unsigned op2_cols)
{
   Matrix<Float> temp(op1->Rows, op2_cols);
   if(op1->Cols!=op2_rows) {err( 0, "��������� ������: ����� ����� � �������� �� ���������"); return temp;}
   for(unsigned int i=0; i<op1->Rows; i++)
      for(unsigned int j=0; j<op2_cols; j++){
         temp[i][j] = 0;
      	for(unsigned int k=0; k<op1->Cols; k++)
      		temp[i][j] += (Float)(*op1)[i][k] * (*op2[k][j]);
         }
   return temp;
}

//---------------------------------------------------------------------------

template<class T> Matrix<T> exp(Matrix<T> X)
{
	unsigned n=X.Rows(), m=X.Cols();
   if(n!=m){err( 0, "���������� �������: ������� ������ ���� ����������"); Matrix<T> N(n,m); return N;}
   Matrix<T> E(n,n);
   for(unsigned i=0;i<n;i++) E[i][i] = 1;
   Matrix<T> Ex = E;
   Float fact = 1;
   Matrix<T> Xn = E;
   Matrix<T> Addend(n,n);
   Float eps = 1e-6;		// �������� ���������� �������� �������
   int i;
   for(i=1; i<1000; i++){
      fact *= i;
      Xn = Xn * X;
      if(i%20)	Ex = Ex + 1/fact * Xn;	// ������ ������� 10 ���
      else{ // ���� �� ������������ ?
      	Addend = 1/fact * Xn;
         Ex = Ex + Addend;
         if( norm(Addend) < eps*norm(Ex))
         	break;
         }
      }
	return Ex;
}

//---------------------------------------------------------------------------

template<class T> Matrix<T> log(Matrix<T> X)
{
	unsigned n=X.Rows(), m=X.Cols();
   if(n!=m){err( 0, "�������� �������: ������� ������ ���� ����������"); Matrix<T> N(n,m); return N;}
   Matrix<T> E(n,n);
   for(unsigned i=0;i<n;i++) E[i][i] = 1;
   Matrix<T> Xn = X-E;
   Matrix<T> mul = Xn;
   Matrix<T> Addend(n,n);
   Matrix<T> LOG = Xn;
   Float eps = 1e-15;		// �������� ���������� �������� �������
   int i;
   for(i=2; i<1000; i++){

      Xn = Xn * mul;
      if(i%20)	LOG = LOG + 1/i * Xn;	// ������ ������� 10 ���
      else{ // ���� �� ������������ ?
      	Addend = 1/i * Xn;
         LOG = LOG + Addend;
         if( norm(Addend) < eps*norm(LOG))
         	break;
         }
      }
	return LOG;
}
//---------------------------------------------------------------------------

Float norm(Matrix< complex<Float> > X)
{
   Float N = 0;
	unsigned n=X.Rows(),  m=X.Cols();
   for(unsigned int i=0; i<n; i++)
      for(unsigned int j=0; j<m; j++)
         N += norm(X[i][j]);
	return N;
}

//---------------------------------------------------------------------------

bool equal(Matrix< complex<Float> > &X1, Matrix< complex<Float> > &X2){
   unsigned int n = X1.Rows();
   unsigned int m = X1.Cols();
   if(n!=X2.Rows() || m != X2.Cols())
      {err( 0, "Matrix: equal() ����� ����� � �������� �� ���������"); return false;}
   for(unsigned int i=0; i<n; i++)
      for(unsigned int j=0; j<m; j++){
         complex<Float> dX = X1[i][j] - X2[i][j];
         Float X =  abs(X1[i][j] + X2[i][j]);
         if(fabs(real(dX))>1e-15*X || fabs(imag(dX))>1e-15*X)
            return false;
      }
   return true;
}

//---------------------------------------------------------------------------

Matrix< complex<Float> > sqrt(Matrix< complex<Float> > X)
{
	unsigned n=X.Rows(),  m=X.Cols();
   Matrix< complex<Float> > A = X;
   if(n!=m){err( 0, "Matrix: ����� ����� � �������� �� ���������"); return A;}
   Matrix< complex<Float> > A_1(n,n);
   //Float eps = 1e-15*norm(X);
   while( !equal(A,A_1) ){
   	A_1 = A;
   	A = 0.5 * (A+X*A.inverse());
   	}
	return A;
}

//---------------------------------------------------------------------------

template<class T> void Matrix<T>::set_to_zero()
{
	for(unsigned int i=0; i<rows; i++)
      for(unsigned int j=0; j<cols; j++)
      	A[i][j] = 0;
}

//---------------------------------------------------------------------------
/* �������� ������� ������� � ��������� � ��������� ����� */
template<class T> void Matrix<T>::insert_to(Matrix<T> *To, unsigned to_row, unsigned to_col)
{
	if( (rows + to_row) > To->Rows() || (cols + to_col) > To->Cols()  ){
      err(0, "Matrix: ������� ������� �� ��������, ������� ������� ������� ����");
      return;
		}
   for(unsigned i=0, h=to_row; i<rows; i++, h++)
   	for(unsigned j=0, g=to_col; j<cols; j++, g++)
      	To->A[h][g] = A[i][j];
}
//---------------------------------------------------------------------------
// ��������� ������� �� �����
template<class T> Matrix<T> operator*(T op1, Matrix<T> op2){
	Matrix<T> temp(op2.rows, op2.cols);
	for(unsigned int i=0; i<op2.rows; i++)
      for(unsigned int j=0; j<op2.cols; j++)
      	temp.A[i][j] = op1 * op2.A[i][j];
   return temp;
}
//---------------------------------------------------------------------------
// ��������� ������� �� �����
template<class T> Matrix<T> operator*(Float op1, Matrix<T> op2){
	Matrix<T> temp(op2.rows, op2.cols);
	for(unsigned int i=0; i<op2.rows; i++)
      for(unsigned int j=0; j<op2.cols; j++)
      	temp.A[i][j] = op1 * op2.A[i][j];
   return temp;
}
#endif

