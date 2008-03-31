/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#include "wx/confbase.h"
#include "wx/wx.h"
#include "wx/filename.h"
#include <wx/tipdlg.h>

// svncpp
#include "svncpp/apr.hpp"
#include "svncpp/context.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/status_selection.hpp"
#include "svncpp/targets.hpp"
#include "svncpp/url.hpp"
#include "svncpp/wc.hpp"

// app
#include "rapidsvn_app.hpp"
#include "columns.hpp"
#include "config.hpp"
#include "destination_dlg.hpp"
#include "diff_data.hpp"
#include "get_data.hpp"
#include "ids.hpp"
#include "file_info.hpp"
#include "listener.hpp"
#include "folder_item_data.hpp"
#include "folder_browser.hpp"
#include "filelist_ctrl.hpp"
#include "annotate_data.hpp"
#include "log_data.hpp"
#include "drag_n_drop_data.hpp"
#include "action_factory.hpp"
#include "update_data.hpp"

// actions
#include "view_action.hpp"
#include "get_action.hpp"
#include "diff_action.hpp"
#include "merge_action.hpp"
#include "annotate_action.hpp"
#include "drag_n_drop_action.hpp"
#include "external_program_action.hpp"
#include "mkdir_action.hpp"

#ifdef USE_SIMPLE_WORKER
#include "simple_worker.hpp"
#else
#include "threaded_worker.hpp"
#endif


// dialogs
#include "about_dlg.hpp"
#include "auth_dlg.hpp"
#include "report_dlg.hpp"
#include "preferences.hpp"
#include "preferences_dlg.hpp"
#include "update_dlg.hpp"
#include "log_dlg.hpp"

#include "rapidsvn_frame.hpp"

#include "filelist_ctrl_drop_target.hpp"
#include "folder_browser_drop_target.hpp"

// controls
#include "proportional_splitter.hpp"

// Bitmaps
#include "res/bitmaps/rapidsvn_16x16.xpm"
#include "res/bitmaps/rapidsvn_32x32.xpm"
#include "res/bitmaps/rapidsvn_48x48.xpm"

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

// number of items initially in the list
static const int NUM_ITEMS = 30;

// List config keys here, to avoid duplicating literal text:
const static wxChar ConfigLeft[] = wxT("/MainFrame/Left");
const static wxChar ConfigTop[] = wxT("/MainFrame/Top");
const static wxChar ConfigWidth[] = wxT("/MainFrame/Width");
const static wxChar ConfigHeight[] = wxT("/MainFrame/Height");
const static wxChar ConfigMaximized[] = wxT("/MainFrame/Maximized");
/** changed name to "SplitterHoriz2" since meaning of value changed for 0.9.4 */
const static wxChar ConfigSplitterHoriz[] = wxT("/MainFrame/SplitterHoriz2");
/** changed name to "SplitterVert2" since meaning of value changed for 0.9.4 */
const static wxChar ConfigSplitterVert[] = wxT("/MainFrame/SplitterVert2");

const static wxChar TraceMisc[] = wxT("tracemisc");

// Platform specific constants.
#ifdef __WXMSW__
const static int SPLITTER_STYLE = wxSP_FULLSASH | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN;
#else
const static int SPLITTER_STYLE = wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN;
#endif

/**
 * Local helper function to create the action worker
 *
 * @param parent
 * @return new action worker instance
 */
ActionWorker *
CreateActionWorker (wxWindow * parent)
{
#ifdef USE_SIMPLE_WORKER
  return new SimpleWorker (parent);
#else
  return new ThreadedWorker (parent);
#endif
}

/**
 * class that hide implementation specific data and methods from
 * the interface
 */
struct RapidSvnFrame::Data
{
public:
  FolderBrowser * folderBrowser;
  FileListCtrl * listCtrl;
  wxSplitterWindow * horizSplitter;
  wxSplitterWindow * vertSplitter;
  wxTextCtrl * log;
  EventTracer * logTracer;

  wxMenu * MenuColumns;
  wxMenu * MenuSorting;
  wxMenuBar * MenuBar;

  Listener listener;
  Columns ColumnList;

  bool updateAfterActivate;
  bool dontUpdateFilelist;
  bool skipFilelistUpdate;
  const wxLocale & locale;
  wxString currentPath;
  ActivePane activePane;

private:
  bool m_running;
  size_t m_toolbar_rows;        // 1 or 2 only (toolbar rows)
  wxFrame * m_parent;
  bool m_isErrorDialogActive;

public:

  /**
   * This instance of @a apr is used to initialize/terminate apr
   */
  svn::Apr apr;

  Data (wxFrame * parent, const wxLocale & locale_)
    : folderBrowser (0), listCtrl (0), 
      horizSplitter (0), vertSplitter (0), 
      log (0), logTracer (0),
      MenuColumns (0), MenuSorting (0), MenuBar (0), 
      listener (parent),
      updateAfterActivate (false), dontUpdateFilelist (false),
      skipFilelistUpdate (false), locale (locale_), 
      currentPath (wxT("")), 
      activePane (ACTIVEPANE_FOLDER_BROWSER),
      m_running (false),
      m_toolbar_rows (1), m_parent (parent),
      m_isErrorDialogActive (false)
  {
    InitializeMenu ();
  }

  void
  InitializeMenu ()
  {

    // the following code places the menu items in their correct
    // places on MacOSX:
#ifdef __WXMAC__
    wxApp::s_macAboutMenuItemId = ID_About;
    wxApp::s_macPreferencesMenuItemId = ID_Preferences;
    wxApp::s_macHelpMenuTitleName = _("&Help");
#endif

    // File menu
    wxMenu *menuFile = new wxMenu;
    // Quit menu item is not necessary on MacOSX:
#ifndef __WXMAC__
    //menuFile->AppendSeparator ();
    AppendMenuItem (*menuFile, ID_Quit);
#endif

    // Columns menu
    MenuColumns = new wxMenu;
    AppendMenuItem (*MenuColumns, ID_Column_Reset);
    MenuColumns->AppendSeparator ();

    // Sorting menu
    MenuSorting = new wxMenu;
    MenuSorting->AppendCheckItem (ID_Include_Path, _("Use Path for Sorting"));
    MenuSorting->AppendCheckItem (ID_Sort_Ascending, _("Sort Ascending"));

    MenuSorting->AppendSeparator ();

    for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
    {
      int id = ColumnList[col].id;
      int sortid = id + Columns::SORT_COLUMN_OFFSET;

      if (id != ID_Column_Name && id != ID_Column_Path)
        MenuColumns->AppendCheckItem (id, ColumnList[col].caption);
      MenuSorting->AppendRadioItem (sortid, ColumnList[col].caption);
    }

    // View menu
    wxMenu *menuView = new wxMenu;
    AppendMenuItem (*menuView, ID_Refresh);
    AppendMenuItem (*menuView, ID_Stop);
    menuView->AppendSeparator ();

    AppendMenuItem (*menuView, ID_Explore);
    menuView->AppendSeparator ();

    menuView->Append (0, _("Columns"), MenuColumns);
    menuView->Append (0, _("Sort"), MenuSorting);

    menuView->AppendCheckItem (ID_Flat, _("Flat Mode"));
    menuView->AppendCheckItem (ID_RefreshWithUpdate, _("Refresh with Update"));
    menuView->AppendCheckItem (ID_ShowUnversioned, _("Show unversioned entries"));
    menuView->AppendCheckItem (ID_IgnoreExternals, _("Ignore Externals"));
    menuView->AppendCheckItem (ID_ShowIgnored, _("Show ignored entries"));

    // Preferences menu item goes to its own place on MacOSX,
    // so no separator is necessary.
#ifndef __WXMAC__
    menuView->AppendSeparator ();
#endif
    AppendMenuItem (*menuView, ID_Preferences);

    // Repository menu
    wxMenu *menuRepos = new wxMenu;
    menuRepos->Append (ID_Import, _("&Import...\tCTRL-I"));
    menuRepos->Append (ID_Export, _("&Export...\tCTRL-E"));
    menuRepos->Append (ID_Checkout, _("Check&out...\tCTRL-O"));
    menuRepos->Append (ID_CreateRepository, _("&Create..."));

    menuRepos->AppendSeparator ();

    menuRepos->Append (ID_Merge, _("Merge..."));
    menuRepos->Append (ID_Switch, _("Switch URL...\tCTRL-S"));

    // Modify menu
    wxMenu *menuModif = new wxMenu;
    AppendModifyMenu (menuModif);

    // Query menu
    wxMenu *menuQuery = new wxMenu;
    AppendQueryMenu (menuQuery);

    // Bookmarks menu
    wxMenu *menuBookmarks = new wxMenu;
    AppendBookmarksMenu (menuBookmarks);

    // Extras menu
    wxMenu *menuExtras = new wxMenu;
    AppendMenuItem (*menuExtras, ID_Cleanup);

    // Help Menu
    wxMenu *menuHelp = new wxMenu;

    menuHelp->Append (ID_HelpContents, _("&Contents\tF1"));
    menuHelp->Append (ID_HelpIndex, _("&Index\tShift+F1"));
    menuHelp->AppendSeparator ();
#ifdef USE_STARTUP_TIPS
    menuHelp->Append (ID_HelpStartupTips, _("Show Startup Tips"));
    menuHelp->AppendSeparator ();
#endif
    menuHelp->Append (ID_About, _("&About..."));

#ifdef USE_DEBUG_TESTS
    // Debug Menu
    wxMenu *menuTests = new wxMenu;

    menuTests->Append (ID_TestNewWxString, _("wxString Creation&Tracing Test"));
    menuTests->Append (ID_TestListener, _("Listener Test"));
    menuTests->Append (ID_TestCheckout, _("Checkout Test"));
#endif

    // Create the menu bar and append the menus
    MenuBar = new wxMenuBar;
    // Under wxMac the menu might be empty, so
    // don't show it:
    if (menuFile->GetMenuItemCount()>0)
      MenuBar->Append (menuFile, _("&File"));
    MenuBar->Append (menuView, _("&View"));
    MenuBar->Append (menuRepos, _("&Repository"));
    MenuBar->Append (menuModif, _("&Modify"));
    MenuBar->Append (menuQuery, _("&Query"));
    MenuBar->Append (menuBookmarks, _("&Bookmarks"));
    MenuBar->Append (menuExtras, _("&Extras"));
    MenuBar->Append (menuHelp, _("&Help"));
#ifdef USE_DEBUG_TESTS
    MenuBar->Append (menuTests, _("&Tests"));
#endif
  }

  bool
  IsColumnChecked (int id)
  {
    return MenuColumns->IsChecked (id);
  }

  void
  CheckColumn (int id, bool check)
  {
    MenuColumns->Check (id, check);
  }

  void
  CheckSort (int id)
  {
    MenuSorting->Check (id, true);
  }

  void
  EnableMenuEntry (int id, bool enable)
  {
    MenuSorting->Enable (id, enable);
  }

  bool
  IsMenuChecked (int id)
  {
    return MenuBar->IsChecked (id);
  }

  void
  CheckMenu (int id, bool check)
  {
    MenuBar->Check (id, check);
  }

  void
  SetRunning (bool running)
  {
    if (running && ! m_running)
      listener.cancel (false);

    m_running = running;

    wxToolBarBase * toolBar = m_parent->GetToolBar ();

    wxASSERT (toolBar != 0);

    toolBar->EnableTool (ID_Stop, running);
    MenuBar->Enable (ID_Stop, running);
  }

  bool
  IsRunning () const
  {
    return m_running;
  }

  void
  RecreateToolbar ()
  {
    // delete the old toolbar
    wxToolBarBase * toolBar = m_parent->GetToolBar ();
    delete toolBar;

    m_parent->SetToolBar (NULL);

    long style = wxNO_BORDER | wxTB_FLAT | wxTB_DOCKABLE;
    style |= wxTB_HORIZONTAL;

    toolBar = m_parent->CreateToolBar (style, ID_TOOLBAR);
    toolBar->SetMargins (4, 4);

    AddActionTools (toolBar);
    AddInfoTools (toolBar);

    // Set toolbar refresh button.
    toolBar->AddTool (ID_Refresh,
                      EMBEDDED_BITMAP(refresh_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Refresh"),
                      _("Refresh the file list"));

    toolBar->AddSeparator ();

    // STOP button
    toolBar->AddTool (ID_Stop,
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
    toolBar->Realize ();

    toolBar->SetRows (m_toolbar_rows);
  }

  void
  AddActionTools (wxToolBarBase *toolBar)
  {
    wxASSERT (toolBar);

    toolBar->AddTool (ID_Add,
                      EMBEDDED_BITMAP(add_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Add selected"),
                      _("Put files and directories under revision control"));

    toolBar->AddTool (ID_Delete,
                      EMBEDDED_BITMAP(delete_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Delete selected"),
                      _("Delete files and directories from version control"));

    toolBar->AddTool (ID_Update,
                      EMBEDDED_BITMAP(update_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Update selected"),
                      _("Bring changes from the repository into the working copy"));

    toolBar->AddTool (ID_Commit,
                      EMBEDDED_BITMAP(commit_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Commit selected"),
                      _("Send changes from your working copy to the repository"));

    toolBar->AddTool (ID_Revert,
                      EMBEDDED_BITMAP(revert_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Revert selected"),
                      _("Restore pristine working copy file (undo all local edits)"));

    toolBar->AddTool (ID_Resolve,
                      EMBEDDED_BITMAP(resolve_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Resolve selected"),
                      _("Remove 'conflicted' state on working copy files or directories"));

    toolBar->AddSeparator ();
  }

  void
  AddInfoTools (wxToolBarBase *toolBar)
  {
    toolBar->AddTool (ID_Info,
                      EMBEDDED_BITMAP(info_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Info selected"),
                      _("Display info about selected entries"));

    toolBar->AddTool (ID_Log,
                      EMBEDDED_BITMAP(log_png),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Log selected"),
                      _("Show the log messages for the selected entries"));

    toolBar->AddSeparator ();
  }


  /** 
   * Checks whether @ref currenPath is an URL
   *
   * @retval true valid URL
   */
  bool
  IsUrl () const
  {
    return svn::Url::isValid (LocalToUtf8(currentPath).c_str ());
  }


  /**
   * Checks whether the selected bookmark is "flat"
   *
   * @retval true yes, it's flat
   */
  bool
  IsFlat () const
  {
    if (!listCtrl)
      return 0;

    return listCtrl->IsFlat ();
  }


  /**
   * Checks whether there is an error dialog showing
   * at the moment (not an action dialog).
   *
   * This information is used to decide whether to refresh
   * in @ref RapidSvnFrame::OnActivate
   *
   * @retval true there is an error dialog showing
   */
  bool
  IsErrorDialogActive () const
  {
    return m_isErrorDialogActive;
  }


  /**
   * Shows an error dialog
   *
   * @see IsErrorDialogActive
   *
   * @param msg Error message to display
   */
  void
  ShowErrorDialog (const wxString & msg)
  {
    m_isErrorDialogActive = true;
    wxMessageBox (msg, _("RapidSVN Error"), wxICON_ERROR | wxOK);
    m_isErrorDialogActive = false;
  }


  /**
   * add message to log window
   *
   * @param msg message to show
   */
  void
  Trace (const wxString & msg)
  {
    if (!log)
      return;

    log->AppendText (msg + wxT('\n'));
  }
  
  /**
   * add error message to log window marked red
   *
   * @param msg error message to show
   * @param showDialog show an error dialog with the message
   *                   (besides logging this)
   */
  void
  TraceError (const wxString & msg, bool showDialog=true)
  {
    if (!log)
      return;

    log->SetDefaultStyle(wxTextAttr(*wxRED));
    log->AppendText (wxString::Format (_("Error: %s\n"), msg.c_str ()));
    log->SetDefaultStyle(wxTextAttr(*wxBLACK));

    if (showDialog)
      ShowErrorDialog (msg);
  }


#if 0
  const svn::Targets
  GetTargets () const
  {
    //is there nothing selected in the list control,
    //or is the active window *not* the list control?
    if (listCtrl->GetSelectedItemCount () <= 0 ||
        activePane != ACTIVEPANE_FILELIST)
    {
      wxString path = folderBrowser->GetPath ();

      svn::Path pathUtf8 (PathUtf8 (path));
      if (!pathUtf8.isUrl ())
      {
        wxFileName fname (path);
        path = fname.GetFullPath ();
        pathUtf8 = PathUtf8 (path);
      }

      return svn::Targets (pathUtf8.c_str ());
    }
    else
    {
      //no, build the file list from the list control
      return listCtrl->GetTargets ();
    }
  }
#endif 

  const svn::StatusSel &
  GetStatusSel ()  const
  {
    //is there nothing selected in the list control,
    //or is the active window *not* the list control?
    if (listCtrl->GetSelectedItemCount () <= 0 ||
        activePane != ACTIVEPANE_FILELIST)
    {
      return folderBrowser->GetStatusSel ();
    }
    else
    {
      //no, build the file list from the list control
      return listCtrl->GetStatusSel ();
    }
  }
};


BEGIN_EVENT_TABLE (RapidSvnFrame, wxFrame)
  EVT_ACTIVATE (RapidSvnFrame::OnActivate)
  EVT_MENU (ID_AddWcBookmark, RapidSvnFrame::OnAddWcBookmark)
  EVT_MENU (ID_AddRepoBookmark, RapidSvnFrame::OnAddRepoBookmark)
  EVT_MENU (ID_RemoveBookmark, RapidSvnFrame::OnRemoveBookmark)
  EVT_MENU (ID_EditBookmark, RapidSvnFrame::OnEditBookmark)
  EVT_MENU (ID_Quit, RapidSvnFrame::OnQuit)

  EVT_MENU (ID_Preferences, RapidSvnFrame::OnPreferences)
  EVT_MENU (ID_Refresh, RapidSvnFrame::OnRefresh)

  EVT_MENU (ID_Column_Reset, RapidSvnFrame::OnColumnReset)
  EVT_MENU_RANGE (ID_Column_Min, ID_Column_Max, RapidSvnFrame::OnColumn)
  EVT_MENU (ID_Include_Path, RapidSvnFrame::OnIncludePath)
  EVT_MENU (ID_Sort_Ascending, RapidSvnFrame::OnSortAscending)
  EVT_MENU_RANGE (ID_ColumnSort_Min, ID_ColumnSort_Max, RapidSvnFrame::OnColumnSorting)

  EVT_MENU (ID_Flat, RapidSvnFrame::OnFlatView)
  EVT_MENU (ID_RefreshWithUpdate, RapidSvnFrame::OnRefreshWithUpdate)
  EVT_MENU (ID_ShowUnversioned, RapidSvnFrame::OnShowUnversioned)
  EVT_MENU (ID_IgnoreExternals, RapidSvnFrame::OnIgnoreExternals)
  EVT_MENU (ID_ShowIgnored, RapidSvnFrame::OnShowIgnored)
  EVT_MENU (ID_Login, RapidSvnFrame::OnLogin)
  EVT_MENU (ID_Logout, RapidSvnFrame::OnLogout)
  EVT_MENU (ID_Stop, RapidSvnFrame::OnStop)

  EVT_MENU (ID_Info, RapidSvnFrame::OnInfo)
  EVT_UPDATE_UI (ID_Info, RapidSvnFrame::OnUpdateCommand)

  EVT_MENU (ID_HelpContents, RapidSvnFrame::OnHelpContents)
  EVT_MENU (ID_HelpIndex, RapidSvnFrame::OnHelpIndex)
  EVT_MENU (ID_HelpStartupTips, RapidSvnFrame::OnHelpStartupTips)
  EVT_MENU (ID_About, RapidSvnFrame::OnAbout)

#ifdef USE_DEBUG_TESTS
  EVT_MENU (ID_TestNewWxString, RapidSvnFrame::OnTestNewWxString)
  EVT_MENU (ID_TestListener, RapidSvnFrame::OnTestListener)
  EVT_MENU (ID_TestCheckout, RapidSvnFrame::OnTestCheckout)
#endif

  EVT_MENU_RANGE (ID_File_Min, ID_File_Max, RapidSvnFrame::OnFileCommand)
  EVT_MENU_RANGE (ID_Verb_Min, ID_Verb_Max, RapidSvnFrame::OnFileCommand)

  EVT_UPDATE_UI_RANGE (ID_File_Min, ID_File_Max, RapidSvnFrame::OnUpdateCommand)
  EVT_UPDATE_UI_RANGE (ID_Verb_Min, ID_Verb_Max, RapidSvnFrame::OnUpdateCommand)

  EVT_MENU (ACTION_EVENT, RapidSvnFrame::OnActionEvent)

  EVT_TOOL_ENTER (ID_TOOLBAR, RapidSvnFrame::OnToolEnter)
  EVT_TREE_SEL_CHANGED (-1, RapidSvnFrame::OnFolderBrowserSelChanged)
  EVT_TREE_KEY_DOWN (-1, RapidSvnFrame::OnFolderBrowserKeyDown)
  EVT_LIST_ITEM_SELECTED (-1, RapidSvnFrame::OnFileListSelected)

  EVT_MENU_RANGE (LISTENER_MIN, LISTENER_MAX, RapidSvnFrame::OnListenerEvent)

  EVT_SIZE (RapidSvnFrame::OnSize)

  EVT_MENU (FOLDER_BROWSER, RapidSvnFrame::OnFocusChanged)
  EVT_MENU (FILELIST_CTRL, RapidSvnFrame::OnFocusChanged)
END_EVENT_TABLE ()

/** class implementation **/
RapidSvnFrame::RapidSvnFrame (const wxString & title,
                              const wxLocale & locale)
  : wxFrame ((wxFrame *) NULL, -1, title, wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_FRAME_STYLE),
    m_title (title), m_context (0)
{
  m = new Data (this, locale);
  m_actionWorker = CreateActionWorker (this);

  // enable trace
  wxLog::AddTraceMask (TraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *cfg = wxConfigBase::Get ();

  // Now set icons for the application
  // (several sizes are available OS/windowing system
  // picks whatever it thinks is suitable)
  wxIconBundle iconBundle;
  iconBundle.AddIcon (wxIcon (rapidsvn_16x16_xpm));
  iconBundle.AddIcon (wxIcon (rapidsvn_32x32_xpm));
  iconBundle.AddIcon (wxIcon (rapidsvn_48x48_xpm));
  SetIcons (iconBundle);

  // toolbar rows
  m_toolbar_rows = 1;

  SetMenuBar (m->MenuBar);
  CreateStatusBar ();

  // Create the toolbar
  m->RecreateToolbar ();
  m->SetRunning (false);

  // Note: In the past here was an #if that checked
  // the wxWidgets version, since wxSplitterWindow
  // wasnt available below 2.4.2 (if I remember this correctly).
  // But today we only use wxSplitterWindow
  m->horizSplitter = new wxSplitterWindow (this, -1, 
                                           wxDefaultPosition, 
                                           wxDefaultSize,
                                           SPLITTER_STYLE);

  m_info_panel = new wxPanel (m->horizSplitter, -1,
                              wxDefaultPosition, wxDefaultSize,
                              wxTAB_TRAVERSAL | wxCLIP_CHILDREN);

  m->log = new wxTextCtrl (m->horizSplitter, -1, wxEmptyString,
                          wxPoint (0, 0), wxDefaultSize,
                          wxTE_MULTILINE | wxTE_READONLY);

  // as much as the widget can stand
  m->log->SetMaxLength (0);

  m->logTracer = new EventTracer (this);
  m->listener.SetTracer (m->logTracer, false);


  m->vertSplitter = new wxSplitterWindow (m_info_panel, -1,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          SPLITTER_STYLE);

  // Create the list control to display files
  m->listCtrl = new FileListCtrl (m->vertSplitter, FILELIST_CTRL,
                                 wxDefaultPosition, wxDefaultSize);
  m->CheckMenu (ID_Flat,              false);
  m->CheckMenu (ID_RefreshWithUpdate, m->listCtrl->GetWithUpdate());
  m->CheckMenu (ID_ShowUnversioned,   m->listCtrl->GetShowUnversioned());
  m->CheckMenu (ID_IgnoreExternals,   m->listCtrl->GetIgnoreExternals());
  m->CheckMenu (ID_ShowIgnored,       m->listCtrl->GetShowIgnored());
  m->CheckMenu (ID_Sort_Ascending,    m->listCtrl->GetSortAscending());
  m->CheckSort (m->listCtrl->GetSortColumn() + ID_ColumnSort_Min + 1);

  bool isFlat = m->listCtrl->IsFlat ();
  bool includePath = m->listCtrl->GetIncludePath ();
  if (isFlat == false && includePath == true)
    m->CheckMenu (ID_Include_Path, false);
  else
    m->CheckMenu (ID_Include_Path, includePath);
  m->EnableMenuEntry (ID_Include_Path, isFlat ? true : false);


  // Create the browse control
  m->folderBrowser = new FolderBrowser (m->vertSplitter, FOLDER_BROWSER);

  // Adapt the menu entries
  for (int col=0; col < FileListCtrl::COL_COUNT; col++)
  {
    int id = m->ColumnList[col].id;
    int sortid = id + Columns::SORT_COLUMN_OFFSET;
    if (id != ID_Column_Name && id != ID_Column_Path)
    {
      bool check = m->listCtrl->GetColumnVisible (col);
      m->CheckColumn (id, check);
      m->EnableMenuEntry (sortid, check);
    }
  }

  RefreshFileList ();

  wxSizer *sizer = new wxBoxSizer (wxVERTICAL);
  sizer->Add (m->vertSplitter, 1, wxEXPAND);

  m_info_panel->SetAutoLayout (true);
  m_info_panel->SetSizer (sizer);

  // Read frame position
  if (cfg->Read (ConfigMaximized, (long int)0) == 1)
    Maximize (true);
  else
  {
    int x = cfg->Read (ConfigLeft, 50);
    int y = cfg->Read (ConfigTop, 50);
    int w = cfg->Read (ConfigWidth, 806);
    int h = cfg->Read (ConfigHeight, 480);

    Move (x, y);
    SetClientSize (w, h);
  }

  // Get sash position for every splitter from configuration.
  int w,h;
  GetClientSize (&w, &h);
  int vpos = cfg->Read (ConfigSplitterVert, w / 3);
  int hpos = cfg->Read (ConfigSplitterHoriz, (3 * h) / 4);

  // initialize the folder browser
  m->folderBrowser->ReadConfig (cfg);
  {
    Preferences prefs;
    m->folderBrowser->SetListener        (&m->listener);
    m->folderBrowser->SetAuthCache       (prefs.useAuthCache);
    m->folderBrowser->SetAuthPerBookmark (prefs.authPerBookmark);
  }
  UpdateCurrentPath ();
  RefreshFolderBrowser ();

  // Set sash position for every splitter.
  // Note: do not revert the order of Split calls, as the panels will be messed up.
  m->horizSplitter->SplitHorizontally (m_info_panel, m->log, hpos);
  m->vertSplitter->SplitVertically (m->folderBrowser, m->listCtrl, vpos);

  // Initialize for drag and drop
  m->folderBrowser->SetDropTarget (new FolderBrowserDropTarget (m->folderBrowser));
  m->listCtrl->SetDropTarget (new FileListCtrlDropTarget (m->folderBrowser, m->listCtrl));
}

RapidSvnFrame::~RapidSvnFrame ()
{
  wxConfigBase *cfg = wxConfigBase::Get ();
  if (cfg == NULL)
    return;

  if (m->logTracer)
    delete m->logTracer;

  if (m_actionWorker)
    delete m_actionWorker;

  // Save frame size and position.
  if (!IsIconized())
  {
    if (IsMaximized ())
      cfg->Write (ConfigMaximized, 1);
    else
    {
      int x, y;
      int w, h;

      GetClientSize (&w, &h);
      GetPosition (&x, &y);

      cfg->Write (ConfigMaximized, 0);
      cfg->Write (ConfigLeft, (long) x);
      cfg->Write (ConfigTop, (long) y);
      cfg->Write (ConfigWidth, (long) w);
      cfg->Write (ConfigHeight, (long) h);
    }
  }

  // Save splitter positions
  cfg->Write (ConfigSplitterVert,
              (long) m->vertSplitter->GetSashPosition ());
  cfg->Write (ConfigSplitterHoriz,
              (long) m->horizSplitter->GetSashPosition ());

  m->folderBrowser->WriteConfig (cfg);

  delete m;
}

void
RapidSvnFrame::SetActivePane (ActivePane value)
{
  m->activePane = value;
}

void
RapidSvnFrame::TrimDisabledMenuItems (wxMenu & menu)
{
  // Check for disabled items
  size_t pos = menu.GetMenuItemCount ();
// TODO  unsigned int selectionActionFlags = GetSelectionActionFlags ();
  const svn::StatusSel & statusSel = m->GetStatusSel ();
  while (pos-- > 0)
  {
    wxMenuItem *pItem = menu.FindItemByPosition (pos);
    if (!pItem->IsSeparator () && 
        !ActionFactory::CheckIdForStatusSel (pItem->GetId (), statusSel))
    {
      menu.Destroy (pItem);
    }
  }

  // Trim unnecessary separators
  pos = menu.GetMenuItemCount ();

  if (0 == pos)
    return;

  bool sepNeeded = false;
  while (pos-- > 0)
  {
    wxMenuItem *pItem = menu.FindItemByPosition (pos);
    if (pItem->IsSeparator ())
    {
      // If we're at a separator at the top, trash it & look for any more we skipped along the way
      if (pos == 0)
      {
        ++pos;
        sepNeeded = false;
      }

      if (sepNeeded)
      {
        sepNeeded = false;
      }
      else
      {
        menu.Destroy (pItem);
      }
    }
    else
    {
      sepNeeded = true;
    }
  }
}

void
RapidSvnFrame::RefreshFileList ()
{
  if (m->dontUpdateFilelist)
    return;

  bool isRunning = m->IsRunning ();
  if (!isRunning)
    m->SetRunning (true);

  if (m->listCtrl && m->folderBrowser)
  {
    wxBusyCursor busy;

    try
    {
      // UPDATE
      /**
       * @todo THIS IS A WORKAOUND:
       * Without real information to display in the 
       * filelist (e.g. the "Bookmarks" symbol) we 
       * could simply show a grey background.
       * In the past we simpy used Show (FALSE).
       * But this caused in the right area not being
       * updated at all. 
       * Either we have to so something special with 
       * wxWidgets or this is a wxWidgets bug and will
       * be fixed in there
       */
      if (m->currentPath.length () == 0)
      {
        // calling "UpdateColumns" should be necessary
        // only for the first call. But without any
        // items this call should be cheap.
        m->listCtrl->DeleteAllItems ();
        m->listCtrl->UpdateColumns ();
      }
      else
      {
        m->listCtrl->SetContext (m_context);
        m->listCtrl->RefreshFileList (m->currentPath);
      }

    }
    catch (svn::ClientException & e)
    {
      wxString msg, errtxt (Utf8ToLocal (e.message ()));
      msg.Printf (_("Error while updating filelist (%s)"),
                  errtxt.c_str ());
      m->TraceError (msg, false);

      // calling "UpdateColumns" should be necessary
      // only for the first call. But without any
      // items this call should be cheap.
      m->listCtrl->DeleteAllItems ();
      m->listCtrl->UpdateColumns ();

    }
    catch (...)
    {
      m->TraceError (_("Error while updating filelist"), false);

      // calling "UpdateColumns" should be necessary
      // only for the first call. But without any
      // items this call should be cheap.
      m->listCtrl->DeleteAllItems ();
      m->listCtrl->UpdateColumns ();
    }

  }
  if (!isRunning)
    m->SetRunning (false);
}

void
RapidSvnFrame::RefreshFolderBrowser ()
{
  wxBusyCursor busy;

  bool isRunning = m->IsRunning ();
  if (!isRunning)
    m->SetRunning (true);

  try
  {
    if (!m->skipFilelistUpdate)
      m->dontUpdateFilelist = true;

    if (m->folderBrowser)
      m->folderBrowser->RefreshFolderBrowser ();
  }
  catch (...)
  {
    m->skipFilelistUpdate = false;
  }

  if (!isRunning)
    m->SetRunning (false);

  if (!m->skipFilelistUpdate)
  {
    m->dontUpdateFilelist = false;
    RefreshFileList ();
  }
  m->skipFilelistUpdate = false;
}

/*** START OF SECTION EVENTS ***/

void
RapidSvnFrame::OnActivate (wxActivateEvent & event)
{
  // in the past we used to refresh only if we had
  // had an action which changed stuff.
  //
  // now we refresh every time - but only for
  // working copies and if NOT flat
  // and NOT if another action is running.
  if (event.GetActive ())
  {
    if (m->updateAfterActivate || 
        (!m->IsUrl () && 
         !m->IsFlat () && 
         !m->IsRunning () &&
         !m->IsErrorDialogActive ()))
    {
      m->updateAfterActivate = false;

      RefreshFileList ();
    }
  }

  // wxMac needs this, otherwise the menu doesn't show:
#ifdef __WXMAC__
  event.Skip();
#endif
}

void
RapidSvnFrame::OnAddWcBookmark (wxCommandEvent & event)
{
  AddWcBookmark ();
}

void
RapidSvnFrame::OnAddRepoBookmark (wxCommandEvent & event)
{
  AddRepoBookmark ();
}

void
RapidSvnFrame::OnRemoveBookmark (wxCommandEvent & event)
{
  RemoveBookmark ();
}

void 
RapidSvnFrame::OnEditBookmark (wxCommandEvent & event)
{
  EditBookmark ();
}

void
RapidSvnFrame::OnQuit (wxCommandEvent & WXUNUSED (event))
{
  Close (TRUE);
}

void
RapidSvnFrame::OnPreferences (wxCommandEvent & WXUNUSED (event))
{
  ShowPreferences ();
}

void
RapidSvnFrame::OnRefresh (wxCommandEvent & WXUNUSED (event))
{
  RefreshFolderBrowser ();
}

void
RapidSvnFrame::OnColumnReset (wxCommandEvent &)
{
  m->listCtrl->ResetColumns ();
  for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
  {
    bool visible = m->listCtrl->GetColumnVisible (col);
    int id = m->ColumnList [col].id;

    if (id != ID_Column_Name && id != ID_Column_Path)
      m->CheckColumn (id, visible);

    int sortid = id + Columns::SORT_COLUMN_OFFSET;
    m->EnableMenuEntry (sortid, visible);
  }
  RefreshFileList ();
}

void
RapidSvnFrame::OnColumn (wxCommandEvent & event)
{
  int eventId = event.GetId ();
  int col = 0;
  while (col < FileListCtrl::COL_COUNT &&
         m->ColumnList[col].id != eventId)
  {
    col++;
  }

  // a passing column found
  if (m->ColumnList[col].id == eventId)
  {
    // Enable/disable the corresponding column. Automatically define
    // a new sorting column after an old one is disabled.
    bool visible = m->IsColumnChecked(eventId);
    m->listCtrl->SetColumnVisible (col, visible);

    int sortid = eventId + Columns::SORT_COLUMN_OFFSET;
    m->EnableMenuEntry (sortid, visible);

    UpdateMenuSorting ();
    UpdateMenuAscending ();

    RefreshFileList ();
  }
}

void
RapidSvnFrame::OnIncludePath (wxCommandEvent & WXUNUSED (event))
{
  m->listCtrl->SetIncludePath (!m->listCtrl->GetIncludePath ());
}

void
RapidSvnFrame::OnSortAscending (wxCommandEvent & event)
{
  m->listCtrl->SetSortAscending (!m->listCtrl->GetSortAscending ());
}

void
RapidSvnFrame::OnColumnSorting (wxCommandEvent & event)
{
  // we dont want to list FileListCtrl::COL_NAME/COL_PATH/... here
  int col = event.GetId () - ID_ColumnSort_Name;

  m->listCtrl->SetSortColumn (col);
  m->listCtrl->SetSortAscending (true);

  UpdateMenuAscending ();
}

void
RapidSvnFrame::OnFlatView (wxCommandEvent & event)
{
  bool newFlatMode = !m->folderBrowser->IsFlat ();

  // if this cannot be set (e.g. invalid selection
  // like the root element, we uncheck this
  if (!m->folderBrowser->SetFlat (newFlatMode))
    newFlatMode = false;

  m->CheckMenu (ID_Flat, newFlatMode);
  m->listCtrl->SetFlat (newFlatMode);

  SetIncludePathVisibility (newFlatMode);

  RefreshFileList ();
}


void
RapidSvnFrame::OnRefreshWithUpdate (wxCommandEvent & WXUNUSED (event))
{
  bool checked = m->IsMenuChecked (ID_RefreshWithUpdate);
  m->listCtrl->SetWithUpdate (checked);
  RefreshFolderBrowser ();
}


void
RapidSvnFrame::OnShowUnversioned (wxCommandEvent & WXUNUSED (event))
{
  bool checked = m->IsMenuChecked (ID_ShowUnversioned);
  m->listCtrl->SetShowUnversioned (checked);
  RefreshFileList ();
}

void
RapidSvnFrame::OnIgnoreExternals (wxCommandEvent & WXUNUSED (event))
{
  bool checked = m->IsMenuChecked (ID_IgnoreExternals);
  m->listCtrl->SetIgnoreExternals (checked);
  RefreshFileList ();
}

void
RapidSvnFrame::OnShowIgnored (wxCommandEvent & WXUNUSED (event))
{
  bool checked = m->IsMenuChecked (ID_ShowIgnored);
  m->listCtrl->SetShowIgnored (checked);
  RefreshFileList ();
}

void
RapidSvnFrame::OnLogin (wxCommandEvent & event)
{
  svn::Context * context = m->folderBrowser->GetContext ();

  if (context == 0)
    return;

  wxString LocalUsername(Utf8ToLocal (context->getUsername ()));
  AuthDlg dlg (this, LocalUsername);
  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    std::string username, password;
    LocalToUtf8 (dlg.GetUsername (), username);
    LocalToUtf8 (dlg.GetPassword (), password);
    context->setLogin (username.c_str (), password.c_str ());
  }
}

void
RapidSvnFrame::OnLogout (wxCommandEvent & event)
{
  svn::Context * context = m->folderBrowser->GetContext ();

  if (context == 0)
    return;

  context->setLogin ("", "");
}

void
RapidSvnFrame::OnStop (wxCommandEvent & event)
{
  m->listener.cancel (true);
}

void
RapidSvnFrame::OnInfo (wxCommandEvent & WXUNUSED (event))
{
  ShowInfo ();
}

void
RapidSvnFrame::OnUpdateCommand (wxUpdateUIEvent & updateUIEvent)
{
  try
  {
    updateUIEvent.Enable (
      ActionFactory::CheckIdForStatusSel (
        updateUIEvent.GetId (), m->GetStatusSel ()));
  }
  catch (svn::ClientException & e)
  {
    m->Trace (Utf8ToLocal (e.message ()));
    return;
  }
  catch (...)
  {
    m->Trace (_("An error occurred while checking valid actions"));
  }
}

void
RapidSvnFrame::OnHelpContents (wxCommandEvent & WXUNUSED (event))
{
  try
  {
    OpenURL (wxT("http://www.rapidsvn.org/help/index.php?id=OnlineHelp:Contents"));
  }
  catch(...)
  {
    m->Trace (_("An error occured while launching the browser"));
  }
    
// WE DONT USE THIS CODE NOW
//#ifdef  USE_HTML_HELP
//  ::wxGetApp ().GetHelpController().DisplayContents();
//#endif
//#endif
}

void
RapidSvnFrame::OnHelpIndex (wxCommandEvent & WXUNUSED (event))
{
  try
  {
    OpenURL (wxT("http://www.rapidsvn.org/help/index.php?id=OnlineHelp:Index"));
  }
  catch(...)
  {
    m->Trace (_("An error occured while launching the browser"));
  }

//#ifdef  USE_HTML_HELP
//  ::wxGetApp ().GetHelpController().DisplayIndex();
//#endif
}

void
RapidSvnFrame::OnHelpStartupTips (wxCommandEvent & WXUNUSED (event))
{
#ifdef USE_STARTUP_TIPS
  RapidSvnApp& app = ::wxGetApp();
  wxTipProvider* tipProvider = app.MakeTipProvider (true);
  if (tipProvider)
  {
    bool showAtStartup = ::wxShowTip (this, tipProvider);
    app.SaveTipsInfo (showAtStartup, tipProvider->GetCurrentTip ());
    delete tipProvider;
  }
#endif
}

void
RapidSvnFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  AboutDlg dlg (this, m->locale);

  dlg.ShowModal ();
}

#ifdef USE_DEBUG_TESTS
static void
info_print_time (apr_time_t atime, const wxChar * desc, wxString & str)
{
  apr_time_exp_t extime;
  apr_status_t apr_err;

  /* if this returns an error, just don't print anything out */
  apr_err = apr_time_exp_tz (&extime, atime, 0);
  if (!apr_err)
    str.Printf (wxT("%s: %02lu:%02lu:%02lu:%02lu"), desc,
                (unsigned long) (extime.tm_hour),
                (unsigned long) (extime.tm_min),
                (unsigned long) (extime.tm_sec),
                (unsigned long) (extime.tm_usec));
}

void
RapidSvnFrame::PrintTimeMeasurements (apr_time_t start, apr_time_t end, const wxString & name)
{
  wxString msg (name);
  Trace (wxT('\n') + msg);

  info_print_time (start, _("Test started at"), msg);
  Trace (wxT("\n") + msg);

  info_print_time (end, _("Test ended at"), msg);
  Trace (msg);

  apr_time_t duration = end - start;
  info_print_time (duration, _("Test duration"), msg);
  Trace (msg + wxT("\n"));
}

void
RapidSvnFrame::OnTestNewWxString (wxCommandEvent & WXUNUSED (event))
{
  apr_time_t start = apr_time_now ();

  // starting the test
  for (int i = 1; i <= 10000; i++)
  {
    wxString message;
    message.Printf (wxT("Tracing a message from newely created wxString, round #%d"), i);
    Trace (message);

    static apr_time_t last_access = apr_time_now ();
    if (apr_time_now () - last_access > 2000000)
    {
      last_access = apr_time_now ();
      wxSafeYield ();
    }
  }
  // ending the test
  apr_time_t end = apr_time_now ();

  wxString name (wxT("Created 10 000 wxStrings and printed them with Trace ()"));
  PrintTimeMeasurements (start, end, name);
}

void
RapidSvnFrame::OnTestListener (wxCommandEvent & WXUNUSED (event))
{
  apr_time_t start = apr_time_now ();

  // starting the test

  const int MAX_ACTION = svn_wc_notify_failed_unlock;

  for (int j = 1; j <= 1000; j++)
  {
    for (int i = 0; i < MAX_ACTION; i++)
    {
      static const wxChar *
      ACTION_NAMES [] =
      {
        _("Add"),              // svn_wc_notify_add,
        _("Copy"),             // svn_wc_notify_copy,
        _("Delete"),           // svn_wc_notify_delete,
        _("Restore"),          // svn_wc_notify_restore,
        _("Revert"),           // svn_wc_notify_revert,
        NULL ,                 // NOT USED HERE svn_wc_notify_failed_revert,
        _("Resolved"),         // svn_wc_notify_resolved,
        _("Skip"),             // NOT USED HERE svn_wc_notify_skip,
        _("Deleted"),          // svn_wc_notify_update_delete,
        _("Added"),            // svn_wc_notify_update_add,
        _("Updated"),          // svn_wc_notify_update_update,
        NULL,                  // NOT USED HERE svn_wc_notify_update_completed,
        NULL,                  // NOT USED HERE svn_wc_notify_update_external,
        NULL,                  // NOT USED HERE svn_wc_notify_status_completed
        NULL,                  // NOT USED HERE svn_wc_notify_status_external
        _("Modified"),         // svn_wc_notify_commit_modified,
        _("Added"),            // svn_wc_notify_commit_added,
        _("Deleted"),          // svn_wc_notify_commit_deleted,
        _("Replaced"),         // svn_wc_notify_commit_replaced,
        NULL,                  // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
        NULL,                  // NOT USED HERE svn_wc_notify_blame_revision
        _("Locked"),           // svn_wc_notify_locked
        _("Unlocked"),         // svn_wc_notify_unlocked
        _("Failed to lock"),   // svn_wc_notify_failed_lock
        _("Failed to unlock")  // svn_wc_notify_failed_unlock
      };
    
      if (ACTION_NAMES[i] != NULL)
      {
        static wxString wxpath;
    
#ifdef __WXMSW__
        wxpath = Utf8ToLocal ("O:/rapidsvn/TRANSLATIONS");
#else
        wxpath = Utf8ToLocal ("/home/sleepy/projects/rapidsvn/TRANSLATIONS");
#endif    
        static wxString msg;
        msg.Printf (wxT("%s: %s, %d"), ACTION_NAMES[i], wxpath.c_str (), j);
    
        Trace (msg);
      }

      static apr_time_t last_access = apr_time_now ();
      if (apr_time_now () - last_access > 2000000)
      {
        last_access = apr_time_now ();
        wxSafeYield ();
      }
    }
  }
  // ending the test
  apr_time_t end = apr_time_now ();

  wxString name (wxT("Tested Listener, 1000 loops through all the 22/26 actions"));
  PrintTimeMeasurements (start, end, name);
}

void
RapidSvnFrame::OnTestCheckout (wxCommandEvent & WXUNUSED (event))
{
  apr_time_t start = apr_time_now ();

  Action * action = new CheckoutAction (this);

  if (action)
    Perform (action);

  apr_time_t end = apr_time_now ();
  wxString name (wxT("Tested Checkout (works only in single-threaded version)"));
  PrintTimeMeasurements (start, end, name);
}
#endif

void
RapidSvnFrame::OnFileCommand (wxCommandEvent & event)
{
  int id = event.GetId ();

  switch (id)
  {
  case ID_CreateRepository:
    wxMessageBox ( 
_("Please use the command line utility 'svnadmin'\n\
to create a new repository.\n\n\
This command line utility is not part of the\n\
RapidSVN distribution.\n\n\
More information about subversion:\n\
http://svnbook.red-bean.com/\n\
http://subversion.tigris.org"),
    _("Information"),
    wxOK);
    break;

  case ID_Default_Action:
    InvokeDefaultAction();
    break;

  case ID_Mkdir:
    Perform (new MkdirAction (this, m->currentPath));

    break;

  default:
    // Let the action factory decide
    Action * action = ActionFactory::CreateAction (this, id);

    if (action)
      Perform (action);
    else
      m->logTracer->Trace (_("Unimplemented action!"));
  }

}

void
RapidSvnFrame::OnActionEvent (wxCommandEvent & event)
{
  const int token = event.GetInt ();

  switch (token)
  {
  case TOKEN_INFO:
    m->Trace (event.GetString ());
    break;

  case TOKEN_ERROR:
    m->TraceError (event.GetString ());
    break;

  case TOKEN_SVN_INTERNAL_ERROR:
  case TOKEN_INTERNAL_ERROR:
    m->TraceError (event.GetString ());
    // Action is interrupted, so cancel listener 
    // (in order to enable filelist update)
    // and disable "Running" state
    m->listener.cancel (false);
    RefreshFileList ();
    m->Trace (_("Ready\n"));
    m->SetRunning (false);
    break;

  case TOKEN_ACTION_START:
    m->Trace (event.GetString ());
    wxLogStatus (event.GetString ());
    break;

  case TOKEN_ACTION_END:
    {
      unsigned int* af = (unsigned int*) event.GetClientData ();
      unsigned int actionFlags = 0;
      if (af)
      {
        actionFlags = *af;
        delete af;
      }
      else
      {
        m->TraceError (_("Internal Error: no client data for action event!"));
      }

      if ((actionFlags & Action::UPDATE_LATER) != 0)
      {
        // dont update immediately but set this
        // marker to update right after
        // the next time the app gets activated
        m->updateAfterActivate = true;
      }
      else
      {
        if ((actionFlags & Action::UPDATE_TREE) != 0)
        {
          m->Trace (_("Updating..."));
          RefreshFolderBrowser ();
        }
        else if ((actionFlags & Action::DONT_UPDATE) == 0)
        {
          m->Trace (_("Updating..."));
          RefreshFileList ();
        }
      }

      m->Trace (_("Ready\n"));
      m->SetRunning (false);
    }
    break;

  case TOKEN_CMD:
  case TOKEN_CMD_DIFF:
  case TOKEN_CMD_MERGE:
  case TOKEN_CMD_VIEW:
    {
      // execute the command sent
      wxExecute (event.GetString ());
      /**
       * @todo Would be good to be able to interpret the return value
       *       to see if the command could be executed. 
       *       The documentation on @a wxExecute teaches us 0 means
       *       process couldnt be created. But with Linux/wxGTK I got
       *       a positive integer even if the command couldnt be found.
       *       Another way to check this might be to explicitely scan 
       *       the PATH variable to find the executable if a relative
       *       pathname is given.
       */
      // not working code
      // if (result != 0)
      // {
      //  // 0 as return value means the command couldnt be executed
      //  TraceError (wxString::Format (_("Execution of command failed: '%s'"), 
      //                                event.GetString ().c_str ()));
      // }
    }
    break;

  case TOKEN_VIEW:
    {
      GetData * pData = static_cast<GetData *>(event.GetClientData ());

      if (pData != 0)
      {
        // copy the data first. This makes sure
        // the memory is released in the occurence
        // of an exception
        GetData data (*pData);
        delete pData;

        Action * action = new ViewAction (this, data);
        Perform (action);
      }
    }
    break;

  case TOKEN_ADD_BOOKMARK:
    {
      const wxString bookmark = event.GetString ();

      m->folderBrowser->AddBookmark (bookmark);
      m->folderBrowser->RefreshFolderBrowser ();
      m->folderBrowser->SelectBookmark (bookmark);
    }
    break;

  case TOKEN_GET:
    {
      GetData * pData = static_cast<GetData *>(event.GetClientData ());

      if (pData != 0)
      {
        // copy the data first. This makes sure
        // the memory is released in the occurence
        // of an exception
        GetData data (*pData);
        delete pData;
        Action * action;

        action = new GetAction (this, data);
        Perform (action);
      }
    }
    break;

  case TOKEN_DIFF:
    {
      DiffData * pData = static_cast<DiffData *>(event.GetClientData ());

      if (pData != 0)
      {
        // copy and delete data
        DiffData data (*pData);
        delete pData;
        Action * action;

        action = new DiffAction (this, data);
        Perform (action);
      }
    }
    break;

  case TOKEN_MERGE:
    {
      MergeData * pData = static_cast<MergeData *>(event.GetClientData ());

      if (pData != 0)
      {
        // copy and delete data
        MergeData data (*pData);
        delete pData;
        Action * action;

        action = new MergeAction (this, data);
        Perform (action);
      }
    }
    break;

  case TOKEN_DELETE_ACTION:
    {
      Action * action = static_cast<Action *>(event.GetClientData ());

      if (action != 0)
        delete action;
    }
    break;

  case TOKEN_UPDATE_ASCENDING:
    UpdateMenuAscending ();
    break;

  case TOKEN_UPDATE_SORTING:
    UpdateMenuSorting ();
    break;

  case TOKEN_ANNOTATE:
    {
      AnnotateData * pData = static_cast<AnnotateData *>(event.GetClientData ());

      if (pData != 0)
      {
        // copy and delete data
        AnnotateData data (*pData);
        delete pData;
        Action * action;

        action = new AnnotateAction (this, data);
        Perform (action);
      }
    }
    break;
  case TOKEN_LOG:
    {
      LogData * pData = static_cast<LogData *>(event.GetClientData ());

      if (pData != 0)
      {
        LogDlg dlg (this, pData->target.c_str (), pData->logEntries);
        dlg.ShowModal ();

        delete pData->logEntries;
        delete pData;
      }
    }
    break;
  case TOKEN_DRAG_N_DROP:
    {
      DragAndDropData * pData = static_cast<DragAndDropData *>(event.GetClientData ());
      if (pData != 0)
      {
        // copy and delete data
        DragAndDropData data (*pData);
        delete pData;
        Action * action;

        action = new DragAndDropAction (this, data);
        Perform (action);
      }
    }
    break;
  }
}

void
RapidSvnFrame::OnToolEnter (wxCommandEvent & event)
{
}

void
RapidSvnFrame::OnFolderBrowserSelChanged (wxTreeEvent & event)
{
  if (m->IsRunning ())
    return;

  try
  {
    m->activePane = ACTIVEPANE_FOLDER_BROWSER;

    // Update the menu and list control flat-mode setting 
    bool flatMode = m->folderBrowser->IsFlat ();
    m->listCtrl->SetFlat (flatMode);
    m->CheckMenu (ID_Flat, flatMode);

    SetIncludePathVisibility (flatMode);
    
    // Disable menu entry if no path is selected (e.g. root)
    const wxString & path = m->folderBrowser->GetPath ();  
    m->MenuBar->Enable (ID_Flat, !path.IsEmpty ());

    UpdateCurrentPath ();
    RefreshFileList ();
  }
  catch(...)
  {
    m->Trace (_("Exception occured during filelist update"));
  }
}

void
RapidSvnFrame::OnFolderBrowserKeyDown (wxTreeEvent & event)
{
  switch (event.GetKeyEvent ().GetKeyCode ())
  {
  case WXK_RETURN:
    ProcessCommand (ID_Default_Action);
    break;

  default:
    event.Skip ();
    break;
  }
}

void
RapidSvnFrame::OnFileListSelected (wxListEvent & event)
{
  m->activePane = ACTIVEPANE_FILELIST;
}


void
RapidSvnFrame::OnListenerEvent (wxCommandEvent & event)
{
  m->listener.handleEvent (event);
}

/*** END OF SECTION EVENTS ***/


void
RapidSvnFrame::AddWcBookmark ()
{
  wxDirDialog dialog (this, _("Select a directory"), wxGetHomeDir ());
  bool add = TRUE;

  // select dir dialog
  if (dialog.ShowModal () != wxID_OK)
  {
    return;
  }

  // admin dir?
  // TODO: Should we have a translated-once constant
  // rather than keep converting SVN_WC_ADM_DIR_NAME?
  wxFileName fileName (dialog.GetPath ());
  if ((fileName.GetName () + fileName.GetExt ()) ==
      Utf8ToLocal (SVN_WC_ADM_DIR_NAME))
  {
    add = FALSE;
    wxMessageBox (
_("You cannot add a subversion administrative \
directory to the bookmarks!"),
                  _("Error"), wxOK);
    return;
  }

  // add
  m->folderBrowser->AddBookmark (dialog.GetPath ());

  m->skipFilelistUpdate = true;
  RefreshFolderBrowser ();

  wxLogStatus (_("Added working copy to bookmarks '%s'"),
               dialog.GetPath ().c_str ());
}

void
RapidSvnFrame::AddRepoBookmark ()
{
  const int flags =
    UpdateDlg::WITH_URL |
    UpdateDlg::WITHOUT_RECURSIVE |
    UpdateDlg::WITHOUT_REVISION;
  UpdateDlg dialog (this, _("Repository URL"), flags);

  if (dialog.ShowModal () != wxID_OK)
  {
    return;
  }

  // add
  wxString url = dialog.GetData ().url;
  m->folderBrowser->AddBookmark (url);

  m->skipFilelistUpdate = true;
  RefreshFolderBrowser ();

  wxLogStatus (_("Added repository to bookmarks '%s'"),
               url.c_str ());
}

inline void
RapidSvnFrame::RemoveBookmark ()
{
  wxASSERT (m->folderBrowser);
  if( m->folderBrowser->RemoveBookmark() )
  {
    wxLogStatus (_("Removed bookmark"));
  }
}

void
RapidSvnFrame::EditBookmark ()
{
  wxASSERT (m->folderBrowser);

  const FolderItemData * bookmark = m->folderBrowser->GetSelectedItemData ();

  // if nothing is selected, or the wrong type,
  // just exit (this should be handled by the routine
  // that is responsible for greying out menu entries,
  // but hey: nobody is perfect
  if (!m->folderBrowser)
    return;

  if (bookmark->getFolderType () != FOLDER_TYPE_BOOKMARK)
    return;

  // now edit the data
  wxString oldPath (bookmark->getPath ());
  DestinationDlg dlg (this, _("Edit Bookmark"), _("Bookmark"), 
                      0, oldPath);

  if (dlg.ShowModal () == wxID_OK)
  {
    // and if everything goes right, remove the old
    // selection and add the new one
    wxString newPath (dlg.GetDestination ());

    // if nothing has changed, just exit
    if (oldPath == newPath)
      return;

    m->folderBrowser->RemoveBookmark ();
    m->folderBrowser->AddBookmark (newPath);
    RefreshFolderBrowser ();
    m->folderBrowser->SelectBookmark (newPath);
  }
}

#if 0
unsigned int
RapidSvnFrame::GetSelectionActionFlags () const
{
  unsigned int flags = 0;

  //is there nothing selected in the list control,
  //or is the active window *not* the list control?
  if (m->listCtrl->GetSelectedItemCount () <= 0 ||
      m_activePane != ACTIVEPANE_FILELIST)
  {
    //yes, so examine the folder browser

    flags |= Action::IS_DIR;
    wxString path = m->folderBrowser->GetPath ();

    svn::Path pathUtf8 (PathUtf8 (path));
    if (pathUtf8.length () > 0)
    {
      flags |= Action::SINGLE_TARGET;
      if (pathUtf8.isUrl ())
      {
        flags |= Action::RESPOSITORY_TYPE;
      }
      else
      {
        if (svn::Wc::checkWc (pathUtf8.c_str ()))
        {
          flags |= Action::VERSIONED_WC_TYPE;
        }
        else
        {
          flags |= Action::UNVERSIONED_WC_TYPE;
        }
      }
    }
  }
  else
  {
    //no, ask the list control
    flags = m->listCtrl->GetSelectionActionFlags ();
  }

  return flags;
}
#endif


void
RapidSvnFrame::ShowInfo ()
{
  bool withUpdate = false;
  if (m->listCtrl)
    withUpdate = m->listCtrl->GetWithUpdate ();

  FileInfo fileInfo (m_context, withUpdate);

  try
  {
    std::vector<svn::Path> vector = m->GetStatusSel ().targets ();
    std::vector<svn::Path>::const_iterator it;

    for (it = vector.begin (); it != vector.end (); it++)
    {
      svn::Path path = *it;
      fileInfo.addPath (path.c_str ());
    }

    wxString info = fileInfo.info ();

    ReportDlg dlg (this, _("Info"), info, NORMAL_REPORT);
    dlg.ShowModal ();
  }
  catch (svn::ClientException & e)
  {
    m->Trace (Utf8ToLocal (e.message ()));
    return;
  }

}


void
RapidSvnFrame::ShowPreferences ()
{
  Preferences prefs;
  PreferencesDlg dlg (this, & prefs);
  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    m->folderBrowser->SetAuthCache (prefs.useAuthCache);
    m->folderBrowser->SetAuthPerBookmark (prefs.authPerBookmark);
  }
}

void
RapidSvnFrame::UpdateCurrentPath ()
{
  if (m->folderBrowser == 0)
  {
    m->currentPath.Clear ();
    m_context = 0;
  }
  else
  {
    m->currentPath = m->folderBrowser->GetPath ();
    m_context = m->folderBrowser->GetContext ();
  }

  if (m->currentPath.Length () > 0)
    SetTitle (m_title + wxT(" - ") + m->currentPath);
  else
    SetTitle (m_title);
}

bool
RapidSvnFrame::InvokeDefaultAction ()
{
  const svn::StatusSel & statusSel = m->GetStatusSel ();

  // the default action will be invoked only for a single file
  // or folder.
  // if more or less than one file or folder is selected nothing
  // will happen.

  if (1 != statusSel.size ())
    return false;

  if (statusSel.hasDirs ())
  {
    // go one folder deeper...
    m->folderBrowser->SelectFolder (PathToNative (statusSel.target ()));
  }
  else
  {
    Perform (new ExternalProgramAction (this, -1, false));
  }

  return true;
}

void
RapidSvnFrame::Perform (Action * action)
{
  try
  {
    m->SetRunning (true);
    m->listener.cancel (false);

    svn::Path currentPathUtf8 (PathUtf8 (m->currentPath));
    action->SetPath (currentPathUtf8);
    action->SetContext (m_context);
    action->SetStatusSel (m->GetStatusSel ());
    action->SetTracer (m->logTracer, false);
    m_actionWorker->SetTracer (m->logTracer);
    m_actionWorker->SetContext (m_context, false);
    if (!m_actionWorker->Perform (action) &&
        m_actionWorker->GetState () != ACTION_RUNNING)
      m->SetRunning (false);
  }
  catch (...)
  {
    m->SetRunning (false);
    throw; // svn::Excepton (e);
  }
}

inline void
RapidSvnFrame::UpdateMenuSorting ()
{
  m->CheckSort (m->listCtrl->GetSortColumn() + ID_ColumnSort_Min + 1);
}

inline void
RapidSvnFrame::UpdateMenuIncludePath ()
{
  m->CheckMenu (ID_Include_Path, m->listCtrl->GetIncludePath());
}

inline void
RapidSvnFrame::UpdateMenuAscending ()
{
  m->CheckMenu (ID_Sort_Ascending, m->listCtrl->GetSortAscending());
}

void
RapidSvnFrame::SetIncludePathVisibility (bool flatMode)
{
  if (flatMode == false)
  {
    if (m->listCtrl->GetIncludePath () == true)
      m->CheckMenu (ID_Include_Path, false);
 
    UpdateMenuSorting ();
    UpdateMenuAscending ();
  }
  else
  {
    UpdateMenuIncludePath ();
  }
  m->EnableMenuEntry (ID_Include_Path, flatMode);
}

void
RapidSvnFrame::OnSize (wxSizeEvent & sizeEvent)
{
  if (!this->IsMaximized () && IsShown ())
  {
    m->vertSplitter->SetSashGravity (0.0f);
    m->horizSplitter->SetSashGravity (1.0f);
  }

  sizeEvent.Skip ();
}

void 
RapidSvnFrame::OnFocusChanged (wxCommandEvent & event)
{
  switch (event.GetId ())
  {
  case FOLDER_BROWSER:
    SetActivePane (ACTIVEPANE_FOLDER_BROWSER);
    break;
    
  case FILELIST_CTRL:
    SetActivePane (ACTIVEPANE_FILELIST);
    break;

  default:
    ; // unknown/not interesting
  }
}
  

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
