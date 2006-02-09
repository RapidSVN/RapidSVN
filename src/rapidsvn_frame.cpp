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
#include "wx/confbase.h"
#include "wx/wx.h"
#include "wx/filename.h"
#include <wx/tipdlg.h>

// svncpp
#include "svncpp/apr.hpp"
#include "svncpp/context.hpp"
#include "svncpp/check.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/targets.hpp"
#include "svncpp/url.hpp"
#include "svncpp/wc.hpp"

// app
#include "rapidsvn_app.hpp"
#include "config.hpp"
#include "destination_dlg.hpp"
#include "diff_data.hpp"
#include "ids.hpp"
#include "file_info.hpp"
#include "listener.hpp"
#include "folder_item_data.hpp"

#ifdef USE_SIMPLE_WORKER
#include "simple_worker.hpp"
#else
#include "threaded_worker.hpp"
#endif

// actions
#include "add_action.hpp"
#include "checkout_action.hpp"
#include "cleanup_action.hpp"
#include "commit_action.hpp"
#include "delete_action.hpp"
#include "diff_action.hpp"
#include "external_program_action.hpp"
#include "get_action.hpp"
#include "import_action.hpp"
#include "export_action.hpp"
#include "lock_action.hpp"
#include "merge_action.hpp"
#include "mkdir_action.hpp"
#include "move_action.hpp"
#include "property_action.hpp"
#include "resolve_action.hpp"
#include "revert_action.hpp"
#include "switch_action.hpp"
#include "unlock_action.hpp"
#include "update_action.hpp"
#include "view_action.hpp"

// dialogs
#include "about_dlg.hpp"
#include "auth_dlg.hpp"
#include "report_dlg.hpp"
#include "preferences.hpp"
#include "preferences_dlg.hpp"
#include "update_dlg.hpp"

#include "rapidsvn_frame.hpp"

// controls
#include "proportional_splitter.hpp"

// Bitmaps
#include "res/bitmaps/svn.xpm"
#include "res/bitmaps/refresh.xpm"
#include "res/bitmaps/update.xpm"
#include "res/bitmaps/commit.xpm"
#include "res/bitmaps/add.xpm"
#include "res/bitmaps/delete.xpm"
#include "res/bitmaps/revert.xpm"
#include "res/bitmaps/resolve.xpm"
#include "res/bitmaps/log.xpm"
#include "res/bitmaps/info.xpm"
#include "res/bitmaps/stop.xpm"

// number of items initially in the list
static const int NUM_ITEMS = 30;

// List config keys here, to avoid duplicating literal text:
const static wxChar ConfigLeft[] = wxT("/MainFrame/Left");
const static wxChar ConfigTop[] = wxT("/MainFrame/Top");
const static wxChar ConfigWidth[] = wxT("/MainFrame/Width");
const static wxChar ConfigHeight[] = wxT("/MainFrame/Height");
const static wxChar ConfigMaximized[] = wxT("/MainFrame/Maximized");
const static wxChar ConfigSplitterHoriz[] = wxT("/MainFrame/SplitterHoriz");
const static wxChar ConfigSplitterVert[] = wxT("/MainFrame/SplitterVert");

const static wxChar TraceMisc[] = wxT("tracemisc");

// Platform specific constants.
#ifdef __WXMSW__
  const static int SPLITTER_STYLE = wxSP_FULLSASH | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN;
#else
  const static int SPLITTER_STYLE = wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN;
#endif


/**
 * map to match columns with menu Ids.
 *
 * @remark Make sure every time you add a new column
 *         to @a FileListCtrl you have to add an entry
 *         here. Same order!
 */
static const int COLUMN_ID_MAP[FileListCtrl::COL_COUNT] =
{
  -1, // cannot show/hide name
  -1, // cannot show/hide path
  ID_Column_Rev,
  ID_Column_Cmt_Rev,
  ID_Column_Author,
  ID_Column_Text_Status,
  ID_Column_Prop_Status,
  ID_Column_Cmt_Date,
  ID_Column_Extension,
  ID_Column_Text_Time,
  ID_Column_Prop_Time,
  ID_Column_LockOwner,
  ID_Column_LockComment,
  ID_Column_Checksum,
  ID_Column_Url,
  ID_Column_Repos,
  ID_Column_Uuid,
  ID_Column_Schedule,
  ID_Column_Copied,
  ID_Column_ConflictOld,
  ID_Column_ConflictNew,
  ID_Column_ConflictWrk
};


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


class LogTracer:public wxTextCtrl, public Tracer
{

public:
  LogTracer (wxWindow * parent)
    : wxTextCtrl (parent, -1, wxEmptyString,
                  wxPoint (0, 0), wxDefaultSize,
                  wxTE_MULTILINE | wxTE_READONLY)
  {
    SetMaxLength (0);
  }

  void Trace (const wxString & str)
  {
    AppendText (str + wxT("\n"));
  }
};


/**
 * class that hide implementation specific data and methods from
 * the interface
 */
struct RapidSvnFrame::Data
{
public:
  wxMenu * MenuColumns;
  wxMenuBar * MenuBar;
  Listener listener;
  bool updateAfterActivate;
  bool dontUpdateFilelist;
  const wxLocale & locale;
private:
  bool m_running;
  size_t m_toolbar_rows;        // 1 or 2 only (toolbar rows)
  wxFrame * m_parent;

public:

  /**
   * This instance of @a apr is used to initialize/terminate apr
   */
  svn::Apr apr;

  Data (wxFrame * parent, const wxLocale & locale_)
    : MenuColumns (0), MenuBar (0), listener (parent),
      updateAfterActivate (false), dontUpdateFilelist (false),
      locale (locale_), m_running (false), m_toolbar_rows (1),
      m_parent (parent)
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

    /**
     * Captions for the columns. We cant use the texts used for
     * the column headings in FileListCtrl since they dont have
     * shortcuts.
     */
    const wxChar *
    COLUMN_CAPTIONS[FileListCtrl::COL_COUNT] =
    {
      wxT(""), // Name is not used here
      wxT(""), // Path is not used here
      _("&Revision"),
      _("R&ep. Rev."),
      _("&Author"),
      _("&Status"),
      _("&Prop Status"),
      _("&Last Changed"),
      _("E&xtension"),
      _("&Date"),
      _("Pr&op Date"),
      _("Lock Owner"),
      _("Lock Comment"),
      _("Checksum"),
      _("URL"),
      _("Repository"),
      _("UUID"),
      _("Schedule"),
      _("Copied"),
      _("Conflict Old"),
      _("Conflict New"),
      _("Conflict Work")
    };

    for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
    {
      int id = COLUMN_ID_MAP[col];
      if (id != -1)
      {
        MenuColumns->AppendCheckItem (id, COLUMN_CAPTIONS[col]);
      }
    }

    // View menu
    wxMenu *menuView = new wxMenu;
    AppendMenuItem (*menuView, ID_Refresh);
    AppendMenuItem (*menuView, ID_Stop);
    menuView->AppendSeparator ();

    AppendMenuItem (*menuView, ID_Explore);
    menuView->AppendSeparator ();

    menuView->Append (0, _("Columns"), MenuColumns);
    menuView->AppendCheckItem (ID_Flat, _("Flat Mode"));
    menuView->AppendCheckItem (ID_RefreshWithUpdate, _("Refresh with Update"));
    menuView->AppendCheckItem (ID_ShowUnversioned, _("Show unversioned entries"));
    if (svn::SUPPORTS_EXTERNALS)
      menuView->AppendCheckItem (ID_IgnoreExternals, _("Ignore Externals"));

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

#ifdef  USE_HTML_HELP
    menuHelp->Append (ID_HelpContents, _("&Contents\tF1"));
    menuHelp->Append (ID_HelpIndex, _("&Index\tShift+F1"));
    menuHelp->AppendSeparator ();
#endif
#ifdef USE_STARTUP_TIPS
    menuHelp->Append (ID_HelpStartupTips, _("Show Startup Tips"));
    menuHelp->AppendSeparator ();
#endif
    menuHelp->Append (ID_About, _("&About..."));

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
                      wxBitmap (refresh_xpm),
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
                      wxBitmap (stop_xpm),
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
                      wxBitmap (add_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Add selected"),
                      _("Put files and directories under revision control"));

    toolBar->AddTool (ID_Delete,
                      wxBitmap (delete_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Delete selected"),
                      _("Delete files and directories from version control"));

    toolBar->AddTool (ID_Update,
                      wxBitmap (update_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Update selected"),
                      _("Bring changes from the repository into the working copy"));

    toolBar->AddTool (ID_Commit,
                      wxBitmap (commit_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Commit selected"),
                      _("Send changes from your working copy to the repository"));

    toolBar->AddTool (ID_Revert,
                      wxBitmap (revert_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Revert selected"),
                      _("Restore pristine working copy file (undo all local edits)"));

    toolBar->AddTool (ID_Resolve,
                      wxBitmap (resolve_xpm),
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
                      wxBitmap (info_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Info selected"),
                      _("Display info about selected entries"));

    toolBar->AddTool (ID_Log,
                      wxBitmap (log_xpm),
                      wxNullBitmap,
                      FALSE,
                      -1,
                      -1,
                      (wxObject *) NULL,
                      _("Log selected"),
                      _("Show the log messages for a set entries"));

    toolBar->AddSeparator ();
  }
};

BEGIN_EVENT_TABLE (RapidSvnFrame, wxFrame)
  EVT_ACTIVATE (RapidSvnFrame::OnActivate)
  EVT_MENU (ID_AddWcBookmark, RapidSvnFrame::OnAddWcBookmark)
  EVT_MENU (ID_AddRepoBookmark, RapidSvnFrame::OnAddRepoBookmark)
  EVT_MENU (ID_RemoveBookmark, RapidSvnFrame::OnRemoveBookmark)
  EVT_MENU (ID_EditBookmark, RapidSvnFrame::OnEditBookmark)
  EVT_MENU (ID_Quit, RapidSvnFrame::OnQuit)
  EVT_MENU (ID_About, RapidSvnFrame::OnAbout)

  EVT_MENU (ID_Info, RapidSvnFrame::OnInfo)
  EVT_UPDATE_UI (ID_Info, RapidSvnFrame::OnUpdateCommand)

  EVT_MENU (ID_HelpContents, RapidSvnFrame::OnHelpContents)
  EVT_MENU (ID_HelpIndex, RapidSvnFrame::OnHelpIndex)
  EVT_MENU (ID_HelpStartupTips, RapidSvnFrame::OnHelpStartupTips)

  EVT_MENU (ID_Preferences, RapidSvnFrame::OnPreferences)
  EVT_MENU (ID_Refresh, RapidSvnFrame::OnRefresh)
  EVT_MENU (ID_Column_Reset, RapidSvnFrame::OnColumnReset)
  EVT_MENU (ID_Flat, RapidSvnFrame::OnFlatView)
  EVT_MENU (ID_RefreshWithUpdate, RapidSvnFrame::OnRefreshWithUpdate)
  EVT_MENU (ID_ShowUnversioned, RapidSvnFrame::OnShowUnversioned)
#if CHECK_SVN_SUPPORTS_EXTERNALS
    EVT_MENU (ID_IgnoreExternals, RapidSvnFrame::OnIgnoreExternals)
#endif
  EVT_MENU (ID_Login, RapidSvnFrame::OnLogin)
  EVT_MENU (ID_Logout, RapidSvnFrame::OnLogout)
  EVT_MENU (ID_Stop, RapidSvnFrame::OnStop)

  EVT_MENU_RANGE (ID_File_Min, ID_File_Max, RapidSvnFrame::OnFileCommand)
  EVT_MENU_RANGE (ID_Verb_Min, ID_Verb_Max, RapidSvnFrame::OnFileCommand)
  EVT_MENU_RANGE (ID_Column_Min, ID_Column_Max, RapidSvnFrame::OnColumn)

  EVT_UPDATE_UI_RANGE (ID_File_Min, ID_File_Max, RapidSvnFrame::OnUpdateCommand)
  EVT_UPDATE_UI_RANGE (ID_Verb_Min, ID_Verb_Max, RapidSvnFrame::OnUpdateCommand)

  EVT_MENU (ACTION_EVENT, RapidSvnFrame::OnActionEvent)

  EVT_TOOL_ENTER (ID_TOOLBAR, RapidSvnFrame::OnToolEnter)
  EVT_TREE_SEL_CHANGED (-1, RapidSvnFrame::OnFolderBrowserSelChanged)
  EVT_TREE_KEY_DOWN (-1, RapidSvnFrame::OnFolderBrowserKeyDown)
  EVT_LIST_ITEM_SELECTED (-1, RapidSvnFrame::OnFileListSelected)
END_EVENT_TABLE ()

/** class implementation **/
RapidSvnFrame::RapidSvnFrame (const wxString & title,
                              const wxLocale & locale)
  : wxFrame ((wxFrame *) NULL, -1, title, wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_FRAME_STYLE),
    m_folder_browser (NULL), m_listCtrl (NULL), m_title (title), m_context (0),
    m_activePane (ACTIVEPANE_FOLDER_BROWSER)
{
  m = new Data (this, locale);
  m_actionWorker = CreateActionWorker (this);

  // enable trace
  wxLog::AddTraceMask (TraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *cfg = wxConfigBase::Get ();

  SetIcon (wxIcon (svn_xpm));

  // toolbar rows
  m_toolbar_rows = 1;

  SetMenuBar (m->MenuBar);
  CreateStatusBar ();

  // Create the toolbar
  m->RecreateToolbar ();
  m->SetRunning (false);

  m_horiz_splitter =
#if wxCHECK_VERSION(2, 6, 0)
    new wxSplitterWindow (this,
                                    SPLITTER_WINDOW,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    SPLITTER_STYLE);
  m_horiz_splitter->SetSashGravity(1.0f);
#else
    new ProportionalSplitterWindow (1.0f, this,
                                    SPLITTER_WINDOW,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    SPLITTER_STYLE);
#endif

  m_info_panel = new wxPanel (m_horiz_splitter, -1,
                              wxDefaultPosition, wxDefaultSize,
                              wxTAB_TRAVERSAL | wxCLIP_CHILDREN);

  m_log = new wxTextCtrl (m_horiz_splitter, -1, wxEmptyString,
                          wxPoint (0, 0), wxDefaultSize,
                          wxTE_MULTILINE | wxTE_READONLY);

  // as much as the widget can stand
  m_log->SetMaxLength (0);

  m_logTracer = new EventTracer (this);
  m->listener.SetTracer (m_logTracer, false);


  m_vert_splitter = new wxSplitterWindow (m_info_panel,
                                          SPLITTER_WINDOW,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          SPLITTER_STYLE);
#if wxCHECK_VERSION(2, 6, 0)
  // Not a complete solution, but a workaround for now...
  m_vert_splitter->SetSashGravity(0.2f);
#endif

  // Create the list control to display files
  m_listCtrl = new FileListCtrl (m_vert_splitter, FILELIST_CTRL,
                                 wxDefaultPosition, wxDefaultSize);

  m->CheckMenu (ID_Flat,              false);
  m->CheckMenu (ID_RefreshWithUpdate, m_listCtrl->GetWithUpdate());
  m->CheckMenu (ID_ShowUnversioned,   m_listCtrl->GetShowUnversioned());
  if (svn::SUPPORTS_EXTERNALS)
    m->CheckMenu (ID_IgnoreExternals,   m_listCtrl->GetIgnoreExternals());

  // Create the browse control
  m_folder_browser = new FolderBrowser (m_vert_splitter, FOLDER_BROWSER);

  // Adapt the menu entries
  for (int col=0; col < FileListCtrl::COL_COUNT; col++)
  {
    int id = COLUMN_ID_MAP[col];
    if (id != -1)
    {
      m->CheckColumn (id, m_listCtrl->GetColumnVisible (col));
    }
  }

  UpdateFileList ();

  wxSizer *sizer = new wxBoxSizer (wxVERTICAL);
  sizer->Add (m_vert_splitter, 1, wxEXPAND);

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
  GetSize (&w, &h);
  int vpos = cfg->Read (ConfigSplitterVert, w / 3);
  int hpos = cfg->Read (ConfigSplitterHoriz, (3 * h) / 4);

  // initialize the folder browser
  m_folder_browser->ReadConfig (cfg);
  {
    Preferences prefs;
    m_folder_browser->SetListener        (&m->listener);
    m_folder_browser->SetAuthCache       (prefs.useAuthCache);
    m_folder_browser->SetAuthPerBookmark (prefs.authPerBookmark);
  }
  UpdateCurrentPath ();
  UpdateFolderBrowser ();

  // Set sash position for every splitter.
  // Note: to not revert the order of Split calls, as the panels will be messed up.
  m_horiz_splitter->SplitHorizontally (m_info_panel, m_log, hpos);
  m_vert_splitter->SplitVertically (m_folder_browser, m_listCtrl, vpos);
}

RapidSvnFrame::~RapidSvnFrame ()
{
  wxConfigBase *cfg = wxConfigBase::Get ();
  if (cfg == NULL)
    return;

  if (m_logTracer)
    delete m_logTracer;

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
                  (long) m_vert_splitter->GetSashPosition ());
  cfg->Write (ConfigSplitterHoriz,
                  (long) m_horiz_splitter->GetSashPosition ());

  m_folder_browser->WriteConfig (cfg);

  delete m;
}

void
RapidSvnFrame::UpdateFileList ()
{
  if (m->dontUpdateFilelist)
    return;

  m->SetRunning (true);

  if (m_listCtrl && m_folder_browser)
  {
    wxBusyCursor busy;
    if (m_currentPath.length () == 0)
      m_listCtrl->Show (FALSE);
    else
    {
      try
      {
        m_listCtrl->SetContext (m_context);
        m_listCtrl->UpdateFileList (m_currentPath);
        m_listCtrl->Show (TRUE);

      }
      catch (svn::ClientException & e)
      {
        wxString msg, errtxt (Utf8ToLocal (e.message ()));
        msg.Printf (_("Error while updating filelist (%s)"),
                    errtxt.c_str ());
        Trace (msg);

        // probably unversioned resource
        m_listCtrl->Show (FALSE);
      }
      catch (...)
      {
        Trace (_("Error while updating filelist"));
      }
    }
  }

  m->SetRunning (false);
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
RapidSvnFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  AboutDlg dlg (this, m->locale);

  dlg.ShowModal ();
}

void
RapidSvnFrame::OnInfo (wxCommandEvent & WXUNUSED (event))
{
  ShowInfo ();
}

void
RapidSvnFrame::OnHelpContents (wxCommandEvent & WXUNUSED (event))
{
#ifdef  USE_HTML_HELP
  ::wxGetApp ().GetHelpController().DisplayContents();
#endif
}

void
RapidSvnFrame::OnHelpIndex (wxCommandEvent & WXUNUSED (event))
{
#ifdef  USE_HTML_HELP
  ::wxGetApp ().GetHelpController().DisplayIndex();
#endif
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

void RapidSvnFrame::OnHelp (wxCommandEvent &WXUNUSED(event))
{
#ifdef USE_HTML_HELP
    wxWindow *active = wxGetActiveWindow();
    wxString helptext;
    while (active && helptext.IsEmpty())
    {
        helptext = active->GetHelpText();
        active = GetParent();
    }
    wxGetApp ().GetHelpController().Display(helptext);
#endif
}

void
RapidSvnFrame::OnPreferences (wxCommandEvent & WXUNUSED (event))
{
  ShowPreferences ();
}

void
RapidSvnFrame::OnRefresh (wxCommandEvent & WXUNUSED (event))
{
  UpdateFolderBrowser ();
}


void
RapidSvnFrame::OnActivate (wxActivateEvent & event)
{
  if (event.GetActive () && m->updateAfterActivate)
  {
    m->updateAfterActivate = false;

    UpdateFileList ();
  }

  // wxMac needs this, otherwise the menu doesn't show:
#ifdef __WXMAC__
  event.Skip();
#endif
}


void
RapidSvnFrame::OnToolEnter (wxCommandEvent & event)
{
}


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
  m_folder_browser->AddBookmark (dialog.GetPath ());
  UpdateFolderBrowser ();

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
  m_folder_browser->AddBookmark (url);
  UpdateFolderBrowser ();

  wxLogStatus (_("Added repository to bookmarks '%s'"),
               url.c_str ());
}



void
RapidSvnFrame::RemoveBookmark ()
{
  wxASSERT (m_folder_browser);
  if( m_folder_browser->RemoveBookmark() )
  {
    wxLogStatus (_("Removed bookmark"));
  }
}


void
RapidSvnFrame::EditBookmark ()
{
  wxASSERT (m_folder_browser);

  const FolderItemData * bookmark = m_folder_browser->GetSelection ();

  // if nothing is selected, or the wrong type,
  // just exit (this should be handled by the routine
  // that is responsible for greying out menu entries,
  // but hey: nobody is perfect
  if (!m_folder_browser)
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

    m_folder_browser->RemoveBookmark ();
    m_folder_browser->AddBookmark (newPath);
    UpdateFolderBrowser ();
    m_folder_browser->SelectBookmark (newPath);
  }
}


const svn::Targets
RapidSvnFrame::GetActionTargets () const
{
  //is there nothing selected in the list control,
  //or is the active window *not* the list control?
  if (m_listCtrl->GetSelectedItemCount () <= 0 ||
      m_activePane != ACTIVEPANE_FILELIST)
  {
    wxString path = m_folder_browser->GetPath ();

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
    return m_listCtrl->GetTargets ();
  }
}

unsigned int
RapidSvnFrame::GetSelectionActionFlags () const
{
  unsigned int flags = 0;

  //is there nothing selected in the list control,
  //or is the active window *not* the list control?
  if (m_listCtrl->GetSelectedItemCount () <= 0 ||
      m_activePane != ACTIVEPANE_FILELIST)
  {
    //yes, so examine the folder browser

    flags |= Action::IS_DIR;
    wxString path = m_folder_browser->GetPath ();

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
    flags = m_listCtrl->GetSelectionActionFlags ();
  }

  return flags;
}

/* ValidateIDActionFlags and OnFileCommand could be refactored into a single class factory (big switch statement)
   and GetFlags/Perform calls, but that seems a little messy, so keeping them separate for now. */
bool
RapidSvnFrame::ValidateIDActionFlags (int id, unsigned int selectionActionFlags)
{
  unsigned int baseActionFlags = 0;
  if ((id >= ID_Verb_Min) && (id <= ID_Verb_Max))
  {
    baseActionFlags = ExternalProgramAction::GetBaseFlags ();
  }
  else
  {
    switch (id)
    {
    case ID_Explore:
      // Special case of ExternalProgramAction - needs to be a working copy, not just single target
      baseActionFlags = ExternalProgramAction::GetBaseFlags () & ~Action::RESPOSITORY_TYPE;
      break;

    case ID_Default_Action:
      baseActionFlags = ExternalProgramAction::GetBaseFlags ();
      break;

    case ID_Update:
      baseActionFlags = UpdateAction::GetBaseFlags ();
      break;

    case ID_Commit:
      baseActionFlags = CommitAction::GetBaseFlags ();
      break;

    case ID_Add:
      baseActionFlags = AddAction::GetBaseFlags ();
      break;

    case ID_AddRecursive:
      baseActionFlags = AddAction::GetBaseFlags ();
      break;

    case ID_Import:
      baseActionFlags = ImportAction::GetBaseFlags ();
      break;

    case ID_Export:
      baseActionFlags = ExportAction::GetBaseFlags ();
      break;

    case ID_Checkout:
      baseActionFlags = CheckoutAction::GetBaseFlags ();
      break;

    case ID_Cleanup:
      baseActionFlags = CleanupAction::GetBaseFlags ();
      break;

    case ID_Lock:
      if (svn::SUPPORTS_LOCK)
        baseActionFlags = LockAction::GetBaseFlags ();
      break;

    case ID_Log:
      baseActionFlags = LogAction::GetBaseFlags ();
      break;

    case ID_Revert:
      baseActionFlags = RevertAction::GetBaseFlags ();
      break;

    case ID_Resolve:
      baseActionFlags = ResolveAction::GetBaseFlags ();
      break;

    case ID_Delete:
      baseActionFlags = DeleteAction::GetBaseFlags ();
      break;

    case ID_Copy:
      baseActionFlags = MoveAction::GetBaseFlags ();
      break;

    case ID_Move:
      baseActionFlags = MoveAction::GetBaseFlags ();
      break;

    case ID_Mkdir:
      baseActionFlags = MkdirAction::GetBaseFlags ();
      break;

    case ID_Merge:
      baseActionFlags = MergeAction::GetBaseFlags ();
      break;

    case ID_Property:
      baseActionFlags = PropertyAction::GetBaseFlags ();
      break;

    case ID_Rename:
      baseActionFlags = MoveAction::GetBaseFlags ();
      break;

    case ID_Switch:
      baseActionFlags = SwitchAction::GetBaseFlags ();
      break;

    case ID_Diff:
    case ID_DiffBase:
    case ID_DiffHead:
      baseActionFlags = DiffAction::GetBaseFlags ();
      break;

    case ID_Info:
      // Not actually part of the Action hierarchy, but here for completeness
      baseActionFlags = Action::SINGLE_TARGET|Action::MULTIPLE_TARGETS|Action::RESPOSITORY_TYPE|Action::VERSIONED_WC_TYPE|Action::UNVERSIONED_WC_TYPE;
      break;

    case ID_Unlock:
      if (svn::SUPPORTS_LOCK)
        baseActionFlags = UnlockAction::GetBaseFlags ();
      break;

    case ID_Edit:
      baseActionFlags = ViewAction::GetEditFlags ();
      break;

    default:
      // If unrecognised, by default return true
      return true;
      break;
    }
  }

  if (baseActionFlags & Action::WITHOUT_TARGET)
  {
    return true;
  }

  // Check the sole quantity flag in selectedActionFlags is in baseActionFlags
  // then check any type flags in selectedActionFlags are in baseActionFlags, but selectedActionFlags doesn't include any other types
  return (selectionActionFlags & baseActionFlags & Action::TARGET_QUANTITY_MASK) != 0
            && (selectionActionFlags & baseActionFlags & Action::TARGET_TYPE_MASK) == (selectionActionFlags & Action::TARGET_TYPE_MASK);
}

void
RapidSvnFrame::OnUpdateCommand (wxUpdateUIEvent & updateUIEvent)
{
  updateUIEvent.Enable (ValidateIDActionFlags (updateUIEvent.m_id, GetSelectionActionFlags ()));
}

void
RapidSvnFrame::TrimDisabledMenuItems (wxMenu & menu)
{
  // Check for disabled items
  size_t pos = menu.GetMenuItemCount ();
  unsigned int selectionActionFlags = GetSelectionActionFlags ();
  while (pos-- > 0)
  {
    wxMenuItem *pItem = menu.FindItemByPosition (pos);
    if (!pItem->IsSeparator () && !ValidateIDActionFlags (pItem->GetId (), selectionActionFlags))
    {
      menu.Destroy (pItem);
    }
  }

  // Trim unnecessary separators
  pos = menu.GetMenuItemCount ();
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
RapidSvnFrame::SetActivePane (ActivePane value)
{
  m_activePane = value;
}

void
RapidSvnFrame::OnFileCommand (wxCommandEvent & event)
{
  Action* action = NULL;

  if ((event.m_id >= ID_Verb_Min) && (event.m_id <= ID_Verb_Max))
  {
    action = new ExternalProgramAction (this, event.m_id - ID_Verb_Min, false);
  }
  else
  {
    switch (event.m_id)
    {
    case ID_Explore:
      action = new ExternalProgramAction (this, -1, true);
      break;

    case ID_Default_Action:
      InvokeDefaultAction();
      break;

    case ID_Update:
      action = new UpdateAction(this);
      break;

    case ID_Commit:
      action = new CommitAction(this);
      break;

    case ID_Add:
      action = new AddAction (this);
      break;

    case ID_AddRecursive:
      action = new AddAction (this, true);
      break;

    case ID_Import:
      action = new ImportAction (this);
      break;

    case ID_Export:
      action = new ExportAction (this);
      break;

    case ID_Checkout:
      action = new CheckoutAction (this);
      break;

    case ID_Cleanup:
      action = new CleanupAction (this);
      break;

    case ID_Lock:
      action = new LockAction (this);
      break;

    case ID_Log:
      action = new LogAction (this);
      break;

    case ID_Revert:
      action = new RevertAction (this);
      break;

    case ID_Resolve:
      action = new ResolveAction (this);
      break;

    case ID_Delete:
      action = new DeleteAction (this);
      break;

    case ID_Copy:
      action = new MoveAction (this, MOVE_COPY);
      break;

    case ID_Move:
      action = new MoveAction (this, MOVE_MOVE);
      break;

    case ID_Mkdir:
      action = new MkdirAction (this, m_currentPath);
      break;

    case ID_Merge:
      action = new MergeAction (this);
      break;

    case ID_Property:
      action = new PropertyAction (this);
      break;

    case ID_Rename:
      action = new MoveAction (this, MOVE_RENAME);
      break;

    case ID_Switch:
      action = new SwitchAction (this);
      break;

    case ID_Diff:
      action = new DiffAction (this);
      break;

    case ID_DiffBase:
      {
        DiffData data (svn::Revision::BASE);
        action = new DiffAction (this, data);
        break;
      }

    case ID_DiffHead:
      {
        DiffData data (svn::Revision::HEAD);
        action = new DiffAction (this, data);
        break;
      }

    case ID_Unlock:
      action = new UnlockAction (this);
      break;

    case ID_Edit:
      action = new ViewAction (this);
      break;

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

    default:
      m_logTracer->Trace (_("Unimplemented action!"));
      break;
    }
  }

  if (action)
    Perform (action);
}

void
RapidSvnFrame::OnActionEvent (wxCommandEvent & event)
{
  const int token = event.GetInt ();

  switch (token)
  {
  case TOKEN_INFO:
    Trace (event.GetString ());
    break;

  case TOKEN_SVN_INTERNAL_ERROR:
  case TOKEN_INTERNAL_ERROR:
    Trace (event.GetString ());
    UpdateFileList ();
    Trace (_("Ready\n"));
    m->SetRunning (false);
    break;

  case TOKEN_ACTION_START:
    Trace (event.GetString ());
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
        Trace (_("Error receiving action event!"));
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
          Trace (_("Updating..."));
          UpdateFolderBrowser ();
        }
        else if ((actionFlags & Action::DONT_UPDATE) == 0)
        {
          Trace (_("Updating..."));
          UpdateFileList ();
        }
      }

      Trace (_("Ready\n"));
      m->SetRunning (false);
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

      m_folder_browser->AddBookmark (bookmark);
      m_folder_browser->Refresh ();
      m_folder_browser->SelectBookmark (bookmark);
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
  }
}

void
RapidSvnFrame::ShowInfo ()
{
  bool withUpdate = false;
  if (m_listCtrl)
    withUpdate = m_listCtrl->GetWithUpdate ();

  FileInfo fileInfo (m_context, withUpdate);

  try
  {
    std::vector<svn::Path> vector = GetActionTargets ().targets ();
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
    Trace (Utf8ToLocal (e.message ()));
    return;
  }

}

void
RapidSvnFrame::OnFolderBrowserSelChanged (wxTreeEvent & event)
{
  try
  {
    m_activePane = ACTIVEPANE_FOLDER_BROWSER;

    // Update the menu and list control flat-mode setting 
    bool flatMode = m_folder_browser->IsFlat ();
    m_listCtrl->SetFlat (flatMode);
    m->CheckMenu (ID_Flat, flatMode);
    
    // Disable menu entry if no path is selected (e.g. root)
    const wxString & path = m_folder_browser->GetPath ();  
    m->MenuBar->Enable (ID_Flat, !path.IsEmpty ());

    UpdateCurrentPath ();
    UpdateFileList ();

    m_folder_browser->ExpandSelection ();
  }
  catch(...)
  {
    Trace (_("Exception occured during filelist update"));
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
RapidSvnFrame::ShowPreferences ()
{
  Preferences prefs;
  PreferencesDlg dlg (this, & prefs);
  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    m_folder_browser->SetAuthCache (prefs.useAuthCache);
    m_folder_browser->SetAuthPerBookmark (prefs.authPerBookmark);
  }
}

void
RapidSvnFrame::UpdateCurrentPath ()
{
  if (m_folder_browser == 0)
  {
    m_currentPath.Clear ();
    m_context = 0;
  }
  else
  {
    m_currentPath = m_folder_browser->GetPath ();
    m_context = m_folder_browser->GetContext ();
  }

  if (m_currentPath.Length () > 0)
    SetTitle (m_title + wxT(" - ") + m_currentPath);
  else
    SetTitle (m_title);
}

bool
RapidSvnFrame::InvokeDefaultAction ()
{
  unsigned int selectionActionFlags = GetSelectionActionFlags ();
  std::vector<svn::Path> targets = GetActionTargets ().targets ();

  // the default action will be invoked only for a single file
  // or folder.
  // if more or less than one file or folder is selected nothing
  // will happen.

  if (targets.size () != 1)
    return false;

  if (selectionActionFlags & Action::IS_DIR)
  {
    // go one folder deeper...
    m_folder_browser->SelectFolder (Utf8ToLocal(targets[0].c_str ()));
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

    svn::Path currentPathUtf8 (PathUtf8 (m_currentPath));
    action->SetPath (currentPathUtf8);
    action->SetContext (m_context);
    action->SetTargets (GetActionTargets ());
    action->SetTracer (m_logTracer, false);
    m_actionWorker->SetTracer (m_logTracer);
    m_actionWorker->SetContext (m_context, false);
    if (!m_actionWorker->Perform (action))
      m->SetRunning (false);
  }
  catch (...)
  {
    m->SetRunning (false);
    throw; // svn::Excepton (e);
  }
}

void
RapidSvnFrame::OnFileListSelected (wxListEvent & event)
{
  m_activePane = ACTIVEPANE_FILELIST;
}

void
RapidSvnFrame::OnColumn (wxCommandEvent & event)
{
  bool visible = m->IsColumnChecked(event.m_id);
  for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
  {
    if (COLUMN_ID_MAP[col] == event.m_id)
    {
      m_listCtrl->SetColumnVisible (col, visible);
      break;
    }
  }
  UpdateFileList ();
}


void
RapidSvnFrame::OnFlatView (wxCommandEvent & event)
{
  bool newFlatMode = !m_folder_browser->IsFlat ();

  // if this cannot be set (e.g. invalid selecttion
  // like the root element, we uncheck this
  if (!m_folder_browser->SetFlat (newFlatMode))
    newFlatMode = false;

  m->CheckMenu (ID_Flat, newFlatMode);
  m_listCtrl->SetFlat (newFlatMode);
    
  UpdateFileList ();
}


void
RapidSvnFrame::OnRefreshWithUpdate (wxCommandEvent & WXUNUSED (event))
{
  bool checked = m->IsMenuChecked (ID_RefreshWithUpdate);
  m_listCtrl->SetWithUpdate (checked);
  UpdateFolderBrowser ();
}


void
RapidSvnFrame::OnShowUnversioned (wxCommandEvent & WXUNUSED (event))
{
  bool checked = m->IsMenuChecked (ID_ShowUnversioned);
  m_listCtrl->SetShowUnversioned (checked);
  UpdateFolderBrowser ();
}

void
RapidSvnFrame::OnIgnoreExternals (wxCommandEvent & WXUNUSED (event))
{
  if (svn::SUPPORTS_EXTERNALS)
  {
    bool checked = m->IsMenuChecked (ID_IgnoreExternals);
    m_listCtrl->SetIgnoreExternals (checked);
  }
  UpdateFolderBrowser ();
}

void
RapidSvnFrame::OnColumnReset (wxCommandEvent &)
{
  m_listCtrl->ResetColumns ();
  for (int col = 0; col < FileListCtrl::COL_COUNT; col++)
  {
    bool visible = m_listCtrl->GetColumnVisible (col);
    int id = COLUMN_ID_MAP [col];
    if (id != -1)
    {
      m->CheckColumn (id, visible);
    }
  }
  UpdateFileList ();
}

void
RapidSvnFrame::UpdateFolderBrowser ()
{
  wxBusyCursor busy;

  m_currentPath.Clear ();
  UpdateFileList ();

  m->SetRunning (true);

  try
  {
    m->dontUpdateFilelist = true;

    if (m_folder_browser)
      m_folder_browser->Refresh ();
  }
  catch (...)
  {
  }

  m->SetRunning (false);

  m->dontUpdateFilelist = false;
  UpdateFileList ();
}

void
RapidSvnFrame::Trace (wxString msg)
{
  if (m_log != NULL)
  {
    msg += wxT('\n');
    m_log->AppendText (msg);
  }
}

void
RapidSvnFrame::OnLogin (wxCommandEvent & event)
{
  svn::Context * context = m_folder_browser->GetContext ();

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
  svn::Context * context = m_folder_browser->GetContext ();

  if (context == 0)
    return;

  context->setLogin ("", "");
}

void
RapidSvnFrame::OnStop (wxCommandEvent & event)
{
  m->listener.cancel (true);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
