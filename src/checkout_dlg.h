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
  wxTextCtrl * destFolder;
  wxTextCtrl * revision;
  wxTextCtrl * moduleName;
  wxTextCtrl * user;
  wxTextCtrl * pass;
  wxCheckBox * recursive;
  wxStaticText * pRevisionLabel; 

  DECLARE_EVENT_TABLE ()
};

#endif
