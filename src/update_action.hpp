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
#ifndef _UPDATE_ACTION_H_INCLUDED_
#define _UPDATE_ACTION_H_INCLUDED_

// wxwindows
//#include "wx/frame.h"

//// svncpp
//#include "svncpp/targets.hpp"

// app
//#include "action_thread.hpp"
//REMOVE#include "file_action.hpp"
//#include "update_dlg.hpp"
#include "action.hpp"
#include "update_data.hpp"

class svn::Targets;
class Tracer;
class wxWindow;

class UpdateAction:public Action
{
public:
  UpdateAction (wxWindow * parent, const svn::Targets & targets, 
                wxString & path, Tracer * tr, bool owns);
  bool Perform ();
  bool Prepare ();

private:
  svn::Targets m_targets;
  wxString m_path;
  UpdateData m_data;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
