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
#ifndef _CHECKOUT_ACTION_H_INCLUDED_
#define _CHECKOUT_ACTION_H_INCLUDED_

// app
#include "action.hpp"
#include "checkout_data.hpp"

class CheckoutAction:public Action
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   * @param tracer
   * @param own own tracer?
   */
  CheckoutAction (wxWindow * parent, 
                  Tracer * tracer);

  virtual bool Perform ();

  virtual bool Prepare ();

private:
  CheckoutData m_data;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
