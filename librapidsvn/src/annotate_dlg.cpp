/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// app
#include "annotate_dlg.hpp"
#include "utils.hpp"
#include <math.h>

#define ANNOTATE_RED wxColour(255, 220, 220)
#define ANNOTATE_BLUE wxColour(220, 220, 255)

AnnotateDlg::AnnotateDlg(wxWindow * parent,
                         const wxString & caption)
  : AnnotateDlgBase(parent, -1, caption), revMin(-1), revMax(-1)
{
  m_list->InsertColumn(0, _("Revision"), wxLIST_FORMAT_RIGHT);
  m_list->InsertColumn(1, _("Author"), wxLIST_FORMAT_RIGHT);
  m_list->InsertColumn(2, _("Line"), wxLIST_FORMAT_RIGHT);
  m_list->InsertColumn(3, wxT(""));  // Empty spacer column
  m_list->InsertColumn(4, wxT(""));

  // On Windows, the first column isn't properly right aligned
  //  in the previous InsertColumn call.  Here we set it again
  //  to be sure that it is right aligned
  wxListItem item;
  m_list->GetColumn(0, item);
  item.SetAlign(wxLIST_FORMAT_RIGHT);
  m_list->SetColumn(0, item);

  m_list->SetColumnWidth(0, 75);
  m_list->SetColumnWidth(1, 100);
  m_list->SetColumnWidth(2, 50);
  m_list->SetColumnWidth(3, 10);
  m_list->SetColumnWidth(4, 150);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  // Set size of this dialog to be 5% smaller than parent window.
  wxSize parentSize = parent->GetSize();
  parentSize.DecBy(parentSize.GetWidth() * 0.05);
  this->SetSize(parentSize);

  Layout();
  CentreOnParent();

  Maximize();
}

void
AnnotateDlg::AddAnnotateLine(int revision, const wxString & author,
                             const wxString & line)
{
  int index = m_list->GetItemCount();
  wxString rev = wxString::Format(wxT("%ld"), (long) revision);

  m_list->InsertItem(index, rev);
  m_list->SetItem(index, 1, author);
  m_list->SetItem(index, 2, wxString::Format(wxT("%ld"), (long) index + 1));
  m_list->SetItem(index, 3, wxT(""));
  // Windows displays the link breaks as squares.  Format the line
  //  to avoid that
  wxString formattedLine = line;
  formattedLine.Replace(wxT("\r"), wxT(""));
  formattedLine.Replace(wxT("\n"), wxT(""));
  formattedLine.Replace(wxT("\t"), wxT("  "));
  m_list->SetItem(index, 4, formattedLine);

  if (UsePalette()) {
    wxColour bgCol = GetBackgroundColour(revision);
    m_list->SetItemBackgroundColour(index, bgCol);
  }
}

void
AnnotateDlg::AutoSizeColumn()
{
  m_list->SetColumnWidth(4, wxLIST_AUTOSIZE);
}

void
AnnotateDlg::SetRevisionRange(int revMin, int revMax)
{
  this->revMin = revMin;
  this->revMax = revMax;
}

static int DoRound(double v)
{
  return (int)floor(v + 0.5);
}

wxColour
AnnotateDlg::GetBackgroundColour(int rev)
{
  double fac = (rev - revMin) / ((double)(revMax - revMin));

  wxColour left, right;
  if (fac < 0.5) {
    // relatively old item; interpolate from red to blue
    left = ANNOTATE_RED;
    right = ANNOTATE_BLUE;
    fac *= 2.0; // now between 0 and 1
  }
  else {
    // newer item, interpolate from blue to (bkgnd)
    left = ANNOTATE_BLUE;
    right = m_list->GetBackgroundColour();
    fac = (fac - 0.5) * 2.0; // normalized between 0 and 1
  }
  double fl = 1.0 - fac;
  // now perform interpolation
  wxColour ret((unsigned char) DoRound(fl * left.Red() + fac * right.Red()),
               (unsigned char) DoRound(fl * left.Green() + fac * right.Green()),
               (unsigned char) DoRound(fl * left.Blue() + fac * right.Blue())
              );
  return ret;

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
