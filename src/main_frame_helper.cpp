/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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
#include "wx/toolbar.h"

// app
#include "ids.hpp"
#include "main_frame_helper.hpp"
#include "utils.hpp"

// bitmaps
#include "res/bitmaps/refresh.png.h"
#include "res/bitmaps/update.png.h"
#include "res/bitmaps/commit.png.h"
#include "res/bitmaps/add.png.h"
#include "res/bitmaps/delete.png.h"
#include "res/bitmaps/revert.png.h"
#include "res/bitmaps/resolve.png.h"
#include "res/bitmaps/log.png.h"
#include "res/bitmaps/info.png.h"
#include "res/bitmaps/stop.png.h"

#include "res/bitmaps/flat_mode.png.h"
#include "res/bitmaps/nonsvn_file.png.h"
#include "res/bitmaps/normal_file.png.h"
#include "res/bitmaps/modified_file.png.h"
#include "res/bitmaps/conflicted_file.png.h"


static void
AddActionTools(wxToolBarBase *toolBar)
{
  wxASSERT(toolBar);
  
  toolBar->AddTool(ID_Add,
		   EMBEDDED_BITMAP(add_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Add selected"),
		   _("Put files and directories under revision control"));
  
  toolBar->AddTool(ID_Delete,
		   EMBEDDED_BITMAP(delete_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Delete selected"),
		   _("Delete files and directories from version control"));
  
  toolBar->AddTool(ID_Update,
		   EMBEDDED_BITMAP(update_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Update selected"),
		   _("Bring changes from the repository into the working copy"));
  
  toolBar->AddTool(ID_Commit,
		   EMBEDDED_BITMAP(commit_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Commit selected"),
		   _("Send changes from your working copy to the repository"));
  
  toolBar->AddTool(ID_Revert,
		   EMBEDDED_BITMAP(revert_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Revert selected"),
		   _("Restore pristine working copy file (undo all local edits)"));
  
  toolBar->AddTool(ID_Resolve,
		   EMBEDDED_BITMAP(resolve_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Resolve selected"),
		   _("Remove 'conflicted' state on working copy files or directories"));
  
  toolBar->AddSeparator();
}


static void
AddInfoTools(wxToolBarBase *toolBar)
{
  toolBar->AddTool(ID_Info,
		   EMBEDDED_BITMAP(info_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Info selected"),
		   _("Display info about selected entries"));
  
  toolBar->AddTool(ID_Log,
		   EMBEDDED_BITMAP(log_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Log selected"),
		   _("Show the log messages for the selected entries"));
  
  toolBar->AddSeparator();
}


void 
CreateMainToolBar(wxFrame * frame)
{
  wxASSERT(0 != frame);
  
  // delete the old toolbar
  wxToolBarBase * toolBar = frame->GetToolBar();
  delete toolBar;
  frame->SetToolBar(NULL);

  // create the new toolbar
  toolBar = frame->CreateToolBar(wxNO_BORDER | wxTB_FLAT | 
				 wxTB_DOCKABLE | wxTB_HORIZONTAL);
  toolBar->SetMargins(4, 4);

  AddActionTools(toolBar);
  AddInfoTools(toolBar);
  
  toolBar->AddCheckTool(ID_Flat,
			wxEmptyString,
			EMBEDDED_BITMAP(flat_mode_png),
			wxNullBitmap,
			_("Show subdirectories"),
			_("Show entries in subdirectories"));
  toolBar->AddCheckTool(ID_ShowUnversioned,
			wxEmptyString,
			EMBEDDED_BITMAP(nonsvn_file_png),
			wxNullBitmap,
			_("Show unversioned entries"),
			_("Display unversioned files/directories"));
  toolBar->AddCheckTool(ID_ShowUnmodified,
			wxEmptyString,
			EMBEDDED_BITMAP(normal_file_png),
			wxNullBitmap,
			_("Show unmodified entries"),
			_("Display unmodified files/directories"));
  toolBar->AddCheckTool(ID_ShowModified,
			wxEmptyString,
			EMBEDDED_BITMAP(modified_file_png),
			wxNullBitmap,
			_("Show modified entries"),
			_("Display modified files/directories"));
  toolBar->AddCheckTool(ID_ShowConflicted,
			wxEmptyString,
			EMBEDDED_BITMAP(conflicted_file_png),
			wxNullBitmap,
			_("Show conflicted entries"),
			_("Display conflicted files/directories"));
  toolBar->AddSeparator();

  // Set toolbar refresh button.
  toolBar->AddTool(ID_Refresh,
		   EMBEDDED_BITMAP(refresh_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   (wxObject *) NULL,
		   _("Refresh"),
		   _("Refresh the file list"));
  
  toolBar->AddSeparator();
  
  // STOP button
  toolBar->AddTool(ID_Stop,
		   EMBEDDED_BITMAP(stop_png),
		   wxNullBitmap,
		   FALSE,
		   -1,
		   -1,
		   0,
		   _("Stop"),
		   _("Stop the current action"));
  
  // After adding the buttons to the toolbar,
  // must call Realize() to reflect the changes.
  toolBar->Realize();
  toolBar->SetRows(1);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
