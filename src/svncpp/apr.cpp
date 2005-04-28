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

// apr
#include "apr_general.h"

// svncpp
#include "svncpp/apr.hpp"


/**
 * SvnCpp namespace.
 */
namespace svn
{
	Apr::Apr ()
	{
		apr_initialize ();
	}

	Apr::~Apr ()
	{
		apr_terminate ();
	}
}
