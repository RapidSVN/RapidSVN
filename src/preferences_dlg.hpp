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
#ifndef _PREFERENCES_DLG_H_INCLUDED_
#define _PREFERENCES_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

// forward declarations
struct Preferences;

/**
 * Preferences dialog that displays the preference pages.
 */
class PreferencesDlg : public wxDialog
{
public:
  /**
   * constructor
   */
  PreferencesDlg (wxWindow* parent, Preferences * prefs);

  /**
   * destructor
   */
  virtual ~PreferencesDlg ();

  /**
   * A specialised version of TransferDataFromWindow that calls
   * TransferDataFromWindow for each panel in the dialog. Sadly this
   * is not the default base class behaviour. 
   * @return true if transfer succeeded.
   */
  virtual bool TransferDataFromWindow ();
  
  /**
   * A specialised version of TransferDataToWindow that calls
   * TransferDataToWindow for each panel in the dialog. Sadly this
   * is not the default base class behaviour. 
   * @return true if transfer succeeded.
   */
  virtual bool TransferDataToWindow ();

private:
  /** 
   * disallow default constructor
   */
  PreferencesDlg ();

  /**
   * disallow copy constructor
   */
  PreferencesDlg (const Preferences &);

  /**
   * disallow assignment operator
   */
  PreferencesDlg & 
  operator = (const PreferencesDlg & src);

  struct Data;
  Data * m;
  
private:
  DECLARE_EVENT_TABLE ()
};

#endif // _PREFERENCES_DLG_H_INCLUDED_

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
