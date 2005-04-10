/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _CHECKOUT_DLG_H_INCLUDED_
#define _CHECKOUT_DLG_H_INCLUDED_

// wxwindows
#include "wx/wx.h"

// app
#include "checkout_data.hpp"

class CheckoutDlg : public wxDialog
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   */
  CheckoutDlg (wxWindow *parent);

  /**
   * destructor
   */
  virtual ~CheckoutDlg ();

  /**
   * returns the checkout data
   */
  const CheckoutData &
  GetData () const;

private:
  /** hide implementation details */
  struct Data;
  Data * m;

  void 
  OnBrowse (wxCommandEvent & event);

  void 
  OnText (wxCommandEvent & event);

  void 
  OnHelp (wxCommandEvent & event);
  
  void 
  OnUseLatest (wxCommandEvent & event);

  void 
  InitDialog ();

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
