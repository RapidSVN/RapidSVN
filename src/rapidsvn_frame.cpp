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
#include "svncpp/exception.hpp"
#include "svncpp/targets.hpp"

// app
#include "ids.hpp"

#include "svn_file_info.hpp"

#include "checkout_action.hpp"
#include "import_action.hpp"
#include "update_action.hpp"
#include "add_action.hpp"
#include "delete_action.hpp"
#include "commit_action.hpp"
#include "revert_action.hpp"
#include "resolve_action.hpp"
#include "copy_action.hpp"
#include "mkdir_action.hpp"
#include "merge_action.hpp"
#include "property_action.hpp"
#include "cleanup_action.hpp"
#include "external_program_action.hpp"

#include "report_dlg.hpp"
#include "preferences_dlg.hpp"

#include "rapidsvn_frame.hpp"
#include "svn_version.h"
#include "version.hpp"

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
const static char ConfigProjectFmt[] = "/Workbench/Project%d";

const static char TraceMisc[] = "tracemisc";

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
    wxMenuItem *pItem;

    menuFile->Append (ID_AddProject, _("&Add to Workbench..."));
    menuFile->Append (ID_RemoveProject, _("&Remove from Workbench..."));
    menuFile->AppendSeparator ();
    menuFile->Append (ID_Quit, _("E&xit"));

    // Columns menu
    MenuColumns = new wxMenu;
    MenuColumns->Append (ID_Column_Reset, _T("Reset Columns"));

    MenuColumns->AppendSeparator ();

    for (int col = 0; col <= FileListCtrl::COL_COUNT; col++)
    {
      int id = COLUMN_ID_MAP[col];
      if (id != -1)
      {
        MenuColumns->AppendCheckItem (id, COLUMN_CAPTIONS[col]);
      }
    }

    // View menu
    wxMenu *menuView = new wxMenu;
    pItem = new wxMenuItem (menuView, ID_Refresh, _("Refresh        F5"));
    pItem->SetBitmap (wxBITMAP (refresh));
    menuView->Append (pItem);

    menuView->AppendSeparator ();

    menuView->Append (0, _("Columns"), MenuColumns);

    menuView->AppendSeparator ();
    
    pItem = new wxMenuItem (menuView, ID_Preferences, _("Preferences"));
    menuView->Append (pItem);

    // Create menu
    wxMenu *menuCreate = new wxMenu;
    menuCreate->Append (ID_Import, _("&Import an unversioned file or tree ..."));
    menuCreate->Append (ID_Checkout, _("&Checkout working copy ..."));

    menuCreate->AppendSeparator ();

    menuCreate->Append (ID_Mkdir, _("Make a new directory ..."));
    menuCreate->Append (ID_Copy, _("C&opy remembering history ..."));

    menuCreate->AppendSeparator ();

    menuCreate->Append (ID_Merge, _("Merge differences"));
    menuCreate->Append (ID_Switch, _("Switch to URL ..."));

    // Modify menu
    wxMenu *menuModif = new wxMenu;
    pItem = new wxMenuItem (menuModif, ID_Update, _("Update"));
    pItem->SetBitmap (wxBITMAP (update));
    menuModif->Append (pItem);

    pItem = new wxMenuItem (menuModif, ID_Commit, _("Commit"));
    pItem->SetBitmap (wxBITMAP (commit));
    menuModif->Append (pItem);

    menuModif->AppendSeparator ();

    pItem = new wxMenuItem (menuModif, ID_Add, _("Add"));
    pItem->SetBitmap (wxBITMAP (add));
    menuModif->Append (pItem);

    pItem = new wxMenuItem (menuModif, ID_Delete, _("Delete"));
    pItem->SetBitmap (wxBITMAP (delete));
    menuModif->Append (pItem);

    menuModif->AppendSeparator ();

    pItem = new wxMenuItem (menuModif, ID_Revert, _("Revert"));
    pItem->SetBitmap (wxBITMAP (revert));
    menuModif->Append (pItem);

    pItem = new wxMenuItem (menuModif, ID_Resolve, _("Resolve conflicts"));
    pItem->SetBitmap (wxBITMAP (resolve));
    menuModif->Append (pItem);

    // Copy and rename menu
    menuModif->AppendSeparator ();

    pItem = new wxMenuItem (menuModif, ID_CopyTo, _("Copy"));
    //pItem->SetBitmap (wxBITMAP (copy));
    menuModif->Append (pItem);

    pItem = new wxMenuItem (menuModif, ID_MoveTo, _("Move"));
    //pItem->SetBitmap (wxBITMAP (rename));
    menuModif->Append (pItem);

    pItem = new wxMenuItem (menuModif, ID_RenameHere, _("Rename"));
    //pItem->SetBitmap (wxBITMAP (rename));
    menuModif->Append (pItem);

    // Query menu
    wxMenu *menuQuery = new wxMenu;
    pItem = new wxMenuItem (menuQuery, ID_Log, _("Log"));
    pItem->SetBitmap (wxBITMAP (log));
    menuQuery->Append (pItem);

    pItem = new wxMenuItem (menuQuery, ID_Info, _("Info"));
    pItem->SetBitmap (wxBITMAP (info));
    menuQuery->Append (pItem);

    pItem = new wxMenuItem (menuQuery, ID_Property, _("Properties"));
    pItem->SetBitmap (wxBITMAP (info));
    menuQuery->Append (pItem);

    // Extras menu
    wxMenu *menuExtras = new wxMenu;
    pItem = new wxMenuItem (menuExtras, ID_Cleanup, _("Cleanup"));
    menuExtras->Append (pItem);

    // Help Menu
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append (ID_Contents, _("&Contents"));
    menuHelp->AppendSeparator ();
    menuHelp->Append (ID_About, _("&About..."));

    // Create the menu bar and append the menus
    MenuBar = new wxMenuBar;
    MenuBar->Append (menuFile, _("&File"));
    MenuBar->Append (menuView, _("&View"));
    MenuBar->Append (menuCreate, _("&Create"));
    MenuBar->Append (menuModif, _("&Modify"));
    MenuBar->Append (menuQuery, _("&Query"));
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

};

BEGIN_EVENT_TABLE (RapidSvnFrame, wxFrame)
  EVT_SIZE (RapidSvnFrame::OnSize)
  EVT_MENU (ID_AddProject, RapidSvnFrame::OnAddProject)
  EVT_MENU (ID_RemoveProject, RapidSvnFrame::OnRemoveProject)
  EVT_MENU (ID_Quit, RapidSvnFrame::OnQuit)
  EVT_MENU (ID_About, RapidSvnFrame::OnAbout)
  EVT_MENU (ID_Info, RapidSvnFrame::OnInfo)

  EVT_MENU (ID_Contents, RapidSvnFrame::OnContents)
  EVT_MENU (ID_Preferences, RapidSvnFrame::OnPreferences)
  EVT_MENU (ID_Refresh, RapidSvnFrame::OnRefresh)
  EVT_MENU (ID_Column_Reset, RapidSvnFrame::OnColumnReset)

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
  // apr stuff
  apr_initialize ();
  m_folder_browser = NULL;
  m_listCtrl = NULL;
  m_title = title;
  m_actionWorker = new SimpleWorker (this);
  m_context = NULL;
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

  m_horiz_splitter = new wxSplitterWindow (this,
                                           SPLITTER_WINDOW,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxSP_FULLSASH | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);


  m_info_panel = new InfoPanel (m_horiz_splitter);
  m_log = new wxTextCtrl (m_horiz_splitter,
                          -1,
                          _(""),
                          wxPoint (0, 0),
                          wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

  // as much as the widget can stand
  m_log->SetMaxLength (0);

  m_logTracer = new EventTracer (this);


  m_vert_splitter = new wxSplitterWindow (m_info_panel,
                                          SPLITTER_WINDOW,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxSP_FULLSASH | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);

  // Create the list control to display files
  m_listCtrl = new FileListCtrl (m_vert_splitter, FILELIST_CTRL, 
                                 wxDefaultPosition, wxDefaultSize);

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

  PreferencesDlg::Data.Read (pConfig);
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


  // Save the workbench contents
  wxString key;
  size_t item;
  UniqueArrayString & workbenchItems = m_folder_browser->GetWorkbenchItems ();
  const size_t itemCount = workbenchItems.GetCount ();
  for (item = 0; item < itemCount; item++)
  {
    key.Printf (_(ConfigProjectFmt), item);
    pConfig->Write (key, workbenchItems.Item (item));
  }

  PreferencesDlg::Data.Write (pConfig);

  delete m;
}

void
RapidSvnFrame::UpdateFileList ()
{
  if (m_listCtrl && m_folder_browser)
  {
    if (m_currentPath.length () > 0)
    {
      try
      {
        m_listCtrl->UpdateFileList (m_currentPath);
        m_listCtrl->Show (TRUE);
      }
      catch (svn::Exception & e)
      {
        m_logTracer->Trace (e.message ());
        // probably unversioned resource
        m_listCtrl->Show (FALSE);
      }
      catch (...)
      {
        m_logTracer->Trace (_("Exception when updating filelist"));
      }
    }
    else
    {
      m_listCtrl->Show (FALSE);
    }
  }
}

void
RapidSvnFrame::OnAddProject (wxCommandEvent & event)
{
  AddProject ();
}

void
RapidSvnFrame::OnRemoveProject (wxCommandEvent & event)
{
  RemoveProject ();
}

void
RapidSvnFrame::OnQuit (wxCommandEvent & WXUNUSED (event))
{
  Close (TRUE);
}

void
RapidSvnFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  wxString msg;

  msg.Printf (_("%s Version %d.%d.%d\n"
                "Milestone: %s\n"
                "\n%s\n\n"
                "For more information see:\n"
                "http://rapidsvn.tigris.org\n"
                "\n"
                "\nBuilt with:\n"
                "Subversion %d.%d.%d\n"
                "wxWindows %d.%d.%d"),
              APPLICATION_NAME,
              RAPIDSVN_VER_MAJOR, RAPIDSVN_VER_MINOR, RAPIDSVN_VER_MICRO,
              RAPIDSVN_VER_MILESTONE,
              RAPIDSVN_COPYRIGHT,
              SVN_VER_MAJOR, SVN_VER_MINOR, SVN_VER_MICRO,
              wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);

  wxString title;
  title.Printf (_("About %s"), APPLICATION_NAME);
  wxMessageBox (msg, title, wxOK | wxICON_INFORMATION);
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
  Preferences ();
}

void
RapidSvnFrame::OnRefresh (wxCommandEvent & WXUNUSED (event))
{
  if (m_folder_browser)
  {
    m_folder_browser->Refresh ();
  }
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
RapidSvnFrame::AddProject ()
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
                    "administrative directory to the workbench!"),
                  APPLICATION_NAME, wxOK);
    return;
  }

  // add
  m_folder_browser->AddProject (dialog.GetPath ());
  m_folder_browser->Refresh ();

  wxLogStatus (_("Added project to workbench  '%s'"),
               dialog.GetPath ().c_str ());
}

void
RapidSvnFrame::RemoveProject ()
{
  wxASSERT (m_folder_browser);
  if( m_folder_browser->RemoveProject() )
  {
    wxLogStatus (_("Removed project from workbench"));
  }
}

void
RapidSvnFrame::InitFolderBrowser ()
{
  wxConfigBase *pConfig = wxConfigBase::Get ();

  wxASSERT (m_folder_browser);

  int i;
  wxString key;
  wxString val;
  UniqueArrayString & workbenchItems = 
    m_folder_browser->GetWorkbenchItems ();

  for (i = 0;; i++)
  {
    key.Printf (ConfigProjectFmt, i);
    if (pConfig->Read (key, &val, ""))
    {
      workbenchItems.Add (val);
    }

    else
      break;
  }

  m_folder_browser->Refresh ();
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
  ActionType lastAction = ACTION_TYPE_NONE;
  Action* action = NULL;

  if ((event.m_id >= ID_Verb_Min) && (event.m_id <= ID_Verb_Max))
  {
    action = new ExternalProgramAction (this, event.m_id - ID_Verb_Min, false);
    lastAction = ACTION_TYPE_EXTERNAL_PROGRAM;
  }
  else
  {
    switch (event.m_id)
    {
    case ID_Explore:
      action = new ExternalProgramAction (this, -1, true);
      lastAction = ACTION_TYPE_EXPLORE;
      break;

    case ID_Default_Action:
      InvokeDefaultAction();
      break;

    case ID_Update:
      action = new UpdateAction(this);
      lastAction = ACTION_TYPE_UPDATE;
      break;

    case ID_Commit:
      action = new CommitAction(this);
      lastAction = ACTION_TYPE_COMMIT;
      break;

    case ID_Add:
      action = new AddAction (this);
      lastAction = ACTION_TYPE_ADD;
      break;

    case ID_Import:
      action = new ImportAction (this);
      lastAction = ACTION_TYPE_IMPORT;
      break;

    case ID_Checkout:
      action = new CheckoutAction (this);
      lastAction = ACTION_TYPE_CHECKOUT;
      break;

    case ID_Cleanup:
      action = new CleanupAction (this);
      lastAction = ACTION_TYPE_CLEANUP;
      break;

    case ID_Log:
      action = new LogAction (this);
      lastAction = ACTION_TYPE_LOG;
      break;

    case ID_Revert:
      action = new RevertAction (this);
      lastAction = ACTION_TYPE_REVERT;
      break;

    case ID_Delete:
      action = new DeleteAction (this);
      lastAction = ACTION_TYPE_DELETE;
      break;

    case ID_Copy:
      //TODO Make sure there is only one file selected
      action = new CopyAction (this);
      lastAction = ACTION_TYPE_COPY;
      break;

    case ID_Mkdir:
      action = new MkdirAction (this);
      lastAction = ACTION_TYPE_MKDIR;
      break;

    case ID_Merge:
      action = new MergeAction (this);
      lastAction = ACTION_TYPE_MERGE;
      break;

    case ID_Property:
      action = new PropertyAction (this);
      lastAction = ACTION_TYPE_PROPERTY;

    case ID_Contents: //TODO
    case ID_Rename: //TODO
    default:
      m_logTracer->Trace ("Unimplemented action!");
      break;
    }
  }

  if (action)
    Perform (lastAction, action);
}

void
RapidSvnFrame::OnActionEvent (wxCommandEvent & event)
{
  switch (event.GetInt ())
  {
  case TOKEN_INFO:
  {
    m_log->AppendText (event.GetString () + "\n");
  }
  break;

  case TOKEN_SVN_INTERNAL_ERROR:
  {
    if (event.GetClientData ())
    {
      ErrorTracer err_tr (this);

      handle_svn_error ((svn_error_t *) event.GetClientData (), &err_tr);
      err_tr.ShowErrors ();
    }
  }
  break;

  case TOKEN_INTERNAL_ERROR:
  {
    ErrorTracer err_tr (this);

    err_tr.Trace (event.GetString ());
    err_tr.ShowErrors ();
  }
  break;

  case TOKEN_ACTION_START:
  {
    m_log->AppendText (event.GetString () + "\n");
    wxLogStatus (event.GetString ());
  }
  break;

  case TOKEN_ACTION_END:
  {
    UpdateFileList ();
    m_log->AppendText ( _("Ready\n"));
  }
  break;
  }
}

void
RapidSvnFrame::ShowInfo ()
{
  IndexArray arr = m_listCtrl->GetSelectedItems ();
  size_t i;
  wxString path = m_folder_browser->GetPath ();
  wxString all_info;
  wxString info;
  wxString _path;
  svn::Pool subPool;
  svn_error_t *err = NULL;
  bool wasError = false;

  for (i = 0; i < arr.GetCount (); i++)
  {
    wxFileName fname (path, m_listCtrl->GetItemText (arr[i]));
    _path = fname.GetFullPath ();
    err = svn_get_file_info (UnixPath (_path), subPool.pool(), info);

    if (err)
    {
      all_info.Empty ();

      StringTracer ertr (all_info);
      handle_svn_error (err, &ertr);
      wasError = true;
      break;
    }
    all_info += info + "\n";
  }

  {
    int rep_type = NORMAL_REPORT;
    wxString caption = _("Info");
    if (wasError)
    {
      rep_type = ERROR_REPORT;
      caption = _("Info error");
    }

    ReportDlg rdlg (this, caption, all_info, rep_type);
    rdlg.ShowModal ();
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
  case WXK_F5:
    m_activePane = ACTIVEPANE_FOLDER_BROWSER;
    UpdateFileList ();
    break;

  case WXK_F2:
    ProcessCommand (ID_Explore);
    break;

  case WXK_RETURN:
    ProcessCommand (ID_Default_Action);
    break;

  default:
    event.Skip ();
    break;
  }
}

void
RapidSvnFrame::Preferences ()
{
  PreferencesDlg *pDlg = PreferencesDlg::CreateInstance (this);
  pDlg->ShowModal ();
  pDlg->Close (TRUE);
}

void
RapidSvnFrame::UpdateCurrentPath ()
{
  if( m_folder_browser)
  {
    m_currentPath = m_folder_browser->GetPath ();
  }

  SetTitle (m_title + ": " + m_currentPath);
}

bool
RapidSvnFrame::InvokeDefaultAction ()
{
  size_t folder_count = 0, file_count = 0;
  std::vector<svn::Path> targets = GetActionTargets ().targets ();

  if (targets.empty())
    return false;
  
  for (std::vector<svn::Path>::iterator i = targets.begin (); i != targets.end (); i++)
  {
    wxFileName path (i->c_str());
    if (wxDirExists (path.GetFullPath ()))
      folder_count++;
    else
      file_count++;
  }

  // We can't decide
  if (folder_count && file_count)
    return false;

  if (folder_count)
  {
    // TODO: Select the target in the folder browser, then:
    /*
      UpdateCurrentPath ();
      UpdateFileList ();
    */
  }
  else
  {
    Perform (ACTION_TYPE_EXTERNAL_PROGRAM,
             new ExternalProgramAction (this, -1, false));
  }

  return true;
}

void
RapidSvnFrame::Perform (ActionType type, Action * action)
{
  m_lastAction = type;
  action->SetPath (m_currentPath.c_str ());
  action->SetContext (m_context);
  if (action->GetOptions () != actionWithoutTarget)
  {
    action->SetTargets (GetActionTargets ());
  }
  action->SetTracer (m_logTracer, false);
  m_actionWorker->SetTracer (m_logTracer);
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
  AppendText (str + _("\n"));
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
