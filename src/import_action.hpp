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
#ifndef _IMPORT_ACTION_H_INCLUDED_
#define _IMPORT_ACTION_H_INCLUDED_

#include "action.hpp"
#include "import_dlg.hpp"

class ImportAction:public Action
{
private:
  ImportDlg::sData m_data;

public:
  ImportAction (wxWindow * parent);

  virtual bool Perform ();
  virtual bool Prepare ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
