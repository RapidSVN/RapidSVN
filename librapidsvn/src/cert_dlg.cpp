/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"

// app
#include "cert_dlg.hpp"
#include "utils.hpp"

CertDlg::CertDlg(wxWindow * parent,
                 const svn::ContextListener::SslServerTrustData & trustData)
  : CertDlgBase(parent), m_answer(svn::ContextListener::DONT_ACCEPT),
    m_acceptedFailures(trustData.failures)
{
  m_staticTitle->SetLabel(_("There were errors validating the server certificate.\nDo you want to trust this certificate?"));

  wxString failureStr;

  const struct
  {
    apr_uint32_t failure;
    wxString descr;
  } CERT_FAILURES [] =
  {
    {SVN_AUTH_SSL_UNKNOWNCA, _("- The certificate is not issued by a trusted authority.\n  Use the fingerprint to validate the certificate manually!")},
    {SVN_AUTH_SSL_CNMISMATCH, _("- The certificate hostname does not match.")},
    {SVN_AUTH_SSL_NOTYETVALID, _("- The certificate is not yet valid.")},
    {SVN_AUTH_SSL_EXPIRED, _("- The certificate has expired.")},
    {SVN_AUTH_SSL_OTHER, _("- The certificate has an unknown error.")}
  };

  const int count = sizeof(CERT_FAILURES)/sizeof(CERT_FAILURES[0]);
  for (int i=0; i < count; i++)
  {
    if ((CERT_FAILURES[i].failure & trustData.failures) != 0)
    {
      failureStr += CERT_FAILURES[i].descr;
      failureStr += wxT("\n");
    }
  }

  m_staticFailures->SetLabel(failureStr);

  wxString realm(Utf8ToLocal(trustData.realm));
  wxString failuresTitle;
  failuresTitle.Printf(_("Error validating server certificate for '%s':"),
                       realm.c_str());
  m_sizerFailures->GetStaticBox()->SetLabel(failuresTitle);

  m_staticHostname->SetLabel(Utf8ToLocal(trustData.hostname.c_str()));
  m_staticIssuer->SetLabel(Utf8ToLocal(trustData.issuerDName.c_str()));
  m_staticValidFrom->SetLabel(Utf8ToLocal(trustData.validFrom.c_str()));
  m_staticValidUntil->SetLabel(Utf8ToLocal(trustData.validUntil.c_str()));
  m_staticFingerprint->SetLabel(Utf8ToLocal(trustData.fingerprint.c_str()));
}


CertDlg::~CertDlg()
{
}


void
CertDlg::OnPerm(wxCommandEvent & WXUNUSED(event))
{
  m_answer = svn::ContextListener::ACCEPT_PERMANENTLY;
  EndModal(wxID_OK);
}


void
CertDlg::OnTemp(wxCommandEvent & WXUNUSED(event))
{
  m_answer = svn::ContextListener::ACCEPT_TEMPORARILY;
  EndModal(wxID_OK);
}


svn::ContextListener::SslServerTrustAnswer
CertDlg::Answer() const
{
  return m_answer;
}


wxUint32
CertDlg::AcceptedFailures() const
{
  return m_acceptedFailures;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
