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
// wxwindows
#include "wx/wx.h"
#include "wx/confbase.h"

// app
#include "preferences.hpp"

/*********************************************************************/

// platform dependant constants
#ifdef _WIN32
  static const wxChar * DEFAULT_EXPLORER = wxT("explorer.exe");
#else
  static const wxChar * DEFAULT_EXPLORER = wxT("");
#endif
static const wxChar * DEFAULT_EDITOR = wxT("");
static const wxChar * DEFAULT_DIFF_TOOL = wxT("");

/**
 * Configuration key names
 */
static const wxChar CONF_EDITOR[] = 
  wxT("/Preferences/StandardEditor");
static const wxChar CONF_EDITOR_ALWAYS[] = 
  wxT("/Preferences/AlwaysStandardEditor");
static const wxChar CONF_EDITOR_ARGS[] =
  wxT("/Preferences/StandardEditorArgs");
static const wxChar CONF_EXPLORER[] = 
  wxT("/Preferences/StandardFileExplorer");
static const wxChar CONF_EXPLORER_ALWAYS[] = 
  wxT("/Preferences/AlwaysStandardFileExplorer");
static const wxChar CONF_EXPLORER_ARGS[] =
  wxT("/Preferences/StandardFileExplorerArgs");
static const wxChar CONF_PURGE_TEMP_FILES[] = 
  wxT("/Preferences/PurgeTempFiles");
static const wxChar CONF_AUTH_PER_BOOKMARK[] =
  wxT("/Preferences/AuthPerBookmark");
static const wxChar CONF_DIFF_TOOL[] =
  wxT("/Preferences/DiffTool");
static const wxChar CONF_DIFF_TOOL_ARGS[] =
  wxT("/Preferences/DiffToolArgs");

Preferences::Preferences ()
  : editor (DEFAULT_EDITOR), editorAlways (false), editorArgs (wxEmptyString),
    explorer (DEFAULT_EXPLORER), explorerAlways (false), 
    explorerArgs (wxEmptyString), diffTool (DEFAULT_DIFF_TOOL), 
    diffToolArgs (wxEmptyString), purgeTempFiles (true),
    authPerBookmark (false)
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
  editorArgs = config->Read (CONF_EDITOR_ARGS, editorArgs);

  explorer = config->Read (CONF_EXPLORER, explorer);
  config->Read (CONF_EXPLORER_ALWAYS, &explorerAlways);
  explorerArgs = config->Read (CONF_EXPLORER_ARGS, explorerArgs);

  diffTool = config->Read (CONF_DIFF_TOOL, diffTool);
  diffToolArgs = config->Read (CONF_DIFF_TOOL_ARGS, diffToolArgs);

  config->Read (CONF_PURGE_TEMP_FILES, &purgeTempFiles);

  config->Read (CONF_AUTH_PER_BOOKMARK, &authPerBookmark);
}

void Preferences::Write () const
{
  wxConfigBase * config = wxConfigBase::Get ();

  config->Write (CONF_EDITOR, editor);
  config->Write (CONF_EDITOR_ALWAYS, editorAlways);
  config->Write (CONF_EDITOR_ARGS, editorArgs);

  config->Write (CONF_EXPLORER, explorer);
  config->Write (CONF_EXPLORER_ALWAYS, explorerAlways);
  config->Write (CONF_EXPLORER_ARGS, explorerArgs);

  config->Write (CONF_DIFF_TOOL, diffTool);
  config->Write (CONF_DIFF_TOOL_ARGS, diffToolArgs);

  config->Write (CONF_PURGE_TEMP_FILES, purgeTempFiles);
  
  config->Write (CONF_AUTH_PER_BOOKMARK, authPerBookmark);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
