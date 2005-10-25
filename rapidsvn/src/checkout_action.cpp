/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"

// app
#include "checkout_action.hpp"
#include "checkout_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

CheckoutAction::CheckoutAction (wxWindow * parent)
  : Action (parent, _("Checkout"), GetBaseFlags ())
{
}

bool
CheckoutAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  CheckoutDlg dlg (GetParent ());
  if( dlg.ShowModal () != wxID_OK )
  {
    return false;
  }
  m_data = dlg.GetData ();
  return true;
}

bool
CheckoutAction::Perform ()
{
  svn::Client client (GetContext ());

  TrimString(m_data.DestFolder);
  UnixPath(m_data.DestFolder);
  TrimString(m_data.RepUrl);
  
  long revnum=-1;
  
  svn::Revision revision (svn::Revision::HEAD);

  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty ())
    {
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
    }
    revision = svn::Revision(revnum);
  }
  
  wxSetWorkingDirectory (m_data.DestFolder);
  std::string DestFolderUtf8;
  LocalToUtf8 (m_data.DestFolder, DestFolderUtf8);
  client.checkout (m_data.RepUrl.mb_str (), 
                   svn::Path (DestFolderUtf8), 
                   revision, m_data.Recursive);

  // now post event to add bookmark to bookmarks
  if (m_data.Bookmarks)
  {
    wxCommandEvent event = CreateActionEvent (TOKEN_ADD_BOOKMARK);
    event.SetString (m_data.DestFolder);

    PostEvent (event);
  }
 
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
