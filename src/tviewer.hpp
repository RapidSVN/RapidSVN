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
#ifndef _TVIEWER_H_INCLUDED_
#define _TVIEWER_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

class TextViewer : public wxDialog
{
public:
  /**
   * constructor
   *
   * @param title window title
   */
  TextViewer (const char * title);

  /**
   * destructor
   */
  virtual ~TextViewer ();

  /** 
   * sets the contents of the viewer to @a text
   *
   * @param text new contents (with newlines)
   */
  void
  SetText (const char * text);
  
private:
  struct Data;
  Data * m;

  void
  OnClose (wxCloseEvent & event);

  void 
  OnCancel (wxCommandEvent & event);

  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
