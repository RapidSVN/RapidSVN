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
#include "mainwindow.h"
#include "ui_mainwindow.h"

// svncpp
#include <svncpp/apr.hpp>
#include <svncpp/status_selection.hpp>
#include <svncpp/url.hpp>

// app
#include "utils.hpp"
#ifdef PORTED_TO_QT
#include "rapidsvn_app.hpp"
#include "columns.hpp"
#endif
#include "config.h"
#include "listener.hpp"
#include "ids.hpp"
#ifdef PORTED_TO_QT
#include "destination_dlg.hpp"
#include "diff_data.hpp"
#include "get_data.hpp"
#include "file_info.hpp"
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
#endif

// actions
#ifdef USE_SIMPLE_WORKER
#include "simple_worker.hpp"
#else
#include "threaded_worker.hpp"
#endif

#ifdef PORTED_TO_QT
#include "view_action.hpp"
#include "get_action.hpp"
#include "diff_action.hpp"
#include "merge_action.hpp"
#include "annotate_action.hpp"
#include "drag_n_drop_action.hpp"
#include "external_program_action.hpp"
#include "mkdir_action.hpp"

#endif


// dialogs
#ifdef PORTED_TO_QT
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
#endif

#ifdef PORTED_TO_QT

#ifdef USE_DEBUG_TESTS
#include "checkout_action.hpp"
#include "cert_dlg.hpp"
#include "destination_dlg.hpp"
#include "dnd_dlg.hpp"
#endif

#endif

// number of items initially in the list
static const int NUM_ITEMS = 30;

// List config keys here, to avoid duplicating literal text:
const static char ConfigLeft[] = "/MainWindow/Left";
const static char ConfigTop[] = "/MainWindow/Top";
const static char ConfigWidth[] = "/MainWindow/Width";
const static char ConfigHeight[] = "/MainWindow/Height";
const static char ConfigMaximized[] = "/MainWindow/Maximized";
const static char ConfigSplitterHoriz[] = "/MainWindow/SplitterHoriz";
const static char ConfigSplitterVert[] = "/MainWindow/SplitterVert";

const static char TraceMisc[] = "tracemisc";

/**
 * Local helper function to create the action worker
 *
 * @param parent
 * @return new action worker instance
 */
ActionWorker *
CreateActionWorker(QWidget * parent)
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
struct MainWindow::Data
{
public:
#ifdef PORTED_TO_QT
  EventTracer * logTracer;
#endif

  Listener listener;
#ifdef PORTED_TO_QT
  Columns ColumnList;
#endif

  QAction * actionUsePathForSorting;
  QAction * actionSortAscending;

  bool updateAfterActivate;
  bool dontUpdateFilelist;
  bool skipFilelistUpdate;
  const QLocale & locale;
  QString currentPath;
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

  QWidget * m_parent;
  bool m_isErrorDialogActive;
#ifdef PORTED_TO_QT
  FolderBrowser * m_folderBrowser;
  FileListCtrl * m_listCtrl;
  wxTextCtrl * m_log;
#endif

public:

  /**
   * This instance of @a apr is used to initialize/terminate apr
   */
  svn::Apr apr;

  Data(QWidget * parent, /*, FolderBrowser * folderBrowser,
       FileListCtrl * listCtrl, wxTextCtrl * log, */
       const QLocale & locale_)
    : listener(parent),
      updateAfterActivate(false), dontUpdateFilelist(false),
      skipFilelistUpdate(false), locale(locale_),
      currentPath(""),
      activePane(ACTIVEPANE_FOLDER_BROWSER),
      showUnversioned(false), showUnmodified(false),
      showModified(false), showConflicted(false), idleCount(0),
      m_running(false), m_parent(parent),
      m_isErrorDialogActive(false)
      // , m_folderBrowser(folderBrowser),
      // m_listCtrl(listCtrl), m_log(log)
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
    wxApp::s_macHelpMenuTitleName = tr("&Help");
#endif

    // Quit menu item is not necessary on MacOSX:
#ifdef PORTED_TO_QT
#ifndef __WXMAC__
    //menuFile->addSeparator ();
    AppendMenuItem(*menuFile, ID_Quit);
#endif
#endif

#ifdef PORTED_TO_QT
    for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
    {
      int id = ColumnList[col].id;
      int sortid = id + Columns::SORT_COLUMN_OFFSET;

      if (id != ID_Column_Name && id != ID_Column_Path)
        MenuColumns->AppendCheckItem(id, ColumnList[col].caption);
      MenuSorting->AppendRadioItem(sortid, ColumnList[col].caption);
    }
#endif

#ifdef PORTED_TO_QT
    // Preferences menu item goes to its own place on MacOSX,
    // so no separator is necessary.

#ifndef __WXMAC__
    menuView->addSeparator();
#endif
#endif

#ifdef USE_DEBUG_TESTS
    // Debug Menu
    QMenu *menuTests = new QMenu;

    menuTests->Append(ID_TestNewQString, "QString Creation&Tracing Test");
    menuTests->Append(ID_TestListener, "Listener Test");
    menuTests->Append(ID_TestCheckout, "Checkout Test");
    menuTests->Append(ID_TestCertDlg, "Certificate Dlg");
    menuTests->Append(ID_TestDestinationDlg, "Destination Dlg (Simple"));
    menuTests->Append(ID_TestDestinationDlgWithForce, "Destination Dlg (With Force"));

    QMenu * menuDnd = new QMenu;
    menuDnd->AppendRadioItem(ID_TestDndImport, "with action \"Import\"");
    menuDnd->AppendRadioItem(ID_TestDndCopy, "with action \"Copy\"");
    menuDnd->AppendRadioItem(ID_TestDndCopyMove, "with action \"Copy/mive\"");
    menuDnd->Append(ID_TestDndDlg, "Show Dialog");
    menuTests->AppendSubMenu(menuDnd, "Dnd Dlg");
#endif

#ifdef USE_DEBUG_TESTS
    MenuBar->Append(menuTests, tr("&Tests"));
#endif
  }

  bool
  IsColumnChecked(int id)
  {
#ifdef PORTED_TO_QT
    return MenuColumns->IsChecked(id);
#else
    Q_UNUSED(id);
    return false;
#endif
  }

  void
  CheckColumn(int id, bool check)
  {
#ifdef PORTED_TO_QT
    MenuColumns->Check(id, check);
#else
    Q_UNUSED(id);
    Q_UNUSED(check);
#endif
  }

  void
  CheckSort(int id)
  {
#ifdef PORTED_TO_QT
    MenuSorting->Check(id, true);
#else
    Q_UNUSED(id);
#endif
  }

  void
  EnableMenuEntry(int id, bool enable)
  {
#ifdef PORTED_TO_QT
    MenuSorting->Enable(id, enable);
#else
    Q_UNUSED(id);
    Q_UNUSED(enable);
#endif
  }

  bool
  IsMenuChecked(int id)
  {
#ifdef PORTED_TO_QT
    return MenuBar->IsChecked(id);
#else
    Q_UNUSED(id);

    return false;
#endif
  }

  void
  CheckMenu(int id, bool check)
  {
#ifdef PORTED_TO_QT
    MenuBar->Check(id, check);
#else
    Q_UNUSED(id);
    Q_UNUSED(check);
#endif
  }

  void
  CheckTool(int id, bool check)
  {
#ifdef PORTED_TO_QT
    wxToolBar * toolbar = m_parent->GetToolBar();

    if (0 != toolbar)
      toolbar->ToggleTool(id, check);
#else
    Q_UNUSED(id);
    Q_UNUSED(check);
#endif
  }

  void
  SetMenuAndTool(int id, bool & toggleValue, bool newValue)
  {
#ifdef PORTED_TO_QT
    toggleValue = newValue;

    // first update the menu entry
    MenuBar->Check(id, toggleValue);

    // next update the tool
    wxToolBarBase * toolBar = m_parent->GetToolBar();
    if (toolBar != 0)
      toolBar->ToggleTool(id, toggleValue);
#else
    Q_UNUSED(id);
    Q_UNUSED(toggleValue);
    Q_UNUSED(newValue);
#endif
  }

  bool
  ToggleMenuAndTool(int id, bool & toggleValue)
  {
#ifdef PORTED_TO_QT
    SetMenuAndTool(id, toggleValue, !toggleValue);

    return toggleValue;
#else
    Q_UNUSED(id);
    Q_UNUSED(toggleValue);
    return false;
#endif
  }

  void
  SetRunning(bool running)
  {
    if (running && ! m_running)
      listener.cancel(false);

    m_running = running;

#ifdef PORTED_TO_QT
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
#endif
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
#ifdef PORTED_TO_QT
    if (!m_listCtrl)
      return 0;

    return m_listCtrl->IsFlat();
#else
    return false;
#endif
  }


  /**
   * Checks whether there is an error dialog showing
   * at the moment (not an action dialog).
   *
   * This information is used to decide whether to refresh
   * in @ref MainWindow::OnActivate
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
  ShowErrorDialog(const QString & msg)
  {
    m_isErrorDialogActive = true;
#ifdef PORTED_TO_QT
    wxMessageBox(msg, tr("RapidSVN Error"), wxICON_ERROR | wxOK);
#else
    Q_UNUSED(msg);
#endif
    m_isErrorDialogActive = false;
  }


  /**
   * add message to log window
   *
   * @param msg message to show
   */
  void
  Trace(const QString & msg)
  {
#ifdef PORTED_TO_QT
    if (!m_log)
      return;

    m_log->AppendText(msg + '\n');
#else
    Q_UNUSED(msg);
#endif
  }

  /**
   * add error message to log window marked red
   *
   * @param msg error message to show
   * @param showDialog show an error dialog with the message
   *                   (besides logging this)
   */
  void
  TraceError(const QString & msg, bool showDialog=true)
  {
#ifdef PORTED_TO_QT
    if (!m_log)
      return;

    m_log->SetDefaultStyle(wxTextAttr(*wxRED));
    m_log->AppendText(QString::Format(tr("Error: %s\n"), msg.c_str()));
    m_log->SetDefaultStyle(wxTextAttr(*wxBLACK));

    if (showDialog)
      ShowErrorDialog(msg);
#else
    Q_UNUSED(msg);
    Q_UNUSED(showDialog);
#endif
  }

  const svn::StatusSel &
  GetStatusSel()  const
  {
#ifdef PORTED_TO_QT
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
#else
    static svn::StatusSel sel;
    return sel;
#endif
  }
};

#ifdef PORTED_TO_QT
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
  EVT_MENU(ID_AddWcBookmark, MainWindow::OnAddWcBookmark)
  EVT_MENU(ID_AddRepoBookmark, MainWindow::OnAddRepoBookmark)
  EVT_MENU(ID_RemoveBookmark, MainWindow::OnRemoveBookmark)
  EVT_MENU(ID_EditBookmark, MainWindow::OnEditBookmark)
  EVT_MENU(ID_Quit, MainWindow::OnQuit)

  EVT_MENU(ID_Preferences, MainWindow::OnPreferences)
  EVT_MENU(ID_Refresh, MainWindow::OnRefresh)
  EVT_UPDATE_UI(ID_Refresh, MainWindow::OnUpdateCommand)

  EVT_MENU(ID_Column_Reset, MainWindow::OnColumnReset)
  EVT_MENU_RANGE(ID_Column_Min, ID_Column_Max, MainWindow::OnColumn)
  EVT_MENU(ID_Include_Path, MainWindow::OnIncludePath)
  EVT_MENU(ID_Sort_Ascending, MainWindow::OnSortAscending)
  EVT_MENU_RANGE(ID_ColumnSort_Min, ID_ColumnSort_Max, MainWindow::OnColumnSorting)

  EVT_MENU(ID_Flat, MainWindow::OnFlatView)
  EVT_MENU(ID_RefreshWithUpdate, MainWindow::OnRefreshWithUpdate)
  EVT_MENU(ID_ShowUnversioned, MainWindow::OnShowUnversioned)
  EVT_MENU(ID_IgnoreExternals, MainWindow::OnIgnoreExternals)
  EVT_MENU(ID_ShowIgnored, MainWindow::OnShowIgnored)
  EVT_MENU(ID_ShowUnmodified, MainWindow::OnShowUnmodified)
  EVT_MENU(ID_ShowModified, MainWindow::OnShowModified)
  EVT_MENU(ID_ShowConflicted, MainWindow::OnShowConflicted)
  EVT_MENU(ID_IndicateModifiedChildren, MainWindow::OnIndicateModifiedChildren)

  EVT_MENU(ID_Login, MainWindow::OnLogin)
  EVT_MENU(ID_Logout, MainWindow::OnLogout)
  EVT_MENU(ID_Stop, MainWindow::OnStop)

  EVT_MENU(ID_Info, MainWindow::OnInfo)
  EVT_UPDATE_UI(ID_Info, MainWindow::OnUpdateCommand)

  EVT_MENU(ID_HelpContents, MainWindow::OnHelpContents)
  EVT_MENU(ID_HelpIndex, MainWindow::OnHelpIndex)
  EVT_MENU(ID_HelpStartupTips, MainWindow::OnHelpStartupTips)
  EVT_MENU(ID_About, MainWindow::OnAbout)

  #ifdef USE_DEBUG_TESTS
  EVT_MENU(ID_TestNewQString, MainWindow::OnTestNewQString)
  EVT_MENU(ID_TestListener, MainWindow::OnTestListener)
  EVT_MENU(ID_TestCheckout, MainWindow::OnTestCheckout)
  EVT_MENU(ID_TestCertDlg, MainWindow::OnTestCertDlg)
  EVT_MENU(ID_TestDestinationDlg, MainWindow::OnTestDestinationDlg)
  EVT_MENU(ID_TestDestinationDlgWithForce, MainWindow::OnTestDestinationDlg)
  EVT_MENU(ID_TestDndDlg, MainWindow::OnTestDndDlg)
  #endif

  EVT_MENU_RANGE(ID_File_Min, ID_File_Max, MainWindow::OnFileCommand)
  EVT_MENU_RANGE(ID_Verb_Min, ID_Verb_Max, MainWindow::OnFileCommand)

  EVT_UPDATE_UI_RANGE(ID_File_Min, ID_File_Max, MainWindow::OnUpdateCommand)
  EVT_UPDATE_UI_RANGE(ID_Verb_Min, ID_Verb_Max, MainWindow::OnUpdateCommand)
  EVT_UPDATE_UI_RANGE(ID_Filter_Min, ID_Filter_Max, MainWindow::OnUpdateCommand)

  EVT_MENU(ACTION_EVENT, MainWindow::OnActionEvent)

  EVT_TOOL_ENTER(ID_TOOLBAR, MainWindow::OnToolEnter)
  EVT_TREE_SEL_CHANGED(-1, MainWindow::OnFolderBrowserSelChanged)
  EVT_TREE_KEY_DOWN(-1, MainWindow::OnFolderBrowserKeyDown)
  EVT_LIST_ITEM_SELECTED(-1, MainWindow::OnFileListSelected)

  EVT_MENU_RANGE(LISTENER_MIN, LISTENER_MAX, MainWindow::OnListenerEvent)
END_EVENT_TABLE()

#endif


/** class implementation **/
MainWindow(QWidget *parent, const QString & title)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_title(title), m_context(0)
{
  ui->setupUi(this);

  m = new Data(this, m_folderBrowser, m_listCtrl, m_log, locale);
  m_actionWorker = CreateActionWorker(this);

#ifdef PORTED_TO_QT
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
#endif

  SetMenuBar(m->MenuBar);

  // Create the toolbar
  CreateMainToolBar(this);
  m->SetRunning(false);

  // as much as the widget can stand
  m_log->SetMaxLength(0);

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
  Connect(wxEVT_IDLE, wxIdleEventHandler(MainWindow::OnIdle), NULL, this);
}

MainWindow::~MainWindow()
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
MainWindow::SetActivePane(ActivePane value)
{
  m->activePane = value;
}

void
MainWindow::TrimDisabledMenuItems(QMenu & menu)
{
  // Check for disabled items
  size_t pos = menu.GetMenuItemCount();
// TODO  unsigned int selectionActionFlags = GetSelectionActionFlags ();
  const svn::StatusSel & statusSel = m->GetStatusSel();
  while (pos-- > 0)
  {
    QMenuItem *pItem = menu.FindItemByPosition(pos);
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
    QMenuItem *pItem = menu.FindItemByPosition(pos);
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
MainWindow::RefreshFileList()
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
      QString msg, errtxt(Utf8ToLocal(e.message()));
      msg.Printf(tr("Error while updating filelist (%s)"),
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
      m->TraceError(tr("Error while updating filelist"), false);

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
MainWindow::RefreshFolderBrowser()
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
MainWindow::OnActivate(wxActivateEvent & event)
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
MainWindow::OnAddWcBookmark()
{
  AddWcBookmark();
}

void
MainWindow::OnAddRepoBookmark()
{
  AddRepoBookmark();
}

void
MainWindow::OnRemoveBookmark()
{
  RemoveBookmark();
}

void
MainWindow::OnEditBookmark()
{
  EditBookmark();
}

void
MainWindow::OnQuit()
{
  Close(TRUE);
}

void
MainWindow::OnPreferences()
{
  ShowPreferences();
}

void
MainWindow::OnRefresh()
{
  RefreshFolderBrowser();
}

void
MainWindow::OnColumnReset()
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
MainWindow::OnColumn()
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
MainWindow::OnIncludePath()
{
  m_listCtrl->SetIncludePath(!m_listCtrl->GetIncludePath());
}

void
MainWindow::OnSortAscending()
{
  m_listCtrl->SetSortAscending(!m_listCtrl->GetSortAscending());
}

void
MainWindow::OnColumnSorting()
{
  // we dont want to list FileListCtrl::COL_NAME/COL_PATH/... here
  int col = event.GetId() - ID_ColumnSort_Name;

  m_listCtrl->SetSortColumn(col);
  m_listCtrl->SetSortAscending(true);

  UpdateMenuAscending();
}


void
MainWindow::OnFlatView()
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
MainWindow::OnIndicateModifiedChildren()
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
MainWindow::OnRefreshWithUpdate()
{
  bool checked = m->IsMenuChecked(ID_RefreshWithUpdate);
  m_listCtrl->SetWithUpdate(checked);
  RefreshFolderBrowser();
}


void
MainWindow::OnShowUnversioned()
{
  bool checked = m->ToggleMenuAndTool(ID_ShowUnversioned, m->showUnversioned);
  m_listCtrl->SetShowUnversioned(checked);
  RefreshFileList();
}

void
MainWindow::OnShowUnmodified()
{
  bool checked = m->ToggleMenuAndTool(ID_ShowUnmodified, m->showUnmodified);
  m_listCtrl->SetShowUnmodified(checked);
  RefreshFileList();
}

void
MainWindow::OnShowModified()
{
  bool checked = m->ToggleMenuAndTool(ID_ShowModified, m->showModified);
  m_listCtrl->SetShowModified(checked);
  RefreshFileList();
}

void
MainWindow::OnShowConflicted()
{
  bool checked = m->ToggleMenuAndTool(ID_ShowConflicted, m->showConflicted);
  m_listCtrl->SetShowConflicted(checked);
  RefreshFileList();
}

void
MainWindow::OnIgnoreExternals()
{
  bool checked = m->IsMenuChecked(ID_IgnoreExternals);
  m_listCtrl->SetIgnoreExternals(checked);
  RefreshFileList();
}

void
MainWindow::OnShowIgnored()
{
  bool checked = m->IsMenuChecked(ID_ShowIgnored);
  m_listCtrl->SetShowIgnored(checked);
  RefreshFileList();
}

void
MainWindow::OnLogin()
{
  svn::Context * context = m_folderBrowser->GetContext();

  if (context == 0)
    return;

  QString LocalUsername(Utf8ToLocal(context->getUsername()));
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
MainWindow::OnLogout()
{
  svn::Context * context = m_folderBrowser->GetContext();

  if (context == 0)
    return;

  context->setLogin("", "");
}

void
MainWindow::OnStop()
{
  m->listener.cancel(true);
}

void
MainWindow::OnInfo()
{
  ShowInfo();
}

void
MainWindow::OnUpdateCommand(wxUpdateUIEvent & updateUIEvent)
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
    m->Trace(Utf8ToLocal(e.message()));
    return;
  }
  catch (...)
  {
    m->Trace(tr("An error occurred while checking valid actions"));
  }
}

void
MainWindow::OnHelpContents()
{
  try
  {
    OpenURL("http://www.rapidsvn.org/help/index.php?id=OnlineHelp:Contents");
  }
  catch (...)
  {
    m->Trace(tr("An error occured while launching the browser"));
  }

// WE DONT USE THIS CODE NOW
//#ifdef  USE_HTML_HELP
//  ::wxGetApp ().GetHelpController().DisplayContents();
//#endif
//#endif
}

void
MainWindow::OnHelpIndex()
{
  try
  {
    OpenURL("http://www.rapidsvn.org/help/index.php?id=OnlineHelp:Index");
  }
  catch (...)
  {
    m->Trace(tr("An error occured while launching the browser"));
  }

//#ifdef  USE_HTML_HELP
//  ::wxGetApp ().GetHelpController().DisplayIndex();
//#endif
}

void
MainWindow::OnHelpStartupTips()
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
MainWindow::OnAbout()
{
  AboutDlg dlg(this, m->locale);

  dlg.ShowModal();
}

#ifdef USE_DEBUG_TESTS
static void
info_print_time(apr_time_t atime, const QChar * desc, QString & str)
{
  apr_time_exp_t extime;
  apr_status_t apr_err;

  /* if this returns an error, just don't print anything out */
  apr_err = apr_time_exp_tz(&extime, atime, 0);
  if (!apr_err)
    str.Printf("%s: %02lu:%02lu:%02lu:%02lu", desc,
               (unsigned long)(extime.tm_hour),
               (unsigned long)(extime.tm_min),
               (unsigned long)(extime.tm_sec),
               (unsigned long)(extime.tm_usec));
}

void
MainWindow::PrintTimeMeasurements(apr_time_t start, apr_time_t end, const QString & name)
{
  QString msg(name);
  m->Trace('\n' + msg);

  info_print_time(start, tr("Test started at"), msg);
  m->Trace("\n" + msg);

  info_print_time(end, tr("Test ended at"), msg);
  m->Trace(msg);

  apr_time_t duration = end - start;
  info_print_time(duration, tr("Test duration"), msg);
  m->Trace(msg + "\n");
}

void
MainWindow::OnTestNewQString()
{
  apr_time_t start = apr_time_now();

  // starting the test
  for (int i = 1; i <= 10000; i++)
  {
    QString message;
    message.Printf("Tracing a message from newely created QString, round #%d", i);
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

  QString name("Created 10 000 QStrings and printed them with Trace ("));
  PrintTimeMeasurements(start, end, name);
}

void
MainWindow::OnTestListener()
{
  apr_time_t start = apr_time_now();

  // starting the test

  const int MAX_ACTION = svn_wc_notify_failed_unlock;

  for (int j = 1; j <= 1000; j++)
  {
    for (int i = 0; i < MAX_ACTION; i++)
    {
      static const QChar *
      ACTION_NAMES [] =
      {
        tr("Add"),              // svn_wc_notify_add,
        tr("Copy"),             // svn_wc_notify_copy,
        tr("Delete"),           // svn_wc_notify_delete,
        tr("Restore"),          // svn_wc_notify_restore,
        tr("Revert"),           // svn_wc_notify_revert,
        NULL ,                 // NOT USED HERE svn_wc_notify_failed_revert,
        tr("Resolved"),         // svn_wc_notify_resolved,
        tr("Skip"),             // NOT USED HERE svn_wc_notify_skip,
        tr("Deleted"),          // svn_wc_notify_update_delete,
        tr("Added"),            // svn_wc_notify_update_add,
        tr("Updated"),          // svn_wc_notify_update_update,
        NULL,                  // NOT USED HERE svn_wc_notify_update_completed,
        NULL,                  // NOT USED HERE svn_wc_notify_update_external,
        NULL,                  // NOT USED HERE svn_wc_notify_status_completed
        NULL,                  // NOT USED HERE svn_wc_notify_status_external
        tr("Modified"),         // svn_wc_notify_commit_modified,
        tr("Added"),            // svn_wc_notify_commit_added,
        tr("Deleted"),          // svn_wc_notify_commit_deleted,
        tr("Replaced"),         // svn_wc_notify_commit_replaced,
        NULL,                  // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
        NULL,                  // NOT USED HERE svn_wc_notify_blame_revision
        tr("Locked"),           // svn_wc_notify_locked
        tr("Unlocked"),         // svn_wc_notify_unlocked
        tr("Failed to lock"),   // svn_wc_notify_failed_lock
        tr("Failed to unlock")  // svn_wc_notify_failed_unlock
      };

      if (ACTION_NAMES[i] != NULL)
      {
        static QString wxpath;

#ifdef __WXMSW__
        wxpath = Utf8ToLocal("O:/rapidsvn/TRANSLATIONS");
#else
        wxpath = Utf8ToLocal("/home/sleepy/projects/rapidsvn/TRANSLATIONS");
#endif
        static QString msg;
        msg.Printf("%s: %s, %d", ACTION_NAMES[i], wxpath.c_str(), j);

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

  QString name("Tested Listener, 1000 loops through all the 22/26 actions");
  PrintTimeMeasurements(start, end, name);
}

void
MainWindow::OnTestCheckout()
{
  apr_time_t start = apr_time_now();

  Action * action = new CheckoutAction(this);

  if (action)
    Perform(action);

  apr_time_t end = apr_time_now();
  QString name("Tested Checkout (works only in single-threaded version"));
  PrintTimeMeasurements(start, end, name);
}

void
MainWindow::OnTestCertDlg()
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
  QString modalDescr;
  switch (modalResult)
  {
  case wxID_OK:
    modalDescr="wxID_OK";
    break;
  case wxID_CANCEL:
    modalDescr="wxID_CANCEL";
    break;
  default:
    modalDescr=QString::Format("%08x", modalResult);
  }

  QString answer;
  switch (dlg.Answer())
  {
  case svn::ContextListener::DONT_ACCEPT:
    answer = "DONT_ACCEPT";
    break;
  case svn::ContextListener::ACCEPT_TEMPORARILY:
    answer = "ACCEPT_TEMPORARILY";
    break;
  case svn::ContextListener::ACCEPT_PERMANENTLY:
    answer = "ACCEPT_PERMANENTLY";
    break;
  default:
    answer = "Invalid answer";
  }


  QString msg = QString::Format(
    "Modal result:%s\nAccepted Failures: %08x\nAnswer: %s",
    modalDescr.c_str(), dlg.AcceptedFailures(), answer.c_str());
  ::wxMessageBox(msg, "Certificate Dlg Results", wxOK);
}

void
MainWindow::OnTestDestinationDlg()
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

  DestinationDlg dlg(this, "DestinationDlg Title",
                     "Here comes the description",
                     flags, "/home/foo");

  int result = dlg.ShowModal();
  QString modalDescr;
  switch (result)
  {
  case wxID_OK:
    modalDescr = "wxID_OK";
    break;
  case wxID_CANCEL:
    modalDescr = "wxID_CANCEL";
    break;
  default:
    modalDescr = QString::Format("%08x");
  }
  int force = dlg.GetForce();

  QString msg = QString::Format(
    "Modal result:%s\nDestination: \"%s\"\nForce: %s",
    modalDescr.c_str(), dlg.GetDestination().c_str(),
    force ? "true" : "false");
  ::wxMessageBox(msg, "DestinationDlg Results", wxOK);
}


void
MainWindow::OnTestDndDlg(wxCommandEvent &)
{
  int action = DragAndDropDlg::IMPORT;
  if (m->IsMenuChecked(ID_TestDndCopy))
    action = DragAndDropDlg::COPY;
  else if (m->IsMenuChecked(ID_TestDndCopyMove))
    action = DragAndDropDlg::COPY_MOVE;

  DragAndDropDlg dlg(this, "/home/foo/src",
                     "/home/foo/dst",
                     action);

  int result = dlg.ShowModal();
  QString resultStr;
  switch (result)
  {
  case wxID_CANCEL:
    resultStr = "wxID_CANCEL";
    break;
  case DragAndDropDlg::RESULT_MOVE:
    resultStr = "RESULT_MOVE";
    break;
  case DragAndDropDlg::RESULT_COPY:
    resultStr = "RESULT_COPY";
    break;
  case DragAndDropDlg::RESULT_IMPORT:
    resultStr = "RESULT_IMPORT";
    break;
  default:
    resultStr = QString::Format("Unknown %08x", result);
  }

  QString msg = QString::Format(
    "Modal result:%s", resultStr.c_str());
  ::wxMessageBox(msg, "DndDlg Results", wxOK);
}
#endif

void
MainWindow::OnFileCommand()
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
      m->logTracer->Trace(tr("UnUNPORTED_TO_QT action!"));
  }

}

void
MainWindow::OnActionEvent()
{
  const int token = event.GetInt();

  switch (token)
  {
  case TOKEN_INFO:
    m->Trace(event.GetString());
    break;

  case TOKEN_ERROR:
    m->TraceError(event.GetString());
    break;

  case TOKEN_SVN_INTERNAL_ERROR:
  case TOKEN_INTERNAL_ERROR:
    m->TraceError(event.GetString());
    // Action is interrupted, so cancel listener
    // (in order to enable filelist update)
    // and disable "Running" state
    m->listener.cancel(false);
    RefreshFileList();
    m->Trace(tr("Ready\n"));
    m->SetRunning(false);
    break;

  case TOKEN_ACTION_START:
    m->Trace(event.GetString());
    wxLogStatus(event.GetString());
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
      m->TraceError(tr("Internal Error: no client data for action event!"));
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
        m->Trace(tr("Updating..."));
        RefreshFolderBrowser();
      }
      else if ((actionFlags & Action::DONT_UPDATE) == 0)
      {
        m->Trace(tr("Updating..."));
        RefreshFileList();
      }
    }

    m->Trace(tr("Ready\n"));
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
    QString apr_iconv_path;
    bool saved_apr_iconv_path = false;
    if (wxGetEnv("__SAVED_APR_ICONV_PATH", &apr_iconv_path))
    {
      saved_apr_iconv_path = true;
      wxSetEnv("APR_ICONV_PATH", apr_iconv_path.c_str());
      wxUnsetEnv("__SAVED_APR_ICONV_PATH");
    }
#endif
    // execute the command sent
    wxExecute(event.GetString());
#ifdef _WIN32
    // set custom environment again
    if (saved_apr_iconv_path)
    {
      wxSetEnv("__SAVED_APR_ICONV_PATH", apr_iconv_path.c_str());
      wxSetEnv("APR_ICONV_PATH", wxStandardPaths::Get().GetDataDir() + "\\iconv");
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
    //  TraceError (QString::Format (tr("Execution of command failed: '%s'"),
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
    const QString bookmark = event.GetString();

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
      LogDlg dlg(this, pData->target.c_str(), pData->logEntries);
      dlg.ShowModal();

      delete pData->logEntries;
      delete pData;
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
MainWindow::OnToolEnter()
{
}

void
MainWindow::OnFolderBrowserSelChanged(wxTreeEvent & WXUNUSED(event))
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
    const QString & path = m_folderBrowser->GetPath();
    m->MenuBar->Enable(ID_Flat, !path.IsEmpty());

    UpdateCurrentPath();
    RefreshFileList();
  }
  catch (...)
  {
    m->Trace(tr("Exception occured during filelist update"));
  }
}

void
MainWindow::OnFolderBrowserKeyDown(wxTreeEvent & event)
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
MainWindow::OnFileListSelected(wxListEvent & WXUNUSED(event))
{
  m->activePane = ACTIVEPANE_FILELIST;
}


void
MainWindow::OnListenerEvent()
{
  m->listener.handleEvent(event);
}

/*** END OF SECTION EVENTS ***/


void
MainWindow::AddWcBookmark()
{
  const wxArrayString & lastDirs=TheHistoryManager.ReadList(HISTORY_EXISTING_WORKING_DIRECTORY);

  QString dir;
  if (lastDirs.IsEmpty())
    dir = wxGetHomeDir();
  else
    dir = lastDirs[0];

  wxDirDialog dialog(this, tr("Select a directory"), dir);
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
      tr("You cannot add a subversion administrative \
directory to the bookmarks!"),
      tr("Error"), wxOK);
    return;
  }

  // add
  m_folderBrowser->AddBookmark(dialog.GetPath());

  m->skipFilelistUpdate = true;
  RefreshFolderBrowser();

  wxLogStatus(tr("Added working copy to bookmarks '%s'"),
              dialog.GetPath().c_str());

  m_folderBrowser->SelectBookmark(dialog.GetPath());

  TheHistoryManager.AddEntryToList(HISTORY_EXISTING_WORKING_DIRECTORY, dialog.GetPath());
}

void
MainWindow::AddRepoBookmark()
{
  const int flags =
    UpdateDlg::WITH_URL |
    UpdateDlg::WITHOUT_RECURSIVE |
    UpdateDlg::WITHOUT_REVISION |
    UpdateDlg::WITHOUT_IGNORE_EXTERNALS;
  UpdateDlg dialog(this, tr("Repository URL"), flags);

  if (dialog.ShowModal() != wxID_OK)
  {
    return;
  }

  // add
  QString url = dialog.GetData().url;
  m_folderBrowser->AddBookmark(url);

  m->skipFilelistUpdate = true;
  RefreshFolderBrowser();

  wxLogStatus(tr("Added repository to bookmarks '%s'"),
              url.c_str());

  m_folderBrowser->SelectBookmark(url);
  //UpdateCurrentPath ();
  //RefreshFileList ();
}

inline void
MainWindow::RemoveBookmark()
{
  wxASSERT(m_folderBrowser);
  if (m_folderBrowser->RemoveBookmark())
  {
    wxLogStatus(tr("Removed bookmark"));
  }
}

void
MainWindow::EditBookmark()
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
  QString oldPath(bookmark->getPath());
  DestinationDlg dlg(this, tr("Edit Bookmark"), tr("Bookmark"),
                     0, oldPath);

  if (dlg.ShowModal() == wxID_OK)
  {
    // and if everything goes right, remove the old
    // selection and add the new one
    QString newPath(dlg.GetDestination());

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
MainWindow::ShowInfo()
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

    QString info = fileInfo.info();

    ReportDlg dlg(this, tr("Info"), info, NORMAL_REPORT);
    dlg.ShowModal();
  }
  catch (svn::ClientException & e)
  {
    m->Trace(Utf8ToLocal(e.message()));
    return;
  }

}


void
MainWindow::ShowPreferences()
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
MainWindow::UpdateCurrentPath()
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
    SetTitle(m_title + " - " + m->currentPath);
  else
    SetTitle(m_title);
}

bool
MainWindow::InvokeDefaultAction()
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
MainWindow::Perform(Action * action)
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
MainWindow::UpdateMenuSorting()
{
  m->CheckSort(m_listCtrl->GetSortColumn() + ID_ColumnSort_Min + 1);
}

inline void
MainWindow::UpdateMenuIncludePath()
{
  m->CheckMenu(ID_Include_Path, m_listCtrl->GetIncludePath());
}

inline void
MainWindow::UpdateMenuAscending()
{
  m->CheckMenu(ID_Sort_Ascending, m_listCtrl->GetSortAscending());
}

void
MainWindow::SetIncludePathVisibility(bool flatMode)
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
MainWindow::OnSize(wxSizeEvent & sizeEvent)
{
  sizeEvent.Skip();
}


void
MainWindow::OnFolderBrowserSetFocus(wxFocusEvent& WXUNUSED(event))
{
  SetActivePane(ACTIVEPANE_FOLDER_BROWSER);
}


void
MainWindow::OnListCtrlSetFocus(wxFocusEvent& WXUNUSED(event))
{
  SetActivePane(ACTIVEPANE_FILELIST);
}

/**
 * In @ref MainWindowBase there is an event handler that uses
 * the idle event as well to set the splitter sash position.
 * So we have to wait just one call longer until we can set the
 * desired sash position
 */
void
MainWindow::OnIdle(wxIdleEvent & WXUNUSED(event))
{
  m->idleCount++;

  if (m->idleCount >= 2)
  {
    m_splitterVert->SetSashPosition(m->vertSashPos);
    m_splitterHoriz->SetSashPosition(m->horizSashPos);

    // we dont need this event anymore
    Disconnect(wxEVT_IDLE, wxIdleEventHandler(MainWindow::OnIdle), NULL, this);
  }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
