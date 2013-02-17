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

extern int num_f;	// число гармоник

using namespace std;

typedef complex<Float> Complex;

//---------------------------------------------------------------------------
// экспортируемые функции
class element;
class FieldInfo;
template<class T> class phase;
template<class T> class power;
template<class T> ostream& operator << (ostream& out, const phase<T>& p);
template<class T> istream& operator >> (istream& in, phase<T>& p);
void SaveObj(element *Obj, ostream &stream);               // сохранение объекта в поток
element* CreateObj(istream &stream);                       // создание объекта по данным из потока
element* CreateObj(String ClassName, vector<void*> Params);// создать модель по имени класса и переданным параметрам, которые передаются в конструктор модели
template<class T> T imag( const phase<complex<T> > &X);
template<class T> T abs( const phase<complex<T> > &X);
template<class T> T abs( const phase<complex<T>* > &X);
template<class T> T imag( const power<complex<T> > &X);
template<class T> T abs( const power<complex<T> > &X);
//---------------------------------------------------------------------------

template<class T>
class phase{
	public:
   	T A, B, C;			// значения параметра в фазах
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
      void Describe_Fields();                   // описание свойств элемента
      int _id;												// уникальный номер элемента
   public:
      element();
      ~element();
      int id();
      string  Name;
      virtual string ClassName();					// имя класса
      virtual bool accept(){return true;};      // применить свойства
      virtual bool update(){return false;};      // изменить параметры объекта к текущему моменту времени
      vector<FieldInfo>   Fields;
      FieldInfo* Field(void* Var);              // возвращает поле для указанной переменной
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
};

//---------------------------------------------------------------------------
/* узел */
class node : public element{
   private:
      typedef element sire;               // класс предка
      void Describe_Fields();             // описание свойств элемента
   protected:
      node();
   public:
      power<Complex> S;                   // S==действ. мощность трехфазная, a S.A == действ мощночть фазы, потребление в узле (+), генерация (-)     
      Dynamic_Array< phase<Complex*> > U; // напряжение в узле (комплекс фазного амплитудного значения)
      virtual string ClassName();			// имя класса
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
};

//---------------------------------------------------------------------------
/* узел бесконечной мощности */
class big_system : public node{
   private:
      typedef node sire;                  // класс предка
      void Describe_Fields();             // описание свойств элемента
   public:
      big_system();
      Dynamic_Array< Complex > Ustart;   	// напряжение которое будет на шинах после выполнения Make_Equation()
      virtual string ClassName();			// имя класса
      virtual bool update();              // изменить параметры объекта к текущему моменту времени
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
      virtual bool accept();
};

//---------------------------------------------------------------------------
/* генератор */
class source : public node{
   private:
      typedef node sire;                    		// класс предка
      void Describe_Fields();                   // описание свойств элемента
   public:
      source();
      Float X2;                                 // сопротивление обратной последовательности генератора                             
      Float Unom;                               // напряжение поддерживаемое на шинах (линейное дествующее)
      Float P;                                  // активная мощность отдаваемая станцией
      Float Qmax;                               // потолок реактивной мощности станции
      virtual string ClassName();					// имя класса
      virtual bool accept();                    // применить свойства объекта
      virtual bool update();                    // изменить параметры объекта к текущему моменту времени
      virtual void Save(ostream &out);				// сохранение параметров объекта
      virtual void Read(istream &in);     		// загрузка параметров элемента
};


//---------------------------------------------------------------------------
/* потребитель */
class consumer : public node{
   private:
      typedef node sire;                  // класс предка
      void Describe_Fields();             // описание свойств элемента
   public:
      consumer();
      virtual string ClassName();			// имя класса
      virtual bool update();              // изменить параметры объекта к текущему моменту времени
      virtual bool accept();
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
};


//---------------------------------------------------------------------------
/* нелинейный потребитель */
class rectifier : public node{
   private:
      typedef node sire;                  // класс предка
      void Describe_Fields();             // описание свойств элемента
   public:
      rectifier();
      Dynamic_Array< Complex >  I;        // ток высших гармоник (% от номинального) и начальная фаза (рад относ U(f)) в форме(ток, фаза)
      Dynamic_Array< phase<Complex> >  J; // текущий узловой ток высшей гармоники (комплекс амплитудного значения)
      virtual string ClassName();			// имя класса
      virtual bool update();              // изменить параметры объекта к текущему моменту времени
      virtual bool accept();
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
};

//---------------------------------------------------------------------------
 /* линия */
class link : public element{
   private:
      typedef element sire;               // класс предка
   	node* _From;
      node* _To;
      void Describe_Fields();             // описание свойств элемента
      void rise_wire_tempetarute();       // расчет пересчет температуры проводов фаз, учитывая нагрув за шаг времени 
      Float Pr_air(Float t);              // коэффициент Прандтля
      Float Ra_air(Float t_env, Float t_wire);// коэффициент Ралея
      Float V_air(Float t);               // вязкость воздуха
      Float Y_air(Float t);               // коэффициент теплопроводности воздуха
      void calc_k();                      // расчитывает коэфициенты длинных линий
      void calc_Z0_Y0();						// расчет сопротивлений и проводимостей 
      void calc_Z0();                		// расчитать матрицы сопротивлений
      void calc_Y0(Matrix<Float>* C0);		// расчитать матрицу проводимостей, принимает емкость в Ф/км
      Matrix<Float>* calc_P0();           // расчитывает коэф. Максвелла, км/Ф
      vector<Complex> d_sag;					// геометрическое место проводов с учетом стрелы провеса (начало координат должно совпадать с землей и осью симметрии опоры), провода ГТ имеют номера больше wire_num (число фаз на опоре)
      void calc_d_sag();
      template<class T> Matrix<T> * calc_to_phase( Matrix<T> * _Z ); // пересчет фазных сопротивлений, искл. уравнения ГТ
      template<class U> void cacl_transposition( Matrix<U> *X); // пересчет матрицы удельных параметров с учетом транспозиции
      Float K_wire_Length;                // коэф. увеличения длинны провода по сравнению с длинной ЛЭП 
   public:
      link(node* From, node* To);
      ~link();
      node* From();                       // где начинается
      node* To();                         // где заканчивается
      int const num_wire;						// число проводов (без учета грозотросов)
      vector< Matrix<Complex>* > Z0,Y0;	// удельные сопротивления и проводимости по частотам
      Dynamic_Array< phase<Complex*> > I1, I2; // токи в линии (комплекс амплитудного значения)
      vector< vector< Matrix<Complex> > > k;// коэфициенты телеграфных уравнений
      Float Length;                       // длинна проводов (учесть стрелу провеса)
      Float R0;                           // сопротивление при 20 град для фазных проводов
      Float R0_gt;                        // сопротивление при 20 град для проводов ГТ (грозотросса)
      Float R0t[3];                       // текущее значение сопротивления проводов фаз
      Float pe;									// удельное сопротивление земли  (Ом*м)
      Float gm0, gz0;     						// активные проводимости на 1 км
      vector<Complex> d;						// вектора до точек подвеса проводов (начало координат должно совпадать с землей и осью симметрии опоры)
      vector<Complex> d_gt;					// вектора до точек подвеса проводов грозотросов
      Float Sag, Sag_gt;                  // стрелы провеса
      Float spanLength;                   // длина пролета
      int phase_wire_num;						// число проводов в фазе
      int const gt_phase_wire_num;			// число проводов в ГТ
		int  num_gt_wire;							// число заземленных с обоих концов проводов грозотросов (ГТ)
      Float phase_radius;						// радиус окружности по которой расположены провода расщепленной фазы
      Float P_sun;                        // мощность излучения солнца, Вт/м^2
      Float q_sun;                        // угол наклона солнечных лучей к оси провода
      Float T_wire[3];                    // температура провода фазы
      Float T_env;                        // температура среды
      Float v, q;                         // скорость и угол атаки (рад) ветра
      Float k_F;                          // отношения площади попер сеч ал. к стал
      Float D;                            // диаметр провода фазы
      Float D_gt;									// диаметр провода ГТ
      Float SF, SF_gt;							// stranding factor - коэф.учитыв умешьшение эквивал радиуса провода (1 провода в расщю фазе)
      Float u_gt;									// магнитная проницаемость материала провода ГТ
      bool is_Transpositioned;			   // линия с транспозицией фаз ?
      Float m;                            // масса провода на 1 км
      Float e_blackness;                  // степень черноты провода (коэф. излучения)
      Float e_intake;                     // коэф. поглощения
      virtual string ClassName();			// имя класса
      bool update();                      //обновление параметров линии
      virtual bool accept();
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
};

//---------------------------------------------------------------------------
 /* сеть */
class grid : public element{
   private:
      typedef element sire;                    // класс предка
      bool Update_Equation(int i_freq); // обновить матрицу коэффициентов для указанной гармоники
      void Update_B(int i_freq);        // обновить только правые части уравнений
      bool update();             	// обновляем параметры системы
      Float eps;                 	// допустимая погрешность расчета
      bool bigger_than_eps(Matrix< complex<Float> > &dX ); // лежит ли погрешность в допустимых пределах?
      vector<node*> Nodes;          // узлы сети
      vector<link*> Links;          // связи сети
      int Node_Num(node* Node);     // получаем номер ноды по ее указателю или -1 если не существует
      Matrix< Complex > *A;			// матрица коэффициентов перед неизвестными в системе
      vector< Complex >  B; 			// правые части систем уравнений
      bool calc_non_nominal_freq;   // расчитывать ли не номинальную частоту (есть ли в сети источники высших гармоник)
   public:
      grid(vector<link*> Link);     // создаем сеть
      ~grid();
      bool Make_Equation();        	// если grid конечная рассчетная сеть, создаем уравнения для рассчета
      Matrix< Complex > *X; 			// Хранит токи и напряжения
      virtual string ClassName();   // имя класса
      void calc();                  // пересчет параметров
      element* find(string Name);   // поис объекта

};

//---------------------------------------------------------------------------
/* Класс содержит информацию о свойстве объекта, который могжет быть
отредактировн  */
enum rwMode{ro,   // режим доступа
            rw,
            rwf}; // значение ключа может быть прочитано из файла  
class FieldInfo{
   private:
      string File;            // откуда читаем значения
      vector<string> fromFile;// значения прочитанные из файла
      void setNewValue(string newVal);   // установим новое значение
   public:
      FieldInfo();
      string Name;            // имя свойства
      string Description;     // описание поля
      string Value();         // возвращаем значение в виде строки
      void   Value(string newVal);   // установим новое значение
      string Type;            // тип
      string Value(const Float &time); // возвращает значение на соответствующий момент времени (прочитанное из файла)
      bool update();				// обновить значение поля
      rwMode Mode;            // режим чтения записи {ro, rw}
      void  *ptr;             // указатель на свойство
      virtual void Save(ostream &out);		// сохранение параметров объекта
      virtual void Read(istream &in);     // загрузка параметров элемента
};
//---------------------------------------------------------------------------
/* Power Object List - содержит список всех созданных объектов */
class ObjectList{
      vector<element*> Obj;
   public:
      ~ObjectList();
      void Add    (element* obj);
      bool Delete (element* obj);
      element* Find (string Name);             // возвращает объект по имени или (если объект не найден)NULL
      element* Find (int ID);						  // поиск элемента по id
};
//---------------------------------------------------------------------------
#endif
