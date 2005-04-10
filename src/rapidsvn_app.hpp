/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _RAPIDSVN_APP_H_INCLUDED_
#define _RAPIDSVN_APP_H_INCLUDED_

// wxwindows
#include "wx/app.h"
#include "wx/intl.h"
#include <wx/html/helpctrl.h> // html help support

class wxTipProvider;

class RapidSvnApp:public wxApp
{
public:
  void OptionallyRegisterTempFile (const wxString & filename);
  void OptionallyPurgeTempFiles ();
  
#if wxUSE_WXHTML_HELP
  wxHtmlHelpController& GetHelpController()
  {
    return *m_helpController;
  }
#endif

#if wxUSE_STARTUP_TIPS
  wxTipProvider*
  MakeTipProvider (bool force = false);

  void
  SaveTipsInfo (bool showAtStartup, int tipIndex);
#endif

protected:
  virtual bool OnInit ();
  virtual int OnExit ();
  
private:
  wxLocale m_locale;
  
  wxArrayString m_TempFiles;
  
#if wxUSE_WXHTML_HELP
  wxHtmlHelpController* m_helpController;
  
  bool 
  LocateHelp ();
#endif
};

DECLARE_APP (RapidSvnApp)

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
