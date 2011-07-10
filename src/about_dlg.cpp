/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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

// wxWidgets
#include "wx/wx.h"

// app
#include "about_dlg.hpp"
#include "svn_version.h"
#include "version.hpp"
#include "res/bitmaps/logo.png.h"
#include "utils.hpp"

AboutDlg::AboutDlg(wxWindow * parent, const wxLocale & locale)
    : AboutDlgBase(parent, -1, wxEmptyString, wxDefaultPosition)
{
  const wxString title(wxString::Format(
                         _("About %s"), APPLICATION_NAME));
  SetTitle(title);

  // format string
  wxString version;
  
  if (0 == RAPIDSVN_VER_REVISION)
  {
    version = wxString::Format(_("%s Version %s"),
                               APPLICATION_NAME, Utf8ToLocal(RAPIDSVN_VER_STR).c_str());
  }
  else
  {
    version = wxString::Format(_("%s Version %s Revision %d"),
                               APPLICATION_NAME, Utf8ToLocal(RAPIDSVN_VER_STR).c_str(), RAPIDSVN_VER_REVISION);
  }

  // TODO: Make these two constants in version.hpp translatable and wxT()'ed respectively.
  // Until then use the kludge of pretending they're UTF8 to save some silly looking ifdef's
  const wxString strCopyrightMessage(Utf8ToLocal(RAPIDSVN_COPYRIGHT));
  const wxString strGpl(_("This program is licensed under the terms\nof the GNU General Public License version 3\n\nAvailable online under:"));
  const wxString strVerMilestone(Utf8ToLocal(RAPIDSVN_VER_MILESTONE));
  
#ifdef wxUSE_UNICODE
  wxString unicode(_("Unicode"));
#else
  wxString unicode(_("ANSI"));
#endif
  
  const wxString copy(wxString::Format(
                        wxT("%s\n") // version
                        wxT("\n%s\n\n") // copyright
                        wxT("%s\nhttp://www.gnu.org/licenses/gpl-3-0.html\n\n") // gpl
                        wxT("%s\n") // for more information
                        wxT("http://rapidsvn.tigris.org"),
                        version.c_str(),
                        strCopyrightMessage.c_str(),
                        strGpl.c_str(),
                        _("For more information see:")));


  const wxString builtFmt(_("\
Built with:\n\
wxWidgets %d.%d.%d (%s)\n\
Subversion %d.%d.%d\n"));

  const wxString infoFmt(_("\
Locale Information:\n\
Language: %s\n\
System Name: %s\n\
Canonical Name: %s\n"));

  const svn_version_t * pSvnVersion = svn_subr_version();
  assert(pSvnVersion != 0);

  const wxString built(wxString::Format(
                         builtFmt,
                         wxMAJOR_VERSION, wxMINOR_VERSION,
                         wxRELEASE_NUMBER, unicode.c_str(),
                         pSvnVersion->major, pSvnVersion->minor, pSvnVersion->patch));

  const wxString info(wxString::Format(
                        infoFmt,
                        locale.GetLocale(), locale.GetSysName().c_str(),
                        locale.GetCanonicalName().c_str()));

  m_bitmapLogo->SetBitmap(EMBEDDED_BITMAP(logo_png));
  m_staticCopy->SetLabel(copy);
  m_staticBuilt->SetLabel(built);
  m_staticInfo->SetLabel(info);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();
}

AboutDlg::~AboutDlg()
{
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
