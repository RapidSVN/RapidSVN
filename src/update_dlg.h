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
