/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _PROPERTY_ACTION_H_INCLUDED_
#define _PROPERTY_ACTION_H_INCLUDED_

#include "action_thread.hpp"
#include "property_dlg.hpp"

class PropertyAction : public ActionThread
{
private:
  wxFrame *thisframe;
  const char * _target;
  PropertyDlg * propDialog;

public:
  PropertyAction (wxFrame * frame,Tracer * tr, const char * target);

  void Perform ();
  void *Entry ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
