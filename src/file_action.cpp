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

#include "include.hpp"
#include "file_action.hpp"

FileAction::FileAction (wxFrame * frame)
    : m_mainFrame (frame)
{
  m_tracer = NULL;
  m_ownTracer = FALSE;
}

FileAction::~FileAction ()
{
}

void
FileAction::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m_mainFrame, event);

}

void
FileAction::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m_mainFrame, event);

}

bool
FileAction::PerformAction()
{
  bool success = PerformUI();

  //see if the user cancelled the UI
  if (!success)
  {
    //if so clean up and return
    delete this;

    return false;
  }

  //user didn't cancel UI, create a new thread helper
  FileActionThreadHelper * helper = 
    new FileActionThreadHelper(m_mainFrame, this);
  
  //let it go
  helper->Create();
  helper->Run();

  //once the thread finishes, it will delete us!
  return true;
}

/**
* Create a detatched thread for the file action helper.
*/
FileActionThreadHelper::FileActionThreadHelper (wxFrame * mainFrame, 
                                                FileAction * action)
:wxThread ()
{
  m_action = action;
  m_mainFrame = mainFrame;
  m_mainFrame->SetCursor(wxCURSOR_ARROWWAIT);
}

FileActionThreadHelper::~FileActionThreadHelper ()
{
  delete m_action;
  m_mainFrame->SetCursor(wxCURSOR_ARROW);
}

void *
FileActionThreadHelper::Entry ()
{
  //perform the action
  m_action->Perform ();

  return NULL;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
