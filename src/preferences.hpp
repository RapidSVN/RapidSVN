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

  wxString editor;
  wxString explorer;
  bool editorAlways;
  bool explorerAlways;
  bool authPerProject;

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
