/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997
 *	Sleepycat Software.  All rights reserved.
 */

#include "config.h"

#ifndef lint
static const char sccsid[] = "@(#)os_stat.c	10.8 (Sleepycat) 10/25/97";
#endif /* not lint */

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <string.h>
#endif

#include "db_int.h"
#include "common_ext.h"

/*
 * __os_exists --
 *	Return if the file exists.
 *
 * PUBLIC: int __os_exists __P((const char *, int *));
 */
int
__os_exists(path, isdirp)
	const char *path;
	int *isdirp;
{
	struct stat sb;

	if (stat(path, &sb) != 0)
		return (errno);
	if (isdirp != NULL)
		*isdirp = S_ISDIR(sb.st_mode);
	return (0);
}

/*
 * __os_ioinfo --
 *	Return file size and I/O size; abstracted to make it easier
 *	to replace.
 *
 * PUBLIC: int __os_ioinfo __P((const char *, int, off_t *, off_t *));
 */
int
__os_ioinfo(path, fd, sizep, iop)
	const char *path;
	int fd;
	off_t *sizep, *iop;
{
	struct stat sb;

	if (fstat(fd, &sb) == -1)
		return (errno);

	/* Return the size of the file. */
	if (sizep != NULL)
		*sizep = sb.st_size;

	/*
	 * Return the underlying filesystem blocksize, if available.  Default
	 * to 8K on the grounds that most OS's use less than 8K as their VM
	 * page size.
	 */
#ifdef HAVE_ST_BLKSIZE
	if (iop != NULL)
		*iop = sb.st_blksize;
#else
	if (iop != NULL)
		*iop = 8 * 1024;
#endif
	return (0);
}
