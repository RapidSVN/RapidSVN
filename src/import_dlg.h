#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

#include "res/import.h"

#if defined(__WXMSW__)
// Under Windows, some compilers can't include
// a whole .wxr file. import_dlg will point to the whole .wxr 'file'.
static char *Import_Dialog = NULL;
#else
// Other platforms should have sensible compilers that
// cope with long strings.
#include "res/import.wxr"
#endif


class ImportDlg:public wxDialog
{

public:
  wxTextCtrl * reposURL;
  wxTextCtrl *path;
  wxTextCtrl *new_entry;
  wxTextCtrl *logMsg;
  wxRadioBox *path_type;
  wxTextCtrl *user;
  wxTextCtrl *pass;
  wxCheckBox *recursive;

public:
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);
  void OnBrowse (wxCommandEvent & event);
  void OnRadio (wxCommandEvent & event);

  void InitializeData ();

private:
 DECLARE_EVENT_TABLE ()};

#endif
