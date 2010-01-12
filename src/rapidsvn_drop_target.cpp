/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
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

// wx
#include "wx/wx.h"

// app
#include "rapidsvn_drop_target.hpp"
#include "drag_n_drop_data.hpp"
#include "utils.hpp"
#include "rapidsvn_app.hpp"
#include "ids.hpp"
#include "action_event.hpp"

bool
RapidSvnDropTarget::OnDropFiles(wxCoord x, wxCoord y,
                                const wxArrayString & filenames)
{
  m_destination = GetDestinationPath(wxPoint(x, y));

  if (!m_destination.IsEmpty())
  {
    if (filenames.GetCount() > 0)
    {
      DragAndDropData* data = new DragAndDropData();
      data->m_files = filenames;
      data->m_destination = m_destination;
      ActionEvent::Post(wxGetApp().GetTopWindow(),
                        TOKEN_DRAG_N_DROP, data);
    }
  }
  else
  {
    wxMessageBox(_("Unknown destination path"));
    return false;
  }

  return false;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
