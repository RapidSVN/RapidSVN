/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library (in the file LGPL.txt); if not, 
 * write to the Free Software Foundation, Inc., 51 Franklin St, 
 * Fifth Floor, Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _SVNCPP_M_SVN_CONVERTER_HPP_
#define _SVNCPP_M_SVN_CONVERTER_HPP_

// subversion api
#include "svn_wc.h"

// svncpp
#include "svncpp/status.hpp"
#include "m_svn_status.hpp"
#include "m_svn_data.hpp"

namespace svn
{
  class Converter
  {
  public:
    Converter () { }

    ~Converter () { }

    Status
    toStatus (const char * path, const SvnStatus * src)
    {
      if (!src)
      {
        m_dst.m->isVersioned = false;
        m_dst.m->status = 0;
      }
      else
      { 
        m_dst.m->isVersioned = src->text_status > svn_wc_status_unversioned;
  
        m_dst.m->status = (SvnStatus *)
          apr_pcalloc (m_dst.m->pool, sizeof (SvnStatus));

        if (src->entry)
          m_dst.m->status->entry = svn_wc_entry_dup (src->entry, m_dst.m->pool);

        m_dst.m->status->text_status = src->text_status;
        m_dst.m->status->prop_status = src->prop_status;
        m_dst.m->status->locked = src->locked;
        m_dst.m->status->copied = src->copied;
        m_dst.m->status->switched = src->switched;
        m_dst.m->status->repos_text_status = src->repos_text_status;
        m_dst.m->status->repos_prop_status = src->repos_prop_status;
    
        // duplicate the contents of repos_lock structure
#if CHECK_SVN_SUPPORTS_LOCK
        if (src->repos_lock)
          m_dst.m->status->repos_lock = svn_lock_dup (src->repos_lock, m_dst.m->pool);
#endif
      }

      if (path)
      {
        m_dst.m->path = path;
      }
      else
      {
        m_dst.m->path = "";
      }

      return m_dst;
    }

  private:
    Status m_dst;
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
