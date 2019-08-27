//---------------------------------------------------------------------------

#ifndef modelH
#define modelH

#include <complex>
#include <vector>
#include <string>
#include <Matrix.h>
#include <math.h>
#include "Dynamic_Array.h"
#include "kernel.h"
#include <fstream>

#define MAX 1e10
#define MIN 1e-6
#define Rp	complex<Float>(cos(2*M_PI/3), sin(2*M_PI/3))
#define Rn	complex<Float>(cos(-2*M_PI/3), sin(-2*M_PI/3))

extern int num_f;	// ����� ��������

using namespace std;

typedef complex<Float> Complex;

//---------------------------------------------------------------------------
// �������������� �������
class element;
class FieldInfo;
template<class T> class phase;
template<class T> class power;
template<class T> ostream& operator << (ostream& out, const phase<T>& p);
template<class T> istream& operator >> (istream& in, phase<T>& p);
void SaveObj(element *Obj, ostream &stream);               // ���������� ������� � �����
element* CreateObj(istream &stream);                       // �������� ������� �� ������ �� ������
element* CreateObj(String ClassName, vector<void*> Params);// ������� ������ �� ����� ������ � ���������� ����������, ������� ���������� � ����������� ������
template<class T> T imag( const phase<complex<T> > &X);
template<class T> T abs( const phase<complex<T> > &X);
template<class T> T abs( const phase<complex<T>* > &X);
template<class T> T imag( const power<complex<T> > &X);
template<class T> T abs( const power<complex<T> > &X);
//---------------------------------------------------------------------------

template<class T>
class phase{
	public:
   	T A, B, C;			// �������� ��������� � �����
      phase(){A=B=C=T();};
      T& operator[](int i);
      friend ostream& operator << <>(ostream& out, const phase<T>& p);
      friend istream& operator >> <>(istream& in, phase<T>& p);
      phase &operator=(const T &op2);
};

template<class T>
class power : public phase<T>{
   public:
      power &operator=(const T &op2);
};

//---------------------------------------------------------------------------

class element{
   private:
      void Describe_Fields();                   // �������� ������� ��������
      int _id;												// ���������� ����� ��������
   public:
      element();
      ~element();
      int id();
      string  Name;
      virtual string ClassName();					// ��� ������
      virtual bool accept(){return true;};      // ��������� ��������
      virtual bool update(){return false;};      // �������� ��������� ������� � �������� ������� �������
      vector<FieldInfo>   Fields;
      FieldInfo* Field(void* Var);              // ���������� ���� ��� ��������� ����������
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
};

//---------------------------------------------------------------------------
/* ���� */
class node : public element{
   private:
      typedef element sire;               // ����� ������
      void Describe_Fields();             // �������� ������� ��������
   protected:
      node();
   public:
      power<Complex> S;                   // S==������. �������� ����������, a S.A == ������ �������� ����, ����������� � ���� (+), ��������� (-)     
      Dynamic_Array< phase<Complex*> > U; // ���������� � ���� (�������� ������� ������������ ��������)
      virtual string ClassName();			// ��� ������
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
};

//---------------------------------------------------------------------------
/* ���� ����������� �������� */
class big_system : public node{
   private:
      typedef node sire;                  // ����� ������
      void Describe_Fields();             // �������� ������� ��������
   public:
      big_system();
      Dynamic_Array< Complex > Ustart;   	// ���������� ������� ����� �� ����� ����� ���������� Make_Equation()
      virtual string ClassName();			// ��� ������
      virtual bool update();              // �������� ��������� ������� � �������� ������� �������
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
      virtual bool accept();
};

//---------------------------------------------------------------------------
/* ��������� */
class source : public node{
   private:
      typedef node sire;                    		// ����� ������
      void Describe_Fields();                   // �������� ������� ��������
   public:
      source();
      Float X2;                                 // ������������� �������� ������������������ ����������                             
      Float Unom;                               // ���������� �������������� �� ����� (�������� ����������)
      Float P;                                  // �������� �������� ���������� ��������
      Float Qmax;                               // ������� ���������� �������� �������
      virtual string ClassName();					// ��� ������
      virtual bool accept();                    // ��������� �������� �������
      virtual bool update();                    // �������� ��������� ������� � �������� ������� �������
      virtual void Save(ostream &out);				// ���������� ���������� �������
      virtual void Read(istream &in);     		// �������� ���������� ��������
};


//---------------------------------------------------------------------------
/* ����������� */
class consumer : public node{
   private:
      typedef node sire;                  // ����� ������
      void Describe_Fields();             // �������� ������� ��������
   public:
      consumer();
      virtual string ClassName();			// ��� ������
      virtual bool update();              // �������� ��������� ������� � �������� ������� �������
      virtual bool accept();
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
};


//---------------------------------------------------------------------------
/* ���������� ����������� */
class rectifier : public node{
   private:
      typedef node sire;                  // ����� ������
      void Describe_Fields();             // �������� ������� ��������
   public:
      rectifier();
      Dynamic_Array< Complex >  I;        // ��� ������ �������� (% �� ������������) � ��������� ���� (��� ����� U(f)) � �����(���, ����)
      Dynamic_Array< phase<Complex> >  J; // ������� ������� ��� ������ ��������� (�������� ������������ ��������)
      virtual string ClassName();			// ��� ������
      virtual bool update();              // �������� ��������� ������� � �������� ������� �������
      virtual bool accept();
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
};

//---------------------------------------------------------------------------
 /* ����� */
class link : public element{
   private:
      typedef element sire;               // ����� ������
   	node* _From;
      node* _To;
      void Describe_Fields();             // �������� ������� ��������
      void rise_wire_tempetarute();       // ������ �������� ����������� �������� ���, �������� ������ �� ��� ������� 
      Float Pr_air(Float t);              // ����������� ��������
      Float Ra_air(Float t_env, Float t_wire);// ����������� �����
      Float V_air(Float t);               // �������� �������
      Float Y_air(Float t);               // ����������� ���������������� �������
      void calc_k();                      // ����������� ����������� ������� �����
      void calc_Z0_Y0();						// ������ ������������� � ������������� 
      void calc_Z0();                		// ��������� ������� �������������
      void calc_Y0(Matrix<Float>* C0);		// ��������� ������� �������������, ��������� ������� � �/��
      Matrix<Float>* calc_P0();           // ����������� ����. ���������, ��/�
      vector<Complex> d_sag;					// �������������� ����� �������� � ������ ������ ������� (������ ��������� ������ ��������� � ������ � ���� ��������� �����), ������� �� ����� ������ ������ wire_num (����� ��� �� �����)
      void calc_d_sag();
      template<class T> Matrix<T> * calc_to_phase( Matrix<T> * _Z ); // �������� ������ �������������, ����. ��������� ��
      template<class U> void cacl_transposition( Matrix<U> *X); // �������� ������� �������� ���������� � ������ ������������
      Float K_wire_Length;                // ����. ���������� ������ ������� �� ��������� � ������� ��� 
   public:
      link(node* From, node* To);
      ~link();
      node* From();                       // ��� ����������
      node* To();                         // ��� �������������
      int const num_wire;						// ����� �������� (��� ����� �����������)
      vector< Matrix<Complex>* > Z0,Y0;	// �������� ������������� � ������������ �� ��������
      Dynamic_Array< phase<Complex*> > I1, I2; // ���� � ����� (�������� ������������ ��������)
      vector< vector< Matrix<Complex> > > k;// ����������� ����������� ���������
      Float Length;                       // ������ �������� (������ ������ �������)
      Float R0;                           // ������������� ��� 20 ���� ��� ������ ��������
      Float R0_gt;                        // ������������� ��� 20 ���� ��� �������� �� (�����������)
      Float R0t[3];                       // ������� �������� ������������� �������� ���
      Float pe;									// �������� ������������� �����  (��*�)
      Float gm0, gz0;     						// �������� ������������ �� 1 ��
      vector<Complex> d;						// ������� �� ����� ������� �������� (������ ��������� ������ ��������� � ������ � ���� ��������� �����)
      vector<Complex> d_gt;					// ������� �� ����� ������� �������� �����������
      Float Sag, Sag_gt;                  // ������ �������
      Float spanLength;                   // ����� �������
      int phase_wire_num;						// ����� �������� � ����
      int const gt_phase_wire_num;			// ����� �������� � ��
		int  num_gt_wire;							// ����� ����������� � ����� ������ �������� ����������� (��)
      Float phase_radius;						// ������ ���������� �� ������� ����������� ������� ������������ ����
      Float P_sun;                        // �������� ��������� ������, ��/�^2
      Float q_sun;                        // ���� ������� ��������� ����� � ��� �������
      Float T_wire[3];                    // ����������� ������� ����
      Float T_env;                        // ����������� �����
      Float v, q;                         // �������� � ���� ����� (���) �����
      Float k_F;                          // ��������� ������� ����� ��� ��. � ����
      Float D;                            // ������� ������� ����
      Float D_gt;									// ������� ������� ��
      Float SF, SF_gt;							// stranding factor - ����.������ ���������� ������� ������� ������� (1 ������� � ����� ����)
      Float u_gt;									// ��������� ������������� ��������� ������� ��
      bool is_Transpositioned;			   // ����� � ������������� ��� ?
      Float m;                            // ����� ������� �� 1 ��
      Float e_blackness;                  // ������� ������� ������� (����. ���������)
      Float e_intake;                     // ����. ����������
      virtual string ClassName();			// ��� ������
      bool update();                      //���������� ���������� �����
      virtual bool accept();
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
};

//---------------------------------------------------------------------------
 /* ���� */
class grid : public element{
   private:
      typedef element sire;                    // ����� ������
      bool Update_Equation(int i_freq); // �������� ������� ������������� ��� ��������� ���������
      void Update_B(int i_freq);        // �������� ������ ������ ����� ���������
      bool update();             	// ��������� ��������� �������
      Float eps;                 	// ���������� ����������� �������
      bool bigger_than_eps(Matrix< complex<Float> > &dX ); // ����� �� ����������� � ���������� ��������?
      vector<node*> Nodes;          // ���� ����
      vector<link*> Links;          // ����� ����
      int Node_Num(node* Node);     // �������� ����� ���� �� �� ��������� ��� -1 ���� �� ����������
      Matrix< Complex > *A;			// ������� ������������� ����� ������������ � �������
      vector< Complex >  B; 			// ������ ����� ������ ���������
      bool calc_non_nominal_freq;   // ����������� �� �� ����������� ������� (���� �� � ���� ��������� ������ ��������)
   public:
      grid(vector<link*> Link);     // ������� ����
      ~grid();
      bool Make_Equation();        	// ���� grid �������� ���������� ����, ������� ��������� ��� ��������
      Matrix< Complex > *X; 			// ������ ���� � ����������
      virtual string ClassName();   // ��� ������
      void calc();                  // �������� ����������
      element* find(string Name);   // ���� �������

};

//---------------------------------------------------------------------------
/* ����� �������� ���������� � �������� �������, ������� ������ ����
�������������  */
enum rwMode{ro,   // ����� �������
            rw,
            rwf}; // �������� ����� ����� ���� ��������� �� �����  
class FieldInfo{
   private:
      string File;            // ������ ������ ��������
      vector<string> fromFile;// �������� ����������� �� �����
      void setNewValue(string newVal);   // ��������� ����� ��������
   public:
      FieldInfo();
      string Name;            // ��� ��������
      string Description;     // �������� ����
      string Value();         // ���������� �������� � ���� ������
      void   Value(string newVal);   // ��������� ����� ��������
      string Type;            // ���
      string Value(const Float &time); // ���������� �������� �� ��������������� ������ ������� (����������� �� �����)
      bool update();				// �������� �������� ����
      rwMode Mode;            // ����� ������ ������ {ro, rw}
      void  *ptr;             // ��������� �� ��������
      virtual void Save(ostream &out);		// ���������� ���������� �������
      virtual void Read(istream &in);     // �������� ���������� ��������
};
//---------------------------------------------------------------------------
/* Power Object List - �������� ������ ���� ��������� �������� */
class ObjectList{
      vector<element*> Obj;
   public:
      ~ObjectList();
      void Add    (element* obj);
      bool Delete (element* obj);
      element* Find (string Name);             // ���������� ������ �� ����� ��� (���� ������ �� ������)NULL
      element* Find (int ID);						  // ����� �������� �� id
};
//---------------------------------------------------------------------------
#endif
