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
#ifndef _UPDATE_DLG_H_INCLUDED_
#define _UPDATE_DLG_H_INCLUDED_

class UpdateDlg:public wxDialog
{
public:
  struct sData 
  {
    sData();
    
    wxString User;
    wxString Password;
    wxString Revision;
    bool UseLatest;
  };

  UpdateDlg(wxWindow* parent, sData* pData);
  virtual void InitDialog();
  
private:
  void InitializeData();
  void OnUseLatest(wxCommandEvent &event);
  void EnableControls();

  sData* m_pData;
  wxCheckBox* pUseLatest;
  wxStaticText* pRevisionLabel; 
  wxTextCtrl* pRevision;
  
  DECLARE_EVENT_TABLE()
};

#endif
