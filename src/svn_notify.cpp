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

// app
#include "tracer.hpp"
#include "svn_notify.hpp"


SvnNotify::SvnNotify (Tracer * tracer) : 
  _tracer(tracer)
{
  sent_first_txdelta = false;
  received_some_change = false;
  is_checkout = false;
  suppress_final_line = false;
}

SvnNotify::~SvnNotify ()
{
}

void
SvnNotify::onNotify (const char *path,
    svn_wc_notify_action_t action,
    svn_node_kind_t kind,
    const char *mime_type,
    svn_wc_notify_state_t content_state,
    svn_wc_notify_state_t prop_state,
    long revision)
{
  switch (action)
  {
  case svn_wc_notify_delete:
  case svn_wc_notify_update_delete:
    {
      received_some_change = true;
      wxString str = wxString::Format ("Deleted  %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_update_add:
    {
      received_some_change = true;
      wxString str = wxString::Format ("Added  %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_restore:
    {
      wxString str = wxString::Format ("Restored %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_revert:
    {
      wxString str = wxString::Format ("Reverted %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_resolved:
    {
      wxString str =
        wxString::Format ("Resolved conflicted state of %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_add:
    /* We *should* only get the MIME_TYPE if PATH is a file.  If we
       do get it, and the mime-type is not in the "text/" grouping,
       note that this is a binary addition.  */
    {
      wxString str;
      wxString mimeType (mime_type);
      if (mime_type
          && (mimeType.Length () > 5) && (mimeType.StartsWith ("text/") != 0))
      {
        str = wxString::Format ("Added  (bin)  %s", path);
      }
      else
      {
        str = wxString::Format ("Added         %s", path);
      }
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_update_update:
    {
      wxString status;
      if (kind == svn_node_file)
      {
        /* Do this only for files, not for dirs, since we don't want
           to treat directory closings as "change" events. */
        received_some_change = true;

        if (content_state == svn_wc_notify_state_conflicted)
          status = "Conflict";
        else if (content_state == svn_wc_notify_state_merged)
          status = "Merged";
        else if (content_state == svn_wc_notify_state_changed)
          status = "Updated";
      }

      if (prop_state == svn_wc_notify_state_conflicted)
        status = "Conflict found";
      else if (prop_state == svn_wc_notify_state_merged)
        status = "Merged";
      else if (prop_state == svn_wc_notify_state_changed)
        status = "Updated";

      if (!((kind == svn_node_dir)
            && ((prop_state == svn_wc_notify_state_unknown)
                || (prop_state == svn_wc_notify_state_unchanged))))
      {
        wxString str = wxString::Format ("%s: %s", status.c_str(), path);
        _tracer->Trace (str);
      }
    }

    break;

  case svn_wc_notify_update_external:
    /* Currently this is used for checkouts and switches too.  If we
       want different output, we'll have to add new actions. */
    {
      wxString str =
        wxString::Format ("Fetching external item into %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_update_completed:
    {
      if (!suppress_final_line)
      {
        if (SVN_IS_VALID_REVNUM (revision))
        {
          if (is_checkout)
          {
            wxString str =
              wxString::Format ("Checked out revision %" SVN_REVNUM_T_FMT ".",
                                revision);
            _tracer->Trace (str);
          }
          else
          {
            if (received_some_change)
            {
              wxString str =
                wxString::
                Format ("Updated to revision %" SVN_REVNUM_T_FMT ".",
                        revision);
              _tracer->Trace (str);
            }
            else
            {
              wxString str =
                wxString::Format ("At revision %" SVN_REVNUM_T_FMT ".",
                                  revision);
              _tracer->Trace (str);
            }
          }
        }
        else                    /* no revision */
        {
          if (is_checkout)
            _tracer->Trace ("Checkout complete.");
          else
            _tracer->Trace ("Update complete.");
        }
      }
    }

    break;

  case svn_wc_notify_commit_modified:
    {
      wxString str = wxString::Format ("Sending        %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_commit_added:
    {
      wxString str;
      wxString mimeType (mime_type);

      if (mime_type
          && (mimeType.Length () > 5) && (mimeType.StartsWith ("text/") != 0))
      {
        str = wxString::Format ("Adding  (bin)  %s", path);
      }
      else
      {
        str = wxString::Format ("Adding         %s", path);
      }
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_commit_deleted:
    {
      wxString str = wxString::Format ("Deleting       %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_commit_replaced:
    {
      wxString str = wxString::Format ("Replacing      %s", path);
      _tracer->Trace (str);
    }
    break;

  case svn_wc_notify_commit_postfix_txdelta:
    if (!sent_first_txdelta)
    {
      _tracer->Trace ("Transmitting file data ");
      sent_first_txdelta = true;
    }

    _tracer->Trace( "." );
    break;

  default:
    break;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
