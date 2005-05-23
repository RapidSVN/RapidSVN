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

#ifndef _LOG_DLG_H_INCLUDED_
#define _LOG_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// svncpp
#include "svncpp/client.hpp"

//forward declarations
class wxTextCtrl;
class LogList;
class wxListEvent;

class LogDlg : public wxDialog
{
public:
  /**
   * constructor. the @a entries are NOT owned by
   * this class.
   *
   * @param parent parent window
   * @param path path of selected item
   * @param entries log entries
   */
  LogDlg (wxWindow * parent,
          const char * path,
          const svn::LogEntries * entries);

  /**
   * destructor
   */
  virtual ~LogDlg ();

private:
  /** hide implementation details */
  struct Data;
  Data * m;


  /**
   * Event handler for the "Get" button
   * @param event
   */
  void OnGet (wxCommandEvent & event);


  /**
   * Event handler for the "View" button
   * @param event
   */
  void OnView (wxCommandEvent & event);


  /**
   * Event handler that will be called whenever
   * the selection in the list of revision changes
   *
   * @param event
   */
  void OnSelected (wxListEvent& event);


  /**
   * Event handler for the "Diff" button
   * @param event
   */
  void OnDiff (wxCommandEvent & event);

  /**
   * Event handler for the "Merge" button
   * @param event
   */
  void OnMerge (wxCommandEvent & event);

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
