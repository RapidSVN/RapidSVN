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
// wxwindows
#include "wx/confbase.h"
#include "wx/wx.h"
#include "wx/filename.h"

// svncpp
#include "svncpp/apr.hpp"
#include "svncpp/context.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/targets.hpp"

// app
#include "ids.hpp"

#include "file_info.hpp"

// actions
#include "add_action.hpp"
#include "checkout_action.hpp"
#include "cleanup_action.hpp"
#include "commit_action.hpp"
#include "delete_action.hpp"
#include "external_program_action.hpp"
#include "get_action.hpp"
#include "import_action.hpp"
#include "merge_action.hpp"
#include "mkdir_action.hpp"
#include "move_action.hpp"
#include "property_action.hpp"
#include "resolve_action.hpp"
#include "revert_action.hpp"
#include "switch_action.hpp"
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

// number of items initially in the list
static const int NUM_ITEMS = 30;

// List config keys here, to avoid duplicating literal text:
const static char ConfigLeft[] = "/MainFrame/Left";
const static char ConfigTop[] = "/MainFrame/Top";
const static char ConfigWidth[] = "/MainFrame/Width";
const static char ConfigHeight[] = "/MainFrame/Height";
const static char ConfigSplitterHoriz[] = "/MainFrame/SplitterHoriz";
const static char ConfigSplitterVert[] = "/MainFrame/SplitterVert";
const static char ConfigBookmarkFmt[] = "/Bookmarks/Bookmark%d";
const static char ConfigBookmarkCount[] = "/Bookmarks/Count";

const static char TraceMisc[] = "tracemisc";

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
  ID_Column_Text_Time,
  ID_Column_Prop_Time,
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
 * Captions for the columns. We cant use the texts used for
 * the column headings in FileListCtrl since they dont have
 * shortcuts.
 */
static const char *
COLUMN_CAPTIONS[FileListCtrl::COL_COUNT] =
{
  "", // Name is not used here
  "", // Path is not used here
  _("&Revision"),
  _("R&ep. Rev."),
  _("&Author"),
  _("&Status"),
  _("&Prop Status"),
  _("&Last Changed"),
  _("&Date"),
  _("Pr&op Date"),
  _("Checksum"),
  _("Url"),
  _("Repository"),
  _("UUID"),
  _("Schedule"),
  _("Copied"),
  _("Conflict Old"),
  _("Conflict New"),
  _("Conflict Work")
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

  /** 
   * This instance of @a apr is used to initialize/terminate apr 
   */
  svn::Apr apr;

  Data ()
    : MenuColumns (0), MenuBar (0)
  {
    InitializeMenu ();
  }

  void
  RapidSvnFrame::Data::InitializeMenu ()
  {
    // File menu
    wxMenu *menuFile = new wxMenu;
    //menuFile->AppendSeparator ();
    AppendMenuItem (*menuFile, ID_Quit);

    // Columns menu
    MenuColumns = new wxMenu;
    AppendMenuItem (*MenuColumns, ID_Column_Reset);
    MenuColumns->AppendSeparator ();

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
    menuView->AppendSeparator ();

    AppendMenuItem (*menuView, ID_Explore);
    menuView->AppendSeparator ();

    menuView->Append (0, _("Columns"), MenuColumns);
    menuView->AppendCheckItem (ID_Flat, _("Flat Mode"));

    menuView->AppendSeparator ();
    
    AppendMenuItem (*menuView, ID_Preferences);

    // Repository menu
    wxMenu *menuRepos = new wxMenu;
    menuRepos->Append (ID_Import, _("&Import..."));
    menuRepos->Append (ID_Checkout, _("&Checkout..."));

    menuRepos->AppendSeparator ();

    menuRepos->Append (ID_Merge, _("Merge..."));
    menuRepos->Append (ID_Switch, _("Switch..."));

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
    //dont lie about not existant help!
    //menuHelp->Append (ID_Contents, _("&Contents"));
    //menuHelp->AppendSeparator ();
    menuHelp->Append (ID_About, _("&About..."));

    // Create the menu bar and append the menus
    MenuBar = new wxMenuBar;
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

};

BEGIN_EVENT_TABLE (RapidSvnFrame, wxFrame)
  EVT_SIZE (RapidSvnFrame::OnSize)
  EVT_MENU (ID_AddWcBookmark, RapidSvnFrame::OnAddWcBookmark)
  EVT_MENU (ID_AddRepoBookmark, RapidSvnFrame::OnAddRepoBookmark)
  EVT_MENU (ID_RemoveBookmark, RapidSvnFrame::OnRemoveBookmark)
  EVT_MENU (ID_Quit, RapidSvnFrame::OnQuit)
  EVT_MENU (ID_About, RapidSvnFrame::OnAbout)
  EVT_MENU (ID_Info, RapidSvnFrame::OnInfo)

  EVT_MENU (ID_Contents, RapidSvnFrame::OnContents)
  EVT_MENU (ID_Preferences, RapidSvnFrame::OnPreferences)
  EVT_MENU (ID_Refresh, RapidSvnFrame::OnRefresh)
  EVT_MENU (ID_Column_Reset, RapidSvnFrame::OnColumnReset)
  EVT_MENU (ID_Flat, RapidSvnFrame::OnFlatView)
  EVT_MENU (ID_Login, RapidSvnFrame::OnLogin)
  EVT_MENU (ID_Logout, RapidSvnFrame::OnLogout)

  EVT_MENU_RANGE (ID_File_Min, ID_File_Max, RapidSvnFrame::OnFileCommand)
  EVT_MENU_RANGE (ID_Verb_Min, ID_Verb_Max, RapidSvnFrame::OnFileCommand)
  EVT_MENU_RANGE (ID_Column_Min, ID_Column_Max, RapidSvnFrame::OnColumn)

  EVT_MENU (ACTION_EVENT, RapidSvnFrame::OnActionEvent)

  EVT_TOOL_ENTER (ID_TOOLBAR, RapidSvnFrame::OnToolEnter)
  EVT_TREE_SEL_CHANGED (-1, RapidSvnFrame::OnFolderBrowserSelChanged)
  EVT_TREE_KEY_DOWN (-1, RapidSvnFrame::OnFolderBrowserKeyDown) 
  EVT_LIST_ITEM_SELECTED (-1, RapidSvnFrame::OnFileListSelected)
END_EVENT_TABLE ()

/** class implementation **/
  RapidSvnFrame::RapidSvnFrame (const wxString & title)
    : wxFrame ((wxFrame *) NULL, -1, title, wxDefaultPosition, wxDefaultSize, 
               wxDEFAULT_FRAME_STYLE)
{
  m = new Data ();
  m_folder_browser = NULL;
  m_listCtrl = NULL;
  m_title = title;
  m_actionWorker = new SimpleWorker (this);
  m_context = 0;
  m_activePane = ACTIVEPANE_FOLDER_BROWSER;

  // enable trace
  wxLog::AddTraceMask (TraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *pConfig = wxConfigBase::Get ();

  SetIcon (wxIcon (svn_xpm));

  // Toolbar
  m_tbar = NULL;

  // toolbar rows
  m_toolbar_rows = 1;

  SetMenuBar (m->MenuBar);
  CreateStatusBar ();

  // Create the toolbar
  RecreateToolbar ();

  m_horiz_splitter = 
    new ProportionalSplitterWindow (1.0f, this,
                                    SPLITTER_WINDOW,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    SPLITTER_STYLE);


  m_info_panel = new InfoPanel (m_horiz_splitter);
  m_log = new wxTextCtrl (m_horiz_splitter,
                          -1,
                          "",
                          wxPoint (0, 0),
                          wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

  // as much as the widget can stand
  m_log->SetMaxLength (0);

  m_logTracer = new EventTracer (this);


  m_vert_splitter = new wxSplitterWindow (m_info_panel,
                                          SPLITTER_WINDOW,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          SPLITTER_STYLE);

  // Create the list control to display files
  m_listCtrl = new FileListCtrl (m_vert_splitter, FILELIST_CTRL, 
                                 wxDefaultPosition, wxDefaultSize);

  // Create the browse control
  m_folder_browser = new FolderBrowser (m_vert_splitter, FOLDER_BROWSER);
  {
    Preferences prefs;
    m_folder_browser->SetAuthPerBookmark (prefs.authPerBookmark);
  }

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
  int x = pConfig->Read (ConfigLeft, 50);
  int y = pConfig->Read (ConfigTop, 50);
  int w = pConfig->Read (ConfigWidth, 806);
  int h = pConfig->Read (ConfigHeight, 480);

  Move (x, y);
  SetClientSize (w, h);

  // Get sash position for every splitter from configuration.
  int vpos = pConfig->Read (ConfigSplitterVert, w / 3);
  int hpos = pConfig->Read (ConfigSplitterHoriz, (3 * h) / 4);

  // Set sash position for every splitter.
  // Note: to not revert the order of Split calls, as the panels will be messed up.
  m_horiz_splitter->SplitHorizontally (m_info_panel, m_log, hpos);
  m_vert_splitter->SplitVertically (m_folder_browser, m_listCtrl, vpos);

  // put the working copy selections in the combo browser
  InitFolderBrowser ();
}

RapidSvnFrame::~RapidSvnFrame ()
{
  wxConfigBase *pConfig = wxConfigBase::Get ();
  if (pConfig == NULL)
    return;

  if (m_logTracer)
    delete m_logTracer;

  if (m_actionWorker)
    delete m_actionWorker;

  // Save frame size and position.

  int x, y;
  int w, h;

  GetClientSize (&w, &h);
  GetPosition (&x, &y);

  pConfig->Write (ConfigLeft, (long) x);
  pConfig->Write (ConfigTop, (long) y);
  pConfig->Write (ConfigWidth, (long) w);
  pConfig->Write (ConfigHeight, (long) h);


  // Save splitter positions
  pConfig->Write (ConfigSplitterVert,
                  (long) m_vert_splitter->GetSashPosition ());
  pConfig->Write (ConfigSplitterHoriz,
                  (long) m_horiz_splitter->GetSashPosition ());


  // Save the bookmarks contents
  size_t item;
  const size_t itemCount = m_folder_browser->GetBookmarkCount ();
  pConfig->Write (ConfigBookmarkCount, (long)itemCount);
  for (item = 0; item < itemCount; item++)
  {
    wxString key;
    key.Printf (ConfigBookmarkFmt, item);

    const char * bookmark = m_folder_browser->GetBookmark (item);

    pConfig->Write (key, bookmark);
  }

  delete m;
}

void
RapidSvnFrame::UpdateFileList ()
{
  wxBusyCursor busy;
  if (m_listCtrl && m_folder_browser)
  {
    if (m_currentPath.length () > 0)
    {
      try
      {
        m_listCtrl->UpdateFileList (m_currentPath);
        m_listCtrl->Show (TRUE);
      }
      catch (svn::ClientException & e)
      {
        wxString msg;
        msg.Printf (_("Error while updating filelist (%s)"),
                    e.message ());
        Trace (msg);

        // probably unversioned resource
        m_listCtrl->Show (FALSE);
      }
      catch (...)
      {
        Trace (_("Error while updating filelist"));
      }
    }
    else
    {
      m_listCtrl->Show (FALSE);
    }
  }
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
RapidSvnFrame::OnQuit (wxCommandEvent & WXUNUSED (event))
{
  Close (TRUE);
}

void
RapidSvnFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  AboutDlg dlg (this);

  dlg.ShowModal ();
}

void
RapidSvnFrame::OnInfo (wxCommandEvent & WXUNUSED (event))
{
  ShowInfo ();
}

void
RapidSvnFrame::OnContents (wxCommandEvent & WXUNUSED (event))
{
  //TODO 
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
  UpdateFileList ();
}

void
RapidSvnFrame::LayoutChildren ()
{
  wxSize size = GetClientSize ();

  int offset;
  if (m_tbar)
  {
    m_tbar->SetSize (-1, size.y);
    m_tbar->Move (0, 0);

    offset = m_tbar->GetSize ().x;
  }
  else
  {
    offset = 0;
  }
}

void
RapidSvnFrame::OnSize (wxSizeEvent & event)
{
  if (m_tbar)
  {
    LayoutChildren ();
  }
  else
  {
    event.Skip ();
  }
}

void
RapidSvnFrame::RecreateToolbar ()
{
  // delete the old toolbar
  wxToolBarBase *toolBar = GetToolBar ();
  delete toolBar;

  SetToolBar (NULL);

  long style = wxNO_BORDER | wxTB_FLAT | wxTB_DOCKABLE;
  style |= wxTB_HORIZONTAL;

  toolBar = CreateToolBar (style, ID_TOOLBAR);
  toolBar->SetMargins (4, 4);

  AddActionTools ();
  AddInfoTools ();

  // Set toolbar refresh button.
  toolBar->AddTool (ID_Refresh,
                    wxBITMAP (refresh),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL, 
                    _("Refresh"), 
                    _("Refresh the file list"));

  toolBar->AddSeparator ();

  // After adding the buttons to the toolbar, 
  // must call Realize() to reflect the changes.  
  toolBar->Realize ();

  toolBar->SetRows (m_toolbar_rows);
}

void
RapidSvnFrame::AddActionTools ()
{
  wxToolBarBase *toolBar = GetToolBar ();

  toolBar->AddTool (ID_Add,
                    wxBITMAP (add),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Add selected"),
                    _("Put files and directories under revision control"));

  toolBar->AddTool (ID_Delete,
                    wxBITMAP (delete),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Delete selected"),
                    _("Delete files and directories from version control"));

  toolBar->AddTool (ID_Update,
                    wxBITMAP (update),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Update selected"),
                    _("Bring changes from the repository into the working copy"));

  toolBar->AddTool (ID_Commit,
                    wxBITMAP (commit),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Commit selected"),
                    _("Send changes from your working copy to the repository"));

  toolBar->AddTool (ID_Revert,
                    wxBITMAP (revert),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Revert selected"),
                    _("Restore pristine working copy file (undo all local edits)"));

  toolBar->AddTool (ID_Resolve,
                    wxBITMAP (resolve),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Resolve selected"),
                    _("Remove 'conflicted' state on working copy files or directories)"));

  toolBar->AddSeparator ();
}

void
RapidSvnFrame::AddInfoTools ()
{
  wxToolBarBase *toolBar = GetToolBar ();

  toolBar->AddTool (ID_Info,
                    wxBITMAP (info),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Info selected"),
                    _("Display info about selected entries"));

  toolBar->AddTool (ID_Log,
                    wxBITMAP (log),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _("Log selected"),
                    _("Show the log messages for a set entries"));

  toolBar->AddSeparator ();
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
  wxFileName fileName (dialog.GetPath ());
  if ((fileName.GetName () + fileName.GetExt ()) == SVN_WC_ADM_DIR_NAME)
  {
    add = FALSE;
    wxMessageBox (_("You cannot add a subversion "
                    "administrative directory to the bookmarks!"),
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
RapidSvnFrame::InitFolderBrowser ()
{
  wxConfigBase *pConfig = wxConfigBase::Get ();

  wxASSERT (m_folder_browser);

  wxString key;
  wxString bookmark;

  int item, count;
  pConfig->Read (ConfigBookmarkCount, &count, 0);
  for (item = 0; item < count; item++)
  {
    key.Printf (ConfigBookmarkFmt, item);
    if (pConfig->Read (key, &bookmark, ""))
    {
      m_folder_browser->AddBookmark (bookmark);
    }

    else
      break;
  }

  UpdateFolderBrowser ();
}

const svn::Targets
RapidSvnFrame::GetActionTargets () const
{
  //is there nothing selected in the list control, 
  //or is the active window *not* the list control?
  if (m_listCtrl->GetSelectedItemCount () <= 0 || 
      m_activePane != ACTIVEPANE_FILELIST)
  {
    //yes, so build the file list from the folder browser
    wxFileName fname (m_folder_browser->GetPath ());

    wxString path = fname.GetFullPath ();
    return svn::Targets (path.c_str ());
  }
  else
  {
    //no, build the file list from the list control
    return m_listCtrl->GetTargets ();
  }
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

    case ID_Import:
      action = new ImportAction (this);
      break;

    case ID_Checkout:
      action = new CheckoutAction (this);
      break;

    case ID_Cleanup:
      action = new CleanupAction (this);
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

    case ID_Contents: //TODO
    default:
      m_logTracer->Trace ("Unimplemented action!");
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
  {
    Trace (event.GetString ());
  }
  break;

  case TOKEN_SVN_INTERNAL_ERROR:
  case TOKEN_INTERNAL_ERROR:
  {
    Trace (event.GetString ());
    UpdateFileList ();
    Trace (_("Ready\n"));
  }
  break;

  case TOKEN_ACTION_START:
  {
    Trace (event.GetString ());
    wxLogStatus (event.GetString ());
  }
  break;

  case TOKEN_ACTION_END:
  {
    UpdateFileList ();
    Trace (_("Ready\n"));
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
    const char * bookmark = event.GetString ().c_str ();

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
  }
}

void
RapidSvnFrame::ShowInfo ()
{
  FileInfo fileInfo (m_context);

  try
  {
    svn::Targets targets = m_listCtrl->GetTargets ();
    std::vector<svn::Path> vector = targets.targets ();
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
    Trace (e.message ());
    return;
  }

}

void
RapidSvnFrame::OnFolderBrowserSelChanged (wxTreeEvent & event)
{
  m_activePane = ACTIVEPANE_FOLDER_BROWSER;
  UpdateCurrentPath ();
  UpdateFileList ();
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
    m_folder_browser->SetAuthPerBookmark (prefs.authPerBookmark);
  }
}

void
RapidSvnFrame::UpdateCurrentPath ()
{
  if (m_folder_browser == 0)
  {
    m_currentPath = "";
    m_context = 0;
  }
  else
  {
    m_currentPath = m_folder_browser->GetPath ();
    m_context = m_folder_browser->GetContext ();
  }

  SetTitle (m_title + ": " + m_currentPath);
}

bool
RapidSvnFrame::InvokeDefaultAction ()
{
  size_t folder_count = 0, file_count = 0;
  std::vector<svn::Path> targets = GetActionTargets ().targets ();

  // the default action will be invoked only for a single file
  // or folder.
  // if more or less than one file or folder is selected nothing 
  // will happen.

  if (targets.size () != 1)
    return false;

  wxFileName path (targets[0].c_str ());
  wxString fullPath = path.GetFullPath ();
  if (wxDirExists (fullPath))
    folder_count++;
  else
    file_count++;
  
  // We can't decide
  if (folder_count && file_count)
    return false;

  // We will do anything only if ONE file or folder is selected
  if (folder_count == 1)
  {
    // go one folder deeper...
    m_folder_browser->SelectFolder (fullPath.c_str ());
  }
  else if (file_count == 1)
  {
    Perform (new ExternalProgramAction (this, -1, false));
  }

  return true;
}

void
RapidSvnFrame::Perform (Action * action)
{
  action->SetPath (m_currentPath.c_str ());
  action->SetContext (m_context);
  if (action->GetOptions () != actionWithoutTarget)
  {
    action->SetTargets (GetActionTargets ());
  }
  action->SetTracer (m_logTracer, false);
  m_actionWorker->SetTracer (m_logTracer);
  m_actionWorker->SetContext (m_context, false);
  m_actionWorker->Perform (action);
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
  bool checked = m->IsMenuChecked (ID_Flat);
  //m->CheckMenu (ID_Flat, !checked);
  m_listCtrl->SetFlat (checked);
  UpdateFileList ();
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

  if (m_folder_browser)
  {
    m_folder_browser->Refresh ();
  }
}

void
RapidSvnFrame::Trace (const char *msg)
{
  if (m_log != 0)
  {
    wxString message;
    message.Printf ("%s\n", msg);
    m_log->AppendText (message);
  }
}

void 
RapidSvnFrame::OnLogin (wxCommandEvent & event)
{
  svn::Context * context = m_folder_browser->GetContext ();

  if (context == 0)
    return;

  AuthDlg dlg (this, context->getUsername ());
  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    context->setLogin (dlg.GetUsername (), dlg.GetPassword ());
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

InfoPanel::InfoPanel (wxWindow * parent)
  : wxPanel (parent, -1, wxDefaultPosition, wxDefaultSize, 
             wxTAB_TRAVERSAL | wxCLIP_CHILDREN)
{
}

LogTracer::LogTracer (wxWindow * parent)
  : wxTextCtrl (parent, -1, "", wxPoint (0, 0),
                wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)
{
  SetMaxLength (0);
}

void
LogTracer::Trace (const wxString & str)
{
  AppendText (str + "\n");
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
