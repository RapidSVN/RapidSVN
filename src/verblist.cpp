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
#include <exception>

#include "verblist.hpp"

/****************************************************************************/

void VerbList::InitFromDocument (const wxString &)
{
}

size_t VerbList::GetCount () const
{
  return 0;
}

const char *VerbList::GetName (size_t) const
{
  throw std::out_of_range("Index out of range");
}

void VerbList::Launch (size_t) const
{
  throw std::out_of_range("Index out of range");
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
