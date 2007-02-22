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

// svncpp
#include "svncpp/info.hpp"
#include "svncpp/path.hpp"
#include "svncpp/pool.hpp"

namespace svn
{
  struct Info::Data
  {
    svn_info_t * info;
    Path path;
    Pool pool;
  
    /** constructor (because of optional param */
    Data (const Path & path_, const svn_info_t * info_ = 0)
      : info (0), path (path_)
    {
      if (info_ != 0)
        info = svn_info_dup (info_, pool);
    }
  
    /** copy constructor */
    Data (const Data * src)
      : info (0), path (src->path)
    {
      if (src->info != 0)
        info = svn_info_dup (src->info, pool);
    }
  };
    
  Info::Info (const Path & path, const svn_info_t * info)
    : m (new Data (path, info))
  {
  }

  Info::Info (const Info & src)
    : m (new Data (src.m))
  {
  }

  Info::~Info ()
  {
    delete m;
  }


  Info &
  Info::operator = (const Info & src)
  {
    if (this != &src)
    {
      delete m;
      m = new Data (src.m);
    }

    return *this;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
