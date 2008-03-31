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

// app
#include "about_dlg.hpp"
#include "svn_version.h"
#include "version.hpp"
#include "res/bitmaps/logo.png.h"
#include "utils.hpp"

AboutDlg::AboutDlg (wxWindow * parent, const wxLocale & locale)
  : wxDialog (parent, -1, wxEmptyString, wxDefaultPosition)
{
  const wxString title (wxString::Format(
    _("About %s"), APPLICATION_NAME));
  SetTitle (title);

  // format string
  const wxString version (wxString::Format (
    _("%s Version %s"),
    APPLICATION_NAME, Utf8ToLocal (RAPIDSVN_VER_STR).c_str ()));

  // TODO: Make these two constants in version.hpp translatable and wxT()'ed respectively.
  // Until then use the kludge of pretending they're UTF8 to save some silly looking ifdef's
  const wxString strCopyrightMessage (Utf8ToLocal (RAPIDSVN_COPYRIGHT));
  const wxString strVerMilestone (Utf8ToLocal (RAPIDSVN_VER_MILESTONE));

#ifdef wxUSE_UNICODE
  wxString unicode (_("Unicode"));
#else
  wxString unicode (_("ANSI"));
#endif

  const wxString copy (wxString::Format(
    wxT("%s\n") // version
    wxT("\n%s\n\n") // copyright
    wxT("%s\n") // for more information
    wxT("http://rapidsvn.tigris.org"),
    version.c_str (),
    strCopyrightMessage.c_str (),
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

  const wxString built (wxString::Format (
    builtFmt, 
    wxMAJOR_VERSION, wxMINOR_VERSION, 
    wxRELEASE_NUMBER, unicode.c_str (),
    SVN_VER_MAJOR, SVN_VER_MINOR, SVN_VER_MICRO));

  const wxString info (wxString::Format(
    infoFmt,
    locale.GetLocale (), locale.GetSysName ().c_str (),
    locale.GetCanonicalName ().c_str ()));

  // create controls
  wxStaticBitmap * logo =
    new wxStaticBitmap (this, -1, EMBEDDED_BITMAP(logo_png));
  wxStaticText * labelCopy = new wxStaticText (this, -1, copy);
  wxStaticText * labelBuilt = new wxStaticText (this, -1, built);
  wxStaticText * labelInfo = new wxStaticText (this, -1, info);
  wxButton * button = new wxButton (this, wxID_OK, _("OK"));

  // position controls
  wxFlexGridSizer * topSizer = new wxFlexGridSizer (2, 10, 10);
  topSizer->Add (logo, 0);
  topSizer->Add (labelCopy, 1, wxEXPAND);
  topSizer->Add (labelBuilt, 0, wxALL);
  topSizer->Add (labelInfo, 0, wxALL);

  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
  mainSizer->Add (topSizer, 0, wxALL, 5);
  mainSizer->Add (button, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);

  CentreOnParent ();
}

AboutDlg::~AboutDlg ()
{
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
