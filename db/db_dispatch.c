/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996, 1997
 *	Sleepycat Software.  All rights reserved.
 */
/*
 * Copyright (c) 1995, 1996
 *	The President and Fellows of Harvard University.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Margo Seltzer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "config.h"

#ifndef lint
static const char sccsid[] = "@(#)db_dispatch.c	10.6 (Sleepycat) 10/25/97";
#endif /* not lint */

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "db_int.h"
#include "db_page.h"
#include "db_dispatch.h"
#include "db_am.h"
#include "common_ext.h"

/*
 * Data structures to manage the DB dispatch table.  The dispatch table
 * is a dynamically allocated array of pointers to dispatch functions.
 * The dispatch_size is the number of entries possible in the current
 * dispatch table and the dispatch_valid is the number of valid entries
 * in the dispatch table.
 */
static int (**dispatch_table) __P((DB_LOG *, DBT *, DB_LSN *, int, void *));
static u_int32_t dispatch_size = 0;

/*
 * __db_dispatch --
 *
 * This is the transaction dispatch function used by the db access methods.
 * It is designed to handle the record format used by all the access
 * methods (the one automatically generated by the db_{h,log,read}.sh
 * scripts in the tools directory).  An application using a different
 * recovery paradigm will supply a different dispatch function to txn_open.
 *
 * PUBLIC: int __db_dispatch __P((DB_LOG *, DBT *, DB_LSN *, int, void *));
 */
int
__db_dispatch(logp, db, lsnp, redo, info)
	DB_LOG *logp;		/* The log file. */
	DBT *db;		/* The log record upon which to dispatch. */
	DB_LSN *lsnp;		/* The lsn of the record being dispatched. */
	int redo;		/* Redo this op (or undo it). */
	void *info;
{
	u_int32_t rectype, txnid;

	memcpy(&rectype, db->data, sizeof(rectype));
	memcpy(&txnid, (u_int8_t *)db->data + sizeof(rectype), sizeof(txnid));

	switch (redo) {
	case TXN_REDO:
	case TXN_UNDO:
		return ((dispatch_table[rectype])(logp, db, lsnp, redo, info));
	case TXN_OPENFILES:
		if (rectype < DB_txn_BEGIN )
			return ((dispatch_table[rectype])(logp,
			    db, lsnp, redo, info));
		break;
	case TXN_BACKWARD_ROLL:
		/*
		 * Running full recovery in the backward pass.  If we've
		 * seen this txnid before and added to it our commit list,
		 * then we do nothing during this pass.  If we've never
		 * seen it, then we call the appropriate recovery routine
		 * in "abort mode".
		 */
		if (__db_txnlist_find(info, txnid) == DB_NOTFOUND)
			return ((dispatch_table[rectype])(logp,
			    db, lsnp, TXN_UNDO, info));
		break;
	case TXN_FORWARD_ROLL:
		/*
		 * In the forward pass, if we haven't seen the transaction,
		 * do nothing, else recovery it.
		 */
		if (__db_txnlist_find(info, txnid) != DB_NOTFOUND)
			return ((dispatch_table[rectype])(logp,
			    db, lsnp, TXN_REDO, info));
		break;
	default:
		abort();
	}
	return (0);
}

/*
 * __db_add_recovery --
 *
 * PUBLIC: int __db_add_recovery __P((DB_ENV *,
 * PUBLIC:    int (*)(DB_LOG *, DBT *, DB_LSN *, int, void *), u_int32_t));
 */
int
__db_add_recovery(dbenv, func, ndx)
	DB_ENV *dbenv;
	int (*func) __P((DB_LOG *, DBT *, DB_LSN *, int, void *));
	u_int32_t ndx;
{
	u_int32_t i;

	/* Check if function is already registered. */
	if (dispatch_table && ndx < dispatch_size &&
	    dispatch_table[ndx] != 0 && dispatch_table[ndx] != func)
		return (DB_REGISTERED);

	/* Check if we have to grow the table. */
	if (ndx >= dispatch_size) {
		if (dispatch_table == NULL)
			dispatch_table = (int (**)
			 __P((DB_LOG *, DBT *, DB_LSN *, int, void *)))
			 __db_malloc(DB_user_BEGIN * sizeof(dispatch_table[0]));
		else
			dispatch_table = (int (**)
			    __P((DB_LOG *, DBT *, DB_LSN *, int, void *)))
			    __db_realloc(dispatch_table, (DB_user_BEGIN +
			    dispatch_size) * sizeof(dispatch_table[0]));
		if (dispatch_table == NULL) {
			__db_err(dbenv, "%s", strerror(ENOMEM));
			return (ENOMEM);
		}
		for (i = dispatch_size,
		    dispatch_size += DB_user_BEGIN; i < dispatch_size; ++i)
			dispatch_table[i] = NULL;
	}

	dispatch_table[ndx] = func;
	return (0);
}

/*
 * __db_txnlist_init --
 *	Initialize transaction linked list.
 *
 * PUBLIC: int __db_txnlist_init __P((void *));
 */
int
__db_txnlist_init(retp)
	void *retp;
{
	__db_txnhead *headp;

	if ((headp = (struct __db_txnhead *)
	    __db_malloc(sizeof(struct __db_txnhead))) == NULL)
		return (ENOMEM);

	LIST_INIT(&headp->head);
	headp->maxid = 0;

	*(void **)retp = headp;
	return (0);
}

/*
 * __db_txnlist_add --
 *	Add an element to our transaction linked list.
 *
 * PUBLIC: int __db_txnlist_add __P((void *, u_int32_t));
 */
int
__db_txnlist_add(listp, txnid)
	void *listp;
	u_int32_t txnid;
{
	__db_txnhead *hp;
	__db_txnlist *elp;

	if ((elp = (__db_txnlist *)__db_malloc(sizeof(__db_txnlist))) == NULL)
		return (ENOMEM);

	elp->txnid = txnid;
	hp = (struct __db_txnhead *)listp;
	LIST_INSERT_HEAD(&hp->head, elp, links);
	if (txnid > hp->maxid)
		hp->maxid = txnid;

	return (0);
}

/*
 * __db_txnlist_find --
 *	Checks to see if txnid is in the txnid list, returns 1 if found,
 *	0 if not found.
 *
 * PUBLIC: int __db_txnlist_find __P((void *, u_int32_t));
 */
int
__db_txnlist_find(listp, txnid)
	void *listp;
	u_int32_t txnid;
{
	__db_txnlist *p;
	__db_txnhead *hp;

	if ((hp = (struct __db_txnhead *)listp) == NULL)
		return (DB_NOTFOUND);

	if (hp->maxid < txnid) {
		hp->maxid = txnid;
		return (DB_NOTFOUND);
	}

	for (p = hp->head.lh_first; p != NULL; p = p->links.le_next)
		if (p->txnid == txnid)
			return (0);

	return (DB_NOTFOUND);
}

#ifdef DEBUG
void
__db_txnlist_print(listp)
	void *listp;
{
	__db_txnlist *p;
	__db_txnhead *hp;

	hp = (struct __db_txnhead *)listp;
	printf("Maxid: %lu\n", (u_long)hp->maxid);
	for (p = hp->head.lh_first; p != NULL; p = p->links.le_next)
		printf("TXNID: %lu\n", (u_long)p->txnid);
}
#endif
