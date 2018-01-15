/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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

// svncpp
#include "svncpp/targets.hpp"

// app
#include "action.hpp"
#include "action_factory.hpp"
#include "ids.hpp"

// data
#include "diff_data.hpp"

// actions
#include "add_action.hpp"
#include "add_recursive_action.hpp"
#include "checkout_action.hpp"
#include "cleanup_action.hpp"
#include "create_repos_action.hpp"
#include "commit_action.hpp"
#include "delete_action.hpp"
#include "diff_action.hpp"
#include "export_action.hpp"
#include "external_program_action.hpp"
#include "get_action.hpp"
#include "import_action.hpp"
#include "log_action.hpp"
#include "lock_action.hpp"
#include "merge_action.hpp"
#include "mkdir_action.hpp"
#include "move_action.hpp"
#include "property_action.hpp"
#include "rename_action.hpp"
#include "resolve_action.hpp"
#include "userresolve_action.hpp"
#include "revert_action.hpp"
#include "switch_action.hpp"
#include "unlock_action.hpp"
#include "update_action.hpp"
#include "upgrade_action.hpp"
#include "view_action.hpp"
#include "annotate_action.hpp"
#include "drag_n_drop_action.hpp"
#include "ignore_action.hpp"

bool
ActionFactory::CheckIdForStatusSel(int id, const svn::StatusSel & statusSel)
{
  bool result = true;
  if ((id >= ID_Verb_Min) && (id <= ID_Verb_Max))
    return ExternalProgramAction::CheckStatusSel(statusSel);

  switch (id)
  {
  case ID_Explore:
    // Special case of ExternalProgramAction - needs to be a working copy, not just single target
    result = ExternalProgramAction::CheckStatusSel(statusSel);
    break;

  case ID_Default_Action:
    result = ExternalProgramAction::CheckStatusSel(statusSel);
    break;

  case ID_Update:
    result = UpdateAction::CheckStatusSel(statusSel);
    break;

  case ID_Commit:
    result = CommitAction::CheckStatusSel(statusSel);
    break;

  case ID_Add:
    result = AddAction::CheckStatusSel(statusSel);
    break;

  case ID_AddRecursive:
    result = AddRecursiveAction::CheckStatusSel(statusSel);
    break;

  case ID_Import:
    result = ImportAction::CheckStatusSel(statusSel);
    break;

  case ID_Export:
    result = ExportAction::CheckStatusSel(statusSel);
    break;

  case ID_Checkout:
    result = CheckoutAction::CheckStatusSel(statusSel);
    break;

  case ID_Cleanup:
    result = CleanupAction::CheckStatusSel(statusSel);
    break;

  case ID_Upgrade:
    result = UpgradeAction::CheckStatusSel(statusSel);
    break;

  case ID_Lock:
    result = LockAction::CheckStatusSel(statusSel);
    break;

  case ID_Log:
    result = LogAction::CheckStatusSel(statusSel);
    break;

  case ID_Revert:
    result = RevertAction::CheckStatusSel(statusSel);
    break;

  case ID_UserResolve:
    result = UserResolveAction::CheckStatusSel(statusSel);
    break;

  case ID_Resolve:
    result = ResolveAction::CheckStatusSel(statusSel);
    break;

  case ID_Delete:
    result = DeleteAction::CheckStatusSel(statusSel);
    break;

  case ID_Copy:
    result = MoveAction::CheckStatusSel(statusSel);
    break;

  case ID_Move:
    result = MoveAction::CheckStatusSel(statusSel);
    break;

  case ID_Mkdir:
    result = MkdirAction::CheckStatusSel(statusSel);
    break;

  case ID_Merge:
    result = MergeAction::CheckStatusSel(statusSel);
    break;

  case ID_Property:
    result = PropertyAction::CheckStatusSel(statusSel);
    break;

  case ID_Rename:
    result = RenameAction::CheckStatusSel(statusSel);
    break;

  case ID_Switch:
    result = SwitchAction::CheckStatusSel(statusSel);
    break;

  case ID_Diff:
  case ID_DiffBase:
  case ID_DiffHead:
    result = DiffAction::CheckStatusSel(statusSel);
    break;

  case ID_Info:
    // Not actually part of the Action hierarchy, but here for completeness
    result = true;
    break;

  case ID_Unlock:
    result = UnlockAction::CheckStatusSel(statusSel);
    break;

  case ID_Edit:
    result = ViewAction::CheckStatusSel(statusSel);
    break;

  case ID_Annotate:
    result = AnnotateAction::CheckStatusSel(statusSel);
    break;
  case ID_Ignore:
    result = IgnoreAction::CheckStatusSel(statusSel);
    break;
  case ID_CreateRepository:
    result = CreateRepositoryAction::CheckStatusSel(statusSel);
  };

  return result;
}


Action *
ActionFactory::CreateAction(wxWindow * parent, int id)
{
  Action* action = NULL;

  if ((id >= ID_Verb_Min) && (id <= ID_Verb_Max))
  {
    action = new ExternalProgramAction(parent, id - ID_Verb_Min, false);
  }
  else
  {
    switch (id)
    {
    case ID_Explore:
      action = new ExternalProgramAction(parent, -1, true);
      break;

    case ID_Update:
      action = new UpdateAction(parent);
      break;

    case ID_Commit:
      action = new CommitAction(parent);
      break;

    case ID_Add:
      action = new AddAction(parent);
      break;

    case ID_AddRecursive:
      action = new AddRecursiveAction(parent);
      break;

    case ID_Import:
      action = new ImportAction(parent);
      break;

    case ID_Export:
      action = new ExportAction(parent);
      break;

    case ID_Checkout:
      action = new CheckoutAction(parent);
      break;

    case ID_Cleanup:
      action = new CleanupAction(parent);
      break;

    case ID_Upgrade:
      action = new UpgradeAction(parent);
      break;

    case ID_Lock:
      action = new LockAction(parent);
      break;

    case ID_Log:
      action = new LogAction(parent);
      break;

    case ID_LogNext:
    {
      LogNextData data;
      action = new LogNextAction(parent, data);
      break;
    }

    case ID_Revert:
      action = new RevertAction(parent);
      break;

    case ID_UserResolve:
      action = new UserResolveAction(parent);
      break;

    case ID_Resolve:
      action = new ResolveAction(parent);
      break;

    case ID_Delete:
      action = new DeleteAction(parent);
      break;

    case ID_Copy:
      action = new MoveAction(parent, MOVE_COPY);
      break;

    case ID_Move:
      action = new MoveAction(parent, MOVE_MOVE);
      break;

    case ID_Merge:
      action = new MergeAction(parent);
      break;

    case ID_Property:
      action = new PropertyAction(parent);
      break;

    case ID_Rename:
      action = new RenameAction(parent);
      break;

    case ID_Switch:
      action = new SwitchAction(parent);
      break;

    case ID_Diff:
      action = new DiffAction(parent);
      break;

    case ID_DiffBase:
    {
      DiffData data(svn::Revision::BASE);
      action = new DiffAction(parent, data);
      break;
    }

    case ID_DiffHead:
    {
      DiffData data(svn::Revision::HEAD);
      action = new DiffAction(parent, data);
      break;
    }

    case ID_Unlock:
      action = new UnlockAction(parent);
      break;

    case ID_Edit:
      action = new ViewAction(parent);
      break;

    case ID_Annotate:
    {
      AnnotateData data;
      action = new AnnotateAction(parent, data);
      break;
    }
    case ID_Ignore:
      action = new IgnoreAction(parent);
      break;

    case ID_CreateRepository:
      action = new CreateRepositoryAction(parent);
    }
  }

  return action;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
