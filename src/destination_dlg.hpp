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
#ifndef _DESTINATION_DLG_H_INCLUDED_
#define _DESTINATION_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

class DestinationDlg:public wxDialog
{
public:
  DestinationDlg (wxWindow* parent, const char * title,
                  const char * descr, const char * dst);

  virtual ~DestinationDlg ();

  const char *
  GetDestination () const;

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
