/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_sleepycat_db_DbLock */

#ifndef _Included_com_sleepycat_db_DbLock
#define _Included_com_sleepycat_db_DbLock
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_sleepycat_db_DbLock
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLock_finalize
  (JNIEnv *, jobject);

/*
 * Class:     com_sleepycat_db_DbLock
 * Method:    put
 * Signature: (Lcom/sleepycat/db/DbLockTab;)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLock_put
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_sleepycat_db_DbLock
 * Method:    get_lock_id
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_sleepycat_db_DbLock_get_1lock_1id
  (JNIEnv *, jobject);

/*
 * Class:     com_sleepycat_db_DbLock
 * Method:    set_lock_id
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sleepycat_db_DbLock_set_1lock_1id
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
