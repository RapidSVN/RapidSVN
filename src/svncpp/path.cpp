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

// subversion api
#include "svn_path.h"

// apr api
#include "apr_file_io.h"

// svncpp 
#include "svncpp/path.hpp"
#include "svncpp/pool.hpp"


namespace svn
{
  Path::Path (const char * path)
  {
    init (path);
  }

  Path::Path (const std::string & path) 
  {
    init (path.c_str ());
  }

  Path::Path (const Path & path) 
  {
    init (path.c_str ());
  }

  void
  Path::init (const char * path)
  {
    Pool pool;

    if (path == 0)
      m_path = "";
    else
    {
      const char * int_path = 
        svn_path_internal_style (path, pool.pool () );
    
      m_path = int_path;
    }
  }

  const std::string &
  Path::path () const
  {
    return m_path;
  }

  const char * 
  Path::c_str() const
  {
    return m_path.c_str ();
  }

  Path& 
  Path::operator=(const Path & path)
  {
    if (this == &path)
      return *this;
    m_path = path.c_str();
    return *this;
  }

  bool
  Path::isset () const
  {
    return m_path.length () > 0;
  }

  void
  Path::addComponent (const char * component)
  {
    Pool pool;

    const char * newPath =
      svn_path_url_add_component (m_path.c_str (),
                                  component,
                                  pool);
    m_path = newPath;
  }


  void 
  Path::addComponent (const std::string & component)
  {
    addComponent (component.c_str ());
  }


  void
  Path::split (std::string & dirpath, std::string & basename) const
  {
    Pool pool;

    const char * cdirpath;
    const char * cbasename;

    svn_path_split (m_path.c_str (), &cdirpath, &cbasename, pool);

    dirpath = cdirpath;
    basename = cbasename;
  }


  void
  Path::split (std::string & dir, std::string & filename, std::string & ext) const
  {
    std::string basename;

    // first split path into dir and filename+ext
    split (dir, basename);

    // next search for last .
    size_t pos = basename.find_last_of (".");
    if (pos == std::string::npos)
    {
      filename = basename;
      ext = "";
    }
    else
    {
      filename = basename.substr (0, pos);
      ext = basename.substr (pos);
    }
  }


  Path
  Path::getTempDir ()
  {
    const char * tempdir;
    Pool pool;

    apr_temp_dir_get (&tempdir, pool);

    return tempdir;
  }


  size_t 
  Path::length () const
  {
    return m_path.length ();
  }


  std::string
  Path::native () const
  {
    Pool pool;

    return svn_path_local_style (m_path.c_str (), pool);
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
