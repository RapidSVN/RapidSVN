/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
#include "wx/wx.h"

// app
#include "about_dlg.hpp"
#include "res/bitmaps/logo.xpm"
#include "svn_version.h"
#include "version.hpp"

AboutDlg::AboutDlg (wxWindow * parent)
  : wxDialog (parent, -1, "", wxDefaultPosition)
{
  wxString title;
  title.Printf (_("About %s"), APPLICATION_NAME);
  SetTitle (title);

  // format string
  wxString version;
  version.Printf (_("%s Version %d.%d.%d"),
                  APPLICATION_NAME,
                  RAPIDSVN_VER_MAJOR, 
                  RAPIDSVN_VER_MINOR, 
                  RAPIDSVN_VER_MICRO);

  wxString milestone;
  milestone.Printf (_("Milestone: %s"), 
                    RAPIDSVN_VER_MILESTONE);

  wxString subversion;
  subversion.Printf ("Subversion %d.%d.%d",
                     SVN_VER_MAJOR, 
                     SVN_VER_MINOR, 
                     SVN_VER_MICRO);

  wxString wx;
  wx.Printf ("wxWindows %d.%d.%d",
             wxMAJOR_VERSION, 
             wxMINOR_VERSION, 
             wxRELEASE_NUMBER);
  
  wxString msg;
  msg.Printf ("%s\n"
              "%s\n"
              "\n%s\n\n"
              "%s\n"
              "http://rapidsvn.tigris.org\n"
              "\n"
              "\n%s\n"
              "%s\n"
              "%s",
              version.c_str (),
              milestone.c_str (),
              RAPIDSVN_COPYRIGHT,
              _("For more information see:"),
              _("Built with:"),
              subversion.c_str (),
              wx.c_str ());

  // create controls

  wxStaticText * label = new wxStaticText (this, -1, msg);
  wxStaticBitmap * logo = 
    new wxStaticBitmap (this, -1, wxBitmap (logo_xpm));

  wxButton * button = new wxButton (this, wxID_OK, _("OK"));

  // position controls
  wxBoxSizer * topSizer = new wxBoxSizer (wxHORIZONTAL);
  topSizer->Add (logo, 0, wxALL, 20);
  topSizer->Add (label, 1, wxALL | wxEXPAND, 20);

  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
  mainSizer->Add (topSizer);
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
