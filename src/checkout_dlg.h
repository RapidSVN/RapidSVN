#ifndef _CHECKOUT_DLG_H_INCLUDED_
#define _CHECKOUT_DLG_H_INCLUDED_

#include "res/checkout.h"

#if defined(__WXMSW__)
static char *Checkout_Dialog = NULL;
#else
# include "res/checkout.wxr"
#endif

class CheckoutDlg:public wxDialog
{

public:
  wxTextCtrl * moduleName;
  wxTextCtrl *destFolder;
  wxTextCtrl *user;
  wxTextCtrl *pass;
  wxTextCtrl *revision;
  wxCheckBox *recursive;

public:
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);
  void OnBrowse (wxCommandEvent & event);

  void InitializeData ();

private:
 DECLARE_EVENT_TABLE ()};


#endif
