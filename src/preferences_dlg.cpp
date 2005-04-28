/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
// wxWidgets
#include "wx/wx.h"
#include "wx/confbase.h"
#include "wx/notebook.h"
#include "wx/valgen.h"
#include "wx/filedlg.h"
#include "wx/button.h"

// app
#include "ids.hpp"
#include "preferences_dlg.hpp"
#include "utils.hpp"
#include "preferences.hpp"

enum
{
  ID_StandardEditorLookup = wxID_HIGHEST,
  ID_StandardExplorerLookup,
  ID_DiffToolLookup
};

/****************************************************************************/

// platform dependant constants
#ifdef _WIN32
static const wxChar * EXECUTABLE_WILDCARD =
  _("Executable Files|*.exe;*.com;*.bat|All files (*.*)|*.*");

#else
// NOTE: (WX 2.4.0) On Motif, neither multiple extensions nor multiple file
// types are supported (wildcard is limited to a single "*.ext" string)
static const wxChar * EXECUTABLE_WILDCARD = _("All files|*");
#endif

/* GeneralPanel **************************************************************/
class GeneralPanel : public wxPanel
{
public:
  GeneralPanel (wxWindow * parent, Preferences * prefs)
    : wxPanel (parent), m_prefs (prefs)
  {
    wxBoxSizer * mainsizer = new wxBoxSizer (wxVERTICAL);
    {
      wxGenericValidator valCheck (&m_prefs->purgeTempFiles);
      wxCheckBox * check =
        new wxCheckBox (this, -1,
                        _("Purge temporary files on program exit"),
                        wxDefaultPosition,
                        wxDefaultSize, 0, valCheck);

      mainsizer->Add (20, 20);
      mainsizer->Add (check, 0, wxALL, 5);
    }

    SetSizer (mainsizer);
    SetAutoLayout (true);
  }
private:
  Preferences * m_prefs;
};


/* ProgramsPanel *********************************************************/

/**
 * External program settings page for the preferences dialog.
 * Use this as a model for adding new pages.
 *
 */
class ProgramsPanel : public wxPanel
{
public:

  ProgramsPanel::ProgramsPanel (wxWindow* parent, Preferences * prefs)
    : wxPanel (parent), m_prefs (prefs)
  {
    InitializeData ();
  }

  virtual ~ProgramsPanel ()
  {
  }


  /**
   * Show a dialog to select an executable file
   *
   * @param title Dialog title
   * @param textCtrl Text-Control where the path is stored
   * @retval true if everything is ok
   * @retval false if dialog was aborted
   */
  bool SelectExecutable (const wxString & title, wxTextCtrl * textCtrl)
  {
    wxFileDialog dlg (this, title);

    dlg.SetStyle (wxHIDE_READONLY | wxOPEN);
    dlg.SetWildcard (EXECUTABLE_WILDCARD);
    dlg.SetPath (textCtrl->GetValue ());

    if (dlg.ShowModal () != wxID_OK)
      return false;

    textCtrl->SetValue (dlg.GetPath ());
    return true;
  }


  /**
   * Called when "browse-button" of Standard Editor field is clicked
   */
  void
  OnStandardEditorLookup (wxCommandEvent & event)
  {
    SelectExecutable (
      _("Select standard editor executable"),
      mTextEditor);
  }

  /**
   * Called when "browse-button" of Standard File Browser field is clicked
   */
  void
  OnStandardFileExplorerLookup (wxCommandEvent & event)
  {
    SelectExecutable (
      _("Select standard file explorer executable"),
      mTextExplorer);
  }

  /**
   * Called when "browse-button" of Diff-Tool field is clicked
   */
  void
  OnDiffToolLookup (wxCommandEvent & event)
  {
    SelectExecutable (
      _("Select diff tool executable"),
      mTextDiffTool);
  }

private:
  Preferences * m_prefs;
  wxTextCtrl * mTextEditor;
  wxTextCtrl * mTextExplorer;
  wxTextCtrl * mTextDiffTool;

  void InitializeData ()
  {
    // Standard Editor
    wxStaticBox * boxEditor =
      new wxStaticBox (this, -1, _("Standard editor:"));

    wxStaticBoxSizer * sizerEditor =
      new wxStaticBoxSizer (boxEditor, wxVERTICAL);
    {
      // text ctrl
      wxTextValidator val (wxFILTER_NONE, &m_prefs->editor);
      mTextEditor = new wxTextCtrl (
        this, -1, wxEmptyString, wxDefaultPosition,
        wxSize (200, -1), 0, val);

      // button
      wxButton * button =
        CreateEllipsisButton(this, ID_StandardEditorLookup);

      // arguments
      wxStaticText * labelArgs = new wxStaticText (
        this, -1, _("Program arguments (%1=selected file):"),
        wxDefaultPosition);
      wxTextValidator valArgs (wxFILTER_NONE, &m_prefs->editorArgs);
      wxTextCtrl * args = new wxTextCtrl (
        this, -1, wxEmptyString, wxDefaultPosition,
        wxSize (200, -1), 0, valArgs);

      // checkbox
      wxGenericValidator valCheck (&m_prefs->editorAlways);
      wxCheckBox * check =
        new wxCheckBox (this, -1, _("Use always"),
                        wxDefaultPosition,
                        wxDefaultSize, 0, valCheck );

      // position controls
      wxFlexGridSizer * sizer = new wxFlexGridSizer (2);
      sizer->Add (mTextEditor, 1,
                  wxALIGN_CENTER | wxEXPAND | wxALL, 5);
      sizer->AddGrowableCol (0);
      sizer->Add (button, 0, wxALIGN_CENTER);
      sizerEditor->Add (sizer, 1, wxEXPAND);
      sizerEditor->Add (labelArgs, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
      sizerEditor->Add (args, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
      sizerEditor->Add (check, 1, wxALL | wxEXPAND, 5);
    }

    // Standard File Explorer
    wxStaticBox * boxExplorer =
      new wxStaticBox (this, -1, _("Standard file explorer:"));

    wxStaticBoxSizer * sizerExplorer =
      new wxStaticBoxSizer (boxExplorer, wxVERTICAL);
    {
      // text ctrl
      wxTextValidator valText (wxFILTER_NONE, &m_prefs->explorer);
      mTextExplorer =
        new wxTextCtrl (this, -1, wxEmptyString, wxDefaultPosition,
                        wxSize (200, -1),
                        0, valText);

      // button
      wxButton * button =
        CreateEllipsisButton(this, ID_StandardExplorerLookup);

      // arguments
      wxStaticText * labelArgs = new wxStaticText (
        this, -1, _("Program arguments (%1=selected directory):"),
        wxDefaultPosition);
      wxTextValidator valArgs (wxFILTER_NONE, &m_prefs->explorerArgs);
      wxTextCtrl * args = new wxTextCtrl (
        this, -1, wxEmptyString, wxDefaultPosition,
        wxSize (200, -1), 0, valArgs);

      // check
      wxGenericValidator valCheck (&m_prefs->explorerAlways);
      wxCheckBox * check =
        new wxCheckBox (this, -1, _("Use always"),
                        wxDefaultPosition,
                        wxDefaultSize, 0, valCheck);

      // position controls
      wxFlexGridSizer * sizer = new wxFlexGridSizer (2);
      sizer->Add (mTextExplorer, 1,
                  wxALIGN_CENTER | wxEXPAND | wxALL, 5);
      sizer->AddGrowableCol (0);
      sizer->Add (button, 0, wxALIGN_CENTER);
      sizerExplorer->Add (sizer, 1, wxEXPAND);
      sizerExplorer->Add (labelArgs, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
      sizerExplorer->Add (args, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
      sizerExplorer->Add (check, 1, wxEXPAND | wxALL, 5);
    }


    // Diff Tool
    wxStaticBox * boxDiffTool =
      new wxStaticBox (this, -1, _("Diff tool:"));

    wxStaticBoxSizer * sizerDiffTool =
      new wxStaticBoxSizer (boxDiffTool, wxVERTICAL);
    {
      // text ctrl
      wxTextValidator valText (wxFILTER_NONE, &m_prefs->diffTool);
      mTextDiffTool =
        new wxTextCtrl (this, -1, wxEmptyString, wxDefaultPosition,
                        wxSize (200, -1),
                        0, valText);

      // button
      wxButton * button =
        CreateEllipsisButton(this, ID_DiffToolLookup);

      // arguments
      wxStaticText * labelArgs = new wxStaticText (
        this, -1, _("Program arguments (%1=file1, %2=file2):"),
        wxDefaultPosition);
      wxTextValidator valArgs (wxFILTER_NONE, &m_prefs->diffToolArgs);
      wxTextCtrl * args = new wxTextCtrl (
        this, -1, wxEmptyString, wxDefaultPosition,
        wxSize (200, -1), 0, valArgs);

      // position controls
      wxFlexGridSizer * sizer = new wxFlexGridSizer (2);
      sizer->Add (mTextDiffTool, 1,
                  wxALIGN_CENTER | wxEXPAND | wxALL, 5);
      sizer->AddGrowableCol (0);
      sizer->Add (button, 0, wxALIGN_CENTER);
      sizerDiffTool->Add (sizer, 1, wxEXPAND);
      sizerDiffTool->Add (labelArgs, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
      sizerDiffTool->Add (args, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
    }


    // Position main elements
    wxBoxSizer *panelsizer = new wxBoxSizer (wxHORIZONTAL);

    // Left column
    wxBoxSizer *leftsizer = new wxBoxSizer (wxVERTICAL);
    panelsizer->Add (leftsizer, 1, wxALL | wxALIGN_TOP, 10);

    leftsizer->Add (sizerEditor, 0, wxEXPAND);
    leftsizer->Add (5, 5);
    leftsizer->Add (sizerExplorer, 0, wxEXPAND);
    leftsizer->Add (5, 5);
    leftsizer->Add (sizerDiffTool, 0, wxEXPAND);

    SetSizer (panelsizer);
    SetAutoLayout (true);
  }

  DECLARE_EVENT_TABLE ()
};


BEGIN_EVENT_TABLE (ProgramsPanel, wxPanel)
  EVT_BUTTON (ID_StandardEditorLookup,
    ProgramsPanel::OnStandardEditorLookup)
  EVT_BUTTON (ID_StandardExplorerLookup,
    ProgramsPanel::OnStandardFileExplorerLookup)
  EVT_BUTTON (ID_DiffToolLookup,
  ProgramsPanel::OnDiffToolLookup)
END_EVENT_TABLE ()

/* AuthPanel **************************************************************/
class AuthPanel : public wxPanel
{
public:
  AuthPanel (wxWindow * parent, Preferences * prefs)
    : wxPanel (parent), m_prefs (prefs)
  {
    wxBoxSizer * mainsizer = new wxBoxSizer (wxVERTICAL);
    {
      wxGenericValidator valCheck (&m_prefs->authPerBookmark);
      wxCheckBox * check =
        new wxCheckBox (this, -1,
                        _("Different login for each bookmark in the bookmarks list"),
                        wxDefaultPosition,
                        wxDefaultSize, 0, valCheck);
      mainsizer->Add (20, 20);
      mainsizer->Add (check, 0, wxALL, 5);
    }

    SetSizer (mainsizer);
    SetAutoLayout (true);
  }
private:
  Preferences * m_prefs;
};

/* PreferencesDlg *********************************************************/

BEGIN_EVENT_TABLE (PreferencesDlg, wxDialog)
END_EVENT_TABLE ()

/**
 * The preference data, edited in the dialog.
 */
struct PreferencesDlg::Data
{
public:
  wxWindow * window;
  wxNotebook * notebook;
  Preferences * prefs;

  /**
   * constructor
   */
  Data(wxWindow * wnd, Preferences * pref)
    : window (wnd), prefs (pref)
  {

    // Create the top-level controls in the dialog.
    // Each page will create its own controls.

    wxBoxSizer *topsizer = new wxBoxSizer (wxVERTICAL);
    wxBoxSizer *button_sizer = new wxBoxSizer (wxHORIZONTAL);

    notebook = new wxNotebook (wnd, -1, wxDefaultPosition, wxDefaultSize);
    wxNotebookSizer *nbs = new wxNotebookSizer (notebook);

    // Add the pages
    GeneralPanel *generalPanel = new GeneralPanel (notebook, prefs);
    notebook->AddPage (generalPanel, _("General"));

    ProgramsPanel *programsPanel = new ProgramsPanel (notebook, prefs);
    notebook->AddPage (programsPanel, _("Programs"));

    AuthPanel * authPanel = new AuthPanel (notebook, prefs);
    notebook->AddPage (authPanel, _("Authentication"));

    // buttons
    button_sizer->Add (
      new wxButton (wnd, wxID_OK, _("OK")),
      0, wxALL, 10);
    button_sizer->Add (
      new wxButton (wnd, wxID_CANCEL, _("Cancel")),
      0, wxALL, 10);

    // Externals
    // no useful entries in here yet
    //ExternalsPanel *externalsPanel = ExternalsPanel::Create (notebook);
    //notebook->AddPage (externalsPanel, _("Externals"));

    topsizer->Add (nbs, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 6);
    topsizer->Add (button_sizer, 0, wxALIGN_CENTER);

    // Order is important here:
    wnd->SetSizer (topsizer);

    // Must precede call to Fit to work on Windows.
    wnd->SetAutoLayout (true);
    topsizer->SetSizeHints (wnd);
    topsizer->Fit (wnd);
  }

  bool
  TransferDataFromWindow ()
  {
    bool b = true;
    // Transfer data for every page. This doesn't happen by default.
    if (notebook)
    {
      for (int i = 0; i < notebook->GetPageCount (); i++)
        b = b && notebook->GetPage (i)->TransferDataFromWindow ();
    }
    return b;
  }

  bool
  TransferDataToWindow ()
  {
    bool b = true;
    // Transfer data for every page. This doesn't happen by default.
    if (notebook)
    {
      for (int i = 0; i < notebook->GetPageCount (); i++)
        b = b && notebook->GetPage (i)->TransferDataToWindow ();
    }
    return b;
  }
};

PreferencesDlg::PreferencesDlg (wxWindow* parent, Preferences * prefs)
  : wxDialog (parent, -1, _("Preferences"),
      wxDefaultPosition, wxDefaultSize,
      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, prefs);

  CentreOnParent ();
}

PreferencesDlg::~PreferencesDlg ()
{
  delete m;
}

/**
 * A specialised version of TransferDataFromWindow that calls
 * TransferDataFromWindow for each panel in the dialog. Sadly this
 * is not the default base class behaviour.
 * @return true if transfer succeeded.
 */
bool
PreferencesDlg::TransferDataFromWindow ()
{
  bool b = wxDialog::TransferDataFromWindow ();
  b = b && m->TransferDataFromWindow ();

  return b;
}

/**
 * A specialised version of TransferDataToWindow that calls
 * TransferDataToWindow for each panel in the dialog. Sadly this
 * is not the default base class behaviour.
 * @return true if transfer succeeded.
 */
bool
PreferencesDlg::TransferDataToWindow ()
{
  bool b = wxDialog::TransferDataToWindow ();
  b = b && m->TransferDataToWindow ();

  return b;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
