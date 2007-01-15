/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
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
// wxWidgets
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
static const wxChar CONF_USE_AUTH_CACHE[] =
  wxT("/Preferences/UseAuthCache");
static const wxChar CONF_DIFF_TOOL[] =
  wxT("/Preferences/DiffTool");
static const wxChar CONF_DIFF_TOOL_ARGS[] =
  wxT("/Preferences/DiffToolArgs");
static const wxChar CONF_USE_LAST_COMMIT_MESSAGE[] =
  wxT("/Preferences/UseLastCommitMessage");

Preferences::Preferences ()
  : editor (DEFAULT_EDITOR), editorAlways (false), editorArgs (wxEmptyString),
    explorer (DEFAULT_EXPLORER), explorerAlways (false),
    explorerArgs (wxEmptyString), diffTool (DEFAULT_DIFF_TOOL),
    diffToolArgs (wxEmptyString), purgeTempFiles (true),
    authPerBookmark (false), useAuthCache (true)
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

  config->Read (CONF_PURGE_TEMP_FILES,  &purgeTempFiles);
  config->Read (CONF_AUTH_PER_BOOKMARK, &authPerBookmark);
  config->Read (CONF_USE_AUTH_CACHE,    &useAuthCache);
  
  config->Read (CONF_USE_LAST_COMMIT_MESSAGE, &useLastCommitMessage);
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
  config->Write (CONF_USE_AUTH_CACHE, useAuthCache);

  config->Write (CONF_USE_LAST_COMMIT_MESSAGE, useLastCommitMessage);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
