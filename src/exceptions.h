#ifndef _EXCEPTIONS_H_INCLUDED_
#define _EXCEPTIONS_H_INCLUDED_

class RapidSvnEx
{
public:
  wxString estr;
  long code;

public:
   RapidSvnEx (const wxString & __estr = "", const long __code = -1);

   virtual ~ RapidSvnEx ();

   RapidSvnEx (const RapidSvnEx & rhs)
  {
    *this = rhs;
  }

  RapidSvnEx & operator = (const RapidSvnEx & rhs);

  void what (const wxString & __estr);

  virtual const wxChar *what () const
  {
    return estr.c_str ();
  }

  const wxString & str (void) const
  {
    return estr;
  }
};
#endif
