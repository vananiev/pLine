//---------------------------------------------------------------------------

#pragma hdrstop

#include "kernel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

const Float w = 2*M_PI*freq;		      //круговая частота 1 гармоники
Float smp = 1e-4;				// sample - шаг по времени для расчета модели
Float TIME=0;                   //текущее время

//---------------------------------------------------------------------------

ostream& operator<<(ostream& out, const string& c)
{
   string s = c;
   if(s=="") s = "_";
   for(int i=1,n=s.length(); i<=n; i++ )
   	if(s[i]==' ') s[i]='_';
   return out << s.c_str();
}
//---------------------------------------------------------------------------

istream& operator>>(istream& in, string& c)
{
   char name[10240];
   name[0]=0;
	in >> name;    c = name;
   return in;
}

//---------------------------------------------------------------------------

ostream& operator<<(ostream& out, const String& c)
{
   String s = c;
   if(s=="") s = "_";
   for(int i=1,n=s.Length(); i<=n; i++ )
   	if(s[i]==' ') s[i]='_';
   return out << s.c_str();
}

//---------------------------------------------------------------------------

istream& operator>>(istream& in, String& c)
{
   char name[10240];
   name[0]=0;
	in >> name;    c = name;
   return in;
}

//---------------------------------------------------------------------------

template < class T >
std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
{
    out << v.size() << " ";
    for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii)
    {
      out <<  *ii << " ";
    }
    return out;
}


//---------------------------------------------------------------------------

template < class T >
std::istream& operator >> (std::istream& in, std::vector<T>& v)
{
   unsigned int n;
   in >> n;
   v.resize(n);
   for(unsigned int i=0; i<n; i++)
      in >> v[i];
   return in;
}





