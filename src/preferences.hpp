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
#ifndef _PREFERENCES_H_INCLUDED_
#define _PREFERENCES_H_INCLUDED_

// wxwindows
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
   * location of the executable that will be 
   * used for exploring folders
   */
  wxString explorer;

  /** 
   * always use the explorer executable for folders 
   */
  bool explorerAlways;

  /**
   * location of the diff-tool executable
   */
  wxString diffTool;

  /**
   * remember passwords for each bookmark (vs. the 
   * same passwords for all bookmarks)
   */
  bool authPerBookmark;

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
