//---------------------------------------------------------------------------
#pragma hdrstop

#include "model.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


ObjectList Objects;		// содержит список всех созданных объектов
int num_f =  2;     		// число гармоник

//---------------------------------------------------------------------------

element::element(){
	static ID=0;
   _id = ID++;
   Name = "Устройство";
   Describe_Fields();
   Objects.Add(this);
}

//---------------------------------------------------------------------------

element::~element(){
	Objects.Delete(this);
}

//---------------------------------------------------------------------------
void element::Describe_Fields(){
   // Сохраняем информацию о свойствах класса
   FieldInfo fi;
   fi.Name = "Имя";
   fi.Type = "string";
   fi.ptr = (void*) &Name;
   Fields.push_back(fi);
}
//---------------------------------------------------------------------------

string element::ClassName(){return "element";}

//---------------------------------------------------------------------------

int element::id(){return _id;};

//---------------------------------------------------------------------------

FieldInfo* element::Field(void* Var){
   vector<FieldInfo>::iterator it = Fields.begin();
   vector<FieldInfo>::iterator end = Fields.end();
   while( it != end ){
      if( it->ptr == Var ) return it;
      it++;
      }
   return NULL;
}

//---------------------------------------------------------------------------

void element::Save(ostream &out){
   out << Name << " ";
   out << _id << " ";
   for(int i=0, n=Fields.size(); i<n; i++)
      Fields[i].Save(out);
}

//---------------------------------------------------------------------------

void element::Read(istream &in){
	in >> Name;
   in >> _id;
   for(int i=0, n=Fields.size(); i<n; i++)
      Fields[i].Read(in);
}

//---------------------------------------------------------------------------

node::node(){
   Name = "Узел";
   Describe_Fields();
}

//---------------------------------------------------------------------------
void node::Describe_Fields(){
}

//---------------------------------------------------------------------------

string node::ClassName(){return "node";}

//---------------------------------------------------------------------------

void node::Save(ostream &out){
   sire::Save(out);
   out << S << " ";
}

//---------------------------------------------------------------------------

void node::Read(istream &in){
	sire::Read(in);
   in >> S;
}

//---------------------------------------------------------------------------

big_system::big_system(){
   for(int i_freq=0; i_freq<num_f; i_freq++) Ustart.push_back( complex<Float>(0,0) );
   Ustart[1] = 115e3;
   Name = "Система";
   Describe_Fields();
}
//---------------------------------------------------------------------------
void big_system::Describe_Fields(){
   // Сохраняем информацию о свойствах класса
   FieldInfo fi;
   fi.Name = "Напряжение линейное";
   fi.Type = "Dynamic_Array<complex>";
   fi.ptr = (void*) &Ustart;
   Fields.push_back(fi);
}
//---------------------------------------------------------------------------

string big_system::ClassName(){return "big_system";}

//---------------------------------------------------------------------------

bool big_system::accept(){
	U.resize(num_f);
	Ustart.resize(num_f);
   return true;
}

//---------------------------------------------------------------------------

bool big_system::update(){
   for(int i_freq=1; i_freq<num_f; i_freq++){
      for(int Phase=0; Phase<3; Phase++){
      	Float Amp = abs(*U[i_freq][Phase]);
      	Float Arg = (Amp!=0)?arg(*U[i_freq][Phase]):0;
      	Arg += i_freq*w*smp;
      	*U[i_freq][Phase] = polar<Float>(Amp,Arg);
         }
      }
   return true;
}

//---------------------------------------------------------------------------

void big_system::Save(ostream &out){
   sire::Save(out);
   out << Ustart;
   out << " ";
}

//---------------------------------------------------------------------------

void big_system::Read(istream &in){
	sire::Read(in);
   in >> Ustart;
}

//---------------------------------------------------------------------------

source::source(){
   X2 = 200;
   Unom = 110e3;
   P = 1e6;
   Qmax = 1e6;
   Name = "Станция";
   Describe_Fields();
}

//---------------------------------------------------------------------------
void source::Describe_Fields(){
   // Сохраняем информацию о свойствах класса
   FieldInfo fi;
   fi.Name = "Сопр.обр.посл";
   fi.Type = "Float";
   fi.ptr = (void*) &X2;
   Fields.push_back(fi);

   fi.Name = "Unom линейное";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &Unom;
   Fields.push_back(fi);

   fi.Name = "Акт.мощн";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &P;
   Fields.push_back(fi);

   fi.Name = "Maкс.реак.мощн";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &Qmax;
   Fields.push_back(fi);
}

//---------------------------------------------------------------------------

string source::ClassName(){return "source";}
//---------------------------------------------------------------------------

bool source::accept(){
	U.resize(num_f);
   S = - complex<Float>(P,0);
   return true;
}

//---------------------------------------------------------------------------

bool source::update(){

   if(Field(&P)->update())
      S = - complex<Float>(P,imag(S));

   Field(&Unom)->update();
   // увеличиваем напряжение на шинах за счет увеличения
   // отдаваемой реактивной мощности
   if(abs(U[1]) < sqrt((Float)2/3)*Unom){
      // скорость поъема реак.мощности (шаг поъема)
      Float dQ = 1e7; // ВАр/сек
      Float Q = imag(S) - dQ*smp;
      if( -Q <= Qmax)  S = complex<Float>(-P,Q);
      }
   else{
      // скорость поъема реак.мощности (шаг поъема)
      Float dQ = 1e7; // ВАр/сек
      Float Q = imag(S) + dQ*smp;
      if( -Q>=0 )	S = complex<Float>(-P,Q);
      }
   return true;
}

//---------------------------------------------------------------------------

void source::Save(ostream &out){
   sire::Save(out);
   out << complex<Float>(0,X2) << " ";
   out << Unom << " ";
   out << P << " ";
   out << Qmax << " ";
}

//---------------------------------------------------------------------------

void source::Read(istream &in){
	sire::Read(in);
   complex<Float> Z2;
   in >> Z2;   X2 = imag(Z2);
   in >> Unom;
   in >> P;
   in >> Qmax;
}

//---------------------------------------------------------------------------

consumer::consumer(){
   Name = "Потребитель";
   Describe_Fields();
}

//---------------------------------------------------------------------------
void consumer::Describe_Fields(){
   // Сохраняем информацию о свойствах класса
   FieldInfo fi;
   fi.Name = "Мощность";
   fi.Type = "power";
   fi.Mode = rwf;
   fi.ptr = (void*) &S;
   Fields.push_back(fi);
}

//---------------------------------------------------------------------------

string consumer::ClassName(){return "consumer";}

//---------------------------------------------------------------------------

bool consumer::accept(){
	U.resize(num_f);
   return true;
}

//---------------------------------------------------------------------------

bool consumer::update(){
   return Field(&S)->update();
}

//---------------------------------------------------------------------------

void consumer::Save(ostream &out){
   sire::Save(out);
}

//---------------------------------------------------------------------------

void consumer::Read(istream &in){
	sire::Read(in);
}

//---------------------------------------------------------------------------

rectifier::rectifier(){
   I.resize(num_f);
   Name = "Нелинейный потребитель";
   Describe_Fields();
}

//---------------------------------------------------------------------------
void rectifier::Describe_Fields(){
   // Сохраняем информацию о свойствах класса
   FieldInfo fi;
   fi.Name = "Мощность";
   fi.Type = "power";
   fi.Mode = rwf;
   fi.ptr = (void*) &S;
   Fields.push_back(fi);

   fi.Name = "Гарм.тока.(% от Inom, рад.ф.А при t0)";
   fi.Type = "Dynamic_Array<complex>";
   fi.Mode = rw;
   fi.ptr = (void*) &I;
   Fields.push_back(fi);
}

//---------------------------------------------------------------------------

string rectifier::ClassName(){return "rectifier";}

//---------------------------------------------------------------------------

bool rectifier::accept(){
	U.resize(num_f);
   I.resize(num_f);
   J.resize(num_f);
   I[1] = Complex(100,0);
   return true;
}
//---------------------------------------------------------------------------

bool rectifier::update(){

   Field(&S)->update();

   for(int i_freq=0; i_freq<num_f; i_freq++)
      if(i_freq!=1){
         //Float Arg = (norm(J[i_freq].A)!=0)?arg(J[i_freq].A):0;  <- вносит большую погрешность. из-за чего график получается скачкообразным, пришолсь отказаться
         for(int Phase=0;Phase<3;Phase++){
         	Float Uav =  abs(*(U[1][Phase]));
         	Float Amp = (Uav<MIN)? 0 : (Float)0.01 * real(I[i_freq]) * ( (Float)2 * abs(S[Phase]) / Uav ) ;
            Float Arg = i_freq*w*TIME;
            Arg += imag(I[i_freq]);
            // ток отдаем поэтому минус
            J[i_freq][Phase] = - polar<Float>(Amp,Arg - Phase*2*M_PI/3);
            }
         }
      else
         J[i_freq].A = J[i_freq].B = J[i_freq].C =0;

   return true;
}

//---------------------------------------------------------------------------

void rectifier::Save(ostream &out){
   sire::Save(out);
   out << I << " ";
}

//---------------------------------------------------------------------------

void rectifier::Read(istream &in){
	sire::Read(in);
   in >> I;
}

//---------------------------------------------------------------------------

link::link(node* From, node* To):
	_From(From),
   _To(To),
   num_wire(3),
   gt_phase_wire_num(1)
{
   /*for(int i_freq=0; i_freq<num_f; i_freq++){
     I1.push_back( NULL );
     I2.push_back( NULL );
     } */
   Length = 100;
   R0t[0] = R0t[1] = R0t[2] = R0 = 0.422;
   gz0 = 1e-6;
   gm0 = 0;
   P_sun = v = 0;
   q = M_PI/2;
   q_sun = M_PI/2;
   T_wire[0] = T_wire[1] = T_wire[2] = T_env = 20;
   e_blackness = 0.5; // [0.11-0.8]
   e_intake = 0.6;
   k_F = 6;
   D = 11.4e-3;
   m = 276;
   D_gt = 11e-3;
   R0_gt = 1.74;
   pe = 100;
   spanLength = 100;
   Sag = 4;
   Sag_gt = 4;
   phase_wire_num = 1;
   phase_radius = 0.15;
   SF = 0.73;
   SF_gt = 0.7;
   u_gt = 1000;
   num_gt_wire = 1;
   is_Transpositioned = true;
   d.resize(num_wire);
   d[0] = Complex(1.75, 17.5);
   d[1] = Complex(3.3, 14.5);
   d[2] = Complex(-1.75, 14.5);
   d_gt.resize(num_gt_wire);
   if(num_gt_wire>0) d_gt[0] = Complex(0, 18.6);
   K_wire_Length = 1 + Sag*Sag*8/3/spanLength;

   Name = "Линия";
   Describe_Fields();
}

link::~link(){
   for(int i=0, n=Z0.size(); i<n; i++)
   	{delete Z0[i]; Z0[i] = NULL;}
   for(int i=0, n=Y0.size(); i<n; i++)
   	{delete Y0[i]; Y0[i] = NULL;}
}

//---------------------------------------------------------------------------
void link::Describe_Fields(){
   // Сохраняем информацию о свойствах класса
   FieldInfo fi;
   fi.Name = "Длинна";
   fi.Type = "Float";
   fi.ptr = (void*) &Length;
   Fields.push_back(fi);

   fi.Name = "R0";
   fi.Type = "Float";
   fi.ptr = (void*) &R0;
   Fields.push_back(fi);

   fi.Name = "R0 для ГТ";
   fi.Type = "Float";
   fi.ptr = (void*) &R0_gt;
   Fields.push_back(fi);

   fi.Name = "Уд.сопр.земли,Ом*м";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &pe;
   Fields.push_back(fi);

   fi.Name = "gz0,См";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &gz0;
   Fields.push_back(fi);

   fi.Name = "gm0,См";
   fi.Type = "Float";
   fi.ptr = (void*) &gm0;
   Fields.push_back(fi);

   fi.Name = "Длина пролета,м.";
   fi.Type = "Float";
   fi.ptr = (void*) &spanLength;
   Fields.push_back(fi);

   fi.Name = "Стрела пров,м.";
   fi.Type = "Float";
   fi.ptr = (void*) &Sag;
   Fields.push_back(fi);

   fi.Name = "Стрела пров.ГТ,м";
   fi.Type = "Float";
   fi.ptr = (void*) &Sag_gt;
   Fields.push_back(fi);

   fi.Name = "Пров.в фазе";
   fi.Type = "int";
   fi.ptr = (void*) &phase_wire_num;
   Fields.push_back(fi);

   fi.Name = "Радиус расщепления,м";
   fi.Type = "Float";
   fi.ptr = (void*) &phase_radius;
   Fields.push_back(fi);

   fi.Name = "F_Al/F_ст";
   fi.Type = "Float";
   fi.ptr = (void*) &k_F;
   Fields.push_back(fi);

   fi.Name = "Диам.пров, м";
   fi.Type = "Float";
   fi.ptr = (void*) &D;
   Fields.push_back(fi);

   fi.Name = "Диам.пров.ГТ, м";
   fi.Type = "Float";
   fi.ptr = (void*) &D_gt;
   Fields.push_back(fi);

   fi.Name = "Stranding Factor";
   fi.Type = "Float";
   fi.ptr = (void*) &SF;
   Fields.push_back(fi);

   fi.Name = "Stranding Factor для ГТ";
   fi.Type = "Float";
   fi.ptr = (void*) &SF_gt;
   Fields.push_back(fi);

   fi.Name = "Магн.прониц.для пров.ГТ";
   fi.Type = "Float";
   fi.ptr = (void*) &u_gt;
   Fields.push_back(fi);

   fi.Name = "Провода,кординаты подвеса на опоре,м";
   fi.Type = "vector<complex>";
   fi.ptr = (void*) &d;
   Fields.push_back(fi);

   fi.Name = "Число ГТ";
   fi.Type = "int";
   fi.ptr = (void*) &num_gt_wire;
   Fields.push_back(fi);

   fi.Name = "ГТ кординаты подвеса на опоре,м";
   fi.Type = "vector<complex>";
   fi.ptr = (void*) &d_gt;
   Fields.push_back(fi);

   fi.Name = "Налич.транспонир";
   fi.Type = "bool";
   fi.ptr = (void*) &is_Transpositioned;
   Fields.push_back(fi);

   fi.Name = "Масса.пров,кг/км";
   fi.Type = "Float";
   fi.ptr = (void*) &m;
   Fields.push_back(fi);

   fi.Name = "Темп.возд";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &T_env;
   Fields.push_back(fi);

   fi.Name = "Темп.пров A";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &T_wire[0];
   Fields.push_back(fi);

   fi.Name = "Темп.пров B";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &T_wire[1];
   Fields.push_back(fi);

   fi.Name = "Темп.пров C";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &T_wire[2];
   Fields.push_back(fi);

   fi.Name = "Солн.изл, Вт/м^2";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &P_sun;
   Fields.push_back(fi);

   fi.Name = "Наклон солн,рад";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &q_sun;
   Fields.push_back(fi);

   fi.Name = "Ск.ветра,м/с";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &v;
   Fields.push_back(fi);

   fi.Name = "Атака.ветра,рад";
   fi.Type = "Float";
   fi.Mode = rwf;
   fi.ptr = (void*) &q;
   Fields.push_back(fi);

   fi.Name = "Коэф.погл";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &e_intake;
   Fields.push_back(fi);

   fi.Name = "Коэф.излуч";
   fi.Type = "Float";
   fi.Mode = rw;
   fi.ptr = (void*) &e_blackness;
   Fields.push_back(fi);
}

//---------------------------------------------------------------------------

bool link::accept(){
   I1.resize(num_f);
   I2.resize(num_f);
   d.resize(num_wire);
   d_gt.resize(num_gt_wire);
   K_wire_Length = 1 + Sag*Sag*8/3/spanLength;
   calc_k();
   return true;
}

//---------------------------------------------------------------------------

bool link::update(){

   Field(&T_env)->update();
   Field(&P_sun)->update();
   Field(&q_sun)->update();
   Field(&v)->update();
   Field(&q)->update();
   Field(&pe)->update();

   rise_wire_tempetarute();
   // еще нужно расчитывать сопротивление грозотроса в зависимости от температуры. А нужно ли?
   calc_k();
   return true;   
}

//---------------------------------------------------------------------------

void link::calc_k(){
	calc_Z0_Y0();
   k.clear();
   for(int i=0; i<num_f; i++){
      /*
      // вместо этого нужно сдвигать по фазам на заданные углы
      // токи в зависимости от кратности частоты
      complex<Float> a, b;
      if( i%3 != 0 ){
          a =  complex<Float>(R0t, 2*M_PI*i*freq*(L0-M0));
          b =  complex<Float>(g0, 2*M_PI*i*freq*C0);}
      else{
          a =  complex<Float>(R0t, 2*M_PI*i*freq*(L0+2*M0));
          b =  complex<Float>(gz0, 2*M_PI*i*freq*Cz0);} */

      Matrix<Complex> y = sqrt( (*Z0[i]) * (*Y0[i]) ) ;
      Matrix<Complex> Zv = (*Z0[i]) * y.inverse() ;

      vector< Matrix<Complex> > _k;
      Matrix<Complex> yl = K_wire_Length * Length * y ;
      Matrix<Complex> exp_sum = 0.5*(exp(yl) + exp(-yl));
      Matrix<Complex> exp_sub = 0.5*(exp(yl) - exp(-yl));
      Matrix<Complex> Zv_1 = Zv.inverse();
      Matrix<Complex> E(num_wire,num_wire);
      for(int i=0; i<num_wire; i++) E[i][i] = 1;
      Matrix<Complex> N(num_wire,num_wire);

      _k.push_back( N );             // перед I1
      _k.push_back( exp_sub * Zv );  // перед I2
      _k.push_back( -E );            // перед U1
      _k.push_back( exp_sum );		 // перед U2
      _k.push_back( E );             // перед I1 // ток направлен в сторону узла, тоэтому +1
      _k.push_back( Zv_1 * exp_sum * Zv); // перед I2
      _k.push_back( N );				 // перед U1
      _k.push_back( Zv_1 * exp_sub );// перед U2
      k.push_back(_k);
      }
}

//---------------------------------------------------------------------------

void link::rise_wire_tempetarute(){
   Float p_Al = 2689;   // плотность
   Float p_ct = 7800;
   Float c_Al = 930; // Дж/кг/K
   Float c_ct = 500;

  for(int i=0; i<num_wire; i++){   
   Float Nu = 0;
   if(v!=0){
      Float Re = v*D/V_air(T_env);
      Float Pr = Pr_air(T_env);
      Float e_q = sqrt(sin(q));
      Float e_t = pow(Pr/Pr_air(T_wire[i])  ,(Float)0.25);
      if(1<Re && Re<40)          Nu = 0.76 * pow(Re,(Float)0.4) * pow(Pr,(Float)0.37) * e_t * e_q;
      else if(40<=Re && Re<1e3)  Nu = 0.52 * pow(Re,(Float)0.5) * pow(Pr,(Float)0.37) * e_t * e_q;
      else if(1e3<=Re && Re<2e5) Nu = 0.26 * pow(Re,(Float)0.6) * pow(Pr,(Float)0.37) * e_t * e_q;
      else if(2e5<=Re && Re<1e7) Nu = 0.023* pow(Re,(Float)0.8) * pow(Pr,(Float)0.40) * e_t * e_q;
   }else{
      Float C=0, n=0;
      Float Ra = Ra_air(T_env, T_wire[i]);
      if(Ra<1e-3)                   {C=0.5; n=0;}
      else if( 1e-3<=Ra && Ra<500)  {C=1.18; n=0.125;}
      else if( 500<=Ra && Ra<2e7)   {C=0.54; n=0.25;}
      else if( 2e7<=Ra )            {C=0.135; n=1/3.0;}
      Nu = C * pow(Ra,n);
      }
   if(Nu==0){err(_ERROR_,"link: не удалось расчитать коэффициент Nu");}
   Float K_k = Nu*Y_air(T_env)/D; // коэффиициент конвекции

   // коэффициент лучистого теплообмена
   Float K_l = (fabs(T_wire[i]-T_env)<0.1)?5.67*e_blackness:5.67*e_blackness/(T_wire[i]-T_env) * ( pow((273+T_wire[i])/100, 4) - pow((273+T_env)/100, 4) );

   // коэф. теплоотдачи
   Float K = K_k + K_l;

   Float dP_sun = e_intake * D * 1000 * sin(q_sun) * P_sun; // на 1 км

   Float i1 = 0, i2 = 0, i_max;
   for(int i_freq=0; i_freq < num_f; i_freq++){
      i1 += imag(*I1[i_freq][i]);
      i2 += imag(*I2[i_freq][i]);
      }
   i_max = (i1>i2)?i1:i2;
   Float dP_elec = pow(i_max,2) * R0t[i];

   Float dP = dP_sun + dP_elec;   // на один км

   Float S_ohl = M_PI*D*1000;

   Float m_Al = m/(1 + p_ct/p_Al/k_F);
   Float cm = m_Al*c_Al + (m-m_Al)*c_ct;

   Float dT = ( dP - K*S_ohl*(T_wire[i]-T_env) ) * smp / cm;
   T_wire[i] += dT;
   }
}

//---------------------------------------------------------------------------

Float link::V_air(Float t){
   Float T[28] = {-50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 500, 600, 700};
   Float V[28] = {9.23e-6, 10.04e-6, 10.8e-6, 12.79e-6, 12.43e-6, 13.28e-6, 14.16e-6, 15.06e-6, 16e-6, 16.96e-6, 17.95e-6, 18.97e-6, 20.02e-6, 21.09e-6, 22.1e-6, 23.13e-6, 25.45e-6, 27.8e-6, 30.09e-6, 32.49e-6, 34.85e-6, 40.61e-6, 48.33e-6, 55.46e-6, 63.09e-6, 79.38e-6, 96.89e-6, 115.4e-6};
   int i;
   for(i=0; i<28; i++) if(t<T[i]) break;
   if(i==0) {err(_ERROR_, "link: не возможно определить коэффициент Re для данной температуры");return V[0];}
   if(i>=28){err(_ERROR_, "link: не возможно определить коэффициент Re для данной температуры");return V[27];}
   Float k = (V[i]-V[i-1])/(T[i]-T[i-1]);
   Float c = V[i] - k*T[i];
   return k*t+c;
}

//---------------------------------------------------------------------------

Float link::Y_air(Float t){
   Float T[28] = {-50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 500, 600, 700};
   Float Y[28] = {2.04e-2, 2.12e-2, 2.2e-2, 2.28e-2, 2.36e-2, 2.44e-2, 2.51e-2, 2.59e-2, 2.67e-2, 2.76e-2, 2.83e-2, 2.9e-2, 2.96e-2, 3.05e-2, 3.13e-2, 3.21e-2, 3.34e-2, 3.49e-2, 3.64e-2, 3.78e-2, 3.93e-2, 4.27e-2, 4.6e-2, 4.91e-2, 5.21e-2, 5.74e-2, 6.22e-2, 6.71e-2};
   int i;
   for(i=0; i<28; i++) if(t<T[i]) break;
   if(i==0) {err(_ERROR_, "link: не возможно определить коэффициент Re для данной температуры");return Y[0];}
   if(i>=28){err(_ERROR_, "link: не возможно определить коэффициент Re для данной температуры");return Y[27];}
   Float k = (Y[i]-Y[i-1])/(T[i]-T[i-1]);
   Float c = Y[i] - k*T[i];
   return k*t+c;
}

//---------------------------------------------------------------------------

Float link::Pr_air(Float t){
   Float T[28] = {-50, -40, -30, -20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 500, 600, 700};
   Float pr[28]= {0.724990196, 0.726301887, 0.722913636, 0.716921053, 0.713995763, 0.708442623, 0.704701195, 0.702335907, 0.70011236, 0.69548913, 0.696042403, 0.696568966, 0.702209459, 0.698029508, 0.693083067, 0.688383178, 0.688778443, 0.687911175, 0.684519231, 0.684037037, 0.678778626, 0.666070258, 0.675997826, 0.677242363, 0.67646833, 0.689313589, 0.700279743, 0.70704918 };
   int i;
   for(i=0; i<28; i++) if(t<T[i]) break;
   if(i==0) {err(_ERROR_, "link: не возможно определить коэффициент прандтля для данной температуры");return pr[0];}
   if(i>=28){err(_ERROR_, "link: не возможно определить коэффициент прандтля для данной температуры");return pr[27];}
   Float k = (pr[i]-pr[i-1])/(T[i]-T[i-1]);
   Float c = pr[i] - k*T[i];
   return k*t+c;
}

//---------------------------------------------------------------------------

Float link::Ra_air(Float t_env, Float t_wire){
   Float Pr = Pr_air( (t_wire+t_env)/2 );
   Float Gr = 9.8*3.665e-3*fabs(t_wire+t_env)*pow(D,(Float)3)/V_air(t_env);
   return Pr*Gr;
}

//---------------------------------------------------------------------------

node* link::From(){return _From;}
node* link::To(){return _To;}
string link::ClassName(){return "link";}

//---------------------------------------------------------------------------

void link::Save(ostream &out){
   sire::Save(out);
   out << Length << " ";
   out << R0 << " ";
   for(int i=0; i<num_wire;i++)  out << R0t[i] << " ";
   out << R0_gt << " ";
   out << gz0 << " ";
   out << gm0 << " ";
   out << P_sun << " ";
   out << v << " ";
   out << q << " ";
   out << q_sun << " ";
	out << T_env << " ";
   for(int i=0; i<num_wire;i++) out << T_wire[i] << " ";
   out << e_blackness << " ";
   out << e_intake << " ";
   out << k_F << " ";
   out << D << " ";
   out << D_gt << " ";
   out << m << " ";
   out << pe << " ";
   out << spanLength << " ";
   out << Sag << " ";
   out << Sag_gt << " ";
   out << phase_wire_num << " ";
   out << SF << " ";
   out << u_gt << " ";
   out << num_gt_wire << " ";
   out << is_Transpositioned << " ";
   for(int i=0, n=d.size(); i<n; i++) out << d[i] << " ";
   for(int i=0, n=d_gt.size(); i<n; i++) out << d_gt[i] << " ";
   out << _From->id() << " ";
   out << _To->id() << " "; 
}

//---------------------------------------------------------------------------

void link::Read(istream &in){
   sire::Read(in);
   in >>  Length ;
   in >>  R0 ;
   for(int i=0; i<num_wire;i++) in >>  R0t[i] ;
   in >>  R0_gt ;
   in >>  gz0 ;
   in >>  gm0 ;
   in >>  P_sun ;
   in >>  v ;
   in >>  q ;
   in >>  q_sun ;
	in >>  T_env ;
   for(int i=0; i<num_wire;i++) in >>  T_wire[i] ;
   in >>  e_blackness ;
   in >>  e_intake ;
   in >>  k_F ;
   in >>  D ;
   in >>  D_gt ;
   in >>  m ;
   in >>  pe ;
   in >> spanLength;
   in >>  Sag ;
   in >>  Sag_gt ;
   in >>  phase_wire_num ;
   in >>  SF ;
   in >>  u_gt ;
   in >>  num_gt_wire ;
   in >>  is_Transpositioned ;
   d.clear(); 	  for(int i=0; i<num_wire; i++) {Complex tmp; in >> tmp; d.push_back(tmp) ;}
   d_gt.clear(); for(int i=0; i<num_gt_wire; i++) {Complex tmp; in >> tmp; d_gt.push_back(tmp) ;}
	string from, to;
   in >> from >> to;
   _From = dynamic_cast<node*>(Objects.Find( fromStr<int>(from) ));
   _To = dynamic_cast<node*>(Objects.Find( fromStr<int>(to) ));
   if( !(_From && _To) ) err(0, "link: не найден объект начала или конца линии");
}

//---------------------------------------------------------------------------

void link::calc_Z0_Y0()
{
   for(int i=0; i<num_wire; i++)
   	R0t[i] = R0*(1+0.00403*(T_wire[i]-20));
   //link * l = this;
	// расчитываем высоту проводов
   calc_d_sag();

   // расчитываем сопротивления и коэф. максвела с учетом сопротивл земли для фазных
   // проводов и ГТ
   calc_Z0();
   // перерасчитаем фазные сопротивл и коэф. максвела, исключив уравнения для ГТ
   for(int i=0; i<num_f;i++){
   	Matrix<Complex> * phase = calc_to_phase( Z0[i] );
      delete Z0[i]; Z0[i] = NULL;
      Z0[i] = phase;
   	}
   // изменяем сопротивления с учетом транспозиции
   if(is_Transpositioned)
   	for(int i=0; i<num_f;i++)
   		cacl_transposition( Z0[i] );
   // расчитываем коэф. максвела с учетом сопротивл земли для фазных
   // проводов и ГТ
   Matrix<Float>* P0 = calc_P0();
   // перерасчитаем коэф. максвела, исключив уравнения для ГТ
   Matrix<Float> * ph = calc_to_phase( P0 );
   delete P0; P0 = NULL;
   // высчитываем емкости
   Matrix<Float> C0 = ph->inverse();
   delete ph; ph = NULL;
   // изменяем емкости с учетом транспозиции
   if(is_Transpositioned)
   	cacl_transposition( &C0 );
   // расчитываем проводимости
   calc_Y0(&C0);
}

//---------------------------------------------------------------------------

void link::calc_d_sag()
{
   d_sag.clear();
   for(int i=0, n=d.size(); i<n; i++){
      Float H = imag(d[i]);
      Float tmp = sqrt(Sag*(2*H-Sag));
      Float Im = tmp / log((H+tmp)/(H-Sag));
   	d_sag.push_back( Complex( real(d[i]), Im) );
      }
   for(int i=0, n=d_gt.size(); i<n; i++){
      Float H = imag(d_gt[i]);
      Float tmp = sqrt(Sag_gt*(2*H-Sag_gt));
      Float Im = tmp / log((H+tmp)/(H-Sag_gt));
   	d_sag.push_back(  Complex( real(d_gt[i]), Im) );
      }
}

//---------------------------------------------------------------------------

void link::calc_Z0(){
	// проверить abs(comp-comp) - не явл ли отриц
   for(int i=0, n=Z0.size(); i<n; i++)
   	{delete Z0[i]; Z0[i] = NULL;}

   Z0.resize(num_f);
   int num = num_wire + num_gt_wire;

  for(int n=0; n<num_f;n++){
   Z0[n] = new Matrix<Complex>(num,num);
   Matrix<Complex> *Z_f = Z0[n];

   Float f = freq * n;

   if((int)Z_f->Rows() != num || (int)Z_f->Rows()!= num )
      {err(_ERROR_, "link: число строк и столбцов матрицы Z_f не верны"); }

	// среднегеометрический радиус фазы
   Float GMRc = SF * 0.5 * D;
   Float GMReq;
   switch(phase_wire_num){
      case 1:	GMReq = GMRc; break;
      case 2:	GMReq = pow( phase_wire_num * GMRc * pow(phase_radius,phase_wire_num-1), (Float)1.0/phase_wire_num ); break;
      default:	err(_ERROR_, "line: число проводов в расщепленной фазе должно быть больше 0"); GMReq = GMRc; break;
   	}
   Float GMRc_gt = SF_gt * 0.5 * D_gt;
   Float GMReq_gt;
   switch(gt_phase_wire_num){
      case 1:	GMReq_gt = GMRc_gt; break;
      default:	err(_ERROR_, "line: число проводов в расщепленном ГТ должно быть равно 1"); GMReq_gt = GMRc_gt; break;
   	}
   // глубина залегания эквивалентного провода "земля"
   Float Derc = (f>1e-3)?658.87 * sqrt(pe/f) : MAX;

   // расчет матрица сопротивлений
   Float Re, Im;
	for(int i=0; i<num; i++)
   	for(int j=0; j<num; j++)
      	if(i==j){
         	if(i<num_wire){ // провод фазы
            	Im = 4*M_PI*1e-4*f* (0.25/phase_wire_num + log(Derc/GMReq) ) + f/(911.812*Derc/imag(d_sag[i])/2.0);
            	Re = R0t[i]/phase_wire_num + pow(M_PI,2)*1e-4*f - f/(911.812*Derc/imag(d_sag[i])/2.0);
            }else{ // провод ГТ
            	Im = 4*M_PI*1e-4*f* (u_gt*0.25/gt_phase_wire_num + log(Derc/GMReq_gt) ) + f/(911.812*Derc/imag(d_sag[i])/2.0);
            	Re = R0_gt/gt_phase_wire_num + pow(M_PI,2)*1e-4*f  - f/(911.812*Derc/imag(d_sag[i])/2.0);
               }
            (*Z_f)[i][j] = Complex(Re, Im);
         }else{
            Re = pow(M_PI,2)*1e-4*f - f/(911.812*Derc/(imag(d_sag[i])+imag(d_sag[j])) );
            Im = 4*M_PI*1e-4*f*log(Derc/abs(d_sag[i]-d_sag[j]) ) + f/(911.812*Derc/(imag(d_sag[i])+imag(d_sag[j])));
            (*Z_f)[i][j] = Complex(Re, Im);
         	}
	}
}

//---------------------------------------------------------------------------

void link::calc_Y0(Matrix<Float>* C0)
{
	for(int i=0, n=Y0.size(); i<n; i++)
   	{delete Y0[i]; Y0[i] = NULL;}
   Y0.resize(num_f);

  	for(int n=0; n<num_f; n++){
   	Y0[n] = new Matrix<Complex>(C0->Rows(),C0->Cols());
   	Matrix<Complex> *Y_f = Y0[n];

   	Float f = freq * n;

   	// учитываем активные проводимости
 		for(int i=0; i<num_wire; i++)
   		for(int j=0; j<num_wire; j++)
      		if(i==j)	(*Y_f)[i][j] = Complex(gz0, 2*M_PI*f*(*C0)[i][j]);
         	else     (*Y_f)[i][j] = Complex(gm0, 2*M_PI*f*(*C0)[i][j]);
   	}
}

//---------------------------------------------------------------------------

Matrix<Float>* link::calc_P0()
{
   int num = num_wire + num_gt_wire;
   Matrix<Float>* P0 = new Matrix<Float>(num,num);

	// среднегеометрический радиус фазы
   Float GMRc = 0.5 * D;       // да-да SF не учитываем!
   Float GMReq;
   switch(phase_wire_num){
      case 1:	GMReq = GMRc; break;
      case 2:	GMReq = pow( phase_wire_num * GMRc * pow(phase_radius,phase_wire_num-1), (Float)1.0/phase_wire_num ); break;
      default:	err(_ERROR_, "line: число проводов в расщепленной фазе должно быть больше 0"); GMReq = GMRc; break;
   	}
   Float GMRc_gt = 0.5 * D_gt;    // да-да SF_gt не учитываем!
   Float GMReq_gt;
   switch(gt_phase_wire_num){
      case 1:	GMReq_gt = GMRc_gt; break;
      default:	err(_ERROR_, "line: число проводов в расщепленном ГТ должно быть равно 1"); GMReq_gt = GMRc_gt; break;
   	}

   // расчет матрицы проводимостей
   Float Re, Im;
	for(int i=0; i<num; i++)
   	for(int j=0; j<num; j++)
      	if(i==j){
         	if(i<3) // провод фазы
            	(*P0)[i][j] = 17.975109 * log(2*imag(d_sag[i])/GMReq) * 1e6; // км/Ф
            else // провод ГТ
            	(*P0)[i][j] = 17.975109 * log(2*imag(d_sag[i])/GMReq_gt) * 1e6;
         }else
            (*P0)[i][j] = 17.975109 * log ( abs(d_sag[i]-conj(d_sag[j])) / abs(d_sag[i]-d_sag[j]) ) * 1e6;
   return P0;
}

//---------------------------------------------------------------------------
template<class T>
Matrix<T> * link::calc_to_phase( Matrix<T> * _Z )
{
	Matrix<T> * Zph = new Matrix<T>(num_wire, num_wire);
   // заземленных ГТ нет
   if( (int)_Z->Rows() == num_wire && (int)_Z->Cols() == num_wire ){
      *Zph = *_Z;
      return Zph;
   	}

   Matrix<T> * Z_aa = new Matrix<T>(num_wire, num_wire);
   Matrix<T> * Z_as = new Matrix<T>(num_wire, num_gt_wire);
   Matrix<T> * Z_ss = new Matrix<T>(num_gt_wire, num_gt_wire);

   for(int i=0; i<num_wire; i++)
   	for(int j=0; j<num_wire; j++)
      	(*Z_aa)[i][j] = (*_Z)[i][j];

   for(int i=0; i<num_wire; i++)
   	for(int j=0; j<num_gt_wire; j++)
      	(*Z_as)[i][j] = (*_Z)[i][j+num_wire];

   for(int i=0; i<num_gt_wire; i++)
   	for(int j=0; j<num_gt_wire; j++)
      	(*Z_ss)[i][j] = (*_Z)[i+num_wire][j+num_wire];

   *Zph = (*Z_aa) - (*Z_as) * Z_ss->inverse() * Z_as->transpose();

   delete Z_aa;	Z_aa = NULL;
   delete Z_as;   Z_as = NULL;
   delete Z_ss;   Z_ss = NULL;

   return Zph;
}

/*/---------------------------------------------------------------------------

Matrix<Float> * link::calc_to_phase( Matrix<Float> * _P )
{
	Matrix<Float> * Pph = new Matrix<Float>(num_wire, num_wire);
   // заземленных ГТ нет
   if( (int)_P->Rows() == num_wire && (int)_P->Cols() == num_wire ){
      *Pph = *_P;
      return Pph;
   	}

   Matrix<Float> * P_aa = new Matrix<Float>(num_wire, num_wire);
   Matrix<Float> * P_as = new Matrix<Float>(num_wire, num_gt_wire);
   Matrix<Float> * P_ss = new Matrix<Float>(num_gt_wire, num_gt_wire);

   for(int i=0; i<num_wire; i++)
   	for(int j=0; j<num_wire; j++)
      	(*P_aa)[i][j] = (*_P)[i][j];

   for(int i=0; i<num_wire; i++)
   	for(int j=0; j<num_gt_wire; j++)
      	(*P_as)[i][j] = (*_P)[i][j+num_wire];

   for(int i=0; i<num_gt_wire; i++)
   	for(int j=0; j<num_gt_wire; j++)
      	(*P_ss)[i][j] = (*_P)[i+num_wire][j+num_wire];

   *Pph = (*P_aa) - (*P_as) * P_ss->inverse() * P_as->transpose();

   delete P_aa;	P_aa = NULL;
   delete P_as;   P_as = NULL;
   delete P_ss;   P_ss = NULL;

   return Pph;
}*/

//---------------------------------------------------------------------------

template<class U> void link::cacl_transposition(Matrix<U> *X)
{
   if(!is_Transpositioned) return;
	U Self   = ((*X)[0][0] + (*X)[1][1] + (*X)[2][2]) / (Float)3.0;
   U Mutual = ((*X)[1][0] + (*X)[2][0] + (*X)[2][1]) / (Float)3.0;;
   for(int i=0; i<num_wire; i++)
   	for(int j=0; j<num_wire; j++)
      	if(i==j) (*X)[i][j] = Self;
         else     (*X)[i][j] = Mutual;
}

//---------------------------------------------------------------------------

string grid::ClassName(){return "grid";}
//---------------------------------------------------------------------------
// создаем сеть
grid::grid(vector<link*> Link){
	A = NULL;
   X = NULL;
   eps = 1e-6;

   if(Link.size() <=0 ){
      err(0, (std::string)"grid: не возможно собрать сеть, т.к. не указано ни одной связи между элементами");
   	return;
   	}

   // подготовка данных для сборки сети
   Links = Link;
   Nodes.clear();
   for(unsigned i=0;i<Link.size();i++) {
      // первый объект
      unsigned j, n=Nodes.size(); // уже добавили ?
      node *f = Link[i]->From();
      for(j=0;j<n;j++) if(f == Nodes[j]) break;
      if(j==n)   Nodes.push_back(f);
      // второй объект
      n=Nodes.size(); // уже добавили ?
      node *s = Link[i]->To();
      for(j=0;j<n;j++) if(s == Nodes[j]) break;
      if(j==n)  Nodes.push_back(s);
      }
   // есть ли узел бесконечной мощности?
   // есть ли в сети источники высших гармоник
   bool system_find = false;
   calc_non_nominal_freq = false;
   for(int i=0,n=Nodes.size(); i<n; i++){
      if(Nodes[i]->ClassName()=="big_system")
         system_find = true;
      if(Nodes[i]->ClassName()=="rectifier")
         calc_non_nominal_freq = true;
      }
   if(!system_find){
      err(0, "grid: Не найден узел с бесконечной мощностью");
      return;}

   //Make_Equation();
   // задаем имя
   Name = "Сеть";
}

//---------------------------------------------------------------------------

grid::~grid(){
   delete A; A = NULL;
   delete X; X = NULL;
}

//---------------------------------------------------------------------------

bool grid::Make_Equation(){

   // применяем свойства элементов
   vector<link*>::iterator Link = Links.begin(); 
   while( Link != Links.end() ){
      (*Link)->accept();
      Link++;
      }
   vector<node*>::iterator Node = Nodes.begin();
   while( Node != Nodes.end() ){
      (*Node)->accept();
      Node++;
      }

   //какова должна быть размерность системы
   // в строках матрицы уравнения, в столбцах неизвестные
   int num = 3*( 2*Links.size() + Nodes.size());
   // коэффициенты при неизвестных
   delete A; A = NULL;
   A = new Matrix<Complex>(num,num);
   // правая часть системы - задающие токи
   B = vector< Complex >(num, Complex(0,0));
   // токи и напряжения (решения системы) по числу гармоник num_f
   delete X; X = NULL;
   X = new Matrix<Complex>(num,num_f);
   // ссылаем переменные объектов на элементы матрицы результата
   int num_I = 3*(2*Links.size());

   for(int i=0, n=Nodes.size(); i<n; i++ )
      for(int g=0; g<num_f; g++){
         if(Nodes[i]->ClassName() == "big_system"){
         	Complex U_st = sqrt((Float)2/3)*((big_system*)Nodes[i])->Ustart[g];
         	(*X)[ num_I + 3*i     ][g] = U_st;
            (*X)[ num_I + 3*i + 1 ][g] = U_st * Rn;
            (*X)[ num_I + 3*i + 2 ][g] = U_st * Rp;
            }
         Nodes[i]->U[g].A = &( (*X)[ num_I + 3*i     ][g] );
         Nodes[i]->U[g].B = &( (*X)[ num_I + 3*i + 1 ][g] );
         Nodes[i]->U[g].C = &( (*X)[ num_I + 3*i + 2 ][g] );
      }
   for(int i=0, n=Links.size(); i<n; i++)
      for(int g=0; g<num_f; g++){
         Links[i]->I1[g].A = &( (*X)[ 2*3*i     ][g] );
         Links[i]->I1[g].B = &( (*X)[ 2*3*i + 1 ][g] );
         Links[i]->I1[g].C = &( (*X)[ 2*3*i + 2 ][g] );
         Links[i]->I2[g].A = &( (*X)[ 2*3*i + 3 ][g] );
         Links[i]->I2[g].B = &( (*X)[ 2*3*i + 4 ][g] );
         Links[i]->I2[g].C = &( (*X)[ 2*3*i + 5 ][g] );
         }
   return true;
}
//---------------------------------------------------------------------------

bool grid::Update_Equation(int i_freq){

   //int num = 3*(2*Links.size() + Nodes.size());
   A->set_to_zero();

   // уравнения по формулам длинных ЛЭП
   int num_I = 3*2*Links.size();   // число переменных-токов
   for(int i=0, n=Links.size(); i<n; i++){
      // первое уравнение длинной ЛЭП
      int from = 3*Node_Num(Links[i]->From());
      int to   = 3*Node_Num(Links[i]->To());
      unsigned row = 2*3*i;
      unsigned col = row;
      Links[i]->k[i_freq][0] . insert_to(A, row, col);     		// I1
      Links[i]->k[i_freq][1] . insert_to(A, row, col + 3); 		// I2
      Links[i]->k[i_freq][2] . insert_to(A, row, num_I + from);// U1
      Links[i]->k[i_freq][3] . insert_to(A, row, num_I + to);  // U2

      row += 3;
      Links[i]->k[i_freq][4] . insert_to(A, row, col);     		// I1
      Links[i]->k[i_freq][5] . insert_to(A, row, col + 3); 		// I2
      Links[i]->k[i_freq][6] . insert_to(A, row, num_I + from);// U1
      Links[i]->k[i_freq][7] . insert_to(A, row, num_I + to);  // U2
      /*(*A)[2*i][2*i  ] = Links[i]->k[i_freq][0];
      (*A)[2*i][2*i+1] = Links[i]->k[i_freq][1];
      (*A)[2*i][num_I+from] = Links[i]->k[i_freq][2];
      (*A)[2*i][num_I+to  ] = Links[i]->k[i_freq][3];
      //B[2*i] = complex<Float>(0,0);
      // второе уравнение длинной ЛЭП
      (*A)[2*i+1][2*i  ] = Links[i]->k[i_freq][4];
      (*A)[2*i+1][2*i+1] = Links[i]->k[i_freq][5];
      (*A)[2*i+1][num_I+from ] = Links[i]->k[i_freq][6];
      (*A)[2*i+1][num_I+to   ] = Links[i]->k[i_freq][7];
      //B[2*i+1] = complex<Float>(0,0);  */
      }

   // уравнения по величинам мощности у узлах
   Matrix<Complex> E(3,3);
   E[0][0] = E[1][1] = E[2][2] = 1;
   int num_prev_equation = 3*2*Links.size();
   for(int i=0, n=Nodes.size(); i<n; i++){
         if(Nodes[i]->ClassName() == "big_system" ) continue;
         // поиск линий соединенных с узлом
         for(int j=0, m=Links.size(); j<m; j++){
         	// ток всегда направлен в сторону узла
            if(      Links[j]->From() == Nodes[i] ) E.insert_to(A, num_prev_equation + 3*i, 2*3*j);
            else if( Links[j]->To()   == Nodes[i] ) E.insert_to(A, num_prev_equation + 3*i, 2*3*j + 3);
            }
      }
   // уравнение по напряжению в узле системы
   for(int i=0, n=Nodes.size(); i<n; i++ )
      if(Nodes[i]->ClassName() == "big_system" )
         E.insert_to(A, num_prev_equation + 3*i, num_prev_equation + 3*i);

   Complex a[12][12];
   for(int i=0;i<12;i++)
      for(int j=0;j<12;j++)
         a[i][j] = (*A)[i][j];

   // обратная матрица
   *A = A->inverse();

   return true;
}

//---------------------------------------------------------------------------

int grid::Node_Num(node* Node){

   for(int i=0,n=Nodes.size(); i<n; i++)
      if(Nodes[i] == Node) return i;
   return -1;

}

//---------------------------------------------------------------------------

void grid::calc(){

   //static Matrix<Complex> X_1  = *X;

   update();
   // решаем систему
   int num = 3*(2*Links.size() + Nodes.size());

   for(int i_freq=0; i_freq<num_f; i_freq++){
    if(calc_non_nominal_freq || i_freq==1){
      Matrix< Complex > previous_X (num,1);
      Matrix< Complex > delta_X   (num,1);
      Matrix< Complex > current_X (num,1);
      for(int j=0; j<num; j++) previous_X[j][0] = (*X)[j][i_freq];

      Update_Equation(i_freq);
      do{
         Update_B(i_freq);
         current_X = (*A)*B;
         delta_X = current_X - previous_X;
         previous_X = current_X;
         } while(  bigger_than_eps(delta_X) );
      for(int j=0; j<num; j++) (*X)[j][i_freq] = current_X[j][0];
      }
    }

    /*/ фильтр, сглаживающий скачки решения вверх-вниз
    Matrix<Complex> X_0 = *X;
    *X = 0.5 * (X_0 + X_1);
    X_1 = X_0;*/
}

//---------------------------------------------------------------------------

bool grid::update(){
   bool ret = false;
   vector<link*>::iterator Link = Links.begin();    // связи сети
   while( Link != Links.end() ){
      ret = ret | (*Link)->update();
      Link++;
      }
   vector<node*>::iterator Node = Nodes.begin();    // узлы сети
   while( Node != Nodes.end() ){
      ret = ret | (*Node)->update();
      Node++;
      }
   return ret;
}

//---------------------------------------------------------------------------

void grid::Update_B(int i_freq){

   int num = 3*(2*Links.size() + Nodes.size());

   for(int i=0; i<num; i++)
      B[i] = 0;

   int num_prev_equation = 3*2*Links.size();
   
   // уравнение по напряжению в узле системы
   for(int i=0, n=Nodes.size(); i<n; i++ )
      if(Nodes[i]->ClassName() == "big_system"){
         B[num_prev_equation + 3*i    ] = *Nodes[i]->U[i_freq].A;
         B[num_prev_equation + 3*i + 1] = *Nodes[i]->U[i_freq].B;
         B[num_prev_equation + 3*i + 2] = *Nodes[i]->U[i_freq].C;
         }

   if(i_freq==1){
      // уравнения по величинам мощности в узлах
      for(int i=0, n=Nodes.size(); i<n; i++){
         if(Nodes[i]->ClassName() == "big_system")  continue;
         // если S>0 то идет потребление с узла
         // conj - сопряженный комплекс
         // 2 из-за того что исп амплитудные значения векторов тока и напряжения
         B[ num_prev_equation + 3*i     ] = (norm(*Nodes[i]->U[1].A)==0)?complex<Float>(0,0):conj ( (Float)2 * Nodes[i]->S.A / (*Nodes[i]->U[1].A) );// * sqrt((Float)12) );  // S - действ мощность фазы
         B[ num_prev_equation + 3*i + 1 ] = (norm(*Nodes[i]->U[1].B)==0)?complex<Float>(0,0):conj ( (Float)2 * Nodes[i]->S.B / (*Nodes[i]->U[1].B) );//* sqrt((Float)12) );
         B[ num_prev_equation + 3*i + 2 ] = (norm(*Nodes[i]->U[1].C)==0)?complex<Float>(0,0):conj ( (Float)2 * Nodes[i]->S.C / (*Nodes[i]->U[1].C) );//* sqrt((Float)12) );
         }
      }
   else{
      // уравнения по величинам токов высших гармоник и частоте == 0
      int num_prev_equation = 3*2*Links.size();
      for(int i=0, n=Nodes.size(); i<n; i++){
         if(Nodes[i]->ClassName() == "big_system") continue;
         else if(Nodes[i]->ClassName() == "rectifier"){
               B[ num_prev_equation + 3*i    ] = ((rectifier*)Nodes[i])->J[i_freq].A;
               B[ num_prev_equation + 3*i + 1] = ((rectifier*)Nodes[i])->J[i_freq].B;
               B[ num_prev_equation + 3*i + 2] = ((rectifier*)Nodes[i])->J[i_freq].C;
               }
         else if(Nodes[i]->ClassName() == "consumer"){
            for(int Phase=0; Phase<3; Phase++)
               {
               Complex Z = MAX;
               if(norm(Nodes[i]->S[Phase])) Z = conj( norm(*Nodes[i]->U[1][Phase]) / Nodes[i]->S[Phase] / (Float)2);
               Complex Z_i_freq = (norm(Z)< MIN)? Complex(MIN,0) : complex<Float>(real(Z), i_freq*imag(Z) );
               B[ num_prev_equation + 3*i + Phase  ] = (*Nodes[i]->U[i_freq][Phase]) / Z_i_freq;
               }
            }
         else if(Nodes[i]->ClassName() == "source"){
            complex<Float> K;
            if(i_freq!=0)
               K = (Float)0.9 * complex<Float>(0, i_freq*((source*)Nodes[i])->X2 );
            else
               K = complex<Float>(100,0);
            if( norm(K)<MIN )  K = complex<Float>(1,0);
            B[ num_prev_equation + 3*i     ] =  (*Nodes[i]->U[i_freq].A)  / K;
            B[ num_prev_equation + 3*i + 1 ] =  (*Nodes[i]->U[i_freq].B)  / K; 
            B[ num_prev_equation + 3*i + 2 ] =  (*Nodes[i]->U[i_freq].C)  / K;
            }
         }
      }
}

//---------------------------------------------------------------------------
bool grid::bigger_than_eps(Matrix< complex<Float> > &dX ){
   for(int i=0, n=dX.Rows(); i<n; i++)
      if( fabs(real(dX[i][0]))>eps || fabs(imag(dX[i][0]))>eps)
         return true;
   return false;
}
//---------------------------------------------------------------------------
void SaveObj(element *Obj, ostream &stream){
   string cname = Obj->ClassName();
   stream << cname << " "; // имя класса
   Obj->Save( stream );
}
//---------------------------------------------------------------------------
element* CreateObj(istream &stream){
	element *Obj;
   string ClassName;
   stream >> ClassName;
	if      ( ClassName == "big_system" )            Obj = new big_system() ;
   	else if ( ClassName == "source" )                Obj = new source() ;
   	else if ( ClassName == "consumer" )              Obj = new consumer() ;
   	else if ( ClassName == "rectifier" )             Obj = new rectifier() ;
   	else if ( ClassName == "NULL" )					 Obj = NULL;
   	else								  			 Obj = NULL;
	// читаем данные объекта
    if(Obj) Obj->Read(stream);
    return Obj;
}
//---------------------------------------------------------------------------
// Params - параметры используемые при создании объекта
element* CreateObj(String ClassName, vector<void*> Params){
	element *Obj;
	if ( ClassName == "big_system" )     	  Obj = new big_system() ;
	else if ( ClassName == "source" )         Obj = new source() ;
	else if ( ClassName == "consumer" )       Obj = new consumer() ;
   else if ( ClassName == "rectifier" )      Obj = new rectifier() ;
   else if ( ClassName == "NULL" )			  Obj = NULL;
   else									  Obj = NULL;
   return Obj;
}
//---------------------------------------------------------------------------
/* Описатель свойств классов */
FieldInfo::FieldInfo(){
   ptr = NULL;
   Type = "Float";
   Mode = rw;     // режим чтения записи
}
//---------------------------------------------------------------------------
string FieldInfo::Value(){
   if(!ptr) return "";
   if(Mode== rwf && !File.empty()) return File;
   if     (Type == "string")
      return *(string*)ptr;
   else if(Type == "Float")
      return toStr<Float>(*(Float*)ptr);
   else if(Type == "int")
      return toStr<int>(*(int*)ptr);
   else if(Type == "bool")
      return toStr<bool>(*(bool*)ptr);
   else if(Type == "complex")
      return toStr< complex<Float> >(*(complex<Float>*)ptr);
   else if(Type == "Voltage")
      return toStr< complex<Float> >( *((*(Dynamic_Array< complex<Float>* >*)ptr)[1])  );
   else if(Type == "Dynamic_Array"){
      Dynamic_Array<Float> *I = (Dynamic_Array< Float>*)ptr;
      string str = "";
      for(int i=0, n=I->size(); i<n; i++) str = str + toStr<Float>( (*I)[i] ) + "  " ;
      return str;
      }
   else if(Type == "Dynamic_Array<complex>"){
      Dynamic_Array< complex<Float> > *A = (Dynamic_Array< complex<Float> >*)ptr;
      string str = "";
      for(int i=0, n=A->size(); i<n; i++) str = str + toStr< complex<Float> >( (*A)[i] ) + "  " ;
      return str;
      }
   else if(Type == "vector<complex>"){
      vector< complex<Float> > *A = (vector< complex<Float> >*)ptr;
      string str = "";
      for(int i=0, n=A->size(); i<n; i++) str = str + toStr< complex<Float> >( (*A)[i] ) + "  " ;
      return str;
      }
   else if(Type == "power"){
      power< complex<Float> > *S = (power< complex<Float> >*)ptr;
      Float eps = norm(S->A) / 1e6;
      eps = (eps<MIN)?MIN:eps;
      if( norm(S->A - S->B) < eps && norm(S->B - S->C) < eps )
         return toStr<Complex>(S->A + S->B + S->C);
      else
         return toStr<Complex>(S->A) + " " + toStr<Complex>(S->B) + " " + toStr<Complex>(S->C);
      }
   return "";
}
//---------------------------------------------------------------------------
void FieldInfo::Value(string newVal){
	if( Mode== ro) return;
   if( Mode== rwf){
         ifstream in(newVal.c_str());
         if(in){
            int i,n;
            for(i=1,n=newVal.length(); i<=n; i++ ) if(newVal[i]==' '){err(_WARNING_,"FieldInfo: пробелы в имени файла не разрешены"); break;};
            if(i>n){
               File = newVal;
               string tmp;
               fromFile.clear();
               while(!in.eof()){
                  getline(in,tmp);
                  if(!tmp.empty())
                     fromFile.push_back(tmp);
                  }
               in.close();
               return;
               }
            }
         File.clear();
         fromFile.clear();
      }
   setNewValue(newVal);
}
//---------------------------------------------------------------------------
void FieldInfo::setNewValue(string newVal)
{
   if(!ptr) return;
   try{
      if (Type == "string")
         *(string*)ptr = newVal;
      else if(Type == "Float")
         *(Float*)ptr = fromStr<Float>(newVal);
      else if(Type == "int")
         *(int*)ptr = fromStr<int>(newVal);
      else if(Type == "bool")
         *(bool*)ptr = fromStr<bool>(newVal);
      else if(Type == "complex")
         *(complex<Float>*)ptr = fromStr< complex<Float> >(newVal);
      else if(Type == "Voltage")
         *((*(Dynamic_Array< complex<Float>* >*)ptr)[1]) =  fromStr< complex<Float> >(newVal);
      else if(Type == "Dynamic_Array"){
         std::istringstream _iss(newVal);
         Dynamic_Array<Float> *I = (Dynamic_Array< Float>*)ptr;
         for(int i=0, n=I->size(); i<n; i++) _iss >> (*I)[i] ;
         }
      else if(Type == "Dynamic_Array<complex>"){
         std::istringstream _iss(newVal);
         Dynamic_Array< complex<Float> > *A = (Dynamic_Array< complex<Float> >*)ptr;
         A->clear();
         while(!_iss.eof()){complex<Float> tmp; _iss >> tmp; A->push_back(tmp);}
         }
      else if(Type == "vector<complex>"){
         std::istringstream _iss(newVal);
         vector< complex<Float> > *A = (vector< complex<Float> >*)ptr;
         A->clear();
         while(!_iss.eof()){complex<Float> tmp; _iss >> tmp; A->push_back(tmp);}
         }
      else if(Type == "power"){
         std::istringstream _iss(newVal);
         power<Complex> *S = (power<Complex>*)ptr;
         int cnt=0;
         while(!_iss.eof() && cnt<3){
            string tmp;
            _iss >> tmp;
            if(!tmp.empty()){
               (*S)[cnt] = fromStr<Complex>(tmp);
               cnt++;}
            }
         switch(cnt)
            {
            case 0:  (*S) = 0; break;
            case 1:  (*S) = S -> A; break; // задана трехфазная мощность
            case 2:  S->C = 0; break;
            }
         }
   } catch( std::logic_error ) {
      err(1, "PFieldInfo: Недопустимый символ при преобразовании строки в число");
   }
}

//---------------------------------------------------------------------------

bool FieldInfo::update(){
   if(File.empty()) return false;
	string newVal = Value(TIME);
   if(newVal.empty())  return false;
   if(Type=="string")		*(string*)ptr = newVal;
   else if(Type=="Float")	*(Float*)ptr = fromStr<Float>(newVal);
   else if(Type=="int")		*(int*)ptr = fromStr<int>(newVal);
   else if(Type=="complex")*(complex<Float>*)ptr = fromStr< complex<Float> >(newVal);
   else if(Type=="power")  setNewValue(newVal);
   else return false;
   return true;
}

//---------------------------------------------------------------------------

string FieldInfo::Value(const Float &time){
   if(Mode != rwf) {err(_ERROR_, "FiledInfo: значение поля не может быть прочитано из файла"); return "";}
   if(File.empty()) return "";
   int sz = fromFile.size();
   if(sz<2){err(_ERROR_, "FiledInfo: файл пуст"); return "";}
   Float file_smp = fromStr<Float>(fromFile[0]);
   int n = 1 + ((int)(time/file_smp))%(sz-1);
   return fromFile[n];
}

//---------------------------------------------------------------------------

void FieldInfo::Save(ostream &out){
   if(!File.empty())
      out << File << " ";
   else
      out << "-" << " ";
}

//---------------------------------------------------------------------------

void FieldInfo::Read(istream &in){
   string tmp;
   in >> tmp;
   if(tmp!="-")   { File = tmp; Value(File);}
}

//---------------------------------------------------------------------------
ObjectList::~ObjectList(){
	vector<element*> to_del = Obj;
   vector<element*>::iterator iter = to_del.begin();
    while( iter != to_del.end() )
    {
        delete *iter;
        iter++;
    }
}
//---------------------------------------------------------------------------
void ObjectList::Add    (element* obj){
	/*/ добавляем только объекты с уникальным именем
   - бесполезно так как Add вызывается из element
   а там всегда задается имя, равным которому нет в сети
   element *ex = Find(obj->Name);
   if(ex){
   	string name;
      for(int i=0; i<1e6; i++){
      	name = obj->Name + toStr<int>(i);
         ex = Find(name);
         if(!ex) break;
         }
      obj->Name = name;
      err(_WARNING_, "ObjectList: добавление объекта. Такое имя существует. Объект переименован");
   	}*/
   Obj.push_back(obj);
}
//---------------------------------------------------------------------------
bool ObjectList::Delete (element* obj){
   vector<element*>::iterator iter = Obj.begin();
    while( iter != Obj.end() )
    {
      if( *iter == obj){
        Obj.erase( iter );
        return true;
        }
      else
        ++iter;
    }
   return false;
}
//---------------------------------------------------------------------------
// имя объекта (сети или оборудования) в своей подсети должно быть уникально
element* ObjectList::Find (string Name){
   // найдем этот объект
   for(int i=0,n=Obj.size(); i<n; i++)
		if( Obj[i]->Name == Name)
			return Obj[i];
   return NULL;
}
//---------------------------------------------------------------------------
// имя объекта (сети или оборудования) в своей подсети должно быть уникально
element* ObjectList::Find (int ID){
   // найдем этот объект
   for(int i=0,n=Obj.size(); i<n; i++)
		if( Obj[i]->id() == ID)
			return Obj[i];
   return NULL;
}

//---------------------------------------------------------------------------

template<class T>
T& phase<T>::operator[](int i)
{
   switch(i)
   {
      case 0:  return A;
      case 1:  return B;
      default: return C;
   }
};

//---------------------------------------------------------------------------

template<class T>
ostream& operator << (ostream& out, const phase<T>& p)
{
   return out << p.A << " " << p.B << " " << p.C << " ";
}

//---------------------------------------------------------------------------

template<class T>
istream& operator >> (istream& in, phase<T>& p)

{
   return in >> p.A >> p.B >> p.C;
}

//---------------------------------------------------------------------------

template<class T>
phase<T> & phase<T>::operator=(const T &op2)
{
   A = B = C = op2;
   return *this;
}

//---------------------------------------------------------------------------

template<class T> T imag( const phase<complex<T> > &X)
{
   return ( imag(X.A) + imag(X.B) + imag(X.C) ) / 3;
}

//---------------------------------------------------------------------------

template<class T> T abs( const phase<complex<T> > &X)
{
   return ( abs(X.A) + abs(X.B) + abs(X.C) ) / 3; 
}


//---------------------------------------------------------------------------

template<class T> T abs( const phase<complex<T>* > &X)
{
   return ( abs(*X.A) + abs(*X.B) + abs(*X.C) ) / 3; 
}

//---------------------------------------------------------------------------

template<class T>
power<T> & power<T>::operator=(const T &op2)
{
   A = B = C = op2 / Complex(3,0); // пересчет мощности на фазу 
   return *this;
}


//---------------------------------------------------------------------------

template<class T> T imag( const power<complex<T> > &X)
{
   return ( imag(X.A) + imag(X.B) + imag(X.C) );  // суммарная трехфазная мощность
}

//---------------------------------------------------------------------------

template<class T> T abs( const power<complex<T> > &X)
{
   return ( abs(X.A) + abs(X.B) + abs(X.C) );   // суммарная трехфазная мощность
}

