/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _IDS_H_INCLUDED_
#define _IDS_H_INCLUDED_

// wxwindows
#include "wx/defs.h"

/*
 * Menu commands IDs
 */
enum
{
  ID_Quit = wxID_HIGHEST,
  ID_About,
  ID_Refresh,
  ID_Import,
  ID_Checkout,
  ID_Copy,
  ID_Rename,
  ID_Combo,
  ID_Update,
  ID_Commit,
  ID_Add,
  ID_Del,
  ID_Revert,
  ID_Log,
  ID_Info,
  ID_Resolve,
  ID_Merge,
  ID_Contents,
  ID_Mkdir,
  ID_Switch,
  ID_Preferences,
  ID_Property,
  ID_AddProject,
  ID_RemoveProject,
  ID_MoveTo,
  ID_CopyTo,
  ID_RenameHere,
  ID_CopyHere,
  ID_Cleanup,
  ID_Delete,
  ACTION_EVENT,                 // this one gets sent from the action threads
// Ids used for communication events between threads and frame
  TOKEN_BASE,
  TOKEN_INFO,
  TOKEN_SVN_INTERNAL_ERROR,
  TOKEN_VSVN_INTERNAL_ERROR,
  TOKEN_ACTION_END,
  FILELIST_CTRL,
  FOLDER_BROWSER,
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
