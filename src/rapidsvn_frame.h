#ifndef _RAPIDSVN_FRAME_HEADER_H_INCLUDED_
#define _RAPIDSVN_FRAME_HEADER_H_INCLUDED_

#include <wx/toolbar.h>
#include <wx/splitter.h>

#include "tracer.h"
#include "log_action.h"

#define SPLITTER_WINDOW   100

typedef enum
{
  ACTION_TYPE_NONE,
  ACTION_TYPE_UPDATE,
  ACTION_TYPE_COMMIT,
  ACTION_TYPE_CHECKOUT,
  ACTION_TYPE_IMPORT,
  ACTION_TYPE_ADD,
  ACTION_TYPE_DEL,
  ACTION_TYPE_REVERT,
  ACTION_TYPE_RESOLVE,
  ACTION_TYPE_COPY,
  ACTION_TYPE_MKDIR,
  ACTION_TYPE_MERGE
}
ActionType;

class RapidSvnFrame;

/**
* Panel holding the splitter with the folder browser
* and the file list.
*/
class InfoPanel:public wxPanel
{
public:
  InfoPanel (wxWindow * parent);
};

class LogTracer:public wxTextCtrl, public Tracer
{

public:
  LogTracer (wxWindow * parent);
  void Trace (const wxString & str);
};

class RapidSvnFrame:public wxFrame
{
public:
  RapidSvnFrame (const wxString & title);
  ~RapidSvnFrame ();

  void OnSize (wxSizeEvent & event);

  // File menu
  void OnAddProject (wxCommandEvent & event);
  void OnRemoveProject (wxCommandEvent & event);
  void OnQuit (wxCommandEvent & event);

  // View menu
  void OnPreferences (wxCommandEvent & event);

  // Query menu
  void OnStatus (wxCommandEvent & event);
  void OnLog (wxCommandEvent & event);
  void OnInfo (wxCommandEvent & event);

  // Create menu
  void OnCheckout (wxCommandEvent & event);
  void OnImport (wxCommandEvent & event);
  void OnCopy (wxCommandEvent & event);
  void OnRename (wxCommandEvent & event);
  void OnMkdir (wxCommandEvent & event);
  void OnMerge (wxCommandEvent & event);

  // Modify menu
  void OnUpdate (wxCommandEvent & event);
  void OnAdd (wxCommandEvent & event);
  void OnDelete (wxCommandEvent & event);
  void OnCommit (wxCommandEvent & event);
  void OnRevert (wxCommandEvent & event);
  void OnResolve (wxCommandEvent & event);

  // Help menu
  void OnContents (wxCommandEvent & event);
  void OnAbout (wxCommandEvent & event);

  // toolbar administration
  void LayoutChildren ();
  void RecreateToolbar ();
  void AddActionTools ();
  void AddInfoTools ();

  // menu stuff
  void InitializeMenu ();

  // toolbar events
  void OnToolEnter (wxCommandEvent & event);
  void OnToolLeftClick (wxCommandEvent & event);

  // Events from action threads
  void OnActionEvent (wxCommandEvent & event);

  // Folder Browser
  void InitFolderBrowser ();
  void AddProject ();
  void RemoveProject ();
  void OnFolderBrowserSelChanged (wxTreeEvent & event);
  void OnFolderBrowserKeyDown (wxTreeEvent & event);


  // list control
  void InitFileList ();
  void UpdateFileList ();

  // utility functions

  void ShowStatus ();
  void ShowLog ();
  void ShowInfo ();

  void MakeUpdate ();
  void AddEntries ();
  void DelEntries ();
  void MakeCommit ();
  void MakeRevert ();
  void MakeResolve ();

  void MakeCopy ();
  void Rename ();
  void Mkdir ();
  void Merge ();
  void Contents ();
  void Preferences ();


  FileListCtrl *GetFileList ()
  {
    return m_listCtrl;
  }

  FolderBrowser *GetFolderBrowser ()
  {
    return m_folder_browser;
  }

  LogAction *getLogAction ()
  {
    return m_logAction;
  }


private:

  FolderBrowser * m_folder_browser;
  FileListCtrl *m_listCtrl;
  LogAction *m_logAction;

  wxSplitterWindow *m_horiz_splitter;
  wxSplitterWindow *m_vert_splitter;

  InfoPanel *m_info_panel;

  wxTextCtrl *m_log;
  EventTracer *m_logTracer;

  wxToolBar *m_tbar;
  size_t m_toolbar_rows;        // 1 or 2 only (toolbar rows)

  ActionType lastAction;

  apr_pool_t *pool;
      /**
      * Used for allocating stuff before some actions,
      * stuff that will be used in that actions
      */
  apr_pool_t *aux_pool;

DECLARE_EVENT_TABLE ()};

const int ID_TOOLBAR = 500;

/*
 * Menu commands IDs
 */
enum
{
  ID_Quit = 1,
  ID_About,
  ID_Refresh,
  ID_Import,
  ID_Checkout,
  ID_Copy,
  ID_Rename,
  ID_Combo,
  ID_Update,
  ID_Commit,
  ID_Add,
  ID_Del,
  ID_Revert,
  ID_Status,
  ID_Log,
  ID_Info,
  ID_Resolve,
  ID_Merge,
  ID_Contents,
  ID_Mkdir,
  ID_Switch,
  ID_Preferences,
  ID_AddProject,
  ID_RemoveProject,

  ACTION_EVENT,                 // this one gets sent from the action threads

  LIST_CTRL = 1000
};

#endif
