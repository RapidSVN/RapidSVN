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

#ifndef _SVNCPP_APR_H_
#define _SVNCPP_APR_H_

/**
 * SvnCpp namespace.
 */
namespace svn
{

	/**
	 * APR class. Include this class in your application for apr
	 * support.
	 */
	class Apr
	{
	public:
		/**
		 * Default constructor. Initializes APR
		 */
		Apr ();

		/**
		 * Destructor. Terminates APR
		 */
		~Apr ();

	private:
		/** Disallow copy constructor */
		Apr (const Apr &);

		/** Disallow assignment operator */
		Apr &
		operator = (const Apr &);
	};
}

#endif