#include "include.h"
#include "commit_dlg.h"

BEGIN_EVENT_TABLE (CommitDlg, wxDialog)
EVT_BUTTON (ID_BUTTON_OK, CommitDlg::OnOk)
EVT_BUTTON (ID_BUTTON_CANCEL, CommitDlg::OnCancel) END_EVENT_TABLE ()
     void
     CommitDlg::OnOk (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_OK);
}

void
CommitDlg::OnCancel (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_CANCEL);
}

void
CommitDlg::InitializeData ()
{
  user = (wxTextCtrl *) wxFindWindowByName ("user", this);
  pass = (wxTextCtrl *) wxFindWindowByName ("pass", this);
  logMsg = (wxTextCtrl *) wxFindWindowByName ("logMsg", this);
  recursive = (wxCheckBox *) wxFindWindowByName ("recursive", this);

  recursive->SetValue (TRUE);
}
