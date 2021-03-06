#ifndef KernelH
#define KernelH

#include <string>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <system.hpp>   // String

#define   _ERROR_     0
#define   _WARNING_   3
#define   _NOTICE_    6
#define   _DEBUG_     7

#define freq 50						//частота основной гармоники

using namespace std;

typedef long double Float;

extern const Float w;
extern Float smp;
extern Float TIME;
extern void err(unsigned, std::string);

template < typename T > std::string toStr(T aVal);
template < typename T > T fromStr(const std::string &aS);
ostream& operator<<(ostream& out, const string& c);
istream& operator>>(istream& in,        string& c);
ostream& operator<<(ostream& out, const String& c);
istream& operator>>(istream& in,        String& c);
template < class T >
std::ostream& operator << (std::ostream& out, const std::vector<T>& v);
template < class T >
std::istream& operator >> (std::istream& in, std::vector<T>& v);

//---------------------------------------------------------------------------

template < typename T > std::string toStr(T aVal)
{
    std::ostringstream _oss;
    _oss << aVal;
    return _oss.str();
}

//---------------------------------------------------------------------------

template < typename T > T fromStr(const std::string& aS)
{
   std::istringstream _iss(aS);
   T _res;
   _iss >> _res;
   return _res;
}

//---------------------------------------------------------------------------

#endif
 