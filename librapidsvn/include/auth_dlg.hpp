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
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _AUTH_DLG_H_INCLUDED_
#define _AUTH_DLG_H_INCLUDED_

#include "rapidsvn_generated.h"

class AuthDlg:public AuthDlgBase
{
public:
  enum
  {
    HIDE_USERNAME=0x1,
    HIDE_PASSWORD=0x2
  };

  AuthDlg(wxWindow * parent,
          const wxString & username = wxEmptyString,
          const wxString & password = wxEmptyString,
          int flags = 0);

  virtual ~AuthDlg();

  const wxString &
  GetUsername() const;

  const wxString &
  GetPassword() const;

private:
  wxString m_username;
  wxString m_password;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
