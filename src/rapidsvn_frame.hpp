/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
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
#ifndef _RAPIDSVN_FRAME_HEADER_H_INCLUDED_
#define _RAPIDSVN_FRAME_HEADER_H_INCLUDED_

// wxWidgets
#include "wx/frame.h"
#include "wx/textctrl.h"
#include "wx/toolbar.h"
#include "wx/splitter.h"

// app
#include "action_worker.hpp"
#include "tracer.hpp"
#include "log_action.hpp"
#include "filelist_ctrl.hpp"
#include "folder_browser.hpp"
#include "utils.hpp"

#define SPLITTER_WINDOW   100

typedef enum
{
  ACTIVEPANE_FOLDER_BROWSER,
  ACTIVEPANE_FILELIST
}
ActivePane;


// forward declarations
class svn::Targets;
class wxFrame;

class RapidSvnFrame:public wxFrame
{
public:
  /**
   * Constructor that will be called by the App
   *
   * @param title Application title
   * @param locale The locale that's used by the app
   */
  RapidSvnFrame (const wxString & title,
                 const wxLocale & locale);
  virtual ~RapidSvnFrame ();

  /** Allow children to trim their popup menus using the frames logic */
  void
  TrimDisabledMenuItems (wxMenu & menu);

  /**
  * Change the active window, if the corresponding event is handled
  * in one of the sub-windows
  */
  void
  SetActivePane (ActivePane value);

private:
  /** disallow default constructor */
  RapidSvnFrame ();

  /** disallow copy constructor */
  RapidSvnFrame (const RapidSvnFrame &);

  void OnActivate (wxActivateEvent & event);

  // File menu
  void OnAddWcBookmark (wxCommandEvent & event);
  void OnAddRepoBookmark (wxCommandEvent & event);
  void OnRemoveBookmark (wxCommandEvent & event);
  void OnEditBookmark (wxCommandEvent & event);
  void OnQuit (wxCommandEvent & event);

  // View menu
  void OnPreferences (wxCommandEvent & event);
  void OnRefresh (wxCommandEvent & event);
  void OnColumnReset (wxCommandEvent & event);
  void OnColumn (wxCommandEvent & event);
  void OnIncludePath (wxCommandEvent & event);
  void OnSortAscending (wxCommandEvent & event);
  void OnColumnSorting (wxCommandEvent & event);
  void OnFlatView (wxCommandEvent & event);
  void OnRefreshWithUpdate (wxCommandEvent & event);
  void OnShowUnversioned (wxCommandEvent & event);
  void OnIgnoreExternals (wxCommandEvent & event);
  void OnStop (wxCommandEvent & event);

  // Query menu
  void OnLog (wxCommandEvent & event);
  void OnInfo (wxCommandEvent & event);

  // Modify menu
  void OnFileCommand (wxCommandEvent & event);

  // Extras menu
  void OnCleanup (wxCommandEvent & event);

  // Help menu
//  void OnHelp(wxCommandEvent & event);
  void OnHelpContents (wxCommandEvent & event);
  void OnHelpIndex (wxCommandEvent & event);
  void OnHelpStartupTips (wxCommandEvent & event);
  void OnAbout (wxCommandEvent & event);

  // toolbar events
  void OnToolEnter (wxCommandEvent & event);

  // Events from action threads
  void OnActionEvent (wxCommandEvent & event);

  // Folder Browser
  void AddWcBookmark ();
  void AddRepoBookmark ();
  void RemoveBookmark ();
  void EditBookmark ();
  void OnFolderBrowserSelChanged (wxTreeEvent & event);
  void OnFolderBrowserKeyDown (wxTreeEvent & event);
  void OnFileListSelected (wxListEvent & event);
  void OnLogin (wxCommandEvent & event);
  void OnLogout (wxCommandEvent & event);

  void UpdateCurrentPath ();
  void UpdateMenuSorting ();
  void UpdateMenuIncludePath ();
  void UpdateMenuAscending ();
  void SetIncludePathVisibility (bool flatMode);

  /**
   * Invoke the default action on the currently selected item(s)
   *
   * For folders, this is to open them within RapidSVN. For files, this is to
   * activate the default external editor.
   *
   * @return false if no items were found to invoke default action on
   */
  bool InvokeDefaultAction ();

  /** update the contents of the filelist */
  void UpdateFileList ();

  /** update the contents of the folder browser */
  void UpdateFolderBrowser ();

  // utility functions
  void ShowLog ();
  void ShowInfo ();

  // Enable/disable action menu items
  bool ValidateIDActionFlags (int id, unsigned int selectionActionFlags);
  void OnUpdateCommand (wxUpdateUIEvent & updateUIEvent);

  const svn::Targets
  GetActionTargets() const;

  /**
   * determine the properties of what has been selected
   * in terms of the relevant Action Flags.
   */
  unsigned int
  GetSelectionActionFlags () const;

  void DelEntries ();
  void MakeRevert ();
  void MakeResolve ();

  void MakeCopy ();
  void Rename ();
  void Mkdir ();
  void Merge ();
  void Contents ();
  void ShowPreferences ();

  FolderBrowser *GetFolderBrowser ()
  {
    return m_folder_browser;
  }

  void Perform (Action * action);

private:
  struct Data;

  /** hide implementation data */
  Data * m;

  ActionWorker * m_actionWorker;

  FolderBrowser * m_folder_browser;
  FileListCtrl *m_listCtrl;

  wxSplitterWindow *m_horiz_splitter;
  wxSplitterWindow *m_vert_splitter;

  wxPanel *m_info_panel;

  wxTextCtrl *m_log;
  EventTracer *m_logTracer;

  size_t m_toolbar_rows;        // 1 or 2 only (toolbar rows)

  wxString m_currentPath;
  wxString m_title;
  svn::Context * m_context;
  ActivePane m_activePane;

  /**
   * add message to log window
   *
   * @param msg message to add
   */
  void
  Trace (wxString msg) ;

private:
  DECLARE_EVENT_TABLE ()
};

const int ID_TOOLBAR = 500;

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
