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
   : wxPanel (parent, id, wxDefaultPosition, wxDefaultSize),
     mEnableUrl (true)
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
      svn::DateTime datetime(time);
      return svn::Revision (datetime);
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


  void
  SetRevision (const svn::Revision & revision)
  {
    // set revnum
    if (revision.kind () == svn_opt_revision_date)
    {
      mRadioUseDate->SetValue (true);
      mTextDate->SetValue (
        FormatDateTime (revision.date (), wxT("%c")));
      mTextRevision->SetValue (wxEmptyString);
      mCheckUseLatest->SetValue (true);
    }
    else
    {
      mRadioUseRevision->SetValue (true);
      mTextDate->SetValue (wxEmptyString);

      if (revision.kind () == svn_opt_revision_head)
      {
        mTextRevision->SetValue (wxEmptyString);
        mCheckUseLatest->SetValue (true);
      }
      else
      {
        wxString value;
        value.Printf (wxT("%" SVN_REVNUM_T_FMT), revision.revnum ());
        mTextRevision->SetValue (value);
        mCheckUseLatest->SetValue (false);
      }
    }
  }


  void
  SetUseUrl (bool value)
  {
    mCheckUseUrl->SetValue (value);
  }


  void
  SetUrl (const wxString & url)
  {
    mTextUrl->SetValue (url);
  }


  void
  EnableUrl (bool enable)
  {
    // @todo
  }


private:
  bool mEnableUrl;

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
    mTextRevision = new wxTextCtrl (this, ID_Revision, wxEmptyString);
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
    mTextDate = new wxTextCtrl (this, ID_Date, wxEmptyString);

    gridSizer->Add (mRadioUseDate);
    gridSizer->Add (mTextDate);
    gridSizer->Add (0,0);

    // third row: url
    mCheckUseUrl = new wxCheckBox (
      this, ID_UseUrl, _("Use Url/Path:"));
    mTextUrl = new wxTextCtrl (this, ID_Url, wxEmptyString);
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
      mTextUrl->Enable (false);
      mButtonBrowse->Enable (false);
      return;
    }

    mTextUrl->Enable (mEnableUrl);
    mButtonBrowse->Enable (mEnableUrl);
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


static const DiffData::CompareType COMPARE_TYPES [] = {
  DiffData::WITH_SAME_REVISION,
  DiffData::WITH_DIFFERENT_REVISION,
  DiffData::TWO_REVISIONS};

static const wxString COMPARE_TYPE_LABELS [] =
{
  _("Working copy against same remote revision"),
  _("Working copy against different remote revision/date"),
  _("Two revisions/dates against each other")
};


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
    DiffData diffData;
    diffData.compareType = GetCompareType ();

    switch (diffData.compareType)
    {
    case DiffData::WITH_DIFFERENT_REVISION:
      diffData.revision1 = mRevisionOne->GetRevision ();
      break;

    case DiffData::TWO_REVISIONS:
      diffData.revision1 = mRevisionOne->GetRevision ();
      diffData.revision2 = mRevisionTwo->GetRevision ();
      break;
    case DiffData::WITH_SAME_REVISION:
    default:
      // nothing special
      break;
    }
    return diffData;
  }


  void
  SetData (const DiffData & diffData)
  {
    mRevisionOne->SetRevision (diffData.revision1);
    mRevisionTwo->SetRevision (diffData.revision2);

    SetCompareType (diffData.compareType);
  }


  void
  EnableUrl (bool value)
  {
    mRevisionOne->EnableUrl (value);
    mRevisionTwo->EnableUrl (value);
  }


  void
  AllowCompareTypes (const DiffData::CompareType types [],
                     size_t count)
  {
    if (count == 0)
    {
      AllowCompareTypes ();
      return;
    }

    // remember old selection and clear contents
    DiffData::CompareType oldCompareType = GetCompareType ();
    mComboCmpType->Clear ();

    // otherwise allow only the types that were passed
    // as parameters
    size_t i;

    for (i=0; i<count; i++)
      AddCompareType (types [i]);

    // try to set old selection
    SetCompareType (oldCompareType);
  }


  void
  AllowCompareTypes ()
  {
    // remember old selection and clear contents
    DiffData::CompareType oldCompareType = GetCompareType ();
    mComboCmpType->Clear ();

    // fill list
    AddCompareType (DiffData::WITH_SAME_REVISION);
    AddCompareType (DiffData::WITH_DIFFERENT_REVISION);
    AddCompareType (DiffData::TWO_REVISIONS);

    // try to set remembered value
    SetCompareType (oldCompareType);
  }


private:
  wxWindow * mParent;
  wxButton * mButtonOK;

  void
  InitControls ()
  {
    // first row: label + combo with selection of options
    wxFlexGridSizer * typeSizer = new wxFlexGridSizer (2, 5, 5);
    typeSizer->AddGrowableCol (1);
    {
      wxStaticText * label = new wxStaticText (
        this, -1, _("Compare:"));

      mComboCmpType = new wxComboBox (
        this, ID_CompareType, wxEmptyString, wxDefaultPosition, wxDefaultSize,
        WXSIZEOF (COMPARE_TYPE_LABELS), COMPARE_TYPE_LABELS,
        wxCB_READONLY);

      typeSizer->Add (label);
      typeSizer->Add (mComboCmpType, 0, wxEXPAND);
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
    mainSizer->Add (typeSizer, 0, wxEXPAND);
    mainSizer->Add (mRevisionOne, 0, wxEXPAND);
    mainSizer->Add (mRevisionTwo, 0, wxEXPAND);
    mainSizer->Add (buttonSizer, 0, wxALIGN_CENTER);

    this->SetAutoLayout (true);
    this->SetSizer (mainSizer);

    mainSizer->SetSizeHints (this);
    mainSizer->Fit (this);

    AllowCompareTypes ();
  }


  void
  CheckControls ()
  {
    bool one = false;
    bool two = false;

    switch (GetCompareType ())
    {
    case DiffData::WITH_SAME_REVISION:
      one = two = false;
      break;

    case DiffData::WITH_DIFFERENT_REVISION:
      one = true;
      break;

    case DiffData::TWO_REVISIONS:
      one = two = true;
      break;

    default:
      one = two = false;
    }
    mRevisionOne->Enable (one);
    mRevisionTwo->Enable (two);

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


  /**
   * Add a compare type to the combo box
   *
   * @return success
   * @retval false @a CompareType entry in @a CompareTypeLabels
   *               not found
   */
  bool
  AddCompareType (DiffData::CompareType compareType)
  {
    bool ok = false;
    const int c = WXSIZEOF (COMPARE_TYPES);
    for (int i=0; i < c; i++)
    {
      if (COMPARE_TYPES [i] == compareType)
      {
        mComboCmpType->Append (COMPARE_TYPE_LABELS [i],
                               (void*) & (COMPARE_TYPES [i]));
        ok = true;
        break;
      }
    }

    return ok;
  }


  /**
   * Get selected @a CompareType
   *
   * @retval INVALID_COMPARE_TYPE if nothing was
   *         selected or invalid result of @a GetClientData
   */
  DiffData::CompareType
  GetCompareType () const
  {
    int sel = mComboCmpType->GetSelection ();

    if (sel < 0)
      return DiffData::INVALID_COMPARE_TYPE;

    DiffData::CompareType* ct = (DiffData::CompareType*) mComboCmpType->GetClientData (sel);
    if (!ct)
      return DiffData::INVALID_COMPARE_TYPE;

    return *ct;
  }


  /**
   * Select @a CompareType in the combo-box.
   * If not found, select first entry in the combo-box
   *
   * @return found?
   * @retval true found!
   */
  bool
  SetCompareType (DiffData::CompareType compareType)
  {
    bool found = false;

    const int c = mComboCmpType->GetCount ();

    for (int i=0; i < c; i++)
    {
      DiffData::CompareType* ct = (DiffData::CompareType*) mComboCmpType->GetClientData (i);
      if (ct && compareType == *ct)
      {
        mComboCmpType->SetSelection (i);
        found = true;
        break;
      }
    }

    if (!found)
      mComboCmpType->SetSelection (0);

    CheckControls ();

    return found;
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


void
DiffDlg::SetData (const DiffData & diffData)
{
  m->SetData (diffData);
}


void
DiffDlg::EnableUrl (bool value)
{
  m->EnableUrl (value);
}


void
DiffDlg::AllowCompareTypes (const DiffData::CompareType types [],
                            size_t count)
{
  m->AllowCompareTypes (types, count);
}


void
DiffDlg::AllowCompareTypes ()
{
  m->AllowCompareTypes ();
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
