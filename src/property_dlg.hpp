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

#ifndef _PROPERTY_DLG_H_INCLUDED_
#define _PROPERTY_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"
#include "wx/window.h"

// svncpp
#include "svncpp/property.hpp"

// app
#include "listed_dlg.hpp"

class PropertyDlg : public ListEditorDlg
{
public:
  PropertyDlg (wxWindow * parent, svn::Context * context, 
               const svn::Path & target);

  /**
   * destructor
   */
  virtual ~PropertyDlg ();


protected:
  virtual void
  ReadFromGrid ();

  virtual void
  WriteToGrid ();

private:
  struct Data;
  Data * m;

  /**
   * disallow default constructor
   */
  PropertyDlg ();

  /**
   * disallow copy constructor
   */
  PropertyDlg (const PropertyDlg &);

  /**
   * disallow assignment operator
   */
  PropertyDlg operator & (const PropertyDlg &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
