/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _LISTENER_H
#define _LISTENER_H

// svncpp
#include "svncpp/context_listener.hpp"
#include "svncpp/path.hpp"
#include "svncpp/targets.hpp"

// wxwindows
#include "wx/string.h"

// forward declarations
class Tracer;
class wxWindow;
namespace svn
{
  class Context;
}


/**
 * Class that listens to the @a svn::ContextListener interface
 * and forwards information to the app and requests informations
 * like authentication and such
 */
class Listener : public svn::ContextListener
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  Listener (wxWindow * parent);


  /**
   * destructor
   */
  virtual ~Listener ();


  /**
   * @return Tracer instance
   */
  Tracer * GetTracer ();


  /**
   * Sets the tracer passed as an argument.
   * If own is TRUE, then the @a Listener class
   * is responsible for deleting the tracer.
   */
  void SetTracer (Tracer * t, bool own);


  /**
   * set actions parent window
   *
   * @param parent the parent that will receive events
   */
  void SetParent (wxWindow * parent);


  /**
   * @return parent
   */
  wxWindow * GetParent ();


  /**
   * sets the context 
   *
   * @param context
   */
  void SetContext (svn::Context * context);


  /**
   * @return the context of the action
   */
  svn::Context *
  GetContext ();


  /**
   * @see svn::ContextListener
   */
  virtual bool 
  contextGetLogin (const std::string & realm,
                   std::string & username, 
                   std::string & password,
				           bool & maySave);

  /**
   * @see svn::ContextListener
   */
  virtual void
  contextNotify (const char *path,
                 svn_wc_notify_action_t action,
                 svn_node_kind_t kind,
                 const char *mime_type,
                 svn_wc_notify_state_t content_state,
                 svn_wc_notify_state_t prop_state,
                 svn_revnum_t revision);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextGetLogMessage (std::string & msg);

  /**
   * @see svn::ContextListener
   */
  virtual svn::ContextListener::SslServerTrustAnswer
  contextSslServerTrustPrompt (
    const svn::ContextListener::SslServerTrustData & data,
    apr_uint32_t & acceptedFailures);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextSslClientCertPrompt (std::string & certFile);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextSslClientCertPwPrompt (std::string & password,
                                const std::string & realm, 
                                bool & maySave);


  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextCancel ();


  /**
   * shall the ongoing operation be cancelled?
   *
   * @see isCancelled
   * 
   * @param value
   *  @li true cancel operation
   *  @li false dont cancel
   */
  void
  cancel (bool value);


  /**
   * @see cancel
   * @return check whether the ongoing operation is to be cancelled
   * @retval true cancel
   * @retval false dont cancel
   */
  bool
  isCancelled () const;

protected:
  void Trace (const wxString & msg);

  void TraceError (const wxString & msg);

private:
  struct Data;
  // this structure contains implementation specific data
  Data * m;

  /**
   * disable private default constructor
   */
  Listener ();

  /**
   * disable private copy constructor
   */
  Listener (const Listener &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
