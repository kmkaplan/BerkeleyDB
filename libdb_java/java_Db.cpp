/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997
 *	Sleepycat Software.  All rights reserved.
 */
#include "config.h"

#ifndef lint
static const char sccsid[] = "@(#)java_Db.cpp	10.1 (Sleepycat) 11/10/97";
#endif /* not lint */

#include <jni.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "java_util.h"
#include "com_sleepycat_db_Db.h"

JNIEXPORT void JNICALL Java_com_sleepycat_db_Db_close
  (JNIEnv *jnienv, /*Db*/ jobject jthis, jint flags)
{
    int err;
    DB *db = get_DB(jnienv, jthis);

    if (!verify_non_null(jnienv, db))
        return;
    err = db->close(db, flags);
    if (verify_return(jnienv, err))
    {
        set_private_info(jnienv, name_DB, jthis, 0);
    }
}

JNIEXPORT jobject JNICALL Java_com_sleepycat_db_Db_cursor
  (JNIEnv *jnienv, /*Db*/ jobject jthis, /*DbTxn*/ jobject txnid)
{
    int err;
    DB *db = get_DB(jnienv, jthis);
    DB_TXN *dbtxnid = get_DB_TXN(jnienv, txnid);

    if (!verify_non_null(jnienv, db))
        return NULL;
    DBC *dbc;
    err = db->cursor(db, dbtxnid, &dbc);
    verify_return(jnienv, err);
    return get_Dbc(jnienv, dbc);
}

JNIEXPORT void JNICALL Java_com_sleepycat_db_Db_del
  (JNIEnv *jnienv, /*Db*/ jobject jthis, /*DbTxn*/ jobject txnid,
   /*Dbt*/ jobject key, jint dbflags)
{
    int err;
    DB *db = get_DB(jnienv, jthis);
    if (!verify_non_null(jnienv, db))
        return;
    DB_TXN *dbtxnid = get_DB_TXN(jnienv, txnid);
    LockedDBT dbkey(jnienv, key);

    err = db->del(db, dbtxnid, dbkey.dbt, dbflags);
    verify_return(jnienv, err);
}

JNIEXPORT jint JNICALL Java_com_sleepycat_db_Db_fd
  (JNIEnv *jnienv, /*Db*/ jobject jthis)
{
    int err;
    int return_value = 0;
    DB *db = get_DB(jnienv, jthis);

    if (!verify_non_null(jnienv, db))
        return -1;
    err = db->fd(db, &return_value);
    verify_return(jnienv, err);
    return return_value;
}

JNIEXPORT void JNICALL Java_com_sleepycat_db_Db_get
  (JNIEnv *jnienv, /*Db*/ jobject jthis, /*DbTxn*/ jobject txnid,
   /*Dbt*/ jobject key, /*Dbt*/ jobject data, jint flags)
{
    int err;
    DB *db = get_DB(jnienv, jthis);
    DB_TXN *dbtxnid = get_DB_TXN(jnienv, txnid);
    LockedDBT dbkey(jnienv, key);
    LockedDBT dbdata(jnienv, data);

    if (!verify_non_null(jnienv, db))
        return;
    err = db->get(db, dbtxnid, dbkey.dbt, dbdata.dbt, flags);
    verify_return(jnienv, err);
}

JNIEXPORT jint JNICALL Java_com_sleepycat_db_Db_put
  (JNIEnv *jnienv, /*Db*/ jobject jthis, /*DbTxn*/ jobject txnid,
   /*Dbt*/ jobject key, /*Dbt*/ jobject data, jint flags)
{
    int err;
    DB *db = get_DB(jnienv, jthis);
    DB_TXN *dbtxnid = get_DB_TXN(jnienv, txnid);
    LockedDBT dbkey(jnienv, key);
    LockedDBT dbdata(jnienv, data);

    if (!verify_non_null(jnienv, db))
        return -1;
    err = db->put(db, dbtxnid, dbkey.dbt, dbdata.dbt, flags);
    if (err != 0 && err != DB_KEYEXIST) {
        verify_return(jnienv, err);
    }
    return err;
}

JNIEXPORT jobject JNICALL Java_com_sleepycat_db_Db_stat
  (JNIEnv *jnienv, jobject jthis, jint flags)
{
    int err;
    DB *db = get_DB(jnienv, jthis);
    DB_BTREE_STAT *statp = 0;

    if (!verify_non_null(jnienv, db))
        return 0;

    err = db->stat(db, &statp, 0, flags);
    verify_return(jnienv, err);
    return get_DbBtreeStat(jnienv, statp);
}

JNIEXPORT void JNICALL Java_com_sleepycat_db_Db_sync
  (JNIEnv *jnienv, /*Db*/ jobject jthis, jint flags)
{
    int err;
    DB *db = get_DB(jnienv, jthis);

    if (!verify_non_null(jnienv, db))
        return;
    err = db->sync(db, flags);
    verify_return(jnienv, err);
}

JNIEXPORT jint JNICALL Java_com_sleepycat_db_Db_get_1type
  (JNIEnv *jnienv, /*Db*/ jobject jthis)
{
    DB *db = get_DB(jnienv, jthis);
    if (!verify_non_null(jnienv, db))
        return -1;

    return (jint)db->type;
}

JNIEXPORT jobject JNICALL Java_com_sleepycat_db_Db_open
  (JNIEnv *jnienv, /*static Db*/ jclass /*jthis_class*/,
   jstring fname, jint type, jint flags, jint mode,
   /*DbEnv*/ jobject dbenv, /*DbInfo*/ jobject info)
{
    int err;
    jobject retval = NULL;
    DB *db;
    DB_ENV *db_dbenv = get_DB_ENV(jnienv, dbenv);
    DB_INFO *dbinfo = get_DB_INFO(jnienv, info);
    LockedString dbfname(jnienv, fname);

    if (verify_non_null(jnienv, db_dbenv)) {
        err = db_open(dbfname.string, (DBTYPE)type, flags, mode,
                      db_dbenv, dbinfo, &db);
        if (verify_return(jnienv, err)) {
            retval = create_default_object(jnienv, name_DB);
            set_private_info(jnienv, name_DB, retval, db);
        }
    }
    return retval;
}

JNIEXPORT void JNICALL Java_com_sleepycat_db_Db_finalize
  (JNIEnv *jnienv, jobject jthis)
{
    DB *db = get_DB(jnienv, jthis);
    if (db) {
        // Free any info related to DB here.
        // Unfortunately, we cannot really autoclose because
        // finalize is not called in any particular order
    }
}
