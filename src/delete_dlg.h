#ifndef _DELETE_DLG_H_INCLUDED_
#define _DELETE_DLG_H_INCLUDED_

#include "res/delete.h"

#if defined(__WXMSW__)
static char *Delete_Dialog = NULL;
#else
# include "res/delete.wxr"
#endif

class DeleteDlg:public wxDialog
{

public:
  wxTextCtrl * user;
  wxTextCtrl *pass;
  wxTextCtrl *logMsg;
  wxCheckBox *force;

public:
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);

  void InitializeData ();

private:
 DECLARE_EVENT_TABLE ()};

#endif
