/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
#ifndef _SVN_EXECUTABLES_H_INCLUDED_
#define _SVN_EXECUTABLES_H_INCLUDED_

#include "wx/setup.h"
#include "wx/string.h"

#ifdef __WXMSW__

#define SVNADMIN_EXECUTABLE wxT("svnadmin.exe")
#define SVN_EXECUTABLE wxT("svn.exe")

#else

#define SVNADMIN_EXECUTABLE wxT("svnadmin")
#define SVN_EXECUTABLE wxT("svn")

#endif


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
