#ifndef _CHECKOUT_DLG_H_INCLUDED_
#define _CHECKOUT_DLG_H_INCLUDED_

#include "res/checkout.h"

class CheckoutDlg : public wxDialog
{

public:
  wxTextCtrl *moduleName;
  wxTextCtrl *destFolder;
  wxTextCtrl *user;
  wxTextCtrl *pass;
  wxTextCtrl *revision;
  wxCheckBox *recursive;
  CheckoutDlg (wxWindow *parent);

public:
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);
  void OnBrowse (wxCommandEvent & event);

  void InitializeData ();

private:
 DECLARE_EVENT_TABLE ()};


#endif
