#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

class MergeDlg:public wxDialog
{
public:
  struct sData 
  {
    sData();

    wxString Path1;
    wxString Path2;
    wxString Path1Rev1;
    wxString Path1Rev2;
    wxString Path2Rev;
    wxString User;
    wxString Password;
    bool Recursive;
    bool Force;
  };
  
  MergeDlg (wxWindow *parent, sData* pData);
  void OnOK (wxCommandEvent& event);

private:
  void InitializeData ();
  int TestRev (wxString & val);
  
  sData * m_pData;

  DECLARE_EVENT_TABLE ()
};

#endif
