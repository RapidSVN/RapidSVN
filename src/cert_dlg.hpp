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
#ifndef _CERT_DLG_H_INCLUDED_
#define _CERT_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// svncpp
#include "svncpp/context_listener.hpp"

// forward declarations
namespace svn
{
  struct ContextListener::SslServerTrustData;
};

class CertDlg : public wxDialog
{
public:
  CertDlg (wxWindow * parent,
    const svn::ContextListener::SslServerTrustData & data);

  virtual ~CertDlg ();

  svn::ContextListener::SslServerTrustAnswer
  Answer () const;

  wxUint32
  AcceptedFailures () const;

private:
  struct Data;
  Data * m;

private:
  DECLARE_EVENT_TABLE ()

  void OnPerm (wxCommandEvent & event);
  void OnTemp (wxCommandEvent & event);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
