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
#ifndef _COMMIT_DLG_H_INCLUDED_
#define _COMMIT_DLG_H_INCLUDED_

class CommitDlg:public wxDialog
{
public:
  struct sData 
  {
    sData();
    
    wxString LogMessage;
    wxString User;
    wxString Password;
    bool Recursive;
  };
  
  CommitDlg(wxWindow* parent, sData* pData);

private:
  void InitializeData ();
  
  sData* m_pData;
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
