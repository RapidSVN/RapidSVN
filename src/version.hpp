/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _RAPIDSVN_VERSION_H_INCLUDED_
#define _RAPIDSVN_VERSION_H_INCLUDED_

#define APPLICATION_NAME wxT("RapidSVN")

#define RAPIDSVN_COPYRIGHT "(C) 2002-2008 The RapidSvn Group. All rights reserved."

/* Symbols that define the version number.
 * The numbers are maintained in the file version.xml (in the
 * main project directory). Before creating a release, you should
 * run <main project directiry>/tools/check-version.py to make
 * sure all the places with version information are updated.
 */

/* Version numbers: <major>.<minor>.<micro>-<patch> */

/**
 * Major version number.
 */
#define RAPIDSVN_VER_MAJOR 0

/** 
 * Minor version number.
 */
#define RAPIDSVN_VER_MINOR 9

/** 
 * Micro version number. 
 */
#define RAPIDSVN_VER_MICRO 7

/**
 * Patch string. Can be empty
 */
#define RAPIDSVN_VER_PATCH 0

/**
 * Combined version string
 */
#define RAPIDSVN_VER_STR "0.9.7"

/* Symbolic name of the milestone
   Modify whenever Minor number is changed. */
#define RAPIDSVN_VER_MILESTONE "0.9.8"

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
