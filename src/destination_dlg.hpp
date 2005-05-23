/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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
#ifndef _DESTINATION_DLG_H_INCLUDED_
#define _DESTINATION_DLG_H_INCLUDED_

// wxWidgets
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
  DestinationDlg (wxWindow* parent, const wxString & title,
                  const wxString & descr = wxEmptyString, const int flags=0,
                  const wxString & dst = wxEmptyString);

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
  Create (wxWindow* parent, const wxString & title,
          const wxString & descr = wxEmptyString, const int flags=0,
          const wxString & dst = wxEmptyString);

  /**
   * get destination
   *
   * @return destination
   */
  const wxString &
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
