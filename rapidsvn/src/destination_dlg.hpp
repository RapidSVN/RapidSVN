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
#ifndef _DESTINATION_DLG_H_INCLUDED_
#define _DESTINATION_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

/**
 * this is a multi-purpose dialog that can be used
 * to fetch a single path or URL. It can be configured
 * using the different flags
 */
class DestinationDlg:public wxDialog
{
public:
  static const int WITH_FORCE;
    
  /**
   * constructor
   *
   * @param parent parent window
   * @param title title caption
   * @param descr descriptive label
   * @param flags WITH_FORCE add a force checkbox. See @a GetForce
   * @param dsr default destination
   */
  DestinationDlg (wxWindow* parent, const char * title,
                  const char * descr = "", const int flags=0,
                  const char * dst = "");

  /**
   * default constructor. Use @a Create to initialize the dialog
   */
  DestinationDlg ();
  
  /**
   * destructor
   */
  virtual ~DestinationDlg ();

  /**
   * initialize instance. See @a DestinationDlg for parameters
   */
  void
  Create (wxWindow* parent, const char * title,
          const char * descr = "", const int flags=0,
          const char * dst = "");

  /**
   * get destination
   *
   * @return destination
   */
  const char *
  GetDestination () const;

  /**
   * get the value of the "force" checkbox
   *
   * @return true=force/false=dont force
   */
  bool
  GetForce () const;

private:
  /**
   * hide implementation details
   */
  struct Data;
  Data * m;
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
