/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_sleepycat_db_DbLog */

#ifndef _Included_com_sleepycat_db_DbLog
#define _Included_com_sleepycat_db_DbLog
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    archive
 * Signature: (I)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_sleepycat_db_DbLog_archive
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_close
  (JNIEnv *, jobject);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    compare
 * Signature: (Lcom/sleepycat/db/DbLsn;Lcom/sleepycat/db/DbLsn;)I
 */
JNIEXPORT jint JNICALL Java_com_sleepycat_db_DbLog_compare
  (JNIEnv *, jclass, jobject, jobject);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    file
 * Signature: (Lcom/sleepycat/db/DbLsn;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_sleepycat_db_DbLog_file
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    flush
 * Signature: (Lcom/sleepycat/db/DbLsn;)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_flush
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    get
 * Signature: (Lcom/sleepycat/db/DbLsn;Lcom/sleepycat/db/Dbt;I)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_get
  (JNIEnv *, jobject, jobject, jobject, jint);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    put
 * Signature: (Lcom/sleepycat/db/DbLsn;Lcom/sleepycat/db/Dbt;I)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_put
  (JNIEnv *, jobject, jobject, jobject, jint);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    db_register
 * Signature: (Lcom/sleepycat/db/Db;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_sleepycat_db_DbLog_db_1register
  (JNIEnv *, jobject, jobject, jstring, jint);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    db_unregister
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_db_1unregister
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    open
 * Signature: (Ljava/lang/String;IILcom/sleepycat/db/DbEnv;)Lcom/sleepycat/db/DbLog;
 */
JNIEXPORT jobject JNICALL Java_com_sleepycat_db_DbLog_open
  (JNIEnv *, jclass, jstring, jint, jint, jobject);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    unlink
 * Signature: (Ljava/lang/String;ILcom/sleepycat/db/DbEnv;)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_unlink
  (JNIEnv *, jclass, jstring, jint, jobject);

/*
 * Class:     com_sleepycat_db_DbLog
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLog_finalize
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
