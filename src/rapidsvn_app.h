#ifndef _VSVN_APP_H_INCLUDED_
#define _VSVN_APP_H_INCLUDED_

#include "filelist_ctrl.h"
#include "folder_browser.h"
#include "utils.h"
#include "rapidsvn_frame.h"

class RapidSvnFrame;

class RapidSvnApp:public wxApp
{
  virtual bool OnInit ();
  virtual int OnExit ();

public:
   RapidSvnFrame * appFrame;

  RapidSvnFrame *GetAppFrame ()
  {
    return appFrame;
  }
};

DECLARE_APP (RapidSvnApp)
#endif
