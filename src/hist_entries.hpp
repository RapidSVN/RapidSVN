/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#ifndef _HIST_ENTRIES_H_INCLUDED_
#define _HIST_ENTRIES_H_INCLUDED_


// wxWidgets
#include "wx/wx.h"


/**
 * History entry for commit log messages
 */
extern const wxChar HISTORY_COMMIT_LOG [];


/**
 * History entry for repository paths
 */
extern const wxChar HISTORY_REPOSITORY [];


/**
 * History entry for working directory paths
 */
extern const wxChar HISTORY_WORKING_DIRECTORY [];


/**
 * History entry for destinations of copy/move
 * operations
 */
extern const wxChar HISTORY_COPYMOVE_DESTINATION [];


/**
 * History entry for Diff URLs
 */
extern const wxChar HISTORY_DIFF_URL [];


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
