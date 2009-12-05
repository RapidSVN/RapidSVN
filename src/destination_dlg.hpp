/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.  
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _DESTINATION_DLG_H_INCLUDED_
#define _DESTINATION_DLG_H_INCLUDED_

// app
#include "rapidsvn_generated.h"

/**
 * this is a multi-purpose dialog that can be used
 * to fetch a single path or URL. It can be configured
 * using the different flags
 */
class DestinationDlg:public DestinationDlgBase
{
public:
  enum
  {
    WITH_FORCE=0x1
  };

  /**
   * constructor
   *
   * @param parent parent window
   * @param title title caption
   * @param descr descriptive label
   * @param flags WITH_FORCE add a force checkbox. See @a GetForce
   * @param dst default value
   * @param history if this parameter is used a combo box with
   *                previous entries will be used instead of the edit
   *                box @see HistoryValidator
   */
  DestinationDlg(wxWindow* parent, const wxString & title,
                 const wxString & descr = wxEmptyString, int flags=0,
                 const wxString & dst = wxEmptyString,
                 const wxString & history = wxEmptyString);

  /**
   * destructor
   */
  virtual ~DestinationDlg();

  /**
   * get destination
   *
   * @return destination
   */
  const wxString &
  GetDestination() const;

  /**
   * get the value of the "force" checkbox
   *
   * @return true=force/false=dont force
   */
  bool
  GetForce() const;

protected: // event handlers inherited from DestinationDlgBase
  virtual void OnText(wxCommandEvent & event);

private:
  /**
   * hide implementation details
   */
  struct Data;
  Data * m;

  void CheckControls();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
