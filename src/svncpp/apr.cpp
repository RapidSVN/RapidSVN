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

// apr
#include "apr_general.h"

// svncpp
#include "svncpp/apr.hpp"


/**
 * SvnCpp namespace.
 */
namespace svn
{

	/** 
	 * Counter that is used by @a Apr::Apr and @a Apr::~Apr to
	 * count the number of instances.
	 */

	static int mAprCount = 0;

	Apr::Apr ()
	{
		if (mAprCount == 0)
			apr_initialize ();

		mAprCount++;
	}

	Apr::~Apr ()
	{
		mAprCount--;

		if (mAprCount == 0)
			apr_terminate ();
	}
}
