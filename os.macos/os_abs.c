/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997
 *	Sleepycat Software.  All rights reserved.
 */

#include "config.h"

#ifndef lint
static const char sccsid[] = "@(#)os_abs.c	10.7 (Sleepycat) 10/25/97";
#endif /* not lint */

#ifndef NO_SYSTEM_INCLUDES
#include <string.h>
#endif

#include "db_int.h"

/*
 * __db_abspath --
 *	Return if a path is an absolute path.
 */
int
__db_abspath(path)
	const char *path;
{
	/*
	 * !!!
	 * Absolute pathnames always start with a volume name, which must be
	 * followed by a colon, thus they are of the form:
	 *	volume: or volume:dir1:dir2:file
	 *
	 * Relative pathnames are either a single name without colons or a
	 * path starting with a colon, thus of the form:
	 *	file or :file or :dir1:dir2:file
	 */
	return (strchr(path, ':') != NULL && path[0] != ':');
}
