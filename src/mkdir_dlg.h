#ifndef _MKDIR_DLG_H_INCLUDED_
#define _MKDIR_DLG_H_INCLUDED_

class MkdirDlg:public wxDialog
{
public:
  struct sData 
  {
    sData();
    
    wxString Target;
    wxString LogMessage;
    wxString User;
    wxString Password;
  };

  MkdirDlg(wxWindow* parent, sData* pData);

private:
  void InitializeData ();
  
  sData* m_pData;

  DECLARE_EVENT_TABLE ()
};

#endif
