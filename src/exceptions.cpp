#include "include.h"
#include "exceptions.h"

RapidSvnEx::RapidSvnEx (const wxString & __estr, const long __code)
{
  code = __code;
  what (__estr);
}

RapidSvnEx::~RapidSvnEx ()
{
}

RapidSvnEx & RapidSvnEx::operator = (const RapidSvnEx & rhs)
{
  if (this == &rhs)
    return *this;

  estr = rhs.estr;
  code = rhs.code;

  return *this;
}

void
RapidSvnEx::what (const wxString & __what)
{
  estr.Printf (_T ("%s  code: %ld"), __what.c_str (), code);
}
