/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "svncpp/log.hpp"
#include "include.hpp"
#include <wx/utils.h>
#include <wx/log.h>
#include <wx/resource.h>
#include "wx/filename.h"

#include "svn_file_info.hpp"
#include "rapidsvn_app.hpp"

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

#include "report_dlg.hpp"
#include "preferences_dlg.hpp"

#include "rapidsvn_frame.hpp"
#include "wx/version.h"
#include "svn_version.h"
#include "version.hpp"

// Bitmaps
#include "res/bitmaps/aalogo.xpm"
#include "res/bitmaps/refresh.xpm"
#include "res/bitmaps/update.xpm"
#include "res/bitmaps/commit.xpm"
#include "res/bitmaps/add.xpm"
#include "res/bitmaps/delete.xpm"
#include "res/bitmaps/revert.xpm"
#include "res/bitmaps/resolve.xpm"
#include "res/bitmaps/log.xpm"
#include "res/bitmaps/info.xpm"

// Toolbars' ids
#define TOOLBAR_REFRESH  101
#define TOOLBAR_ADD   103
#define TOOLBAR_DEL   104
#define TOOLBAR_UPDATE  105
#define TOOLBAR_COMMIT  106
#define TOOLBAR_REVERT  107
#define TOOLBAR_INFO  108
#define TOOLBAR_LOG   110
#define TOOLBAR_RESOLVE  111
#define TOOLBAR_FOLDERUP 112

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
  EVT_MENU (ID_Log, RapidSvnFrame::OnLog)
  EVT_MENU (ID_Info, RapidSvnFrame::OnInfo)
  EVT_MENU (ID_Checkout, RapidSvnFrame::OnCheckout)
  EVT_MENU (ID_Import, RapidSvnFrame::OnImport)
  EVT_MENU (ID_Update, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Add, RapidSvnFrame::OnAdd)
  EVT_MENU (ID_Del, RapidSvnFrame::OnDelete)
  EVT_MENU (ID_Commit, RapidSvnFrame::OnFileCommand)
  EVT_MENU (ID_Revert, RapidSvnFrame::OnRevert)
  EVT_MENU (ID_Copy, RapidSvnFrame::OnCopy)
  EVT_MENU (ID_Rename, RapidSvnFrame::OnRename)
  EVT_MENU (ID_Resolve, RapidSvnFrame::OnResolve)
  EVT_MENU (ID_Property, RapidSvnFrame::OnProperties)
  EVT_MENU (ID_Mkdir, RapidSvnFrame::OnMkdir)
  EVT_MENU (ID_Merge, RapidSvnFrame::OnMerge)
  EVT_MENU (ID_Contents, RapidSvnFrame::OnContents)
  EVT_MENU (ID_Preferences, RapidSvnFrame::OnPreferences)
  EVT_MENU (ID_Refresh, RapidSvnFrame::OnRefresh)
  EVT_MENU (ID_MoveTo, RapidSvnFrame::OnMoveTo)
  EVT_MENU (ID_CopyTo, RapidSvnFrame::OnCopyTo)
  EVT_MENU (ID_RenameHere, RapidSvnFrame::OnRenameHere)
  EVT_MENU (ID_CopyHere, RapidSvnFrame::OnCopyHere)
  EVT_MENU (ACTION_EVENT, RapidSvnFrame::OnActionEvent)
  EVT_MENU (-1, RapidSvnFrame::OnToolLeftClick)
  EVT_TOOL_ENTER (ID_TOOLBAR, RapidSvnFrame::OnToolEnter)
  EVT_TREE_SEL_CHANGED (-1, RapidSvnFrame::OnFolderBrowserSelChanged)
  EVT_TREE_KEY_DOWN (-1, RapidSvnFrame::OnFolderBrowserKeyDown) 
END_EVENT_TABLE ()

/** class implementation **/
RapidSvnFrame::RapidSvnFrame (const wxString & title)
  : wxFrame ((wxFrame *) NULL, -1, title, wxDefaultPosition, wxDefaultSize, 
             wxDEFAULT_FRAME_STYLE)
{
  // apr stuff
  apr_initialize ();
  m_pool.Create(NULL);
  m_folder_browser = NULL;
  m_listCtrl = NULL;

  // enable trace
  wxLog::AddTraceMask (wxTraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *pConfig = wxConfigBase::Get ();

  SetIcon (wxICON (aalogo));

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
  m_listCtrl = new FileListCtrl (m_vert_splitter, LIST_CTRL, 
                                 wxDefaultPosition, wxDefaultSize);

  // Set the current browse position:
  wxString BrowsePosition =
    pConfig->Read (szBrowserPathKey, wxDirDialogDefaultFolderStr);

  // Create the browse control
  m_folder_browser = new FolderBrowser (m_vert_splitter);

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

  menuFile->Append (ID_AddProject, "&Add to Workbench...");
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
  menuCreate->Append (ID_Import, "&Import an unversioned file or tree ...");
  menuCreate->Append (ID_Checkout, "&Checkout working copy ...");

  menuCreate->AppendSeparator ();

  menuCreate->Append (ID_Mkdir, "Make a new directory ...");
  menuCreate->Append (ID_Copy, "C&opy remembering history ...");

  menuCreate->AppendSeparator ();

  menuCreate->Append (ID_Merge, "Merge differences");
  menuCreate->Append (ID_Switch, "Switch to URL ...");

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

  // Help Menu
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append (ID_Contents, "&Contents");
  menuHelp->AppendSeparator ();
  menuHelp->Append (ID_About, "&About...");

  // Create the menu bar and append the menus
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append (menuFile, "&File");
  menuBar->Append (menuView, "&View");
  menuBar->Append (menuCreate, "&Create");
  menuBar->Append (menuModif, "&Modify");
  menuBar->Append (menuQuery, "&Query");
  menuBar->Append (menuHelp, "&Help");

  SetMenuBar (menuBar);
}

void
RapidSvnFrame::UpdateFileList ()
{
  if (m_listCtrl && m_folder_browser)
  {
    const wxString & path = m_folder_browser->GetPath ();

    if (path.length () > 0)
    {
      m_listCtrl->UpdateFileList (path);
      m_listCtrl->Show (TRUE);
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

  msg.Printf (_T ("%s\nVersion %d.%d.%d\n"
                  "Milestone: %s\n"
                  "\nBuilt with:\n"
                  "Subversion %d.%d.%d\n"
                  "wxWindows %d.%d.%d"),
              APPLICATION_NAME,
              RAPIDSVN_VER_MAJOR, RAPIDSVN_VER_MINOR, RAPIDSVN_VER_MICRO,
              RAPIDSVN_VER_MILESTONE,
              SVN_VER_MAJOR, SVN_VER_MINOR, SVN_VER_MICRO,
              wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);
  wxMessageBox (msg, "About RapidSVN", wxOK | wxICON_INFORMATION);
}

void
RapidSvnFrame::OnInfo (wxCommandEvent & WXUNUSED (event))
{
  ShowInfo ();
}

void
RapidSvnFrame::OnLog (wxCommandEvent & WXUNUSED (event))
{
  ShowLog ();
}

void
RapidSvnFrame::OnImport (wxCommandEvent & WXUNUSED (event))
{
  lastAction = ACTION_TYPE_IMPORT;
  ImportAction *imp_act = new ImportAction (this, m_logTracer);
  imp_act->Perform ();
}

void
RapidSvnFrame::OnCheckout (wxCommandEvent & WXUNUSED (event))
{
  lastAction = ACTION_TYPE_CHECKOUT;
  CheckoutAction *co_act = new CheckoutAction (this, m_logTracer);
  co_act->Perform ();
}

void
RapidSvnFrame::OnAdd (wxCommandEvent & WXUNUSED (event))
{
  AddEntries ();
}

void
RapidSvnFrame::OnDelete (wxCommandEvent & WXUNUSED (event))
{
  DelEntries ();
}

void
RapidSvnFrame::OnRevert (wxCommandEvent & WXUNUSED (event))
{
  MakeRevert ();
}

void
RapidSvnFrame::OnResolve (wxCommandEvent & WXUNUSED (event))
{
  MakeResolve ();
}

void
RapidSvnFrame::OnCopy (wxCommandEvent & WXUNUSED (event))
{
  MakeCopy ();
}

void
RapidSvnFrame::OnRename (wxCommandEvent & WXUNUSED (event))
{
  Rename ();
}

void
RapidSvnFrame::OnMkdir (wxCommandEvent & WXUNUSED (event))
{
  Mkdir ();
}

void
RapidSvnFrame::OnMerge (wxCommandEvent & WXUNUSED (event))
{
  Merge ();
}

void
RapidSvnFrame::OnContents (wxCommandEvent & WXUNUSED (event))
{
  Contents ();
}

void
RapidSvnFrame::OnPreferences (wxCommandEvent & WXUNUSED (event))
{
  Preferences ();
}

void
RapidSvnFrame::OnProperties (wxCommandEvent & WXUNUSED (event))
{
  Properties ();
}


void
RapidSvnFrame::OnRefresh (wxCommandEvent & WXUNUSED (event))
{
  if (m_folder_browser)
  {
    m_folder_browser->Refresh ();
  }
  //Preferences ();
}

void
RapidSvnFrame::OnCopyTo (wxCommandEvent & WXUNUSED (event))
{
}

void
RapidSvnFrame::OnMoveTo (wxCommandEvent & WXUNUSED (event))
{
}

void
RapidSvnFrame::OnRenameHere (wxCommandEvent & WXUNUSED (event))
{
}

void
RapidSvnFrame::OnCopyHere (wxCommandEvent & WXUNUSED (event))
{
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
  toolBar->AddTool (TOOLBAR_REFRESH,
                    wxBITMAP (refresh),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL, "Refresh", "Refresh the file list");

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

  toolBar->AddTool (TOOLBAR_ADD,
                    wxBITMAP (add),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Add selected"),
                    _T ("Put files and directories under revision control"));

  toolBar->AddTool (TOOLBAR_DEL,
                    wxBITMAP (delete),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Remove selected"),
                    _T ("Remove files and directories from version control"));

  toolBar->AddTool (TOOLBAR_UPDATE,
                    wxBITMAP (update),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Update selected"),
                    _T
                    ("Bring changes from the repository into the working copy"));

  toolBar->AddTool (TOOLBAR_COMMIT,
                    wxBITMAP (commit),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Commit selected"),
                    _T
                    ("Send changes from your working copy to the repository"));

  toolBar->AddTool (TOOLBAR_REVERT,
                    wxBITMAP (revert),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Revert selected"),
                    _T
                    ("Restore pristine working copy file (undo all local edits)"));

  toolBar->AddTool (TOOLBAR_RESOLVE,
                    wxBITMAP (resolve),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Resolve selected"),
                    _T
                    (" Remove 'conflicted' state on working copy files or directories)"));

  toolBar->AddSeparator ();
}

void
RapidSvnFrame::AddInfoTools ()
{
  wxToolBarBase *toolBar = GetToolBar ();

  toolBar->AddTool (TOOLBAR_INFO,
                    wxBITMAP (info),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Info selected"),
                    _T ("Display info about selected entries"));

  toolBar->AddTool (TOOLBAR_LOG,
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
RapidSvnFrame::OnToolLeftClick (wxCommandEvent & event)
{
  switch (event.GetId ())
  {
  case TOOLBAR_REFRESH:
    m_listCtrl->UpdateFileList (m_folder_browser->GetPath ());
    break;

  case TOOLBAR_INFO:
    ShowInfo ();
    break;

  case TOOLBAR_LOG:
    ShowLog ();
    break;

  case TOOLBAR_COMMIT:
  case TOOLBAR_UPDATE:
    OnFileCommand (event);
    break;

  case TOOLBAR_ADD:
    AddEntries ();
    break;

  case TOOLBAR_DEL:
    DelEntries ();
    break;

  case TOOLBAR_REVERT:
    MakeRevert ();
    break;

  case TOOLBAR_RESOLVE:
    MakeResolve ();
    break;

  case TOOLBAR_FOLDERUP:
    {
      wxString gigi;
      int x;
      wxFileName path (m_folder_browser->GetPath ());
      //path.AppendDir( path.GetPathSeparators() );
      x = path.GetDirCount ();
      //path.RemoveDir( x );
      //gigi = path.GetFullPath();
      //m_folder_browser->SetPath( path.GetFullPath() );
    }
    break;
  }
}

void
RapidSvnFrame::AddProject ()
{
  wxDirDialog dialog (this, "Select a directory", wxGetHomeDir ());
  bool add = TRUE;

  if (dialog.ShowModal () == wxID_OK)
  {
    wxFileName fileName (dialog.GetPath ());
    if ((fileName.GetName () + fileName.GetExt ()) == SVN_WC_ADM_DIR_NAME)
    {
      add = FALSE;
      wxMessageBox (_T
                    ("You cannot add a subversion administrative directory to the workbench!"),
                    APPLICATION_NAME, wxOK);
    }
  }
  if (add)
  {
    m_folder_browser->AddProject (dialog.GetPath ());
    m_folder_browser->Refresh ();

    wxLogStatus (_T ("Added project to workbench  '%s'"),
                 dialog.GetPath ().c_str ());
  }
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

apr_array_header_t *
RapidSvnFrame::GetActionTargets ()
{
  svn::Pool localPool(m_pool);

  wxWindow* activeWindow = FindFocus();
  apr_array_header_t *targets = 
    apr_array_make (localPool.pool(),
                    DEFAULT_ARRAY_SIZE,
                    sizeof (const char *));

  //is there nothing selected in the list control, 
  //or is the active window *not* the list control?
  if (m_listCtrl->GetSelectedItemCount () <= 0 || activeWindow != m_listCtrl)
  {
    //yes, so build the file list from the folder browser
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (localPool.pool(), path);
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    //no, build the file list from the list control
    targets = m_listCtrl->GetTargets (localPool);
  }

  return targets;
}

void
RapidSvnFrame::OnFileCommand (wxCommandEvent & event)
{
  FileAction* action = NULL;
  apr_array_header_t* targets = GetActionTargets ();

  switch (event.m_id)
  {
  case ID_Update:
    action = new UpdateAction(this, m_logTracer, targets);
    lastAction = ACTION_TYPE_UPDATE;
    break;
  case ID_Commit:
    action = new CommitAction(this, m_logTracer, targets);
    lastAction = ACTION_TYPE_COMMIT;
    break;
  }

  if( action )
  {
    action->PerformAction();
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
      if (lastAction == ACTION_TYPE_UPDATE ||
          lastAction == ACTION_TYPE_ADD ||
          lastAction == ACTION_TYPE_DEL ||
          lastAction == ACTION_TYPE_COMMIT ||
          lastAction == ACTION_TYPE_REVERT ||
          lastAction == ACTION_TYPE_RESOLVE)
      {
        m_listCtrl->UpdateFileList (m_folder_browser->GetPath ());
      }
    }
    break;
  }
}

void
RapidSvnFrame::ShowLog ()
{
  svn::Pool localPool(m_pool);
  apr_array_header_t *targets = 
    m_listCtrl->GetTargets (localPool);
  const char *target;

  if (targets->nelts > 0)
  {
    target = ((const char **) (targets->elts))[0];
    m_logAction = new LogAction (this, m_logTracer, target);
    m_logAction->Perform ();
  }
}

void
RapidSvnFrame::Properties ()
{
  svn::Pool localPool(m_pool);

  apr_array_header_t *targets = m_listCtrl->GetTargets (localPool);
  const char *target;

  if (targets->nelts > 0)
  {
    target = ((const char **) (targets->elts))[0];
    PropertyAction * propAction = 
      new PropertyAction (this, m_logTracer, target);
    propAction->Perform ();
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
  svn::Pool subPool(m_pool);
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
    all_info += info + _T ("\n");
  }

  {
    int rep_type = NORMAL_REPORT;
    wxString caption = _T ("Info");
    if (wasError)
    {
      rep_type = ERROR_REPORT;
      caption = _T ("Info error");
    }

    Report_Dlg rdlg (this, caption, all_info, rep_type);
    rdlg.ShowModal ();
  }
}

void
RapidSvnFrame::AddEntries ()
{
  if (m_listCtrl->GetSelectedItemCount () == 0)
    return;

  svn::Pool localPool(m_pool);
  apr_array_header_t *targets = apr_array_make (localPool.pool(),
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  targets = m_listCtrl->GetTargets (localPool);

  lastAction = ACTION_TYPE_ADD;

  AddAction *add_act = new AddAction (this, m_logTracer, targets);
  add_act->Perform ();
}

void
RapidSvnFrame::MakeRevert ()
{
  wxMessageDialog sure_dlg (this,
                            _T
                            ("Are you sure you want to revert local changes to pristine files?"),
                            _T ("Revert"), wxYES_NO | wxICON_QUESTION);

  if (sure_dlg.ShowModal () != wxID_YES)
    return;

  svn::Pool localPool(m_pool);

  apr_array_header_t *targets = 
    apr_array_make (localPool.pool(),
                    DEFAULT_ARRAY_SIZE,
                    sizeof (const char *));

  if (m_listCtrl->GetSelectedItemCount () <= 0)
    // nothing selected in the file list
  {
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (localPool.pool(), UnixPath (path));
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    targets = m_listCtrl->GetTargets (localPool);
  }

  lastAction = ACTION_TYPE_REVERT;

  RevertAction *add_act = new RevertAction (this, m_logTracer, targets);
  add_act->Perform ();
}

void
RapidSvnFrame::MakeResolve ()
{
  svn::Pool localPool(m_pool);
  apr_array_header_t *targets = 
    apr_array_make (localPool.pool(),
                    DEFAULT_ARRAY_SIZE,
                    sizeof (const char *));

  if (m_listCtrl->GetSelectedItemCount () <= 0)
    // nothing selected in the file list
  {
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (localPool.pool(), UnixPath (path));
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    targets = m_listCtrl->GetTargets (localPool);
  }

  lastAction = ACTION_TYPE_RESOLVE;

  ResolveAction *add_act = 
    new ResolveAction (this, m_logTracer, targets);
  add_act->Perform ();
}

void
RapidSvnFrame::DelEntries ()
{
  if (m_listCtrl->GetSelectedItemCount () == 0)
    return;

  svn::Pool localPool(m_pool);
  apr_array_header_t *targets = 
    apr_array_make (localPool.pool(),
                    DEFAULT_ARRAY_SIZE,
                    sizeof (const char *));

  targets = m_listCtrl->GetTargets (localPool);

  lastAction = ACTION_TYPE_DEL;

  DeleteAction *del_act = new DeleteAction (this, m_logTracer, targets);
  del_act->Perform ();
}

void
RapidSvnFrame::MakeCopy ()
{
  if (m_listCtrl->GetSelectedItemCount () != 1)
  {
    wxMessageDialog dlg (this, _T ("You can only copy one file at a time"),
                         _T ("Copy error"), wxOK);
    dlg.ShowModal ();
    return;
  }

  svn::Pool localPool(m_pool);

  apr_array_header_t *targets = apr_array_make (localPool.pool(),
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  targets = m_listCtrl->GetTargets (localPool);

  lastAction = ACTION_TYPE_COPY;
  CopyAction *cp_act = new CopyAction (this, m_logTracer, targets);
  cp_act->Perform ();
}

void
RapidSvnFrame::Rename ()
{

}

void
RapidSvnFrame::Mkdir ()
{
  lastAction = ACTION_TYPE_MKDIR;
  MkdirAction *mk_act = new MkdirAction (this, m_logTracer);
  mk_act->Perform ();
}

void
RapidSvnFrame::Merge ()
{
  lastAction = ACTION_TYPE_MERGE;
  MergeAction *mrg_act = new MergeAction (this, m_logTracer);
  mrg_act->Perform ();
}

void
RapidSvnFrame::Contents ()
{
  return;
}

void
RapidSvnFrame::OnFolderBrowserSelChanged (wxTreeEvent & event)
{
//  ((wxTreeCtrl*)event.m_eventObject)->Refresh( false, NULL );
  UpdateFileList ();
}

void
RapidSvnFrame::OnFolderBrowserKeyDown (wxTreeEvent & event)
{
  if (event.GetKeyEvent ().GetKeyCode () == WXK_F5)
  {
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

InfoPanel::InfoPanel (wxWindow * parent):wxPanel (parent, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxCLIP_CHILDREN)
{
}

LogTracer::LogTracer (wxWindow * parent):wxTextCtrl (parent, -1, _T (""), wxPoint (0, 0),
            wxDefaultSize,
            wxTE_MULTILINE | wxTE_READONLY)
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
