/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#if defined( _MSC_VER) && _MSC_VER <= 1200
#pragma warning( disable: 4786 )// debug symbol truncated
#endif

// wxWidgets
#include "wx/wx.h"
#include "wx/aboutdlg.h"

// app
#include "about_dlg.hpp"
#include "svn_version.h"
#include "version.hpp"
#include "res/bitmaps/logo.png.h"
#include "utils.hpp"


#ifdef wxUSE_UNICODE
  static const int isUnicode=true;
#else
  static const int isUnicode=false;
#endif


void
::ShowAboutDlg (const wxLocale & locale)
{
  wxAboutDialogInfo info;

  info.SetName(APPLICATION_NAME);
  info.SetVersion(wxT(RAPIDSVN_VER_STR));
  info.SetCopyright(wxT(RAPIDSVN_COPYRIGHT));
  info.SetLicense(wxT("GNU General Public License v3"));
  info.SetWebSite(wxT("http://www.rapidsvn.org"));

  wxString unicode (isUnicode ? _("Unicode") : _("ANSI"));

  const wxString builtFmt(_("\
Built with:\n\
wxWidgets %d.%d.%d (%s)\n\
Subversion %d.%d.%d\n"));

  const wxString infoFmt(_("\
Locale Information:\n\
Language: %s\n\
System Name: %s\n\
Canonical Name: %s\n"));

  const wxString built(wxString::Format(
                         builtFmt,
                         wxMAJOR_VERSION, wxMINOR_VERSION,
                         wxRELEASE_NUMBER, unicode.c_str(),
                         SVN_VER_MAJOR, SVN_VER_MINOR, SVN_VER_MICRO));

  const wxString localeInfo(wxString::Format(
                              infoFmt,
                              locale.GetLocale(), locale.GetSysName().c_str(),
                              locale.GetCanonicalName().c_str()));

  const wxString descr(wxString::Format(wxT("%s\n%s"),
                         built, localeInfo));

  info.SetDescription(descr);  
  ::wxAboutBox(info);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
