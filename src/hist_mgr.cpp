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

// wxWidgets
#include "wx/wx.h"
#include "wx/string.h"
#include "wx/hashmap.h"
#include "wx/confbase.h"
#include "wx/debug.h"

// app
#include "hist_mgr.hpp"
#include "utils.hpp"


/** a hash map that contains wxArrayString */
WX_DECLARE_STRING_HASH_MAP (wxArrayString, wxArrayStringHashMap);


/** maximal number of entries per list */
static const unsigned int MAX_COUNT = 100;


/** format string for number values */
static const wxChar CONF_COUNT_FMT[] = wxT("/%s/Count");

/** format string for a single value */
static const wxChar CONF_VALUE_FMT[] = wxT("/%s/Value%d");


HistoryManager TheHistoryManager;


/**
 * structure that hides implementation details from 
 * hist_mgr.hpp
 */
struct HistoryManager::Data
{
public:
  /** 
   * this flag signalizes all data has been 
   * written to the disk.
   */
  bool clean;
  wxArrayStringHashMap lists;


  /** default constructor; nothing to initialize */
  Data ()
    : clean (true)
  {
  }


  /** 
   * destructor: there will be an assertion if
   * not all data has been written to the disk!
   */
  ~Data ()
  {
    wxASSERT_MSG (clean, wxT("HistoryManager: not all data has been written"));
  }


  /**
   * checks whether the list named @a name exists 
   * and returns its value. If this doesnt 
   * exist, create a new empty list
   *
   * @param name name of the list
   * @return list of strings
   */
  const wxArrayString &
  ReadList (const wxString & name)
  {
    // is the list already in the memory?
    if (FindList (name))
      return lists [name]; // yeah!
    else
      return ReadSingleList (name); // no! read it from config.
  }


  /**
   * checks whether the list named @a name is already there
   *
   * @param name name of the list
   * @retval true list is there
   */
  bool
  FindList (const wxString & name)
  {
    wxArrayStringHashMap::iterator it = lists.find (name);

    // is the list already in the memory?
    if (it != lists.end ())
      return true;
    else
      return false;
  }


  /**
   * Writes the @a list named @a name back for later
   * storage
   *
   * @param name name of the list
   * @param list list of strings
   */
  void
  WriteList (const wxString & name, const wxArrayString & list)
  {
    lists [name] = list;
  }


  /**
   * Adds a new entry to the list @a name. If the
   * list doesnt exist it will be created. 
   *
   * If the entry doesnt exist it will be insert at the
   * first position in the array. 
   *
   * If the entry already exists it will be deleted from
   * the old position and insert to the first position
   * in the array.
   *
   * @param name name of the list
   * @param entry entry to add
   */
  void AddEntryToList (const wxString & name,
                       const wxString & entry)
  {
    // if list doesnt exist, read it
    if (!FindList (name))
      ReadSingleList (name);

    wxArrayString & list = lists [name];

    // try to find the entry
    int i = list.Index (entry);

    if (i != wxNOT_FOUND)
      list.Remove (i);

    // insert the string at the first position
    list.Insert (entry, 0);

    clean = false;
  }


  /**
   * Write all the lists to the configuration
   * (file or registry)
   *
   * @see WriteSingleList
   */
  void
  WriteAllLists ()
  {
    wxArrayStringHashMap::iterator  it;

    // cycle through all the lists
    for (it = lists.begin (); it != lists.end (); it++)
    {
      wxString name (it->first);
      wxArrayString list (it->second);

      WriteSingleList (name, list);
    }

    clean = true;
  }


private:


  /**
   * Write a single named list to the configuration
   *
   * @param name Name of the list
   * @param list list with entries
   */
  void
  WriteSingleList (const wxString & name, const wxArrayString & list)
  {
    // allow only maximal number
    size_t count = list.Count ();
    
    if (count > MAX_COUNT)
      count = MAX_COUNT;

    // first write 0, only if everything goes smooth
    // we wanna write the real value
    wxConfigBase * cfg = wxConfigBase::Get ();
    wxString countStr;
    countStr.Printf(CONF_COUNT_FMT, name);
    cfg->Write (countStr, 0);

    size_t index;
    for (index = 0; index < count; index++)
    {
      wxString valueStr;
      valueStr.Printf (CONF_VALUE_FMT, name, index);

      cfg->Write (valueStr, list[index]);
    }

    // no write real number of entries
    cfg->Write (countStr, (long)count);
  }


  /**
   * Reads a single named list from the configuration.
   *
   * @param name name of the list
   * @return list of values
   */
  wxArrayString &
  ReadSingleList (const wxString & name)
  {
    wxArrayString & list = lists [name];
    list.Empty ();

    // retrieve number of entries in the stored list
    // and limit the number
    wxConfigBase * cfg = wxConfigBase::Get ();
    wxString countStr;
    countStr.Printf(CONF_COUNT_FMT, name);

    long count;
    cfg->Read (countStr, &count, 0);
    if (count > MAX_COUNT)
      count = MAX_COUNT;

    // now read the list
    list.Alloc (count);
    size_t index;
    for (index=0; index<count; index++)
    {
      // read a single entry for the list
      wxString valueStr;
      valueStr.Printf (CONF_VALUE_FMT, name, index);
      wxString value ("");
      cfg->Read (valueStr, &value);

      // trim it and append it to the list (only if non-empty)
      TrimString (value);

      if (value.Length () > 0)
        list.Add (value);
    }

    clean = false;

    return list;
  }

};


HistoryManager::HistoryManager ()
 : m (new Data ())
{
}


HistoryManager::~HistoryManager ()
{
  delete m;
}


const wxArrayString &
HistoryManager::ReadList (const wxString & id)
{
  return m->ReadList (id);
}


void
HistoryManager::WriteList (const wxString & id, 
                           const wxArrayString & list)
{
  m->WriteList (id, list);
}


void
HistoryManager::AddEntryToList (const wxString & id,
                                const wxString & entry)
{
  m->AddEntryToList (id, entry);
}


void 
HistoryManager::Cleanup ()
{
  m->WriteAllLists ();
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
