/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#ifndef _DND_DLG_H_INCLUDED_
#define _DND_DLG_H_INCLUDED_

// app
#include "rapidsvn_generated.h"

class DragAndDropDlg : public DragAndDropDlgBase
{
public:
  /** one of the possible actions for drag'n'drop */
  enum {
    IMPORT,
    COPY,
    COPY_MOVE
  };

  enum
  {
    RESULT_CANCEL=wxID_CANCEL,
    RESULT_IMPORT=wxID_HIGHEST,
    RESULT_COPY,
    RESULT_MOVE
  };

  DragAndDropDlg(wxWindow *parent, wxString src, wxString dest, int action);


protected: // event handlers inherited from DragNDropDlgBase
  virtual void
  OnButtonImport(wxCommandEvent & event);

  virtual void
  OnButtonMove(wxCommandEvent & event);

  virtual void
  OnButtonCopy(wxCommandEvent & event);

private:
  struct Data; 
  Data * m;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
