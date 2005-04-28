/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
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
  HistoryManager ();


  /** Destructor */
  virtual ~ HistoryManager ();


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
  ReadList (const wxString & id);


  /**
   * Add an entry to an existing list. If the
   * list doesnt exist already a new one is created
   * 
   * @param id unique identifier for the list
   * @param entry string to add
   */
  void
  AddEntryToList (const wxString & id,
                  const wxString & entry);


  /**
   * Writes an entry identified by @a id back to
   * the configuration file.
   *
   * @param id unique identifier for the list
   * @param list string list
   */
  void
  WriteList (const wxString & id, 
             const wxArrayString & list);


  /**
   * Call this method to ensure all the data is
   * written to the configuration files
   */
  void
  Cleanup ();


private:
  struct Data;

  Data * m;

  /** disallow copy constructor */
  HistoryManager (const HistoryManager &);


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
