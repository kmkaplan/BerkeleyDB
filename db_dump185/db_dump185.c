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
static const char sccsid[] = "@(#)db_dump185.c	10.8 (Sleepycat) 9/21/97";
#endif

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

#include "db_185.h"
#include "clib_ext.h"

/* Hash Table Information */
typedef struct hashhdr {		/* Disk resident portion */
	int		magic;		/* Magic NO for hash tables */
	int		version;	/* Version ID */
	u_int32_t	lorder;		/* Byte Order */
	int		bsize;		/* Bucket/Page Size */
	int		bshift;		/* Bucket shift */
	int		dsize;		/* Directory Size */
	int		ssize;		/* Segment Size */
	int		sshift;		/* Segment shift */
	int		ovfl_point;	/* Where overflow pages are being
					 * allocated */
	int		last_freed;	/* Last overflow page freed */
	int		max_bucket;	/* ID of Maximum bucket in use */
	int		high_mask;	/* Mask to modulo into entire table */
	int		low_mask;	/* Mask to modulo into lower half of
					 * table */
	int		ffactor;	/* Fill factor */
	int		nkeys;		/* Number of keys in hash table */
} HASHHDR;

typedef struct htab	 {		/* Memory resident data structure */
	HASHHDR 	hdr;		/* Header */
} HTAB;

typedef struct _epgno {
	u_int32_t pgno;			/* the page number */
	u_int16_t index;		/* the index on the page */
} EPGNO;

typedef struct _epg {
	void	*page;			/* the (pinned) page */
	u_int16_t index;		/* the index on the page */
} EPG;

typedef struct _cursor {
	EPGNO	 pg;			/* B: Saved tree reference. */
	DBT	 key;			/* B: Saved key, or key.data == NULL. */
	u_int32_t rcursor;		/* R: recno cursor (1-based) */

#define	CURS_ACQUIRE	0x01		/*  B: Cursor needs to be reacquired. */
#define	CURS_AFTER	0x02		/*  B: Unreturned cursor after key. */
#define	CURS_BEFORE	0x04		/*  B: Unreturned cursor before key. */
#define	CURS_INIT	0x08		/* RB: Cursor initialized. */
	u_int8_t flags;
} CURSOR;

/* The in-memory btree/recno data structure. */
typedef struct _btree {
	void	 *bt_mp;		/* memory pool cookie */

	void	 *bt_dbp;		/* pointer to enclosing DB */

	EPG	  bt_cur;		/* current (pinned) page */
	void	 *bt_pinned;		/* page pinned across calls */

	CURSOR	  bt_cursor;		/* cursor */

	EPGNO	  bt_stack[50];		/* stack of parent pages */
	EPGNO	 *bt_sp;		/* current stack pointer */

	DBT	  bt_rkey;		/* returned key */
	DBT	  bt_rdata;		/* returned data */

	int	  bt_fd;		/* tree file descriptor */

	u_int32_t bt_free;		/* next free page */
	u_int32_t bt_psize;		/* page size */
	u_int16_t bt_ovflsize;		/* cut-off for key/data overflow */
	int	  bt_lorder;		/* byte order */
					/* sorted order */
	enum { NOT, BACK, FORWARD } bt_order;
	EPGNO	  bt_last;		/* last insert */

					/* B: key comparison function */
	int	(*bt_cmp) __P((const DBT *, const DBT *));
					/* B: prefix comparison function */
	size_t	(*bt_pfx) __P((const DBT *, const DBT *));
					/* R: recno input function */
	int	(*bt_irec) __P((struct _btree *, u_int32_t));

	FILE	 *bt_rfp;		/* R: record FILE pointer */
	int	  bt_rfd;		/* R: record file descriptor */

	void	 *bt_cmap;		/* R: current point in mapped space */
	void	 *bt_smap;		/* R: start of mapped space */
	void	 *bt_emap;		/* R: end of mapped space */
	size_t	  bt_msize;		/* R: size of mapped region. */

	u_int32_t bt_nrecs;		/* R: number of records */
	size_t	  bt_reclen;		/* R: fixed record length */
	u_char	  bt_bval;		/* R: delimiting byte/pad character */

/*
 * NB:
 * B_NODUPS and R_RECNO are stored on disk, and may not be changed.
 */
#define	B_INMEM		0x00001		/* in-memory tree */
#define	B_METADIRTY	0x00002		/* need to write metadata */
#define	B_MODIFIED	0x00004		/* tree modified */
#define	B_NEEDSWAP	0x00008		/* if byte order requires swapping */
#define	B_RDONLY	0x00010		/* read-only tree */

#define	B_NODUPS	0x00020		/* no duplicate keys permitted */
#define	R_RECNO		0x00080		/* record oriented tree */

#define	R_CLOSEFP	0x00040		/* opened a file pointer */
#define	R_EOF		0x00100		/* end of input file reached. */
#define	R_FIXLEN	0x00200		/* fixed length records */
#define	R_MEMMAPPED	0x00400		/* memory mapped file. */
#define	R_INMEM		0x00800		/* in-memory file */
#define	R_MODIFIED	0x01000		/* modified file */
#define	R_RDONLY	0x02000		/* read-only file */

#define	B_DB_LOCK	0x04000		/* DB_LOCK specified. */
#define	B_DB_SHMEM	0x08000		/* DB_SHMEM specified. */
#define	B_DB_TXN	0x10000		/* DB_TXN specified. */
	u_int32_t flags;
} BTREE;

void	db_185_btree __P((DB *, int));
void	db_185_hash __P((DB *, int));
void	dbt_dump __P((DBT *));
void	dbt_print __P((DBT *));
int	main __P((int, char *[]));
void	usage __P((void));

const char
	*progname = "db_dump185";			/* Program name. */

int
main(argc, argv)
	int argc;
	char *argv[];
{
	extern char *optarg;
	extern int optind;
	DB *dbp;
	DBT key, data;
	int ch, pflag, rval;

	pflag = 0;
	while ((ch = getopt(argc, argv, "f:p")) != EOF)
		switch (ch) {
		case 'f':
			if (freopen(optarg, "w", stdout) == NULL)
				err(1, "%s", optarg);
			break;
		case 'p':
			pflag = 1;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage();

	if ((dbp = dbopen(argv[0], O_RDONLY, 0, DB_BTREE, NULL)) == NULL) {
		if ((dbp = dbopen(argv[0], O_RDONLY, 0, DB_HASH, NULL)) == NULL)
			err(1, "%s", argv[0]);
		db_185_hash(dbp, pflag);
	} else
		db_185_btree(dbp, pflag);

	/*
	 * !!!
	 * DB 1.85 DBTs are a subset of DB 2.0 DBTs, so we just use the
	 * new dump/print routines.
	 */
	if (pflag)
		while (!(rval = dbp->seq(dbp, &key, &data, R_NEXT))) {
			dbt_print(&key);
			dbt_print(&data);
		}
	else
		while (!(rval = dbp->seq(dbp, &key, &data, R_NEXT))) {
			dbt_dump(&key);
			dbt_dump(&data);
		}

	if (rval == -1)
		err(1, "seq");
	return (0);
}

/*
 * db_185_hash --
 *	Dump out hash header information.
 */
void
db_185_hash(dbp, pflag)
	DB *dbp;
	int pflag;
{
	HTAB *hashp;

	hashp = dbp->internal;

	printf("format=%s\n", pflag ? "print" : "bytevalue");
	printf("type=hash\n");
	printf("h_ffactor=%lu\n", (u_long)hashp->hdr.ffactor);
#ifdef NOT_AVAILABLE_IN_DB_185
	printf("h_nelem=%lu\n", (u_long)hashp->hdr.nelem);
#endif
	if (hashp->hdr.lorder != 0)
		printf("db_lorder=%lu\n", (u_long)hashp->hdr.lorder);
	printf("db_pagesize=%lu\n", (u_long)hashp->hdr.bsize);
	printf("HEADER=END\n");
}

/*
 * db_185_btree --
 *	Dump out btree header information.
 */
void
db_185_btree(dbp, pflag)
	DB *dbp;
	int pflag;
{
	BTREE *btp;

	btp = dbp->internal;

	printf("format=%s\n", pflag ? "print" : "bytevalue");
	printf("type=btree\n");
#ifdef NOT_AVAILABLE_IN_185
	printf("bt_minkey=%lu\n", (u_long)XXX);
	printf("bt_maxkey=%lu\n", (u_long)XXX);
#endif
	if (btp->bt_lorder != 0)
		printf("db_lorder=%lu\n", (u_long)btp->bt_lorder);
	printf("db_pagesize=%lu\n", (u_long)btp->bt_psize);
	if (!(btp->flags & B_NODUPS))
		printf("duplicates=1\n");
	printf("HEADER=END\n");
}

static char hex[] = "0123456789abcdef";

/*
 * dbt_dump --
 *	Write out a key or data item using byte values.
 */
void
dbt_dump(dbtp)
	DBT *dbtp;
{
	size_t len;
	u_int8_t *p;

	for (len = dbtp->size, p = dbtp->data; len--; ++p)
		(void)printf("%c%c",
		    hex[(*p & 0xf0) >> 4], hex[*p & 0x0f]);
	printf("\n");
}

/*
 * dbt_print --
 *	Write out a key or data item using printable characters.
 */
void
dbt_print(dbtp)
	DBT *dbtp;
{
	size_t len;
	u_int8_t *p;

	for (len = dbtp->size, p = dbtp->data; len--; ++p)
		if (isprint(*p)) {
			if (*p == '\\')
				(void)printf("\\");
			(void)printf("%c", *p);
		} else
			(void)printf("\\%c%c",
			    hex[(*p & 0xf0) >> 4], hex[*p & 0x0f]);
	printf("\n");
}

/*
 * usage --
 *	Display the usage message.
 */
void
usage()
{
	(void)fprintf(stderr, "usage: db_dump [-p] [-f file] db_file\n");
	exit(1);
}
