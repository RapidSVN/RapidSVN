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
#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/valgen.h"

// app
#include "diff_data.hpp"
#include "diff_dlg.hpp"
#include "utils.hpp"


/** event IDs for the controls used in this file */
enum
{
  ID_UseLatest = wxID_HIGHEST,
  ID_Browse,
  ID_UseDate,
  ID_Date,
  ID_UseRevision,
  ID_Revision,
  ID_UseUrl,
  ID_Url,
  ID_CompareType,
  ID_RevisionOne,
  ID_RevisionTwo
};


/* custom event to forward necessary updates */
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVENT_UPDATE, wxEVT_USER_FIRST)
END_DECLARE_EVENT_TYPES()


DEFINE_EVENT_TYPE(wxEVENT_UPDATE)


#define EVENT_UPDATE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVENT_UPDATE, id, -1, \
        (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, \
        (wxObject *) NULL \
    ),


/**
 * Panel that lets the user select either a revision or a date and
 * optionally another URL (if she doesnt want to compare against
 * the same URL as the working copy)
 */
class RevisionPanel : public wxPanel
{
public:
  /** Constructor */
  RevisionPanel (wxWindow * parent, 
                wxWindowID id, 
                const wxString & title)
   : wxPanel (parent, id, wxDefaultPosition, wxDefaultSize)
  {
    InitControls (title);
    CheckControls ();
  }


  /** 
   * Checks the controls for validity and
   * enables/disables controls as result of this check
   */
  void
  CheckControls ()
  {
    CheckRevisionCtrls ();
    CheckDateCtrls ();
    CheckUrlCtrls ();
  }


  /**
   * returns whether the controls contain valid entries
   *
   * @retval true everything is fine
   */
  bool
  IsValid ()
  {
    bool valid = true;

    if (mRadioUseRevision->GetValue ())
    {
      if (!mCheckUseLatest->GetValue ())
        valid = CheckRevision (mTextRevision->GetValue ());
    }
    else 
      valid = CheckDateTime (mTextDate->GetValue ());

    if (valid)
      if (mCheckUseUrl->GetValue ())
        valid = mTextUrl->GetValue ().Trim ().Length () > 0;

    return valid;
  }


  const svn::Revision 
  GetRevision () const
  {
    if (mRadioUseRevision->GetValue ())
    {
      if (mCheckUseLatest->GetValue ())
        return svn::Revision (svn::Revision::HEAD);
      else
      {
        svn_revnum_t revnum;
        ParseRevision (mTextRevision->GetValue (), revnum);
        return svn::Revision (revnum);
      }
    }
    else
    {
      apr_time_t time=0;

      ParseDateTime (mTextDate->GetValue (), time);
      return svn::Revision (time);
    }
  }


  bool 
  GetUseUrl () const
  {
    return mCheckUseUrl->GetValue ();
  }


  const wxString 
  GetUrl () const
  {
    return mTextUrl->GetValue ();
  }

private:
  /** radio button: if checked use revision */
  wxRadioButton * mRadioUseRevision;

  /** text control with the revision number */
  wxTextCtrl * mTextRevision;

  /** 
   * check box: if not checked use revision number
   * (otherwise HEAD)
   */
  wxCheckBox * mCheckUseLatest;

  /** radio button: if checked use date */
  wxRadioButton * mRadioUseDate;

  /** text control with a date */
  wxTextCtrl * mTextDate;

  /** check box: use URL if checked */
  wxCheckBox * mCheckUseUrl;

  /** text control for an optional URL */
  wxTextCtrl * mTextUrl;

  /** browse button if the user wants to search for a local file */
  wxButton * mButtonBrowse;


  /** Initialize and position the controls for the panel */
  void
  InitControls (const wxString & title)
  {
    wxFlexGridSizer * gridSizer = new wxFlexGridSizer (3, 5, 5);
    gridSizer->AddGrowableCol (1);

    // first row: revision
    mRadioUseRevision = new wxRadioButton (
      this, ID_UseRevision, _("Revision:"));
    mRadioUseRevision->SetValue (true);
    mTextRevision = new wxTextCtrl (this, ID_Revision, "");
    mCheckUseLatest = new wxCheckBox (
      this, ID_UseLatest, _("Use latest"));
    mCheckUseLatest->SetValue (true);
    wxBoxSizer * revisionSizer = new wxBoxSizer (wxHORIZONTAL);
    revisionSizer->Add (mTextRevision);
    revisionSizer->Add (mCheckUseLatest, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);

    gridSizer->Add (mRadioUseRevision);
    gridSizer->Add (revisionSizer);
    gridSizer->Add (0,0);

    // second row: date
    mRadioUseDate = new wxRadioButton (
      this, ID_UseDate, _("Date:"));
    mTextDate = new wxTextCtrl (this, ID_Date, "");

    gridSizer->Add (mRadioUseDate);
    gridSizer->Add (mTextDate);
    gridSizer->Add (0,0);

    // third row: url
    mCheckUseUrl = new wxCheckBox (
      this, ID_UseUrl, _("Use Url/Path:"));
    mTextUrl = new wxTextCtrl (this, ID_Url, "");
    mButtonBrowse = CreateEllipsisButton (this, ID_Browse);
    gridSizer->Add (mCheckUseUrl);
    gridSizer->Add (mTextUrl, 0, wxEXPAND);
    gridSizer->Add (mButtonBrowse);

    // create the static box that surrounds the controls
    // and add those controls
    wxSizer * mainSizer = new wxStaticBoxSizer (
      new wxStaticBox (this, -1, title),
      wxVERTICAL);
    mainSizer->Add (gridSizer, 0, wxEXPAND);
  
    this->SetAutoLayout (true);
    this->SetSizer (mainSizer);

    mainSizer->SetSizeHints (this);
    mainSizer->Fit (this);
  }


  /** checks the revision controls for validity */
  void
  CheckRevisionCtrls ()
  {
    if (!mRadioUseRevision->GetValue ())
    {
      mTextRevision->Enable (false);
      mCheckUseLatest->Enable (false);

      return;
    }

    mCheckUseLatest->Enable (true);
    mTextRevision->Enable (!mCheckUseLatest->GetValue ());
  }


  /** checks the date control for validity */
  void 
  CheckDateCtrls ()
  {
    mTextDate->Enable (mRadioUseDate->GetValue ());
  }


  /** checks the url controls for validity */
  void
  CheckUrlCtrls ()
  {
    if (!mCheckUseUrl->GetValue ())
    {
      mTextUrl->SetValue ("");
      mTextUrl->Enable (false);
      mButtonBrowse->Enable (false);
      return;
    }

    mTextUrl->Enable (true);
    mButtonBrowse->Enable (true);
  }


  /** 
   * If anything has changed in the form,
   * e.g. a text has been entered, a radio button
   * or checkbox has been clicked, this method 
   * is executed to check all the controls for validity.
   * 
   * An wxEVENT_UPDATE event is then sent to the parent of
   * this panel.
   */
  void 
  OnCommand (wxCommandEvent & event)
  {
    CheckControls ();

    wxCommandEvent updateEvent (wxEVENT_UPDATE);
    wxPostEvent (GetParent (), updateEvent);
  }


private:
  DECLARE_EVENT_TABLE ()
};


BEGIN_EVENT_TABLE (RevisionPanel, wxPanel)
  EVT_CHECKBOX (ID_UseUrl, RevisionPanel::OnCommand)
  EVT_CHECKBOX (ID_UseLatest, RevisionPanel::OnCommand)
  EVT_RADIOBUTTON (ID_UseRevision, RevisionPanel::OnCommand)
  EVT_RADIOBUTTON (ID_UseDate, RevisionPanel::OnCommand)
  EVT_TEXT (ID_Revision, RevisionPanel::OnCommand)
  EVT_TEXT (ID_Date, RevisionPanel::OnCommand)
END_EVENT_TABLE ()


/**
 * This panel contains all the controls relevant for
 * the diff dialog. We are using a separate class to
 * hide implementation details 
 */
class DiffDlg::Data : public wxPanel
{
public:
  wxComboBox * mComboCmpType;
  RevisionPanel * mRevisionOne;
  RevisionPanel * mRevisionTwo;

  /** Constructor */
  Data (wxWindow * parent)
    : wxPanel (parent), mParent (parent)
  {
    InitControls ();
    CheckControls ();
  }


  const DiffData
  GetDiffData () const
  {
    int sel = mComboCmpType->GetSelection ();
    DiffData diffData;

    switch (sel)
    {
    case 0:
      diffData.compareType = DiffData::CMP_WC_WITH_SAME_REV;
      break;

    case 1:
      diffData.compareType = DiffData::CMP_WC_WITH_DIFFERENT_REV;
      diffData.revision1 = mRevisionOne->GetRevision ();
      break;

    default:
      diffData.compareType = DiffData::CMP_TWO_REV;
      diffData.revision1 = mRevisionOne->GetRevision ();
      diffData.revision2 = mRevisionTwo->GetRevision ();
    }
    return diffData;
  }


private:
  wxWindow * mParent;
  wxButton * mButtonOK;

  void 
  InitControls ()
  {
    // first row: label + combo with selection of options
    wxFlexGridSizer * typeSizer = new wxFlexGridSizer (2, 5, 5);
    {
      wxStaticText * label = new wxStaticText (
        this, -1, _("Compare:"));

      const wxString choices [] = {
        _("Working copy against same remote revision"),
        _("Working copy against different remote revision/date"),
        _("Two revisions/dates against each other")
      };

      mComboCmpType = new wxComboBox (
        this, ID_CompareType, choices [0], 
        wxDefaultPosition, wxDefaultSize, 
        WXSIZEOF (choices), choices,
        wxCB_READONLY);

      typeSizer->Add (label);
      typeSizer->Add (mComboCmpType);
    }

    // second row: first revision/url
    mRevisionOne = new RevisionPanel (
      this, ID_RevisionOne, _("Revision or date #&1:"));

    // third row: second revision/url
    mRevisionTwo = new RevisionPanel (
      this, ID_RevisionTwo, _("Revision or date #&2:"));

    // fourth row: buttons
    wxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    {
      mButtonOK = new wxButton (this, wxID_OK, _("OK"));
      wxButton * buttonCancel = new wxButton (this, wxID_CANCEL, _("Cancel"));

      buttonSizer->Add (mButtonOK, 0, wxALL, 5);
      buttonSizer->Add (buttonCancel, 0, wxALL, 5);
    }

    // create the static box for the diff options
    wxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
    mainSizer->Add (typeSizer);
    mainSizer->Add (mRevisionOne, 0, wxEXPAND);
    mainSizer->Add (mRevisionTwo, 0, wxEXPAND);
    mainSizer->Add (buttonSizer, 0, wxALIGN_CENTER);

    this->SetAutoLayout (true);
    this->SetSizer (mainSizer);

    mainSizer->SetSizeHints (this);
    mainSizer->Fit (this);
  }


  void
  CheckControls ()
  {
    int sel = mComboCmpType->GetSelection ();
    mRevisionOne->Enable (sel > 0);
    mRevisionTwo->Enable (sel > 1);

    mButtonOK->Enable (IsValid ());
  }


  bool 
  IsValid () const
  {
    bool valid = true;

    if (mRevisionOne->IsEnabled ())
      valid &= mRevisionOne->IsValid ();

    if (mRevisionTwo->IsEnabled ())
      valid &= mRevisionTwo->IsValid ();

    return valid;
  }


  void 
  OnCommand (wxCommandEvent & event)
  {
    CheckControls ();
  }


private:
  DECLARE_EVENT_TABLE ()
};


BEGIN_EVENT_TABLE (DiffDlg::Data, wxPanel)
  EVT_COMBOBOX (ID_CompareType, Data::OnCommand)
  EVENT_UPDATE (-1, Data::OnCommand)
END_EVENT_TABLE ()


DiffDlg::DiffDlg (wxWindow * parent)
  : wxDialog (parent, -1, _("Diff"), wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);

  // Add all sizers to main sizer
  wxBoxSizer *mainSizer = new wxBoxSizer (wxVERTICAL);
  mainSizer->Add (m, 0, wxALL, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);

  CentreOnParent();
}


DiffDlg::~DiffDlg ()
{
}


const DiffData
DiffDlg::GetData () const
{
  return m->GetDiffData ();
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
