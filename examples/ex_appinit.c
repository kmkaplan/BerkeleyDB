/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996, 1997
 *	Sleepycat Software.  All rights reserved.
 */

#include "config.h"

#ifndef lint
static const char copyright[] =
"@(#) Copyright (c) 1997\n\
	Sleepycat Software Inc.  All rights reserved.\n";
static const char sccsid[] = "@(#)ex_appinit.c	10.13 (Sleepycat) 10/25/97";
#endif

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include <db.h>

#ifdef macintosh
#define	DATABASE_HOME	":database"
#define	CONFIG_DATA_DIR	"DB_DATA_DIR :database"
#else
#ifdef _WIN32
#define	DATABASE_HOME	"\\home\\database"
#define	CONFIG_DATA_DIR	"DB_DATA_DIR \\database\\files"
#else
#define	DATABASE_HOME	"/home/database"
#define	CONFIG_DATA_DIR	"DB_DATA_DIR /database/files"
#endif
#endif

DB_ENV *db_setup(FILE *);
int	main __P((int, char *[]));

const char
	*progname = "ex_appinit";			/* Program name. */

/*
 * An example of a program using db_appinit(3) to configure its DB
 * environment.
 */
int
main(argc, argv)
	int argc;
	char *argv[];
{
	DB_ENV *dbenv;

	dbenv = db_setup(stderr);
	return (0);
}

DB_ENV *
db_setup(errfp)
	FILE *errfp;
{
	DB_ENV *dbenv;
	char *config[2];

	/* Allocate and initialize the DB environment. */
	if ((dbenv = (DB_ENV *)malloc(sizeof(DB_ENV))) == NULL) {
		fprintf(stderr, "%s: %s\n", progname, strerror(ENOMEM));
		exit (1);
	}
	memset(dbenv, 0, sizeof(DB_ENV));

	/* Output errors to the application's log. */
	dbenv->db_errfile = errfp;
	dbenv->db_errpfx = progname;

	/*
	 * All of the shared database files live in /home/database,
	 * but data files live in /database.
	 */
	config[0] = CONFIG_DATA_DIR;
	config[1] = NULL;

	/*
	 * We want to specify the shared memory buffer pool cachesize,
	 * but everything else is the default.
	 */
	dbenv->mp_size = 64 * 1024;

	/*
	 * We have multiple processes reading/writing these files, so
	 * we need concurrency control and a shared buffer pool, but
	 * not logging or transactions.
	 */
	if ((errno = db_appinit(DATABASE_HOME,
	    config, dbenv, DB_CREATE | DB_INIT_LOCK | DB_INIT_MPOOL)) != 0) {
		fprintf(stderr,
		    "%s: db_appinit: %s\n", progname, strerror(errno));
		exit (1);
	}
	return (dbenv);
}
