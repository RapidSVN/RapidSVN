/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
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
  AuthDlg (wxWindow * parent, 
           const wxString & username, 
           const wxString & password);

  virtual ~AuthDlg ();

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
