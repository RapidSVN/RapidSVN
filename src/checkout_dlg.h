/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
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

class CheckoutDlg : public wxDialog
{
public:
  struct sData 
  {
    sData();
    
    wxString ModuleName;
    wxString DestFolder;
    wxString User;
    wxString Password;
    wxString Revision;
    bool Recursive;
    bool UseLatest;
  };

  CheckoutDlg (wxWindow *parent, sData* pData);
  virtual void InitDialog();

  void OnOK (wxCommandEvent& event);
  void OnBrowse (wxCommandEvent & event);

private:
  void InitializeData ();
  void OnUseLatest(wxCommandEvent &event);
  void EnableControls();

  sData * m_pData;
  wxCheckBox * pUseLatest;
  wxTextCtrl * revision;
  wxStaticText * pRevisionLabel; 
  wxTextCtrl * destFolder;

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
