#include "include.h"
#include "update_dlg.h"

BEGIN_EVENT_TABLE (UpdateDlg, wxDialog)
EVT_BUTTON (ID_BUTTON_OK, UpdateDlg::OnOk)
EVT_BUTTON (ID_BUTTON_CANCEL, UpdateDlg::OnCancel) END_EVENT_TABLE ()
     void
     UpdateDlg::OnOk (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_OK);
}

void
UpdateDlg::OnCancel (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_CANCEL);
}

void
UpdateDlg::InitializeData ()
{
  user = (wxTextCtrl *) wxFindWindowByName ("user", this);
  pass = (wxTextCtrl *) wxFindWindowByName ("pass", this);
  revision = (wxTextCtrl *) wxFindWindowByName ("revision", this);
}
