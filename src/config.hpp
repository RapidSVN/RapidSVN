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
#ifndef _RAPIDSVN_CONFIG_HPP_INCLUDED_
#define _RAPIDSVN_CONFIG_HPP_INCLUDED_

// wxWidgets
#include "wx/wx.h"


/**
 * Do we want to use the simple single threaded or multi-threaded
 * action worker?
 */
//#define USE_SIMPLE_WORKER

#ifndef USE_SIMPLE_WORKER
#define USE_THREADED_WORKER
#endif

/**
 * Do we want startup tips?
 */
//#define USE_STARTUP_TIPS

/**
 * Do we want tests for debugging?
 */
//#define USE_DEBUG_TESTS

/**
 * Do we want support for online help?
 */
//#define USE_HTML_HELP

// consitency checking with wx settings
#ifdef USE_HTML_HELP
#ifndef wxUSE_WXHTML_HELP
#undef USE_HTML_HELP
#endif
#endif

#ifdef USE_STARTUP_TIPS
#ifndef wxUSE_STARTUP_TIPS
#undef USE_STARTUP_TIPS
#endif
#endif

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
