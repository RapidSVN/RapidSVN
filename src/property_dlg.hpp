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
#include "wx/grid.h"
#include "wx/dialog.h"
#include "wx/window.h"

// svncpp
#include "svncpp/property.hpp"


class PropertyGrid : public wxGrid
{
public:
  PropertyGrid (wxWindow * parent, const svn::Property & property);
  void FillList ();

private:
  const svn::Property & m_property;
  void InitializeData ();
  
  DECLARE_EVENT_TABLE ()
};

class PropertyDlg : public wxDialog
{
public:
  PropertyDlg (wxWindow * parent, svn::Context * context, 
               const svn::Path & target);
private:
  svn::Property m_property;
  PropertyGrid * propGrid;

  void InitializeData ();

  /**
   * Save all of the valid property key/value pairs.
   */
  void SaveData ();

  /**
   * Remove all of the property key/value pairs that do not 
   * exist any more.
   */
  void RemoveData ();

  void OnClose (wxCommandEvent & event);
  void OnOK (wxCommandEvent & event);
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
