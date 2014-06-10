/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sleepycat.db.internal;

import com.sleepycat.db.*;
import java.util.Comparator;

public class DbMpoolFile {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected DbMpoolFile(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(DbMpoolFile obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  /* package */ synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        throw new UnsupportedOperationException("C++ destructor does not have public access");
      }
      swigCPtr = 0;
    }
  }

  public int get_priority() throws com.sleepycat.db.DatabaseException {
    return db_javaJNI.DbMpoolFile_get_priority(swigCPtr, this);
  }

  public void set_priority(int priority) throws com.sleepycat.db.DatabaseException { db_javaJNI.DbMpoolFile_set_priority(swigCPtr, this, priority); }

  public int get_flags() throws com.sleepycat.db.DatabaseException { return db_javaJNI.DbMpoolFile_get_flags(swigCPtr, this); }

  public void set_flags(int flags, boolean onoff) throws com.sleepycat.db.DatabaseException { db_javaJNI.DbMpoolFile_set_flags(swigCPtr, this, flags, onoff); }

  public long get_maxsize() throws com.sleepycat.db.DatabaseException {
    return db_javaJNI.DbMpoolFile_get_maxsize(swigCPtr, this);
  }

  public void set_maxsize(long bytes) throws com.sleepycat.db.DatabaseException { db_javaJNI.DbMpoolFile_set_maxsize(swigCPtr, this, bytes); }

}
