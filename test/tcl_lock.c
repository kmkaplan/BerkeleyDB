/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996, 1997
 *	Sleepycat Software.  All rights reserved.
 */

#include "config.h"

#ifndef lint
static const char sccsid[] = "@(#)tcl_lock.c	10.6 (Sleepycat) 9/28/97";
#endif /* not lint */

/*
 * This file is divided up into 4 sets of functions:
 * 1. The lock command and its support functions.
 * 2. The lock_unlink command.
 * 3. The lock manager widget commands.
 * 4. The lock widget commands.
 */

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#endif
#include <tcl.h>

#include "db_int.h"
#include "dbtest.h"
#include "test_ext.h"

/* Internal functions */

static int do_lockvec __P((Tcl_Interp *, DB_LOCKTAB *, int, char **));

#ifdef DEBUG
void __lock_dump_region __P((DB_LOCKTAB *, unsigned long));
#endif
typedef struct _mgr_data {
	DB_LOCKTAB *tabp;
	DB_ENV *env;
} mgr_data;
typedef struct _lock_data {
	DB_LOCKTAB *tabp;
	DB_LOCK lock;
} lock_data;

/*
 * lock_cmd --
 *	Implements lock_open for dbtest.  Lock_open creates a lock
 * manager and all the necessary files in the file system.  It then
 * creates a command that implements the other lock functions.
 */

#define LOCKMGR_USAGE "lock_open path flags mode [options]\n\toptions:\n"

int
lockmgr_cmd(notused, interp, argc, argv)
	ClientData notused;
	Tcl_Interp *interp;
	int argc;
	char *argv[];
{
	DB_LOCKTAB *mgrp;
	DB_ENV *env;
	int flags, mode, ret;
	mgr_data *md;
	char mgrname[50];
	static int mgr_number = 0;

	notused = NULL;

	debug_check();

	/* Check number of arguments. */
	USAGE_GE(argc, 4, LOCKMGR_USAGE, DO_ENV);
	if (Tcl_GetInt(interp, argv[2], &flags) != TCL_OK ||
	    Tcl_GetInt(interp, argv[3], &mode) != TCL_OK)
		return (TCL_ERROR);


	/*
	 * Call lock_open.
	 */
	if (process_env_options(interp, argc, argv, &env)) {
		Tcl_PosixError(interp);
		Tcl_SetResult(interp, "NULL", TCL_STATIC);
		return (TCL_OK);
	}

	if (F_ISSET(env, DB_ENV_STANDALONE))
		mgrp = env->lk_info;
	else if ((ret = lock_open(argv[1], flags, mode, env, &mgrp)) != 0) {
		errno = ret;
		Tcl_PosixError(interp);
		Tcl_SetResult(interp, "NULL", TCL_STATIC);
		return (TCL_OK);
	} else
		env->lk_info = mgrp;

	/* Create new command name. */
	if ((md = (mgr_data *)malloc(sizeof(mgr_data))) == NULL) {
		if (!F_ISSET(env, DB_ENV_STANDALONE)) {
			(void)db_appexit(env);
			if (env->lk_conflicts)
				free(env->lk_conflicts);
			free(env);
		}
		Tcl_SetResult(interp, "lock_open: ", TCL_STATIC);
		errno = ENOMEM;
		Tcl_AppendResult(interp, Tcl_PosixError(interp), 0);
		return (TCL_ERROR);
	}
	md->tabp = mgrp;
	md->env = env;
	sprintf(&mgrname[0], "lockmgr%d", mgr_number);
	mgr_number++;

	/* Create widget command. */
	Tcl_CreateCommand(interp, mgrname, lockwidget_cmd, (int *)md, NULL);
	Tcl_SetResult(interp, mgrname, TCL_VOLATILE);
	return (TCL_OK);
}

/*
 * lockunlink_cmd --
 *	Implements lock_unlink for dbtest.
 */

#define LOCKUNLINK_USAGE "lock_unlink path force"

int
lockunlink_cmd(notused, interp, argc, argv)
	ClientData notused;
	Tcl_Interp *interp;
	int argc;
	char *argv[];
{
	DB_ENV *env;
	int force;

	notused = NULL;

	debug_check();

	USAGE_GE(argc, 3, LOCKUNLINK_USAGE, 0);

	if (Tcl_GetInt(interp, argv[2], &force) != TCL_OK)
		return (TCL_ERROR);

	if (process_env_options(interp, argc, argv, &env)) {
		Tcl_PosixError(interp);
		return (TCL_ERROR);
	}

	if (lock_unlink(argv[1], force, env) != 0) {
		Tcl_SetResult(interp, "-1", TCL_STATIC);
		return (TCL_OK);
	}
	Tcl_SetResult(interp, "0", TCL_STATIC);
	return (TCL_OK);
}

/*
 * lockwidget --
 * This is that command that implements the lock widget.  If we
 * ever add new "methods" we add new widget commands here.
 */
#define LOCKWIDGET_USAGE "lockmgrN option ?arg arg ...?"
#define LOCKCLOSE_USAGE "lockmgrN close"
#define LOCKDUMP_USAGE "lockmgrN dump flags"
#define	LOCKGET_USAGE "lockmgrN get locker obj mode flags"
#define LOCKVEC_USAGE \
	"lockmgrN vec locker flags ?{obj mode op} {obj mode op}...?"
/* Not yet implemented. */

int
lockwidget_cmd(cd_mgr, interp, argc, argv)
	ClientData cd_mgr;
	Tcl_Interp *interp;
	int argc;
	char *argv[];
{
	static int lock_id = 0;
	DB_LOCKTAB *mgr;
	DB_LOCK lock;
	DBT obj_dbt;
	DB_ENV *env;
	db_lockmode_t lmode;
	lock_data *ld;
	u_int32_t locker;
	int iflags, ilocker, mode, ret;
	char lockname[128];

	debug_check();

	mgr = ((mgr_data *)cd_mgr)->tabp;

	USAGE_GE(argc, 2, LOCKWIDGET_USAGE, 0);

	if (strcmp(argv[1], "close") == 0) {
		USAGE(argc, 2, LOCKCLOSE_USAGE, 0);
		env = ((mgr_data *)cd_mgr)->env;
		if (!F_ISSET(env, DB_ENV_STANDALONE)) {
			(void)db_appexit(env);
			if (env->lk_conflicts)
				free(env->lk_conflicts);
			free(env);
		}
		Tcl_DeleteCommand(interp, argv[0]);
		Tcl_SetResult(interp, "0", TCL_STATIC);
		return (TCL_OK);
	} else if (strcmp(argv[1], "get") == 0) {
		USAGE(argc, 6, LOCKGET_USAGE, 0);
		if (Tcl_GetInt(interp, argv[2], &ilocker) != TCL_OK) {
			Tcl_PosixError(interp);
			return (TCL_ERROR);
		}
		obj_dbt.data = argv[3];
		obj_dbt.size = strlen(argv[3]) + 1;
		if (Tcl_GetInt(interp, argv[4], &mode) != TCL_OK)
			return (TCL_ERROR);
		if (Tcl_GetInt(interp, argv[5], &iflags) != TCL_OK)
			return (TCL_ERROR);

		lmode = mode;
		locker = (u_int32_t)ilocker;
		ret = lock_get(mgr, locker, iflags, &obj_dbt, lmode, &lock);

		switch (ret) {
		case 0:	/* Success */
			sprintf(&lockname[0], "%s.lock%d", argv[0], lock_id);
			lock_id++;
			if ((ld =
			    (lock_data *)malloc(sizeof(lock_data))) == NULL) {
				Tcl_PosixError(interp);
				return (TCL_OK);
			}

			ld->tabp = mgr;
			ld->lock = lock;

			Tcl_CreateCommand(interp, lockname, lock_cmd,
			    (int *)ld, NULL);
			Tcl_SetResult(interp, lockname, TCL_VOLATILE);
			break;
		case DB_LOCK_NOTGRANTED:
			Tcl_SetResult(interp, "BLOCKED", TCL_STATIC);
			return (TCL_OK);
		case DB_LOCK_DEADLOCK:
			Tcl_SetResult(interp, "DEADLOCK", TCL_STATIC);
			return (TCL_OK);
		default:
			Tcl_PosixError(interp);
			return (TCL_ERROR);
		}
	} else if (strcmp(argv[1], "vec") == 0) {
		USAGE(argc, 5, LOCKVEC_USAGE, 0);
		return (do_lockvec(interp, mgr, argc, argv));
#ifdef DEBUG
	} else if (strcmp(argv[1], "dump") == 0) {
		u_long flags;
		int ival;

		USAGE(argc, 3, LOCKDUMP_USAGE, 0);
		if (Tcl_GetInt(interp, argv[2], &ival) != TCL_OK)
			flags = 0xffff;
		else
			flags = ival;
		__lock_dump_region(mgr, flags);
		return (TCL_OK);
#endif
	} else {
		Tcl_SetResult(interp, LOCKWIDGET_USAGE, TCL_STATIC);
		return (TCL_ERROR);
	}
	return (TCL_OK);
}

#define LOCK_USAGE "lockmgrN.lockM cmd ?arg arg ...?"
#define LOCKPUT_USAGE "lockmgrN.lockM put"
int
lock_cmd(cd_lock, interp, argc, argv)
	ClientData cd_lock;
	Tcl_Interp *interp;
	int argc;
	char *argv[];
{
	DB_LOCK lock;
	DB_LOCKTAB *tab;
	lock_data *ld;
	int ret;

	debug_check();

	USAGE_GE(argc, 2, LOCK_USAGE, 0);

	ld = (lock_data *)cd_lock;
	lock = ld->lock;
	tab = ld->tabp;
	free(ld);

	if (strcmp(argv[1], "put") == 0) {
		USAGE(argc, 2, LOCKPUT_USAGE, 0);
		Tcl_DeleteCommand(interp, argv[0]);
		if ((ret = lock_put(tab, lock)) != 0) {
			errno = ret;
			Tcl_PosixError(interp);
			return (TCL_OK);
		}
	} else {
		Tcl_SetResult(interp, LOCK_USAGE, TCL_STATIC);
		return (TCL_ERROR);
	}
	Tcl_SetResult(interp, "0", TCL_STATIC);
	return (TCL_OK);
}

static int
do_lockvec(interp, mgr, argc, argv)
	Tcl_Interp *interp;
	DB_LOCKTAB *mgr;
	int argc;
	char **argv;
{
	DB_LOCKREQ *reqlist, *err;
	DBT *dblist;
	db_lockmode_t lmode;
	db_lockop_t liop;
	int i, ilocker, ntup, nreqs, ret;
	int iflags, iop, imode;
	u_int32_t flags, locker;
	char **ap, **tuplist;

	/* Get the locker id and the flags. */
	if (Tcl_GetInt(interp, argv[2], &ilocker) != TCL_OK ||
	    Tcl_GetInt(interp, argv[3], &iflags) != TCL_OK)
		return (TCL_ERROR);
	flags = (u_int32_t)iflags;
	locker = (u_int32_t)ilocker;

	/* The first three args are command, the rest are tuples. */
	nreqs = argc - 4;
	if ((reqlist =
	    (DB_LOCKREQ *)calloc(sizeof(DB_LOCKREQ), nreqs)) == NULL ||
	    (dblist = (DBT *)calloc(sizeof(DBT), nreqs)) == NULL) {
		Tcl_PosixError(interp);
		return (TCL_ERROR);
	}

	for (i = 0; i < nreqs; i++)
		reqlist[i].obj = &dblist[i];


	for (ap = &argv[4], i = 0; i < nreqs; i++) {
		tuplist = NULL;
		if (Tcl_SplitList(interp, *ap, &ntup, &tuplist) !=
		    TCL_OK)
			break;
		if (Tcl_GetInt(interp, tuplist[1], &imode) != TCL_OK ||
		    Tcl_GetInt(interp, tuplist[2], &iop) != TCL_OK)
			break;

		liop = iop;
		reqlist[i].op = liop;
		reqlist[i].obj->data = (char *)strdup(tuplist[0]);
		reqlist[i].obj->size = strlen(tuplist[0]);
		lmode = imode;
		reqlist[i].mode = lmode;

		free(tuplist);
	}

	if (tuplist == NULL || i != nreqs)
		ret = TCL_ERROR;
	else if ((ret =
	    lock_vec(mgr, locker, flags, reqlist, nreqs, &err)) != 0) {
		Tcl_SetResult(interp, "lock_vec failed, returned ", TCL_STATIC);
		switch (ret) {
		case DB_LOCK_DEADLOCK:
			Tcl_AppendResult(interp, "DEADLOCK", 0);
			break;
		case DB_LOCK_NOTHELD:
			Tcl_AppendResult(interp, "NOTHELD", 0);
			break;
		case DB_LOCK_NOTGRANTED:
			Tcl_AppendResult(interp, "NOTGRANTED", 0);
			break;
		default:
			errno = ret;
			Tcl_AppendResult(interp, Tcl_PosixError(interp), 0);
			break;
		}
		Tcl_AppendResult(interp, " on request ",
		    argv[4 + (err - reqlist)], NULL);
		ret = TCL_ERROR;
	} else {
		Tcl_SetResult(interp, "0", TCL_STATIC);
		ret = TCL_OK;
	}

	for (i = 0; i < nreqs; i++)
		if (reqlist[i].obj->data != NULL)
			free(reqlist[i].obj->data);
	free(reqlist);
	free(dblist);
	return (ret);
}
