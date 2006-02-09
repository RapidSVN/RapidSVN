/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
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
#define USE_SIMPLE_WORKER

#ifndef USE_SIMPLE_WORKER
#define USE_THREADED_WORKER
#endif

/**
 * Do we want startup tips?
 */
//#define USE_STARTUP_TIPS

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
