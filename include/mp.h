/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996, 1997
 *	Sleepycat Software.  All rights reserved.
 *
 *	@(#)mp.h	10.19 (Sleepycat) 10/25/97
 */

struct __bh;		typedef struct __bh BH;
struct __db_mpreg;	typedef struct __db_mpreg DB_MPREG;
struct __mpool;		typedef struct __mpool MPOOL;
struct __mpoolfile;	typedef struct __mpoolfile MPOOLFILE;

					/* Default mpool name. */
#define	DB_DEFAULT_MPOOL_FILE	"__db_mpool.share"

/*
 *  We default to 128K (16 8K pages) if the user doesn't specify, and
 * require a minimum of 20K.
 */
#define	DB_CACHESIZE_DEF	(128 * 1024)
#define	DB_CACHESIZE_MIN	( 20 * 1024)

#define	INVALID		0		/* Invalid shared memory offset. */
#define	TEMPORARY	"<tmp>"		/* Temporary file name. */

/*
 * There are three ways we do locking in the mpool code:
 *
 * Locking a handle mutex to provide concurrency for DB_THREAD operations.
 * Locking the region mutex to provide mutual exclusion while reading and
 *    writing structures in the shared region.
 * Locking buffer header mutexes during I/O.
 *
 * The first will not be further described here.  We use the shared mpool
 * region lock to provide mutual exclusion while reading/modifying all of
 * the data structures, including the buffer headers.  We use a per-buffer
 * header lock to wait on buffer I/O.  The order of locking is as follows:
 *
 * Searching for a buffer:
 *	Acquire the region lock.
 *	Find the buffer header.
 *	Increment the reference count (guarantee the buffer stays).
 *	If the BH_LOCKED flag is set (I/O is going on):
 *		Release the region lock.
 *		Request the buffer lock.
 *		The I/O will complete...
 *		Acquire the buffer lock.
 *		Release the buffer lock.
 *		Acquire the region lock.
 *	Return the buffer.
 *
 * Reading/writing a buffer:
 *	Acquire the region lock.
 *	Find/create the buffer header.
 *	If reading, increment the reference count (guarantee the buffer stays).
 *	Set the BH_LOCKED flag.
 *	Acquire the buffer lock (guaranteed not to block).
 *	Release the region lock.
 *	Do the I/O and/or initialize buffer contents.
 *	Acquire the region lock.
 *	Clear the BH_LOCKED flag.
 *	Release the region lock.
 *	Release the buffer lock.
 *	If reading, return the buffer.
 *
 * Pointers to DB_MPOOL, MPOOL, DB_MPOOLFILE and MPOOLFILE structures are not
 * reacquired when a region lock is reacquired because they couldn't have been
 * closed/discarded and because they never move in memory.
 */
#define	LOCKINIT(dbmp, mutexp)						\
	if (F_ISSET(dbmp, MP_LOCKHANDLE | MP_LOCKREGION))		\
		(void)__db_mutex_init(mutexp, (dbmp)->fd)

#define	LOCKHANDLE(dbmp, mutexp)					\
	if (F_ISSET(dbmp, MP_LOCKHANDLE))				\
		(void)__db_mutex_lock(mutexp, (dbmp)->fd)
#define	UNLOCKHANDLE(dbmp, mutexp)					\
	if (F_ISSET(dbmp, MP_LOCKHANDLE))				\
		(void)__db_mutex_unlock(mutexp, (dbmp)->fd)

#define	LOCKREGION(dbmp)						\
	if (F_ISSET(dbmp, MP_LOCKREGION))				\
		(void)__db_mutex_lock(&((RLAYOUT *)(dbmp)->mp)->lock,	\
		    (dbmp)->fd)
#define	UNLOCKREGION(dbmp)						\
	if (F_ISSET(dbmp, MP_LOCKREGION))				\
		(void)__db_mutex_unlock(&((RLAYOUT *)(dbmp)->mp)->lock,	\
		(dbmp)->fd)

#define	LOCKBUFFER(dbmp, bhp)						\
	if (F_ISSET(dbmp, MP_LOCKREGION))				\
		(void)__db_mutex_lock(&(bhp)->mutex, (dbmp)->fd)
#define	UNLOCKBUFFER(dbmp, bhp)						\
	if (F_ISSET(dbmp, MP_LOCKREGION))				\
		(void)__db_mutex_unlock(&(bhp)->mutex, (dbmp)->fd)

/*
 * DB_MPOOL --
 *	Per-process memory pool structure.
 */
struct __db_mpool {
/* These fields need to be protected for multi-threaded support. */
	db_mutex_t	*mutexp;	/* Structure lock. */

					/* List of pgin/pgout routines. */
	LIST_HEAD(__db_mpregh, __db_mpreg) dbregq;

					/* List of DB_MPOOLFILE's. */
	TAILQ_HEAD(__db_mpoolfileh, __db_mpoolfile) dbmfq;

/* These fields are not protected. */
	DB_ENV     *dbenv;		/* Reference to error information. */

	MPOOL	   *mp;			/* Address of the shared MPOOL. */

	void	   *maddr;		/* Address of mmap'd region. */
	void	   *addr;		/* Address of shalloc() region. */

	DB_HASHTAB *htab;		/* Hash table of bucket headers. */

	int	    fd;			/* Underlying mmap'd fd. */


#define	MP_ISPRIVATE	0x01		/* Private, so local memory. */
#define	MP_LOCKHANDLE	0x02		/* Threaded, lock handles and region. */
#define	MP_LOCKREGION	0x04		/* Concurrent access, lock region. */
	u_int32_t  flags;
};

/*
 * DB_MPREG --
 *	DB_MPOOL registry of pgin/pgout functions.
 */
struct __db_mpreg {
	LIST_ENTRY(__db_mpreg) q;	/* Linked list. */

	int ftype;			/* File type. */
					/* Pgin, pgout routines. */
	int (*pgin) __P((db_pgno_t, void *, DBT *));
	int (*pgout) __P((db_pgno_t, void *, DBT *));
};

/*
 * DB_MPOOLFILE --
 *	Per-process DB_MPOOLFILE information.
 */
struct __db_mpoolfile {
/* These fields need to be protected for multi-threaded support. */
	db_mutex_t	*mutexp;	/* Structure lock. */

	int	   fd;			/* Underlying file descriptor. */

	u_int32_t pinref;		/* Pinned block reference count. */

/* These fields are not protected. */
	TAILQ_ENTRY(__db_mpoolfile) q;	/* Linked list of DB_MPOOLFILE's. */

	char	  *path;		/* Initial file path. */
	DB_MPOOL  *dbmp;		/* Overlying DB_MPOOL. */
	MPOOLFILE *mfp;			/* Underlying MPOOLFILE. */

	void	  *addr;		/* Address of mmap'd region. */
	size_t	   len;			/* Length of mmap'd region. */

/* These fields need to be protected for multi-threaded support. */
#define	MP_PATH_ALLOC	0x001		/* Path is allocated memory. */
#define	MP_PATH_TEMP	0x002		/* Backing file is a temporary. */
#define	MP_READONLY	0x004		/* File is readonly. */
#define	MP_UPGRADE	0x008		/* File descriptor is readwrite. */
#define	MP_UPGRADE_FAIL	0x010		/* Upgrade wasn't possible. */
	u_int32_t  flags;
};

/*
 * MPOOL --
 *	Shared memory pool region.  One of these is allocated in shared
 *	memory, and describes the pool.
 */
struct __mpool {
	RLAYOUT	    rlayout;		/* General region information. */

	SH_TAILQ_HEAD(__bhq) bhq;	/* LRU list of buckets. */
	SH_TAILQ_HEAD(__bhfq) bhfq;	/* Free buckets. */
	SH_TAILQ_HEAD(__mpfq) mpfq;	/* List of MPOOLFILEs. */

	/*
	 * We make the assumption that the early pages of the file are far
	 * more likely to be retrieved than the later pages, which means
	 * that the top bits are more interesting for hashing since they're
	 * less likely to collide.  On the other hand, since 512 4K pages
	 * represents a 2MB file, only the bottom 9 bits of the page number
	 * are likely to be set.  We XOR in the offset in the MPOOL of the
	 * MPOOLFILE that backs this particular page, since that should also
	 * be unique for the page.
	 */
#define	BUCKET(mp, mf_offset, pgno)					\
	(((pgno) ^ ((mf_offset) << 9)) % (mp)->htab_buckets)

	size_t	    htab;		/* Hash table offset. */
	size_t	    htab_buckets;	/* Number of hash table entries. */

	DB_LSN	    lsn;		/* Maximum checkpoint LSN. */
	int	    lsn_cnt;		/* Checkpoint buffers left to write. */

	DB_MPOOL_STAT stat;		/* Global mpool statistics. */

#define	MP_LSN_RETRY	0x01		/* Retry all BH_WRITE buffers. */
	u_int32_t  flags;
};

/*
 * MPOOLFILE --
 *	Shared DB_MPOOLFILE information.
 */
struct __mpoolfile {
	SH_TAILQ_ENTRY  q;		/* List of MPOOLFILEs */

	u_int32_t ref;			/* Reference count. */

	int	  ftype;		/* File type. */
	int	  can_mmap;		/* If the file can be mmap'd. */

	int	  lsn_off;		/* Page's LSN offset. */

	size_t	  path_off;		/* File name location. */

	size_t	  fileid_off;		/* File identification location. */

	size_t	  pgcookie_len;		/* Pgin/pgout cookie length. */
	size_t	  pgcookie_off;		/* Pgin/pgout cookie location. */

	int	  lsn_cnt;		/* Checkpoint buffers left to write. */

	DB_MPOOL_FSTAT stat;		/* Per-file mpool statistics. */
};

/*
 * BH --
 *	Buffer header.
 */
struct __bh {
	db_mutex_t	mutex;		/* Structure lock. */

	u_int16_t	ref;		/* Reference count. */

#define	BH_CALLPGIN	0x001		/* Page needs to be reworked... */
#define	BH_DIRTY	0x002		/* Page was modified. */
#define	BH_DISCARD	0x004		/* Page is useless. */
#define	BH_LOCKED	0x008		/* Page is locked (I/O in progress). */
#define	BH_TRASH	0x010		/* Page is garbage. */
#define	BH_WRITE	0x020		/* Page scheduled for writing. */
	u_int16_t  flags;

	SH_TAILQ_ENTRY	q;		/* LRU queue. */
	SH_TAILQ_ENTRY	hq;		/* MPOOL hash bucket queue. */

	db_pgno_t pgno;			/* Underlying MPOOLFILE page number. */
	size_t	  mf_offset;		/* Associated MPOOLFILE offset. */

	/*
	 * !!!
	 * This array must be size_t aligned -- the DB access methods put PAGE
	 * and other structures into it, and expect to be able to access them
	 * directly.  (We guarantee size_t alignment in the db_mpool(3) manual
	 * page as well.)
	 */
	u_int8_t   buf[1];		/* Variable length data. */
};

#include "mp_ext.h"
