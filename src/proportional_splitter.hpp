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
#ifndef _PROPORTIONAL_SPLITTER_HPP_INCLUDED_
#define _PROPORTIONAL_SPLITTER_HPP_INCLUDED_

#include "wx/wx.h"
#include "wx/splitter.h"

/**
 * Splitter class that allows for selecting how the change should be
 * distributed between the child windows when the splitter window is resized
 * (NOT when the sash is moved)
 */
class ProportionalSplitterWindow : public wxSplitterWindow
{
public:
  ProportionalSplitterWindow (float proportion, wxWindow *parent, 
    wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, 
    const wxSize& size = wxDefaultSize, long style = wxSP_3D, 
    const wxString& name = wxT("splitter"));

  /**
   * We need to update the old length when the window is resplit, as
   * orientation may have changed
   */
  bool SplitVertically(wxWindow *window1, wxWindow *window2, int sashPosition = 0);
  bool SplitHorizontally(wxWindow *window1, wxWindow *window2, int sashPosition = 0);

protected:
  /**
   * 1.0f = 100% to the first, 0.0f = 100% to the second
   */
  float m_proportion;
  int m_old_length_tot;

  void OnSize (wxSizeEvent & event);

  /*
   * Get the relevant length component from the given size
   */
  int GetLength(const wxSize & size) const;

private:
  DECLARE_EVENT_TABLE();
};

#endif //_PROPORTIONAL_SPLITTER_HPP_INCLUDED_
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
