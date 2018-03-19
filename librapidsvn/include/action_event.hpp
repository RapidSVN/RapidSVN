/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _ACTION_EVENT_H_
#define _ACTION_EVENT_H_

// wxWidgets
#include "wx/string.h"

// forward declarations
class wxWindow;
#include "utils.hpp"

class ActionEvent
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param event_id id of event
   */
  ActionEvent(wxWindow * parent, int event_id);

  /**
   * constructor
   *
   * @param parent parent window
   * @param event_id id of event
   * @param msg message event has to pass
   */
  ActionEvent(wxWindow * parent, int event_id, const wxString & msg);

  ActionEvent(wxWindow * parent, int event_id, LogItemType type,
              const wxString & action, const wxString & msg);

  /**
   * constructor
   *
   * @param parent parent window
   * @param event_id id of event
   * @param data data event has to pass
   */
  ActionEvent(wxWindow * parent, int event_id, void * data);

  /**
   * destructor
   */
  virtual ~ActionEvent();

  void
  init(wxWindow * parent, int event_id);

  void
  init(wxWindow * parent, int event_id, const wxString & msg);

  void
  init(wxWindow * parent, int event_id, LogItemType type,
       const wxString & action, const wxString & msg);

  void
  init(wxWindow * parent, int event_id, void * data);

//   /**
//    * set the event code to be passed
//    *
//    * @param code event code to pass
//    */
//   void
//   SetInt (int code);

//   /**
//    * set the message to be passed
//    *
//    * @param msg message event has to pass
//    */
//   void
//   SetStr (wxString msg);

//   /**
//    * set the data to be passed
//    *
//    * @param data data event has to pass
//    */
//   void
//   SetData (void * data);

  /**
   * post the event
   */
  void
  Post() const;

  static void
  Post(wxWindow * parent, int event_id);

  static void
  Post(wxWindow * parent, int event_id, const wxString & msg);

  static void
  Post(wxWindow * parent, int event_id, LogItemType type,
       const wxString & action, const wxString & msg);

  static void
  Post(wxWindow * parent, int event_id, void * data);

private:
  struct Data;
  Data * m;

  /**
   * forbid default constructor
   */
  ActionEvent();

  /**
   * forbid default copy constructor
   */
  ActionEvent(const ActionEvent &);

  /**
   * forbid default assignment operator
   */
  ActionEvent operator = (const ActionEvent &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
