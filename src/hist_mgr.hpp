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
#ifndef _HIST_MGR_H_INCLUDED_
#define _HIST_MGR_H_INCLUDED_

// wxWidgets
#include "wx/list.h"


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


  /** Default maximal number for the lists */
  static const unsigned int DEFAULT_MAX_COUNT;

  
  /** 
   * Read a list of strings identified by @a id
   * from the configuration file.
   *
   * The first entry in the list will be the most
   * recent entered string.
   *
   * @param id unique identier for the list
   * @param maxCount the maximal number of entries
   *                 for the list. 0 means unlimited
   * @return a string list
   */
  wxStringList 
  ReadList (const wxString & id, 
            unsigned int maxCount = DEFAULT_MAX_COUNT);


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
   * @param maxCount the maximal number of entries 
   *                 for the list. 0 means unlimited
   */
  void
  WriteList (const wxString & id, 
             const wxStringList & list,
             unsigned int maxCount = DEFAULT_MAX_COUNT);


private:

  /** disallow copy constructor */
  HistoryManager (const HistoryManager &);


  /** disallow assignment operator */
  HistoryManager & operator = (const HistoryManager &);
};
#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
