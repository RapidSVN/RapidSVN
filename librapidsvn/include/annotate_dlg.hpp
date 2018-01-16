/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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
#ifndef _ANNOTATE_DLG_H_INCLUDED_
#define _ANNOTATE_DLG_H_INCLUDED_

#include "rapidsvn_generated.h"

class AnnotateDlg:public AnnotateDlgBase
{
public:
  AnnotateDlg(wxWindow * parent,
              const wxString & caption);

  void AddAnnotateLine(int revision, const wxString & author,
                       const wxString & line);
  void AutoSizeColumn();
  void SetRevisionRange(int revMin, int revMax);
private:
  int revMin, revMax;
  bool UsePalette() const {
    return revMax > revMin;
  }
  wxColour GetBackgroundColour(int rev);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
