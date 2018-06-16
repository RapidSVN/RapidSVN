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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
/**
 * @file main_frame.cpp
 */
// wxWidgets
#include "wx/confbase.h"
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "wx/listctrl.h"
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
#include "hist_entries.hpp"
#include "hist_mgr.hpp"
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
#include "log_action.hpp"

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


#include "main_frame.hpp"
#include "main_frame_helper.hpp"

#include "filelist_ctrl_drop_target.hpp"
#include "folder_browser_drop_target.hpp"

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

#include "res/bitmaps/flat_mode.png.h"
#include "res/bitmaps/nonsvn_file.png.h"
#include "res/bitmaps/normal_file.png.h"
#include "res/bitmaps/modified_file.png.h"
#include "res/bitmaps/conflicted_file.png.h"

#ifdef USE_DEBUG_TESTS
#include "checkout_action.hpp"
#include "cert_dlg.hpp"
#include "destination_dlg.hpp"
#include "dnd_dlg.hpp"
#endif

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

/**
 * Local helper function to create the action worker
 *
 * @param parent
 * @return new action worker instance
 */
ActionWorker *
CreateActionWorker(wxWindow * parent)
{
#ifdef USE_SIMPLE_WORKER
  return new SimpleWorker(parent);
#else
  return new ThreadedWorker(parent);
#endif
}

/**
 * class that hide implementation specific data and methods from
 * the interface
 */
struct MainFrame::Data
{
public:
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
  bool showUnversioned;
  bool showUnmodified;
  bool showModified;
  bool showConflicted;
  int horizSashPos;
  int vertSashPos;
  int idleCount;

private:
  bool m_running;
  wxFrame * m_parent;
  bool m_isErrorDialogActive;
  FolderBrowser * m_folderBrowser;
  FileListCtrl * m_listCtrl;
  LogList * m_log;

public:

  /**
   * This instance of @a apr is used to initialize/terminate apr
   */
  svn::Apr apr;

  Data(wxFrame * parent, FolderBrowser * folderBrowser,
       FileListCtrl * listCtrl, LogList * log,
       const wxLocale & locale_)
    : MenuColumns(0), MenuSorting(0), MenuBar(0),
      listener(parent),
      updateAfterActivate(false), dontUpdateFilelist(false),
      skipFilelistUpdate(false), locale(locale_),
      currentPath(wxT("")),
      activePane(ACTIVEPANE_FOLDER_BROWSER),
      showUnversioned(false), showUnmodified(false),
      showModified(false), showConflicted(false), idleCount(0),
      m_running(false), m_parent(parent),
      m_isErrorDialogActive(false),
      m_folderBrowser(folderBrowser),
      m_listCtrl(listCtrl), m_log(log)
  {
    InitializeMenu();
  }

  void
  InitializeMenu()
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
    AppendMenuItem(*menuFile, ID_Quit);
#endif

    // Columns menu
    MenuColumns = new wxMenu;
    AppendMenuItem(*MenuColumns, ID_Column_Reset);
    MenuColumns->AppendSeparator();

    // Sorting menu
    MenuSorting = new wxMenu;
    MenuSorting->AppendCheckItem(ID_Include_Path, _("Use Path for Sorting"));
    MenuSorting->AppendCheckItem(ID_Sort_Ascending, _("Sort Ascending"));

    MenuSorting->AppendSeparator();

    for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
    {
      int id = ColumnList[col].id;
      int sortid = id + Columns::SORT_COLUMN_OFFSET;

      if (id != ID_Column_Name && id != ID_Column_Path)
        MenuColumns->AppendCheckItem(id, ColumnList[col].caption);
      MenuSorting->AppendRadioItem(sortid, ColumnList[col].caption);
    }

    // View menu
    wxMenu *menuView = new wxMenu;
    AppendMenuItem(*menuView, ID_Refresh);
    AppendMenuItem(*menuView, ID_Stop);
    menuView->AppendSeparator();

    AppendMenuItem(*menuView, ID_Explore);
    menuView->AppendSeparator();

    menuView->Append(0, _("Columns"), MenuColumns);
    menuView->Append(0, _("Sort"), MenuSorting);

    menuView->AppendCheckItem(ID_RefreshWithUpdate, _("Refresh with Update"));
    menuView->AppendCheckItem(ID_Flat, _("Show subdirectories"));
    menuView->AppendCheckItem(ID_IndicateModifiedChildren, _("Indicate modified children"));
    menuView->AppendCheckItem(ID_ShowUnversioned, _("Show unversioned entries"));
    menuView->AppendCheckItem(ID_ShowUnmodified, _("Show unmodified entries"));
    menuView->AppendCheckItem(ID_ShowModified, _("Show modified entries"));
    menuView->AppendCheckItem(ID_ShowConflicted, _("Show conflicted entries"));
    menuView->AppendCheckItem(ID_IgnoreExternals, _("Ignore Externals"));
    menuView->AppendCheckItem(ID_ShowIgnored, _("Show ignored entries"));

    // Preferences menu item goes to its own place on MacOSX,
    // so no separator is necessary.
#ifndef __WXMAC__
    menuView->AppendSeparator();
#endif
    AppendMenuItem(*menuView, ID_Preferences);

    // Repository menu
    wxMenu *menuRepos = new wxMenu;
    menuRepos->Append(ID_Import, _("&Import...\tCTRL-I"));
    menuRepos->Append(ID_Export, _("&Export...\tCTRL-E"));
    menuRepos->Append(ID_Checkout, _("Check&out...\tCTRL-O"));
    menuRepos->Append(ID_CreateRepository, _("&Create..."));

    menuRepos->AppendSeparator();

    menuRepos->Append(ID_Merge, _("Merge..."));
    menuRepos->Append(ID_Switch, _("Switch URL...\tCTRL-S"));

    // Modify menu
    wxMenu *menuModif = new wxMenu;
    AppendModifyMenu(menuModif);

    // Query menu
    wxMenu *menuQuery = new wxMenu;
    AppendQueryMenu(menuQuery, true);

    // Bookmarks menu
    wxMenu *menuBookmarks = new wxMenu;
    AppendBookmarksMenu(menuBookmarks);

    // Extras menu
    wxMenu *menuExtras = new wxMenu;
    AppendMenuItem(*menuExtras, ID_Cleanup);
    AppendMenuItem(*menuExtras, ID_Upgrade);

    // Help Menu
    wxMenu *menuHelp = new wxMenu;

    menuHelp->Append(ID_HelpContents, _("&Contents\tF1"));
    menuHelp->Append(ID_HelpIndex, _("&Index\tShift+F1"));
    menuHelp->AppendSeparator();
#ifdef USE_STARTUP_TIPS
    menuHelp->Append(ID_HelpStartupTips, _("Show Startup Tips"));
    menuHelp->AppendSeparator();
#endif
    menuHelp->Append(ID_About, _("&About..."));

#ifdef USE_DEBUG_TESTS
    // Debug Menu
    wxMenu *menuTests = new wxMenu;

    menuTests->Append(ID_TestNewWxString, wxT("wxString Creation&Tracing Test"));
    menuTests->Append(ID_TestListener, wxT("Listener Test"));
    menuTests->Append(ID_TestCheckout, wxT("Checkout Test"));
    menuTests->Append(ID_TestCertDlg, wxT("Certificate Dlg"));
    menuTests->Append(ID_TestDestinationDlg, wxT("Destination Dlg (Simple)"));
    menuTests->Append(ID_TestDestinationDlgWithForce, wxT("Destination Dlg (With Force)"));

    wxMenu * menuDnd = new wxMenu;
    menuDnd->AppendRadioItem(ID_TestDndImport, wxT("with action \"Import\""));
    menuDnd->AppendRadioItem(ID_TestDndCopy, wxT("with action \"Copy\""));
    menuDnd->AppendRadioItem(ID_TestDndCopyMove, wxT("with action \"Copy/mive\""));
    menuDnd->Append(ID_TestDndDlg, wxT("Show Dialog"));
    menuTests->AppendSubMenu(menuDnd, wxT("Dnd Dlg"));
#endif

    // Create the menu bar and append the menus
    MenuBar = new wxMenuBar;
    // Under wxMac the menu might be empty, so
    // don't show it:
    if (menuFile->GetMenuItemCount()>0)
      MenuBar->Append(menuFile, _("&File"));
    MenuBar->Append(menuView, _("&View"));
    MenuBar->Append(menuRepos, _("&Repository"));
    MenuBar->Append(menuModif, _("&Modify"));
    MenuBar->Append(menuQuery, _("&Query"));
    MenuBar->Append(menuBookmarks, _("&Bookmarks"));
    MenuBar->Append(menuExtras, _("&Extras"));
    MenuBar->Append(menuHelp, _("&Help"));
#ifdef USE_DEBUG_TESTS
    MenuBar->Append(menuTests, _("&Tests"));
#endif
  }

  bool
  IsColumnChecked(int id)
  {
    return MenuColumns->IsChecked(id);
  }

  void
  CheckColumn(int id, bool check)
  {
    MenuColumns->Check(id, check);
  }

  void
  CheckSort(int id)
  {
    MenuSorting->Check(id, true);
  }

  void
  EnableMenuEntry(int id, bool enable)
  {
    MenuSorting->Enable(id, enable);
  }

  bool
  IsMenuChecked(int id)
  {
    return MenuBar->IsChecked(id);
  }

  void
  CheckMenu(int id, bool check)
  {
    MenuBar->Check(id, check);
  }

  void
  CheckTool(int id, bool check)
  {
    wxToolBar * toolbar = m_parent->GetToolBar();

    if (0 != toolbar)
      toolbar->ToggleTool(id, check);
  }

  void
  SetMenuAndTool(int id, bool & toggleValue, bool newValue)
  {
    toggleValue = newValue;

    // first update the menu entry
    MenuBar->Check(id, toggleValue);

    // next update the tool
    wxToolBarBase * toolBar = m_parent->GetToolBar();
    if (toolBar != 0)
      toolBar->ToggleTool(id, toggleValue);
  }

  bool
  ToggleMenuAndTool(int id, bool & toggleValue)
  {
    SetMenuAndTool(id, toggleValue, !toggleValue);

    return toggleValue;
  }

  void
  SetRunning(bool running)
  {
    if (running && ! m_running)
      listener.cancel(false);

    m_running = running;

    wxToolBarBase * toolBar = m_parent->GetToolBar();

    wxASSERT(toolBar != 0);

    toolBar->EnableTool(ID_Stop, running);
    MenuBar->Enable(ID_Stop, running);

    m_parent->SetCursor(running ? *wxHOURGLASS_CURSOR : *wxSTANDARD_CURSOR);

    if (m_folderBrowser)
    {
      m_folderBrowser->SetCursor(running ? *wxHOURGLASS_CURSOR : *wxSTANDARD_CURSOR);
      m_folderBrowser->Enable(!running);
    }
    if (m_listCtrl)
    {
      m_listCtrl->SetCursor(running ? *wxHOURGLASS_CURSOR : *wxSTANDARD_CURSOR);
      m_listCtrl->Enable(!running);
    }
  }

  bool
  IsRunning() const
  {
    return m_running;
  }


  /**
   * Checks whether @ref currenPath is an URL
   *
   * @retval true valid URL
   */
  bool
  IsUrl() const
  {
    return svn::Url::isValid(LocalToUtf8(currentPath).c_str());
  }


  /**
   * Checks whether the selected bookmark is "flat"
   *
   * @retval true yes, it's flat
   */
  bool
  IsFlat() const
  {
    if (!m_listCtrl)
      return 0;

    return m_listCtrl->IsFlat();
  }


  /**
   * Checks whether there is an error dialog showing
   * at the moment (not an action dialog).
   *
   * This information is used to decide whether to refresh
   * in @ref MainFrame::OnActivate
   *
   * @retval true there is an error dialog showing
   */
  bool
  IsErrorDialogActive() const
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
  ShowErrorDialog(const wxString & msg)
  {
    m_isErrorDialogActive = true;
    wxMessageBox(msg, _("RapidSVN Error"), wxICON_ERROR | wxOK);
    m_isErrorDialogActive = false;
  }


  /**
   * add message to log window
   *
   * @param msg message to show
   */
  void
  Trace(LogItemType type, const wxString & action, const wxString & msg)
  {
    if (!m_log)
      return;
    m_log->AppendItem(type, action, msg);
    //m_log->AppendText(msg + wxT('\n'));
  }

  /**
   * add error message to log window marked red
   *
   * @param msg error message to show
   * @param showDialog show an error dialog with the message
   *                   (besides logging this)
   */
  void
  TraceError(const wxString & msg, bool showDialog=true)
  {
    if (!m_log)
      return;

    m_log->AppendItem(LogItem_Error, _("Error"), msg);
    /*m_log->SetDefaultStyle(wxTextAttr(*wxRED));
    m_log->AppendText(wxString::Format(_("Error: %s\n"), msg.c_str()));
    m_log->SetDefaultStyle(wxTextAttr(*wxBLACK));*/

    if (showDialog)
      ShowErrorDialog(msg);
  }

  const svn::StatusSel &
  GetStatusSel()  const
  {
    //is there nothing selected in the list control,
    //or is the active window *not* the list control?
    if (m_listCtrl->GetSelectedItemCount() <= 0 ||
        activePane != ACTIVEPANE_FILELIST)
    {
      return m_folderBrowser->GetStatusSel();
    }
    else
    {
      //no, build the file list from the list control
      return m_listCtrl->GetStatusSel();
    }
  }
};


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(ID_AddWcBookmark, MainFrame::OnAddWcBookmark)
  EVT_MENU(ID_AddRepoBookmark, MainFrame::OnAddRepoBookmark)
  EVT_MENU(ID_RemoveBookmark, MainFrame::OnRemoveBookmark)
  EVT_MENU(ID_EditBookmark, MainFrame::OnEditBookmark)
  EVT_MENU(ID_Quit, MainFrame::OnQuit)

  EVT_MENU(ID_Preferences, MainFrame::OnPreferences)
  EVT_MENU(ID_Refresh, MainFrame::OnRefresh)
  EVT_UPDATE_UI(ID_Refresh, MainFrame::OnUpdateCommand)

  EVT_MENU(ID_Column_Reset, MainFrame::OnColumnReset)
  EVT_MENU_RANGE(ID_Column_Min, ID_Column_Max, MainFrame::OnColumn)
  EVT_MENU(ID_Include_Path, MainFrame::OnIncludePath)
  EVT_MENU(ID_Sort_Ascending, MainFrame::OnSortAscending)
  EVT_MENU_RANGE(ID_ColumnSort_Min, ID_ColumnSort_Max, MainFrame::OnColumnSorting)

  EVT_MENU(ID_Flat, MainFrame::OnFlatView)
  EVT_MENU(ID_RefreshWithUpdate, MainFrame::OnRefreshWithUpdate)
  EVT_MENU(ID_ShowUnversioned, MainFrame::OnShowUnversioned)
  EVT_MENU(ID_IgnoreExternals, MainFrame::OnIgnoreExternals)
  EVT_MENU(ID_ShowIgnored, MainFrame::OnShowIgnored)
  EVT_MENU(ID_ShowUnmodified, MainFrame::OnShowUnmodified)
  EVT_MENU(ID_ShowModified, MainFrame::OnShowModified)
  EVT_MENU(ID_ShowConflicted, MainFrame::OnShowConflicted)
  EVT_MENU(ID_IndicateModifiedChildren, MainFrame::OnIndicateModifiedChildren)

  EVT_MENU(ID_Login, MainFrame::OnLogin)
  EVT_MENU(ID_Logout, MainFrame::OnLogout)
  EVT_MENU(ID_Stop, MainFrame::OnStop)

  EVT_MENU(ID_Info, MainFrame::OnInfo)
  EVT_UPDATE_UI(ID_Info, MainFrame::OnUpdateCommand)

  EVT_MENU(ID_HelpContents, MainFrame::OnHelpContents)
  EVT_MENU(ID_HelpIndex, MainFrame::OnHelpIndex)
  EVT_MENU(ID_HelpStartupTips, MainFrame::OnHelpStartupTips)
  EVT_MENU(ID_About, MainFrame::OnAbout)

#ifdef USE_DEBUG_TESTS
  EVT_MENU(ID_TestNewWxString, MainFrame::OnTestNewWxString)
  EVT_MENU(ID_TestListener, MainFrame::OnTestListener)
  EVT_MENU(ID_TestCheckout, MainFrame::OnTestCheckout)
  EVT_MENU(ID_TestCertDlg, MainFrame::OnTestCertDlg)
  EVT_MENU(ID_TestDestinationDlg, MainFrame::OnTestDestinationDlg)
  EVT_MENU(ID_TestDestinationDlgWithForce, MainFrame::OnTestDestinationDlg)
  EVT_MENU(ID_TestDndDlg, MainFrame::OnTestDndDlg)
#endif

  EVT_MENU(ID_Log_Clear, MainFrame::OnLogClear)
  EVT_MENU_RANGE(ID_Log_Min, ID_Log_Max, MainFrame::OnLogToggle)
  EVT_UPDATE_UI_RANGE(ID_Log_Min, ID_Log_Max, MainFrame::OnLogUpdate)

  EVT_MENU_RANGE(ID_File_Min, ID_File_Max, MainFrame::OnFileCommand)
  EVT_MENU_RANGE(ID_Verb_Min, ID_Verb_Max, MainFrame::OnFileCommand)

  EVT_UPDATE_UI_RANGE(ID_File_Min, ID_File_Max, MainFrame::OnUpdateCommand)
  EVT_UPDATE_UI_RANGE(ID_Verb_Min, ID_Verb_Max, MainFrame::OnUpdateCommand)
  EVT_UPDATE_UI_RANGE(ID_Filter_Min, ID_Filter_Max, MainFrame::OnUpdateCommand)

  EVT_MENU(ACTION_EVENT, MainFrame::OnActionEvent)

  EVT_TOOL_ENTER(ID_TOOLBAR, MainFrame::OnToolEnter)
  EVT_TREE_SEL_CHANGED(-1, MainFrame::OnFolderBrowserSelChanged)
  EVT_TREE_KEY_DOWN(-1, MainFrame::OnFolderBrowserKeyDown)
  EVT_LIST_ITEM_SELECTED(-1, MainFrame::OnFileListSelected)

  EVT_MENU_RANGE(LISTENER_MIN, LISTENER_MAX, MainFrame::OnListenerEvent)

  EVT_MENU(ID_LogList_BrowseTo, MainFrame::OnLogListBrowse)
  EVT_MENU(ID_LogList_Diff, MainFrame::OnLogListDiff)
END_EVENT_TABLE()

/** class implementation **/
MainFrame::MainFrame(const wxString & title,
                     const wxLocale & locale)
  : MainFrameBase((wxFrame *) NULL, -1, title, wxDefaultPosition, wxDefaultSize,
                  wxDEFAULT_FRAME_STYLE),
    m_title(title), m_context(0)
{
  m = new Data(this, m_folderBrowser, m_listCtrl, m_log, locale);
  m_actionWorker = CreateActionWorker(this);

  // enable trace
  wxLog::AddTraceMask(TraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *cfg = wxConfigBase::Get();

  // Now set icons for the application
  // (several sizes are available OS/windowing system
  // picks whatever it thinks is suitable)
  wxIconBundle iconBundle;
  iconBundle.AddIcon(wxIcon(rapidsvn_16x16_xpm));
  iconBundle.AddIcon(wxIcon(rapidsvn_32x32_xpm));
  iconBundle.AddIcon(wxIcon(rapidsvn_48x48_xpm));
  SetIcons(iconBundle);

  SetMenuBar(m->MenuBar);

  // Create the toolbar
  CreateMainToolBar(this);
  m->SetRunning(false);

  // Populate the logFilterBar
  CreateLogFilterBar(m_logFilterBar);
  /*  // as much as the widget can stand
  #ifndef __WXGTK__
    m_log->SetMaxLength(0);
  #endif */

  m->logTracer = new EventTracer(this);
  m->listener.SetTracer(m->logTracer, false);


  // Create the list control to display files
  m->CheckMenu(ID_Flat,              false);
  m->CheckMenu(ID_RefreshWithUpdate, m_listCtrl->GetWithUpdate());
  m->SetMenuAndTool(ID_ShowUnversioned, m->showUnversioned,
                    m_listCtrl->GetShowUnversioned());
  m->SetMenuAndTool(ID_ShowUnmodified, m->showUnmodified, true);
  m->SetMenuAndTool(ID_ShowModified, m->showModified, true);
  m->SetMenuAndTool(ID_ShowConflicted, m->showConflicted, true);
  m->CheckMenu(ID_IgnoreExternals,   m_listCtrl->GetIgnoreExternals());
  m->CheckMenu(ID_ShowIgnored,       m_listCtrl->GetShowIgnored());
  m->CheckMenu(ID_Sort_Ascending,    m_listCtrl->GetSortAscending());
  m->CheckSort(m_listCtrl->GetSortColumn() + ID_ColumnSort_Min + 1);
  m_listCtrl->SetShowUnmodified(true);
  m_listCtrl->SetShowModified(true);
  m_listCtrl->SetShowConflicted(true);

  bool isFlat = m_listCtrl->IsFlat();
  bool includePath = m_listCtrl->GetIncludePath();
  if (isFlat == false && includePath == true)
    m->CheckMenu(ID_Include_Path, false);
  else
    m->CheckMenu(ID_Include_Path, includePath);
  m->EnableMenuEntry(ID_Include_Path, isFlat ? true : false);

  // Adapt the menu entries
  for (int col=0; col < FileListCtrl::COL_COUNT; col++)
  {
    int id = m->ColumnList[col].id;
    int sortid = id + Columns::SORT_COLUMN_OFFSET;
    if (id != ID_Column_Name && id != ID_Column_Path)
    {
      bool check = m_listCtrl->GetColumnVisible(col);
      m->CheckColumn(id, check);
      m->EnableMenuEntry(sortid, check);
    }
  }

  RefreshFileList();

  // Read frame position
  int x = cfg->Read(ConfigLeft, 50);
  int y = cfg->Read(ConfigTop, 50);
  int w = cfg->Read(ConfigWidth, 806);
  int h = cfg->Read(ConfigHeight, 480);

  Move(x, y);
  SetClientSize(w, h);

  if (cfg->Read(ConfigMaximized, (long int)0) == 1)
    Maximize(true);

  // Get sash position for every splitter from configuration.
  GetClientSize(&w, &h);
  m->vertSashPos = cfg->Read(ConfigSplitterVert, w / 3);
  m->horizSashPos = cfg->Read(ConfigSplitterHoriz, (3 * h) / 4);

  // initialize the folder browser
  m_folderBrowser->ReadConfig(cfg);
  {
    Preferences prefs;
    m_folderBrowser->SetListener(&m->listener);
    m_folderBrowser->SetAuthCache(prefs.useAuthCache);
    m_folderBrowser->SetAuthPerBookmark(prefs.authPerBookmark);
  }
  UpdateCurrentPath();
  RefreshFolderBrowser();

  // Initialize for drag and drop
  m_folderBrowser->SetDropTarget(new FolderBrowserDropTarget(m_folderBrowser));
  m_listCtrl->SetDropTarget(new FileListCtrlDropTarget(m_folderBrowser, m_listCtrl));

  // this is a workaround for the buggy Splitter initialisation
  Connect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle), NULL, this);
}

MainFrame::~MainFrame()
{
  wxConfigBase *cfg = wxConfigBase::Get();
  if (cfg == NULL)
    return;

  if (m->logTracer)
    delete m->logTracer;

  if (m_actionWorker)
    delete m_actionWorker;

  // Save frame size and position.
  if (!IsIconized())
  {
    if (IsMaximized())
      cfg->Write(ConfigMaximized, 1);
    else
    {
      int x, y;
      int w, h;

      GetClientSize(&w, &h);
      GetPosition(&x, &y);

      cfg->Write(ConfigMaximized, 0);
      cfg->Write(ConfigLeft, (long) x);
      cfg->Write(ConfigTop, (long) y);
      cfg->Write(ConfigWidth, (long) w);
      cfg->Write(ConfigHeight, (long) h);
    }
  }

  // Save splitter positions
  cfg->Write(ConfigSplitterVert,
             (long) m_splitterVert->GetSashPosition());
  cfg->Write(ConfigSplitterHoriz,
             (long) m_splitterHoriz->GetSashPosition());

  m_folderBrowser->WriteConfig(cfg);

  delete m;
}

void
MainFrame::SetActivePane(ActivePane value)
{
  m->activePane = value;
}

void
MainFrame::TrimDisabledMenuItems(wxMenu & menu)
{
  // Check for disabled items
  size_t pos = menu.GetMenuItemCount();
// TODO  unsigned int selectionActionFlags = GetSelectionActionFlags ();
  const svn::StatusSel & statusSel = m->GetStatusSel();
  while (pos-- > 0)
  {
    wxMenuItem *pItem = menu.FindItemByPosition(pos);
    if (!pItem->IsSeparator() &&
        !ActionFactory::CheckIdForStatusSel(pItem->GetId(), statusSel))
    {
      menu.Destroy(pItem);
    }
  }

  // Trim unnecessary separators
  pos = menu.GetMenuItemCount();

  if (0 == pos)
    return;

  bool sepNeeded = false;
  while (pos-- > 0)
  {
    wxMenuItem *pItem = menu.FindItemByPosition(pos);
    if (pItem->IsSeparator())
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
        menu.Destroy(pItem);
      }
    }
    else
    {
      sepNeeded = true;
    }
  }
}

void
MainFrame::RefreshFileList()
{
  if (m->dontUpdateFilelist)
    return;

  bool isRunning = m->IsRunning();
  if (!isRunning)
    m->SetRunning(true);

  if (m_listCtrl && m_folderBrowser)
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
      if (m->currentPath.length() == 0)
      {
        // calling "UpdateColumns" should be necessary
        // only for the first call. But without any
        // items this call should be cheap.
        m_listCtrl->DeleteAllItems();
        m_listCtrl->UpdateColumns();
      }
      else
      {
        m_listCtrl->SetContext(m_context);
        m_listCtrl->RefreshFileList(m->currentPath);
      }

    }
    catch (svn::ClientException & e)
    {
      wxString msg, errtxt(Utf8ToLocal(e.message()));
      msg.Printf(_("Error while updating filelist (%s)"),
                 errtxt.c_str());
      m->TraceError(msg, false);

      // calling "UpdateColumns" should be necessary
      // only for the first call. But without any
      // items this call should be cheap.
      m_listCtrl->DeleteAllItems();
      m_listCtrl->UpdateColumns();

    }
    catch (...)
    {
      m->TraceError(_("Error while updating filelist"), false);

      // calling "UpdateColumns" should be necessary
      // only for the first call. But without any
      // items this call should be cheap.
      m_listCtrl->DeleteAllItems();
      m_listCtrl->UpdateColumns();
    }

  }
  if (!isRunning)
    m->SetRunning(false);
}

void
MainFrame::RefreshFolderBrowser()
{
  wxBusyCursor busy;

  bool isRunning = m->IsRunning();
  if (!isRunning)
    m->SetRunning(true);

  try
  {
    if (!m->skipFilelistUpdate)
      m->dontUpdateFilelist = true;

    if (m_folderBrowser)
      m_folderBrowser->RefreshFolderBrowser();
  }
  catch (...)
  {
    m->skipFilelistUpdate = false;
  }

  if (!isRunning)
    m->SetRunning(false);

  if (!m->skipFilelistUpdate)
  {
    m->dontUpdateFilelist = false;
    RefreshFileList();
  }
  m->skipFilelistUpdate = false;
}

/*** START OF SECTION EVENTS ***/

void
MainFrame::OnActivate(wxActivateEvent & event)
{
  // in the past we used to refresh only if we had
  // had an action which changed stuff.
  //
  // now we refresh every time - but only for
  // working copies and if NOT flat
  // and NOT if another action is running.
  wxFrame::OnActivate(event);
  if (event.GetActive())
  {
    if (m->updateAfterActivate ||
        (!m->IsUrl() &&
         !m->IsFlat() &&
         !m->IsRunning() &&
         !m->IsErrorDialogActive()))
    {
      m->updateAfterActivate = false;

      RefreshFileList();
    }
  }

  // wxMac needs this, otherwise the menu doesn't show:
#ifdef __WXMAC__
  event.Skip();
#endif
}

void
MainFrame::OnAddWcBookmark(wxCommandEvent & WXUNUSED(event))
{
  AddWcBookmark();
}

void
MainFrame::OnAddRepoBookmark(wxCommandEvent & WXUNUSED(event))
{
  AddRepoBookmark();
}

void
MainFrame::OnRemoveBookmark(wxCommandEvent & WXUNUSED(event))
{
  RemoveBookmark();
}

void
MainFrame::OnEditBookmark(wxCommandEvent & WXUNUSED(event))
{
  EditBookmark();
}

void
MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
  Close(TRUE);
}

void
MainFrame::OnPreferences(wxCommandEvent & WXUNUSED(event))
{
  ShowPreferences();
}

void
MainFrame::OnRefresh(wxCommandEvent & WXUNUSED(event))
{
  RefreshFolderBrowser();
}

void
MainFrame::OnColumnReset(wxCommandEvent & WXUNUSED(event))
{
  m_listCtrl->ResetColumns();
  for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
  {
    bool visible = m_listCtrl->GetColumnVisible(col);
    int id = m->ColumnList [col].id;

    if (id != ID_Column_Name && id != ID_Column_Path)
      m->CheckColumn(id, visible);

    int sortid = id + Columns::SORT_COLUMN_OFFSET;
    m->EnableMenuEntry(sortid, visible);
  }
  RefreshFileList();
}

void
MainFrame::OnColumn(wxCommandEvent & event)
{
  int eventId = event.GetId();
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
    m_listCtrl->SetColumnVisible(col, visible);

    int sortid = eventId + Columns::SORT_COLUMN_OFFSET;
    m->EnableMenuEntry(sortid, visible);

    UpdateMenuSorting();
    UpdateMenuAscending();

    RefreshFileList();
  }
}

void
MainFrame::OnIncludePath(wxCommandEvent & WXUNUSED(event))
{
  m_listCtrl->SetIncludePath(!m_listCtrl->GetIncludePath());
}

void
MainFrame::OnSortAscending(wxCommandEvent & WXUNUSED(event))
{
  m_listCtrl->SetSortAscending(!m_listCtrl->GetSortAscending());
}

void
MainFrame::OnColumnSorting(wxCommandEvent & event)
{
  // we dont want to list FileListCtrl::COL_NAME/COL_PATH/... here
  int col = event.GetId() - ID_ColumnSort_Name;

  m_listCtrl->SetSortColumn(col);
  m_listCtrl->SetSortAscending(true);

  UpdateMenuAscending();
}


void
MainFrame::OnFlatView(wxCommandEvent & WXUNUSED(event))
{
  bool newFlatMode = !m_folderBrowser->IsFlat();

  // if this cannot be set (e.g. invalid selection
  // like the root element, we uncheck this
  if (!m_folderBrowser->SetFlat(newFlatMode))
    newFlatMode = false;

  m->CheckMenu(ID_Flat, newFlatMode);
  m->CheckTool(ID_Flat, newFlatMode);
  m_listCtrl->SetFlat(newFlatMode);

  SetIncludePathVisibility(newFlatMode);

  RefreshFileList();
}


void
MainFrame::OnIndicateModifiedChildren(wxCommandEvent & WXUNUSED(event))
{
  bool newValue = !m_folderBrowser->GetIndicateModifiedChildren();

  // if this cannot be set (e.g. invalid selection
  // like the root element, we uncheck this
  if (!m_folderBrowser->SetIndicateModifiedChildren(newValue))
    newValue = false;

  m->CheckMenu(ID_IndicateModifiedChildren, newValue);

  RefreshFolderBrowser();
}


void
MainFrame::OnRefreshWithUpdate(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->IsMenuChecked(ID_RefreshWithUpdate);
  m_listCtrl->SetWithUpdate(checked);
  RefreshFolderBrowser();
}


void
MainFrame::OnShowUnversioned(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->ToggleMenuAndTool(ID_ShowUnversioned, m->showUnversioned);
  m_listCtrl->SetShowUnversioned(checked);
  RefreshFileList();
}

void
MainFrame::OnShowUnmodified(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->ToggleMenuAndTool(ID_ShowUnmodified, m->showUnmodified);
  m_listCtrl->SetShowUnmodified(checked);
  RefreshFileList();
}

void
MainFrame::OnShowModified(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->ToggleMenuAndTool(ID_ShowModified, m->showModified);
  m_listCtrl->SetShowModified(checked);
  RefreshFileList();
}

void
MainFrame::OnShowConflicted(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->ToggleMenuAndTool(ID_ShowConflicted, m->showConflicted);
  m_listCtrl->SetShowConflicted(checked);
  RefreshFileList();
}

void
MainFrame::OnIgnoreExternals(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->IsMenuChecked(ID_IgnoreExternals);
  m_listCtrl->SetIgnoreExternals(checked);
  RefreshFileList();
}

void
MainFrame::OnShowIgnored(wxCommandEvent & WXUNUSED(event))
{
  bool checked = m->IsMenuChecked(ID_ShowIgnored);
  m_listCtrl->SetShowIgnored(checked);
  RefreshFileList();
}

void
MainFrame::OnLogin(wxCommandEvent & WXUNUSED(event))
{
  svn::Context * context = m_folderBrowser->GetContext();

  if (context == 0)
    return;

  wxString LocalUsername(Utf8ToLocal(context->getUsername()));
  AuthDlg dlg(this, LocalUsername);
  bool ok = dlg.ShowModal() == wxID_OK;

  if (ok)
  {
    std::string username, password;
    LocalToUtf8(dlg.GetUsername(), username);
    LocalToUtf8(dlg.GetPassword(), password);
    context->setLogin(username.c_str(), password.c_str());
  }
}

void
MainFrame::OnLogout(wxCommandEvent & WXUNUSED(event))
{
  svn::Context * context = m_folderBrowser->GetContext();

  if (context == 0)
    return;

  context->setLogin("", "");
}

void
MainFrame::OnStop(wxCommandEvent & WXUNUSED(event))
{
  m->listener.cancel(true);
}

void
MainFrame::OnInfo(wxCommandEvent & WXUNUSED(event))
{
  ShowInfo();
}

void
MainFrame::OnUpdateCommand(wxUpdateUIEvent & updateUIEvent)
{
  if (m->IsRunning()) {
    updateUIEvent.Enable(false);
    return;
  }

  try
  {
    updateUIEvent.Enable(
      ActionFactory::CheckIdForStatusSel(
        updateUIEvent.GetId(), m->GetStatusSel()));
  }
  catch (svn::ClientException & e)
  {
    m->Trace(LogItem_Error, _("Error"), Utf8ToLocal(e.message()));
    return;
  }
  catch (...)
  {
    m->Trace(LogItem_Error, _("Error"), _("An error occurred while checking valid actions"));
  }
}

void
MainFrame::OnHelpContents(wxCommandEvent & WXUNUSED(event))
{
  try
  {
    OpenURL(wxT("http://www.rapidsvn.org/index.php?id=OnlineHelp:Contents"));
  }
  catch (...)
  {
    m->Trace(LogItem_Error, _("Error"), _("An error occured while launching the browser"));
  }

// WE DONT USE THIS CODE NOW
//#ifdef  USE_HTML_HELP
//  ::wxGetApp ().GetHelpController().DisplayContents();
//#endif
//#endif
}

void
MainFrame::OnHelpIndex(wxCommandEvent & WXUNUSED(event))
{
  try
  {
    OpenURL(wxT("http://www.rapidsvn.org/index.php?id=OnlineHelp:Index"));
  }
  catch (...)
  {
    m->Trace(LogItem_Error, _("Error"), _("An error occured while launching the browser"));
  }

//#ifdef  USE_HTML_HELP
//  ::wxGetApp ().GetHelpController().DisplayIndex();
//#endif
}

void
MainFrame::OnHelpStartupTips(wxCommandEvent & WXUNUSED(event))
{
#ifdef USE_STARTUP_TIPS
  RapidSvnApp& app = ::wxGetApp();
  wxTipProvider* tipProvider = app.MakeTipProvider(true);
  if (tipProvider)
  {
    bool showAtStartup = ::wxShowTip(this, tipProvider);
    app.SaveTipsInfo(showAtStartup, tipProvider->GetCurrentTip());
    delete tipProvider;
  }
#endif
}

void
MainFrame::OnAbout(wxCommandEvent & WXUNUSED(event))
{
  AboutDlg dlg(this, m->locale);

  dlg.ShowModal();
}

#ifdef USE_DEBUG_TESTS
static void
info_print_time(apr_time_t atime, const wxChar * desc, wxString & str)
{
  apr_time_exp_t extime;
  apr_status_t apr_err;

  /* if this returns an error, just don't print anything out */
  apr_err = apr_time_exp_tz(&extime, atime, 0);
  if (!apr_err)
    str.Printf(wxT("%s: %02lu:%02lu:%02lu:%02lu"), desc,
               (unsigned long)(extime.tm_hour),
               (unsigned long)(extime.tm_min),
               (unsigned long)(extime.tm_sec),
               (unsigned long)(extime.tm_usec));
}

void
MainFrame::PrintTimeMeasurements(apr_time_t start, apr_time_t end, const wxString & name)
{
  wxString msg(name);
  m->Trace(wxT('\n') + msg);

  info_print_time(start, _("Test started at"), msg);
  m->Trace(wxT("\n") + msg);

  info_print_time(end, _("Test ended at"), msg);
  m->Trace(msg);

  apr_time_t duration = end - start;
  info_print_time(duration, _("Test duration"), msg);
  m->Trace(msg + wxT("\n"));
}

void
MainFrame::OnTestNewWxString(wxCommandEvent & WXUNUSED(event))
{
  apr_time_t start = apr_time_now();

  // starting the test
  for (int i = 1; i <= 10000; i++)
  {
    wxString message;
    message.Printf(wxT("Tracing a message from newely created wxString, round #%d"), i);
    m->Trace(message);

    static apr_time_t last_access = apr_time_now();
    if (apr_time_now() - last_access > 2000000)
    {
      last_access = apr_time_now();
      wxSafeYield();
    }
  }
  // ending the test
  apr_time_t end = apr_time_now();

  wxString name(wxT("Created 10 000 wxStrings and printed them with Trace ()"));
  PrintTimeMeasurements(start, end, name);
}

void
MainFrame::OnTestListener(wxCommandEvent & WXUNUSED(event))
{
  apr_time_t start = apr_time_now();

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
        NULL,                  // NOT USED HERE svn_wc_notify_failed_revert,
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
        wxpath = Utf8ToLocal("O:/rapidsvn/TRANSLATIONS");
#else
        wxpath = Utf8ToLocal("/home/sleepy/projects/rapidsvn/TRANSLATIONS");
#endif
        static wxString msg;
        msg.Printf(wxT("%s: %s, %d"), ACTION_NAMES[i], wxpath.c_str(), j);

        m->Trace(msg);
      }

      static apr_time_t last_access = apr_time_now();
      if (apr_time_now() - last_access > 2000000)
      {
        last_access = apr_time_now();
        wxSafeYield();
      }
    }
  }
  // ending the test
  apr_time_t end = apr_time_now();

  wxString name(wxT("Tested Listener, 1000 loops through all the 22/26 actions"));
  PrintTimeMeasurements(start, end, name);
}

void
MainFrame::OnTestCheckout(wxCommandEvent & WXUNUSED(event))
{
  apr_time_t start = apr_time_now();

  Action * action = new CheckoutAction(this);

  if (action)
    Perform(action);

  apr_time_t end = apr_time_now();
  wxString name(wxT("Tested Checkout (works only in single-threaded version)"));
  PrintTimeMeasurements(start, end, name);
}

void
MainFrame::OnTestCertDlg(wxCommandEvent & WXUNUSED(event))
{
  svn::ContextListener::SslServerTrustData data;
  data.failures = (apr_uint32_t)-1;
  data.hostname = "hostname";
  data.fingerprint = "this is the fingerprint";
  data.validFrom = "Apr 25th 2007";
  data.validUntil = "Apr 31th 2015";
  data.issuerDName = "RapidSVN SSL Certificate Authority";
  data.realm = "RapidSVN Tests";

  CertDlg dlg(this, data);

  int modalResult = dlg.ShowModal();
  wxString modalDescr;
  switch (modalResult)
  {
  case wxID_OK:
    modalDescr=wxT("wxID_OK");
    break;
  case wxID_CANCEL:
    modalDescr=wxT("wxID_CANCEL");
    break;
  default:
    modalDescr=wxString::Format(wxT("%08x"), modalResult);
  }

  wxString answer;
  switch (dlg.Answer())
  {
  case svn::ContextListener::DONT_ACCEPT:
    answer = wxT("DONT_ACCEPT");
    break;
  case svn::ContextListener::ACCEPT_TEMPORARILY:
    answer = wxT("ACCEPT_TEMPORARILY");
    break;
  case svn::ContextListener::ACCEPT_PERMANENTLY:
    answer = wxT("ACCEPT_PERMANENTLY");
    break;
  default:
    answer = wxT("Invalid answer");
  }


  wxString msg = wxString::Format(
                   wxT("Modal result:%s\nAccepted Failures: %08x\nAnswer: %s"),
                   modalDescr.c_str(), dlg.AcceptedFailures(), answer.c_str());
  ::wxMessageBox(msg, wxT("Certificate Dlg Results"), wxOK);
}

void
MainFrame::OnTestDestinationDlg(wxCommandEvent & event)
{
  int flags = 0;

  switch (event.GetId())
  {
  case ID_TestDestinationDlg:
    flags = 0;
    break;
  case ID_TestDestinationDlgWithForce:
    flags = DestinationDlg::WITH_FORCE;
    break;
  }

  DestinationDlg dlg(this, wxT("DestinationDlg Title"),
                     wxT("Here comes the description"),
                     flags, wxT("/home/foo"));

  int result = dlg.ShowModal();
  wxString modalDescr;
  switch (result)
  {
  case wxID_OK:
    modalDescr = wxT("wxID_OK");
    break;
  case wxID_CANCEL:
    modalDescr = wxT("wxID_CANCEL");
    break;
  default:
    modalDescr = wxString::Format(wxT("%08x"));
  }
  int force = dlg.GetForce();

  wxString msg = wxString::Format(
                   wxT("Modal result:%s\nDestination: \"%s\"\nForce: %s"),
                   modalDescr.c_str(), dlg.GetDestination().c_str(),
                   force ? wxT("true") : wxT("false"));
  ::wxMessageBox(msg, wxT("DestinationDlg Results"), wxOK);
}


void
MainFrame::OnTestDndDlg(wxCommandEvent &)
{
  int action = DragAndDropDlg::IMPORT;
  if (m->IsMenuChecked(ID_TestDndCopy))
    action = DragAndDropDlg::COPY;
  else if (m->IsMenuChecked(ID_TestDndCopyMove))
    action = DragAndDropDlg::COPY_MOVE;

  DragAndDropDlg dlg(this, wxT("/home/foo/src"),
                     wxT("/home/foo/dst"),
                     action);

  int result = dlg.ShowModal();
  wxString resultStr;
  switch (result)
  {
  case wxID_CANCEL:
    resultStr = wxT("wxID_CANCEL");
    break;
  case DragAndDropDlg::RESULT_MOVE:
    resultStr = wxT("RESULT_MOVE");
    break;
  case DragAndDropDlg::RESULT_COPY:
    resultStr = wxT("RESULT_COPY");
    break;
  case DragAndDropDlg::RESULT_IMPORT:
    resultStr = wxT("RESULT_IMPORT");
    break;
  default:
    resultStr = wxString::Format(wxT("Unknown %08x"), result);
  }

  wxString msg = wxString::Format(
                   wxT("Modal result:%s"), resultStr.c_str());
  ::wxMessageBox(msg, wxT("DndDlg Results"), wxOK);
}
#endif

void
MainFrame::OnLogClear(wxCommandEvent & event)
{
  m_log->DeleteAllItems();
}

static LogItemType
GetLogItemType(int id)
{
  switch (id) {
  case ID_Log_Added :
    return LogItem_Added;
  case ID_Log_Conflicted:
    return LogItem_Conflicted;
  case ID_Log_Deleted :
    return LogItem_Deleted;
  case ID_Log_Updated:
    return LogItem_Updated;
  default:
    return LogItem_Normal;
  }
}

void
MainFrame::OnLogToggle(wxCommandEvent & event)
{
  int id = event.GetId();
  LogItemType t = GetLogItemType(id);
  m_log->SetItemFilter(t, !m_log->GetItemFilter(t));
}

void
MainFrame::OnLogUpdate(wxUpdateUIEvent & event)
{
  int id = event.GetId();
  LogItemType t = GetLogItemType(id);
  bool checked = m_log->GetItemFilter(t);
  event.Check(checked);
}

void
MainFrame::OnFileCommand(wxCommandEvent & event)
{
  int id = event.GetId();

  switch (id)
  {
  case ID_Default_Action:
    InvokeDefaultAction();
    break;

  case ID_Mkdir:
    Perform(new MkdirAction(this, m->currentPath));

    break;

  default:
    // Let the action factory decide
    Action * action = ActionFactory::CreateAction(this, id);

    if (action)
      Perform(action);
    else
      m->logTracer->Trace(_("Unimplemented action!"));
  }

}

void
MainFrame::OnActionEvent(wxCommandEvent & event)
{
  const int token = event.GetInt();
  const long liType = event.GetExtraLong();
  wxString action, message;
  int splitPos = event.GetString().Find("|||");
  if (splitPos != wxNOT_FOUND) {
    action = event.GetString().Left(splitPos);
    message = event.GetString().Mid(splitPos+3);
  }
  else {
    message = event.GetString();
  }

  switch (token)
  {
  case TOKEN_INFO:
    m->Trace((LogItemType)liType, action, message);
    break;

  case TOKEN_ERROR:
    m->TraceError(message);
    break;

  case TOKEN_SVN_INTERNAL_ERROR:
  case TOKEN_INTERNAL_ERROR:
    m->TraceError(message);
    // Action is interrupted, so cancel listener
    // (in order to enable filelist update)
    // and disable "Running" state
    m->listener.cancel(false);
    RefreshFileList();
    m->Trace(LogItem_Normal, wxEmptyString, _("Ready\n"));
    m->SetRunning(false);
    break;

  case TOKEN_ACTION_START:
    m->Trace(LogItem_Normal, action, message);
    wxLogStatus(message);
    break;

  case TOKEN_ACTION_END:
  {
    unsigned int* af = (unsigned int*) event.GetClientData();
    unsigned int actionFlags = 0;
    if (af)
    {
      actionFlags = *af;
      delete af;
    }
    else
    {
      m->TraceError(_("Internal Error: no client data for action event!"));
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
        m->Trace(LogItem_Normal, wxEmptyString, _("Updating file list..."));
        RefreshFolderBrowser();
      }
      else if ((actionFlags & Action::DONT_UPDATE) == 0)
      {
        m->Trace(LogItem_Normal, wxEmptyString, _("Updating file list..."));
        RefreshFileList();
      }
    }

    m->Trace(LogItem_Normal, wxEmptyString, _("Ready\n"));
    m->SetRunning(false);
  }
  break;

  case TOKEN_CMD:
  case TOKEN_CMD_DIFF:
  case TOKEN_CMD_MERGE:
  case TOKEN_CMD_VIEW:
  {
#ifdef _WIN32
    // recover initial environment
    wxString apr_iconv_path;
    bool saved_apr_iconv_path = false;
    if (wxGetEnv(wxT("__SAVED_APR_ICONV_PATH"), &apr_iconv_path))
    {
      saved_apr_iconv_path = true;
      wxSetEnv(wxT("APR_ICONV_PATH"), apr_iconv_path.c_str());
      wxUnsetEnv(wxT("__SAVED_APR_ICONV_PATH"));
    }
#endif
    // execute the command sent
    wxExecute(event.GetString());
#ifdef _WIN32
    // set custom environment again
    if (saved_apr_iconv_path)
    {
      wxSetEnv(wxT("__SAVED_APR_ICONV_PATH"), apr_iconv_path.c_str());
      wxSetEnv(wxT("APR_ICONV_PATH"), wxStandardPaths::Get().GetDataDir() + wxT("\\iconv"));
    }
#endif
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
    GetData * pData = static_cast<GetData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy the data first. This makes sure
      // the memory is released in the occurence
      // of an exception
      GetData data(*pData);
      delete pData;

      Action * action = new ViewAction(this, data);
      Perform(action);
    }
  }
  break;

  case TOKEN_ADD_BOOKMARK:
  {
    const wxString bookmark = event.GetString();

    m_folderBrowser->AddBookmark(bookmark);
    RefreshFolderBrowser();
    m_folderBrowser->SelectBookmark(bookmark);
    UpdateCurrentPath();
    RefreshFileList();
  }
  break;

  case TOKEN_GET:
  {
    GetData * pData = static_cast<GetData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy the data first. This makes sure
      // the memory is released in the occurence
      // of an exception
      GetData data(*pData);
      delete pData;
      Action * action;

      action = new GetAction(this, data);
      Perform(action);
    }
  }
  break;

  case TOKEN_DIFF:
  {
    DiffData * pData = static_cast<DiffData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy and delete data
      DiffData data(*pData);
      delete pData;
      Action * action;

      action = new DiffAction(this, data);
      Perform(action);
    }
  }
  break;

  case TOKEN_MERGE:
  {
    MergeData * pData = static_cast<MergeData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy and delete data
      MergeData data(*pData);
      delete pData;
      Action * action;

      action = new MergeAction(this, data);
      Perform(action);
    }
  }
  break;

  case TOKEN_DELETE_ACTION:
  {
    Action * action = static_cast<Action *>(event.GetClientData());

    if (action != 0)
      delete action;
  }
  break;

  case TOKEN_UPDATE_ASCENDING:
    UpdateMenuAscending();
    break;

  case TOKEN_UPDATE_SORTING:
    UpdateMenuSorting();
    break;

  case TOKEN_ANNOTATE:
  {
    AnnotateData * pData = static_cast<AnnotateData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy and delete data
      AnnotateData data(*pData);
      delete pData;
      Action * action;

      action = new AnnotateAction(this, data);
      Perform(action);
    }
  }
  break;
  case TOKEN_LOG:
  {
    LogData * pData = static_cast<LogData *>(event.GetClientData());

    if (pData != 0)
    {
      LogDlg dlg(this, pData->target, const_cast<svn::LogEntries*>(pData->logEntries));
      dlg.ShowModal();

      delete pData->logEntries;
      delete pData;
    }
  }
  break;
  case TOKEN_LOG_NEXT:
  {
    LogNextData * pData = static_cast<LogNextData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy and delete data
      LogNextData data(*pData);
      delete pData;
      Action * action;

      action = new LogNextAction(this, data);
      Perform(action);
    }
  }
  break;
  case TOKEN_LOG_NEXT_CALLBACK:
  {
    LogNextData * pData = static_cast<LogNextData *>(event.GetClientData());

    if (pData != 0)
    {
      // copy and delete data
      LogNextData data(*pData);
      delete pData;

      if (data.logdlg != NULL && data.logEntries != NULL)
      {
        data.logdlg->NextLogEntriesCallback(data.logEntries);
      }
    }
  }
  break;
  case TOKEN_DRAG_N_DROP:
  {
    DragAndDropData * pData = static_cast<DragAndDropData *>(event.GetClientData());
    if (pData != 0)
    {
      // copy and delete data
      DragAndDropData data(*pData);
      delete pData;
      Action * action;

      action = new DragAndDropAction(this, data);
      Perform(action);
    }
  }
  break;
  }
}

void
MainFrame::OnToolEnter(wxCommandEvent & WXUNUSED(event))
{
}

void
MainFrame::OnFolderBrowserSelChanged(wxTreeEvent & WXUNUSED(event))
{
  if (m->IsRunning())
    return;

  try
  {
    m->activePane = ACTIVEPANE_FOLDER_BROWSER;

    // Update the menu and list control
    bool flatMode = m_folderBrowser->IsFlat();
    m_listCtrl->SetFlat(flatMode);
    m->CheckMenu(ID_Flat, flatMode);
    m->CheckTool(ID_Flat, flatMode);
    bool indicateModifiedChildren = m_folderBrowser->GetIndicateModifiedChildren();
    m->CheckMenu(ID_IndicateModifiedChildren, indicateModifiedChildren);

    SetIncludePathVisibility(flatMode);

    // Disable menu entry if no path is selected (e.g. root)
    const wxString & path = m_folderBrowser->GetPath();
    m->MenuBar->Enable(ID_Flat, !path.IsEmpty());

    UpdateCurrentPath();
    RefreshFileList();
  }
  catch (...)
  {
    m->Trace(LogItem_Error, wxEmptyString, _("Exception occured during filelist update"));
  }
}

void
MainFrame::OnFolderBrowserKeyDown(wxTreeEvent & event)
{
  switch (event.GetKeyEvent().GetKeyCode())
  {
  case WXK_RETURN:
    ProcessCommand(ID_Default_Action);
    break;

  default:
    event.Skip();
    break;
  }
}

void
MainFrame::OnFileListSelected(wxListEvent & WXUNUSED(event))
{
  m->activePane = ACTIVEPANE_FILELIST;
}


void MainFrame::OnLogListBrowse(wxCommandEvent &event)
{
  wxString selFile = m_log->GetSelectedFileOrDir();
  if (selFile.IsEmpty())
    return;

  // find matching bookmark
  wxString containingBkmk = m_folderBrowser->FindContainingBookmark(selFile);
  if (containingBkmk.IsEmpty())
    return;

  // select that bookmark
  if (!m_folderBrowser->SelectBookmark(containingBkmk))
    return;
  wxString selDir, selFileNameWithExt;
  if (wxFileName::DirExists(selFile)) {
    // it is a directory. the file name is empty
    selDir = selFile;
  }
  else {
    wxString selFileName, selFileExt;
    wxFileName::SplitPath(selFile, &selDir, &selFileName, &selFileExt);
    selFileNameWithExt = selFileName;
    if (!selFileExt.IsEmpty())
      selFileNameWithExt << '.' << selFileExt;
  }
  // select the folder within the bookmark
  if (!m_folderBrowser->SelectFolder(selDir))
    return;

  // Select the file, if it's a file
  if (!selFileNameWithExt.IsEmpty()) {
    // Update our file list
    UpdateCurrentPath();
    RefreshFileList();
    // Find the file and focus and select it
    long i = m_listCtrl->FindItem(-1, selFileNameWithExt);
    if (i >= 0) {
      m_listCtrl->Focus(i);
      m_listCtrl->Select(i);
    }
  }
}

void MainFrame::OnLogListDiff(wxCommandEvent& event)
{
  // Check if exactly one updated or conflicted file is selected.
  wxString selFile = m_log->GetSelectedFileOrDir();
  if (selFile.IsEmpty())
    return;
  LogItemType type = m_log->GetSelectedItemType();
  if (type != LogItem_Updated && type != LogItem_Conflicted)
    return;

  // OK have a valid file to diff.
  // Run the diff (vs. previous revision).
  DiffData data(svn::Revision::PREVIOUS);
  data.compareType = DiffData::WITH_PREVIOUS;
  data.path = selFile;
  Action* action = new DiffAction(this, data);
  Perform(action);
}


void
MainFrame::OnListenerEvent(wxCommandEvent & event)
{
  m->listener.handleEvent(event);
}

/*** END OF SECTION EVENTS ***/


void
MainFrame::AddWcBookmark()
{
  const wxArrayString & lastDirs=TheHistoryManager.ReadList(HISTORY_EXISTING_WORKING_DIRECTORY);

  wxString dir;
  if (lastDirs.IsEmpty())
    dir = wxGetHomeDir();
  else
    dir = lastDirs[0];

  wxDirDialog dialog(this, _("Select a directory"), dir);
  bool add = TRUE;

  // select dir dialog
  if (dialog.ShowModal() != wxID_OK)
  {
    return;
  }

  // admin dir?
  // TODO: Should we have a translated-once constant
  // rather than keep converting SVN_WC_ADM_DIR_NAME?
  wxFileName fileName(dialog.GetPath());
  if ((fileName.GetName() + fileName.GetExt()) ==
      Utf8ToLocal(SVN_WC_ADM_DIR_NAME))
  {
    add = FALSE;
    wxMessageBox(
      _("You cannot add a subversion administrative \
directory to the bookmarks!"),
      _("Error"), wxOK);
    return;
  }

  // add
  m_folderBrowser->AddBookmark(dialog.GetPath());

  m->skipFilelistUpdate = true;
  RefreshFolderBrowser();

  wxLogStatus(_("Added working copy to bookmarks '%s'"),
              dialog.GetPath().c_str());

  m_folderBrowser->SelectBookmark(dialog.GetPath());

  TheHistoryManager.AddEntryToList(HISTORY_EXISTING_WORKING_DIRECTORY, dialog.GetPath());
}

void
MainFrame::AddRepoBookmark()
{
  const int flags =
    UpdateDlg::WITH_URL |
    UpdateDlg::WITHOUT_DEPTH |
    UpdateDlg::WITHOUT_REVISION |
    UpdateDlg::WITHOUT_IGNORE_EXTERNALS;
  UpdateDlg dialog(this, _("Repository URL"), flags);

  if (dialog.ShowModal() != wxID_OK)
  {
    return;
  }

  // add
  wxString url = dialog.GetData().url;
  m_folderBrowser->AddBookmark(url);

  m->skipFilelistUpdate = true;
  RefreshFolderBrowser();

  wxLogStatus(_("Added repository to bookmarks '%s'"),
              url.c_str());

  m_folderBrowser->SelectBookmark(url);
  //UpdateCurrentPath ();
  //RefreshFileList ();
}

inline void
MainFrame::RemoveBookmark()
{
  wxASSERT(m_folderBrowser);
  if (m_folderBrowser->RemoveBookmark())
  {
    wxLogStatus(_("Removed bookmark"));
  }
}

void
MainFrame::EditBookmark()
{
  wxASSERT(m_folderBrowser);

  const FolderItemData * bookmark = m_folderBrowser->GetSelectedItemData();

  // if nothing is selected, or the wrong type,
  // just exit (this should be handled by the routine
  // that is responsible for greying out menu entries,
  // but hey: nobody is perfect
  if (!m_folderBrowser)
    return;

  if (bookmark->getFolderType() != FOLDER_TYPE_BOOKMARK)
    return;

  // now edit the data
  wxString oldPath(bookmark->getPath());
  DestinationDlg dlg(this, _("Edit Bookmark"), _("Bookmark"),
                     0, oldPath);

  if (dlg.ShowModal() == wxID_OK)
  {
    // and if everything goes right, remove the old
    // selection and add the new one
    wxString newPath(dlg.GetDestination());

    // if nothing has changed, just exit
    if (oldPath == newPath)
      return;

    m_folderBrowser->RemoveBookmark();
    m_folderBrowser->AddBookmark(newPath);
    RefreshFolderBrowser();
    m_folderBrowser->SelectBookmark(newPath);
  }
}


void
MainFrame::ShowInfo()
{
  bool withUpdate = false;
  if (m_listCtrl)
    withUpdate = m_listCtrl->GetWithUpdate();

  FileInfo fileInfo(m_context, withUpdate);

  try
  {
    svn::PathVector vector = m->GetStatusSel().targets();
    svn::PathVector::const_iterator it;

    for (it = vector.begin(); it != vector.end(); it++)
    {
      svn::Path path = *it;
      fileInfo.addPath(path.c_str());
    }

    wxString info = fileInfo.info();

    ReportDlg dlg(this, _("Info"), info, NORMAL_REPORT);
    dlg.ShowModal();
  }
  catch (svn::ClientException & e)
  {
    m->Trace(LogItem_Error, wxEmptyString, Utf8ToLocal(e.message()));
    return;
  }

}


void
MainFrame::ShowPreferences()
{
  Preferences prefs;
  PreferencesDlg dlg(this, & prefs);
  bool ok = dlg.ShowModal() == wxID_OK;

  if (ok)
  {
    m_folderBrowser->SetAuthCache(prefs.useAuthCache);
    m_folderBrowser->SetAuthPerBookmark(prefs.authPerBookmark);
  }
}

void
MainFrame::UpdateCurrentPath()
{
  if (m_folderBrowser == 0)
  {
    m->currentPath.Clear();
    m_context = 0;
  }
  else
  {
    m->currentPath = m_folderBrowser->GetPath();
    m_context = m_folderBrowser->GetContext();
  }

  if (m->currentPath.Length() > 0)
    SetTitle(m_title + wxT(" - ") + m->currentPath);
  else
    SetTitle(m_title);
}

bool
MainFrame::InvokeDefaultAction()
{
  const svn::StatusSel & statusSel = m->GetStatusSel();

  // the default action will be invoked only for a single file
  // or folder.
  // if more or less than one file or folder is selected nothing
  // will happen.

  if (1 != statusSel.size())
    return false;

  if (statusSel.hasDirs())
  {
    // go one folder deeper...
    m_folderBrowser->SelectFolder(
      FullNativePath(statusSel.target(), m->currentPath, m_listCtrl->IsFlat())
    );
  }
  else
  {
    Perform(new ExternalProgramAction(this, -1, false));
  }

  return true;
}

void
MainFrame::Perform(Action * action)
{
  try
  {
    m->listener.cancel(false);

    svn::Path currentPathUtf8(PathUtf8(m->currentPath));
    action->SetPath(currentPathUtf8);
    action->SetContext(m_context);
    action->SetStatusSel(m->GetStatusSel());
    action->SetTracer(m->logTracer, false);
    m_actionWorker->SetTracer(m->logTracer);
    m_actionWorker->SetContext(m_context, false);
    if (m_actionWorker->Perform(action))
      m->SetRunning(true);
  }
  catch (...)
  {
    throw; // svn::Excepton (e);
  }
}

inline void
MainFrame::UpdateMenuSorting()
{
  m->CheckSort(m_listCtrl->GetSortColumn() + ID_ColumnSort_Min + 1);
}

inline void
MainFrame::UpdateMenuIncludePath()
{
  m->CheckMenu(ID_Include_Path, m_listCtrl->GetIncludePath());
}

inline void
MainFrame::UpdateMenuAscending()
{
  m->CheckMenu(ID_Sort_Ascending, m_listCtrl->GetSortAscending());
}

void
MainFrame::SetIncludePathVisibility(bool flatMode)
{
  if (flatMode == false)
  {
    if (m_listCtrl->GetIncludePath() == true)
      m->CheckMenu(ID_Include_Path, false);

    UpdateMenuSorting();
    UpdateMenuAscending();
  }
  else
  {
    UpdateMenuIncludePath();
  }
  m->EnableMenuEntry(ID_Include_Path, flatMode);
}

void
MainFrame::OnSize(wxSizeEvent & sizeEvent)
{
  sizeEvent.Skip();
}


void
MainFrame::OnFolderBrowserSetFocus(wxFocusEvent& WXUNUSED(event))
{
  SetActivePane(ACTIVEPANE_FOLDER_BROWSER);
}


void
MainFrame::OnListCtrlSetFocus(wxFocusEvent& WXUNUSED(event))
{
  SetActivePane(ACTIVEPANE_FILELIST);
}

/**
 * In @ref MainFrameBase there is an event handler that uses
 * the idle event as well to set the splitter sash position.
 * So we have to wait just one call longer until we can set the
 * desired sash position
 */
void
MainFrame::OnIdle(wxIdleEvent & WXUNUSED(event))
{
  m->idleCount++;

  if (m->idleCount >= 2)
  {
    m_splitterVert->SetSashPosition(m->vertSashPos);
    m_splitterHoriz->SetSashPosition(m->horizSashPos);

    // we dont need this event anymore
    Disconnect(wxEVT_IDLE, wxIdleEventHandler(MainFrame::OnIdle), NULL, this);
  }
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
