/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _AUTH_DLG_H_INCLUDED_
#define _AUTH_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

class AuthDlg:public wxDialog
{
public:
  static const int HIDE_USERNAME;

  AuthDlg (wxWindow * parent, 
           const char * username = "", 
           const char * password = "",
           int flags = 0);

  virtual ~AuthDlg ();

  const char * 
  GetUsername () const;

  const char *
  GetPassword () const;

private:
  struct Data;
  Data * m;
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
