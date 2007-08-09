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
#ifndef _PREFERENCES_H_INCLUDED_
#define _PREFERENCES_H_INCLUDED_

// wxWidgets
#include "wx/string.h"

/**
 * All Preference information is stored in this class.
 */
struct Preferences
{
public:
  /**
   * default constructor
   */
  Preferences ();

  /**
   * destructor
   */
  virtual ~Preferences ();

  /**
   * location of the executable to edit files
   */
  wxString editor;

  /**
   * always use the editor executable to edit files
   */
  bool editorAlways;

  /**
   * arguments to pass to the editor
   */
  wxString editorArgs;

  /**
   * location of the executable that will be
   * used for exploring folders
   */
  wxString explorer;

  /**
   * always use the explorer executable for folders
   */
  bool explorerAlways;

  /**
   * arguments to pass to the explorer
   */
  wxString explorerArgs;

  /**
   * location of the diff-tool executable
   */
  wxString diffTool;

  /**
   * arguments to pass to the diff tool
   */
  wxString diffToolArgs;

  /**
   * delete all temporary files made by the program upon exit
   */
  bool purgeTempFiles;

  /**
   * remember passwords for each bookmark (vs. the
   * same passwords for all bookmarks)
   */
  bool authPerBookmark;

  /**
   * Store authentication data on hard disk
   */
  bool useAuthCache;

  /**
   * Default commit comment to last comment used
   */
  bool useLastCommitMessage;

   /**
   * always save state of flat mode as reset/false
   */
  bool resetFlatModeOnStart;

private:
  void Read ();
  void Write () const;
};

#endif // _PREFERENCES_H_INCLUDED_

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
