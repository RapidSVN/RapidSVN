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
#ifndef _WORKBENCH_H_
#define _WORKBENCH_H_

// forward declarations
class Project;
namespace svn
{
  class Context;
};

class Workbench
{
public:
  /**
   * default constructor
   */
  Workbench ();

  /**
   * destructor
   */
  virtual ~Workbench ();

  /**
   * add a project to the workbench. The list
   * of projects is unique. So if the project
   * already exists nothing happens
   */
  void
  AddProject (const char * name);

  /**
   * return the number of projects in the
   * workbench
   *
   * @return number of projects
   */
  const size_t
  Count () const;

  /**
   * return the project at the @a index
   *
   * @param index 0 based index
   * @return Project name at this index
   */
  const char *
  GetProject (const size_t index) const;

  /**
   * returns the authentication context
   * of the project at @a index.
   *
   * @param index 0-based index
   * @return authentication context
   */
  svn::Context *
  GetContext (const size_t index);

  /**
   * returns the authentication context
   * for the given @a path
   *
   * @param path to the project
   * @return authentication context
   */
  svn::Context *
  GetContext (const char * path);

  /**
   * remove all projects from the workbench
   */
  void
  Clear ();

  /**
   * remove project with given @a path
   *
   * @param path project path
   * @retval true success
   */
  bool
  RemoveProject (const char * path);

  /**
   * setting whether the authenticaion will
   * be remembered for each project
   */
  void
  SetAuthPerProject (const bool value);

  const bool
  GetAuthPerProject () const;

private:
  /**
   * hide implementation details
   */
  struct Data;
  Data * m;

  /** 
   * disallow copy constructor
   */
  Workbench (const Workbench &);

  /**
   * disallow assignment operator
   */
  const Workbench &
  operator = (const Workbench &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
