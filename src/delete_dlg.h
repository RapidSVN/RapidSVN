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
#ifndef _DELETE_DLG_H_INCLUDED_
#define _DELETE_DLG_H_INCLUDED_

class DeleteDlg:public wxDialog
{
public:
  struct sData
  {
    sData();
    
    wxString LogMessage;
    wxString User;
    wxString Password;
    bool Force;
  };
  
  DeleteDlg(wxWindow* parent, sData* pData);

private:
  void InitializeData ();
  
  sData* m_pData;
  
  DECLARE_EVENT_TABLE ()
};

#endif
