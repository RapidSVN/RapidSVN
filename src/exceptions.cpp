#include "include.h"
#include "exceptions.h"

VSvnEx::VSvnEx (const wxString & __estr, const long __code)
{
  code = __code;
  what (__estr);
}

VSvnEx::~VSvnEx ()
{
}

VSvnEx & VSvnEx::operator = (const VSvnEx & rhs)
{
  if (this == &rhs)
    return *this;

  estr = rhs.estr;
  code = rhs.code;

  return *this;
}

void
VSvnEx::what (const wxString & __what)
{
  estr.Printf (_T ("%s  code: %ld"), __what.c_str (), code);
}
