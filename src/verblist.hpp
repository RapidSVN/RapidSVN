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
#ifndef _VERBLIST_HPP_INCLUDED_
#define _VERBLIST_HPP_INCLUDED_

#include "wx/filename.h"

/**
 * IVerbList abstracts a list of editors/handlers for a given file, with the
 * default one listed first.
 *
 * First version by jorgenhs@netcom.no.
 *
 * NOTE: All exceptions thrown by the methods of this interface must be
 * portable
 *
 * Declared as struct (not class) to avoid "public:" label
 */
struct IVerbList
{
  /**
   * Assemble list of verbs based on the given document
   *
   * @throw std::exception on unexpected errors
   */
  virtual void InitFromDocument (const wxString &document_path) = 0;

  /**
   * @return Number of verbs in list
   */
  virtual size_t GetCount() const = 0;

  /**
   * @return Name of verb with the given index
   */
  virtual const char *GetName(size_t index) const = 0;

  /**
   * Launches the verb with the given index on the document on which the verb
   * list is based
   */
  virtual void Launch(size_t index) const = 0;
};

/**
 * Include implementation based on platform
 */
#ifdef _WIN32
#include "verblist_win32.hpp"
#else
#include "verblist_stubbed.hpp"
#endif

#endif // _VERBLIST_HPP_INCLUDED_
