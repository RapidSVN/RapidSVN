#ifndef _PREFERENCES_DLG_H_INCLUDED_
#define _PREFERENCES_DLG_H_INCLUDED_

/* Panels for the preferences dialog. */

class GeneralPanel : public wxPanel
{
public:
  static GeneralPanel* Create(wxWindow* parent);
  
private:
  GeneralPanel(wxWindow* parent); // Call Create please.
  void InitializeData ();
  
  DECLARE_EVENT_TABLE ()
};

class ExternalsPanel : public wxPanel
{
public:
  static ExternalsPanel* Create(wxWindow* parent);
  
private:
  ExternalsPanel(wxWindow* parent); // Call Create please.
  void InitializeData ();
  
  DECLARE_EVENT_TABLE ()
};

/* The preferences dialog. */

class PreferencesDlg : public wxDialog
{
public:
  static PreferencesDlg* CreateInstance(wxWindow* parent);

  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();

  // The preference data, edited in the dialog.  
  static struct SData
  {
    SData();
    void Read(wxConfigBase* pConfig);
    void Write(wxConfigBase* pConfig) const;
    
    // Replace this with the real preference data in due course.
    // Use types supported by wxValidator classes.
    wxString Text;
    bool EnableX, EnableY;
    bool Enable1, Enable2, Enable3;
    int Choice;
  } Data;

private:
  PreferencesDlg(wxWindow* parent); // Please use CreateInstance().
  void InitializeData ();
  
  wxNotebook *m_pNB;
  
  DECLARE_EVENT_TABLE ()
};

#endif

