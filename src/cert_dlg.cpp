/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
 * along with this program (in the file GPL.txt.  
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

enum
{
  ID_PERM = wxID_HIGHEST,
  ID_TEMP
};


BEGIN_EVENT_TABLE(CertDlg, wxDialog)
  EVT_BUTTON(ID_PERM, CertDlg::OnPerm)
  EVT_BUTTON(ID_TEMP, CertDlg::OnTemp)
END_EVENT_TABLE()


typedef struct
{
  wxUint32 failure;
  const wxChar * descr;
} FailureEntry;


struct CertDlg::Data
{
public:
  svn::ContextListener::SslServerTrustAnswer answer;
  wxUint32 acceptedFailures;

  Data(wxUint32 acceptedFailures_)
      : answer(svn::ContextListener::DONT_ACCEPT), acceptedFailures(acceptedFailures_)
  {
  }
};

CertDlg::CertDlg(wxWindow * parent,
                 const svn::ContextListener::SslServerTrustData & trustData)
    : wxDialog(parent, -1, _("SSL Certificate"), wxDefaultPosition),
    m(new Data(trustData.failures))
{
  wxStaticText * labelTitle = new wxStaticText(
    this, -1, _("There were errors validating the server certificate.\nDo you want to trust this certificate?"));

  wxString failureStr;

  static const FailureEntry CERT_FAILURES [] =
  {
    {SVN_AUTH_SSL_UNKNOWNCA   , _("- The certificate is not issued by a trusted authority.\n  Use the fingerprint to validate the certificate manually!")},
    {SVN_AUTH_SSL_CNMISMATCH  , _("- The certificate hostname does not match.")},
    {SVN_AUTH_SSL_NOTYETVALID , _("- The certificate is not yet valid.")},
    {SVN_AUTH_SSL_EXPIRED     , _("- The certificate has expired.")},
    {SVN_AUTH_SSL_OTHER       , _("- The certificate has an unknown error.")}
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

  wxStaticText * labelFailure = new wxStaticText(
    this, -1, failureStr);

  wxString failBoxLabel, tmp(Utf8ToLocal(trustData.realm));
  failBoxLabel.Printf(_("Error validating server certificate for '%s':"),
                      tmp.c_str());
  wxStaticBox * failBox = new wxStaticBox(
    this, -1, failBoxLabel);
  wxStaticBoxSizer * failBoxSizer =
    new wxStaticBoxSizer(failBox, wxVERTICAL);
  failBoxSizer->Add(labelFailure, 0, wxALL, 10);

  wxFlexGridSizer * certSizer = new wxFlexGridSizer(2, 10, 10);
  certSizer->Add(new wxStaticText(
                   this, -1, _("Hostname:")));
  certSizer->Add(new wxStaticText(
                   this, -1, Utf8ToLocal(trustData.hostname.c_str())));
  certSizer->Add(new wxStaticText(
                   this, -1, _("Issue:")));
  certSizer->Add(new wxStaticText(
                   this, -1, Utf8ToLocal(trustData.issuerDName.c_str())));
  certSizer->Add(new wxStaticText(
                   this, -1, _("Valid from:")));
  certSizer->Add(new wxStaticText(
                   this, -1, Utf8ToLocal(trustData.validFrom.c_str())));
  certSizer->Add(new wxStaticText(
                   this, -1, _("Valid until:")));
  certSizer->Add(new wxStaticText(
                   this, -1, Utf8ToLocal(trustData.validUntil.c_str())));
  certSizer->Add(new wxStaticText(
                   this, -1, _("Fingerprint:")));
  certSizer->Add(new wxStaticText(
                   this, -1, Utf8ToLocal(trustData.fingerprint.c_str())));

  wxStaticBox * certBox = new wxStaticBox(
    this, -1, _("Certificate Information:"));
  wxStaticBoxSizer * certBoxSizer =
    new wxStaticBoxSizer(certBox, wxVERTICAL);
  certBoxSizer->Add(certSizer);

  wxBoxSizer * buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(new wxButton(this, ID_PERM, _("&Permanently")),
                   0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, ID_TEMP, _("&Temporarily")),
                   0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")),
                   0, wxALL, 5);

  wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(labelTitle, 0, wxALL, 5);
  mainSizer->Add(failBoxSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add(certBoxSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add(buttonSizer, 0,
                 wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);

  CentreOnParent();
}


CertDlg::~CertDlg()
{
  delete m;
}


void
CertDlg::OnPerm(wxCommandEvent & event)
{
  m->answer = svn::ContextListener::ACCEPT_PERMANENTLY;
  Close(true);
}


void
CertDlg::OnTemp(wxCommandEvent & event)
{
  m->answer = svn::ContextListener::ACCEPT_TEMPORARILY;
  Close(true);
}


svn::ContextListener::SslServerTrustAnswer
CertDlg::Answer() const
{
  return m->answer;
}


wxUint32
CertDlg::AcceptedFailures() const
{
  return m->acceptedFailures;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
