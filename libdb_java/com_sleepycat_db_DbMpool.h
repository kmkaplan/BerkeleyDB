/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_sleepycat_db_DbMpool */

#ifndef _Included_com_sleepycat_db_DbMpool
#define _Included_com_sleepycat_db_DbMpool
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbMpool_close
  (JNIEnv *, jobject);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    stat
 * Signature: ()Lcom/sleepycat/db/DbMpoolStat;
 */
JNIEXPORT jobject JNICALL Java_com_sleepycat_db_DbMpool_stat
  (JNIEnv *, jobject);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    fstat
 * Signature: ()[Lcom/sleepycat/db/DbMpoolFStat;
 */
JNIEXPORT jobjectArray JNICALL Java_com_sleepycat_db_DbMpool_fstat
  (JNIEnv *, jobject);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    sync
 * Signature: (Lcom/sleepycat/db/DbLsn;)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbMpool_sync
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    trickle
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_sleepycat_db_DbMpool_trickle
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    open
 * Signature: (Ljava/lang/String;IILcom/sleepycat/db/DbEnv;)Lcom/sleepycat/db/DbMpool;
 */
JNIEXPORT jobject JNICALL Java_com_sleepycat_db_DbMpool_open
  (JNIEnv *, jclass, jstring, jint, jint, jobject);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    unlink
 * Signature: (Ljava/lang/String;ILcom/sleepycat/db/DbEnv;)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbMpool_unlink
  (JNIEnv *, jclass, jstring, jint, jobject);

/*
 * Class:     com_sleepycat_db_DbMpool
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbMpool_finalize
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
