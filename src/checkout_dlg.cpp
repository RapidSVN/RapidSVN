#include "include.h"
#include "rapidsvn_app.h"
#include "checkout_dlg.h"

BEGIN_EVENT_TABLE (CheckoutDlg, wxDialog)
EVT_BUTTON (ID_BUTTON_OK, CheckoutDlg::OnOk)
EVT_BUTTON (ID_BUTTON_CANCEL, CheckoutDlg::OnCancel)
EVT_BUTTON (ID_BUTTON_BROWSE, CheckoutDlg::OnBrowse) END_EVENT_TABLE ()
     void
     CheckoutDlg::OnOk (wxCommandEvent & WXUNUSED (event))
{
  wxString val;
  unsigned long rev = 0;

  val = revision->GetValue ();
  TrimString (val);

  if (!val.ToULong (&rev, 10) && rev < 0)
  {
    // could not convert revision to a number
    wxMessageBox (_T ("Revision must be an unsigned number!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);
    return;
  }

  EndModal (ID_BUTTON_OK);
}

void
CheckoutDlg::OnCancel (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_CANCEL);
}

void
CheckoutDlg::OnBrowse (wxCommandEvent & event)
{
  wxDirDialog dialog (this,
                      _T ("Select a destination folder to checkout to"),
                      wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->
                      GetPath ());

  if (dialog.ShowModal () == wxID_OK)
    destFolder->SetValue (dialog.GetPath ());
}

void
CheckoutDlg::InitializeData ()
{
  moduleName = (wxTextCtrl *) wxFindWindowByName ("moduleName", this);
  destFolder = (wxTextCtrl *) wxFindWindowByName ("destFolder", this);
  user = (wxTextCtrl *) wxFindWindowByName ("user", this);
  pass = (wxTextCtrl *) wxFindWindowByName ("pass", this);
  revision = (wxTextCtrl *) wxFindWindowByName ("revision", this);
  recursive = (wxCheckBox *) wxFindWindowByName ("recursive", this);
  recursive->SetValue (TRUE);
}
