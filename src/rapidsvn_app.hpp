/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _RAPIDSVN_APP_H_INCLUDED_
#define _RAPIDSVN_APP_H_INCLUDED_

// wxWidgets
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
