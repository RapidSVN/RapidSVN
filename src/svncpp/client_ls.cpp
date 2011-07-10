/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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
// subversion api
#include "svn_client.h"
#include "svn_path.h"
#include "svn_sorts.h"
//#include "svn_utf.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/dirent.hpp"
#include "svncpp/exception.hpp"

#include "m_is_empty.hpp"


static int
compare_items_as_paths(const svn_sort__item_t *a, const svn_sort__item_t *b)
{
  return svn_path_compare_paths((const char *)a->key, (const char *)b->key);
}

namespace svn
{
  static svn_error_t*
  listEntriesFunc(void *baton, const char *path,
        const svn_dirent_t *dirent, const svn_lock_t *lock,
        const char *abs_path, apr_pool_t *pool)
  {
    if (!isEmpty(path))
    {
      DirEntries * entries = static_cast<DirEntries *>(baton);
      entries->push_back(
        DirEntry(path, const_cast<svn_dirent_t *>(dirent), lock));
    }
    return 0;
  }

  DirEntries
  Client::list(const char * pathOrUrl,
               svn_opt_revision_t * revision,
               bool recurse) throw(ClientException)
  {
    Pool pool;

    DirEntries entries;
    svn_error_t * error =
      svn_client_list2(pathOrUrl,
                       revision,
                       revision,
                       recurse ? svn_depth_infinity : svn_depth_immediates,
                       SVN_DIRENT_ALL,
                       true,
                       listEntriesFunc,
                       &entries,
                       *m_context,
                       pool);

    if (error != 0)
      throw ClientException(error);

    return entries;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
