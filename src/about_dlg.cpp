/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
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

// svncpp
#include "svncpp/url.hpp"

// app
#include "about_dlg.hpp"
#include "svn_version.h"
#include "version.hpp"
#include "res/bitmaps/logo.xpm"
#include "utils.hpp"

AboutDlg::AboutDlg (wxWindow * parent, const wxLocale & locale)
  : wxDialog (parent, -1, wxEmptyString, wxDefaultPosition)
{
  wxString title;
  title.Printf (_("About %s"), APPLICATION_NAME);
  SetTitle (title);

  std::vector<std::string> schemasVector =
    svn::Url::supportedSchemas ();
  wxString schemasStr;
  std::vector<std::string>::const_iterator it;
  bool first = true;
  for (it = schemasVector.begin (); it != schemasVector.end (); it++)
  {
    if (first)
      first = false;
    else
      schemasStr += wxT("\n");
    std::string schema = *it;
    schemasStr += wxT("- ");
    schemasStr += Utf8ToLocal(schema);
  }


  // format string
  wxString version;
  version.Printf (_("%s Version %d.%d.%d"),
                  APPLICATION_NAME,
                  RAPIDSVN_VER_MAJOR,
                  RAPIDSVN_VER_MINOR,
                  RAPIDSVN_VER_MICRO);

  // TODO: Make these two constants in version.hpp translatable and wxT()'ed respectively.
  // Until then use the kludge of pretending they're UTF8 to save some silly looking ifdef's
  wxString strCopyrightMessage (Utf8ToLocal (RAPIDSVN_COPYRIGHT));
  wxString strVerMilestone (Utf8ToLocal (RAPIDSVN_VER_MILESTONE));

  wxString milestone;
  milestone.Printf (_("Milestone: %s"),
                    strVerMilestone.c_str ());

  wxString subversion;
  subversion.Printf (_("Subversion %d.%d.%d"),
                     SVN_VER_MAJOR,
                     SVN_VER_MINOR,
                     SVN_VER_MICRO);

  wxString wx;
  wx.Printf (_("wxWidgets %d.%d.%d"),
             wxMAJOR_VERSION,
             wxMINOR_VERSION,
             wxRELEASE_NUMBER);

  wxString copy;
  copy.Printf (wxT("%s\n" // version
               "%s\n" // milestone
               "\n%s\n\n" // copyright
               "%s\n" // for more information
               "http://rapidsvn.tigris.org"),
               version.c_str (),
               milestone.c_str (),
               strCopyrightMessage.c_str (),
               _("For more information see:"));

  wxString built;
  built.Printf (wxT("%s\n" // built with
                "%s\n" // subversion
                "\n"
                "%s"), // wxWidgets
                _("Built with:"),
                subversion.c_str (),
                wx.c_str ());

  wxString schemas;
  schemas.Printf (wxT("%s\n" // "supported url schemas"
                  "%s"), // list of schemas
                  _("Supported URL schemas: "),
                  schemasStr.c_str ());

  // create controls
  wxStaticText * labelCopy = new wxStaticText (this, -1, copy);
  wxStaticText * labelBuilt = new wxStaticText (this, -1, built);
  wxStaticText * labelSchemas = new wxStaticText (this, -1, schemas);
  wxStaticText * labelTitle = new wxStaticText (
    this, -1, _("Locale Information:"));
  wxStaticText * labelLanguageTitle = new wxStaticText (
    this, -1, _("Language:"));
  wxStaticText * labelLanguageName = new wxStaticText (
    this, -1, locale.GetLocale ());
  wxStaticText * labelSysTitle = new wxStaticText (
    this, -1, _("System Name:"));
  wxStaticText * labelSysName = new wxStaticText (
    this, -1, locale.GetSysName ());
  wxStaticText * labelCanonicalTitle = new wxStaticText (
    this, -1, _("CanoncialName:"));
  wxStaticText * labelCanonicalName = new wxStaticText (
    this, -1, locale.GetCanonicalName ());

  wxStaticBitmap * logo =
    new wxStaticBitmap (this, -1, wxBitmap (logo_xpm));

  wxButton * button = new wxButton (this, wxID_OK, _("OK"));

  // position controls
  wxFlexGridSizer * topSizer = new wxFlexGridSizer (2, 0, 10);
  topSizer->Add (logo, 0);
  topSizer->Add (labelCopy, 1, wxEXPAND);
  topSizer->Add (0, 10, wxBottom);
  topSizer->Add (0, 10, wxBottom);
  topSizer->Add (labelBuilt, 0);
  topSizer->Add (labelSchemas, 0);
  topSizer->Add (0, 10, wxBottom);
  topSizer->Add (0, 10, wxBottom);
  topSizer->Add (labelTitle, 0);
  topSizer->Add (0, 0);
  topSizer->Add (labelLanguageTitle, 0);
  topSizer->Add (labelLanguageName, 0);
  topSizer->Add (labelSysTitle, 0);
  topSizer->Add (labelSysName, 0);
  topSizer->Add (labelCanonicalTitle, 0);
  topSizer->Add (labelCanonicalName, 0);

  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
  mainSizer->Add (topSizer, 0, wxALL, 20);
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
