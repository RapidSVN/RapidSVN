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
const static char *szBrowserPathKey = "/MainFrame/BrowserPath";

#define wxTraceMisc wxT("tracemisc")

// define this to 1 to use wxToolBarSimple instead of the native one
#define USE_GENERIC_TBAR 0

BEGIN_EVENT_TABLE (RapidSvnFrame, wxFrame)
  EVT_SIZE (RapidSvnFrame::OnSize)
  EVT_MENU (ID_AddProject, RapidSvnFrame::OnAddProject)
  EVT_MENU (ID_RemoveProject, RapidSvnFrame::OnRemoveProject)
  EVT_MENU (ID_Quit, RapidSvnFrame::OnQuit)
  EVT_MENU (ID_About, RapidSvnFrame::OnAbout)
  EVT_MENU (ID_Log, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Info, RapidSvnFrame::OnInfo)
  EVT_MENU (ID_Checkout, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Import, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Update, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Add, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Del, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Commit, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Revert, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Copy, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Rename, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Resolve, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Property, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Mkdir, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Merge, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Contents, RapidSvnFrame::OnContents)
  EVT_MENU (ID_Preferences, RapidSvnFrame::OnPreferences)
  EVT_MENU (ID_Refresh, RapidSvnFrame::OnRefresh)
  EVT_MENU (ID_MoveTo, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_CopyTo, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_RenameHere, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_CopyHere, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Cleanup, RapidSvnFrame::OnFileCommand)
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
  // apr stuff
  apr_initialize ();
  m_folder_browser = NULL;
  m_listCtrl = NULL;
  m_title = title;
  m_actionWorker = new SimpleWorker (this);
  m_context = NULL;
  m_activePane = ACTIVEPANE_FOLDER_BROWSER;

  // enable trace
  wxLog::AddTraceMask (wxTraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *pConfig = wxConfigBase::Get ();

  SetIcon (wxIcon (svn_xpm));

  // Toolbar
  m_tbar = NULL;

  // toolbar rows
  m_toolbar_rows = 1;

  InitializeMenu ();
  CreateStatusBar ();

  // Create the toolbar
  RecreateToolbar ();

  m_horiz_splitter = new wxSplitterWindow (this,
                                           SPLITTER_WINDOW,
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);


  m_info_panel = new InfoPanel (m_horiz_splitter);
  m_log = new wxTextCtrl (m_horiz_splitter,
                          -1,
                          _T (""),
                          wxPoint (0, 0),
                          wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

  // as much as the widget can stand
  m_log->SetMaxLength (0);

  m_logTracer = new EventTracer (this);


  m_vert_splitter = new wxSplitterWindow (m_info_panel,
                                          SPLITTER_WINDOW,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);

  // Create the list control to display files
  m_listCtrl = new FileListCtrl (m_vert_splitter, FILELIST_CTRL, 
                                 wxDefaultPosition, wxDefaultSize);

  // Set the current browse position:
  wxString BrowsePosition =
    pConfig->Read (szBrowserPathKey, wxDirDialogDefaultFolderStr);

  // Create the browse control
  m_folder_browser = new FolderBrowser (m_vert_splitter, FOLDER_BROWSER);

  UpdateFileList ();

  wxSizer *sizer = new wxBoxSizer (wxVERTICAL);
  sizer->Add (m_vert_splitter, 1, wxEXPAND);

  m_info_panel->SetAutoLayout (true);
  m_info_panel->SetSizer (sizer);


  int x, y;
  int w, h;

  pConfig->SetPath ("/MainFrame");

  // The second parameter is a default value in case the entry was not found.
  x = pConfig->Read ("posx", 50);
  y = pConfig->Read ("posy", 50);
  w = pConfig->Read ("width", 806);
  h = pConfig->Read ("height", 480);

  Move (x, y);
  SetClientSize (w, h);

  // Get sash position for every splitter from configuration.
  int vpos, hpos;
  vpos = pConfig->Read ("/MainFrame/vertsplitsashpos", w / 3);
  hpos = pConfig->Read ("/MainFrame/horizsplitsashpos", (3 * h) / 4);

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

  pConfig->Write ("/MainFrame/posx", (long) x);
  pConfig->Write ("/MainFrame/posy", (long) y);
  pConfig->Write ("/MainFrame/width", (long) w);
  pConfig->Write ("/MainFrame/height", (long) h);


  // Save splitters sash positions

  pConfig->Write ("/MainFrame/vertsplitsashpos",
                  (long) m_vert_splitter->GetSashPosition ());
  pConfig->Write ("/MainFrame/horizsplitsashpos",
                  (long) m_horiz_splitter->GetSashPosition ());


  // Save the current browse position:
  pConfig->Write (szBrowserPathKey, m_folder_browser->GetPath ());

  // Save the workbench contents

  wxString key;
  size_t item;
  UniqueArrayString & workbenchItems = m_folder_browser->GetWorkbenchItems ();
  const size_t itemCount = workbenchItems.GetCount ();
  for (item = 0; item < itemCount; item++)
  {
    key.Printf (_T ("/MainFrame/wc%d"), item);
    pConfig->Write (key, workbenchItems.Item (item));
  }

  PreferencesDlg::Data.Write (pConfig);
}

void
RapidSvnFrame::InitializeMenu ()
{
  // File menu
  wxMenu *menuFile = new wxMenu;
  wxMenuItem *pItem;

  menuFile->Append (ID_AddProject, ("&Add to Workbench..."));
  menuFile->Append (ID_RemoveProject, "&Remove from Workbench...");
  menuFile->AppendSeparator ();
  menuFile->Append (ID_Quit, "E&xit");

  // View menu
  wxMenu *menuView = new wxMenu;
  pItem = new wxMenuItem (menuView, ID_Refresh, _T ("Refresh        F5"));
  pItem->SetBitmap (wxBITMAP (refresh));
  menuView->Append (pItem);

  //menuView->AppendSeparator ();
  //
  //pItem = new wxMenuItem (menuView, ID_Preferences, _T ("Preferences"));
  //menuView->Append (pItem);

  // Create menu
  wxMenu *menuCreate = new wxMenu;
  menuCreate->Append (ID_Import, _T ("&Import an unversioned file or tree ..."));
  menuCreate->Append (ID_Checkout, _T ("&Checkout working copy ..."));

  menuCreate->AppendSeparator ();

  menuCreate->Append (ID_Mkdir, _T ("Make a new directory ..."));
  menuCreate->Append (ID_Copy, _T ("C&opy remembering history ..."));

  menuCreate->AppendSeparator ();

  menuCreate->Append (ID_Merge, _T ("Merge differences"));
  menuCreate->Append (ID_Switch, _T ("Switch to URL ..."));

  // Modify menu
  wxMenu *menuModif = new wxMenu;
  pItem = new wxMenuItem (menuModif, ID_Update, _T ("Update"));
  pItem->SetBitmap (wxBITMAP (update));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_Commit, _T ("Commit"));
  pItem->SetBitmap (wxBITMAP (commit));
  menuModif->Append (pItem);

  menuModif->AppendSeparator ();

  pItem = new wxMenuItem (menuModif, ID_Add, _T ("Add"));
  pItem->SetBitmap (wxBITMAP (add));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_Del, _T ("Remove"));
  pItem->SetBitmap (wxBITMAP (delete));
  menuModif->Append (pItem);

  menuModif->AppendSeparator ();

  pItem = new wxMenuItem (menuModif, ID_Revert, _T ("Revert"));
  pItem->SetBitmap (wxBITMAP (revert));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_Resolve, _T ("Resolve conflicts"));
  pItem->SetBitmap (wxBITMAP (resolve));
  menuModif->Append (pItem);

  // Copy and rename menu
  menuModif->AppendSeparator ();

  pItem = new wxMenuItem (menuModif, ID_CopyTo, _T ("Copy"));
  //pItem->SetBitmap (wxBITMAP (copy));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_MoveTo, _T ("Move"));
  //pItem->SetBitmap (wxBITMAP (rename));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_RenameHere, _T ("Rename"));
  //pItem->SetBitmap (wxBITMAP (rename));
  menuModif->Append (pItem);

  // Query menu
  wxMenu *menuQuery = new wxMenu;
  pItem = new wxMenuItem (menuQuery, ID_Log, _T ("Log"));
  pItem->SetBitmap (wxBITMAP (log));
  menuQuery->Append (pItem);

  pItem = new wxMenuItem (menuQuery, ID_Info, _T ("Info"));
  pItem->SetBitmap (wxBITMAP (info));
  menuQuery->Append (pItem);

  pItem = new wxMenuItem (menuQuery, ID_Property, _T ("Properties"));
  pItem->SetBitmap (wxBITMAP (info));
  menuQuery->Append (pItem);

  // Extras menu
  wxMenu *menuExtras = new wxMenu;
  pItem = new wxMenuItem (menuExtras, ID_Cleanup, _T ("Cleanup"));
  menuExtras->Append (pItem);

  // Help Menu
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append (ID_Contents, _T ("&Contents"));
  menuHelp->AppendSeparator ();
  menuHelp->Append (ID_About, _T ("&About..."));

  // Create the menu bar and append the menus
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append (menuFile, _T ("&File"));
  menuBar->Append (menuView, _T ("&View"));
  menuBar->Append (menuCreate, _T ("&Create"));
  menuBar->Append (menuModif, _T ("&Modify"));
  menuBar->Append (menuQuery, _T ("&Query"));
  menuBar->Append (menuExtras, _T ("&Extras"));
  menuBar->Append (menuHelp, _T ("&Help"));

  SetMenuBar (menuBar);
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
        m_logTracer->Trace (_T("Exception when updating filelist"));
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

  msg.Printf (_T ("%s Version %d.%d.%d\n"
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
  title.Printf (_T ("About %s"), APPLICATION_NAME);
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
                    _T ("Refresh"), 
                    _T ("Refresh the file list"));

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
                    _T ("Add selected"),
                    _T ("Put files and directories under revision control"));

  toolBar->AddTool (ID_Delete,
                    wxBITMAP (delete),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Remove selected"),
                    _T ("Remove files and directories from version control"));

  toolBar->AddTool (ID_Update,
                    wxBITMAP (update),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Update selected"),
                    _T
                    ("Bring changes from the repository into the working copy"));

  toolBar->AddTool (ID_Commit,
                    wxBITMAP (commit),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Commit selected"),
                    _T
                    ("Send changes from your working copy to the repository"));

  toolBar->AddTool (ID_Revert,
                    wxBITMAP (revert),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Revert selected"),
                    _T
                    ("Restore pristine working copy file (undo all local edits)"));

  toolBar->AddTool (ID_Resolve,
                    wxBITMAP (resolve),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Resolve selected"),
                    _T
                    ("Remove 'conflicted' state on working copy files or directories)"));

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
                    _T ("Info selected"),
                    _T ("Display info about selected entries"));

  toolBar->AddTool (ID_Log,
                    wxBITMAP (log),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Log selected"),
                    _T ("Show the log messages for a set entries"));

  toolBar->AddSeparator ();
}

void
RapidSvnFrame::OnToolEnter (wxCommandEvent & event)
{
}

void
RapidSvnFrame::AddProject ()
{
  wxDirDialog dialog (this, "Select a directory", wxGetHomeDir ());
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
    wxMessageBox (_T
                  ("You cannot add a subversion administrative directory to the workbench!"),
                  APPLICATION_NAME, wxOK);
    return;
  }

  // add
  m_folder_browser->AddProject (dialog.GetPath ());
  m_folder_browser->Refresh ();

  wxLogStatus (_T ("Added project to workbench  '%s'"),
               dialog.GetPath ().c_str ());
}

void
RapidSvnFrame::RemoveProject ()
{
  wxASSERT (m_folder_browser);
  if( m_folder_browser->RemoveProject() )
  {
    wxLogStatus (_T ("Removed project from workbench"));
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
  UniqueArrayString & workbenchItems = m_folder_browser->GetWorkbenchItems ();

  for (i = 0;; i++)
  {
    key.Printf (_T ("/MainFrame/wc%d"), i);
    if (pConfig->Read (key, &val, _T ("")))
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

  switch (event.m_id)
  {
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
    m_logTracer->Trace (_T("Unimplemented action!"));
    break;

  }

  if( action )
  {
    Perform (lastAction, action);
  }
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

  case TOKEN_VSVN_INTERNAL_ERROR:
    {
      ErrorTracer err_tr (this);

      err_tr.Trace (event.GetString ());
      err_tr.ShowErrors ();
    }
    break;

  case TOKEN_ACTION_END:
    {
      // well, why not always update
      // after an action?
//       switch (m_lastAction)
//       {
//         case ACTION_TYPE_UPDATE:
//         case ACTION_TYPE_ADD:
//         case ACTION_TYPE_DEL:
//         case ACTION_TYPE_COMMIT:
//         case ACTION_TYPE_REVERT:
//         case ACTION_TYPE_RESOLVE:
//           UpdateFileList ();
//           break;
//       }
      UpdateFileList ();
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
    wxString caption = _T ("Info");
    if (wasError)
    {
      rep_type = ERROR_REPORT;
      caption = _T ("Info error");
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
  if (event.GetKeyEvent ().GetKeyCode () == WXK_F5)
  {
    m_activePane = ACTIVEPANE_FOLDER_BROWSER;
    UpdateFileList ();
  }
  else
  {
    event.Skip ();
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

InfoPanel::InfoPanel (wxWindow * parent)
  : wxPanel (parent, -1, wxDefaultPosition, wxDefaultSize, 
             wxTAB_TRAVERSAL | wxCLIP_CHILDREN)
{
}

LogTracer::LogTracer (wxWindow * parent)
  : wxTextCtrl (parent, -1, _T (""), wxPoint (0, 0),
                wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)
{
  SetMaxLength (0);
}

void
LogTracer::Trace (const wxString & str)
{
  AppendText (str + _T ("\n"));
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
