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
#ifndef _REPORT_DLG_H_INCLUDED_
#define _REPORT_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

enum
{
  NORMAL_REPORT,
  ERROR_REPORT
};

// forward declarations
class wxButton;
class wxTextCtrl;

class ReportDlg:public wxDialog
{
public:
   ReportDlg (wxWindow * parent,
               const wxString & caption,
               const wxString & data, int type = NORMAL_REPORT);

  void OnButton (wxCommandEvent & event);

private:
  wxButton *m_button;
  wxTextCtrl *m_text;

  const wxString & m_str;
 DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
