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
#include "wx/confbase.h"

// app
#include "preferences.hpp"

/*********************************************************************/

// platform dependant constants
#ifdef _WIN32
  static const char DEFAULT_EXPLORER [] =
    "explorer.exe";
#else
  static const char DEFAULT_EXPLORER [] = "";
#endif
static const char DEFAULT_EDITOR [] = "";

/**
 * Configuration key names
 */
static const char CONF_EDITOR[] = 
  "/Preferences/StandardEditor";
static const char CONF_EDITOR_ALWAYS[] = 
  "/Preferences/AlwaysStandardEditor";
static const char CONF_EXPLORER[] = 
  "/Preferences/StandardFileExplorer";
static const char CONF_EXPLORER_ALWAYS[] = 
  "/Preferences/AlwaysStandardFileExplorer";

/**
 * The Preferences singleton
 */
static Preferences * m_preferences;

Preferences::Preferences ()
  : editor (DEFAULT_EDITOR), explorer (DEFAULT_EXPLORER),
    editorAlways (false), explorerAlways (false)
{
  Read ();
}

Preferences::~Preferences ()
{
  Write ();
}

void 
Preferences::Read ()
{
  wxConfigBase * config = wxConfigBase::Get ();

  // If these reads fail, the default values from the constructor remain
  editor = config->Read (CONF_EDITOR, editor);
  config->Read (CONF_EDITOR_ALWAYS, &editorAlways);

  explorer = config->Read (CONF_EXPLORER, explorer);
  config->Read (CONF_EXPLORER_ALWAYS, &explorerAlways);
}

void Preferences::Write () const
{
  wxConfigBase * config = wxConfigBase::Get ();

  config->Write (CONF_EDITOR, editor);
  config->Write (CONF_EDITOR_ALWAYS, editorAlways);

  config->Write (CONF_EXPLORER, explorer);
  config->Write (CONF_EXPLORER_ALWAYS, explorerAlways);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
