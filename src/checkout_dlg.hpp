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
#ifndef _CHECKOUT_DLG_H_INCLUDED_
#define _CHECKOUT_DLG_H_INCLUDED_

// wxwindows
#include "wx/wx.h"

// app
#include "checkout_data.hpp"

class CheckoutDlg : public wxDialog
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   */
  CheckoutDlg (wxWindow *parent);

  virtual void InitDialog();

  void OnOK (wxCommandEvent& event);
  void OnBrowse (wxCommandEvent & event);

  const CheckoutData &
  GetData () const
  {
    return m_data;
  }

private:
  void InitControls ();
  void OnUseLatest(wxCommandEvent &event);
  void EnableControls();

  CheckoutData m_data;
  wxCheckBox * m_useLatestCheck;
  wxTextCtrl * m_revisionText;
  wxStaticText * m_revisionLabel; 
  wxTextCtrl * m_destFolderText;

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
