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
#ifndef _COMMIT_ACTION_H_INCLUDED_
#define _COMMIT_ACTION_H_INCLUDED_

// app
#include "action.hpp"
#include "commit_data.hpp"

// forward declarations
namespace svn
{
  class Targets;
}
class Tracer;

class CommitAction : public Action
{
public:
  CommitAction (wxWindow * parent);

  virtual bool Perform ();
  virtual bool Prepare ();

private:
  CommitData m_data;

  // hide default and copy constructor
  CommitAction ();
  CommitAction (const CommitAction &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
