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
static const char sccsid[] = "@(#)db_apprec.c	10.18 (Sleepycat) 9/30/97";
#endif

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <time.h>
#include <string.h>
#include <stdlib.h>
#endif

#include "db_int.h"
#include "shqueue.h"
#include "db_page.h"
#include "db_dispatch.h"
#include "db_am.h"
#include "log.h"
#include "txn.h"
#include "common_ext.h"

/*
 * __db_apprec --
 *	Perform recovery.
 *
 * PUBLIC: int __db_apprec __P((DB_ENV *, int));
 */
int
__db_apprec(dbenv, flags)
	DB_ENV *dbenv;
	int flags;
{
	DBT data;
	DB_LOG *lp;
	DB_LSN ckp_lsn, first_lsn, lsn, tmp_lsn;
	time_t now;
	int first_flag, is_thread, ret;
	void *txninfo;

	lp = dbenv->lg_info;

	/* Initialize the transaction list. */
	if ((ret = __db_txnlist_init(&txninfo)) != 0)
		return (ret);

	/*
	 * Save the state of the thread flag -- we don't need it on at the
	 * moment because we're single-threaded until recovery is complete.
	 */
	is_thread = F_ISSET(lp, DB_AM_THREAD);
	F_CLR(lp, DB_AM_THREAD);

	/*
	 * Read forward through the log, opening the appropriate files so that
	 * we can call recovery routines.  In general, we start at the last
	 * checkpoint prior to the last checkpointed LSN.  For catastrophic
	 * recovery, we begin at the first LSN that appears in any log file
	 * (log_get figures this out for us when we pass it the DB_FIRST flag).
	 */
	if (LF_ISSET(DB_RECOVER_FATAL))
		first_flag = DB_FIRST;
	else {
		if ((ret = __log_findckp(lp, &lsn)) == DB_NOTFOUND) {
			F_SET(lp, is_thread);
			return (0);
		}
		first_flag = DB_SET;
	}

	/* If we're a threaded application, we have to allocate space. */
	memset(&data, 0, sizeof(data));
	if ((ret = log_get(lp, &lsn, &data, first_flag)) != 0) {
		__db_err(dbenv, "Failure: unable to get log record");
		if (first_flag == DB_SET)
			__db_err(dbenv, "Retrieving LSN %lu %lu",
			    (u_long)lsn.file, (u_long)lsn.offset);
		else
			__db_err(dbenv, "Retrieving first LSN");
		goto err;
	}

	first_lsn = lsn;
	for (;;) {
		ret = __db_dispatch(lp, &data, &lsn, TXN_OPENFILES, txninfo);
		if (ret != 0 && ret != DB_TXN_CKP)
			goto msgerr;
		if ((ret =
		    log_get(dbenv->lg_info, &lsn, &data, DB_NEXT)) != 0) {
			if (ret != DB_NOTFOUND)
				goto err;
			break;
		}
	}

	/*
	 * Initialize the ckp_lsn to 0,0.  If we never find a valid
	 * checkpoint in the log, then leaving ckp_lsn at 0,0 is correct.
	 */
	ZERO_LSN(ckp_lsn);
	for (ret = log_get(lp, &lsn, &data, DB_LAST);
	    ret == 0 && log_compare(&lsn, &first_lsn) > 0;
	    ret = log_get(lp,&lsn, &data, DB_PREV)) {
		tmp_lsn = lsn;
		ret = __db_dispatch(lp,
		    &data, &lsn, TXN_BACKWARD_ROLL, txninfo);
		if (ret == DB_TXN_CKP) {
			if (IS_ZERO_LSN(ckp_lsn))
				ckp_lsn = tmp_lsn;
			ret = 0;
		} else if (ret != 0)
			goto msgerr;
	}
	if (ret != 0 && ret != DB_NOTFOUND)
		goto err;

	for (ret = log_get(lp, &lsn, &data, DB_NEXT);
	    ret == 0; ret = log_get(lp, &lsn, &data, DB_NEXT)) {
		ret = __db_dispatch(lp, &data, &lsn, TXN_FORWARD_ROLL, txninfo);
		if (ret == DB_TXN_CKP)
			ret = 0;
		else if (ret != 0)
			goto msgerr;
	}
	if (ret != DB_NOTFOUND)
		goto err;

	/* Now close all the db files that are open. */
	__log_close_files(lp);

	/*
	 * Now set the maximum transaction id, set the last checkpoint lsn,
	 * and the current time.  Then take a checkpoint.
	 */
	(void)time(&now);
	dbenv->tx_info->region->last_txnid = ((__db_txnhead *)txninfo)->maxid;
	dbenv->tx_info->region->last_ckp = ckp_lsn;
	dbenv->tx_info->region->time_ckp = (u_int32_t)now;
	if ((ret = txn_checkpoint(dbenv->tx_info, 0, 0)) != 0)
		goto err;

	if (dbenv->db_verbose) {
		__db_err(lp->dbenv, "Recovery complete at %s", ctime(&now));
		__db_err(lp->dbenv, "%s %lu %s [%lu][%lu]",
		    "Maximum transaction id",
		    (u_long)dbenv->tx_info->region->last_txnid,
		    "Recovery checkpoint",
		    (u_long)dbenv->tx_info->region->last_ckp.file,
		    (u_long)dbenv->tx_info->region->last_ckp.offset);
	}

	F_SET(lp, is_thread);
	return (0);

msgerr:	__db_err(dbenv, "Recovery function for LSN %lu %lu failed",
	    (u_long)lsn.file, (u_long)lsn.offset);

err:	F_SET(lp, is_thread);
	return (ret);
}
