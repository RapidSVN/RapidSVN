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
#ifndef _UPDATE_DLG_H_INCLUDED_
#define _UPDATE_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

// app
#include "update_data.hpp"

// forward declarations
class wxCheckBox;
class wxStaticText;
class wxTextCtrl;

class UpdateDlg:public wxDialog
{
public:
  UpdateDlg (wxWindow* parent);
  
  const UpdateData &
  GetData () const;
private:
  void InitData();
  void InitDialog();
  void OnUseLatest(wxCommandEvent &event);
  void EnableControls();

  UpdateData m_data;
  wxCheckBox* m_useLatestCheck;
  wxStaticText* m_revisionLabel; 
  wxTextCtrl* m_revisionText;
  
  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
