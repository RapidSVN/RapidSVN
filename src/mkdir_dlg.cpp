#include "include.h"
#include "mkdir_dlg.h"

BEGIN_EVENT_TABLE (MkdirDlg, wxDialog)
EVT_BUTTON (ID_BUTTON_OK, MkdirDlg::OnOk)
EVT_BUTTON (ID_BUTTON_CANCEL, MkdirDlg::OnCancel) END_EVENT_TABLE ()
     void
     MkdirDlg::OnOk (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_OK);
}

void
MkdirDlg::OnCancel (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_CANCEL);
}

void
MkdirDlg::InitializeData ()
{
  user = (wxTextCtrl *) wxFindWindowByName ("user", this);
  pass = (wxTextCtrl *) wxFindWindowByName ("pass", this);
  logMsg = (wxTextCtrl *) wxFindWindowByName ("logMsg", this);
  target = (wxTextCtrl *) wxFindWindowByName ("target", this);
}
