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
