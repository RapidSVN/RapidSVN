
#include "svncpp/log.h"
#include "include.h"
#include <wx/utils.h>
#include <wx/log.h>
#include <wx/resource.h>
#include "wx/filename.h"

#include "svn_file_info.h"
#include "rapidsvn_app.h"

#include "checkout_action.h"
#include "import_action.h"
#include "update_action.h"
#include "add_action.h"
#include "delete_action.h"
#include "commit_action.h"
#include "revert_action.h"
#include "resolve_action.h"
#include "copy_action.h"
#include "mkdir_action.h"
#include "merge_action.h"

#include "report_dlg.h"
#include "preferences_dlg.h"

#include "rapidsvn.h"

// Toolbars' ids
#define TOOLBAR_REFRESH  101
#define TOOLBAR_ADD   103
#define TOOLBAR_DEL   104
#define TOOLBAR_UPDATE  105
#define TOOLBAR_COMMIT  106
#define TOOLBAR_REVERT  107
#define TOOLBAR_INFO  108
#define TOOLBAR_STATUS  109
#define TOOLBAR_LOG   110
#define TOOLBAR_RESOLVE  111
#define TOOLBAR_FOLDERUP 112

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__)
#include "icons.h"
#include "bitmaps.h"
#endif
// number of items initially in the list
static const int NUM_ITEMS = 30;

// List config keys here, to avoid duplicating literal text:
const static char *szBrowserPathKey = "/MainFrame/BrowserPath";

#define wxTraceMisc wxT("tracemisc")

// define this to 1 to use wxToolBarSimple instead of the native one
#define USE_GENERIC_TBAR 0

BEGIN_EVENT_TABLE (VSvnFrame, wxFrame)
EVT_SIZE (VSvnFrame::OnSize)
EVT_MENU (ID_AddProject, VSvnFrame::OnAddProject)
EVT_MENU (ID_RemoveProject, VSvnFrame::OnRemoveProject)
EVT_MENU (ID_Quit, VSvnFrame::OnQuit)
EVT_MENU (ID_About, VSvnFrame::OnAbout)
EVT_MENU (ID_Status, VSvnFrame::OnStatus)
EVT_MENU (ID_Log, VSvnFrame::OnLog)
EVT_MENU (ID_Info, VSvnFrame::OnInfo)
EVT_MENU (ID_Checkout, VSvnFrame::OnCheckout)
EVT_MENU (ID_Import, VSvnFrame::OnImport)
EVT_MENU (ID_Update, VSvnFrame::OnUpdate)
EVT_MENU (ID_Add, VSvnFrame::OnAdd)
EVT_MENU (ID_Del, VSvnFrame::OnDelete)
EVT_MENU (ID_Commit, VSvnFrame::OnCommit)
EVT_MENU (ID_Revert, VSvnFrame::OnRevert)
EVT_MENU (ID_Copy, VSvnFrame::OnCopy)
EVT_MENU (ID_Rename, VSvnFrame::OnRename)
EVT_MENU (ID_Resolve, VSvnFrame::OnResolve)
EVT_MENU (ID_Mkdir, VSvnFrame::OnMkdir)
EVT_MENU (ID_Merge, VSvnFrame::OnMerge)
EVT_MENU (ID_Contents, VSvnFrame::OnContents)
EVT_MENU (ID_Preferences, VSvnFrame::OnPreferences)
EVT_MENU (ACTION_EVENT, VSvnFrame::OnActionEvent)
EVT_MENU (-1, VSvnFrame::OnToolLeftClick)
EVT_TOOL_ENTER (ID_TOOLBAR, VSvnFrame::OnToolEnter)
EVT_TREE_SEL_CHANGED (-1, VSvnFrame::OnFolderBrowserSelChanged)
EVT_TREE_KEY_DOWN (-1, VSvnFrame::OnFolderBrowserKeyDown) END_EVENT_TABLE ()
/** class implementation **/
  VSvnFrame::VSvnFrame (const wxString & title):
wxFrame ((wxFrame *) NULL, -1, title)
{
  // apr stuff
  apr_initialize ();
  pool = svn_pool_create (NULL);
  m_folder_browser = NULL;
  m_listCtrl = NULL;

  // enable trace
  wxLog::AddTraceMask (wxTraceMisc);

  // Retrieve a pointer to the application configuration object.
  // If the object is not created, it will be created upon the first
  // call to Get().
  wxConfigBase *pConfig = wxConfigBase::Get ();

  SetIcon (wxICON (mondrian));

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
                                           wxSP_3D | wxSP_LIVE_UPDATE);


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
                                          wxSP_3D | wxSP_LIVE_UPDATE);

  // Create the list control to display files
  m_listCtrl = new FileListCtrl (m_vert_splitter,
                                 pool,
                                 LIST_CTRL, wxDefaultPosition, wxDefaultSize);

  // Set the current browse position:
  wxString
    BrowsePosition =
    pConfig->Read (szBrowserPathKey, wxDirDialogDefaultFolderStr);

  InitFileList ();

  // Create the browse control
  m_folder_browser = new FolderBrowser (m_vert_splitter,
                                        pool,
                                        -1,
                                        BrowsePosition,
                                        wxDefaultPosition,
                                        wxDefaultSize,
                                        wxDIRCTRL_DIR_ONLY,
                                        _T (""), 0, _T (""));

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

VSvnFrame::~VSvnFrame ()
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
  int item;
  const wxArrayString & workbenchItems =
    m_folder_browser->GetWorkbenchItems ();
  const int itemCount = workbenchItems.GetCount ();
  for (item = 0; item < itemCount; item++)
  {
    key.Printf (_T ("/MainFrame/wc%d"), item);
    pConfig->Write (key, workbenchItems.Item (item));
  }

  PreferencesDlg::Data.Write (pConfig);

  apr_pool_destroy (pool);
}

void
VSvnFrame::InitializeMenu ()
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

  menuView->AppendSeparator ();

  pItem = new wxMenuItem (menuView, ID_Preferences, _T ("Preferences"));
  menuView->Append (pItem);

  // Create menu
  wxMenu *menuCreate = new wxMenu;
  menuCreate->Append (ID_Import, "&Import an unversioned file or tree ...");
  menuCreate->Append (ID_Checkout, "&Checkout module ...");

  menuCreate->AppendSeparator ();

  menuCreate->Append (ID_Mkdir, "Make a new directory ...");
  menuCreate->Append (ID_Copy, "C&opy remembering history ...");

  menuCreate->AppendSeparator ();

  menuCreate->Append (ID_Merge, "Merge differences");
  menuCreate->Append (ID_Switch, "Switch to URL ...");

  // Modify menu
  wxMenu *menuModif = new wxMenu;
  pItem = new wxMenuItem (menuModif, ID_Update, _T ("Get latest"));
  pItem->SetBitmap (wxBITMAP (update));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_Commit, _T ("Check in"));
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

  pItem = new wxMenuItem (menuModif, ID_Copy, _T ("Copy"));
  //pItem->SetBitmap (wxBITMAP (copy));
  menuModif->Append (pItem);

  pItem = new wxMenuItem (menuModif, ID_Rename, _T ("Rename"));
  //pItem->SetBitmap (wxBITMAP (rename));
  menuModif->Append (pItem);

  // Query menu
  wxMenu *menuQuery = new wxMenu;
  pItem = new wxMenuItem (menuQuery, ID_Status, _T ("Status"));
  pItem->SetBitmap (wxBITMAP (status));
  menuQuery->Append (pItem);

  pItem = new wxMenuItem (menuQuery, ID_Log, _T ("Log"));
  pItem->SetBitmap (wxBITMAP (log));
  menuQuery->Append (pItem);

  pItem = new wxMenuItem (menuQuery, ID_Info, _T ("Info"));
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
VSvnFrame::InitFileList ()
{
  wxListItem itemCol;
  itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
  itemCol.m_text = wxT ("Name");
  itemCol.m_image = -1;
  m_listCtrl->InsertColumn (0, itemCol);

  itemCol.m_text = "Revision";
  m_listCtrl->InsertColumn (1, itemCol);

  itemCol.m_text = "Last changed";
  m_listCtrl->InsertColumn (2, itemCol);

  itemCol.m_text = "Status";
  m_listCtrl->InsertColumn (3, itemCol);

  itemCol.m_text = "Prop Status";
  m_listCtrl->InsertColumn (4, itemCol);

  m_listCtrl->SetColumnWidth (0, 150);
  m_listCtrl->SetColumnWidth (1, 75);
  m_listCtrl->SetColumnWidth (2, 75);
  m_listCtrl->SetColumnWidth (3, 75);
  m_listCtrl->SetColumnWidth (4, 75);
}

void
VSvnFrame::UpdateFileList ()
{
  if (m_listCtrl && m_folder_browser)
  {
    m_listCtrl->UpdateFileList (m_folder_browser->GetPath ());
  }
}

void
VSvnFrame::OnAddProject (wxCommandEvent & event)
{
  AddProject ();
}

void
VSvnFrame::OnRemoveProject (wxCommandEvent & event)
{
  RemoveProject ();
}

void
VSvnFrame::OnQuit (wxCommandEvent & WXUNUSED (event))
{
  Close (TRUE);
}

void
VSvnFrame::OnAbout (wxCommandEvent & WXUNUSED (event))
{
  wxMessageBox ("RapidSVN\nBuilt with Subversion Alpha\nand wxWindows 2.3.2",
                "About VSvn", wxOK | wxICON_INFORMATION);
}

void
VSvnFrame::OnStatus (wxCommandEvent & WXUNUSED (event))
{
  /* doesnt seem to be needed now.
     wxString items = m_folder_browser->GetPath () + "\r\n";
     wxString fullPath;

     svn_revnum_t youngest = SVN_INVALID_REVNUM;
   */

  ShowStatus ();
}

void
VSvnFrame::OnInfo (wxCommandEvent & WXUNUSED (event))
{
  ShowInfo ();
}

void
VSvnFrame::OnLog (wxCommandEvent & WXUNUSED (event))
{
  ShowLog ();
}

void
VSvnFrame::OnImport (wxCommandEvent & WXUNUSED (event))
{
  lastAction = ACTION_TYPE_IMPORT;
  ImportAction *imp_act = new ImportAction (this, pool, m_logTracer);
  imp_act->Perform ();
}

void
VSvnFrame::OnCheckout (wxCommandEvent & WXUNUSED (event))
{
  lastAction = ACTION_TYPE_CHECKOUT;
  CheckoutAction *co_act = new CheckoutAction (this, pool, m_logTracer);
  co_act->Perform ();
}

void
VSvnFrame::OnAdd (wxCommandEvent & WXUNUSED (event))
{
  AddEntries ();
}

void
VSvnFrame::OnDelete (wxCommandEvent & WXUNUSED (event))
{
  DelEntries ();
}

void
VSvnFrame::OnCommit (wxCommandEvent & WXUNUSED (event))
{
  MakeCommit ();
}

void
VSvnFrame::OnRevert (wxCommandEvent & WXUNUSED (event))
{
  MakeRevert ();
}

void
VSvnFrame::OnResolve (wxCommandEvent & WXUNUSED (event))
{
  MakeResolve ();
}

void
VSvnFrame::OnCopy (wxCommandEvent & WXUNUSED (event))
{
  MakeCopy ();
}

void
VSvnFrame::OnRename (wxCommandEvent & WXUNUSED (event))
{
  Rename ();
}

void
VSvnFrame::OnMkdir (wxCommandEvent & WXUNUSED (event))
{
  Mkdir ();
}

void
VSvnFrame::OnMerge (wxCommandEvent & WXUNUSED (event))
{
  Merge ();
}

void
VSvnFrame::OnContents (wxCommandEvent & WXUNUSED (event))
{
  Contents ();
}

void
VSvnFrame::OnPreferences (wxCommandEvent & WXUNUSED (event))
{
  Preferences ();
}

void
VSvnFrame::LayoutChildren ()
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
VSvnFrame::OnSize (wxSizeEvent & event)
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
VSvnFrame::RecreateToolbar ()
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
VSvnFrame::AddActionTools ()
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
VSvnFrame::AddInfoTools ()
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

  toolBar->AddTool (TOOLBAR_STATUS,
                    wxBITMAP (status),
                    wxNullBitmap,
                    FALSE,
                    -1,
                    -1,
                    (wxObject *) NULL,
                    _T ("Status selected"),
                    _T
                    ("Print the status of working copy files and directories"));

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
VSvnFrame::OnToolEnter (wxCommandEvent & event)
{
}

void
VSvnFrame::OnToolLeftClick (wxCommandEvent & event)
{
  switch (event.GetId ())
  {
  case TOOLBAR_REFRESH:
    m_listCtrl->UpdateFileList (m_folder_browser->GetPath ());
    break;

  case TOOLBAR_STATUS:
    ShowStatus ();
    break;

  case TOOLBAR_INFO:
    ShowInfo ();
    break;

  case TOOLBAR_LOG:
    ShowLog ();
    break;

  case TOOLBAR_UPDATE:
    MakeUpdate ();
    break;

  case TOOLBAR_ADD:
    AddEntries ();
    break;

  case TOOLBAR_DEL:
    DelEntries ();
    break;

  case TOOLBAR_COMMIT:
    MakeCommit ();
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
VSvnFrame::AddProject ()
{
  wxDirDialog dialog (this, "Select a directory", wxGetHomeDir ());

  if (dialog.ShowModal () == wxID_OK)
  {
    m_folder_browser->AddProject (dialog.GetPath ());
    m_folder_browser->Refresh ();

    wxLogStatus (_T ("Added project to workbench  '%s'"),
                 dialog.GetPath ().c_str ());
  }
}

void
VSvnFrame::RemoveProject ()
{
  wxASSERT (m_folder_browser);
  wxTreeCtrl *treeCtrl = m_folder_browser->GetTreeCtrl ();
  wxASSERT (treeCtrl);

  const wxTreeItemId selectionId = treeCtrl->GetSelection ();
  if (selectionId.IsOk ())
  {
    m_folder_browser->RemoveProject (selectionId);
    m_folder_browser->Refresh ();
    wxLogStatus (_T ("Removed project from workbench"));
  }
}

void
VSvnFrame::InitFolderBrowser ()
{
  wxConfigBase *pConfig = wxConfigBase::Get ();

  wxASSERT (m_folder_browser);

  int i;
  wxString key;
  wxString val;
  wxArrayString & workbenchItems = m_folder_browser->GetWorkbenchItems ();

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

void
VSvnFrame::MakeUpdate ()
{
  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  if (m_listCtrl->GetSelectedItemCount () <= 0)
    // nothing selected in the file list
  {
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (aux_pool, UnixPath (path));
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    targets = m_listCtrl->GetTargets (aux_pool);
  }

  lastAction = ACTION_TYPE_UPDATE;
  UpdateAction *up_act = new UpdateAction (this, pool, m_logTracer, targets);
  up_act->Perform ();
}

void
VSvnFrame::MakeCommit ()
{
  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  if (m_listCtrl->GetSelectedItemCount () <= 0)
    // nothing selected in the file list
  {
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (aux_pool, UnixPath (path));
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    targets = m_listCtrl->GetTargets (aux_pool);
  }

  lastAction = ACTION_TYPE_COMMIT;
  CommitAction *ci_act = new CommitAction (this, pool, m_logTracer, targets);
  ci_act->Perform ();
}

void
VSvnFrame::OnUpdate (wxCommandEvent & event)
{
  MakeUpdate ();
}

void
VSvnFrame::OnActionEvent (wxCommandEvent & event)
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
        // Subpool was used for UpdateAction
        svn_pool_destroy (aux_pool);
      }
    }
    break;
  }
}

void
VSvnFrame::ShowStatus ()
{
  IndexArray arr = m_listCtrl->GetSelectedItems ();
  size_t i;
  wxString path = m_folder_browser->GetPath ();
  wxString all_status;
  wxString line;
  wxString _path;

  for (i = 0; i < arr.GetCount (); i++)
  {
    wxFileName fname (path, m_listCtrl->GetItemText (arr[i]));
    _path = fname.GetFullPath ();

    statusString (fname.GetFullPath (), line);
    if (!line.IsEmpty ())
      all_status += line + _T ("\n");
  }

  if (!all_status.IsEmpty ())
  {
    Report_Dlg rdlg (this, _T ("Status"), all_status);
    rdlg.ShowModal ();
  }
}

void
VSvnFrame::ShowLog ()
{
  apr_pool_t *subpool = svn_pool_create (pool);
  apr_array_header_t *targets = m_listCtrl->GetTargets (subpool);
  wxString all;
  svn::Log log;
  char rev[sizeof (long) * 8 + 1];

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    log.loadPath (target, -2, 1);
    if (log.isVersioned ())
    {
      while (log.next ())
      {
        sprintf (rev, "%ld", (long) log.revision ());
        all += "--------------------\n";
        all += "Revision: ";
        all += rev;
        all += "\nAuthor: ";
        all += log.author ();
        all += "\nDate: ";
        all += log.date ();
        all += "\n\nMessage: ";
        all += log.message ();
        all += "\n";
      }
    }
  }

  if (!all.IsEmpty ())
  {
    int rep_type = NORMAL_REPORT;
    wxString caption = _T ("Log");
    if (!log.isVersioned ())
    {
      rep_type = ERROR_REPORT;
      caption = _T ("File is not versioned.");
    }

    Report_Dlg rdlg (this, caption, all, rep_type);
    rdlg.ShowModal ();
  }

  svn_pool_destroy (subpool);
}

void
VSvnFrame::ShowInfo ()
{
  IndexArray arr = m_listCtrl->GetSelectedItems ();
  size_t i;
  wxString path = m_folder_browser->GetPath ();
  wxString all_info;
  wxString info;
  wxString _path;
  apr_pool_t *subpool = svn_pool_create (pool);
  svn_error_t *err = NULL;
  bool wasError = false;

  for (i = 0; i < arr.GetCount (); i++)
  {
    wxFileName fname (path, m_listCtrl->GetItemText (arr[i]));
    _path = fname.GetFullPath ();
    err = svn_get_file_info (UnixPath (_path), subpool, info);

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

  svn_pool_destroy (subpool);
}

void
VSvnFrame::AddEntries ()
{
  if (m_listCtrl->GetSelectedItemCount () == 0)
    return;

  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  targets = m_listCtrl->GetTargets (aux_pool);

  lastAction = ACTION_TYPE_ADD;

  AddAction *add_act = new AddAction (this, pool, m_logTracer, targets);
  add_act->Perform ();
}

void
VSvnFrame::MakeRevert ()
{
  wxMessageDialog sure_dlg (this,
                            _T
                            ("Are you sure you want to revert local changes to pristine files?"),
                            _T ("Revert"), wxYES_NO | wxICON_QUESTION);

  if (sure_dlg.ShowModal () != wxID_YES)
    return;

  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  if (m_listCtrl->GetSelectedItemCount () <= 0)
    // nothing selected in the file list
  {
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (aux_pool, UnixPath (path));
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    targets = m_listCtrl->GetTargets (aux_pool);
  }

  lastAction = ACTION_TYPE_REVERT;

  RevertAction *add_act = new RevertAction (this, pool, m_logTracer, targets);
  add_act->Perform ();
}

void
VSvnFrame::MakeResolve ()
{
  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  if (m_listCtrl->GetSelectedItemCount () <= 0)
    // nothing selected in the file list
  {
    wxFileName fname (m_folder_browser->GetPath ());
    wxString path = fname.GetFullPath ();
    const char *target = apr_pstrdup (aux_pool, UnixPath (path));
    (*((const char **) apr_array_push (targets))) = target;
  }
  else
  {
    targets = m_listCtrl->GetTargets (aux_pool);
  }

  lastAction = ACTION_TYPE_RESOLVE;

  ResolveAction *add_act =
    new ResolveAction (this, pool, m_logTracer, targets);
  add_act->Perform ();
}

void
VSvnFrame::DelEntries ()
{
  if (m_listCtrl->GetSelectedItemCount () == 0)
    return;

  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  targets = m_listCtrl->GetTargets (aux_pool);

  lastAction = ACTION_TYPE_DEL;

  DeleteAction *del_act = new DeleteAction (this, pool, m_logTracer, targets);
  del_act->Perform ();
}

void
VSvnFrame::MakeCopy ()
{
  if (m_listCtrl->GetSelectedItemCount () != 1)
  {
    wxMessageDialog dlg (this, _T ("You can only copy one file at a time"),
                         _T ("Copy error"), wxOK);
    dlg.ShowModal ();
    return;
  }

  aux_pool = svn_pool_create (pool);
  apr_array_header_t *targets = apr_array_make (aux_pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  targets = m_listCtrl->GetTargets (aux_pool);

  lastAction = ACTION_TYPE_COPY;
  CopyAction *cp_act = new CopyAction (this, pool, m_logTracer, targets);
  cp_act->Perform ();
}

void
VSvnFrame::Rename ()
{

}

void
VSvnFrame::Mkdir ()
{
  lastAction = ACTION_TYPE_MKDIR;
  MkdirAction *mk_act = new MkdirAction (this, pool, m_logTracer);
  mk_act->Perform ();
}

void
VSvnFrame::Merge ()
{
  lastAction = ACTION_TYPE_MERGE;
  MergeAction *mrg_act = new MergeAction (this, pool, m_logTracer);
  mrg_act->Perform ();
}

void
VSvnFrame::Contents ()
{
  return;
}

void
VSvnFrame::OnFolderBrowserSelChanged (wxTreeEvent & event)
{
  UpdateFileList ();
}

void
VSvnFrame::OnFolderBrowserKeyDown (wxTreeEvent & event)
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
VSvnFrame::Preferences ()
{
  PreferencesDlg *pDlg = PreferencesDlg::CreateInstance (this);
  pDlg->ShowModal ();
  pDlg->Close (TRUE);
}

InfoPanel::InfoPanel (wxWindow * parent):wxPanel (parent, -1)
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
