/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
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
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
