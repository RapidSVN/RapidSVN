#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

class ImportDlg:public wxDialog
{
public:
  struct sData
  {
    sData();
    
    wxString Repository;
    wxString Path;
    wxString NewEntry;
    wxString LogMessage;
    wxString User;
    wxString Password;
    bool Recursive;
    bool TreeType, FileType; 
  };
  
  ImportDlg (wxWindow *parent, sData* pData);
  void OnOk (wxCommandEvent & event);
  void OnBrowse (wxCommandEvent & event);

private:  
  void InitializeData ();
  sData * m_pData;

  DECLARE_EVENT_TABLE ()
};

#endif
