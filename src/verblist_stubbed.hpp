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
#ifndef _VERBLIST_STUBBED_HPP_INCLUDED_
#define _VERBLIST_STUBBED_HPP_INCLUDED_

/**
 * Stubbed NOP IVerbList implementation. 
 * May be used as a starting point for porting to new platforms
 */
class VerbList : public IVerbList
{
public:
  /**
   * Construct an empty verb list
   */
  VerbList ();

  /** 
   * @see IVerbList
   */ 
  void InitFromDocument (const wxString &document_path);

  /** 
   * @see IVerbList
   */ 
  size_t GetCount () const;

  /** 
   * @see IVerbList
   */ 
  const char *GetName (size_t index) const;

  /** 
   * @see IVerbList
   */ 
  void Launch (size_t index) const;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
