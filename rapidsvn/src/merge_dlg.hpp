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
#ifndef _MERGE_DLG_H_INCLUDED_
#define _MERGE_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

struct MergeData 
{
  MergeData();

  wxString Path1;
  wxString Path2;
  wxString Path1Rev;
  wxString Path2Rev;
  wxString Destination;
  bool Recursive;
  bool Force;
};

class MergeDlg:public wxDialog
{
public:
  MergeDlg (wxWindow *parent, bool calledByLogDlg, MergeData & data);
  void OnOK (wxCommandEvent& event);
  void OnBrowse (wxCommandEvent & event);

private:
  void InitializeData ();
  int TestRev (wxString & val);
  
  MergeData & m_data;
  bool m_calledByLogDlg;

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
