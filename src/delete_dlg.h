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
