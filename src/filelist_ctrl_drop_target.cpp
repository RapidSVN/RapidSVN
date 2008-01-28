/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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

// wx
#include "wx/wx.h"

// svncpp
#include "svncpp/status.hpp"

// app
#include "filelist_ctrl_drop_target.hpp"
#include "filelist_ctrl.hpp"
#include "folder_browser.hpp"
#include "folder_item_data.hpp"

#include "utils.hpp"

FileListCtrlDropTarget::FileListCtrlDropTarget (FolderBrowser * browser, 
                                                FileListCtrl * parent)
  : m_browser (browser), m_parent (parent)
{
}

wxString
FileListCtrlDropTarget::GetDestinationPath (const wxPoint & point)
{
  wxString path = wxEmptyString;
  int flags = wxLIST_HITTEST_ONITEM;
  long destinationItem = m_parent->HitTest (point, flags);
  if (destinationItem != wxNOT_FOUND)  // Did we land on an item??
  {
    svn::Status * status = 
      (svn::Status*)m_parent->GetItemData (destinationItem);
    if (status->isVersioned () && 
        (status->entry ().kind() == svn_node_dir))
    {
      path = Utf8ToLocal (status->path ());
    }
  }

  // If the path is still empty, then the hit test was on an 
  // unversioned item, it wasn't on an item at all, or it 
  // wasn't a directory:
  // Get the path from the folder browser
  if (path.IsEmpty ())
  {
    // Get the path from the folder browser
    path = m_browser->GetSelection ()->getPath ();
  }

  return path;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
