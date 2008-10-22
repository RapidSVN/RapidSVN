/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#ifndef _HIST_MGR_H_INCLUDED_
#define _HIST_MGR_H_INCLUDED_

// wxWidgets
#include "wx/string.h"


/**
 * This class manages lists of historical entries. Such a list
 * has an unique identifier and a list of strings associated to it.
 *
 * You can read them with @a ReadList and write them back with
 * @a WriteList.
 *
 */
class HistoryManager
{
public:
  /** Default constructor */
  HistoryManager();


  /** Destructor */
  virtual ~ HistoryManager();


  /**
   * Read a list of strings identified by @a id
   * from the configuration file and write the
   * results to @a list.
   *
   * The first entry in the list will be the most
   * recent entered string.
   *
   * @param id unique identier for the list
   * @return list of entries
   */
  const wxArrayString &
  ReadList(const wxString & id);


  /**
   * Add an entry to an existing list. If the
   * list doesnt exist already a new one is created
   *
   * @param id unique identifier for the list
   * @param entry string to add
   */
  void
  AddEntryToList(const wxString & id,
                 const wxString & entry);


  /**
   * Writes an entry identified by @a id back to
   * the configuration file.
   *
   * @param id unique identifier for the list
   * @param list string list
   */
  void
  WriteList(const wxString & id,
            const wxArrayString & list);


  /**
   * Call this method to ensure all the data is
   * written to the configuration files
   */
  void
  Cleanup();


private:
  struct Data;

  Data * m;

  /** disallow copy constructor */
  HistoryManager(const HistoryManager &);


  /** disallow assignment operator */
  HistoryManager & operator = (const HistoryManager &);
};


/**
 * This is the singleton instance of the history manager
 */
extern HistoryManager TheHistoryManager;

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
