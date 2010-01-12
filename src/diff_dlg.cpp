/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "wx/datectrl.h"

// app
#include "diff_data.hpp"
#include "diff_dlg.hpp"
#include "utils.hpp"
#include "hist_entries.hpp"
#include "hist_mgr.hpp"
#include "hist_val.hpp"


struct DiffDlg::Data
{
  bool enableUrl;
  size_t compareTypesCount;
  DiffData::CompareType compareTypes [DiffData::COMPARE_TYPE_COUNT-1];
  DiffData diffData;
  wxComboBox * comboCompareTypes;

  struct RevisionControls
  {
    wxRadioButton * radioUseRevision;
    wxTextCtrl * textRevision;
    wxCheckBox * checkUseLatest;
    wxRadioButton * radioUseDate;
    wxDatePickerCtrl * datePicker;
    wxCheckBox * checkUsePath;
    wxComboBox * comboPath;
  } revCtrlsArray[2];
  wxComboBox * comboCompare;

  Data()
      : enableUrl(true), compareTypesCount(0)
  {
  }

  bool
  TransferRevisionFromWindow(int no, bool enabled, svn::Revision & revision)
  {
    struct RevisionControls * revCtrls = &revCtrlsArray[no];

    // first we enable/disable controls according to settings
    revCtrls->radioUseRevision->Enable(enabled);
    bool useRevision = enabled && revCtrls->radioUseRevision->GetValue();
    revCtrls->checkUseLatest->Enable(useRevision);
    bool useLatest = useRevision && revCtrls->checkUseLatest->GetValue();
    revCtrls->textRevision->Enable(enabled && !useLatest);

    revCtrls->radioUseDate->Enable(enabled);
    bool useDate = enabled && revCtrls->radioUseDate->GetValue();
    revCtrls->datePicker->Enable(useDate);

    bool enablePath = enabled && enableUrl;
    revCtrls->checkUsePath->Enable(enablePath);
    bool usePath = enablePath && revCtrls->checkUsePath->GetValue();
    revCtrls->comboPath->Enable(usePath);

    // now transfer values
    bool isValid = true;
    if (revCtrls->radioUseRevision->GetValue())
    {
      if (revCtrls->checkUseLatest->GetValue())
        revision = svn::Revision(svn::Revision::HEAD);
      else
      {
        svn_revnum_t revnum;
        bool isValidRevision = ParseRevision(
                                 revCtrls->textRevision->GetValue(), revnum);
        revision = svn::Revision(revnum);

        if (enabled && !isValidRevision)
          isValid = false;
      }
    }
    else
    {
      apr_time_t time=0;
      apr_time_ansi_put(
        &time, revCtrls->datePicker->GetValue().GetTicks());
      svn::DateTime datetime(time);
      revision =  svn::Revision(datetime);
    }

    return isValid;
  }


  /**
   * Add a compare type to the combo box
   */
  void
  AddCompareType(DiffData::CompareType compareType)
  {
    compareTypes[compareTypesCount] = compareType;
    compareTypesCount++;

    switch (compareType)
    {
    case DiffData::WITH_BASE:
      comboCompare->Append(_("Diff to BASE"));
      break;
    case DiffData::WITH_HEAD:
      comboCompare->Append(_("Diff to HEAD"));
      break;
    case DiffData::WITH_DIFFERENT_REVISION:
      comboCompare->Append(_("Diff to another revision/date"));
      break;
    case DiffData::TWO_REVISIONS:
      comboCompare->Append(_("Diff two revisions/dates"));
      break;
    default:
      comboCompare->Append(wxT("Invalid value"));
    }
  }


  /**
   * Select @a CompareType in the combo-box.
   * If not found, select first entry in the combo-box
   *
   * @return found?
   * @retval true found!
   */
  bool
  SetCompareType(DiffData::CompareType compareType)
  {
    bool found = false;

    diffData.compareType = compareType;
    for (size_t i=0; i < compareTypesCount; i++)
    {
      if (compareTypes[i] == compareType)
      {
        comboCompare->SetSelection(i);
        found = true;
        break;
      }
    }

    if (!found)
    {
      diffData.compareType = compareTypes[0];
      comboCompare->SetSelection(0);
    }

    return found;
  }
};

DiffDlg::DiffDlg(wxWindow * parent, const wxString & selectedUrl)
    : DiffDlgBase(parent, -1, _("Diff"), wxDefaultPosition,
                  wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
    m(new Data())
{
  // Create the control mapping for the use
  // with @ref Data::SetRevision and @ref Data::GetRevision
  m->comboCompare = m_comboCompare;
  m->revCtrlsArray[0].radioUseRevision = m_radioUseRevision1;
  m->revCtrlsArray[0].textRevision = m_textRevision1;
  m->revCtrlsArray[0].checkUseLatest = m_checkUseLatest1;
  m->revCtrlsArray[0].radioUseDate = m_radioUseDate1;
  m->revCtrlsArray[0].datePicker = m_datePicker1;
  m->revCtrlsArray[0].checkUsePath = m_checkUsePath1;
  m->revCtrlsArray[0].comboPath = m_comboPath1;

  m->revCtrlsArray[1].radioUseRevision = m_radioUseRevision2;
  m->revCtrlsArray[1].textRevision = m_textRevision2;
  m->revCtrlsArray[1].checkUseLatest = m_checkUseLatest2;
  m->revCtrlsArray[1].radioUseDate = m_radioUseDate2;
  m->revCtrlsArray[1].datePicker = m_datePicker2;
  m->revCtrlsArray[1].checkUsePath = m_checkUsePath2;
  m->revCtrlsArray[1].comboPath = m_comboPath2;

  m_comboPath1->SetValue(selectedUrl);
  m_comboPath2->SetValue(selectedUrl);
  HistoryValidator valModule1(
    HISTORY_DIFF_URL, &m->diffData.url1, true, true);
  valModule1.SetWindow(this);
  m_comboPath1->SetValidator(valModule1);

  HistoryValidator valModule2(
    HISTORY_DIFF_URL, &m->diffData.url2, true, true);
  valModule2.SetWindow(this);

  m_comboPath2->SetValidator(valModule2);

  TransferDataToWindow();

  CentreOnParent();

  // fill list
  AllowCompareTypes();
  m->SetCompareType(DiffData::WITH_DIFFERENT_REVISION);
  m_radioUseRevision1->SetValue(true);
  m_radioUseRevision2->SetValue(true);

  TransferDataFromWindow();
}

DiffDlg::~DiffDlg()
{
  delete m;
}

const DiffData
DiffDlg::GetData() const
{
  return m->diffData;
}

void
DiffDlg::EnableUrl(bool value)
{
  m->enableUrl = value;

  TransferDataFromWindow();
}

void
DiffDlg::AllowCompareTypes(const DiffData::CompareType types [],
                           size_t count)
{
  if (count == 0)
  {
    AllowCompareTypes();
    return;
  }

  // remember old selection and clear contents
  DiffData::CompareType oldCompareType = m->diffData.compareType;
  m_comboCompare->Clear();
  m->compareTypesCount = 0;

  // otherwise allow only the types that were passed
  // as parameters
  size_t i;

  for (i=0; i<count; i++)
    m->AddCompareType(types [i]);

  // try to set old selection
  m->SetCompareType(oldCompareType);
  TransferDataFromWindow();
}

void
DiffDlg::AllowCompareTypes()
{
  // remember old selection and clear contents
  DiffData::CompareType oldCompareType = m->diffData.compareType;
  m_comboCompare->Clear();
  m->compareTypesCount = 0;

  m->AddCompareType(DiffData::WITH_BASE);
  m->AddCompareType(DiffData::WITH_HEAD);
  m->AddCompareType(DiffData::WITH_DIFFERENT_REVISION);
  m->AddCompareType(DiffData::TWO_REVISIONS);

  m->SetCompareType(oldCompareType);
  TransferDataFromWindow();
}

bool
DiffDlg::TransferDataFromWindow()
{
  bool revision1 = false;
  bool revision2 = false;

  m->diffData.compareType =
    m->compareTypes[m_comboCompare->GetSelection()];

  switch (m->diffData.compareType)
  {
  case DiffData::WITH_DIFFERENT_REVISION:
    revision1 = true;
    revision2 = false;
    break;
  case DiffData::TWO_REVISIONS:
    revision1 = true;
    revision2 = true;
  default:
    ; // Values are already false
  }

  // retrieve revisions / dates
  bool isValid = true;

  if (!m->TransferRevisionFromWindow(
        0, revision1, m->diffData.revision1))
  {
    isValid = false;
  }

  if (!m->TransferRevisionFromWindow(
        1, revision2, m->diffData.revision2))
  {
    isValid = false;
  }

  m->diffData.useUrl1 = m_checkUsePath1->GetValue();
  m->diffData.url1 = m_comboPath1->GetValue();
  m->diffData.useUrl2 = m_checkUsePath2->GetValue();
  m->diffData.url2 = m_comboPath2->GetValue();

  m_buttonOK->Enable(isValid);

  return isValid;
}


void
DiffDlg::OnComboCompare(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnRadioUseRevision1(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnTextRevision1(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnCheckUseLatest1(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnRadioUseDate1(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnDatePicker1(wxDateEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnUsePath1(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnComboPath1(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnRadioUseRevision2(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnTextRevision2(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnCheckUseLatest2(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnRadioUseDate2(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnDatePicker2(wxDateEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnCheckUsePath2(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnComboPath2(wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
}

void
DiffDlg::OnButtonOK(wxCommandEvent& event)
{
  TheHistoryManager.AddEntryToList(HISTORY_DIFF_URL,
                                   m_comboPath1->GetValue());
  TheHistoryManager.AddEntryToList(HISTORY_DIFF_URL,
                                   m_comboPath2->GetValue());

  event.Skip();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */

